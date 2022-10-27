/**
 * @file hl_drv_audio.h
 * @author lixiang (rd37@hollyland-tech.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-08
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-13     <td>v1.0     <td>lixiang     <td>内容
 * </table>
 * 
 */ 


/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __HL_DRV_AUDIO_H__
#define __HL_DRV_AUDIO_H__

/* Includes ------------------------------------------------------------------*/
#include "hal_audio.h"
#include "rk_audio.h"
#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN
#include "pcm/pcm_types.h"
#endif

/* typedef -------------------------------------------------------------------*/
typedef void(*drv_audio_stream_rcv_cb_t)(void* dst, const void* src, rt_ubase_t count);

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
rt_err_t hl_drv_audio_register_stream(drv_audio_stream_rcv_cb_t audio_cb_func);


#endif /* __RK_AUDIO_H__ */
/*
 * EOF
 */
