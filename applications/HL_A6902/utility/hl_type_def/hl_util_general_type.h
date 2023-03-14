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

typedef enum _larksound_ctrl_cmd_enum_
{
    /// 获取软件版本号
    LARKSOUND_GET_SOFT_VERSION_CMD  = 0x01,
    /// 获取硬件版本号
    LARKSOUND_GET_HARD_VERSION_CMD  = 0x02,
    /// 获取设备连接状态
    LARKSOUND_GET_CONTECT_STATE_CMD = 0x03,
    /// 获取设备MAC地址
    LARKSOUND_GET_MAC_CMD           = 0x04,
    /// 获取设备电量
    LARKSOUND_GET_SOC_CMD           = 0x05,
    /// 获取设备UV值
    LARKSOUND_GET_UV_CMD            = 0x06,
    /// 获取设备ID
    LARKSOUND_GET_DEV_NUB_CMD       = 0x07,
    /// 获取TX增益
    LARKSOUND_GET_TX_GAIN_CMD       = 0x21,
    /// 设置TX增益
    LARKSOUND_SET_TX_GAIN_CMD       = 0x22,
    /// 获取降噪等级
    LARKSOUND_GET_DENOISE_CMD       = 0x23,
    /// 设置降噪等级
    LARKSOUND_SET_DENOISE_CMD       = 0x24,
    /// 获取低切状态
    LARKSOUND_GET_LOWCUT_CMD        = 0x25,
    /// 设置低切状态
    LARKSOUND_SET_LOWCUT_CMD        = 0x26,
    /// 获取MUTE状态
    LARKSOUND_GET_MUTE_CMD          = 0x27,
    /// 设置MUTE状态
    LARKSOUND_SET_MUTE_CMD          = 0x28,
    /// 获取音效模式
    LARKSOUND_GET_SOUND_EFFECT_CMD  = 0x29,
    /// 设置音效模式
    LARKSOUND_SET_SOUND_EFFECT_CMD  = 0x2A,
    /// 获取手机外放状态
    LARKSOUND_GET_PHONE_OUT_CMD     = 0x2B,
    /// 设置手机外放状态
    LARKSOUND_SET_PHONE_OUT_CMD     = 0x2C,
    /// 获取UAC增益
    LARKSOUND_GET_UAC_GAIN_CMD      = 0x2D,
    /// 设置UAC增益
    LARKSOUND_SET_UAC_GAIN_CMD      = 0x2E,
    /// 获取RX监听口增益
    LARKSOUND_GET_RX_GAIN_CMD       = 0x2F,
    /// 设置RX监听口增益
    LARKSOUND_SET_RX_GAIN_CMD       = 0x30,
    /// 获取相机口增益
    LARKSOUND_GET_CAM_GAIN_CMD      = 0x31,
    /// 设置相机口增益
    LARKSOUND_SET_CAM_GAIN_CMD      = 0x32,
    /// 开始升级
    LARKSOUND_UPDATE_START_CMD      = 0x41,
    /// 传输升级包
    LARKSOUND_UPDATE_PACKET_CMD     = 0x42,
    /// 结束升级
    LARKSOUND_UPDATE_STOP_CMD       = 0x43,
    /// 维持心跳包
    LARKSOUND_KEEP_ALIVE_CMD        = 0x61,
    /// 设备重启
    LARKSOUND_REBOOT_CMD            = 0x62,
} HL_ENUM8(hl_larksound_ctrl_cmd_e);

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#endif
/*
 * EOF
 */