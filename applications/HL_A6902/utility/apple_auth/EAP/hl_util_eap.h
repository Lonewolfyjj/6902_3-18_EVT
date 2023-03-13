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

typedef enum _eap_ctrl_cmd_enum_
{
    EAP_GET_SOFT_VERSION_CMD  = 0x01,
    EAP_GET_HARD_VERSION_CMD  = 0x02,
    EAP_GET_CONTECT_STATE_CMD = 0x03,
    EAP_GET_MAC_CMD           = 0x04,
    EAP_GET_SOC_CMD           = 0x05,
    EAP_GET_UV_CMD            = 0x06,
    EAP_GET_DEV_NUB_CMD       = 0x07,
    EAP_GET_GAIN_CMD          = 0x21,
    EAP_SET_GAIN_CMD          = 0x22,
    EAP_GET_DENOISE_CMD       = 0x23,
    EAP_SET_DENOISE_CMD       = 0x24,
    EAP_GET_LOWCUT_CMD        = 0x25,
    EAP_SET_LOWCUT_CMD        = 0x26,
    EAP_GET_MUTE_CMD          = 0x27,
    EAP_SET_MUTE_CMD          = 0x28,
    EAP_GET_SOUND_EFFECT_CMD  = 0x29,
    EAP_SET_SOUND_EFFECT_CMD  = 0x2A,
    EAP_GET_PHONE_OUT_CMD     = 0x2B,
    EAP_SET_PHONE_OUT_CMD     = 0x2C,
    EAP_UPDATE_START_CMD      = 0x41,
    EAP_UPDATE_PACKET_CMD     = 0x42,
    EAP_UPDATE_STOP_CMD       = 0x43,
    EAP_KEEP_ALIVE_CMD        = 0x61,
    EAP_REBOOT_CMD            = 0x62,
} HL_ENUM8(hl_eap_ctrl_cmd_e);

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
