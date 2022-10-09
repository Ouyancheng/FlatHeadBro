OUTPUT_FORMAT("elf64-littleriscv", "elf64-littleriscv", "elf64-littleriscv")
OUTPUT_ARCH("riscv")
ENTRY(_start)


SECTIONS
{
    . = 0x40000000; 
    PROVIDE(__kernel_image_start = .);
    .text : {
        PROVIDE(__text_start = .);
        *(.text.start)  /* the starter code */
        *(.text*)       /* the rest of the text section */
        . = ALIGN(4096);
        /* NOTE: the trampoline is mapped to .text* above, the following block is empty */
        /* now the interrupt / exception trampoline */
        
        PROVIDE(__trampoline_start = .); 
        *(.trampoline.text)
        . = ALIGN(4096);
        PROVIDE(__trampoline_end = .);
        ASSERT(__trampoline_end - __trampoline_start <= 4096, "trampoline is larger than a page"); 
        

        PROVIDE(__text_end = .); 
    }
    .rodata : {
        PROVIDE(__rodata_start = .);
        . = ALIGN(16);
        *(.srodata .srodata.*) /* riscv abi specific, do not need to distinguish this from .rodata */
        . = ALIGN(16);
        *(.rodata .rodata.*)
        . = ALIGN(16); 
        *(.got*) /* Global Offset Table */
        . = ALIGN(4096);  /* align the rodata so that it could align in pages */
        PROVIDE(__rodata_end = .); 
    }

    .data : {
        PROVIDE(__data_start = .); 
        PROVIDE(__global_pointer = . + 2048); /* because the ld address offset is +-2048 */
        . = ALIGN(16);
        *(.sdata .sdata.*) /* riscv abi specific, do not need to distinguish this from .data */
        . = ALIGN(16);
        *(.data .data.*)
        PROVIDE(__data_end = .); 
    }

    PROVIDE(__bss_start = .); 
    .bss : {
        . = ALIGN(16);
        *(.sbss .sbss.*) /* riscv abi specific, do not need to distinguish this from .bss */
        . = ALIGN(16);
        *(.bss .bss.*)
    }
    PROVIDE(__bss_end = .);

    . = ALIGN(4096); 
    PROVIDE(__kernel_image_end = .); 

    . = 0x40000000 + 0x08000000;  /* 128MB kernel memory */
    PROVIDE(__kernel_memory_end = .); 
}











