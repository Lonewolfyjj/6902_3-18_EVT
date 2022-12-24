/**
 * @file hl_mod_page_volume_set.c
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


//音效模式界面
LV_IMG_DECLARE(Other_high_true_black);//高保真
LV_IMG_DECLARE(Other_high_true_white);//高保真
LV_IMG_DECLARE(Other_sound_black);//声音增强
LV_IMG_DECLARE(Other_sound_white);//声音增强

static void hl_soundeffect_test_cb(hl_b_two_in_one_check_t event_num)
{
    uint8_t value = 0;
    switch(event_num){
        case HL_B_TWO_ONE_CHECK_LEFT:
            value = HIGH_FIDELITY;
            break;
        case HL_B_TWO_ONE_CHECK_RIGHT:
            value = SOUND_ENHANCEMENT;
            break;
        default:
            return;
            break;
    }
    hl_mod_display_send_msg(VOICE_MODULE_VAL_IND,&value,0);
}
static void soundeffect_test(void)
{
    hl_lvgl_b_two_in_one_init_t two_in_one_test = 
    {
        .func_cb = hl_soundeffect_test_cb,
        .src11 = &Other_high_true_black,
        .src12 = &Other_high_true_white,
        .src21 = &Other_sound_black,
        .src22 = &Other_sound_white,
        .ptr_lift = "高保真",
        .ptr_right = "声音增强",
        .ptr_top = "音效模式",
        .b_two_in_one_choose = HL_B_TWO_ONE_CHOOSE_LEFT,
    };
    hl_mod_b_two_in_one_init(&two_in_one_test);

    hl_lvgl_b_two_in_one_ioctl_t two_in_one_test_ctl = 
    {
        .b_two_in_one_choose = HL_B_TWO_ONE_CHOOSE_LEFT,
    };

    hl_mod_b_two_in_one_ioctl(&two_in_one_test_ctl);
}

static void hl_mod_page_setup(void)
{
    soundeffect_test();
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_b_two_in_one_ioctl_t two_in_one_test_ctl = 
    {
        .b_two_in_one_choose = HL_B_TWO_ONE_CHOOSE_EXIT,
    };

    hl_mod_b_two_in_one_ioctl(&two_in_one_test_ctl);
    two_in_one_test_ctl.b_two_in_one_choose = HL_B_TWO_ONE_CHOOSE_DEL_STYLE;
    hl_mod_b_two_in_one_ioctl(&two_in_one_test_ctl);
}

static void hl_mod_page_loop(void)
{
    hl_mod_menu_backbtn_scan();
  
}

PAGE_DEC(PAGE_SOUND_EFFECT_MODE)
{
    bool result;

    result     = PageManager_PageRegister(PAGE_SOUND_EFFECT_MODE, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit,
                                      NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_SOUND_EFFECT_MODE init fail\n");
    }
}

#endif
/*
 * EOF
 */



