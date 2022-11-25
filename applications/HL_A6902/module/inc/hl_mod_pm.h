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

typedef enum _hl_mod_pm_cmd_e
{
    /// 打开总电源，无参数
    HL_PM_POWER_UP_CMD,
    /// 关闭总电源，无参数
    HL_PM_POWER_DOWN_CMD,
} hl_mod_pm_cmd_e;

typedef enum _hl_mod_pm_ind_e
{
    /// 通知 app 电量更新，参数为<uint8_t *>
    HL_SOC_UPDATE_IND,
} hl_mod_pm_ind_e;

/* define --------------------------------------------------------------------*/

#define HL_MOD_PM_FUNC_RET_OK 0
#define HL_MOD_PM_FUNC_RET_ERR 1

/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * 
 * @brief pm 模块初始化
 * @param [in] msg_hd 消息队列指针
 * @return int 
 * @date 2022-11-25
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-25      <td>lilin     <td>新建
 * </table>
 */
int hl_mod_pm_init(rt_mq_t msg_hd);

/**
 * 
 * @brief pm 模块解初始化
 * @return int 
 * @date 2022-11-25
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-25      <td>lilin     <td>新建
 * </table>
 */
int hl_mod_pm_deinit(void);

/**
 * 
 * @brief pm 模块启动，会拉起线程和使能中断
 * @return int 
 * @date 2022-11-25
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-25      <td>lilin     <td>新建
 * </table>
 */
int hl_mod_pm_start(void);

/**
 * 
 * @brief pm 模块停止，会停止线程和失能中断
 * @return int 
 * @date 2022-11-25
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-25      <td>lilin     <td>新建
 * </table>
 */
int hl_mod_pm_stop(void);

/**
 * 
 * @brief pm 控制函数
 * @param [in] cmd 见<hl_mod_pm_cmd_e>
 * @param [in] arg 命令的参数指针
 * @param [in] arg_size 参数的大小
 * @return int 
 * @date 2022-11-25
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-25      <td>lilin     <td>新建
 * </table>
 */
int hl_mod_pm_ctrl(hl_mod_pm_cmd_e cmd, void* arg, int arg_size);

#endif
/*
 * EOF
 */