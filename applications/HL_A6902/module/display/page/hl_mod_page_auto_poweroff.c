/**
 * @file hl_mod_page_auto_poweroff.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief UI的自动关机选项页面
 * @version V1.0
 * @date 2023-01-10
 * 
 * ██╗  ██╗ ██████╗ ██╗     ██╗  ██╗   ██╗██╗      █████╗ ███╗   ██╗██████╗ 
 * ██║  ██║██╔═══██╗██║     ██║  ╚██╗ ██╔╝██║     ██╔══██╗████╗  ██║██╔══██╗
 * ███████║██║   ██║██║     ██║   ╚████╔╝ ██║     ███████║██╔██╗ ██║██║  ██║
 * ██╔══██║██║   ██║██║     ██║    ╚██╔╝  ██║     ██╔══██║██║╚██╗██║██║  ██║
 * ██║  ██║╚██████╔╝███████╗███████╗██║   ███████╗██║  ██║██║ ╚████║██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝
 * @copyright Copyright (c) 2023 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2023-01-10     <td>v1.0     <td>liujie     <td>内容
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
#include "hl_util_general_type.h"
#include "page_language.h"

static int16_t                 knob_choose;
static hl_three_in_one_check_t display_choose;

static void hl_three_in_one_trg(hl_three_in_one_check_t choose)
{
    hl_lvgl_three_in_one_ioctl_t three_in_one_test_ctl;

    three_in_one_test_ctl.three_in_one_choose = choose;
    hl_mod_three_in_one_ioctl(&three_in_one_test_ctl);
}

static void hl_three_in_one_update(void)
{
    uint16_t                    value;
    hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();

    if (flag->auto_poweroff) {
        hl_mod_display_mux_take();
        flag->auto_poweroff = 0;
        value               = data_ptr->auto_poweroff;
        hl_mod_display_mux_release();
        switch (value) {
            case 0:
                value = HL_THREE_ONE_CHECK_LEFT;
                break;
            case 15:
                value = HL_THREE_ONE_CHECK_MID;
                break;
            case 30:
                value = HL_THREE_ONE_CHECK_RIGHT;
                break;
            default:
                break;
        }
        hl_three_in_one_trg(value);
    }
}

//自动关机界面
static void hl_autopoweroff_test_cb(hl_three_in_one_check_t event_num)
{

    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();
    uint32_t             value;

    if (display_choose != event_num) {
        display_choose = event_num;
        knob_choose    = (int16_t)display_choose;
        LOG_E("event_num=%d\n", event_num);
        // 上报并更新参数
        hl_mod_display_mux_take();
        switch (display_choose) {
            case HL_THREE_ONE_CHECK_LEFT:
                value = 0;

                break;
            case HL_THREE_ONE_CHECK_MID:
                value = 15;
                break;
            case HL_THREE_ONE_CHECK_RIGHT:
                value = 30;
                break;
            default:
                hl_mod_display_mux_release();
                return;
                break;
        }
        data_ptr->auto_poweroff = (uint16_t)value;

        hl_mod_display_mux_release();
        hl_mod_display_send_msg(POWEROFF_SET_VAL_IND, &value, 0);
    }
}

static void hl_mod_page_setup(void)
{
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();
    a6902_language_typ_t* page_ptr = (a6902_language_typ_t *)hl_a6902_language_ptr_get();
    switch (data_ptr->auto_poweroff) {
        case 0:
            display_choose = HL_THREE_ONE_CHECK_LEFT;
            break;
        // 15min关机
        case 15:
            display_choose = HL_THREE_ONE_CHECK_MID;
            break;
        // 30min关机
        case 30:
            display_choose = HL_THREE_ONE_CHECK_RIGHT;
            break;
        default:
            display_choose = HL_THREE_ONE_CHECK_LEFT;
            break;
    }

    knob_choose = (int16_t)display_choose;

    hl_lvgl_three_in_one_init_t three_in_one_test = {
        .func_cb             = hl_autopoweroff_test_cb,
        .ptr_lift            = page_ptr->three_in_one_page_ptr->page_auto_poweroff->ptr_left,//"永不",
        .ptr_mid             = page_ptr->three_in_one_page_ptr->page_auto_poweroff->ptr_mid,//"15min",
        .ptr_right           = page_ptr->three_in_one_page_ptr->page_auto_poweroff->ptr_right,//"30min",
        .ptr_top             = page_ptr->three_in_one_page_ptr->page_auto_poweroff->ptr_top,//"自动关机",
        .three_in_one_choose = display_choose,
    };
    hl_mod_three_in_one_init(&three_in_one_test);
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_three_in_one_ioctl_t three_in_one_test_ctl;

    three_in_one_test_ctl.three_in_one_choose = HL_THREE_ONE_CHOOSE_EXIT;
    hl_mod_three_in_one_ioctl(&three_in_one_test_ctl);

    // three_in_one_test_ctl.three_in_one_choose = HL_THREE_ONE_CHOOSE_DEL_STYLE;
    // hl_mod_three_in_one_ioctl(&three_in_one_test_ctl);
}

static void hl_mod_page_loop(void)
{
    // OK按键
    uint8_t ok_btn = hl_mod_get_knob_okkey_val();
    // 返回按键
    uint8_t back_btn = hl_mod_keypad_touchkey_read();

    // 触摸返回
    if (1 == back_btn) {
        hl_three_in_one_trg(knob_choose);
        PageManager_PagePop();
    }

    // 旋钮对配置的更改
    if (hl_mod_knob_select_val_change(&knob_choose, 0, 2, true)) {
        LOG_E("knob choose chg=%d\n", knob_choose);
        hl_three_in_one_trg(knob_choose);
    }

    // OK按键
    if (ok_btn == HL_KEY_EVENT_SHORT) {
        hl_three_in_one_trg(knob_choose);
    }

    // 上层对配置的更改
    hl_three_in_one_update();
}

PAGE_DEC(PAGE_AUTO_POWEROFF)
{
    bool result;

    result = PageManager_PageRegister(PAGE_AUTO_POWEROFF, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_AUTO_POWEROFF init fail\n");
    }
}

#endif
/*
 * EOF
 */
