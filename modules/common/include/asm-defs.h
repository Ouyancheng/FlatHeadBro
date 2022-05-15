#ifndef ASM_DEFS_H  
#define ASM_DEFS_H  
#if __riscv_xlen == 64
    #define STR        sd
    #define LDR        ld
    #define XLEN_BYTES 8
    #define RVPTR      .dword
#else
    #define STR        sw
    #define LDR        lw
    #define XLEN_BYTES 4
    #define RVPTR      .word
#endif
#if 0 
#if __riscv_xlen == 64
    #define LDR          ld
    #define STR          sd
    #define XLEN_BYTES   8
    #define RVPTR   .dword
#elif __riscv_xlen == 32
    #define LDR          lw
    #define STR          sw
    #define XLEN_BYTES   4
    #define RVPTR   .word
#endif
#endif 
#if __riscv_flen == 32
    #define IREG_TO_FREG fmv.w.x
    // older versions of this instruction naming is fmv.s.x 
    #define FLDR         flw 
    #define FSTR         fsw 
    #define FLEN_BYTES   4 
#elif __riscv_flen == 64
    #define IREG_TO_FREG fmv.d.x
    #define FLDR         fld 
    #define FSTR         fsd 
    #define FLEN_BYTES   8 
#endif 
#define CORE_COUNT 1 
#define KERNEL_STACK_SIZE_PER_CORE 2097152 
#define RISCV_STACK_ALIGN_BYTES 16 
#endif 






