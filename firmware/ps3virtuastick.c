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
#include "ps3virtuastick.h"
#include "usbhost.h"

static PS3VirtuaStick_JoystickReport_Data_t *usb_data;
static uint8_t in_ep;

void ps3virtuastick_init(uint8_t *data_buffer) {
	usbhost_set_address(1);

	usbhost_get_device_descriptor();

	usbhost_get_config_descriptor();

	in_ep = usbhost_get_in_endpoint();

	if(!in_ep) {
		in_ep = 1;
	}

	usbhost_get_string_descriptor(0);
	usbhost_get_string_descriptor(2);
	usbhost_get_string_descriptor(1);

	usbhost_set_configuration(1);

	usb_data = (PS3VirtuaStick_JoystickReport_Data_t *) data_buffer;
}

void ps3virtuastick_update(AbstractPad_t *padData) {
	if(!usbhost_in_transfer(usbhost_usb_device_address(), in_ep, (uint8_t *) usb_data, sizeof(PS3VirtuaStick_JoystickReport_Data_t), 64)) {
		return;
	}

	padData->d_up = padData->d_down = padData->d_left = padData->d_right = 0;

	switch(usb_data->direction & 0x0F) {
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

	padData->start = usb_data->start_btn;
	padData->r3 = usb_data->r3_btn;
	padData->l3 = usb_data->l3_btn;
	padData->select = usb_data->select_btn;

	padData->square = usb_data->square_btn;
	padData->cross = usb_data->cross_btn;
	padData->circle = usb_data->circle_btn;
	padData->triangle = usb_data->triangle_btn;
	padData->r1 = usb_data->r1_btn;
	padData->l1 = usb_data->l1_btn;
	padData->r2 = usb_data->r2_btn;
	padData->l2 = usb_data->l2_btn;

	padData->home = usb_data->ps_btn || (padData->start && padData->select);

	padData->l_x_axis = usb_data->l_x_axis;
	padData->l_y_axis = usb_data->l_y_axis;
	padData->r_x_axis = usb_data->r_x_axis;
	padData->r_y_axis = usb_data->r_y_axis;
}
