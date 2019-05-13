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
#include "usb_in.h"
#include "multiout.h"

static AbstractPad_t pad_data;

static void Set16MhzInternalClock() {
	disableInterrupts();

	CLK_ICKR = 0;                   //  Reset the Internal Clock Register.
	CLK_ICKR = 1 << 0;              //  Enable the HSI.
	
	while (!(CLK_ICKR & (1 << 1))); //  Wait for HSIRDY.
	
	CLK_CKDIVR = 0;                 //  Ensure the clocks are running at full speed.
	CLK_SWR = 0xE1;                 //  Use HSI as the clock source.
	CLK_SWCR = 0;                   //  Reset the clock switch control register.
	CLK_SWCR = 1 << 1;              //  Enable switching.
	
	while (CLK_SWCR & (1 << 0));    //  Pause while the clock switch is busy.

	enableInterrupts();
}

void main() {
	/* Set internal clock to 16Mhz */
	Set16MhzInternalClock();

	/* Reset pad data buffer */
	ResetBuffer(&pad_data);

	/* Initialize USB input and serial output drivers */
	usb_in_init();
	multiout_init();

	/* Main polling loop */
	for(;;) {
		usb_in_get_pad_state(&pad_data);
		multiout_update(&pad_data);
	}
}
