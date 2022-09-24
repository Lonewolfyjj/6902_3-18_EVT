/**
 * @file hl_mod_2831pl.h
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

#ifndef __HL_MOD_2831PL_H__
#define __HL_MOD_2831PL_H__

/* Includes ------------------------------------------------------------------*/

#include "rtthread.h"
#include "rtdevice.h"
#include "hl_config.h"

/* typedef -------------------------------------------------------------------*/

typedef enum _hl_mod_2831pl_op
{
    ///start wireless pair, type of parameter is <hl_mod_2831pl_wireless_pair_e> pointer
    HL_MOD_PL_WIRELESS_PAIR,
    ///switch slave or master, type of parameter is <hl_mod_2831pl_work_mode_e> pointer
    HL_MOD_PL_SWITCH_SLAVE_MASTER,
    ///get link state, type of parameter is <NULL> pointer
    HL_MOD_PL_GET_LINK_STATE,
} hl_mod_2831pl_op_e;

typedef enum _hl_mod_2831pl_wireless_pair
{
    PL_WIRELESS_PAIR_START,
    PL_WIRELESS_PAIR_STOP,
} hl_mod_2831pl_wireless_pair_e;

typedef enum _hl_mod_2831pl_work_mode
{
    PL_WORK_MODE_SLAVE,
    PL_WORK_MODE_MASTER,
    PL_WORK_MODE_UNKNOWN,
} hl_mod_2831pl_work_mode_e;
/* define --------------------------------------------------------------------*/

#define HL_MOD_PL_FUNC_RET_ERR 1
#define HL_MOD_PL_FUNC_RET_OK 0

/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

int hl_mod_2831pl_init(void* msgHd);

void hl_mod_2831pl_deinit(void);

int hl_mod_2831pl_ctrl(int op, void* arg, int arg_size);

#endif
/*
 * EOF
 */