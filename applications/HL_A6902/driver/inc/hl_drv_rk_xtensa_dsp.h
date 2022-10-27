/**
 * @file hl_drv_rk_xtensa_dsp.h
 * @author yangxianyun (rd52@hollyland-tech.com)
 * @brief Hollyland的DSP音频驱动，用来提供一系列的API来进行CM4和DSP的音频数据交互的
 *      在TX：CM4做音频数据的采集，DSP做降噪等功能
 *      在RX：CM4做音频数据的输出，DSP做混音等功能
 * 
 * @version 0.1
 * @date 2022-08-10
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
 * <tr><td>2022-08-10     <td>v1.0     <td>yangxianyun     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_DRV_RK_XTENSA_DSP_H__
#define __HL_DRV_RK_XTENSA_DSP_H__
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "hl_config.h"

#ifdef __cplusplus
extern "C" {
#endif
/* typedef -------------------------------------------------------------------*/

/// DSP的控制命令枚举
typedef enum _hl_drv_rk_xtensa_dsp_cmd_e_
{
    /// 设置配置，主要是DSP的帧格式
    HL_EM_DRV_RK_DSP_CMD_SET_CONFIG = 0x00,
    /// 启动DSP
    HL_EM_DRV_RK_DSP_CMD_START_DSP,
    /// 停止DSP
    HL_EM_DRV_RK_DSP_CMD_STOP_DSP,
    /// 降噪
    HL_EM_DRV_RK_DSP_CMD_DENOISE_DSP,
} hl_drv_rk_xtensa_dsp_cmd_e;

/// DSP的运行角色枚举
typedef enum _hl_drv_rk_xtensa_dsp_role_e_
{
    /// DSP驱动在TX上
    HL_EM_DRV_RK_DSP_ROLE_TX = 0x00,
    /// DSP驱动在RX上
    HL_EM_DRV_RK_DSP_ROLE_RX,
} hl_drv_rk_xtensa_dsp_role_e;

/**
 * 
 * @brief 该结构体用于在DSP运行过程中对资源的统一管理 
 * @date 2022-08-10
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-10      <td>yangxianyun     <td>新建
 * </table>
 */
typedef struct _hl_drv_rk_xtensa_dsp_config_t_
{
    /// 采集和输出的操作的大小
    uint32_t process_size;
    /// 采样率
    uint32_t rate;
    /// 采集的通道数 默认为2
    uint32_t channels;
    /// 采样深度
    uint32_t bits;
    /// 采集周期，与帧长正相关
    uint32_t buffer_size_b32_2ch;
    uint32_t period_size;
    /// 音频处理过程中的暂存输入buffer
    char* audio_process_in_buffer_b32_2ch;
    /// 音频处理过程中的暂存输出buffer
    char* audio_process_out_buffer_b32_2ch;
    /// 音频处理过程中的暂存输出buffer
#if HL_GET_DEVICE_TYPE()
    /// tx
    uint32_t buffer_size_b24_1ch;
    char* audio_before_process_out_buffer_b24_1ch;
    char* audio_after_process_out_buffer_b24_1ch;
#else
    /// rx
    uint32_t buffer_size_b24_2ch;
    char* audio_before_process_out_buffer_b24_2ch;
    char* audio_after_process_out_buffer_b24_2ch;
#endif
} hl_drv_rk_xtensa_dsp_config_t, *hl_drv_rk_xtensa_dsp_config_t_p;

/**
 * 
 * @brief 帧格式的结构体
 * @date 2022-08-10
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-10      <td>yangxianyun     <td>新建
 * </table>
 */
typedef struct _hl_drv_rk_xtensa_dsp_frame_t_
{
    /// 帧的采样深度 16：16bit | 24：24bit | 32：32bit
    uint8_t frame_sample_bit;
    /// 帧的通道数 1：1个通道 2：2个通道
    uint8_t frame_channel;
    /// 帧长
    uint16_t frame_period;
    /// 帧的采样率 一般为48KHz：48000
    uint32_t frame_sample_rate;
} hl_drv_rk_xtensa_dsp_frame_t;

/**
 * 
 * @brief 驱动的句柄结构体
 * @date 2022-08-10
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-10      <td>yangxianyun     <td>新建
 * </table>
 */
typedef struct _hl_drv_rk_xtensa_dsp_t_
{
    /// 是否使能DSP， 1：已使能 0：未使能
    uint8_t enable;
    /// 是否使初始化， 1：已初始化 0：未初始化
    uint8_t is_init;
    /// 驱动所在的设备类型
    hl_drv_rk_xtensa_dsp_role_e device_role;
    /// 驱动所使用的帧格式数据
    hl_drv_rk_xtensa_dsp_frame_t frame;
} hl_drv_rk_xtensa_dsp_t, *hl_drv_rk_xtensa_dsp_t_p;

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * hl_drv_rk_xtensa_dsp_init
 * @brief DSP驱动的初始化，建议上电启动就调起。
 *      该函数主要做:  
 *          1. DSP的资源申请
 *          2. DSP驱动的调起
 *          3. DSP work进行初始化
 * @return uint8_t 初始化结果： 0-成功 | 非0-失败
 * @date 2022-08-10
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-10      <td>yangxianyun     <td>新建
 * </table>
 */
uint8_t hl_drv_rk_xtensa_dsp_init();

/**
 * hl_drv_rk_xtensa_dsp_deinit
 * @brief DSP驱动的去初始化
 *      该函数做：
 *          1. 使用DSP work做DSP的去初始化
 *          2. DSP驱动关闭
 *          3. DSP的资源释放
 * @return uint8_t 去初始化结果： 0-成功 | 非0-失败
 * @date 2022-08-10
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-10      <td>yangxianyun     <td>新建
 * </table>
 */
uint8_t hl_drv_rk_xtensa_dsp_deinit();

/**
 * hl_drv_rk_xtensa_dsp_transfer
 * @brief DSP的传输函数，为音频链路的重要功能函数，该函数提供了上层将`write_data`的音频数据传输到DSP中进行处理并写入到`read_data`中的能力，该函数是阻塞的
 *      函数做以下处理：
 *          1. 同步RAM
 *          2. work处理DSP
 *          3. 等待数据dequeue
 * @param [in] write_data 输入的音频数据地址
 * @param [in] write_len 输入的音频数据长度
 * @param [out] read_data 输出的音频数据地址
 * @param [out] read_data_len 输出的音频数据的长度的地址
 * @return uint8_t 运行状态： 0：运行成功 | 非0：运行失败，失败码如下：
 * @date 2022-08-10
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 失败码：
 *              1： work处理失败
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-10      <td>yangxianyun     <td>新建
 * </table>
 */
uint8_t hl_drv_rk_xtensa_dsp_transfer();

/**
 * hl_drv_rk_xtensa_dsp_io_ctrl
 * @brief DSP驱动的控制模块，提供了外部介入的配置功能
 * @param [in] cmd 输入的命令
 * @param [in] ptr 输入的数据地址
 * @param [in] len 输入的数据长度
 * @return uint8_t 命令执行状态： 0：成功 | 非0：失败，具体失败返回看执行函数内容
 * @date 2022-08-10
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-10      <td>yangxianyun     <td>新建
 * </table>
 */
uint8_t hl_drv_rk_xtensa_dsp_io_ctrl(uint8_t cmd, void* ptr, uint16_t len);

#ifdef __cplusplus
}
#endif
#endif  //__HL_DRV_RK_XTENSA_DSP_H__
        /*
 * EOF
 */