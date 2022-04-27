/*
 * (C) Copyright 2018
 * SPDX-License-Identifier: GPL-2.0+
 * wangwei <wangwei@allwinnertech.com>
 */
#include "config.h" 

/* mxstatus */
#define EN_THEADISAEE (0x1 << 22)
#define REGBYTES      4
.section .text.boot0entry
.globl _entry 
_entry: 
    /* disable interrupt */
    csrw mie, zero

    /* enable theadisaee -- use the extended instruction set of C906 */
    li t1, EN_THEADISAEE
    csrs 0x7c0, t1 /* mxstatus */ /* it's C906's extended control register */

    /* invaild ICACHE/DCACHE/BTB/BHT */
    li t2, 0x30013
    csrs 0x7c2, t2 /* mcor */ /* it's C906's extended control register */

    li sp, CONFIG_NBOOT_STACK
    jal clear_bss
    jal  main
    j .

clear_bss:
    la  t0, __bss_start
    la  t1, __bss_end
clrbss_loop:
    sw zero, 0(t0)
    addi t0, t0, REGBYTES
    blt t0, t1, clrbss_loop
    ret



