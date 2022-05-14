#include "kernelsdk.h"

uint8_t __attribute__((aligned(4096))) kernel_stack[4096]; 
int panic_at(const char *file, int line, const char *function, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int n = 0; 
    n += printf("At file: %s:%d function %s...\nPANIC: ", file, line, function); 
    n += vprintf(format, args); 
    va_end(args); 
    software_reset(); 
    return n; 
}


