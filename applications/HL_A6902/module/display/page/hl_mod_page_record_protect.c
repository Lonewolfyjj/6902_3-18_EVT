#include "hl_mod_page_common.h"

#if (!HL_IS_TX_DEVICE())
#include "hl_mod_display.h"
#include "lvgl.h"

#include "hl_mod_page.h"
#include "lv_port_indev.h"
#include "page_test.h"
#include "page_menu.h"
#include "hl_util_general_type.h"


static void hl_recodeprotect_test_cb(hl_two_in_one_check_t event_num)
{
    uint8_t value = 0;
    switch(event_num){
        case HL_TWO_ONE_CHECK_LEFT:
            value = 1;
            break;
        case HL_TWO_ONE_CHECK_RIGHT:
            value = 0;
            break;
        default:
            return;
            break;
    }
    hl_mod_display_send_msg(AUTO_RECORD_PORTECT_SWITCH_IND,&value,0);
}

static void hl_mod_page_setup(void)
{
    hl_lvgl_two_in_one_init_t two_in_one_test = 
    {
        .func_cb = hl_recodeprotect_test_cb,
        .ptr_lift = "ON",
        .ptr_right = "OFF",
        .ptr_top = "录制保护",
        .two_in_one_choose = HL_TWO_ONE_CHOOSE_LEFT,
    };
    hl_mod_two_in_one_init(&two_in_one_test);

    hl_lvgl_two_in_one_ioctl_t two_in_one_test_ctl = 
    {
        .two_in_one_choose = HL_TWO_ONE_CHOOSE_LEFT,
    };

    hl_mod_two_in_one_ioctl(&two_in_one_test_ctl);
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_two_in_one_ioctl_t two_in_one_test_ctl = 
    {
        .two_in_one_choose = HL_TWO_ONE_CHOOSE_EXIT,
    };

    hl_mod_two_in_one_ioctl(&two_in_one_test_ctl);
    // two_in_one_test_ctl.two_in_one_choose = HL_TWO_ONE_CHOOSE_DEL_STYLE;
    // hl_mod_two_in_one_ioctl(&two_in_one_test_ctl);
}

static void hl_mod_page_loop(void)
{
    // 返回按键
    hl_mod_menu_backbtn_scan();
}

PAGE_DEC(PAGE_RECORD_PROTECT)
{
    bool result;

    result     = PageManager_PageRegister(PAGE_RECORD_PROTECT, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit,
                                      NULL);

    if (result == false) {
        LV_LOG_USER("page_record_protect init fail\n");
    }
}

#endif
/*
 * EOF
 */



