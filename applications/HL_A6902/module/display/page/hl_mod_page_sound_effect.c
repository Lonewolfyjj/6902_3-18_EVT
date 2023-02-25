/**
 * @file hl_mod_page_volume_set.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 主菜单音效模式设置界面
 * @version V1.0
 * @date 2022-12-09
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
 * <tr><td>2022-12-09     <td>v1.0     <td>liujie     <td>内容
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

#include "hl_mod_page_common.h"

#if (!HL_IS_TX_DEVICE())
#include "hl_mod_display.h"
#include "lvgl.h"

#include "hl_mod_page.h"
#include "lv_port_indev.h"
#include "page_test.h"
#include "page_language.h"
#include "hl_util_general_type.h"

//音效模式界面
LV_IMG_DECLARE(Other_high_true_black);  //高保真
LV_IMG_DECLARE(Other_high_true_white);  //高保真
LV_IMG_DECLARE(Other_sound_black);      //声音增强
LV_IMG_DECLARE(Other_sound_white);      //声音增强

static int16_t                 knob_choose;
static hl_b_two_in_one_check_t display_choose;



static void hl_b_two_in_one_update(void)
{
    uint8_t                     value;
    hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();

    if (flag->voice_module) {

        hl_mod_display_mux_take();
        value              = data_ptr->voice_module;
        flag->voice_module = 0;
        hl_mod_display_mux_release();
        switch (value) {
            case HIGH_FIDELITY:
                value = HL_B_TWO_ONE_CHOOSE_LEFT;
                break;
            case SOUND_ENHANCEMENT:
                value = HL_B_TWO_ONE_CHOOSE_RIGHT;
                break;
            default:
                value = HL_B_TWO_ONE_CHOOSE_LEFT;
                break;
        }

        hl_b_two_in_one_trg(value);
    }
}

static void hl_soundeffect_test_cb(hl_b_two_in_one_check_t event_num)
{
    uint32_t             value    = 0;
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();

    LOG_E("event_num=%d\n", event_num);

    if (display_choose != event_num) {
        display_choose = event_num;
        knob_choose    = (int16_t)event_num;
        // 上报并更新参数
        hl_mod_display_mux_take();
        switch (event_num) {
            case HL_B_TWO_ONE_CHECK_LEFT:
                value = HIGH_FIDELITY;
                break;
            case HL_B_TWO_ONE_CHECK_RIGHT:
                value = SOUND_ENHANCEMENT;
                break;
            default:
                hl_mod_display_mux_release();
                return;
                break;
        }
        data_ptr->voice_module = value;
        hl_mod_display_mux_release();
        hl_mod_display_send_msg(VOICE_MODULE_VAL_IND, &value, 0);
    }
    // } else {
    //     switch (event_num) {
    //         case HL_B_TWO_ONE_CHECK_LEFT:
    //             value = HIGH_FIDELITY;
    //             break;
    //         case HL_B_TWO_ONE_CHECK_RIGHT:
    //             value = SOUND_ENHANCEMENT;
    //             break;
    //         default:
    //             break;
    //     }

    //     if (data_ptr->voice_module != value) {

    //         hl_mod_display_mux_take();
    //         data_ptr->monitor_category = value;
    //         hl_mod_display_mux_release();
    //     }
    //     hl_mod_display_send_msg(MONITOR_CATEGORY_VAL_IND, &value, 0);
    // }
}

static void hl_mod_page_setup(void)
{
    hl_display_screen_s*         data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_lvgl_b_two_in_one_ioctl_t two_in_one_test_ctl;
    a6902_language_typ_t* page_ptr = (a6902_language_typ_t *)hl_a6902_language_ptr_get();
    LOG_D("voiceeffect=%d\n", data_ptr->voice_module);

    switch (data_ptr->voice_module) {

        case HIGH_FIDELITY:
            display_choose = HL_B_TWO_ONE_CHECK_LEFT;
            break;
        case SOUND_ENHANCEMENT:
            display_choose = HL_B_TWO_ONE_CHECK_RIGHT;
            break;
        default:
            display_choose = HL_B_TWO_ONE_CHECK_LEFT;
            break;
    }

    knob_choose = (int16_t)display_choose;

    hl_lvgl_b_two_in_one_init_t two_in_one_test = {
        .func_cb             = hl_soundeffect_test_cb,
        .src11               = &Other_high_true_black,
        .src12               = &Other_high_true_white,
        .src21               = &Other_sound_black,
        .src22               = &Other_sound_white,
        .ptr_lift            = page_ptr->b_two_in_one_page_ptr->page_sound->ptr_left,//"高保真",
        .ptr_right           = page_ptr->b_two_in_one_page_ptr->page_sound->ptr_right,//"人声增强",
        .ptr_top             = page_ptr->b_two_in_one_page_ptr->page_sound->ptr_top,//"音效模式",
        .b_two_in_one_choose = display_choose,
    };
    hl_mod_b_two_in_one_init(&two_in_one_test);

    two_in_one_test_ctl.b_two_in_one_choose = display_choose;
    hl_mod_b_two_in_one_ioctl(&two_in_one_test_ctl);
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_b_two_in_one_ioctl_t two_in_one_test_ctl = {
        .b_two_in_one_choose = HL_B_TWO_ONE_CHOOSE_EXIT,
    };

    hl_mod_b_two_in_one_ioctl(&two_in_one_test_ctl);
    // two_in_one_test_ctl.b_two_in_one_choose = HL_B_TWO_ONE_CHOOSE_DEL_STYLE;
    // hl_mod_b_two_in_one_ioctl(&two_in_one_test_ctl);
}

static void hl_mod_page_loop(void)
{
    // OK按键
    uint8_t ok_btn = hl_mod_get_knob_okkey_val();
    // 返回按键
    uint8_t back_btn = hl_mod_keypad_touchkey_read();

    // 触摸返回
    if (1 == back_btn) {
        hl_b_two_in_one_trg(knob_choose);
        PageManager_PagePop();
    }

    // 旋钮对配置的更改
    if (hl_mod_knob_select_val_change(&knob_choose, 0, 1, true)) {
        LOG_E("knob choose chg=%d\n", knob_choose);

        hl_b_two_in_one_trg(knob_choose);
    }

    // OK按键
    if (ok_btn == HL_KEY_EVENT_SHORT) {
        hl_b_two_in_one_trg(knob_choose);
    }

    hl_b_two_in_one_update();
}

PAGE_DEC(PAGE_SOUND_EFFECT_MODE)
{
    bool result;

    result =
        PageManager_PageRegister(PAGE_SOUND_EFFECT_MODE, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_SOUND_EFFECT_MODE init fail\n");
    }
}

#endif
/*
 * EOF
 */
