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


/* typedef -------------------------------------------------------------------*/
#if HL_GET_DEVICE_TYPE()
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
    /// uint8_t(0=OFF, 1=ON)
    HL_AUDIO_SET_DENOISE_CMD,
    /// uint8_t(0=OFF, 1=ON)
    HL_AUDIO_SET_MUTE_CMD,
    /// uint8_t(0=OFF, 1=ON)
    HL_AUDIO_SET_EQ_CMD,
    /// uint8_t(0=OFF, 1=ON)
    HL_AUDIO_RECORD_CMD,
    /// uint8_t(0=External Mic, 1=Internal Mic)
    HL_AUDIO_MIC_SWITCH_CMD,  
} hl_mod_audio_ctrl_cmd;
#else
typedef enum _hl_mod_audio_ctrl_cmd
{
    /// 暂无
    HL_AUDIO_GET_INFO_CMD = 0x00,
    /// int(-103~24)
    HL_AUDIO_SET_GAIN_CMD,
    /// int8_t(0=OFF, 1=ON)
    HL_AUDIO_SET_HP_AMP_CMD,    
} hl_mod_audio_ctrl_cmd;
#endif

typedef enum _hl_mod_audio_switch
{
    /// 关闭
    HL_AUDIO_OFF = 0x00,
    /// 打开
    HL_AUDIO_ON,  
} hl_mod_audio_switch;

typedef enum _hl_mod_audio_mic
{
    /// 内置麦
    HL_AUDIO_MIC_EXTERNAL = 0x00,
    /// 外置麦
    HL_AUDIO_MIC_INTERNAL,  
} hl_mod_audio_mic;
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