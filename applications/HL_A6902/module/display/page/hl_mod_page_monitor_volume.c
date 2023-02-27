/**
 * @file hl_mod_page_volume_cfg.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 音量调节监听音量
 * @version V1.0
 * @date 2022-12-12
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
 * <tr><td>2022-12-12     <td>v1.0     <td>liujie     <td>内容
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

#include "hl_mod_page.h"
#include "lv_port_indev.h"
#include "page_test.h"
#include "page_menu.h"
#include "hl_util_general_type.h"
#include "hl_mod_page_volume_bar_set.h"
#include "page_language.h"

LV_IMG_DECLARE(Other_mic_black);

//UAC输入音量设置界面
static void page_uac_out_init(void)
{
    hl_display_screen_s* data = hl_mod_page_get_screen_data_ptr();
    a6902_language_typ_t* page_ptr = (a6902_language_typ_t *)hl_a6902_language_ptr_get();
    // 设置当前音量
    hl_mod_page_volume_init(data->monitor_volume);

    hl_lvgl_barset_init_t bar_test = {
        .func_cb    = hl_mod_page_volume_update,
        .icontyp    = HL_NO_ICON,
        .init_value = hl_mod_page_volume_get(),
        .ptr        = page_ptr->barset_page_prt->page_monitor_volume->ptr_monitor,//"监听音量",
        .range_max  = MAX_MONITOR_VOLUME,
        .range_min  = MIN_MONITOR_VOLUME,
        .src        = &Other_mic_black,
    };
    hl_mod_barset_init(&bar_test);
}

static void hl_mod_page_setup(void)
{
    page_uac_out_init();
}

static void hl_mod_page_exit(void)
{
    hl_mod_page_volume_exit();
}

static void save_before_back(void)
{
    hl_display_screen_s* ptr = hl_mod_page_get_screen_data_ptr();

    ptr->monitor_volume = hl_mod_page_volume_get();
    LOG_D("vol=[%d]\n", ptr->monitor_volume);
}

static void hl_mod_page_loop(void)
{
    hl_mod_page_volume_loop(MONITOR_VOLUME_VAL_IND, save_before_back,MIN_MONITOR_VOLUME,MAX_MONITOR_VOLUME);
}

PAGE_DEC(PAGE_MONITOR_VOLUME_SET)
{
    bool result;

    result     = PageManager_PageRegister(PAGE_MONITOR_VOLUME_SET, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit,
                                      NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_MONITOR_VOLUME_SET init fail\n");
    }
}

#endif
/*
 * EOF
 */



