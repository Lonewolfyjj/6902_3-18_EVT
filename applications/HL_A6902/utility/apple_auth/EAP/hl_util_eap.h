/**
 * @file hl_util_eap.h
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * @brief 苹果APP的EA Session外部配件协议通信功能具体实现
 * @version 1.0
 * @date 2023-02-07
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
 * <tr><td>2023-02-07     <td>v1.0     <td>lisonglin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_UTIL_EAP_H__
#define __HL_UTIL_EAP_H__

/* Includes ------------------------------------------------------------------*/
#include "hl_util_eap_type.h"
#include "hl_util_apple_packet.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

int hl_util_eap_init(hl_util_apple_p apple);

int hl_util_eap_deinit(hl_util_apple_p apple);

/**
 * hl_eap_process_oneshot
 * @brief 接收并处理EA Session消息
 * @param [in] iap2 iap2句柄
 * @return int 成功 0 | 失败 非0
 * @date 2023-02-07
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-02-07      <td>lisonglin     <td>新建
 * </table>
 */
int hl_util_eap_oneshot(hl_util_apple_p apple);

#endif
/*
 * EOF
 */
