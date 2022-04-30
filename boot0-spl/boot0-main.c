#include "uart.h"
#include "clock-init.h"
#include "dram-init.h"
#include "boot0-header.h"
#include "reset.h"
#include "gpio.h"
#include "uartgetcode.h"
#include "delay.h"
#ifndef FEL_BASE
#define FEL_BASE   0x20UL 
#endif 
#define DRAM_START 0x40000000UL
void boot0_jmp(uintptr_t addr) {
    asm volatile ("jr a0"); 
}
int sys_uart_printf(const char * fmt, ...);
void main(void) {
    int dram_size;
    int status;

    uart0_ctl = uart_init(0, 1); 
    // const char msg[] = "Hello World!!!"; 
    // for (unsigned i = 0; i < sizeof(msg); ++i) {
    //     if (msg[i] == '\0') {
    //         uart_putc(uart0_ctl, '\n'); 
    //     } else {
    //         uart_putc(uart0_ctl, msg[i]); 
    //     }
    // }
    sys_clock_init(); 
    sys_dram_init(&(BT0_head.prvt_head.dram_para)); 

    // gpio_set_config(gpio_pe, 16, gpio_config_output); 
    // int pe16v = 0; 
    // int count = 0; 
    // while (count < 10) {
    //     char c = uart_getc(uart0_ctl); 
    //     uart_putc(uart0_ctl, c);
    //     pe16v = (!pe16v); 
    //     gpio_write(gpio_pe, 16, pe16v);
    //     count += 1; 
    // }

    // software_reset(); 

    // get code via UART 
    uintptr_t secondary_program_start_address = uart_get_code(); 
    if (!secondary_program_start_address || secondary_program_start_address != DRAM_START) {
        const char msg[] = "Boot Error! Rebooting..."; 
        for (unsigned i = 0; i < sizeof(msg); ++i) {
            if (msg[i] == '\0') {
                // uart_putc(uart0_ctl, '\r'); 
                uart_putc(uart0_ctl, '\n'); 
            } else {
                uart_putc(uart0_ctl, msg[i]); 
            }
        }
        delay_ms(1000); 
        software_reset(); 
    }

    // boot0_jmp(DRAM_START); 
    void(*dram_start)(void) = (void(*)(void))(uintptr_t)DRAM_START;

    dram_start(); 
    // if the secondary program exits gracefully, it's great!!! 
    software_reset(); 

    // complete! 
    boot0_jmp(FEL_BASE); 
} 

















