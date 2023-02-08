/**
 * @file hl_iap2_func.h
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * @brief iap2功能函数具体实现
 * @version 0.1
 * @date 2022-08-31
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-08-31     <td>v1.0     <td>lisonglin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_UTIL_IAP2_FUNC_H__
#define __HL_UTIL_IAP2_FUNC_H__

/* Includes ------------------------------------------------------------------*/
#include "hl_util_iap2_type.h"
#include "hl_util_apple_packet.h"

/* typedef -------------------------------------------------------------------*/
typedef enum _iap2_challange_response_status_enum_
{
    EM_HL_CHALLENGE_RESP_STM_WRITE_DATA,
    EM_HL_CHALLENGE_RESP_STM_READ_DATA_LEN,
    EM_HL_CHALLENGE_RESP_STM_WRITE_CTRL,
    EM_HL_CHALLENGE_RESP_STM_READ_CTRL,
    EM_HL_CHALLENGE_RESP_STM_READ_RESP_LEN,
    EM_HL_CHALLENGE_RESP_STM_READ_RESP_DATA,
} HL_ENUM8(hl_iap2_challange_response_status_e);

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * hl_check_usb_insert
 * @brief  检测USB是否插入
 * @param  iap2             iap2结构体
 * @return int 成功 0 | 失败 非0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-06      <td>lisl     <td>新建
 * </table>
 */
int hl_check_usb_insert(hl_util_apple_p apple);

/**
 * hl_iap2_detect_send
 * @brief  发送detect检测包
 * @param  iap2             iap2结构体
 * @return int 成功 0 | 失败 非0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-06      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_detect_send(hl_util_apple_p apple);

/**
 * hl_iap2_detect_recv
 * @brief  接收detect检测包
 * @param  iap2             iap2结构体
 * @return int 成功 0 | 失败 非0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-06      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_detect_recv(hl_util_apple_p apple);

/**
 * hl_iap2_link_send_sync
 * @brief  发送sync同步信号
 * @param  iap2             iap2结构体
 * @return int 成功 0 | 失败 非0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-06      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_link_send_sync(hl_util_apple_p apple);

/**
 * hl_iap2_link_recv_sync_ack
 * @brief  接收sync同步信号、ack应答信号
 * @param  iap2             iap2结构体
 * @return int 成功 0 | 失败 非0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-06      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_link_recv_sync_ack(hl_util_apple_p apple);

/**
 * hl_iap2_link_send_ack
 * @brief  发送ack应答信号
 * @param  iap2             iap2结构体
 * @return int 成功 0 | 失败 非0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-06      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_link_send_ack(hl_util_apple_p apple);

/**
 * hl_iap2_identify_req_auth
 * @brief  接受证书请求
 * @param  iap2             iap2结构体
 * @return int 成功 0 | 失败 非0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-06      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_identify_req_auth(hl_util_apple_p apple);

/**
 * hl_iap2_identify_ack_auth
 * @brief  应答证书请求
 * @param  iap2             iap2结构体
 * @return int 成功 0 | 失败 非0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-06      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_identify_ack_auth(hl_util_apple_p apple);

/**
 * hl_iap2_identify_req_challenge
 * @brief  接受挑战请求
 * @param  iap2             iap2结构体
 * @return int 成功 0 | 失败 非0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-07      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_identify_req_challenge(hl_util_apple_p apple);

/**
 * hl_iap2_identify_ack_challenge
 * @brief  应答挑战请求
 * @param  iap2             iap2结构体
 * @return int 成功 0 | 失败 非0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-07      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_identify_ack_challenge(hl_util_apple_p apple);

/**
 * hl_iap2_identify_succedd
 * @brief  成功应答
 * @param  iap2             iap2结构体
 * @return int 成功 0 | 失败 非0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-07      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_identify_succedd(hl_util_apple_p apple);

/**
 * hl_iap2_identify_start_identification
 * @brief  开始识别信息，应答证书认证成功消息
 * @param  iap2             iap2结构体
 * @return int 成功 0 | 失败 非0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-07      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_identify_start_identification(hl_util_apple_p apple);

/**
 * hl_iap2_identify_identification_info
 * @brief  发送厂商识别信息
 * @param  iap2             iap2结构体
 * @return int 成功 0 | 失败 非0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-07      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_identify_identification_info(hl_util_apple_p apple);

/**
 * hl_iap2_identify_identification_accepted
 * @brief  识别信息接受
 * @param  iap2             iap2结构体
 * @return int 成功 0 | 失败 非0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-07      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_identify_identification_accepted(hl_util_apple_p apple);

/**
 * hl_iap2_powerupdate_send_power
 * @brief  发送START_POWER_UPDATES消息（0xAE00）
 * @param  iap2             iap2结构体
 * @return int 成功 0 | 失败 非0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-07      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_powerupdate_send_power(hl_util_apple_p apple);

/**
 * hl_iap2_powerupdate_recv_update
 * @brief  接受POWER_UPDATE消息（0xAE01）
 * @param  iap2             iap2结构体
 * @return int 成功 0 | 失败 非0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-07      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_powerupdate_recv_update(hl_util_apple_p apple);

/**
 * hl_iap2_powerupdate_send_power_source
 * @brief  发送POWER_SOURCE_UPDATES消息（0xAE03）
 * @param  iap2             iap2结构体
 * @return int 成功 0 | 失败 非0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-07      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_powerupdate_send_power_source(hl_util_apple_p apple);

int hl_iap2_eap_check_power_insert(hl_util_apple_p apple);

#endif
/*
 * EOF
 */
