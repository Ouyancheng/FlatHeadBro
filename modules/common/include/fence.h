#ifndef FENCE_H 
#define FENCE_H 

static inline void dev_barrier(void) {
    asm volatile (
        "fence iorw, iorw"
        :
        :
        : "memory"
    ); 
}

static inline void dev_read_barrier(void) {
    asm volatile (
        "fence ir, ir"
        :
        :
        : "memory"
    ); 
}

static inline void dev_write_barrier(void) {
    asm volatile (
        "fence ow, ow"
        :
        :
        : "memory"
    ); 
}

static inline void mem_barrier(void) {
    asm volatile (
        "fence rw, rw"
        :
        :
        : "memory"
    ); 
}

static inline void mem_read_barrier(void) {
    asm volatile (
        "fence r, r"
        :
        :
        : "memory"
    ); 
}

static inline void mem_write_barrier(void) {
    asm volatile (
        "fence w, w"
        :
        :
        : "memory"
    ); 
}

#endif 
