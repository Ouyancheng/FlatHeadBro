OUTPUT_FORMAT("elf64-littleriscv", "elf64-littleriscv", "elf64-littleriscv")
OUTPUT_ARCH("riscv")
ENTRY(_entry)
SECTIONS
{
    . = 0x20000;
    . = ALIGN(4);
    .boot0header :
    {
        *(.boot0header.header)
        /*CMakeFiles/boot0main.elf.dir/boot0-header.c.o (.rodata)*/
    }
    . = ALIGN(1);
    .text :
    {
        *(.text.boot0entry)
        /*CMakeFiles/boot0main.elf.dir/boot0-entry.S.o (.text)*/
        *(.text)
    }
    . = ALIGN(16);
    .rodata : { *(.rodata) }
    . = ALIGN(16);
    .data : { *(.data) }
    . = ALIGN(4);
    .bss :
    {
        __bss_start = .;
        *(.bss)
    }
    . = ALIGN(4);
    __bss_end = .;
    _end = .;
    ASSERT(. <= (0x20000 + 0x10000), "boot0 image has exceeded its limit.")
}
