/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    board.c
  * @author  Jason Zhu
  * @version V0.1
  * @date    1-Aug-2019
  * @brief
  *
  ******************************************************************************
  */

#include <rtthread.h>
#include <rthw.h>
#include "hal_base.h"
#include "hal_bsp.h"
#include "board.h"
#include "drv_cache.h"
#include "drv_uart.h"
#include "iomux.h"

#ifdef RT_USING_CRU
#include "drv_clock.h"
#endif

void write_reg(uint32_t addr, uint32_t val)
{
    volatile uint32_t *pVal;

    pVal = (uint32_t *)addr;
    *pVal = val;
}


#ifdef RT_USING_CRU

RT_WEAK const struct clk_init clk_inits[] =
{
    { /* sentinel */ },
};

RT_WEAK const struct clk_unused clks_unused[] =
{
    { /* sentinel */ },
};
#endif
#if defined(RT_USING_UART0)
RT_WEAK const struct uart_board g_uart0_board =
{
    .baud_rate = UART_BR_1500000,
    .dev_flag = ROCKCHIP_UART_SUPPORT_FLAG_DEFAULT,
    .bufer_size = RT_SERIAL_RB_BUFSZ,
    .name = "uart0",
};
#endif /* RT_USING_UART0 */
#if defined(RT_USING_UART2)
RT_WEAK const struct uart_board g_uart2_board =
{
    .baud_rate = UART_BR_1500000,
    .dev_flag = ROCKCHIP_UART_SUPPORT_FLAG_DEFAULT,
    .bufer_size = RT_SERIAL_RB_BUFSZ,
    .name = "uart2",
};
#endif /* RT_USING_UART2 */

extern void SysTick_Handler(void);
RT_WEAK void tick_isr(int vector, void *param)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_IncTick();
    rt_tick_increase();
#ifdef RT_USING_CORE_FREERTOS
    SysTick_Handler();
#endif
#ifdef TICK_TIMER
    HAL_TIMER_ClrInt(TICK_TIMER);
#endif

    /* leave interrupt */
    rt_interrupt_leave();
}

extern const uint32_t __sram_heap_start__[];
extern const uint32_t __sram_heap_end__[];

void interrupt_prio_init(void)
{
    /* Improve the interrupt priority of Buffer Manager and USB to reduce overflow */
}

static void rt_idle_wfi(void)
{
    rt_ubase_t level;

    asm volatile("MRS     %0, PRIMASK\n"
                 "CPSID   I"
                 : "=r"(level) : : "memory", "cc");

    asm volatile("dsb");
    asm volatile("wfi");

    asm volatile("MSR     PRIMASK, %0"
                 : : "r"(level) : "memory", "cc");
}

int rt_hw_board_init(void)
{
    /* HAL_Init */
    HAL_Init();

    /* hal bsp init */
    BSP_Init();

    /* interrupt init */
    rt_hw_interrupt_init();

    /* interrupt priority init */
    interrupt_prio_init();

    /* tick init */
    HAL_SetTickFreq(1000 / RT_TICK_PER_SECOND);
    rt_hw_interrupt_install(SysTick_IRQn, tick_isr, RT_NULL, "tick");
    rt_hw_interrupt_umask(SysTick_IRQn);
    HAL_NVIC_SetPriority(SysTick_IRQn, NVIC_PERIPH_PRIO_LOWEST, NVIC_PERIPH_SUB_PRIO_LOWEST);
    HAL_SYSTICK_CLKSourceConfig(HAL_SYSTICK_CLKSRC_EXT);
    HAL_SYSTICK_Config((SYSTICK_EXT_SRC / RT_TICK_PER_SECOND) - 1);
    HAL_SYSTICK_Enable();

#ifdef RT_USING_CACHE
    rt_hw_cpu_cache_init();
#endif

#ifdef RT_USING_PIN
    rt_hw_iomux_config();
#endif
    write_reg(0xFD5F4000, 0xffff8888);
    write_reg(0xFD5F800C, 0x0ff00aa0);

#ifdef RT_USING_CRU
    clk_init(clk_inits, false);
    clk_disable_unused(clks_unused);
#endif

    rt_system_heap_init((void *)__sram_heap_start__, (void *)__sram_heap_end__);

#ifdef RT_USING_UART
    rt_hw_usart_init();
#endif
#ifdef RT_USING_CONSOLE
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif

    rt_thread_idle_sethook(rt_idle_wfi);

    rt_components_board_init();

    return 0;
}
