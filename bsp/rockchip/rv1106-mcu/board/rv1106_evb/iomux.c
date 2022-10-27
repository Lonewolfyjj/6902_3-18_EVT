/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-08     Steven Liu   first implementation
 */

#include "rtdef.h"
#include "iomux.h"
#include "hal_base.h"

/**
 * @brief  Config iomux for RV1106's camera
 */
static void cam_iomux_config(void)
{
    /* Config iomux gpio3c4 :MIPI_REFCLK_OUT0 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK3, GPIO_PIN_C4, PIN_CONFIG_MUX_FUNC2);
    /* Config iomux gpio3A2 : GPIO */
    HAL_GPIO_SetPinDirection(GPIO3, GPIO_PIN_A2, GPIO_OUT);
    HAL_GPIO_SetPinLevel(GPIO3, GPIO_PIN_A2, GPIO_HIGH);
    HAL_PINCTRL_SetIOMUX(GPIO_BANK3, GPIO_PIN_A2, PIN_CONFIG_MUX_FUNC0);
}

/**
 * @brief  Config iomux for RV1106
 */
void rt_hw_iomux_config(void)
{
    i2c4_m2_iomux_config();
    uart2_m0_iomux_config();

    cam_iomux_config();
}
