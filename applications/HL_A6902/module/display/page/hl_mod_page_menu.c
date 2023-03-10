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

#if (!HL_IS_TX_DEVICE())
#include "hl_mod_display.h"
#include "lvgl.h"
#include "hl_mod_page.h"
#include "lv_port_indev.h"
#include "page_test.h"
#include "page_menu.h"
#include "hl_util_general_type.h"
#include "page_language.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

//主菜单页面
LV_IMG_DECLARE(Menu_single_voice);  //单声道
LV_IMG_DECLARE(Menu_stereo);        //立体声
LV_IMG_DECLARE(Menu_saft_track);    //安全音轨

LV_IMG_DECLARE(Menu_tx_config);       //Tx设置
LV_IMG_DECLARE(Menu_noise_config);    //降噪设置
LV_IMG_DECLARE(Menu_voice_config);    //音量调节
LV_IMG_DECLARE(Menu_monitor_config);  //监听设置
LV_IMG_DECLARE(Menu_high_true);       //音效模式
LV_IMG_DECLARE(Menu_common_config);   //通用设置

#define MENU_ICON_NUM 6
static int16_t menu_center_icon = 0;
// 下级菜单表
static const hl_screen_page_e next_level_menu_tab[MENU_ICON_NUM] = {
    PAGE_SOUND_MODULE,      PAGE_TX_CONF_MENU, /*PAGE_NOISE_REDUCTION_INTENSITY,*/ PAGE_VOLUME_MENU, PAGE_MONITOR_SET,
    PAGE_SOUND_EFFECT_MODE, PAGE_OTHER_SET
};

static void hl_mod_menu_defaut_set(uint32_t current)
{
    hl_display_screen_change_s* flag = hl_mod_page_get_screen_change_flag();

    switch ((current & 0x7F)) {
        case 0:
            flag->menu_defaut = 1;
            break;
        case 1:
            flag->menu_defaut = 1;
            break;
        case 3:
            flag->menu_defaut = 1;
            break;
        case 6:
            flag->menu_defaut = 1;
            break;
        default:
            break;
    }
}

static void page_7_test_cb(uint32_t current)
{
    if (!(current & 0x80)) {
        hl_mod_menu_defaut_set(current);
        hl_mod_next_menu_enter(next_level_menu_tab, current & 0x7F, MENU_ICON_NUM);
    } else {
        rt_kprintf("page_7_test_cb=%d\n",current & 0x7F);
        hl_mod_knob_select_val_set(&menu_center_icon, current & 0x7F);
    }
}

static void hl_mod_page_setup(void)
{
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();
    hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();
    a6902_language_typ_t* page_ptr = (a6902_language_typ_t *)hl_a6902_language_ptr_get();
    LV_LOG_USER("PAGE_MAIN_MENU\n");
    
    menu_data_t pic_list[MENU_ICON_NUM] = {
        ADD_IMG_DATA(NULL, NULL, &Menu_single_voice, page_ptr->menu_page_ptr->page_menu->ptr_single_voice),//"单声道"),
        ADD_IMG_DATA(NULL, NULL, &Menu_tx_config, page_ptr->menu_page_ptr->page_menu->ptr_tx_config),//"TX设置"),
        // ADD_IMG_DATA(NULL, NULL, &Menu_noise_config, page_ptr->menu_page_ptr->page_menu->ptr_noise_config),//"降噪设置"),
        ADD_IMG_DATA(NULL, NULL, &Menu_voice_config, page_ptr->menu_page_ptr->page_menu->ptr_voice_config),//"音量调节"),
        ADD_IMG_DATA(NULL, NULL, &Menu_monitor_config, page_ptr->menu_page_ptr->page_menu->ptr_monitor_config),//"监听设置"),
        ADD_IMG_DATA(NULL, NULL, &Menu_high_true, page_ptr->menu_page_ptr->page_menu->ptr_high_true),//"音效模式"),
        ADD_IMG_DATA(NULL, NULL, &Menu_common_config, page_ptr->menu_page_ptr->page_menu->ptr_common_config),//"通用设置"),
    };

    switch (data_ptr->now_sound_module) {
        case MONO:
            pic_list[0].lab     = NULL;
            pic_list[0].obj     = NULL;
            pic_list[0].pic_src = &Menu_single_voice;
            pic_list[0].ptr     = page_ptr->menu_page_ptr->page_sound_module->ptr_single_voice;//"单声道";
            break;
        case STEREO:
            pic_list[0].lab     = NULL;
            pic_list[0].obj     = NULL;
            pic_list[0].pic_src = &Menu_stereo;
            pic_list[0].ptr     = page_ptr->menu_page_ptr->page_sound_module->ptr_stereo;//"立体声";
            break;
        case SAFE_TRACK:
            pic_list[0].lab     = NULL;
            pic_list[0].obj     = NULL;
            pic_list[0].pic_src = &Menu_saft_track;
            pic_list[0].ptr     = page_ptr->menu_page_ptr->page_sound_module->ptr_saft_track;//"安全音轨";
            break;
        default:
            break;
    }
    // 如果是下一级菜单，就默认显示最左边图标
    if (flag->menu_defaut) {
        flag->menu_defaut = 0;
        hl_mod_knob_select_val_set(&menu_center_icon, 0);
    }

    page_menu_init(pic_list, MENU_ICON_NUM, page_7_test_cb, menu_center_icon);
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
        rt_kprintf("chg=%d\n",menu_center_icon);
    }
}

// void hl_mod_page_menu_init(void)
PAGE_DEC(PAGE_MAIN_MENU)
{
    bool result;

    result = PageManager_PageRegister(PAGE_MAIN_MENU, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_FIRST_MENU init fail\n");
    }
}

#endif
/*
 * EOF
 */
