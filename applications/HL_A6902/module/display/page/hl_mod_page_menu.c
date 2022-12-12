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

// //主菜单页面
// LV_IMG_DECLARE(Menu_single_voice);    //单声道
// LV_IMG_DECLARE(Menu_tx_config);       //Tx设置
// LV_IMG_DECLARE(Menu_noise_config);    //降噪设置
// LV_IMG_DECLARE(Menu_voice_config);    //音量调节
// LV_IMG_DECLARE(Menu_monitor_config);  //监听设置
// LV_IMG_DECLARE(Menu_common_config);   //通用设置

// static void page_7_test_cb(uint32_t current)
// {
//     printf("Page_7 check centre icon event :%d\n", current);
// }

// static void page_7_test(void)
// {
//     menu_data_t pic_list[6] = {
//         ADD_IMG_DATA(NULL, NULL, &Menu_single_voice, "单声道"),
//         ADD_IMG_DATA(NULL, NULL, &Menu_monitor_config, "监听设置"),
//         ADD_IMG_DATA(NULL, NULL, &Menu_tx_config, "TX设置"),
//         // ADD_IMG_DATA(NULL,NULL,&Menu_tx_config,"TX设置"),
//         ADD_IMG_DATA(NULL, NULL, &Menu_noise_config, "降噪设置"),
//         ADD_IMG_DATA(NULL, NULL, &Menu_voice_config, "音量调节"),
        
        
//         ADD_IMG_DATA(NULL, NULL, &Menu_common_config, "通用设置"),
//     };
//     page_menu_init(pic_list, 6, page_7_test_cb);
// }

// ////////////////////////////////////////////////////////////

// static lv_obj_t* app_window;
// static lv_obj_t* btn2;
// lv_obj_t* btn1;

// static void      event_handler(lv_event_t* e)
// {
//     lv_event_code_t code = lv_event_get_code(e);
//     lv_obj_t*       obj  = lv_event_get_target(e);
//     if (code > LV_EVENT_HIT_TEST) {
//         return;
//     }
//     LV_LOG_USER("code = %d\r\n", code);
//     LV_LOG_USER("obj = %d\r\n", obj);
//     LV_LOG_USER("//////////////////\r\n");


//     if (obj == btn2 && code == LV_EVENT_KEY) {
//       //  PageManager_PagePush(PAGE_HOME);
      
//         LV_LOG_USER("new knob\n");

//     //   LV_LOG_USER("key = %d\r\n", lv_indev_get_key(lv_indev_get_act()));

//     }
// }
// // 创建主页 两个按键 一个滚动条
// void hl_app_gui_home_create()
// {
//     lv_obj_t* label;

//     btn1 = lv_btn_create(app_window);
//     lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
//     lv_obj_align_to(btn1, app_window, LV_ALIGN_BOTTOM_LEFT, 0, 0);

//     label = lv_label_create(btn1);
//     lv_label_set_text(label, "Button1");
//     lv_obj_center(label);

//     btn2 = lv_btn_create(app_window);
//     lv_obj_add_event_cb(btn2, event_handler, LV_EVENT_ALL, NULL);

//     lv_obj_align_to(btn2, app_window, LV_ALIGN_TOP_LEFT, 0, 0);
//     //lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
//     lv_obj_set_height(btn2, LV_SIZE_CONTENT);

//     label = lv_label_create(btn2);
//     lv_label_set_text(label, "Button2");
//     lv_obj_center(label);
// }

// // static void hl_mod_page_setup(void)
// // {
// //     /*将此页面移到前台*/
// //     //app_window = hl_app_gui_app_win_init(HOME);
// //     // lv_obj_move_foreground(app_window);
// //     // hl_app_gui_top_update("HOME");
// //     rt_kprintf("pagemenu!\n");
// //     rt_kprintf("pagemenu!=%d\n", lv_obj_get_width(lv_scr_act()));
// //     rt_kprintf("pagemenu!=%d\n", lv_obj_get_height(lv_scr_act()));
// //     hl_app_gui_home_create();
// // }
// static now_pagedatae = 0;
//主菜单页面
LV_IMG_DECLARE(Menu_single_voice);    //单声道
LV_IMG_DECLARE(Menu_tx_config);       //Tx设置
LV_IMG_DECLARE(Menu_noise_config);    //降噪设置
LV_IMG_DECLARE(Menu_voice_config);    //音量调节
LV_IMG_DECLARE(Menu_monitor_config);  //监听设置
LV_IMG_DECLARE(Menu_common_config);   //通用设置

#define MENU_ICON_NUM 6
static uint8_t now_center_icon = 0;
// 下级菜单表
static const hl_screen_page_e next_level_menu_tab[MENU_ICON_NUM] = {
     PAGE_SOUND_MODULE, PAGE_NOISE_REDUCTION_INTENSITY, PAGE_VOLUME_CONTROL, PAGE_TX_CONF_MENU, PAGE_MONITOR_SET,\
     PAGE_OTHER_SET\
};

static void page_7_test_cb(uint32_t current)
{
    printf("Page_7 check centre icon event :%d\n", current);

    
    //未锁屏回主界面
    if(! (current & 0x80) )  {
        hl_mod_next_menu_enter(next_level_menu_tab, current & 0x7F, MENU_ICON_NUM);
    } else {
        now_center_icon = current & 0x7F;
    }

}

static void hl_mod_page_setup(void)
{
    LV_LOG_USER("PAGE_MAIN_MENU\n");
    menu_data_t pic_list[MENU_ICON_NUM] = {
        ADD_IMG_DATA(NULL, NULL, &Menu_single_voice, "单声道"),
        // ADD_IMG_DATA(NULL,NULL,&Menu_tx_config,"TX设置"),
        ADD_IMG_DATA(NULL, NULL, &Menu_noise_config, "降噪设置"),
        ADD_IMG_DATA(NULL, NULL, &Menu_voice_config, "音量调节"),
        ADD_IMG_DATA(NULL, NULL, &Menu_tx_config, "TX设置"),
        ADD_IMG_DATA(NULL, NULL, &Menu_monitor_config, "监听设置"),
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
    hl_mod_menu_enterbtn_scan(now_center_icon);
    hl_mod_menu_backbtn_scan();
    hl_mod_menu_knob_icon_change(&now_center_icon,MENU_ICON_NUM);
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



