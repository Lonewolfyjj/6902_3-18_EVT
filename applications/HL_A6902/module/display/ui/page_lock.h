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
    /// @brief 显示锁屏图标
    HL_LOCK_ICON_SHOW = 0,
    /// @brief 隐藏锁屏图标
    HL_LOCK_ICON_HIDE,
    /// @brief 显示解锁图标
    HL_UNLOCK_ICON_SHOW,
    /// @brief 隐藏解锁图标
    HL_UNLOCK_ICON_HIDE,
    /// @brief 初始化界面
    HL_LOCK_PAGE_INIT,
    /// @brief 退出界面
    HL_LOCK_PAGE_EXIT,
    /// @brief 图标动画
    HL_LOCK_ICON_ANIM,
    /// @brief 锁屏情况下TP单机触发
    HL_LOCK_TP_CLICK_CB,
    /// @brief 锁屏情况下TP单击触发的禁用
    HL_LOCK_TP_CLICK_CB_DEL,
    /// 初始化关于LOCK相关的代码（主要是主页面的,需要加回调）
    HL_LOCK_RSOURCE_INIT,
    /// 反初始化关于LOCK相关的代码（每个页面结束，必须回收锁屏相关资源）
    HL_LOCK_RSOURCE_DEINIT
} hl_lvgl_lock_cmd_t;

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
