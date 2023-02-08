/**
 * @file hl_util_iap2_config.h
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * @brief iap2苹果相关功能配置
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
#ifndef __HL_UTIL_APPLE_CONFIG_H__
#define __HL_UTIL_APPLE_CONFIG_H__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
#define POWERSOURCEUPDATE_OPEN 1

// 内存资源大小
#define SEND_BUFFER_SIZE 1024
#define RECV_BUFFER_SIZE 1024
#define APPLE_FIFO_BUF_SIZE 1024
#define TIMEOUT_US 1000

// 厂商识别信息
#define IAP2_DEVICE_NAME "LARK 150 Pro"
#define IAP2_MODEID "A6902"
#define IAP2_MANUFATORY "Hollyland"
#define IAP2_SERIAL_NAME "0123456789AB"
#define IAP2_FIRMWAREVERSION "v1.0.0.1"
#define IAP2_HARDWAREVERSION "v2.0.0.1"
#define IAP2_PUID "0babaf959f694c97"
#define IAP2_UHOST_COMPONENT "USBHostTransportComponent"
#define IAP2_HID_COMPONENT "HIDComponent"
#define IAP2_EXTERNAL_ACCESSORY_PROTOCOL_NAME "com.hollyland.protocol"
#define IAP2_APP_MATCH_TEAM_ID "XDB3HGKJ3W"

/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#endif
/*
 * EOF
 */
