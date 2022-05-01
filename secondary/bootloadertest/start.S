#include "mcsr-ext.h"
#include "asm-defs.h"
.section .text.start
    .global _start
_start:
    j reset

/* a macro for floating point mov */
.macro ZERO_OUT_REG rd 
    li \rd, 0 
.endm 



/*
 * The actual reset code
 */
reset:
    /* Disable all interrupts */
    csrw mideleg, zero
    csrw medeleg, zero
    csrw mie, zero
    csrw mip, zero

    /* Initial registers */
    li x1, 0 /* ra, we are not returning to boot0, if we plan to return, should also save sp */
    li x2, 0 /* sp, initialize later */
    li x3, 0 /* gp, initialized to .data + 0x800 (2048) to achieve largest address access */
    li x4, 0 /* tp */
    li x5, 0 /* t0 */
    li x6, 0
    li x7, 0
    li x8, 0 /* s0 or fp */
    li x9, 0
    li x10, 0
    li x11, 0
    li x12, 0
    li x13, 0
    li x14, 0
    li x15, 0
    li x16, 0
    li x17, 0
    li x18, 0
    li x19, 0
    li x20, 0
    li x21, 0
    li x22, 0
    li x23, 0
    li x24, 0
    li x25, 0
    li x26, 0
    li x27, 0
    li x28, 0
    li x29, 0
    li x30, 0
    li x31, 0

    /* Enable FPU and accelerator if present */
    li t0, MSTATUS_FS | MSTATUS_XS
    csrs mstatus, t0
    /* the floating point registers */
#ifdef __riscv_flen 
    fssr x0
    IREG_TO_FREG f0, x0
    IREG_TO_FREG f1, x0
    IREG_TO_FREG f2, x0
    IREG_TO_FREG f3, x0
    IREG_TO_FREG f4, x0
    IREG_TO_FREG f5, x0
    IREG_TO_FREG f6, x0
    IREG_TO_FREG f7, x0
    IREG_TO_FREG f8, x0
    IREG_TO_FREG f9, x0
    IREG_TO_FREG f10, x0
    IREG_TO_FREG f11, x0
    IREG_TO_FREG f12, x0
    IREG_TO_FREG f13, x0
    IREG_TO_FREG f14, x0
    IREG_TO_FREG f15, x0
    IREG_TO_FREG f16, x0
    IREG_TO_FREG f17, x0
    IREG_TO_FREG f18, x0
    IREG_TO_FREG f19, x0
    IREG_TO_FREG f20, x0
    IREG_TO_FREG f21, x0
    IREG_TO_FREG f22, x0
    IREG_TO_FREG f23, x0
    IREG_TO_FREG f24, x0
    IREG_TO_FREG f25, x0
    IREG_TO_FREG f26, x0
    IREG_TO_FREG f27, x0
    IREG_TO_FREG f28, x0
    IREG_TO_FREG f29, x0
    IREG_TO_FREG f30, x0
    IREG_TO_FREG f31, x0
#endif 
    /* Initialize global pointer, should not relax because relaxing uses the global pointer... */
.option push
.option norelax
    la t0, _global_pointer$
    ld gp, (t0)
.option pop

    /* Initialize stacks */
    la t1, _stack_start
    ld t1, (t1)
    la t2, _stack_end
    ld t2, (t2)
    sub t0, t2, t1  /* basically the stack size */
    
    csrr t3, mhartid  /* the thread id */
    /* since we have only one core, we only need to care about core0, if multicore, other cores go to sleep (WFI?) */
    li t4, 1
    div t0, t0, t4
    mul t0, t0, t3
    sub sp, t2, t0
    
    jal clear_bss 
    jal main
    /* we need to call this if we don't return, */
    /* and we couldn't return because we've cleared the ra/x1 register */
    jal software_reset 

clear_bss:
    la    t0, __bss_start
    la    t1, __bss_end
clbss_1:
    sw zero, 0(t0)
    addi t0, t0, XLEN_BYTES
    blt t0, t1, clbss_1
    ret

/*
 * The location of section
 */
    .align 3
_image_start:
    RVPTR __image_start
_image_end:
    RVPTR __image_end
_global_pointer$:
    RVPTR __global_pointer$
_data_start:
    RVPTR __data_start
_data_end:
    RVPTR __data_end
_bss_start:
    RVPTR __bss_start
_bss_end:
    RVPTR __bss_end
_stack_start:
    RVPTR __stack_start
_stack_end:
    RVPTR __stack_end
