# Name: Makefile
# Author: Kenya Ukai
# License: MIT


DEVICE     := atmega168p
CLOCK      := 1000000
PROGRAMMER := -c usbasp -P usb
OBJECTS    := main.o 
FUSES      := -U hfuse:w:0xdf:m -U lfuse:w:0x62:m

#Fuse Bits
# Low :
# xxxxxxxx
# 7--------CKDIV8   : クロック速度を1/8に
#  6-------CKOUT    : クロック出力
#   5------SUT1   
#    4-----SUT0     : スタートアップ待機時間
#     3----CKSEL3
#      2---CKSEL2
#       1--CKSEL1
#        0-CKSEL0   : クロック
#
# High :
# xxxxxxxx
# 7--------RSTDISBL : リセットピン無効化
#  6-------DWEN     : DebugWire有効化
#   5------SPIEN    : シリアルプログラミング有効化
#    4-----WDTON    : ウォッチドッグタイマ有効化
#     3----EESAVE   : EEPROM保護有効化
#      2---BODLEVEL2
#       1--BODLEVEL1
#        0-BODLEVEL0: リセット電圧
#
# Extended :
#      xxx
#      3---BOOTSZ1 
#       2--BOOTSZ0 : ブート領域サイズ
#        1-BOOTRST : リセットベクタ
#
# Default Value low  0x62 = 0b10011101
#               high 0xDF = 0b00100000
#               ext  0xF9 = 0b110
#
# My Value      l:h:ex = 0xE2 : 0xDF : 0xF9 (CKDIV8を変えているだけ)


AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all:	main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -f main.hex main.elf $(OBJECTS)

# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf

c:
	$(COMPILE) -E main.c
