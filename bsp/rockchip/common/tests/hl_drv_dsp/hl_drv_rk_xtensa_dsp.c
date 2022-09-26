/**
 * @file hl_drv_rk_xtensa_dsp.c
 * @author yangxianyun (rd52@hollyland-tech.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-10
 * 
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
/* Includes ------------------------------------------------------------------*/
#if 1
#include <rtthread.h>
#include <rtdevice.h>
#include "hl_drv_rk_xtensa_dsp.h"
#include <stdlib.h>
#include "drv_dsp.h"
#include "dma.h"
#include "rk_audio.h"
#include "drv_heap.h"

/* typedef -------------------------------------------------------------------*/

typedef struct _lib_bypass_param_
{
    uint8_t *in_buf;
    uint8_t *out_buf;
    uint8_t *out_24bit_buf_after_process;
    uint8_t *out_24bit_buf_before_process;
    uint16_t process_buff_len;
    uint16_t out_buff_len_24bit;
} lib_bypass_param_type_t, *lib_bypass_param_type_t_p;

/**
 * 
 * @brief alango的算法参数结构体
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
typedef struct _lib_alango_srp_param_
{
    /// 音频输入数据地址
    uint8_t* main_in_buf;
    uint8_t* subs_in_buf;
    /// 音频输出数据地址
    uint8_t* out_buf;
    /// 采样数据长度，字节
    uint32_t sample_size;
    /// 帧长， 采样周期
    uint32_t frame_length;
    /// 通道数
    uint16_t channel;
} lib_alango_srp_param_type_t, *lib_alango_srp_param_type_t_p;

/* define --------------------------------------------------------------------*/

#ifndef HL_A6902_DEVICE_TYPE
#error "HL_A6902_DEVICE_TYPE not define"
#endif

#define DSP_FRAME_CHANNEL 2
#define DSP_FRAME_BIT 32
#define DSP_FRAME_RATE 48000
#define DSP_FRAME_PERIOD 120

#define DSP_DRIVER_NAME "dsp0"

#define HL_DRV_DSP_LOG rt_kprintf

//-----------------------------DSP feature-------------------------------------

/// DSP work flag
#define ASR_WAKE_ID 0x50000002
/// bypass 功能的初始化枚举
#define DSP_ALGO_BYPASS_INIT 0x40000009
/// bypass 功能的运行枚举
#define DSP_ALGO_BYPASS_PROCESS 0x4000000a
/// bypass 功能的去初始化枚举
#define DSP_ALGO_BYPASS_DEINIT 0x4000000b
/// tx alango srp算法功能的初始化枚举
#define DSP_ALGO_ALANGO_SRP_INIT 0x4000000c
/// tx alango srp算法功能的运行入口枚举
#define DSP_ALGO_ALANGO_SRP_PROCESS 0x4000000d
/// tx alango srp算法功能的去初始化枚举
#define DSP_ALGO_ALANGO_SRP_DEINIT 0x4000000e
//-----------------------------------------------------------------------------

/* variables -----------------------------------------------------------------*/

/// 全局的DSP驱动的控制句柄
static hl_drv_rk_xtensa_dsp_t_p sg_dsp_drv_handle = NULL;
/// 全局的DSP参数的配置句柄
static hl_drv_rk_xtensa_dsp_config_t_p sg_dsp_drv_process_config = NULL;
/// 全局的DSP驱动设备的指针
static struct rt_device* sg_dsp_dev = NULL;
/// 全局的DSP任务句柄
static struct dsp_work* sg_dsp_work = NULL;
/// 全局的帧长计算保存
static uint16_t sg_dsp_frame_bytes = 0;

#if HL_A6902_DEVICE_TYPE
// tx = 1
static lib_alango_srp_param_type_t_p sg_tx_dsp_param = NULL;
#else
// rx = 0
static lib_bypass_param_type_t_p sg_rx_dsp_param = NULL;
#endif

/* Private function(only *.c)  -----------------------------------------------*/

static struct dsp_work* dsp_create_work(uint32_t id, uint32_t algo_type, uint32_t param, uint32_t param_size)
{
    struct dsp_work* work;

    work = rk_dsp_work_create(RKDSP_ALGO_WORK);
    if (work) {
        work->id         = id;
        work->algo_type  = algo_type;
        work->param      = param;
        work->param_size = param_size;
    }
    return work;
}

/**
 * _hl_drv_rk_xtensa_dsp_enable_dsp
 * @brief 该函数提供给io_ctrl使用，用来调起DSP的初始化
 * @return uint8_t 运行结果 0：运行成功 | 非0：运行失败
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
static uint8_t _hl_drv_rk_xtensa_dsp_enable_dsp()
{
    int      ret = 0;
    uint32_t rate;
    rt_device_control(sg_dsp_dev, RKDSP_CTL_SET_FREQ, (void*)(396 * MHZ));
    rt_device_control(sg_dsp_dev, RKDSP_CTL_GET_FREQ, (void*)&rate);
    HL_DRV_DSP_LOG("current dsp freq: %d MHz\n", rate / MHZ);

#if HL_A6902_DEVICE_TYPE
    RT_ASSERT(sg_tx_dsp_param != NULL);
    sg_dsp_work = dsp_create_work(ASR_WAKE_ID, DSP_ALGO_ALANGO_SRP_INIT, (uint32_t)sg_tx_dsp_param,
                                  sizeof(lib_alango_srp_param_type_t));
    if (!sg_dsp_work) {
        HL_DRV_DSP_LOG("dsp create config work fail\n");
    }
    ret = rt_device_control(sg_dsp_dev, RKDSP_CTL_QUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);
    ret = rt_device_control(sg_dsp_dev, RKDSP_CTL_DEQUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);

    sg_dsp_work->algo_type = DSP_ALGO_ALANGO_SRP_PROCESS;
#else
    // todo rx dsp init
    RT_ASSERT(sg_rx_dsp_param != NULL);
    sg_dsp_work =
        dsp_create_work(ASR_WAKE_ID, DSP_ALGO_BYPASS_INIT, (uint32_t)sg_rx_dsp_param, sizeof(lib_bypass_param_type_t));
    if (!sg_dsp_work) {
        HL_DRV_DSP_LOG("dsp create config work fail\n");
    }
    ret = rt_device_control(sg_dsp_dev, RKDSP_CTL_QUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);
    ret = rt_device_control(sg_dsp_dev, RKDSP_CTL_DEQUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);

    sg_dsp_work->algo_type = DSP_ALGO_BYPASS_PROCESS;
#endif
    sg_dsp_drv_handle->enable = 1;
    return 0;
}

/**
 * _hl_drv_rk_xtensa_dsp_disable_dsp
 * @brief 该函数提供给ot_ctrl使用，用来调起DSP的去初始化
 * @return uint8_t 运行结果 0：运行成功 | 非0：运行失败
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
static uint8_t _hl_drv_rk_xtensa_dsp_disable_dsp()
{
    int      ret = 0;
    uint32_t rate;

    sg_dsp_drv_handle->enable = 0;

#if HL_A6902_DEVICE_TYPE
    sg_dsp_work->algo_type = DSP_ALGO_ALANGO_SRP_DEINIT;
    ret                    = rt_device_control(sg_dsp_dev, RKDSP_CTL_QUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);
    ret = rt_device_control(sg_dsp_dev, RKDSP_CTL_DEQUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);

    rk_dsp_work_destroy(sg_dsp_work);
    sg_dsp_work = NULL;
#else
    // todo rx dsp deinit
    sg_dsp_work->algo_type = DSP_ALGO_BYPASS_DEINIT;
    ret                    = rt_device_control(sg_dsp_dev, RKDSP_CTL_QUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);
    ret = rt_device_control(sg_dsp_dev, RKDSP_CTL_DEQUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);

    rk_dsp_work_destroy(sg_dsp_work);
    sg_dsp_work = NULL;
#endif

    return 0;
}

/**
 * _hl_drv_rk_xtensa_dsp_set_dsp_config
 * @brief 该函数是给io_ctrl提供的
 * @param [in] config 
 * @return uint8_t 
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
static uint8_t _hl_drv_rk_xtensa_dsp_set_dsp_config(hl_drv_rk_xtensa_dsp_config_t_p config)
{

#if HL_A6902_DEVICE_TYPE
    // tx = 1
    if (!sg_tx_dsp_param) {
        HL_DRV_DSP_LOG("not init\r\n");
        return 1;
    }
    sg_tx_dsp_param->main_in_buf  = config->audio_process_in_buffer;
    sg_tx_dsp_param->out_buf      = config->audio_process_out_buffer;
    sg_tx_dsp_param->out_buf      = config->audio_process_out_buffer;
    sg_tx_dsp_param->channel      = config->channels;
    sg_tx_dsp_param->sample_size  = config->bits >> 3;
    sg_tx_dsp_param->frame_length = config->period_size;
    sg_dsp_frame_bytes = sg_tx_dsp_param->frame_length * sg_tx_dsp_param->sample_size * sg_tx_dsp_param->channel;
    RT_ASSERT(sg_dsp_frame_bytes != 0);

    HL_DRV_DSP_LOG("\r\nsg_tx_dsp_param->main_in_buf = %08x\r\n", sg_tx_dsp_param->main_in_buf);
    HL_DRV_DSP_LOG("sg_tx_dsp_param->out_buf = %08x\r\n", sg_tx_dsp_param->out_buf);
    HL_DRV_DSP_LOG("sg_tx_dsp_param->channel = %d\r\n", sg_tx_dsp_param->channel);
    HL_DRV_DSP_LOG("sg_tx_dsp_param->sample_size = %d\r\n", sg_tx_dsp_param->sample_size);
    HL_DRV_DSP_LOG("sg_tx_dsp_param->frame_length = %d\r\n", sg_tx_dsp_param->frame_length);

#else
    // rx = 0
    if (!sg_rx_dsp_param) {
        // not init
        return 1;
    }
    // todo: set rx param
    sg_rx_dsp_param->in_buf         = config->audio_process_in_buffer;
    sg_rx_dsp_param->out_buf        = config->audio_process_out_buffer;
    sg_rx_dsp_param->out_24bit_buf_after_process  = config->audio_process_out_buffer_24bit_a;
    sg_rx_dsp_param->out_24bit_buf_before_process  = config->audio_process_out_buffer_24bit_b;
    sg_rx_dsp_param->process_buff_len = config->period_size * config->channels * (4);
    sg_rx_dsp_param->out_buff_len_24bit = config->period_size * config->channels * (3);
    sg_dsp_frame_bytes              = sg_rx_dsp_param->process_buff_len;
    RT_ASSERT(sg_dsp_frame_bytes != 0);

    // HL_DRV_DSP_LOG("\r\nsg_rx_dsp_param->in_buf = %08x\r\n", sg_tx_dsp_param->main_in_buf);
    // HL_DRV_DSP_LOG("sg_rx_dsp_param->out_buf = %08x\r\n", sg_tx_dsp_param->out_buf);
    // HL_DRV_DSP_LOG("sg_rx_dsp_param->channel = %d\r\n", sg_tx_dsp_param->channel);
    // HL_DRV_DSP_LOG("sg_rx_dsp_param->sample_size = %d\r\n", sg_tx_dsp_param->sample_size);
    // HL_DRV_DSP_LOG("sg_rx_dsp_param->frame_length = %d\r\n", sg_tx_dsp_param->frame_length);
#endif

    return 0;
}

/**
 * _hl_drv_rk_stensa_dsp_init_frame
 * @brief 初始化帧格式
 * @param [in] handle 输入的DSP驱动的句柄
 * @return uint8_t 初始化结果 0：成功
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
static uint8_t _hl_drv_rk_stensa_dsp_init_frame(hl_drv_rk_xtensa_dsp_t_p handle)
{
    if (!handle) {
        return 1;
    }

    handle->frame.frame_channel     = DSP_FRAME_CHANNEL;
    handle->frame.frame_sample_bit  = DSP_FRAME_BIT;
    handle->frame.frame_sample_rate = DSP_FRAME_RATE;
    handle->frame.frame_period      = DSP_FRAME_PERIOD;
    return 0;
}

/* Exported functions --------------------------------------------------------*/

uint8_t hl_drv_rk_xtensa_dsp_init()
{
    int ret = 0;

    // 1. 资源创建
    if (!sg_dsp_drv_handle) {
        sg_dsp_drv_handle = (hl_drv_rk_xtensa_dsp_t_p)malloc(sizeof(hl_drv_rk_xtensa_dsp_t));
    }

#if (HL_A6902_DEVICE_TYPE)
    // tx = 1
    sg_dsp_drv_handle->device_role = HL_EM_DRV_RK_DSP_ROLE_TX;
    sg_tx_dsp_param                = rkdsp_malloc(sizeof(lib_alango_srp_param_type_t));
#else
    // rx = 0
    sg_dsp_drv_handle->device_role = HL_EM_DRV_RK_DSP_ROLE_RX;
    sg_rx_dsp_param                = (lib_bypass_param_type_t_p)malloc(sizeof(lib_bypass_param_type_t));
#endif

    ret = _hl_drv_rk_stensa_dsp_init_frame(sg_dsp_drv_handle);
    if (ret) {
        // do something to show msg
        HL_DRV_DSP_LOG("fram init error\r\n");
    }

    sg_dsp_dev = rt_device_find("dsp0");
    RT_ASSERT(sg_dsp_dev != RT_NULL);
    rt_device_open(sg_dsp_dev, RT_DEVICE_OFLAG_RDWR);

    return ret;
}

uint8_t hl_drv_rk_xtensa_dsp_deinit()
{
    if (!sg_dsp_drv_handle) {
        return 0;
    }

    if (sg_dsp_drv_handle->enable) {
        // not disable it
        return 1;
    }

    rt_device_close(sg_dsp_dev);

#if (HL_A6902_DEVICE_TYPE)
    // tx = 1
    if (sg_tx_dsp_param) {
        rkdsp_free(sg_tx_dsp_param);
        sg_tx_dsp_param = NULL;
    }
#else
    // rx = 0
    if (sg_rx_dsp_param) {
        rkdsp_free(sg_rx_dsp_param);
        sg_rx_dsp_param = NULL;
    }
#endif

    free(sg_dsp_drv_handle);

    return 0;
}

uint8_t hl_drv_rk_xtensa_dsp_transfer()
{
    int ret = 0;
    if (!sg_dsp_drv_handle->enable) {
        // not enable dsp
        return 1;
    }
    // HL_DRV_DSP_LOG("dsp work type = 0x%02x\r\n", sg_dsp_work->algo_type);
#if HL_A6902_DEVICE_TYPE
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, sg_tx_dsp_param->main_in_buf, sg_dsp_frame_bytes);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, sg_tx_dsp_param->out_buf, sg_dsp_frame_bytes);

    ret = rt_device_control(sg_dsp_dev, RKDSP_CTL_QUEUE_WORK, sg_dsp_work);
    if (ret) {
        HL_DRV_DSP_LOG("dsp push work error\r\n");
        return ret;
    }

    ret = rt_device_control(sg_dsp_dev, RKDSP_CTL_DEQUEUE_WORK, sg_dsp_work);
    if (ret) {
        HL_DRV_DSP_LOG("dsp pop work error\r\n");
        return ret;
    }
#else
    // do rx dsp process
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, sg_rx_dsp_param->in_buf, sg_dsp_frame_bytes);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, sg_rx_dsp_param->out_buf, sg_dsp_frame_bytes);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, sg_rx_dsp_param->out_24bit_buf_after_process, sg_rx_dsp_param->out_buff_len_24bit);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, sg_rx_dsp_param->out_24bit_buf_before_process, sg_rx_dsp_param->out_buff_len_24bit);

    ret = rt_device_control(sg_dsp_dev, RKDSP_CTL_QUEUE_WORK, sg_dsp_work);
    if (ret) {
        HL_DRV_DSP_LOG("dsp push work error\r\n");
        return ret;
    }

    ret = rt_device_control(sg_dsp_dev, RKDSP_CTL_DEQUEUE_WORK, sg_dsp_work);
    if (ret) {
        HL_DRV_DSP_LOG("dsp pop work error\r\n");
        return ret;
    }
#endif
    return 0;
}

uint8_t hl_drv_rk_xtensa_dsp_io_ctrl(uint8_t cmd, void* ptr, uint16_t len)
{
    switch (cmd) {
        case HL_EM_DRV_RK_DSP_CMD_SET_CONFIG:
            _hl_drv_rk_xtensa_dsp_set_dsp_config((hl_drv_rk_xtensa_dsp_t_p)ptr);
            break;
        case HL_EM_DRV_RK_DSP_CMD_START_DSP:
            _hl_drv_rk_xtensa_dsp_enable_dsp();
            break;
        case HL_EM_DRV_RK_DSP_CMD_STOP_DSP:
            _hl_drv_rk_xtensa_dsp_disable_dsp();
            break;

        default:
            HL_DRV_DSP_LOG("dsp error ctrl msg = 0x%02x\r\n", cmd);
            break;
    }
    return 0;
}
#endif
/*
 * EOF
 */