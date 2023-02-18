/**
 * @file page_lineout.h
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
#ifndef _PAGE_LINEOUT_H
#define _PAGE_LINEOUT_H

#include "lvgl.h"
#include "stdio.h"

/// @brief 点击对象枚举
typedef enum _hl_lineout_check_t
{
    /// @brief 左侧
    HL_LINEOUT_CHECK_LEFT = 0,
    /// @brief 右侧
    HL_LINEOUT_CHECK_RIGHT
}hl_lineout_check_t;

/// @brief 操作命令枚举
typedef enum _hl_lineout_choose_t
{
    /// @brief 选中左侧按钮
    HL_LINEOUT_CHOOSE_LEFT = 0,
    /// @brief 选中右侧按钮
    HL_LINEOUT_CHOOSE_RIGHT,
    /// @brief 删除页面
    HL_LINEOUT_DELETE_PAGE,
    /// @brief 删除风格样式
    HL_LINEOUT_DELETE_STYLE
}hl_lineout_choose_t;

/// @brief 回调函数
typedef void(*hl_lineout_event_cb)(hl_lineout_check_t);

/// @brief 初始化结构体
typedef struct __hl_lvgl_lineout_init_t
{
    /// @brief 初始化选中按钮
    hl_lineout_choose_t lineout_choose;
    /// @brief 左声道音量 
    int16_t left_volume;
    /// @brief 左声道音量最小值
    int16_t left_volume_min;
    /// @brief 左声道音量最大值
    int16_t left_volume_max;
    /// @brief 右声道音量 
    int16_t right_volume;
    /// @brief 右声道音量最小值
    int16_t right_volume_min;
    /// @brief 右声道音量最大值
    int16_t right_volume_max;
    /// @brief 回调函数
    hl_lineout_event_cb func_cb;
}hl_lvgl_lineout_init_t;

/// @brief 控制函数
typedef struct __hl_lvgl_lineout_ioctl_t
{
    /// @brief 命令
    hl_lineout_choose_t lineout_choose;//
}hl_lvgl_lineout_ioctl_t;

/**
 * 
 * @brief 控制函数
 * @param [in] ctl_data 参考结构体 hl_lvgl_lineout_ioctl_t
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
void hl_mod_lineout_ioctl(void * ctl_data);

/**
 * 
 * @brief 初始化函数
 * @param [in] init_data 参考结构体 hl_lvgl_lineout_init_t
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
void hl_mod_lineout_init(void * init_data);

#endif
