#include "asm-defs.h" 

// allocate 31 positions for the registers (except for x0/zero) 
.macro INTERRUPT_PUSH_ALL_IREGS 
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
.macro INTERRUPT_POP_ALL_IREGS
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

.macro INTERRUPT_PUSH_IREGS 
    addi sp, sp, -16*XLEN_BYTES
    STR ra,  0*XLEN_BYTES(sp)
    STR t0,  1*XLEN_BYTES(sp)
    STR t1,  2*XLEN_BYTES(sp)
    STR t2,  3*XLEN_BYTES(sp)
    STR a0,  4*XLEN_BYTES(sp)
    STR a1,  5*XLEN_BYTES(sp)
    STR a2,  6*XLEN_BYTES(sp)
    STR a3,  7*XLEN_BYTES(sp)
    STR a4,  8*XLEN_BYTES(sp)
    STR a5,  9*XLEN_BYTES(sp)
    STR a6,  10*XLEN_BYTES(sp)
    STR a7,  11*XLEN_BYTES(sp)
    STR t3,  12*XLEN_BYTES(sp)
    STR t4,  13*XLEN_BYTES(sp)
    STR t5,  14*XLEN_BYTES(sp)
    STR t6,  15*XLEN_BYTES(sp)
.endm 

.macro INTERRUPT_POP_IREGS 
    LDR ra,  0*XLEN_BYTES(sp)
    LDR t0,  1*XLEN_BYTES(sp)
    LDR t1,  2*XLEN_BYTES(sp)
    LDR t2,  3*XLEN_BYTES(sp)
    LDR a0,  4*XLEN_BYTES(sp)
    LDR a1,  5*XLEN_BYTES(sp)
    LDR a2,  6*XLEN_BYTES(sp)
    LDR a3,  7*XLEN_BYTES(sp)
    LDR a4,  8*XLEN_BYTES(sp)
    LDR a5,  9*XLEN_BYTES(sp)
    LDR a6,  10*XLEN_BYTES(sp)
    LDR a7,  11*XLEN_BYTES(sp)
    LDR t3,  12*XLEN_BYTES(sp)
    LDR t4,  13*XLEN_BYTES(sp)
    LDR t5,  14*XLEN_BYTES(sp)
    LDR t6,  15*XLEN_BYTES(sp)
    addi sp, sp, 16*XLEN_BYTES
.endm 

.macro INTERRUPT_PUSH_FREGS 
    addi sp, sp, -20*FLEN_BYTES
    FSTR ft0,  0*FLEN_BYTES(sp)
    FSTR ft1,  1*FLEN_BYTES(sp)
    FSTR ft2,  2*FLEN_BYTES(sp)
    FSTR ft3,  3*FLEN_BYTES(sp)
    FSTR ft4,  4*FLEN_BYTES(sp)
    FSTR ft5,  5*FLEN_BYTES(sp)
    FSTR ft6,  6*FLEN_BYTES(sp)
    FSTR ft7,  7*FLEN_BYTES(sp)
    FSTR fa0,  8*FLEN_BYTES(sp)
    FSTR fa1,  9*FLEN_BYTES(sp)
    FSTR fa2,  10*FLEN_BYTES(sp)
    FSTR fa3,  11*FLEN_BYTES(sp)
    FSTR fa4,  12*FLEN_BYTES(sp)
    FSTR fa5,  13*FLEN_BYTES(sp)
    FSTR fa6,  14*FLEN_BYTES(sp)
    FSTR fa7,  15*FLEN_BYTES(sp) 
    FSTR ft8,  16*FLEN_BYTES(sp)
    FSTR ft9,  17*FLEN_BYTES(sp)
    FSTR ft10, 18*FLEN_BYTES(sp)
    FSTR ft11, 19*FLEN_BYTES(sp)
.endm 

.macro INTERRUPT_POP_FREGS 
    FLDR ft0,  0*FLEN_BYTES(sp)
    FLDR ft1,  1*FLEN_BYTES(sp)
    FLDR ft2,  2*FLEN_BYTES(sp)
    FLDR ft3,  3*FLEN_BYTES(sp)
    FLDR ft4,  4*FLEN_BYTES(sp)
    FLDR ft5,  5*FLEN_BYTES(sp)
    FLDR ft6,  6*FLEN_BYTES(sp)
    FLDR ft7,  7*FLEN_BYTES(sp)
    FLDR fa0,  8*FLEN_BYTES(sp)
    FLDR fa1,  9*FLEN_BYTES(sp)
    FLDR fa2,  10*FLEN_BYTES(sp)
    FLDR fa3,  11*FLEN_BYTES(sp)
    FLDR fa4,  12*FLEN_BYTES(sp)
    FLDR fa5,  13*FLEN_BYTES(sp)
    FLDR fa6,  14*FLEN_BYTES(sp)
    FLDR fa7,  15*FLEN_BYTES(sp) 
    FLDR ft8,  16*FLEN_BYTES(sp)
    FLDR ft9,  17*FLEN_BYTES(sp)
    FLDR ft10, 18*FLEN_BYTES(sp)
    FLDR ft11, 19*FLEN_BYTES(sp)
    addi sp, sp, 20*FLEN_BYTES
.endm 
/// TODO: save and load vector registers 

.macro mtrampoline_save_IREGS handler_function
    INTERRUPT_PUSH_IREGS
    // the cause of the interrupt 
    csrr a0, mcause 
    // the fault pc 
    csrr a1, mepc 
    // if applicable, the fault address 
    csrr a2, mtval 
    // call handler 
    jal \handler_function
    // NOTE: if increamenting PC is needed, do it in handler_function
    INTERRUPT_POP_IREGS
    mret
.endm 

.macro mtrampoline_save_IFREGS handler_function
    INTERRUPT_PUSH_IREGS
    INTERRUPT_PUSH_FREGS
    // the cause of the interrupt 
    csrr a0, mcause 
    // the fault pc 
    csrr a1, mepc 
    // if applicable, the fault address 
    csrr a2, mtval 
    // call handler 
    jal \handler_function
    // NOTE: if increamenting PC is needed, do it in handler_function
    INTERRUPT_POP_FREGS
    INTERRUPT_POP_IREGS
    mret
.endm 

.section .text.trampoline 
.align 4 
.globl direct_interrupt_trampoline 
direct_interrupt_trampoline: 
    INTERRUPT_PUSH_IREGS 

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

    INTERRUPT_POP_IREGS 
    mret 


.align 5
.global interrupt_vector 
interrupt_vector: 
    j default_interrupt_trampoline // 0: user software interrupt / all of the exceptions 
    j default_interrupt_trampoline // 1: supervisor software interrupt 
    j default_interrupt_trampoline // 2: reserved 
    j default_interrupt_trampoline // 3: machine software interrupt 
    j default_interrupt_trampoline // 4: user timer interrupt 
    j default_interrupt_trampoline // 5: supervisor timer interrupt 
    j default_interrupt_trampoline // 6: reserved 
    j default_interrupt_trampoline // 7: machine timer interrupt 
    j default_interrupt_trampoline // 8: user external interrupt 
    j default_interrupt_trampoline // 9: supervisor external interrupt 
    j default_interrupt_trampoline // 10: reserved 
    j default_interrupt_trampoline // 11: machine external interrupt 

.align 4
.global default_interrupt_trampoline 
default_interrupt_trampoline: 
    mtrampoline_save_IFREGS default_vector_handler


#if 0
.align 4
.global exception_trampoline 
exception_trampoline: 
    INTERRUPT_PUSH_IREGS
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
    INTERRUPT_POP_IREGS 
    mret 

#endif 













