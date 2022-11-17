/**
 * @file hl_app_mng.h
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-11-14
 * 
 * ██╗  ██╗ ██████╗ ██╗     ██╗  ██╗   ██╗██╗      █████╗ ███╗   ██╗██████╗ 
 * ██║  ██║██╔═══██╗██║     ██║  ╚██╗ ██╔╝██║     ██╔══██╗████╗  ██║██╔══██╗
 * ███████║██║   ██║██║     ██║   ╚████╔╝ ██║     ███████║██╔██╗ ██║██║  ██║
 * ██╔══██║██║   ██║██║     ██║    ╚██╔╝  ██║     ██╔══██║██║╚██╗██║██║  ██║
 * ██║  ██║╚██████╔╝███████╗███████╗██║   ███████╗██║  ██║██║ ╚████║██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-11-14     <td>v1.0     <td>luzhanghao     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_APP_MNG_H__
#define __HL_APP_MNG_H__
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "hl_config.h"

/* typedef -------------------------------------------------------------------*/
/// 模块层发送给APP的消息格式
#if HL_GET_DEVICE_TYPE()
typedef struct _tx_app_info_t
{
    /// usb接口状态(1:连接 0：未连接)
    uint8_t             usb_plug:1;
    /// 外置mic接口状态(1:连接 0：未连接)
    uint8_t             ex_mic_plug:1;
    /// 录制标志(1:录制中  0：没在录制)
    uint8_t             rec_flag:1;
    /// 降噪标志(1:打开降噪 0:关闭降噪)
    uint8_t             denoise_flag:1;

    /// 无线状态
    uint8_t      rf_state;

} tx_app_info_t;
extern tx_app_info_t tx_info;
#else
typedef struct _rx_app_info_t
{
    /// usb接口状态(1:连接 0：未连接)
    uint8_t             usb_plug:1;
    /// 耳机监听接口状态(1:连接 0：未连接)
    uint8_t             hp_spk_plug:1;
    /// 耳机监听接口状态(1:连接 0：未连接)
    uint8_t             cam_spk_plug:1;

    /// 无线状态
    uint8_t             rf_state;
    int32_t             cur_volume_r;
    int32_t             cur_volume_l;

    /// 单声道音量
    int32_t             single_volume;
    /// 立体声右音量
    int32_t             stereo_volume_r;
    /// 立体声左音量
    int32_t             stereo_volume_l;
    /// 安全音轨右音量
    int32_t             safety_volume_r;
    /// 安全音轨左音量
    int32_t             safety_volume_l;
} rx_app_info_t;
extern rx_app_info_t rx_info;
#endif

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#endif /* __HL_APP_MNG_H__ */
/*
 * EOF
 */
