/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    iomux.c
  * @version V0.1
  * @brief   iomux for rk2108 avb
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-05-25     Cliff.Chen      first implementation
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Board_Driver
 *  @{
 */

/** @addtogroup IOMUX
 *  @{
 */

/** @defgroup How_To_Use How To Use
 *  @{
 @verbatim

 ==============================================================================
                    #### How to use ####
 ==============================================================================
 This file provide IOMUX for board, it will be invoked when board initialization.

 @endverbatim
 @} */
#include "rtdef.h"
#include "iomux.h"
#include "hal_base.h"

/********************* Private MACRO Definition ******************************/
/** @defgroup IOMUX_Private_Macro Private Macro
 *  @{
 */

/** @} */  // IOMUX_Private_Macro

/********************* Private Structure Definition **************************/
/** @defgroup IOMUX_Private_Structure Private Structure
 *  @{
 */

/** @} */  // IOMUX_Private_Structure

/********************* Private Variable Definition ***************************/
/** @defgroup IOMUX_Private_Variable Private Variable
 *  @{
 */

/** @} */  // IOMUX_Private_Variable

/********************* Private Function Definition ***************************/
/** @defgroup IOMUX_Private_Function Private Function
 *  @{
 */

/** @} */  // IOMUX_Private_Function

/********************* Public Function Definition ****************************/

/** @defgroup IOMUX_Public_Functions Public Functions
 *  @{
 */

/**
 * @brief  Config iomux for rk2108 avb board
 */

/**
 * @brief  Config iomux for AUDIO PWM M0
 */

void sdio_iomux_config(void)
{
    HAL_PINCTRL_SetIOMUX(GPIO_BANK0,
                         GPIO_PIN_C0 |  // SDIO_CLK
                         GPIO_PIN_C1 |  // SDIO_CMD
                         GPIO_PIN_C2 |  // SDIO_D0
                         GPIO_PIN_C3 |  // SDIO_D1
                         GPIO_PIN_C4,   // SDIO_D2
                         PIN_CONFIG_MUX_FUNC4);

    HAL_PINCTRL_SetIOMUX(GPIO_BANK0,
                         GPIO_PIN_C5,   // SDIO_D3
                         PIN_CONFIG_MUX_FUNC3);

    HAL_PINCTRL_SetParam(GPIO_BANK0,
                         GPIO_PIN_C1 |  // SDIO_CMD
                         GPIO_PIN_C2 |  // SDIO_D0
                         GPIO_PIN_C3 |  // SDIO_D1
                         GPIO_PIN_C4 |  // SDIO_D2
                         GPIO_PIN_C5,   // SDIO_D3
                         PIN_CONFIG_PUL_UP |
                         PIN_CONFIG_DRV_LEVEL1);
    HAL_PINCTRL_SetParam(GPIO_BANK0, GPIO_PIN_A3, PIN_CONFIG_PUL_UP); //Emmc_Reset
    HAL_GPIO_SetPinDirection(GPIO0, GPIO_PIN_A3, GPIO_OUT);
    HAL_GPIO_SetPinLevel(GPIO0, GPIO_PIN_A3, GPIO_HIGH);
}

void audio_iomux_config(void)
{
    HAL_PINCTRL_SetIOMUX(GPIO_BANK0,
                         GPIO_PIN_D1 |  // AUDIO_LOUT_M0
                         GPIO_PIN_D2,   // AUDIO_ROUT_M0
                         PIN_CONFIG_MUX_FUNC3);
}

void rt_hw_iomux_config(void)
{
    pdm_input_iomux_config();

    sfc0_iomux_config();

    sdio_iomux_config();

    uart2_iomux_config();

    uart1_m0_iomux_config();

    uart0_iomux_config();

    i2c0_m2_iomux_config();

    i2c1_m1_iomux_config();

    i2c2_m2_iomux_config();
#ifdef RT_USING_I2STDM1
    i2s1_output_iomux_config();
#endif
#ifdef RT_USING_I2STDM
    i2s0_input_iomux_config();
#endif
#ifdef RT_USING_AUDIOPWM
    audio_iomux_config();
#endif
}
/** @} */  // IOMUX_Public_Functions

/** @} */  // IOMUX

/** @} */  // RKBSP_Board_Driver
