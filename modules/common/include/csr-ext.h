#ifndef CSR_EXT_H 
#define CSR_EXT_H 
#include "csr-read-write.h"

/////////////////////////////////////////// RISC-V standard user-mode csrs ////////////////////////////////////
// floating point accrued exceptions 
#define FFLAGS "0x001"
// floating point dynamic rounding mode 
#define FRM    "0x002"
// floating point control and status register 
#define FCSR   "0x003"
// usermode cycle counter 
#define CYCLE  "0xC00"
// usermode timer 
#define TIME   "0xC01"
// usermode retired instruction counter 
#define INSTRET "0xC02"
// usermode performance-monitoring counter 3..31 (only the first 3..17 are listed)
#define HPMCOUNTER3 "0xC03" 
#define HPMCOUNTER4 "0xC04" 
#define HPMCOUNTER5 "0xC05" 
#define HPMCOUNTER6 "0xC06" 
#define HPMCOUNTER7 "0xC07" 
#define HPMCOUNTER8 "0xC08" 
#define HPMCOUNTER9 "0xC09" 
#define HPMCOUNTER10 "0xC0A" 
#define HPMCOUNTER11 "0xC0B" 
#define HPMCOUNTER12 "0xC0C" 
#define HPMCOUNTER13 "0xC0D" 
#define HPMCOUNTER14 "0xC0E" 
#define HPMCOUNTER15 "0xC0F" 
#define HPMCOUNTER16 "0xC10" 
#define HPMCOUNTER17 "0xC11" 
// vector start 
#define VSTART "0x008" 
// vector overflow flag 
#define VXSAT  "0x009" 
// vector rounding mode 
#define VXRM   "0x00A"
// vector length register 
#define VL     "0xC20"
// vector datatype 
#define VTYPE  "0xC21"
// vector number of bytes 
#define VLENB  "0xC22" 
/////////////////////////////////////////// C906 extended user-mode csrs ////////////////////////////////////
// user-mode extended floating point control register 
#define FXCR "0x800"


#endif 


















