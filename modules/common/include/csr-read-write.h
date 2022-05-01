#ifndef CSR_READ_WRITE_H 
#define CSR_READ_WRITE_H 
#include <stdint.h>
#define write_csr(csr, val) \
({ \
    uint64_t v = (uint64_t)val; \
    asm volatile ( \
        "csrw " csr ", %0" \
        : \
        : "rK"(v) \
        : "memory" \
    ); \
})

#define read_csr(csr) \
({ \
    uint64_t v; \
    asm volatile ( \
        "csrr %0, " csr  \
        : "=r"(v) \
        : \
        : "memory" \
    ); \
    v; \
})

#define swap_csr(csr, val) \
({ \
    uint64_t v = (uint64_t)(val); \
    asm volatile ( \
        "csrrw %0, " csr ", %1" \
        : "=r"(v) \
        : "rK" (v) \
        : "memory" \
    ); \
    v; \
})

#define csr_set_bit(csr, bitmask) \
({ \
    uint64_t mask = (uint64_t)(bitmask); \
    asm volatile ( \
        "csrs " csr ", %0" \
        : \
        : "rK"(mask) \
        : "memory" \
    ); \
})

#define csr_clear_bit(csr, bitmask) \
({ \
    uint64_t mask = (uint64_t)(bitmask); \
    asm volatile ( \
        "csrc " csr ", %0" \
        : \
        : "rK"(mask) \
        : "memory" \
    ); \
})

#define csr_read_and_set_bit(csr, bitmask) \
({ \
    uint64_t v = (uint64_t)bitmask; \
    asm volatile ( \
        "csrrs %0, " csr ", %1" \
        : "=r"(v) \
        : "rK"(v) \
        : "memory" \
    ); \
    v; \
})

#define csr_read_and_clear_bit(csr, bitmask) \
({ \
    uint64_t v = (uint64_t)bitmask; \
    asm volatile ( \
        "csrrc %0, " csr ", %1" \
        : "=r"(v) \
        : "rK"(v) \
        : "memory" \
    ); \
    v; \
})

#endif 
