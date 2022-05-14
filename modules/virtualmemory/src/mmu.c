#include "mmu.h"
#include "kernelsdk.h"
pagetable_t kernel_pagetable; 

pagetable_entry_t * get_pte(pagetable_t pagetable, uintptr_t virtual_address, int alloc_on_not_found, enum pagesize *psize) {
    if (virtual_address >= VIRTUAL_ADDRESS_END) {
        panic("virtual address %p is out of Sv39 virtual address range!\n", virtual_address);
    }
    for (int level = 2; level > 0; --level) {
        pagetable_entry_t *pte = &(pagetable[VA_GET_VPN(virtual_address, level)]); 
        if (!(*pte & PTE_V)) {
            // invalid PTE 
            if (alloc_on_not_found) {
                pagetable_t *new_pt = (pagetable_t*)pagealloc(); 
                if (!new_pt) {
                    return NULL; 
                }
                memset(new_pt, 0, PAGESIZE); 
                (*pte) = ((((uintptr_t)new_pt >> PA_PPN_OFFSET) << PTE_PPN_OFFSET) | PTE_V);
            } else {
                return NULL; 
            }
        } else if ((*pte) & (PTE_R | PTE_W | PTE_X)) {
            // mega pages: 2M / 1G 
            if (level == 2) {
                *psize = pagesize_1G; 
            } else {
                assert(level == 1); 
                *psize = pagesize_2M;
            }
            return pte; 
        }
        pagetable = (pagetable_t)(((*pte) >> PTE_PPN_OFFSET) << PA_PPN_OFFSET); 
    }
    (*psize) = pagesize_4K; 
    return &(pagetable[VA_GET_VPN(virtual_address, 0)]); 
}


