/**
 * @file hl_mod_page_soundout_setting.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 声音外放设置界面
 * @version V1.0
 * @date 2023-01-12
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
 * <tr><td>2023-01-12     <td>v1.0     <td>liujie     <td>内容
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
#include "page_upgrade.h"
#include "hl_util_general_type.h"

static int16_t                knob_choose;
static hl_two_in_one_choose_t display_choose;
// static uint8_t                goto_reboot_dialog_flag = 0;

static void hl_two_in_one_trg(hl_three_in_one_check_t choose)
{
    hl_lvgl_two_in_one_ioctl_t two_in_one_test_ctl;

    two_in_one_test_ctl.two_in_one_choose = choose;
    hl_mod_two_in_one_ioctl(&two_in_one_test_ctl);
}

static void hl_two_in_one_update(void)
{
    uint8_t                     value;
    hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();

    if (flag->sys_status.soundout_setting) {

        hl_mod_display_mux_take();
        value                             = data_ptr->sys_status.soundout_setting;
        flag->sys_status.soundout_setting = 0;
        hl_mod_display_mux_release();

        hl_two_in_one_trg(value);
    }
}

static void hl_soundout_setting_reboot_page(void)
{
    hl_lvgl_upgrade_ioctl_t    ioctrl;
    hl_lvgl_two_in_one_ioctl_t two_in_one_test_ctl;

    // if (goto_reboot_dialog_flag == 1) {
        // 删除
        two_in_one_test_ctl.two_in_one_choose = HL_TWO_ONE_CHOOSE_EXIT;
        hl_mod_two_in_one_ioctl(&two_in_one_test_ctl);

        ioctrl.upgrade_ioctl = HL_UPGRADE_SUCCESS_CMD;
        ioctrl.ptr           = "设置成功";
        hl_mod_lvgl_upgrade_ioctl(&ioctrl);
    // }
}

static void hl_autorecode_test_cb(hl_two_in_one_check_t event_num)
{
    uint32_t                   value    = 0;
    hl_display_screen_s*       data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_lvgl_two_in_one_ioctl_t two_in_one_test_ctl;

    LOG_E("event_num=%d\n", event_num);

    if (display_choose == event_num) {
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
        data_ptr->sys_status.soundout_setting = value;
        hl_mod_display_mux_release();
        hl_mod_display_send_msg(SOUNDOUT_SETTING_SWITCH_IND, &value, 0);
        // goto_reboot_dialog_flag = 1;

    } else {
        display_choose = event_num;
        knob_choose    = (int16_t)event_num;
    }
}

static void hl_mod_page_setup(void)
{
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();

    switch (data_ptr->sys_status.soundout_setting) {
        case 1:
            display_choose = HL_TWO_ONE_CHOOSE_LEFT;
            break;
        case 0:
            display_choose = HL_TWO_ONE_CHOOSE_RIGHT;
            break;
        default:
            break;
    }

    knob_choose = (int16_t)display_choose;

    hl_lvgl_two_in_one_init_t two_in_one_test = {
        .func_cb           = hl_autorecode_test_cb,
        .ptr_lift          = "ON",
        .ptr_right         = "OFF",
        .ptr_top           = "外放设置",
        .two_in_one_choose = display_choose,
    };
    hl_mod_two_in_one_init(&two_in_one_test);

    // goto_reboot_dialog_flag = 0;
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_two_in_one_ioctl_t two_in_one_test_ctl;

    two_in_one_test_ctl.two_in_one_choose = HL_TWO_ONE_CHOOSE_EXIT;
    /// 因为这个页面是全部递归删除的，所以即使已经改成对话框了，调用这个函数也不会有什么影响
    hl_mod_two_in_one_ioctl(&two_in_one_test_ctl);

    // two_in_one_test_ctl.two_in_one_choose = HL_TWO_ONE_CHOOSE_DEL_STYLE;
    // hl_mod_two_in_one_ioctl(&two_in_one_test_ctl);
}

static void hl_mod_page_loop(void)
{
    // // 如果已经进入了让客户重启的对话框页面,就不再响应输入了
    // if (goto_reboot_dialog_flag == 1) {
    //     return;
    // }
    // OK按键
    uint8_t ok_btn = hl_mod_get_knob_okkey_val();
    // 返回按键
    uint8_t back_btn = hl_mod_keypad_touchkey_read();

    // 触摸返回
    if (1 == back_btn) {
        // hl_two_in_one_trg(knob_choose);
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

    // hl_soundout_setting_reboot_page();
}

PAGE_DEC(PAGE_SOUNDOUT_SETTING)
{
    bool result;

    result =
        PageManager_PageRegister(PAGE_SOUNDOUT_SETTING, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_SOUNDOUT_SETTING init fail\n");
    }
}

#endif
/*
 * EOF
 */
