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

#if HL_GET_DEVICE_TYPE()
// Tx
static const hl_gpio_config_t gpio_config_table[USER_GPIO_COUNT] =
{
    // IN
    {GPIO1_C0,  PIN_MODE_INPUT},            //0 GPIO_PAIR_KEY 新版Tx GPIO1_A3：用作I2C_MST0_SDA_M2  GPIO1_C0：用作GPIO_PAIR_KEY
    {GPIO1_B2,  PIN_MODE_INPUT},            //1 GPIO_PWR_KEY
    {GPIO1_C1,  PIN_MODE_INPUT},            //2 GPIO_REC_KEY
    {GPIO1_A4,  PIN_MODE_INPUT_PULLDOWN},   //3 GPIO_VBUS_DET
    {GPIO1_B1,  PIN_MODE_INPUT},            //4 GPIO_GAUGE_INT
    {GPIO1_C5,  PIN_MODE_INPUT_PULLDOWN},   //5 GPIO_MIC_DET
    // OUT
    {GPIO0_A3,  PIN_MODE_OUTPUT},           //6 GPIO_EMMC_RST
    {GPIO1_A5,  PIN_MODE_OUTPUT},           //7 GPIO_EMMC_PWR_EN
    {GPIO0_C6,  PIN_MODE_OUTPUT},           //8 GPIO_PWR_EN
    {GPIO1_B0,  PIN_MODE_OUTPUT},           //9 GPIO_DC3V3_EN 新版Tx GPIO1_A2：用作I2C_MST0_SCL_M2  GPIO1_B0：用作 EN1_DC_3V3
    {GPIO1_A7,  PIN_MODE_OUTPUT},           //10 GPIO_2831P_EN
    {GPIO1_C4,  PIN_MODE_OUTPUT},           //11 GPIO_RF_PWR_EN
    {GPIO1_C6,  PIN_MODE_OUTPUT},           //12 GPIO_MIC_SW
    {GPIO1_C7,  PIN_MODE_OUTPUT},           //13 GPIO_ATS_RESET
    {GPIO0_D7,  PIN_MODE_OUTPUT},           //14 GPIO_ALL_PWR
};
#else
// RX
static const hl_gpio_config_t gpio_config_table[USER_GPIO_COUNT] =
{
    // IN
    {GPIO1_B1,  PIN_MODE_OUTPUT},            //0 GPIO_PWR_KEY
    {GPIO0_A6,  PIN_MODE_OUTPUT},            //1 GPIO_L_VOL_A
    {GPIO0_A5,  PIN_MODE_OUTPUT},            //2 GPIO_L_VOL_B
    {GPIO0_A7,  PIN_MODE_OUTPUT},            //3 GPIO_L_VOL_KEY
    {GPIO0_C4,  PIN_MODE_OUTPUT},            //4 GPIO_R_VOL_A 旧版Rx 对照原理图这里应该是 GPIO0_C4，GPIO0_C0是oled复位引脚 新版Rx 用作 VBUS_P_DET 检测
    {GPIO0_C5,  PIN_MODE_OUTPUT},            //5 GPIO_R_VOL_B 新版Rx 悬空未使用
    {GPIO0_C6,  PIN_MODE_OUTPUT},            //6 GPIO_R_VOL_KEY 新版Rx 用作 LCD_PWR_EN 功能，应考虑使用OUTPUT工作模式
    {GPIO0_A3,  PIN_MODE_OUTPUT},            //7 GPIO_GAUGE_INT
    {GPIO1_A4,  PIN_MODE_OUTPUT},            //8 GPIO_VBUS_DET
    {GPIO0_C1,  PIN_MODE_OUTPUT},            //9 GPIO_HP_DET
    {GPIO1_B2,  PIN_MODE_OUTPUT},            //10 GPIO_GSENSOR_INT
    {GPIO1_C0,  PIN_MODE_OUTPUT},            //11 GPIO_OLED_TE
    // OUT
    {GPIO0_B0,  PIN_MODE_OUTPUT},           //12 GPIO_PWR_EN 新版Rx 已去掉该引脚功能
    {GPIO1_A5,  PIN_MODE_OUTPUT},           //13 GPIO_USB_SW 新版Rx SGM7222芯片新版原理图已经去掉，更改为检测外部麦克风插入检测相关的引脚
    {GPIO0_C0,  PIN_MODE_OUTPUT},           //14 GPIO_OLED_RST
    {GPIO1_A7,  PIN_MODE_OUTPUT},           //15 GPIO_RF_PWR_EN
    {GPIO1_B0,  PIN_MODE_OUTPUT},           //16 GPIO_ATS_RESET
    {GPIO1_C2,  PIN_MODE_OUTPUT},           //17 GPIO_ATS_PWR_EN 旧版Rx 悬空未连接  新版Rx 已更改为触摸屏复位引脚
    {GPIO1_C3,  PIN_MODE_OUTPUT},           //18 GPIO_AMP_EN
    {GPIO1_C7,  PIN_MODE_OUTPUT},           //19 GPIO_OLED_DCX
    {GPIO1_C1,  PIN_MODE_OUTPUT },          //20 GPIO_OLED_SWIRE 新版Rx 已更改为触摸屏事件中断引脚
    {GPIO0_D7,  PIN_MODE_OUTPUT},           // GPIO_ALL_PWR
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