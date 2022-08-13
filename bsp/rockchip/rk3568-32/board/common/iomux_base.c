/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-12     Steven Liu   first implementation
 */

#include "rtdef.h"
#include "iomux.h"
#include "hal_base.h"

#ifdef RT_USING_CAN
/**
 * @brief  Config iomux m1 for CAN1
 */
RT_WEAK RT_UNUSED void can1_m1_iomux_config(void)
{
    /* CAN1 M1 RX TX */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK4,
                         GPIO_PIN_C2 |
                         GPIO_PIN_C3,
                         PIN_CONFIG_MUX_FUNC3);
    HAL_PINCTRL_IOFuncSelForCAN1(IOFUNC_SEL_M1);
}
#endif

#ifdef RT_USING_GMAC
#ifdef RT_USING_GMAC0
/**
 * @brief  Config iomux for GMAC0
 */
RT_WEAK RT_UNUSED void gmac0_iomux_config(void)
{
    /* GMAC0 iomux */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK2,
                         GPIO_PIN_C3 | /* gmac0_mdc */
                         GPIO_PIN_C4 | /* gmac0_mdio */
                         GPIO_PIN_C0 | /* gmac0_rxdvcrs */
                         GPIO_PIN_B7 | /* gmac0_rxd1 */
                         GPIO_PIN_A3 | /* gmac0_rxd2 */
                         GPIO_PIN_A4 | /* gmac0_rxd3 */
                         GPIO_PIN_A6 | /* gmac0_rxclk */
                         GPIO_PIN_A7 | /* gmac0_txclk */
                         GPIO_PIN_A5 | /* gmac0_rxclk */
                         PIN_CONFIG_MUX_FUNC2);

    HAL_PINCTRL_SetIOMUX(GPIO_BANK2,
                         GPIO_PIN_B3 | /* gmac0_txd0 */
                         GPIO_PIN_B4 | /* gmac0_txd1 */
                         GPIO_PIN_B5,  /* gmac0_txen */
                         PIN_CONFIG_MUX_FUNC1 | PIN_CONFIG_DRV_LEVEL2);

    HAL_PINCTRL_SetIOMUX(GPIO_BANK2,
                         GPIO_PIN_A7 | /* gmac0_txd3 */
                         GPIO_PIN_A6 | /* gmac0_txd2 */
                         PIN_CONFIG_MUX_FUNC2 | PIN_CONFIG_DRV_LEVEL2);
    HAL_PINCTRL_SetIOMUX(GPIO_BANK2,   /* gmac0_txclk */
                         PIN_CONFIG_MUX_FUNC2 | PIN_CONFIG_DRV_LEVEL1);
}
#endif

#ifdef RT_USING_GMAC1
/**
 * @brief  Config iomux for GMAC1
 */
RT_WEAK RT_UNUSED void gmac1_m1_iomux_config(void)
{
    /* GMAC1 M1 iomux */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK4,
                         GPIO_PIN_B6 | /* gmac1_mdcm1 */
                         GPIO_PIN_B7 | /* gmac1_mdiom1 */
                         GPIO_PIN_B1 | /* gmac1_rxdvcrsm1 */
                         GPIO_PIN_A7 | /* gmac1_rxd0m1 */
                         GPIO_PIN_B0 | /* gmac1_rxd1m1 */
                         GPIO_PIN_A1 | /* gmac1_rxd2m1 */
                         GPIO_PIN_A2 | /* gmac1_rxd3m1 */
                         GPIO_PIN_A6 | /* gmac1_txenm1 */
                         GPIO_PIN_A3,  /* gmac1_rxclkm1 */
                         PIN_CONFIG_MUX_FUNC3);

    HAL_PINCTRL_SetIOMUX(GPIO_BANK4,
                         GPIO_PIN_A0,  /* gmac1_txd1m1 */
                         PIN_CONFIG_MUX_FUNC3 | PIN_CONFIG_DRV_LEVEL3);

    HAL_PINCTRL_SetIOMUX(GPIO_BANK4,
                         GPIO_PIN_A4 | /* gmac1_txd0m1 */
                         GPIO_PIN_A5,  /* gmac1_txd1m1 */
                         PIN_CONFIG_MUX_FUNC3 | PIN_CONFIG_DRV_LEVEL2);

    /* GMAC1 M1 iomux */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK3,
                         GPIO_PIN_D6 | /* gmac1_txd2m1 */
                         GPIO_PIN_D7,  /* gmac1_txd3m1 */
                         PIN_CONFIG_MUX_FUNC3 | PIN_CONFIG_DRV_LEVEL2);

    HAL_PINCTRL_IOFuncSelForGMAC1(IOFUNC_SEL_M1);
}
#endif
#endif

/**
 * @brief  Config iomux m0 for UART2
 */
RT_WEAK RT_UNUSED void uart2_m0_iomux_config(void)
{
    /* UART2 M0 RX-0D0 TX-0D1 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK0,
                         GPIO_PIN_D0 |
                         GPIO_PIN_D1,
                         PIN_CONFIG_MUX_FUNC1);
}

/**
 * @brief  Config io domian for board of rk3568 evb1
 */
RT_WEAK RT_UNUSED void rt_hw_iodomain_config(void)
{
}

/**
 * @brief  Config iomux for RK3568
 */
RT_WEAK RT_UNUSED void rt_hw_iomux_config(void)
{

    rt_hw_iodomain_config();

    uart2_m0_iomux_config();

#ifdef RT_USING_CAN
    can1_m1_iomux_config();
#endif
}
