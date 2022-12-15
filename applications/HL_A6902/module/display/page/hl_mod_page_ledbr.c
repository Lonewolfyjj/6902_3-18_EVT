/**
 * @file hl_mod_page_ledbr.c
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
#include "hl_mod_input.h"

//亮度调节界面
LV_IMG_DECLARE(Other_led_high_black);// 11
LV_IMG_DECLARE(Other_led_high_white);// 12
LV_IMG_DECLARE(Other_led_low_black);//  21
LV_IMG_DECLARE(Other_led_low_white);//  22

static void hl_light_test_cb(hl_two_in_one_check_t event_num)
{
    printf("event_num = %d\n", event_num);
}

static void hl_mod_page_setup(void)
{
    hl_lvgl_b_two_in_one_init_t two_in_one_test = 
    {
        .func_cb = hl_light_test_cb,
        .src11 = &Other_led_high_black,
        .src12 = &Other_led_high_white,
        .src21 = &Other_led_low_black,
        .src22 = &Other_led_low_white,
        .ptr_lift = "正常",
        .ptr_right = "低亮",
        .ptr_top = "状态灯调节",
        .b_two_in_one_choose = HL_TWO_ONE_CHOOSE_LEFT,
    };
    hl_mod_b_two_in_one_init(&two_in_one_test);

    hl_lvgl_b_two_in_one_ioctl_t two_in_one_test_ctl = 
    {
        .b_two_in_one_choose = HL_TWO_ONE_CHOOSE_LEFT,
    };

    hl_mod_b_two_in_one_ioctl(&two_in_one_test_ctl);
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_b_two_in_one_ioctl_t two_in_one_test_ctl = 
    {
        .b_two_in_one_choose = HL_TWO_ONE_CHOOSE_EXIT,
    };

    hl_mod_b_two_in_one_ioctl(&two_in_one_test_ctl);
}

static void hl_mod_page_loop(void)
{
    hl_mod_menu_backbtn_scan();
}

PAGE_DEC(PAGE_TX_LED_BRIGHT)
{
    bool result;

    result     = PageManager_PageRegister(PAGE_TX_LED_BRIGHT, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit,
                                      NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_TX_LED_BRIGHT init fail\n");
    }
}

#endif
/*
 * EOF
 */



