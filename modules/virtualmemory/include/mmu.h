#ifndef MMU_H 
#define MMU_H 

#include "csr-read-write.h"
#include "scsr-ext.h"
#include "cache.h"
#include "fence.h"
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
 * bit 45 - 19 : virtual page number, updated by hardware on page fault/TLB read, pre-written by software on TLB write 
 * bit 18, 17, 16 : page size, one-hot vector: bit 16: 4K, bit 17: 2M, bit 18: 1G 
 * bit 15 - 0 : the ASID address space ID, used to distinguish different processes, updated by hardware on TLB read, pre-written by software on TLB write 
 * 
 * 
 * MMU EntryLo register (SMEL)
 * bit 63 : SO strong order 0 = not strong order (for normal memory, default), 1 = strong order (for devices)
 * bit 62 : C cachable 0 = noncacheable 1 = cacheable, default is 0 
 * bit 61 : B bufferable 0 = nonbufferable 1 = bufferable, default is 0 
 * bit 60 : reserved 
 * bit 59 : Sec / T trustable not used in C906, default is 1 
 * 
 * 
 * bit 37 - 10 : physical page number (38 bits)
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
 * MMU address translation register (SATP) Sv39 standard 
 * bit 63 - 60 : Mode translation mode 
 * bit 59 - 44 : ASID 
 * bit 27 - 0 : PPN 
 */





#endif 
