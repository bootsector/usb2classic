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
#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>

typedef struct
{
	// Digital Buttons
	uint8_t square;
	uint8_t cross;
	uint8_t circle;
	uint8_t triangle;
	uint8_t l1;
	uint8_t r1;
	uint8_t l2;
	uint8_t r2;
	uint8_t select;
	uint8_t start;
	uint8_t l3;
	uint8_t r3;
	uint8_t home;

	// Digital Pad
	uint8_t d_up;
	uint8_t d_down;
	uint8_t d_left;
	uint8_t d_right;

	// left and right analog sticks, 0x00 left/up, 0x80 middle, 0xff right/down
	uint8_t l_x_axis;
	uint8_t l_y_axis;
	uint8_t r_x_axis;
	uint8_t r_y_axis;

	// Reserved generic purpose registers
	uint8_t reserved1;
	uint8_t reserved2;
	uint8_t reserved3;
	uint8_t reserved4;
	uint8_t reserved5;
	uint8_t reserved6;
	uint8_t reserved7;

	uint8_t l_analog;
	uint8_t r_analog;

} AbstractPad_t;

void ResetBuffer(AbstractPad_t *padData);

#endif /* UTIL_H_ */