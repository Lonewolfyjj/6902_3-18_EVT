/**
 * @file hl_mod_pm.h
 * @author lilin (lin.li@hollyland-tech.com)
 * @brief 电源管理模块头文件
 * @version 1.0
 * @date 2022-09-29
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
 * <tr><td>2022-09-29     <td>v1.0     <td>lilin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __HL_MOD_PM_H__
#define __HL_MOD_PM_H__

/* Includes ------------------------------------------------------------------*/

#include "rtthread.h"
#include "rtdevice.h"
#include "hl_config.h"
#include "stdbool.h"

/* typedef -------------------------------------------------------------------*/
typedef struct _HL_PMIC_INPUT_PARAM_T
{
    uint8_t cfg_opt;//参数参考 ： sgm41518 或者 sy6971 枚举内容
    uint8_t param; //参数参考 ： 功能选项参数宏
}HL_PMIC_INPUT_PARAM_T;
/* define --------------------------------------------------------------------*/

#define HL_MOD_PM_FUNC_RET_OK 0
#define HL_MOD_PM_FUNC_RET_ERR 1

#define HL_MOD_RK2108_POWER_UP_CMD  0xFE
#define HL_MOD_RK2108_POWER_DOWN_CMD 0xFF

/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
int hl_mod_pm_init(void* msg_hd);

int hl_mod_pm_deinit(void);

int hl_mod_pm_start(void);

int hl_mod_pm_stop(void);

int hl_mod_pm_ctrl(int op, void* arg, int arg_size);

#endif
/*
 * EOF
 */