/**
 * @file hl_mod_page_home.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-11-30
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
 * <tr><td>2022-11-30     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/

#include "hl_mod_page_common.h"

#if (!HL_IS_TX_DEVICE())
#include "hl_mod_display.h"
#include "lvgl.h"
#include "rtthread.h"
#include "hl_mod_page.h"
#include "page_test.h"
#include "page_top.h"
#include "hl_mod_input.h"
#include "lv_port_indev.h"
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

// static void hl_mod_rx_knob_key_pro(struct _lv_indev_drv_t* drv, lv_indev_data_t* data)
// {
//     // static uint32_t last_key       = 0;
//     // uint8_t key_event;

//     // key_event  = hl_mod_get_knob_okkey_val();
//     // if (key_event) {

//     //     switch (key_event) {

//     //         case HL_KEY_EVENT_SHORT:
//     //             last_key = LV_KEY_ENTER;
//     //             LV_LOG_USER("LV_KEY_ENTER\n");
//     //             data->state = LV_INDEV_STATE_PR;
//     //             data->key   = last_key;
//     //             break;
//     //         case HL_KEY_EVENT_LONG:
//     //             last_key = LV_KEY_NEXT;
//     //             LV_LOG_USER("LV_KEY_NEXT\n");
//     //             data->state = LV_INDEV_STATE_PR;
//     //             data->key   = last_key;
//     //             break;
//     //         default:
//     //             last_key    = 0;
//     //             data->state = LV_INDEV_STATE_REL;
//     //             data->key   = last_key;
//     //             LV_LOG_USER("def\n");
//     //             break;
//     //     }

//     // } else {
//     //     data->state = LV_INDEV_STATE_REL;
//     //     data->key   = last_key;
//     // }
// }

// void eventbtn_cb(lv_event_t * e)
// {
//     lv_event_code_t code = lv_event_get_code(e);
//     lv_obj_t*       obj  = lv_event_get_target(e);

//     if (code == LV_EVENT_KEY) {
//         // LV_LOG_USER("key= %d\r\n", lv_indev_get_key(hl_mod_get_okkey_indev()));
//         LV_LOG_USER("key=\n" );
//     }
// }

static hl_signal_int_t hl_mod_page_signal_deal(uint8_t data)
{
    uint8_t signal;
    signal = data / 25;
    if (data % 25) {
        signal += 1;
    }
    if (signal >= 4)
        signal = 4;
    return signal;
}
static void hl_mod_page_home_init(void)
{
    hl_lvgl_main_init_t data;
    hl_lvgl_top_init_t  top_init;
    hl_lvgl_top_ioctl_t ioctl_top;

    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();

    if (data_ptr->sys_status.tx1_net_switch && data_ptr->sys_status.tx2_net_switch) {
        data.display_tx_device = HL_DISPLAY_DOUBLE;

        data.tx_device_1.electric = data_ptr->tx1_bat_val;
        data.tx_device_1.signal   = hl_mod_page_signal_deal(data_ptr->tx1_signal);
        data.tx_device_1.record   = data_ptr->sys_status.tx1_record_state;
        data.tx_device_1.volume   = data_ptr->tx1_vu;

        data.tx_device_2.electric = data_ptr->tx2_bat_val;
        data.tx_device_2.signal   = hl_mod_page_signal_deal(data_ptr->tx2_signal);
        data.tx_device_2.record   = data_ptr->sys_status.tx2_record_state;
        data.tx_device_2.volume   = data_ptr->tx2_vu;

    } else if (data_ptr->sys_status.tx2_net_switch && !data_ptr->sys_status.tx1_net_switch) {
        data.display_tx_device    = HL_DISPLAY_TX2;
        data.tx_device_2.electric = data_ptr->tx2_bat_val;
        data.tx_device_2.signal   = hl_mod_page_signal_deal(data_ptr->tx2_signal);
        data.tx_device_2.record   = data_ptr->sys_status.tx2_record_state;
        data.tx_device_2.volume   = data_ptr->tx2_vu;

    } else if (data_ptr->sys_status.tx1_net_switch && !data_ptr->sys_status.tx2_net_switch) {
        data.display_tx_device    = HL_DISPLAY_TX1;
        data.tx_device_1.electric = data_ptr->tx1_bat_val;
        data.tx_device_1.signal   = hl_mod_page_signal_deal(data_ptr->tx1_signal);
        data.tx_device_1.record   = data_ptr->sys_status.tx1_record_state;
        data.tx_device_1.volume   = data_ptr->tx1_vu;
    } else {
        data.display_tx_device    = HL_DISPLAY_TX1;
        data.tx_device_1.electric = data_ptr->tx1_bat_val;
        data.tx_device_1.signal   = hl_mod_page_signal_deal(data_ptr->tx1_signal);
        data.tx_device_1.record   = data_ptr->sys_status.tx1_record_state;
        data.tx_device_1.volume   = data_ptr->tx1_vu;
    }

    hl_mod_main_init(&data);

    // 更新当前的RX电量信息
    top_init.electric_top = data_ptr->rx_bat_val;
    hl_mod_top_init(&top_init);

    if (data_ptr->sound_module == STEREO) {
        ioctl_top.top_cmd   = HL_TOP_ADD_ICON_CMD;
        ioctl_top.top_param = HL_TOP_ICON_VOICE_MOD;
        hl_mod_top_ioctl(&ioctl_top);
    }

    

    if (data_ptr->sys_status.tx1_noise && data_ptr->sys_status.tx2_noise) {
        ioctl_top.top_cmd   = HL_TOP_ADD_ICON_CMD;
        ioctl_top.top_param = HL_TOP_ICON_NOISE;
        hl_mod_top_ioctl(&ioctl_top);
    }

    if (data_ptr->sys_status.screen_lock) {
        ioctl_top.top_cmd   = HL_TOP_ADD_ICON_CMD;
        ioctl_top.top_param = HL_TOP_ICON_LOCK;
        hl_mod_top_ioctl(&ioctl_top);
    }

    if (data_ptr->sys_status.line_out_in) {
        ioctl_top.top_cmd   = HL_TOP_ADD_ICON_CMD;
        ioctl_top.top_param = HL_TOP_ICON_LINEOUT;
        hl_mod_top_ioctl(&ioctl_top);
    }

    if (data_ptr->sys_status.usb_in) {
        ioctl_top.top_cmd   = HL_TOP_ADD_ICON_CMD;
        ioctl_top.top_param = HL_TOP_ICON_TYPEC;
        hl_mod_top_ioctl(&ioctl_top);
    }

    if (data_ptr->sys_status.monitor_in) {
        ioctl_top.top_cmd   = HL_TOP_ADD_ICON_CMD;
        ioctl_top.top_param = HL_TOP_ICON_HEATSET;
        hl_mod_top_ioctl(&ioctl_top);
    }
}
static void hl_mod_page_top_update(hl_display_screen_change_s* flag, hl_display_screen_s* now)
{

    hl_lvgl_top_ioctl_t ioctl_top;
    if (flag->rx_bat_val) {
        ioctl_top.top_cmd      = HL_TOP_BAT_VAL;
        ioctl_top.electric_top = now->rx_bat_val;

        flag->rx_bat_val = 0;
    }
    // 降噪一起的
    if (now->sys_status.tx1_noise == now->sys_status.tx2_noise) {
        if (flag->sys_status.tx1_noise) {

            ioctl_top.top_cmd   = (hl_top_cmd_t)now->sys_status.tx1_noise;
            ioctl_top.top_param = HL_TOP_ICON_NOISE;
            hl_mod_top_ioctl(&ioctl_top);

            flag->sys_status.tx1_noise = 0;
        }
    }

    // 锁屏
    if (flag->sys_status.screen_lock) {

        ioctl_top.top_cmd   = (hl_top_cmd_t)now->sys_status.screen_lock;
        ioctl_top.top_param = HL_TOP_ICON_LOCK;
        hl_mod_top_ioctl(&ioctl_top);
        flag->sys_status.screen_lock = 0;
    }

    // line out
    if (flag->sys_status.line_out_in) {

        ioctl_top.top_cmd   = (hl_top_cmd_t)now->sys_status.line_out_in;
        ioctl_top.top_param = HL_TOP_ICON_LINEOUT;
        hl_mod_top_ioctl(&ioctl_top);
        flag->sys_status.line_out_in = 0;
    }

    // usb
    if (flag->sys_status.usb_in) {
        ioctl_top.top_cmd   = (hl_top_cmd_t)now->sys_status.usb_in;
        ioctl_top.top_param = HL_TOP_ICON_TYPEC;
        hl_mod_top_ioctl(&ioctl_top);
        flag->sys_status.usb_in = 0;
    }
    // 耳机插入
    if (flag->sys_status.monitor_in) {

        ioctl_top.top_cmd   = (hl_top_cmd_t)now->sys_status.monitor_in;
        ioctl_top.top_param = HL_TOP_ICON_LINEOUT;
        hl_mod_top_ioctl(&ioctl_top);
        flag->sys_status.monitor_in = 0;
    }
}

static void hl_mod_page_home_tx1_update(hl_display_screen_change_s* flag, hl_display_screen_s* now)
{
    hl_lvgl_main_ioctl_t data1;

    if (flag->tx1_bat_val) {
        data1.cmd                  = HL_CHANGE_TX1_ELEC;
        data1.tx_device_1.electric = now->tx1_bat_val;
        hl_mod_main_ioctl(&data1);

        flag->tx1_bat_val = 0;
    }

    if (flag->tx1_signal) {
        data1.cmd                = HL_CHANGE_TX1_SIGNAL;
        data1.tx_device_1.signal = hl_mod_page_signal_deal(now->tx1_signal);
        ;
        hl_mod_main_ioctl(&data1);

        flag->tx1_signal = 0;
    }

    //vu值
    if (flag->tx1_vu) {
        data1.cmd                = HL_CHANGE_TX1_VOL;
        data1.tx_device_1.volume = now->tx1_vu;
        hl_mod_main_ioctl(&data1);

        flag->tx1_vu = 0;
    }

    if (flag->sys_status.tx1_record_state) {
        data1.cmd                = HL_CHANGE_TX1_REC;
        data1.tx_device_1.record = (hl_record_status_t)now->sys_status.tx1_record_state;
        hl_mod_main_ioctl(&data1);

        flag->sys_status.tx1_record_state = 0;
    }
}

static void hl_mod_page_home_tx2_update(hl_display_screen_change_s* flag, hl_display_screen_s* now)
{
    hl_lvgl_main_ioctl_t data2;

    if (flag->tx2_bat_val) {
        data2.cmd                  = HL_CHANGE_TX2_ELEC;
        data2.tx_device_2.electric = now->tx2_bat_val;
        hl_mod_main_ioctl(&data2);

        flag->tx2_bat_val = 0;
    }

    if (flag->tx2_signal) {
        data2.cmd                = HL_CHANGE_TX2_SIGNAL;
        data2.tx_device_2.signal = hl_mod_page_signal_deal(now->tx2_signal);
        hl_mod_main_ioctl(&data2);

        flag->tx2_signal = 0;
    }

    //vu值
    if (flag->tx2_vu) {
        data2.cmd                = HL_CHANGE_TX2_VOL;
        data2.tx_device_2.volume = now->tx2_vu;
        hl_mod_main_ioctl(&data2);

        flag->tx2_vu = 0;
    }

    if (flag->sys_status.tx2_record_state) {
        data2.cmd                = HL_CHANGE_TX2_REC;
        data2.tx_device_2.record = (hl_record_status_t)now->sys_status.tx2_record_state;
        hl_mod_main_ioctl(&data2);

        flag->sys_status.tx2_record_state = 0;
    }
}

static void hl_mod_page_home_update(void)
{

    hl_lvgl_main_ioctl_t data;
    // 这一次的数据
    hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();

    // TX显示个数的的处理
    if (flag->sys_status.tx1_net_switch || flag->sys_status.tx2_net_switch) {

        //两个
        if (data_ptr->sys_status.tx1_net_switch && data_ptr->sys_status.tx2_net_switch) {
            data.cmd = HL_CHANGE_DOUBLE_DEVICE;
            hl_mod_main_ioctl(&data);
            hl_mod_page_home_tx1_update(flag, data_ptr);
            hl_mod_page_home_tx2_update(flag, data_ptr);
        } else if (data_ptr->sys_status.tx1_net_switch && !data_ptr->sys_status.tx2_net_switch) {
            // 一个tx1
            data.cmd = HL_CHANGE_TX1_DEVICE;
            hl_mod_main_ioctl(&data);
            hl_mod_page_home_tx1_update(flag, data_ptr);
        } else if (!data_ptr->sys_status.tx1_net_switch && data_ptr->sys_status.tx2_net_switch) {
            // 一个tx2
            data.cmd = HL_CHANGE_TX2_DEVICE;
            hl_mod_main_ioctl(&data);
            hl_mod_page_home_tx2_update(flag, data_ptr);
        }
        flag->sys_status.tx1_net_switch = 0;
        flag->sys_status.tx2_net_switch = 0;
    } else {
        hl_mod_page_home_tx1_update(flag, data_ptr);
        hl_mod_page_home_tx2_update(flag, data_ptr);
    }

    hl_mod_page_top_update(flag, data_ptr);
}

static void hl_mod_page_setup(void)
{
    hl_mod_page_home_init();
}

static void hl_mod_page_exit(void)
{
    hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();

       //删除主界面
    if (data_ptr->sys_status.tx1_net_switch && data_ptr->sys_status.tx2_net_switch) {
        hl_lvgl_main_ioctl_t main_ctl = {
            .cmd = HL_CHANGE_DELETE_DOUBLE,
        };
        hl_mod_main_ioctl(&main_ctl);

    } else if (data_ptr->sys_status.tx2_net_switch && !data_ptr->sys_status.tx1_net_switch) {
        hl_lvgl_main_ioctl_t main_ctl = {
            .cmd = HL_CHANGE_DELETE_TX2,
        };
        hl_mod_main_ioctl(&main_ctl);

    } else if (data_ptr->sys_status.tx1_net_switch && !data_ptr->sys_status.tx2_net_switch) {
        hl_lvgl_main_ioctl_t main_ctl = {
            .cmd = HL_CHANGE_DELETE_TX1,
        };
        hl_mod_main_ioctl(&main_ctl);
    } else {
        //删除主界面
        hl_lvgl_main_ioctl_t main_ctl = {
            .cmd = HL_CHANGE_DELETE_TX1,

        };
        hl_mod_main_ioctl(&main_ctl);
    }

    //删除TOP
    hl_lvgl_top_ioctl_t ctl_data = {
        .top_cmd = HL_TOP_ALL_DEL,
    };
    hl_mod_top_ioctl(&ctl_data);

    // hl_mod_page_delete(lv_scr_act());
    // uint8_t i = 0;

    // lv_obj_del(area_tx1);
    // lv_obj_del(area_tx2);
    // lv_obj_del(voice_bar_tx1);
    // lv_obj_del(voice_bar_tx2);
    // lv_obj_del(power_bar_tx1);
    // lv_obj_del(power_bar_tx2);
    // lv_obj_del(voice_img_tx1);
    // lv_obj_del(voice_img_tx2);
    // lv_obj_del(power_img_tx1);
    // lv_obj_del(power_img_tx2);
    // lv_obj_del(voice_lab_tx1);
    // lv_obj_del(voice_lab_tx2);
    // lv_obj_del(power_lab_tx1);
    // lv_obj_del(power_lab_tx2);
    // lv_obj_del(device_lab_tx1);
    // lv_obj_del(device_lab_tx2);
    // lv_obj_del(video_dot_tx1);
    // lv_obj_del(video_dot_tx2);

    // for (i = 0; i < 5; i++) {
    //     lv_obj_del(tx1_signal_obj[i]);
    //     lv_obj_del(tx2_signal_obj[i]);
    // }
}

// static void hl_mod_page_event(void* btn, int event)
// {

// }

static void hl_mod_page_run(void)
{
    uint8_t key_event;

    //单机确定键进入菜单
    key_event = hl_mod_get_knob_okkey_val();

    if (key_event == HL_KEY_EVENT_SHORT) {
        PageManager_PagePush(PAGE_MAIN_MENU);
    }

    // 单机触摸键进入快捷界面
    hl_mod_menu_goto_fast_config_scan();

    // 快捷设置
    hl_mod_menu_goto_quickset_scan();

    // 菜单的页面更新
    hl_mod_page_home_update();
}

// void hl_mod_page_home_init(void)
PAGE_DEC(PAGE_HOME)
{
    bool result;

    result = PageManager_PageRegister(PAGE_HOME, hl_mod_page_setup, hl_mod_page_run, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_HOME init fail\n");
    }
}
#endif
/*
 * EOF
 */