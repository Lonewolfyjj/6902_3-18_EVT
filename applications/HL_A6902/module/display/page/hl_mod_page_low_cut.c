
/**
 * @file hl_mod_page_low_cut.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
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
#include "page_menu.h"
#include "hl_util_general_type.h"
#include "page_language.h"

static int16_t knob_choose;
static hl_three_in_one_check_t display_choose;

static void hl_three_in_one_trg(hl_three_in_one_check_t choose)
{
    hl_lvgl_three_in_one_ioctl_t three_in_one_test_ctl;
    three_in_one_test_ctl.three_in_one_choose = choose;
    hl_mod_three_in_one_ioctl(&three_in_one_test_ctl);
}

// 低切界面
static void hl_diqie_test_cb(hl_three_in_one_check_t event_num)
{
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();
    uint32_t             value;

    if (display_choose != event_num) {
        display_choose = event_num;
        knob_choose  = (int16_t)display_choose;
        LOG_E("event_num=%d\n", event_num);
        // 上报并更新参数
        hl_mod_display_mux_take();
        switch (display_choose) {
            case HL_THREE_ONE_CHECK_LEFT:
                value = LOW_CUT_OFF;
                break;
            case HL_THREE_ONE_CHECK_MID:
                value = LOW_CUT_75HZ;
                break;
            case HL_THREE_ONE_CHECK_RIGHT:
                value = LOW_CUT_150HZ;
                break;
            default:
                hl_mod_display_mux_release();
                return;
                break;
        }
        data_ptr->low_cut = value;
        hl_mod_display_mux_release();
        hl_mod_display_send_msg(LOW_CUT_VAL_IND, &value, 0);
    }
}

static void hl_three_in_one_update(void)
{
    uint8_t value;
    hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();

    if (flag->low_cut) {
        hl_mod_display_mux_take();
        flag->low_cut = 0;
        value = data_ptr->low_cut;
        hl_mod_display_mux_release();

        hl_three_in_one_trg(value);
    }
}

static void hl_mod_page_setup(void)
{
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();
    a6902_language_typ_t* page_ptr = (a6902_language_typ_t *)hl_a6902_language_ptr_get();
    switch (data_ptr->low_cut) {
        case LOW_CUT_OFF:
            display_choose = HL_THREE_ONE_CHECK_LEFT;
            break;
        case LOW_CUT_75HZ:
            display_choose = HL_THREE_ONE_CHECK_MID;
            break;
        case LOW_CUT_150HZ:
            display_choose = HL_THREE_ONE_CHECK_RIGHT;
            break;
        default:
            break;
    }
    knob_choose = (int16_t)display_choose;

    hl_lvgl_three_in_one_init_t three_in_one_test = {
        .func_cb             = hl_diqie_test_cb,
        .ptr_lift            = page_ptr->three_in_one_page_ptr->page_low_cut->ptr_left,//"无低切",
        .ptr_mid             = page_ptr->three_in_one_page_ptr->page_low_cut->ptr_mid,//"75HZ",
        .ptr_right           = page_ptr->three_in_one_page_ptr->page_low_cut->ptr_right,//"150HZ",
        .ptr_top             = page_ptr->three_in_one_page_ptr->page_low_cut->ptr_top,//"低切",
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

PAGE_DEC(PAGE_TX_LOW_CUT)
{
    bool result;

    result = PageManager_PageRegister(PAGE_TX_LOW_CUT, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("page_tx_low_cut init fail\n");
    }
}

#endif
/*
 * EOF
 */



