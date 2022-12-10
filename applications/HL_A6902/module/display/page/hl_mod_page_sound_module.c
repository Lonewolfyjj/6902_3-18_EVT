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


#define MENU_ICON_NUM 3
static uint8_t now_center_icon = 0;
// 下级菜单表
// static const hl_screen_page_e next_level_menu_tab[MENU_ICON_NUM] = {
//     PAGE_SOUND_MODULE, PAGE_NOISE_REDUCTION_INTENSITY, PAGE_VOLUME_CONTROL, PAGE_TX_CONF_MENU, PAGE_MONITOR_SET,
//     PAGE_OTHER_SET\
// };

//模式选择菜单界面
LV_IMG_DECLARE(Menu_single_voice);//单声道
LV_IMG_DECLARE(Menu_stereo);//立体声
LV_IMG_DECLARE(Menu_saft_track);//安全音轨

static void page_10_test_cb(uint32_t current)
{
    rt_kprintf("Page_s:%x\n",current);

    //未锁屏回主界面
    if(! (current & 0x80) )  {
        hl_mod_menu_goto_home_page();
    } else {
        now_center_icon = current & 0x7F;
    }
    
}

static void hl_mod_page_setup(void)
{
    menu_data_t pic_list[MENU_ICON_NUM] = {
        ADD_IMG_DATA(NULL,NULL,&Menu_single_voice,"单声道"),
        ADD_IMG_DATA(NULL,NULL,&Menu_stereo,"立体声"),
        ADD_IMG_DATA(NULL,NULL,&Menu_saft_track,"安全音轨"),
    };
    page_menu_init(&pic_list,MENU_ICON_NUM,page_10_test_cb);
}

static void hl_mod_page_exit(void)
{
    lv_menu_exit();
}

static void hl_mod_page_loop(void)
{

    //菜单点击按键   
    hl_mod_menu_enterbtn_scan(now_center_icon);

    // 返回按键
    hl_mod_menu_backbtn_scan();

    hl_mod_menu_knob_icon_change(now_center_icon,MENU_ICON_NUM);
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



