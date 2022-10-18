/**
 * @file hl_drv_rk_xtensa_dsp_example.c
 * @author yangxianyun (rd52@hollyland-tech.com)
 * @brief hl_drv_rk_xtensa_dsp驱动的示例文件
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
#include "hl_drv_rk_xtensa_dsp.h"
#include <rtdevice.h>
#include <ipc/ringbuffer.h>
#include "drv_dsp.h"
#include "string.h"
#include "stdlib.h"
#include "dma.h"
#include "rk_audio.h"
#include "drv_heap.h"
#include <finsh.h>
#include <stdint.h>
/* typedef -------------------------------------------------------------------*/

// 测试时设为1，项目中设为0
#if 0

struct capture_config
{
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
    struct rt_device* card;
    uint32_t          rate;
    uint32_t          channels;
    uint32_t          bits;
    uint32_t          period_size;
    uint32_t          period_count;
    uint32_t          total_frames;
};

/* define --------------------------------------------------------------------*/

#define AUDIO_FRAME_PERIOD 120
#define AUDIO_FRAME_SIZE_OF_LA_TEST (AUDIO_FRAME_PERIOD * 2 * 4)
#define RING_BUFFER_SIZE_OF_LA_TEST ((AUDIO_FRAME_SIZE_OF_LA_TEST * 2) + 5)

#define RECOVERY_KEY BANK_PIN(1, 14)
#define PWM_LED BANK_PIN(1, 4)

/* variables -----------------------------------------------------------------*/

static struct rt_ringbuffer* rb                                 = RT_NULL;
static uint8_t               dsp_alango_loopback_test_alive     = 0;
static uint8_t               dsp_alango_loopback_denoise_enable = 0;
/* Private function(only *.c)  -----------------------------------------------*/
#include "hal_base.h"

struct TGPIO_INFO
{
    char*            desc;
    struct GPIO_REG* gpio;
    rt_uint32_t      mask;
};

static struct TGPIO_INFO gpios[] = {
    { "gpio0", GPIO0, 0x00000000 },
#ifdef GPIO1
    { "gpio1", GPIO1, 0x00000000 },
#endif
#ifdef GPIO2
    { "gpio2", GPIO2, 0x00000000 },
#endif
#ifdef GPIO3
    { "gpio3", GPIO3, 0x00000000 },
#endif
};

static void _tgpio_prepare(uint8_t bank_id, uint32_t pin_id)
{
    if (bank_id > 4) {
        return;
    }
    gpios[bank_id].mask |= 0x01 << pin_id;
    // rt_kprintf("%s: %s to test pins: 0x%lx\n", __func__, gpios[bank_id].desc, pin_id);
    HAL_PINCTRL_SetIOMUX(bank_id, gpios[bank_id].mask, 0);
}

static void _tgpio_output(uint8_t bank_id, uint32_t pin_id, uint8_t level)
{
    _tgpio_prepare(bank_id, pin_id);
    HAL_GPIO_SetPinsDirection(gpios[bank_id].gpio, gpios[bank_id].mask, GPIO_OUT);
    switch (level) {
        case 0:
            HAL_GPIO_SetPinsLevel(gpios[bank_id].gpio, gpios[bank_id].mask, GPIO_LOW);
            HAL_PINCTRL_SetParam(bank_id, gpios[bank_id].mask, PIN_CONFIG_DRV_LEVEL0);
            break;
        case 1:
            HAL_GPIO_SetPinsLevel(gpios[bank_id].gpio, gpios[bank_id].mask, GPIO_HIGH);
            HAL_PINCTRL_SetParam(bank_id, gpios[bank_id].mask, PIN_CONFIG_DRV_LEVEL1);
            break;
        case 2:
            HAL_GPIO_SetPinsLevel(gpios[bank_id].gpio, gpios[bank_id].mask, GPIO_HIGH);
            HAL_PINCTRL_SetParam(bank_id, gpios[bank_id].mask, PIN_CONFIG_DRV_LEVEL2);
            break;
        case 3:
            HAL_GPIO_SetPinsLevel(gpios[bank_id].gpio, gpios[bank_id].mask, GPIO_HIGH);
            HAL_PINCTRL_SetParam(bank_id, gpios[bank_id].mask, PIN_CONFIG_DRV_LEVEL3);
            break;
        default:
            rt_kprintf("wrong pinbank\r\n");
            break;
    }
}

static void do_cap(void* arg)
{
    // -------------------cap init-------------------
    struct capture_config* cap_config;
    char                   cap_card[RT_NAME_MAX] = { 0 };
    uint32_t               cap_timeout           = 10; /* 10 seconds default */
    struct AUDIO_PARAMS    cap_param;
    struct audio_buf       cap_abuf;
    rt_err_t               cap_ret;
    char*                  cap_buffer;
    char*                  cap_rb_buffer;
    uint32_t               cap_size;
    int                    ret = 0;
#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN_SOFTVOL
    snd_softvol_t  cap_softvol, cap_softvol2;
    snd_pcm_type_t cap_type = SND_PCM_TYPE_SOFTVOL;
#endif

    uint8_t last_denoise_state = 0;

    cap_config = rt_malloc(sizeof(struct capture_config));

    cap_config->rate         = 48000;
    cap_config->bits         = 32;
    cap_config->channels     = 2;
    cap_config->period_size  = AUDIO_FRAME_PERIOD;
    cap_config->period_count = 4;
    cap_config->card         = rt_device_find("pdmc");

    //-------------------start prepare cap a frame-------------------
    cap_ret = rt_device_open(cap_config->card, RT_DEVICE_OFLAG_RDONLY);
    if (cap_ret < 0) {
        rt_kprintf("[%d]: Failed to open %s, err: %d\n", __LINE__, cap_config->card->parent.name, cap_ret);
        return 0;
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
    RT_ASSERT(cap_ret == RT_EOK);

#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN_SOFTVOL
    ret = rt_device_control(cap_config->card, RK_AUDIO_CTL_PLUGIN_PREPARE, (void*)cap_type);
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

    cap_size   = cap_abuf.period_size * cap_config->channels * (cap_config->bits >> 3);
    cap_buffer = rt_malloc(cap_size);
    if (!cap_buffer) {
        rt_kprintf("[%d]: Unable to allocate %ld bytes\n", __LINE__, cap_size);
        rt_device_close(cap_config->card);
        return 0;
    }

    rt_kprintf("[%d]: Capturing sample: %lu ch, %lu hz, %lu bits\n", __LINE__, cap_config->channels, cap_config->rate,
               cap_config->bits);

    // -------------------DSP init-------------------

    // -------------------cap-------------------

    hl_drv_rk_xtensa_dsp_config_t_p dsp_config = 
        (hl_drv_rk_xtensa_dsp_config_t_p)rkdsp_malloc(sizeof(hl_drv_rk_xtensa_dsp_config_t));
    dsp_config->bits                = 32;
    dsp_config->channels            = 2;
    dsp_config->period_size         = AUDIO_FRAME_PERIOD;
    dsp_config->rate                = 48000;
    dsp_config->process_size        = dsp_config->period_size * dsp_config->channels * (dsp_config->bits >> 3);
    dsp_config->buffer_size_b32_2ch = dsp_config->process_size;
    dsp_config->audio_process_in_buffer_b32_2ch         = rkdsp_malloc(dsp_config->buffer_size_b32_2ch + 2);
    dsp_config->audio_process_out_buffer_b32_2ch        = rkdsp_malloc(dsp_config->buffer_size_b32_2ch + 2);
#if HL_GET_DEVICE_TYPE()
    dsp_config->buffer_size_b24_1ch = dsp_config->period_size * 1 * 3;
    dsp_config->audio_after_process_out_buffer_b24_1ch  = rkdsp_malloc(dsp_config->buffer_size_b24_1ch + 2);
    dsp_config->audio_before_process_out_buffer_b24_1ch = rkdsp_malloc(dsp_config->buffer_size_b24_1ch + 2);
#else
    dsp_config->buffer_size_b24_2ch = dsp_config->period_size * 2 * 3;
    dsp_config->audio_after_process_out_buffer_b24_2ch  = rkdsp_malloc(dsp_config->buffer_size_b24_2ch + 2);
    dsp_config->audio_before_process_out_buffer_b24_2ch = rkdsp_malloc(dsp_config->buffer_size_b24_2ch + 2);
#endif
    hl_drv_rk_xtensa_dsp_init();
    hl_drv_rk_xtensa_dsp_io_ctrl(HL_EM_DRV_RK_DSP_CMD_SET_CONFIG, dsp_config, sizeof(hl_drv_rk_xtensa_dsp_config_t));
    hl_drv_rk_xtensa_dsp_io_ctrl(HL_EM_DRV_RK_DSP_CMD_START_DSP, NULL, 0);

    _tgpio_output(1, 4, 3);
    rt_kprintf("denoise enable = %d\r\n", dsp_alango_loopback_denoise_enable);
    volatile rt_uint32_t past = rt_tick_get();
    volatile rt_uint32_t now = rt_tick_get();
    while (dsp_alango_loopback_test_alive) {

        if (last_denoise_state != dsp_alango_loopback_denoise_enable) {
            last_denoise_state = dsp_alango_loopback_denoise_enable;
            if (dsp_alango_loopback_denoise_enable) {
                _tgpio_output(1, 4, 0);
            } else {
                _tgpio_output(1, 4, 3);
            }
            rt_kprintf("denoise enable = %d\r\n", dsp_alango_loopback_denoise_enable);
        }

        if (rt_ringbuffer_data_len(rb) < RING_BUFFER_SIZE_OF_LA_TEST - AUDIO_FRAME_SIZE_OF_LA_TEST) {
            if (rt_device_read(cap_config->card, 0, dsp_config->audio_process_in_buffer_b32_2ch, cap_abuf.period_size)
                <= 0) {
                rt_kprintf("Error capturing sample\n");
                break;
            }

            if (dsp_alango_loopback_denoise_enable) {
                //dsp process
                past = rt_tick_get();
                ret = hl_drv_rk_xtensa_dsp_transfer();
                now = rt_tick_get();
                // rt_kprintf("hl_drv_rk_xtensa_dsp_transfer ret = %d , tick start : %d , finish at %d\r\n", ret, past , now);
                // rt_kprintf("%ld %ld\n",past , now);

                rt_ringbuffer_put(rb, dsp_config->audio_process_out_buffer_b32_2ch, dsp_config->buffer_size_b32_2ch);

            } else {
                rt_ringbuffer_put(rb, dsp_config->audio_process_in_buffer_b32_2ch, dsp_config->buffer_size_b32_2ch);
            }
        } else {
            rt_thread_mdelay(1);
            rt_kprintf("ring buffer full\r\n");
        }
    }

    // -------------------finish dsp-------------------
    hl_drv_rk_xtensa_dsp_io_ctrl(HL_EM_DRV_RK_DSP_CMD_STOP_DSP, NULL, 0);
    hl_drv_rk_xtensa_dsp_deinit();
    rkdsp_free(dsp_config->audio_process_in_buffer_b32_2ch);
    rkdsp_free(dsp_config->audio_process_out_buffer_b32_2ch);
#if HL_GET_DEVICE_TYPE()
    rkdsp_free(dsp_config->audio_after_process_out_buffer_b24_1ch);
    rkdsp_free(dsp_config->audio_before_process_out_buffer_b24_1ch);
#else
    rkdsp_free(dsp_config->audio_after_process_out_buffer_b24_2ch);
    rkdsp_free(dsp_config->audio_before_process_out_buffer_b24_2ch);
#endif
    rkdsp_free(dsp_config);

    // -------------------free cap src-------------------
    free(cap_buffer);
    rt_free_uncache(cap_abuf.buf);

#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN_SOFTVOL
    cap_ret = rt_device_control(cap_config->card, RK_AUDIO_CTL_PLUGIN_RELEASE, (void*)cap_type);
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
static void do_ply(void* arg)
{
    // play init
    struct playback_config* ply_config;
    uint32_t                ply_timeout           = UINT32_MAX;
    int                     ply_more_chunks       = 1;
    char                    ply_card[RT_NAME_MAX] = { 0 };
    struct AUDIO_PARAMS     ply_param;
    struct audio_buf        ply_abuf;
    rt_err_t                ply_ret;
    char*                   ply_buffer;
    uint32_t                ply_size;
#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN_SOFTVOL
    snd_softvol_t  ply_softvol, ply_softvol2;
    snd_pcm_type_t ply_type = SND_PCM_TYPE_SOFTVOL;
#endif

    ply_config = rt_malloc(sizeof(struct playback_config));

    ply_config->rate         = 48000;
    ply_config->bits         = 32;
    ply_config->channels     = 2;
    ply_config->period_size  = AUDIO_FRAME_PERIOD;
    ply_config->period_count = 4;
    ply_config->card         = rt_device_find("es8311p");

    rt_kprintf("[%d]: start ini ply\n", __LINE__);

    // start prepare play card
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
    rt_kprintf("[%d]: ply card open ok\n", __LINE__);

#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN_SOFTVOL
    ply_ret = rt_device_control(card, RK_AUDIO_CTL_PLUGIN_PREPARE, (void*)type);
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
    if (!ply_buffer) {
        rt_kprintf("[%d]: Unable to allocate %d bytes\n", __LINE__, ply_size);
        dsp_alango_loopback_test_alive = 0;
        // rt_device_close(ply_card);
        return;
    }
    rt_kprintf("[%d]: ply card malloc OK\n", __LINE__);

    rt_kprintf("[%d]: Playing sample: %lu ch, %lu hz, %lu bit\n", __LINE__, ply_config->channels, ply_config->rate,
               ply_config->bits);

    rt_kprintf("now dsp_alango_loopback_test_alive = %d\r\n", dsp_alango_loopback_test_alive);

    //play
    uint16_t idx = 0;
    while (dsp_alango_loopback_test_alive) {
        if (rt_ringbuffer_data_len(rb) >= AUDIO_FRAME_SIZE_OF_LA_TEST) {
            rt_ringbuffer_get(rb, ply_buffer, ply_size);
            if (rt_device_write(ply_config->card, 0, ply_buffer, ply_abuf.period_size) <= 0) {
                rt_kprintf("Error playing sample\n");
                break;
            }
        } else {
            rt_thread_delay(1);
        }

        if (!rt_pin_read(RECOVERY_KEY) && idx == 1000) {
            dsp_alango_loopback_denoise_enable = dsp_alango_loopback_denoise_enable ? 0 : 1;
            idx                                = 0;
        } else {
            idx++;
            if (idx >= 1000) {
                idx = 1000;
            }
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

    //free ply src
#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN_SOFTVOL
    ply_ret = rt_device_control(card, RK_AUDIO_CTL_PLUGIN_RELEASE, (void*)ply_type);
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

static void do_ctl(void* arg)
{
    while (1) {
        if (!rt_pin_read(RECOVERY_KEY)) {
            dsp_alango_loopback_denoise_enable = dsp_alango_loopback_denoise_enable ? 0 : 1;
            rt_thread_mdelay(100);
        }
        if (dsp_alango_loopback_denoise_enable) {
            _tgpio_output(1, 4, 0);
        } else {
            _tgpio_output(1, 4, 3);
        }
        rt_thread_mdelay(10);
    }
}

/* Exported functions --------------------------------------------------------*/

uint8_t hl_dsp_test(int argc, char** argv)
{

    rt_thread_mdelay(3000);

    rt_thread_t cap_tid = RT_NULL;
    rt_thread_t ply_tid = RT_NULL;
    rt_thread_t ctl_tid = RT_NULL;
    rt_pin_mode(RECOVERY_KEY, PIN_MODE_INPUT);

    if (argc > 1) {
        if (!strcmp("start", argv[1])) {
            rt_kprintf("loopback chain test start\r\n");
        } else if (!strcmp("stop", argv[1])) {
            rt_kprintf("loopback chain test stop\r\n");
            dsp_alango_loopback_test_alive = 0;
            return 0;
        } else {
            // rt_kprintf("wrong parameter, please type: dsp_alango_loopback_test [start/stop]\r\n");
            // return 0;
        }
    } else {
        // rt_kprintf("wrong parameter, please type: dsp_alango_loopback_test [start/stop]\r\n");
        // return 0;
    }
    dsp_alango_loopback_test_alive = 1; 

    rb = rt_ringbuffer_create(RING_BUFFER_SIZE_OF_LA_TEST);
    RT_ASSERT(rb != RT_NULL);

    ply_tid = rt_thread_create("do_ply", do_ply, RT_NULL, 2048, RT_THREAD_PRIORITY_MAX / 2, 1);
    cap_tid = rt_thread_create("do_cap", do_cap, RT_NULL, 2048, RT_THREAD_PRIORITY_MAX / 2, 1);
    ctl_tid = rt_thread_create("do_ctl", do_ctl, RT_NULL, 2048, RT_THREAD_PRIORITY_MAX / 2, 1);

    if (ply_tid)
        rt_thread_startup(ply_tid);

    if (cap_tid)
        rt_thread_startup(cap_tid);

    return 0;
}

uint8_t hl_dsp_test_enable_denoise(int argc, char** argv)
{
    if (argc != 2) {
        return 1;
    }
    int mode = atoi(argv[1]);

    switch (mode) {
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

uint8_t test_gpio(int argc, char** argv)
{
    rt_pin_mode(RECOVERY_KEY, PIN_MODE_INPUT);
    // rt_pin_mode(PWM_LED, PIN_MODE_OUTPUT);

    rt_kprintf("revocery key now is %d\r\n", rt_pin_read(RECOVERY_KEY));
    rt_kprintf("LED off \r\n");
    // rt_pin_write(PWM_LED, PIN_HIGH);
    _tgpio_output(1, 4, 3);
    rt_thread_mdelay(3000);
    rt_kprintf("LED on \r\n");
    // rt_pin_write(PWM_LED, PIN_LOW);
    _tgpio_output(1, 4, 0);
    rt_thread_mdelay(3000);

    return 0;
}

// MSH_CMD_EXPORT(hl_dsp_test, hl_dsp_test);
// MSH_CMD_EXPORT(test_gpio, test_gpio);
MSH_CMD_EXPORT(hl_dsp_test_enable_denoise, 1 : enable | 0 : disable);

INIT_APP_EXPORT(hl_dsp_test);

#endif

/*
 * EOF
 */