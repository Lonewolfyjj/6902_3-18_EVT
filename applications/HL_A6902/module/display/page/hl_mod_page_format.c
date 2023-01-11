/**
 * @file hl_mod_page_format.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
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
#if 0
#include "hl_mod_page_common.h"

#if (!HL_IS_TX_DEVICE())
#include "hl_mod_display.h"
#include "lvgl.h"

#include "hl_mod_page.h"
#include "lv_port_indev.h"
#include "page_test.h"
#include "hl_util_general_type.h"

static int16_t now_num;

//配对界面
static void hl_pair_test_cb(hl_s_two_in_one_check_t event_num)
{
    uint32_t value = 0;
    switch (event_num) {
        case HL_S_TWO_ONE_CHECK_LEFT:
            PageManager_PagePop();
            break;
        case HL_S_TWO_ONE_CHECK_RIGHT:
            value = 1;
            PageManager_PagePush(PAGE_PARING);
            break;
        default:
            return;
            break;
    }
    hl_mod_display_send_msg(DEVICE_PAIR_IND, &value, 0);
}

static void pair_test(void)
{
    // char str[20];
    hl_lvgl_s_two_in_one_init_t s_two_in_one_test = {
        .func_cb             = hl_pair_test_cb,
        .ptr_lift            = "取消",
        .ptr_right           = "确定",
        .ptr_top             = "是否格式化TX1",
        .s_two_in_one_choose = HL_S_TWO_ONE_CHOOSE_LEFT,
    };
    
    // s_two_in_one_test.ptr_top = 

    hl_mod_s_two_in_one_init(&s_two_in_one_test);

    hl_lvgl_s_two_in_one_ioctl_t s_two_in_one_test_ctl = {
        .s_two_in_one_choose = HL_S_TWO_ONE_CHOOSE_LEFT,
    };

    hl_mod_s_two_in_one_ioctl(&s_two_in_one_test_ctl);
    now_num = s_two_in_one_test_ctl.s_two_in_one_choose;
}

static void hl_mod_page_setup(void)
{
    pair_test();
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
        s_two_in_one_test.s_two_in_one_choose = now_num;
        hl_mod_s_two_in_one_ioctl(&s_two_in_one_test);
        PageManager_PagePop();
    }

    // 旋钮选择
    if (hl_mod_knob_select_val_change(&now_num,0,1,true) ) {
        s_two_in_one_test.s_two_in_one_choose = now_num;
        hl_mod_s_two_in_one_ioctl(&s_two_in_one_test);
    }

    // OK按键
    if (ok_btn == HL_KEY_EVENT_SHORT) {
        s_two_in_one_test.s_two_in_one_choose = now_num;
        hl_mod_s_two_in_one_ioctl(&s_two_in_one_test);
    }
}

PAGE_DEC(PAGE_PAIR)
{
    bool result;

    result = PageManager_PageRegister(PAGE_PAIR, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_PAIR init fail\n");
    }
}

#endif
/*
 * EOF
 */
#endif