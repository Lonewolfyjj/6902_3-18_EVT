/**
 * @file hl_mod_page_volume_bar_set.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief lineout页面，立体声的左声道或者安全音轨和单声道的声道
 * @version V1.0
 * @date 2022-12-20
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
 * <tr><td>2022-12-20     <td>v1.0     <td>liujie     <td>内容
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
#include "hl_util_general_type.h"
#include "lvgl.h"
#include "hl_util_timeout.h"
#include "hl_mod_page.h"
#include "lv_port_indev.h"
#include "page_test.h"
#include "hl_mod_page_volume_bar_set.h"
#include "page_language.h"


LV_IMG_DECLARE(Other_voice);
LV_IMG_DECLARE(Other_mic_black);

    

    

//左声道设置界面
static void page_voc_bar_left_init()
{
    hl_display_screen_s* data = hl_mod_page_get_screen_data_ptr();
    a6902_language_typ_t* page_ptr = (a6902_language_typ_t *)hl_a6902_language_ptr_get();
    hl_mod_page_volume_init(data->tx1_line_out_volume);

    hl_lvgl_barset_init_t bar_test = {
        .func_cb    = hl_mod_page_volume_update,
        .icontyp    = HL_STEREO_ICON,
        .init_value = hl_mod_page_volume_get(),
        .ptr        = page_ptr->barset_page_prt->page_lineout_left,//"左声道",
        .range_max  = MAX_LINEOUT_VOLUME,
        .range_min  = MIN_LINEOUT_VOLUME,
        .src        = &Other_voice,
    };
    hl_mod_barset_init(&bar_test);
}

//mono声道设置界面
static void page_voc_bar_mono_init()
{
    hl_display_screen_s* data = hl_mod_page_get_screen_data_ptr();

    hl_mod_page_volume_init(data->mono_line_out_volume);

    hl_lvgl_barset_init_t bar_test = {
        .func_cb    = hl_mod_page_volume_update,
        .icontyp    = HL_SINGLE_ICON,
        .init_value = hl_mod_page_volume_get(),
        .ptr        = "单声道",
        .range_max  = MAX_LINEOUT_VOLUME,
        .range_min  = MIN_LINEOUT_VOLUME,
        .src        = &Other_voice,
    };
    hl_mod_barset_init(&bar_test);
}

//安全音轨设置界面
static void page_voc_bar_safe_track_init()
{
    hl_display_screen_s* data = hl_mod_page_get_screen_data_ptr();

    hl_mod_page_volume_init(data->track_line_out_volume);

    hl_lvgl_barset_init_t bar_test = {
        .func_cb    = hl_mod_page_volume_update,
        .icontyp    = HL_SAYFT_ICON,
        .init_value = hl_mod_page_volume_get(),
        .ptr        = "安全音轨",
        .range_max  = MAX_LINEOUT_VOLUME,
        .range_min  = MIN_LINEOUT_VOLUME,
        .src        = &Other_voice,
    };
    hl_mod_barset_init(&bar_test);
}



static void hl_mod_page_setup(void)
{
    hl_display_screen_s* data = hl_mod_page_get_screen_data_ptr();

    switch (data->now_sound_module) {
        case STEREO:
            page_voc_bar_left_init();
            break;
        case MONO:
            page_voc_bar_mono_init();
            break;
        case SAFE_TRACK:
            page_voc_bar_safe_track_init();
            break;
        default:
            break;
    }
}

static void hl_mod_page_exit(void)
{
    hl_mod_page_volume_exit();
}

static void save_before_back(void)
{
    hl_display_screen_s* data = hl_mod_page_get_screen_data_ptr();

    hl_mod_display_mux_take();
    switch (data->now_sound_module) {
        case STEREO:
            data->tx1_line_out_volume = hl_mod_page_volume_get();
            break;
        case MONO:
            data->mono_line_out_volume = hl_mod_page_volume_get();
            break;
        case SAFE_TRACK:
            data->track_line_out_volume = hl_mod_page_volume_get();
            break;
        default:
            break;
    }
    hl_mod_display_mux_release();
}

static void hl_mod_page_loop(void)
{
    hl_display_screen_s* data = hl_mod_page_get_screen_data_ptr();

    switch (data->now_sound_module) {
        case STEREO:
            hl_mod_page_volume_loop(TX1_LINE_OUT_VOLUME_VAL_IND, save_before_back, MIN_LINEOUT_VOLUME,
                                    MAX_LINEOUT_VOLUME);
            break;
        case MONO:
            hl_mod_page_volume_loop(MONO_LINE_OUT_VOLUME_VAL_IND, save_before_back, MIN_LINEOUT_VOLUME,
                                    MAX_LINEOUT_VOLUME);
            break;
        case SAFE_TRACK:
            hl_mod_page_volume_loop(SAFETRACK_LINE_OUT_VOLUME_VAL_IND, save_before_back, MIN_LINEOUT_VOLUME,
                                    MAX_LINEOUT_VOLUME);
            break;
        default:
            break;
    }
}

PAGE_DEC(PAGE_LINE_OUT_STEREO_LEFT)
{
    bool result;

    result = PageManager_PageRegister(PAGE_LINE_OUT_STEREO_LEFT, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit,
                                      NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_LINE_OUT_STEREO_LEFT init fail\n");
    }
}

#endif
/*
 * EOF
 */
