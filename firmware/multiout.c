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
static uint8_t map_config_id = 0;

static void multiout_spi_transfer(void) {
	bit_clear(PD_ODR, 1 << 5); // Latch LOW
	delay_us(10);
	bit_clear(PD_ODR, 1 << 4); // Clock LOW

	for (uint8_t i = 0; i < 16; i++)  {
		delay_us(10);

		bit_write(pad_data & (1 << (15 - i)), PD_ODR, 1 << 6);

		bit_set(PD_ODR, 1 << 4); // Clock HIGH
		delay_us(10);
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
	map_config_id = 0;
	
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

static void multiout_cycle_map_config(void) {
	pad_data = 0xFFFF;
	map_config_id = (map_config_id + 1) % 4;
	delay_ms(250);
}

void multiout_update(AbstractPad_t *padData) {

	// Check for map config cycle combo (SELECT + HOME)
	if (padData->select && padData->home) {
		multiout_cycle_map_config();
	}

	// Map analogs to D-Pad
	if (padData->l_x_axis > 192) {
		padData->d_right = 1;
	} else if (padData->l_x_axis < 64) {
		padData->d_left = 1;
	}

	if (padData->l_y_axis > 192) {
		padData->d_down = 1;
	} else if (padData->l_y_axis < 64) {
		padData->d_up = 1;
	}

	// Analog triggers map to L2/R2
	if (!padData->l2) {
		padData->l2 = (padData->l_analog >= 0x80);
	}

	if (!padData->r2) {
		padData->r2 = (padData->r_analog >= 0x80);
	}

	switch (map_config_id) {
		case 0x01:
			pad_data = (!padData->r2 << MULTIOUT_PAD_RES_2) | 
			(!padData->l2 << MULTIOUT_PAD_RES_1) |
			(!padData->r1 << MULTIOUT_PAD_A) |
			(!padData->l1 << MULTIOUT_PAD_R) |
			(!padData->triangle << MULTIOUT_PAD_X) |
			(!padData->circle << MULTIOUT_PAD_B) |
			(!padData->d_right << MULTIOUT_PAD_RIGHT) |
			(!padData->d_left << MULTIOUT_PAD_LEFT) |
			(!padData->d_down << MULTIOUT_PAD_DOWN) |
			(!padData->d_up << MULTIOUT_PAD_UP) |
			(!padData->start << MULTIOUT_PAD_START) |
			(!padData->select << MULTIOUT_PAD_SELECT) |
			(!padData->square << MULTIOUT_PAD_L) |
			(!padData->cross << MULTIOUT_PAD_Y);
			
			break;
		case 0x02:
			pad_data = (!padData->r2 << MULTIOUT_PAD_A) | 
			(!padData->l2 << MULTIOUT_PAD_RES_2) |
			(!padData->r1 << MULTIOUT_PAD_R) |
			(!padData->l1 << MULTIOUT_PAD_RES_1) |
			(!padData->triangle << MULTIOUT_PAD_X) |
			(!padData->circle << MULTIOUT_PAD_B) |
			(!padData->d_right << MULTIOUT_PAD_RIGHT) |
			(!padData->d_left << MULTIOUT_PAD_LEFT) |
			(!padData->d_down << MULTIOUT_PAD_DOWN) |
			(!padData->d_up << MULTIOUT_PAD_UP) |
			(!padData->start << MULTIOUT_PAD_START) |
			(!padData->select << MULTIOUT_PAD_SELECT) |
			(!padData->square << MULTIOUT_PAD_L) |
			(!padData->cross << MULTIOUT_PAD_Y);

			break;
		case 0x03:
			pad_data = (!padData->r2 << MULTIOUT_PAD_X) | 
			(!padData->l2 << MULTIOUT_PAD_Y) |
			(!padData->r1 << MULTIOUT_PAD_R) |
			(!padData->l1 << MULTIOUT_PAD_L) |
			(!padData->triangle << MULTIOUT_PAD_START) |
			(!padData->circle << MULTIOUT_PAD_A) |
			(!padData->d_right << MULTIOUT_PAD_RIGHT) |
			(!padData->d_left << MULTIOUT_PAD_LEFT) |
			(!padData->d_down << MULTIOUT_PAD_DOWN) |
			(!padData->d_up << MULTIOUT_PAD_UP) |
			(!padData->select << MULTIOUT_PAD_SELECT) |
			(!padData->cross << MULTIOUT_PAD_B);

			break;
		default:
			pad_data = (!padData->r2 << MULTIOUT_PAD_RES_2) | 
			(!padData->l2 << MULTIOUT_PAD_RES_1) |
			(!padData->r1 << MULTIOUT_PAD_R) |
			(!padData->l1 << MULTIOUT_PAD_L) |
			(!padData->triangle << MULTIOUT_PAD_X) |
			(!padData->circle << MULTIOUT_PAD_A) |
			(!padData->d_right << MULTIOUT_PAD_RIGHT) |
			(!padData->d_left << MULTIOUT_PAD_LEFT) |
			(!padData->d_down << MULTIOUT_PAD_DOWN) |
			(!padData->d_up << MULTIOUT_PAD_UP) |
			(!padData->start << MULTIOUT_PAD_START) |
			(!padData->select << MULTIOUT_PAD_SELECT) |
			(!padData->square << MULTIOUT_PAD_Y) |
			(!padData->cross << MULTIOUT_PAD_B);
			
			break;
	}

	multiout_spi_transfer();
}