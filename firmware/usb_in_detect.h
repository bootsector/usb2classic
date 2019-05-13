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
#ifndef USB_IN_DETECT_H_
#define USB_IN_DETECT_H_

#include <stdint.h>

typedef struct
{
	uint16_t VID;
	uint16_t PID;
} USB_Device_ID_t;

uint8_t usb_in_detect_is_xbox360usb(uint16_t vid, uint16_t pid);
uint8_t usb_in_detect_is_xbox360rcv(uint16_t vid, uint16_t pid);
uint8_t usb_in_detect_is_ps4usb(uint16_t vid, uint16_t pid);
uint8_t usb_in_detect_is_ps3virtuastick(uint16_t vid, uint16_t pid);

#endif /* USB_IN_DETECT_H_ */
