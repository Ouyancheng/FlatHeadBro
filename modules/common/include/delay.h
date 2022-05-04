#ifndef DELAY_H
#define DELAY_H
#include <stdint.h>
static inline uint64_t get_arch_counter(void) {
    uint64_t cnt = 0;
    asm volatile(
        "csrr %0, time\n"
        : "=r"(cnt)
        :
        : 
    ); // shouldn't need clobber 
    return cnt;
}

static inline uint64_t get_current_time_ms(void) {
    return get_arch_counter() / 24000UL; 
}
static inline uint64_t get_current_time_us(void) {
    return get_arch_counter() / 24UL; 
}

void delay_us(uint64_t us); 
void delay_ms(uint64_t ms); 
void delay_cycles(uint64_t cycles); 
#endif 






