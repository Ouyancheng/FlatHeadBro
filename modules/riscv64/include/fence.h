#ifndef FENCE_H 
#define FENCE_H 
// #define USE_BUILTIN_SYNC 1 
static inline void dev_barrier(void) {
#if 1
    asm volatile (
        "fence iorw, iorw"
        :
        :
        : "memory"
    ); 
#else 
    // this intrinsic inserts a fence rw, rw instruction in clang 
    // but inserts a fence (iorw, iorw) instruction in gcc 
    // this is somehow inconsistent! 
    // this is not enough! 
    __sync_synchronize();  
#endif 
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
