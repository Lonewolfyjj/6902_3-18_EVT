/**
 * @file hl_util_iap2_type.h
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * @brief 
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
#ifndef __HL_UTIL_IAP2_TYPE_H__
#define __HL_UTIL_IAP2_TYPE_H__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>
#include "hl_util_general_type.h"

/* typedef -------------------------------------------------------------------*/

typedef enum _iap2_protocol_status_enum_
{
    EM_HL_IAP2_STM_MAIN_IDLE,
    EM_HL_IAP2_STM_MAIN_LINK,
    EM_HL_IAP2_STM_MAIN_DETECT,
    EM_HL_IAP2_STM_MAIN_IDENTIFY,
    EM_HL_IAP2_STM_MAIN_POWER_UPDATE,
    EM_HL_IAP2_STM_MAIN_ACK_CTRL,
    EM_HL_IAP2_STM_MAIN_SUCCEED,
    EM_HL_IAP2_STM_MAIN_FAILED,
} HL_ENUM8(hl_iap2_protocol_status_e);

typedef enum _iap2_detect_status_enum_
{
    EM_HL_IAP2_STM_DETECT_SEND,
    EM_HL_IAP2_STM_DETECT_RECV,
} HL_ENUM8(hl_iap2_detect_status_e);

typedef enum _iap2_link_status_enum_
{
    EM_HL_IAP2_STM_LINK_SEND_SYN,
    EM_HL_IAP2_STM_LINK_RECV_SYN_ACK,
    EM_HL_IAP2_STM_LINK_SEND_ACK,
} HL_ENUM8(hl_iap2_link_status_e);

typedef enum _iap2_identify_status_enum_
{
    EM_HL_IAP2_STM_IDENTIFY_REQ_AUTH,
    EM_HL_IAP2_STM_IDENTIFY_ACK_AUTH,
    EM_HL_IAP2_STM_IDENTIFY_REQ_CHALLENGE,
    EM_HL_IAP2_STM_IDENTIFY_ACK_CHALLENGE,
    EM_HL_IAP2_STM_IDENTIFY_SUCCEDD,
    EM_HL_IAP2_STM_IDENTIFY_START_IDENTIFICATION,
    EM_HL_IAP2_STM_IDENTIFY_IDENTIFICATION_INFO,
    EM_HL_IAP2_STM_IDENTIFY_IDENTIFICATION_ACCEPTED,
} HL_ENUM8(hl_iap2_identify_status_e);

typedef enum _iap2_power_update_status_enum_
{
    EM_HL_IAP2_STM_POWERUPDATE_SEND_POWER,
    EM_HL_IAP2_STM_POWERUPDATE_RECV_POWER_UPDATE,
    EM_HL_IAP2_STM_POWERUPDATE_SEND_POWER_SOURCE,
} HL_ENUM8(hl_iap2_power_update_status_e);

typedef struct _hl_apple_iap2_t_
{
    volatile uint8_t* send_buffer;
    volatile uint8_t* recv_buffer;

    /// 设备SN码
    char dev_sn[128];

    // 挑战请求的长度
    uint8_t* challenge_resp_data;
    uint16_t challenge_req_len;
    uint16_t challenge_resp_len;

    // 重试次数
    uint8_t retry_time;

    volatile hl_iap2_protocol_status_e     main_status;
    volatile hl_iap2_detect_status_e       detect_status;
    volatile hl_iap2_link_status_e         link_status;
    volatile hl_iap2_identify_status_e     identify_status;
    volatile hl_iap2_power_update_status_e powerupdate_status;
} hl_apple_iap2_t, *hl_apple_iap2_p;

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#endif
/*
 * EOF
 */
