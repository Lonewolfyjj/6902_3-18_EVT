/**
 * @file hl_mod_page_out_box_charge.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2023-01-04
 * 
 * ██╗  ██╗ ██████╗ ██╗     ██╗  ██╗   ██╗██╗      █████╗ ███╗   ██╗██████╗ 
 * ██║  ██║██╔═══██╗██║     ██║  ╚██╗ ██╔╝██║     ██╔══██╗████╗  ██║██╔══██╗
 * ███████║██║   ██║██║     ██║   ╚████╔╝ ██║     ███████║██╔██╗ ██║██║  ██║
 * ██╔══██║██║   ██║██║     ██║    ╚██╔╝  ██║     ██╔══██║██║╚██╗██║██║  ██║
 * ██║  ██║╚██████╔╝███████╗███████╗██║   ███████╗██║  ██║██║ ╚████║██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝
 * @copyright Copyright (c) 2023 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2023-01-04     <td>v1.0     <td>liujie     <td>内容
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
#include "page_holding.h"
#include "hl_util_general_type.h"

static void hl_mod_page_setup(void)
{
    hl_display_screen_s*    data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_lvgl_holding_init_t  init;
    hl_lvgl_holding_ioctl_t ioctrl;

    init.electric = data_ptr->rx_bat_val;
    hl_mod_holding_init(&init);

    ioctrl.holding_cmd = HL_HOLDING_RX_BAT_COLOR_GREEN;
    hl_mod_holding_ioctl(&ioctrl);

    ioctrl.holding_cmd = HL_HOLDING_RX_ICON_DISHIDE;
    hl_mod_holding_ioctl(&ioctrl);
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_holding_ioctl_t ioctrl;

    ioctrl.holding_cmd = HL_HOLDING_DELETE_PAGE;
    hl_mod_holding_ioctl(&ioctrl);
}

static void hl_mod_page_loop(void)
{
    uint8_t                     rx_bat_val;
    hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();

    if (flag->sys_status.rx_charge_status) {
        hl_mod_display_mux_take();
        flag->sys_status.rx_charge_status = 0;
        rx_bat_val                        = data_ptr->sys_status.rx_charge_status;
        hl_mod_display_mux_release();

        hl_lvgl_holding_ioctl_t ioctrl;

        if (data_ptr->sys_status.rx_charge_status) {
            ioctrl.holding_cmd = HL_HOLDING_RX_BAT_COLOR_GREEN;
            hl_mod_holding_ioctl(&ioctrl);

            ioctrl.holding_cmd = HL_HOLDING_RX_ICON_DISHIDE;
            hl_mod_holding_ioctl(&ioctrl);
        } else {
            ioctrl.holding_cmd = HL_HOLDING_RX_BAT_COLOR_WHITE;
            hl_mod_holding_ioctl(&ioctrl);

            ioctrl.holding_cmd = HL_HOLDING_RX_ICON_HIDE;
            hl_mod_holding_ioctl(&ioctrl);
        }
    }

    if (flag->rx_bat_val) {
        hl_mod_display_mux_take();
        flag->rx_bat_val = 0;
        rx_bat_val       = data_ptr->rx_bat_val;
        hl_mod_display_mux_release();

        hl_lvgl_holding_ioctl_t ioctrl;

        ioctrl.holding_cmd = HL_HOLDING_RX_ELEC;
        hl_mod_holding_ioctl(&ioctrl);
    }
}

PAGE_DEC(PAGE_POWEROFF_CHARGE)
{
    bool result;

    result =
        PageManager_PageRegister(PAGE_POWEROFF_CHARGE, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_POWEROFF_CHARGE init fail\n");
    }
}

#endif
/*
 * EOF
 */
