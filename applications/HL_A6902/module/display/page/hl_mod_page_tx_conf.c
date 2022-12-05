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
#include "hl_mod_page_menu_frame.h"
#if (!HL_IS_TX_DEVICE())
#include "hl_mod_display.h"
#include "lvgl.h"
#include "rtthread.h"



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
}

static void page_8_test(void)
{
    menu_data_t pic_list[7] = {
        ADD_IMG_DATA(NULL,NULL,&Menu_tx_gain,"TX增益"),
        ADD_IMG_DATA(NULL,NULL,&Menu_low_qie,"低切"),
        ADD_IMG_DATA(NULL,NULL,&Menu_auto_recording,"自动录制"),
        ADD_IMG_DATA(NULL,NULL,&Menu_recording_protection,"录制保护"),
        ADD_IMG_DATA(NULL,NULL,&Menu_storage,"存储"),
        ADD_IMG_DATA(NULL,NULL,&Menu_auto_poweroff,"自动关机"),
        ADD_IMG_DATA(NULL,NULL,&Menu_status_led,"状态灯调节"),
    };
    page_menu_init(pic_list,7,page_8_test_cb);
}



static void hl_mod_page_setup(void)
{
    LV_LOG_USER("PAGE_TX_CONF_MENU\n");
    page_8_test();
}

static void hl_mod_page_loop(void)
{
}

static void hl_mod_page_exit(void)
{
    
}

static void hl_mod_page_event(void* btn, int event)
{

}

void hl_mod_page_txconf_init(void)
{
    bool result;

    result =
        PageManager_PageRegister(PAGE_TX_CONF_MENU, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, hl_mod_page_event);

    if (result == false) {
        LV_LOG_USER("PAGE_TX_CONF_MENU init fail\n");
    }
}
#endif
/*
 * EOF
 */