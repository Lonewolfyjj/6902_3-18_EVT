/**
 * @file hl_mod_page_tx1gain_set.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
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

LV_IMG_DECLARE(Other_voice);
LV_IMG_DECLARE(Other_mic_black);

static void page_voc_bar_tx1_init(void)
{
    hl_display_screen_s* data = hl_mod_page_get_screen_data_ptr();
    // 设置当前音量
    hl_mod_page_volume_init(data->tx1_gain_volume);

    hl_lvgl_barset_init_t bar_test = {
        .func_cb    = hl_mod_page_volume_update,
        .icontyp    = HL_NO_ICON,
        .init_value = hl_mod_page_volume_get(),
        .ptr        = "TX1",
        .range_max  = MAX_LINEOUT_VOLUME,
        .range_min  = MIN_LINEOUT_VOLUME,
        .src        = &Other_mic_black,
    };
    hl_mod_barset_init(&bar_test);
}

static void hl_mod_page_setup(void)
{
    page_voc_bar_tx1_init();
}

static void hl_mod_page_exit(void)
{
    hl_mod_page_volume_exit();
}

static void save_before_back(void)
{
    hl_display_screen_s* ptr = hl_mod_page_get_screen_data_ptr();
    
    ptr->tx1_gain_volume     = hl_mod_page_volume_get();
    LOG_D("vol=[%d]\n", ptr->tx1_gain_volume);
}

static void hl_mod_page_loop(void)
{
    hl_mod_page_volume_loop(TX1_GAIN_VAL_IND, save_before_back,MIN_LINEOUT_VOLUME,MAX_LINEOUT_VOLUME);
}

PAGE_DEC(PAGE_TX_GAIN_TX1)
{
    bool result;

    result = PageManager_PageRegister(PAGE_TX_GAIN_TX1, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_LINE_OUT_STEREO_LEFT init fail\n");
    }
}

#endif
/*
 * EOF
 */
