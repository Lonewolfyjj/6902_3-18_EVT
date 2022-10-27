/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-08     Steven Liu   first implementation
 */

#include <rthw.h>
#include <rtthread.h>

#include "board.h"

void board_softirq_handler(int vector, void *param)
{
    int *soft_irq = param;
    COREGRF->MCU_CACHE_MISC = 0x00080000;
    *soft_irq = 1;
}

