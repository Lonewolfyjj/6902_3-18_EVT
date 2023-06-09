/**
 * @file hl_mod_page_tx_conf.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-12-05
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
 * <tr><td>2022-12-05     <td>v1.0     <td>liujie     <td>内容
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
#include "rtthread.h"
#include "hl_mod_page.h"
#include "hl_util_general_type.h"
#include "page_menu.h"
#include "page_language.h"
#define MENU_ICON_NUM 7

static int8_t menu_center_icon = 0;

// 下级菜单表
static const hl_screen_page_e next_level_menu_tab[MENU_ICON_NUM] = {
     PAGE_TX_GAIN_CONF,PAGE_TX_LOW_CUT ,PAGE_AUTO_RECORD, PAGE_RECORD_PROTECT,PAGE_RECORD_FORMAT ,PAGE_AUTO_POWEROFF,
     PAGE_TX_LED_BRIGHT
};


//Tx菜单界面
LV_IMG_DECLARE(Menu_tx_gain);//TX增益
LV_IMG_DECLARE(Menu_low_qie);//低切
LV_IMG_DECLARE(Menu_auto_recording);//自动录制
LV_IMG_DECLARE(Menu_recording_protection);//录制保护
LV_IMG_DECLARE(Menu_storage);//存储
LV_IMG_DECLARE(Menu_auto_poweroff);//自动关机
LV_IMG_DECLARE(Menu_status_led);//状态灯调节

static void page_8_test_cb(uint32_t current)
{ 
    if (!(current & 0x80)) {
        hl_mod_next_menu_enter(next_level_menu_tab, current & 0x7F, MENU_ICON_NUM);
    } else {
        hl_mod_knob_select_val_set(&menu_center_icon, current & 0x7F);
    }
}

static void hl_mod_page_setup(void)
{
    hl_display_screen_change_s* flag = hl_mod_page_get_screen_change_flag();
    a6902_language_typ_t* page_ptr = (a6902_language_typ_t *)hl_a6902_language_ptr_get();
    // LV_LOG_USER("PAGE_TX_CONF_MENU\n");    
    menu_data_t pic_list[MENU_ICON_NUM] = {
        ADD_IMG_DATA(NULL,NULL,&Menu_tx_gain,page_ptr->menu_page_ptr->page_tx_config->ptr_tx_gain),//"TX增益"),
        ADD_IMG_DATA(NULL,NULL,&Menu_low_qie,page_ptr->menu_page_ptr->page_tx_config->ptr_low_qie),//"低切"),
        ADD_IMG_DATA(NULL,NULL,&Menu_auto_recording,page_ptr->menu_page_ptr->page_tx_config->ptr_auto_recording),//"自动录制"),
        ADD_IMG_DATA(NULL,NULL,&Menu_recording_protection,page_ptr->menu_page_ptr->page_tx_config->ptr_recording_protection),//"录制保护"),
        ADD_IMG_DATA(NULL,NULL,&Menu_storage,page_ptr->menu_page_ptr->page_tx_config->ptr_storage),//"存储"),
        ADD_IMG_DATA(NULL,NULL,&Menu_auto_poweroff,page_ptr->menu_page_ptr->page_tx_config->ptr_auto_poweroff),//"自动关机"),
        ADD_IMG_DATA(NULL,NULL,&Menu_status_led,page_ptr->menu_page_ptr->page_tx_config->ptr_status_led),//"状态灯调节"),
    };
    // 如果是当前页面是下一级菜单，就默认居中最左边图标；如果是当前页面下一级返回的，就默认居中上一次的图标
    if (flag->menu_defaut) {
        flag->menu_defaut = 0;
        hl_mod_knob_select_val_set(&menu_center_icon, 0);
    }
    page_menu_init(pic_list,MENU_ICON_NUM,page_8_test_cb,menu_center_icon);
}

static void hl_mod_page_loop(void)
{
    hl_mod_menu_enterbtn_scan(hl_mod_knob_select_val_get(&menu_center_icon));
    hl_mod_menu_backbtn_scan();
    if (hl_mod_knob_select_val_change(&menu_center_icon,0,MENU_ICON_NUM - 1, false) ) {
        lv_set_icon_postion(menu_center_icon, false);
    }
}

static void hl_mod_page_exit(void)
{
    lv_menu_exit();
}



PAGE_DEC(PAGE_TX_CONF_MENU)
{
    bool result;

    result =
        PageManager_PageRegister(PAGE_TX_CONF_MENU, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_TX_CONF_MENU init fail\n");
    }
}
#endif
/*
 * EOF
 */