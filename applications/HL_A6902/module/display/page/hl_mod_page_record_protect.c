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

static int16_t                knob_choose;
static hl_two_in_one_choose_t display_choose;

static void hl_two_in_one_trg(hl_two_in_one_choose_t choose)
{
    hl_lvgl_two_in_one_ioctl_t two_in_one_test_ctl = {
        .two_in_one_choose = choose,
    };

    hl_mod_two_in_one_ioctl(&two_in_one_test_ctl);
}

static void hl_two_in_one_update(void)
{
    uint8_t                     value;
    hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();

    if (flag->sys_status.auto_record_portect) {

        hl_mod_display_mux_take();
        value                                = data_ptr->sys_status.auto_record_portect;
        flag->sys_status.auto_record_portect = 0;
        hl_mod_display_mux_release();

        hl_two_in_one_trg(value);
    }
}

static void hl_recodeprotect_test_cb(hl_two_in_one_check_t event_num)
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
            case HL_TWO_ONE_CHECK_LEFT:
                value = 1;
                break;
            case HL_TWO_ONE_CHECK_RIGHT:
                value = 0;
                break;
            default:
                hl_mod_display_mux_release();
                return;
                break;
        }
        data_ptr->sys_status.auto_record_portect = value;
        hl_mod_display_mux_release();
        hl_mod_display_send_msg(AUTO_RECORD_PORTECT_SWITCH_IND, &value, 0);
    }
}

static void hl_mod_page_setup(void)
{
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();
    a6902_language_typ_t* page_ptr = (a6902_language_typ_t *)hl_a6902_language_ptr_get();
    switch (data_ptr->sys_status.auto_record_portect) {
        case 1:
            display_choose = HL_TWO_ONE_CHOOSE_LEFT;
            break;
        case 0:
            display_choose = HL_TWO_ONE_CHOOSE_RIGHT;
            break;
        default:
            break;
    }

    knob_choose                               = (int16_t)display_choose;
    hl_lvgl_two_in_one_init_t two_in_one_test = {
        .func_cb           = hl_recodeprotect_test_cb,
        .ptr_lift          = page_ptr->two_in_one_page_ptr->page_record_protect->ptr_left,//"ON",
        .ptr_right         = page_ptr->two_in_one_page_ptr->page_record_protect->ptr_right,//"OFF",
        .ptr_top           = page_ptr->two_in_one_page_ptr->page_record_protect->prt_top,//"录制保护",
        .two_in_one_choose = display_choose,
    };
    hl_mod_two_in_one_init(&two_in_one_test);
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_two_in_one_ioctl_t two_in_one_test_ctl = {
        .two_in_one_choose = HL_TWO_ONE_CHOOSE_EXIT,
    };

    hl_mod_two_in_one_ioctl(&two_in_one_test_ctl);
    // two_in_one_test_ctl.two_in_one_choose = HL_TWO_ONE_CHOOSE_DEL_STYLE;
    // hl_mod_two_in_one_ioctl(&two_in_one_test_ctl);
}

static void hl_mod_page_loop(void)
{
    // OK按键
    uint8_t ok_btn = hl_mod_get_knob_okkey_val();
    // 返回按键
    uint8_t back_btn = hl_mod_keypad_touchkey_read();

    // 触摸返回
    if (1 == back_btn) {
        hl_two_in_one_trg(knob_choose);
        PageManager_PagePop();
    }

    // 旋钮对配置的更改
    if (hl_mod_knob_select_val_change(&knob_choose, 0, 1, true)) {
        LOG_E("knob choose chg=%d\n", knob_choose);

        hl_two_in_one_trg(knob_choose);
    }

    // OK按键
    if (ok_btn == HL_KEY_EVENT_SHORT) {
        hl_two_in_one_trg(knob_choose);
    }

    hl_two_in_one_update();
}

PAGE_DEC(PAGE_RECORD_PROTECT)
{
    bool result;

    result = PageManager_PageRegister(PAGE_RECORD_PROTECT, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("page_record_protect init fail\n");
    }
}

#endif
/*
 * EOF
 */
