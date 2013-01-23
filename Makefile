##########################################################################
#                    STM32F4 Project Template Makefile                   #
#   based on work of Jonathon Thomson http://jethomson.wordpress.com     #
#                      modified by mail@florianzahn.de                   #
##########################################################################

TARGET:=main
TOOLCHAIN_PATH:=~/Development/sat/bin
TOOLCHAIN_PREFIX:=arm-none-eabi
OPTLVL:=3 # Optimization level, can be [0, 1, 2, 3, s].

PROJECT_NAME:=$(notdir $(lastword $(CURDIR)))

#TOP is Path to firmware-folder from STM
TOP:=/Users/florianzahn/Development/stm32f4xx-fw


DISCOVERY:=$(TOP)/Utilities/STM32F4-Discovery
STMLIB:=$(TOP)/Libraries
STD_PERIPH:=$(STMLIB)/STM32F4xx_StdPeriph_Driver
STARTUP:=$(STMLIB)/CMSIS/ST/STM32F4xx/Source/Templates/gcc_ride7
LINKER_SCRIPT=stm32_flash.ld


INCLUDE=-I$(CURDIR)/inc
INCLUDE+=-I$(STMLIB)/CMSIS/Include
INCLUDE+=-I$(STMLIB)/CMSIS/ST/STM32F4xx/Include
INCLUDE+=-I$(STD_PERIPH)/inc
INCLUDE+=-I$(DISCOVERY)

# vpath is used so object files are written to the current directory instead
# of the same directory as their source files
vpath %.c $(DISCOVERY) $(STD_PERIPH)/src
vpath %.s $(STARTUP)

ASRC=startup_stm32f4xx.s

# Project Source Files
SRC=main.c
SRC+=ws2812.c
SRC+=delay.c
SRC+=system_stm32f4xx.c


# Discovery Source Files
SRC+=stm32f4_discovery.c

# Standard Peripheral Source Files
SRC+=misc.c
SRC+=stm32f4xx_gpio.c
SRC+=stm32f4xx_rcc.c
SRC+=stm32f4xx_tim.c
SRC+=stm32f4xx_dma.c


CDEFS=-DSTM32F4XX
CDEFS+=-DUSE_STDPERIPH_DRIVER

MCUFLAGS=-mcpu=cortex-m4 -mthumb -std=c99
#MCUFLAGS=-mcpu=cortex-m4 -mthumb -mlittle-endian -mfpu=fpa -mfloat-abi=hard -mthumb-interwork
#MCUFLAGS=-mcpu=cortex-m4 -mfpu=vfpv4-sp-d16 -mfloat-abi=hard
COMMONFLAGS=-O$(OPTLVL) -g -Wall
CFLAGS=$(COMMONFLAGS) $(MCUFLAGS) $(INCLUDE) $(CDEFS)

LDLIBS=
LDFLAGS=$(COMMONFLAGS) -fno-exceptions -ffunction-sections -fdata-sections \
        -nostartfiles -Wl,--gc-sections,-T$(LINKER_SCRIPT)

#####
#####

OBJ = $(SRC:%.c=%.o) $(ASRC:%.s=%.o)

CC=$(TOOLCHAIN_PATH)/$(TOOLCHAIN_PREFIX)-gcc
LD=$(TOOLCHAIN_PATH)/$(TOOLCHAIN_PREFIX)-gcc
OBJCOPY=$(TOOLCHAIN_PATH)/$(TOOLCHAIN_PREFIX)-objcopy
AS=$(TOOLCHAIN_PATH)/$(TOOLCHAIN_PREFIX)-as
AR=$(TOOLCHAIN_PATH)/$(TOOLCHAIN_PREFIX)-ar
GDB=$(TOOLCHAIN_PATH)/$(TOOLCHAIN_PREFIX)-gdb


bin: $(OBJ)
	$(CC) -o $(TARGET).elf $(LDFLAGS) $(OBJ)	$(LDLIBS)
	$(OBJCOPY) -O ihex   $(TARGET).elf $(TARGET).hex
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin


flash: bin
	st-flash write $(TARGET).bin 0x08000000

.PHONY: clean

clean:
	rm -f $(OBJ)
	rm -f $(TARGET).elf
	rm -f $(TARGET).hex
	rm -f $(TARGET).bin
