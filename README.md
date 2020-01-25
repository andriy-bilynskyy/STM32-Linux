# STM32 development under Linux

## YouTube demo

https://youtu.be/57-iolU-PpE

## Cheap startup hardware

**USB JTAG Jlink clone** <br/>
https://ru.aliexpress.com/item/New-JLink-OB-ARM-Emulator-Debugger-Programmer-Downloader-for-Replace-V8-SWD-M74/32827782488.html?spm=a2g0v.10010108.1000014.5.63f67f76c8Ikyq&pvid=de125860-82b2-43f4-a546-0bd2bf70b9c7&gps-id=pcDetailBottomMoreOtherSeller&scm=1007.13338.112235.000000000000000&scm-url=1007.13338.112235.000000000000000&scm_id=1007.13338.112235.000000000000000

**Evaluation STM32 board** <br/>
https://ru.aliexpress.com/item/STM32F103C8T6-ARM-STM32-arduino/32887666464.html?spm=2114.13010708.0.0.134f33edtKnEcc

## Board description:

https://wiki.stm32duino.com/index.php?title=Blue_Pill#Software_installation

## Flashing tools

https://www.segger.com/downloads/flasher/ <br/>
Select product for your operation system. In case of Ubuntu 64 bit use:
J-Link Software and Documentation pack for Linux, DEB installer, 64-bit.
To flash/erase use JFlashLiteExe.

## Development tools

**GNU Arm Embedded Toolchain** <br/>
https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads

**openocd** <br/>
Available as package in most Linux distros.

## OpenOCD debugging

Start server

    openocd -f openocd/jlink.cfg -f openocd/stm32f1x.cfg

Start client

    <path_to_your_toolchain>/bin/arm-none-eabi-gdb -ex "target remote localhost:3333" <your_generated_elf_file>

Download binary file

    monitor reset halt
    load
    monitor reset run

General commands:

| command                              | description                                       |
|--------------------------------------|---------------------------------------------------|
| monitor reset halt                   | reset MCU and stop                                |
| continue                             | run SW (to stop Ctrl+C)                           |
| next/nexti                           | next codeline/instruction                         |
| step/stepi                           | next codeline/instruction with entering functions |
| finish                               | exit from current function                        |
| until                                | run until reach line greater current (loop exit)  |
| list/list-                           | show 10 code lines down/up                        |
| break < function, line, file:line >  | add breakpoint                                    |
| break *0x< address >                 | add breakpoint on hw address                      |
| watch < variable >                   | set watchpoint on write to variable               |
| watch *0x< address >                 | set watchpoint on write to address                |
| rwatch < variable >                  | set watchpoint on read from variable              |
| awatch < variable >                  | set watchpoint on read or write variable          |
| print < variable >                   | show variable                                     |
| display < variable >                 | add variable to watch list                        |
| info breakpoints/dispaly             | show breakpoints/watchpoints information          |
| delete breakpoint/dispaly < id >     | delete breakpoint/watchpoint by id from display   |
| enable breakpoint/dispaly < id >     | enable breakpoint/watchpoint by id from display   |
| disable breakpoint/dispaly < id >    | disable breakpoint/watchpoint by id from display  |
| set < variable >:=< value >          | assign new value to variable                      |
| info registers                       | show registers                                    |
| monitor reg                          | add registers to permanent watch                  |
| backtrace                            | show stack                                        |
| quit                                 | exit debugger                                     |

## gdb GUI

**ddd** - The Data Display Debugger
Available as package in most Linux distros.

Start ddd

    ddd <your_generated_elf_file> --debugger "<path_to_your_toolchain>/bin/arm-none-eabi-gdb"

Connect to gdb server (issue command in command window)

    target extended-remote :3333

## SEGGER's Real Time Transfer (RTT)
It's JTAG console like semihosting but seems to be more fast and flexible.
https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/
Installation described in section **Flashing tools**

## Project SW components

**Segger RTT** <br/>
https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/

**Free RTOS** <br/>
https://www.freertos.org/

**STM32F10x standard peripheral library** <br/>
https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32-standard-peripheral-libraries/stsw-stm32054.html

## Quick start guide

### Install tools

    wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/8-2018q4/gcc-arm-none-eabi-8-2018-q4-major-linux.tar.bz2?revision=d830f9dd-cd4f-406d-8672-cca9210dd220?product=GNU%20Arm%20Embedded%20Toolchain,64-bit,,Linux,8-2018-q4-major
    sudo tar xjC /opt/ -f 'gcc-arm-none-eabi-8-2018-q4-major-linux.tar.bz2?revision=d830f9dd-cd4f-406d-8672-cca9210dd220?product=GNU Arm Embedded Toolchain,64-bit,,Linux,8-2018-q4-major'

    sudo apt-get install openocd make ddd

    wget https://www.segger.com/downloads/flasher/JLink_Linux_x86_64.deb
    sudo dpkg -i JLink_Linux_x86_64.deb
    export LD_LIBRARY_PATH="/opt/SEGGER/JLink"

*Note:* <br/>
To persist LD_LIBRARY_PATH after reboot add the next lines to the end of your ~/.bashrc

    if [ -d "/opt/SEGGER/JLink" ] ; then
        export LD_LIBRARY_PATH="/opt/SEGGER/JLink"
    fi

### Obtain STM32 project template

    git clone https://github.com/andriy-bilynskyy/STM32-Linux.git
    cd cd STM32-Linux/

### Build and flash

    make build
    make flash

*Note:* <br/>
Use make help to see all options: debug, console...
