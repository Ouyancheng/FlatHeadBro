#include "delay.h"
#include "uart.h"
#include "gpio.h"
#include "reset.h"
#include "fence.h"
#include "printf.h"
#include "cache.h"
#include "interrupt.h"
/** get the hardware core id, should be 0 as D1 only has single core */
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
int array[1048576];
extern char __data_start;
void main(void) {
    uart0_ctl = uart_init(0, 1); 
    print_str("hello!!!\n");
    uintptr_t gp; 
    asm volatile (
        "mv %0, gp"
        : "=r"(gp)
        :
        : 
    ); 
    printf("gp register = %x\n__data_start = %x\n+0x800=%x\n", gp, &__data_start, &__data_start+0x800); 
    uint64_t current_us = get_current_time_us(); 
    uint64_t end_us = 0; 
    for (int i = 0; i < 1048576; ++i) {
        array[i] = array[i] + 1; 
    }
    end_us = get_current_time_us(); 
    printf("dcache-disabled: writing 1M integers takes %d us\n", end_us - current_us); 
    dev_barrier();
    
    cache_enable_status_set(1, 1, 0, 1, 1, 1, 1); 
    cache_prefetch_status_set(1, 1, DCACHE_PREFETCH4); 
    cache_invalidate(1, 1, 1, 1, 1, 1); 
    delay_cycles(5000); // we technically don't need this 
    current_us = get_current_time_us(); 
    end_us = 0; 
    for (int i = 0; i < 1048576; ++i) {
        array[i] = array[i] + 2; 
    }
    end_us = get_current_time_us(); 
    printf("dcache-enabled: writing 1M integers takes %d us\n", end_us - current_us); 
    dev_barrier(); 
    /*
    clang: 
    dcache-disabled: writing 1M integers takes 211395 us
    dcache-enabled: writing 1M integers takes 8765 us
    */
    /*
    T-Head Semi's riscv64-unknown-elf-gcc: 
    dcache-disabled: writing 1M integers takes 216301 us
    dcache-enabled: writing 1M integers takes 10701 us
    */
    // wait... why the ``optimized'' gcc's slower than the generic clang??? 

    // print_hex(get_mhartid()); 
    // uart_putc(uart0_ctl, '\n'); 
    // printf("%d %x %b %s\n", 123, 123, 123, "hello world!!! testing printf!!!"); 
    printf("the current thread id is: %b\n", get_mhartid()); 

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
    // delay_ms(1000);
    clint_enable(); 
    dev_barrier();
    // clint_set_machine_software_interrupt(1); 
    dev_barrier(); 
    while (1) {
        clint_set_machine_timer_interrupt(24*1000); 
        ////////// Something happens, we will figure this out later... ////////// 
        // clint_set_supervisor_timer_interrupt(24*1000); 
        delay_ms(2000); 
    }
    

#ifdef ECHO_TEST
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
#endif 
    return; 
}


























