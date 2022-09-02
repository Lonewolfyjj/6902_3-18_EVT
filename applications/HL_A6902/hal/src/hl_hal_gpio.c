/**
 * @file hl_hal_gpio.c
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-31
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-08-31     <td>v1.0     <td>luzhanghao     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include <rtdevice.h>
#include "hal_gpio.h"
#include "hl_hal_gpio.h"
#include "hl_config.h"

/* typedef -------------------------------------------------------------------*/
// gpio config struct
typedef struct _HL_GPIO_CONFIG_T
{
    rt_base_t    pin;
    rt_base_t    mode;
}HL_GPIO_CONFIG_T;

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
// notice: The array element sequence must be the same as HL_GPIO PORT_E

#if HL_GET_DEVICE_TYPE()
// Tx
static const HL_GPIO_CONFIG_T gpio_config_table[USER_GPIO_COUNT] =
{
    // IN
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_A3),  PIN_MODE_INPUT},    // GPIO_PAIR_KEY
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_B2),  PIN_MODE_INPUT},    // GPIO_PWR_KEY
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_C1),  PIN_MODE_INPUT},    // GPIO_REC_KEY
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_A4),  PIN_MODE_INPUT},    // GPIO_VBUS_DET
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_B1),  PIN_MODE_INPUT},    // GPIO_GAUGE_INT
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_C5),  PIN_MODE_INPUT},    // GPIO_MIC_DET
    // OUT
    {BANK_PIN(GPIO_BANK0, GPIO_PIN_A3),  PIN_MODE_OUTPUT},   // GPIO_EMMC_RST
    {BANK_PIN(GPIO_BANK0, GPIO_PIN_D3),  PIN_MODE_OUTPUT},   // GPIO_LVT_EN
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_A5),  PIN_MODE_OUTPUT},   // GPIO_EMMC_PWR_EN
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_C6),  PIN_MODE_OUTPUT},   // GPIO_PWR_EN
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_A2),  PIN_MODE_OUTPUT},   // GPIO_DC3V3_EN
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_A7),  PIN_MODE_OUTPUT},   // GPIO_2831P_EN
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_C4),  PIN_MODE_OUTPUT},   // GPIO_RF_PWR_EN
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_C6),  PIN_MODE_OUTPUT},   // GPIO_MIC_SW
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_C7),  PIN_MODE_OUTPUT},   // GPIO_ATS_RESET
};
#else
// RX
static const HL_GPIO_CONFIG_T gpio_config_table[USER_GPIO_COUNT] =
{
    // IN
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_B1),  PIN_MODE_INPUT},    // GPIO_PWR_KEY       (开关按键，外部上拉，低电平有效)
    {BANK_PIN(GPIO_BANK0, GPIO_PIN_A6),  PIN_MODE_INPUT},    // GPIO_L_VOL_A       (左旋钮A，外部上拉，低电平有效)
    {BANK_PIN(GPIO_BANK0, GPIO_PIN_A5),  PIN_MODE_INPUT},    // GPIO_L_VOL_B       (左旋钮B，外部上拉，低电平有效)
    {BANK_PIN(GPIO_BANK0, GPIO_PIN_A7),  PIN_MODE_INPUT},    // GPIO_L_VOL_KEY     (左旋钮按键，外部上拉，低电平有效)
    {BANK_PIN(GPIO_BANK0, GPIO_PIN_C0),  PIN_MODE_INPUT},    // GPIO_R_VOL_A       (左旋钮A，外部上拉，低电平有效)
    {BANK_PIN(GPIO_BANK0, GPIO_PIN_C5),  PIN_MODE_INPUT},    // GPIO_R_VOL_B       (左旋钮B，外部上拉，低电平有效)
    {BANK_PIN(GPIO_BANK0, GPIO_PIN_C6),  PIN_MODE_INPUT},    // GPIO_R_VOL_KEY     (左旋钮按键，外部上拉，低电平有效)
    {BANK_PIN(GPIO_BANK0, GPIO_PIN_A3),  PIN_MODE_INPUT},    // GPIO_GAUGE_INT     (电量计中断，外部上拉，低电平有效)
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_A4),  PIN_MODE_INPUT},    // GPIO_VBUS_DET      (usb插入检测，外部下拉，高电平有效)
    {BANK_PIN(GPIO_BANK0, GPIO_PIN_C1),  PIN_MODE_INPUT},    // GPIO_HP_DET        (耳机插入检测，外部上拉，低电平有效)
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_B2),  PIN_MODE_INPUT},    // GPIO_GSENSOR_INT   (加速度传感器中断脚，外部上拉，低电平有效)
    // OUT
    {BANK_PIN(GPIO_BANK0, GPIO_PIN_B0),  PIN_MODE_OUTPUT},   // GPIO_PWR_EN        (电源使能，无外部上下拉，高电平有效)
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_B5),  PIN_MODE_OUTPUT},   // GPIO_USB_SW        (USB切换开关，外部下拉，低：RK USB 高：2831P USB)
    {BANK_PIN(GPIO_BANK0, GPIO_PIN_C0),  PIN_MODE_OUTPUT},   // GPIO_OLED_RST      (OLED RST脚，无外部上下拉，低电平有效)
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_A7),  PIN_MODE_OUTPUT},   // GPIO_RF_PWR_EN     (RF电源使能，无外部上下拉，高电平有效)
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_B0),  PIN_MODE_OUTPUT},   // GPIO_ATS_RESET     (电池供电使能，外部上下拉都有，高电平有效)
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_C2),  PIN_MODE_OUTPUT},   // GPIO_ATS_PWR_EN    (ATS供电使能，外部上下拉都有，高电平有效)
    {BANK_PIN(GPIO_BANK1, GPIO_PIN_C3),  PIN_MODE_OUTPUT},   // GPIO_AMP_EN        (放大器使能，无外部上下拉，低电平有效)
};
#endif


/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
rt_err_t hl_hal_gpio_init(hl_gpio_pin_e gpio_index)
{
    rt_pin_mode(gpio_config_table[gpio_index].pin, gpio_config_table[gpio_index].mode);
    return RT_EOK;
}

rt_err_t hl_hal_gpio_deinit(hl_gpio_pin_e gpio_index)
{
    rt_pin_mode(gpio_config_table[gpio_index].pin, PIN_MODE_INPUT);
    return RT_EOK;
}

void hl_hal_gpio_high(hl_gpio_pin_e gpio_index)
{
    rt_pin_write(gpio_config_table[gpio_index].pin, PIN_HIGH);
}

void hl_hal_gpio_low(hl_gpio_pin_e gpio_index)
{
    rt_pin_write(gpio_config_table[gpio_index].pin, PIN_LOW);
}

int hl_hal_gpio_read(hl_gpio_pin_e gpio_index)
{
    return rt_pin_read(gpio_config_table[gpio_index].pin);
}

rt_err_t hl_hal_gpio_attach_irq(hl_gpio_pin_e gpio_index, rt_uint32_t mode, void (*hdr)(void *args), void *args)
{
    return rt_pin_attach_irq(gpio_config_table[gpio_index].pin, mode, hdr, args);
}

rt_err_t hl_hal_gpio_detach_irq(hl_gpio_pin_e gpio_index)
{
    return rt_pin_detach_irq(gpio_config_table[gpio_index].pin);
}

rt_err_t hl_hal_gpio_irq_enable(hl_gpio_pin_e gpio_index, rt_uint32_t enabled)
{
    return rt_pin_irq_enable(gpio_config_table[gpio_index].pin, enabled);
}

/*
 * EOF
 */