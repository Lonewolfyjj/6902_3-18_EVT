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
#ifndef _HL_IAP2_FUNC_H
#define _HL_IAP2_FUNC_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>
#include "hl_util_iap2_packet.h"

/* typedef -------------------------------------------------------------------*/
typedef enum _iap2_challange_response_status_enum_
{
    EM_HL_CHALLENGE_RESP_STM_WRITE_DATA,
    EM_HL_CHALLENGE_RESP_STM_READ_DATA_LEN,
    EM_HL_CHALLENGE_RESP_STM_WRITE_CTRL,
    EM_HL_CHALLENGE_RESP_STM_READ_CTRL,
    EM_HL_CHALLENGE_RESP_STM_READ_RESP_LEN,
    EM_HL_CHALLENGE_RESP_STM_READ_RESP_DATA,
} em_iap2_challange_response_status_t;

typedef struct _iap2_protocol_
{
    volatile uint8_t* send_buffer;
    volatile uint8_t* recv_buffer;

    st_packet_header_arg packet_arg;
    uint8_t              challenge_req_len;

    em_iap2_protocol_status_t     main_status;
    em_iap2_detect_status_t       detect_status;
    em_iap2_link_status_t         link_status;
    em_iap2_identify_status_t     identify_status;
    em_iap2_power_update_status_t powerupdate_status;

    void (*delay_usec_func)(uint16_t usec);
    int (*iap2_usb_read)(uint8_t* read_data_addr, uint16_t read_data_len, uint16_t timeout);
    int (*iap2_usb_write)(uint8_t* write_data_addr, uint16_t write_data_len);
    int (*iap2_iic_read)(uint8_t reg_addr, uint8_t* read_data_addr, uint16_t read_data_len, uint16_t timeout);
    int (*iap2_iic_write)(uint8_t reg_addr, uint8_t* write_data_addr, uint16_t write_data_len);
    /// 打印函数
    void (*iap2_printf)(const char *fmt, ...);
} st_iap2_protocol_t, *st_iap2_protocol_p;

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
int hl_check_usb_insert(st_iap2_protocol_p iap2);

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
int hl_iap2_detect_send(st_iap2_protocol_p iap2);

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
int hl_iap2_detect_recv(st_iap2_protocol_p iap2);

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
int hl_iap2_link_send_sync(st_iap2_protocol_p iap2);

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
int hl_iap2_link_recv_sync_ack(st_iap2_protocol_p iap2);

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
int hl_iap2_link_send_ack(st_iap2_protocol_p iap2);

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
int hl_iap2_identify_req_auth(st_iap2_protocol_p iap2);

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
int hl_iap2_identify_ack_auth(st_iap2_protocol_p iap2);

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
int hl_iap2_identify_req_challenge(st_iap2_protocol_p iap2);

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
int hl_iap2_identify_ack_challenge(st_iap2_protocol_p iap2);

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
int hl_iap2_identify_succedd(st_iap2_protocol_p iap2);

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
int hl_iap2_identify_start_identification(st_iap2_protocol_p iap2);

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
int hl_iap2_identify_identification_info(st_iap2_protocol_p iap2);

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
int hl_iap2_identify_identification_accepted(st_iap2_protocol_p iap2);

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
int hl_iap2_powerupdate_send_power(st_iap2_protocol_p iap2);

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
int hl_iap2_powerupdate_recv_update(st_iap2_protocol_p iap2);

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
int hl_iap2_powerupdate_send_power_source(st_iap2_protocol_p iap2);

int hl_iap2_eap_handle_msg(st_iap2_protocol_p iap2);

int hl_iap2_eap_check_power_insert(st_iap2_protocol_p iap2);

#endif
/*
 * EOF
 */
