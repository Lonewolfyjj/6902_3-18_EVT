/**
 * @file hl_mod_telink.h
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * @brief 负责与Telink无线芯片命令交互
 * @version 1.0
 * @date 2022-09-09
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-09     <td>v1.0     <td>lisonglin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_MOD_TELINK_H__
#define __HL_MOD_TELINK_H__

/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
#include <stdint.h>
#include "hl_util_hup.h"
#include "hl_util_fifo.h"

/* typedef -------------------------------------------------------------------*/
typedef struct
{
    rt_bool_t thread_flag;
    /// APP层消息队列
    rt_mq_t* app_msq;
    /// 串口设备
    rt_device_t serial;
    /// hup结构体
    hl_util_hup_t hup;
    /// fifo结构体
    hl_util_fifo_t fifo;
} hl_mod_telink_t;

/* define --------------------------------------------------------------------*/
#define TELINK_THREAD_STACK_SIZE 1024
#define TELINK_THREAD_PRIORITY 7
#define TELINK_THREAD_TIMESLICE 10

#define TELINK_UART_DEV_NAME "uart2"
#define TELINK_UART_BUF_SIZE 256
#define TELINK_FIFO_BUF_SIZE 256
#define TELINK_HUP_BUF_SIZE 256

#define TELINK_CMD_PAIR 0x01
#define TELINK_CMD_GET_PAIR_INFO 0x02
#define TELINK_CMD_WIRELESS_PAIR_ALL 0x03
#define TELINK_CMD_WIRELESS_PAIR_LEFT 0x03
#define TELINK_CMD_WIRELESS_PAIR_RIGHT 0x04
#define TELINK_CMD_GET_RSSI 0x08
#define TELINK_CMD_SWITCH_ANTENNA 0x11
#define TELINK_CMD_BY_PASS 0x21
#define TELINK_CMD_SET_RF_POWER 0x31
#define TELINK_CMD_SET_PA_MODE 0x41

#define TELINK_PAIR_START 0x01
#define TELINK_PAIR_FAILED 0x02
#define TELINK_PAIR_SUCCEDD_L 0x03
#define TELINK_PAIR_SUCCEDD_R 0x04

/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
uint8_t hl_mod_telink_init(rt_mq_t* input_msq);

uint8_t hl_mod_telink_deinit(void);

uint8_t hl_mod_telink_start(void);

uint8_t hl_mod_telink_stop(void);

uint8_t hl_mod_telink_ioctl(uint8_t cmd, uint8_t* data_addr, uint16_t data_len);

#endif

/*
 * EOF
 */
