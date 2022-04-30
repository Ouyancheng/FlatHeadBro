#include "delay.h"
#include "uart.h"
#include "gpio.h"
#include "reset.h"

void main(void) {
    struct uart_control *ctl = uart_init(0, 1); 
    uart_putc(ctl, 'h');
    uart_putc(ctl, 'e');
    uart_putc(ctl, 'l');
    uart_putc(ctl, 'l');
    uart_putc(ctl, 'o');
    // uart_putc(ctl, '\r');
    uart_putc(ctl, '\n');

    gpio_set_config(gpio_pe, 16, gpio_config_output); 
    int pe16v = 0; 
    int i = 0; 
    while (i < 10) {
        char c = uart_getc(ctl);
        uart_putc(ctl, c);
        i += 1;
        pe16v = (!pe16v); 
        gpio_write(gpio_pe, 16, pe16v);
    }
}


























