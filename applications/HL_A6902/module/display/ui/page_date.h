/**
 * @file page_date.h
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
#ifndef _PAGE_DATE_H
#define _PAGE_DATE_H

#include "lvgl.h"
#include "stdio.h"

/// @brief 配置项类型枚举
typedef enum _hl_date_choose_t
{
    HL_DATE_YEAR = 0,
    HL_DATE_MONTH,
    HL_DATE_DAY,
    HL_DATE_HOUR,
    HL_DATE_MIN
}hl_date_choose_t;

/// @brief 命令类型枚举
typedef enum _hl_date_cmd_t
{
    HL_DATE_SET_OPT_CMD = 0,
    HL_DATE_SET_VALUE_CMD,
    HL_DATE_DELETE_STYLE_CMD,
    HL_DATE_EXTI_CMD
}hl_date_cmd_t;

/// @brief 回调函数
typedef void(*hl_date_event_cb)(hl_date_choose_t,int16_t);

/// @brief 初始化结构体
typedef struct _hl_lvgl_date_init_t
{
    /// @brief 初始化选中类型
    hl_date_choose_t current_choose;
    /// @brief 年 2000 - 2050
    int16_t year;
    /// @brief 月
    int8_t month;
    /// @brief 日
    int8_t day;
    /// @brief 小时
    int8_t hour;
    /// @brief 分钟
    int8_t min;
    /// @brief 回调函数
    hl_date_event_cb func_cb;
}hl_lvgl_date_init_t;

/// @brief 控制结构体
typedef struct _hl_lvgl_date_ioctl_t
{
    /// @brief 命令
    hl_date_cmd_t date_cmd;
    /// @brief 控制项
    hl_date_choose_t opt;
    /// @brief 参数
    int16_t param;
}hl_lvgl_date_ioctl_t;

/**
 * 
 * @brief 控制函数
 * @param [in] ctl_data 参考结构体 hl_lvgl_date_ioctl_t
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
void hl_mod_date_ioctl(void * ctl_data);

/**
 * 
 * @brief 初始化函数
 * @param [in] init_data 参考结构体 hl_lvgl_date_init_t
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
void hl_mod_date_init(void * init_data);

#endif