/**
 * @file hl_util_apple_packet.h
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * @brief 
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
#ifndef __HL_UTIL_APPLE_PACKET_H__
#define __HL_UTIL_APPLE_PACKET_H__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "hl_util_apple_type.h"
#include "hl_util_apple_config.h"
#include "hl_util_general_type.h"

/* typedef -------------------------------------------------------------------*/

typedef struct _iap2_packet_header_
{
    /// 协议包起始头部
    uint16_t StartOfPacket;
    /// 协议包长度
    uint16_t PacketLength;
    /// 协议包控制命令
    uint8_t ControlByte;
    /// 协议包seq值
    uint8_t PacketSeqNum;
    /// 协议包ack值
    uint8_t PacketAckNum;
    /// 协议包SessionId值
    uint8_t SessionIdentif;
    /// 协议包头部异或校验
    uint8_t HeaderCksum;
} __attribute__((packed, aligned(1))) st_iap2_packet_header_t;

typedef struct _iap2_sync_payload_
{
    ///
    uint8_t LinkVersion;
    ///
    uint8_t MaxNumOfOutstandingPacket;
    ///
    uint16_t MaxReceivedPacketLen;
    ///
    uint16_t RetransmitTimeout;
    ///
    uint16_t CumulativeAckTimeout;
    ///
    uint8_t MaxNumOfRetransmit;
    ///
    uint8_t MaxCumulativeAckNum;
    ///
    uint8_t SessionId1;
    ///
    uint8_t SessionType1;
    ///
    uint8_t SessionVer1;
    ///
    uint8_t SessionId2;
    ///
    uint8_t SessionType2;
    ///
    uint8_t SessionVer2;
    ///
    uint8_t PayloadCksum;
} __attribute__((packed, aligned(1))) st_iap2_sync_payload_t;

typedef struct _iap2_ctrl_param_
{
    ///
    uint16_t Len;
    ///
    uint16_t Id;
    ///
    uint8_t* Data;
} __attribute__((packed, aligned(1))) st_iap2_ctrl_param_t;

typedef struct _iap2_ctrl_payload_
{
    ///
    uint16_t StartOfMessage;
    ///
    uint16_t MessageLen;
    ///
    uint16_t MessageId;
    ///
    uint16_t ParamLen;
    ///
    uint16_t ParamId;
    ///
    uint8_t* ParamData;
    ///
    uint8_t PayloadCksum;
} __attribute__((packed, aligned(1))) st_iap2_ctrl_payload_t;

typedef struct _iap2_ea_payload_
{
    ///
    uint16_t EASessionIdentifier;
    ///
    uint8_t* EASessionData;
} __attribute__((packed, aligned(1))) st_iap2_ea_payload_t;

typedef struct _iap2_sync_packet_
{
    ///
    st_iap2_packet_header_t packet_header;
    ///
    st_iap2_sync_payload_t sync_payload;
} __attribute__((packed, aligned(1))) st_iap2_sync_packet_t;

typedef struct _iap2_ctrl_packet_
{
    ///
    st_iap2_packet_header_t packet_header;
    ///
    st_iap2_ctrl_payload_t ctrl_payload;
} __attribute__((packed, aligned(1))) st_iap2_ctrl_packet_t;

typedef struct _iap2_ea_packet_
{
    ///
    st_iap2_packet_header_t packet_header;
    ///
    st_iap2_ea_payload_t ea_payload;
} __attribute__((packed, aligned(1))) st_iap2_ea_packet_t;

/* define --------------------------------------------------------------------*/

#define EXCHANGE_HIGH_LOW_BYTE(x) ((uint16_t)((((x) >> 8) & 0xFF) | (((x)&0xFF) << 8)))

#define LINK_CONTROL_SYN (1UL << (7))
#define LINK_CONTROL_ACK (1UL << (6))
#define LINK_CONTROL_EAK (1UL << (5))
#define LINK_CONTROL_RST (1UL << (4))
#define LINK_CONTROL_SLP (1UL << (3))

#define MESSAGE_ID_START_EAP 0xEA00
#define MESSAGE_ID_STOP_EAP 0xEA01
#define MESSAGE_ID_START_POWER_UPDATES 0xAE00
#define MESSAGE_ID_POWER_UPDATE 0xAE01
#define MESSAGE_ID_POWEERSOURCE_UPDATE 0xAE03
#define SESSION_ID_START_USB_DEV_MODE_AUDIO 0xDA00
#define SESSION_ID_USB_DEV_MODE_AUDIO_INF 0xDA01
#define SESSION_ID_STOP_USB_DEV_MODE_AUDIO 0XDA02
#define SESSION_ID_REQUESTAUTHCERTIFICATE 0xAA00
#define SESSION_ID_AUTHENTICATION_CERTIFICATE 0xAA01
#define SESSION_ID_REQUEST_AUTH_CHALLENG_RESP 0xAA02
#define SESSION_ID_AUTH_RESPONSE 0xAA03
#define SESSION_ID_AUTH_FAIL 0xAA04
#define SESSION_ID_AUTH_SUCCEDD 0xAA05
#define SESSION_ID_ACCESSORY_AUTH_SERIAL_NUM 0xAA06
#define SESSION_ID_START_IDENTIFICATION 0x1D00
#define SESSION_ID_IDENTIFICATION_INFO 0x1D01
#define SESSION_ID_IDEN_ACCEPTED 0x1D02
#define SESSION_ID_IDEN_REJECTED 0x1D03
#define SESSION_ID_CANCELL_IDEN 0x1D05
#define SESSION_ID_IDEN_UPDATE 0x1D06
#define CP_DEVICE_VERSION 0x00
#define CP_AUTHENTICATION_RIVISON 0x01
#define CP_AUTHENTICATION_MAJOR_VER 0x02
#define CP_AUTHENTICATION_MIN_VER 0x03
#define CP_DEVICE_ID 0x04
#define CP_ERROR_CODE 0x05
#define CP_AUTHENTICATION_CONTROL_STATUES 0x10
#define CP_CHALLENGE_RESPONSE_DATA_LEN 0x11
#define CP_CHALLENGE_RESPONSE_DATA 0x12
#define CP_CHALLENGE_DATA_LEN 0x20
#define CP_CHALLENGE_DATA 0x21
#define CP_ACCESSORY_CERTIFICATION_DATA_LEN 0x30
#define CP_ACCESSORY_CERTIFICATION_DATA_1 0x31
#define CP_ACCESSORY_CERTIFICATION_DATA_2 0x32
#define CP_ACCESSORY_CERTIFICATION_DATA_3 0x33
#define CP_ACCESSORY_CERTIFICATION_DATA_4 0x34
#define CP_ACCESSORY_CERTIFICATION_DATA_5 0x35
#define CP_SELF_TEST_STATUS 0x40
#define CP_DEV_CERTIFICATION_SERIAL_NUM 0x4E
#define CP_SLEEP 0x60

#define SESSION_ID_CTRL 0x01
#define SESSION_ID_EA 0x02

#define LINK_SESSION1_ID 0x01
#define LINK_SESSION1_TYPE 0x00
#define LINK_SESSION1_VER 0x01
#define LINK_SESSION2_ID 0x02
#define LINK_SESSION2_TYPE 0x02
#define LINK_SESSION2_VER 0x01

#define DETECT_FRAME_SIZE 6
#define PACKET_HEADER_SIZE 8
#define PACKET_CTRL_HEADER_SIZE 6
#define PACKET_PARAM_HEADER_SIZE 4
#define IDENTIFY_FRAME_SIZE 16
#define POWERUPDATE_FRAME_SIZE 21

/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** 
 * hl_checksum_calculation
 * @brief  计算负载校验
 * @param  buffer           校验内容地址
 * @param  start            开始位置
 * @param  len              校验内容长度
 * @return uint8_t	校验结果
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-06      <td>lisl     <td>新建
 * </table>
 */
uint8_t hl_checksum_calculation(uint8_t* buffer, uint16_t start, uint16_t len);

/**
 * hl_iap2_detect_packet_encode
 * @brief  detect检测包打包
 * @param  write_data_addr  写入数据地址
 * @return int 成功 写入数据长度 | 失败 <0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-06      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_detect_packet_encode(uint8_t* write_data_addr);

/**
 * hl_iap2_detect_packet_decode
 * @brief  detect检测包解包
 * @param  read_data_addr   读出数据地址
 * @return int 成功 0 | 失败 非0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-06      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_detect_packet_decode(uint8_t* read_data_addr);

/**
 * hl_apple_get_packet_len
 * @brief 获取消息包整体长度
 * @param [in] packet_header 包头指针
 * @return uint16_t 成功 消息包整体长度 | 失败 <0
 * @date 2023-02-13
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-02-13      <td>lisonglin     <td>新建
 * </table>
 */
uint16_t hl_apple_get_packet_len(st_iap2_packet_header_t* packet_header);

/**
 * hl_iap2_packet_header_encode
 * @brief  填写协议包头部内容
 * @param  packet_header    协议包头部地址
 * @param  packet_len       协议包整体长度
 * @param  ctrl_byte        包头部分控制位
 * @param  header_arg       包头部分seq值、ack值、session值
 * @return int 成功 协议包头部长度 | 失败 <0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-06      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_packet_header_encode(st_iap2_packet_header_t* packet_header, uint16_t packet_len, uint8_t ctrl_byte,
                                 st_packet_header_arg header_arg);

/**
 * hl_iap2_packet_header_decode
 * @brief  解析协议包头部内容
 * @param  packet_header    协议包头部地址
 * @param  packet_len       传出协议包整体长度
 * @param  ctrl_byte        包头部分控制位
 * @param  header_arg       包头部分seq值、ack值、session值
 * @return int 成功 0 | 失败 <0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-06      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_packet_header_decode(uint8_t* packet_header, uint16_t* packet_len, uint8_t ctrl_byte,
                                 st_packet_header_arg* header_arg);

/**
 * hl_iap2_packet_sync_payload_encode
 * @brief  填写协议包sync负载部分内容
 * @param  packet_payload   协议包负载部分地址
 * @param  payload_len      负载内容长度
 * @return int 成功 协议包负载内容长度 | 失败 <0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-06      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_packet_sync_payload_encode(st_iap2_sync_payload_t* packet_payload, uint8_t payload_len);

/**
 * hl_iap2_ctrl_packet_get_message_id
 * @brief  获取message id
 * @param  data_addr        协议包数据地址
 * @return uint16_t 成功 message id | 失败 <0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-06      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_ctrl_packet_get_message_id(uint8_t* data_addr);

/**
 * hl_iap2_ctrl_packet_get_param_len
 * @brief  获取param长度
 * @param  data_addr        协议包数据地址
 * @return int 成功 param长度 | 失败 <0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-07      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_ctrl_packet_get_param_len(uint8_t* data_addr);

/**
 * hl_iap2_ctrl_packet_get_param_id
 * @brief  获取param编号id
 * @param  data_addr        协议包数据地址
 * @return int 成功 param编号id | 失败 <0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-07      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_ctrl_packet_get_param_id(uint8_t* data_addr);

/**
 * hl_iap2_ctrl_payload_encode
 * @brief  填充control session帧头部内容
 * @param  packet_payload   协议包负载部分地址
 * @param  message_len      消息长度
 * @param  message_id       消息编号
 * @return int 成功 control session帧头部长度 | 失败 <0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-06      <td>lisl     <td>新建
 * </table>
 */
int hl_iap2_ctrl_payload_encode(st_iap2_ctrl_payload_t* packet_payload, uint16_t message_len, uint16_t message_id);

/**
 * hl_iap2_ctrl_add_param
 * @brief  添加control session帧中param内容
 * @param  write_addr       写入数据地址
 * @param  param_len        param长度
 * @param  param_id         param编号
 * @param  write_data       写入数据内容
 * @return uint16_t 成功 param内容长度 | 失败 <0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-07      <td>lisl     <td>新建
 * </table>
 */
uint16_t hl_iap2_ctrl_add_param(uint8_t* write_addr, uint16_t param_len, uint16_t param_id, uint8_t* write_data);

/**
 * hl_eap_payload_decode
 * @brief 解析EAP消息包并读出数据内容
 * @param [in] packet_payload EAP消息包
 * @param [in] easession_id EASessionId
 * @param [in] data_addr 数据存放缓冲区
 * @return uint16_t 成功 读取数据长度 | 失败 <0
 * @date 2023-02-08
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-02-08      <td>lisonglin     <td>新建
 * </table>
 */
uint16_t hl_eap_payload_decode(st_iap2_ea_packet_t* ea_packet, uint16_t easession_id, uint8_t* data_addr);

#endif
/*
 * EOF
 */
