#include "delay.h"
#if 0
void delay_us(uint64_t us) {
    uint64_t current; 
    uint64_t initial; 
    us *= 24UL; 
    initial = get_arch_counter(); 
    do {
        current = get_arch_counter(); 
    } while (current - initial <= us); 
}
#endif 
// This handles wrap around 
void delay_us(uint64_t us) {
    uint64_t us24 = us * 24UL; 
    uint64_t start = get_arch_counter(); 
    while (1) {
        if (get_arch_counter() - start >= us24) {
            break;
        }
    }
}
void delay_ms(uint64_t ms) {
    return delay_us(ms * 1000UL); 
}
void delay_cycles(uint64_t cycles) {
    while (cycles --> 0) {
        asm volatile ("nop" :::); 
    }
}

void enable_supervisor_counter_access(void) {
    asm volatile (
        "csrw mcounteren, %0"
        :
        :"rK"(0xFFFFFFFF)
        :
    ); 
}
