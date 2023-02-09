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
    /// 更新一次电池信息到App，无参数
    HL_PM_BAT_INFO_UPDATE_CMD,
    /// 设置VBUS_C的插入状态，参数为<uint8_t *>(1:连接 0：未连接)
    HL_PM_SET_VBUS_C_STATE_CMD,
    /// 设置VBUS_P的插入状态，参数为<uint8_t *>(1:连接 0：未连接)
    HL_PM_SET_VBUS_P_STATE_CMD,
} hl_mod_pm_cmd_e;

typedef enum _hl_mod_pm_ind_e
{
    /// 通知 app 电量更新，参数为<uint8_t *>
    HL_SOC_UPDATE_IND,
    /// 通知 app 当前温度超过最大温度设定值<45℃>，参数为<int8_t *>
    HL_MAX_TEMP_ALERT_IND,
    /// 通知 app 当前温度超过最小温度设定值<-10℃>，参数为<int8_t *>
    HL_MIN_TEMP_ALERT_IND,
    /// 通知 app 当前的充电状态，参数为<hl_mod_pm_charge_state_e *>
    HL_CHARGE_STATE_IND,
    /// 通知 app 当前处于极低电量状态，无参数
    HL_ULTRA_LOWPOWER_IND,
} hl_mod_pm_ind_e;

typedef enum _hl_mod_pm_charge_state_e
{
    HL_CHARGE_STATE_UNKNOWN = 0,
    HL_CHARGE_STATE_NO_CHARGE,
    HL_CHARGE_STATE_CHARGING,
    HL_CHARGE_STATE_CHARGE_DONE,
} hl_mod_pm_charge_state_e;

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
 * @return int HL_MOD_PM_FUNC_RET_OK or HL_MOD_PM_FUNC_RET_ERR
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
 * @return int HL_MOD_PM_FUNC_RET_OK or HL_MOD_PM_FUNC_RET_ERR
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
 * @return int HL_MOD_PM_FUNC_RET_OK or HL_MOD_PM_FUNC_RET_ERR
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
 * @return int HL_MOD_PM_FUNC_RET_OK or HL_MOD_PM_FUNC_RET_ERR
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
 * @return int HL_MOD_PM_FUNC_RET_OK or HL_MOD_PM_FUNC_RET_ERR
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