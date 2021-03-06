/*
 * (C) Copyright 2018
 * SPDX-License-Identifier: GPL-2.0+
 * wangwei <wangwei@allwinnertech.com>
 */
/// File instrumented by Yancheng Ou 
#ifndef BOOT0_HEADER_H
#define BOOT0_HEADER_H
#include <stdint.h>
#include "config.h"
#include "spare-head.h"
#include "dram-param.h"
#define BOOT0_MAGIC                     "eGON.BT0"
#define DRAM_EXT_MAGIC                  "DRAM.ext"
#define SYS_PARA_LOG                    0x4d415244

/******************************************************************************/
/*                              file head of Boot                             */
/******************************************************************************/
typedef struct _Boot_file_head {
    uint32_t  jump_instruction;   /* one intruction jumping to real code */
    uint8_t   magic[MAGIC_SIZE];  /* ="eGON.BT0" */
    uint32_t  check_sum;          /* generated by PC */
    uint32_t  length;             /* generated by PC */
    uint32_t  pub_head_size;      /* the size of boot_file_head_t */
    uint8_t   pub_head_vsn[4];    /* the version of boot_file_head_t */
    uint32_t  ret_addr;           /* the return value */
    uint32_t  run_addr;           /* run addr */
    uint32_t  boot_cpu;           /* eGON version */
    uint8_t   platform[8];        /* platform information */
} boot_file_head_t;


/******************************************************************************/
/*                              file head of Boot0                            */
/******************************************************************************/
typedef struct _boot0_private_head_t {
    uint32_t                       prvt_head_size;
    /* debug_mode = 0 : do not print any message,debug_mode = 1 ,print debug message */
    uint8_t                        debug_mode;
    /* 0:axp, 1: no axp  */
    uint8_t                        power_mode;
    uint8_t                        reserve[2];
    /* DRAM patameters for initialising dram. Original values is arbitrary */
    // unsigned int                dram_para[32];
    struct ddr3_param_t            dram_para;
    /* uart: num & uart pin */
    int32_t                        uart_port;
    normal_gpio_cfg                uart_ctrl[2];
    /* jtag: 1 : enable,  0 : disable */
    int32_t                       enable_jtag;
    normal_gpio_cfg	               jtag_gpio[5];
    /* nand/mmc pin */
    normal_gpio_cfg                storage_gpio[32];
    /* reserve data */
    char                           storage_data[512 - sizeof(normal_gpio_cfg) * 32];
} boot0_private_head_t;

typedef struct __fes_aide_info {
    uint32_t dram_init_flag;       /* flag for Dram init */
    uint32_t dram_update_flag;     /* flag for dram para */
    uint32_t dram_paras[SUNXI_DRAM_PARA_MAX];
} fes_aide_info_t;

typedef struct _boot0_file_head_t {
    boot_file_head_t      boot_head;
    boot0_private_head_t  prvt_head;
    char hash[16];
    normal_gpio_cfg       i2c_gpio[2];
    uint8_t               reserved[40];
    union {
        #ifdef CFG_SUNXI_SELECT_DRAM_PARA
            boot_extend_head_t    extd_head;
        #endif
        fes_aide_info_t fes1_res_addr;
    } fes_union_addr;
} boot0_file_head_t;

typedef struct _sboot_file_head_t {
    boot_file_head_t      boot_head;
    char hash[16];
    normal_gpio_cfg       i2c_gpio[2];
    char data[8];
    char res[24];
    #ifdef CFG_SUNXI_SELECT_DRAM_PARA
        boot_extend_head_t  extd_head;
    #endif
} sboot_file_head_t;

extern const sboot_file_head_t  sboot_head ;
extern 
#ifdef __clang__
// __attribute__((section (".header"))) 
#endif 
const boot0_file_head_t BT0_head;
extern const boot0_file_head_t fes1_head;


#endif 

