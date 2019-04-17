#****************************************************************************
#*
#*  (C) 2019 Andrii Bilynskyi <andriy.bilynskyy@gmail.com>
#*
#*  This code is licenced under the MIT.
#*
#****************************************************************************


TOOLCHAIN_PATH := /opt/gcc-arm-none-eabi-8-2018-q4-major


CC     := ${TOOLCHAIN_PATH}/bin/arm-none-eabi-gcc
CXX    := ${TOOLCHAIN_PATH}/bin/arm-none-eabi-g++
OBJCPY := ${TOOLCHAIN_PATH}/bin/arm-none-eabi-objcopy
FLASH  := openocd


PROJECT ?= stm32-test


STM32_PERIPH_CSRC := \
  ${wildcard stm32-periph/*.c} \

STM32_PERIPH_CXXSRC := \
  ${wildcard stm32-periph/*.cpp} \

STM32_FREERTOS_CSRC := \
  ${wildcard free-rtos/src/*.c} \

STM32_FREERTOS_CXXSRC := \
  ${wildcard freertos/src/*.cpp} \

SEGGER_RTT_CSRC := \
  ${wildcard segger-rtt/*.c} \

SEGGER_RTT_CXXSRC := \
  ${wildcard segger-rtt/*.cpp} \

STM32_USER_CSRC := \
  ${wildcard src/*.c} \

STM32_USER_CXXSRC := \
  ${wildcard src/*.cpp} \

INCLUDE_PATH := \
  src \
  stm32-periph \
  free-rtos/inc \
  segger-rtt \

LDLIBS := \

LD_SCRIPT := linker/stm32_flash.ld


OBJECTS := \
  ${STM32_PERIPH_CSRC:.c=.o} \
  ${STM32_PERIPH_CXXSRC:.cpp=.o} \
  ${STM32_FREERTOS_CSRC:.c=.o} \
  ${STM32_FREERTOS_CXXSRC:.cpp=.o} \
  ${SEGGER_RTT_CSRC:.c=.o} \
  ${SEGGER_RTT_CXXSRC:.cpp=.o} \
  ${STM32_USER_CSRC:.c=.o} \
  ${STM32_USER_CXXSRC:.cpp=.o}

CFLAGS  := -g -O3 
CFLAGS  += -Wall -std=c99
CFLAGS  += -mlittle-endian -mthumb -mcpu=cortex-m3
CFLAGS  += -DSTM32F10X_MD
CFLAGS  += ${addprefix -I, ${INCLUDE_PATH}}

CXXFLAGS  := -g -O3 
CXXFLAGS  += -Wall
CXXFLAGS  += -mlittle-endian -mthumb -mcpu=cortex-m3
CXXFLAGS  += -DSTM32F10X_MD
CXXFLAGS  += ${addprefix -I, ${INCLUDE_PATH}}

LDFLAGS := -T${LD_SCRIPT}
LDFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m3
LDFLAGS += -Wl,--gc-sections
LDFLAGS += ${addprefix -l, ${LDLIBS}}


FLASHCONFIGS := \
  openocd/jlink.cfg \
  openocd/stm32f1x.cfg \

FLASHCOMMANDS := \
  -c "init" \
  -c "reset init" \
  -c "flash write_image erase $(PROJECT).hex" \
  -c "reset" \
  -c "shutdown" \

FLASHOPTIONS := ${addprefix -f , ${FLASHCONFIGS}} ${FLASHCOMMANDS}


.PHONY: all clean flash help

all: ${PROJECT}.hex

${PROJECT}.hex: ${PROJECT}.elf
	${OBJCPY} -O ihex ${PROJECT}.elf ${PROJECT}.hex

${PROJECT}.elf: ${OBJECTS}
	${CC} ${OBJECTS} -o ${PROJECT}.elf ${LDFLAGS}
	
%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.cpp
	${CC} ${CXXFLAGS} -c $< -o $@

clean:
	rm -f ${OBJECTS} ${PROJECT}.elf ${PROJECT}.hex

flash: ${PROJECT}.hex
	${FLASH} ${FLASHOPTIONS}

help:
	@echo "---------------------------------------------------------------------"
	@echo "all   - create binary files (.elf, .hex)"
	@echo "clean - remove generated files"	
	@echo "flash - flash target"
	@echo "---------------------------------------------------------------------"