/**
 * @file hl_util_apple_type.h
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * @brief 该文件声明苹果认证及外部配件通信所需的模块结构体
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
#ifndef __HL_UTIL_APPLE_TYPE_H__
#define __HL_UTIL_APPLE_TYPE_H__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>
#include "hl_util_apple_packet.h"
#include "hl_util_iap2_type.h"
#include "hl_util_eap_type.h"
#include "hl_util_fifo.h"

/* typedef -------------------------------------------------------------------*/

typedef enum _apple_oneshot_result_e_
{
    /// 返回认证结果：成功
    EM_HL_IAP2_SUCCEED = 0x00,
    /// 返回认证结果：失败
    EM_HL_IAP2_FAILED,
    /// 返回EASession开始
    EM_HL_EAP_START,
    /// 返回EASession结束
    EM_HL_EAP_STOP,
} apple_oneshot_result_e;

typedef struct _iap2_packet_header_arg_
{
    /// seq值
    uint8_t seq_num;
    /// ack值
    uint8_t ack_num;
    /// SessionId值
    uint8_t session_id;
} st_packet_header_arg;

typedef struct _hl_util_apple_t_
{
    /// fifo缓冲区
    uint8_t* fifo_buf;
    /// fifo结构体
    hl_util_fifo_t fifo;
    /// iAP认证句柄
    hl_apple_iap2_t iap2;
    /// EAP通信句柄
    hl_apple_eap_t eap;

    volatile st_packet_header_arg packet_arg;

    //  * @brief 延时函数
    //  * @param [in] usec 延时时间（微秒）
    void (*delay_usec)(uint16_t usec);

    //  * @brief USB读数据操作
    //  * @param [in] read_data_addr 读数据缓冲区
    //  * @param [in] read_data_len 读数据长度
    //  * @param [in] timeout 超时时间
    //  * @return int 成功 读取数据长度 | 失败 0
    int (*usb_read)(uint8_t* read_data_addr, uint16_t read_data_len, uint16_t timeout);

    //  * @brief USB写数据操作
    //  * @param [in] write_data_addr 写数据内容
    //  * @param [in] write_data_len 写数据长度
    //  * @return int 成功 0
    int (*usb_write)(uint8_t* write_data_addr, uint16_t write_data_len);

    //  * @brief IIC读数据操作
    //  * @param [in] reg_addr 读取寄存器
    //  * @param [in] read_data_addr 读数据缓冲区
    //  * @param [in] read_data_len 读数据长度
    //  * @param [in] timeout 超时时间
    //  * @return int 成功 读数据长度 | 失败 0
    int (*iic_read)(uint8_t reg_addr, uint8_t* read_data_addr, uint16_t read_data_len, uint16_t timeout);

    //  * @brief IIC写数据操作
    //  * @param [in] reg_addr 写数据寄存器
    //  * @param [in] write_data_addr 写数据内容
    //  * @param [in] write_data_len 写数据长度
    //  * @return int 成功 0 | 失败 1
    int (*iic_write)(uint8_t reg_addr, uint8_t* write_data_addr, uint16_t write_data_len);

    // 打印函数
    void (*log)(const char* fmt, ...);
} hl_util_apple_t, *hl_util_apple_p;

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#endif
/*
 * EOF
 */
