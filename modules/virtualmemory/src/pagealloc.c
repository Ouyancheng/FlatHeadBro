#include "pagealloc.h"
#include "kernelsdk.h" 
struct page_freelist_node {
    struct page_freelist_node *next; 
};
struct kernel_memory {
    // need a lock if multiprocessing 
    struct page_freelist_node *page_freelist; 
} kmem; 
extern char __kernel_memory_end; 
extern char __kernel_image_end; 
void pagealloc_init(void) {
    char *pages_start = &__kernel_image_end; 
    char *pages_end = &__kernel_memory_end;
    if ((uintptr_t)pages_start % PAGESIZE != 0 || (uintptr_t)pages_end % PAGESIZE != 0) {
        panic("invalid start and end of pages: page_start = %p, pages_end = %p\n", pages_start, pages_end); 
    }
    // lock 
    // build up the page link list 
    struct page_freelist_node *pn; 
    kmem.page_freelist = NULL; 
    for (char *p = pages_start; p < pages_end; p += PAGESIZE) {
        pn = (struct page_freelist_node *)p; 
        pn->next = kmem.page_freelist; 
        kmem.page_freelist = pn; 
    }
    // unlock 
}

void pagefree(void *page) {
    if ((uintptr_t)page % PAGESIZE != 0 || 
        (uintptr_t)page <  (uintptr_t)&__kernel_image_end || 
        (uintptr_t)page >= (uintptr_t)&__kernel_memory_end
    ) {
        panic("pagefree: page %p is not a valid page!\n", page); 
    }
    struct page_freelist_node *freenode = (struct page_freelist_node*)page; 
    // lock 
    freenode->next = kmem.page_freelist;
    kmem.page_freelist = freenode; 
    // unlock 
}


void *pagealloc(void) {
    // lock 
    struct page_freelist_node *freenode = kmem.page_freelist; 
    if (!freenode) {
        // unlock 
        return (void*)freenode; 
    }
    kmem.page_freelist = freenode->next; 
    // unlock 
    freenode->next = NULL; 
    return (void*)freenode; 
}

