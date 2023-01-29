/**
 * @file page_upgrade.h
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
#ifndef _PAGE_UPGRADE_H
#define _PAGE_UPGRADE_H

#include "lvgl.h"
#include "stdio.h"

/// @brief 命令枚举
typedef enum _hl_upgrade_ioctl_t
{
    /// @brief 升级进度
    HL_UPGRADE_PARAM_CMD = 0,
    /// @brief 升级成功
    HL_UPGRADE_SUCCESS_CMD,
    /// @brief 升级失败
    HL_UPGRADE_FAIL_CMD,
    /// @brief 删除界面
    HL_UPGRADE_CLEAR_CMD,
    /// @brief 删除风格样式
    HL_UPGRADE_DEL_STYLE_CMD,
}hl_upgrade_ioctl_t;

/// @brief 初始化结构体
typedef struct _hl_lvgl_upgrade_init_t
{
    /// @brief 升级进度 0 - 100
    uint8_t upgrade_progress;
}hl_lvgl_upgrade_init_t;

/// @brief 控制结构体
typedef struct _hl_lvgl_upgrade_ioctl_t
{
    /// @brief 升级进度条下的字符
    char * ptr;
    /// @brief 升级进度 0 - 100
    uint8_t upgrade_progress;
    /// @brief 命令
    hl_upgrade_ioctl_t upgrade_ioctl;
}hl_lvgl_upgrade_ioctl_t;

/**
 * 
 * @brief 控制函数
 * @param [in] ctl_data 参考结构体 hl_lvgl_upgrade_ioctl_t
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
void hl_mod_lvgl_upgrade_ioctl(void * ctl_data);

/**
 * 
 * @brief 初始化函数
 * @param [in] init_data 参考结构体 hl_lvgl_upgrade_init_t
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
void hl_mod_lvgl_upgrade_init(void * init_data);

#endif