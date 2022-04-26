#ifndef GET_PUT_H 
#define GET_PUT_H 

#include <stdint.h>

static inline void put32(uintptr_t addr, uint32_t val) {
    *((volatile uint32_t*)addr) = val; 
}
static inline uint32_t get32(uintptr_t addr) {
    return *((volatile uint32_t*)addr); 
}




#endif 



