#include <stdint.h>
#include "dram-init.h"
#include "delay.h"
extern int init_DRAM(int type, const struct ddr3_param_t *param);

int sys_dram_init(const struct ddr3_param_t *param) {
    return init_DRAM(0, param);
}
void sdelay(unsigned long us) {
    uint64_t t1 = get_arch_counter();
    uint64_t t2 = t1 + us * 24;
    do { t1 = get_arch_counter(); } while(t2 >= t1);
}







