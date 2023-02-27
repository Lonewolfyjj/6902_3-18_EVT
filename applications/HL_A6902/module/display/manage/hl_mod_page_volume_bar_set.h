/**
 * @file hl_mod_page_volume_bar_set.h
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-12-20
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
 * <tr><td>2022-12-20     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _HL_MOD_PAGE_VOLUME_BAR_SET_H__
#define _HL_MOD_PAGE_VOLUME_BAR_SET_H__


#include "rtdef.h"
#include "hl_mod_display.h"

#define MAX_LINEOUT_VOLUME 10
#define MIN_LINEOUT_VOLUME -10

#define MAX_MONITOR_VOLUME 8
#define MIN_MONITOR_VOLUME -8

#define AUTO_GOTO_HOME_TIMER 5000

#define AUTO_SILDER_TRG_TIME 50

/* Includes ------------------------------------------------------------------*/
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
typedef void (*save_func)(void);
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void hl_mod_page_volume_init(int16_t volume);
void hl_mod_page_volume_exit(void);
void hl_mod_page_volume_update(int16_t volume);
void hl_mod_page_volume_loop(hl_out_msg_e msg, save_func func,int16_t left, int16_t right);
int16_t hl_mod_page_volume_get(void);
uint8_t hl_mod_page_volume_send(hl_out_msg_e msg_cmd,int16_t *get_data);
#endif
/*
 * EOF
 */