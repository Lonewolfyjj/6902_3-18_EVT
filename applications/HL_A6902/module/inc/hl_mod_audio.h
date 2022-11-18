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
typedef enum _hl_mod_audio_ctrl_cmd
{
    /// 暂无
    HL_AUDIO_GET_INFO_CMD = 0x00,
    /// 暂无
    HL_AUDIO_SET_TIME_CMD,
    /// 暂无
    HL_AUDIO_GET_TIME_CMD,
    /// int(-103~24)
    HL_AUDIO_SET_GAIN_CMD,
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
} HL_ENUM8(hl_mod_audio_ctrl_cmd);
#else
typedef enum _hl_mod_audio_ctrl_cmd
{
    /// 暂无
    HL_AUDIO_GET_INFO_CMD = 0x00,
    /// int(-103~24)
    HL_AUDIO_SET_GAIN_CMD,
    /// int8_t(0=OFF, 1=ON)
    HL_AUDIO_SET_HP_AMP_CMD,    
} HL_ENUM8(hl_mod_audio_ctrl_cmd);
#endif

typedef enum _hl_mic_switch_e
{
    /// 内置麦
    HL_MIC_EXTERNAL = 0x00,
    /// 外置麦
    HL_MIC_INTERNAL,  
} HL_ENUM8(hl_mic_switch_e);

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/


/**
 * 
 * @brief init audio config
 * @param [in] p_msg_handle 上层传入的消息队列句柄
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
uint8_t hl_mod_audio_init(void *p_msg_handle);


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
uint8_t hl_mod_audio_io_ctrl(uint8_t cmd, void *ptr, uint16_t len);

#endif /* __HL_MOD_AUDIO_H__ */
/*
 * EOF
 */