/**
 * @file page_top.h
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
#ifndef _PAGE_TOP_H
#define _PAGE_TOP_H

#include "lvgl.h"
#include "stdio.h"

/// @brief 图标状态枚举
typedef enum _hl_lvgl_lock_sta_t
{
    /// @brief 锁屏图标消失
    HL_LOCK_STATUS_HIDE = 0,
    /// @brief 解锁图标消失
    HL_UNLOCK_STATUS_HIDE,
} hl_lvgl_lock_sta_t;

/// @brief 回调函数
typedef void (*hl_lock_event_cb)(hl_lvgl_lock_sta_t);

/// @brief 顶部图标类型枚举，枚举顺序影响图标排序
typedef enum _hl_top_icon_t
{
    /// @brief  下面都是在中心不需要显示大图标的
    HL_TOP_ICON_STEREO_MOD = 0,
    HL_TOP_ICON_SINGLE_MOD,
    HL_TOP_ICON_TRACK_MOD,
    HL_TOP_ICON_NOISE,

    HL_TOP_ICON_APPLE,

    /// @brief  下面都是需要在中心显示大图标的
    HL_TOP_ICON_LOCK,
    HL_TOP_ICON_UNLOCK,
    HL_TOP_ICON_LINEOUT,
    HL_TOP_ICON_TYPEC,
    HL_TOP_ICON_HEATSET,
    HL_TOP_ICON_CNT,
} hl_top_icon_t;

/// @brief 命令枚举
typedef enum _hl_top_cmd_t
{
    HL_TOP_DELETE_ICON_CMD = 0,
    HL_TOP_ADD_ICON_CMD,
    HL_TOP_INPUT_CMD,
    HL_TOP_BAT_VAL,
    HL_TOP_BAT_COLOR_GREEN,
    HL_TOP_BAT_CHARGER_HIDE,
    HL_TOP_BAT_CHARGER_NOT_HIDE,
    HL_TOP_BAT_COLOR_WHITE,
    HL_TOP_BAT_COLOR_RED,
    HL_TOP_GET_ICON_OBJ_CMD,
    HL_TOP_DELETE_STYLE,
    HL_TOP_ALL_DEL,
    HL_TOP_CENTER_DEL,
    HL_TOP_LOCK_TP_FORBIDDEN,
    /// @brief 锁屏情况下TP单击触发的禁用
    HL_TOP_UNKLOCK_TP,
} hl_top_cmd_t;

/// @brief 控制结构体
typedef struct _hl_lvgl_top_ioctl_t
{
    /// @brief 命令
    hl_top_cmd_t top_cmd;
    /// @brief 参数
    hl_top_icon_t top_param;
    /// @brief 顶部电池电量 0 - 100 %
    int16_t electric_top;
    /// @brief 图标对象
    lv_obj_t* icon_obj;

} hl_lvgl_top_ioctl_t;

/// @brief 初始化结构体
typedef struct _hl_lvgl_top_init_t
{
    /// @brief 顶部电池图标电量 0 - 100 %
    int16_t electric_top;
    /// @brief 回调函数
    hl_lock_event_cb lock_event_cb;
} hl_lvgl_top_init_t;

/**
 * 
 * @brief 控制函数
 * @param [in] ctl_data 参考结构体 hl_lvgl_top_ioctl_t
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
void hl_mod_top_ioctl(void* ctl_data);

/**
 * 
 * @brief 初始化函数
 * @param [in] init_data 参考结构体 hl_lvgl_top_init_t
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
void hl_mod_top_init(void* init_data);

#endif