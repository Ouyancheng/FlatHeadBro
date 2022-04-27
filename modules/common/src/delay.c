#include "delay.h"
void delay_us(uint64_t us) {
    uint64_t current; 
    uint64_t initial; 
    us *= 24UL; 
    initial = get_arch_counter(); 
    do {
        current = get_arch_counter(); 
    } while (current - initial <= us); // NOTE: this handles wrap around 
}
void delay_ms(uint64_t ms) {
    return delay_us(ms * 1000UL); 
}
