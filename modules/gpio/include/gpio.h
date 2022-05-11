#ifndef GPIO_H 
#define GPIO_H 

#include <stdint.h> 

/**
 * According to D1 user manual section 9.7.3.1 
 * The GPIO has 88 pins, and they are divided by 6 ports: 
 * PB 13 pins, power 3.3V 
 * PC 8 pins,  power 3.3V/1.8V 
 * PD 23 pins, power 3.3V/1.8V
 * PE 18 pins, power 3.3V/2.8V/1.8V
 * PF 8 pins,  power 3.3V/1.8V 
 * PG 19 pins, power 3.3V/1.8V 
 */
#define GPIO_BASE (0x02000000)
/**
 * 0x0030 - 0x0054 
 */
struct gpio_pb_control {
    volatile uint32_t cfg0;
    volatile uint32_t cfg1; 
    volatile uint32_t reserved[2];  // 0x0038 // 0x003C
    volatile uint32_t dat; 
    volatile uint32_t drv0; 
    volatile uint32_t drv1; 
    volatile uint32_t reserved1[2];  // 0x004C // 0x0050
    volatile uint32_t pull0; 
};

/**
 * 0x0060 
 */
struct gpio_pc_control {
    volatile uint32_t cfg0; 
    volatile uint32_t reserved0[3]; // 0x0064 // 0x0068 // 0x006C 
    volatile uint32_t dat; 
    volatile uint32_t drv0; 
    volatile uint32_t reserved1[3]; // 78,7C,80
    volatile uint32_t pull0; 
}; 

/**
 * 0x0090 
 */
struct gpio_pd_control {
    volatile uint32_t cfg0; 
    volatile uint32_t cfg1; 
    volatile uint32_t cfg2; 
    volatile uint32_t reserved0; // 0x009C 
    volatile uint32_t dat; 
    volatile uint32_t drv0; 
    volatile uint32_t drv1; 
    volatile uint32_t drv2; 
    volatile uint32_t reserved1; // 0x00B0 
    volatile uint32_t pull0; 
    volatile uint32_t pull1; 
};

/**
 * 0x00C0 
 */
struct gpio_pe_control {
    volatile uint32_t cfg0;
    volatile uint32_t cfg1; 
    volatile uint32_t reserved[2];  // 0x00C8 // 0x00CC
    volatile uint32_t dat; 
    volatile uint32_t drv0; 
    volatile uint32_t drv1; 
    volatile uint32_t reserved1[2];  // 0x00DC // 0x00E0
    volatile uint32_t pull0; 
};

/**
 * 0x00F0 
 */
struct gpio_pf_control {
    volatile uint32_t cfg0; 
    volatile uint32_t reserved0[3]; // 0x00f4 // 0x00f8 // 0x00fC 
    volatile uint32_t dat; 
    volatile uint32_t drv0; 
    volatile uint32_t reserved1[3]; // 108,10C,110
    volatile uint32_t pull0; 
}; 

/**
 * 0x0120 
 */
struct gpio_pg_control {
    volatile uint32_t cfg0; 
    volatile uint32_t cfg1; 
    volatile uint32_t reserved0[2]; // 0x0128, 0x012C 
    volatile uint32_t dat; 
    volatile uint32_t drv0; 
    volatile uint32_t drv1; 
    volatile uint32_t reserved1; // 0x013C 
    volatile uint32_t drv3;  // it's named drv3 in the user manual 
    volatile uint32_t pull0; 
};

/**
 * PB 0x0030
 * PC 0x0060
 * PD 0x0090 
 * PE 0x00C0 
 * PF 0x00F0 
 * PG 0x0120 
 */
struct gpio_control {
    volatile uint32_t cfg[3]; 
    volatile uint32_t reserved; 
    volatile uint32_t dat;
    volatile uint32_t drv[4]; 
    volatile uint32_t pull[2]; 
};
// _Static_assert(sizeof(struct gpio_control) == sizeof(struct gpio_pd_control), "gpio control size not matching"); 

/**
 * 0x0220 
 */
struct gpio_pb_eint {
    volatile uint32_t cfg0; 
    volatile uint32_t reserved0[3]; // 0224,0228,022C 
    volatile uint32_t ctl; 
    volatile uint32_t status; 
    volatile uint32_t deb; // debounce register 
};

/**
 * 0x0240 
 */
struct gpio_pc_eint {
    volatile uint32_t cfg0; 
    volatile uint32_t reserved0[3]; // 0244,0248,024C 
    volatile uint32_t ctl; 
    volatile uint32_t status; 
    volatile uint32_t deb; 
};

/**
 * 0x0260 
 */
struct gpio_pd_eint {
    volatile uint32_t cfg0; 
    volatile uint32_t cfg1; 
    volatile uint32_t cfg2; 
    volatile uint32_t reserved0; 
    volatile uint32_t ctl; 
    volatile uint32_t status; 
    volatile uint32_t deb; 
};

/**
 * 0x0280 
 */
struct gpio_pe_eint {
    volatile uint32_t cfg0; 
    volatile uint32_t cfg1; 
    volatile uint32_t reserved0[2]; 
    volatile uint32_t ctl; 
    volatile uint32_t status; 
    volatile uint32_t deb; 
};

/**
 * 0x02A0 
 */
struct gpio_pf_eint {
    volatile uint32_t cfg0; 
    volatile uint32_t reserved0[3]; 
    volatile uint32_t ctl; 
    volatile uint32_t status; 
    volatile uint32_t deb; 
};

/**
 * 0x02C0 
 */
struct gpio_pg_eint {
    volatile uint32_t cfg0; 
    volatile uint32_t cfg1; 
    volatile uint32_t reserved0[2]; 
    volatile uint32_t ctl; 
    volatile uint32_t status; 
    volatile uint32_t deb; 
};

/**
 * PB 0x0220
 * PC 0x0240 
 * PD 0x0260 
 * PE 0x0280 
 * PF 0x02A0 
 * PG 0x02C0 
 */
struct gpio_eint {
    volatile uint32_t cfg[3]; 
    volatile uint32_t reserved; 
    volatile uint32_t ctl; 
    volatile uint32_t status; 
    volatile uint32_t deb; 
};
// _Static_assert(sizeof(struct gpio_eint) == sizeof(struct gpio_pd_eint), "gpio eint size not matching"); 

/**
 * 0x0340 
 */
struct gpio_pio_power {
    volatile uint32_t mod_sel; 
    volatile uint32_t ms_ctl; 
    volatile uint32_t val; 
    volatile uint32_t reserved;
    volatile uint32_t vol_sel_ctl; 
};

enum gpio_pull_state {
    gpio_pull_disable = 0b00, 
    gpio_pull_up = 0b01, 
    gpio_pull_down = 0b10, 
    gpio_pull_reserved = 0b11 
};

enum gpio_multi_driving_state {
    gpio_multi_driving_level0 = 0b00, 
    gpio_multi_driving_level1 = 0b01, 
    gpio_multi_driving_level2 = 0b10, 
    gpio_multi_driving_level3 = 0b11, 
}; 

enum gpio_config_state {
    gpio_config_input = 0b0000, 
    gpio_config_output = 0b0001, 
    gpio_config_function2 = 0b0010, 
    gpio_config_function3 = 0b0011, 
    gpio_config_function4 = 0b0100, 
    gpio_config_function5 = 0b0101, 
    gpio_config_function6 = 0b0110, 
    gpio_config_function7 = 0b0111, 
    gpio_config_function8 = 0b1000, 
    gpio_config_function14 = 0b1110, 
    gpio_config_external_interrupt = 0b1110, 
    gpio_config_io_diable = 0b1111 
};

enum gpio_port {
    gpio_pb = 1, 
    gpio_pc = 2, 
    gpio_pd = 3, 
    gpio_pe = 4, 
    gpio_pf = 5, 
    gpio_pg = 6 
};
// function 
void gpio_set_config(enum gpio_port port, int pin, enum gpio_config_state config); 
enum gpio_config_state gpio_get_config(enum gpio_port port, int pin); 
// input output 
void gpio_write(enum gpio_port port, int pin, int value); 
int gpio_read(enum gpio_port port, int pin); 
// pull 
void gpio_set_pull(enum gpio_port port, int pin, enum gpio_pull_state pull); 
enum gpio_pull_state gpio_get_pull(enum gpio_port port, int pin); 
// multi-driving, the multi-driving register in pg is somehow confuing, should I use drv[2] or drv[3]? 
void gpio_set_multi_driving(enum gpio_port port, int pin, enum gpio_multi_driving_state drive); 
enum gpio_multi_driving_state gpio_get_multi_driving(enum gpio_port port, int pin); 


enum gpio_interrupt_config_state {
    gpio_interrupt_positive_edge = 0x0, 
    gpio_interrupt_negative_edge = 0x1, 
    gpio_interrupt_high_level = 0x2, 
    gpio_interrupt_low_level = 0x3, 
    gpio_interrupt_double_edge = 0x4, 

    gpio_interrupt_disable = 0xF
};

// config interrupt 
void gpio_set_external_interrupt_config(enum gpio_port port, int pin, enum gpio_interrupt_config_state eint_cfg); 
enum gpio_interrupt_config_state gpio_get_external_interrupt_config(enum gpio_port port, int pin); 
// interrupt status 
int gpio_external_interrupt_irq_pending(enum gpio_port port, int pin); 
void gpio_external_interrupt_irq_clear(enum gpio_port port, int pin); 
enum gpio_interrupt_debounce_clock {
    gpio_interrupt_debounce_LOSC_32KHz = 0, 
    gpio_interrupt_debounce_HOSC_24MHz = 1, 
};
/** The selected clock source is divided by 2^prescaler, the range of prescaler is 0 to 7 */
void gpio_external_interrupt_debounce_get(enum gpio_port port, int pin, int *prescaler, enum gpio_interrupt_debounce_clock *clock);
/** The selected clock source is divided by 2^prescaler, the range of prescaler is 0 to 7 */
void gpio_external_interrupt_debounce_set(enum gpio_port port, int pin, int prescaler, enum gpio_interrupt_debounce_clock clock);  
#endif 

