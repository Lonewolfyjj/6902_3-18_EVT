/**
 * @file hl_mod_audio.c
 * @author lixiang (rd37@hollyland-tech.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-13
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
#ifndef __HL_MOD_AUDIO_C__
#define __HL_MOD_AUDIO_C__

/* Includes ------------------------------------------------------------------*/
#include <rtdevice.h>
#include <rtthread.h>
#include <ipc/ringbuffer.h>
#include "drv_dsp.h"
#include "string.h"
#include "stdlib.h"
#include "dma.h"
#include "drv_heap.h"
#include "dfs_posix.h"

#include "hl_mod_audio.h"
#include "hl_drv_audio.h"
#include "hal_audio.h"
#include "hl_drv_rk_xtensa_dsp.h"

#include "hl_drv_aw21009.h"
#include "hl_hal_gpio.h"
#include "./class/uac1.h"
#include "hl_util_msg_type.h"
#include "hl_drv_pcf85063atl.h"

#include "./class/mstorage.h"
#include <dfs_fs.h>

#define DBG_SECTION_NAME "mod_aud"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

/* typedef -------------------------------------------------------------------*/
/// wav音频头结构体
struct wav_header
{
    uint32_t riff_id;
    uint32_t riff_sz;
    uint32_t riff_fmt;
    uint32_t fmt_id;
    uint32_t fmt_sz;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    uint32_t data_id;
    uint32_t data_sz;
};

/// 声卡类型
typedef enum _audio_card_type_e 
{
    /// 空闲状态
    HL_PLAY_CARD = 0,
    /// 等待数据level
    HL_CAPTURE_CARD,
} HL_ENUM8(audio_card_type_e);

/// 声卡参数结构体
typedef struct _hl_card_param_t
{
    char                    card_name[8];
    audio_card_type_e       card_type;
    struct rt_device*       card;
    struct AUDIO_PARAMS     param;
    struct audio_buf        abuf;
} hl_card_param_t;

/// uac参数结构体
typedef struct _hl_uacd_param_t
{
    char*                   buff24b;
    char*                   buff32b;
    uint32_t                buff24size;
    uint32_t                buff32size;
    struct rt_mailbox       *p_mailbox;
    rt_device_t             card;
} hl_uacd_param_t;

/// 录制参数结构体
typedef struct _hl_record_param_t
{
    int                     file_audio_after;
    int                     file_audio_bypass;
    struct rt_ringbuffer*   record_after_rb;
    struct rt_ringbuffer*   record_bypass_rb;
} hl_record_param_t;

/// UAC状态枚举
typedef enum _play_uac_state_e 
{
    /// 空闲状态
    HL_PLAY_UAC_IDLE = 0,
    /// 等待数据level
    HL_PLAY_UAC_WAIT,
    /// UAC数据流读取中
    HL_PLAY_UAC_GOING,
    /// UAC异常
    HL_PLAY_UAC_ERR,
} HL_ENUM8(play_uac_state_e);

/* define --------------------------------------------------------------------*/
#define HL_MOD_AUDIO_RATE (48000)
/// 音频接口传输的是32bit数据，但实际处理的是24bit数据
#define HL_MOD_AUDIO_BITS (32)  /// 24BIT
#define HL_MOD_AUDIO_CHANNELS (2)
#define HL_MOD_AUDIO_PERIOD_SIZE (96) //(120)
#define HL_MOD_AUDIO_PERIOD_COUNT (4)
#define HL_MOD_AUDIO_FRAME_SIZE (HL_MOD_AUDIO_PERIOD_SIZE * HL_MOD_AUDIO_CHANNELS * 3)
#define HL_MOD_AUDIO_RECORD_RING_BUFFER_SIZE (HL_MOD_AUDIO_FRAME_SIZE * 30)  //((HL_MOD_AUDIO_FRAME_SIZE * 30) + 5)

#if HL_IS_TX_DEVICE()
#define HL_PLAY_DEV    "wifip"
#define HL_CAPTURE_DEV "codecc"
#define HL_PDM_CAP_DEV "pdmc"
#else
#define HL_PLAY_DEV    "codecp"
#define HL_CAPTURE_DEV "wific"
#endif

#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT  0x20746d66
#define ID_DATA 0x61746164

#define FORMAT_PCM 1
/* variables -----------------------------------------------------------------*/
static hl_drv_rk_xtensa_dsp_config_t_p dsp_config                = NULL;
struct wav_header                      s_audio_header            = { 0 };

#if HL_IS_TX_DEVICE()
static char                            s_record_switch           = 0;
static uint32_t                        s_record_after_size       = 0;
static uint32_t                        s_record_bypass_size      = 0;
#endif

///  app层消息队列
rt_mq_t s_audio_to_app_mq = RT_NULL;
/// 当前音频流模式
volatile static hl_stream_mode_e  s_stream_mode_cur           = HL_STREAM_IDLE;
/// 下个音频流模式
volatile static hl_stream_mode_e  s_stream_mode_next          = HL_STREAM_CAP2PLAY;
/// 播放声卡参数信息
volatile static hl_card_param_t        play_info;
/// 捕获声卡参数信息
volatile static hl_card_param_t        cap_info;
/// uac声卡参数信息
volatile static hl_uacd_param_t        uac_info;
/// 录制参数信息
volatile static hl_record_param_t      record_info;
/// 录制线程id
rt_thread_t record_thread_id       = RT_NULL;
/// cap2play线程id
rt_thread_t cap2play_thread_id     = RT_NULL;
/// uac2play线程id
rt_thread_t uac2play_thread_id     = RT_NULL;
/// cap2uac线程id
rt_thread_t cap2uac_thread_id      = RT_NULL;
/// cap2p2u线程id
rt_thread_t cap2play2uac_thread_id = RT_NULL;
/// 音频控制线程id
rt_thread_t audio_ctrl_thread_id   = RT_NULL;

/* Private function(only *.c)  -----------------------------------------------*/

#if HL_IS_TX_DEVICE()
static void hl_mod_audio_record(int p_file_audio, uint8_t* buffer, uint32_t size, uint32_t* s_record_size);
static void hl_mod_audio_record_save(int p_file_audio, char* file_name, uint32_t* s_record_size);
static void hl_mod_audio_record_stop(int p_file_audio, uint32_t* s_record_size);
static void hl_mod_audio_record_start(int p_file_audio, uint32_t* s_record_size);
#endif
static void do_record_audio(void* arg);
static int  hl_mod_audio_record_switch(uint8_t record_switch);
// 音频模块发送消息给APP层
static void hl_mod_audio_send_msg(hl_mod_audio_indicate msg_cmd, uint32_t param);

static uint8_t s_record_key_flag = 0;

static void mstorage_switch_cb(uint8_t mstorage_state)
{
    if(mstorage_state != 0) {
#if HL_IS_TX_DEVICE()
        hl_mod_audio_record_switch(0);
#endif
        hl_mod_audio_send_msg(MSG_USB_MSTORAGE_DET, 1);
    } else {
        hl_mod_audio_send_msg(MSG_USB_MSTORAGE_DET, 0);
    }
}

int hl_mod_audio_rtc_set(void)
{    
    // rtc_time time1;
    // time_t now;
    // rt_err_t ret = RT_EOK; 
    
    // hl_drv_rtc_pcf85063_io_ctrl(RTC_GET_TIME, (void*)&time1, sizeof(rtc_time));
    // rt_kprintf("20%02d-%02d-%02d-%02d-%02d-%02d\r\n", time1.year, time1.month&0x1f, time1.day&0x3f, time1.hour&0x3f, time1.minute&0x7f, time1.second&0x7f);
    
    // ret = set_date((time1.year + 2000), (time1.month&0x1f), (time1.day&0x3f)); 
    // if(ret != RT_EOK)
    // {
    //     rt_kprintf("[RTC Test]Set RTC Date failed\n"); 
    //     return RT_ERROR;
    // }
    
    // ret = set_time((time1.hour&0x3f), (time1.minute&0x7f), (time1.second&0x7f)); 
    // if(ret != RT_EOK)
    // {
    //     rt_kprintf("[RTC Test]Set RTC Time failed\n"); 
    //     return RT_ERROR;
    // }
    
    // now = time(RT_NULL);
    // rt_kprintf("[RTC Test]Read RTC Date and Time: %s \r\n", ctime(&now)); 

    return RT_EOK;
}

#if HL_IS_TX_DEVICE()
static void hl_mod_audio_dfs()
{
    rt_device_t disk;
    int ret = 0;
    
    disk = rt_device_find(RT_USB_MSTORAGE_DISK_NAME);
    if(disk == RT_NULL)
    {
        LOG_E("no disk named %s", RT_USB_MSTORAGE_DISK_NAME);
        return -RT_ERROR;
    }

#ifdef RT_USING_DFS_MNTTABLE
    dfs_unmount_device(disk);
    if (dfs_mount_device(disk) < 0) {
        dfs_mkfs("elm", "sd0");
        dfs_mount_device(disk);
        LOG_I("sd0 elm mkfs dfs ");
    }
#endif
    LOG_I("hl mod audio dfs");
}
// static void hl_mod_audio_rtc_set()
// {
//     rtc_time time;
//     memset(&time, 0, sizeof(rtc_time));

//     time.year   = atoi(argv[2]);
//     time.month  = atoi(argv[3]);
//     time.day    = atoi(argv[4]);
//     time.hour   = atoi(argv[5]);
//     time.minute = atoi(argv[6]);
//     time.second = atoi(argv[7]);
//     hl_drv_rtc_pcf85063_io_ctrl(RTC_SET_TIME, (void*)&time, sizeof(rtc_time));
// }

// 获取时间 timer_name参数BUF大小需要大于24
static void hl_mod_audio_rtc_get(char *timer_name)
{
    rtc_time time;
    memset(&time, 0, sizeof(rtc_time));

    hl_drv_rtc_pcf85063_io_ctrl(RTC_GET_TIME, (void*)&time, sizeof(rtc_time));
    /*时间存储格式是按 BCD 码的格式来存储的。如果需要使用，需要转换成十六进制之后才能使用，时间年的范围：0~99，需要注意*/
    rt_kprintf("20%02d-%02d-%02d-%02d-%02d-%02d\r\n", time.year, time.month&0x1f, time.day&0x3f, time.hour&0x3f, time.minute&0x7f, time.second&0x7f);
    if (timer_name != NULL) {
        rt_sprintf(timer_name, "20%02d-%02d-%02d-%02d-%02d-%02d", time.year, time.month&0x1f, time.day&0x3f, time.hour&0x3f, time.minute&0x7f, time.second&0x7f);  
    } else {
        rt_kprintf("timer name get error (timer_name == NULL)\r\n");
    }      
}

static void hl_hal_gpio_audio_record_irq_process(void* args)
{
    s_record_key_flag = 1;
}

static void hl_mod_audio_record(int p_file_audio, uint8_t* buffer, uint32_t size, uint32_t* s_record_size)
{
    if (p_file_audio == -1) {
        LOG_E("Error record file no open");
        return;
    }

    if (write(p_file_audio, buffer, size) != size) {  
        LOG_E("write p_file_audio failed !!!");
        return;
    }
    *s_record_size = (size / 1 / (3)) + *s_record_size; // 只录制一个通道(1)
}

static void hl_mod_audio_record_stop(int p_file_audio, uint32_t* s_record_size)
{
    if(p_file_audio < 0){
        LOG_E("Auaio record stop error(p_file_audio:(NULL))");
        return;
    }
    s_audio_header.data_sz = (*s_record_size) * s_audio_header.block_align;
    s_audio_header.riff_sz = s_audio_header.data_sz + sizeof(s_audio_header) - 8;

    lseek(p_file_audio, 0, SEEK_SET);
    write(p_file_audio, &s_audio_header, sizeof(struct wav_header));
    close(p_file_audio);

    fsync(p_file_audio);
    rt_thread_mdelay(20);

    LOG_I("Auaio record stop (data_sz:(0x%08x),riff_sz:(0x%08x))", s_audio_header.data_sz, s_audio_header.riff_sz);
}

static void hl_mod_audio_record_save(int p_file_audio, char* file_name, uint32_t* s_record_size)
{
    if(*s_record_size < 86400000) {
        return;
    }

    close(p_file_audio);
    // 文件名改变 file_name
    p_file_audio = open(file_name, O_WRONLY | O_CREAT);
    if (p_file_audio == -1) {
        LOG_E("%s:Unable to open file '%s'\n", file_name);
        hl_mod_audio_record_stop(p_file_audio, s_record_size);
        *s_record_size = 0x00;
        return;
    }
}

static void hl_mod_audio_record_start(int p_file_audio, uint32_t* s_record_size)
{
    if (p_file_audio == -1) {
        LOG_E("%s:p_file_audio = -1");
        hl_mod_audio_record_stop(p_file_audio, s_record_size);
        return;
    }

    *s_record_size = 0;

    rt_memset(&s_audio_header, 0x00, sizeof(struct wav_header));

    s_audio_header.riff_id      = ID_RIFF;
    s_audio_header.riff_sz      = 0;
    s_audio_header.riff_fmt     = ID_WAVE;
    s_audio_header.fmt_id       = ID_FMT;
    s_audio_header.fmt_sz       = 16;
    s_audio_header.audio_format = FORMAT_PCM;
    s_audio_header.num_channels = 1;
    s_audio_header.sample_rate  = cap_info.param.sampleRate;

    s_audio_header.bits_per_sample = 24;
    s_audio_header.byte_rate       = (s_audio_header.bits_per_sample / 8) * s_audio_header.num_channels * s_audio_header.sample_rate;
    s_audio_header.block_align     = s_audio_header.num_channels * (s_audio_header.bits_per_sample / 8);
    s_audio_header.data_id         = ID_DATA;

    lseek(p_file_audio, 0, SEEK_SET);
    write(p_file_audio, &s_audio_header, sizeof(struct wav_header));

    /* leave enough room for header */
    lseek(p_file_audio, sizeof(struct wav_header), SEEK_SET);

    //hl_mod_audio_record_save();
}

// 
static int hl_mod_audio_record_switch(uint8_t record_switch)
{
    if (s_record_switch == record_switch) {
        return -1;
    }

    static char timer_name[50] = {0};
    static char timer_name_after[50] = {0};
    static char timer_name_bypass[50] = {0};
    static char timer_name_file[50] = {0};

    if (record_switch) {
        rt_ringbuffer_reset(record_info.record_after_rb);
        rt_ringbuffer_reset(record_info.record_bypass_rb);

        hl_mod_audio_rtc_get(timer_name);
        memcpy(&timer_name_file[0], "/mnt/sdcard/", 12); 
        memcpy(&timer_name_file[12], timer_name, 10); 

        if (access(timer_name_file, 0) < 0)
        {
            LOG_I("create record mkdir %s.", timer_name_file);
            mkdir(timer_name_file, 0); //此处添加异常处理<
        }
        rt_sprintf(timer_name_after, "%s/%s-after.wav", timer_name_file, &timer_name[11]); 
        rt_sprintf(timer_name_bypass, "%s/%s-bypass.wav", timer_name_file, &timer_name[11]); 
        record_info.file_audio_after  = open(timer_name_after, O_WRONLY | O_CREAT | O_TRUNC);   //open("/mnt/sdcard/hl_audio_after.wav", O_WRONLY | O_CREAT | O_TRUNC);
        record_info.file_audio_bypass = open(timer_name_bypass, O_WRONLY | O_CREAT | O_TRUNC);  //open("/mnt/sdcard/hl_audio_bypass.wav", O_WRONLY | O_CREAT | O_TRUNC);

        hl_mod_audio_record_start(record_info.file_audio_after, &s_record_after_size);
        hl_mod_audio_record_start(record_info.file_audio_bypass, &s_record_bypass_size);
        s_record_switch = 1;
        LOG_I("Audio mod record start");
    } else {
        s_record_switch = 0;
        hl_mod_audio_record_stop(record_info.file_audio_after, &s_record_after_size);
        hl_mod_audio_record_stop(record_info.file_audio_bypass, &s_record_bypass_size);
        LOG_I("Audio mod record stop");
    }
    return 0;
}

#endif

// 捕获和播放声卡内存申请和参数配置
static rt_err_t hl_mod_audio_param_config(void)
{
    /* 捕获声卡配置 */
    strncpy(cap_info.card_name, HL_CAPTURE_DEV, 8);
    cap_info.card_type          = HL_CAPTURE_CARD;
    cap_info.param.channels     = HL_MOD_AUDIO_CHANNELS;
    cap_info.param.sampleRate   = HL_MOD_AUDIO_RATE;
    cap_info.param.sampleBits   = HL_MOD_AUDIO_BITS;
 
    cap_info.abuf.period_size   = HL_MOD_AUDIO_PERIOD_SIZE;
    cap_info.abuf.buf_size      = HL_MOD_AUDIO_PERIOD_SIZE * HL_MOD_AUDIO_PERIOD_COUNT;
    cap_info.abuf.buf           = rt_malloc_uncache(cap_info.abuf.buf_size * cap_info.param.channels * (cap_info.param.sampleBits >> 3));
    if (cap_info.abuf.buf == RT_NULL) {
        LOG_E("cap_info.abuf.buf malloc failed!");
        return RT_ENOMEM;
    }

    /* 播放声卡配置 */
    strncpy(play_info.card_name, HL_PLAY_DEV, 8);
    play_info.card_type        = HL_PLAY_CARD;
    play_info.param.channels   = HL_MOD_AUDIO_CHANNELS;
    play_info.param.sampleRate = HL_MOD_AUDIO_RATE;
    play_info.param.sampleBits = HL_MOD_AUDIO_BITS;

    play_info.abuf.period_size = HL_MOD_AUDIO_PERIOD_SIZE;
    play_info.abuf.buf_size    = HL_MOD_AUDIO_PERIOD_SIZE * HL_MOD_AUDIO_PERIOD_COUNT;
    play_info.abuf.buf         = rt_malloc_uncache(play_info.abuf.buf_size * play_info.param.channels * (play_info.param.sampleBits >> 3));
    if (play_info.abuf.buf == RT_NULL) {
        LOG_E("play_info.abuf.buf malloc failed!");
        rt_free_uncache(cap_info.abuf.buf);
        return RT_ENOMEM;
    }

    LOG_D("audio codec config succeed! dev is %d", HL_IS_TX_DEVICE());
    return RT_EOK;
}

// 声卡内存释放和参数去配置
static void hl_mod_audio_param_deconfig(void)
{
    rt_free_uncache(cap_info.abuf.buf);
    rt_free_uncache(play_info.abuf.buf);
    LOG_D("audio codec deconfig!");
}

// uac声卡内存申请
static rt_err_t hl_mod_audio_uac_config(void)
{
    rt_err_t ret;

    uac_info.card = rt_device_find("uacd");
    if (uac_info.card == RT_NULL) {
        LOG_E("not find uac device");
        goto err0;
    }

    ret = rt_device_open(uac_info.card, RT_DEVICE_OFLAG_RDWR);
    if (ret < 0) {
        LOG_E("Failed to open %s, err: %d", uac_info.card->parent.name, ret);
        goto err0;
    }

    rt_device_control(uac_info.card, HL_GET_UAC_MB_CMD, &uac_info.p_mailbox);
    if (uac_info.p_mailbox == RT_NULL) {
        LOG_E("Failed to get uac mailbox handle");
        goto err1;
    }

    uac_info.buff32size = HL_MOD_AUDIO_PERIOD_SIZE*HL_MOD_AUDIO_CHANNELS * 4; 
    uac_info.buff24size = HL_MOD_AUDIO_PERIOD_SIZE*HL_MOD_AUDIO_CHANNELS * 3;

    uac_info.buff32b    = rt_malloc(uac_info.buff32size);
    if (RT_NULL == uac_info.buff32b) {
        LOG_E("uac_info.buff32b malloc failed!");
        goto err1;
    }

    uac_info.buff24b    = rt_malloc(uac_info.buff24size);
    if (RT_NULL == uac_info.buff24b) {
        LOG_E("uac_info.buff24b malloc failed!");
        goto err2;
    }

    LOG_I("uac param config succeed!");
    return RT_EOK;

err2:
    rt_free(uac_info.buff32b);
err1:
    rt_device_close(uac_info.card);
err0:
    return RT_ERROR;
}

// uac声卡内存释放
static rt_err_t hl_mod_audio_uac_deconfig(void)
{
    rt_free(uac_info.buff24b);
    rt_free(uac_info.buff32b);
    rt_device_close(uac_info.card);

    LOG_I("uac param deconfig succeed!");
    return RT_EOK;
}

#if HL_IS_TX_DEVICE()
// 录制内存申请
static rt_err_t hl_mod_audio_record_param_config(void)
{
    record_info.record_after_rb = rt_ringbuffer_create(dsp_config->buffer_size_b24_1ch * 65);
    if (record_info.record_after_rb  == RT_NULL) {
        LOG_E("record_info.record_after_rb create failed!!!");
        goto err0;
    }

    record_info.record_bypass_rb = rt_ringbuffer_create(dsp_config->buffer_size_b24_1ch * 65);
    if (record_info.record_bypass_rb  == RT_NULL) {
        LOG_E("record_info.record_bypass_rb create failed!!!");
        goto err1;
    }

    return RT_EOK;

err1:
    rt_ringbuffer_destroy(record_info.record_after_rb);
err0:
    return RT_ERROR;
}
#endif

// 录制内存释放
static rt_err_t hl_mod_audio_record_param_deconfig(void)
{
    rt_ringbuffer_destroy(record_info.record_bypass_rb);
    rt_ringbuffer_destroy(record_info.record_after_rb);
    return RT_EOK;
}

// 声卡配置
static rt_err_t hl_mod_audio_codec_config(hl_card_param_t *p_param)
{
    rt_err_t            ret;

    p_param->card = rt_device_find(p_param->card_name);
    if (p_param->card == RT_NULL) {
        LOG_E("not find %d audio card", p_param->card_name);
        goto err0;
    }

    if (HL_PLAY_CARD == p_param->card_type) {
        ret = rt_device_open(p_param->card, RT_DEVICE_OFLAG_WRONLY);
    } else {
        ret = rt_device_open(p_param->card, RT_DEVICE_OFLAG_RDONLY);
    }
    if (ret < 0) {
        LOG_E("Failed to open %s, err: %d", p_param->card->parent.name, ret);
        goto err0;
    }

    ret = rt_device_control(p_param->card, RK_AUDIO_CTL_PCM_PREPARE, &p_param->abuf);
    if(ret != RT_EOK) {
        LOG_E("cap set pcm prepare failed! err: %d", ret);
        goto err1;
    }

    ret = rt_device_control(p_param->card, RK_AUDIO_CTL_HW_PARAMS, &p_param->param);
    if(ret != RT_EOK) {
        LOG_E("cap set hardware params failed! err: %d", ret);
        goto err2;
    }

    LOG_I("open card:%s %d channel  %d hz  %d bits", p_param->card_name, p_param->param.channels, p_param->param.sampleRate, p_param->param.sampleBits);
    LOG_I("audio card param deconfig succeed!");
    return RT_EOK;

err2:
    rt_device_control(p_param->card, RK_AUDIO_CTL_STOP, NULL);
    rt_device_control(p_param->card, RK_AUDIO_CTL_PCM_RELEASE, NULL);
err1:
    rt_device_close(p_param->card);
err0:
    return RT_ERROR;
}

// 声卡去配置
static rt_err_t hl_mod_audio_codec_deconfig(hl_card_param_t *p_param)
{
    rt_device_control(p_param->card, RK_AUDIO_CTL_STOP, NULL);
    rt_device_control(p_param->card, RK_AUDIO_CTL_PCM_RELEASE, NULL);
    rt_device_close(p_param->card);

    LOG_I("audio card param deconfig succeed!");
    return RT_EOK;
}

// dsp内存申请、参数配置和初始化
static rt_err_t hl_mod_audio_dsp_config(void)
{
    dsp_config               = (hl_drv_rk_xtensa_dsp_config_t_p)rkdsp_malloc(sizeof(hl_drv_rk_xtensa_dsp_config_t));
    if (RT_NULL == dsp_config) {
        LOG_D("dsp_config rkdsp_malloc failed!");
        goto err0;
    }

    dsp_config->bits         = 32;
    dsp_config->channels     = 2;
    dsp_config->period_size  = HL_MOD_AUDIO_PERIOD_SIZE;
    dsp_config->rate         = 48000;
    dsp_config->process_size = dsp_config->period_size * dsp_config->channels * (dsp_config->bits >> 3);
    dsp_config->buffer_size_b32_2ch              = dsp_config->process_size;
    dsp_config->audio_process_in_buffer_b32_2ch  = rkdsp_malloc(dsp_config->buffer_size_b32_2ch + 2);
    if (RT_NULL == dsp_config->audio_process_in_buffer_b32_2ch) {
        LOG_D("audio_process_in_buffer_b32_2ch rkdsp_malloc failed!");
        goto err1;
    }
    dsp_config->audio_process_out_buffer_b32_2ch = rkdsp_malloc(dsp_config->buffer_size_b32_2ch + 2);
    if (RT_NULL == dsp_config->audio_process_out_buffer_b32_2ch) {
        LOG_D("audio_process_out_buffer_b32_2ch rkdsp_malloc failed!");
        goto err2;
    }

#if HL_IS_TX_DEVICE()
    dsp_config->buffer_size_b24_1ch                     = dsp_config->period_size * 1 * 3;
    dsp_config->audio_after_process_out_buffer_b24_1ch  = rkdsp_malloc(dsp_config->buffer_size_b24_1ch + 2);
    if (RT_NULL == dsp_config->audio_after_process_out_buffer_b24_1ch) {
        LOG_D("audio_after_process_out_buffer_b24_1ch rkdsp_malloc failed!");
        goto err3;
    }
    dsp_config->audio_before_process_out_buffer_b24_1ch = rkdsp_malloc(dsp_config->buffer_size_b24_1ch + 2);
    if (RT_NULL == dsp_config->audio_before_process_out_buffer_b24_1ch) {
        LOG_D("audio_before_process_out_buffer_b24_1ch rkdsp_malloc failed!");
        goto err4;
    }
#else
    dsp_config->buffer_size_b24_2ch                     = dsp_config->period_size * 2 * 3;
    dsp_config->audio_after_process_out_buffer_b24_2ch  = rkdsp_malloc(dsp_config->buffer_size_b24_2ch + 2);
    if (RT_NULL == dsp_config->audio_after_process_out_buffer_b24_2ch) {
        LOG_D("audio_after_process_out_buffer_b24_2ch rkdsp_malloc failed!");
        goto err3;
    }
    dsp_config->audio_before_process_out_buffer_b24_2ch = rkdsp_malloc(dsp_config->buffer_size_b24_2ch + 2);
    if (RT_NULL == dsp_config->audio_before_process_out_buffer_b24_2ch) {
        LOG_D("audio_before_process_out_buffer_b24_2ch rkdsp_malloc failed!");
        goto err4;
    }
#endif
    if (0 != hl_drv_rk_xtensa_dsp_init()) {
        goto err5;
    }
    hl_drv_rk_xtensa_dsp_io_ctrl(HL_EM_DRV_RK_DSP_CMD_SET_CONFIG, dsp_config, sizeof(hl_drv_rk_xtensa_dsp_config_t));
    hl_drv_rk_xtensa_dsp_io_ctrl(HL_EM_DRV_RK_DSP_CMD_START_DSP, NULL, 0);

    LOG_D("audio dsp config succeed!");
    return RT_EOK;

#if HL_IS_TX_DEVICE()
err5:
    rkdsp_free(dsp_config->audio_before_process_out_buffer_b24_1ch);
err4:
    rkdsp_free(dsp_config->audio_after_process_out_buffer_b24_1ch);
#else
err5:
    rkdsp_free(dsp_config->audio_before_process_out_buffer_b24_2ch);
err4:
    rkdsp_free(dsp_config->audio_after_process_out_buffer_b24_2ch);
#endif

err3:
    rkdsp_free(dsp_config->audio_process_out_buffer_b32_2ch);
err2:
    rkdsp_free(dsp_config->audio_process_in_buffer_b32_2ch);
err1:
    rkdsp_free(dsp_config);
err0:
    return RT_ERROR;
}

// dsp内存释放和去初始化
static rt_err_t hl_mod_audio_dsp_deconfig(void)
{
    hl_drv_rk_xtensa_dsp_deinit();

#if HL_IS_TX_DEVICE()
    rkdsp_free(dsp_config->audio_before_process_out_buffer_b24_1ch);
    rkdsp_free(dsp_config->audio_after_process_out_buffer_b24_1ch);
#else
    rkdsp_free(dsp_config->audio_before_process_out_buffer_b24_2ch);
    rkdsp_free(dsp_config->audio_after_process_out_buffer_b24_2ch);
#endif

    rkdsp_free(dsp_config->audio_process_out_buffer_b32_2ch);
    rkdsp_free(dsp_config->audio_process_in_buffer_b32_2ch);
    rkdsp_free(dsp_config);

    LOG_D("audio dsp deconfig succeed!");
    return RT_EOK;
}

#if HL_IS_TX_DEVICE()
static void do_record_audio(void* arg)
{
    char*    record_buffer;
    uint32_t record_size;
    uint8_t  ter = 0;
    static uint8_t  s_denoise_switch = 0;
    char*    record_buffer1;
    uint32_t record_size1;

    record_size   = dsp_config->buffer_size_b24_1ch * 57;  
    record_buffer = rt_malloc(record_size);
    if (record_buffer == RT_NULL) {
        LOG_E("record_buffer malloc failed");
        goto err0;
    }

    record_size1   = dsp_config->buffer_size_b24_1ch * 57; 
    record_buffer1 = rt_malloc(record_size);
    if (record_buffer1 == RT_NULL) {
        LOG_E("record_buffer1 malloc failed");
        goto err1;
    }
    
    while (1) {
        if ((rt_ringbuffer_data_len(record_info.record_after_rb) < record_size) || (s_record_switch != 1)
            || (rt_ringbuffer_data_len(record_info.record_bypass_rb) < record_size1)) {
            rt_thread_delay(1);
        } else {
            if (rt_ringbuffer_data_len(record_info.record_after_rb) >= record_size) {
                rt_ringbuffer_get(record_info.record_after_rb, record_buffer, record_size);
                hl_mod_audio_record(record_info.file_audio_after, record_buffer, record_size, &s_record_after_size);
            }

            if (rt_ringbuffer_data_len(record_info.record_bypass_rb) >= record_size1) {
                rt_ringbuffer_get(record_info.record_bypass_rb, record_buffer1, record_size1);
                hl_mod_audio_record(record_info.file_audio_bypass, record_buffer1, record_size1, &s_record_bypass_size);
            }
        }
        
        if (s_record_key_flag == 1) {
            s_record_key_flag = 0;
            if (s_record_switch == 0) {
                ter = hl_mod_audio_record_switch(1); 
            } else {
                ter = hl_mod_audio_record_switch(0);  
            }
        }
    }
err1:
    rt_free(record_buffer);
err0:
    return;
}
#endif

#if 0
static struct rt_ringbuffer*           s_record_rb        = RT_NULL;
static        uint8_t                  record_flag        = 0;
static void hl_do_record_audio(void* arg)
{
    char*    record_buffer;
    uint32_t record_size;
    uint8_t  ter = 0;
    static uint8_t  s_denoise_switch = 0;
    char*    record_buffer1;
    uint32_t record_size1;

    record_size   = 2048;  
    record_buffer = rt_malloc(record_size);

    uint32_t s_record_size = 0;

    int record_file = open("hl_record_test.wav", O_WRONLY | O_CREAT | O_TRUNC);
    if (record_file == -1) {
        LOG_E("open hl_record_test.wav failed!");
        return 0;
    }

    hl_mod_audio_record_start(record_file, &s_record_size);

    //s_record_rb = rt_ringbuffer_create(4096);

    record_flag = 0;
    while (record_flag != 2) {
        if (rt_ringbuffer_data_len(s_record_rb) < record_size) {
            rt_thread_delay(1);
        } else {
            if (rt_ringbuffer_data_len(s_record_rb) >= record_size) {
                rt_ringbuffer_get(s_record_rb, record_buffer, record_size);
                hl_mod_audio_record(record_file, record_buffer, record_size, &s_record_size);
            }
        }
    }

    hl_mod_audio_record_stop(record_file, &s_record_size);
    close(record_file);
    record_buffer = rt_malloc(record_size);
    rt_free(record_buffer);
}
#endif

static void _hl_cap2play_thread_entry(void* arg)
{
    LOG_D("audio cap2play thread run");
    while (1) {
        if (rt_device_read(cap_info.card, 0, dsp_config->audio_process_in_buffer_b32_2ch, cap_info.abuf.period_size) <= 0) {
            LOG_E("read %s failed", cap_info.card->parent.name);
            break;
        }

        hl_drv_rk_xtensa_dsp_transfer();

        if (rt_device_write(play_info.card, 0, dsp_config->audio_process_out_buffer_b32_2ch, play_info.abuf.period_size) <= 0) {
            LOG_E("write %s failed", play_info.card->parent.name);
        }

#if HL_IS_TX_DEVICE()
        if (s_record_switch == 1) {
            rt_ringbuffer_put(record_info.record_after_rb, dsp_config->audio_after_process_out_buffer_b24_1ch,
                              dsp_config->buffer_size_b24_1ch);
        }

        if (s_record_switch == 1) {
            rt_ringbuffer_put(record_info.record_bypass_rb, dsp_config->audio_before_process_out_buffer_b24_1ch,
                              dsp_config->buffer_size_b24_1ch);
        }
#endif
    }
}

static void _hl_cap2uac_thread_entry(void* arg)
{
    LOG_D("audio cap2uac thread run");
    while (1) {
        if (rt_device_read(cap_info.card, 0, dsp_config->audio_process_in_buffer_b32_2ch, cap_info.abuf.period_size) <= 0) {
            LOG_E("read %s failed", cap_info.card->parent.name);
            break;
        }

        hl_drv_rk_xtensa_dsp_transfer();
#if (!HL_IS_TX_DEVICE())
        rt_device_write(uac_info.card, 0, dsp_config->audio_after_process_out_buffer_b24_2ch, dsp_config->buffer_size_b24_2ch);
#endif
    }
}

static void _hl_uac2play_thread_entry(void* arg)
{
    char*               p;
    rt_size_t           get_data_size;
    rt_uint8_t          uac_p_state;
    play_uac_state_e    playback_state = HL_PLAY_UAC_IDLE;

    LOG_D("audio uac2play thread run");
    while (1) {
        switch (playback_state) {
            case HL_PLAY_UAC_IDLE:
                memset(uac_info.buff32b, 0x00, uac_info.buff32size);
                playback_state = HL_PLAY_UAC_WAIT;
                LOG_I("enter playback wait");
                break;

            case HL_PLAY_UAC_WAIT:
                rt_device_control(uac_info.card, HL_GET_UAC_P_STATE_CMD, &uac_p_state);
                if (uac_p_state == 0) {
                    break;
                }
                playback_state = HL_PLAY_UAC_GOING;
                LOG_I("p_buffer ready enter playback going");

            case HL_PLAY_UAC_GOING:
                get_data_size = rt_device_read(uac_info.card, 0, uac_info.buff24b, uac_info.buff24size);
                if (get_data_size < uac_info.buff24size) {
                    // 数据不足
                    memset(&uac_info.buff24b[get_data_size], 0x00, uac_info.buff24size - get_data_size);
                    playback_state = HL_PLAY_UAC_IDLE;
                    LOG_I("p_buffer ready enter playback idle");
                }

                p = uac_info.buff32b;
                for (uint32_t i =0; i < uac_info.buff24size; ) {
                    p[0] = 0;
                    p[1] = uac_info.buff24b[i++];
                    p[2] = uac_info.buff24b[i++];
                    p[3] = uac_info.buff24b[i++];
                    p   += 4;
                }
                break;

            case HL_PLAY_UAC_ERR:
                break;

            default:
                break;
        }

        /* 写入无线 */
        if (rt_device_write(play_info.card, 0, uac_info.buff32b, play_info.abuf.period_size) <= 0) {
            LOG_E("write %s failed", play_info.card->parent.name);
        }
    }
}

static void _hl_cap2play2uac_thread_entry(void* arg)
{
    LOG_D("audio cap2play2uac thread run");
    while (1) {
        if (rt_device_read(cap_info.card, 0, dsp_config->audio_process_in_buffer_b32_2ch, cap_info.abuf.period_size) <= 0) {
            LOG_E("read %s failed", cap_info.card->parent.name);
            break;
        }

        hl_drv_rk_xtensa_dsp_transfer();

        if (rt_device_write(play_info.card, 0, dsp_config->audio_process_out_buffer_b32_2ch, play_info.abuf.period_size)<= 0) {
            LOG_E("write %s failed", play_info.card->parent.name);
        }
#if (!HL_IS_TX_DEVICE())
        if (rt_device_write(uac_info.card, 0, dsp_config->audio_after_process_out_buffer_b24_2ch, dsp_config->buffer_size_b24_2ch)<= 0) {
            LOG_E("write %s failed", uac_info.card->parent.name);
        }
#endif
    }
}

// 设置输入声卡增益
static void hl_mod_audio_set_gain(int dB, uint8_t ch)
{
    // int8_t ret = 0;
    // struct AUDIO_DB_CONFIG db_config = {0};

    // db_config.dB = dB;
    // db_config.ch = ch;

    // ret = rt_device_control(play_info.card, RK_AUDIO_CTL_SET_GAIN, &db_config);
    // if (ret != RT_EOK) {
    //     LOG_E("fail to set gain\n");
    //     return -RT_ERROR;
    // }
}

#if HL_IS_TX_DEVICE()
// 设置输入声卡静音状态
static void hl_mod_audio_set_mute(uint8_t mute)
{
    int8_t ret = 0;

    ret = hl_drv_rk_xtensa_dsp_io_ctrl(HL_EM_DRV_RK_DSP_CMD_SET_MUTE, &mute, 1);
    if (ret != RT_EOK) {
        LOG_E("fail to set mute\n");
        return -RT_ERROR;
    }
}
#endif

// 音频流模式设置
static void hl_mod_audio_stream_set(void *ptr) 
{
    hl_stream_mode_e stream_mode = *((hl_stream_mode_e*)ptr);

    switch (stream_mode) {
        case HL_STREAM_IDLE:
            s_stream_mode_next = HL_STREAM_IDLE;
            LOG_I("app set stream stop");
            break;
        
        case HL_STREAM_PDM2PLAY:
            s_stream_mode_next = HL_STREAM_PDM2PLAY;
            LOG_I("app set stream pdm2play");
            break;
        
        case HL_STREAM_CAP2PLAY:
            s_stream_mode_next = HL_STREAM_CAP2PLAY;
            LOG_I("app set stream cap2play");
            break;

        case HL_STREAM_UAC2PLAY:
            s_stream_mode_next = HL_STREAM_UAC2PLAY;
            LOG_I("app set stream uac2play");
            break;

        case HL_STREAM_CAP2UAC_UAC2PLAY:
            s_stream_mode_next = HL_STREAM_CAP2UAC_UAC2PLAY;
            LOG_I("app set stream cap2uac uac2play");
            break;

        case HL_STREAM_CAP2PLAY_CAP2UAC:
            s_stream_mode_next = HL_STREAM_CAP2PLAY_CAP2UAC;
            LOG_I("app set stream cap2play2uac");
            break;

        default:
            LOG_E("err:app stream ctrl(%d)", stream_mode);
            break;
    }
}

// 音频模块发送消息给APP层
static void hl_mod_audio_send_msg(hl_mod_audio_indicate msg_cmd, uint32_t param)
{
    rt_err_t res;
    mode_to_app_msg_t msg;

    msg.sender          = AUDIO_MODE;
    msg.cmd             = msg_cmd;
    msg.len             = 0;
    msg.param.u32_param = param;

    res = rt_mq_send(s_audio_to_app_mq, (void*)(&msg), sizeof(mode_to_app_msg_t));
    if (res == -RT_EFULL) {
        LOG_E("audio to app msgq full");
    } else if (res == -RT_ERROR) {
        LOG_E("audio to app msgq err");
    }
}

// 音频流控制
static void _hl_audio_stream_thread_ctrl(hl_stream_mode_e mode, hl_switch_e state) 
{
    rt_err_t ret;

    if (state == HL_SWITCH_OFF) {
        switch (mode) {
            case HL_STREAM_IDLE:
                hl_mod_audio_codec_config(&cap_info);
                hl_mod_audio_codec_config(&play_info);
                break;
                
            case HL_STREAM_PDM2PLAY:
                hl_mod_audio_codec_deconfig(&cap_info);
                strncpy(cap_info.card_name, HL_CAPTURE_DEV, 8);
                hl_mod_audio_codec_config(&cap_info);
            case HL_STREAM_CAP2PLAY:
                rt_thread_delete(cap2play_thread_id);
                break;

            case HL_STREAM_UAC2PLAY:
                rt_thread_delete(uac2play_thread_id);
                break;

            case HL_STREAM_CAP2UAC_UAC2PLAY:
                rt_thread_delete(uac2play_thread_id);
                rt_thread_delete(cap2uac_thread_id);
                break;

            case HL_STREAM_CAP2PLAY_CAP2UAC:
                rt_thread_delete(cap2play2uac_thread_id);
                break;

            default:
                LOG_E("stream_mode unknow");
                break;
        }
    } else {
        switch (mode) {
            case HL_STREAM_IDLE:
                break;

            case HL_STREAM_PDM2PLAY:
                #if HL_IS_TX_DEVICE()
                    hl_mod_audio_codec_deconfig(&cap_info);
                    strncpy(cap_info.card_name, HL_PDM_CAP_DEV, 8);
                    hl_mod_audio_codec_config(&cap_info);
                #endif

            case HL_STREAM_CAP2PLAY:
                cap2play_thread_id = rt_thread_create("cap2play", _hl_cap2play_thread_entry, RT_NULL, 2048, 0, 3);
                if (cap2play_thread_id != RT_NULL) {
                    rt_thread_startup(cap2play_thread_id);
                } else {
                    LOG_E("cap2play thread create failed!");
                }
                break;

            case HL_STREAM_UAC2PLAY:
                uac2play_thread_id = rt_thread_create("uac2play", _hl_uac2play_thread_entry, RT_NULL, 2048, 0, 3);
                if (uac2play_thread_id != RT_NULL) {
                    rt_thread_startup(uac2play_thread_id);
                } else {
                    LOG_E("uac2play thread create failed!");
                }
                break;

            case HL_STREAM_CAP2UAC_UAC2PLAY:
                cap2uac_thread_id = rt_thread_create("cap2uac", _hl_cap2uac_thread_entry, RT_NULL, 2048, 0, 3);
                if (cap2uac_thread_id != RT_NULL) {
                    rt_thread_startup(cap2uac_thread_id);
                } else {
                    LOG_E("cap2uac thread create failed!");
                }
                uac2play_thread_id = rt_thread_create("uac2play", _hl_uac2play_thread_entry, RT_NULL, 2048, 0, 3);
                if (uac2play_thread_id != RT_NULL) {
                    rt_thread_startup(uac2play_thread_id);
                } else {
                    LOG_E("uac2play thread create failed!");
                }
                break;

            case HL_STREAM_CAP2PLAY_CAP2UAC:
                cap2play2uac_thread_id = rt_thread_create("cap2p2u", _hl_cap2play2uac_thread_entry, RT_NULL, 2048, 0, 3);
                if (cap2play2uac_thread_id != RT_NULL) {
                    rt_thread_startup(cap2play2uac_thread_id);
                } else {
                    LOG_E("cap2play2uac thread create failed!");
                }
                break;

            default:
                LOG_E("stream_mode unknow");
                break;
        }
    }
}

static void _hl_audio_ctrl_thread_entry(void* arg)
{
    rt_err_t            ret;
    rt_uint32_t         msg;

    LOG_D("audio ctrl thread run");
    while (1) {
        msg = 0;
        while (RT_EOK == rt_mb_recv(uac_info.p_mailbox, &msg, 0)) {
            switch (msg) {
                case HL_UAC_LINK_IND:
                    hl_mod_audio_send_msg(HL_AUDIO_UAC_LINK_IND, 0);
                    LOG_D("receive HL_UAC_LINK_IND");
                    break;
                case HL_UAC_SET_P_VOL_IND:
                    LOG_D("receive HL_UAC_SET_P_VOL_IND");
                    break;
                case HL_UAC_SET_C_VOL_IND:
                    LOG_D("receive HL_UAC_SET_C_VOL_IND");
                    break;
                default:
                    LOG_E("receive unknow indicate");
                    break;
            }
        }

        if (s_stream_mode_cur != s_stream_mode_next) {
            _hl_audio_stream_thread_ctrl(s_stream_mode_cur, HL_SWITCH_OFF);
            s_stream_mode_cur = s_stream_mode_next;
            _hl_audio_stream_thread_ctrl(s_stream_mode_cur, HL_SWITCH_ON);
            LOG_I("stream mode change(%d)", s_stream_mode_cur);
            rt_thread_mdelay(10);
        }
        rt_thread_mdelay(10);
    }
}

/* Exported functions --------------------------------------------------------*/
uint8_t hl_mod_audio_init(rt_mq_t* p_msg_handle)
{
    rt_err_t ret;
    uint8_t temp = 0;

    s_audio_to_app_mq = p_msg_handle;
    // elm_init();
#if HL_IS_TX_DEVICE()
    hl_mod_audio_dfs();
    s_record_switch = 0;
    hl_hal_gpio_init(GPIO_MIC_SW);    
    hl_hal_gpio_low(GPIO_MIC_SW);
    hl_drv_rtc_pcf85063_init();
#else
    // hl_hal_gpio_init(GPIO_AMP_EN);
    // hl_hal_gpio_high(GPIO_AMP_EN);
#endif
   
    ret = hl_mod_audio_param_config();
    if (RT_EOK != ret) {
        LOG_E("hl_mod_audio_param_config failed");
        goto err0;
    }

    ret = hl_mod_audio_dsp_config();
    if (RT_EOK != ret) {
        LOG_E("hl_mod_audio_dsp_config failed");
        goto err1;
    }

    ret = hl_mod_audio_uac_config();
    if (RT_EOK != ret) {
        LOG_E("hl_mod_audio_uac_config failed");
        goto err2;
    }

    rt_usbd_msc_state_register(mstorage_switch_cb);

#if HL_IS_TX_DEVICE()
    ret = hl_mod_audio_record_param_config();
    if (RT_EOK != ret) {
        LOG_E("hl_mod_audio_record_param_config failed");
        goto err3;
    }

    record_thread_id = rt_thread_create("record_after", do_record_audio, RT_NULL, 2048, 16, 1);
    if (record_thread_id != RT_NULL) {
        rt_thread_startup(record_thread_id);
    } else {
        LOG_E("audio record thread create failed");
        goto err4;
    }
#endif

    audio_ctrl_thread_id   = rt_thread_create("au_ctrl", _hl_audio_ctrl_thread_entry, RT_NULL, 512, 10, 3);
    if (audio_ctrl_thread_id != RT_NULL) {
        rt_thread_startup(audio_ctrl_thread_id);
    } else {
        LOG_E("audio ctrl thread create failed");
        goto err5;
    }

    return RT_EOK;

err5:
#if HL_IS_TX_DEVICE()
    rt_thread_delete(record_thread_id);
err4:
    hl_mod_audio_record_param_deconfig();
err3:
#endif
    hl_mod_audio_uac_deconfig();
err2:
    hl_mod_audio_dsp_deconfig();
err1:
    hl_mod_audio_param_deconfig();
err0:
    return RT_ERROR;
}

uint8_t hl_mod_audio_deinit(void)
{
#if HL_IS_TX_DEVICE()
    hl_mod_audio_record_switch(0);
#endif

    hl_mod_audio_codec_deconfig(&cap_info);
    hl_mod_audio_codec_deconfig(&play_info);

    rt_thread_delete(audio_ctrl_thread_id);

    s_stream_mode_next = HL_STREAM_IDLE;
    if (s_stream_mode_cur != s_stream_mode_next) {
        _hl_audio_stream_thread_ctrl(s_stream_mode_cur, HL_SWITCH_OFF);
        s_stream_mode_cur = s_stream_mode_next;
    }

#if HL_IS_TX_DEVICE()
    rt_thread_delete(record_thread_id);
    hl_mod_audio_record_param_deconfig();
#endif
    hl_mod_audio_uac_deconfig();
    hl_mod_audio_dsp_deconfig();
    hl_mod_audio_param_deconfig();

    return 0;
}

void hl_mod_audio_show_info(void)
{
#if HL_IS_TX_DEVICE()
    // TBD
#else
    // TBD
#endif
}

#if HL_IS_TX_DEVICE()
uint8_t hl_mod_audio_io_ctrl(hl_mod_audio_ctrl_cmd cmd, void* ptr, uint16_t len)
{
    int8_t ret = 0;

    switch (cmd) {
        case HL_AUDIO_GET_INFO_CMD:
            hl_mod_audio_show_info();
            break;
        case HL_AUDIO_SET_TIME_CMD:
            break;
        case HL_AUDIO_GET_TIME_CMD:
            break;
        case HL_AUDIO_SET_DENOISE_CMD:
            if (ptr == NULL) {
                LOG_E("HL_MOD_AUDIO_SET_DENOISE_CMD parem error");
                return -1;
            }
            hl_drv_rk_xtensa_dsp_io_ctrl(HL_EM_DRV_RK_DSP_CMD_DENOISE_DSP, ptr, 1);
            break;
        case HL_AUDIO_SET_GAIN_CMD:
            hl_mod_audio_set_gain(((int *)ptr)[0], 0x55);
            break;
        case HL_AUDIO_SET_MUTE_CMD:
            if(((char*)ptr)[0] != 0) {
                hl_mod_audio_set_mute(1);
                LOG_I("[%s][line:%d] mic open mute !!!\r\n", __FUNCTION__, __LINE__);
            } else {
                hl_mod_audio_set_mute(0);
                LOG_I("[%s][line:%d] mic close mute !!!\r\n", __FUNCTION__, __LINE__);
            }
            
            
            break;
        case HL_AUDIO_SET_EQ_CMD:
            break;
        case HL_AUDIO_RECORD_CMD:
            if (ptr == NULL) {
                LOG_E("HL_MOD_AUDIO_RECORD_CMD parem error");
                return -1;
            }
            return hl_mod_audio_record_switch(((char*)ptr)[0]);
            break;

        case HL_AUDIO_MIC_SWITCH_CMD:
            if (ptr == NULL) {
                LOG_E("HL_MOD_AUDIO_RECORD_CMD parem error");
                return -1;
            }
            if(((char*)ptr)[0] != 0) {
                hl_hal_gpio_high(GPIO_MIC_SW);
                LOG_I("[%s][line:%d] external mic!!!\r\n", __FUNCTION__, __LINE__);
            } else {
                hl_hal_gpio_low(GPIO_MIC_SW);
                LOG_I("[%s][line:%d] Internal mic!!!\r\n", __FUNCTION__, __LINE__);
            }
            break;

        case HL_AUDIO_STREAM_SET_CMD:
            hl_mod_audio_stream_set(ptr);
            break;
        case HL_USB_MSTORAGE_DISABLE_CMD:
            rt_usbd_msc_disable();
            break;

        default:
            LOG_E("audio_io_ctrl cmd(%d) error!!! \r\n", cmd);
            break;
    }

    return 0;
}
#else
uint8_t hl_mod_audio_io_ctrl(hl_mod_audio_ctrl_cmd cmd, void* ptr, uint16_t len)
{
    int8_t ret = 0;

    switch (cmd) {
        case HL_AUDIO_GET_INFO_CMD:
            hl_mod_audio_show_info();
            break;

        case HL_AUDIO_SET_GAIN_CMD:
            hl_mod_audio_set_gain(((int *)ptr)[0], 0x55);
            break;

        case HL_AUDIO_SET_HP_AMP_CMD:
            if(((char*)ptr)[0] != 0) {
                // hl_hal_gpio_high(GPIO_AMP_EN);
                LOG_I("enable hp amp!!!");
            } else {
                // hl_hal_gpio_low(GPIO_AMP_EN);
                LOG_I("disable hp amp!!!");
            }
            break;

        case HL_AUDIO_STREAM_SET_CMD:
            hl_mod_audio_stream_set(ptr);
            break;
            
        case HL_USB_MSTORAGE_DISABLE_CMD:
            rt_usbd_msc_disable();
            break;

        default:
            LOG_E("audio_io_ctrl cmd(%d) error!!!", cmd);
            break;
    }

    return 0;
}
#endif

// hl_mod_audio_test [cmd ] [param] [size]
int hl_mod_audio_test(int argc, char** argv)
{
    hl_mod_audio_ctrl_cmd  cmd;
    uint8_t                u8_data;
    uint16_t               u16_data;
    uint32_t               u32_data;
    uint16_t               size;
    char                   *endptr;         

    if (argc != 4) {
        LOG_E("wrong parameter, please type: hl_mod_audio_test [cmd] [param] [size]");
        return 0;
    }

    cmd = strtoul(argv[1], &endptr, 0);
    if (*endptr) {
        LOG_E("Bad <cmd> value '%s'", argv[1]);
        return 0;
    }

    size = strtoul(argv[3], &endptr, 0);
    if (*endptr) {
        LOG_E("Bad <size> value '%s'", argv[3]);
        return 0;
    }

    switch (size) {
        case 0x01:
            u8_data = strtoul(argv[2], &endptr, 0);
            if (*endptr) {
                LOG_E("Bad <data> value '%s'", argv[2]);
                return 0;
            }
            hl_mod_audio_io_ctrl(cmd, &u8_data, size);
            break;

        case 0x02:
            u16_data = strtoul(argv[2], &endptr, 0);
            if (*endptr) {
                LOG_E("Bad <data> value '%s'", argv[2]);
                return 0;
            }
            hl_mod_audio_io_ctrl(cmd, &u16_data, size);
            break;

        case 0x04:
            u32_data = strtoul(argv[2], &endptr, 0);
            if (*endptr) {
                LOG_E("Bad <data> value '%s'", argv[2]);
                return 0;
            }
            hl_mod_audio_io_ctrl(cmd, &u32_data, size);
            break;
#if HL_IS_TX_DEVICE()    
        case 0x05:
            hl_mod_audio_rtc_get(NULL);
            break;

        case 0x06:
            rt_usbd_msc_disable(); //hl_mod_audio_dfs();
            break;
        case 0x07:
            hl_mod_audio_set_mute(1);
            break;
        case 0x08:
            hl_mod_audio_set_mute(0);
            break;            
#endif
        default:
            LOG_E("Bad <size> value '%s'", argv[3]);
            break;
    }

    return 0;
}

#ifdef RT_USING_FINSH

MSH_CMD_EXPORT(hl_mod_audio_test, audio io ctrl cmd);

#endif

// INIT_APP_EXPORT(hl_mod_audio_init);

#endif  // HL_MOD_AUDIO_C
        /*
 * EOF
 */