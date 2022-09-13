/**
 * @file hl_util_fifo.c
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @brief fifo模块，提供fifo相关操作接口
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
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "rtthread.h"
#include "hl_util_fifo.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
#define HL_MAX(a, b)    ((a) > (b))?(a):(b)
#define HL_MIN(a, b)    ((a) < (b))?(a):(b)

/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
int8_t hl_util_fifo_init(hl_util_fifo_t *p_fifo, uint8_t *buff, uint32_t buf_size)
{
    if ((p_fifo == NULL) || (buf_size == 0)) {
        return -1;
    }

    p_fifo->fifo_size     = buf_size;
    p_fifo->read_pointer  = 0;
    p_fifo->write_pointer = 0;
    p_fifo->buffer        = buff;

    if (p_fifo->buffer == NULL) {
        return -2;
    }

    return 0;
}

int8_t hl_util_fifo_deinit(hl_util_fifo_t *p_fifo)
{
    if ((p_fifo == NULL) || (p_fifo->buffer == NULL)) {
        return -1;
    }

    p_fifo->fifo_size     = 0;
    p_fifo->read_pointer  = 0;
    p_fifo->write_pointer = 0;
    p_fifo->buffer        = NULL;

    return 0;
}

uint32_t hl_util_fifo_data_size(hl_util_fifo_t *p_fifo)
{
    if ((p_fifo == NULL) || (p_fifo->fifo_size == 0)) {
        return 0;
    }

    if (p_fifo->write_pointer >= p_fifo->read_pointer)  {
        return (p_fifo->write_pointer - p_fifo->read_pointer);
    } else {
        return (p_fifo->fifo_size - p_fifo->read_pointer + p_fifo->write_pointer);
    }
}

uint32_t hl_util_fifo_free_size(hl_util_fifo_t *p_fifo)
{
    return (p_fifo->fifo_size - hl_util_fifo_data_size(p_fifo) - 1);
}

uint32_t hl_util_fifo_read(hl_util_fifo_t *p_fifo, uint8_t *p_buf, uint32_t len)
{
   uint32_t size       = 0;
   uint32_t targe_size = 0;

   if ((p_fifo == NULL) || (p_fifo->fifo_size == 0)) {
       return 0;
   }

   if (p_fifo->read_pointer == p_fifo->write_pointer) {
        return 0;
   } else {
       targe_size = HL_MIN(hl_util_fifo_data_size(p_fifo), len);
       size = p_fifo->fifo_size - p_fifo->read_pointer;
       if ((p_fifo->write_pointer < p_fifo->read_pointer) && (targe_size > size))  {
           memcpy(p_buf, (uint8_t *)&p_fifo->buffer[p_fifo->read_pointer], size);
           memcpy((uint8_t *)&p_buf[p_fifo->fifo_size - p_fifo->read_pointer], (uint8_t *)&p_fifo->buffer[0], targe_size - size);
       } else {
           memcpy(p_buf, (uint8_t *)&p_fifo->buffer[p_fifo->read_pointer], targe_size);
       }

       p_fifo->read_pointer = (p_fifo->read_pointer + targe_size) % p_fifo->fifo_size;

       return targe_size;
   }
}

uint32_t hl_util_fifo_write(hl_util_fifo_t *p_fifo, uint8_t *p_buf, uint32_t len)
{
    uint32_t targe_size = 0;
    uint32_t size = 0;

    if ((p_fifo == NULL) || (p_fifo->fifo_size == 0)) {
        return 0;
    }

    targe_size = HL_MIN(hl_util_fifo_free_size(p_fifo), len);
    size       = p_fifo->fifo_size - p_fifo->write_pointer;
    if ((p_fifo->write_pointer >= p_fifo->read_pointer) && (targe_size > size)) {
        memcpy(&p_fifo->buffer[p_fifo->write_pointer], p_buf, size);
        memcpy(&p_fifo->buffer[0], (p_buf + size), (targe_size - size));
    } else {
        memcpy(&p_fifo->buffer[p_fifo->write_pointer], p_buf, targe_size);
    }

    p_fifo->write_pointer += targe_size;
    p_fifo->write_pointer %= p_fifo->fifo_size;

    return targe_size;
}

int8_t hl_util_fifo_clear(hl_util_fifo_t *p_fifo)
{
    if (p_fifo == NULL) {
        return -1;
    }

    p_fifo->read_pointer  = 0;
    p_fifo->write_pointer = 0;

    return 0;
}

/*
 * EOF
 */