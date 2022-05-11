#include "delay.h"
#include "uart.h"
#include "gpio.h"
#include "reset.h"
#include "fence.h"
#include "printf.h"
#include "cache.h"
#include "interrupt.h"
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
void gpio_has_interrupt(void) {
    gpio_external_interrupt_irq_clear(gpio_pe, 16);  // note: clear first! 
    print_str("gpio has interrupt\n"); 
    print_hex(gpio_read(gpio_pe, 16));
    uart_putc(uart0_ctl, '\n'); 
}
void main(void) {
    uart0_ctl = uart_init(0, 1); 
    print_str("hello!!!\n");
    uint64_t mcor_csr = read_csr(MCOR); 
    uint64_t mhcr_csr = read_csr(MHCR); 
    uint64_t mhint_csr = read_csr(MHINT); 
    printf("the current MCOR csr is:  0b%032b\nthe current MHCR csr is:  0b%032b\nthe current MHINT csr is: 0b%032b\n\n", mcor_csr, mhcr_csr, mhint_csr); 
    uintptr_t mtvec = set_interrupt_vector((uintptr_t)&interrupt_vector, INTERRUPT_HANDLER_VECTOR); 
    // uintptr_t mtvec = set_interrupt_vector((uintptr_t)&direct_interrupt_trampoline, INTERRUPT_HANDLER_DIRECT); 
    uintptr_t mie = enable_all_interrupts(); 
    // printf("mtvec = 0b%b\nmie = 0b%b\n\n", mtvec, mie); 
    dev_barrier(); 
#if TEST_ECALL
    asm volatile (
        "ecall" : : : "memory"
    );

#endif 

#if TEST_TIMER
    clint_enable(); 
    dev_barrier();
    // clint_set_machine_software_interrupt(1);  // this raises an interrupt immediately 
    dev_barrier(); 
    while (1) {
        clint_set_machine_timer_interrupt(24*1000); 
        ////////// we didn't delegate this to supervisor mode... ////////// 
        // clint_set_supervisor_timer_interrupt(24*1000); 
        delay_ms(2000); 
    }
#endif 
    plic_reset(); 
    dev_barrier(); 
    plic_enable(); 
    dev_barrier(); 
    gpio_set_config(gpio_pe, 16, gpio_config_external_interrupt); 
    gpio_external_interrupt_debounce_set(gpio_pe, 16, 0b000, gpio_interrupt_debounce_LOSC_32KHz); 
    gpio_set_external_interrupt_config(gpio_pe, 16, gpio_interrupt_positive_edge); 
    plic_interrupt_enable(91, gpio_has_interrupt, 1); 
    int i = 10;
    while (i --> 0) {
        // print_str("a");
        asm volatile ("wfi":::"memory"); 
    }
    return; 
}


























