/**
 * @file hl_mod_rx_display.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-12-07
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
 * <tr><td>2022-12-07     <td>v1.0     <td>liujie     <td>内容
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

#if !HL_IS_TX_DEVICE()

#include "hl_drv_aw2016a.h"
#include "hl_drv_rm690a0.h"
#include "hl_util_msg_type.h"
#include "hl_hal_gpio.h"
#include "hl_mod_page_common.h"
#include <rtthread.h>
#include "hl_util_msg_type.h"

#define DBG_SECTION_NAME "display"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

#define DISPLAY_THREAD_PRIORITY 18
#define DISPLAY_THREAD_STACK_SIZE 20480
#define DISPLAY_THREAD_TIMESLICE 5

// 单位 毫秒
#define RTHEAD_DELAY_TIME LV_DISP_DEF_REFR_PERIOD

static rt_thread_t display_tid = RT_NULL;

typedef struct _hl_display_msg_t
{
    /// 消息队列句柄
    rt_mq_t msg_hander;
    ///  消息
    mode_to_app_msg_t msg;

} hl_display_msg_t;

static hl_display_msg_t hl_mod_display;

uint8_t hl_mod_display_out_task()
{
    return HL_DISPLAY_SUCCESS;
}
static void hl_mod_display_data_init(void)
{
    hl_display_screen_s* now = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag = hl_mod_page_get_screen_change_flag();

    rt_memset(flag,0,sizeof(*flag));
    rt_memset(now,0,sizeof(*now));


    now->display_fault_code = 0;
    now->monitor_category = MONITOR_TX_IN; //默认TX输入
    now->voice_module = HIGH_FIDELITY;
    now->sound_module = STEREO;//默认立体声

    now->low_cut = LOW_CUT_OFF;
    now->page_id = PAGE_NONE;

     now->tx1_bat_val = 0;
     now->tx2_bat_val = 0;
     now->rx_bat_val = 0;
     now->case_bat_val = 0;
     now->tx1_vu = 0;
     now->tx2_vu = 0;
     now->tx1_signal = 0;
     now->tx2_signal = 0;
     now->tx_noise_level = 4;
     now->tx1_line_out_volume = 10;
     now->tx2_line_out_volume = 10;
     now->uac_in_volume = 0;
     now->uac_out_volume = 10;
     now->tx1_gain_volume = 10;
     now->tx2_gain_volume =  10;
     now->led_britness = 127;
     now->tx1_remained_record_time = 10;
     now->tx2_remained_record_time = 10;
     now->ota_upgrade_progress = 0;
     //后续要改
     rt_sprintf(now->rx_ver,"%s",A6902_VERSION);
     rt_sprintf(now->tx1_ver,"%s","V0.0.0.0");
     rt_sprintf(now->tx2_ver,"%s","V0.0.0.0");
     rt_sprintf(now->sn,"%s","00000000000000000000");
    //  now->sn[36]; 0
    //  now->device_fault_code[20]; 0
     now->sys_status.auto_record = 0;
     now->sys_status.tx1_net_switch = 1;
     now->sys_status.tx2_net_switch = 1;

    //  now->sys_status.tx1_net_switch = 0;
    //  now->sys_status.tx2_net_switch = 0;

    now->sys_status.tx1_charge_status = 0;
}





uint8_t hl_mod_display_io_ctrl(uint8_t cmd, void* ptr, uint16_t len)
{
    uint8_t              res    = HL_DISPLAY_SUCCESS;
    hl_display_screen_s* data_p = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag = hl_mod_page_get_screen_change_flag();
    switch (cmd) {
        // 更新按鍵的操作
        case MSG_INPUT_UPDATE_CMD: {
            mode_to_app_msg_t msg = { 0 };
            msg                   = *(mode_to_app_msg_t*)ptr;
            hl_mod_indev_val_get(&msg);
        } break;

        case NOW_PAGE_GET_READ_CMD: {
            *(uint8_t *)ptr = hl_mod_display_scr_get_page();
        } break;
        // TX1电量
        case TX1_BAT_VAL_VAL_CMD: {
            uint8_t data        = *(uint8_t*)ptr;
            data_p->tx1_bat_val = data;
            flag->tx1_bat_val = 1;

        } break;
        case TX2_BAT_VAL_VAL_CMD: {
            uint8_t data        = *(uint8_t*)ptr;
            data_p->tx2_bat_val = data;
            flag->tx2_bat_val  = 1;
        } break;
        case RX_BAT_VAL_VAL_CMD: {
            uint8_t data       = *(uint8_t*)ptr;

            data_p->rx_bat_val = data;
            flag->rx_bat_val = 1;
        } break;
        case CASE_BAT_VAL_VAL_CMD: {
            uint8_t data         = *(uint8_t*)ptr;
            data_p->case_bat_val = data;
            flag->case_bat_val = 1;
        } break;

        case TX1_VU_VAL_CMD: {
            uint8_t data   = *(uint8_t*)ptr;
            data_p->tx1_vu = data;
            flag->tx1_vu = 1;
        } break;
        case TX2_VU_VAL_CMD: {
            uint8_t data   = *(uint8_t*)ptr;
            data_p->tx2_vu = data;
            flag->tx2_vu = 1;
        } break;
        // 信号强度 uint8_t
        case TX1_SIGNAL_VAL_CMD: {
            uint8_t data       = *(uint8_t*)ptr;
            data_p->tx1_signal = data;
            flag->tx1_signal = 1;
        } break;
        case TX2_SIGNAL_VAL_CMD: {
            uint8_t data       = *(uint8_t*)ptr;
            data_p->tx2_signal = data;
            flag->tx2_signal = 1;
        } break;
        // TX12降噪开关  1： 开 0 ：关
        case TX1_NOISE_SWITCH_CMD: {
            uint8_t data                 = *(uint8_t*)ptr;
            data_p->sys_status.tx1_noise = data;
            flag->sys_status.tx1_noise = 1;
        } break;
        case TX2_NOISE_SWITCH_CMD: {
            uint8_t data                 = *(uint8_t*)ptr;
            data_p->sys_status.tx2_noise = data;
            flag->sys_status.tx2_noise = 1;
        } break;
        case TX1_RECORD_STATE_SWITCH_CMD: {
            uint8_t data                        = *(uint8_t*)ptr;
            data_p->sys_status.tx1_record_state = data;
            flag->sys_status.tx1_record_state = 1;
        } break;
        case TX2_RECORD_STATE_SWITCH_CMD: {
            uint8_t data                        = *(uint8_t*)ptr;
            data_p->sys_status.tx2_record_state = data;
            flag->sys_status.tx2_record_state = 1;
        } break;
        case TX1_CHARGE_STATUS_SWITCH_CMD: {
            uint8_t data                        = *(uint8_t*)ptr;
            data_p->sys_status.tx1_charge_status = data;
            flag->sys_status.tx1_charge_status = 1;
        } break;
        case TX2_CHARGE_STATUS_SWITCH_CMD: {
            uint8_t data                        = *(uint8_t*)ptr;
            data_p->sys_status.tx2_charge_status = data;
            flag->sys_status.tx2_charge_status = 1;
        } break;
        case RX_CHARGE_STATUS_SWITCH_CMD: {
            uint8_t data                        = *(uint8_t*)ptr;
            data_p->sys_status.rx_charge_status = data;
            flag->sys_status.rx_charge_status = 1;
        } break;
        case BOX_CHARGE_STATUS_SWITCH_CMD: {
            uint8_t data                        = *(uint8_t*)ptr;
            data_p->sys_status.box_charge_status = data;
            flag->sys_status.box_charge_status = 1;
        } break;
        case USB_IN_SWITCH_CMD: {
            uint8_t data                        = *(uint8_t*)ptr;
            data_p->sys_status.usb_in = data;
            flag->sys_status.usb_in = 1;
        } break;
        case LINE_OUT_IN_SWITCH_CMD: {
            uint8_t data                        = *(uint8_t*)ptr;
            data_p->sys_status.line_out_in = data;
            flag->sys_status.line_out_in = 1;
        } break;
        case MONITOR_IN_SWITCH_CMD: {
            uint8_t data                        = *(uint8_t*)ptr;
            data_p->sys_status.monitor_in = data;
            flag->sys_status.monitor_in = 1;
        } break;
        case TX1_MUTE_SWITCH_SWITCH_CMD: {
            uint8_t data                        = *(uint8_t*)ptr;
            data_p->sys_status.tx1_mute_switch = data;
            flag->sys_status.tx1_mute_switch = 1;
        } break;
        case TX2_MUTE_SWITCH_SWITCH_CMD: {
            uint8_t data                        = *(uint8_t*)ptr;
            data_p->sys_status.tx2_mute_switch = data;
            flag->sys_status.tx2_mute_switch = 1;
        } break;
        case IN_BOX_STATE_SWITCH_CMD: {
            uint8_t data                        = *(uint8_t*)ptr;
            data_p->sys_status.in_box_state = data;
            flag->sys_status.in_box_state = 1;
        } break;
        case AUTO_RECORD_PORTECT_SWITCH_CMD: {
            uint8_t data                        = *(uint8_t*)ptr;
            data_p->sys_status.auto_record_portect = data;
            flag->sys_status.auto_record_portect = 1;
        } break;
        // case TX1_NET_SWITCH_CMD: {
        //     uint8_t data                        = *(uint8_t*)ptr;
        //     data_p->sys_status.tx1_net_switch = data;
        //     flag->sys_status.tx1_net_switch = 1;
        // } break;
        // case TX2_NET_SWITCH_CMD: {
        //     uint8_t data                        = *(uint8_t*)ptr;
        //     data_p->sys_status.tx2_net_switch = data;
        //     flag->sys_status.tx2_net_switch = 1;
        // } break;
        default:
            LOG_D("cmd=%d\r\n", cmd);
            break;
    }

    return res;
}

// RX
static void hl_mod_display_task(void* param)
{

    while (1) {

        PageManager_Running();
        // rt_thread_mdelay(RTHEAD_DELAY_TIME);
        lv_task_handler();
        rt_thread_mdelay(LV_DISP_DEF_REFR_PERIOD);
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

uint8_t hl_mod_display_init(void* display_msq)
{
    uint32_t frambufferaddr;

    if (display_msq == NULL) {
        LOG_E("msghander err!");
        return HL_DISPLAY_FAILED;
    }

    hl_mod_display.msg_hander = (rt_mq_t)display_msq;

    hl_drv_aw2016a_deinit();
    // hl_drv_aw2016a_init();

    // RX
    hl_drv_rm690a0_init();
    lvgl2rtt_init();
    hl_mod_display_data_init();
    hl_mod_page_all_init();
    
    display_tid = rt_thread_create("display_thread", hl_mod_display_task, RT_NULL, DISPLAY_THREAD_STACK_SIZE,
                                   DISPLAY_THREAD_PRIORITY, DISPLAY_THREAD_TIMESLICE);

    if (display_tid != RT_NULL) {

        LOG_D("display thread init ok!\r\n");
        rt_thread_startup(display_tid);

        return HL_DISPLAY_SUCCESS;
    }

    return HL_DISPLAY_FAILED;
}
#endif
