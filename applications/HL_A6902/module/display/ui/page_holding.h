/**
 * @file page_holding.h
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
#ifndef _PAGE_HOLDING_H
#define _PAGE_HOLDING_H

#include "lvgl.h"
#include "stdio.h"

/// @brief 命令枚举
typedef enum _hl_lvgl_holding_cmd_t
{
    /// @brief 电池电量
    HL_HOLDING_RX_ELEC,
    /// @brief 设置电池颜色为绿色
    HL_HOLDING_RX_BAT_COLOR_GREEN,
    /// @brief 设置电池颜色为白色
    HL_HOLDING_RX_BAT_COLOR_WHITE,
    /// @brief 隐藏充电闪电图标
    HL_HOLDING_RX_ICON_HIDE,
    /// @brief 显示充电闪电图标
    HL_HOLDING_RX_ICON_DISHIDE,
    /// @brief 删除界面
    HL_HOLDING_DELETE_PAGE,
    /// @brief 删除风格样式
    HL_HOLDING_DELETE_STYLE
}hl_lvgl_holding_cmd_t;

/// @brief 控制结构体
typedef struct __hl_lvgl_holding_ioctl_t
{
    /// @brief 命令
    hl_lvgl_holding_cmd_t holding_cmd;
    /// @brief 参数
    int16_t electric;
}hl_lvgl_holding_ioctl_t;

/// @brief 初始化结构体
typedef struct __hl_lvgl_holding_init_t
{
    /// @brief 电量参数，电量 0 - 100 %
    int16_t electric;
}hl_lvgl_holding_init_t;

/**
 * 
 * @brief 初始化函数
 * @param [in] init_data 参考结构体 hl_lvgl_holding_init_t
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
void hl_mod_holding_init(void * init_data);

/**
 * 
 * @brief 控制函数
 * @param [in] ctl_data 参考结构体 hl_lvgl_holding_ioctl_t
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
void hl_mod_holding_ioctl(void * ctl_data);
#endif