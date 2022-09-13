/**
 * @file hl_hal_gpio_test.c
 * @author lilin (lin.li@hollyland-tech.com)
 * @brief gpio测试源文件
 * @version 1.0
 * @date 2022-09-13
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-13     <td>v1.0     <td>lilin     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/

#include <rtthread.h>

#include "hl_hal_gpio.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#if HL_GET_DEVICE_TYPE()

int gpio_test_init(void)
{
    hl_hal_gpio_init(GPIO_EMMC_RST);
    hl_hal_gpio_init(GPIO_LVT_EN);
    hl_hal_gpio_init(GPIO_EMMC_PWR_EN);
    hl_hal_gpio_init(GPIO_PWR_EN);
    hl_hal_gpio_init(GPIO_DC3V3_EN);
    hl_hal_gpio_init(GPIO_2831P_EN);
    hl_hal_gpio_init(GPIO_RF_PWR_EN);
    hl_hal_gpio_init(GPIO_MIC_SW);
    hl_hal_gpio_init(GPIO_ATS_RESET);

    hl_hal_gpio_high(GPIO_EMMC_RST);
    hl_hal_gpio_high(GPIO_LVT_EN);
    hl_hal_gpio_high(GPIO_EMMC_PWR_EN);
    hl_hal_gpio_high(GPIO_PWR_EN);
    hl_hal_gpio_high(GPIO_DC3V3_EN);
    hl_hal_gpio_high(GPIO_2831P_EN);
    hl_hal_gpio_high(GPIO_RF_PWR_EN);
    hl_hal_gpio_low(GPIO_MIC_SW);
    hl_hal_gpio_low(GPIO_ATS_RESET);

    return RT_EOK;
}

INIT_APP_EXPORT(gpio_test_init);

#endif
/*
 * EOF
 */