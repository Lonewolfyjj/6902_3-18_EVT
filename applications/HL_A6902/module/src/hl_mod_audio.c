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
#include "hl_drv_rk_xtensa_dsp.h"
#include "hl_config.h"

#include "hl_drv_aw21009.h"
#include "hl_hal_gpio.h"
/* typedef -------------------------------------------------------------------*/

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

struct capture_config
{
    int*              file_audio_after;  //FILE
    int*              file_audio_bypass;
    struct rt_device* card;
    uint32_t          rate;
    uint32_t          channels;
    uint32_t          bits;
    uint32_t          period_size;
    uint32_t          period_count;
    uint32_t          total_frames;
};

struct playback_config
{
    FILE*             file;
    struct rt_device* card;
    uint32_t          rate;
    uint32_t          channels;
    uint32_t          bits;
    uint32_t          period_size;
    uint32_t          period_count;
    uint32_t          total_frames;
};

/* define --------------------------------------------------------------------*/

#define HL_MOD_AUDIO_RATE (48000)
/// 音频接口传输的是32bit数据，但实际处理的是24bit数据
#define HL_MOD_AUDIO_BITS (32)  /// 24BIT
#define HL_MOD_AUDIO_CHANNELS (2)
#define HL_MOD_AUDIO_PERIOD_SIZE (96) //(120)
#define HL_MOD_AUDIO_PERIOD_COUNT (4)
#define HL_MOD_AUDIO_FRAME_SIZE (HL_MOD_AUDIO_PERIOD_SIZE * HL_MOD_AUDIO_CHANNELS * 3)
#define HL_MOD_AUDIO_RECORD_RING_BUFFER_SIZE (HL_MOD_AUDIO_FRAME_SIZE * 30)  //((HL_MOD_AUDIO_FRAME_SIZE * 30) + 5)

#if HL_GET_DEVICE_TYPE()
#define HL_MOD_AUDIO_DEFAULT_DEVICE_PLAY "wifip"
#define HL_MOD_AUDIO_DEFAULT_DEVICE_CAPTURE "codecc"
#else
#define HL_MOD_AUDIO_DEFAULT_DEVICE_PLAY "codecp"
#define HL_MOD_AUDIO_DEFAULT_DEVICE_CAPTURE "wific"
#endif

#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT 0x20746d66
#define ID_DATA 0x61746164

#define FORMAT_PCM 1
/* variables -----------------------------------------------------------------*/
struct playback_config*                ply_config;
struct capture_config*                 cap_config;
static hl_drv_rk_xtensa_dsp_config_t_p dsp_config                = NULL;
static char                            card_play[RT_NAME_MAX]    = { 0 };
static char                            card_capture[RT_NAME_MAX] = { 0 };
struct wav_header                      s_audio_header            = { 0 };
static struct rt_ringbuffer*           s_audio_rb                = RT_NULL;
static char                            s_audio_switch            = 0;

#if HL_GET_DEVICE_TYPE()
static char                            s_record_switch           = 0;
static struct rt_ringbuffer*           s_record_after_rb         = RT_NULL;
static struct rt_ringbuffer*           s_record_bypass_rb        = RT_NULL;
static uint32_t                        s_record_after_size       = 0;
static uint32_t                        s_record_bypass_size      = 0;
#endif

/* Private function(only *.c)  -----------------------------------------------*/

#if HL_GET_DEVICE_TYPE()
static void hl_mod_audio_record(int p_file_audio, uint8_t* buffer, uint32_t size, uint32_t* s_record_size);
static void hl_mod_audio_record_save(int p_file_audio, char* file_name, uint32_t* s_record_size);
static void hl_mod_audio_record_stop(int p_file_audio, uint32_t* s_record_size);
static void hl_mod_audio_record_start(int p_file_audio, uint32_t* s_record_size);
#endif
static void hl_mod_audio_codec_config(void);
static void hl_mod_audio_dsp_config(void);
static void do_read_audio(void* arg);
static void do_record_audio(void* arg);
static int  hl_mod_audio_record_switch(uint8_t record_switch);

static uint8_t s_record_key_flag = 0;

#if HL_GET_DEVICE_TYPE()
static void hl_hal_gpio_audio_record_irq_process(void* args)
{
    s_record_key_flag = 1;
}

static void _hl_drv_key_init(void)
{
    static hl_gpio_pin_e hl_audio_record_key = 0;

    hl_audio_record_key = GPIO_REC_KEY;
    hl_hal_gpio_init(GPIO_REC_KEY);
    hl_hal_gpio_attach_irq(GPIO_REC_KEY, PIN_IRQ_MODE_FALLING, hl_hal_gpio_audio_record_irq_process,
                           &hl_audio_record_key);
    hl_hal_gpio_irq_enable(GPIO_REC_KEY, PIN_IRQ_ENABLE);
}
/* __HL_HAL_LED_C__ */


static void hl_mod_audio_record(int p_file_audio, uint8_t* buffer, uint32_t size, uint32_t* s_record_size)
{
    if (p_file_audio == NULL) {
        rt_kprintf("Error record file no open\n");
        return;
    }

    if (write(p_file_audio, buffer, size) != size) {  
        rt_kprintf("Error capturing sample (write)\n");
        return;
    }
    *s_record_size = (size / 1 / (3)) + *s_record_size; // 只录制一个通道(1)
}

static void hl_mod_audio_record_stop(int p_file_audio, uint32_t* s_record_size)
{
    s_audio_header.data_sz = (*s_record_size) * s_audio_header.block_align;
    s_audio_header.riff_sz = s_audio_header.data_sz + sizeof(s_audio_header) - 8;

    lseek(p_file_audio, 0, SEEK_SET);
    write(p_file_audio, &s_audio_header, sizeof(struct wav_header));
    close(p_file_audio);

    rt_kprintf("Auaio recoord stop (data_sz:(0x%08x),riff_sz:(0x%08x)) \n", s_audio_header.data_sz,
               s_audio_header.riff_sz);
}

static void hl_mod_audio_record_save(int p_file_audio, char* file_name, uint32_t* s_record_size)
{
    close(p_file_audio);

    p_file_audio = open(file_name, O_WRONLY | O_CREAT);
    if (!p_file_audio) {
        rt_kprintf("%s:Unable to create file '%s'\n", __func__, file_name);
        hl_mod_audio_record_stop(p_file_audio, s_record_size);
        return;
    }
}

static void hl_mod_audio_record_start(int p_file_audio, uint32_t* s_record_size)
{
    if (p_file_audio == -1) {
        rt_kprintf("%s:Unable to create file \n", __func__);
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
    s_audio_header.num_channels = cap_config->channels;
    s_audio_header.sample_rate  = cap_config->rate;

    s_audio_header.bits_per_sample = 24;  //cap_config->bits;
    s_audio_header.byte_rate       = (s_audio_header.bits_per_sample / 8) * cap_config->channels * cap_config->rate;
    s_audio_header.block_align     = cap_config->channels * (s_audio_header.bits_per_sample / 8);
    s_audio_header.data_id         = ID_DATA;

    lseek(p_file_audio, 0, SEEK_SET);
    write(p_file_audio, &s_audio_header, sizeof(struct wav_header));

    /* leave enough room for header */
    lseek(p_file_audio, sizeof(struct wav_header), SEEK_SET);

    //hl_mod_audio_record_save();
}

/**
 * 
 * @brief 
 * @param [in] record_switch 
 * @return int 
 * @date 2022-10-17
 * @author lixiang (rd37@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>lixiang     <td>新建
 * </table>
 */
static int hl_mod_audio_record_switch(uint8_t record_switch)
{
    if (s_record_switch == record_switch) {
        return -1;
    }

    if (record_switch) {
        rt_ringbuffer_reset(s_record_after_rb);
        rt_ringbuffer_reset(s_record_bypass_rb);

        cap_config->file_audio_after  = open("/mnt/sdcard/hl_audio_after.wav", O_WRONLY | O_CREAT | O_TRUNC);
        cap_config->file_audio_bypass = open("/mnt/sdcard/hl_audio_bypass.wav", O_WRONLY | O_CREAT | O_TRUNC);

        hl_mod_audio_record_start(cap_config->file_audio_after, &s_record_after_size);
        hl_mod_audio_record_start(cap_config->file_audio_bypass, &s_record_bypass_size);
        s_record_switch = 1;
        rt_kprintf("Audio mod record start\r\n");
    } else {
        s_record_switch = 0;
        hl_mod_audio_record_stop(cap_config->file_audio_after, &s_record_after_size);
        hl_mod_audio_record_stop(cap_config->file_audio_bypass, &s_record_bypass_size);
        rt_kprintf("Audio mod record stop\r\n");
    }
    return 0;
}

static int hl_drv_aw21009_test(bool led_switch)  //manual模式
{
    bool                                flag;
    hl_drv_aw21009_pattern_mode_e       pat_mode;
    hl_drv_aw21009_auto_breath_param_st auto_breath;
    hl_drv_aw21009_led_light_st         light;
    hl_drv_aw21009_led_group_e          led_group;

    hl_drv_aw21009_init();

    flag = false;

    hl_drv_aw21009_ctrl(1, HL_DRV_AW21009_SET_GROUP_CTRL_DISABLE, &flag, sizeof(flag));

    led_group = HL_DRV_AW21009_LED_GROUP_1 | HL_DRV_AW21009_LED_GROUP_2 | HL_DRV_AW21009_LED_GROUP_3;

    hl_drv_aw21009_ctrl(1, HL_DRV_AW21009_SET_LED_GROUP_MODE, &led_group, sizeof(led_group));

    auto_breath.rise_time    = HL_DRV_AW21009_PAT_TIME_0S38;
    auto_breath.on_time      = HL_DRV_AW21009_PAT_TIME_0S38;
    auto_breath.fall_time    = HL_DRV_AW21009_PAT_TIME_0S38;
    auto_breath.off_time     = HL_DRV_AW21009_PAT_TIME_0S00;
    auto_breath.repeat_times = 0;
    auto_breath.start_point  = HL_DRV_AW21009_START_POINT_T0;
    auto_breath.stop_point   = HL_DRV_AW21009_STOP_POINT_T1;

    hl_drv_aw21009_ctrl(1, HL_DRV_AW21009_SET_AUTO_BREATH_PARAM, &auto_breath, sizeof(auto_breath));

    light.r          = 100;
    light.g          = 100;
    light.b          = 100;
    light.brightness = 0xFF00;

    hl_drv_aw21009_ctrl(1, HL_DRV_AW21009_SET_LED_GROUP_LIGHT_EFFECT, &light, sizeof(light));

    pat_mode = HL_DRV_AW21009_PAT_MANUAL_MODE;

    hl_drv_aw21009_ctrl(1, HL_DRV_AW21009_SET_PATTERN_MODE, &pat_mode, sizeof(pat_mode));

    flag = 1;

    hl_drv_aw21009_ctrl(1, HL_DRV_AW21009_MANUAL_SET_RAMP, &flag, sizeof(flag));

    flag = led_switch;

    hl_drv_aw21009_ctrl(1, HL_DRV_AW21009_MANUAL_SET_SWITCH, &flag, sizeof(flag));

    return 0;
}
#endif 

static void hl_mod_audio_codec_config(void)
{
    /* 输入音频设备配置 */
    cap_config = rt_malloc(sizeof(struct capture_config));

    cap_config->rate         = HL_MOD_AUDIO_RATE;
    cap_config->bits         = HL_MOD_AUDIO_BITS;  //24;
    cap_config->channels     = HL_MOD_AUDIO_CHANNELS;
    cap_config->period_size  = HL_MOD_AUDIO_PERIOD_SIZE;
    cap_config->period_count = HL_MOD_AUDIO_PERIOD_COUNT;
    if (card_capture[0] == 0x00) {
        cap_config->card = rt_device_find(HL_MOD_AUDIO_DEFAULT_DEVICE_CAPTURE);
    } else {
        cap_config->card = rt_device_find(card_capture);
    }

    /* 输出音频设备配置 */
    ply_config = rt_malloc(sizeof(struct playback_config));

    ply_config->rate         = HL_MOD_AUDIO_RATE;
    ply_config->bits         = HL_MOD_AUDIO_BITS;  //24;
    ply_config->channels     = HL_MOD_AUDIO_CHANNELS;
    ply_config->period_size  = HL_MOD_AUDIO_PERIOD_SIZE;
    ply_config->period_count = HL_MOD_AUDIO_PERIOD_COUNT;
    if (card_play[0] == 0x00) {
        ply_config->card = rt_device_find(HL_MOD_AUDIO_DEFAULT_DEVICE_PLAY);
    } else {
        ply_config->card = rt_device_find(card_play);
    }

    rt_kprintf("[%d]: audio codec config succeed! , dev is %d\n", __LINE__, HL_GET_DEVICE_TYPE());
}

static void hl_mod_audio_dsp_config(void)
{
    dsp_config               = (hl_drv_rk_xtensa_dsp_config_t_p)rkdsp_malloc(sizeof(hl_drv_rk_xtensa_dsp_config_t));
    dsp_config->bits         = 32;
    dsp_config->channels     = 2;
    dsp_config->period_size  = HL_MOD_AUDIO_PERIOD_SIZE;
    dsp_config->rate         = 48000;
    dsp_config->process_size = dsp_config->period_size * dsp_config->channels * (dsp_config->bits >> 3);
    dsp_config->buffer_size_b32_2ch              = dsp_config->process_size;
    dsp_config->audio_process_in_buffer_b32_2ch  = rkdsp_malloc(dsp_config->buffer_size_b32_2ch + 2);
    dsp_config->audio_process_out_buffer_b32_2ch = rkdsp_malloc(dsp_config->buffer_size_b32_2ch + 2);
#if HL_GET_DEVICE_TYPE()
    dsp_config->buffer_size_b24_1ch                     = dsp_config->period_size * 1 * 3;
    dsp_config->audio_after_process_out_buffer_b24_1ch  = rkdsp_malloc(dsp_config->buffer_size_b24_1ch + 2);
    dsp_config->audio_before_process_out_buffer_b24_1ch = rkdsp_malloc(dsp_config->buffer_size_b24_1ch + 2);
#else
    dsp_config->buffer_size_b24_2ch                     = dsp_config->period_size * 2 * 3;
    dsp_config->audio_after_process_out_buffer_b24_2ch  = rkdsp_malloc(dsp_config->buffer_size_b24_2ch + 2);
    dsp_config->audio_before_process_out_buffer_b24_2ch = rkdsp_malloc(dsp_config->buffer_size_b24_2ch + 2);
#endif
    hl_drv_rk_xtensa_dsp_init();
    hl_drv_rk_xtensa_dsp_io_ctrl(HL_EM_DRV_RK_DSP_CMD_SET_CONFIG, dsp_config, sizeof(hl_drv_rk_xtensa_dsp_config_t));
    hl_drv_rk_xtensa_dsp_io_ctrl(HL_EM_DRV_RK_DSP_CMD_START_DSP, NULL, 0);

    rt_kprintf("[%d]: audio dsp config succeed! \n", __LINE__);
}

#if HL_GET_DEVICE_TYPE()
static void do_record_audio(void* arg)
{
    char*    record_buffer;
    uint32_t record_size;
    uint8_t  ter = 0;
    static uint8_t  s_denoise_switch = 0;
    char*    record_buffer1;
    uint32_t record_size1;

    record_size   = dsp_config->buffer_size_b24_1ch * 10;  
    record_buffer = rt_malloc(record_size);

    record_size1   = dsp_config->buffer_size_b24_1ch * 10; 
    record_buffer1 = rt_malloc(record_size);
    
    while (1) {
        if ((rt_ringbuffer_data_len(s_record_after_rb) < record_size) || (s_record_switch != 1)
            || (rt_ringbuffer_data_len(s_record_bypass_rb) < record_size)) {
            rt_thread_delay(1);
        } else {
            if (rt_ringbuffer_data_len(s_record_after_rb) >= record_size) {
                rt_ringbuffer_get(s_record_after_rb, record_buffer, record_size);
                hl_mod_audio_record(cap_config->file_audio_after, record_buffer, record_size, &s_record_after_size);
            }

            if (rt_ringbuffer_data_len(s_record_bypass_rb) >= record_size1) {
                rt_ringbuffer_get(s_record_bypass_rb, record_buffer1, record_size1);
                hl_mod_audio_record(cap_config->file_audio_bypass, record_buffer1, record_size1, &s_record_after_size);
            }
        }
        
        if (s_record_key_flag == 1) {
            s_record_key_flag = 0;
            if (s_record_switch == 0) {
                ter = hl_mod_audio_record_switch(1); 
                if (ter == 0) {
                    hl_drv_aw21009_test(1);
                }
            } else {
                ter = hl_mod_audio_record_switch(0);  
                if (ter == 0) {
                    hl_drv_aw21009_test(0);
                }
            }
        }
    }
}
#endif

static void do_write_audio(void* arg)
{
    char*    audio_buffer;
    uint32_t audio_size;

    audio_size   = dsp_config->process_size;
    audio_buffer = rt_malloc(audio_size);

    while (1) {
        if ((rt_ringbuffer_data_len(s_audio_rb) >= audio_size) && (s_audio_switch == 1)) {
            rt_ringbuffer_get(s_audio_rb, audio_buffer, audio_size);
            /* 写入无线 */
            if (rt_device_write(ply_config->card, 0, audio_buffer, ply_config->period_size)
                <= 0) {
                rt_kprintf("Error playing sample\n");
            }
        } else {
            rt_thread_delay(1);
        }
    }
}

static void do_read_audio(void* arg)
{
    // -------------------cap init-------------------

    char                cap_card[RT_NAME_MAX] = { 0 };
    uint32_t            cap_timeout           = 10; /* 10 seconds default */
    struct AUDIO_PARAMS cap_param;
    struct audio_buf    cap_abuf;
    rt_err_t            cap_ret;
    char*               cap_buffer;
    uint32_t            cap_size;

    uint32_t            ply_timeout           = UINT32_MAX;
    int                 ply_more_chunks       = 1;
    char                ply_card[RT_NAME_MAX] = { 0 };
    struct AUDIO_PARAMS ply_param;
    struct audio_buf    ply_abuf;
    rt_err_t            ply_ret;
    char*               ply_buffer;
    uint32_t            ply_size;

#if 1

    //-------------------start prepare cap a frame-------------------
    if (cap_config->card == RT_NULL) {
        return;
    }
    cap_ret = rt_device_open(cap_config->card, RT_DEVICE_OFLAG_RDONLY);
    if (cap_ret < 0) {
        rt_kprintf("dev:%s \n", cap_config->card);
        rt_kprintf("[%d]: Failed to open %s, err: %d\n", __LINE__, cap_config->card->parent.name, cap_ret);
        return;
    }

    cap_abuf.period_size = cap_config->period_size;
    cap_abuf.buf_size    = cap_config->period_size * cap_config->period_count;
    cap_size             = cap_abuf.buf_size * cap_config->channels * (cap_config->bits >> 3); /* frames to bytes */
    cap_abuf.buf         = rt_malloc_uncache(cap_size);
    if (!cap_abuf.buf) {
        rt_kprintf("[%d]: Buffer alloc failed!\n", __LINE__);
        return 0;
    }

    cap_param.channels   = cap_config->channels;
    cap_param.sampleRate = cap_config->rate;
    cap_param.sampleBits = cap_config->bits;

    cap_ret = rt_device_control(cap_config->card, RK_AUDIO_CTL_PCM_PREPARE, &cap_abuf);
    if(cap_ret != RT_EOK) {
        rt_kprintf("[%d]: PCM error !\n", __LINE__);
        return 0;
    }

    cap_ret = rt_device_control(cap_config->card, RK_AUDIO_CTL_HW_PARAMS, &cap_param);
    if(cap_ret != RT_EOK) {
        rt_kprintf("[%d]: codec error !\n", __LINE__);
        return 0;
    }

    rt_kprintf("[%d]: Capturing sample: %lu ch, %lu hz, %lu bits\n", __LINE__, cap_config->channels, cap_config->rate,
               cap_config->bits);

    //-------------------start prepare ply a frame-------------------
    // start prepare play card
    if (ply_config->card == RT_NULL) {
        return;
    }
    ply_ret = rt_device_open(ply_config->card, RT_DEVICE_OFLAG_WRONLY);
    if (ply_ret < 0) {
        rt_kprintf("[%d]: Failed to open %s, err: %d\n", __LINE__, ply_config->card->parent.name, ply_ret);
        return;
    }

    ply_abuf.period_size = ply_config->period_size;
    ply_abuf.buf_size    = ply_config->period_size * ply_config->period_count;
    ply_size             = ply_abuf.buf_size * ply_config->channels * (ply_config->bits >> 3); /* frames to bytes */
    ply_abuf.buf         = rt_malloc_uncache(ply_size);
    if (!ply_abuf.buf) {
        rt_kprintf("[%d]: Buffer alloc failed!, size = %d\n", __LINE__, ply_size);
        return;
    }

    ply_param.channels   = ply_config->channels;
    ply_param.sampleRate = ply_config->rate;
    ply_param.sampleBits = ply_config->bits;

    ply_ret = rt_device_control(ply_config->card, RK_AUDIO_CTL_PCM_PREPARE, &ply_abuf);
    RT_ASSERT(ply_ret == RT_EOK);

    ply_ret = rt_device_control(ply_config->card, RK_AUDIO_CTL_HW_PARAMS, &ply_param);
    RT_ASSERT(ply_ret == RT_EOK);

    cap_size   = cap_abuf.period_size * cap_config->channels * (cap_config->bits >> 3);
    cap_buffer = rt_malloc(cap_size);
    if (!cap_buffer) {
        rt_kprintf("[%d]: Unable to allocate %ld bytes\n", __LINE__, cap_size);
        rt_device_close(cap_config->card);
        return 0;
    }

    while (1) {
        if (rt_device_read(cap_config->card, 0, dsp_config->audio_process_in_buffer_b32_2ch, cap_abuf.period_size) <= 0) {
            rt_kprintf("Error capturing sample\n");
            break;
        }

        hl_drv_rk_xtensa_dsp_transfer();

        if ((rt_ringbuffer_space_len(s_audio_rb) >= cap_size) && (s_audio_switch == 1)) {
            rt_ringbuffer_put(s_audio_rb, dsp_config->audio_process_out_buffer_b32_2ch, dsp_config->process_size);
        } else {
            rt_kprintf("rt_ringbuffer_get_size(s_audio_rb) =  %d", rt_ringbuffer_get_size(s_audio_rb));
        }

#if HL_GET_DEVICE_TYPE()        
        if (s_record_switch == 1) {
            rt_ringbuffer_put(s_record_after_rb, dsp_config->audio_after_process_out_buffer_b24_1ch,
                              dsp_config->buffer_size_b24_1ch);
        }

        if (s_record_switch == 1) {
            rt_ringbuffer_put(s_record_bypass_rb, dsp_config->audio_before_process_out_buffer_b24_1ch,
                              dsp_config->buffer_size_b24_1ch);
        }
#endif
    }

    cap_ret = rt_device_control(cap_config->card, RK_AUDIO_CTL_STOP, NULL);
    RT_ASSERT(cap_ret == RT_EOK);
    cap_ret = rt_device_control(cap_config->card, RK_AUDIO_CTL_PCM_RELEASE, NULL);
    RT_ASSERT(cap_ret == RT_EOK);
    rt_device_close(cap_config->card);

    rt_free(cap_config);
#endif
}




/* Exported functions --------------------------------------------------------*/

uint8_t hl_mod_audio_init(void* p_msg_handle)
{
    rt_thread_t audio_tid   = RT_NULL;
    rt_thread_t record_tid  = RT_NULL;
    uint8_t temp = 0;

    card_play[0]    = NULL;
    card_capture[0] = NULL;

#if HL_GET_DEVICE_TYPE()
    hl_drv_aw21009_init();
    _hl_drv_key_init();
    s_record_switch = 0;
    ///
    hl_hal_gpio_init(GPIO_MIC_SW);
    hl_hal_gpio_high(GPIO_MIC_SW);
    temp = 0;
    hl_drv_rk_xtensa_dsp_io_ctrl(HL_EM_DRV_RK_DSP_CMD_DENOISE_DSP, &temp, 1);
#else
    hl_hal_gpio_init(GPIO_AMP_EN);
    hl_hal_gpio_high(GPIO_AMP_EN);
#endif
   
    s_audio_switch  = 1;
    hl_mod_audio_codec_config();
    hl_mod_audio_dsp_config();

    s_audio_rb = rt_ringbuffer_create(dsp_config->process_size * 30);
    RT_ASSERT(s_audio_rb != RT_NULL);

#if HL_GET_DEVICE_TYPE()
    s_record_after_rb = rt_ringbuffer_create(dsp_config->buffer_size_b24_1ch * 30);
    RT_ASSERT(s_record_after_rb != RT_NULL);

    s_record_bypass_rb = rt_ringbuffer_create(dsp_config->buffer_size_b24_1ch * 30);
    RT_ASSERT(s_record_bypass_rb != RT_NULL);
#endif

    audio_tid = rt_thread_create("audio_read", do_read_audio, RT_NULL, 2048, 5, 1);  //
    if (audio_tid)
        rt_thread_startup(audio_tid);

    audio_tid = rt_thread_create("audio_write", do_write_audio, RT_NULL, 2048, 5, 1);  //
    if (audio_tid)
        rt_thread_startup(audio_tid);

#if HL_GET_DEVICE_TYPE()
    record_tid = rt_thread_create("record_after", do_record_audio, RT_NULL, 2048, RT_THREAD_PRIORITY_MAX / 2, 1);  //
    if (record_tid)
        rt_thread_startup(record_tid);
#endif

    return 0;
}

uint8_t hl_mod_audio_deinit()
{
    return 0;
}
uint8_t hl_mod_audio_io_ctrl(uint8_t cmd, void* ptr, uint16_t len)
{
    switch (cmd) {
        case HL_MOD_AUDIO_GET_INFO_CMD:
            break;
        case HL_MOD_AUDIO_SET_TIME_CMD:
            break;
        case HL_MOD_AUDIO_GET_TIME_CMD:
            break;
        case HL_MOD_AUDIO_SET_DENOISE_CMD:
            if (ptr == NULL) {
                rt_kprintf("HL_MOD_AUDIO_SET_DENOISE_CMD parem error");
                return -1;
            }
            hl_drv_rk_xtensa_dsp_io_ctrl(HL_EM_DRV_RK_DSP_CMD_DENOISE_DSP, ptr, 1);
            break;
        case HL_MOD_AUDIO_SET_GAIN_CMD:
            break;
        case HL_MOD_AUDIO_SET_MUTE_CMD:
            break;
        case HL_MOD_AUDIO_SET_EQ_CMD:
            break;
        case HL_MOD_AUDIO_RECORD_CMD:
            if (ptr == NULL) {
                rt_kprintf("HL_MOD_AUDIO_RECORD_CMD parem error");
                return -1;
            }
            return hl_mod_audio_record_switch(((char*)ptr)[0]);
            break;
#if HL_GET_DEVICE_TYPE()
        case HL_MOD_AUDIO_MIC_SWITCH_CMD:
            if (ptr == NULL) {
                rt_kprintf("HL_MOD_AUDIO_RECORD_CMD parem error");
                return -1;
            }
            if(((char*)ptr)[0] != 0) {
                hl_hal_gpio_init(GPIO_MIC_SW);
                hl_hal_gpio_high(GPIO_MIC_SW);
            } else {
                hl_hal_gpio_init(GPIO_MIC_SW);
                hl_hal_gpio_low(GPIO_MIC_SW);
            }                
            break;
#endif
        default:
            rt_kprintf("Audio io ctrl param error \r\n");
            break;
    }

    return 0;
}

/**
 * 
 * @brief 
 * @param [in] argc 
 * @param [in] argv 
 * @return int 
 * @date 2022-10-13
 * @author lixiang (rd37@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-13      <td>lixiang     <td>新建
 * </table>
 */
int hl_mod_audio_test(int argc, char** argv)
{
    uint8_t audio_param = 0; 

    if (argc <= 1) {
#if HL_GET_DEVICE_TYPE()
        rt_kprintf("wrong parameter, please type: hl_mod_audio_test [info | time | denoise | gain | mute | record | eq | micswitch] [param] \r\n");
#else
        rt_kprintf("wrong parameter, please type: hl_mod_audio_test [info | denoise | gain | mute | eq ] [param] \r\n");
#endif
        return 0;
    }    
    
    if (!strcmp("record", argv[1])) {
        if (argc <= 2) {
            rt_kprintf("wrong parameter, please type: hl_mod_audio_test record [0 | 1] \r\n");
            rt_kprintf("                              param:(0=off,1=no)\r\n");
            return 0;
        }
        audio_param = (uint8_t) atoi(argv[2]);
        hl_mod_audio_io_ctrl(HL_MOD_AUDIO_RECORD_CMD, &audio_param, 1);
    } else if (!strcmp("denoise", argv[1])) {
        if (argc <= 2) {
            rt_kprintf("wrong parameter, please type: hl_mod_audio_test denoise [0 | 1] \r\n");
            rt_kprintf("                              param:(0=off,1=no)\r\n");
            return 0;
        }
        audio_param = (uint8_t) atoi(argv[2]);
        hl_mod_audio_io_ctrl(HL_MOD_AUDIO_SET_DENOISE_CMD, &audio_param, 1);
    } else  if (!strcmp("micswitch", argv[1])) {
        if (argc <= 2) {
            rt_kprintf("wrong parameter, please type: hl_mod_audio_test micswitch [0 | 1] \r\n");
            rt_kprintf("                              param:(0=External Mic, 1=Internal Mic)\r\n");
            return 0;
        }
        audio_param = (uint8_t) atoi(argv[2]);
        hl_mod_audio_io_ctrl(HL_MOD_AUDIO_MIC_SWITCH_CMD, &audio_param, 1);
    } else {
        rt_kprintf("wrong parameter, please type: hl_mod_audio_test cmd error\r\n");
        return 0;
    }

    return 0;
}


#ifdef RT_USING_FINSH

#include <finsh.h>

#if HL_GET_DEVICE_TYPE()
MSH_CMD_EXPORT(hl_mod_audio_test, audio io ctrl cmd);
#endif

#endif

INIT_APP_EXPORT(hl_mod_audio_init);

#endif  // HL_MOD_AUDIO_C
        /*
 * EOF
 */