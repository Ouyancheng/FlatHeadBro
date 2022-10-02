#include "boot0.h"
#include "boot0-header.h"
#include "uart.h"
#include "clock-init.h"
#include "dram-init.h"
#include "reset.h"
#include "gpio.h"
#include "uartgetcode.h"
#include "delay.h"
#include "fence.h"

void print_str(const char *str) {
    while (*str != '\0') {
        uart_putc(uart0_ctl, *str++); 
    }
}
void print_hex(uintptr_t i) {
    char buffer[sizeof(uintptr_t)*2 + 2]; 
    int buf_idx = 0; 
    if (i == 0) {
        buffer[buf_idx++] = '0'; 
    }
    while (i) {
        uint8_t hex = (i & 0xF); 
        i >>= 4; 
        if (hex < 10) {
            buffer[buf_idx] = hex + '0'; 
        } else {
            buffer[buf_idx] = hex - 10 + 'A'; 
        }
        buf_idx += 1; 
    }
    buffer[buf_idx++] = 'x'; 
    buffer[buf_idx++] = '0'; 
    while (buf_idx --> 0) {
        uart_putc(uart0_ctl, buffer[buf_idx]); 
    }
    dev_barrier(); 
}

void main(void) {
    int dram_size;
    int status;
    uart0_ctl = uart_init(0, 1); 
    
    sys_clock_init(); 
    int dram_init_result = sys_dram_init(&(BT0_header.private_header.dram_parameter)); 
    print_str("dram init result (dram size): ");
    print_hex(dram_init_result); 
    print_str("MB\n"); 
    dev_barrier(); 

    // get code via UART 
    uintptr_t secondary_program_start_address = uart_get_code(DRAM_START); 
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

}

