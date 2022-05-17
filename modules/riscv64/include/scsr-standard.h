#ifndef SCSR_STANDARD_H 
#define SCSR_STANDARD_H 
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

#endif 
