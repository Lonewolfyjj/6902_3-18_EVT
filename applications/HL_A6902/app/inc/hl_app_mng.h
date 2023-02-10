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
#include "hl_util_general_type.h"

/* typedef -------------------------------------------------------------------*/
/// 模块层发送给APP的消息格式
#if HL_IS_TX_DEVICE()
typedef struct _tx_app_info_t
{
    /// 设备开关标志(0:关机状态  1：开机状态)
    uint8_t             on_off_flag;
    /// usb接口状态(1:连接 0：未连接)
    uint8_t             usb_plug:1;
    /// usb接口状态(1:连接 0：未连接)
    uint8_t             usb_pogo_flag:1;
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
    /// 充电标志(1:充电中 0:不充电 2：充满)
    uint8_t             charge_flag:2;
    /// 升级标志(0:空闲 1：升级中 2：升级成功 3：升级失败)
    uint8_t             upgrade_flag:2;
    /// mute(1:mute on 0:mute off)
    uint8_t             mute_flag;
    /// 录制保护标志(1:录制保护开  0：录制保护关)
    uint8_t             rec_protect_flag;
    /// 自动录制标志(1:自动录制开  0：自动录制关)
    uint8_t             rec_auto_flag;
    /// 降噪保护标志(1:降噪保护开  0：降噪保护关)
    uint8_t             denoise_protect_flag;
    /// 自动降噪标志(1:自动降噪开  0：自动降噪关)
    uint8_t             denoise_auto_flag;
    /// 降噪等级()
    uint8_t             denoise_class;
    /// 升级状态(hl_mod_upgrade_state)
    uint8_t             upgrade_state;
    /// TX增益设置
    int32_t              gain;
    /// UAC的增益设置
    int32_t              uac_gain;
    
    /// 无线状态
    hl_rf_state_e       rf_state;
    /// TX当前声道(0:左声道  1：右声道)
    hl_rf_channel_e     rf_chn;
    /// 当前电量
    uint8_t             soc;

    /// 本地Mac地址
    uint8_t             local_mac[6];
    /// 配对Mac地址
    uint8_t             remote_mac[6];
} tx_app_info_t;
extern tx_app_info_t tx_info;
#else
typedef struct _rx_app_info_t
{
    /// 设备开关标志(0:关机状态  1：开机状态)
    uint8_t             on_off_flag;
    /// usb接口状态(1:连接 0：未连接)
    uint8_t             usb_plug:1;
    /// usb接口状态(1:连接 0：未连接)
    uint8_t             usb_pogo_flag:1;
    /// uac连接标志(1:连接 0：未连接)
    uint8_t             uac_link_flag:1;
    /// 大容量接口状态(1:连接 0：未连接)
    uint8_t             mstorage_plug:1;
    /// 耳机监听接口状态(1:连接 0：未连接)
    uint8_t             hp_spk_plug:1;
    /// 耳机监听接口状态(1:连接 0：未连接)
    uint8_t             cam_spk_plug:1;
    /// 充电标志(1:充电中 0:不充电 2：充满)
    uint8_t             charge_flag:2;

    /// MUTE标志(1:MUTE 0:正常)
    uint8_t             tx1_mute:1;
    /// MUTE标志(1:MUTE 0:正常)
    uint8_t             tx2_mute:1;

    // /// 降噪标志(1:降噪中 0:不降噪)
    // uint8_t             tx1_denoise:1;
    // /// 降噪标志(1:降噪中 0:不降噪)
    // uint8_t             tx2_denoise:1;

    // /// 录制标志(1:录制中 0:不录制)
    // uint8_t             tx1_record:1;
    // /// 录制标志(1:录制中 0:不录制)
    // uint8_t             tx2_record:1;

    /// 无线状态
    hl_rf_state_e       rf_state;    
    /// TX当前声道(0:左声道  1：右声道)
    hl_rf_channel_e     rf_chn;
    /// 当前电量
    uint8_t             soc;
    /// 当前TX亮度
    uint8_t             tx_led_britness;

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

    /// 监听口增益设置
    int32_t              hp_gain;
    /// 相机口增益设置
    int32_t              cam_gain_l;
    int32_t              cam_gain_r;

    /// 本地Mac地址
    uint8_t             local_mac[6];
    /// 配对Mac地址 tx1:[0 - 5] tx2:[6 - 11]
    uint8_t             remote_mac[12];
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

/**
 * 
 * @brief 关机充电APP，Type-C关机充电显示
 * @param [in] msg_q 消息队列
 * @date 2023-01-03
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-01-03      <td>yangxianyun     <td>新建
 * </table>
 */
void hl_app_mng_charger_entry(void *msg_q);

#endif /* __HL_APP_MNG_H__ */
/*
 * EOF
 */
