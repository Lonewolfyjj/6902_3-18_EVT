/**
 * @file page_main.h
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief 
 * @version 1.0
 * @date 2023-01-14
 * 
 * ██╗  ██╗ ██████╗ ██╗     ██╗  ██╗   ██╗██╗      █████╗ ███╗   ██╗██████╗ 
 * ██║  ██║██╔═══██╗██║     ██║  ╚██╗ ██╔╝██║     ██╔══██╗████╗  ██║██╔══██╗
 * ███████║██║   ██║██║     ██║   ╚████╔╝ ██║     ███████║██╔██╗ ██║██║  ██║
 * ██╔══██║██║   ██║██║     ██║    ╚██╔╝  ██║     ██╔══██║██║╚██╗██║██║  ██║
 * ██║  ██║╚██████╔╝███████╗███████╗██║   ███████╗██║  ██║██║ ╚████║██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝
 * @copyright Copyright (c) 2023 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2023-01-14     <td>v1.0     <td>dujunjie       <td>初次发布
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/*
 * EOF
 */
#ifndef _PAGE_MAIN_H
#define _PAGE_MAIN_H

#include "lvgl.h"
#include "stdio.h"

/// @brief 录制状态枚举
typedef enum _hl_record_status_t
{
    /// @brief 未录制
    HL_NO_RECODE = 0,
    /// @brief 录制中
    HL_RECODING
}hl_record_status_t;

/// @brief 信号状态枚举
typedef enum _hl_signal_int_t
{
    /// @brief 无信号
    HL_NO_SIGNAL = 0,
    /// @brief 一格信号
    HL_ONE_SIGNAL,
    /// @brief 两格信号
    HL_TWO_SIGNAL,
    /// @brief 三格信号
    HL_THREE_SIGNAL,
    /// @brief 四格信号
    HL_FOUR_SIGNAL
}hl_signal_int_t;

/// @brief 显示设备枚举
typedef enum _hl_device_display_t
{
    /// @brief 只显示TX1
    HL_DISPLAY_TX1 = 0,
    /// @brief 只显示TX2
    HL_DISPLAY_TX2,
    /// @brief 显示TX1和TX2
    HL_DISPLAY_DOUBLE
}hl_device_display_t;

/// @brief 命令枚举
typedef enum _hl_lvgl_main_cmd_type_t
{
    /// @brief 创建TX1设备
    HL_CHANGE_TX1_DEVICE = 0,
    /// @brief 创建TX2设备
    HL_CHANGE_TX2_DEVICE,
    /// @brief 创建TX1、TX2设备
    HL_CHANGE_DOUBLE_DEVICE,
    /// @brief 更改TX1信号
    HL_CHANGE_TX1_SIGNAL,
    /// @brief 更改TX1电量
    HL_CHANGE_TX1_ELEC,
    /// @brief 更改TX1音量
    HL_CHANGE_TX1_VOL,
    /// @brief 更改TX1录制状态
    HL_CHANGE_TX1_REC,
    /// @brief 更改TX1电池颜色为红色
    HL_CHANGE_TX1_BAR_RED,
    /// @brief 更改TX1电池颜色为白色
    HL_CHANGE_TX1_BAR_WHITE,
    /// @brief TX1静音
    HL_CHANGE_TX1_MUTE,
    /// @brief TX1取消静音
    HL_CHANGE_TX1_MUTE_DEL,
    /// @brief 更改TX2信号
    HL_CHANGE_TX2_SIGNAL,
    /// @brief 更改TX2电量
    HL_CHANGE_TX2_ELEC,
    /// @brief 更改TX2音量
    HL_CHANGE_TX2_VOL,
    /// @brief 更改TX2录制状态
    HL_CHANGE_TX2_REC,
    /// @brief 更改TX2电池颜色为红色
    HL_CHANGE_TX2_BAR_RED,
    /// @brief 更改TX2电池颜色为白色
    HL_CHANGE_TX2_BAR_WHITE,
    /// @brief TX1电量显示绿色
    HL_CHANGE_TX1_BAR_GREEN,
    /// @brief TX2电量显示绿色
    HL_CHANGE_TX2_BAR_GREEN,
  /// @brief 隐藏TX1设备充电闪电图标
    HL_CHANGE_TX1_ICON_HIDE,

    /// @brief 隐藏TX2设备充电闪电图标
    HL_CHANGE_TX2_ICON_HIDE,
    /// @brief 显示TX1设备充电闪电图标
    HL_CHANGE_TX1_ICON_DISHIDE,

    /// @brief 显示TX2设备充电闪电图标
    HL_CHANGE_TX2_ICON_DISHIDE,
    
    /// @brief TX2静音
    HL_CHANGE_TX2_MUTE,
    HL_CHANGE_TX2_MUTE_DEL,
    HL_CHANGE_DELETE_DOUBLE,
    /// @brief 只删除TX1设备
    HL_CHANGE_DELETE_TX1,
    /// @brief 只删除TX2设备
    HL_CHANGE_DELETE_TX2,
    /// @brief 删除风格样式
    HL_CHANGE_DELETE_STYLE
}hl_lvgl_main_cmd_type_t;

/// @brief 界面参数结构体
typedef struct __device_data_t
{
    /// @brief 录制状态
    hl_record_status_t record;
    /// @brief 值
    int16_t line_out_value;
    /// @brief 增益
    int16_t tx_gain; 
    /// @brief Tx音量 -40 - 60 db
    int16_t volume;
    /// @brief 电量 0 - 100 %
    int16_t electric;
    /// @brief 信号强度 0 - 4
    hl_signal_int_t signal;
}device_data_t;

/// @brief 初始化结构体
typedef struct __hl_lvgl_main_init_t
{
    /// @brief 默认显示设备
    hl_device_display_t display_tx_device;
    /// @brief TX1设备参数
    device_data_t tx_device_1;
    /// @brief TX2设备参数
    device_data_t tx_device_2;
}hl_lvgl_main_init_t;

/// @brief 控制结构体
typedef struct __hl_lvgl_main_ioctl_t
{
    /// @brief 命令
    hl_lvgl_main_cmd_type_t cmd;
    /// @brief TX1设备参数
    device_data_t tx_device_1;
    /// @brief TX2设备参数
    device_data_t tx_device_2;
}hl_lvgl_main_ioctl_t;

/**
 * 
 * @brief 控制函数
 * @param [in] ctl_data 参考结构体 hl_lvgl_main_ioctl_t
 * @date 2023-01-14
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-01-14      <td>dujunjie     <td>新建
 * </table>
 */
void hl_mod_main_ioctl(void * ctl_data);

/**
 * 
 * @brief 初始化函数
 * @param [in] init_data 参考结构体 hl_lvgl_main_init_t
 * @date 2023-01-14
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-01-14      <td>dujunjie     <td>新建
 * </table>
 */
void hl_mod_main_init(void * init_data);

#endif