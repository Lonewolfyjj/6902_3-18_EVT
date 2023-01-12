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

static int16_t now_choose;
    // static void update_ver()
    // {
    //     hl_lvgl_verson_ioctl_t data;
    //     hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();
    //     hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();

    //     if (flag->rx_ver) {
    //         hl_mod_display_mux_take();
    //         flag->rx_ver            = 0;
    //         data.hl_verson_firmware = data_ptr->rx_ver;
    //         hl_mod_display_mux_release();
    //         hl_mod_verson_ioctl(&data);
    //     }

    //     if (flag->tx1_ver) {
    //         hl_mod_display_mux_take();
    //         flag->tx1_ver      = 0;
    //         data.hl_verson_tx1 = data_ptr->tx1_ver;
    //         hl_mod_display_mux_release();
    //         hl_mod_verson_ioctl(&data);
    //     }

    //     if (flag->tx2_ver) {
    //         hl_mod_display_mux_take();
    //         flag->tx1_ver      = 0;
    //         data.hl_verson_tx2 = data_ptr->tx1_ver;
    //         hl_mod_display_mux_release();
    //         hl_mod_verson_ioctl(&data);
    //     }

    //     if (flag->rx_sn) {
    //         hl_mod_display_mux_take();
    //         flag->sn                 = 0;
    //         data.hl_verson_sn_number = data_ptr->sn;
    //         hl_mod_display_mux_release();
    //         hl_mod_verson_ioctl(&data);
    //     }
    // }

static void hl_mod_page_setup(void)
{
    hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();
    hl_mod_display_mux_take();
    hl_lvgl_verson_init_t data = { .hl_verson_firmware  = data_ptr->rx_ver,
                                   .hl_verson_sn_number = data_ptr->sn,
                                   .hl_verson_tx1       = data_ptr->tx1_ver,
                                   .hl_verson_tx2       = data_ptr->tx2_ver };

    if (flag->rx_ver) {

        flag->rx_ver            = 0;
        data.hl_verson_firmware = data_ptr->rx_ver;
    }

    if (flag->tx1_ver) {

        flag->tx1_ver      = 0;
        data.hl_verson_tx1 = data_ptr->tx1_ver;

    }

    if (flag->tx2_ver) {

        flag->tx1_ver      = 0;
        data.hl_verson_tx2 = data_ptr->tx1_ver;

    }

    if (flag->rx_sn) {

        flag->rx_sn              = 0;
        data.hl_verson_sn_number = data_ptr->sn;

    }

    hl_mod_verson_init(&data);
    hl_mod_display_mux_release();

    hl_lvgl_verson_ioctl_t verson_ctl = {
        .verson_choose_opt = HL_VERSON_OPTION_ONE,
    };
    now_choose = (int16_t)verson_ctl.verson_choose_opt;
    hl_mod_verson_ioctl(&verson_ctl);
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_verson_ioctl_t verson_ctl = {
        .verson_choose_opt = HL_VERSON_OPTION_EXTI,
    };
    hl_mod_verson_ioctl(&verson_ctl);
    // verson_ctl.verson_choose_opt = HL_VERSON_OPTION_DEL_STYLE;
    // hl_mod_verson_ioctl(&verson_ctl);
}

static void hl_mod_page_loop(void)
{
    // 返回按键
    hl_mod_menu_backbtn_scan();

    // 旋钮选择
    if (hl_mod_knob_select_val_change(&now_choose, 0, 2, false)) {

        hl_lvgl_verson_ioctl_t verson_ctl = {
            .verson_choose_opt = now_choose,
        };
        hl_mod_verson_ioctl(&verson_ctl);
    }
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
