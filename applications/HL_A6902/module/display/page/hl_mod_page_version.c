/**
 * @file hl_mod_page_version.c
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

static void hl_mod_page_setup(void)
{
    hl_lvgl_verson_init_t data = { .hl_verson_firmware  = "v1.0.0.0",
                                   .hl_verson_sn_number = "ME234576",
                                   .hl_verson_tx1       = "v1.0.0.1",
                                   .hl_verson_tx2       = "v1.0.0.2" };
    hl_mod_verson_init(&data);

    hl_lvgl_verson_ioctl_t verson_ctl = {
        .verson_choose_opt = HL_VERSON_OPTION_TWO,
    };
    hl_mod_verson_ioctl(&verson_ctl);
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_verson_ioctl_t verson_ctl = {
        .verson_choose_opt = HL_VERSON_OPTION_EXTI,
    };
    hl_mod_verson_ioctl(&verson_ctl);
    verson_ctl.verson_choose_opt = HL_VERSON_OPTION_DEL_STYLE;
    hl_mod_verson_ioctl(&verson_ctl);
}

static void hl_mod_page_loop(void)
{
    // 返回按键
    hl_mod_menu_backbtn_scan();
}

PAGE_DEC(PAGE_VERSION)
{
    bool result;

    result = PageManager_PageRegister(PAGE_VERSION, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_PAIR init fail\n");
    }
}

#endif
/*
 * EOF
 */
