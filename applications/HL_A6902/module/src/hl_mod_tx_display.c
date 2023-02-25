/**
 * @file hl_mod_tx_display.c
 * @author lilin (lin.li@hollyland-tech.com)
 * @brief 
 * @version 1.0
 * @date 2022-12-08
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
 * <tr><td>2022-12-08     <td>v1.0     <td>lilin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/

#include "hl_mod_display.h"
#include <stdbool.h>

#include "hl_drv_aw2016a.h"
#include "hl_hal_gpio.h"
#include "hl_util_nvram.h"

#define DBG_SECTION_NAME "display"
#define DBG_LEVEL DBG_INFO
#include "rtdbg.h"

#if HL_IS_TX_DEVICE()
// TX

/* typedef -------------------------------------------------------------------*/

typedef enum _hl_mod_display_state_e
{
    HL_DISPLAY_IDLE,
    HL_DISPLAY_IN_BOX,
    HL_DISPLAY_CHARGING,
    HL_DISPLAY_CHARGE_FULL,
    HL_DISPLAY_CHARGE_STOP,
    HL_DISPLAY_FAULT,
    HL_DISPLAY_UPDATE,
    HL_DISPLAY_PAIR,
    HL_DISPLAY_CONNECTED_DENOISE,
    HL_DISPLAY_RECONNECT_DENOISE,
    HL_DISPLAY_CONNECTED,
    HL_DISPLAY_RECONNECT,
    HL_DISPLAY_LOWPOWER,
    HL_DISPLAY_MUTE,
    HL_DISPLAY_RECORD,
    HL_DISPLAY_RECORD_STOP,
    HL_DISPLAY_RECORD_ERR,
} hl_mod_display_state_e;

typedef struct _hl_mod_display_st
{
    bool                   init_flag;
    bool                   aw2016a_init_flag;
    bool                   display_update_flag_1;
    bool                   display_update_flag_2;
    uint8_t                charging_led_count;
    hl_mod_display_state_e display_state_1;
    hl_mod_display_state_e display_state_2;
    hl_led_net_mode        net_mode;
    hl_led_switch          in_box_state;
    hl_led_switch          record_state;
    hl_led_switch          record_fault_state;
    hl_led_switch          denoise_state;
    hl_charge_mode         charge_state;
    hl_led_switch          fault_state;
    hl_led_switch          mute_state;
    hl_led_switch          update_state;
    uint8_t                bat_soc;
    uint8_t                led_brightness;
    void*                  msg_hd;
    rt_thread_t            display_thread;
    int                    thread_exit_flag;
} hl_mod_display_st;

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/

static hl_mod_display_st _display_mod = {
    .init_flag         = false,
    .aw2016a_init_flag = false,
    .msg_hd            = NULL,
};

/* Private function(only *.c)  -----------------------------------------------*/

static void hl_mod_display_dump_info(void)
{
    LOG_I("init_flag:%d", _display_mod.init_flag);
    LOG_I("aw2016a_init_flag:%d", _display_mod.aw2016a_init_flag);
    LOG_I("display_state_1:%d", _display_mod.display_state_1);
    LOG_I("display_state_2:%d", _display_mod.display_state_2);
    LOG_I("net_mode:%d", _display_mod.net_mode);
    LOG_I("in_box_state:%d", _display_mod.in_box_state);
    LOG_I("record_state:%d", _display_mod.record_state);
    LOG_I("record_fault_state:%d", _display_mod.record_fault_state);
    LOG_I("denoise_state:%d", _display_mod.denoise_state);
    LOG_I("charge_state:%d", _display_mod.charge_state);
    LOG_I("fault_state:%d", _display_mod.fault_state);
    LOG_I("mute_state:%d", _display_mod.mute_state);
    LOG_I("update_state:%d", _display_mod.update_state);
    LOG_I("bat_soc:%d", _display_mod.bat_soc);
    LOG_I("led_brightness:%d", _display_mod.led_brightness);
}

static void _display_state_check_1(void)
{
    hl_mod_display_state_e state;

    if (_display_mod.in_box_state == SWITCH_OPEN) {
        state = HL_DISPLAY_IN_BOX;
    } else if (_display_mod.charging_led_count != 0 && _display_mod.charge_state == CHARGING) {
        state = HL_DISPLAY_CHARGING;
        _display_mod.charging_led_count--;
    } else if (_display_mod.fault_state == SWITCH_OPEN) {
        state = HL_DISPLAY_FAULT;
    } else if (_display_mod.update_state == SWITCH_OPEN) {
        state = HL_DISPLAY_UPDATE;
    } else if (_display_mod.net_mode == LED_NET_MODE_PAIR) {
        state = HL_DISPLAY_PAIR;
    } else if (_display_mod.bat_soc <= 6 && _display_mod.charge_state != CHARGING) {
        state = HL_DISPLAY_LOWPOWER;
    } else if (_display_mod.mute_state == SWITCH_OPEN) {
        state = HL_DISPLAY_MUTE;
    } else if (_display_mod.net_mode == LED_NET_MODE_CONNECTED && _display_mod.denoise_state == SWITCH_OPEN) {
        state = HL_DISPLAY_CONNECTED_DENOISE;
    } else if (_display_mod.net_mode == LED_NET_MODE_RECONNECTION && _display_mod.denoise_state == SWITCH_OPEN) {
        state = HL_DISPLAY_RECONNECT_DENOISE;
    } else if (_display_mod.net_mode == LED_NET_MODE_CONNECTED) {
        state = HL_DISPLAY_CONNECTED;
    } else if (_display_mod.net_mode == LED_NET_MODE_RECONNECTION) {
        state = HL_DISPLAY_RECONNECT;
    } else if ((_display_mod.bat_soc >= 95 && _display_mod.charge_state != NOT_CHARGE)
               || _display_mod.charge_state == FULL_CHARGE) {  //此处开始是关机的情况
        state = HL_DISPLAY_CHARGE_FULL;
    } else if (_display_mod.charge_state == NOT_CHARGE) {
        state = HL_DISPLAY_CHARGE_STOP;
    } else if (_display_mod.charge_state == CHARGING) {
        state = HL_DISPLAY_CHARGING;
    } else {
        state = HL_DISPLAY_IDLE;
    }

    if (state != _display_mod.display_state_1) {
        _display_mod.display_state_1       = state;
        _display_mod.display_update_flag_1 = true;
    }
}

static void _display_update_1(void)
{
    if (_display_mod.display_update_flag_1 == false) {
        return;
    }

    hl_drv_aw2016a_led_ctrl_st led;

    switch (_display_mod.display_state_1) {
        case HL_DISPLAY_IN_BOX: {
            led.breath_mode = HL_DRV_AW2016A_BREATH_MODE_KEEP;
            led.color       = HL_DRV_AW2016A_COLOR_BLACK;
        } break;
        case HL_DISPLAY_FAULT: {
            led.breath_mode = HL_DRV_AW2016A_BREATH_MODE_RB_FAST;
            led.color       = HL_DRV_AW2016A_COLOR_WHITE;
        } break;
        case HL_DISPLAY_UPDATE: {
            led.breath_mode = HL_DRV_AW2016A_BREATH_MODE_KEEP;
            led.color       = HL_DRV_AW2016A_COLOR_PINK;
        } break;
        case HL_DISPLAY_PAIR: {
            led.breath_mode = HL_DRV_AW2016A_BREATH_MODE_FAST;
            led.color       = HL_DRV_AW2016A_COLOR_BLUE;
        } break;
        case HL_DISPLAY_LOWPOWER: {
            led.breath_mode = HL_DRV_AW2016A_BREATH_MODE_SLOW;
            led.color       = HL_DRV_AW2016A_COLOR_RED;
        } break;
        case HL_DISPLAY_MUTE: {
            led.breath_mode = HL_DRV_AW2016A_BREATH_MODE_KEEP;
            led.color       = HL_DRV_AW2016A_COLOR_RED;
        } break;
        case HL_DISPLAY_CONNECTED_DENOISE: {
            led.breath_mode = HL_DRV_AW2016A_BREATH_MODE_KEEP;
            led.color       = HL_DRV_AW2016A_COLOR_GREEN;
        } break;
        case HL_DISPLAY_RECONNECT_DENOISE: {
            led.breath_mode = HL_DRV_AW2016A_BREATH_MODE_SLOW;
            led.color       = HL_DRV_AW2016A_COLOR_GREEN;
        } break;
        case HL_DISPLAY_CONNECTED: {
            led.breath_mode = HL_DRV_AW2016A_BREATH_MODE_KEEP;
            led.color       = HL_DRV_AW2016A_COLOR_BLUE;
        } break;
        case HL_DISPLAY_RECONNECT: {
            led.breath_mode = HL_DRV_AW2016A_BREATH_MODE_SLOW;
            led.color       = HL_DRV_AW2016A_COLOR_BLUE;
        } break;
        case HL_DISPLAY_CHARGING: {
            led.breath_mode = HL_DRV_AW2016A_BREATH_MODE_SLOW;
            led.color       = HL_DRV_AW2016A_COLOR_ORANGE;
        } break;
        case HL_DISPLAY_CHARGE_FULL: {
            led.breath_mode = HL_DRV_AW2016A_BREATH_MODE_KEEP;
            led.color       = HL_DRV_AW2016A_COLOR_ORANGE;
        } break;
        case HL_DISPLAY_CHARGE_STOP: {
            led.breath_mode = HL_DRV_AW2016A_BREATH_MODE_KEEP;
            led.color       = HL_DRV_AW2016A_COLOR_BLACK;
        } break;
        default: {
            led.breath_mode = HL_DRV_AW2016A_BREATH_MODE_SKIP;
            led.color       = HL_DRV_AW2016A_COLOR_BLACK;
        } break;
    }

    hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_LED_CTRL, &led, sizeof(led));

    _display_mod.display_update_flag_1 = false;
}

static void _display_state_check_2(void)
{
    hl_mod_display_state_e state;

    if (_display_mod.in_box_state == SWITCH_OPEN) {
        state = HL_DISPLAY_IN_BOX;
    } else if (_display_mod.record_fault_state == SWITCH_OPEN) {
        state = HL_DISPLAY_RECORD_ERR;
    } else if (_display_mod.record_state == SWITCH_OPEN) {
        state = HL_DISPLAY_RECORD;
    } else if (_display_mod.record_state == SWITCH_CLOSE) {
        state = HL_DISPLAY_RECORD_STOP;
    } else {
        state = HL_DISPLAY_IDLE;
    }

    if (state != _display_mod.display_state_2) {
        _display_mod.display_state_2       = state;
        _display_mod.display_update_flag_2 = true;
    }
}

static void _display_update_2(void)
{
    if (_display_mod.display_update_flag_2 == false) {
        return;
    }

    switch (_display_mod.display_state_2) {
        case HL_DISPLAY_IN_BOX: {
            hl_hal_gpio_low(GPIO_REC_LED_EN);
        } break;
        case HL_DISPLAY_RECORD_ERR: {
            // do nothing
        } break;
        case HL_DISPLAY_RECORD: {
            hl_hal_gpio_high(GPIO_REC_LED_EN);
        } break;
        case HL_DISPLAY_RECORD_STOP: {
            hl_hal_gpio_low(GPIO_REC_LED_EN);
        } break;
        default: {
            hl_hal_gpio_low(GPIO_REC_LED_EN);
        } break;
    }

    _display_mod.display_update_flag_2 = false;
}

static int _set_brightness(void)
{
    int                                 ret;
    hl_drv_aw2016a_max_output_current_e imax;

    if (_display_mod.led_brightness == 30) {
        imax = HL_DRV_AW2016A_IMAX_15MA;
    } else {
        imax = HL_DRV_AW2016A_IMAX_5MA;
    }

    ret =
        hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_GLOBAL_MAX_OUTPUT_CURRENT, &imax, sizeof(uint8_t));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return HL_DISPLAY_FAILED;
    }

    return HL_DISPLAY_SUCCESS;
}

static void _record_led_flash_ctrl(void)
{
    static uint8_t count = 0;

    if (_display_mod.display_state_2 == HL_DISPLAY_RECORD_ERR) {
        if (count == 0) {
            hl_hal_gpio_high(GPIO_REC_LED_EN);
        } else if (count == 5) {
            hl_hal_gpio_low(GPIO_REC_LED_EN);
        } else if (count == 10) {
            count = 0;
            return;
        }

        count++;
    } else {
        count = 0;
    }
}

static void _aw2016a_drv_status_check(void)
{
    int                          ret;
    static uint8_t               count = 0;
    hl_drv_aw2016a_chip_status_e chip_status;

    if (count == 0) {
        if (_display_mod.aw2016a_init_flag == true) {
            ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_CHECK_CHIP_STATUS, &chip_status,
                                      sizeof(chip_status));
            if (ret == AW2016A_FUNC_RET_ERR || chip_status != 0) {
                _display_mod.aw2016a_init_flag = false;
            }
        } else {
            ret = hl_drv_aw2016a_deinit();
            if (ret == AW2016A_FUNC_RET_OK) {
                ret = hl_drv_aw2016a_init();
            }

            if (ret == AW2016A_FUNC_RET_OK) {
                _display_mod.aw2016a_init_flag     = true;
                _display_mod.display_update_flag_1 = true;
            }
        }
        count = 10;
    } else {
        count--;
    }
}

static void _display_thread_entry(void* arg)
{
    while (_display_mod.thread_exit_flag == 0) {
        _display_state_check_1();
        _display_update_1();  //状态灯

        _display_state_check_2();
        _display_update_2();  //录制灯

        _record_led_flash_ctrl();

        _aw2016a_drv_status_check();

        rt_thread_mdelay(100);
    }

    _display_mod.thread_exit_flag = -1;
}

/* Exported functions --------------------------------------------------------*/

uint8_t hl_mod_display_init(void* display_msq)
{
    if (_display_mod.init_flag == true) {
        return HL_DISPLAY_SUCCESS;
    }

    int ret;

    ret = hl_drv_aw2016a_init();
    if (ret == AW2016A_FUNC_RET_ERR) {
        _display_mod.aw2016a_init_flag = false;
    } else {
        _display_mod.aw2016a_init_flag = true;
    }

    hl_hal_gpio_init(GPIO_REC_LED_EN);

    hl_util_nvram_param_get_integer("LED_BRIGHTNESS", &_display_mod.led_brightness, 30);
    _set_brightness();

    _display_mod.display_update_flag_1 = false;
    _display_mod.display_update_flag_2 = false;
    _display_mod.net_mode              = LED_NET_MODE_ID_CNT;
    _display_mod.in_box_state          = SWITCH_CLOSE;
    _display_mod.record_state          = SWITCH_CLOSE;
    _display_mod.record_fault_state    = SWITCH_CLOSE;
    _display_mod.denoise_state         = SWITCH_CLOSE;
    _display_mod.charge_state          = NOT_CHARGE;
    _display_mod.fault_state           = SWITCH_CLOSE;
    _display_mod.mute_state            = SWITCH_CLOSE;
    _display_mod.update_state          = SWITCH_CLOSE;
    _display_mod.bat_soc               = 50;
    _display_mod.display_state_1       = HL_DISPLAY_IDLE;
    _display_mod.display_state_2       = HL_DISPLAY_IDLE;
    _display_mod.charging_led_count    = 0;
    _display_mod.msg_hd                = display_msq;

    _display_mod.thread_exit_flag = 0;

    _display_mod.display_thread =
        rt_thread_create("hl_mod_display_thread", _display_thread_entry, RT_NULL, 1024, 15, 5);
    if (_display_mod.display_thread == RT_NULL) {
        LOG_E("display thread create failed");
        return HL_DISPLAY_FAILED;
    }

    rt_thread_startup(_display_mod.display_thread);

    LOG_I("display mod init success!");

    _display_mod.init_flag = true;

    return HL_DISPLAY_SUCCESS;
}

uint8_t hl_mod_display_deinit(void)
{
    if (_display_mod.init_flag == false) {
        return HL_DISPLAY_SUCCESS;
    }

    int ret;

    _display_mod.thread_exit_flag = 1;

    LOG_I("wait display thread exit");

    while (_display_mod.thread_exit_flag != -1) {
        rt_thread_mdelay(10);
    }

    LOG_I("display deinit success!");

    hl_drv_aw2016a_deinit();

    hl_hal_gpio_low(GPIO_REC_LED_EN);

    LOG_I("display mod deinit success!");

    _display_mod.init_flag = false;

    return HL_DISPLAY_SUCCESS;
}

uint8_t hl_mod_display_io_ctrl(uint8_t cmd, void* ptr, uint16_t len)
{
    if (_display_mod.init_flag == false) {
        LOG_W("display mod not init!");
        return HL_DISPLAY_FAILED;
    }

    int ret;

    switch (cmd) {
        case LED_FAULT_STATUS_READ_CMD: {
            if (len != sizeof(uint8_t)) {
                LOG_E("len err, ctrl arg need <uint8_t> type pointer!");
                return HL_DISPLAY_FAILED;
            }

            if (_display_mod.aw2016a_init_flag == true) {
                *(uint8_t*)ptr = 0;
            } else {
                *(uint8_t*)ptr = 1;
            }
        } break;
        case LED_NET_MODLE_CMD: {
            if (len != sizeof(hl_led_net_mode)) {
                LOG_E("len err, ctrl arg need <hl_led_net_mode> type pointer!");
                return HL_DISPLAY_FAILED;
            }

            _display_mod.net_mode = *(hl_led_net_mode*)ptr;
        } break;
        case LED_RECORD_STATE_CMD: {
            if (len != sizeof(hl_led_switch)) {
                LOG_E("len err, ctrl arg need <hl_led_switch> type pointer!");
                return HL_DISPLAY_FAILED;
            }

            _display_mod.record_state = *(hl_led_switch*)ptr;
        } break;
        case LED_SWITCH_NOISE_REDUCTION_CMD: {
            if (len != sizeof(hl_led_switch)) {
                LOG_E("len err, ctrl arg need <hl_led_switch> type pointer!");
                return HL_DISPLAY_FAILED;
            }

            _display_mod.denoise_state = *(hl_led_switch*)ptr;
        } break;
        case LED_CHARGE_STATUS_CMD: {
            if (len != sizeof(hl_charge_mode)) {
                LOG_E("len err, ctrl arg need <hl_charge_mode> type pointer!");
                return HL_DISPLAY_FAILED;
            }

            if (_display_mod.charge_state != *(hl_charge_mode*)ptr) {
                _display_mod.charge_state = *(hl_charge_mode*)ptr;
                if (_display_mod.charge_state == CHARGING) {
                    _display_mod.charging_led_count = 50;
                }
            }
        } break;
        case LED_SWITCH_FAULT_CMD: {
            if (len != sizeof(hl_led_switch)) {
                LOG_E("len err, ctrl arg need <hl_led_switch> type pointer!");
                return HL_DISPLAY_FAILED;
            }

            _display_mod.fault_state = *(hl_led_switch*)ptr;
        } break;
        case LED_SWITCH_MUTE_CMD: {
            if (len != sizeof(hl_led_switch)) {
                LOG_E("len err, ctrl arg need <hl_led_switch> type pointer!");
                return HL_DISPLAY_FAILED;
            }

            _display_mod.mute_state = *(hl_led_switch*)ptr;
        } break;
        case LED_SWITCH_UPDATE_CMD: {
            if (len != sizeof(hl_led_switch)) {
                LOG_E("len err, ctrl arg need <hl_led_switch> type pointer!");
                return HL_DISPLAY_FAILED;
            }

            _display_mod.update_state = *(hl_led_switch*)ptr;
        } break;
        case LED_BATTERY_VAL_CMD: {
            if (len != sizeof(uint8_t)) {
                LOG_E("len err, ctrl arg need <uint8_t> type pointer!");
                return HL_DISPLAY_FAILED;
            }

            _display_mod.bat_soc = *(uint8_t*)ptr;
        } break;
        case LED_BRIGHT_SET_CMD: {
            if (len != sizeof(uint8_t)) {
                LOG_E("len err, ctrl arg need <uint8_t> type pointer!");
                return HL_DISPLAY_FAILED;
            }

            _display_mod.led_brightness = *(uint8_t*)ptr;

            hl_util_nvram_param_set_integer("LED_BRIGHTNESS", _display_mod.led_brightness);
            hl_util_nvram_param_save();

            ret = _set_brightness();
            if (ret == HL_DISPLAY_FAILED) {
                return HL_DISPLAY_FAILED;
            }
        } break;
        case LED_RECORD_FAULT_CMD: {
            if (len != sizeof(hl_led_switch)) {
                LOG_E("len err, ctrl arg need <hl_led_switch> type pointer!");
                return HL_DISPLAY_FAILED;
            }

            _display_mod.record_fault_state = *(hl_led_switch*)ptr;
        } break;
        case LED_IN_BOX_SET_CMD: {
            if (len != sizeof(hl_led_switch)) {
                LOG_E("len err, ctrl arg need <hl_led_switch> type pointer!");
                return HL_DISPLAY_FAILED;
            }

            _display_mod.charging_led_count = 0;
            _display_mod.in_box_state       = *(hl_led_switch*)ptr;
        } break;
        default:
            break;
    }

    return HL_DISPLAY_SUCCESS;
}

MSH_CMD_EXPORT(hl_mod_display_dump_info, dump display mod all info);

#endif
/*
 * EOF
 */
