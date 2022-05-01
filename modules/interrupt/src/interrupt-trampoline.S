#include "asm-defs.h" 

// allocate 31 positions for the registers (except for x0/zero) 
.macro INTERRUPT_SAVE_REGS 
    addi sp, sp, -31 * XLEN_BYTES
    STR x1,  0*XLEN_BYTES(sp)
    STR x2,  1*XLEN_BYTES(sp)
    STR x3,  2*XLEN_BYTES(sp)
    STR x4,  3*XLEN_BYTES(sp)
    STR x5,  4*XLEN_BYTES(sp)
    STR x6,  5*XLEN_BYTES(sp)
    STR x7,  6*XLEN_BYTES(sp)
    STR x8,  7*XLEN_BYTES(sp)
    STR x9,  8*XLEN_BYTES(sp)
    STR x10, 9*XLEN_BYTES(sp)
    STR x11, 10*XLEN_BYTES(sp)
    STR x12, 11*XLEN_BYTES(sp)
    STR x13, 12*XLEN_BYTES(sp)
    STR x14, 13*XLEN_BYTES(sp)
    STR x15, 14*XLEN_BYTES(sp)
    STR x16, 15*XLEN_BYTES(sp)
    STR x17, 16*XLEN_BYTES(sp)
    STR x18, 17*XLEN_BYTES(sp)
    STR x19, 18*XLEN_BYTES(sp)
    STR x20, 19*XLEN_BYTES(sp)
    STR x21, 20*XLEN_BYTES(sp)
    STR x22, 21*XLEN_BYTES(sp)
    STR x23, 22*XLEN_BYTES(sp)
    STR x24, 23*XLEN_BYTES(sp)
    STR x25, 24*XLEN_BYTES(sp)
    STR x26, 25*XLEN_BYTES(sp)
    STR x27, 26*XLEN_BYTES(sp)
    STR x28, 27*XLEN_BYTES(sp)
    STR x29, 28*XLEN_BYTES(sp)
    STR x30, 29*XLEN_BYTES(sp)
    STR x31, 30*XLEN_BYTES(sp)
.endm 
.macro INTERRUPT_LOAD_REGS
    LDR x1,  0*XLEN_BYTES(sp)
    LDR x2,  1*XLEN_BYTES(sp)
    LDR x3,  2*XLEN_BYTES(sp)
    LDR x4,  3*XLEN_BYTES(sp)
    LDR x5,  4*XLEN_BYTES(sp)
    LDR x6,  5*XLEN_BYTES(sp)
    LDR x7,  6*XLEN_BYTES(sp)
    LDR x8,  7*XLEN_BYTES(sp)
    LDR x9,  8*XLEN_BYTES(sp)
    LDR x10, 9*XLEN_BYTES(sp)
    LDR x11, 10*XLEN_BYTES(sp)
    LDR x12, 11*XLEN_BYTES(sp)
    LDR x13, 12*XLEN_BYTES(sp)
    LDR x14, 13*XLEN_BYTES(sp)
    LDR x15, 14*XLEN_BYTES(sp)
    LDR x16, 15*XLEN_BYTES(sp)
    LDR x17, 16*XLEN_BYTES(sp)
    LDR x18, 17*XLEN_BYTES(sp)
    LDR x19, 18*XLEN_BYTES(sp)
    LDR x20, 19*XLEN_BYTES(sp)
    LDR x21, 20*XLEN_BYTES(sp)
    LDR x22, 21*XLEN_BYTES(sp)
    LDR x23, 22*XLEN_BYTES(sp)
    LDR x24, 23*XLEN_BYTES(sp)
    LDR x25, 24*XLEN_BYTES(sp)
    LDR x26, 25*XLEN_BYTES(sp)
    LDR x27, 26*XLEN_BYTES(sp)
    LDR x28, 27*XLEN_BYTES(sp)
    LDR x29, 28*XLEN_BYTES(sp)
    LDR x30, 29*XLEN_BYTES(sp)
    LDR x31, 30*XLEN_BYTES(sp)
    addi sp, sp, 31 * XLEN_BYTES 
.endm 


.section .text.trampoline 
.align 4 
.globl direct_interrupt_trampoline 
direct_interrupt_trampoline: 
    INTERRUPT_SAVE_REGS 

    // the cause of the interrupt 
    csrr a0, mcause 
    // the fault pc 
    csrr a1, mepc 
    // if applicable, the fault address 
    csrr a2, mtval 

    // call the interrupt handler written in C 
    jal interrupt_handler 

    // should we add pc by 4 so that it won't raise the interrupt again? 
    csrr t0, mepc 
    addi t0, t0, 4 
    csrw mepc, t0 

    INTERRUPT_LOAD_REGS 
    mret 


.align 5
.global interrupt_vector 
interrupt_vector: 
    j direct_interrupt_trampoline // 0: user software interrupt / all of the exceptions 
    j direct_interrupt_trampoline // 1: supervisor software interrupt 
    j direct_interrupt_trampoline // 2: reserved 
    j direct_interrupt_trampoline // 3: machine software interrupt 
    j direct_interrupt_trampoline // 4: user timer interrupt 
    j direct_interrupt_trampoline // 5: supervisor timer interrupt 
    j direct_interrupt_trampoline // 6: reserved 
    j direct_interrupt_trampoline // 7: machine timer interrupt 
    j direct_interrupt_trampoline // 8: user external interrupt 
    j direct_interrupt_trampoline // 9: supervisor external interrupt 
    j direct_interrupt_trampoline // 10: reserved 
    j direct_interrupt_trampoline // 11: machine external interrupt 

#if 0
.align 4
.global exception_trampoline 
exception_trampoline: 
    INTERRUPT_SAVE_REGS
    // the cause of the interrupt 
    csrr a0, mcause 
    // the fault pc 
    csrr a1, mepc 
    // if applicable, the fault address 
    csrr a2, mtval 
    // call the interrupt handler written in C 
    // jal exception_handler  
    // should we add pc by 4 so that it won't raise the exception again? 
    csrr t0, mepc 
    addi t0, t0, 4 
    csrw mepc, t0 
    INTERRUPT_LOAD_REGS 
    mret 

#endif 













