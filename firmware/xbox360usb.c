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
#include "xbox360usb.h"

#include "usbhost.h"
#include "max3421e.h"
#include "stm8s.h"

static XBOX360USB_JoystickReport_Data_t *usb_data;
static uint8_t in_ep, out_ep;

static void xbox360usb_custom_request() {
	// Prepare a XBOX368 USB controller specific report
	USB_Control_Setup_t setup;

	// Build SETUP package
	setup.bmRequestType = 0x80; // Device-To-Host / Standard / Device
	setup.bRequest = 0x06;
	setup.wValue = _fix_endianness(0x0600);
	setup.wIndex = 0;
	setup.wLength = _fix_endianness(0x0A);

	// Start SETUP transfer
	usbhost_setup_transfer(usbhost_usb_device_address(), 0, (uint8_t *) &setup);

	// Set receive data toggle
	max3421e_write_byte(REG_HCTL, REG_HCTL_RCVTOG1);

	// IN data transfer
	if(usbhost_in_transfer(usbhost_usb_device_address(), 0, 0, 10, usbhost_usb_device_descriptor()->bMaxPacketSize0)) {
		// Start OUT-HS transfer
		usbhost_ouths_transfer(usbhost_usb_device_address(), 0);
	}
}

void xbox360usb_init(uint8_t *data_buffer) {
	uint8_t led_data[] = {0x01, 0x03, 0x02};

	usbhost_set_address(1);
	usbhost_get_device_descriptor();

	xbox360usb_custom_request();

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

	// Turn first led on
	usbhost_out_transfer(usbhost_usb_device_address(), out_ep, led_data, 3);

	usb_data = (XBOX360USB_JoystickReport_Data_t *) data_buffer;
}

void xbox360usb_update(AbstractPad_t *padData) {
	if(!usbhost_in_transfer(usbhost_usb_device_address(), in_ep, (uint8_t *) usb_data, 20, usbhost_usb_device_descriptor()->bMaxPacketSize0)) {
		return;
	}

	if(usb_data->rid != 0x00) {
		return;
	}

	padData->d_up    = bit_check(usb_data->digital_buttons_1, XBOX360USB_DPAD_UP);
	padData->d_down  = bit_check(usb_data->digital_buttons_1, XBOX360USB_DPAD_DOWN);
	padData->d_left  = bit_check(usb_data->digital_buttons_1, XBOX360USB_DPAD_LEFT);
	padData->d_right = bit_check(usb_data->digital_buttons_1, XBOX360USB_DPAD_RIGHT);

	padData->start  = bit_check(usb_data->digital_buttons_1, XBOX360USB_START);
	padData->select = bit_check(usb_data->digital_buttons_1, XBOX360USB_BACK);
	padData->l3     = bit_check(usb_data->digital_buttons_1, XBOX360USB_LEFT_STICK);
	padData->r3     = bit_check(usb_data->digital_buttons_1, XBOX360USB_RIGHT_STICK);

	padData->cross    = bit_check(usb_data->digital_buttons_2, XBOX360USB_A);
	padData->circle   = bit_check(usb_data->digital_buttons_2, XBOX360USB_B);
	padData->square   = bit_check(usb_data->digital_buttons_2, XBOX360USB_X);
	padData->triangle = bit_check(usb_data->digital_buttons_2, XBOX360USB_Y);

	padData->l1 = bit_check(usb_data->digital_buttons_2, XBOX360USB_LB);
	padData->r1 = bit_check(usb_data->digital_buttons_2, XBOX360USB_RB);

	padData->l_x_axis = ((int16_t)_fix_endianness(usb_data->l_x) + 32768) / 257;
	padData->l_y_axis = ((int16_t)_fix_endianness(usb_data->l_y) - 32768) / -257;
	padData->r_x_axis = ((int16_t)_fix_endianness(usb_data->r_x) + 32768) / 257;
	padData->r_y_axis = ((int16_t)_fix_endianness(usb_data->r_y) - 32768) / -257;

	padData->home = bit_check(usb_data->digital_buttons_2, XBOX360USB_HOME) || (padData->start && padData->select);

	padData->l2 = 0;
	padData->r2 = 0;

	padData->l_analog = usb_data->lt;
	padData->r_analog = usb_data->rt;
}
