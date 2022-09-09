/**
 * @file hl_util_fifo.h
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-09-09
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-09     <td>v1.0     <td>luzhanghao     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APPLICATIONS_UTILITY_INC_HL_UTIL_FIFO_H_
#define APPLICATIONS_UTILITY_INC_HL_UTIL_FIFO_H_

/* Includes ------------------------------------------------------------------*/
#include "rtdef.h"
#include <stdint.h>

/* typedef -------------------------------------------------------------------*/
/// fifo结构体
typedef struct _hl_fifo_t
{
    /// fifo 大小
    uint32_t fifo_size;
    /// fifo写指针
    uint32_t write_pointer;
    /// fifo读指针
    uint32_t read_pointer;
    /// fifo 环形缓冲区
    uint8_t  *buffer;
} hl_fifo_t;

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief fifo初始化
 * @param [in] p_fifo fifo指针 
 * @param [in] buff fifo数据缓冲区
 * @param [in] buf_size 数据缓冲区大小
 * @return int8_t 0：初始化成功 | 1:初始化失败
 * @date 2022-09-09
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-09      <td>luzhanghao     <td>新建
 * </table>
 */
int8_t hl_util_fifo_init(hl_fifo_t *p_fifo, uint8_t *buff, uint32_t buf_size);

/**
 * @brief fifo去初始化
 * @param [in] p_fifo fifo指针 
 * @return int8_t 0：去初始化成功 | 1:去初始化失败 
 * @date 2022-09-09
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-09      <td>luzhanghao     <td>新建
 * </table>
 */
int8_t hl_util_fifo_deinit(hl_fifo_t *p_fifo);

/**
 * @brief 
 * @param [in] p_fifo fifo指针 
 * @return uint32_t 
 * @date 2022-09-09
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-09      <td>luzhanghao     <td>新建
 * </table>
 */
uint32_t hl_util_fifo_data_size(hl_fifo_t *p_fifo);

/**
 * @brief 获取fifo剩余空间大小
 * @param [in] p_fifo fifo指针 
 * @return uint32_t 返回fifo剩余空间大小
 * @date 2022-09-09
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-09      <td>luzhanghao     <td>新建
 * </table>
 */
uint32_t hl_util_fifo_free_size(hl_fifo_t *p_fifo);

/**
 * @brief 读fifo
 * @param [in] p_fifo fifo指针 
 * @param [out] p_buf 数据存储目标地址
 * @param [in] len 要读取的数据长度
 * @return uint32_t 读取到的数据长度
 * @date 2022-09-09
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-09      <td>luzhanghao     <td>新建
 * </table>
 */
uint32_t hl_util_fifo_read(hl_fifo_t *p_fifo, uint8_t *p_buf, uint32_t len);

/**
 * @brief 写fifo
 * @param [in] p_fifo fifo指针 
 * @param [in] p_buf 要写入的数据起始地址
 * @param [in] len 要写入的数据长度
 * @return uint32_t 实际写入的数据长度
 * @date 2022-09-09
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-09      <td>luzhanghao     <td>新建
 * </table>
 */
uint32_t hl_util_fifo_write(hl_fifo_t *p_fifo, uint8_t *p_buf, uint32_t len);

/**
 * @brief 清除fifo
 * @param [in] p_fifo fifo指针 
 * @return int8_t 0：清除成功 | 1:清除失败
 * @date 2022-09-09
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-09      <td>luzhanghao     <td>新建
 * </table>
 */
int8_t hl_util_fifo_clear(hl_fifo_t *p_fifo);

#endif /* APPLICATIONS_UTILITY_INC_HL_UTIL_FIFO_H_ */

/*
 * EOF
 */
