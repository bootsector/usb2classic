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
#ifndef MULTIOUT_H_
#define MULTIOUT_H_

#include "util.h"

// Multi-Out board uses SNES pad layout
#define MULTIOUT_PAD_B		0
#define MULTIOUT_PAD_Y		1
#define MULTIOUT_PAD_SELECT	2
#define MULTIOUT_PAD_START	3
#define MULTIOUT_PAD_UP		4
#define MULTIOUT_PAD_DOWN	5
#define MULTIOUT_PAD_LEFT	6
#define MULTIOUT_PAD_RIGHT	7
#define MULTIOUT_PAD_A		8
#define MULTIOUT_PAD_X		9
#define MULTIOUT_PAD_L		10
#define MULTIOUT_PAD_R		11
#define MULTIOUT_PAD_RES_1	12
#define MULTIOUT_PAD_RES_2	13

void multiout_init(void);
void multiout_update(AbstractPad_t *padData);

#endif /* MULTIOUT_H_ */