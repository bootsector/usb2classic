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

#include "max3421e.h"

#include "delay.h"

static volatile uint8_t spi_transceive_status;

static void max3421e_spi_transceive_begin() {
	bit_clear(PC_ODR, 1 << 5); // CLK LOW
	bit_clear(PA_ODR, 1 << 3); // SS LOW

	__asm__("nop\nnop\n");
}

static void max3421e_spi_transceive_end() {
	__asm__("nop\nnop\n");

	bit_clear(PC_ODR, 1 << 5); // CLK LOW
	bit_set(PA_ODR, 1 << 3); // SS HIGH
}

static uint8_t max3421e_spi_transceive_byte(uint8_t data) {
	uint8_t readval = 0;

	__asm__("nop\nnop\n");

	// The MAX3421E changes its output data (MISO) on the falling edge of SCLK
	// and samples input data (MOSI) on the rising edge of SCLK.
	for(int8_t bit = 7; bit >= 0; bit--) {

		// MOSI
		if(data & (1 << bit)) {
			bit_set(PC_ODR, 1 << 6);
		} else {
			bit_clear(PC_ODR, 1 << 6);
		}

		bit_set(PC_ODR, 1 << 5); // CLK HIGH

		// MISO
		if(bit_check(PC_IDR, 7)) {
			readval |= (1 << bit);
		}

		bit_clear(PC_ODR, 1 << 5); // CLK LOW
	}

	__asm__("nop\nnop\n");

	return readval;
}

uint8_t max3421e_spi_last_transceive_status() {
	return spi_transceive_status;
}

void max3421e_write_byte(uint8_t reg, uint8_t value) {
	reg = (reg << 3) | 2;

	max3421e_spi_transceive_begin();

	spi_transceive_status = max3421e_spi_transceive_byte(reg);

	max3421e_spi_transceive_byte(value);

	max3421e_spi_transceive_end();
}

void max3421e_write_buffer(uint8_t reg, uint8_t *data, int len) {
	reg = (reg << 3) | 2;

	max3421e_spi_transceive_begin();

	spi_transceive_status = max3421e_spi_transceive_byte(reg);

	for(int i = 0; i < len; i++) {
		max3421e_spi_transceive_byte(data[i]);
	}

	max3421e_spi_transceive_end();
}

uint8_t max3421e_read_byte(uint8_t reg) {
	uint8_t readval;

	reg = (reg << 3);

	max3421e_spi_transceive_begin();

	spi_transceive_status = max3421e_spi_transceive_byte(reg);

	readval = max3421e_spi_transceive_byte(0x00);

	max3421e_spi_transceive_end();

	return readval;
}

void max3421e_read_buffer(uint8_t reg, uint8_t *data, int len) {
	reg = (reg << 3);

	max3421e_spi_transceive_begin();

	spi_transceive_status = max3421e_spi_transceive_byte(reg);

	for(int i = 0; i < len; i++) {
		data[i] = max3421e_spi_transceive_byte(0x00);
	}

	max3421e_spi_transceive_end();
}

void max3421e_reset() {
	bit_clear(PB_ODR, 1 << 4); // RES LOW
	
	delay_us(1);

	bit_set(PB_ODR, 1 << 4);; // RES HIGH
}

uint8_t max3421e_int_state() {
	return bit_check(PC_IDR, 3);
}

void max3421e_init() {
	/* Init SPI pins */

	// PC5 - CLK
	// PC6 - MOSI
	// PC7 - MISO
	// PA3 - SS
	// PC3 - INT
	// PB4 - RES

	// MISO (Input Pull Up)
	bit_clear(PC_DDR, 1 << 7);
	bit_set(PC_CR1, 1 << 7);

	// MOSI (push pull output, fast mode)
	bit_set(PC_DDR, 1 << 6);
	bit_set(PC_CR1, 1 << 6);
	bit_set(PC_CR2, 1 << 6);

	// SS (push pull output, fast mode)
	bit_set(PA_DDR, 1 << 3);
	bit_set(PA_CR1, 1 << 3);
	bit_set(PA_CR2, 1 << 3);

	// CLK (push pull output, fast mode)
	bit_set(PC_DDR, 1 << 5);
	bit_set(PC_CR1, 1 << 5);
	bit_set(PC_CR2, 1 << 5);

	bit_clear(PC_ODR, 1 << 5); // CLK
	bit_clear(PC_ODR, 1 << 6); // MOSI
	bit_set(PA_ODR, 1 << 3);   // SS

	/* Init INT pin */

	// INT (Input Floating)
	bit_clear(PC_DDR, 1 << 3);
	bit_clear(PC_CR1, 1 << 3);

	/* Init RES pin */

	// RES - output open drain
	bit_set(PB_DDR, 1 << 4);
	bit_clear(PB_CR1, 1 << 4);
	bit_clear(PB_CR2, 1 << 4);

	bit_set(PB_ODR, 1 << 4);   // RES is HIGH (open drain)

	spi_transceive_status = 0;
}
