/*
 * (C) Copyright 2018
 * SPDX-License-Identifier: GPL-2.0+
 * wangwei <wangwei@allwinnertech.com>
 */
/// File instrumented by Yancheng Ou 
#include "boot0-header.h"
/** 
 * this is the information of the commit we used in 
 * https://github.com/smaeul/sun20i_d1_spl 
 * More detailed: 
 * https://github.com/smaeul/sun20i_d1_spl/commit/882671fcf53137aaafc3a94fa32e682cb7b921f1 
 */
#define CI_INFO "882671f-dirty"
#ifdef CFG_ARCH_RISCV

#define BROM_FILE_HEAD_SIZE         (sizeof(boot0_file_head_t) & 0x00FFFFF)
#define BROM_FILE_HEAD_BIT_10_1     ((BROM_FILE_HEAD_SIZE & 0x7FE) >> 1)
#define BROM_FILE_HEAD_BIT_11       ((BROM_FILE_HEAD_SIZE & 0x800) >> 11)
#define BROM_FILE_HEAD_BIT_19_12    ((BROM_FILE_HEAD_SIZE & 0xFF000) >> 12)
#define BROM_FILE_HEAD_BIT_20       ((BROM_FILE_HEAD_SIZE & 0x100000) >> 20)

#define BROM_FILE_HEAD_SIZE_OFFSET  ((BROM_FILE_HEAD_BIT_20 << 31) | \
                                    (BROM_FILE_HEAD_BIT_10_1 << 21) | \
                                    (BROM_FILE_HEAD_BIT_11 << 20) | \
                                    (BROM_FILE_HEAD_BIT_19_12 << 12))
#define JUMP_INSTRUCTION        (BROM_FILE_HEAD_SIZE_OFFSET | 0x6f)
#else
/// ARM's bl instruction 
#define BROM_FILE_HEAD_SIZE_OFFSET  (((sizeof(boot0_file_head_t) + sizeof(int) - 1) / sizeof(int) - 2))
#define JUMP_INSTRUCTION        (BROM_FILE_HEAD_SIZE_OFFSET | 0xEA000000)
#endif
#ifdef __clang__
// __attribute__((section (".header"))) 
#endif 
const boot0_file_head_t __attribute__((section (".boot0header.header")))  BT0_head = {
    {
        /* jump_instruction*/
        JUMP_INSTRUCTION,
        BOOT0_MAGIC,
        STAMP_VALUE,
    #ifdef ALIGN_SIZE_8K
        0x2000,
    #else
        0x4000,
    #endif
        sizeof(boot_file_head_t),
        BOOT_PUB_HEAD_VERSION,
        CONFIG_BOOT0_RET_ADDR,
        CONFIG_BOOT0_RUN_ADDR,
        0,
        {
        /*brom modify: nand-4bytes sdmmc-2bytes*/
        5, 0, 0, 0, '4', '.', '0', 0
        },
    },

    {
        /*__u32 prvt_head_size;*/
        0,
        /*char prvt_head_vsn[4];*/
        8,
        0,/* power_mode */
        {0},/* reserver[2]  */
        /*unsigned int     dram_para[32] ;*/
        {
            0x00000318,
            0x00000003,
            0x007b7bfb,
            0x00000001,
            0x000010d2,
            0x00000000,
            0x00001c70,
            0x00000042,
            0x00000018,
            0x00000000,
            0x004a2195,
            0x02423190,
            0x0008b061,
            0xb4787896,
            0x00000000,
            0x48484848,
            0x00000048,
            0x1620121e,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00870000,
            0x00000024,
            0x34050100,
        },
        /*__s32	uart_port;*/
        0,
        /*normal_gpio_cfg   uart_ctrl[2];*/
        {
            {2, 8, 6, 1, 0xff, 0xff, {0} }, /*PB8: 6--RX*/
            {2, 9, 6, 1, 0xff, 0xff, {0} }, /*PB9: 6--TX*/
        },
        /*__s32 enable_jtag;*/
        0,
        /*normal_gpio_cfg	 jtag_gpio[5];*/
        {
            {6, 0, 4, 0xff, 0xff, 0xff, {0} },
            {6, 5, 4, 0xff, 0xff, 0xff, {0} },
            {6, 3, 4, 0xff, 0xff, 0xff, {0} },
            {6, 1, 4, 0xff, 0xff, 0xff, {0} },
        },
        /*normal_gpio_cfg   storage_gpio[32];*/
        {
            {6, 2, 2, 1, 2, 0xff, {0} },/*PF0-5: 2--SDC*/
            {6, 3, 2, 1, 2, 0xff, {0} },
            {6, 1, 2, 1, 2, 0xff, {0} },
            {6, 0, 2, 1, 2, 0xff, {0} },
            {6, 5, 2, 1, 2, 0xff, {0} },
            {6, 4, 2, 1, 2, 0xff, {0} },
        },
        /*char  storage_data[512 - sizeof(normal_gpio_cfg) * 32];*/
#ifdef CFG_SUNXI_SDMMC
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
            0x02, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
            0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        }
#endif
#ifdef CFG_SUNXI_SPINAND
        // for Nezha_D1-H 2Gb MX35LF2GE4AD SPI NAND Flash
        {
            0x01, 0x00, 0x01, 0x01, 0x01, 0x04, 0x00, 0x00,
            0x40, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
            0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0xc2, 0x26, 0x03, 0xff,
            0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0xe8, 0xfd, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
        }
#endif
    },
    {CI_INFO},
#ifdef CFG_SUNXI_SELECT_DRAM_PARA
    .fes_union_addr.extd_head.magic = DRAM_EXT_MAGIC,
#endif
};

_Static_assert(sizeof(BT0_head) == 0x03c8, "BT0 header size wrong!"); 
