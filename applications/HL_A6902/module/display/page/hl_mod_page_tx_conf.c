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
#include "hl_mod_input.h"
#include "page_menu.h"

#define MENU_ICON_NUM 7

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
    printf("Page_8 check centre icon event :%d\n",current);
    
    if (hl_mod_menu_icon_event(current)) {
        hl_mod_next_menu_enter(next_level_menu_tab, current & 0x7F, MENU_ICON_NUM);
    }
}

static void hl_mod_page_setup(void)
{
    // LV_LOG_USER("PAGE_TX_CONF_MENU\n");
    menu_data_t pic_list[MENU_ICON_NUM] = {
        ADD_IMG_DATA(NULL,NULL,&Menu_tx_gain,"TX增益"),
        ADD_IMG_DATA(NULL,NULL,&Menu_low_qie,"低切"),
        ADD_IMG_DATA(NULL,NULL,&Menu_auto_recording,"自动录制"),
        ADD_IMG_DATA(NULL,NULL,&Menu_recording_protection,"录制保护"),
        ADD_IMG_DATA(NULL,NULL,&Menu_storage,"存储"),
        ADD_IMG_DATA(NULL,NULL,&Menu_auto_poweroff,"自动关机"),
        ADD_IMG_DATA(NULL,NULL,&Menu_status_led,"状态灯调节"),
    };
    // 旋钮复位
    hl_mod_menu_icon_init();
    page_menu_init(pic_list,MENU_ICON_NUM,page_8_test_cb);
}

static void hl_mod_page_loop(void)
{
    hl_mod_menu_enterbtn_scan(hl_mod_menu_get_icon());
    hl_mod_menu_backbtn_scan();
    hl_mod_menu_knob_icon_change(hl_mod_menu_get_icon(),MENU_ICON_NUM);
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