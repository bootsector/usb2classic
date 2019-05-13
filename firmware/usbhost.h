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
#ifndef USBHOST_H_
#define USBHOST_H_

#include <stdint.h>

// Comment out if your interfacing uC is little-endian
#define HIGH_ENDIAN_PLATFORM

typedef struct {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint16_t bcdUSB;
	uint8_t  bDeviceClass;
	uint8_t  bDeviceSubClass;
	uint8_t  bDeviceProtocol;
	uint8_t  bMaxPacketSize0;
	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t bcdDevice;
	uint8_t  iManufacturer;
	uint8_t  iProduct;
	uint8_t  iSerialNumber;
	uint8_t  bNumConfigurations;
} USB_Device_Descriptor_t;

typedef struct {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bInterfaceNumber;
    uint8_t  bAlternateSetting;
    uint8_t  bNumEndpoints;
    uint8_t  bInterfaceClass;
    uint8_t  bInterfaceSubClass;
    uint8_t  bInterfaceProtocol;
    uint8_t  iInterface;
} USB_Interface_Descriptor_t;

typedef struct {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint16_t wTotalLength;
	uint8_t  bNumInterfaces;
	uint8_t  bConfigurationValue;
	uint8_t  iConfiguration;
	uint8_t  bmAttributes;
	uint8_t  MaxPower;

	USB_Interface_Descriptor_t InterfaceDescriptor;
} USB_Config_Descriptor_t;

typedef struct {
	uint8_t  bmRequestType;
	uint8_t  bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;
} USB_Control_Setup_t;

uint8_t usbhost_init();
void usbhost_task();
void usbhost_bus_reset();
uint8_t usbhost_device_connected();
uint8_t usbhost_usb_device_address();
USB_Device_Descriptor_t* usbhost_usb_device_descriptor();
USB_Config_Descriptor_t* usbhost_usb_config_descriptor();

uint8_t usbhost_request_transfer(uint8_t address, uint8_t token, uint8_t endpoint, uint16_t retries);
uint8_t usbhost_setup_transfer(uint8_t address, uint8_t endpoint, uint8_t *setup);
uint8_t usbhost_ouths_transfer(uint8_t address, uint8_t endpoint);
uint8_t usbhost_inhs_transfer(uint8_t address, uint8_t endpoint);
uint8_t usbhost_in_transfer(uint8_t address, uint8_t endpoint, uint8_t *buffer, int size, int maxpacketsize);
uint8_t usbhost_out_transfer(uint8_t address, uint8_t endpoint, uint8_t *buffer, int size);

uint8_t usbhost_get_device_descriptor();
uint8_t usbhost_get_config_descriptor();
uint8_t usbhost_get_string_descriptor(uint8_t string_id);
uint8_t usbhost_set_configuration(uint16_t configuration);
uint8_t usbhost_set_address(uint16_t address);

uint8_t usbhost_get_in_endpoint();
uint8_t usbhost_get_out_endpoint();

#ifdef HIGH_ENDIAN_PLATFORM
#define _fix_endianness(data) (uint16_t)((((uint16_t)data & (uint16_t)0x00FF) << 8) | (((uint16_t)data & (uint16_t)0xFF00) >> 8))
#else
#define _fix_endianness(data) data
#endif

#endif /* USBHOST_H_ */
