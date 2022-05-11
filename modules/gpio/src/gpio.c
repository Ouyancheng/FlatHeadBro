#include "gpio.h"

// function 
void gpio_set_config(enum gpio_port port, int pin, enum gpio_config_state config) {
    struct gpio_control *ctl = (struct gpio_control *)((uintptr_t)GPIO_BASE + (uintptr_t)port * 0x0030UL); 
    int cfg_index = pin / (32/4);
    int cfg_offset = (pin % (32/4)) * 4; 
    uint32_t cfg_reg = ctl->cfg[cfg_index]; 
    cfg_reg &= ~(0b1111 << cfg_offset); 
    cfg_reg |= (((uint32_t)config & 0b1111) << cfg_offset); 
    ctl->cfg[cfg_index] = cfg_reg; 
}
enum gpio_config_state gpio_get_config(enum gpio_port port, int pin) {
    struct gpio_control *ctl = (struct gpio_control *)((uintptr_t)GPIO_BASE + (uintptr_t)port * 0x0030UL); 
    int cfg_index = pin / (32/4);
    int cfg_offset = (pin % (32/4)) * 4; 
    uint32_t cfg_reg = ctl->cfg[cfg_index]; 
    return (enum gpio_config_state)(
        (cfg_reg >> cfg_offset) & 0b1111 
    );
}
// input output 
void gpio_write(enum gpio_port port, int pin, int value) {
    struct gpio_control *ctl = (struct gpio_control *)((uintptr_t)GPIO_BASE + (uintptr_t)port * 0x0030UL); 
    uint32_t data = ctl->dat; 
    if (value > 0) {
        data |= (1 << pin); 
    } else {
        data &= ~(1 << pin); 
    }
    ctl->dat = data; 
}
int gpio_read(enum gpio_port port, int pin) {
    struct gpio_control *ctl = (struct gpio_control *)((uintptr_t)GPIO_BASE + (uintptr_t)port * 0x0030UL); 
    return (ctl->dat >> pin) & 1; 
}
// pull 
void gpio_set_pull(enum gpio_port port, int pin, enum gpio_pull_state pull) {
    struct gpio_control *ctl = (struct gpio_control *)((uintptr_t)GPIO_BASE + (uintptr_t)port * 0x0030UL); 
    int pull_index = pin / (32/2); 
    int pull_offset = (pin % (32/2)) * 2; 
    uint32_t pull_reg = ctl->pull[pull_index]; 
    pull_reg &= ~(0b11 << pull_offset); 
    pull_reg |= (((uint32_t)pull & 0b11) << pull_offset); 
    ctl->pull[pull_index] = pull_reg; 
}
enum gpio_pull_state gpio_get_pull(enum gpio_port port, int pin) {
    struct gpio_control *ctl = (struct gpio_control *)((uintptr_t)GPIO_BASE + (uintptr_t)port * 0x0030UL); 
    int pull_index = pin / (32/2); 
    int pull_offset = (pin % (32/2)) * 2; 
    uint32_t pull_reg = ctl->pull[pull_index]; 
    return (enum gpio_pull_state)((pull_reg >> pull_offset) & 0b11); 
}
// multi-driving 
void gpio_set_multi_driving(enum gpio_port port, int pin, enum gpio_multi_driving_state drive) {
    struct gpio_control *ctl = (struct gpio_control *)((uintptr_t)GPIO_BASE + (uintptr_t)port * 0x0030UL); 
    int drive_index = pin / (32/4); 
    int drive_offset = (pin % (32/4)) * 4; 
    uint32_t drive_reg = ctl->drv[drive_index]; 
    drive_reg &= ~(0b11 << drive_offset); 
    drive_reg |= (((uint32_t)drive & 0b11) << drive_offset); 
    ctl->drv[drive_index] = drive_reg; 
}
enum gpio_multi_driving_state gpio_get_multi_driving(enum gpio_port port, int pin) {
    struct gpio_control *ctl = (struct gpio_control *)((uintptr_t)GPIO_BASE + (uintptr_t)port * 0x0030UL); 
    int drive_index = pin / (32/4); 
    int drive_offset = (pin % (32/4)) * 4; 
    uint32_t drive_reg = ctl->drv[drive_index]; 
    return (enum gpio_multi_driving_state)(
        (drive_reg >> drive_offset) & 0b11 
    );
}



void gpio_set_external_interrupt_config(enum gpio_port port, int pin, enum gpio_interrupt_config_state eint_cfg) {
    // enable interrupt first 
    struct gpio_eint *eint = (struct gpio_eint *)((uintptr_t)GPIO_BASE + 0x0200UL + (uintptr_t)port * 0x0020UL); 
    if (eint_cfg == gpio_interrupt_disable) {
        eint->ctl &= ~(1 << pin); 
        return; 
    }
    eint->ctl |= (1 << pin); 
    
    // then set the config 
    int cfg_index = pin / (32/4); 
    int cfg_offset = (pin % (32/4)) * 4; 
    uint32_t cfg_reg = eint->cfg[cfg_index]; 
    cfg_reg &= ~(0b1111 << cfg_offset); 
    cfg_reg |= ((uint32_t)eint_cfg & 0b111) << cfg_offset; // 0 1 2 3 4 
    eint->cfg[cfg_index] = cfg_reg; 
    return; 
}
enum gpio_interrupt_config_state gpio_get_external_interrupt_config(enum gpio_port port, int pin) {
    struct gpio_eint *eint = (struct gpio_eint *)((uintptr_t)GPIO_BASE + 0x0200UL + (uintptr_t)port * 0x0020UL); 
    if (((eint->ctl >> pin) & 1) == 0) {
        return gpio_interrupt_disable; 
    }
    int cfg_index = pin / (32/4); 
    int cfg_offset = (pin % (32/4)) * 4; 
    uint32_t cfg_reg = eint->cfg[cfg_index]; 
    return (enum gpio_interrupt_config_state)(
        (cfg_reg >> cfg_offset) & 0b1111 
    );
}

int gpio_external_interrupt_irq_pending(enum gpio_port port, int pin) {
    struct gpio_eint *eint = (struct gpio_eint *)((uintptr_t)GPIO_BASE + 0x0200UL + (uintptr_t)port * 0x0020UL); 
    return (eint->status >> pin) & 1; 
}
void gpio_external_interrupt_irq_clear(enum gpio_port port, int pin) {
    struct gpio_eint *eint = (struct gpio_eint *)((uintptr_t)GPIO_BASE + 0x0200UL + (uintptr_t)port * 0x0020UL); 
    eint->status = (1 << pin); 
}

void gpio_external_interrupt_debounce_get(enum gpio_port port, int pin, int *prescaler, enum gpio_interrupt_debounce_clock *clock) {
    struct gpio_eint *eint = (struct gpio_eint *)((uintptr_t)GPIO_BASE + 0x0200UL + (uintptr_t)port * 0x0020UL); 
    uint32_t debounce_reg = eint->deb; 
    (*clock) = (enum gpio_interrupt_debounce_clock)(debounce_reg & 1);
    (*prescaler) = ((debounce_reg >> 4) & 0b111);
}
void gpio_external_interrupt_debounce_set(enum gpio_port port, int pin, int prescaler, enum gpio_interrupt_debounce_clock clock) {
    struct gpio_eint *eint = (struct gpio_eint *)((uintptr_t)GPIO_BASE + 0x0200UL + (uintptr_t)port * 0x0020UL); 
    eint->deb = (((uint32_t)clock) & 1) | ((prescaler & 0b111) << 4); 
}
