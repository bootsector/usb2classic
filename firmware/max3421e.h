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
#ifndef MAX3421E_H_
#define MAX3421E_H_

#include <stdint.h>

void max3421e_init();
uint8_t max3421e_spi_last_transceive_status();
void max3421e_write_byte(uint8_t reg, uint8_t value);
void max3421e_write_buffer(uint8_t reg, uint8_t *data, int len);
uint8_t max3421e_read_byte(uint8_t reg);
void max3421e_read_buffer(uint8_t reg, uint8_t *data, int len);
void max3421e_reset();
uint8_t max3421e_int_state();

/* MAX3421E HOST mode registers and their bits */

#define REG_RCVFIFO	1

#define REG_SNDFIFO	2

#define REG_SUDFIFO	4

#define REG_RCVBC	6

#define REG_SNDBC	7

#define REG_USBIRQ	13
#define REG_USBIRQ_VBUSIRQ		(uint8_t)(1 << 6)
#define REG_USBIRQ_NOVBUSIRQ	(uint8_t)(1 << 5)
#define REG_USBIRQ_OSCOKIRQ		(uint8_t)(1 << 0)

#define REG_USBIEN	14
#define REG_USBIEN_VBUSIE	(uint8_t)(1 << 6)
#define REG_USBIEN_NOVBUSIE	(uint8_t)(1 << 5)
#define REG_USBIEN_OSCOKIE	(uint8_t)(1 << 0)

#define REG_CPUCTL	16
#define REG_CPUCTL_PULSEWID1	(uint8_t)(1 << 7)
#define REG_CPUCTL_PULSEWID0	(uint8_t)(1 << 6)
#define REG_CPUCTL_IE			(uint8_t)(1 << 0)

#define REG_USBCTL	15
#define REG_USBCTL_CHIPRES	(uint8_t)(1 << 5)
#define REG_USBCTL_PWRDOWN	(uint8_t)(1 << 4)

#define REG_PINCTL	17
#define REG_PINCTL_FDUPSPI	(uint8_t)(1 << 4)
#define REG_PINCTL_INTLEVEL	(uint8_t)(1 << 3)
#define REG_PINCTL_POSINT	(uint8_t)(1 << 2)
#define REG_PINCTL_GPXB		(uint8_t)(1 << 1)
#define REG_PINCTL_GPXA		(uint8_t)(1 << 0)

#define REG_HIRQ	25
#define REG_HIRQ_HXFRDNIRQ		(uint8_t)(1 << 7)
#define REG_HIRQ_FRAMEIRQ		(uint8_t)(1 << 6)
#define REG_HIRQ_CONDETIRQ		(uint8_t)(1 << 5)
#define REG_HIRQ_SUSDNIRQ		(uint8_t)(1 << 4)
#define REG_HIRQ_SNDBAVIRQ		(uint8_t)(1 << 3)
#define REG_HIRQ_RCVDAVIRQ		(uint8_t)(1 << 2)
#define REG_HIRQ_RWUIRQ			(uint8_t)(1 << 1)
#define REG_HIRQ_BUSEVENTIRQ	(uint8_t)(1 << 0)

#define REG_HIEN	26
#define REG_HIEN_HXFRDNIE	(uint8_t)(1 << 7)
#define REG_HIEN_FRAMEIE	(uint8_t)(1 << 6)
#define REG_HIEN_CONDETIE	(uint8_t)(1 << 5)
#define REG_HIEN_SUSDNIE	(uint8_t)(1 << 4)
#define REG_HIEN_SNDBAVIE	(uint8_t)(1 << 3)
#define REG_HIEN_RCVDAVIE	(uint8_t)(1 << 2)
#define REG_HIEN_RWUIE		(uint8_t)(1 << 1)
#define REG_HIEN_BUSEVENTIE	(uint8_t)(1 << 0)

#define REG_MODE	27
#define REG_MODE_DPPULLDN	(uint8_t)(1 << 7)
#define REG_MODE_DMPULLDN	(uint8_t)(1 << 6)
#define REG_MODE_SOFKAENAB	(uint8_t)(1 << 3)
#define REG_MODE_HUBPRE		(uint8_t)(1 << 2)
#define REG_MODE_LOWSPEED	(uint8_t)(1 << 1)
#define REG_MODE_HOST		(uint8_t)(1 << 0)

#define REG_PERADDR	28

#define REG_HCTL	29
#define REG_HCTL_SNDTOG1	(uint8_t)(1 << 7)
#define REG_HCTL_SNDTOG0	(uint8_t)(1 << 6)
#define REG_HCTL_RCVTOG1	(uint8_t)(1 << 5)
#define REG_HCTL_RCVTOG0	(uint8_t)(1 << 4)
#define REG_HCTL_SIGRSM		(uint8_t)(1 << 3)
#define REG_HCTL_SAMPLEBUS	(uint8_t)(1 << 2)
#define REG_HCTL_FRMRST		(uint8_t)(1 << 1)
#define REG_HCTL_BUSRST		(uint8_t)(1 << 0)

#define REG_HXFR	30
#define REG_HXFR_HS		(uint8_t)(1 << 7)
#define REG_HXFR_ISO	(uint8_t)(1 << 6)
#define REG_HXFR_OUTNIN	(uint8_t)(1 << 5)
#define REG_HXFR_SETUP	(uint8_t)(1 << 4)
#define REG_HXFR_EP3	(uint8_t)(1 << 3)
#define REG_HXFR_EP2	(uint8_t)(1 << 2)
#define REG_HXFR_EP1	(uint8_t)(1 << 1)
#define REG_HXFR_EP0	(uint8_t)(1 << 0)

#define REG_HRSL	31
#define REG_HRSL_JSTATUS	(uint8_t)(1 << 7)
#define REG_HRSL_KSTATUS	(uint8_t)(1 << 6)
#define REG_HRSL_SNDTOGRD	(uint8_t)(1 << 5)
#define REG_HRSL_RCVTOGRD	(uint8_t)(1 << 4)
#define REG_HRSL_HRSLT3		(uint8_t)(1 << 3)
#define REG_HRSL_HRSLT2		(uint8_t)(1 << 2)
#define REG_HRSL_HRSLT1		(uint8_t)(1 << 1)
#define REG_HRSL_HRSLT0		(uint8_t)(1 << 0)

#endif /* MAX3421E_H_ */
