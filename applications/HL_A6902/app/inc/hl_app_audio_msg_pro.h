/**
 * @file hl_app_audio_msg_pro.h
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
#ifndef __HL_APP_AUDIO_MSG_PRO_H__
#define __HL_APP_AUDIO_MSG_PRO_H__

/* Includes ------------------------------------------------------------------*/
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief 音频模块消息处理函数
 * @param [in] p_msg 消息指针
 * @date 2022-11-15
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-15      <td>luzhanghao     <td>新建
 * </table>
 */
void hl_app_audio_msg_pro(mode_to_app_msg_t *p_msg);

/**
 * @brief 音频流更新(判断当前状态选择正确的音频流)
 * @date 2022-12-05
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-12-05      <td>luzhanghao     <td>新建
 * </table>
 */
void hl_app_audio_stream_updata(void);

/**
 * 
 * @brief codec状态更新(判断当前状态是否关闭codec)
 * @date 2023-02-23
 * @author lixiang (rd37@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-02-23      <td>lixiang     <td>新建
 * </table>
 */
void hl_app_audio_switch(void);

/**
 * 
 * @brief 音频的增益设置
 * @param [in] gain 
 * @date 2023-02-27
 * @author lixiang (rd37@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-02-27      <td>lixiang     <td>新建
 * </table>
 */
void hl_app_audio_gain(int32_t gain);

/**
 * 
 * @brief uac增益设置
 * @param [in] uac_gain 
 * @date 2023-02-28
 * @author lixiang (rd37@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-02-28      <td>lixiang     <td>新建
 * </table>
 */
void hl_app_audio_gain_uac(int32_t uac_gain);

/**
 * 
 * @brief 
 * @date 2023-03-09
 * @author lixiang (rd37@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-03-09      <td>lixiang     <td>新建
 * </table>
 */
void hl_app_audio_cam_gain_update(void);

/**
 * 
 * @brief 
 * @date 2023-03-09
 * @author lixiang (rd37@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-03-09      <td>lixiang     <td>新建
 * </table>
 */
void hl_app_audio_hp_gain_update(void);
#endif /* __HL_APP_AUDIO_MSG_PRO_H__ */
/*
 * EOF
 */