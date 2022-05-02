#include "interrupt.h"
#include "mcsr-ext.h"
#include "csr-read-write.h"
#include "printf.h"


uintptr_t set_interrupt_vector(uintptr_t address, enum interrupt_handler_mode mode) {
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
    printf("interrupt arises! cause=0x%x, pc=%x, fault_address=%x\n", cause, pc, fault_address); 
    return; 
}

void default_vector_handler(uintptr_t cause, uintptr_t pc, uintptr_t fault_address) {
    printf("vector interrupt arises! cause=0x%x, pc=%x, fault_address=%x\n", cause, pc, fault_address); 
    // should we add pc by 4 so that it won't raise the interrupt again? 
    add_to_mexception_return_pc(4); 
}
#if 0
void exception_trampoline(void) __attribute__((interrupt("machine")));
void exception_trampoline(void) {
    printf("hello interrupt!\n");
}
#endif 







