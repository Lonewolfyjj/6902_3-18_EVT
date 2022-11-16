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

/* typedef -------------------------------------------------------------------*/
typedef enum _hl_mod_telink_ctrl_cmd
{
    /// 获取版本号
    HL_MOD_TELINK_GET_VERSION_CMD = 0x00,
    /// 获取配对信息
    HL_MOD_TELINK_GET_PAIR_INFO_CMD,
    /// 开始配对 (CMD + 0x00左声道/0x01右声道)
    HL_MOD_TELINK_PAIR_START_CMD,
    /// 停止配对
    HL_MOD_TELINK_PAIR_STOP_CMD,
    /// (测试命令)获取RSSI值
    HL_MOD_TELINK_GET_RSSI_CMD,
    /// (测试命令)切换发射天线 (CMD + 0x00双天线自动切换/0x01固定天线A/0x02固定天线B)
    HL_MOD_TELINK_SWITCH_ANTENNA_CMD,
    /// 无线透传数据 (CMD + 数据地址 + 数据长度)
    HL_MOD_TELINK_BY_PASS_CMD,
    /// (测试命令)设置RF射频发射功率 (0~23档位调节)
    HL_MOD_TELINK_SET_RF_POWER_CMD,
} hl_mod_telink_ctrl_cmd;

typedef enum _hl_mod_telink_ctrl_ind
{
    /// 返回版本号 (CMD + 版本字符串)
    HL_MOD_TELINK_VERSION_IND = 0x00,
    /// 返回配对信息 
    HL_MOD_TELINK_PAIR_INFO_IND,
    /// 开始配对
    HL_MOD_TELINK_PAIR_START_IND,
    /// 停止配对
    HL_MOD_TELINK_PAIR_STOP_IND,
    /// (测试命令)返回RSSI值 (CMD + RSSI值[0~100])
    HL_MOD_TELINK_RSSI_IND,
    /// 无线透传数据 (CMD + 数据地址 + 数据长度)
    HL_MOD_TELINK_BY_PASS_IND,
} hl_mod_telink_ctrl_ind;

typedef enum
{
    PAIR_STOP       = 0x00,
    PAIR_START      = 0x01,
    PAIR_FAILED     = 0x00,
    PAIR_L_SUCCEDD  = 0x01,
    PAIR_R_SUCCEDD  = 0x02,
    PAIR_LR_SUCCEDD = 0x03,
} hl_pair_state_em;

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
 * @return uint8_t 成功 0 | 失败 -1
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
 * @return uint8_t 成功 0 | 失败 -1
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
 * @return uint8_t 成功 0 | 失败 -1
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
