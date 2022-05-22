#ifndef UART_H
#define UART_H

#include <stdint.h>

#define UART0_BASE 0x02500000
#define UART1_BASE 0x02500400
#define UART2_BASE 0x02500800
#define UART3_BASE 0x02500C00
#define UART4_BASE 0x02501000
#define UART5_BASE 0x02501400

/**
 * 0x0000 - 0x001C 
 */
struct uart_control {
    volatile uint32_t rbr;  // receive buffer register 
    volatile uint32_t ier;  // interrupt enable register 
    volatile uint32_t fcr;  // fifo control register 
    volatile uint32_t lcr;  // line control register 
    volatile uint32_t mcr;  // modem control register 
    volatile uint32_t lsr;  // line status register 
    volatile uint32_t msr;  // modem status register 
    volatile uint32_t sch;  // scratch register 
};
// transmit holding register 
#define thr rbr  
// divisor latch low register 
#define dll rbr
// divisor latch high register 
#define dlh ier 
// interrupt identity register, NOTE: iir is all read, fcr is all write, so no conflict  
#define iir fcr

// /**
//  * 0x007C
//  */
// struct uart_status {
//     volatile uint32_t usr;  // uart status register 
// };

/**
 * 0x007C - 0x0080 - 0x008C 
 */
struct uart_status {
    volatile uint32_t usr;  // uart status register 
    volatile uint32_t tfl;  // transmit fifo level register 
    volatile uint32_t rfl;  // receive fifo level register 
    volatile uint32_t hsk;  // DMA handshake configuration register 
    volatile uint32_t dma_req_en;  // DMA request enable register 
};
/**
 * 0x00A4 
 */
struct uart_halt {
    volatile uint32_t halt;  // uart halt tx register 
};
/**
 * 0x00B0 - 0x00B4
 */
struct uart_dbg {
    volatile uint32_t dbg_dll;  // uart debug dll register 
    volatile uint32_t dbg_dlh;  // uart debug dlh register 
};
/**
 * 0x00F0
 */
struct uart_fifo_clock_control {
    volatile uint32_t fcc;  // uart fifo clock control register 
};
// _Static_assert(sizeof(struct uart_fifo_clock_control) == 4, "uart address misaligned"); 
/**
 * 0x0100 - 0x0138 
 */
struct uart_rxdma {
    volatile uint32_t ctrl;    // uart rxdma control register 
    volatile uint32_t str;     // uart rxdma start register 
    volatile uint32_t sta;     // uart rxdma status register 
    volatile uint32_t lmt;     // uart rxdma limit register 
    volatile uint32_t saddrl;  // uart rxdma buffer start address low register 
    volatile uint32_t saddrh;  // uart rxdma buffer start address high register 
    volatile uint32_t bl;      // uart rxdma buffer length register 
    volatile uint32_t reserved;  // 0x011C 
    volatile uint32_t ie;      // uart rxdma interrupt enable register 
    volatile uint32_t is;      // uart rxdma interrupt status register 
    volatile uint32_t waddrl;  // uart rxdma write address low register 
    volatile uint32_t waddrh;  // uart rxdma write address high register 
    volatile uint32_t raddrl;  // uart rxdma read address low register 
    volatile uint32_t raddrh;  // uart rxdma read address high register 
    volatile uint32_t dcnt;    // uart rxdma data count register 
};

// _Static_assert(sizeof(struct uart_rxdma) == (0x0138 - 0x0100 + sizeof(uint32_t))); 


#define CCM_UART_RST_OFFSET       (16)
#define CCM_UART_GATING_OFFSET    (0)
#define CCM_UART_ADDR_OFFSET      (0x400)

/* Baud rate for UART,Compute the divisor factor */
#define   UART_BAUD    115200

/* UART Line Control Parameter */
/* Parity: 0,2 - no parity; 1 - odd parity; 3 - even parity */
#define   PARITY       0 
/* Number of Stop Bit: 0 - 1bit; 1 - 2(or 1.5)bits */
#define   STOP         0
/* Data Length: 0 - 5bits; 1 - 6bits; 2 - 7bits; 3 - 8bit */
#define   DLEN         3 

struct uart_control *uart_init(int port, int set_gpio); 
void uart_putc(struct uart_control *ctl, char c); 
char uart_getc(struct uart_control *ctl); 
int uart_has_data(struct uart_control *ctl);

enum uart_interrupt_enable {
    uart_enable_received_data_available_interrupt         = 1, 
    uart_enable_transmit_holding_register_empty_interrupt = 1 << 1, 
    uart_enable_receiver_line_status_interrupt            = 1 << 2, 
    uart_enable_modem_status_interrupt                    = 1 << 3, 
    uart_enable_RS485_interrupt                           = 1 << 4, 
    uart_enable_programmable_THRE_interrupt               = 1 << 7 
};

void uart_interrupt_enable_set(struct uart_control *ctl, uint32_t interrupt_enable_register); 
uint32_t uart_interrupt_enable_get(struct uart_control *ctl); 
/** 
 * the interrupt id for the uart, priority sorted from high to low, with the except of none which has no priority 
 * the interrupt will be cleared by performing specific actions on uart 
 * see D1 user manual 9.2.6.6 
 */
enum uart_interrupt_id {
    uart_interrupt_id_none                            = 0b0001, 
    uart_interrupt_id_receiver_line_status            = 0b0110, 
    uart_interrupt_id_RS485                           = 0b0011, 
    uart_interrupt_id_received_data_available         = 0b0100, 
    uart_interrupt_id_character_timeout               = 0b1100, 
    uart_interrupt_id_transmit_holding_register_empty = 0b0010, 
    uart_interrupt_id_modem_status                    = 0b0000, 
    uart_interrupt_id_busy                            = 0b0111 
};
/** reads the interrupt identity register, note: the interrupt identity register is read only */
void uart_get_interrupt_identity(struct uart_control *ctl, int *fifo_enabled, enum uart_interrupt_id *interrupt_id); 
extern struct uart_control *uart0_ctl; 
#endif 





