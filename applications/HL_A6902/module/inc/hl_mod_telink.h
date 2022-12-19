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
#include "hl_util_general_type.h"

/* typedef -------------------------------------------------------------------*/

typedef enum _hl_mod_telink_ctrl_cmd
{
    /// 获取版本号：类型无
    HL_RF_GET_VERSION_CMD = 0x00,
    /// 获取无线模块当前状态
    HL_RF_GET_PAIR_STATE_CMD = 0x01,
    /// 开始配对：类型hl_rf_channel_e
    HL_RF_PAIR_START_CMD = 0x02,
    /// 停止配对：类型无
    HL_RF_PAIR_STOP_CMD = 0x03,
    /// 获取RSSI值：类型无
    HL_RF_GET_RSSI_CMD = 0x04,
    /// 切换发射天线：类型hl_rf_antenna_e
    HL_RF_SWITCH_ANTENNA_CMD = 0x05,
    /// 设置RF射频发射功率：类型uint8_t (0~23)
    HL_RF_SET_RF_POWER_CMD = 0x06,
    /// 设置工作模式：类型hl_rf_work_mode_e
    HL_RF_SET_WORK_MODE_CMD = 0x08,
    /// 获取本地配对信息：类型无
    HL_RF_GET_LOCAL_MAC_CMD = 0x10,
    /// 设置配对设备配对信息：类型hl_rf_pair_info_t
    HL_RF_SET_REMOTE_MAC_CMD = 0x11,
    /// 获取配对设备配对信息：类型无
    HL_RF_GET_REMOTE_MAC_CMD = 0x12,
    /// 透传mute静音状态：类型hl_rf_bypass_info_t
    HL_RF_BYPASS_MUTE_CMD = 0x20,
    /// 透传降噪状态：类型hl_rf_bypass_info_t
    HL_RF_BYPASS_DENOISE_CMD = 0x21,
    /// 透传音量状态：类型hl_rf_bypass_info_t
    HL_RF_BYPASS_VOLUME_CMD = 0x22,
    /// 透传录音状态：类型hl_rf_bypass_info_t
    HL_RF_BYPASS_RECORD_CMD = 0x23,
    /// 透传设置状态：类型hl_rf_bypass_info_t
    HL_RF_BYPASS_SETTING_CMD = 0x24,
} hl_mod_telink_ctrl_cmd;

typedef enum _hl_mod_telink_ctrl_ind
{
    /// 返回版本号：类型uint8_t version[4]
    HL_RF_VERSION_IND = 0x00,
    /// 返回无线模块当前状态：类型hl_rf_pair_state_e
    HL_RF_PAIR_STATE_IND = 0x01,
    /// 返回RSSI值：类型uint8_t (0~100)
    HL_RF_RSSI_IND = 0x04,
    /// 返回设置结果：类型rt_err_t
    HL_RF_SET_WORK_MODE_IND = 0x08,
    /// 返回本地配对信息：类型uint8_t mac[6]
    HL_RF_GET_LOCAL_MAC_IND = 0x10,
    /// 设置配对设备配对信息：类型rt_err_t
    HL_RF_SET_REMOTE_MAC_IND = 0x11,
    /// 返回配对设备配对信息：类型TX uint8_t mac[6] / RX uint8_t mac[12]
    HL_RF_GET_REMOTE_MAC_IND = 0x12,
    /// 返回mute静音状态：类型hl_rf_bypass_info_t
    HL_RF_BYPASS_MUTE_IND = 0x20,
    /// 返回降噪状态：类型hl_rf_bypass_info_t
    HL_RF_BYPASS_DENOISE_IND = 0x21,
    /// 返回音量状态：类型hl_rf_bypass_info_t
    HL_RF_BYPASS_VOLUME_IND = 0x22,
    /// 返回录音状态：类型hl_rf_bypass_info_t
    HL_RF_BYPASS_RECORD_IND = 0x23,
    /// 返回设置状态：类型hl_rf_bypass_info_t
    HL_RF_BYPASS_SETTING_IND = 0x24,
} hl_mod_telink_ctrl_ind;

typedef enum _hl_rf_channel_e
{
    /// 左声道
    HL_RF_LEFT_CHANNEL = 0x00,
    /// 右声道
    HL_RF_RIGHT_CHANNEL,
} hl_rf_channel_e;

typedef enum _hl_rf_onoff_e
{
    /// 关状态
    HL_RF_OFF = 0x00,
    /// 开状态
    HL_RF_ON,
} hl_rf_onoff_e;

typedef enum _hl_rf_antenna_e
{
    /// 双天线自动切换
    EMUN_DOUBLE_ANTENNA = 0x00,
    /// 单天线A
    EMUN_SINGLE_L_ANTENNA,
    /// 单天线B
    EMUN_SINGLE_R_ANTENNA,
} hl_rf_antenna_e;

typedef enum _hl_rf_work_mode_e
{
    /// 高功耗模式（开启RF、音频功能）
    HL_RF_FULL_POWER = 0x00,
    /// 低功耗模式（关闭RF、音频功能）
    HL_RF_LOW_POWER,
} hl_rf_work_mode_e;

typedef struct
{
    /// 主版本号
    uint8_t major_ver;
    /// 次版本号
    uint8_t miror_rev;
    /// 测试版本号
    uint8_t miror_tes;
    /// 有BUG版本号
    uint8_t mioor_bug;
} hl_rf_version_t;

typedef struct
{
    /// 配对声道
    hl_rf_channel_e chn;
    /// MAC地址
    uint8_t mac[6];
} hl_rf_pair_info_t;

typedef struct
{
    /// 透传声道
    hl_rf_channel_e chn;
    union {
        /// 透传状态
        uint8_t state;
        /// 透传数据
        uint8_t* data;
    } info;
} hl_rf_bypass_info_t;

typedef struct
{
    /// 线程运行状态标志
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
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * 
 * @brief 初始化Telink模块,申请内存资源
 * @param [in] input_msq APP层消息队列
 * @return uint8_t 成功 0 | 失败 1
 * @date 2022-11-15
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-15      <td>lisonglin     <td>新建
 * </table>
 */
uint8_t hl_mod_telink_init(rt_mq_t* input_msq);

/**
 * 
 * @brief 去初始化Telink模块,释放内存资源
 * @return uint8_t 成功 0
 * @date 2022-11-15
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-15      <td>lisonglin     <td>新建
 * </table>
 */
uint8_t hl_mod_telink_deinit(void);

/**
 * 
 * @brief 启动Telink线程
 * @return uint8_t 成功 0 | 失败 1
 * @date 2022-11-15
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-15      <td>lisonglin     <td>新建
 * </table>
 */
uint8_t hl_mod_telink_start(void);

/**
 * 
 * @brief 停止Telink线程
 * @return uint8_t 成功 0
 * @date 2022-11-15
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-15      <td>lisonglin     <td>新建
 * </table>
 */
uint8_t hl_mod_telink_stop(void);

/**
 * 
 * @brief 通过命令控制Telink模块
 * @param [in] cmd 命令
 * @param [in] data_addr 数据内容
 * @param [in] data_len 数据长度
 * @return uint8_t 成功 0 | 失败 1
 * @date 2022-11-15
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-15      <td>lisonglin     <td>新建
 * </table>
 */
uint8_t hl_mod_telink_ioctl(uint8_t cmd, uint8_t* data_addr, uint16_t data_len);

#endif
/*
 * EOF
 */
