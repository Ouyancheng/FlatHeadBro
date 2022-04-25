#include "uart.h"
#include "clock-init.h"
#include "dram-init.h"
#include "boot0-header.h"
#include "gpio.h"
#define FEL_BASE 0x20 
void boot0_jmp(uintptr_t addr) {
    asm volatile ("jr a0"); 
}
void main(void) {
    int dram_size;
    int status;

    uart_ctl = uart_init(0, 1); 
    const char msg[] = "Hello World!!!"; 
    for (unsigned i = 0; i < sizeof(msg); ++i) {
        if (msg[i] == '\0') {
            uart_putc(uart_ctl, '\n'); 
        } else {
            uart_putc(uart_ctl, msg[i]); 
        }
    }
    sys_clock_init(); 
    sys_dram_init(&(BT0_head.prvt_head.dram_para)); 

    gpio_set_config(gpio_pe, 16, gpio_config_output); 
    int pe16v = 0; 
    while (1) {
        char c = uart_getc(uart_ctl); 
        uart_putc(uart_ctl, c);
        pe16v = (!pe16v); 
        gpio_write(gpio_pe, 16, pe16v);
    }

    // complete! 
    boot0_jmp(FEL_BASE); 
} 


