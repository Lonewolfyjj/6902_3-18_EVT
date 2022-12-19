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
#include "hl_mod_input.h"


LV_IMG_DECLARE(Other_mic_black);


static void hl_bar_test_cb(int16_t bar_num)
{
    printf("bar_num = %d\n", bar_num);
}

//监听音量设置界面
static void page_monit_init(void)
{
    hl_lvgl_barset_init_t bar_test = 
    {
        .func_cb = hl_bar_test_cb,
        .icontyp = HL_NO_ICON,
        .init_value = -16,
        .ptr = "监听音量",
        .range_max = 40,
        .range_min = -60,
        .src = &Other_mic_black,
    };
    hl_mod_barset_init(&bar_test);
}


static void hl_mod_page_setup(void)
{
    page_monit_init();
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_barset_ioctl_t ctl = {
        .barset_value = HL_EXTI,
    };
    hl_mod_barset_ioctl(&ctl);
    ctl.barset_value = HL_DELETE_STYLE;
    hl_mod_barset_ioctl(&ctl);
}

static void hl_mod_page_loop(void)
{
    // 返回按键
    hl_mod_menu_backbtn_scan();
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



