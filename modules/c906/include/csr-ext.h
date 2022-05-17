#ifndef CSR_EXT_H 
#define CSR_EXT_H 


/////////////////////////////////////////// C906 extended user-mode csrs ////////////////////////////////////
// user-mode extended floating point control register 
#define FXCR "0x800"

/*
FXCR is almost the same as FCSR, with a few more fields 
bit 5: FE if there's any floating point exception, this bit is 1 
bit 23: DQNaN 
    if this is 0, then the QNaN output will be RISC-V's specified fixed value, 
    i.e., sign = 0, exponent = all 1, mantissa = 10000...0, 
    otherwise the QNaN will comply with IEEE754 (multiple values) 
*/



#endif 


















