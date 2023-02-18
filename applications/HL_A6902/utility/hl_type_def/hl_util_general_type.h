/**
 * @file hl_util_general_type.h
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-11-14
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
 * <tr><td>2022-11-14     <td>v1.0     <td>luzhanghao     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_UTIL_GENERAL_TYPE__
#define __HL_UTIL_GENERAL_TYPE__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* typedef -------------------------------------------------------------------*/
#ifndef HL_ENUM8
#define HL_ENUM8(EnumName) \
    Enum_##EnumName;       \
    typedef uint8_t EnumName
#endif

#ifndef HL_ENUM16
#define HL_ENUM16(EnumName) \
    Enum_##EnumName;        \
    typedef uint16_t EnumName
#endif

typedef enum _hl_switch_e
{
    /// 关闭
    HL_SWITCH_OFF = 0x00,
    /// 打开
    HL_SWITCH_ON,
} HL_ENUM8(hl_switch_e);

typedef enum _hl_rf_state_e
{
    /// 无配对设备
    HL_RF_UNCONNECT = 0x00,
    /// 左声道已配对
    HL_RF_L_CONNECT,
    /// 右声道已配对
    HL_RF_R_CONNECT,
    /// 左右声道已配对
    HL_RF_LR_CONNECT,
    /// 正在配对中
    HL_RF_PAIRING,
} HL_ENUM8(hl_rf_state_e);

typedef enum _hl_rf_channel_e
{
    /// 左声道
    HL_RF_LEFT_CHANNEL = 0x00,
    /// 右声道
    HL_RF_RIGHT_CHANNEL,
    /// 左右双声道
    HL_RF_DOUBLE_CHANNEL,
} HL_ENUM8(hl_rf_channel_e);

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
} HL_ENUM8(hl_key_event_e);

typedef struct _hl_audio_time_t_
{
    /// 年
    uint16_t year;
    /// 月
    uint8_t month;
    /// 日
    uint8_t day;
    /// 时
    uint8_t hour;
    /// 分
    uint8_t minute;
    /// 秒
    uint8_t second;
} __attribute__((packed, aligned(1))) hl_audio_time_t;

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#endif
/*
 * EOF
 */