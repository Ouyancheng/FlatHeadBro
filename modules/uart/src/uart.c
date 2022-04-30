#include "uart.h"
#include "get-put.h"
#include "gpio.h"
#include "delay.h"
#ifndef CCU_BASE
#define CCU_BASE                  0x02001000
#define CCU_UART_BGR_REG          (CCU_BASE+0x90C)
#endif 
// struct uart_control *uart0_control = (struct uart_control*)(UART0_BASE); 
// struct uart_status  *uart0_status  = (struct uart_status*)(UART0_BASE + 0x007C); 
// struct uart_halt    *uart0_halt    = (struct uart_halt*)(UART0_BASE + 0x00A4); 
// struct uart_dbg     *uart0_dbg     = (struct uart_dbg*)(UART0_BASE + 0x00B0); 
// struct uart_fifo_clock_control *uart0_fifo_clock_control = (struct uart_fifo_clock_control*)(UART0_BASE + 0x00F0); 
// struct uart_rxdma   *uart0_rxdma   = (struct uart_rxdma*)(UART0_BASE + 0x0100); 
struct uart_control *uart0_ctl;
void uart_clock_init(int port) {
    uint32_t uart_bgr = get32(CCU_UART_BGR_REG); 
    uart_bgr &= ~(1<<(CCM_UART_RST_OFFSET + port));
    put32(CCU_UART_BGR_REG, uart_bgr); 
    delay_us(100); 
    uart_bgr |= (1 << (CCM_UART_RST_OFFSET + port));
    put32(CCU_UART_BGR_REG, uart_bgr);

    uart_bgr = get32(CCU_UART_BGR_REG);
    uart_bgr &= ~(1<<(CCM_UART_GATING_OFFSET + port));
    put32(CCU_UART_BGR_REG, uart_bgr);
    delay_us(100);
    uart_bgr |= (1 << (CCM_UART_GATING_OFFSET + port));
    put32(CCU_UART_BGR_REG, uart_bgr);
}
void uart_set_gpio(int port) {
    switch (port) {
    case 0:
        gpio_set_config(gpio_pb, 8, gpio_config_function6); 
        gpio_set_config(gpio_pb, 9, gpio_config_function6); 
        gpio_set_pull(gpio_pb, 8, gpio_pull_up); 
        gpio_set_pull(gpio_pb, 9, gpio_pull_up); 
        break;
    case 1:
        gpio_set_config(gpio_pb, 8, gpio_config_function7); 
        gpio_set_config(gpio_pb, 9, gpio_config_function7); 
        gpio_set_pull(gpio_pb, 8, gpio_pull_up); 
        gpio_set_pull(gpio_pb, 9, gpio_pull_up); 
        break;
    case 2:
        gpio_set_config(gpio_pb, 0, gpio_config_function7); 
        gpio_set_config(gpio_pb, 1, gpio_config_function7); 
        gpio_set_pull(gpio_pb, 0, gpio_pull_up); 
        gpio_set_pull(gpio_pb, 1, gpio_pull_up); 
        break; 
    case 3: 
        gpio_set_config(gpio_pb, 6, gpio_config_function7); 
        gpio_set_config(gpio_pb, 7, gpio_config_function7); 
        gpio_set_pull(gpio_pb, 6, gpio_pull_up); 
        gpio_set_pull(gpio_pb, 7, gpio_pull_up); 
        break; 
    case 4: 
        gpio_set_config(gpio_pb, 2, gpio_config_function7); 
        gpio_set_config(gpio_pb, 3, gpio_config_function7); 
        gpio_set_pull(gpio_pb, 2, gpio_pull_up); 
        gpio_set_pull(gpio_pb, 3, gpio_pull_up); 
        break; 
    case 5:
        gpio_set_config(gpio_pb, 4, gpio_config_function7); 
        gpio_set_config(gpio_pb, 5, gpio_config_function7); 
        gpio_set_pull(gpio_pb, 4, gpio_pull_up); 
        gpio_set_pull(gpio_pb, 5, gpio_pull_up); 
        break;  
    default:
        break;
    }
}
struct uart_control *uart_init(int port, int set_gpio) {
    if (!(port <= 0 && port <= 5)) {
        return (struct uart_control *)0; 
    }
    /// set clock 
    uart_clock_init(port); 

    /// set gpio 
    if (set_gpio) {
        uart_set_gpio(port); 
    }
    
    struct uart_control *ctl = (struct uart_control*)((uintptr_t)UART0_BASE+port*0x400UL); 
    ctl->mcr = 0b11;
    uint32_t uart_clock_divisor = (24000000 + 8 * UART_BAUD) / (16 * UART_BAUD); 
    ctl->lcr |= 0x80;  // enable the divisor latch access bit to set baud 
    ctl->dlh = ((uart_clock_divisor >> 8) & 0xFF);
    ctl->dll = (uart_clock_divisor & 0xFF); 
    ctl->lcr &= (~0x80);  // disable the latch access 
    ctl->lcr = ((PARITY & 0x03) << 3) | ((STOP & 0x01) << 2) | (DLEN & 0x03);  // 8n1 
    ctl->fcr = 0x7; 
    return ctl;

}
void uart_putc(struct uart_control *ctl, char c) {
    while ((ctl->lsr & (1 << 6)) == 0) {} 
    ctl->thr = c; 
}
char uart_getc(struct uart_control *ctl) {
    while ((ctl->lsr & 1) == 0) {} 
    return ctl->rbr; 
}
int uart_has_data(struct uart_control *ctl) {
    return (ctl->lsr & 1); 
}





