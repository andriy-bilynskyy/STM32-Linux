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
SIZE   := ${TOOLCHAIN_PATH}/bin/arm-none-eabi-size
GDB    := ${TOOLCHAIN_PATH}/bin/arm-none-eabi-gdb


PROJECT ?= stm32-test


PROJECT_CSRC := \
  ${wildcard stm32-periph/*.c} \
  ${wildcard stm32-periph/*.s} \
  ${wildcard stm32-periph/spl/src/*.c} \
  ${wildcard free-rtos/src/*.c} \
  ${wildcard src/*.c} \
  ${wildcard src/*.cpp} \

INCLUDE_PATH := \
  src \
  config \
  stm32-periph \
  stm32-periph/spl/inc \
  free-rtos/inc \

LDLIBS := \
  stdc++ \

LD_SCRIPT := linker/stm32_flash.ld

OBJECTS := ${addsuffix .o, ${PROJECT_CSRC}}

CFLAGS  := -Wall -std=c99
CFLAGS  += -mlittle-endian -mthumb -mcpu=cortex-m3
CFLAGS  += -DSTM32F10X_MD -DHSE_VALUE=8000000u -DUSE_STDPERIPH_DRIVER

ASFLAGS  := -Wall -std=c99
ASFLAGS  += -mlittle-endian -mthumb -mcpu=cortex-m3
ASFLAGS  += -DSTM32F10X_MD -DHSE_VALUE=8000000u -DUSE_STDPERIPH_DRIVER

CXXFLAGS  := -Wall -fno-exceptions
CXXFLAGS  += -mlittle-endian -mthumb -mcpu=cortex-m3
CXXFLAGS  += -DSTM32F10X_MD -DHSE_VALUE=8000000u -DUSE_STDPERIPH_DRIVER

LDFLAGS := -T${LD_SCRIPT}
LDFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m3
LDFLAGS += -Wl,--gc-sections --specs=nosys.specs
LDFLAGS += -Xlinker -Map=${PROJECT}.map
LDFLAGS += ${addprefix -l, ${LDLIBS}}


ifndef BUILD
  BUILD := DEBUG
endif

ifeq ($(BUILD),DEBUG)
  DEBUG_PROJECT_CSRC := \
    ${wildcard segger-rtt/*.c} \
    ${wildcard src/dbg-ext/*.c} \
  
  INCLUDE_PATH += \
    segger-rtt \
    src/dbg-ext \

  OBJECTS += ${addsuffix .o, ${DEBUG_PROJECT_CSRC}}
  CFLAGS  += -g3 -O1 -DDEBUG
  ASFLAGS += -g3 -O1 -DDEBUG
  CXXFLAGS += -g3 -O1 -DDEBUG
else ifeq ($(BUILD),RELEASE)
  CFLAGS  += -g0 -O3
  ASFLAGS += -g0 -O3
  CXXFLAGS += -g0 -O3
else
  $(error Wrong BUILD '$(BUILD)'! Should be: DEBUG or RELEASE)
endif

CFLAGS  += ${addprefix -I, ${INCLUDE_PATH}}
CXXFLAGS  += ${addprefix -I, ${INCLUDE_PATH}}

.PHONY: all build clean flash help

all: ${PROJECT}.elf

build: ${PROJECT}.elf

${PROJECT}.elf: ${OBJECTS}
	$(info $(BUILD) BUILD)
	${CC} $^ -o $@ ${LDFLAGS}
	${OBJCPY} -O ihex $@ ${PROJECT}.hex
	${SIZE} --format=berkeley $@
	
%.c.o: %.c
	${CC} ${CFLAGS} -MD -c $< -o $@

%.s.o: %.s
	${CC} ${ASFLAGS} -MD -c $< -o $@

%.cpp.o: %.cpp
	${CXX} ${CXXFLAGS} -MD -c $< -o $@

clean:
	rm -f ${OBJECTS} $(OBJECTS:.o=.d) ${PROJECT}.elf ${PROJECT}.hex ${PROJECT}.map

flash:
	@if [ -f "${PROJECT}.hex" ] ;                                                 \
  then                                                                          \
    openocd -f openocd/jlink.cfg -f openocd/stm32f1x.cfg                        \
            -c "init" -c "reset init"                                           \
            -c "flash write_image erase $(PROJECT).hex"                         \
            -c "reset" -c "shutdown";                                           \
  else                                                                          \
    echo "Output .hex file doesn't exist. Run 'make all' before!";              \
  fi

erase:
	openocd -f openocd/jlink.cfg -f openocd/stm32f1x.cfg                          \
            -c "init" -c "reset init"                                           \
            -c "flash probe 0" -c "flash protect 0 0 63 off"                    \
            -c "reset halt" -c "stm32f1x mass_erase 0"                          \
            -c "reset" -c "shutdown";                                           \

debug:
	@if [ -f "${PROJECT}.elf" ] ;                                                 \
  then                                                                          \
    openocd -f openocd/jlink.cfg -f openocd/stm32f1x.cfg &                      \
    ddd "${PROJECT}.elf" --debugger "${GDB} -ex 'target extended-remote :3333'  \
                                            -ex 'monitor reset halt'            \
                                            -ex 'load'                          \
                                            -ex 'monitor reset halt'";          \
    kill $$!;                                                                   \
  else                                                                          \
    echo "Output .elf file doesn't exist. Run 'make all' before!";              \
  fi

console:
	JLinkRTTViewer --device STM32F103C8 --rttaddr 0x20000000 --autoconnect

help:
	$(info Help)
	$(info ---------------------------------------------------------------------)
	$(info make Help                     - this help text)
	$(info make BUILD=<build_type> build - create binary files (.elf, .hex))
	$(info make BUILD=<build_type> all   - create binary files (.elf, .hex). same as build)
	$(info make clean                    - remove generated files)
	$(info make flash                    - flash target)
	$(info make erase                    - flash erase)
	$(info make debug                    - start debugging session DDD(gdb))
	$(info make console                  - start debugging console(traces, RTOS statistics...))
	$(info ---------------------------------------------------------------------)
	$(info Available build_type [DEBUG RELEASE], eg: make BUILD=RELEASE ...)
	$(info Default build_type DEBUG)

-include $(OBJECTS:.o=.d)
