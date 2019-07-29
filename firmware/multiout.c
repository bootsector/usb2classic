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
#include "stm8s.h"
#include "multiout.h"
#include "delay.h"
#include "bitwise.h"

static uint16_t pad_data = 0xFFFF;

static void multiout_spi_transfer(void) {
	bit_clear(PD_ODR, 1 << 5); // Latch LOW
	bit_clear(PD_ODR, 1 << 4); // Clock LOW

	for (uint8_t i = 0; i < 16; i++)  {

		bit_write(pad_data & (1 << (15 - i)), PD_ODR, 1 << 6);

		delay_us(2);

		bit_set(PD_ODR, 1 << 4); // Clock HIGH
		delay_us(2);
		bit_clear(PD_ODR, 1 << 4); // Clock LOW
	}

	bit_set(PD_ODR, 1 << 5); // Latch HIGH
}

void multiout_init(void) {
	// Clock - PD4
	// Latch - PD5
	// Data  - PD6
	// OE    - PC4

	pad_data = 0xFFFF;
	
	// Clock (push pull output, fast mode)
	bit_set(PD_DDR, 1 << 4);
	bit_set(PD_CR1, 1 << 4);
	bit_set(PD_CR2, 1 << 4);

	bit_set(PD_ODR, 1 << 4); // Clock HIGH

	// Latch (push pull output, fast mode)
	bit_set(PD_DDR, 1 << 5);
	bit_set(PD_CR1, 1 << 5);
	bit_set(PD_CR2, 1 << 5);

	bit_set(PD_ODR, 1 << 5); // Latch HIGH

	// Data (push pull output, fast mode)
	bit_set(PD_DDR, 1 << 6);
	bit_set(PD_CR1, 1 << 6);
	bit_set(PD_CR2, 1 << 6);

	// Init output data
	multiout_spi_transfer();

	// OE (push pull output, fast mode)
	bit_set(PC_DDR, 1 << 4);
	bit_set(PC_CR1, 1 << 4);
	bit_set(PC_CR2, 1 << 4);

	// Enable Output
	bit_clear(PC_ODR, 1 << 4);
}

void multiout_update(AbstractPad_t *padData) {

	// Map analogs to D-Pad
	if(padData->l_x_axis > 192) {
		padData->d_right = 1;
	} else if(padData->l_x_axis < 64) {
		padData->d_left = 1;
	}

	if(padData->l_y_axis > 192) {
		padData->d_down = 1;
	} else if(padData->l_y_axis < 64) {
		padData->d_up = 1;
	}

	// Analog triggers map to L2/R2
	if(padData->l_analog >= 0x80) {
		padData->l2 = 1;
	}

	if(padData->r_analog >= 0x80) {
		padData->r2 = 1;
	}

	bit_write(!padData->d_left, pad_data, MULTIOUT_PAD_LEFT);
	bit_write(!padData->d_right, pad_data, MULTIOUT_PAD_RIGHT);
	bit_write(!padData->d_up, pad_data, MULTIOUT_PAD_UP);
	bit_write(!padData->d_down, pad_data, MULTIOUT_PAD_DOWN);
	bit_write(!padData->square, pad_data, MULTIOUT_PAD_Y);
	bit_write(!padData->cross, pad_data, MULTIOUT_PAD_B);
	bit_write(!padData->triangle, pad_data, MULTIOUT_PAD_X);
	bit_write(!padData->circle, pad_data, MULTIOUT_PAD_A);
	bit_write(!padData->select, pad_data, MULTIOUT_PAD_SELECT);
	bit_write(!padData->start, pad_data, MULTIOUT_PAD_START);
	bit_write(!padData->l1, pad_data, MULTIOUT_PAD_L);
	bit_write(!padData->r1, pad_data, MULTIOUT_PAD_R);
	bit_write(!padData->l2, pad_data, MULTIOUT_PAD_RES_1);
	bit_write(!padData->r2, pad_data, MULTIOUT_PAD_RES_2);

	multiout_spi_transfer();
}