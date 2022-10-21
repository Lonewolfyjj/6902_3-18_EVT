/**
 * @file hl_drv_rk_xtensa_dsp.c
 * @author yangxianyun (rd52@hollyland-tech.com)
 * @brief Hollyland的DSP驱动的主要实现
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
/* Includes ------------------------------------------------------------------*/

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
    /// role = 0:RX role = 1:TX
    uint16_t role;
    /// 0:get 1:set
    uint16_t io_ctrl_op;
    int16_t  io_ctrl_value;
    uint16_t channel;
    uint16_t b24_2ch_len;
    uint16_t b24_1ch_len;
    uint16_t sample_size;
    uint16_t srp_profile_length;
    uint32_t io_ctrl_param;
    uint32_t b32_2ch_len;
    uint32_t frame_length;
    long*    in_buf_b32_2ch;
    long*    out_buf_b32_2ch;
    long*    out_buf_b24_1ch_after_process;
    long*    out_buf_b24_1ch_before_process;
    long*    out_buf_b24_2ch_after_process;
    long*    out_buf_b24_2ch_before_process;
    char*    srp_profile;
} __attribute__((packed, aligned(1))) lib_alango_srp_param_type_t, *lib_alango_srp_param_type_t_p;

/* define --------------------------------------------------------------------*/

#ifndef HL_GET_DEVICE_TYPE()
#error "HL_GET_DEVICE_TYPE() not define"
#endif

// 以下一段为DSP的帧处理配置，后续可能优化
/// DSP的处理采样通道数
#define DSP_FRAME_CHANNEL 2
/// DSP的处理采样深度
#define DSP_FRAME_BIT 32
/// DSP的处理采样率
#define DSP_FRAME_RATE 48000
/// DSP的处理采样周期
#define DSP_FRAME_PERIOD 120

/// DSP的设备名
#define DSP_DRIVER_NAME "dsp0"

/// DSP驱动的打印函数
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

#define HL_DEVICE_RX_USE_BYPSS_TEST 1
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

#if HL_GET_DEVICE_TYPE()
// tx = 1
static lib_alango_srp_param_type_t_p sg_tx_dsp_param = NULL;
#else
// rx = 0
static lib_alango_srp_param_type_t_p sg_rx_dsp_param = NULL;
#if HL_DEVICE_RX_USE_BYPSS_TEST
static lib_bypass_param_type_t_p sg_rx_bypass_dsp_param = NULL;
#endif
#endif

extern const unsigned char srp_profile[356];

/* Private function(only *.c)  -----------------------------------------------*/

/**
 * _hl_drv_rk_xtensa_dsp_create_work
 * @brief DSP的work创建函数，方便进行work的创建
 * @param [in] id work的id，可以直接使用ASR_WAKE_ID
 * @param [in] algo_type work的类型，后期会动态更改，DSP内会配合该类型进行DSP的处理
 * @param [in] param DSP的参数配置结构体指针
 * @param [in] param_size DSP的参数配置结构体的大小
 * @return struct dsp_work* 
 * @date 2022-09-06
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 该方法直接摘录test内的功能
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-06      <td>yangxianyun     <td>新建
 * </table>
 */
static struct dsp_work* _hl_drv_rk_xtensa_dsp_create_work(uint32_t id, uint32_t algo_type, uint32_t param, uint32_t param_size)
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

#if HL_GET_DEVICE_TYPE()
    RT_ASSERT(sg_tx_dsp_param != NULL);
    sg_dsp_work = _hl_drv_rk_xtensa_dsp_create_work(ASR_WAKE_ID, DSP_ALGO_ALANGO_SRP_INIT, (uint32_t)sg_tx_dsp_param,
                                  sizeof(lib_alango_srp_param_type_t));
    if (!sg_dsp_work) {
        HL_DRV_DSP_LOG("dsp create config work fail\n");
    }
    HL_DRV_DSP_LOG("dsp create config work OK\n");

    // HL_DRV_DSP_LOG("dsp create config work srp OK,addr = 0x%08x, size is %ld\n", sg_tx_dsp_param->srp_profile, sg_tx_dsp_param->srp_profile_length);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, sg_tx_dsp_param->srp_profile, sg_tx_dsp_param->srp_profile_length + 1);

    ret = rt_device_control(sg_dsp_dev, RKDSP_CTL_QUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);
    ret = rt_device_control(sg_dsp_dev, RKDSP_CTL_DEQUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);

    sg_dsp_work->algo_type = DSP_ALGO_ALANGO_SRP_PROCESS;
#else
    // todo rx dsp init
#if !(HL_DEVICE_RX_USE_BYPSS_TEST)
    RT_ASSERT(sg_rx_dsp_param != NULL);
    sg_dsp_work =
        _hl_drv_rk_xtensa_dsp_create_work(ASR_WAKE_ID, DSP_ALGO_ALANGO_SRP_INIT, (uint32_t)sg_rx_dsp_param, sizeof(lib_bypass_param_type_t));
    if (!sg_dsp_work) {
        HL_DRV_DSP_LOG("dsp create config work fail\n");
    }

    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, sg_rx_dsp_param->srp_profile, sg_rx_dsp_param->srp_profile_length + 1);
    ret = rt_device_control(sg_dsp_dev, RKDSP_CTL_QUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);
    ret = rt_device_control(sg_dsp_dev, RKDSP_CTL_DEQUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);

    sg_dsp_work->algo_type = DSP_ALGO_ALANGO_SRP_PROCESS;
#else
    RT_ASSERT(sg_rx_bypass_dsp_param != NULL);
    sg_dsp_work = _hl_drv_rk_xtensa_dsp_create_work(ASR_WAKE_ID, DSP_ALGO_BYPASS_INIT, (uint32_t)sg_rx_bypass_dsp_param,
                                  sizeof(lib_alango_srp_param_type_t));
    if (!sg_dsp_work) {
        HL_DRV_DSP_LOG("dsp create config work fail\n");
    }
    HL_DRV_DSP_LOG("dsp create config work OK\n");

    ret = rt_device_control(sg_dsp_dev, RKDSP_CTL_QUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);
    ret = rt_device_control(sg_dsp_dev, RKDSP_CTL_DEQUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);

    sg_dsp_work->algo_type = DSP_ALGO_BYPASS_PROCESS;
#endif
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

#if HL_GET_DEVICE_TYPE()
    sg_dsp_work->algo_type = DSP_ALGO_ALANGO_SRP_DEINIT;
    ret                    = rt_device_control(sg_dsp_dev, RKDSP_CTL_QUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);
    ret = rt_device_control(sg_dsp_dev, RKDSP_CTL_DEQUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);

    rk_dsp_work_destroy(sg_dsp_work);
    sg_dsp_work = NULL;
#else
    // todo rx dsp deinit
#if !(HL_DEVICE_RX_USE_BYPSS_TEST)
    sg_dsp_work->algo_type = DSP_ALGO_BYPASS_DEINIT;
    ret                    = rt_device_control(sg_dsp_dev, RKDSP_CTL_QUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);
    ret = rt_device_control(sg_dsp_dev, RKDSP_CTL_DEQUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);

    rk_dsp_work_destroy(sg_dsp_work);
    sg_dsp_work = NULL;
#else
    sg_dsp_work->algo_type = DSP_ALGO_BYPASS_DEINIT;
    ret                    = rt_device_control(sg_dsp_dev, RKDSP_CTL_QUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);
    ret = rt_device_control(sg_dsp_dev, RKDSP_CTL_DEQUEUE_WORK, sg_dsp_work);
    RT_ASSERT(!ret);

    rk_dsp_work_destroy(sg_dsp_work);
    sg_dsp_work = NULL;
#endif
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

#if HL_GET_DEVICE_TYPE()
    // tx = 1
    if (!sg_tx_dsp_param) {
        HL_DRV_DSP_LOG("not init\r\n");
        return 1;
    }
    sg_tx_dsp_param->in_buf_b32_2ch                 = config->audio_process_in_buffer_b32_2ch;
    sg_tx_dsp_param->out_buf_b32_2ch                = config->audio_process_out_buffer_b32_2ch;
    sg_tx_dsp_param->out_buf_b24_1ch_before_process = config->audio_before_process_out_buffer_b24_1ch;
    sg_tx_dsp_param->out_buf_b24_1ch_after_process  = config->audio_after_process_out_buffer_b24_1ch;
    sg_tx_dsp_param->channel                        = config->channels;
    sg_tx_dsp_param->sample_size                    = config->bits >> 3;
    sg_tx_dsp_param->frame_length                   = config->period_size;
    sg_tx_dsp_param->b32_2ch_len                    = config->buffer_size_b32_2ch;
    sg_tx_dsp_param->b24_1ch_len                    = config->buffer_size_b24_1ch;
    sg_tx_dsp_param->role                           = 1;
    sg_tx_dsp_param->srp_profile                    = srp_profile;
    sg_tx_dsp_param->srp_profile_length             = sizeof(srp_profile);
    sg_dsp_frame_bytes = sg_tx_dsp_param->frame_length * sg_tx_dsp_param->sample_size * sg_tx_dsp_param->channel;
    RT_ASSERT(sg_dsp_frame_bytes != 0);

    HL_DRV_DSP_LOG("\r\nsg_tx_dsp_param->main_in_buf = %08x\r\n", sg_tx_dsp_param->in_buf_b32_2ch);
    HL_DRV_DSP_LOG("sg_tx_dsp_param->out_buf = %08x\r\n", sg_tx_dsp_param->out_buf_b32_2ch);
    HL_DRV_DSP_LOG("sg_tx_dsp_param->channel = %d\r\n", sg_tx_dsp_param->channel);
    HL_DRV_DSP_LOG("sg_tx_dsp_param->sample_size = %d\r\n", sg_tx_dsp_param->sample_size);
    HL_DRV_DSP_LOG("sg_tx_dsp_param->frame_length = %d\r\n", sg_tx_dsp_param->frame_length);

#else
    // rx = 0
#if !(HL_DEVICE_RX_USE_BYPSS_TEST)
    if (!sg_rx_dsp_param) {
        // not init
        return 1;
    }
    sg_rx_dsp_param->in_buf_b32_2ch                 = config->audio_process_in_buffer_b32_2ch;
    sg_rx_dsp_param->out_buf_b32_2ch                = config->audio_process_out_buffer_b32_2ch;
    sg_rx_dsp_param->out_buf_b24_2ch_before_process = config->audio_before_process_out_buffer_b24_2ch;
    sg_rx_dsp_param->out_buf_b24_2ch_after_process  = config->audio_after_process_out_buffer_b24_2ch;
    sg_rx_dsp_param->channel                        = config->channels;
    sg_rx_dsp_param->sample_size                    = config->bits >> 3;
    sg_rx_dsp_param->frame_length                   = config->period_size;
    sg_rx_dsp_param->b32_2ch_len                    = config->buffer_size_b32_2ch;
    sg_rx_dsp_param->b24_2ch_len                    = config->buffer_size_b24_2ch;
    sg_rx_dsp_param->role                           = 1;
    sg_rx_dsp_param->srp_profile                    = srp_profile;
    sg_rx_dsp_param->srp_profile_length             = sizeof(srp_profile);
    sg_dsp_frame_bytes = sg_rx_dsp_param->frame_length * sg_rx_dsp_param->sample_size * sg_rx_dsp_param->channel;
    RT_ASSERT(sg_dsp_frame_bytes != 0);

    HL_DRV_DSP_LOG("\r\nsg_rx_dsp_param->main_in_buf = %08x\r\n", sg_rx_dsp_param->in_buf_b32_2ch);
    HL_DRV_DSP_LOG("sg_rx_dsp_param->out_buf = %08x\r\n", sg_rx_dsp_param->out_buf_b32_2ch);
    HL_DRV_DSP_LOG("sg_rx_dsp_param->channel = %d\r\n", sg_rx_dsp_param->channel);
    HL_DRV_DSP_LOG("sg_rx_dsp_param->sample_size = %d\r\n", sg_rx_dsp_param->sample_size);
    HL_DRV_DSP_LOG("sg_rx_dsp_param->frame_length = %d\r\n", sg_rx_dsp_param->frame_length);
#else
    if (!sg_rx_bypass_dsp_param) {
        HL_DRV_DSP_LOG("not init\r\n");
        return 1;
    }
    sg_rx_bypass_dsp_param->in_buf                 = config->audio_process_in_buffer_b32_2ch;
    sg_rx_bypass_dsp_param->out_buf                = config->audio_process_out_buffer_b32_2ch;
    sg_rx_bypass_dsp_param->out_24bit_buf_after_process = config->audio_after_process_out_buffer_b24_2ch;
    sg_rx_bypass_dsp_param->out_24bit_buf_before_process  = config->audio_before_process_out_buffer_b24_2ch;
    sg_rx_bypass_dsp_param->process_buff_len                        = config->buffer_size_b32_2ch;
    sg_rx_bypass_dsp_param->out_buff_len_24bit                    = config->buffer_size_b24_2ch;
    sg_dsp_frame_bytes = sg_rx_bypass_dsp_param->process_buff_len;
    RT_ASSERT(sg_dsp_frame_bytes != 0);
#endif
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
    handle->is_init = 1;
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

#if (HL_GET_DEVICE_TYPE())
    // tx = 1
    sg_dsp_drv_handle->device_role = HL_EM_DRV_RK_DSP_ROLE_TX;
    sg_tx_dsp_param                = rkdsp_malloc(sizeof(lib_alango_srp_param_type_t));
#else
    // rx = 0
#if !(HL_DEVICE_RX_USE_BYPSS_TEST)
    sg_dsp_drv_handle->device_role = HL_EM_DRV_RK_DSP_ROLE_RX;
    sg_rx_dsp_param                = malloc(sizeof(lib_alango_srp_param_type_t));
#else
    sg_dsp_drv_handle->device_role = HL_EM_DRV_RK_DSP_ROLE_TX;
    sg_rx_bypass_dsp_param                = rkdsp_malloc(sizeof(lib_bypass_param_type_t));
#endif
#endif

#if HL_GET_DEVICE_TYPE()
    sg_tx_dsp_param->io_ctrl_op = 3;
#else
    sg_rx_dsp_param->io_ctrl_op = 3;
#endif //<

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

#if (HL_GET_DEVICE_TYPE())
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
#if HL_GET_DEVICE_TYPE()
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, sg_tx_dsp_param->in_buf_b32_2ch, sg_tx_dsp_param->b32_2ch_len);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, sg_tx_dsp_param->out_buf_b32_2ch, sg_tx_dsp_param->b32_2ch_len);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, sg_tx_dsp_param->out_buf_b24_1ch_after_process,
                         sg_tx_dsp_param->b24_1ch_len);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, sg_tx_dsp_param->out_buf_b24_1ch_before_process,
                         sg_tx_dsp_param->b24_1ch_len);

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
#if !(HL_DEVICE_RX_USE_BYPSS_TEST)
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, sg_rx_dsp_param->in_buf_b32_2ch, sg_rx_dsp_param->b32_2ch_len);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, sg_rx_dsp_param->out_buf_b32_2ch, sg_rx_dsp_param->b32_2ch_len);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, sg_rx_dsp_param->out_buf_b24_2ch_after_process,
                         sg_rx_dsp_param->b24_2ch_len);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, sg_rx_dsp_param->out_buf_b24_2ch_before_process,
                         sg_rx_dsp_param->b24_2ch_len);

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
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, sg_rx_bypass_dsp_param->in_buf, sg_rx_bypass_dsp_param->process_buff_len);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, sg_rx_bypass_dsp_param->out_buf, sg_rx_bypass_dsp_param->process_buff_len);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, sg_rx_bypass_dsp_param->out_24bit_buf_after_process,
                         sg_rx_bypass_dsp_param->out_buff_len_24bit);
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, sg_rx_bypass_dsp_param->out_24bit_buf_before_process,
                         sg_rx_bypass_dsp_param->out_buff_len_24bit);

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
#endif
    return 0;
}

uint8_t hl_drv_rk_xtensa_dsp_io_ctrl(uint8_t cmd, void* ptr, uint16_t len)
{
    if (!sg_dsp_drv_handle->is_init) {
        // not init
        return 1;
    }

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
        case HL_EM_DRV_RK_DSP_CMD_DENOISE_DSP:
            if(((uint8_t *)ptr)[0] != 0) {
#if HL_GET_DEVICE_TYPE()
                sg_tx_dsp_param->io_ctrl_op = 0;
#else
                sg_rx_dsp_param->io_ctrl_op = 0;
#endif
                }else {
#if HL_GET_DEVICE_TYPE()
                sg_tx_dsp_param->io_ctrl_op = 3;
#else
                sg_rx_dsp_param->io_ctrl_op = 3;
#endif
            }
            
            break;

        default:
            HL_DRV_DSP_LOG("dsp error ctrl msg = 0x%02x\r\n", cmd);
            break;
    }
    return 0;
}

int denoise_set(int argc, char** argv)
{
    if (argc < 2) {
        return 1;
    }

    if (atoi(argv[1])) {
#if HL_GET_DEVICE_TYPE()
        // tx = 1
        sg_tx_dsp_param->io_ctrl_op = 0;
#else
        // rx = 0
        sg_rx_dsp_param->io_ctrl_op = 0;
#endif
    }else {
#if HL_GET_DEVICE_TYPE()
        // tx = 1
        sg_tx_dsp_param->io_ctrl_op = 3;
#else
        // rx = 0
        sg_rx_dsp_param->io_ctrl_op = 3;
#endif
    }

    return 0;
}

MSH_CMD_EXPORT(denoise_set, audio record test cmd);
/*
 * EOF
 */