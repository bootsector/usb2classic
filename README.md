# USB2Classic - USB controllers adapter for retro consoles!
(c) Bruno Freitas - 04/2019 - http://www.brunofreitas.com/  

This project is Open Source (GPLv3) and Open Hardware (CC BY-SA 4.0)  

I've bought some super-cheap STM8S 8-bit microcontroller based boards and I 
was wondering what kind of cool project I could put them to use. I've mixed
it with an USB Host Mini 2.0 module a CPLD and some discrete components and
then the USB2Classic adapter was born!

This adapter allows you to use USB controllers with old consoles, such as:

* Sega Genesis / Mega Drive (can emulate either a 3 button or 6 button pad)
* Sega Master System, Atari 2600 and others
* Panasonic 3DO
* NES
* Super NES
* Sega Saturn
* Neo Geo
* TG16/PC-Engine (through adapter board connected to the Neo Geo output)

Currently compatible USB controllers are:

* Playstation 4 USB controller
* XBOX 360 USB controller (and compatible XInput devices)
* XBOX 360 USB Wireless Receiver
* 8BitDo Wireless USB Adapter (in XInput mode)
* Some PS3/PS4/XBOX360 USB arcade stick controllers

The CPLD originally used by this adapter is an EPM7064SLC44-10N which is 5V
based. Verilog sources are inside the multi-out directory along with Quartus II
project files.

The USB Host Mini board **MUST** be modified for 5V as shown in the
doc/usbhost_mini_5v_mod.png diagram.

Firmware was compiled using SDCC version 3.8.0

Thanks to Bruno Duarte Gouveia (brNX) for all the help with Verilog!

Enjoy!
