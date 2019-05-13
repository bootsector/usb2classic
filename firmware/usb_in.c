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
#include "usb_in.h"
#include "usb_in_detect.h"

#include "usbhost.h"
#include "util.h"

#include "xbox360usb.h"
#include "xbox360rcv.h"
#include "ps4usb.h"
#include "ps3virtuastick.h"

#include "delay.h"
#include "stm8s.h"

static uint8_t connected_device = USB_IN_UNKNOWN;
static uint8_t usb_data[64];

static void usb_in_detect_and_init() {

	uint16_t vid, pid;

	usbhost_bus_reset();

	// Get descriptors
	usbhost_get_device_descriptor();

	vid = _fix_endianness(usbhost_usb_device_descriptor()->idVendor);
	pid = _fix_endianness(usbhost_usb_device_descriptor()->idProduct);

	if(usb_in_detect_is_xbox360usb(vid, pid)) {
		xbox360usb_init(usb_data);
		connected_device = USB_IN_XBOX360USB;
	} else if(usb_in_detect_is_xbox360rcv(vid, pid)) {
		xbox360rcv_init(usb_data);
		connected_device = USB_IN_XBOX360RCV;
	} else if(usb_in_detect_is_ps4usb(vid, pid)) {
		ps4usb_init(usb_data);
		connected_device = USB_IN_PS4USB;
	} else if(usb_in_detect_is_ps3virtuastick(vid, pid)) {
		ps3virtuastick_init(usb_data);
		connected_device = USB_IN_PS3VIRTUASTICK;
	} else {
		connected_device = USB_IN_UNKNOWN;
	}

	delay_ms(100);
}

uint8_t usb_in_get_connected_device() {
	return connected_device;
}

uint8_t usb_in_init(void) {

	connected_device = USB_IN_UNKNOWN;

	delay_ms(200);

	return usbhost_init();
}

void usb_in_get_pad_state(AbstractPad_t *padData) {

	// Run USB host controller interrupt handler/tasks
	usbhost_task();

	// If USB device is disconnected, delay and return
	if(!usbhost_device_connected()) {
		connected_device = USB_IN_UNKNOWN;
		delay_ms(10);
		return;
	}

	// if unknown device, try to detect and initialize it
	if(connected_device == USB_IN_UNKNOWN) {
		ResetBuffer(padData);
		usb_in_detect_and_init();
		return;
	}

	switch(connected_device) {
		case USB_IN_XBOX360USB:
			xbox360usb_update(padData);
			break;
		case USB_IN_XBOX360RCV:
			xbox360rcv_update(padData);
			break;
		case USB_IN_PS4USB:
			ps4usb_update(padData);
			break;
		case USB_IN_PS3VIRTUASTICK:
			ps3virtuastick_update(padData);
			break;
		default:
			break;
	}

	delay_ms(3);
}
