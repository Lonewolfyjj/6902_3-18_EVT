/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
 */

/**
 * @file  hal_conf_template.h
 */

#ifndef _HAL_CONF_H_
#define _HAL_CONF_H_

#include "rtconfig.h"

#define HAL_MCU_CORE

/* Cache maintain need the decoded addr, it must be matched with pre-loader */
#if defined(RKMCU_RK3588_PMU)
#define HAL_CACHE_DECODED_ADDR_BASE          0x200000
#elif defined(RKMCU_RK3588_DDR)
#define HAL_CACHE_DECODED_ADDR_BASE          0x300000  /* Not really necessary */
#elif defined(RKMCU_RK3588_NPU)
#define HAL_CACHE_DECODED_ADDR_BASE          0x400000
#endif

#define SYS_TIMER TIMER0 /* System timer designation (RK TIMER) */


#if defined(RT_USING_CACHE)
#define HAL_DCACHE_MODULE_ENABLED
#define HAL_ICACHE_MODULE_ENABLED
#endif

#ifdef RT_USING_UART
#define HAL_UART_MODULE_ENABLED
#endif

#ifdef RT_USING_WDT
#define HAL_WDT_MODULE_ENABLED
#define HAL_WDT_DYNFREQ_FEATURE_ENABLED
#endif

#ifdef RT_USING_32K_TICK_SRC
#define SYSTICK_EXT_SRC      32768
#else
#define SYSTICK_EXT_SRC      PLL_INPUT_OSC_RATE
#endif

#define HAL_SYSTICK_MODULE_ENABLED
#define HAL_TIMER_MODULE_ENABLED
#define HAL_NVIC_MODULE_ENABLED
#define HAL_MBOX_MODULE_ENABLED

#endif /* _HAL_CONF_H_ */

