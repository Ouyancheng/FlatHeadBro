#ifndef STRING_H 
#define STRING_H 
#include <stdint.h>
#include <stddef.h>
void * memset  (void *dst, int c, size_t n);
int    memcmp  (const void *p1, const void *p2, size_t n);
void * memmove (void *dst, const void *src, size_t n);
void * memcpy  (void *dst, const void *src, size_t n);
int    strncmp (const char *s1, const char *s2, size_t n);
char * strncpy (char *dst, const char *src, size_t n);
size_t strlen  (const char *s) ;

#endif 
