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
AS     := ${TOOLCHAIN_PATH}/bin/arm-none-eabi-gcc
OBJCPY := ${TOOLCHAIN_PATH}/bin/arm-none-eabi-objcopy
FLASH  := openocd


PROJECT ?= stm32-test


STM32_PERIPH_CSRC := \
  ${wildcard stm32-periph/*.c} \

STM32_PERIPH_ASSRC := \
  ${wildcard stm32-periph/*.s} \

STM32_PERIPH_CXXSRC := \
  ${wildcard stm32-periph/*.cpp} \

STM32_SPL_CSRC := \
  ${wildcard stm32-periph/spl/src/*.c} \

STM32_SPL_CXXSRC := \
  ${wildcard stm32-periph/spl/src/*.cpp} \

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
  config \
  stm32-periph \
  stm32-periph/spl/inc \
  free-rtos/inc \

LDLIBS := \

LD_SCRIPT := linker/stm32_flash.ld

OBJECTS := \
  ${STM32_PERIPH_CSRC:.c=.o} \
  ${STM32_PERIPH_ASSRC:.s=.o} \
  ${STM32_PERIPH_CXXSRC:.cpp=.o} \
  ${STM32_SPL_CSRC:.c=.o} \
  ${STM32_SPL_CXXSRC:.cpp=.o} \
  ${STM32_FREERTOS_CSRC:.c=.o} \
  ${STM32_FREERTOS_CXXSRC:.cpp=.o} \
  ${STM32_USER_CSRC:.c=.o} \
  ${STM32_USER_CXXSRC:.cpp=.o}

CFLAGS  := -Wall -std=c99
CFLAGS  += -mlittle-endian -mthumb -mcpu=cortex-m3
CFLAGS  += -DSTM32F10X_MD -DHSE_VALUE=8000000u -DUSE_STDPERIPH_DRIVER

ASFLAGS  := -Wall -std=c99
ASFLAGS  += -mlittle-endian -mthumb -mcpu=cortex-m3
ASFLAGS  += -DSTM32F10X_MD -DHSE_VALUE=8000000u -DUSE_STDPERIPH_DRIVER

CXXFLAGS  := -Wall
CXXFLAGS  += -mlittle-endian -mthumb -mcpu=cortex-m3
CXXFLAGS  += -DSTM32F10X_MD -DHSE_VALUE=8000000u -DUSE_STDPERIPH_DRIVER

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

ifndef BUILD
  BUILD := DEBUG
endif

ifeq ($(BUILD),DEBUG)
  DEBUG_OBJ := \
    ${SEGGER_RTT_CSRC:.c=.o} \
    ${SEGGER_RTT_CXXSRC:.cpp=.o} \

  OBJECTS += $(DEBUG_OBJ)
  INCLUDE_PATH += segger-rtt
  CFLAGS  += -g3 -O0 -DDEBUG
  ASFLAGS += -g3 -O0 -DDEBUG
  CXXFLAGS += -g3 -O0 -DDEBUG
else ifeq ($(BUILD),RELEASE)
  DEBUG_OBJ := \

  CFLAGS  += -g0 -O3
  ASFLAGS += -g0 -O3
  CXXFLAGS += -g0 -O3
else
  $(error Wrong BUILD '$(BUILD)'! Should be: DEBUG or RELEASE)
endif

CFLAGS  += ${addprefix -I, ${INCLUDE_PATH}}
CXXFLAGS  += ${addprefix -I, ${INCLUDE_PATH}}

.PHONY: all clean flash help

all: ${PROJECT}.elf

${PROJECT}.elf: ${OBJECTS}
	$(info $(BUILD) BUILD)
	${CC} ${OBJECTS} -o ${PROJECT}.elf ${LDFLAGS}
	${OBJCPY} -O ihex ${PROJECT}.elf ${PROJECT}.hex
	
%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.s
	${CC} ${ASFLAGS} -c $< -o $@

%.o: %.cpp
	${CC} ${CXXFLAGS} -c $< -o $@

clean:
	rm -f ${OBJECTS} ${PROJECT}.elf ${PROJECT}.hex ${DEBUG_OBJ}

flash:
	@if [ -f "${PROJECT}.hex" ] ; then ${FLASH} ${FLASHOPTIONS}; else echo "Output .hex file doesn't exist. Run 'make all' before!"; fi

help:
	$(info Help)
	$(info ---------------------------------------------------------------------)
	$(info make Help                     - this help text)
	$(info make BUILD=<build_type> all   - create binary files (.elf, .hex))
	$(info make clean                    - remove generated files)
	$(info make flash                    - flash target)
	$(info ---------------------------------------------------------------------)
	$(info Available build_type [DEBUG RELEASE], eg: make BUILD=RELEASE ...)
	$(info Default build_type DEBUG)
