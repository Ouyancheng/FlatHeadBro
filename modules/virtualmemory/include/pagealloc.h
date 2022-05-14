#ifndef PAGEALLOC_H
#define PAGEALLOC_H 
#define PAGESIZE 4096 

void pagealloc_init(void); 
void pagefree(void *page); 
/// allocate a page and return its base address, currently only supports 4K page 
void *pagealloc(void); 
#endif 
