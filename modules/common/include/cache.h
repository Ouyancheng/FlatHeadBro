#ifndef CACHE_H 
#define CACHE_H 

#include "mcsr-ext.h"
#include "csr-read-write.h"
#define SUPPORT_WRITE_THROUGH 0 
static inline void cache_enable_status_set(
    int icache_enable, 
    int dcache_enable, 
    int use_write_allocate,
    int use_write_back, 
    int return_stack_enable, 
    int branch_predictor_enable, 
    int branch_target_buffer_enable 
) {
    uint64_t mhcr_csr = 0; // read_csr(MHCR); 
    if (icache_enable) {
        mhcr_csr |= MHCR_IE; 
    } else {
        mhcr_csr &= (~MHCR_IE); 
    }
    if (dcache_enable) {
        mhcr_csr |= MHCR_DE; 
    } else {
        mhcr_csr &= (~MHCR_DE); 
    }
    if (use_write_allocate) {
        mhcr_csr |= MHCR_WA; 
    } else {
        mhcr_csr &= (~MHCR_WA); 
    }
#if SUPPORT_WRITE_THROUGH 
    if (use_write_back) {
        mhcr_csr |= MHCR_WB; 
    } else {
        mhcr_csr &= (~MHCR_WB); 
    }
#endif 
    if (return_stack_enable) {
        mhcr_csr |= MHCR_RS; 
    } else {
        mhcr_csr &= (~MHCR_RS); 
    }
    if (branch_predictor_enable) {
        mhcr_csr |= MHCR_BPE; 
    } else {
        mhcr_csr &= (~MHCR_BPE); 
    }
    if (branch_target_buffer_enable) {
        mhcr_csr |= MHCR_BTB; 
    } else {
        mhcr_csr &= (~MHCR_BTB); 
    }
    write_csr(MHCR, mhcr_csr); 
}

static inline void cache_invalidate(
    int select_icache,
    int select_dcache, 
    int invalidate_selected_cache,
    int writeback_dirty_lines, 
    int invalidate_BHT, 
    int invalidate_BTB 
) {
    uint64_t mcor_csr = 0; 
    if (select_icache) {
        mcor_csr |= 0b01;
    }
    if (select_dcache) {
        mcor_csr |= 0b10; 
    }
    if (invalidate_selected_cache) {
        mcor_csr |= MCOR_INV; 
    }
    if (writeback_dirty_lines) {
        mcor_csr |= MCOR_CLR; 
    }
    if (invalidate_BHT) {
        mcor_csr |= MCOR_BHT_INV; 
    }
    if (invalidate_BTB) {
        mcor_csr |= MCOR_BTB_INV; 
    }
    write_csr(MCOR, mcor_csr); 
}
enum num_dcache_prefetch_lines {
    DCACHE_PREFETCH2  = 0b00,
    DCACHE_PREFETCH4  = 0b01,
    DCACHE_PREFETCH8  = 0b10,
    DCACHE_PREFETCH16 = 0b11,
};
static inline void cache_prefetch_status_set(
    int enable_dcache_prefetch, 
    int enable_icache_prefetch, 
    enum num_dcache_prefetch_lines dcache_prefetch_lines 
) {
    uint64_t mhint_csr = 0; 
    if (enable_dcache_prefetch) {
        mhint_csr |= MHINT_DPLD; 
    }
    if (enable_icache_prefetch) {
        mhint_csr |= MHINT_IPLD; 
    }
    mhint_csr |= (((uint64_t)(dcache_prefetch_lines)) << 13); 
    write_csr(MHINT, mhint_csr); 
}
static inline void dcache_clear_invalidate_all(void) {
    asm volatile (
        ".word 0b00000000001100000000000000001011"
        :
        :
        : "memory"
    );
}
static inline void icache_invalidate_all_broadcast(void) {
    asm volatile (
        ".word 0b00000001000100000000000000001011"
        :
        :
        : "memory"
    );
}
#endif 


