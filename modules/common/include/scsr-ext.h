#ifndef SCSR_EXT_H 
#define SCSR_EXT_H 
#include "csr-read-write.h"

/////////////////////////////////////////// RISC-V standard supervisor-mode csrs ////////////////////////////////////
// supervisor status 
#define SSTATUS "0x100"
// supervisor interrupt enable 
#define SIE     "0x104"
// supervisor trap vector base register 
#define STVEC   "0x105"
// supervisor counter enable 
#define SCOUNTEREN "0x106" 

// supervisor scratchpad register for interrupt handlers 
#define SSCRATCH   "0x140"
// supervisor exception program counter 
#define SEPC       "0x141"
// supervisor exception cause register 
#define SCAUSE     "0x142"
// supervisor trap bad address or instruction for the corresponding exceptions 
#define STVAL      "0x143"
// supervisor interrupt pending 
#define SIP        "0x144"
// supervisor virtual address translation and protection 
#define SATP       "0x180"

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
#define SHPMCOUNTER3 "0x5E3" 
#define SHPMCOUNTER4 "0x5E4" 
#define SHPMCOUNTER5 "0x5E5" 
#define SHPMCOUNTER6 "0x5E6" 
#define SHPMCOUNTER7 "0x5E7" 
#define SHPMCOUNTER8 "0x5E8" 
#define SHPMCOUNTER9 "0x5E9" 
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



#endif 



















