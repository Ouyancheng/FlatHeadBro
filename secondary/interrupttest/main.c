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
void uart_has_interrupt(void) {
    char c = uart_getc(uart0_ctl); 
    print_str("uart received: "); 
    uart_putc(uart0_ctl, c); 
    uart_putc(uart0_ctl, '\n'); 
}
void main(void) {
    uart0_ctl = uart_init(0, 1); 
    print_str("hello!!!\n");
    uint64_t mcor_csr = read_csr(MCOR); 
    uint64_t mhcr_csr = read_csr(MHCR); 
    uint64_t mhint_csr = read_csr(MHINT); 
    printf("the current MCOR csr is:  0b%032b\nthe current MHCR csr is:  0b%032b\nthe current MHINT csr is: 0b%032b\n\n", mcor_csr, mhcr_csr, mhint_csr); 
    uintptr_t mtvec = set_machine_interrupt_vector((uintptr_t)&interrupt_vector, INTERRUPT_HANDLER_VECTOR); 
    // uintptr_t mtvec = set_machine_interrupt_vector((uintptr_t)&direct_interrupt_trampoline, INTERRUPT_HANDLER_DIRECT); 
    uintptr_t mie = enable_all_machine_interrupts(); 
    // printf("mtvec = 0b%b\nmie = 0b%b\n\n", mtvec, mie); 
    dev_barrier(); 
    // print_str("detecting tselect\n"); 
    // for (int i = 0; i <= 3; ++i) {
    //     write_csr("tselect", i);
    //     int aa = read_csr("tselect");
    //     if (aa != i) {
    //         printf("tselect up to %d\n", i);
    //         break;
    //     } 
    //     uint64_t tdata1 = read_csr("tdata1"); 
    //     uint64_t type = tdata1 >> 60;
    //     if (type == 0) {
    //         printf("tselect up to %d\n", i);
    //         break;
    //     }
    //     // printf("i=%d tinfo = 0b%b\n", i, read_csr("tinfo")); 
    // }
    // printf("tselect = %x\n", read_csr("tselect"));
    ////////// No tselect is accessable in M-mode!!!!!! 
    // printf("tcontrol = %x\n", read_csr("0x7a5"));
    ////////// tcontrol is not accessable in M-mode!!!!!! 
    // print_hex(read_csr("0x7aa"));
#define TEST_ECALL 0 
#define TEST_TIMER 1 
#if TEST_ECALL
    asm volatile (
        "ecall" : : : "memory"
    );
#endif 
#if TEST_TIMER
    // clint_enable(); 
    // clint_enable_supervisor_interrupt(); // this is not needed for machine mode 
    dev_barrier();
    // clint_set_machine_software_interrupt(1);  // this raises an interrupt immediately 
    dev_barrier(); 
    int j = 3; 
    while (j --> 0) {
        clint_set_machine_timer_interrupt(24*1000); 
        ////////// we didn't delegate this to supervisor mode... ////////// 
        // clint_set_supervisor_timer_interrupt(24*1000); 
        delay_ms(2000); 
    }
#endif 
    plic_machine_reset(); 
    dev_barrier(); 
    plic_machine_enable(); 
    dev_barrier(); 
    gpio_set_config(gpio_pe, 16, gpio_config_external_interrupt); 
    gpio_external_interrupt_debounce_set(gpio_pe, 16, 0b000, gpio_interrupt_debounce_LOSC_32KHz); 
    gpio_set_external_interrupt_config(gpio_pe, 16, gpio_interrupt_positive_edge); 
    plic_machine_interrupt_enable(91, gpio_has_interrupt, 1); 
    uart_interrupt_enable_set(uart0_ctl, (uint32_t)uart_enable_received_data_available_interrupt); 
    plic_machine_interrupt_enable(18, uart_has_interrupt, 1); 
    int i = 10;
    while (i --> 0) {
        // print_str("a");
        asm volatile ("wfi":::"memory"); 
    }
    return; 
}


























