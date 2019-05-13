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
#include "delay.h"

/* Functions below assume a 16Mhz Clock */

void delay_us(uint16_t us) {
	// Prescaler = 2^4: Increments at every 1uS
	TIM2_PSCR = 0x04;

	// Initialize counter
	bit_set(TIM2_EGR, 1 << 0);

	// Enable counter
	bit_set(TIM2_CR1, 1 << 0);

	// Wait until counter reaches requested us
	while(TIM2_CNTR < us);
}

void delay_ms(uint16_t ms) {
	// 125 times in 1ms
	TIM2_PSCR = 0x07;

	// Initialize counter
	bit_set(TIM2_EGR, 1 << 0);

	// Enable counter
	bit_set(TIM2_CR1, 1 << 0);

	while(ms > 0) {
		while(TIM2_CNTR < 125);

		TIM2_CNTR = 0;

		ms--;
	}
}
