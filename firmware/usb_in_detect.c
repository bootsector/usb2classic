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
#include "usb_in_detect.h"

#define XBOX360USB_DEVICES_SIZE 116

static const USB_Device_ID_t XBOX360USB_DEVICES[XBOX360USB_DEVICES_SIZE] = {
	{0x0079, 0x18D4},
	{0x044F, 0xB326},
	{0x045E, 0x028E},
	{0x045E, 0x028F},
	{0x046D, 0xC21D},
	{0x046D, 0xC21E},
	{0x046D, 0xC21F},
	{0x046D, 0xC242},
	{0x046D, 0xCAA3},
	{0x056E, 0x2004},
	{0x06A3, 0xF51A},
	{0x0738, 0x4716},
	{0x0738, 0x4718},
	{0x0738, 0x4726},
	{0x0738, 0x4728},
	{0x0738, 0x4736},
	{0x0738, 0x4738},
	{0x0738, 0x4740},
	{0x0738, 0x4758},
	{0x0738, 0x9871},
	{0x0738, 0xB726},
	{0x0738, 0xB738},
	{0x0738, 0xBEEF},
	{0x0738, 0xCB02},
	{0x0738, 0xCB03},
	{0x0738, 0xCB29},
	{0x0738, 0xF738},
	{0x07FF, 0xFFFF},
	{0x0E6F, 0x0105},
	{0x0E6F, 0x0113},
	{0x0E6F, 0x011F},
	{0x0E6F, 0x0131},
	{0x0E6F, 0x0133},
	{0x0E6F, 0x0201},
	{0x0E6F, 0x0213},
	{0x0E6F, 0x021F},
	{0x0E6F, 0x0301},
	{0x0E6F, 0x0401},
	{0x0E6F, 0x0413},
	{0x0E6F, 0x0501},
	{0x0E6F, 0x0143}, // MK X Fight Pad for XBOX 360
	{0x0E6F, 0xF900},
	{0x0F0D, 0x000A},
	{0x0F0D, 0x000C},
	{0x0F0D, 0x000D},
	{0x0F0D, 0x0016},
	{0x0F0D, 0x001B},
	{0x11C9, 0x55F0},
	{0x12AB, 0x0004},
	{0x12AB, 0x0301},
	{0x12AB, 0x0303},
	{0x1430, 0x4748},
	{0x1430, 0xF801},
	{0x146B, 0x0601},
	{0x1532, 0x0037},
	{0x15E4, 0x3F00},
	{0x15E4, 0x3F0A},
	{0x15E4, 0x3F10},
	{0x162E, 0xBEEF},
	{0x1689, 0xFD00},
	{0x1689, 0xFD01},
	{0x1689, 0xFE00},
	{0x1BAD, 0x0002},
	{0x1BAD, 0x0003},
	{0x1BAD, 0x0130},
	{0x1BAD, 0xF016},
	{0x1BAD, 0xF018},
	{0x1BAD, 0xF019},
	{0x1BAD, 0xF021},
	{0x1BAD, 0xF023},
	{0x1BAD, 0xF025},
	{0x1BAD, 0xF027},
	{0x1BAD, 0xF028},
	{0x1BAD, 0xF02E},
	{0x1BAD, 0xF030},
	{0x1BAD, 0xF036},
	{0x1BAD, 0xF038},
	{0x1BAD, 0xF039},
	{0x1BAD, 0xF03A},
	{0x1BAD, 0xF03D},
	{0x1BAD, 0xF03E},
	{0x1BAD, 0xF03F},
	{0x1BAD, 0xF042},
	{0x1BAD, 0xF080},
	{0x1BAD, 0xF501},
	{0x1BAD, 0xF502},
	{0x1BAD, 0xF503},
	{0x1BAD, 0xF504},
	{0x1BAD, 0xF505},
	{0x1BAD, 0xF506},
	{0x1BAD, 0xF900},
	{0x1BAD, 0xF901},
	{0x1BAD, 0xF903},
	{0x1BAD, 0xF904},
	{0x1BAD, 0xF906},
	{0x1BAD, 0xFA01},
	{0x1BAD, 0xFD00},
	{0x1BAD, 0xFD01},
	{0x24C6, 0x5000},
	{0x24C6, 0x5300},
	{0x24C6, 0x5303},
	{0x24C6, 0x530A},
	{0x24C6, 0x531A},
	{0x24C6, 0x5397},
	{0x24C6, 0x5500},
	{0x24C6, 0x5501},
	{0x24C6, 0x5502},
	{0x24C6, 0x5503},
	{0x24C6, 0x5506},
	{0x24C6, 0x550D},
	{0x24C6, 0x550E},
	{0x24C6, 0x5B00},
	{0x24C6, 0x5B02},
	{0x24C6, 0x5B03},
	{0x24C6, 0x5D04},
	{0x24C6, 0xFAFE}
};

#define XBOX360RCV_DEVICES_SIZE 2

static const USB_Device_ID_t XBOX360RCV_DEVICES[XBOX360RCV_DEVICES_SIZE] = {
	{0x045E, 0x0291},
	{0x045E, 0x0719}
};

#define PS4USB_DEVICES_SIZE 8

static const USB_Device_ID_t PS4USB_DEVICES[PS4USB_DEVICES_SIZE] = {
	{0x054C, 0x09CC},
	{0x054C, 0x05C4},
	{0x0738, 0x8250},
	{0x0738, 0x8384}, // Madcatz FightStick TE S+ PS4
	{0x0738, 0x8481}, // Madcatz TE2+ (PS4 Mode)
	{0x0C12, 0x0EF1}, // ZEROPLUS P4 Wired GamepadV2.3
	{0x0F0D, 0x0087}, // Hori Fighting Stick mini 4 PS4 Mode
	{0x1532, 0x0401}  // Razer Panthera (PS4 Mode)
};

#define PS3VIRTUASTICK_DEVICES_SIZE 19

static const USB_Device_ID_t PS3VIRTUASTICK_DEVICES[PS3VIRTUASTICK_DEVICES_SIZE] = {
	{0x04B4, 0x010A}, //  Sega "Saturn Style" USB pad
	{0x0738, 0x3187}, // Madcatz PS3 BrawlStick
	{0x0738, 0x3250},
	{0x0738, 0x3481}, // Madcatz TE2+ (PS3 Mode)
	{0x0738, 0x8818}, // Madcatz SFIV SE Fightstick
	{0x0738, 0x8828}, // MadCatz FightPad PS3 (wireless dongle)
	{0x0738, 0x8838}, // MadCatz Round 1 TE PS3
	{0x0E6F, 0x0124}, // INJUSTICE FightStick for PS3
	{0x0F0D, 0x0011}, // Hori Real Arcade Pro 3
	{0x0F0D, 0x0022}, // PS3 HORI HRAP V3 SA (Hori Real Arcade Pro V3 SA)
	{0x0F0D, 0x002D}, // PS3 Hori Fighting Commander 3 Pro
	{0x0F0D, 0x003D}, // Hori Soul Calibur V stick “RAP N3”
	{0x0F0D, 0x0088}, // Hori Fighting Stick mini 4 PS3 Mode
	{0x0F30, 0x1012}, // Qanba Q4 RAF (PS3 Mode)
	{0x0F30, 0x1100}, // Qanba Q1 Fightstick for PC3/PC
	{0x10C4, 0x82C0},
	{0x1532, 0x0402}, // Razer Panthera (PS3 Mode)
	{0x1C1A, 0x0100}, // Paewang PS3/360 (PS3 Mode)
	{0x1F4F, 0x0008}  // NeoGeo CD USB Pad for PS3
};

static uint8_t usb_in_detect_device_lookup(USB_Device_ID_t *device_lut, int lut_size, uint16_t vid, uint16_t pid) {
	uint8_t result = 0;
	USB_Device_ID_t *d;

	for(int i = 0; i < lut_size; i++) {
		d = &device_lut[i];

		if((d->PID == pid) && (d->VID == vid)) {
			result = 1;
			break;
		}
	}

	return result;
}

uint8_t usb_in_detect_is_xbox360usb(uint16_t vid, uint16_t pid) {
	return usb_in_detect_device_lookup((USB_Device_ID_t *) XBOX360USB_DEVICES, XBOX360USB_DEVICES_SIZE, vid, pid);
}

uint8_t usb_in_detect_is_xbox360rcv(uint16_t vid, uint16_t pid) {
	return usb_in_detect_device_lookup((USB_Device_ID_t *) XBOX360RCV_DEVICES, XBOX360RCV_DEVICES_SIZE, vid, pid);
}

uint8_t usb_in_detect_is_ps4usb(uint16_t vid, uint16_t pid) {
	return usb_in_detect_device_lookup((USB_Device_ID_t *) PS4USB_DEVICES, PS4USB_DEVICES_SIZE, vid, pid);
}

uint8_t usb_in_detect_is_ps3virtuastick(uint16_t vid, uint16_t pid) {
	return usb_in_detect_device_lookup((USB_Device_ID_t *) PS3VIRTUASTICK_DEVICES, PS3VIRTUASTICK_DEVICES_SIZE, vid, pid);
}