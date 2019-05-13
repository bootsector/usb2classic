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
#ifndef XBOX360USB_H_
#define XBOX360USB_H_

#include "util.h"

void xbox360usb_init(uint8_t *data_buffer);
void xbox360usb_update(AbstractPad_t *padData);

typedef struct {
	uint8_t rid;
	uint8_t rsize;
	uint8_t digital_buttons_1;
	uint8_t digital_buttons_2;
	uint8_t lt;
	uint8_t rt;
	int16_t l_x;
	int16_t l_y;
	int16_t r_x;
	int16_t r_y;
	uint8_t reserved_1[6];
} XBOX360USB_JoystickReport_Data_t;

// digital_buttons_1
#define XBOX360USB_DPAD_UP		0
#define XBOX360USB_DPAD_DOWN	1
#define XBOX360USB_DPAD_LEFT	2
#define XBOX360USB_DPAD_RIGHT	3
#define XBOX360USB_START		4
#define XBOX360USB_BACK			5
#define XBOX360USB_LEFT_STICK	6
#define XBOX360USB_RIGHT_STICK	7

// digital_buttons_2
#define XBOX360USB_LB		0
#define XBOX360USB_RB		1
#define XBOX360USB_HOME		2
#define XBOX360USB_A		4
#define XBOX360USB_B		5
#define XBOX360USB_X		6
#define XBOX360USB_Y		7

#endif /* XBOX360USB_H_ */
