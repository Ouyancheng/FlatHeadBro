#include "plic.h"
#include "get-put.h"
#include "mcsr-standard.h"
#include "mcsr-ext.h"
#include "scsr-ext.h"
#include "scsr-standard.h" 
#include "csr-read-write.h"
// #include "printf.h"
void (*irq_dispatch_table[PLIC_MAX_INTERRUPTS])(void); 
void plic_machine_enable(void) {
    csr_set_bit(MIE, MIE_MEIE); 
}
void plic_machine_disable(void) {
    csr_clear_bit(MIE, MIE_MEIE); 
}
void plic_machine_reset(void) {
    plic_machine_disable(); 
    for (int i = 0; i < PLIC_MAX_INTERRUPTS; ++i) {
        // disable all interrupts 
        put32(PLIC_MIEn(i), 0); 
        // set all interrupt priorities to 0 
        put32(PLIC_PRIOn(i), 0); 
        // indicate the interrupt is complete, clear the pending bit 
        put32(PLIC_MCLAIM, i); 
        irq_dispatch_table[i] = (irq_handler_t)0; 
    }
}

void plic_machine_interrupt_enable(int irq, irq_handler_t handler, int priority) {
    irq &= 0xFF; 
    // register on the dispatch table 
    irq_dispatch_table[irq] = handler; 
    // set the irq priority 
    put32(PLIC_PRIOn(irq), priority); 
    // enable the interrupt 
    uintptr_t plic_mie_addr = PLIC_MIEn(irq); 
    uint32_t plic_mie_n = get32(plic_mie_addr); 
    plic_mie_n |= (1 << (irq % 32)); 
    put32(plic_mie_addr, plic_mie_n); 
}

void plic_machine_interrupt_disable(int irq) {
    irq &= 0xFF; 
    // disable the interrupt 
    uintptr_t plic_mie_addr = PLIC_MIEn(irq); 
    uint32_t plic_mie_n = get32(plic_mie_addr); 
    plic_mie_n &= ~(1 << (irq % 32)); 
    put32(plic_mie_addr, plic_mie_n); 
    // set the priority to 0 to invalidate the interrupt 
    put32(PLIC_PRIOn(irq), 0);  
    // deregister on the dispatch table 
    irq_dispatch_table[irq] = (irq_handler_t)0;
}

void plic_machine_dispatch_interrupt(void) {
    while (1) {
        uint32_t irq = get32(PLIC_MCLAIM); 
        if (!irq) return; 
        // handle the interrupt 
        void(*handler)(void) = irq_dispatch_table[irq]; 
        if (handler) { handler(); }
        put32(PLIC_MCLAIM, irq); 
    }
}
/// must be called from machine mode 
void plic_enable_supervisor_access(void) {
    put32(PLIC_CTRL, 1); 
}
/// must be called from machine mode 
void plic_disable_supervisor_access(void) {
    put32(PLIC_CTRL, 0); 
}
/////////////////////////////////////////////// Supervisor mode ///////////////////////////////////////////////////////// 
void plic_supervisor_enable(void) {
    csr_set_bit(SIE, MIE_SEIE); 
}
void plic_supervisor_disable(void) {
    csr_clear_bit(SIE, MIE_SEIE); 
}
void plic_supervisor_reset(void) {
    plic_supervisor_disable(); 
    for (int i = 0; i < PLIC_MAX_INTERRUPTS; ++i) {
        // disable all interrupts 
        put32(PLIC_SIEn(i), 0); 
        // set all interrupt priorities to 0 
        put32(PLIC_PRIOn(i), 0); 
        // indicate the interrupt is complete, clear the pending bit 
        put32(PLIC_SCLAIM, i); 
        irq_dispatch_table[i] = (irq_handler_t)0; 
    }
}

void plic_supervisor_interrupt_enable(int irq, irq_handler_t handler, int priority) {
    irq &= 0xFF; 
    // register on the dispatch table 
    irq_dispatch_table[irq] = handler; 
    // set the irq priority 
    put32(PLIC_PRIOn(irq), priority); 
    // enable the interrupt 
    uintptr_t plic_sie_addr = PLIC_SIEn(irq); 
    uint32_t plic_sie_n = get32(plic_sie_addr); 
    plic_sie_n |= (1 << (irq % 32)); 
    put32(plic_sie_addr, plic_sie_n); 
}

void plic_supervisor_interrupt_disable(int irq) {
    irq &= 0xFF; 
    // disable the interrupt 
    uintptr_t plic_sie_addr = PLIC_SIEn(irq); 
    uint32_t plic_sie_n = get32(plic_sie_addr); 
    plic_sie_n &= ~(1 << (irq % 32)); 
    put32(plic_sie_addr, plic_sie_n); 
    // set the priority to 0 to invalidate the interrupt 
    put32(PLIC_PRIOn(irq), 0);  
    // deregister on the dispatch table 
    irq_dispatch_table[irq] = (irq_handler_t)0;
}

void plic_supervisor_dispatch_interrupt(void) {
    // printf("PLIC dispatching interrupt!\n");
    while (1) {
        // printf("one interrupt start\n");
        uint32_t irq = get32(PLIC_SCLAIM); 
        // printf("one interrupt claimed\n");
        if (!irq) {
            // printf("no interrupt? irq=%d\n", irq);
            return; 
        }
        // printf("about to dispatch\n");
        // handle the interrupt 
        void(*handler)(void) = irq_dispatch_table[irq]; 
        if (handler) { handler(); }
        // printf("one interrupt dispatched\n");
        put32(PLIC_SCLAIM, irq); 
        // printf("one interrupt done\n");
    }
    // printf("PLIC done dispatching interrupt!\n");
}












