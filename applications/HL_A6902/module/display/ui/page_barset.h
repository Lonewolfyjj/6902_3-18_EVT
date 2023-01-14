/**
 * @file page_barset.h
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
#ifndef _PAGE_BARSET_H
#define _PAGE_BARSET_H

#include "lvgl.h"
#include "stdio.h"

/// @brief 顶部图标类型枚举
typedef enum _hl_barset_top_icontyp_t
{
    /// @brief 无图标
    HL_NO_ICON = 0,
    /// @brief 立体声图标
    HL_STEREO_ICON,
    /// @brief 安全音轨图标
    HL_SAYFT_ICON,
    /// @brief 单声道图标
    HL_SINGLE_ICON
}hl_barset_top_icontyp_t;

/// @brief 控制命令枚举
typedef enum _hl_barset_ioctl_t
{
    /// @brief 设置滑块值
    HL_VALUE = 0,
    /// @brief 删除风格样式
    HL_DELETE_STYLE,
    /// @brief 退出界面
    HL_EXTI
}hl_barset_ioctl_t;

/// @brief 控制结构体
typedef struct __hl_lvgl_barset_ioctl_t
{
    /// @brief 控制命令
    hl_barset_ioctl_t barset_value;
    /// @brief 参数
    int16_t value;
}hl_lvgl_barset_ioctl_t;

/// @brief 回调函数
typedef void(*hl_barset_event_cb)(int16_t);

/// @brief 初始化参数结构体
typedef struct __hl_lvgl_barset_init_t
{
    /// @brief 滑块值最小范围
    int16_t range_min;
    /// @brief 滑块最大范围
    int16_t range_max;
    /// @brief 滑块默认值
    int16_t init_value;
    /// @brief 滑块上的图片
    const void * src;
    /// @brief 滑块上的字符
    const char * ptr;
    /// @brief 页面顶部图标
    hl_barset_top_icontyp_t icontyp;
    /// @brief 事件回调
    hl_barset_event_cb func_cb;
}hl_lvgl_barset_init_t;

/**
 * 
 * @brief 控制函数
 * @param [in] ctl_data 参考结构体 hl_lvgl_barset_ioctl_t
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
void hl_mod_barset_ioctl(void * ctl_data);

/**
 * 
 * @brief 初始化函数
 * @param [in] init_data 参考结构体 hl_lvgl_barset_init_t
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
void hl_mod_barset_init(void * init_data);

#endif