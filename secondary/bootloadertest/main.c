#include "riscv64.h"
#include "clock-init.h"
#include "delay.h"
#include "uart.h"
#include "gpio.h"
#include "timer.h"
#if 0
//typedef unsigned int virtual_addr_t;
//typedef unsigned int u32_t;
typedef unsigned int virtual_addr_t;
typedef unsigned int u32_t;

typedef unsigned long uint64_t;
typedef unsigned int uint32_t;
static inline void write32(virtual_addr_t addr, u32_t value)
{
    *((volatile u32_t *)(uintptr_t)(addr)) = value;
}


static inline u32_t read32(virtual_addr_t addr)
{
    return( *((volatile u32_t *)(uintptr_t)(addr)) );
}


void sys_jtag_init(void)
{
    virtual_addr_t addr;
    u32_t val;

    /* Config GPIOF0, GPIOF1, GPIOF3 and GPIOF5 to JTAG mode */
    addr = 0x020000f0 + 0x00;
    val = read32(addr);
    val &= ~(0xf << ((0 & 0x7) << 2));
    val |= ((0x4 & 0xf) << ((0 & 0x7) << 2));
    write32(addr, val);

    val = read32(addr);
    val &= ~(0xf << ((1 & 0x7) << 2));
    val |= ((0x4 & 0xf) << ((1 & 0x7) << 2));
    write32(addr, val);

    val = read32(addr);
    val &= ~(0xf << ((3 & 0x7) << 2));
    val |= ((0x4 & 0xf) << ((3 & 0x7) << 2));
    write32(addr, val);

    val = read32(addr);
    val &= ~(0xf << ((5 & 0x7) << 2));
    val |= ((0x4 & 0xf) << ((5 & 0x7) << 2));
    write32(addr, val);
}

void sys_uart_init(void)
{
    virtual_addr_t addr;
    u32_t val;

    /* Config GPIOB8 and GPIOB9 to txd0 and rxd0 */
    addr = 0x02000030 + 0x04;
    val = read32(addr);
    val &= ~(0xf << ((8 & 0x7) << 2));
    val |= ((0x6 & 0xf) << ((8 & 0x7) << 2));
    write32(addr, val);

    val = read32(addr);
    val &= ~(0xf << ((9 & 0x7) << 2));
    val |= ((0x6 & 0xf) << ((9 & 0x7) << 2));
    write32(addr, val);

    /* Open the clock gate for uart0 */
    addr = 0x0200190c;
    val = read32(addr);
    val |= 1 << 0;
    write32(addr, val);

    /* Deassert uart0 reset */
    addr = 0x0200190c;
    val = read32(addr);
    val |= 1 << 16;
    write32(addr, val);

    /* Config uart0 to 115200-8-1-0 */
    addr = 0x02500000;
    write32(addr + 0x04, 0x0);
    write32(addr + 0x08, 0xf7);
    write32(addr + 0x10, 0x0);
    val = read32(addr + 0x0c);
    val |= (1 << 7);
    write32(addr + 0x0c, val);
    write32(addr + 0x00, 0xd & 0xff);
    write32(addr + 0x04, (0xd >> 8) & 0xff);
    val = read32(addr + 0x0c);
    val &= ~(1 << 7);
    write32(addr + 0x0c, val);
    val = read32(addr + 0x0c);
    val &= ~0x1f;
    val |= (0x3 << 0) | (0 << 2) | (0x0 << 3);
    write32(addr + 0x0c, val);
}

void sys_uart_putc(char c)
{
    virtual_addr_t addr = 0x02500000;

    while((read32(addr + 0x7c) & (0x1 << 1)) == 0);
    write32(addr + 0x00, c);
}
#endif 

void delay_us(uint64_t us) {
    uint64_t current; 
    uint64_t target; 
    current = get_arch_counter(); 
    target = current + us * 24UL; 
    do {
        current = get_arch_counter(); 
    } while (current <= target); 
}
void delay_ms(uint64_t ms) {
    return delay_us(ms * 1000UL); 
}


int main(void)
{
    // sys_clock_init();
    #if 0
    sys_jtag_init();
    sys_uart_init();
    sys_uart_putc('h');
    sys_uart_putc('e');
    sys_uart_putc('l');
    sys_uart_putc('l');
    sys_uart_putc('o');
    #endif 

    // struct uart_control *uart_init(int port, int set_gpio); 
    // void uart_putc(struct uart_control *ctl, char c); 
    // char uart_getc(struct uart_control *ctl); 
    // int uart_has_data(struct uart_control *ctl);

    struct uart_control *ctl = uart_init(0, 1); 
    

    gpio_set_config(gpio_pe, 16, gpio_config_output); 
    int pe16v = 0; 

    int i = 0; 
    while (i < 4) {
        char c = uart_getc(ctl);
        uart_putc(ctl, c);
        // delay_ms(500);
        i += 1;
        pe16v = (!pe16v); 
        gpio_write(gpio_pe, 16, pe16v);
    }

    uart_putc(ctl, 'h');
    uart_putc(ctl, 'e');
    uart_putc(ctl, 'l');
    uart_putc(ctl, 'l');
    uart_putc(ctl, 'o');
    uart_putc(ctl, '\r');
    uart_putc(ctl, '\n');

    // software_reset();
    // delay_ms(1000);

    // while(1);
    // *((volatile uint32_t *)(0x020500a0 + 0x08)) = (0x16aa << 16) | (0x1 << 0);
    return 0;
}
