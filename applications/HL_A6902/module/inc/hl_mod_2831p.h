/**
 * @file hl_mod_2831p.h
 * @author lilin (lin.li@hollyland-tech.com)
 * @brief 2831p无线模块头文件
 * @version 1.0
 * @date 2022-09-13
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-13     <td>v1.0     <td>lilin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __HL_MOD_2831P_H__
#define __HL_MOD_2831P_H__

/* Includes ------------------------------------------------------------------*/

#include "rtthread.h"
#include "rtdevice.h"
#include "hl_config.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

int hl_mod_pm_init(void* msgHd);

void hl_mod_pm_deinit(void);

int hl_mod_pm_ctrl(int op, void* arg, int arg_size);

#endif
/*
 * EOF
 */