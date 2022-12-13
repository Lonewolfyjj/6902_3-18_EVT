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
#if HL_IS_TX_DEVICE()
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
    /// (EMMC电源使能，无外部上下拉，高电平有效)
    GPIO_EMMC_PWR_EN,
    /// (电源使能，无外部上下拉，高电平有效)
    GPIO_PWR_EN,
    /// (3.3V使能，无外部上下拉，高电平有效)
    GPIO_DC3V3_EN,
    /// (无线模块电源使能，无外部上下拉，高电平有效)
    GPIO_2831P_EN,
    /// (RF电源使能，无外部上下拉，高电平有效)
    GPIO_RF_PWR_EN,
    /// (电池供电使能，外部下拉，低：内置mic 高：外置mic)
    GPIO_MIC_SW,
    /// (无线模块复位脚，外部上下拉都有，高电平有效)
    GPIO_ATS_RESET,
    /// (电源总开关，外部下拉，高电平有效)
    GPIO_ALL_POWER,

    // GPIO 总数
    USER_GPIO_COUNT
} hl_gpio_pin_e;

#else
/// RX gpio 引脚枚举
typedef enum _hl_gpio_port_e
{
    // IN
    /// (开关按键，外部上拉，低电平有效)
    GPIO_PWR_KEY,
    /// (左旋钮A，外部上拉，低电平有效)
    GPIO_VOL_A,
    /// (左旋钮B，外部上拉，低电平有效)
    GPIO_VOL_B,
    /// (左旋钮按键，外部上拉，低电平有效)
    GPIO_VOL_OK,
    // /// (左旋钮A，外部上拉，低电平有效)
    // GPIO_R_VOL_A,
    // /// (左旋钮B，外部上拉，低电平有效)
    // GPIO_R_VOL_B,
    // /// (左旋钮按键，外部上拉，低电平有效)
    // GPIO_R_VOL_KEY,
    /// (电量计中断，外部上拉，低电平有效)
    GPIO_GAUGE_INT,
    /// (usb插入检测，外部下拉，高电平有效)
    GPIO_VBUS_DET,
    /// (耳机插入检测，外部上拉，低电平有效)
    GPIO_HP_DET,
    /// (相机口插入检测，外部上拉，低电平有效)
    GPIO_CAM_DET,
    /// (加速度传感器中断脚，外部上拉，低电平有效)
    GPIO_GSENSOR_INT,
    /// (oled帧同步输出脚，无外部上下拉，未激活输出低，激活输出高)
    GPIO_OLED_TE,
    /// (触摸屏中断脚，无外部上下拉，...)
    GPIO_TP_INT,

    // OUT
    /// (OLED RST脚，无外部上下拉，低电平有效)
    GPIO_OLED_RST,
    /// (RF电源使能，无外部上下拉，高电平有效)
    GPIO_RF_PWR_EN,
    /// (电池供电使能，外部上下拉都有，高电平有效)
    GPIO_ATS_RESET,
    /// (触摸屏复位脚，无外部上下拉，...)
    GPIO_TP_RST,
    /// (Codec使能，无外部上下拉，高电平有效)
    GPIO_CODEC_EN,
    /// (oled数据命令选择，无外部上下拉，低电平命令，高电平数据)
    GPIO_OLED_DCX,
    /// (Swire protocol设置脚，无外部上下拉，目前oled供电芯片使用MCU暂时没用到)(硬件用做了oled电源控制脚，后续需要更改名字)
    GPIO_OLED_SWIRE,
    /// (电源总开关，外部下拉，高电平有效)
    GPIO_ALL_POWER,

    // GPIO 总数
    USER_GPIO_COUNT
} hl_gpio_pin_e;

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
rt_err_t hl_hal_gpio_init(hl_gpio_pin_e gpio_index);

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
rt_err_t hl_hal_gpio_deinit(hl_gpio_pin_e gpio_index);

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
void hl_hal_gpio_high(hl_gpio_pin_e gpio_index);

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
void hl_hal_gpio_low(hl_gpio_pin_e gpio_index);

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
int hl_hal_gpio_read(hl_gpio_pin_e gpio_index);

/**
 * @brief 设置中断处理函数
 * @param [in] gpio_index gpio_index gpio枚举值
 * @param [in] mode 中断模式
 *       #define PIN_IRQ_MODE_RISING             0x00
 *       #define PIN_IRQ_MODE_FALLING            0x01
 *       #define PIN_IRQ_MODE_RISING_FALLING     0x02
 *       #define PIN_IRQ_MODE_HIGH_LEVEL         0x03
 *       #define PIN_IRQ_MODE_LOW_LEVEL          0x04
 * @param [in] hdr 中断处理函数
 * @param [in] args 中断处理函数参数
 * @return rt_err_t 函数绑定结果 RT_EOK：成功 | 其他：失败
 * @date 2022-09-02
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-02      <td>luzhanghao     <td>新建
 * </table>
 */
rt_err_t hl_hal_gpio_attach_irq(hl_gpio_pin_e gpio_index, rt_uint32_t mode, void (*hdr)(void *args), void *args);

/**
 * @brief 中断函数解除绑定
 * @param [in] gpio_index gpio_index gpio枚举值
 * @return rt_err_t 函数解除绑定结果 RT_EOK：成功 | 其他：失败
 * @date 2022-09-02
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-02      <td>luzhanghao     <td>新建
 * </table>
 */
rt_err_t hl_hal_gpio_detach_irq(hl_gpio_pin_e gpio_index);

/**
 * @brief 设置中断使能状态
 * @param [in] gpio_index gpio_index gpio枚举值
 * @param [in] enabled 使能：PIN_IRQ_ENABLE | 失能：PIN_IRQ_DISABLE
 * @return rt_err_t gpio中断使能结果 RT_EOK：成功 | 其他：失败
 * @date 2022-09-02
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-02      <td>luzhanghao     <td>新建
 * </table>
 */
rt_err_t hl_hal_gpio_irq_enable(hl_gpio_pin_e gpio_index, rt_uint32_t enabled);

#endif /* __HL_HAL_GPIO_H__ */

/*
 * EOF
 */
