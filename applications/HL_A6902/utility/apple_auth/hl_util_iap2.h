/**
 * @file hl_iap2.h
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * @brief IMF认证流程(嵌套状态机)
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
#ifndef _HL_IAP2_H
#define _HL_IAP2_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>
#include "hl_util_iap2_packet.h"
#include "hl_util_iap2_func.h"

/* typedef -------------------------------------------------------------------*/

typedef struct _iap2_func_handle_
{
    /// 延时函数
    void (*delay_usec_func)(uint16_t usec);
    /// USB读数据函数
    int (*iap2_usb_read)(uint8_t* read_data_addr, uint16_t read_data_len, uint16_t timeout);
    /// USB写数据函数
    int (*iap2_usb_write)(uint8_t* write_data_addr, uint16_t write_data_len);
    /// I2C读数据函数
    int (*iap2_iic_read)(uint8_t reg_addr, uint8_t* read_data_addr, uint16_t read_data_len, uint16_t timeout);
    /// I2C写数据函数
    int (*iap2_iic_write)(uint8_t reg_addr, uint8_t* write_data_addr, uint16_t write_data_len);
    /// 打印函数
    void (*iap2_printf)(const char *fmt, ...);
} func_handle;

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * hl_iap2_process_main_oneshot
 * @brief 循环执行嵌套状态机每个状态，完成苹果认证
 * @param [in] iap2 iap2句柄
 * @return int 成功 0 | 失败 非0
 * @date 2022-09-01
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lisonglin     <td>新建
 * </table>
 */
int hl_iap2_process_main_oneshot(st_iap2_protocol_p iap2);

/**
 * hl_iap2_protocol_init
 * @brief IAP2认证
 * @param [in] handle 函数指针句柄
 * @return int 成功 0 | 失败 非0
 * @date 2022-09-01
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lisonglin     <td>新建
 * </table>
 */
int hl_iap2_protocol_init(st_iap2_protocol_p iap2, func_handle handle);

/**
 * hl_iap2_protocol_deinit
 * @brief IAP2认证去初始化
 * @return int 成功 0 | 失败 非0
 * @date 2022-09-01
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lisonglin     <td>新建
 * </table>
 */
int hl_iap2_protocol_deinit(st_iap2_protocol_p iap2);

#endif
/*
 * EOF
 */
