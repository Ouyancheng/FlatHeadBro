#include "reset.h"
#include "delay.h"
#include <stdint.h>

void software_reset(void) {
    // the watchdog timer 
    *((volatile uint32_t *)(0x020500a0 + 0x08)) = (0x16aa << 16) | (0x1 << 0);
    delay_ms(1000); 
}

