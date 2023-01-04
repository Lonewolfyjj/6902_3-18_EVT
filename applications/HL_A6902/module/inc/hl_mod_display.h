/**
 * @file hl_mod_display.h
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 显示模块
 * @version V1.0
 * @date 2022-11-11
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
 * <tr><td>2022-11-11     <td>v1.0     <td>liujie     <td>内容
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

#ifndef __HL_MOD_DISPLAY_H__
#define __HL_MOD_DISPLAY_H__

#include "rtdef.h"
#include "hl_config.h"
#include "hl_util_general_type.h"

#define HL_DISPLAY_SUCCESS RT_EOK
#define HL_DISPLAY_FAILED RT_ERROR

#if HL_IS_TX_DEVICE()
// TX
typedef enum _hl_led_net_mode
{
    /// 当前TX正在配对
    LED_NET_MODE_PAIR = 0,
    /// 已连接
    LED_NET_MODE_CONNECTED,
    /// 重连
    LED_NET_MODE_RECONNECTION,

    LED_NET_MODE_ID_CNT

} HL_ENUM8(hl_led_net_mode);

typedef enum _hl_charge_mode
{
    /// 未充电
    NOT_CHARGE = 0,
    /// 充电中
    CHARGING,
    /// 已充满
    FULL_CHARGE,
} HL_ENUM8(hl_charge_mode);

typedef enum _hl_led_switch
{
    /// 关闭/不充电/关闭降噪/正常/关闭MUTE/关闭录制
    SWITCH_CLOSE = 0,
    /// 打开/充电/开启降噪/故障/开启MUTE/升级中/电量欠电 /开启录制
    SWITCH_OPEN,
    
} HL_ENUM8(hl_led_switch);

typedef enum _hl_cmd_e
{
    /// 读取当前的模块故障状态，通过指针访问变量，值为 1表示 异常 0 表示正常
    LED_FAULT_STATUS_READ_CMD,
    /// 连接状态变更命令，hl_mod_display_io_ctrl的参数参考<hl_led_net_mode>,参数为四字节
    LED_NET_MODLE_CMD,
    /// 录制LED灯变更命令，hl_mod_display_io_ctrl的参数参考<hl_led_switch>,参数为四字节
    LED_RECORD_STATE_CMD,
    /// 录制是否异常LED灯，hl_mod_display_io_ctrl的参数参考<hl_led_switch>,参数为四字节
    LED_RECORD_FAULT_CMD,
    /// 降噪开关命令，hl_mod_display_io_ctrl的参数参考<hl_led_switch>,参数为四字节
    LED_SWITCH_NOISE_REDUCTION_CMD,
    /// 充电状态标志，hl_mod_display_io_ctrl的参数参考<hl_charge_mode>,参数为四字节 
    LED_CHARGE_STATUS_CMD,
    /// 故障标志，hl_mod_display_io_ctrl的参数参考<hl_led_switch>,参数为四字节 
    LED_SWITCH_FAULT_CMD,
    /// MUTE状态，hl_mod_display_io_ctrl的参数参考<hl_led_switch>,参数为四字节 
    LED_SWITCH_MUTE_CMD,
    ///升级状态，hl_mod_display_io_ctrl的参数参考<hl_led_switch>,参数为四字节 
    LED_SWITCH_UPDATE_CMD,
    ///电量值，hl_mod_display_io_ctrl的参数参考<uint8_t>,参数为四字节 
    LED_BATTERY_VAL_CMD,
    ///LED亮度调节(两个灯一起)，hl_mod_display_io_ctrl的参数参考<uint8_t (0-255)>,参数为1字节 
    LED_BRIGHT_SET_CMD,
    /// 命令个数统计
    MSG_ID_CNT

} hl_cmd_e;



#else
// RX
typedef enum _hl_screen_page_e
{
    ///无页面
    PAGE_NONE,
    /// LOGO页面
    PAGE_LOGO,
    /// 主页面
    PAGE_HOME,
    
    PAGE_POWEROFF_CHARGE,
    /// 一级菜单页面
    PAGE_MAIN_MENU,
    /// 模式选择页面
    PAGE_VOICE_MODEL,

    /// TX配置页面
    PAGE_TX_CONF_MENU,
    /// TX增益调节页面
    PAGE_TX_GAIN_CONF,
    /// TX1增益设置界面
    PAGE_TX_GAIN_TX1,
    /// TX2增益设置界面
    PAGE_TX_GAIN_TX2,
    /// 低切配置
    PAGE_TX_LOW_CUT,
    /// 自动录制
    PAGE_AUTO_RECORD,
    /// 录制保护
    PAGE_RECORD_PROTECT,
    /// 录制格式化
    PAGE_RECORD_FORMAT,
    /// 自动关机
    PAGE_AUTO_POWEROFF,
    /// TXLED亮度调节
    PAGE_TX_LED_BRIGHT,
    /// @brief 盒子页面
    PAGE_BOX_IN,
    /// 降噪强度
    PAGE_NOISE_REDUCTION_INTENSITY,

    /// 音量调节选择菜单
    PAGE_VOLUME_MENU,
    /// 监听音量配置界面
    PAGE_MONITOR_VOLUME_SET,
    /// UAC输入音量设置界面(新的原型去掉了，但是代码保留)
    // PAGE_UACIN_VOLUME_SET,
    /// UAC输出音量设置界面
    PAGE_UACOUT_VOLUME_SET,

    ////监听设置
    PAGE_MONITOR_SET,

    /// 音效模式
    PAGE_SOUND_EFFECT_MODE,

    /// 配对设置
    PAGE_PAIR,
    /// 时间设置
    PAGE_SYS_TIME_SET,
    /// 版本信息
    PAGE_VERSION,
    /// 恢复出厂设置
    PAGE_RESTORE,

    /// 通用设置
    PAGE_OTHER_SET,
    /// TX快捷配置页面
    PAGE_FAST_TX_CONFIG,
    /// 单声道 立体声选择界面
    PAGE_SOUND_MODULE,
    //// 快捷LINE OUT输出音量页面(安全音轨)
    // PAGE_LINE_OUT_SAFE_TRACK,
    /// 快捷LINE OUT输出音量页面(立体声，左声道)
    PAGE_LINE_OUT_STEREO_LEFT,
    /// 快捷LINE OUT输出音量页面(立体声,右声道)
    PAGE_LINE_OUT_STEREO_RIGHT,
    // /// 快捷LINE OUT输出音量页面(单声道)
    // PAGE_LINE_OUT_MONO,
    // LINE OUT快捷设置
    PAGE_QUICK_SETTINGS,
    ///  页面总数
    PAGE_MAX,  

} HL_ENUM8(hl_screen_page_e);

typedef struct _hl_display_systime_s
{
    uint16_t year;
    uint8_t month;
    uint8_t data;
    // 24小时机制
    uint8_t hour;
    uint8_t min;
}hl_display_systime_s;


typedef enum _hl_display_low_cut_e
{
    LOW_CUT_OFF,
    LOW_CUT_75HZ,
    LOW_CUT_150HZ,

}HL_ENUM8(hl_display_low_cut_e);

typedef enum _hl_display_vocie_mode_e
{
    /// 高保真
    HIGH_FIDELITY = 0,
    ///人声增强
    SOUND_ENHANCEMENT,

}HL_ENUM8(hl_display_vocie_mode_e);

typedef enum _hl_display_voice_monitor_e
{
    /// TX输入
    MONITOR_TX_IN = 0,
    /// UAC输入
    MONITOR_UAC_IN,

}HL_ENUM8(hl_display_voice_monitor_e);

typedef enum _hl_display_box_charge_state
{
    /// RX 未入盒子
    BOX_CHARGE_RX_NOT = 0,
    /// TX1 2 RX都入盒子
    BOX_CHARGE_RX_TX12,
    /// TX1 RX入盒子
    BOX_CHARGE_RX_TX1,
    /// TX2 RX入盒子
    BOX_CHARGE_RX_TX2,
    /// @brief  RX单独在盒子
    BOX_CHARGE_RX,
}HL_ENUM8(hl_display_box_charge_state);

typedef enum _hl_display_sound_module_e
{
    /// 立体声
    STEREO = 0,
    /// 单声道,
    MONO,
    /// 安全音轨
    SAFE_TRACK,


}HL_ENUM8(hl_display_sound_module_e);

typedef enum _hl_display_fault_code_e
{
    /// 屏幕错误
    SCREEM_FAULT = 0,
    /// 触摸错误 
    TP_FAULT = 1,

}HL_ENUM8(hl_display_fault_code_e);

typedef enum _hl_out_msg_e
{
    /// 是否恢复出厂设置 1:恢复出厂设置 0 ：不恢复出厂设置 uint8_t
    RESTORE_SET_SWITCH_IND,

    ///自动录制状态 1：开启 0 ：关闭 uint8_t 
    AUTO_RECORD_SWITCH_IND,

    ///录制保护状态 1：开启 0 ：关闭 uint8_t 
    AUTO_RECORD_PORTECT_SWITCH_IND,

    ///声音模式  立体声  单声道  安全音轨 hl_display_sound_module_e
    SOUND_MODULE_SET_IND,

    /// TX12降噪开关  1： 开 0 ：关 uint8_t 
    TX1_NOISE_SWITCH_IND,
    TX2_NOISE_SWITCH_IND,

    /// TX12录制开关  1： 开 0 ：关 uint8_t 
    TX1_RECORD_STATE_SWITCH_IND,
    TX2_RECORD_STATE_SWITCH_IND,

    /// XT1/2 MUTE状态 1：MUTE 0 ：正常 uint8_t 
    TX1_MUTE_SWITCH_SWITCH_IND,
    TX2_MUTE_SWITCH_SWITCH_IND,

    /// XT1/2 增益 int8_t
    TX1_GAIN_VAL_IND,
    TX2_GAIN_VAL_IND,

    /// TX1/2 格式化 1格式化
    TX1_FS_FORMAT_VAL_IND,
    TX2_FS_FORMAT_VAL_IND,

    /// 音效模式模式 （高保真） 见枚举<hl_display_vocie_mode_e>
    VOICE_MODULE_VAL_IND,

    /// 低切模式 见枚举<hl_display_low_cut_e>
    LOW_CUT_VAL_IND,

    /// 降噪等级 uint8_t 
    TX_NOISE_LEVEL_VAL_IND,

    /// TX lineout音量   int8_t
    TX1_LINE_OUT_VOLUME_VAL_IND,
    TX2_LINE_OUT_VOLUME_VAL_IND,

    /// 监听设置 见枚举：hl_display_voice_monitor_e
    MONITOR_CATEGORY_VAL_IND, 

    /// UAC 输出音量设置
    UAC_OUT_VOLUME_VAL_IND,
    /// LINE_OUT音频音量 int8_t
    LINE_OUT_VOLUME_VAL_IND,

    ///监听音量 int8_t
    MONITOR_VOLUME_VAL_IND,

    /// 指示灯的亮度等级 uint8_t : 0-255
    LED_BRITNESS_VAL_IND,

    /// 系统时间设置 <hl_display_systime_s>
    SYSTIME_SET_VAL_IND,

    /// 自动关机模式 见 uint32_t 单位min  0表示永不关机目前只有15和30min两个选项
    POWEROFF_SET_VAL_IND,

    ///配对 uint8_t 0:取消配对 1：进行配对
    DEVICE_PAIR_IND,

    IND_CNT,

} HL_ENUM8(hl_out_msg_e);

typedef enum _hl_cmd_e
{


    /// 读取命令 ， 以READ_CMD结尾 获取当前页面 <hl_screen_page_e>
    NOW_PAGE_GET_READ_CMD,

    /// 读取当前熄屏状态 ， 以READ_CMD结尾 1已经熄屏， 0未熄屏
    NOW_SCREEN_OFF_READ_CMD,

    /// 读取显示模块系统的故障信息  错误码会返回指针<hl_display_fault_code_e>  
    DISPLAY_FAULT_STATUS_READ_CMD,

    /// 读取整个系统的故障信息(故障码) 传入访问BUF的指针，BUF长度16字节
    DEVICE_FAULT_CODE_VAL_READ_CMD,

    /* *******************************命令相关******************/

    /// 恢复出厂设置命令 1：恢复出厂设置
    RESTORE_SET_SWITCH_CMD,

    /// TX12降噪开关  1： 开 0 ：关
    TX1_NOISE_SWITCH_CMD,
    TX2_NOISE_SWITCH_CMD,
    /// TX12录制开关  1： 开 0 ：关 uint8_t 
    TX1_RECORD_STATE_SWITCH_CMD,
    TX2_RECORD_STATE_SWITCH_CMD,

    
    /// TX1/2/RX/CASE 充电状态 1： 充电 0 ：正常 uint8_t 
    TX1_CHARGE_STATUS_SWITCH_CMD,
    TX2_CHARGE_STATUS_SWITCH_CMD,
    RX_CHARGE_STATUS_SWITCH_CMD,
    BOX_CHARGE_STATUS_SWITCH_CMD,

    /// USB插入状态 1：插入 0 ：拔出 uint8_t 
    USB_IN_SWITCH_CMD,

    /// line_out 口插入状态1：插入 0 ：拔出 uint8_t 
    LINE_OUT_IN_SWITCH_CMD,
    /// 监听口插入状态 1：插入 0 ：拔出 uint8_t 
    MONITOR_IN_SWITCH_CMD,

    /// XT1、2MUTE状态 1：MUTE 0 ：正常 uint8_t 
    TX1_MUTE_SWITCH_SWITCH_CMD,
    TX2_MUTE_SWITCH_SWITCH_CMD,

    /// 升级状态 <uint8_t>类型 1：升级中 0 ：正常
    OTA_UPDATE_STATE_SWITCH_CMD,
    /// 重新开始熄屏计数 设置 （无参数）
    SCREEN_OFF_STATUS_SWITCH_CMD,

    /// 放入盒子状态 1：盒子中 0 ：正常 < hl_display_box_charge_state>
    IN_BOX_STATE_VAL_CMD,

    /// 自动录制状态 1：开启  0：关闭 ，
    AUTO_RECORD_SWITCH_CMD,

    ///录制保护状态 1：开启  0：关闭 ，
    AUTO_RECORD_PORTECT_SWITCH_CMD,

    // /// TX1网络状态 1:已连接 2：未连接
    // TX1_NET_SWITCH_CMD,
    // /// TX2网络状态 1:已连接 2：未连接
    // TX2_NET_SWITCH_CMD,
    
    /* *******************************参数相关******************/
    // Rx当前无线通讯状态，参数<hl_rf_state_e>
    RX_RF_STATE_VAL_CMD,

    /// 升级进度下发 0-100 <uint8_t>
    OTA_UPDATE_REMAINED_VAL_CMD,

    /// 整个系统的故障信息(故障码) 单个故障信息传入，<uint8_t>
    DEVICE_FAULT_CODE_VAL_CMD,

    ///  音效模式 （）见枚举<hl_display_vocie_mode_e>
    VOICE_MODULE_VAL_CMD,

    /// 低切模式 见枚举<hl_display_low_cut_e>
    LOW_CUT_VAL_CMD,

    /// 电量更新命令： 值uint8_t 0-100
    TX1_BAT_VAL_VAL_CMD,
    TX2_BAT_VAL_VAL_CMD,
    RX_BAT_VAL_VAL_CMD,
    CASE_BAT_VAL_VAL_CMD,

    /// TX1 UV值更新  uint8_t 
    TX1_VU_VAL_CMD,
    /// TX2 UV值更新  uint8_t 
    TX2_VU_VAL_CMD,

    /// TX1信号强度 uint8_t
    TX1_SIGNAL_VAL_CMD,
    /// TX2信号强度 uint8_t
    TX2_SIGNAL_VAL_CMD,
    
    /// 降噪等级
    TX_NOISE_LEVEL_VAL_CMD,

    /// XT1/2 增益设置 int8_t
    TX1_GAIN_VAL_CMD,
    TX2_GAIN_VAL_CMD,

    /// TX lineout音量   int8_t
    TX1_LINE_OUT_VOLUME_VAL_CMD,
    TX2_LINE_OUT_VOLUME_VAL_CMD,

    /// LINE_OUT音频音量设置 int8_t
    LINE_OUT_VOLUME_VAL_CMD,

    ///监听口音量设置 int8_t
    MONITOR_VOLUME_VAL_CMD,

    /// 监听设置 见枚举：hl_display_voice_monitor_e
    MONITOR_CATEGORY_VAL_CMD, 
    /// UAC输入音量 int8_t
    UAC_IN_VOLUME_VAL_CMD,

    /// UAC输出音频 int8_t
    UAC_OUT_VOLUME_VAL_CMD,

    /// 指示灯的亮度等级 uint8_t : 0-255
    LED_BRITNESS_VAL_CMD,
    /// TX剩余录制时间 uint8_t : 0-255
    TX1_REMAINED_RECORD_TIME_VAL_CMD,
    /// 
    TX2_REMAINED_RECORD_TIME_VAL_CMD,


    /* *******************************字符相关******************/
    /// TX1版本信息 ASCII码 10字节的BUF<char*>
    TX1_VER_STRING_CMD,
    /// TX2版本信息 ASCII码 10字节的BUF<char*>
    TX2_VER_STRING_CMD,
    /// RX版本信息 ASCII码 10字节的BUF<char*>
    RX_VER_STRING_CMD,
    /// 盒子版本 （盒子中显示）ASCII码 10字节的BUF<char*>
    CASE_VER_STRING_CMD,
    /// SN号 ASCII码 32字节的BUF<char*>
    RX_SN_STRING_CMD,


    /// 输入事件更新（按键、旋钮、插入）<mode_to_app_msg_t>
    MSG_INPUT_UPDATE_CMD,

    ///  系统时间设置 <hl_display_systime_s> 
    SYSTIME_SET_VAL_CMD,

    /// 自动关机模式设置 见 uint32_t 单位min  0表示永不关机，目前原型自动关机只有15和30min两个选项
    POWEROFF_SET_VAL_CMD,

    /// 关机充电 页面 1 0
    OUT_BOX_CHARGER_SWITCH_CMD,

    MSG_ID_CNT
    
} HL_ENUM8(hl_cmd_e);



#endif
/**
 * @brief 显示模块初始化
 * @param [in] display_msq 指向APP传入的消息队列
 * @return uint8_t 结果 HL_DISPLAY_SUCCESS
 *                  失败：HL_DISPLAY_FAILED
 * @date 2022-11-15
 * @author liujie (jie.liu@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-15      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_mod_display_init(void* display_msq);

/**
 * @brief 显示模块反初始化
 * @return uint8_t 结果 成功：HL_DISPLAY_SUCCESS 失败：失败：HL_DISPLAY_FAILED
 * @date 2022-11-15
 * @author liujie (jie.liu@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-15      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_mod_display_deinit(void);

/**
 * @brief 显示模块控制命令，包括，LED控制，OLED全屏显示颜色等
 * @param [in] cmd 命令：见<hl_cmd_e>
 * @param [in] ptr 参数的指针，使用具体见命令的注释
 * @param [in] len 长度，具体见命令的注释
 * @return uint8_t 
 * @date 2022-11-15
 * @author liujie (jie.liu@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-15      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_mod_display_io_ctrl(uint8_t cmd, void* ptr, uint16_t len);

#endif /*__HL_MOD_DISPLAY_H__*/

/*
 * EOF
 */