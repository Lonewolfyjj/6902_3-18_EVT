/**
 * @file hl_mod_page_open_msc.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 打开大容量升级页面
 * @version V1.0
 * @date 2023-02-06
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
 * <tr><td>2023-02-06     <td>v1.0     <td>liujie     <td>内容
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
#include "hl_util_nvram.h"
#include "hl_mod_page.h"
#include "lv_port_indev.h"
#include "page_test.h"
#include "page_upgrade.h"
#include "hl_util_general_type.h"
#include "page_language.h"
#include "hl_board_commom.h"

static uint8_t                 update_state   = 0;
static int16_t                 now_num        = HL_S_TWO_ONE_CHOOSE_LEFT;
static hl_s_two_in_one_check_t display_choose = HL_S_TWO_ONE_CHECK_LEFT;
static uint8_t  msc_open;

static void hl_mod_page_exit(void);

static void hl_resetfactory_test_cb(hl_s_two_in_one_check_t event_num)
{
    uint32_t                    value = 0;
    hl_display_screen_change_s* flag  = hl_mod_page_get_screen_change_flag();
    hl_lvgl_upgrade_ioctl_t     ioctl;

    LOG_D("event_num = %d\n", event_num);

    if (event_num == display_choose) {

        switch (event_num) {
            case HL_S_TWO_ONE_CHECK_LEFT:
                // 显示重启页面
                value = 1;
                if (msc_open == 0) {
                    hl_mod_display_send_msg(UPGRADE_SETTING_SWITCH_IND, &value, 0);
                } else {
                    hl_util_nvram_param_set_integer("MSC_OPEN", 0);
                }

                if (!update_state) {
                    update_state = 1;
                }

                break;
            case HL_S_TWO_ONE_CHECK_RIGHT:

                // 取消就回上一页面
                PageManager_PagePop();

                break;
            default:

                break;
        }

    } else {
        display_choose = event_num;
        now_num        = event_num;
    }
}

static void hl_mod_page_setup(void)
{
    hl_display_screen_s*        datat_ptr         = hl_mod_page_get_screen_data_ptr();
    a6902_language_typ_t* page_ptr = (a6902_language_typ_t *)hl_a6902_language_ptr_get();
    hl_lvgl_s_two_in_one_init_t s_two_in_one_test = {
        .func_cb             = hl_resetfactory_test_cb,
        .ptr_lift            = page_ptr->s_two_in_one_page_ptr->page_open_msc->ptr_left,//"确定",
        .ptr_right           = page_ptr->s_two_in_one_page_ptr->page_open_msc->ptr_right,//"取消",
       // .ptr_top             = page_ptr->s_two_in_one_page_ptr->page_open_msc->prt_top,//"是否开启升级",
        .s_two_in_one_choose = HL_S_TWO_ONE_CHOOSE_LEFT,
    };
    msc_open = datat_ptr->msc_s;
    if (msc_open == 0) {
        s_two_in_one_test.ptr_top = page_ptr->s_two_in_one_page_ptr->page_open_msc->prt_top;
    } else {
        s_two_in_one_test.ptr_top = "是否关闭升级";
    }
    hl_mod_s_two_in_one_init(&s_two_in_one_test);
    update_state = 0;
    display_choose = s_two_in_one_test.s_two_in_one_choose;
    now_num = (int16_t)display_choose;
}

static void hl_mod_page_exit(void)
{
    update_state = 0;
    hl_lvgl_s_two_in_one_ioctl_t s_two_in_one_test_ctl = {
        .s_two_in_one_choose = HL_S_TWO_ONE_CHOOSE_EXIT,
    };

    hl_mod_s_two_in_one_ioctl(&s_two_in_one_test_ctl);
    // s_two_in_one_test_ctl.s_two_in_one_choose = HL_S_TWO_ONE_CHOOSE_DEL_STYLE;
    // hl_mod_s_two_in_one_ioctl(&s_two_in_one_test_ctl);
}

static void hl_mod_page_loop(void)
{
    hl_lvgl_s_two_in_one_ioctl_t s_two_in_one_test;
    a6902_language_typ_t* page_ptr = (a6902_language_typ_t *)hl_a6902_language_ptr_get();
    // OK按键
    uint8_t ok_btn = hl_mod_get_knob_okkey_val();
    // 返回按键
    uint8_t back_btn = hl_mod_keypad_touchkey_read();

    if (update_state >= 3) {
        goto lable_reboot;
    }
    // 触摸返回
    if (1 == back_btn) {
        PageManager_PagePop();
    }

    // 旋钮选择
    if (hl_mod_knob_select_val_change(&now_num, 0, 1, true)) {
        s_two_in_one_test.s_two_in_one_choose = now_num;
        hl_mod_s_two_in_one_ioctl(&s_two_in_one_test);
    }

    // OK按键
    if (ok_btn == HL_KEY_EVENT_SHORT) {
        s_two_in_one_test.s_two_in_one_choose = now_num;
        hl_mod_s_two_in_one_ioctl(&s_two_in_one_test);
    }

lable_reboot:

    switch (update_state) {
        case 0: {
        } break;
        case 1: {
            hl_mod_page_exit();
            update_state = 2;
        } break;
        case 2: {
            // hl_mod_page_exit();
            hl_lvgl_upgrade_ioctl_t ioctl;
            uint8_t                 ret = 0;
            if (msc_open) {
                hl_util_nvram_param_set("MSC_OPEN", "0");
                // ret = hl_util_nvram_param_save();

                rt_kprintf("save ret = %d\r\n", ret);
            } else {
                hl_util_nvram_param_set("MSC_OPEN", "1");
                // ret = hl_util_nvram_param_save();
                rt_kprintf("save ret = %d\r\n", ret);
            }
            ret = 0;

            if (ret == 0) {
                ioctl.upgrade_ioctl = HL_UPGRADE_SUCCESS_CMD;
                ioctl.ptr           = page_ptr->upgrade_page_ptr->page_upgrade->ptr_set_success;//"设置成功";
            } else {
                ioctl.upgrade_ioctl = HL_UPGRADE_FAIL_CMD;
                ioctl.ptr           = page_ptr->upgrade_page_ptr->page_upgrade->ptr_set_fail;//"设置失败";
            }

            hl_mod_lvgl_upgrade_ioctl(&ioctl);

            
            update_state = 3;
        } break;
        case 3: {
            hl_board_reboot();
            update_state = 4;
        } break;
        default:
            break;
    }
}

PAGE_DEC(PAGE_OPEN_MSC)
{
    bool result;

    result = PageManager_PageRegister(PAGE_OPEN_MSC, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_OPEN_MSC init fail\n");
    }
}

#endif
/*
 * EOF
 */
