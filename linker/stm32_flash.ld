/*
*****************************************************************************
**
**  (C) 2019 Andrii Bilynskyi <andriy.bilynskyy@gmail.com>
**
**  This code is licenced under the MIT.
**
*****************************************************************************
*/

MEMORY
{
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 128K
    RAM   (rwx)     : ORIGIN = 0x20000000, LENGTH = 20K
}

ENTRY(Reset_Handler)

_estack = 0x20005000;           /* end of 20K RAM */

SECTIONS
{

    .isr_vector : ALIGN(4)
    {
        KEEP(*(.isr_vector))
    } > FLASH

    .text : ALIGN(4)
    {
        *(.text)

        KEEP (*(.init))
        KEEP (*(.fini))

    } > FLASH

    .data_init : ALIGN(4) 
    {   
        _sidata = .;
        *(.data_init)
    } > FLASH

    .rtt : ALIGN(4)
    {
        KEEP(*(.rtt))
    } > RAM

    .data : ALIGN(4)
    {
        _sdata = .;
        *(.data)
        _edata = .;
    } > RAM AT > FLASH

    .bss : ALIGN(4)
    {
        _sbss = .;
        *(.bss)
        _ebss = .;
    } > RAM

}
