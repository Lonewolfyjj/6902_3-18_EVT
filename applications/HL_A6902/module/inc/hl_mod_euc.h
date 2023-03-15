/**
 * @file hl_mod_euc.h
 * @author lilin (lin.li@hollyland-tech.com)
 * @brief （extern uart communication）外部串口通信模块头文件
 * @version 1.0
 * @date 2022-10-08
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
 * <tr><td>2022-10-08     <td>v1.0     <td>lilin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __HL_MOD_EUC_H__
#define __HL_MOD_EUC_H__

/* Includes ------------------------------------------------------------------*/

#include "rtthread.h"
#include "rtdevice.h"
#include "hl_config.h"
#include "stdbool.h"

/* typedef -------------------------------------------------------------------*/

#if HL_IS_TX_DEVICE() == 1

typedef enum _hl_mod_euc_cmd_e
{
    /// 设置设备电量，参数为<uint8_t *>
    HL_SET_SOC_CMD,
    /// 设置设备配对Mac地址，参数为<uint8_t[6]>
    HL_SET_PAIR_MAC_CMD,
    /// 设置设备Mac地址，参数为<uint8_t[6]>
    HL_SET_MAC_CMD,
    /// 设置设备充电状态，参数为<hl_mod_euc_charge_state_e *>
    HL_SET_CHARGE_STATE_CMD,
    /// 设置设备开机状态，参数为<uint8_t *>
    HL_SET_TURN_ON_STATE_CMD,
    /// 回复收纳盒准备关机，无参数
    HL_SHUTDOWN_ACK_CMD,
} hl_mod_euc_cmd_e;

typedef enum _hl_mod_euc_ind_e
{
    /// 通知 app 设备放入收纳盒，参数为<uint8_t> (0:Rx 1:tx1 2:tx2)。
    HL_IN_BOX_IND,
    /// 通知 app 设备离开收纳盒，无参数。
    HL_OUT_BOX_IND,
    /// 请求 app 获取设备电量，无参数。
    HL_GET_SOC_REQ_IND,
    /// 请求 app 获取设备配对的mac地址，无参数。
    HL_GET_PAIR_MAC_REQ_IND,
    /// 请求 app 设置设备配对mac地址，带参数<uint8_t[6]>。
    HL_SET_PAIR_MAC_REQ_IND,
    /// 请求 app 获取设备mac地址，无参数。
    HL_GET_MAC_REQ_IND,
    /// 请求 app 获取设备充电状态，无参数。
    HL_GET_CHARGE_STATE_REQ_IND,
    /// 请求 app 获取设备开机状态，无参数。
    HL_GET_TURN_ON_STATE_REQ_IND,
    /// 请求 app 关机，无参数。
    HL_SHUT_DOWN_REQ_IND,
} hl_mod_euc_ind_e;

#else  // rx

typedef enum _hl_mod_euc_cmd_e
{
    /// 设置设备电量，参数为<uint8_t *>
    HL_SET_SOC_CMD,
    /// 设置设备配对Mac地址，参数为<uint8_t[12]>
    HL_SET_PAIR_MAC_CMD,
    /// 设置设备Mac地址，参数为<uint8_t[6]>
    HL_SET_MAC_CMD,
    /// 开启Tx1配对，回复收纳盒开始配对，无参数
    HL_START_TX1_PAIR_CMD,
    /// 开启Tx2配对，回复收纳盒开始配对，无参数
    HL_START_TX2_PAIR_CMD,
    /// 设置设备充电状态，参数为<hl_mod_euc_charge_state_e *>
    HL_SET_CHARGE_STATE_CMD,
    /// 设置设备RTC时间，参数为<hl_mod_euc_rtc_st>
    HL_SET_RTC_TIME_CMD,
    /// HID发送开始录制命令，无参数
    HL_HID_START_RECORD_CMD,
    /// 设置设备开机状态，参数为<uint8_t *>
    HL_SET_TURN_ON_STATE_CMD,
    /// 回复收纳盒准备关机，无参数
    HL_SHUTDOWN_ACK_CMD,
    /// 手机应用LarkSound命令
    HL_LARKSOUND_CMD,
} hl_mod_euc_cmd_e;

typedef enum _hl_mod_euc_ind_e
{
    /// 通知 app 设备放入收纳盒，参数为<uint8_t> (0:Rx 1:tx1 2:tx2)。
    HL_IN_BOX_IND,
    /// 通知 app 设备离开收纳盒，无参数。
    HL_OUT_BOX_IND,
    /// 请求 app 获取设备电量，无参数。
    HL_GET_SOC_REQ_IND,
    /// 通知 app 收到TX1设备的电池信息更新，参数为<uint8_t *>。
    HL_TX1_BAT_INFO_UPDATE_IND,
    /// 通知 app 收到TX2设备的电池信息更新，参数为<uint8_t *>。
    HL_TX2_BAT_INFO_UPDATE_IND,
    /// 通知 app 收到BOX设备的电池信息更新，参数为<uint8_t *>。
    HL_BOX_BAT_INFO_UPDATE_IND,
    /// 通知 app TX1是否在收纳盒，参数为<bool *>。
    HL_TX1_IN_BOX_STATE_IND,
    /// 通知 app TX2是否在收纳盒，参数为<bool *>。
    HL_TX2_IN_BOX_STATE_IND,
    /// 请求 app 设置TX1的Mac地址进行配对，参数为<uint8_t[6]>。
    HL_SET_PAIR_MAC_TX1_REQ_IND,
    /// 请求 app 设置TX2的Mac地址进行配对，参数为<uint8_t[6]>。
    HL_SET_PAIR_MAC_TX2_REQ_IND,
    /// 请求 app 获取设备mac地址，无参数。
    HL_GET_MAC_REQ_IND,
    /// 请求 app 获取设备配对的mac地址，无参数。
    HL_GET_PAIR_MAC_REQ_IND,
    /// 通知 app 开始Tx1的有线配对，无参数。
    HL_TX1_PAIR_START_IND,
    /// 通知 app 开始Tx2的有线配对，无参数。
    HL_TX2_PAIR_START_IND,
    /// 通知 app Tx1配对结束，参数为<bool *>。
    HL_TX1_PAIR_STOP_IND,
    /// 通知 app Tx2配对结束，参数为<bool *>。
    HL_TX2_PAIR_STOP_IND,
    /// 请求 app 获取设备充电状态，无参数。
    HL_GET_CHARGE_STATE_REQ_IND,
    /// 通知 app 收到TX1设备的充电状态更新，参数为<hl_mod_euc_charge_state_e *>。
    HL_TX1_CHARGE_STATE_IND,
    /// 通知 app 收到TX2设备的充电状态更新，参数为<hl_mod_euc_charge_state_e *>。
    HL_TX2_CHARGE_STATE_IND,
    /// 通知 app 收到BOX设备的充电状态更新，参数为<hl_mod_euc_charge_state_e *>。
    HL_BOX_CHARGE_STATE_IND,
    /// 请求 app 获取设备的RTC时间，无参数。
    HL_GET_RTC_TIME_REQ_IND,
    /// 通知 app 收到BOX的开关盖信息更新，参数为<hl_mod_euc_box_lid_state_e>。
    HL_BOX_LID_STATE_UPDATE_IND,
    /// 请求 app 获取设备开机状态，无参数。
    HL_GET_TURN_ON_STATE_REQ_IND,
    /// 请求 app 关机，无参数。
    HL_SHUT_DOWN_REQ_IND,
} hl_mod_euc_ind_e;

#endif

typedef enum _hl_mod_euc_box_lid_state_e
{
    HL_MOD_EUC_BOX_LID_OPEN = 0,
    HL_MOD_EUC_BOX_LID_CLOSE,
} hl_mod_euc_box_lid_state_e;

typedef enum _hl_mod_euc_charge_state_e
{
    HL_MOD_EUC_CHARGE_STATE_UNKNOWN   = 0,
    HL_MOD_EUC_CHARGE_STATE_NO_CHARGE = 1,
    HL_MOD_EUC_CHARGE_STATE_CHARGING,
    HL_MOD_EUC_CHARGE_STATE_CHARGE_FULL,
    HL_MOD_EUC_CHARGE_STATE_CHARGE_FULL_DONE,
} hl_mod_euc_charge_state_e;

typedef struct _hl_mod_euc_rtc_st
{
    /// 秒  后7bit有效
    uint8_t second;
    /// 分  后7bit有效
    uint8_t minute;
    /// 时  12h制 后5bit有效 24h制 后6bit有效
    uint8_t hour;
    /// 天  后6bit有效
    uint8_t day;
    /// 周几    后3bit有效
    uint8_t weekday;
    ///月   后5bit有效
    uint8_t month;
    ///年
    uint8_t year;
} hl_mod_euc_rtc_st;
/* define --------------------------------------------------------------------*/

#define HL_MOD_EUC_FUNC_RET_ERR 1
#define HL_MOD_EUC_FUNC_RET_OK 0

/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * 
 * @brief 初始化 euc 模块
 * @param [in] msg_hd 消息队列的指针
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
int hl_mod_euc_init(rt_mq_t msg_hd);

/**
 * 
 * @brief 解初始化 euc 模块，会自动停止线程
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
int hl_mod_euc_deinit(void);

/**
 * 
 * @brief 启动 euc 模块，拉起线程和使能通信
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
int hl_mod_euc_start(void);

/**
 * 
 * @brief 停止 euc 模块，会停止线程和通信
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
int hl_mod_euc_stop(void);

/**
 * 
 * @brief euc 控制接口
 * @param [in] cmd 见<hl_mod_euc_cmd_e>
 * @param [in] arg 命令所需参数指针
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
int hl_mod_euc_ctrl(hl_mod_euc_cmd_e cmd, void* arg, int arg_size);

#endif
/*
 * EOF
 */