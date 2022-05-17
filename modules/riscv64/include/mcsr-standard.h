#ifndef MCSR_STANDARD_H 
#define MCSR_STANDARD_H 

/////////////////////////////////////////// RISC-V standard machine-mode csrs ////////////////////////////////////
// vendor id 
#define    MVENDORID      "0xF11" 
// architecture id 
#define    MARCHID        "0xF12"
// implementation id 
#define    MIMPID         "0xF13"
// hardware thread id 
#define    MHARTID        "0xF14"
// processor status 
#define    MSTATUS        "0x300"
// ISA architecture and extensions 
#define    MISA           "0x301"
// machine exceptions delegation register 
#define    MEDELEG        "0x302"
// machine interrupt delegation register 
#define    MIDELEG        "0x303"
// machine interrupt enable 
#define    MIE            "0x304" 
// machine interrupt vector base address 
#define    MTVEC          "0x305" 
// machine counter enable 
#define    MCOUNTEREN     "0x306" 
// scratchpad register for interrupt 
#define    MSCRATCH       "0x340" 
// machine exception program counter 
#define    MEPC           "0x341"
// machine exception cause 
#define    MCAUSE         "0x342" 
// machine bad address or instruction for the corresponding interrupt 
#define    MTVAL          "0x343"
// machine interrupt pending 
#define    MIP            "0x344" 
// physical memory protection configuration 0 
#define    PMPCFG0        "0x3A0"
// physical memory protection configuration 1 
#define    PMPCFG1        "0x3A1"
// physical memory protection configuration 2 
#define    PMPCFG2        "0x3A2"
// physical memory protection base address 0 - 15 
#define    PMPADDR0       "0x3B0"
#define    PMPADDR1       "0x3B1"
#define    PMPADDR2       "0x3B2"
#define    PMPADDR3       "0x3B3"
#define    PMPADDR4       "0x3B4"
#define    PMPADDR5       "0x3B5"
#define    PMPADDR6       "0x3B6"
#define    PMPADDR7       "0x3B7"
#define    PMPADDR8       "0x3B8"
#define    PMPADDR9       "0x3B9"
#define    PMPADDR10      "0x3BA"
#define    PMPADDR11      "0x3BB"
#define    PMPADDR12      "0x3BC"
#define    PMPADDR13      "0x3BD"
#define    PMPADDR14      "0x3BE"
#define    PMPADDR15      "0x3BF"
// machine mode cycle counter 
#define    MCYCLE         "0xB00"
// machine mode retired instruction counter  
#define    MINSTRET       "0xB02"
// machine mode performance monitoring counter 3..17 
#define    MHPMCOUNTER3   "0xB03"
#define    MHPMCOUNTER4   "0xB04"
#define    MHPMCOUNTER5   "0xB05"
#define    MHPMCOUNTER6   "0xB06"
#define    MHPMCOUNTER7   "0xB07"
#define    MHPMCOUNTER8   "0xB08"
#define    MHPMCOUNTER9   "0xB09"
#define    MHPMCOUNTER10  "0xB0A"
#define    MHPMCOUNTER11  "0xB0B"
#define    MHPMCOUNTER12  "0xB0C"
#define    MHPMCOUNTER13  "0xB0D"
#define    MHPMCOUNTER14  "0xB0E"
#define    MHPMCOUNTER15  "0xB0F"
#define    MHPMCOUNTER16  "0xB10"
#define    MHPMCOUNTER17  "0xB11"
// machine mode performance-monitoring event select register 3..17  
#define    MHPMEVENT3     "0x323"
#define    MHPMEVENT4     "0x324"
#define    MHPMEVENT5     "0x325"
#define    MHPMEVENT6     "0x326"
#define    MHPMEVENT7     "0x327"
#define    MHPMEVENT8     "0x328"
#define    MHPMEVENT9     "0x329"
#define    MHPMEVENT10    "0x32A"
#define    MHPMEVENT11    "0x32B"
#define    MHPMEVENT12    "0x32C"
#define    MHPMEVENT13    "0x32D"
#define    MHPMEVENT14    "0x32E"
#define    MHPMEVENT15    "0x32F"
#define    MHPMEVENT16    "0x330"
#define    MHPMEVENT17    "0x331"
///////////////////////////////////// The mstatus register bits //////////////////////////////// 
#define MSTATUS_UIE         (1 << 0)
#define MSTATUS_SIE         (1 << 1)
#define MSTATUS_MIE         (1 << 3)
#define MSTATUS_UPIE        (1 << 4)
#define MSTATUS_SPIE        (1 << 5)
#define MSTATUS_MPIE        (1 << 7)
#define MSTATUS_SPP         (1 << 8)
#define MSTATUS_MPP         (3 << 11)
#define MSTATUS_FS          (3 << 13)
#define MSTATUS_XS          (3 << 15)
#define MSTATUS_MPRV        (1 << 17)
#define MSTATUS_SUM         (1 << 18)
#define MSTATUS_MXR         (1 << 19)
#define MSTATUS_TVM         (1 << 20)
#define MSTATUS_TW          (1 << 21)
#define MSTATUS_TSR         (1 << 22)
#define MSTATUS32_SD        (1 << 31)
#define MSTATUS_UXL         (3ULL << 32)
#define MSTATUS_SXL         (3ULL << 34)
#define MSTATUS64_SD        (1ULL << 63)

///////////////////////////////////// The mie/sie register bits //////////////////////////////// 
// supervisor mode software interrupt enable 
#define MIE_SSIE (1 << 1) 
// machine mode software interrupt enable 
#define MIE_MSIE (1 << 3)
// supervisor mode timer interrupt enable 
#define MIE_STIE (1 << 5)
// machine mode timer interrupt enable 
#define MIE_MTIE (1 << 7) 
// supervisor mode external interrupt enable 
#define MIE_SEIE (1 << 9)
// machine mode external interrupt enable 
#define MIE_MEIE (1 << 11) 
// machine mode performance counter overflow interrupt enable 
#define MIE_MOIE (1 << 17)

#endif 
