/**
 * @file hl_mod_page_noise_level.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-12-09
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
 * <tr><td>2022-12-09     <td>v1.0     <td>liujie     <td>内容
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

//1-7 一共7个档位
#define MAX_LEVEL       7
#define MIN_LEVEL       1


LV_IMG_DECLARE(Other_mic_black);
LV_IMG_DECLARE(Other_voice);


static void hl_mod_page_setup(void)
{
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();

        // 设置当前音量
    hl_mod_page_volume_init(data_ptr->tx_noise_level);

    hl_lvgl_barset_init_t bar_test = {
        .func_cb    = hl_mod_page_volume_update,
        .icontyp    = HL_NO_ICON,
        .init_value = hl_mod_page_volume_get(),
        .ptr        = "降噪调节",
        .range_max  = MAX_LEVEL,
        .range_min  = MIN_LEVEL,
        .src        = &Other_mic_black,
    };
    hl_mod_barset_init(&bar_test);
}

static void hl_mod_page_exit(void)
{
    lv_menu_exit();
}

static void save_before_back(void)
{
    hl_display_screen_s* ptr = hl_mod_page_get_screen_data_ptr();

    ptr->tx_noise_level = hl_mod_page_volume_get();
    LOG_D("vol=[%d]\n", ptr->tx_noise_level);
}

static void hl_mod_page_loop(void)
{
    hl_mod_page_volume_loop(TX_NOISE_LEVEL_VAL_IND, save_before_back,1,MAX_LEVEL);
}

PAGE_DEC(PAGE_NOISE_REDUCTION_INTENSITY)
{
    bool result;

    result = PageManager_PageRegister(PAGE_NOISE_REDUCTION_INTENSITY, hl_mod_page_setup, hl_mod_page_loop,
                                      hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("REDUCTION_INTENSITY init fail\n");
    }
}

#endif
/*
 * EOF
 */



