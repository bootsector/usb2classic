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
#ifndef STM8S_H_
#define STM8S_H_

#include <stdint.h>

#include "bitwise.h"

#define enableInterrupts()    {__asm__("rim\n");}  /* enable interrupts */
#define disableInterrupts()   {__asm__("sim\n");}  /* disable interrupts */

#define _SFR8_(mem_addr)    (*(volatile uint8_t *)(0x5000 + (mem_addr)))
#define _SFR16_(mem_addr)   (*(volatile uint16_t *)(0x5000 + (mem_addr)))

/* PORT A */
#define PA_ODR      _SFR8_(0x0000)
#define PA_IDR      _SFR8_(0x0001)
#define PA_DDR      _SFR8_(0x0002)
#define PA_CR1      _SFR8_(0x0003)
#define PA_CR2      _SFR8_(0x0004)

/* PORT B */
#define PB_ODR      _SFR8_(0x0005)
#define PB_IDR      _SFR8_(0x0006)
#define PB_DDR      _SFR8_(0x0007)
#define PB_CR1      _SFR8_(0x0008)
#define PB_CR2      _SFR8_(0x0009)

/* PORT C */
#define PC_ODR      _SFR8_(0x000A)
#define PC_IDR      _SFR8_(0x000B)
#define PC_DDR      _SFR8_(0x000C)
#define PC_CR1      _SFR8_(0x000D)
#define PC_CR2      _SFR8_(0x000E)

/* PORT D */
#define PD_ODR      _SFR8_(0x000F)
#define PD_IDR      _SFR8_(0x0010)
#define PD_DDR      _SFR8_(0x0011)
#define PD_CR1      _SFR8_(0x0012)
#define PD_CR2      _SFR8_(0x0013)

/* PORT E */
#define PE_ODR      _SFR8_(0x0014)
#define PE_IDR      _SFR8_(0x0015)
#define PE_DDR      _SFR8_(0x0015)
#define PE_CR1      _SFR8_(0x0017)
#define PE_CR2      _SFR8_(0x0018)

/* PORT F */
#define PF_ODR      _SFR8_(0x0019)
#define PF_IDR      _SFR8_(0x001A)
#define PF_DDR      _SFR8_(0x001B)
#define PF_CR1      _SFR8_(0x001C)
#define PF_CR2      _SFR8_(0x001D)

/* CLK */
#define CLK_ICKR    _SFR8_(0x00C0)
#define CLK_ECKR    _SFR8_(0x00C1)
#define CLK_SWR     _SFR8_(0x00C4)
#define CLK_SWCR    _SFR8_(0x00C5)
#define CLK_CKDIVR  _SFR8_(0x00C6)

/* TIMER 2 */
#define TIM2_CR1    _SFR8_(0x0300)
#define TIM2_EGR    _SFR8_(0x0306)
#define TIM2_CNTR   _SFR16_(0x030C) // (CNTRH << 8) | CNTRL
#define TIM2_PSCR   _SFR8_(0x030E)

#endif /* STM8S_H_ */
