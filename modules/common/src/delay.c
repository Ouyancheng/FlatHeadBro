#include "delay.h"
void delay_us(uint64_t us) {
    uint64_t current; 
    uint64_t target; 
    current = get_arch_counter(); 
    target = current + us * 24UL; 
    do {
        current = get_arch_counter(); 
    } while (current <= target); 
}
void delay_ms(uint64_t ms) {
    return delay_us(ms * 1000UL); 
}
