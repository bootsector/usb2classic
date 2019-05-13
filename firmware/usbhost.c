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
#include "usbhost.h"
#include "max3421e.h"
#include "stm8s.h"

#include "delay.h"

#define MAX_CONFIG_DESCRIPTOR_LEN 256

static volatile uint8_t bus_status, last_bus_status;
static volatile uint8_t usb_device_address = 0;
static USB_Device_Descriptor_t usb_device_descriptor;
static USB_Config_Descriptor_t config;
static uint8_t in_endpoint, out_endpoint;

static uint8_t config_descriptor_buffer[MAX_CONFIG_DESCRIPTOR_LEN];

uint8_t usbhost_request_transfer(uint8_t address, uint8_t token, uint8_t endpoint, uint16_t retries) {
	uint16_t timeout;
	uint8_t hrsl;

	max3421e_write_byte(REG_PERADDR, address);

	while(--retries) {
		max3421e_write_byte(REG_HXFR, token | endpoint);

		timeout = 10000;

		while(!(max3421e_read_byte(REG_HIRQ) & REG_HIRQ_HXFRDNIRQ) && --timeout);

		if(timeout == 0) {
			return 0;
		}

		// Clear transfer completion interrupt
		max3421e_write_byte(REG_HIRQ, REG_HIRQ_HXFRDNIRQ);

		// Check host transfer result.
		hrsl = max3421e_read_byte(REG_HRSL) & 0x0F;

		if(hrsl == 0) { // SUCCESS
			return 1;
		} else if(hrsl == 0x04 || hrsl == 0x0E) { // NACK or TIMEOUT
			continue;
		} else { // Other non-SUCCESS statuses
			return 0;
		}
	}

	return 0;
}

uint8_t usbhost_setup_transfer(uint8_t address, uint8_t endpoint, uint8_t *setup) {
	// Write setup data to setup FIFO
	max3421e_write_buffer(REG_SUDFIFO, setup, 8);

	// Request SETUP transfer
	return usbhost_request_transfer(address, REG_HXFR_SETUP, endpoint, 200);
}

uint8_t usbhost_ouths_transfer(uint8_t address, uint8_t endpoint) {
	// Request OUT-HS transfer
	return usbhost_request_transfer(address, REG_HXFR_OUTNIN | REG_HXFR_HS, endpoint, 200);
}

uint8_t usbhost_inhs_transfer(uint8_t address, uint8_t endpoint) {
	// Request IN-HS transfer
	return usbhost_request_transfer(address, REG_HXFR_HS, endpoint, 200);
}

uint8_t usbhost_in_transfer(uint8_t address, uint8_t endpoint, uint8_t *buffer, int size, int maxpacketsize) {
	uint8_t bytes_read;
	int total_bytes_read = 0;
	uint16_t timeout;

	do {
		// Start IN transfer
		if(!usbhost_request_transfer(address, 0x00, endpoint, 200)) {
			return 0;
		}

		// Wait for IN transfer completion
		timeout = 10000;

		while(!(max3421e_read_byte(REG_HIRQ) & REG_HIRQ_RCVDAVIRQ) && --timeout);

		if(timeout == 0) {
			return 0;
		}

		// Copy read bytes into buffer if provided
		bytes_read = max3421e_read_byte(REG_RCVBC);

		if(buffer) {
			max3421e_read_buffer(REG_RCVFIFO, (uint8_t *) buffer, bytes_read);
			buffer += bytes_read;
		}

		total_bytes_read += bytes_read;

		// Clear receive interrupt flag
		max3421e_write_byte(REG_HIRQ, REG_HIRQ_RCVDAVIRQ);
	} while((total_bytes_read < size) && !(bytes_read < maxpacketsize));

	return 1;
}

uint8_t usbhost_out_transfer(uint8_t address, uint8_t endpoint, uint8_t *buffer, int size) {
	uint16_t timeout;

	// Write OUT data to SND FIFO
	max3421e_write_buffer(REG_SNDFIFO, buffer, size);
	max3421e_write_byte(REG_SNDBC, size);

	// Start OUT transfer
	if(!usbhost_request_transfer(address, REG_HXFR_OUTNIN, endpoint, 200)) {
		return 0;
	}

	// Wait for OUT transfer completion
	timeout = 10000;

	while(!(max3421e_read_byte(REG_HIRQ) & REG_HIRQ_SNDBAVIRQ) && --timeout);

	if(timeout == 0) {
		return 0;
	}

	return 1;
}

uint8_t usbhost_get_device_descriptor() {
	USB_Control_Setup_t setup;

	// Build SETUP package
	setup.bmRequestType = 0x80; // Device-To-Host / Standard / Device
	setup.bRequest = 0x06; // Get descriptor
	setup.wValue = _fix_endianness(0x0100); // Device descriptor
	setup.wIndex = 0;
	setup.wLength = _fix_endianness(0x12);

	// Start SETUP transfer
	if(!usbhost_setup_transfer(usb_device_address, 0, (uint8_t *) &setup)) {
		return 0;
	}

	// Set receive data toggle
	max3421e_write_byte(REG_HCTL, REG_HCTL_RCVTOG1);

	// IN data transfer
	if(!usbhost_in_transfer(usb_device_address, 0, (uint8_t *) &usb_device_descriptor, 18, 8)) {
		return 0;
	}

	// Start OUT-HS transfer
	if(!usbhost_ouths_transfer(usb_device_address, 0)) {
		return 0;
	}

	return 1;
}

static void usbhost_parse_config_descriptor(uint8_t *buffer, uint8_t *in_ep, uint8_t *out_ep) {
	int idx = 0;

	uint8_t type, size, iface;
	uint16_t total_size;
	uint8_t found_in, found_out;

	found_in = found_out = 0;

	iface = 0;

	total_size = _fix_endianness(*(buffer + 2));

	if(total_size > MAX_CONFIG_DESCRIPTOR_LEN) {
		total_size = MAX_CONFIG_DESCRIPTOR_LEN;
	}

	while((idx < total_size) && !(found_in && found_out)) {
		size = buffer[idx];
		type = buffer[idx + 1];

		// If invalid size (zero), abort!
		if(!size) {
			break;
		}

		// If INTERFACE set its number as current
		if(type == 0x04) {
			iface = buffer[idx + 2];
		}

		// If ENDPOINT and INTERRUPT and INTERFACE number is 0x00
		if((type == 0x05) && (buffer[idx + 3] == 0x03) && (iface == 0x00)) {
			if(buffer[idx + 2] & 0x80) { // is IN endpoint?
				if(!found_in) {
					found_in = buffer[idx + 2] & 0x7F;
				}
			} else {
				if(!found_out) {
					found_out = buffer[idx + 2] & 0x7F;
				}
			}
		}

		idx += size;
	}

	*in_ep = found_in;
	*out_ep = found_out;
}

uint8_t usbhost_get_config_descriptor() {
	USB_Control_Setup_t setup;

	in_endpoint = out_endpoint = 0;

	// Build SETUP data
	setup.bmRequestType = 0x80; // Device-To-Host / Standard / Device
	setup.bRequest = 0x06; // Get descriptor
	setup.wValue = _fix_endianness(0x0200); // Config descriptor
	setup.wIndex = 0;

	setup.wLength = _fix_endianness(sizeof(USB_Config_Descriptor_t));

	// Start SETUP transfer (First call - Get descriptor size)
	if(!usbhost_setup_transfer(usb_device_address, 0, (uint8_t *) &setup)) {
		return 0;
	}

	// Set receive data toggle
	max3421e_write_byte(REG_HCTL, REG_HCTL_RCVTOG1);

	// IN data transfer
	if(!usbhost_in_transfer(usb_device_address, 0, (uint8_t *) &config, sizeof(USB_Config_Descriptor_t), usb_device_descriptor.bMaxPacketSize0)) {
		return 0;
	}

	// Start OUT-HS transfer
	if(!usbhost_ouths_transfer(usb_device_address, 0)) {
		return 0;
	}

	setup.wLength = config.wTotalLength;

	// Start SETUP transfer (Second call - Get full descriptor)
	if(!usbhost_setup_transfer(usb_device_address, 0, (uint8_t *) &setup)) {
		return 0;
	}

	// Set receive data toggle
	max3421e_write_byte(REG_HCTL, REG_HCTL_RCVTOG1);

	// IN data transfer - Fills config_descriptor_buffer
	if(!usbhost_in_transfer(usb_device_address, 0, config_descriptor_buffer, _fix_endianness(config.wTotalLength) > MAX_CONFIG_DESCRIPTOR_LEN ? MAX_CONFIG_DESCRIPTOR_LEN : _fix_endianness(config.wTotalLength), usb_device_descriptor.bMaxPacketSize0)) {
		return 0;
	}

	// Start OUT-HS transfer
	if(!usbhost_ouths_transfer(usb_device_address, 0)) {
		return 0;
	}

	usbhost_parse_config_descriptor(config_descriptor_buffer, &in_endpoint, &out_endpoint);

	return 1;
}

uint8_t usbhost_get_string_descriptor(uint8_t string_id) {
	USB_Control_Setup_t setup;

	// Build SETUP data
	setup.bmRequestType = 0x80; // Device-To-Host / Standard / Device
	setup.bRequest = 0x06; // Get descriptor
	setup.wValue = _fix_endianness(0x0300 | string_id);

	if(string_id) {
		setup.wIndex = _fix_endianness(0x0409);
	} else {
		setup.wIndex = 0;
	}

	setup.wLength = _fix_endianness(0xFF);

	// Start SETUP transfer
	if(!usbhost_setup_transfer(usb_device_address, 0, (uint8_t *) &setup)) {
		return 0;
	}

	// Set receive data toggle
	max3421e_write_byte(REG_HCTL, REG_HCTL_RCVTOG1);

	// IN data transfer - (Dummy read - no buffer provided)
	if(!usbhost_in_transfer(usb_device_address, 0, 0, 0xFF, usb_device_descriptor.bMaxPacketSize0)) {
		return 0;
	}

	// Start OUT-HS transfer
	if(!usbhost_ouths_transfer(usb_device_address, 0)) {
		return 0;
	}

	return 1;
}

uint8_t usbhost_set_configuration(uint16_t configuration) {
	USB_Control_Setup_t setup;

	// Build SETUP data
	setup.bmRequestType = 0x00; // Host-To-Device / Standard / Device
	setup.bRequest = 0x09; // Set Configuration
	setup.wValue = _fix_endianness(configuration);
	setup.wIndex = 0;
	setup.wLength = 0;

	// Start SETUP transfer
	if(!usbhost_setup_transfer(usb_device_address, 0, (uint8_t *) &setup)) {
		return 0;
	}

	// Start IN-HS transfer
	if(!usbhost_inhs_transfer(usb_device_address, 0)) {
		return 0;
	}

	return 1;
}

uint8_t usbhost_set_address(uint16_t address) {
	USB_Control_Setup_t setup;

	// Build SETUP package
	setup.bmRequestType = 0x00; // Host-To-Device / Standard / Device
	setup.bRequest = 0x05; // Set Address
	setup.wValue = _fix_endianness(address);
	setup.wIndex = 0;
	setup.wLength = 0;

	// Start SETUP transfer
	if(!usbhost_setup_transfer(0, 0, (uint8_t *) &setup)) {
		return 0;
	}

	// Start IN-HS transfer
	if(!usbhost_inhs_transfer(0, 0)) {
		return 0;
	}

	usb_device_address = address;

	return 1;
}

static int usbhost_reset() {
	uint16_t timeout = 1024;

	max3421e_reset();

	while(!(max3421e_read_byte(REG_USBIRQ) & REG_USBIRQ_OSCOKIRQ) && --timeout);

	return timeout;
}

static uint8_t usbhost_sample_bus() {
	uint16_t timeout = 1024;

	max3421e_write_byte(REG_MODE, REG_MODE_DMPULLDN | REG_MODE_DPPULLDN | REG_MODE_HOST);

	max3421e_write_byte(REG_HCTL, REG_HCTL_SAMPLEBUS);

	while((max3421e_read_byte(REG_HCTL) & REG_HCTL_SAMPLEBUS) && --timeout);

	return max3421e_read_byte(REG_HRSL) & (REG_HRSL_JSTATUS | REG_HRSL_KSTATUS);
}

void usbhost_bus_reset() {
	uint16_t timeout;

	// Turn off SOF generation
	max3421e_write_byte(REG_MODE, REG_MODE_DMPULLDN | REG_MODE_DPPULLDN | REG_MODE_HOST);

	// Reset SOF frame counter
	timeout = 1024;

	max3421e_write_byte(REG_HCTL, REG_HCTL_FRMRST);

	while((max3421e_read_byte(REG_HCTL) & REG_HCTL_FRMRST) && --timeout);

	// Reset SOF IRQ signal
	max3421e_write_byte(REG_HIRQ, REG_HIRQ_FRAMEIRQ);

	// Start and wait for BUS reset
	timeout = 10240;

	max3421e_write_byte(REG_HCTL, REG_HCTL_BUSRST);

	while((max3421e_read_byte(REG_HCTL) & REG_HCTL_BUSRST) && --timeout);

	// Setup HOST mode based on connected device type
	switch(bus_status) {
		case REG_HRSL_JSTATUS: // High Speed
			max3421e_write_byte(REG_MODE, REG_MODE_DMPULLDN | REG_MODE_DPPULLDN | REG_MODE_HOST | REG_MODE_SOFKAENAB);
			break;
		case REG_HRSL_KSTATUS: // Low Speed
			max3421e_write_byte(REG_MODE, REG_MODE_DMPULLDN | REG_MODE_DPPULLDN | REG_MODE_HOST | REG_MODE_LOWSPEED | REG_MODE_SOFKAENAB);
			break;
		default:
			max3421e_write_byte(REG_MODE, REG_MODE_DMPULLDN | REG_MODE_DPPULLDN | REG_MODE_HOST);
			break;
	}

	// If High Speed Device, then wait for first SOF package
	if(bus_status == REG_HRSL_JSTATUS) {
		timeout = 10240;

		while(!(max3421e_read_byte(REG_HIRQ) & REG_HIRQ_FRAMEIRQ) && --timeout);
	}

	// Enable connection detection interrupts
	max3421e_write_byte(REG_HIEN, REG_HIEN_CONDETIE);

	// Clear interrupt flags
	max3421e_write_byte(REG_HIRQ, REG_HIRQ_CONDETIRQ);

	// Enable IRQ line
	max3421e_write_byte(REG_CPUCTL, REG_CPUCTL_IE);


	delay_ms(32);
}

static void usbhost_irq_handler() {
	// If INT is HIGH, there are no pending interrupts to handle
	if(max3421e_int_state()) {
		return;
	}

	uint8_t irqs = max3421e_read_byte(REG_HIRQ);

	// Connection/Disconnection event handler
	if(irqs & REG_HIRQ_CONDETIRQ) {

		// Settling time before sampling bus
		delay_ms(200);

		bus_status = usbhost_sample_bus();

		// If bus state has changed from last sampling
		if(last_bus_status != bus_status) {
			usbhost_bus_reset();

			usb_device_address = 0;

			usb_device_descriptor.bMaxPacketSize0 = 8;
			usb_device_descriptor.idProduct = 0;
			usb_device_descriptor.idVendor = 0;

			last_bus_status = bus_status;
		}

		// Clear interrupt flag
		max3421e_write_byte(REG_HIRQ, REG_HIRQ_CONDETIRQ);
	}
}

uint8_t usbhost_init() {

	usb_device_address = 0;

	usb_device_descriptor.bMaxPacketSize0 = 8;
	usb_device_descriptor.idProduct = 0;
	usb_device_descriptor.idVendor = 0;

	// Setup MAX3421E SPI, /INT and /RES pins
	max3421e_init();

	// Full Duplex SPI - INT pin asserted LOW for selected pending IRQs
	max3421e_write_byte(REG_PINCTL, REG_PINCTL_FDUPSPI | REG_PINCTL_INTLEVEL);

	// Reset USB hardware and wait for OSCOKIRQ
	if(!usbhost_reset()) {
		return 0;
	}

	last_bus_status = bus_status = usbhost_sample_bus();

	usbhost_bus_reset();

	return 1;
}

uint8_t usbhost_device_connected() {
	return (bus_status != 0);
}

uint8_t usbhost_usb_device_address() {
	return usb_device_address;
}

USB_Device_Descriptor_t* usbhost_usb_device_descriptor() {
	return &usb_device_descriptor;
}

USB_Config_Descriptor_t* usbhost_usb_config_descriptor() {
	return &config;
}

void usbhost_task() {
	usbhost_irq_handler();
}

uint8_t usbhost_get_in_endpoint() {
	return in_endpoint;
}

uint8_t usbhost_get_out_endpoint() {
	return out_endpoint;
}

