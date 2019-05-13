/*
 * USB2Classic - USB controllers adapter for retro consoles!
 * Copyright (c) 2019 Bruno Freitas - bruno@brunofreitas.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <string.h>

#include "ps4usb.h"
#include "usbhost.h"

#include "bitwise.h"

static uint8_t *usb_data;
static uint8_t ps4_config_bytes[32];
static uint8_t in_ep, out_ep;

static void ps4usb_set_led() {
	memset(ps4_config_bytes, 0, sizeof(ps4_config_bytes));

	ps4_config_bytes[0] = 0x05; // Report ID
	ps4_config_bytes[1]= 0xFF;

	ps4_config_bytes[6] = 0x00; // Red
	ps4_config_bytes[7] = 0x00; // Green
	ps4_config_bytes[8] = 0xC0; // Blue

	usbhost_out_transfer(usbhost_usb_device_address(), out_ep, ps4_config_bytes, 32);
}

void ps4usb_init(uint8_t *data_buffer) {
	usbhost_set_address(1);

	usbhost_get_device_descriptor();

	usbhost_get_config_descriptor();

	in_ep = usbhost_get_in_endpoint();
	out_ep = usbhost_get_out_endpoint();

	if(!in_ep || !out_ep) {
		in_ep = 4;
		out_ep = 3;
	}

	usbhost_get_string_descriptor(0);
	usbhost_get_string_descriptor(2);
	usbhost_get_string_descriptor(1);

	usbhost_set_configuration(1);

	ps4usb_set_led();

	usb_data = data_buffer;
}

void ps4usb_update(AbstractPad_t *padData) {
	if(!usbhost_in_transfer(usbhost_usb_device_address(), in_ep, usb_data, 64, 64)) {
		return;
	}

	// Exit if Report ID != 0x01
	if(usb_data[0] != 0x01) {
		return;
	}

	padData->d_up = padData->d_down = padData->d_left = padData->d_right = 0;

	switch(usb_data[5] & 0x0F) {
		case 0x00:
			padData->d_up = 1;
			break;
		case 0x01:
			padData->d_up = 1;
			padData->d_right = 1;
			break;
		case 0x02:
			padData->d_right = 1;
			break;
		case 0x03:
			padData->d_right = 1;
			padData->d_down = 1;
			break;
		case 0x04:
			padData->d_down = 1;
			break;
		case 0x05:
			padData->d_down = 1;
			padData->d_left = 1;
			break;
		case 0x06:
			padData->d_left = 1;
			break;
		case 0x07:
			padData->d_left = 1;
			padData->d_up = 1;
			break;
		default:
			break;
	}

	padData->start = bit_check(usb_data[6], 5);
	padData->r3 = bit_check(usb_data[6], 7);
	padData->l3 = bit_check(usb_data[6], 6);
	padData->select = bit_check(usb_data[6], 4);

	padData->square = bit_check(usb_data[5], 4);
	padData->cross = bit_check(usb_data[5], 5);
	padData->circle = bit_check(usb_data[5], 6);
	padData->triangle = bit_check(usb_data[5], 7);
	padData->r1 = bit_check(usb_data[6], 1);
	padData->l1 = bit_check(usb_data[6], 0);
	padData->r2 = bit_check(usb_data[6], 3);
	padData->l2 = bit_check(usb_data[6], 2);

	padData->l_analog = usb_data[8];
	padData->r_analog = usb_data[9];

	padData->home = bit_check(usb_data[7], 0);

	padData->l_x_axis = usb_data[1];
	padData->l_y_axis = usb_data[2];
	padData->r_x_axis = usb_data[3];
	padData->r_y_axis = usb_data[4];
}
