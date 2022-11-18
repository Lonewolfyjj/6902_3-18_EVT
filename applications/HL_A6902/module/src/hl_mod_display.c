/**
 * @file hl_mod_display.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-11-11
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
 * <tr><td>2022-11-11     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/*
 * EOF
 */
#include "hl_mod_display.h"
#include "hl_drv_aw2016a.h"
#include "hl_drv_rm690a0.h"
#include "hl_util_msg_type.h"
#include "hl_hal_gpio.h"
#include <rtthread.h>

#define HL_PRINT rt_kprintf

#define DISPLAY_THREAD_PRIORITY 25
#define DISPLAY_THREAD_STACK_SIZE 512
#define DISPLAY_THREAD_TIMESLICE 5

// 单位 毫秒
#define RTHEAD_DELAY_TIME 5

#if HL_GET_DEVICE_TYPE()
#define HL_MOD_TX_RECORD_LED HL_DRV_AW2016A_LED1
#endif

#define HL_MOD_TX_STATE_LED HL_DRV_AW2016A_LED0

#define HL_BLUE_CHANNEL HL_DRV_AW2016A_LED_CHANNEL3
#define HL_RED_CHANNEL HL_DRV_AW2016A_LED_CHANNEL1
#define HL_ORANGE_CHANNEL HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2
#define HL_GREEN_CHANNEL HL_DRV_AW2016A_LED_CHANNEL2
#define HL_ALL_CHANNEL HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3

static rt_thread_t display_tid = RT_NULL;

typedef struct _hl_display_msg_t
{
    /// 消息队列句柄
    rt_mq_t msg_hander;
    ///  消息
    mode_to_app_msg_t msg;

} hl_display_msg_t;

#if HL_GET_DEVICE_TYPE()
// TX
typedef struct _hl_display_led_t
{
    uint32_t last_led_mode;

    uint32_t last_record_led_mode;

    uint32_t current_led_mode;

    uint32_t current_record_led_mode;

    uint32_t sound_channel;

} hl_display_led_t;
#else
// RX

static uint8_t now_color = RGB888_BLACK;
static uint8_t last_now_color = RGB888_BLACK;

typedef struct _hl_display_led_t
{
    uint32_t last_led_mode;

    uint32_t current_led_mode;

} hl_display_led_t;


#endif

static hl_display_led_t hl_display_led;

static hl_display_msg_t hl_mod_display;

static uint8_t hl_mod_display_led_mode_get(uint32_t* now_led_mode);

static void hl_mod_display_led_set(void);

static int  hl_mod_display_led_init(uint8_t led_num);
static void hl_mod_display_led_color_set(uint8_t lednum, uint8_t red, uint8_t green, uint8_t blue);


// HL_DISPLAY_SUCCESS 表示当前有新的led命令下达
static uint8_t hl_mod_display_led_mode_get(uint32_t* now_led_mode)
{

    if (hl_display_led.last_led_mode != hl_display_led.current_led_mode) {
        hl_display_led.last_led_mode = hl_display_led.current_led_mode;
        *now_led_mode                = hl_display_led.current_led_mode;
        HL_PRINT("display: led new mode\r\n");

        return HL_DISPLAY_SUCCESS;
    } else {
        return HL_DISPLAY_FAILED;
    }
}

uint8_t hl_mod_display_out_task()
{
    return HL_DISPLAY_SUCCESS;
}




static int hl_mod_display_led_init(uint8_t led_num)
{
    int                            ret;
    uint8_t                        chip_id;
    uint8_t                        work_mode;
    uint8_t                        led_chan;
    hl_drv_aw2016a_pwm_level_st    pwm_param;
    hl_drv_aw2016a_breath_param_st pattern_param;

    ret = hl_drv_aw2016a_ctrl(led_num, HL_DRV_AW2016A_GET_CHIP_ID, &chip_id, sizeof(chip_id));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    work_mode = HL_DRV_AW2016A_ACTIVE_MODE;
    ret       = hl_drv_aw2016a_ctrl(led_num, HL_DRV_AW2016A_SET_WORK_MODE, &work_mode, sizeof(work_mode));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    work_mode = HL_DRV_AW2016A_IMAX_5MA;
    ret = hl_drv_aw2016a_ctrl(led_num, HL_DRV_AW2016A_SET_GLOBAL_MAX_OUTPUT_CURRENT, &work_mode, sizeof(work_mode));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    ret      = hl_drv_aw2016a_ctrl(led_num, HL_DRV_AW2016A_OPEN_LED_CHANNEL, &led_chan, sizeof(led_chan));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    ret      = hl_drv_aw2016a_ctrl(led_num, HL_DRV_AW2016A_SET_PATTERN_MODE, &led_chan, sizeof(led_chan));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int hl_mod_display_led_long_bright(uint8_t led_num, uint8_t color)
{
    int     ret;
    uint8_t led_chan;

    led_chan = HL_ALL_CHANNEL;

    ret = hl_drv_aw2016a_ctrl(led_num, HL_DRV_AW2016A_CLOSE_LED_CHANNEL, &led_chan, sizeof(led_chan));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_ALL_CHANNEL;

    ret = hl_drv_aw2016a_ctrl(led_num, HL_DRV_AW2016A_OPEN_LED_CHANNEL, &led_chan, sizeof(led_chan));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_ALL_CHANNEL;

    ret = hl_drv_aw2016a_ctrl(led_num, HL_DRV_AW2016A_SET_MANUAL_MODE, &led_chan, sizeof(led_chan));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = hl_drv_aw2016a_ctrl(led_num, HL_DRV_AW2016A_OPEN_LED_CHANNEL, &led_chan, sizeof(led_chan));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int hl_mod_display_led_slow_flash(uint8_t led_num, uint8_t color)
{
    int                            ret;
    uint8_t                        chip_id;
    uint8_t                        work_mode;
    uint8_t                        led_chan;
    hl_drv_aw2016a_pwm_level_st    pwm_param;
    hl_drv_aw2016a_breath_param_st pattern_param;

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    ret      = hl_drv_aw2016a_ctrl(led_num, HL_DRV_AW2016A_OPEN_LED_CHANNEL, &led_chan, sizeof(led_chan));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    ret      = hl_drv_aw2016a_ctrl(led_num, HL_DRV_AW2016A_SET_PATTERN_MODE, &led_chan, sizeof(led_chan));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 0;
    pattern_param.t1       = 9;
    pattern_param.t2       = 3;
    pattern_param.t3       = 9;
    pattern_param.t4       = 0;
    ret = hl_drv_aw2016a_ctrl(led_num, HL_DRV_AW2016A_SET_BREATH_PARAM, &pattern_param, sizeof(pattern_param));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pwm_param.led_chan  = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    pwm_param.pwm_level = 50;
    ret = hl_drv_aw2016a_ctrl(led_num, HL_DRV_AW2016A_SET_LED_CHANNEL_PWM_LEVEL, &pwm_param, sizeof(pwm_param));

    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int hl_mod_display_led_fast_flash(uint8_t led_num, uint8_t color)
{
    int                            ret;
    uint8_t                        chip_id;
    uint8_t                        work_mode;
    uint8_t                        led_chan;
    hl_drv_aw2016a_pwm_level_st    pwm_param;
    hl_drv_aw2016a_breath_param_st pattern_param;

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    ret      = hl_drv_aw2016a_ctrl(led_num, HL_DRV_AW2016A_OPEN_LED_CHANNEL, &led_chan, sizeof(led_chan));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    ret      = hl_drv_aw2016a_ctrl(led_num, HL_DRV_AW2016A_SET_PATTERN_MODE, &led_chan, sizeof(led_chan));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 0;
    pattern_param.t1       = 4;
    pattern_param.t2       = 2;
    pattern_param.t3       = 4;
    pattern_param.t4       = 0;
    ret = hl_drv_aw2016a_ctrl(led_num, HL_DRV_AW2016A_SET_BREATH_PARAM, &pattern_param, sizeof(pattern_param));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pwm_param.led_chan  = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    pwm_param.pwm_level = 50;
    ret = hl_drv_aw2016a_ctrl(led_num, HL_DRV_AW2016A_SET_LED_CHANNEL_PWM_LEVEL, &pwm_param, sizeof(pwm_param));

    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

#if HL_GET_DEVICE_TYPE()
// TX有录制灯

static uint8_t hl_mod_display_record_led_mode_get(uint32_t* now_led_mode)
{

    if (hl_display_led.last_record_led_mode != hl_display_led.current_record_led_mode) {
        hl_display_led.last_record_led_mode = hl_display_led.current_record_led_mode;
        *now_led_mode                       = hl_display_led.current_record_led_mode;
        HL_PRINT("display: record led new mode\r\n");

        return HL_DISPLAY_SUCCESS;
    } else {
        return HL_DISPLAY_FAILED;
    }
}

static void hl_mod_display_led_set(void)
{
    uint32_t now_led_mode;
    uint32_t now_record_led_mode;

    if (HL_DISPLAY_SUCCESS == hl_mod_display_record_led_mode_get(&now_record_led_mode)) {
        switch (now_record_led_mode) {
            case RECORD_LED_MODE_OPEN:
                hl_mod_display_led_long_bright(HL_MOD_TX_RECORD_LED, HL_RED_CHANNEL);
                hl_mod_display_led_color_set(HL_MOD_TX_RECORD_LED, 15, 0, 0);

                break;
            case RECORD_LED_MODE_CLOSE:
                hl_mod_display_led_long_bright(HL_MOD_TX_RECORD_LED, HL_ALL_CHANNEL);
                hl_mod_display_led_color_set(HL_MOD_TX_RECORD_LED, 0, 0, 0);

                break;
            default:
                break;
        }
    }

    if (HL_DISPLAY_SUCCESS == hl_mod_display_led_mode_get(&now_led_mode)) {

        switch (now_led_mode) {
            case LED_MODE_PAIR:
                HL_PRINT("ch3 = %d\r\n", hl_display_led.sound_channel);
                if (hl_display_led.sound_channel == LEFT_SOUND_CHANNEL) {
                    hl_mod_display_led_fast_flash(HL_MOD_TX_STATE_LED, HL_BLUE_CHANNEL);
                    hl_mod_display_led_color_set(HL_MOD_TX_STATE_LED, 0, 0, 15);
                } else {
                    hl_mod_display_led_fast_flash(HL_MOD_TX_STATE_LED, HL_RED_CHANNEL);
                    hl_mod_display_led_color_set(HL_MOD_TX_STATE_LED, 15, 0, 0);
                }
                break;
            case LED_MODE_CONNECTED:
                if (hl_display_led.sound_channel == LEFT_SOUND_CHANNEL) {
                    hl_mod_display_led_long_bright(HL_MOD_TX_STATE_LED, HL_BLUE_CHANNEL);
                    hl_mod_display_led_color_set(HL_MOD_TX_STATE_LED, 0, 0, 15);
                } else {
                    hl_mod_display_led_long_bright(HL_MOD_TX_STATE_LED, HL_RED_CHANNEL);
                    hl_mod_display_led_color_set(HL_MOD_TX_STATE_LED, 15, 0, 0);
                }

                break;
            case LED_MODE_RECONNECTION:
                HL_PRINT("ch2 = %d\r\n", hl_display_led.sound_channel);
                if (hl_display_led.sound_channel == LEFT_SOUND_CHANNEL) {
                    hl_mod_display_led_slow_flash(HL_MOD_TX_STATE_LED, HL_BLUE_CHANNEL);
                    hl_mod_display_led_color_set(HL_MOD_TX_STATE_LED, 0, 0, 15);
                } else {
                    hl_mod_display_led_slow_flash(HL_MOD_TX_STATE_LED, HL_RED_CHANNEL);
                    hl_mod_display_led_color_set(HL_MOD_TX_STATE_LED, 15, 0, 0);
                }
                break;
            case LED_MODE_DENOISE_CONNECTED:
                HL_PRINT("ch = %d\r\n", hl_display_led.sound_channel);
                if (hl_display_led.sound_channel == LEFT_SOUND_CHANNEL) {
                    hl_mod_display_led_long_bright(HL_MOD_TX_STATE_LED, HL_GREEN_CHANNEL);
                    hl_mod_display_led_color_set(HL_MOD_TX_STATE_LED, 0, 15, 0);
                } else {
                    hl_mod_display_led_long_bright(HL_MOD_TX_STATE_LED, HL_ORANGE_CHANNEL);
                    hl_mod_display_led_color_set(HL_MOD_TX_STATE_LED, 12, 1, 0);
                }
                break;
            case LED_MODE_DENOISE_RECONNECTION:

                if (hl_display_led.sound_channel == LEFT_SOUND_CHANNEL) {
                    hl_mod_display_led_slow_flash(HL_MOD_TX_STATE_LED, HL_GREEN_CHANNEL);
                    hl_mod_display_led_color_set(HL_MOD_TX_STATE_LED, 0, 15, 0);
                } else {
                    hl_mod_display_led_slow_flash(HL_MOD_TX_STATE_LED, HL_ORANGE_CHANNEL);
                    hl_mod_display_led_color_set(HL_MOD_TX_STATE_LED, 12, 1, 0);
                }
                break;
            default:
                break;
        }
    }
}

static void hl_mod_display_led_color_set(uint8_t lednum, uint8_t red, uint8_t green, uint8_t blue)
{
    int                     ret;
    hl_drv_aw2016a_light_st output_param;

    output_param.b          = red;
    output_param.g          = green;
    output_param.r          = blue;
    output_param.brightness = 30;
    ret = hl_drv_aw2016a_ctrl(lednum, HL_DRV_AW2016A_SET_LED_LIGHT_EFFECT, &output_param, sizeof(output_param));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }
}

uint8_t hl_mod_display_io_ctrl(uint8_t cmd, void* ptr, uint16_t len)
{
    uint8_t res = HL_DISPLAY_SUCCESS;

    switch (cmd) {
        case MSG_STATE_LED_MODE_CMD: {
            hl_led_mode val = *(hl_led_mode*)ptr;
            if (val >= LED_MODE_ID_CNT) {
                HL_PRINT("[%s][line:%d] cmd(%d) err!!! \r\n", __FUNCTION__, __LINE__, cmd);
                res = HL_DISPLAY_FAILED;
            } else {
                hl_display_led.current_led_mode = val;
                HL_PRINT("[%s][line:%d] val(%d)!!! \r\n", __FUNCTION__, __LINE__, val);
            }
        }
        break;
        case MSG_RECORD_LED_MODE_CMD: {
            hl_record_led_mode val = *(hl_record_led_mode*)ptr;
            if (val >= RECORD_LED_MODE_ID_CNT) {
                HL_PRINT("[%s][line:%d] cmd(%d) err!!! \r\n", __FUNCTION__, __LINE__, cmd);
                res = HL_DISPLAY_FAILED;
            } else {
                hl_display_led.current_record_led_mode = val;
                HL_PRINT("[%s][line:%d] val(%d)!!! \r\n", __FUNCTION__, __LINE__, val);
            }
        }
        break;
        case MSG_SOUND_CHANNEL_CMD: {
            uint32_t val                 = *(uint32_t*)ptr;
            hl_display_led.sound_channel = val;
            HL_PRINT("[%s][line:%d] val(%d)!!! \r\n", __FUNCTION__, __LINE__, val);
        }
        break;
        default:
            break;
    }
    HL_PRINT("cmd=%d,*ptr=%d\r\n", cmd, *(uint32_t*)ptr);
    return res;
}
#else

static void hl_mod_display_led_set(void)
{
    uint32_t now_led_mode;

    if (HL_DISPLAY_SUCCESS == hl_mod_display_led_mode_get(&now_led_mode)) {

        switch (now_led_mode) {
            case LED_MODE_PAIR:
                hl_mod_display_led_fast_flash(HL_MOD_TX_STATE_LED, HL_BLUE_CHANNEL);
                hl_mod_display_led_color_set(HL_MOD_TX_STATE_LED, 0, 0, 15);
                break;
            case LED_MODE_CONNECTED:
                hl_mod_display_led_long_bright(HL_MOD_TX_STATE_LED, HL_BLUE_CHANNEL);
                hl_mod_display_led_color_set(HL_MOD_TX_STATE_LED, 0, 0, 15);
                break;
            case LED_MODE_RECONNECTION:
                hl_mod_display_led_slow_flash(HL_MOD_TX_STATE_LED, HL_BLUE_CHANNEL);
                hl_mod_display_led_color_set(HL_MOD_TX_STATE_LED, 0, 0, 15);
                break;
            default:
                break;
        }
    }
}
static uint8_t hl_mod_display_screen_color_get(uint32_t* now_screen_mode)
{

    if (last_now_color != now_color) {
        last_now_color = now_color;
        *now_screen_mode                = now_color;
        HL_PRINT("display: screen new mode\r\n");

        return HL_DISPLAY_SUCCESS;
    } else {
        return HL_DISPLAY_FAILED;
    }
}
static void hl_mod_display_screen_set(void)
{
    uint8_t now_screen_mode;

    if (HL_DISPLAY_SUCCESS == hl_mod_display_screen_color_get(&now_screen_mode)) {

        hl_drv_rm690a0_write(0, MIPI_OLED_WIDTH - 1, 0, MIPI_OLED_HEIGHT - 1, (const uint8_t*)&now_screen_mode);

    }
}
static void hl_mod_display_led_color_set(uint8_t lednum, uint8_t red, uint8_t green, uint8_t blue)
{
    int                     ret;
    hl_drv_aw2016a_light_st output_param;

    output_param.r          = red;
    output_param.g          = green;
    output_param.b          = blue;
    output_param.brightness = 30;
    ret = hl_drv_aw2016a_ctrl(lednum, HL_DRV_AW2016A_SET_LED_LIGHT_EFFECT, &output_param, sizeof(output_param));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }
}

uint8_t hl_mod_display_io_ctrl(uint8_t cmd, void* ptr, uint16_t len)
{
    uint8_t res = HL_DISPLAY_SUCCESS;

    switch (cmd) {
        case MSG_STATE_LED_MODE_CMD: {
            hl_led_mode val = *(hl_led_mode*)ptr;
            if (val >= LED_MODE_ID_CNT) {
                HL_PRINT("[%s][line:%d] cmd(%d) err!!! \r\n", __FUNCTION__, __LINE__, cmd);
                res = HL_DISPLAY_FAILED;
            } else {
                hl_display_led.current_led_mode = val;
                HL_PRINT("[%s][line:%d] val(%d)!!! \r\n", __FUNCTION__, __LINE__, val);
            }
        }
        break;
        case MSG_OLED_COLOR_CHANGE_CMD: {
            hl_screen_color_e val = *(uint32_t*)ptr;
            if (val >= RGB888_COLOR_CNT) {
                HL_PRINT("[%s][line:%d] cmd(%d) err!!! \r\n", __FUNCTION__, __LINE__, cmd);
                res = HL_DISPLAY_FAILED;
            } else {
                now_color = val;
                HL_PRINT("[%s][line:%d] val(%d)!!! \r\n", __FUNCTION__, __LINE__, val);
            }
        }
        break;
        default:
            break;
    }
    HL_PRINT("cmd=%d,*ptr=%d\r\n", cmd, *(uint32_t*)ptr);
    return res;
}
#endif

#if HL_GET_DEVICE_TYPE()
// TX
static void hl_mod_display_task(void* param)
{

    while (1) {

        hl_mod_display_led_set();
        rt_thread_mdelay(RTHEAD_DELAY_TIME);
    }
}

uint8_t hl_mod_display_deinit(void)
{

    if (display_tid != RT_NULL) {
        rt_thread_delete(display_tid);
    }

    hl_drv_aw2016a_deinit();

    // TX
    hl_hal_gpio_init(GPIO_PWR_EN);
    hl_hal_gpio_low(GPIO_PWR_EN);

    return HL_DISPLAY_SUCCESS;
}

#else
// RX
static void hl_mod_display_task(void* param)
{

    while (1) {

        hl_mod_display_led_set();

        hl_mod_display_screen_set();
        rt_thread_mdelay(RTHEAD_DELAY_TIME);
    }
}

uint8_t hl_mod_display_deinit(void)
{
    if (display_tid != RT_NULL) {
        rt_thread_delete(display_tid);
    }
    hl_drv_aw2016a_deinit();
    hl_drv_rm690a0_deinit();

    return HL_DISPLAY_SUCCESS;
}

#endif

#if HL_GET_DEVICE_TYPE()
//TX
#else
//RX
#endif

uint8_t hl_mod_display_init(void* display_msq)
{

    if (display_msq == NULL) {
        HL_PRINT("msghander err!");
        return HL_DISPLAY_FAILED;
    }

    hl_mod_display.msg_hander = (rt_mq_t)display_msq;

    hl_drv_aw2016a_deinit();
    hl_drv_aw2016a_init();

#if HL_GET_DEVICE_TYPE()
    // TX
    hl_hal_gpio_init(GPIO_PWR_EN);
    hl_hal_gpio_high(GPIO_PWR_EN);
#else
    // RX
    hl_drv_rm690a0_init();
    now_color = RGB888_BLACK;
    last_now_color = RGB888_BLACK;
    hl_drv_rm690a0_write(0, MIPI_OLED_WIDTH - 1, 0, MIPI_OLED_HEIGHT - 1, (const uint8_t*)&now_color);
#endif

#if HL_GET_DEVICE_TYPE()
    //TX
    hl_display_led.last_led_mode           = LED_MODE_PAIR;
    hl_display_led.last_record_led_mode    = RECORD_LED_MODE_CLOSE;
    hl_display_led.current_led_mode        = LED_MODE_PAIR;
    hl_display_led.current_record_led_mode = RECORD_LED_MODE_CLOSE;
    hl_display_led.sound_channel           = LEFT_SOUND_CHANNEL;

    hl_mod_display_led_init(HL_MOD_TX_RECORD_LED);
    hl_mod_display_led_init(HL_MOD_TX_STATE_LED);

    hl_mod_display_led_fast_flash(HL_MOD_TX_STATE_LED, HL_BLUE_CHANNEL);
    hl_mod_display_led_long_bright(HL_MOD_TX_RECORD_LED, HL_ALL_CHANNEL);
    hl_mod_display_led_color_set(HL_MOD_TX_RECORD_LED, 0, 0, 0);
    hl_mod_display_led_color_set(HL_MOD_TX_STATE_LED, 0, 0, 15);

#else
    //RX 默认的模式
    hl_display_led.last_led_mode           = LED_MODE_PAIR;
    hl_display_led.current_led_mode = LED_MODE_PAIR;
    hl_mod_display_led_init(HL_MOD_TX_STATE_LED);
    hl_mod_display_led_fast_flash(HL_MOD_TX_STATE_LED, HL_BLUE_CHANNEL);
    hl_mod_display_led_color_set(HL_MOD_TX_STATE_LED, 0, 0, 15);

#endif

    display_tid = rt_thread_create("display_thread", hl_mod_display_task, RT_NULL, DISPLAY_THREAD_STACK_SIZE,
                                   DISPLAY_THREAD_PRIORITY, DISPLAY_THREAD_TIMESLICE);

    if (display_tid != RT_NULL) {

        HL_PRINT("display thread init ok!\r\n");
        rt_thread_startup(display_tid);

        return HL_DISPLAY_SUCCESS;
    }

    return HL_DISPLAY_FAILED;
}