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
#include "hl_mod_input.h"
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

//主菜单页面
LV_IMG_DECLARE(Menu_single_voice);    //单声道
LV_IMG_DECLARE(Menu_tx_config);       //Tx设置
LV_IMG_DECLARE(Menu_noise_config);    //降噪设置
LV_IMG_DECLARE(Menu_voice_config);    //音量调节
LV_IMG_DECLARE(Menu_monitor_config);  //监听设置
LV_IMG_DECLARE(Menu_high_true);       //音效模式
LV_IMG_DECLARE(Menu_common_config);   //通用设置

#define MENU_ICON_NUM 7
static uint8_t now_center_icon = 0;
// 下级菜单表
static const hl_screen_page_e next_level_menu_tab[MENU_ICON_NUM] = {
     PAGE_SOUND_MODULE,PAGE_TX_CONF_MENU, PAGE_NOISE_REDUCTION_INTENSITY,PAGE_VOLUME_MENU, PAGE_MONITOR_SET,PAGE_SOUND_EFFECT_MODE ,\
     PAGE_OTHER_SET\
};

static void page_7_test_cb(uint32_t current)
{

    if (hl_mod_menu_icon_event(current)) {
        hl_mod_next_menu_enter(next_level_menu_tab, current & 0x7F, MENU_ICON_NUM);
    }
}

static void hl_mod_page_setup(void)
{
    LV_LOG_USER("PAGE_MAIN_MENU\n");
    menu_data_t pic_list[MENU_ICON_NUM] = {
        ADD_IMG_DATA(NULL, NULL, &Menu_single_voice, "单声道"),
        ADD_IMG_DATA(NULL,NULL,&Menu_tx_config,"TX设置"),
        ADD_IMG_DATA(NULL, NULL, &Menu_noise_config, "降噪设置"),
        ADD_IMG_DATA(NULL, NULL, &Menu_voice_config, "音量调节"),
        ADD_IMG_DATA(NULL, NULL, &Menu_monitor_config, "监听设置"),
        ADD_IMG_DATA(NULL, NULL, &Menu_high_true, "音效模式"),
        ADD_IMG_DATA(NULL, NULL, &Menu_common_config, "通用设置"),
    };
    page_menu_init(pic_list, MENU_ICON_NUM, page_7_test_cb);
}

static void hl_mod_page_exit(void)
{
    lv_menu_exit();
}

static void hl_mod_page_loop(void)
{
    hl_mod_menu_enterbtn_scan(hl_mod_menu_get_icon());
    hl_mod_menu_backbtn_scan();
    hl_mod_menu_knob_icon_change(hl_mod_menu_get_icon(),MENU_ICON_NUM);
}

// void hl_mod_page_menu_init(void)
PAGE_DEC(PAGE_MAIN_MENU)
{
    bool result;

    result     = PageManager_PageRegister(PAGE_MAIN_MENU, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit,
                                      NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_FIRST_MENU init fail\n");
    }
}

#endif
/*
 * EOF
 */



