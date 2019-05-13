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
#ifndef USB_IN_H_
#define USB_IN_H_

#include <stdint.h>

#include "util.h"

#define USB_IN_UNKNOWN        0
#define USB_IN_XBOX360USB     1
#define USB_IN_KEYBOARD       2
#define USB_IN_PS3USB         3
#define USB_IN_XBOX360RCV     4
#define USB_IN_PS4USB         5
#define USB_IN_PS3VIRTUASTICK 6
#define USB_IN_XBOXCLASSIC    7
#define USB_IN_XBOXONEUSB     8
#define USB_IN_SWITCHPROUSB   9
#define USB_IN_TWINUSB        10
#define USB_IN_WIIUGCADAPTER  11
#define USB_IN_PSCLASSIC      12
#define USB_IN_GENESISADAPTER 13

uint8_t usb_in_init(void);
void usb_in_get_pad_state(AbstractPad_t *padData);
uint8_t usb_in_get_connected_device();

#endif /* USB_IN_H_ */
