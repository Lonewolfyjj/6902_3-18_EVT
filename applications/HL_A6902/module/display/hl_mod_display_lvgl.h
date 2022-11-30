/**
 * @file hl_mod_display_lvgl.h
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-10-11
 * 
 * ██╗  ██╗ ██████╗ ██╗     ██╗  ██╗   ██╗██╗      █████╗ ███╗   ██╗██████╗ 
 * ██║  ██║██╔═══██╗██║     ██║  ╚██╗ ██╔╝██║     ██╔══██╗████╗  ██║██╔══██╗
 * ███████║██║   ██║██║     ██║   ╚████╔╝ ██║     ███████║██╔██╗ ██║██║  ██║
 * ██╔══██║██║   ██║██║     ██║    ╚██╔╝  ██║     ██╔══██║██║╚██╗██║██║  ██║
 * ██║  ██║╚██████╔╝███████╗███████╗██║   ███████╗██║  ██║██║ ╚████║██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-10-11     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_MOD_DISPLAY_LVGL_H__
#define __HL_MOD_DISPLAY_LVGL_H__

/* Includes ------------------------------------------------------------------*/
#include "hl_config.h"
#include "rtconfig.h"
#if  defined(RT_USING_LITTLEVGL2RTT) && (!HL_GET_DEVICE_TYPE)
#include "lvgl.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/*0 : 默认就是mipi屏幕,1：spi屏幕*/
#define HL_DISPLAY_SCREEN_HARDWARE      1
//#define __RTTHREAD__ // 开启lvgl的rtt线程创建
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief lvgl反初始化
 * @return uint8_t 
 * @date 2022-10-23
 * @author liujie (jie.liu@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-23      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_mod_display_lvgl_init(void);

/**
 * @brief lvgl初始化
 * @return uint8_t 
 * @date 2022-10-23
 * @author liujie (jie.liu@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-23      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_mod_display_lvgl_deinit(void);


#endif /*defined(RT_USING_LITTLEVGL2RTT) && (!HL_GET_DEVICE_TYPE)*/

#endif /*__HL_MOD_DISPLAY_LVGL_H__*/
/*
 * EOF
 */