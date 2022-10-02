#include "mcsr-ext.h"
#include "boot0.h"
#define INV_I_D_CACHE_BTB_BHT (MCOR_CACHESEL_I_D_CACHE | MCOR_INV | MCOR_BHT_INV | MCOR_BTB_INV)
#define BOOT0_STACK (SRAM_C_BASE + SRAM_C_SIZE)

#undef MXSTATUS 
#define MXSTATUS 0x7C0

#undef MCOR 
#define MCOR 0x7C2

.section .text.boot0entry
.globl _entry 
_entry: 
    // disable interrupt 
    csrw mie, zero 
    // enable T-head semi's private ISA extension 
    li t0, MXSTATUS_THEADISAEE
    csrs MXSTATUS, t0

    // invalidate I$, D$, BHT, BTB 
    li t1, INV_I_D_CACHE_BTB_BHT
    csrs MCOR, t1 

    li sp, BOOT0_STACK
    jal _entry_clear_bss 
    jal main 
    // return to FEL 
    li t0, FEL_BASE 
    jr t0 

_entry_clear_bss: 
    la t0, __bss_start
    la t1, __bss_end
_entry_clear_bss_loop: 
    sw zero, 0(t0)
    addi t0, t0, 4 
    blt t0, t1, _entry_clear_bss_loop
    ret
