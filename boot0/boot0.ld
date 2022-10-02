OUTPUT_FORMAT("elf64-littleriscv", "elf64-littleriscv", "elf64-littleriscv")
OUTPUT_ARCH("riscv")
ENTRY(_entry)
SECTIONS
{
    /* SRAM start */ 
    . = 0x20000;
    . = ALIGN(4);
    .boot0header : {
        *(.boot0header.header)
    }
    /* . = ALIGN(1); */ 
    .text : {
        /* the entry code */
        *(.text.boot0entry)
        *(.text)
    }
    . = ALIGN(16);
    .rodata : { 
        *(.rodata) 
        *(.rodata.*)
    }
    . = ALIGN(16);
    .data : { 
        *(.data) 
    }
    . = ALIGN(4);
    .bss : {
        __bss_start = .;
        *(.bss)
    }
    . = ALIGN(4);
    __bss_end = .;
    /* SRAM base = 0x20000, SRAM size = 32KB */
    ASSERT(. < (0x20000 + 32768), "boot0 image has exceeded its limit.")
}

