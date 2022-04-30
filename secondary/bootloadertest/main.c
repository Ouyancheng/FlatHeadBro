#include "delay.h"
#include "uart.h"
#include "gpio.h"
#include "reset.h"
#include "fence.h"
#include "printf.h"
inline uintptr_t get_mhartid(void) {
    uintptr_t thread_id; 
    asm volatile (
        "csrr %0, mhartid"
        : "=r"(thread_id)
        : 
        :
    );
    return thread_id;
}
void print_str(const char *str) {
    while (*str != '\0') {
        uart_putc(uart0_ctl, *str++); 
    }
}
void print_hex(uintptr_t i) {
    char buffer[sizeof(uintptr_t) + 2]; 
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
    uart0_ctl = uart_init(0, 1); 

    print_str("hello!!!\n");

    print_hex(get_mhartid()); 
    uart_putc(uart0_ctl, '\n'); 

    printf("%d %x %b %s\n", 123, 123, 123, "hello world!!! testing printf!!!"); 

    gpio_set_config(gpio_pe, 16, gpio_config_output); 
    int pe16v = 0; 
    int i = 0; 
    while (i < 10) {
        char c = uart_getc(uart0_ctl);
        uart_putc(uart0_ctl, c);
        i += 1;
        pe16v = (!pe16v); 
        gpio_write(gpio_pe, 16, pe16v);
    }
    return; 
}


























