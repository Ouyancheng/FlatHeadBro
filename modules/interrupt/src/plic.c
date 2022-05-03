#include "plic.h"
#include "get-put.h"

void (*irq_dispatch_table[PLIC_MAX_INTERRUPTS])(void); 

void plic_reset(void) {
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

void plic_interrupt_enable(int irq, irq_handler_t handler, int priority) {
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

void plic_interrupt_disable(int irq) {
    irq &= 0xFF; 
    // deregister on the dispatch table 
    irq_dispatch_table[irq] = (irq_handler_t)0;
    // set the priority to 0 to invalidate the interrupt 
    put32(PLIC_PRIOn(irq), 0);  
    // disable the interrupt 
    uintptr_t plic_mie_addr = PLIC_MIEn(irq); 
    uint32_t plic_mie_n = get32(plic_mie_addr); 
    plic_mie_n &= ~(1 << (irq % 32)); 
    put32(plic_mie_addr, plic_mie_n); 
}

void plic_dispatch_interrupt(void) {
    while (1) {
        uint32_t irq = get32(PLIC_MCLAIM); 
        if (!irq || irq >= 256) return; 
        // handle the interrupt 
        void(*handler)(void) = irq_dispatch_table[irq]; 
        if (handler) { handler(); }
        put32(PLIC_MCLAIM, irq); 
    }
}

