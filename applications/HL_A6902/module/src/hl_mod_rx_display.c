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
#include "hl_drv_qma6100p.h"
#include "hl_util_timeout.h"

#define DBG_SECTION_NAME "display"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

#define DISPLAY_THREAD_PRIORITY 15
#define DISPLAY_THREAD_STACK_SIZE 20480
#define DISPLAY_THREAD_TIMESLICE 5

// 单位 毫秒
#define RTHEAD_DELAY_TIME LV_DISP_DEF_REFR_PERIOD

#define GSENSOR_DEBANCE_TIMER          ROT_SCAN_IN_TIME*2

#define ROT_SCAN_IN_TIME 5*40

static hl_timeout_t rot_scan_in;

// gsensor消抖时间
static hl_timeout_t sensor_debance;

static rt_thread_t display_tid = RT_NULL;



static void hl_mod_screen_rot_scan(void);

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
     now->tx1_line_out_volume = 0;
     now->tx2_line_out_volume = 0;
     now->uac_in_volume = 0;
     now->uac_out_volume = 0;
     now->tx1_gain_volume = 0;
     now->tx2_gain_volume =  0;
     now->led_britness = 127;
     now->tx1_remained_record_time = 10;
     now->tx2_remained_record_time = 10;
     now->ota_upgrade_progress = 0;
    //  //后续要改
    //  rt_sprintf(now->rx_ver,"%s",A6902_VERSION);
    //  rt_sprintf(now->tx1_ver,"%s","V0.0.0.0");
    //  rt_sprintf(now->tx2_ver,"%s","V0.0.0.0");
    //  rt_sprintf(now->sn,"%s","00000000000000000000");
    //  now->sn[36]; 0
    //  now->device_fault_code[20]; 0
     now->sys_status.auto_record = 0;
     now->rf_net_connect = 0;


    now->sys_status.tx1_charge_status = 0;
    LOG_D("init_data\n");
}


static device_pose_t hl_mod_device_pose_val(void)
{
    euler_angle_t pose;
    hl_drv_qma6100p_io_ctrl(QMA6100_GET_EULWER_ANGLE, (void*)&pose, sizeof(euler_angle_t));
    if (pose.z > 0) {
        return DEVICE_REVERSE_POSE;
    } else {
        return DEVICE_FORWARD_POSE;
    }
}

static uint8_t hl_mod_device_dir_get(device_pose_t* newdir)
{
    static uint8_t last_stats = 0;
    // 默认正向
    static device_pose_t nowdir = DEVICE_FORWARD_POSE;
    device_pose_t        res;
    uint8_t              changeflag = 0;

    res = hl_mod_device_pose_val();

    switch (last_stats) {
        case 0:
            if (nowdir != res) {
                // 出现下降沿，重新计数
                hl_util_timeout_set(&sensor_debance, GSENSOR_DEBANCE_TIMER);
                last_stats = 1;
            }
            break;
        case 1:
            if (hl_util_timeout_judge(&sensor_debance) == RT_TRUE) {
                //20s内没有上升沿，按键按下
                if (nowdir != res) {

                    nowdir     = res;
                    *newdir    = nowdir;
                    last_stats = 0;
                    changeflag = 1;
                    hl_util_timeout_close(&sensor_debance);
                    LV_LOG_USER("-event_dir%d\n", nowdir);
                } else {
                    hl_util_timeout_close(&sensor_debance);
                    last_stats = 0;
                    changeflag = 0;
                }
            } else {
                // 出现上升沿，则清定时器
                if (res == nowdir) {
                    hl_util_timeout_close(&sensor_debance);
                    last_stats = 0;
                    changeflag = 0;
                }
            }
            break;
        default:
            changeflag = 0;
            break;
    }
    return changeflag;
}

static void         hl_mod_screen_rot_scan(void)
{
    device_pose_t now_dir = DEVICE_FORWARD_POSE;
    lv_disp_t*    screen_ptr;

    if (hl_util_timeout_judge(&rot_scan_in)) {

        if (hl_mod_device_dir_get(&now_dir)) {
            
            screen_ptr = lv_disp_get_default();
            if (now_dir == DEVICE_FORWARD_POSE) {
                LOG_D("rot=%d \n",now_dir);
                lv_disp_set_rotation(screen_ptr, LV_DISP_ROT_270);
            } else {
                lv_disp_set_rotation(screen_ptr, LV_DISP_ROT_90);
                LOG_D("rot=%d \n",now_dir);
            }
        }
        hl_util_timeout_set(&rot_scan_in, ROT_SCAN_IN_TIME);
    }
}

uint8_t hl_mod_display_io_ctrl(uint8_t cmd, void* ptr, uint16_t len)
{
    uint8_t              res    = HL_DISPLAY_SUCCESS;
    hl_display_screen_s* data_p = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag = hl_mod_page_get_screen_change_flag();
    // LOG_D("cmd=%d\r\n", cmd);
    hl_mod_display_mux_take();
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
        case IN_BOX_STATE_VAL_CMD: {
            uint8_t data                        = *(uint8_t*)ptr;
            data_p->in_box_state = data;
            flag->in_box_state = 1;
        } break;
        case AUTO_RECORD_PORTECT_SWITCH_CMD: {
            uint8_t data                        = *(uint8_t*)ptr;
            data_p->sys_status.auto_record_portect = data;
            flag->sys_status.auto_record_portect = 1;
        } break;
        case RX_RF_STATE_VAL_CMD: {
            uint8_t data                        = *(uint8_t*)ptr;
            data_p->rf_net_connect = data;
            flag->rf_net_connect = 1;
        } break;
        case LOW_CUT_VAL_CMD: {
            hl_display_low_cut_e data = *(uint8_t*)ptr;
            data_p->low_cut = data;
            flag->low_cut   = 1;
        } break;
        case OUT_BOX_CHARGER_SWITCH_CMD: {
            uint8_t data = *(uint8_t*)ptr;
            data_p->sys_status.out_box_poweroff_charge = data;
            flag->sys_status.out_box_poweroff_charge   = 1;
        } break;
        default:
            LOG_D("cmd=%d\r\n", cmd);
            break;
    }
    hl_mod_display_mux_release();
    return res;
}

// RX
static void hl_mod_display_task(void* param)
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_color_black());
    lv_style_set_border_width(&style, 0);
    lv_obj_add_style(lv_scr_act(), &style, 0);
    while (1) {
        hl_mod_screen_rot_scan();
        hl_mod_page_goto_box_scan();
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
    hl_mod_display_msq_set((rt_mq_t)display_msq);
    

    hl_drv_aw2016a_deinit();
    // hl_drv_aw2016a_init();
    
    // RX
    hl_drv_rm690a0_init();
    lvgl2rtt_init();
    // hl_mod_display_data_init();
    hl_mod_page_all_init();

    hl_mod_display_mux_init();

    hl_drv_qma6100p_init();
    hl_util_timeout_set(&rot_scan_in, ROT_SCAN_IN_TIME);
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
