#ifndef SCSR_EXT_H 
#define SCSR_EXT_H 



/////////////////////////////////////////// C906 extended supervisor-mode csrs ////////////////////////////////////
// supervisor extended status 
#define SXSTATUS "0x5C0"
// supervisor hardware control 
#define SHCR     "0x5C1"
// supervisor counter overflow event interrupt enable 
#define SCOUNTERINTEN "0x5C4" 
// supervisor counter overflow flag 
#define SCOUNTEROF "0x5C5"
// supervisor cycle counter 
#define SCYCLE "0x5E0" 
// supervisor retired instruction counter 
#define SINSTRET "0x5E2" 
// supervisor counters 
// notice that it's quite surprising this is not in the riscv standard... 
#define SHPMCOUNTER3  "0x5E3" 
#define SHPMCOUNTER4  "0x5E4" 
#define SHPMCOUNTER5  "0x5E5" 
#define SHPMCOUNTER6  "0x5E6" 
#define SHPMCOUNTER7  "0x5E7" 
#define SHPMCOUNTER8  "0x5E8" 
#define SHPMCOUNTER9  "0x5E9" 
#define SHPMCOUNTER10 "0x5EA" 
#define SHPMCOUNTER11 "0x5EB" 
#define SHPMCOUNTER12 "0x5EC" 
#define SHPMCOUNTER13 "0x5ED" 
#define SHPMCOUNTER14 "0x5EE" 
#define SHPMCOUNTER15 "0x5EF" 
#define SHPMCOUNTER16 "0x5F0" 
#define SHPMCOUNTER17 "0x5F1" 
// supervisor MMU index 
#define SMIR "0x9C0" 
// supervisor MMU entry lo 
#define SMEL "0x9C1"
// supervisor MMU entry hi 
#define SMEH "0x9C2" 
// supervisor MMU control 
#define SMCIR "0x9C3" 

/*
SXSTATUS: the same as MXSTATUS, MM/PMDS/PMDM bits are writable in supervisor mode 
SHCR: the same as MHCR, read-only for supervisor mode 
SCOUNTERINTEN: the mapping of MCOUNTERINTEN, the i-th bit writable only if MCOUNTERWEN[i] bit is 1. 
SCOUNTEROF: the mapping of MCOUNTEROF, also complies with MCOUNTERWEN's permission 
SCYCLE: cycle counter 
SINTERET: the counter of retired instructions 
SHPMCOUNTERn: the mapping of MHPMCOUNTERn, complies with MCOUNTERWEN's permission 

SMIR: mmu index 
SMEH: mmu entry high bits 
SMEL: mmu entry low bits 
*/

#endif 



















