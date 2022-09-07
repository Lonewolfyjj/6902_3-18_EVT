/**
 * @file dsp_alango_srp_test.c
 * @author yangxianyun (rd52@hollyland-tech.com)
 * @brief local dsp audio test
 * @version 0.1
 * @date 2022-08-08
 *
 * @copyright Copyright (c) 2022 hollyland
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-08-08 <td>v1.0     <td>yangxianyun     <td>内容
 * </table>
 */

#include <rtdevice.h>
#include <rtthread.h>
#include <ipc/ringbuffer.h>
#include "drv_dsp.h"
#include "string.h"
#include "stdlib.h"
#include "dma.h"
//#include "dsp_alango_srp_test.h"  // LIXIANG

// #ifdef RT_USING_COMMON_TEST_AUDIO
#if 1

#include "rk_audio.h"
#include "drv_heap.h"

#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN_SOFTVOL
#include "pcm_plugin_provider.h"

static uint16_t vol_l = 100, vol_r = 100;
#endif

#define AUDIO_FRAME_PERIOD 120
#define AUDIO_FRAME_SIZE_OF_LA_TEST (AUDIO_FRAME_PERIOD * 4 * 2)
#define RING_BUFFER_SIZE_OF_LA_TEST ((AUDIO_FRAME_SIZE_OF_LA_TEST * 3) + 5)

#define ASR_WAKE_ID 0x50000002
#define DSP_ALGO_BYPASS_INIT 0x40000009
#define DSP_ALGO_BYPASS_PROCESS 0x4000000a
#define DSP_ALGO_BYPASS_DEINIT 0x4000000b

#define DSP_ALGO_ALANGO_SRP_INIT 0x4000000c
#define DSP_ALGO_ALANGO_SRP_PROCESS 0x4000000d
#define DSP_ALGO_ALANGO_SRP_DEINIT 0x4000000e

typedef struct _lib_bypass_param_
{
    uint8_t *in_buf;
    uint8_t *out_buf;
    uint16_t buff_len;
} lib_bypass_param_type_t;

typedef struct _lib_alango_srp_param_
{
    uint8_t *main_in_buf;
    uint8_t *subs_in_buf;
    uint8_t *out_buf;
    uint32_t sample_size;
    uint32_t frame_length;
    uint16_t channel;
} lib_alango_srp_param_type_t;

struct capture_config
{
    struct rt_device *card;
    uint32_t rate;
    uint32_t channels;
    uint32_t bits;
    uint32_t period_size;
    uint32_t period_count;
    uint32_t total_frames;
};

struct playback_config
{
    struct rt_device *card;
    uint32_t rate;
    uint32_t channels;
    uint32_t bits;
    uint32_t period_size;
    uint32_t period_count;
    uint32_t total_frames;
};

static struct rt_ringbuffer *rb = RT_NULL;
static lib_bypass_param_type_t *cfg_param;
static lib_alango_srp_param_type_t *cfg_alango_srp_param;
static uint8_t dsp_alango_loopback_test_alive = 0;
static uint8_t dsp_alango_loopback_denoise_enable = 0;
char card_play[RT_NAME_MAX] = {0};
char card_capture[RT_NAME_MAX] = {0};

void print_data_buffer(uint8_t *addr, uint16_t buffer_len)
{
    int i = 0;
    for (i = 0; i < buffer_len; i++)
    {
        if ((i % 16) == 0)
        {
            rt_kprintf("\r\n");
        }
        rt_kprintf("%02x ", addr[i]);
    }
    rt_kprintf("\r\n");
}

static struct dsp_work *dsp_create_work(uint32_t id, uint32_t algo_type, uint32_t param, uint32_t param_size)
{
    struct dsp_work *work;

    work = rk_dsp_work_create(RKDSP_ALGO_WORK);
    if (work)
    {
        work->id = id;
        work->algo_type = algo_type;
        work->param = param;
        work->param_size = param_size;
    }
    return work;
}

static uint8_t split_stero_16bit_frame(uint16_t *stero_frame, uint16_t *left_channel, uint16_t *right_channel,
                                       uint16_t frame_lenth)
{
    uint16_t i = 0, j = 0;
    for (i = 0; i < frame_lenth * 2;)
    {
        left_channel[j++] = stero_frame[i * 2];
        // right_channel[i] = stero_frame[(i * 2) + 1];

        i += 2;
    }
    return 0;
}

static void dsp_destory_work(struct dsp_work *work)
{
    rk_dsp_work_destroy(work);
}

static void do_cap(void *arg)
{
    // -------------------cap init-------------------
    struct capture_config *cap_config;
    char cap_card[RT_NAME_MAX] = {0};
    uint32_t cap_timeout = 10; /* 10 seconds default */
    struct AUDIO_PARAMS cap_param;
    struct audio_buf cap_abuf;
    rt_err_t cap_ret;
    char *cap_buffer;
    char *cap_rb_buffer;
    uint32_t cap_size;
#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN_SOFTVOL
    snd_softvol_t cap_softvol, cap_softvol2;
    snd_pcm_type_t cap_type = SND_PCM_TYPE_SOFTVOL;
#endif

    cap_config = rt_malloc(sizeof(struct capture_config));

    cap_config->rate = 48000;
    cap_config->bits = 24;
    cap_config->channels = 2;
    cap_config->period_size = AUDIO_FRAME_PERIOD;
    cap_config->period_count = 4;
    if(card_capture[0] == 0x00) {
        cap_config->card = rt_device_find("88l25bc");
    } else {
        cap_config->card = rt_device_find(card_capture);
    }
    

    //-------------------start prepare cap a frame-------------------
    cap_ret = rt_device_open(cap_config->card, RT_DEVICE_OFLAG_RDONLY);
    if (cap_ret < 0)
    {
        rt_kprintf("[%d]: Failed to open %s, err: %d\n", __LINE__, cap_config->card->parent.name, cap_ret);
        return 0;
    }

    cap_abuf.period_size = cap_config->period_size;
    cap_abuf.buf_size = cap_config->period_size * cap_config->period_count;
    cap_size = cap_abuf.buf_size * cap_config->channels * (cap_config->bits >> 3); /* frames to bytes */
    cap_abuf.buf = rt_malloc_uncache(cap_size);
    if (!cap_abuf.buf)
    {
        rt_kprintf("[%d]: Buffer alloc failed!\n", __LINE__);
        return 0;
    }

    cap_param.channels = cap_config->channels;
    cap_param.sampleRate = cap_config->rate;
    cap_param.sampleBits = cap_config->bits;

    cap_ret = rt_device_control(cap_config->card, RK_AUDIO_CTL_PCM_PREPARE, &cap_abuf);
    RT_ASSERT(cap_ret == RT_EOK);

#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN_SOFTVOL
    ret = rt_device_control(cap_config->card, RK_AUDIO_CTL_PLUGIN_PREPARE, (void *)cap_type);
    RT_ASSERT(cap_ret == RT_EOK);

    softvol.vol_l = vol_l;
    softvol.vol_r = vol_r;
    rt_kprintf("[%d]: Set softvol: %d, %d\n", __LINE__, vol_l, vol_r);
    cap_ret = rt_device_control(cap_config->card, RK_AUDIO_CTL_PLUGIN_SET_SOFTVOL, &cap_softvol);
    RT_ASSERT(cap_ret == RT_EOK);

    cap_ret = rt_device_control(cap_config->card, RK_AUDIO_CTL_PLUGIN_GET_SOFTVOL, &cap_softvol2);
    RT_ASSERT(cap_ret == RT_EOK);
    rt_kprintf("[%d]: Get softvol2: %d, %d\n", __LINE__, softvol2.vol_l, softvol2.vol_r);
#endif

    cap_ret = rt_device_control(cap_config->card, RK_AUDIO_CTL_HW_PARAMS, &cap_param);
    RT_ASSERT(cap_ret == RT_EOK);

    cap_size = cap_abuf.period_size * cap_config->channels * (cap_config->bits >> 3);
    cap_buffer = rt_malloc(cap_size);
    if (!cap_buffer)
    {
        rt_kprintf("[%d]: Unable to allocate %ld bytes\n", __LINE__, cap_size);
        rt_device_close(cap_config->card);
        return 0;
    }

    rt_kprintf("[%d]: Capturing sample: %lu ch, %lu hz, %lu bits\n", __LINE__, cap_config->channels, cap_config->rate,
               cap_config->bits);
    uint32_t buf_len;
    buf_len = AUDIO_FRAME_PERIOD * (cap_config->bits >> 3) * 2;
#if 0
    // -------------------DSP init-------------------
    struct rt_device*       dsp_dev;
    struct dsp_work*        work;
    struct wake_work_param* param;
    uint32_t                rate;
    uint32_t                buf_len;
    int                     ret = 0;

    rt_kprintf("dsp wakeup_test\n");

    dsp_dev = rt_device_find("dsp0");
    RT_ASSERT(dsp_dev != RT_NULL);
    rt_device_open(dsp_dev, RT_DEVICE_OFLAG_RDWR);

    rt_device_control(dsp_dev, RKDSP_CTL_SET_FREQ, (void*)(396 * MHZ));
    rt_device_control(dsp_dev, RKDSP_CTL_GET_FREQ, (void*)&rate);
    rt_kprintf("current dsp freq: %d MHz\n", rate / MHZ);

    cfg_alango_srp_param = rkdsp_malloc(sizeof(lib_alango_srp_param_type_t));
    RT_ASSERT(cfg_alango_srp_param != NULL);
    cfg_alango_srp_param->frame_length = AUDIO_FRAME_PERIOD;
    cfg_alango_srp_param->sample_size  = cap_config->bits >> 3;
    cfg_alango_srp_param->channel      = 2;
    work = dsp_create_work(ASR_WAKE_ID, DSP_ALGO_ALANGO_SRP_INIT, (uint32_t)cfg_alango_srp_param,
                           sizeof(lib_alango_srp_param_type_t));
    if (!work) {
        rt_kprintf("dsp create config work fail\n");
    }

    ret = rt_device_control(dsp_dev, RKDSP_CTL_QUEUE_WORK, work);
    RT_ASSERT(!ret);
    ret = rt_device_control(dsp_dev, RKDSP_CTL_DEQUEUE_WORK, work);
    RT_ASSERT(!ret);

    // -------------------cap-------------------

    work->algo_type = DSP_ALGO_ALANGO_SRP_PROCESS;
    buf_len = cfg_alango_srp_param->frame_length * cfg_alango_srp_param->sample_size * cfg_alango_srp_param->channel;
    rt_kprintf("buf size = %d\r\n", buf_len);
    cfg_alango_srp_param->main_in_buf = rt_malloc(buf_len);
    RT_ASSERT(cfg_alango_srp_param->main_in_buf != NULL);
    cfg_alango_srp_param->out_buf = rt_malloc(buf_len);
    RT_ASSERT(cfg_alango_srp_param->out_buf != NULL);
    rt_kprintf("cap now dsp_alango_loopback_test_alive = %d\r\n", dsp_alango_loopback_test_alive);
#endif

    while (dsp_alango_loopback_test_alive)
    {
        if (rt_ringbuffer_data_len(rb) < RING_BUFFER_SIZE_OF_LA_TEST - AUDIO_FRAME_SIZE_OF_LA_TEST)
        {
            if (rt_device_read(cap_config->card, 0, cap_buffer, cap_abuf.period_size) <= 0)
            {
                rt_kprintf("Error capturing sample\n");
                break;
            }
            // rt_kprintf("cap 2 sample: 0x%x%x 0x%x%x\n", cfg_alango_srp_param->main_in_buf[0]
            //                                             , cfg_alango_srp_param->main_in_buf[1]
            //                                             , cfg_alango_srp_param->main_in_buf[2]
            //                                             , cfg_alango_srp_param->main_in_buf[3]);

            // rt_kprintf("cap split_stero_16bit_frame\r\n");
            // split_stero_16bit_frame(cap_buffer, cfg_alango_srp_param->main_in_buf, NULL, cfg_alango_srp_param->frame_length);
            // rt_kprintf("cap split_stero_16bit_frame OK\r\n");

            // memcpy(cfg_param->in_buf, cap_buffer, cap_size);

            if (dsp_alango_loopback_denoise_enable)
            {
                // rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, cfg_alango_srp_param->main_in_buf, buf_len);
                // rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, cfg_alango_srp_param->out_buf, buf_len);

                // // rt_kprintf("cap process DSP\r\n");
                // ret = rt_device_control(dsp_dev, RKDSP_CTL_QUEUE_WORK, work);
                // RT_ASSERT(!ret);
                // // rt_kprintf("cap process DSP OK\r\n");
                // ret = rt_device_control(dsp_dev, RKDSP_CTL_DEQUEUE_WORK, work);
                // RT_ASSERT(!ret);
                // rt_ringbuffer_put(rb, cfg_alango_srp_param->out_buf, buf_len);
            }
            else
            {
                rt_ringbuffer_put(rb, cap_buffer, buf_len);
            }
            // rt_kprintf("finish a loop\n");
        }
        else
        {
            rt_thread_mdelay(1);
            rt_kprintf("ring buffer full\n");
        }
    }

#if 0
    // -------------------finish dsp-------------------
    work->algo_type = DSP_ALGO_ALANGO_SRP_DEINIT;
    ret = rt_device_control(dsp_dev, RKDSP_CTL_QUEUE_WORK, work);
    RT_ASSERT(!ret);
    ret = rt_device_control(dsp_dev, RKDSP_CTL_DEQUEUE_WORK, work);
    RT_ASSERT(!ret);

    free(cfg_alango_srp_param->main_in_buf);
    free(cfg_alango_srp_param->out_buf);

    rk_dsp_work_destroy(work);
    rkdsp_free(cfg_alango_srp_param);
    rt_device_close(dsp_dev);

    // -------------------free cap src-------------------
    free(cap_buffer);
    rt_free_uncache(cap_abuf.buf);
#endif

#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN_SOFTVOL
    cap_ret = rt_device_control(cap_config->card, RK_AUDIO_CTL_PLUGIN_RELEASE, (void *)cap_type);
    RT_ASSERT(cap_ret == RT_EOK);
#endif

    cap_ret = rt_device_control(cap_config->card, RK_AUDIO_CTL_STOP, NULL);
    RT_ASSERT(cap_ret == RT_EOK);
    cap_ret = rt_device_control(cap_config->card, RK_AUDIO_CTL_PCM_RELEASE, NULL);
    RT_ASSERT(cap_ret == RT_EOK);
    rt_kprintf("now dsp_alango_loopback_test_alive = %d\r\n", dsp_alango_loopback_test_alive);

    rt_device_close(cap_config->card);

    rt_free(cap_config);
}

/**
 * do_ply
 * @brief 3456
 * @param [in] arg
 *
 * @details
 */
static void do_ply(void *arg)
{
    // play init
    struct playback_config *ply_config;
    uint32_t ply_timeout = UINT32_MAX;
    int ply_more_chunks = 1;
    char ply_card[RT_NAME_MAX] = {0};
    struct AUDIO_PARAMS ply_param;
    struct audio_buf ply_abuf;
    rt_err_t ply_ret;
    char *ply_buffer;
    uint32_t ply_size;
#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN_SOFTVOL
    snd_softvol_t ply_softvol, ply_softvol2;
    snd_pcm_type_t ply_type = SND_PCM_TYPE_SOFTVOL;
#endif

    ply_config = rt_malloc(sizeof(struct playback_config));

    ply_config->rate = 48000;
    ply_config->bits = 24;
    ply_config->channels = 2;
    ply_config->period_size = AUDIO_FRAME_PERIOD;
    ply_config->period_count = 4;
    if(card_play[0] == 0x00) {
        ply_config->card = rt_device_find("88l25bp");
    } else {
        ply_config->card = rt_device_find(card_play);
    }
    rt_kprintf("[%d]: start ini ply\n", __LINE__);

    // start prepare play card
    ply_ret = rt_device_open(ply_config->card, RT_DEVICE_OFLAG_WRONLY);
    if (ply_ret < 0)
    {
        rt_kprintf("[%d]: Failed to open %s, err: %d\n", __LINE__, ply_config->card->parent.name, ply_ret);
        return;
    }

    ply_abuf.period_size = ply_config->period_size;
    ply_abuf.buf_size = ply_config->period_size * ply_config->period_count;
    ply_size = ply_abuf.buf_size * ply_config->channels * (ply_config->bits >> 3); /* frames to bytes */
    ply_abuf.buf = rt_malloc_uncache(ply_size);
    if (!ply_abuf.buf)
    {
        rt_kprintf("[%d]: Buffer alloc failed!, size = %d\n", __LINE__, ply_size);
        return;
    }

    ply_param.channels = ply_config->channels;
    ply_param.sampleRate = ply_config->rate;
    ply_param.sampleBits = ply_config->bits;

    ply_ret = rt_device_control(ply_config->card, RK_AUDIO_CTL_PCM_PREPARE, &ply_abuf);
    RT_ASSERT(ply_ret == RT_EOK);
    rt_kprintf("[%d]: ply card open ok\n", __LINE__);

#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN_SOFTVOL
    ply_ret = rt_device_control(card, RK_AUDIO_CTL_PLUGIN_PREPARE, (void *)type);
    RT_ASSERT(ply_ret == RT_EOK);

    softvol.vol_l = vol_l;
    softvol.vol_r = vol_r;
    rt_kprintf("[%d]: Set softvol: %d, %d\n", __LINE__, vol_l, vol_r);
    ply_ret = rt_device_control(card, RK_AUDIO_CTL_PLUGIN_SET_SOFTVOL, &softvol);
    RT_ASSERT(ply_ret == RT_EOK);

    ply_ret = rt_device_control(card, RK_AUDIO_CTL_PLUGIN_GET_SOFTVOL, &softvol2);
    RT_ASSERT(ret == RT_EOK);
    rt_kprintf("[%d]: Get softvol2: %d, %d\n", __LINE__, softvol2.vol_l, softvol2.vol_r);
#endif

    rt_kprintf("[%d]: ply card param\n", __LINE__);
    ply_ret = rt_device_control(ply_config->card, RK_AUDIO_CTL_HW_PARAMS, &ply_param);
    rt_kprintf("[%d]: ply card param ok 1\n", __LINE__);
    RT_ASSERT(ply_ret == RT_EOK);
    rt_kprintf("[%d]: ply card param ok 2 \n", __LINE__);

    ply_size = ply_abuf.period_size * ply_config->channels * (ply_config->bits >> 3);
    rt_kprintf("[%d]: ply card malloc size = %d\n", __LINE__, ply_size);
    ply_buffer = rt_malloc(ply_size);
    if (!ply_buffer)
    {
        rt_kprintf("[%d]: Unable to allocate %d bytes\n", __LINE__, ply_size);
        dsp_alango_loopback_test_alive = 0;
        // rt_device_close(ply_card);
        return;
    }
    rt_kprintf("[%d]: ply card malloc OK\n", __LINE__);

    rt_kprintf("[%d]: Playing sample: %lu ch, %lu hz, %lu bit\n", __LINE__, ply_config->channels, ply_config->rate,
               ply_config->bits);

    rt_kprintf("now dsp_alango_loopback_test_alive = %d\r\n", dsp_alango_loopback_test_alive);

    // play
    uint16_t idx = 0;
    // rt_memset(ply_buffer, 0x00, ply_size);
    while (dsp_alango_loopback_test_alive)
    {
        if ((rt_ringbuffer_data_len(rb) >= AUDIO_FRAME_SIZE_OF_LA_TEST))
        {
            rt_ringbuffer_get(rb, ply_buffer, ply_size);
            if (rt_device_write(ply_config->card, 0, ply_buffer, ply_abuf.period_size) <= 0)
            {
                rt_kprintf("Error playing sample\n");
                break;
            }
        }
        else
        {
            rt_thread_delay(1);
        }
        // else {
        //     // rt_memset(ply_buffer, 0x00, ply_size);
        //     if (rt_device_write(ply_config->card, 0, ply_buffer, ply_abuf.period_size) <= 0) {
        //         rt_kprintf("Error playing sample\n");
        //         break;
        //     }
        //     rt_thread_delay(1);
        //     rt_kprintf("ply rb size = %d\r\n", (uint16_t)rt_ringbuffer_data_len(rb));
        // }

        // if (idx++ == 1000) {
        //     idx = 0;
        //     rt_kprintf("rb size = %d\r\n", (uint16_t)rt_ringbuffer_data_len(rb));
        // }
    }

    // free ply src
#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN_SOFTVOL
    ply_ret = rt_device_control(card, RK_AUDIO_CTL_PLUGIN_RELEASE, (void *)ply_type);
    RT_ASSERT(ply_ret == RT_EOK);
#endif

    rt_kprintf("now dsp_alango_loopback_test_alive = %d\r\n", dsp_alango_loopback_test_alive);

    ply_ret = rt_device_control(ply_config->card, RK_AUDIO_CTL_STOP, NULL);
    RT_ASSERT(ply_ret == RT_EOK);
    ply_ret = rt_device_control(ply_config->card, RK_AUDIO_CTL_PCM_RELEASE, NULL);
    RT_ASSERT(ply_ret == RT_EOK);

    rt_free(ply_buffer);
    rt_free_uncache(ply_abuf.buf);
    rt_device_close(ply_config->card);

    rt_free(ply_config);
}

/**
 * dsp_alango_loopback_test
 * @brief 123
 * @param [in] argc
 * @param [in] argv
 * @return int
 *
 * @details
 */
int dsp_alango_loopback_test(int argc, char **argv)
{

    rt_thread_t cap_tid = RT_NULL;
    rt_thread_t ply_tid = RT_NULL;

    if (argc > 1)
    {
        if (!strcmp("start", argv[1]))
        {
            /* parse command line arguments */
        argv += 2;
        argc -= 2;
        while (argc)
        {
            if (strcmp(*argv, "-p") == 0)
            {
                argv++;
                argc--;
                if (*argv)
                    rt_strncpy(card_play, *argv, RT_NAME_MAX);
            }
            if (strcmp(*argv, "-c") == 0)
            {
                argv++;
                argc--;
                if (*argv)
                    rt_strncpy(card_capture, *argv, RT_NAME_MAX);
            }
            
            argv++;
            argc--;
        }
            rt_kprintf("loopback chain test start\r\n");
        }
        else if (!strcmp("stop", argv[1]))
        {
            rt_kprintf("loopback chain test stop\r\n");
            dsp_alango_loopback_test_alive = 0;
            return 0;
        }
        else
        {
            rt_kprintf("wrong parameter, please type: dsp_alango_loopback_test [start/stop]\r\n");
            return 0;
        }
    }
    else
    {
        rt_kprintf("wrong parameter, please type: dsp_alango_loopback_test [start/stop] [-p play] [-c capture]\r\n");
        return 0;
    }
    dsp_alango_loopback_test_alive = 1;

    rb = rt_ringbuffer_create(RING_BUFFER_SIZE_OF_LA_TEST);
    RT_ASSERT(rb != RT_NULL);

    cap_tid = rt_thread_create("do_cap", do_cap, RT_NULL, 2048, RT_THREAD_PRIORITY_MAX / 2, 1);
    ply_tid = rt_thread_create("do_ply", do_ply, RT_NULL, 2048, RT_THREAD_PRIORITY_MAX / 2, 1);

    if (ply_tid)
        rt_thread_startup(ply_tid);

    if (cap_tid)
        rt_thread_startup(cap_tid);

    // //start cap and ply
    // while(1) {
    //     if (rt_device_read(cap_config->card, 0, cap_buffer, cap_abuf.period_size) <= 0) {
    //         rt_kprintf("Error capturing sample\n");
    //         break;
    //     }else {
    //         if (rt_device_write(ply_config->card, 0, cap_buffer, ply_abuf.period_size) <= 0) {
    //             rt_kprintf("Error playing sample\n");
    //             break;
    //         }
    //     }
    // }
    return 0;
}

int dsp_alango_loopback_enable_denoise(int argc, char **argv)
{
    if (argc != 2)
    {
        return 1;
    }
    int mode = atoi(argv[1]);

    switch (mode)
    {
    case 0:
        dsp_alango_loopback_denoise_enable = 0;
        break;
    case 1:
        dsp_alango_loopback_denoise_enable = 1;
        break;
    default:
        break;
    }
    return 0;
}

#ifdef RT_USING_FINSH

#include <finsh.h>
MSH_CMD_EXPORT(dsp_alango_loopback_test, local audio chain test);
MSH_CMD_EXPORT(dsp_alango_loopback_enable_denoise, 1
               : enable | 0
               : disable);

#endif

#endif