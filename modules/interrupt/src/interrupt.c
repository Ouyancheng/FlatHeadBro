#include "interrupt.h"
#include "mcsr-ext.h"
#include "csr-read-write.h"
#include "printf.h"


uintptr_t set_interrupt_handler(uintptr_t address, enum interrupt_handler_mode mode) {
    uintptr_t base = (address & (~((uintptr_t)0b11))); 
    base |= (uintptr_t)mode & 0b11; 
    write_csr(MTVEC, base); 
    return base; 
}


uintptr_t enable_all_interrupts(void) {
    uintptr_t bitmask = MIE_SSIE | MIE_MSIE | MIE_STIE | MIE_MTIE | MIE_SEIE | MIE_MEIE | MIE_MOIE; 
    csr_read_and_set_bit(MIE, bitmask); 
    return bitmask; 
}

uintptr_t disable_all_interrupts(void) {
    uintptr_t bitmask = MIE_SSIE | MIE_MSIE | MIE_STIE | MIE_MTIE | MIE_SEIE | MIE_MEIE | MIE_MOIE; 
    csr_read_and_clear_bit(MIE, bitmask); 
    return bitmask; 
}

void interrupt_handler(uintptr_t cause, uintptr_t pc, uintptr_t fault_address) {
    printf("interrupt arises! cause=%x, pc=%x, fault_address=%x\n", cause, pc, fault_address); 
    return; 
}
