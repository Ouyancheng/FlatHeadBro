#ifndef MSCR_EXT_H
#define MSCR_EXT_H 
#include <stdint.h>

// inhibit counter 
#define    MCOUNTINHIBIT  "0x320" 
// extended status 
#define    MXSTATUS       "0x7C0" 
// hardware configuration, used to enable cache 
#define    MHCR           "0x7C1" 
// hardware operation, used to invalidate i$/d$/BTB/BHT 
#define    MCOR           "0x7C2" 
// hardware hint operation 
#define    MHINT          "0x7C5" 
// the reset vector base 
#define    MRVBR          "0x7C7" 
// L1 cache ECC 
#define    MCER           "0x7C8" 
// counter write enable 
#define    MCOUNTERWEN    "0x7C9" 
// enable interrupt on counter overflow 
#define    MCOUNTERINTEN  "0x7CA" 
// counter overflow 
#define    MCOUNTEROF     "0x7CB" 
// peripherial address high bits 
#define    MAPBADDR       "0xFC1" 
// cache instruction enable
#define    MCINS          "0x7D2"  
// cache access index 
#define    MCINDEX        "0x7D3" 
// cache data 0 
#define    MCDATA0        "0x7D4" 
// cache data 1 
#define    MCDATA1        "0x7D5" 
// CPU ID 
#define    MCPUID         "0xFC0" 

// enum C906MCSRExt {
//     MCOUNTINHIBIT = 0x320, // inhibit counter 
//     MXSTATUS      = 0x7C0, // extended status 
//     MHCR          = 0x7C1, // hardware configuration, used to enable cache 
//     MCOR          = 0x7C2, // hardware operation, used to invalidate i$/d$/BTB/BHT 
//     MHINT         = 0x7C5, // hardware hint operation 
//     MRVBR         = 0x7C7, // the reset vector base 
//     MCER          = 0x7C8, // L1 cache ECC 
//     MCOUNTERWEN   = 0x7C9, // counter write enable 
//     MCOUNTERINTEN = 0x7CA, // enable interrupt on counter overflow 
//     MCOUNTEROF    = 0x7CB, // counter overflow 
//     MAPBADDR      = 0xFC1, // peripherial address high bits 
//     MCINS         = 0x7D2, // cache instruction enable 
//     MCINDEX       = 0x7D3, // cache access index 
//     MCDATA0       = 0x7D4, // cache data 0 
//     MCDATA1       = 0x7D5, // cache data 1 
//     MCPUID        = 0xFC0, // CPU ID 
// };


// pmdu = 0 enable performance counter on user mode and vice versa 
#define MXSTATUS_PMDU  (1 << 10)
// pmds = 0 enable performance counter on super mode and vice versa 
#define MXSTATUS_PMDS  (1 << 11)
// pmdm = 0 enable performance counter on machien mode and vice versa 
#define MXSTATUS_PMDM  (1 << 13)
// the minimum control granularity of PMP, but C906 only supports 4K PMP
#define MXSTATUS_PMP4K (1 << 14)
// MM = 1 enable non-aligned access, otherwise generate an exception on non-aligned access 
#define MXSTATUS_MM    (1 << 15)
// UCME = 0 disable the extended cache instruction, and vice versa 
#define MXSTATUS_UCME  (1 << 16)
// CLINT counter interrupt enable 
#define MXSTATUS_CLINTEE (1 << 17)
// MHRD = 0 enable hardware backfilling on TLB miss 
#define MXSTATUS_MHRD (1 << 18)
// MAEE = 1 user can configure the address property of a PTE 
#define MXSTATUS_MAEE (1 << 21)
// THEADISAEE = 1 enable C906 extended instructions 
#define MXSTATUS_THEADISAEE (1 << 22)
// PM = 0b00 user mode, PM = 0b01 super mode, PM = 0b11 machine mode 
#define MXSTATUS_PM (1 << 30) 

// struct MXSTATUS_csr {
//     unsigned    set0 : 10;
//     unsigned    PMDU : 1;       
//     unsigned    PMDS : 1;       
//     unsigned    set0_1 : 1; 
//     unsigned    PMDM : 1;       
//     unsigned    PMP4K_PMP : 1;  
//     unsigned    MM : 1;         
//     unsigned    UCME : 1;       
//     unsigned    CLINTEE : 1;    
//     unsigned    MHRD : 1;       
//     unsigned    set0_2 : 2; 
//     unsigned    MAEE : 1;       
//     unsigned    THEADISAEE : 1; 
//     unsigned    set0_3 : 7; 
//     unsigned    PM : 2;         
//     unsigned    set0_upper : 32;
// };
// _Static_assert(sizeof(struct MXSTATUS_csr) == 8, "MXSTATUS size is not 64-bit!"); 

// icache enable 
#define MHCR_IE  (1 << 0)
// dcache enable 
#define MHCR_DE  (1 << 1)
// WA=0 use write non-allocate, WA=1 use write allocate 
#define MHCR_WA  (1 << 2)
// WB=0 use write through, WB=1 use write back, C906 only supports write back 
#define MHCR_WB  (1 << 3)
// RS=0 return stack disable, RS=1 return stack enable 
#define MHCR_RS  (1 << 4)
// BPE=0 disable branch predictor, BPE=1 enable branch predictor 
#define MHCR_BPE (1 << 5)
// BTB=0 disable branch target predictor, BTB=1 enable branch target predictor 
#define MHCR_BTB (1 << 6)
// write burst transmission always 1 
#define MHCR_WBR (1 << 8)

// struct MHCR_csr {
//     unsigned    IE    : 1; 
//     unsigned    DE    : 1; 
//     unsigned    WA    : 1; 
//     unsigned    WB    : 1; 
//     unsigned    RS    : 1; 
//     unsigned    BPE   : 1; 
//     unsigned    BTB   : 1; 
//     unsigned    set0  : 1; 
//     unsigned    WBR   : 1; 
//     unsigned    set0_1: 23;
//     unsigned    set0_2: 32;
// };
// _Static_assert(sizeof(struct MHCR_csr) == 8, "MHCR size is not 64-bit!"); 


// cache select 0b01 icache, 0b10 dcache, 0b11 i and d
#define MCOR_CACHESEL_I_CACHE (0b01 << 0)
#define MCOR_CACHESEL_D_CACHE (0b10 << 0)
#define MCOR_CACHESEL_I_D_CACHE (0b11 << 0) 
// invalidate the selected cache 
#define MCOR_INV     (1 << 4)
// write back the dirty cache lines, 0 = no write back 
#define MCOR_CLR     (1 << 5)
// invalidate BHT 
#define MCOR_BHT_INV (1 << 16)
// invalidate BTB 
#define MCOR_BTB_INV (1 << 17)

// dcache prefetch, 1 = ON, 0 = OFF 
#define MHINT_DPLD   (1 << 2)

// MHINT_AMR omitted! 

// icache prefetch 
#define MHINT_IPLD   (1 << 8)

// MHINT_IWPE is the way prediction, but position not on datasheet! 

// prefetch 2 lines for dcache 
#define MHINT_D_DIS_2  (0b00 << 13)
// 4 lines 
#define MHINT_D_DIS_4  (0b01 << 13)
// 8 lines 
#define MHINT_D_DIS_8  (0b10 << 13)
// 16 lines 
#define MHINT_D_DIS_16 (0b11 << 13)

#define struct_to_int64(x) (*(uint64_t*)(&x))

#define write_csr(csr, val) \
({ \
    uint64_t v = (uint64_t)val; \
    asm volatile (\ 
        "csrw " csr ", %0" \
        : \
        : "rK"(v) \
        : "memory" \
    ); \
})

#define read_csr(csr) \
({ \
    uint64_t v; \
    asm volatile (\ 
        "csrr %0, " csr  \
        : "=r"(v) \
        : \
        : "memory" \
    ); \
    v; \
})



int printsize() {
    // struct MHCR_csr hcr = {
    //     .IE = 1,
    //     .DE = 1, 
    //     .WA = 1,
    //     .WB = 1, 
    //     .RS = 1,
    //     .BPE = 1, 
    //     .BTB = 1,
    //     .set0 = 0,
    //     .WBR = 1,
    //     .set0_1 = 0,
    //     .set0_2 = 0
    // };
    // write_csr(MHCR, struct_to_int64(hcr));
    // uint64_t cor; 
    // cor = read_csr(MCOR);
    // return sizeof(struct MHCR_csr);
}

#endif 
