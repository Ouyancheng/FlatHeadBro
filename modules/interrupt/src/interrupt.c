#include "interrupt.h"
#include "printf.h"
#include "reset.h"

uintptr_t set_machine_interrupt_vector(uintptr_t address, enum interrupt_handler_mode mode) {
    uintptr_t base = (address & (~((uintptr_t)0b11))); 
    base |= (uintptr_t)mode & 0b11; 
    write_csr(MTVEC, base); 
    return base; 
}
uintptr_t set_supervisor_interrupt_vector(uintptr_t address, enum interrupt_handler_mode mode) {
    uintptr_t base = (address & (~((uintptr_t)0b11))); 
    base |= (uintptr_t)mode & 0b11; 
    write_csr(STVEC, base); 
    return base; 
}


uintptr_t enable_all_machine_interrupts(void) {
    uintptr_t bitmask = MIE_MSIE | MIE_MTIE | MIE_MEIE | MIE_MOIE; 
    csr_read_and_set_bit(MIE, bitmask); 
    csr_set_bit(MSTATUS, MSTATUS_MIE);
    return bitmask; 
}
uintptr_t enable_all_supervisor_interrupts(void) {
    uintptr_t bitmask = MIE_SSIE | MIE_STIE | MIE_SEIE; 
    csr_read_and_set_bit(SIE, bitmask); 
    csr_set_bit(SSTATUS, MSTATUS_SIE);
    return bitmask; 
}

uintptr_t disable_all_machine_interrupts(void) {
    uintptr_t bitmask = MIE_MSIE | MIE_MTIE | MIE_MEIE | MIE_MOIE; 
    csr_read_and_clear_bit(MIE, bitmask); 
    csr_clear_bit(MSTATUS, MSTATUS_MIE);
    return bitmask; 
}
uintptr_t disable_all_supervisor_interrupts(void) {
    uintptr_t bitmask = MIE_SSIE | MIE_STIE | MIE_SEIE; 
    csr_read_and_clear_bit(SIE, bitmask); 
    csr_clear_bit(SSTATUS, MSTATUS_SIE);
    return bitmask; 
}

void interrupt_handler(uintptr_t cause, uintptr_t pc, uintptr_t fault_address) {
    printf("interrupt arises! cause=0x%x, pc=%x, fault_address=%x\n", cause, pc, fault_address); 
    software_reset(); 
    return; 
}

void default_vector_handler(uintptr_t cause, uintptr_t pc, uintptr_t fault_address) {
    printf("vector interrupt arises! cause=0x%x, pc=%x, fault_address=%x\n", cause, pc, fault_address); 
    // should we add pc by 4 so that it won't raise the interrupt again? 
    // add_imm_to_mexception_return_pc(4); 
    // add_var_to_mexception_return_pc(4); 
    // add_imm_to_csr("mepc", 4); 
    software_reset(); 
}
#if 0
void exception_trampoline(void) __attribute__((interrupt("machine")));
void exception_trampoline(void) {
    printf("hello interrupt!\n");
}
#endif 

void interrupt_delegate_to_supervisor_mode(uintptr_t interrupt_mask) {
    write_csr(MIDELEG, interrupt_mask); 
}
void exception_delegate_to_supervisor_mode(uintptr_t exception_mask) {
    write_csr(MEDELEG, exception_mask); 
}





