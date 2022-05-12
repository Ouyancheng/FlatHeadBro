#ifndef CLINT_H 
#define CLINT_H 
#include <stdint.h>

/// the CLINT base address, not very well documented TBH 
#define CLINT_BASE (0x14000000UL)
/// machine software interrupt pending (can generate an interrupt by setting this to 1?)
#define MSIP0 CLINT_BASE 
/// machine timer comparison register (lower 32-bit)
#define MTIMECMPL0 (CLINT_BASE + 0x4000)
/// machine timer comparison register (higher 32-bit)
#define MTIMECMPH0 (CLINT_BASE + 0x4004) 
/// supervisor software interrupt pending (ditto)
#define SSIP0 (CLINT_BASE + 0xC000) 
/// supervisor timer comparison register (lower 32-bit) 
#define STIMECMPL0 (CLINT_BASE + 0xD000) 
/// supervisor timer comparison register (higher 32-bit) 
#define STIMECMPH0 (CLINT_BASE + 0xD004) 

/// must be called from machine mode, because the corresponding bit in SXSTATUS is not writable 
void clint_enable_supervisor_interrupt(void); 
/// must be called from machine mode, because the corresponding bit in SXSTATUS is not writable 
void clint_disable_supervisor_interrupt(void); 

/// sets the MSIP0 register, val should only be 0 or 1 
void clint_set_machine_software_interrupt(uint32_t val); 
/// sets the SSIP0 register, val should only be 0 or 1 
void clint_set_supervisor_software_interrupt(uint32_t val); 

void clint_set_machine_timer_interrupt(uint64_t ticks_after); 
void clint_clear_machine_timer_interrupt(void); 

void clint_set_supervisor_timer_interrupt(uint64_t ticks_after); 
void clint_clear_supervisor_timer_interrupt(void); 

void clint_machine_timer_interrupt_handler(void) __attribute__((interrupt("machine"))); 
void clint_supervisor_timer_interrupt_handler(void) __attribute__((interrupt("supervisor"))); 



#endif 

