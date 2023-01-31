/**
 * @file page_moreone.h
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
#ifndef _PAGE_MOREONE_H
#define _PAGE_MOREONE_H

#include "lvgl.h"
#include "stdio.h"

/// @brief 外部回调状态枚举
typedef enum _hl_moreone_check_t
{
    /// @brief 点击第一个选项左侧按钮
    HL_MOREONE_CHECK_ONE_LEFT = 0,
    /// @brief 点击第一个选项右侧按钮
    HL_MOREONE_CHECK_ONE_RIGHT,
    /// @brief 点击第二个选项左侧按钮
    HL_MOREONE_CHECK_TWO_LEFT,
    /// @brief 点击第二个选项右侧按钮
    HL_MOREONE_CHECK_TWO_RIGHT,
    /// @brief 点击第三个选项左侧按钮
    HL_MOREONE_CHECK_THREE_LEFT,
    /// @brief 点击第三个选项右侧按钮
    HL_MOREONE_CHECK_THREE_RIGHT,
    /// @brief 选中第一个选项左侧按钮
    HL_MOREONE_CHG_ONE_LEFT,
    /// @brief 选中第一个选项右侧按钮
    HL_MOREONE_CHG_ONE_RIGHT,
    /// @brief 选中第二个选项左侧按钮
    HL_MOREONE_CHG_TWO_LEFT,
    /// @brief 选中第二个选项右侧按钮
    HL_MOREONE_CHG_TWO_RIGHT,
    /// @brief 选中第三个选项左侧按钮
    HL_MOREONE_CHG_THREE_LEFT,
    /// @brief 选中第三个选项右侧按钮
    HL_MOREONE_CHG_THREE_RIGHT
}hl_moreone_check_t;

/// @brief 选中结构体
typedef enum _hl_moreone_choose_t
{
    /// @brief 选中左侧按钮
    HL_MOREONE_CHOOSE_LEFT = 0,
    /// @brief 选中右侧按钮
    HL_MOREONE_CHOOSE_RIGHT    
}hl_moreone_choose_t;

/// @brief 操作命令枚举
typedef enum _hl_moreone_mid_opt_t
{
    /// @brief 使第一个配置项居中
    HL_MOREONE_OPTION_ONE = 0,
    /// @brief 使第二个配置项居中
    HL_MOREONE_OPTION_TWO,
    /// @brief 使第三个配置项居中
    HL_MOREONE_OPTION_THREE,
    /// @brief 删除风格样式
    HL_MOREONE_OPTION_DELETE_STYLE,
    /// @brief 删除界面
    HL_MOREONE_OPTION_EXTI
}hl_moreone_mid_opt_t;

/// @brief 操作命令的类型
typedef enum _hl_moreone_cmd_type_t
{
    /// @brief 改变位置命令
    HL_MOREONE_POSTION_CMD = 0,
    /// @brief 选项命令
    HL_MOREONE_OPTION_CMD    
}hl_moreone_cmd_type_t;

/// @brief 回调函数
typedef void(*hl_moreone_event_cb)(hl_moreone_check_t);

/// @brief 配置项结构体
typedef struct __hl_lvgl_moreone_option_t
{
    /// @brief 第一个配置项参数
    hl_moreone_choose_t option_one;
    /// @brief 第二个配置项参数
    hl_moreone_choose_t option_two;
    /// @brief 第三个配置项参数
    hl_moreone_choose_t option_three;
}hl_lvgl_moreone_option_t;

/// @brief 初始化结构体
typedef struct __hl_lvgl_moreone_init_t
{
    /// @brief 菜单顶部字符
    char * device_ptr;
    /// @brief 初始化配置项参数
    hl_lvgl_moreone_option_t moreone_choose_opt;
    /// @brief 居中配置项
    hl_moreone_mid_opt_t moreone_mid_opt;
    /// @brief 回调函数
    hl_moreone_event_cb func_cb;
}hl_lvgl_moreone_init_t;

/// @brief 控制结构体
typedef struct __hl_lvgl_moreone_ioctl_t
{
    /// @brief 命令类型
    hl_moreone_cmd_type_t cmd_type;
    /// @brief 选中按钮
    hl_moreone_choose_t moreone_choose;
    /// @brief 居中配置项
    hl_moreone_mid_opt_t moreone_choose_opt;
}hl_lvgl_moreone_ioctl_t;

/**
 * 
 * @brief 控制函数
 * @param [in] ctl_data 参考结构体 hl_lvgl_moreone_ioctl_t
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
void hl_mod_moreone_ioctl(void * ctl_data);

/**
 * 
 * @brief 初始化函数
 * @param [in] init_data 参考结构体 hl_lvgl_moreone_init_t
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
void hl_mod_moreone_init(void * init_data);

#endif