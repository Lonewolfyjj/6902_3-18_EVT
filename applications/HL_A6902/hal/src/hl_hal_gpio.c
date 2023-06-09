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
typedef struct _hl_gpio_config_t
{
    /// @brief 引脚号
    rt_base_t    pin;

    /// @brief  引脚模式
    rt_base_t    mode;
} hl_gpio_config_t;

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
// notice: The array element sequence must be the same as HL_GPIO PORT_E

#if HL_IS_TX_DEVICE()
// Tx
static const hl_gpio_config_t gpio_config_table[USER_GPIO_COUNT] =
{
    // IN
    {GPIO1_C0,  PIN_MODE_INPUT},            // GPIO_PAIR_KEY
    {GPIO1_B2,  PIN_MODE_INPUT},            // GPIO_PWR_KEY
    {GPIO1_C1,  PIN_MODE_INPUT},            // GPIO_REC_KEY
    {GPIO1_A4,  PIN_MODE_INPUT_PULLDOWN},   // GPIO_VBUS_DET
    {GPIO1_B1,  PIN_MODE_INPUT},            // GPIO_GAUGE_INT
    {GPIO1_C5,  PIN_MODE_INPUT},   // GPIO_MIC_DET
    {GPIO0_D6,  PIN_MODE_INPUT},    // GPIO_PBUS_DET
    // OUT
    {GPIO0_A3,  PIN_MODE_OUTPUT},           // GPIO_EMMC_RST
    // {GPIO1_A5,  PIN_MODE_OUTPUT},           // GPIO_EMMC_PWR_EN
    // {GPIO0_C6,  PIN_MODE_OUTPUT},           // GPIO_PWR_EN
    {GPIO1_B0,  PIN_MODE_OUTPUT},           // GPIO_DC3V3_EN
    {GPIO1_A7,  PIN_MODE_OUTPUT},           // GPIO_2831P_EN
    {GPIO1_C4,  PIN_MODE_OUTPUT},           // GPIO_RF_PWR_EN
    {GPIO1_C6,  PIN_MODE_OUTPUT},           // GPIO_MIC_SW
    {GPIO1_C7,  PIN_MODE_OUTPUT},           // GPIO_ATS_RESET
    {GPIO0_D7,  PIN_MODE_OUTPUT},           // GPIO_ALL_POWER
    {GPIO1_A5,  PIN_MODE_OUTPUT},           // GPIO_REC_LED_EN

    {GPIO0_A4,  PIN_MODE_INPUT},           // I2S1_MCLK
    {GPIO0_A6,  PIN_MODE_INPUT},           // I2S1_SCLK
    {GPIO0_A7,  PIN_MODE_INPUT},           // I2S1_LCLK
    {GPIO0_B0,  PIN_MODE_INPUT},           // I2S1_DTATE
    {GPIO0_D3,  PIN_MODE_INPUT},           // I2S0_SCLK
    {GPIO0_D4,  PIN_MODE_INPUT},           // I2S0_LCLK
    {GPIO0_D5,  PIN_MODE_INPUT},           // I2S0_DTATE
};
#else
// RX

static const hl_gpio_config_t gpio_config_table[USER_GPIO_COUNT] =
{
    // IN
    {GPIO1_B1,  PIN_MODE_INPUT},            // GPIO_PWR_KEY
    {GPIO0_A6,  PIN_MODE_INPUT},            // GPIO_L_VOL_A
    {GPIO0_A5,  PIN_MODE_INPUT},            // GPIO_L_VOL_B
    {GPIO0_A7,  PIN_MODE_INPUT},            // GPIO_L_VOL_KEY
    {GPIO0_A3,  PIN_MODE_INPUT},            // GPIO_GAUGE_INT
    {GPIO1_A4,  PIN_MODE_INPUT_PULLDOWN},   // GPIO_VBUS_DET
    {GPIO0_C1,  PIN_MODE_INPUT_PULLUP},     // GPIO_HP_DET
    {GPIO1_A5,  PIN_MODE_INPUT_PULLUP},     // GPIO_CAM_DET
    {GPIO1_B2,  PIN_MODE_INPUT},            // GPIO_GSENSOR_INT
    {GPIO1_C0,  PIN_MODE_INPUT},            // GPIO_OLED_TE
    {GPIO1_C1,  PIN_MODE_INPUT},            // GPIO_TP_INT
    {GPIO0_C4,  PIN_MODE_INPUT},            // GPIO_PBUS_DET
    // OUT
    {GPIO0_C0,  PIN_MODE_OUTPUT},           // GPIO_OLED_RST
    {GPIO1_A7,  PIN_MODE_OUTPUT},           // GPIO_RF_PWR_EN
    {GPIO1_B0,  PIN_MODE_OUTPUT},           // GPIO_ATS_RESET
    {GPIO1_C2,  PIN_MODE_OUTPUT},           // GPIO_TP_RST
    {GPIO1_C3,  PIN_MODE_OUTPUT},           // GPIO_CODEC_EN
    {GPIO1_C7,  PIN_MODE_OUTPUT},           // GPIO_OLED_DCX
    {GPIO0_D5,  PIN_MODE_OUTPUT},           // GPIO_OLED_SWIRE
    {GPIO0_D7,  PIN_MODE_OUTPUT},           // GPIO_ALL_POWER

    {GPIO0_A4,  PIN_MODE_INPUT},           // I2S1_MCLK
    {GPIO0_A0,  PIN_MODE_INPUT},           // I2S1_SCLK
    {GPIO0_A1,  PIN_MODE_INPUT},           // I2S1_LCLK
    {GPIO0_A2,  PIN_MODE_INPUT},           // I2S1_DTATE
    {GPIO0_D3,  PIN_MODE_INPUT},           // I2S0_SCLK
    {GPIO0_D4,  PIN_MODE_INPUT},           // I2S0_LCLK
    {GPIO0_D6,  PIN_MODE_INPUT},           // I2S0_DTATE
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