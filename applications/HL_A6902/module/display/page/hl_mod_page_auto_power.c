/**
 * @file hl_mod_page_noise_level.c
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


//自动关机界面
static void hl_autopoweroff_test_cb(hl_three_in_one_check_t event_num)
{
    uint32_t value = 0;
    switch(event_num){
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
            return ;
            break;
    }    
    hl_mod_display_send_msg(POWEROFF_SET_VAL_IND,&value,0);
}

static void hl_mod_page_setup(void)
{
    hl_lvgl_three_in_one_init_t three_in_one_test = 
    {
        .func_cb = hl_autopoweroff_test_cb,
        .ptr_lift = "永不",
        .ptr_mid = "15min",
        .ptr_right = "30min",
        .ptr_top = "自动关机",
        .three_in_one_choose = HL_THREE_ONE_CHOOSE_LEFT,
    };
    hl_mod_three_in_one_init(&three_in_one_test);

    hl_lvgl_three_in_one_ioctl_t three_in_one_test_ctl = 
    {
        .three_in_one_choose = HL_THREE_ONE_CHOOSE_LEFT,
    };

    hl_mod_three_in_one_ioctl(&three_in_one_test_ctl);
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
    // 返回按键
    hl_mod_menu_backbtn_scan();
}

PAGE_DEC(PAGE_AUTO_POWEROFF)
{
    bool result;

    result     = PageManager_PageRegister(PAGE_AUTO_POWEROFF, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit,
                                      NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_AUTO_POWEROFF init fail\n");
    }
}

#endif
/*
 * EOF
 */



