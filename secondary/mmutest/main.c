#include "kernelsdk.h"
#include "gpio.h"
#include "mmu.h"
#include "uart.h"
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
void insupervisormode(void); 
void main(void) {
    uart0_ctl = uart_init(0, 1); 
    print_str("hello!!!\n");
    uint64_t mcor_csr = read_csr(MCOR); 
    uint64_t mhcr_csr = read_csr(MHCR); 
    uint64_t mhint_csr = read_csr(MHINT); 
    printf("the current MCOR csr is:  0b%032b\nthe current MHCR csr is:  0b%032b\nthe current MHINT csr is: 0b%032b\n\n", mcor_csr, mhcr_csr, mhint_csr); 
    cache_enable_status_set(1, 1, 1, 1, 1, 1, 1); 
    cache_prefetch_status_set(1, 1, DCACHE_PREFETCH2); 
    // must enable clint in machine mode 
    clint_enable_supervisor_interrupt(); 
    plic_enable_supervisor_access(); 
    enable_supervisor_counter_access(); 
    // uintptr_t mtvec = set_machine_interrupt_vector((uintptr_t)&interrupt_vector, INTERRUPT_HANDLER_VECTOR); 
    // uintptr_t mtvec = set_machine_interrupt_vector((uintptr_t)&direct_interrupt_trampoline, INTERRUPT_HANDLER_DIRECT); 
    // enable_all_machine_interrupts(); 
    interrupt_delegate_to_supervisor_mode(0xffff); // delegate all interrupts 
    exception_delegate_to_supervisor_mode(0xffff); // delegate all exceptions 
    print_str("now enter supervisor mode~\n"); 
    // enter supervisor mode now 

    // procedure: pretend that we are in a machine mode environment call context 
    // set the corresponding M Previous Privilege mode to Supervisor, so that mret can enter supervisor mode 
    uintptr_t mstatus = read_csr("mstatus"); 
    mstatus &= (~((uintptr_t)MSTATUS_MPP)); 
    mstatus |= (uintptr_t)(1 << 11); 
    write_csr("mstatus", mstatus); 
    // set the M epc to the destination function we want to run in supervisor mode 
    write_csr("mepc", (uintptr_t)(insupervisormode)); 
    // disable virtual memory 
    write_csr("satp", 0); 
    // set PMP to allow RWX on all addresses! use TOR mode, and 0<=x<0xffffffff addresses are RWX 
    // see: https://linxy.dev/2021/05/26/fix-xv6-boot/
    asm volatile("csrw pmpaddr0, %0" : : "r" (0xffffffffULL));
    asm volatile("csrw pmpcfg0, %0" : : "r" (0x0FULL));

    // asm volatile("mv tp, %0" : : "r"(read_csr("mhartid")) : "tp");

    print_str("about to call mret!\n"); 
    // use mret to jump "back" to supervisor mode 
    asm volatile ("mret":::"memory"); 
    software_reset(); 
}

int array[128]; 
extern char default_interrupt_trampoline;
void insupervisormode(void) {
    print_str("Hello from supervisor mode~\n"); 
    uintptr_t stvec = set_supervisor_interrupt_vector((uintptr_t)&interrupt_vector, INTERRUPT_HANDLER_VECTOR); 
    enable_all_supervisor_interrupts(); 
    printf("stvec = 0x%x\nsie = 0b%b\n\n", read_csr(STVEC), read_csr(SIE)); 
    dev_barrier(); 
    // virtual memory 
    vmem_kernel_init();
    printf("setting satp...\n"); 
    vmem_kernel_set_satp(); 
    dcache_clear_invalidate_all();
    icache_invalidate_all_broadcast(); 
    // asm volatile (
    //     "dcache.ciall\n" 
    //     "icache.ialls\n"
    //     : : : "memory"
    // );
    printf("mmu enabled!\n"); 
    for (int i = 0; i < 128; ++i) {
        array[i] += i+1;
    }
    // for (int i = 0; i < 128; ++i) {
    //     print_hex(array[i]); 
    //     uart_putc(uart0_ctl, ' '); 
    // }
    // printf("\n"); 
    // printf("interrupt_vector = %p\n", &interrupt_vector);
#define TEST_TIMER 1  
#if TEST_TIMER
    print_str("testing timer\n"); 
    dev_barrier();
    // clint_set_supervisor_software_interrupt(1);  // this raises an interrupt immediately 
    print_str("done\n"); 
    dev_barrier(); 
    clint_set_supervisor_timer_interrupt(24*1000000); 
    for (int j = 0; j < 10; ++j) {
        asm volatile ("wfi":::"memory"); 
    }
#endif 
    plic_supervisor_reset(); 
    dev_barrier(); 
    plic_supervisor_enable(); 
    dev_barrier(); 
    printf("plic enabled\n");
    gpio_set_pull(gpio_pe, 16, gpio_pull_up); 
    gpio_set_config(gpio_pe, 16, gpio_config_external_interrupt); 
    gpio_external_interrupt_debounce_set(gpio_pe, 16, 0b000, gpio_interrupt_debounce_LOSC_32KHz); 
    gpio_set_external_interrupt_config(gpio_pe, 16, gpio_interrupt_positive_edge); 
    plic_supervisor_interrupt_enable(91, gpio_has_interrupt, 1); 
    uart_interrupt_enable_set(uart0_ctl, (uint32_t)uart_enable_received_data_available_interrupt); 
    plic_supervisor_interrupt_enable(18, uart_has_interrupt, 1); 
    for (int i = 0; i < 10; ++i) {
        printf("waiting for %d-th interrupt...", i);
        asm volatile ("wfi":::"memory"); 
    }

    extern char __rodata_start;
    print_hex(*(&__rodata_start));
    // according to the riscv virtual memory specification, this will raise a page fault!!! this is a store/AMO page fault 
    *(&__rodata_start) = 0x0; 
    software_reset(); 
    return; 
}























