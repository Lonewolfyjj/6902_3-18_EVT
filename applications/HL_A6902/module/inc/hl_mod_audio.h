/**
 * @file hl_mod_audio.h
 * @author lixiang (rd37@hollyland-tech.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-13
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-13     <td>v1.0     <td>lixiang     <td>内容
 * </table>
 * 
 */ 


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_MOD_AUDIO_H__
#define __HL_MOD_AUDIO_H__

/* Includes ------------------------------------------------------------------*/
#include "hl_config.h"
#include "hl_util_general_type.h"

/* typedef -------------------------------------------------------------------*/
#if HL_IS_TX_DEVICE()

typedef enum _hl_mic_switch_e
{
    /// 内置麦
    HL_MIC_EXTERNAL = 0x00,
    /// 外置麦
    HL_MIC_INTERNAL,  
} HL_ENUM8(hl_mic_switch_e);

typedef enum _hl_mod_audio_ctrl_cmd
{
    /// 暂无
    HL_AUDIO_GET_INFO_CMD = 0x00,
    /// 暂无
    HL_AUDIO_SET_TIME_CMD,
    /// 暂无
    HL_AUDIO_GET_TIME_CMD,
    /// 音频流设置命令，hl_mod_audio_io_ctrl的参数参考<hl_stream_mode_e>
    HL_AUDIO_STREAM_SET_CMD,
    /// 设置DSP的所有通道增益 int(-103~24)
    HL_AUDIO_SET_GAIN_CMD,
    /// 设置DSP的左通道增益 int(-103~24)
    HL_AUDIO_SET_GAIN_L_CMD,
    /// 设置DSP的右通道增益 int(-103~24)
    HL_AUDIO_SET_GAIN_R_CMD,
    /// 降噪开关设置, 参数<hl_switch_e>(HL_SWITCH_ON--打开降噪, HL_SWITCH_OFF--关闭降噪)
    HL_AUDIO_SET_DENOISE_CMD,
    /// uint8_t(0=OFF, 1=ON)
    HL_AUDIO_SET_MUTE_CMD,
    /// uint8_t(0=OFF, 1=ON)
    HL_AUDIO_SET_EQ_CMD,
    /// uint8_t(0=OFF, 1=ON)
    HL_AUDIO_RECORD_CMD,
    /// uint8_t(0=External Mic, 1=Internal Mic)
    HL_AUDIO_MIC_SWITCH_CMD,    
    /// 无
    HL_USB_MSTORAGE_DISABLE_CMD, 
    /// 获取RTC时间 参数<audio_time>
    HL_AUDIO_GET_RTC_TIME_CMD, 
    /// 设置RTC时间 参数<audio_time>
    HL_AUDIO_SET_RTC_TIME_CMD, 
    /// 设置监听接口的左通道增益 int(-103~24)
    HL_AUDIO_SET_MIC_GAIN_CMD,
    /// 设置监听接口PGA的右通道增益 int(-103~24)
    HL_AUDIO_SET_MIC_PGA_GAIN_CMD,
    /// 格式化文件系统
    HL_AUDIO_MKFS_DFS_CMD,   
    /// 检查文件系统（如果无法挂载就自动格式化）
    HL_AUDIO_CHECK_DFS_CMD,
} HL_ENUM8(hl_mod_audio_ctrl_cmd);

typedef enum _hl_mod_audio_indicate
{
    /// UAC连接通知，参数<空>
    HL_AUDIO_UAC_LINK_IND = 1,
    /// 大容量状态：类型uint8_t(0=OFF, 1=ON)
    MSG_USB_MSTORAGE_DET,

} HL_ENUM8(hl_mod_audio_indicate);

#else

typedef enum _hl_mod_audio_indicate
{
    /// UAC连接通知，参数<空>
    HL_AUDIO_UAC_LINK_IND = 1,
    /// 大容量状态：类型uint8_t(0=OFF, 1=ON)
    MSG_USB_MSTORAGE_DET,
    /// VU左通道的数据：类型uint8_t(0 ~ 128)
    HL_AUDIO_L_VU_VAL,
    /// VU右通道的数据：类型uint8_t(0 ~ 128)
    HL_AUDIO_R_VU_VAL,

} HL_ENUM8(hl_mod_audio_indicate);

typedef enum _hl_mod_audio_ctrl_cmd
{
    /// 暂无
    HL_AUDIO_GET_INFO_CMD = 0x00,
    /// 设置DSP的所有通道增益 int(-103~24)
    HL_AUDIO_SET_GAIN_CMD,
    /// 设置DSP的左通道增益 int(-103~24)
    HL_AUDIO_SET_GAIN_L_CMD,
    /// 设置DSP的右通道增益 int(-103~24)
    HL_AUDIO_SET_GAIN_R_CMD,
    /// int8_t(0=OFF, 1=ON)
    HL_AUDIO_SET_HP_AMP_CMD, 
    /// 音频流设置命令，hl_mod_audio_io_ctrl的参数参考<hl_stream_mode_e>
    HL_AUDIO_STREAM_SET_CMD,
    /// int8_t(0=OFF, 1=ON)
    HL_AUDIO_SET_MIX_SWITCH_CMD, 
    /// 无
    HL_USB_MSTORAGE_DISABLE_CMD, 
    /// 获取RTC时间 参数<audio_time>
    HL_AUDIO_RTC_TIME_CMD, 
    /// 设置相机接口的左通道增益 int(-103~24)
    HL_AUDIO_SET_CAM_GAIN_L_CMD,
    /// 设置相机接口的右通道增益 int(-103~24)
    HL_AUDIO_SET_CAM_GAIN_R_CMD,
    /// 设置相机接口PGA的左通道增益 int(-103~24)
    HL_AUDIO_SET_CAM_PGA_GAIN_L_CMD,
    /// 设置相机接口PGA的右通道增益 int(-103~24)
    HL_AUDIO_SET_CAM_PGA_GAIN_R_CMD,
    /// 设置监听接口的左通道增益 int(-103~24)
    HL_AUDIO_SET_HP_GAIN_L_CMD,
    /// 设置监听接口的右通道增益 int(-103~24)
    HL_AUDIO_SET_HP_GAIN_R_CMD,
    /// 设置监听接口PGA的左通道增益 int(-103~24)
    HL_AUDIO_SET_HP_PGA_GAIN_L_CMD,
    /// 设置监听接口PGA的右通道增益 int(-103~24)
    HL_AUDIO_SET_HP_PGA_GAIN_R_CMD,
    /// 格式化文件系统
    HL_AUDIO_MKFS_DFS_CMD,   
    /// 检查文件系统（如果无法挂载就自动格式化）
    HL_AUDIO_CHECK_DFS_CMD, 
} HL_ENUM8(hl_mod_audio_ctrl_cmd);
#endif

typedef enum _hl_mod_audio_channel
{
    /// 左声道
    HL_AUDIO_CHANNEL_L = 0,
    /// 右声道
    HL_AUDIO_CHANNEL_R,
    /// 所有声道
    HL_AUDIO_CHANNEL_ALL,
} HL_ENUM8(hl_mod_audio_channel);

typedef enum _hl_stream_mode_e
{
    /// 音频流空闲状态
    HL_STREAM_IDLE = 0,
    /// PDM声卡 =》 输出声卡
    HL_STREAM_PDM2PLAY,
    /// 输入声卡 =》 输出声卡
    HL_STREAM_CAP2PLAY,
    /// UAC SPK => 输出声卡
    HL_STREAM_UAC2PLAY,
    /// 输入声卡 =》UAC MIC    UAC SPK => 输出声卡
    HL_STREAM_CAP2UAC_UAC2PLAY,
    /// 输入声卡 =》 输出声卡    输入声卡 =》UAC MIC
    HL_STREAM_CAP2PLAY_CAP2UAC,
} HL_ENUM8(hl_stream_mode_e);

#pragma pack(1)
typedef struct audio_time_t
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
} audio_time;
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * 
 * @brief init audio config
 * @param [in] p_msg_handle 上层传入的消息队列句柄<rt_mq_t>
 * @return uint8_t 0:succeed  1:error
 * @date 2022-09-13
 * @author lixiang (rd37@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-13      <td>lixiang     <td>新建
 * </table>
 */
uint8_t hl_mod_audio_init(rt_mq_t* p_msg_handle);


/**
 * 
 * @brief deinit audio config
 * @return uint8_t 0:succeed  1:error
 * @date 2022-09-13
 * @author lixiang (rd37@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-13      <td>lixiang     <td>新建
 * </table>
 */
uint8_t hl_mod_audio_deinit();

/**
 * 
 * @brief audio config
 * @param [in] cmd 命令
 * @param [in] ptr 参数指针
 * @param [in] len 参数长度
 * @return uint8_t 0:succeed  1:error
 * @date 2022-09-13
 * @author lixiang (rd37@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-13      <td>lixiang     <td>新建
 * </table>
 */
uint8_t hl_mod_audio_io_ctrl(hl_mod_audio_ctrl_cmd cmd, void *ptr, uint16_t len);

#endif /* __HL_MOD_AUDIO_H__ */
/*
 * EOF
 */