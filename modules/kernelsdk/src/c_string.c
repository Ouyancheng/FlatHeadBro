#include "c_string.h"

void *memset(void *dst, int c, size_t n) {
    uint8_t *dstbyte = (uint8_t*)dst;
    for (size_t i = 0; i < n; ++i) {
        dstbyte[i] = c;
    }
    return dst;
}

int memcmp(const void *p1, const void *p2, size_t n) {
    const uint8_t *s1 = (const uint8_t*)p1; 
    const uint8_t *s2 = (const uint8_t*)p2;
    for (size_t i = 0; i < n; ++i, ++s1, ++s2) {
        if ((*s1) != (*s2)) {
            return (*s1) - (*s2);
        }
    }
    return 0;
}

void *memmove(void *dst, const void *src, size_t n) {
    const uint8_t *s = src;
    uint8_t *d = dst;
    if (s < d && d < (s + n)) {
        // backward, notice that do not minus n 
        s += n;
        d += n;
        for (size_t i = 0; i < n; ++i) {
            d -= 1;
            s -= 1;
            (*d) = (*s);
        }
    } else {
        for (size_t i = 0; i < n; ++i, ++s, ++d) {
            (*d) = (*s);
        }
    }
    return dst;
}

void *memcpy(void *dst, const void *src, size_t n) {
    return memmove(dst, src, n);
}

int strncmp(const char *s1, const char *s2, size_t n) {
    size_t i; 
    for (i = 0; i < n && (*s1) && (*s1 == *s2); ++i, ++s1, ++s2) {

    }
    if (i == n) {
        return 0; 
    }
    return (*s1) - (*s2); // this also handles early termination 
}

char *strncpy(char *dst, const char *src, size_t n) {
    char *dst_backup = dst; 
    size_t i = 0; 
    for (; i < n && (*src); ++i, ++dst, ++src) {
        (*dst) = (*src); 
    }
    for (; i < n; ++i, ++dst) {
        (*dst) = '\0'; 
    }
    return dst_backup; 
}

size_t strlen(const char *s) {
    size_t n = 0; 
    for (; (*s); ++n, ++s) {

    }
    return n;
}

