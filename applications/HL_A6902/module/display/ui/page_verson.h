/**
 * @file page_verson.h
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
#ifndef _PAGE_VERSON_H
#define _PAGE_VERSON_H

#include "lvgl.h"
#include "stdio.h"

/// @brief 命令枚举
typedef enum _hl_verson_mid_opt_t
{
    /// @brief 第一个选项
    HL_VERSON_OPTION_ONE = 0,
    /// @brief 第二个选项
    HL_VERSON_OPTION_TWO,
    /// @brief 第三个选项
    HL_VERSON_OPTION_THREE,
    /// @brief 删除风格样式
    HL_VERSON_OPTION_DEL_STYLE,
    /// @brief 删除界面
    HL_VERSON_OPTION_EXTI
}hl_verson_mid_opt_t;

/// @brief 初始化结构体
typedef struct __hl_lvgl_verson_init_t
{
    /// @brief SN号
    const char * hl_verson_sn_number;
    /// @brief 固件版本
    const char * hl_verson_firmware;
    /// @brief TX1版本
    const char * hl_verson_tx1;
    /// @brief TX2版本
    const char * hl_verson_tx2;
}hl_lvgl_verson_init_t;

/// @brief 控制结构体
typedef struct __hl_lvgl_verson_ioctl_t
{
    /// @brief 命令
    hl_verson_mid_opt_t verson_choose_opt;
}hl_lvgl_verson_ioctl_t;

/**
 * 
 * @brief 控制函数
 * @param [in] ctl_data 参考结构体 hl_lvgl_verson_ioctl_t
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
void hl_mod_verson_ioctl(void * ctl_data);

/**
 * 
 * @brief 初始化函数
 * @param [in] init_data 参考结构体 hl_lvgl_verson_init_t
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
void hl_mod_verson_init(void * init_data);

#endif