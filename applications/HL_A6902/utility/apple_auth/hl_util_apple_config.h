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
#include "hl_config.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/

/// (PowerUpdate)可设置参数<MaxomumCurrentDrawnFromAccessory>
#define POWERUPDATE_ID0 0
/// (PowerUpdate)可设置参数<DeviceBatteryWillChargeIfPowerIsPresent>
#define POWERUPDATE_ID1 0
/// (PowerUpdate)可设置参数<AccessoryPowerMode>
#define POWERUPDATE_ID2 0
/// (PowerUpdate)可设置参数<IsExternalChargerConnected>
#define POWERUPDATE_ID4 0
/// (PowerUpdate)可设置参数<BatteryChargingState>
#define POWERUPDATE_ID5 0
/// (PowerUpdate)可设置参数<BatteryChargeLevel>
#define POWERUPDATE_ID6 0

/// 发送PowerUpdate消息
#define POWERUPDATE_OPEN 0
/// 发送PowerSourceUpdate消息
#define POWERSOURCEUPDATE_OPEN 0

/// 无动作且无法查找到配件APP
#define APP_NO_ACTION 0
/// 未安装则提醒安装配件APP
#define APP_OPTIONAL_ACTION 1
/// 无动作但可在设置中查找配件APP进行安装
#define APP_NO_ALERT 2
/// 认证成功后匹配APP的行为
#define APP_MATCH_PROMPT (APP_OPTIONAL_ACTION)

/// 苹果功能USB读写操作打印
#define LOG_APPLE_USB_OPEN 0
/// 苹果功能I2C读写操作打印
#define LOG_APPLE_I2C_OPEN 0

// 内存资源大小
#define SEND_BUFFER_SIZE 1024
#define RECV_BUFFER_SIZE 1024
#define APPLE_FIFO_BUF_SIZE 1024
/// 超时时间
#define TIMEOUT_US 1000

// 厂商识别信息
#define IAP2_DEVICE_NAME "Wireless Microphone RX"
#define IAP2_MODEID "Lark Max"
#define IAP2_MANUFATORY "Shenzhen Hollyland technology Co.,LTD"
#define IAP2_SERIAL_NAME "0123456789AB"
#define IAP2_FIRMWAREVERSION A6902_VERSION
#define IAP2_HARDWAREVERSION "v1.0.0.38"
#define IAP2_PUID "b0a0340195cc41ee"
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
