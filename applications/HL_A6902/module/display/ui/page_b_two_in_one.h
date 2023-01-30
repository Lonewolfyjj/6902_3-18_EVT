/**
 * @file page_b_two_in_one.h
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
#ifndef _PAGE_B_TWO_IN_ONE_H
#define _PAGE_B_TWO_IN_ONE_H

#include "lvgl.h"
#include "stdio.h"

/// @brief 点击枚举
typedef enum _hl_b_two_in_one_check_t
{
    /// @brief 点击左侧按钮
    HL_B_TWO_ONE_CHECK_LEFT = 0,
    /// @brief 点击右侧按钮
    HL_B_TWO_ONE_CHECK_RIGHT
}hl_b_two_in_one_check_t;

/// @brief 控制命令枚举
typedef enum _hl_b_two_in_one_choose_t
{
    /// @brief 选中左侧按钮
    HL_B_TWO_ONE_CHOOSE_LEFT = 0,
    /// @brief 选中右侧按钮
    HL_B_TWO_ONE_CHOOSE_RIGHT,
    /// @brief 删除风格样式
    HL_B_TWO_ONE_CHOOSE_DEL_STYLE,
    /// @brief 退出界面
    HL_B_TWO_ONE_CHOOSE_EXIT
}hl_b_two_in_one_choose_t;

/// @brief 回调函数类型
typedef void(*hl_b_two_in_one_event_cb)(hl_b_two_in_one_check_t);

/// @brief 初始化结构体
typedef struct __hl_lvgl_b_two_in_one_init_t
{
    /// @brief 初始化选中按钮
    hl_b_two_in_one_choose_t b_two_in_one_choose;
    /// @brief 左侧按钮未被选中图片
    const void * src11;
    /// @brief 左侧按钮被选中图片
    const void * src12;
    /// @brief 右侧按钮未被选中图片
    const void * src21;
    /// @brief 右侧按钮被选中图片
    const void * src22;
    /// @brief 左侧按钮字符
    char * ptr_lift;
    /// @brief 右侧按钮字符
    char * ptr_right;
    /// @brief 左右按钮顶部字符
    char * ptr_top;
    /// @brief 事件触发回调函数
    hl_b_two_in_one_event_cb func_cb;
}hl_lvgl_b_two_in_one_init_t;

/// @brief 控制结构体
typedef struct __hl_lvgl_b_two_in_one_ioctl_t
{
    /// @brief 选择按钮
    hl_b_two_in_one_choose_t b_two_in_one_choose;//
}hl_lvgl_b_two_in_one_ioctl_t;

/**
 * 
 * @brief 初始化函数
 * @param [in] init_data 参考结构体 hl_lvgl_b_two_in_one_init_t
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
void hl_mod_b_two_in_one_init(void * init_data);

/**
 * 
 * @brief 控制函数
 * @param [in] ctl_data 参考结构体 hl_lvgl_b_two_in_one_ioctl_t
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
void hl_mod_b_two_in_one_ioctl(void * ctl_data);

/**
 * 
 * @brief 点击控制函数
 * @param [in] num 被点击对象
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
void page_two_in_one_click(uint8_t num);
#endif