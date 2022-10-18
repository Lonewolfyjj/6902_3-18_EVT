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



/* typedef -------------------------------------------------------------------*/
typedef enum _hl_mod_audio_ctrl_cmd
{
    HL_MOD_AUDIO_GET_INFO_CMD = 0x00,
    HL_MOD_AUDIO_SET_TIME_CMD,
    HL_MOD_AUDIO_GET_TIME_CMD,
    HL_MOD_AUDIO_SET_NOISE_CMD,
    HL_MOD_AUDIO_SET_GAIN_CMD,
    HL_MOD_AUDIO_SET_MUTE_CMD,
    HL_MOD_AUDIO_SET_EQ_CMD,
    HL_MOD_AUDIO_RECORD_CMD,
} hl_mod_audio_ctrl_cmd;
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