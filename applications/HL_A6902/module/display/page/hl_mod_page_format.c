/**
 * @file hl_mod_page_format.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 格式化确定对话框
 * @version V1.0
 * @date 2023-01-11
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
 * <tr><td>2023-01-11     <td>v1.0     <td>liujie     <td>内容
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

static int16_t                 now_num        = HL_S_TWO_ONE_CHOOSE_LEFT;
static hl_s_two_in_one_check_t display_choose = HL_S_TWO_ONE_CHECK_LEFT;
//配对界面
static void hl_pair_test_cb(hl_s_two_in_one_check_t event_num)
{
    uint32_t                    value    = 0;
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();
    hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();

    LOG_D("event_num = %d\n", event_num);

    if (event_num == display_choose) {
        // 如果相同就触发事件
        switch (event_num) {
            case HL_S_TWO_ONE_CHECK_LEFT:
                // 取消就回上一页面
                PageManager_PagePop();
                break;
            case HL_S_TWO_ONE_CHECK_RIGHT:
                // 确定就回下一页面（后面可能加格式化中页面）
                value = 1;

                if (!flag->channel_format_flag) {
                    hl_mod_display_send_msg(TX1_FS_FORMAT_VAL_IND, &value, 0);
                    hl_mod_display_mux_take();
                    data_ptr->tx1_remained_record_time = STROGE_MAX_USED_TIME;
                    hl_mod_display_mux_release();
                } else {
                    hl_mod_display_send_msg(TX2_FS_FORMAT_VAL_IND, &value, 0);
                    hl_mod_display_mux_take();
                    data_ptr->tx2_remained_record_time = STROGE_MAX_USED_TIME;
                    hl_mod_display_mux_release();
                }
                PageManager_PagePop();
                break;
            default:
                return;
                break;
        }
    } else {
        display_choose = event_num;
        now_num        = event_num;
    }
}

static void hl_mod_page_setup(void)
{
    hl_display_screen_change_s* flag = hl_mod_page_get_screen_change_flag();
    a6902_language_typ_t* page_ptr = (a6902_language_typ_t *)hl_a6902_language_ptr_get();
    hl_lvgl_s_two_in_one_init_t s_two_in_one_test = {
        .func_cb             = hl_pair_test_cb,
        .ptr_lift            = page_ptr->s_two_in_one_page_ptr->page_format->ptr_left,//"取消",
        .ptr_right           = page_ptr->s_two_in_one_page_ptr->page_format->ptr_right,//"确定",
        .ptr_top             = "",
        .s_two_in_one_choose = HL_S_TWO_ONE_CHOOSE_LEFT,
    };
    if (!flag->channel_format_flag) {
        s_two_in_one_test.ptr_top = page_ptr->s_two_in_one_page_ptr->page_format->prt_top1;//"是否格式化TX1";
    } else {
        s_two_in_one_test.ptr_top = page_ptr->s_two_in_one_page_ptr->page_format->prt_top2;//"是否格式化TX2";
    }
    hl_mod_s_two_in_one_init(&s_two_in_one_test);
}

static void hl_mod_page_exit(void)
{
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
    // OK按键
    uint8_t ok_btn = hl_mod_get_knob_okkey_val();
    // 返回按键
    uint8_t back_btn = hl_mod_keypad_touchkey_read();

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
}

PAGE_DEC(PAGE_FORMAT)
{
    bool result;

    result = PageManager_PageRegister(PAGE_FORMAT, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_FORMAT init fail\n");
    }
}

#endif
/*
 * EOF
 */
