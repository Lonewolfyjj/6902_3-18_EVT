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
#include "hl_mod_page_menu_frame.h"
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

//主菜单页面
LV_IMG_DECLARE(Menu_single_voice);//单声道
LV_IMG_DECLARE(Menu_tx_config);//Tx设置
LV_IMG_DECLARE(Menu_noise_config);//降噪设置
LV_IMG_DECLARE(Menu_voice_config);//音量调节
LV_IMG_DECLARE(Menu_monitor_config);//监听设置
LV_IMG_DECLARE(Menu_common_config);//通用设置



static void page_7_test_cb(uint32_t current)
{
    printf("Page_7 check centre icon event :%d\n",current);
}

static void page_7_test(void)
{
menu_data_t pic_list[6] = {
    ADD_IMG_DATA(NULL,NULL,&Menu_single_voice,"单声道"),
    // ADD_IMG_DATA(NULL,NULL,&Menu_tx_config,"TX设置"),
    ADD_IMG_DATA(NULL,NULL,&Menu_noise_config,"降噪设置"),        
    ADD_IMG_DATA(NULL,NULL,&Menu_voice_config,"音量调节"),
    ADD_IMG_DATA(NULL,NULL,&Menu_tx_config,"TX设置"),
    ADD_IMG_DATA(NULL,NULL,&Menu_monitor_config,"监听设置"),
    ADD_IMG_DATA(NULL,NULL,&Menu_common_config,"通用设置"),
};
    page_menu_init(pic_list,6,page_7_test_cb);
}

static void hl_mod_page_setup(void)
{
    /*将此页面移到前台*/
    //app_window = hl_app_gui_app_win_init(HOME);
    // lv_obj_move_foreground(app_window);
    // hl_app_gui_top_update("HOME");
    rt_kprintf("pagemenu!\n");
   page_7_test();
}

static void hl_mod_page_loop(void)
{

}




static void hl_mod_page_exit(void)
{
    // lv_scr_load_anim(hl_app_gui_get_page1(),LV_SCR_LOAD_ANIM_MOVE_RIGHT,500,100,true);
    /*删除此页面上的子控件*/

    hl_mod_page_delete(lv_scr_act());
}

static void hl_mod_page_event(void* btn, int event)
{
}

void hl_mod_page_menu_init(void)
{
    bool result;

    result =
        PageManager_PageRegister(PAGE_MAIN_MENU, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, hl_mod_page_event);

    if (result == false) {
        LV_LOG_USER("PAGE_FIRST_MENU init fail\n");
    }
}

#endif
/*
 * EOF
 */



