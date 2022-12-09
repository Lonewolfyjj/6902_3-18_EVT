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

static void hl_mod_page_setup(void)
{
    hl_lvgl_main_init_t data = {
        .display_tx_device    = HL_DISPLAY_DOUBLE,
        .tx_device_1.electric = 20,
        .tx_device_1.signal   = HL_ONE_SIGNAL,
        .tx_device_1.record   = HL_RECODING,
        .tx_device_1.volume   = -13,

        .tx_device_2.electric = 56,
        .tx_device_2.signal   = HL_TWO_SIGNAL,
        .tx_device_2.record   = HL_NO_RECODE,
        .tx_device_2.volume   = 17,
    };
    hl_mod_main_init(&data);

    hl_lvgl_main_ioctl_t main_ctl = {
        .cmd                = HL_CHANGE_TX2_SIGNAL,
        .tx_device_2.signal = HL_FOUR_SIGNAL,
    };
    
    // keypad_knob_ok_update(hl_mod_rx_knob_key_pro);
    // encode_knob_update(hl_mod_rx_knob_val_pro);

    hl_mod_main_ioctl(&main_ctl);
    // hl_mod_page_event_btn_init(eventbtn_cb);
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_main_ioctl_t main_ctl = {
        .cmd                = HL_CHANGE_DELETE_PAGE,
        .tx_device_2.signal = HL_FOUR_SIGNAL,
    };
    hl_mod_main_ioctl(&main_ctl);
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

    key_event  = hl_mod_get_knob_okkey_val();

    if (key_event == HL_KEY_EVENT_SHORT) {
        PageManager_PagePush(PAGE_MAIN_MENU);
    } else if (key_event == HL_KEY_EVENT_LONG) {
    //     PageManager_PagePush(0);
    }
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