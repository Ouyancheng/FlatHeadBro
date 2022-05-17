#ifndef MMU_H 
#define MMU_H 

#include <stdint.h>
#include <stddef.h>
#include "pagealloc.h"
/**
 * The MMU of C906 is compatible with RISC-V Sv39 virtual memory system 
 * functionalities: 
 * 1. address translation: 39-bit virtual address => 40-bit physical address 
 * 2. page protection: RWX privileges 
 * 3. page attributes 
 * 
 * Two level TLB : 
 * 1. I-uTLB (10 entry) + D-uTLB (10 entry) fully associative 
 * 2. jTLB two-way associative 256 indices (512 entries) 
 * configurable 128, 256, 512 entries 
 * NOTE: only S-mode and U-mode should have virtual memory, M-mode should be all physical 
 */

/**
 * MMU registers (in supervisor mode)
 * MMU control register (SMCIR)
 * bit 31 : TLBP query TLB according to SMEH, update SMIR with the TLB index if hit 
 * bit 30 : TLBR read the TLB entry according to SMIR, and update SMEH and SMEL with the value 
 * bit 29 : TLBWI write SMEH, SMEL to the entry specified by SMIR 
 * bit 28 : TLBWR write SMEH, SMEL to the entry specified by Random (random write)
 * bit 27 : TLBASID invalidate all entries those ASID is matched 
 * bit 26 : TLBIALL invalidate all entries 
 * bit 25 : TLBII invalidate the entry specified by SMIR 
 * 
 * bit 15 - 0 : the ASID (16-bit) for TLBASID 
 * 
 * 
 * MMU index register (SMIR)
 * used to index the TLB 
 * bit 31 : P probe failure 1 = miss 0 = hit 
 * bit 30 : Tfatal probe multiple 0 = no multiple match 1 = multiple match 
 * bit 8 - 0 : TLB index 
 * 128 entry configuration: Index[6] = way select, Index[5:0] = set select 
 * 256 entry configuration: Index[7] = way select, Index[6:0] = set select 
 * 512 entry configuration: Index[8] = way select, Index[7:0] = set select 
 * 
 * 
 * MMU EntryHi register (SMEH)
 * bit 63 - 46 : reserved 
 * bit 45 - 19 : virtual page number (27 bits) note: 27+12 = 39 bits virtual address,
 *  updated by hardware on page fault/TLB read, pre-written by software on TLB write 
 * bit 18, 17, 16 : page size, one-hot vector: bit 16: 4K, bit 17: 2M, bit 18: 1G 
 * bit 15 - 0 : the ASID address space ID, used to distinguish different processes, 
 *  updated by hardware on TLB read, pre-written by software on TLB write 
 * 
 * 
 * MMU EntryLo register (SMEL) corresponding to a Sv39 page table entry 
 * bit 63 : SO strong order 0 = not strong order (for normal memory, default), 1 = strong order (for devices)
 * bit 62 : C cachable 0 = noncacheable 1 = cacheable, default is 0 
 * bit 61 : B bufferable 0 = nonbufferable 1 = bufferable, default is 0 
 * bit 60 : reserved 
 * bit 59 : Sec / T trustable not used in C906, default is 1 
 * 
 * 
 * bit 37 - 10 : physical page number (28 bits), note: 28 + 12 = 40 bits physical address 
 * bit 9, 8 : RSW reserved for software 
 * bit 7 : D dirty bit / is writable reset to 0 
 * bit 6 : A accessed bit 1 = accessable 0 = not accessable reset to 0 
 * bit 5 : G global bit whether the page is shared by multiple processes reset to 0 
 * bit 4 : U user accessable reset to 0 
 * bit 3 : X executable 
 * bit 2 : W writable 
 * bit 1 : R readable 
 * 000 = pointer to the next level of page table 
 * 010, 110 = reserved for future use 
 * the rest of the combinations are valid 
 * bit 0 : V valid bit reset to 0 
 * 
 * 
 * MMU address translation and protection register (SATP) Sv39 standard 
 * bit 63 - 60 : Mode translation mode, C906 only suppords Sv39 mode (value=8) and MMU off (value=0)
 * bit 59 - 44 : ASID 
 * bit 43 - 28 : - 
 * bit 27 - 0 : PPN of the root page table 
 */



/*
A virtual address va is translated into a physical address pa as follows:
1. Let a be satp.ppn * PAGESIZE, and let i = LEVELS − 1. 
    (For Sv32, PAGESIZE=2^12 and LEVELS=2.)
2. Let pte be the value of the PTE at address a + va.vpn[i] * PTESIZE. 
    (For Sv32, PTESIZE=4.) 
    If accessing pte violates a PMA or PMP check, raise an access exception.
3. If pte.v = 0, or if pte.r = 0 and pte.w = 1, stop and raise a page-fault exception.
4. Otherwise, the PTE is valid. If pte.r = 1 or pte.x = 1, go to step 5. 
    Otherwise, this PTE is a pointer to the next level of the page table. 
    Let i = i − 1. If i < 0, stop and raise a page-fault exception. 
    Otherwise, let a = pte.ppn * PAGESIZE and go to step 2.
5. A leaf PTE has been found. 
    Determine if the requested memory access is allowed by the pte.r, pte.w, pte.x, and pte.u bits, 
    given the current privilege mode and the value of the SUM and MXR fields of the mstatus register. 
    If not, stop and raise a page-fault exception.
6. If i > 0 and pa.ppn[i − 1 : 0] != 0, this is a misaligned superpage; stop and raise a page-fault exception.
7. If pte.a = 0, or if the memory access is a store and pte.d = 0, either raise a page-fault exception or:
    Set pte.a to 1 and, if the memory access is a store, also set pte.d to 1.
    If this access violates a PMA or PMP check, raise an access exception.
    This update and the loading of pte in step 2 must be atomic; 
        in particular, no intervening store to the PTE may be perceived to have occurred in-between.
8. The translation is successful. The translated physical address is given as follows:
    pa.pgoff = va.pgoff.
    If i > 0, then this is a superpage translation and pa.ppn[i − 1 : 0] = va.vpn[i − 1 : 0]. 
    pa.ppn[LEVELS − 1 : i] = pte.ppn[LEVELS − 1 : i].
*/



/*
For Sv39, 
virtual address: 
|      VPN[2]      |      VPN[1]      |      VPN[0]      |      page offset      | 
38               30 29              21 20              12 11                     0
physical address: 
|      PPN[2]      |      PPN[1]      |      PPN[0]      |      page offset      | 
55               30 29              21 20              12 11                     0 
PTE: 
63 -- reserved -- 54 53 -- PPN[2] -- 28 27 -- PPN[1] -- 19 18 -- PPN[0] -- 10 9 -- RSW -- 8 D A G U X W R V 

Sv39 has 2^9 PTEs for a page table, each entry is 8 bytes. Therefore, a page table size is exactly a page size 
*/
/// little-endian only 
// struct pagetable_entry {
//     unsigned V   : 1;  // valid 
//     unsigned R   : 1;  // readable 
//     unsigned W   : 1;  // writable 
//     unsigned X   : 1;  // executable 
//     unsigned U   : 1;  // user-accessable 
//     unsigned G   : 1;  // global 
//     unsigned A   : 1;  // accessable
//     unsigned D   : 1;  // dirty 
//     unsigned RSW : 2;  // reserved for software 
//     unsigned PPN0: 9;  // 
//     unsigned PPN1: 9;  // 
//     unsigned PPN2: 9;  // 
//     unsigned reserved:27;
// } __attribute__ ((packed));
// _Static_assert(sizeof(struct pagetable_entry) == 8); 

#define PTE_V (UINT64_C(1) << 0)
#define PTE_R (UINT64_C(1) << 1) 
#define PTE_W (UINT64_C(1) << 2)
#define PTE_X (UINT64_C(1) << 3) 
#define PTE_U (UINT64_C(1) << 4)
#define PTE_G (UINT64_C(1) << 5) 
#define PTE_A (UINT64_C(1) << 6) 
#define PTE_D (UINT64_C(1) << 7) 
#define PTE_RSW_OFFSET (8) 
#define PTE_RSW_MASK (0b11 << PTE_RSW_OFFSET) 
#define PTE_PPN_OFFSET (10) 
/////////////////////////////////////////// Sv39/Sv48 specific, in total 44 bits PPN //////////////////////////////////// 
#define PTE_PPN_MASK (UINT64_C(0xFFFFFFFFFFF) << PTE_PPN_OFFSET) 
/////////////////////////////////////////// Sv39/Sv48 specific, each VPN is 9-bit /////////////////////////////////////// 
#define VA_GET_VPN(va, level) ( ((va) >> (12 + (level) * 9)) & 0x1FF) 
// NOTE: the PA in PPN has 28 bits, where PPN[2] is a 10-bit field in C906 !!! 
#define PA_PPN_OFFSET (12) 
/////////////////////////////////////////// Sv39 specific ///////////////////////////////////////////////////////////////
#define VIRTUAL_ADDRESS_END (1ULL << 39) 
#define SATP_SV39 (UINT64_C(8) << 60)
#define MAKE_SATP(pagetable) (SATP_SV39 | (((uint64_t)pagetable) >> PA_PPN_OFFSET))
#define PAGE_TABLE_MAXIMUM_LEVELS (2) 
typedef uint64_t pagetable_entry_t; 
// typedef struct pagetable_entry *pagetable_t; 
/////////////////////////////////////////// End Sv39 specific ///////////////////////////////////////////////////////////////
typedef pagetable_entry_t *pagetable_t; 
#define PTE_GET_PA(pte) ( \
    ( \
        ((pagetable_entry_t)(pte) & PTE_PPN_MASK) \
        >> PTE_PPN_OFFSET \
    ) << PA_PPN_OFFSET \
)
#define PA_TO_PTE(pa) ( \
    ( \
        ((uintptr_t)(pa) >> PA_PPN_OFFSET) << PTE_PPN_OFFSET \
    ) & PTE_PPN_MASK \
)
extern pagetable_t kernel_pagetable; 
enum pagesize {
    pagesize_4K = 12,
    pagesize_2M = 21, 
    pagesize_1G = 30
};
/** 
 * walk the page tables and try to get the pagetable entry given the virtual address 
 * @param pagetable the root pagetable to seek 
 * @param virtual_address the virtual address to translate 
 * @param alloc_on_not_found whether to allocate a new pagetable if doesn't exist (NOTE: this option only supports 4K pages)
 * @param end_level the level of the leaf pagetable (2=1G page, 1=2M page, 0=4K page) 
 * @return the pointer to the pagetable entry 
 * */
pagetable_entry_t * vmem_get_pte(pagetable_t pagetable, uintptr_t virtual_address, int alloc_on_not_found, int *end_level); 

// pagetable_entry_t * pagetable_walk(pagetable_t pagetable, uintptr_t virtual_address, int target_level, int alloc_on_not_found)

int vmem_map_range(pagetable_t pagetable, uintptr_t virtual_address, size_t size, uintptr_t physical_address, uint64_t permission); 

void vmem_kernel_init(void); 
void vmem_kernel_set_satp(void); 
#endif 
