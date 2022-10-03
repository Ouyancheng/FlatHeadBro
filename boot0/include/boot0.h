#ifndef BOOT0_H 
#define BOOT0_H 


#define SRAM_A1_BASE (0x20000)
#define SRAM_A1_SIZE (0x8000)
/* in sun20iw1p1.h, is this documented in the D1 user spec? */
#define SRAM_C_BASE (0x28000)
#define SRAM_C_SIZE (0x20000)

#define FEL_BASE (0x20)

#define BOOT0_MAGIC      "eGON.BT0"
#define BOOT0_MAGIC_SIZE 8
#define DRAM_EXT_MAGIC   "DRAM.ext"
#define STAMP_VALUE      0x5F0A6C39

#define DRAM_START 0x40000000UL



#endif // ! BOOT0_H 
