/**
 * @file hl_mod_page_home.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-11-30
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
 * <tr><td>2022-11-30     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/

#include "hl_mod_page_common.h"

#if (!HL_IS_TX_DEVICE())
#include "hl_mod_display.h"
#include "lvgl.h"
#include "rtthread.h"
#include "hl_mod_page.h"
#include "page_test.h"
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

static void hl_mod_page_setup(void)
{
    
}
static void hl_mod_page_loop(void)
{
    
}

static void hl_mod_page_exit(void)
{
    // hl_mod_page_delete(lv_scr_act());
    // uint8_t i = 0;

    // lv_obj_del(area_tx1);
    // lv_obj_del(area_tx2);
    // lv_obj_del(voice_bar_tx1);
    // lv_obj_del(voice_bar_tx2);
    // lv_obj_del(power_bar_tx1);
    // lv_obj_del(power_bar_tx2);
    // lv_obj_del(voice_img_tx1);
    // lv_obj_del(voice_img_tx2);
    // lv_obj_del(power_img_tx1);
    // lv_obj_del(power_img_tx2);
    // lv_obj_del(voice_lab_tx1);
    // lv_obj_del(voice_lab_tx2);
    // lv_obj_del(power_lab_tx1);
    // lv_obj_del(power_lab_tx2);
    // lv_obj_del(device_lab_tx1);
    // lv_obj_del(device_lab_tx2);
    // lv_obj_del(video_dot_tx1);
    // lv_obj_del(video_dot_tx2);

    // for (i = 0; i < 5; i++) {
    //     lv_obj_del(tx1_signal_obj[i]);
    //     lv_obj_del(tx2_signal_obj[i]);
    // }
}

static void hl_mod_page_event(void* btn, int event)
{
}

void hl_mod_page_home_init(void)
{
    bool result;

    // app_window = lv_obj_create(lv_scr_act());
    // app_window = lv_scr_act();

    result =
        PageManager_PageRegister(PAGE_HOME, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, hl_mod_page_event);

    if (result == false) {
        LV_LOG_USER("PAGE_HOME init fail\n");
    }
}
#endif
/*
 * EOF
 */