/**
 * @file hl_mod_input.h
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 输入模块，检测按键的长按、短按、双击，检测插入的USB或者耳机，事件直接发给消息队列
 * @version V1.0
 * @date 2022-11-15
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
 * <tr><td>2022-11-15     <td>v1.0     <td>liujie     <td>内容
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
#ifndef __HL_MOD_INPUT_H__
#define __HL_MOD_INPUT_H__

#include "rtdef.h"
#include "hl_config.h"
#include "stdbool.h"
// #include "hl_test_pre.h"

typedef enum _hl_key_event_e
{
    HL_KEY_EVENT_IDLE = 0x00,
    /// 按键按下
    HL_KEY_EVENT_PRESS,
    /// 短按
    HL_KEY_EVENT_SHORT,
    /// 长按
    HL_KEY_EVENT_LONG,
    ///  双击
    HL_KEY_EVENT_DOUBLE,
    /// 长按后释放
    HL_KEY_EVENT_RELEASE,
} hl_key_event_e;

typedef enum _hl_switch_event_e
{
    /// 开关断开
    HL_SWITCH_EVENT_OFF = 0x00, 
    /// 开关闭合 
    HL_SWITCH_EVENT_ON,       
} hl_switch_event_e;

#if HL_IS_TX_DEVICE() 
//TX
typedef enum _input_mod_msg_cmd_e
{
    /// 空命令
    MSG_TX_IDLE = 0,
    /// 开关按键消息：类型hl_key_event_e
    MSG_TX_PWR_KEY,
    /// 降噪按键消息：类型hl_key_event_e
    MSG_TX_PAIR_KEY,
    /// 录制按键消息：类型hl_key_event_e
    MSG_TX_REC_KEY,
    /// usb插入检测消息：类型hl_switch_event_e
    MSG_TX_VBUS_DET,
    /// mic插入检测消息：类型hl_switch_event_e
    MSG_TX_MIC_DET,
    /// 组合按键1的消息：降噪和录制
    MSG_COMB_PAIR_REC,
    /// 组合按键2的消息：降噪和开关机
    MSG_COMB_REC_PWR,
    /// 组合按键3的消息：录制和开关机
    MSG_COMB_PAIR_PWR,    
} input_mod_msg_cmd_e;
#else
//RX
typedef enum _input_mod_msg_cmd_e
{
    /// 空命令
    MSG_RX_IDLE = 0,
    /// 开关按键消息：类型hl_key_event_e
    MSG_RX_PWR_KEY,
    /// 旋钮A消息：类型hl_key_event_e
    MSG_RX_A_VOL,
    /// 旋钮B消息：类型hl_key_event_e
    MSG_RX_B_VOL,
    /// 旋钮OK消息：类型hl_key_event_e
    MSG_RX_OK_VOL,
    /// usb插入检测消息：类型hl_switch_event_e
    MSG_RX_VBUS_DET,
    /// 相机口插入检测消息：类型hl_switch_event_e
    MSG_RX_CAM_DET,
    /// 耳机插入检测消息：类型hl_switch_event_e
    MSG_RX_HP_DET,
    /// 组合按键1的消息：左按和右按
    MSG_COMB_L_R_VOL,    
} input_mod_msg_cmd_e;
#endif

/**
 * 
 * @brief 输入模块初始化
 * @param [in] msg_hander 上报给APP的消息句柄
 * @return uint8_t 
 *          HL_SUCCESS 初始化成功
 *          HL_FAILED 初始化失败
 * @date 2022-09-08
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-08      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_mod_input_init(void* msg_hander);

/**
 * 
 * @brief 输入模块反初始化
 * @return uint8_t 
 *          HL_SUCCESS 初始化成功
 *          HL_FAILED 初始化失败
 * @date 2022-09-08
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-08      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_mod_input_deinit(void);

//uint8_t hl_mod_input_io_ctrl(uint8_t cmd, void* ptr, uint16_t len);

#endif /* __HL_MOD_INPUT_H__ */