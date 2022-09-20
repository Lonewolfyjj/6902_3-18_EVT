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
    {GPIO1_A3,  PIN_MODE_INPUT},            // GPIO_PAIR_KEY
    {GPIO1_B2,  PIN_MODE_INPUT},            // GPIO_PWR_KEY
    {GPIO1_C1,  PIN_MODE_INPUT},            // GPIO_REC_KEY
    {GPIO1_A4,  PIN_MODE_INPUT_PULLDOWN},   // GPIO_VBUS_DET
    {GPIO1_B1,  PIN_MODE_INPUT_PULLDOWN},   // GPIO_GAUGE_INT
    {GPIO1_C5,  PIN_MODE_INPUT_PULLDOWN},   // GPIO_MIC_DET
    // OUT
    {GPIO0_A3,  PIN_MODE_OUTPUT},           // GPIO_EMMC_RST
    {GPIO1_A5,  PIN_MODE_OUTPUT},           // GPIO_EMMC_PWR_EN
    {GPIO0_C6,  PIN_MODE_OUTPUT},           // GPIO_PWR_EN
    {GPIO1_A2,  PIN_MODE_OUTPUT},           // GPIO_DC3V3_EN
    {GPIO1_A7,  PIN_MODE_OUTPUT},           // GPIO_2831P_EN
    {GPIO1_C4,  PIN_MODE_OUTPUT},           // GPIO_RF_PWR_EN
    {GPIO1_C6,  PIN_MODE_OUTPUT},           // GPIO_MIC_SW
    {GPIO1_C7,  PIN_MODE_OUTPUT},           // GPIO_ATS_RESET
};
#else
// RX
static const HL_GPIO_CONFIG_T gpio_config_table[USER_GPIO_COUNT] =
{
    // IN
    {GPIO1_B1,  PIN_MODE_INPUT},            // GPIO_PWR_KEY
    {GPIO0_A6,  PIN_MODE_INPUT},            // GPIO_L_VOL_A
    {GPIO0_A5,  PIN_MODE_INPUT},            // GPIO_L_VOL_B
    {GPIO0_A7,  PIN_MODE_INPUT},            // GPIO_L_VOL_KEY
    {GPIO0_C0,  PIN_MODE_INPUT},            // GPIO_R_VOL_A
    {GPIO0_C5,  PIN_MODE_INPUT},            // GPIO_R_VOL_B
    {GPIO0_C6,  PIN_MODE_INPUT},            // GPIO_R_VOL_KEY
    {GPIO0_A3,  PIN_MODE_INPUT},            // GPIO_GAUGE_INT
    {GPIO1_A4,  PIN_MODE_INPUT},            // GPIO_VBUS_DET
    {GPIO0_C1,  PIN_MODE_INPUT},            // GPIO_HP_DET
    {GPIO1_B2,  PIN_MODE_INPUT},            // GPIO_GSENSOR_INT
    // OUT
    {GPIO0_B0,  PIN_MODE_OUTPUT},           // GPIO_PWR_EN
    {GPIO1_A5,  PIN_MODE_OUTPUT},           // GPIO_USB_SW
    {GPIO0_C0,  PIN_MODE_OUTPUT},           // GPIO_OLED_RST
    {GPIO1_A7,  PIN_MODE_OUTPUT},           // GPIO_RF_PWR_EN
    {GPIO1_B0,  PIN_MODE_OUTPUT},           // GPIO_ATS_RESET
    {GPIO1_C2,  PIN_MODE_OUTPUT},           // GPIO_ATS_PWR_EN
    {GPIO1_C3,  PIN_MODE_OUTPUT},           // GPIO_AMP_EN

    {GPIO1_A4,  PIN_MODE_OUTPUT},           // GPIO_LED_TEST
    {GPIO1_B6,  PIN_MODE_INPUT},            // GPIO_KEY_TEST
};
#endif

/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
rt_err_t hl_hal_gpio_init(hl_gpio_pin_e gpio_index)
{
    RT_ASSERT(gpio_index < USER_GPIO_COUNT);
    rt_pin_mode(gpio_config_table[gpio_index].pin, gpio_config_table[gpio_index].mode);
    return RT_EOK;
}

rt_err_t hl_hal_gpio_deinit(hl_gpio_pin_e gpio_index)
{
    RT_ASSERT(gpio_index < USER_GPIO_COUNT);
    rt_pin_mode(gpio_config_table[gpio_index].pin, PIN_MODE_INPUT);
    return RT_EOK;
}

void hl_hal_gpio_high(hl_gpio_pin_e gpio_index)
{
    RT_ASSERT(gpio_index < USER_GPIO_COUNT);
    rt_pin_write(gpio_config_table[gpio_index].pin, PIN_HIGH);
}

void hl_hal_gpio_low(hl_gpio_pin_e gpio_index)
{
    RT_ASSERT(gpio_index < USER_GPIO_COUNT);
    rt_pin_write(gpio_config_table[gpio_index].pin, PIN_LOW);
}

int hl_hal_gpio_read(hl_gpio_pin_e gpio_index)
{
    RT_ASSERT(gpio_index < USER_GPIO_COUNT);
    return rt_pin_read(gpio_config_table[gpio_index].pin);
}

rt_err_t hl_hal_gpio_attach_irq(hl_gpio_pin_e gpio_index, rt_uint32_t mode, void (*hdr)(void *args), void *args)
{
    RT_ASSERT(gpio_index < USER_GPIO_COUNT);
    return rt_pin_attach_irq(gpio_config_table[gpio_index].pin, mode, hdr, args);
}

rt_err_t hl_hal_gpio_detach_irq(hl_gpio_pin_e gpio_index)
{
    RT_ASSERT(gpio_index < USER_GPIO_COUNT);
    return rt_pin_detach_irq(gpio_config_table[gpio_index].pin);
}

rt_err_t hl_hal_gpio_irq_enable(hl_gpio_pin_e gpio_index, rt_uint32_t enabled)
{
    RT_ASSERT(gpio_index < USER_GPIO_COUNT);
    return rt_pin_irq_enable(gpio_config_table[gpio_index].pin, enabled);
}

/*
 * EOF
 */