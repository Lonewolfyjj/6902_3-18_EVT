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
} hl_mod_euc_cmd_e;

typedef enum _hl_mod_euc_ind_e
{
    /// 通知 app 收到收纳盒的探测包，无参数。
    HL_IN_BOX_IND,
    /// 请求 app 获取设备电量，无参数。
    HL_GET_SOC_REQ_IND,
    /// 请求 app 获取设备配对的mac地址，无参数。
    HL_GET_PAIR_MAC_REQ_IND,
    /// 请求 app 设置设备配对mac地址，带参数<uint8_t[6]>。
    HL_SET_PAIR_MAC_REQ_IND,
    /// 请求 app 获取设备mac地址，无参数。
    HL_GET_MAC_REQ_IND,
} hl_mod_euc_ind_e;

#else // rx

typedef enum _hl_mod_euc_cmd_e
{
    /// 设置设备电量，参数为<uint8_t *>
    HL_SET_SOC_CMD,
} hl_mod_euc_cmd_e;

typedef enum _hl_mod_euc_ind_e
{
    /// 通知 app 收到收纳盒的探测包，无参数。
    HL_IN_BOX_IND,
    /// 请求 app 获取设备电量，无参数。
    HL_GET_SOC_IND,
} hl_mod_euc_ind_e;

#endif
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