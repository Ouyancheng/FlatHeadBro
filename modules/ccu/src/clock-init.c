/*
 * sys-clock.c
 *
 * Copyright(c) 2007-2021 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include "clock-init.h"
#include "get-put.h"
#include "delay.h"
static void set_pll_cpux_axi(void) {
    uint32_t val;

    /* Select cpux clock src to osc24m, axi divide ratio is 3, system apb clk ratio is 4 */
    put32(CCU_RISCV_CLK_REG, (0 << 24) | (3 << 8) | (1 << 0));
    delay_us(1);

    /* Disable pll gating */
    val = get32(CCU_PLL_CPU_CTRL_REG);
    val &= ~(1 << 27);
    put32(CCU_PLL_CPU_CTRL_REG, val);

    /* Enable pll ldo */
    val = get32(CCU_PLL_CPU_CTRL_REG);
    val |= (1 << 30);
    put32(CCU_PLL_CPU_CTRL_REG, val);
    delay_us(5);

    /* Set default clk to 1008mhz */
    val = get32(CCU_PLL_CPU_CTRL_REG);
    val &= ~((0x3 << 16) | (0xff << 8) | (0x3 << 0));
    val |= (41 << 8);
    put32(CCU_PLL_CPU_CTRL_REG, val);

    /* Lock enable */
    val = get32(CCU_PLL_CPU_CTRL_REG);
    val |= (1 << 29);
    put32(CCU_PLL_CPU_CTRL_REG, val);

    /* Enable pll */
    val = get32(CCU_PLL_CPU_CTRL_REG);
    val |= (1 << 31);
    put32(CCU_PLL_CPU_CTRL_REG, val);

    /* Wait pll stable */
    while(!(get32(CCU_PLL_CPU_CTRL_REG) & (0x1 << 28)));
    delay_us(20);

    /* Enable pll gating */
    val = get32(CCU_PLL_CPU_CTRL_REG);
    val |= (1 << 27);
    put32(CCU_PLL_CPU_CTRL_REG, val);

    /* Lock disable */
    val = get32(CCU_PLL_CPU_CTRL_REG);
    val &= ~(1 << 29);
    put32(CCU_PLL_CPU_CTRL_REG, val);
    delay_us(1);

    /* Set and change cpu clk src */
    val = get32(CCU_RISCV_CLK_REG);
    val &= ~(0x07 << 24 | 0x3 << 8 | 0xf << 0);
    val |= (0x05 << 24 | 0x1 << 8);
    put32(CCU_RISCV_CLK_REG, val);
    delay_us(1);
}

static void set_pll_periph0(void) {
    uint32_t val;

    /* Periph0 has been enabled */
    if(get32(CCU_PLL_PERI0_CTRL_REG) & (1 << 31)) {
        return;
    }

    /* Change psi src to osc24m */
    val = get32(CCU_PSI_CLK_REG);
    val &= (~(0x3 << 24));
    put32(CCU_PSI_CLK_REG, val);

    /* Set default val */
    put32(CCU_PLL_PERI0_CTRL_REG, 0x63 << 8);

    /* Lock enable */
    val = get32(CCU_PLL_PERI0_CTRL_REG);
    val |= (1 << 29);
    put32(CCU_PLL_PERI0_CTRL_REG, val);

    /* Enabe pll 600m(1x) 1200m(2x) */
    val = get32(CCU_PLL_PERI0_CTRL_REG);
    val |= (1 << 31);
    put32(CCU_PLL_PERI0_CTRL_REG, val);

    /* Wait pll stable */
    while(!(get32(CCU_PLL_PERI0_CTRL_REG) & (0x1 << 28)));
    delay_us(20);

    /* Lock disable */
    val = get32(CCU_PLL_PERI0_CTRL_REG);
    val &= ~(1 << 29);
    put32(CCU_PLL_PERI0_CTRL_REG, val);
}

static void set_ahb(void) {
    put32(CCU_PSI_CLK_REG, (2 << 0) | (0 << 8));
    put32(CCU_PSI_CLK_REG, get32(CCU_PSI_CLK_REG) | (0x03 << 24));
    delay_us(1);
}

static void set_apb(void) {
    put32(CCU_APB0_CLK_REG, (2 << 0) | (1 << 8));
    put32(CCU_APB0_CLK_REG, (0x03 << 24) | get32(CCU_APB0_CLK_REG));
    delay_us(1);
}

static void set_dma(void) {
    /* Dma reset */
    put32(CCU_DMA_BGR_REG, get32(CCU_DMA_BGR_REG) | (1 << 16));
    delay_us(20);
    /* Enable gating clock for dma */
    put32(CCU_DMA_BGR_REG, get32(CCU_DMA_BGR_REG) | (1 << 0));
}

static void set_mbus(void) {
    uint32_t val;

    /* Reset mbus domain */
    val = get32(CCU_MBUS_CLK_REG);
    val |= (0x1 << 30);
    put32(CCU_MBUS_CLK_REG, val);
    delay_us(1);
}

static void set_module(uintptr_t addr) {
    uint32_t val;

    if(!(get32(addr) & (1 << 31))) {
        val = get32(addr);
        put32(addr, val | (1 << 31) | (1 << 30));

        /* Lock enable */
        val = get32(addr);
        val |= (1 << 29);
        put32(addr, val);

        /* Wait pll stable */
        while(!(get32(addr) & (0x1 << 28)));
        delay_us(20);

        /* Lock disable */
        val = get32(addr);
        val &= ~(1 << 29);
        put32(addr, val);
    }
}

void clk_enable_module_uart(uintptr_t addr, uint8_t uart_num) {
    uint32_t val;
    /* Open the clock gate for uart */
    val = get32(addr);
    val |= 1 << (0 + uart_num);
    put32(addr, val);

    /* Deassert uart reset */
    val = get32(addr);
    val |= 1 << (16 + uart_num);
    put32(addr, val);
}

void sys_clock_init(void) {
    set_pll_cpux_axi();
    set_pll_periph0();
    set_ahb();
    set_apb();
    set_dma();
    set_mbus();
    set_module(CCU_PLL_PERI0_CTRL_REG);
    set_module(CCU_PLL_VIDEO0_CTRL_REG);
    set_module(CCU_PLL_VIDEO1_CTRL_REG);
    set_module(CCU_PLL_VE_CTRL);
    set_module(CCU_PLL_AUDIO0_CTRL_REG);
    set_module(CCU_PLL_AUDIO1_CTRL_REG);
}


