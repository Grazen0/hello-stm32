ENTRY(reset_handler)

MEMORY
{
    FLASH (rx): ORIGIN = 0x08000000, LENGTH = 128K
    SRAM  (rwx): ORIGIN = 0x20000000, LENGTH = 20K
}

_estack = ORIGIN(SRAM) + LENGTH(SRAM);

SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(128);
        KEEP(*(.isr_vector))
    } > FLASH

    .text :
    {
        . = ALIGN(4);

        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)

        . = ALIGN(4);
        _etext = .;
    } > FLASH

    _sidata = LOADADDR(.data);

    .data : 
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > SRAM AT > FLASH

    .bss :
    {
        . = ALIGN(4);
        _sbss = .;

        *(.bss)
        *(.bss.*)

        . = ALIGN(4);
        _ebss = .;
    } > SRAM
}

