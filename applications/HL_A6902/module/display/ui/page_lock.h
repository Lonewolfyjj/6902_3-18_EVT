/**
 * @file page_lock.h
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
#ifndef _PAGE_LOCK_H
#define _PAGE_LOCK_H

#include "lvgl.h"
#include "stdio.h"

/// @brief 命令枚举
typedef enum _hl_lvgl_lock_cmd_t
{
    /// @brief 进入锁屏状态
    HL_LOCK_STATUS_INTO = 0,
    /// @brief 退出锁屏状态
    HL_LOCK_STATUS_BACK,
    /// @brief 将锁屏图层移到最前面
    HL_LOCK_ICON_FRONT,
    /// @brief 图标动画
    HL_LOCK_ICON_ANIM,
}hl_lvgl_lock_cmd_t;

/// @brief 图标枚举
typedef enum _hl_lock_icon_t
{
    /// @brief LINE_OUT图标
    HL_LOCK_ICON_LINEOUT = 0,
    /// @brief USB图标
    HL_LOCK_ICON_TYPEC,
    /// @brief 耳机图标
    HL_LOCK_ICON_HEATSET,
    /// @brief 锁屏图标
    HL_LOCK_ICON_LOCK,
}hl_lock_icon_t;

/// @brief 控制结构体
typedef struct __hl_lvgl_lock_ioctl_t
{
    /// @brief 图标类型
    hl_lock_icon_t icon_typ;
    /// @brief 图标对象
    lv_obj_t * icon_obj;
    /// @brief 命令
    hl_lvgl_lock_cmd_t cmd;
}hl_lvgl_lock_ioctl_t;

/**
 * 
 * @brief 控制函数
 * @param [in] ctl_data 参考结构体 hl_lvgl_lock_ioctl_t
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
void hl_mod_lock_ioctl(void * ctl_data);

#endif
