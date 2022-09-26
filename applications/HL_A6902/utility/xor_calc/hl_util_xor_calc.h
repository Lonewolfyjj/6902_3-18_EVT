/**
 * @file hl_util_xor_calc.h
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-09
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-08-09     <td>v1.0     <td>lisonglin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_UTIL_XOR_CALC_H__
#define __HL_UTIL_XOR_CALC_H__
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/

/**
 * hl_util_xor_calc_xor8
 * @brief 8位异或校验计算
 * @param [in] data_buf 数据缓冲区地址
 * @param [in] data_len 数据长度
 * @return uint8_t 异或计算结果
 * @date 2022-08-09
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-09      <td>lisonglin     <td>新建
 * </table>
 */
uint8_t hl_util_xor_calc_xor8(uint8_t* data_buf, uint16_t data_len);

/**
 * hl_util_xor_calc_xor16
 * @brief 16位异或校验计算
 * @param [in] data_buf 数据缓冲区地址
 * @param [in] data_len 数据长度
 * @return uint16_t 异或计算结果
 * @date 2022-08-09
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-09      <td>lisonglin     <td>新建
 * </table>
 */
uint16_t hl_util_xor_calc_xor16(uint8_t* data_buf, uint16_t data_len);

/* Exported functions --------------------------------------------------------*/
/*
 * EOF
 */

#endif