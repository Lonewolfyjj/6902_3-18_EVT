/**
 * @file hl_util_apple.h
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
#ifndef __HL_UTIL_APPLE_H__
#define __HL_UTIL_APPLE_H__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>
#include "hl_util_iap2.h"
#include "hl_util_eap.h"
#include "hl_util_fifo.h"
#include "hl_util_hup.h"

/* typedef -------------------------------------------------------------------*/

typedef enum _hl_apple_action_e_
{
    EM_HL_IAP2,
    EM_HL_EAP,
} HL_ENUM8(hl_apple_action_e);

typedef struct _apple_func_handle_
{
    /// 延时函数
    void (*delay_usec)(uint16_t usec);
    /// USB读数据函数
    int (*usb_read)(uint8_t* read_data_addr, uint16_t read_data_len, uint16_t timeout);
    /// USB写数据函数
    int (*usb_write)(uint8_t* write_data_addr, uint16_t write_data_len);
    /// I2C读数据函数
    int (*iic_read)(uint8_t reg_addr, uint8_t* read_data_addr, uint16_t read_data_len, uint16_t timeout);
    /// I2C写数据函数
    int (*iic_write)(uint8_t reg_addr, uint8_t* write_data_addr, uint16_t write_data_len);
    /// 打印函数
    void (*log)(const char* fmt, ...);
} apple_func_handle;

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

int hl_util_apple_init(hl_util_apple_p apple, apple_func_handle handle);

int hl_util_apple_deinit(hl_util_apple_p apple);

int hl_util_apple_oneshot(hl_util_apple_p apple, hl_apple_action_e action);

#endif
/*
 * EOF
 */
