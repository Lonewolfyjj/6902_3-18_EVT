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
#if HL_IS_TX_DEVICE()
typedef struct _tx_app_info_t
{
    /// 设备开关标志(0:关机状态  1：开机状态)
    uint8_t             on_off_flag;
    /// usb接口状态(1:连接 0：未连接)
    uint8_t             usb_plug:1;
    /// uac连接标志(1:连接 0：未连接)
    uint8_t             uac_link_flag:1;
    /// 大容量接口状态(1:连接 0：未连接)
    uint8_t             mstorage_plug:1;
    /// 外置mic接口状态(1:连接 0：未连接)
    uint8_t             ex_mic_plug:1;
    /// 录制标志(1:录制中  0：没在录制)
    uint8_t             rec_flag:1;
    /// 降噪标志(1:打开降噪 0:关闭降噪)
    uint8_t             denoise_flag:1;
    /// 充电标志(1:充电中 0:不充电)
    uint8_t             charge_flag;

    /// 无线状态
    hl_rf_state_e       rf_state;
    /// 当前电量
    uint8_t             soc;

} tx_app_info_t;
extern tx_app_info_t tx_info;
#else
typedef struct _rx_app_info_t
{
    /// 设备开关标志(0:关机状态  1：开机状态)
    uint8_t             on_off_flag;
    /// usb接口状态(1:连接 0：未连接)
    uint8_t             usb_plug:1;
    /// uac连接标志(1:连接 0：未连接)
    uint8_t             uac_link_flag:1;
    /// 大容量接口状态(1:连接 0：未连接)
    uint8_t             mstorage_plug:1;
    /// 耳机监听接口状态(1:连接 0：未连接)
    uint8_t             hp_spk_plug:1;
    /// 耳机监听接口状态(1:连接 0：未连接)
    uint8_t             cam_spk_plug:1;
    /// 充电标志(1:充电中 0:不充电)
    uint8_t             charge_flag;

    /// 无线状态
    hl_rf_state_e       rf_state;
    /// 当前电量
    uint8_t             soc;

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
/**
 * @brief 开机，初始化所有模块
 * @date 2022-12-14
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-12-14      <td>luzhanghao     <td>新建
 * </table>
 */
void hl_app_mng_powerOn(void);

/**
 * @brief 关机，去初始化所有模块
 * @date 2022-12-14
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-12-14      <td>luzhanghao     <td>新建
 * </table>
 */
void hl_app_mng_powerOff(void);

#endif /* __HL_APP_MNG_H__ */
/*
 * EOF
 */
