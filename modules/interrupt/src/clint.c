#include "clint.h" 
#include "get-put.h"
#include "delay.h"
#include "mcsr-ext.h"
#include "csr-read-write.h"
#include "interrupt.h"
#include "printf.h"
#include "fence.h"
void clint_enable(void) {
    csr_set_bit(MXSTATUS, MXSTATUS_CLINTEE); 
}
void clint_disable(void) {
    csr_clear_bit(MXSTATUS, MXSTATUS_CLINTEE); 
}
/// sets the MSIP0 register, val should only be 0 or 1 
void clint_set_machine_software_interrupt(uint32_t val) {
    if (val) csr_set_bit(MIE, MIE_MSIE); else csr_clear_bit(MIE, MIE_MSIE); 
    put32(MSIP0, val & 1); 
}
/// sets the SSIP0 register, val should only be 0 or 1 
void clint_set_supervisor_software_interrupt(uint32_t val) {
    if (val) csr_set_bit(MIE, MIE_SSIE); else csr_clear_bit(MIE, MIE_SSIE); 
    put32(SSIP0, val & 1); 

}

void clint_set_machine_timer_interrupt(uint64_t ticks_after) {
    uint64_t now = get_arch_counter(); 
    uint64_t target = ticks_after + now; 
    ////////// WTF? why is directly setting it to uint64_t* not working??? ////////// 
    // *(volatile uint64_t*)(MTIMECMPL0) = ticks_after + now; 
    put32(MTIMECMPL0, target & 0xFFFFFFFF); 
    put32(MTIMECMPH0, target >> 32); 
    csr_set_bit(MIE, MIE_MTIE); 
    dev_barrier(); 
}
void clint_clear_machine_timer_interrupt(void) {
    csr_clear_bit(MIE, MIE_MTIE); 
    // *(volatile uint64_t*)(MTIMECMPL0) = UINT64_MAX; 
    put32(MTIMECMPL0, UINT32_MAX); 
    put32(MTIMECMPH0, UINT32_MAX); 
    dev_barrier(); 
}

void clint_set_supervisor_timer_interrupt(uint64_t ticks_after) {
    uint64_t now = get_arch_counter(); 
    uint64_t target = ticks_after + now; 
    // *(volatile uint64_t*)(STIMECMPL0) = now + ticks_after; 
    put32(STIMECMPL0, target & 0xFFFFFFFF); 
    put32(STIMECMPH0, target >> 32); 
    csr_set_bit(MIE, MIE_STIE); 
    dev_barrier(); 
}
void clint_clear_supervisor_timer_interrupt(void) {
    csr_clear_bit(MIE, MIE_STIE); 
    // *(volatile uint64_t*)(STIMECMPL0) = UINT64_MAX; 
    put32(STIMECMPL0, UINT32_MAX); 
    put32(STIMECMPH0, UINT32_MAX);  
    dev_barrier(); 
}

void clint_machine_timer_interrupt_handler(void) {
    printf("timer interrupt occurred!\n"); 
    clint_clear_machine_timer_interrupt(); 
}

void clint_supervisor_timer_interrupt_handler(void) {
    printf("supervisor timer interrupt occurred!\n"); 
    clint_clear_supervisor_timer_interrupt(); 
}















