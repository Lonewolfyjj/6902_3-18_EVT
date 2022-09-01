/**
 * @file hl_hal_gpio.h
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @brief  本模块将gpio操作抽象出来，统一管理所用到的gpio，便于gpio修改和维护。
 * @version V1.0
 * @date 2022-09-01
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-01     <td>v1.0     <td>luzhanghao     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_HAL_GPIO_H__
#define __HL_HAL_GPIO_H__

/* Includes ------------------------------------------------------------------*/
#include "rtdef.h"
#include "hl_config.h"

/* typedef -------------------------------------------------------------------*/
#if HL_GET_DEVICE_TYPE()
/// TX gpio 引脚枚举
typedef enum _hl_gpio_port_e
{
    //IN
    /// (降噪按键，外部上拉，低电平有效)
    GPIO_PAIR_KEY,
    /// (开关按键，外部上拉，低电平有效)
    GPIO_PWR_KEY,
    /// (录制按键，外部上拉，低电平有效)
    GPIO_REC_KEY,
    /// (usb插入检测，外部下拉，高电平有效)
    GPIO_VBUS_DET,
    /// (电量计中断，外部上拉，低电平有效)
    GPIO_GAUGE_INT,
    /// (mic插入检测，外部上拉，低电平有效)
    GPIO_MIC_DET,

    // OUT
    /// (EMMC复位脚，无外部上下拉，低电平有效)
    GPIO_EMMC_RST,
    /// (电平转换使能，外部下拉，高电平有效)
    GPIO_LVT_EN,
    /// (EMMC电源使能，无外部上下拉，高电平有效)
    GPIO_EMMC_PWR_EN,
    /// (电源使能，无外部上下拉，高电平有效)
    GPIO_PWR_EN,
    /// (3.3V使能，无外部上下拉，高电平有效)
    GPIO_DC3V3_EN,
    /// (RF电源使能，无外部上下拉，高电平有效)
    GPIO_2831P_EN,
    /// (RF电源使能，无外部上下拉，高电平有效)
    GPIO_RF_PWR_EN,
    /// (电池供电使能，外部下拉，低：内置mic 高：外置mic)
    GPIO_MIC_SW,
    /// (电池供电使能，外部上下拉都有，高电平有效)
    GPIO_ATS_RESET,

    // GPIO 总数
    USER_GPIO_COUNT
} hl_gpio_port_e;

#else
/// RX gpio 引脚枚举
typedef enum _hl_gpio_port_e
{
    // IN
    /// (开关按键，外部上拉，低电平有效)
    GPIO_PWR_KEY,
    /// (左旋钮A，外部上拉，低电平有效)
    GPIO_L_VOL_A,
    /// (左旋钮B，外部上拉，低电平有效)
    GPIO_L_VOL_B,
    /// (左旋钮按键，外部上拉，低电平有效)
    GPIO_L_VOL_KEY,
    /// (左旋钮A，外部上拉，低电平有效)
    GPIO_R_VOL_A,
    /// (左旋钮B，外部上拉，低电平有效)
    GPIO_R_VOL_B,
    /// (左旋钮按键，外部上拉，低电平有效)
    GPIO_R_VOL_KEY,
    /// (电量计中断，外部上拉，低电平有效)
    GPIO_GAUGE_INT,
    /// (usb插入检测，外部下拉，高电平有效)
    GPIO_VBUS_DET,
    /// (耳机插入检测，外部上拉，低电平有效)
    GPIO_HP_DET,
    /// (加速度传感器中断脚，外部上拉，低电平有效)
    GPIO_GSENSOR_INT,

    // OUT
    /// (电源使能，无外部上下拉，高电平有效)
    GPIO_PWR_EN,
    /// (USB切换开关，外部下拉，低：RK USB 高：2831P USB)
    GPIO_USB_SW,
    /// (OLED RST脚，无外部上下拉，低电平有效)
    GPIO_OLED_RST,
    /// (RF电源使能，无外部上下拉，高电平有效)
    GPIO_RF_PWR_EN,
    /// (电池供电使能，外部上下拉都有，高电平有效)
    GPIO_ATS_RESET,
    /// (ATS供电使能，外部上下拉都有，高电平有效)
    GPIO_ATS_PWR_EN,
    /// (放大器使能，无外部上下拉，低电平有效)
    GPIO_AMP_EN,

    // GPIO 总数
    USER_GPIO_COUNT
} hl_gpio_port_e;

#endif

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief gpio初始化函数
 * @param [in] gpio_index gpio枚举值
 * @return rt_err_t 初始化状态 RT_EOK：成功 | 其他：失败
 * @date 2022-09-01
 * @author luzhanghao (rd14@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>luzhanghao     <td>新建
 * </table>
 */
rt_err_t hl_hal_gpio_init(hl_gpio_port_e gpio_index);

/**
 * @brief gpio去初始化函数
 * @param [in] gpio_index gpio枚举值
 * @return rt_err_t 去初始化状态 RT_EOK：成功 | 其他：失败
 * @date 2022-09-01
 * @author luzhanghao (rd14@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>luzhanghao     <td>新建
 * </table>
 */
rt_err_t hl_hal_gpio_deinit(hl_gpio_port_e gpio_index);

/**
 * @brief gpio输出高电平
 * @param [in] gpio_index gpio枚举值
 * @date 2022-09-01
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>luzhanghao     <td>新建
 * </table>
 */
void hl_hal_gpio_high(hl_gpio_port_e gpio_index);

/**
 * @brief gpio输出低电平
 * @param [in] gpio_index gpio枚举值
 * @date 2022-09-01
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>luzhanghao     <td>新建
 * </table>
 */
void hl_hal_gpio_low(hl_gpio_port_e gpio_index);

/**
 * @brief 获取gpio电平
 * @param [in] gpio_index gpio枚举值
 * @return int 0：低电平 | 非0：高电平
 * @date 2022-09-01
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>luzhanghao     <td>新建
 * </table>
 */
int hl_hal_gpio_read(hl_gpio_port_e gpio_index);

#endif /* __HL_HAL_GPIO_H__ */

/*
 * EOF
 */