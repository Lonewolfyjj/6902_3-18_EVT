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

#include "hl_config.h"
#include "hl_util_general_type.h"


/* typedef -------------------------------------------------------------------*/
typedef void(*drv_audio_stream_rcv_cb_t)(void* dst, const void* src, rt_ubase_t count);

typedef enum _hl_drv_audio_e
{
#if HL_IS_TX_DEVICE()
    /// 外置麦左
    HL_DRV_AUDIO_CH_MIC_L = 0x10,
    /// 外置麦右
    HL_DRV_AUDIO_CH_MIC_R,  
#else    
    /// 相机口左
    HL_DRV_AUDIO_CH1_CAM_L = 0x10,
    /// 相机口右
    HL_DRV_AUDIO_CH1_CAM_R,  
    /// 监听口左
    HL_DRV_AUDIO_CH2_HP_L,
    /// 监听口右
    HL_DRV_AUDIO_CH2_HP_R, 
#endif
} HL_ENUM8(hl_drv_audio_e);

typedef enum _hl_codec_ch_e
{
    /// codec gain volume ch
    HL_CODEC_CH_VOLUME = 0x00,
    /// codec gain pga ch
    HL_CODEC_CH_PGA,  
} HL_ENUM8(hl_codec_ch_e);

typedef enum _hl_codec_sound_ch_e
{
    /// codec gain volume ch
    HL_CODEC_SOUND_CH_ALL = 0x00,
    /// codec gain volume ch
    HL_CODEC_SOUND_CH_L,  
    /// codec gain volume ch
    HL_CODEC_SOUND_CH_R,  
} HL_ENUM8(hl_codec_sound_ch_e);

#if HL_IS_TX_DEVICE()
typedef enum _hl_codec_device_e
{
    /// codec gain volume ch
    HL_CODEC_DEVICE_MIC = 0x00, 
} HL_ENUM8(hl_codec_device_e);
#else
typedef enum _hl_codec_device_e
{
    /// codec gain volume ch
    HL_CODEC_DEVICE_CAM = 0x00,
    /// codec gain volume ch
    HL_CODEC_DEVICE_HP,  
} HL_ENUM8(hl_codec_device_e);
#endif

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
rt_err_t hl_drv_audio_register_stream(drv_audio_stream_rcv_cb_t audio_cb_func);


#endif /* __RK_AUDIO_H__ */
/*
 * EOF
 */
