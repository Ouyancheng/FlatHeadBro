#ifndef KERNEL_SDK 
#define KERNEL_SDK 
#include "asm-defs.h"
#include "csr-standard.h" 
#include "mcsr-standard.h" 
#include "scsr-standard.h" 
#include "cache.h"
#include "csr-ext.h"
#include "csr-read-write.h"
#include "delay.h"
#include "fence.h"
#include "get-put.h"
#include "mcsr-ext.h"
#include "reset.h"
#include "scsr-ext.h"
#include "utilities.h"
#include "printf.h"
#include "c_string.h"
#include "crc.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h> 

#define panic(...) panic_at(__FILE__, __LINE__, __func__, __VA_ARGS__)

int panic_at(const char *file, int line, const char *function, const char *format, ...); 

#define unreachable() panic("unreachable code reached!\n")
#define static_assert _Static_assert 
#ifdef NDEBUG 
#define assert(condition) ((void)0)
#else 
// #define assert(condition) ((condition) ? ((void)0) : ((void)panic("assertion " #condition " failed")))
#define assert(condition) ((condition) ? ((void)0) : ((void)panic("assertion failed")))
#endif // ! NDEBUG 
extern uint8_t __attribute__((aligned(4096))) kernel_stack[KERNEL_STACK_SIZE_PER_CORE * CORE_COUNT];  // we only have single core 
#endif 
