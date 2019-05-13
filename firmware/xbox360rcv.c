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
#include "xbox360rcv.h"

#include "usbhost.h"
#include "bitwise.h"

static uint8_t *usb_data;
static uint8_t in_ep, out_ep;
static uint8_t led_data[] = {0x00, 0x00, 0x08, 0x46};

void xbox360rcv_init(uint8_t *data_buffer) {

	usbhost_set_address(1);

	usbhost_get_device_descriptor();

	usbhost_get_config_descriptor();

	in_ep = usbhost_get_in_endpoint();
	out_ep = usbhost_get_out_endpoint();

	if(!in_ep || !out_ep) {
		in_ep = 1;
		out_ep = 1;
	}

	usbhost_get_string_descriptor(0);
	usbhost_get_string_descriptor(2);
	usbhost_get_string_descriptor(1);
	usbhost_get_string_descriptor(3);

	usbhost_set_configuration(1);

	usbhost_get_string_descriptor(4);

	usb_data = data_buffer;
}

void xbox360rcv_update(AbstractPad_t *padData) {
	if(!usbhost_in_transfer(usbhost_usb_device_address(), in_ep, usb_data, 29, 32)) {
		return;
	}

	if(usb_data[1] != 0x01) {
		// Turn first LED on
		usbhost_out_transfer(usbhost_usb_device_address(), out_ep, led_data, 4);
		return;
	}

	padData->d_up    = bit_check(usb_data[6], 0);
	padData->d_down  = bit_check(usb_data[6], 1);
	padData->d_left  = bit_check(usb_data[6], 2);
	padData->d_right = bit_check(usb_data[6], 3);

	padData->start  = bit_check(usb_data[6], 4);
	padData->select = bit_check(usb_data[6], 5);
	padData->l3     = bit_check(usb_data[6], 6);
	padData->r3     = bit_check(usb_data[6], 7);

	padData->cross    = bit_check(usb_data[7], 4);
	padData->circle   = bit_check(usb_data[7], 5);
	padData->square   = bit_check(usb_data[7], 6);
	padData->triangle = bit_check(usb_data[7], 7);

	padData->l1 = bit_check(usb_data[7], 0);
	padData->r1 = bit_check(usb_data[7], 1);

	padData->l_x_axis = (((int16_t)(((uint16_t)usb_data[11] << 8) | usb_data[10])) + 32768) / 257;
	padData->l_y_axis = (((int16_t)(((uint16_t)usb_data[13] << 8) | usb_data[12])) - 32768) / -257;
	padData->r_x_axis = (((int16_t)(((uint16_t)usb_data[15] << 8) | usb_data[14])) + 32768) / 257;
	padData->r_y_axis = (((int16_t)(((uint16_t)usb_data[17] << 8) | usb_data[16])) - 32768) / -257;

	padData->home = bit_check(usb_data[7], 2);

	padData->l_analog = usb_data[8];
	padData->r_analog = usb_data[9];
}
