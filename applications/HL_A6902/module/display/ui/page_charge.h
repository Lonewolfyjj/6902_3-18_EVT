/**
 * @file page_charge.h
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
#ifndef _PAGE_CHARGE_H
#define _PAGE_CHARGE_H

#include "lvgl.h"
#include "stdio.h"

/// @brief 显示充电设备枚举（多个时可用 或 操作）
typedef enum _hl_charge_device_display_t
{
    /// @brief 显示TX1
    HL_CHARGE_DISPLAY_TX1 = (1<<0),
    /// @brief 显示RX
    HL_CHARGE_DISPLAY_RX = (1<<1),
    /// @brief 显示TX2
    HL_CHARGE_DISPLAY_TX2 = (1<<2),
}hl_charge_device_display_t;

/// @brief 控制命令枚举
typedef enum _hl_lvgl_charge_cmd_type_t
{
    /// @brief 删除TX1设备
    HL_DELETE_TX1_DEVICE = 0,
    /// @brief 删除RX设备
    HL_DELETE_RX_DEVICE,
    /// @brief 删除TX2设备
    HL_DELETE_TX2_DEVICE,    
    /// @brief 添加TX1设备
    HL_ADD_TX1_DEVICE,
    /// @brief 添加RX设备
    HL_ADD_RX_DEVICE,
    /// @brief 添加TX2设备
    HL_ADD_TX2_DEVICE,
    /// @brief 改变右上角充电盒设备电量
    HL_CHARGE_CHANGE_BOX_ELEC,
    /// @brief 改变TX1设备电量
    HL_CHARGE_CHANGE_TX1_ELEC,
    /// @brief 改变RX设备电量
    HL_CHARGE_CHANGE_RX_ELEC,
    /// @brief 改变TX2设备电量
    HL_CHARGE_CHANGE_TX2_ELEC,
    /// @brief 设置顶部电池图标为绿色
    HL_CHARGE_TOP_BAT_COLOR_GREEN,
    /// @brief 设置顶部电池图标为白色
    HL_CHARGE_TOP_BAT_COLOR_WHITE,
    /// @brief 设置TX1设备电池图标为绿色
    HL_CHARGE_TX1_BAT_COLOR_GREEN,
    /// @brief 设置TX1设备电池图标为白色
    HL_CHARGE_TX1_BAT_COLOR_WHITE,
    /// @brief 设置RX设备电池图标为绿色
    HL_CHARGE_RX_BAT_COLOR_GREEN,
    /// @brief 设置RX设备电池图标为白色
    HL_CHARGE_RX_BAT_COLOR_WHITE,
    /// @brief 设置TX2设备电池图标为绿色
    HL_CHARGE_TX2_BAT_COLOR_GREEN,
    /// @brief 设置TX2设备电池图标为白色
    HL_CHARGE_TX2_BAT_COLOR_WHITE,
    /// @brief 隐藏TX1设备充电闪电图标
    HL_CHARGE_TX1_ICON_HIDE,
    /// @brief 隐藏RX充电闪电图标
    HL_CHARGE_RX_ICON_HIDE,
    /// @brief 隐藏TX2设备充电闪电图标
    HL_CHARGE_TX2_ICON_HIDE,
    /// @brief 显示TX1设备充电闪电图标
    HL_CHARGE_TX1_ICON_DISHIDE,
    /// @brief 显示RX设备充电闪电图标
    HL_CHARGE_RX_ICON_DISHIDE,
    /// @brief 显示TX2设备充电闪电图标
    HL_CHARGE_TX2_ICON_DISHIDE,
    /// @brief 删除页面
    HL_CHARGE_CHANGE_DELETE_PAGE,
    /// @brief 删除风格样式
    HL_CHARGE_CHANGE_DELETE_STYLE
}hl_lvgl_charge_cmd_type_t;

/// @brief 电池电量设置结构体
typedef struct __hl_lvgl_charge_elec_t
{
    /// @brief 充电盒设备电量，电量 0 - 100 %
    int16_t electric_box;
    /// @brief TX1设备电量，//电量 0 - 100 %
    int16_t electric_tx1;
    /// @brief RX设备电量，//电量 0 - 100 %
    int16_t electric_rx;
    /// @brief TX2设备电量，//电量 0 - 100 %
    int16_t electric_tx2;
}hl_lvgl_charge_elec_t;

/// @brief 控制结构体
typedef struct __hl_lvgl_charge_ioctl_t
{
    /// @brief 控制命令
    hl_lvgl_charge_cmd_type_t charge_cmd;
    /// @brief 电量参数
    hl_lvgl_charge_elec_t electric;
}hl_lvgl_charge_ioctl_t;

/// @brief 初始化结构体
typedef struct __hl_lvgl_charge_init_t
{
    /// @brief 默认显示设备
    hl_charge_device_display_t display_device;
    /// @brief 电量参数
    hl_lvgl_charge_elec_t electric;
}hl_lvgl_charge_init_t;

/**
 * 
 * @brief 控制函数
 * @param [in] ctl_data 参考结构体 hl_lvgl_charge_ioctl_t
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
void hl_mod_charge_ioctl(void * ctl_data);

/**
 * 
 * @brief 初始化函数
 * @param [in] init_data 参考结构体 hl_lvgl_charge_init_t
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
void hl_mod_charge_init(void * init_data);
#endif