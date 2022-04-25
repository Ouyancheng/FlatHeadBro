/**
 * Clock control unit for Allwinner D1 
 * See: D1 User Manual chapter 3.2 
 */
#ifndef CLOCK_INIT_H
#define CLOCK_INIT_H 


#define CCU_BASE                           0x02001000
#define CCU_PLL_CPU_CTRL_REG               (CCU_BASE+0x000)
#define CCU_PLL_DDR_CTRL_REG               (CCU_BASE+0x010)
#define CCU_PLL_PERI0_CTRL_REG             (CCU_BASE+0x020)
#define CCU_PLL_PERI1_CTRL_REG             (CCU_BASE+0x028)         //NO USER
#define CCU_PLL_GPU_CTRL_REG               (CCU_BASE+0x030)         //NO USER
#define CCU_PLL_VIDEO0_CTRL_REG            (CCU_BASE+0x040)
#define CCU_PLL_VIDEO1_CTRL_REG            (CCU_BASE+0x048)
#define CCU_PLL_VE_CTRL                    (CCU_BASE+0x058)
#define CCU_PLL_DE_CTRL                    (CCU_BASE+0x060)
#define CCU_PLL_HSIC_CTRL                  (CCU_BASE+0x070)
#define CCU_PLL_AUDIO0_CTRL_REG            (CCU_BASE+0x078)
#define CCU_PLL_AUDIO1_CTRL_REG            (CCU_BASE+0x080)
#define CCU_PLL_DDR_PAT0_CTRL_REG          (CCU_BASE+0x110)
#define CCU_PLL_DDR_PAT1_CTRL_REG          (CCU_BASE+0x114)
#define CCU_PLL_PERI0_PAT0_CTRL_REG        (CCU_BASE+0x120)
#define CCU_PLL_PERI0_PAT1_CTRL_REG        (CCU_BASE+0x124)
#define CCU_PLL_PERI1_PAT0_CTRL_REG        (CCU_BASE+0x128)
#define CCU_PLL_PERI1_PAT1_CTRL_REG        (CCU_BASE+0x12c)
#define CCU_PLL_GPU_PAT0_CTRL_REG          (CCU_BASE+0x130)
#define CCU_PLL_GPU_PAT1_CTRL_REG          (CCU_BASE+0x134)
#define CCU_PLL_VIDEO0_PAT0_CTRL_REG       (CCU_BASE+0x140)
#define CCU_PLL_VIDEO0_PAT1_CTRL_REG       (CCU_BASE+0x144)
#define CCU_PLL_VIDEO1_PAT0_CTRL_REG       (CCU_BASE+0x148)
#define CCU_PLL_VIDEO1_PAT1_CTRL_REG       (CCU_BASE+0x14c)
#define CCU_PLL_VE_PAT0_CTRL_REG           (CCU_BASE+0x158)
#define CCU_PLL_VE_PAT1_CTRL_REG           (CCU_BASE+0x15c)
#define CCU_PLL_DE_PAT0_CTRL_REG           (CCU_BASE+0x160)
#define CCU_PLL_DE_PAT1_CTRL_REG           (CCU_BASE+0x164)
#define CCU_PLL_HSIC_PAT0_CTRL_REG         (CCU_BASE+0x170)
#define CCU_PLL_HSIC_PAT1_CTRL_REG         (CCU_BASE+0x174)
#define CCU_PLL_AUDIO0_PAT0_CTRL_REG       (CCU_BASE+0x178)
#define CCU_PLL_AUDIO0_PAT1_CTRL_REG       (CCU_BASE+0x17c)
#define CCU_PLL_AUDIO1_PAT0_CTRL_REG       (CCU_BASE+0x180)
#define CCU_PLL_AUDIO1_PAT1_CTRL_REG       (CCU_BASE+0x184)
#define CCU_PLL_CPU_BIAS_REG               (CCU_BASE+0x300)
#define CCU_PLL_DDR_BIAS_REG               (CCU_BASE+0x310)
#define CCU_PLL_PERI0_BIAS_REG             (CCU_BASE+0x320)
#define CCU_PLL_PERI1_BIAS_REG             (CCU_BASE+0x328)
#define CCU_PLL_GPU_BIAS_REG               (CCU_BASE+0x330)
#define CCU_PLL_VIDEO0_BIAS_REG            (CCU_BASE+0x340)
#define CCU_PLL_VIDEO1_BIAS_REG            (CCU_BASE+0x348)
#define CCU_PLL_VE_BIAS_REG                (CCU_BASE+0x358)
#define CCU_PLL_DE_BIAS_REG                (CCU_BASE+0x360)
#define CCU_PLL_HSIC_BIAS_REG              (CCU_BASE+0x370)
#define CCU_PLL_AUDIO0_BIAS_REG            (CCU_BASE+0x378)
#define CCU_PLL_AUDIO1_BIAS_REG            (CCU_BASE+0x380)
#define CCU_PLL_CPU_TUN_REG                (CCU_BASE+0x400)
#define CCU_CPU_AXI_CFG_REG                (CCU_BASE+0x500)
#define CCU_CPU_GATING_REG                 (CCU_BASE+0x504)
#define CCU_PSI_CLK_REG                    (CCU_BASE+0x510)
#define CCU_AHB3_CLK_REG                   (CCU_BASE+0x51c)
#define CCU_APB0_CLK_REG                   (CCU_BASE+0x520)
#define CCU_APB1_CLK_REG                   (CCU_BASE+0x524)
#define CCU_MBUS_CLK_REG                   (CCU_BASE+0x540)
#define CCU_DMA_BGR_REG                    (CCU_BASE+0x70c)
#define CCU_DRAM_CLK_REG                   (CCU_BASE+0x800)
#define CCU_MBUS_MAT_CLK_GATING_REG        (CCU_BASE+0x804)
#define CCU_DRAM_BGR_REG                   (CCU_BASE+0x80c)
#define CCU_UART_BGR_REG                   (CCU_BASE+0x90C)
#define CCU_TWI_BGR_REG                    (CCU_BASE+0x91C)
#define CCU_SPI0_BGR_REG                   (CCU_BASE+0x940)
#define CCU_SPI1_BGR_REG                   (CCU_BASE+0x944)
#define CCU_SPI_BGR_REG                    (CCU_BASE+0x96C)
#define CCU_RISCV_CLK_REG                  (CCU_BASE+0xd00)
#define CCU_RISCV_GATING_REG               (CCU_BASE+0xd04)
#define CCU_RISCV_CFG_BGR_REG              (CCU_BASE+0xd0c)

void sys_clock_init(void); 
// void clk_enable_module_uart(uintptr_t addr, uint8_t uart_num); 

#endif 

