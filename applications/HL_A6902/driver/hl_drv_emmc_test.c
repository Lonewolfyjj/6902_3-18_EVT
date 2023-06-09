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
#include "hl_config.h"
#include "hl_hal_gpio.h"
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#include "hal_base.h"


struct TGPIO_INFO
{
    char*            desc;
    struct GPIO_REG* gpio;
    rt_uint32_t      mask;
};


static struct TGPIO_INFO gpios[] = {
    { "gpio0", GPIO0, 0x00000000 },
#ifdef GPIO1
    { "gpio1", GPIO1, 0x00000000 },
#endif
#ifdef GPIO2
    { "gpio2", GPIO2, 0x00000000 },
#endif
#ifdef GPIO3
    { "gpio3", GPIO3, 0x00000000 },
#endif
};


static void _tgpio_prepare(uint8_t bank_id, uint32_t pin_id)
{
    if (bank_id > 4) {
        return;
    }
    gpios[bank_id].mask |= 0x01 << pin_id;
    // rt_kprintf("%s: %s to test pins: 0x%lx\n", __func__, gpios[bank_id].desc, pin_id);
    HAL_PINCTRL_SetIOMUX(bank_id, gpios[bank_id].mask, 0);
}


static void _tgpio_output(uint8_t bank_id, uint32_t pin_id, uint8_t level)
{
    _tgpio_prepare(bank_id, pin_id);
    // tgpio_set_direction(GPIO_OUT);
    HAL_GPIO_SetPinsDirection(gpios[bank_id].gpio, gpios[bank_id].mask, GPIO_OUT);
    switch (level) {
        case 0:
            // tgpio_set_level(GPIO_LOW);
            HAL_GPIO_SetPinsLevel(gpios[bank_id].gpio, gpios[bank_id].mask, GPIO_LOW);
            // tgpio_set_param(PIN_CONFIG_DRV_LEVEL0);
            HAL_PINCTRL_SetParam(bank_id, gpios[bank_id].mask, PIN_CONFIG_DRV_LEVEL2);
            break;
        case 1:
            // tgpio_set_level(GPIO_HIGH);
            HAL_GPIO_SetPinsLevel(gpios[bank_id].gpio, gpios[bank_id].mask, GPIO_HIGH);
            // tgpio_set_param(PIN_CONFIG_DRV_LEVEL1);
            HAL_PINCTRL_SetParam(bank_id, gpios[bank_id].mask, PIN_CONFIG_DRV_LEVEL1);
            break;
        case 2:
            // tgpio_set_level(GPIO_HIGH);
            HAL_GPIO_SetPinsLevel(gpios[bank_id].gpio, gpios[bank_id].mask, GPIO_HIGH);
            // tgpio_set_param(PIN_CONFIG_DRV_LEVEL2);
            HAL_PINCTRL_SetParam(bank_id, gpios[bank_id].mask, PIN_CONFIG_DRV_LEVEL2);
            break;
        case 3:
            // tgpio_set_level(GPIO_HIGH);
            HAL_GPIO_SetPinsLevel(gpios[bank_id].gpio, gpios[bank_id].mask, GPIO_HIGH);
            // tgpio_set_param(PIN_CONFIG_DRV_LEVEL3);
            HAL_PINCTRL_SetParam(bank_id, gpios[bank_id].mask, PIN_CONFIG_DRV_LEVEL2);
            break;
        default:
            rt_kprintf("wrong pinbank\r\n");
            break;
    }
}
#if HL_IS_TX_DEVICE()

int hl_drv_emmc_gpio_init(void)
{
    rt_kprintf("emmc init/r/n");
    hl_hal_gpio_init(GPIO_EMMC_RST);
    //hl_hal_gpio_init(GPIO_EMMC_PWR_EN);


    hl_hal_gpio_low(GPIO_EMMC_RST);
    //hl_hal_gpio_high(GPIO_EMMC_PWR_EN);

    //  _tgpio_output(0, GPIO0_A3, 3);          // hl_hal_gpio_high(GPIO_EMMC_RST);
    //  // _tgpio_output(0, GPIO0_D3, 3);       // hl_hal_gpio_high(GPIO_LVT_EN);
    // _tgpio_output(1, GPIO0_A5, 3);          // hl_hal_gpio_high(GPIO_EMMC_PWR_EN);
    // _tgpio_output(0, GPIO0_C6, 3);          // hl_hal_gpio_high(GPIO_PWR_EN);
    // _tgpio_output(1, GPIO0_A2, 3);          // hl_hal_gpio_high(GPIO_DC3V3_EN);
    // _tgpio_output(1, GPIO0_A7, 3);          // hl_hal_gpio_high(GPIO_2831P_EN);
    // _tgpio_output(1, GPIO0_C4, 3);          // hl_hal_gpio_high(GPIO_RF_PWR_EN);
    // _tgpio_output(1, GPIO0_C6, 0);       // hl_hal_gpio_low(GPIO_MIC_SW);
    // _tgpio_output(1, GPIO0_C7, 3);          // hl_hal_gpio_low(GPIO_ATS_RESET);
    // hl_hal_gpio_init(GPIO_TEST);
    // hl_hal_gpio_high(GPIO_TEST);

    return RT_EOK;
}

INIT_BOARD_EXPORT(hl_drv_emmc_gpio_init);


void gpio_emmc_l(int argc, char **argv)
{
    //_tgpio_output(1, GPIO0_A5, 0);          // hl_hal_gpio_high(GPIO_EMMC_PWR_EN);
    _tgpio_output(0, GPIO0_A3, 0);          // hl_hal_gpio_high(GPIO_EMMC_RST);
}
MSH_CMD_EXPORT(gpio_emmc_l, gpio_emmc);


void gpio_emmc_h(int argc, char **argv)
{
    //_tgpio_output(1, GPIO0_A5, 0);          // hl_hal_gpio_high(GPIO_EMMC_PWR_EN);
    _tgpio_output(0, GPIO0_A3, 3);          // hl_hal_gpio_high(GPIO_EMMC_RST);
}
MSH_CMD_EXPORT(gpio_emmc_h, gpio_emmc);


void gpio_emmc_1l(int argc, char **argv)
{
    //_tgpio_output(1, GPIO0_A5, 0);          // hl_hal_gpio_high(GPIO_EMMC_PWR_EN);
    //_tgpio_output(0, GPIO0_A3, 3);          // hl_hal_gpio_high(GPIO_EMMC_RST);
    hl_hal_gpio_init(GPIO_EMMC_RST);
    hl_hal_gpio_low(GPIO_EMMC_RST);
}
MSH_CMD_EXPORT(gpio_emmc_1l, gpio_emmc);
void gpio_emmc_1h(int argc, char **argv)
{
    //_tgpio_output(1, GPIO0_A5, 0);          // hl_hal_gpio_high(GPIO_EMMC_PWR_EN);
    //_tgpio_output(0, GPIO0_A3, 3);          // hl_hal_gpio_high(GPIO_EMMC_RST);
    hl_hal_gpio_init(GPIO_EMMC_RST);
    hl_hal_gpio_high(GPIO_EMMC_RST);
}
MSH_CMD_EXPORT(gpio_emmc_1h, gpio_emmc);
#endif

/*
 * EOF
 */