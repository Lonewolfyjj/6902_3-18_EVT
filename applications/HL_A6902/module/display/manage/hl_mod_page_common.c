/**
 * @file hl_mod_page_common.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-12-01
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
 * <tr><td>2022-12-01     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "hl_mod_page_common.h"
#include "hl_mod_display_mng.h"
#include "hl_mod_input.h"

#include "hl_mod_page.h"
#include "lv_port_indev.h"
#include "lv_port_disp.h"

#if !HL_IS_TX_DEVICE()
/* typedef -------------------------------------------------------------------*/
#include "page_menu.h"
#include "lvgl.h"
/* define --------------------------------------------------------------------*/
#define DBG_SECTION_NAME "display"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

static hl_scr_in_data_t in_data = { 0 };
// static hl_scr_indev_msg_t  in_inputdev = {0};

void hl_mod_display_scr_set_page(uint32_t now_page)
{
    in_data.in_cmd.page_id = now_page;
}

hl_screen_page_e hl_mod_display_scr_get_page(void)
{
    return in_data.in_cmd.page_id;
}

static uint8_t hl_mod_display_scr_scan_incmd(uint32_t* now_screen_mode)
{
    static hl_scr_in_cmd_t last_in_cmd;
    hl_scr_in_cmd_t        now;

    now.page_id = hl_mod_display_scr_get_page();

    if (last_in_cmd.page_id != now.page_id) {
        last_in_cmd.page_id = now.page_id;
        *now_screen_mode    = now.page_id;
        LOG_E("scr new pageid(%d)\n", now.page_id);

        return HL_DISPLAY_SUCCESS;
    } else {
        return HL_DISPLAY_FAILED;
    }
}

//模外设置的代码
uint8_t hl_mod_display_scr_page_incmd(void)
{
    uint8_t now_page_id;

    if (HL_DISPLAY_SUCCESS == hl_mod_display_scr_scan_incmd(&now_page_id)) {
        if (false == PageManager_PagePush(now_page_id)) {
            return HL_DISPLAY_FAILED;
        } else {
            return HL_DISPLAY_SUCCESS;
        }
    }

    return HL_DISPLAY_SUCCESS;
}

static void delete (lv_obj_t* obj, bool obj_typ)
{
    uint32_t child_cnt = 0, i;
    child_cnt          = lv_obj_get_child_cnt(obj);
    if (child_cnt == 0) {
        lv_obj_del(obj);
    } else {
        for (i = 0; i < child_cnt; i++) {
            delete (lv_obj_get_child(obj, i), true);
        }
        if (obj_typ) {
            lv_obj_del(obj);
        }
    }
}
//使用方法

// // 递归删除子控件

void hl_mod_page_delete(lv_obj_t* obj)
{
    uint32_t  child_cnt = 0, i;
    lv_obj_t* obj1;
    // delete (obj, false);
    child_cnt = lv_obj_get_child_cnt(obj);
    LV_LOG_USER("child_cnt=%d\n", child_cnt);
    for (i = 0; i < child_cnt; i++) {
        obj1 = lv_obj_get_child(obj, i);
        LV_LOG_USER("obj1=%x\n", obj1);
        lv_obj_del(obj1);
    }
}

static int8_t now_knob_data;
void          hl_mod_rx_knob_val_pro(struct _lv_indev_drv_t* drv, lv_indev_data_t* data)
{
    static int8_t encoder_knob_diff = 0;

    if (in_data.in_inputdev.encoder_knob_diff != encoder_knob_diff) {

        LV_LOG_USER("encoder_knob=%d\r\n", in_data.in_inputdev.encoder_knob_diff);
        LV_LOG_USER("-now=%d\r\n", encoder_knob_diff);

        data->enc_diff    = in_data.in_inputdev.encoder_knob_diff - encoder_knob_diff;
        now_knob_data     = data->enc_diff;
        encoder_knob_diff = in_data.in_inputdev.encoder_knob_diff;
    }
}

int8_t hl_mod_get_rx_knob_val(void)
{
    int8_t data;
    data          = now_knob_data;
    now_knob_data = 0;
    return data;
}

void hl_mod_menu_knob_icon_change(uint8_t* center, uint8_t maxnum)
{
    int8_t now  = *center;
    int8_t data = hl_mod_get_rx_knob_val();
    if (data != 0) {
        now += data;
        if (now >= maxnum) {
            now = maxnum - 1;
        } else if (now < 0) {
            now = 0;
        }

        lv_set_icon_postion(now);
        *center = now;
    }
}

void hl_mod_page_event_btn_init(lv_event_cb_t event_cb)
{
    lv_obj_t* btn = lv_btn_create(lv_scr_act());
    // lv_obj_add_flag(btn,LV_OBJ_FLAG_HIDDEN);
    lv_group_add_obj(btn, lv_group_get_default());
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_KEY, NULL);
}

uint8_t hl_mod_get_knob_okkey_val(void)
{
    static uint8_t keypad_knob_ok = HL_KEY_EVENT_IDLE;
    uint8_t        data;

    if (keypad_knob_ok != in_data.in_inputdev.keypad_knob_ok) {

        LV_LOG_USER("keypad_knob_ok=%d\r\n", keypad_knob_ok);
        keypad_knob_ok = in_data.in_inputdev.keypad_knob_ok;

        data = keypad_knob_ok;
        switch (keypad_knob_ok) {

            case HL_KEY_EVENT_SHORT:

                LV_LOG_USER("LV_KEY_ENTER\n");
                break;
            case HL_KEY_EVENT_LONG:

                LV_LOG_USER("LV_KEY_NEXT\n");

                break;
            default:
                data = HL_KEY_EVENT_IDLE;
                LV_LOG_USER("def\n");
                break;
        }
    } else {
        data = HL_KEY_EVENT_IDLE;
    }

    return data;
}

void hl_mod_rx_knob_key_pro(struct _lv_indev_drv_t* drv, lv_indev_data_t* data)
{

    static uint32_t last_key = 0;
    uint8_t         key_event;

    key_event = hl_mod_get_knob_okkey_val();
    if (key_event) {

        switch (key_event) {

            case HL_KEY_EVENT_SHORT:
                last_key = LV_KEY_ENTER;
                LV_LOG_USER("LV_KEY_ENTER\n");
                data->state = LV_INDEV_STATE_PR;
                data->key   = last_key;
                break;
            case HL_KEY_EVENT_LONG:
                last_key = LV_KEY_NEXT;
                LV_LOG_USER("LV_KEY_NEXT\n");
                data->state = LV_INDEV_STATE_PR;
                data->key   = last_key;
                break;
            default:
                last_key    = 0;
                data->state = LV_INDEV_STATE_REL;
                data->key   = last_key;
                LV_LOG_USER("def\n");
                break;
        }

    } else {
        data->state = LV_INDEV_STATE_REL;
        data->key   = last_key;
    }
}

bool hl_mod_next_menu_enter(uint8_t* tab, uint8_t num, uint8_t max_num)
{
    if (num < max_num) {
        return PageManager_PagePush(tab[num]);
    } else {
        return false;
    }
}

void hl_mod_indev_val_get(mode_to_app_msg_t* p_msg)
{

    switch (p_msg->cmd) {
            // case MSG_RX_PWR_KEY:
            //     hl_app_rx_pwr_key_pro(p_msg->param.u32_param);
            //     LOG_D("MSG_RX_PWR_KEY:(%d) \r\n", p_msg->param.u32_param);
            //     break;

        case MSG_RX_OK_VOL:
            in_data.in_inputdev.keypad_knob_ok = p_msg->param.u32_param;
            LOG_D("MSG_RX_OK_VOL:(%d) \r\n", in_data.in_inputdev.keypad_knob_ok);

            break;

        case MSG_RX_A_VOL:
            in_data.in_inputdev.encoder_knob_diff += p_msg->param.u32_param;
            LOG_D("MSG_RX_L_VOL_KEY:(%d) \r\n", in_data.in_inputdev.encoder_knob_diff);
            break;

        case MSG_RX_B_VOL:
            in_data.in_inputdev.encoder_knob_diff += p_msg->param.u32_param;
            LOG_D("MSG_RX_R_VOL_KEY:(%d) \r\n", in_data.in_inputdev.encoder_knob_diff);
            break;

            // case MSG_RX_VBUS_DET:
            //     hl_app_rx_usb_plug_pro(p_msg->param.u32_param);
            //     LOG_D("MSG_RX_VBUS_DET:(%d) \r\n", p_msg->param.u32_param);
            //     break;

            // case MSG_RX_HP_DET:
            //     hl_app_rx_hp_plug_pro(p_msg->param.u32_param);
            //     LOG_D("MSG_RX_HP_DET:(%d) \r\n", p_msg->param.u32_param);
            //     break;

            // case MSG_RX_CAM_DET:
            //     hl_app_rx_cam_plug_pro(p_msg->param.u32_param);
            //     LOG_D("MSG_RX_CAM_DET:(%d) \r\n", p_msg->param.u32_param);
            //     break;

        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}

void hl_mod_page_cb_reg(void)
{
    PAGE_REG(PAGE_SOUND_MODULE);
    PAGE_REG(PAGE_HOME);
    PAGE_REG(PAGE_MAIN_MENU);
    PAGE_REG(PAGE_TX_CONF_MENU);
    PAGE_REG(PAGE_AUTO_POWEROFF);
    PAGE_REG(PAGE_AUTO_RECORD);
    PAGE_REG(PAGE_FAST_TX_CONFIG);
    PAGE_REG(PAGE_TX_LED_BRIGHT);
    PAGE_REG(PAGE_TX_LOW_CUT);
    PAGE_REG(PAGE_MONITOR_SET);
    PAGE_REG(PAGE_NOISE_REDUCTION_INTENSITY);
    PAGE_REG(PAGE_OTHER_SET);
    PAGE_REG(PAGE_PAIR);
    PAGE_REG(PAGE_RECORD_FORMAT);
    PAGE_REG(PAGE_RECORD_PROTECT);
    PAGE_REG(PAGE_RESTORE);
    PAGE_REG(PAGE_SOUND_EFFECT_MODE);
    PAGE_REG(PAGE_SOUND_MODULE);
    PAGE_REG(PAGE_SYS_TIME_SET);
    PAGE_REG(PAGE_TX_CONF_MENU);
    PAGE_REG(PAGE_TX_GAIN_CONF);
    PAGE_REG(PAGE_TX_GAIN_CONF);
    PAGE_REG(PAGE_VERSION);
    PAGE_REG(PAGE_VOLUME_CONTROL);
}

void lvgl2rtt_init(void)
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
}
void hl_mod_page_all_init(void)
{
    PageManager_Init(PAGE_MAX, 8);
    hl_mod_page_cb_reg();

    hl_mod_display_scr_set_page(PAGE_NONE);
    hl_mod_display_scr_set_page(PAGE_HOME);
    PageManager_PagePush(PAGE_HOME);
}
#endif
/*
 * EOF
 */