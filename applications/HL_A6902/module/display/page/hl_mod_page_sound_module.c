/**
 * @file hl_mod_page_menu.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-12-01
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
 * <tr><td>2022-12-01     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/

#include "hl_mod_page_common.h"
#include "hl_config.h"
#if (!HL_IS_TX_DEVICE())
#include "hl_mod_display.h"
#include "lvgl.h"

#include "hl_mod_page.h"
#include "lv_port_indev.h"
#include "page_test.h"
#include "page_menu.h"
#include "hl_util_general_type.h"
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/


#define MENU_ICON_NUM 3



// 下级菜单表
// static const hl_screen_page_e next_level_menu_tab[MENU_ICON_NUM] = {
//     PAGE_SOUND_MODULE, PAGE_NOISE_REDUCTION_INTENSITY, PAGE_VOLUME_CONTROL, PAGE_TX_CONF_MENU, PAGE_MONITOR_SET,
//     PAGE_OTHER_SET\
// };
static int8_t menu_center_icon = 0;
//模式选择菜单界面
LV_IMG_DECLARE(Menu_single_voice);//单声道
LV_IMG_DECLARE(Menu_stereo);//立体声
LV_IMG_DECLARE(Menu_saft_track);//安全音轨

static void page_10_test_cb(uint32_t current)
{
    uint32_t             module;
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();

    if (!(current & 0x80)) {

        switch (current) {
            case 0:
                module = MONO;
                break;
            case 1:
                module = STEREO;
                break;
            case 2:
                module = SAFE_TRACK;
                break;
            default:
                break;
        }
        hl_mod_display_send_msg(SOUND_MODULE_SET_IND, &module, 0);
        data_ptr->now_sound_module = module;
        hl_mod_menu_goto_home_page();
    } else {
        hl_mod_knob_select_val_set(&menu_center_icon, current & 0x7F);
    }
}

static void hl_mod_page_setup(void)
{
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();


    

    menu_data_t pic_list[MENU_ICON_NUM] = {
        ADD_IMG_DATA(NULL, NULL, &Menu_single_voice, "单声道"),
        ADD_IMG_DATA(NULL, NULL, &Menu_stereo, "立体声"),
        ADD_IMG_DATA(NULL, NULL, &Menu_saft_track, "安全音轨"),
    };
// 当前状态居中
        switch (data_ptr->now_sound_module) {
            case MONO:
                menu_center_icon = 0;
                break;
            case STEREO:
                menu_center_icon = 1;
                break;
            case SAFE_TRACK:
                menu_center_icon = 2;
                break;
            default:
                break;
        }
        
    page_menu_init(&pic_list, MENU_ICON_NUM, page_10_test_cb, menu_center_icon);
}

static void hl_mod_page_exit(void)
{
    lv_menu_exit();
}

static void hl_mod_page_loop(void)
{
    hl_mod_menu_enterbtn_scan(hl_mod_knob_select_val_get(&menu_center_icon));
    hl_mod_menu_backbtn_scan();
    if (hl_mod_knob_select_val_change(&menu_center_icon,0,MENU_ICON_NUM - 1, false) ) {
        lv_set_icon_postion(menu_center_icon, false);
    }
}



PAGE_DEC(PAGE_SOUND_MODULE)
{
    bool result;

    result     = PageManager_PageRegister(PAGE_SOUND_MODULE, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit,
                                      NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_SOUND_MODULE init fail\n");
    }
}

#endif
/*
 * EOF
 */



