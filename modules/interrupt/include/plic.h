#ifndef PLIC_H 
#define PLIC_H 

#define PLIC_BASE        (0x10000000ULL)
/// PLIC priority register (0 < n < 256) 
#define PLIC_PRIOn(n)     (PLIC_BASE + 0x0004 * (n))
/// PLIC interrupt pending register (0 <= n < 9) 
#define PLIC_IPn(n)       (PLIC_BASE + 0x1000 + 0x0004 * (n/32))
/// PLIC machine mode interrupt enable register (0 <= n < 9) 
#define PLIC_MIEn(n)      (PLIC_BASE + 0x2000 + 0x0004 * (n/32))
/// PLIC supervisor mode interrupt enable register (0 <= n < 9) 
#define PLIC_SIEn(n)      (PLIC_BASE + 0x2080 + 0x0004 * (n/32)) 
/// PLIC control register 
#define PLIC_CTRL        (PLIC_BASE + 0x1FFFFC) 
/// PLIC machine threshold register 
#define PLIC_MTH         (PLIC_BASE + 0x200000)
/// PLIC machine claim register 
#define PLIC_MCLAIM      (PLIC_BASE + 0x200004)
/// PLIC supervisor threshold register 
#define PLIC_STH         (PLIC_BASE + 0x201000) 
/// PLIC supervisor claim register 
#define PLIC_SCLAIM      (PLIC_BASE + 0x201004) 


#define PLIC_IP(x)       (PLIC_BASE + 0x1000 + 0x0004 * (x))
#define PLIC_MIE(x)      (PLIC_BASE + 0x2000 + 0x0004 * (x))
#define PLIC_SIE(x)      (PLIC_BASE + 0x2080 + 0x0004 * (x)) 

#define PLIC_MAX_INTERRUPTS 256

typedef void(*irq_handler_t)(void); 

void plic_reset(void); 

void plic_interrupt_enable(int irq, irq_handler_t handler, int priority); 

void plic_interrupt_disable(int irq); 

void plic_dispatch_interrupt(void) __attribute__((interrupt("machine"))); 

#endif 
