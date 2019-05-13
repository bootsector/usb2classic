# USB2Classic - USB controllers adapter for retro consoles!
# Copyright (c) 2019 Bruno Freitas - bruno@brunofreitas.com

CC = sdcc
ASM = sdasstm8
OBJCOPY = sdobjcopy
RM = rm -f

DEVICE = stm8s103f3

TARGET = USB2CLASSIC

SRC_MAIN = main.c
SRC_OTHERS = delay.c util.c max3421e.c usbhost.c usb_in.c usb_in_detect.c \
ps4usb.c xbox360rcv.c xbox360usb.c ps3virtuastick.c multiout.c 
SRC_ASM =

OBJ_OTHERS = $(SRC_OTHERS:%.c=%.rel)
OBJ_ASM = $(SRC_ASM:%.s=%.rel)

INCLUDE = -I .
PARAMS = -lstm8 -mstm8 --opt-code-size --std-sdcc11

all: sdccversion build

sdccversion:
	@$(CC) -v

%.rel : %.c
	${CC} -c ${INCLUDE} ${PARAMS} $< $(OBJ_ASM)

%.rel : %.s
	${ASM} -l -o $<

$(TARGET).ihx: $(OBJ_ASM) $(OBJ_OTHERS) 
	${CC} ${SRC_MAIN} ${INCLUDE} ${PARAMS} ${OBJ_ASM} ${OBJ_OTHERS} --out-fmt-ihx -o ${TARGET}.ihx

$(TARGET).bin: $(TARGET).ihx
	${OBJCOPY} -I ihex ${TARGET}.ihx -O binary ${TARGET}.bin

build: $(TARGET).ihx

flash: $(TARGET).ihx
	stm8flash -c stlinkv2 -p ${DEVICE} -w $(TARGET).ihx

flash_unlock:
	stm8flash -c stlinkv2 -p ${DEVICE} -u

clean:
	${RM} *.asm *.cdb *.ihx *.lk *.lst *.map *.noi *.rel *.sym *.rst *.bin
