#include "mmu.h"
#include "kernelsdk.h"
pagetable_t kernel_pagetable; 

pagetable_entry_t * vmem_get_pte(pagetable_t pagetable, uintptr_t virtual_address, int alloc_on_not_found, int *end_level) {
    if (virtual_address >= VIRTUAL_ADDRESS_END) {
        panic("virtual address %p is out of virtual address range!\n", virtual_address);
    }
    for (int level = PAGE_TABLE_MAXIMUM_LEVELS; level > 0; --level) {
        pagetable_entry_t *pte = &(pagetable[VA_GET_VPN(virtual_address, level)]); 
        if (!(*pte & PTE_V)) { // invalid PTE 
            if (alloc_on_not_found) {
                pagetable_t new_pt = (pagetable_t)pagealloc(); 
                if (!new_pt) { 
                    (*end_level) = level;
                    return NULL; 
                }
                memset(new_pt, 0, PAGESIZE); 
                // (*pte) = ((((uintptr_t)new_pt >> PA_PPN_OFFSET) << PTE_PPN_OFFSET) | PTE_V);
                (*pte) = PA_TO_PTE(new_pt) | PTE_V; 
            } else {
                (*end_level) = level;
                return NULL; 
            }
        } else if ((*pte) & (PTE_R | PTE_W | PTE_X)) {
            // mega pages: 2M / 1G 
            (*end_level) = level; 
            return pte; 
        }
        // pagetable = (pagetable_t)(((*pte) >> PTE_PPN_OFFSET) << PA_PPN_OFFSET); 
        pagetable = (pagetable_t)PTE_GET_PA(*pte); 
    }
    (*end_level) = 0; 
    return &(pagetable[VA_GET_VPN(virtual_address, 0)]); 
}

int vmem_map_range(pagetable_t pagetable, uintptr_t virtual_address, size_t size, uintptr_t physical_address, uint64_t permission) {
    int end_level;
    for (uintptr_t vaddr = ROUND_DOWN(virtual_address, PAGESIZE); 
        vaddr <= ROUND_DOWN(virtual_address + size - 1, PAGESIZE); 
        vaddr += PAGESIZE, physical_address += PAGESIZE
    ) {
        pagetable_entry_t *pte = vmem_get_pte(pagetable, vaddr, 1, &end_level);
        if (!pte) {
            return -1; // run out of kernel memory 
        }
        if (!((*pte) & PTE_V)) {  // brand-new pte 
            (*pte) = ((physical_address >> PA_PPN_OFFSET) << PTE_PPN_OFFSET) | PTE_V | PTE_A | PTE_D | permission; 
        } else { // otherwise it's a remapping 
            panic("remapping virtual address %p to physical address %p, original physical address is %p\n", 
                vaddr, physical_address, (((*pte) >> PTE_PPN_OFFSET) << PA_PPN_OFFSET));
        }
        
    }
    return 0; 
}
extern char __text_start;
extern char __text_end;
extern char __rodata_start; 
extern char __rodata_end;  
extern char __data_start; 
extern char __data_end; 
extern char __kernel_memory_end; 
void vmem_kernel_init(void) {
    pagealloc_init();
    kernel_pagetable = (pagetable_t)pagealloc();
    memset(kernel_pagetable, 0, PAGESIZE); 
    // manually map the first 1GB I/O address 0x00000000 - 0x40000000 
    /// NOTE: if I use the 1GB superpage then the PLIC's SCLAIM register is always reading 0... why is that??? 
    // kernel_pagetable[0] = PTE_V | PTE_A | PTE_D | PTE_R | PTE_W | PTE_X | PTE_G | (UINT64_C(1) << 63); 
    // this alone costs 1*1024*1024*1024 / 4096 / 512 * 4096 = 2097152 bytes / 2MB for the last level page table 
    // (512 last level page tables, 1 second level page table, and one entry for the first level page table) 
    vmem_map_range(kernel_pagetable, 0x0, 0x40000000, 0x0, PTE_R | PTE_W | PTE_X | PTE_G | (UINT64_C(1) << 63)); 
    // printf("mapping kernel.text... __text_start=%p, __text_end=%p\n", &__text_start, &__text_end);
    // map kernel's text section 
    vmem_map_range(kernel_pagetable, 
        (uintptr_t)&__text_start, 
        &__text_end - &__text_start, 
        (uintptr_t)&__text_start, 
        PTE_R | PTE_X
    ); 
    // printf("mapping kernel.rodata... __rodata_start=%p, __rodata_end=%p\n", &__rodata_start, &__rodata_end);
    // map kernel's data section 
    vmem_map_range(kernel_pagetable, 
        (uintptr_t)&__rodata_start, 
        &__rodata_end - &__rodata_start, 
        (uintptr_t)&__rodata_start, 
        PTE_R
    ); 
    // printf("mapping kernel.data... kernel_mem_end - data_start = %x data_start=%p kernel_mem_end=%p\n", 
    //     &__kernel_memory_end - &__data_start,
    //     &__data_start,
    //     &__kernel_memory_end
    // );
    vmem_map_range(kernel_pagetable, 
        (uintptr_t)&__data_start, 
        &__kernel_memory_end - &__data_start, 
        (uintptr_t)&__data_start, 
        PTE_R | PTE_W | PTE_X
    ); // just set everything after the data segment to RWX 
    // map kernel's stack section 
    // included 
    // printf("done mapping!!!\n");
}
void vmem_kernel_set_satp(void) {
    write_csr(SATP, MAKE_SATP(kernel_pagetable)); 
    asm volatile("sfence.vma zero, zero":::"memory");
}
