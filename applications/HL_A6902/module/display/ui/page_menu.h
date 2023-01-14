/**
 * @file page_menu.h
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
#ifndef _PAGE_MENU_H
#define _PAGE_MENU_H

#include "lvgl.h"
#include "stdio.h"

/// 最大菜单数量
#define MAX_MENU_NUMBER     12
/// 添加菜单
#define ADD_IMG_DATA(OBJ,LAB,SRC,PTR) {.obj = OBJ,.lab = LAB,.pic_src = SRC,.ptr = PTR}

/// @brief 菜单数据结构体
typedef struct _menu_data_t{
    /// @brief 菜单图片对象
    lv_obj_t *obj;
    /// @brief 菜单标签对象
    lv_obj_t *lab;
    /// @brief 菜单图片数据
    const lv_img_dsc_t *pic_src;
    /// @brief 菜单标签字符
    const char *ptr;
}menu_data_t;

/// @brief 回调函数
typedef void(*event_cb)(uint32);

/**
 * 
 * @brief 获取菜单对象
 * @param [in] num 需要获取的对象编号
 * @return lv_obj_t* 
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
lv_obj_t * hl_menu_obj_get(uint8_t num);

/**
 * 
 * @brief 设置居中图标
 * @param [in] pos 图标编号
 * @param [in] en FALSE 表示切换居中的图标，  true表示选中图标  都会触发中断
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
void lv_set_icon_postion(uint8_t pos, bool en);

/**
 * 
 * @brief 初始化函数
 * @param [in] data 菜单图标数据
 * @param [in] menu_num 菜单数量
 * @param [in] func 回调函数
 * @param [in] center 初始化居中的图标编号
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
void page_menu_init(menu_data_t *data,uint8_t menu_num,event_cb func,int8_t center);

/**
 * 
 * @brief 删除页面函数
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
void lv_menu_exit(void);

#endif
