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

// 电池的充电和欠压状态 默认是绿色充电状态
static int16_t rx_bat_state_last = 2;

static void hl_mod_page_setup(void)
{
    hl_display_screen_s*    data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_lvgl_holding_init_t  init;
    hl_lvgl_holding_ioctl_t ioctrl;

    init.electric = data_ptr->rx_bat_val;
    hl_mod_holding_init(&init);

    rx_bat_state_last = 2;

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

static void hl_mod_home_rx_bat_update(hl_display_screen_s* data_ptr)
{
    hl_lvgl_holding_ioctl_t ioctrl;

    uint8_t rx_bat_state;
    uint8_t now_bat_val = data_ptr->rx_bat_val;

    rx_bat_state = bat_state_deal(data_ptr->sys_status.rx_charge_status, now_bat_val, 6);

    // 更新当前的RX电量信息
    ioctrl.holding_cmd = HL_HOLDING_RX_ELEC;
    ioctrl.electric    = now_bat_val;
    hl_mod_holding_ioctl(&ioctrl);

    switch (rx_bat_state) {
        case 0:
            ioctrl.electric    = rx_bat_state_last;
            ioctrl.holding_cmd = HL_HOLDING_RX_BAT_COLOR_WHITE;
            hl_mod_holding_ioctl(&ioctrl);

            ioctrl.holding_cmd = HL_HOLDING_RX_ICON_HIDE;
            hl_mod_holding_ioctl(&ioctrl);
            break;
        case 2:
            ioctrl.electric    = rx_bat_state_last;
            ioctrl.holding_cmd = HL_HOLDING_RX_BAT_COLOR_GREEN;
            hl_mod_holding_ioctl(&ioctrl);

            ioctrl.holding_cmd = HL_HOLDING_RX_ICON_DISHIDE;
            hl_mod_holding_ioctl(&ioctrl);
            break;
        default:
            ioctrl.electric    = rx_bat_state_last;
            ioctrl.holding_cmd = HL_HOLDING_RX_BAT_COLOR_WHITE;
            hl_mod_holding_ioctl(&ioctrl);

            ioctrl.holding_cmd = HL_HOLDING_RX_ICON_HIDE;
            hl_mod_holding_ioctl(&ioctrl);
            break;
    }
    rx_bat_state_last = rx_bat_state;
}

static void hl_mod_page_loop(void)
{
    hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();

    if (flag->rx_bat_val || flag->sys_status.rx_charge_status) {
        hl_mod_home_rx_bat_update(data_ptr);
        hl_mod_display_mux_take();
        flag->rx_bat_val                  = 0;
        flag->sys_status.rx_charge_status = 0;
        hl_mod_display_mux_release();
        LOG_D("update rx bat %d,%d", data_ptr->sys_status.rx_charge_status, data_ptr->rx_bat_val);
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
