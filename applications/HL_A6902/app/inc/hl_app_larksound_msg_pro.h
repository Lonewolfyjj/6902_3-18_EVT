/**
 * @file hl_app_larksound_msg_pro.h
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * @brief 接收LarkSound和MomaSound APP应用消息处理
 * @version 1.0
 * @date 2023-03-14
 * 
 * ██╗  ██╗ ██████╗ ██╗     ██╗  ██╗   ██╗██╗      █████╗ ███╗   ██╗██████╗ 
 * ██║  ██║██╔═══██╗██║     ██║  ╚██╗ ██╔╝██║     ██╔══██╗████╗  ██║██╔══██╗
 * ███████║██║   ██║██║     ██║   ╚████╔╝ ██║     ███████║██╔██╗ ██║██║  ██║
 * ██╔══██║██║   ██║██║     ██║    ╚██╔╝  ██║     ██╔══██║██║╚██╗██║██║  ██║
 * ██║  ██║╚██████╔╝███████╗███████╗██║   ███████╗██║  ██║██║ ╚████║██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝
 * @copyright Copyright (c) 2023 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2023-03-14     <td>v1.0     <td>lisonglin     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_APP_LARKSOUND_MSG_PRO_H__
#define __HL_APP_LARKSOUND_MSG_PRO_H__

/* Includes ------------------------------------------------------------------*/
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * 
 * @brief LarkSound和MomaSound APP应用消息处理函数
 * @param [in] p_msg 消息指针 
 * @date 2023-03-14
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-03-14      <td>lisonglin     <td>新建
 * </table>
 */
void hl_app_larksound_msg_pro(mode_to_app_msg_t *p_msg);

#endif /* __HL_APP_LARKSOUND_MSG_PRO_H__ */
/*
 * EOF
 */