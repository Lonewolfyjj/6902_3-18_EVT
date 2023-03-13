/**
 * @file hl_mod_page_box_in.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-12-27
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
 * <tr><td>2022-12-27     <td>v1.0     <td>liujie     <td>内容
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
#include "hl_util_general_type.h"

typedef struct hl_page_box_bat_state_s
{
    /// @brief 0表示当前正常显示电量 ；1表示当前欠压；2表示当前充电中
    uint8_t rx;
    /// @brief 0表示当前正常显示电量 ；1表示当前欠压；2表示当前充电中
    uint8_t tx1;
    /// @brief 0表示当前正常显示电量 ；1表示当前欠压；2表示当前充电中
    uint8_t tx2;
    uint8_t box;
} hl_page_box_bat_state;

static hl_page_box_bat_state bat_state;

static hl_display_box_charge_state in_box_state;

static void in_box_page_init(hl_display_box_charge_state in_box, hl_display_screen_s* data_ptr);

static void page_update(hl_display_screen_s* data_ptr, hl_display_screen_change_s* flag);
static void page_bat_update(hl_display_screen_s* data_ptr, hl_display_screen_change_s* flag);

static void page_update(hl_display_screen_s* data_ptr, hl_display_screen_change_s* flag)
{
    hl_display_box_charge_state new_state;

    if (flag->in_box_state) {
        hl_mod_display_mux_take();
        flag->in_box_state = 0;
        new_state          = data_ptr->in_box_state;
        hl_mod_display_mux_release();

        if (in_box_state != new_state) {
            in_box_state = new_state;

            // 如果没在盒子，直接退出到主页面
            if (new_state == BOX_CHARGE_RX_NOT) {
                // hl_mod_menu_goto_home_page();
                PageManager_PageStackClear();
                PageManager_PagePush(PAGE_LOGO);
            } else {
                hl_lvgl_charge_ioctl_t ioctrl;
                ioctrl.charge_cmd = HL_CHARGE_CHANGE_DELETE_PAGE;
                hl_mod_charge_ioctl(&ioctrl);

                in_box_page_init(new_state, data_ptr);
            }
        }
    }
}

static void hl_mod_box_rx_bat_update(hl_display_screen_s* data_ptr)
{
    hl_lvgl_charge_ioctl_t ioctrl;
    int16_t                bat_state_last;
    uint8_t                now_rx_data = data_ptr->rx_bat_val;

    bat_state_last = bat_state.rx;
    bat_state.rx   = bat_state_deal(data_ptr->sys_status.rx_charge_status, now_rx_data, 6);

    // 更新当前的RX电量信息
    ioctrl.charge_cmd           = HL_CHARGE_CHANGE_RX_ELEC;
    ioctrl.electric.electric_rx = now_rx_data;
    hl_mod_charge_ioctl(&ioctrl);

    ioctrl.electric.electric_rx = bat_state_last;

    switch (bat_state.rx) {
        case 0:
            ioctrl.charge_cmd = HL_CHARGE_RX_BAT_COLOR_WHITE;
            hl_mod_charge_ioctl(&ioctrl);

            ioctrl.charge_cmd = HL_CHARGE_RX_ICON_HIDE;
            hl_mod_charge_ioctl(&ioctrl);
            break;
        case 2:
            ioctrl.charge_cmd = HL_CHARGE_RX_BAT_COLOR_GREEN;
            hl_mod_charge_ioctl(&ioctrl);

            ioctrl.charge_cmd = HL_CHARGE_RX_ICON_DISHIDE;
            hl_mod_charge_ioctl(&ioctrl);
            break;
        default:
            ioctrl.charge_cmd = HL_CHARGE_RX_BAT_COLOR_WHITE;
            hl_mod_charge_ioctl(&ioctrl);

            ioctrl.charge_cmd = HL_CHARGE_RX_ICON_HIDE;
            hl_mod_charge_ioctl(&ioctrl);
            break;
    }
    LOG_D("box rx bat(%d)(%d)", data_ptr->sys_status.rx_charge_status, now_rx_data);
}

static void hl_mod_box_tx1_bat_update(hl_display_screen_s* data_ptr)
{
    hl_lvgl_charge_ioctl_t ioctrl;
    int16_t                bat_state_last;
    uint8_t                now_tx1_data = data_ptr->tx1_bat_val;

    bat_state_last = bat_state.tx1;
    bat_state.tx1  = bat_state_deal(data_ptr->sys_status.tx1_charge_status, now_tx1_data, 6);

    // 更新当前的RX电量信息
    ioctrl.charge_cmd            = HL_CHARGE_CHANGE_TX1_ELEC;
    ioctrl.electric.electric_tx1 = now_tx1_data;
    hl_mod_charge_ioctl(&ioctrl);

    ioctrl.electric.electric_tx1 = bat_state_last;

    switch (bat_state.tx1) {
        case 0:
            ioctrl.charge_cmd = HL_CHARGE_TX1_BAT_COLOR_WHITE;
            hl_mod_charge_ioctl(&ioctrl);

            ioctrl.charge_cmd = HL_CHARGE_TX1_ICON_HIDE;
            hl_mod_charge_ioctl(&ioctrl);
            break;
        case 2:
            ioctrl.charge_cmd = HL_CHARGE_TX1_BAT_COLOR_GREEN;
            hl_mod_charge_ioctl(&ioctrl);

            ioctrl.charge_cmd = HL_CHARGE_TX1_ICON_DISHIDE;
            hl_mod_charge_ioctl(&ioctrl);
            break;
        default:
            ioctrl.charge_cmd = HL_CHARGE_TX1_BAT_COLOR_WHITE;
            hl_mod_charge_ioctl(&ioctrl);

            ioctrl.charge_cmd = HL_CHARGE_TX1_ICON_HIDE;
            hl_mod_charge_ioctl(&ioctrl);
            break;
    }
    LOG_D("box tx1 bat(%d)(%d)", data_ptr->sys_status.tx1_charge_status, now_tx1_data);
}

static void hl_mod_box_tx2_bat_update(hl_display_screen_s* data_ptr)
{
    hl_lvgl_charge_ioctl_t ioctrl;
    int16_t                bat_state_last;
    uint8_t                now_tx2_data = data_ptr->tx2_bat_val;

    bat_state_last = bat_state.tx2;
    bat_state.tx2  = bat_state_deal(data_ptr->sys_status.tx2_charge_status, now_tx2_data, 6);

    // 更新当前的RX电量信息
    ioctrl.charge_cmd            = HL_CHARGE_CHANGE_TX2_ELEC;
    ioctrl.electric.electric_tx2 = now_tx2_data;
    hl_mod_charge_ioctl(&ioctrl);

    ioctrl.electric.electric_tx2 = bat_state_last;

    switch (bat_state.tx2) {
        case 0:
            ioctrl.charge_cmd = HL_CHARGE_TX2_BAT_COLOR_WHITE;
            hl_mod_charge_ioctl(&ioctrl);

            ioctrl.charge_cmd = HL_CHARGE_TX2_ICON_HIDE;
            hl_mod_charge_ioctl(&ioctrl);
            break;
        case 2:
            ioctrl.charge_cmd = HL_CHARGE_TX2_BAT_COLOR_GREEN;
            hl_mod_charge_ioctl(&ioctrl);

            ioctrl.charge_cmd = HL_CHARGE_TX2_ICON_DISHIDE;
            hl_mod_charge_ioctl(&ioctrl);
            break;
        default:
            ioctrl.charge_cmd = HL_CHARGE_TX2_BAT_COLOR_WHITE;
            hl_mod_charge_ioctl(&ioctrl);

            ioctrl.charge_cmd = HL_CHARGE_TX2_ICON_HIDE;
            hl_mod_charge_ioctl(&ioctrl);
            break;
    }
    LOG_D("box tx2 bat(%d)(%d)", data_ptr->sys_status.tx2_charge_status, now_tx2_data);
}

static void hl_mod_box_case_bat_update(hl_display_screen_s* data_ptr)
{
    hl_lvgl_charge_ioctl_t ioctrl;
    int16_t                bat_state_last;
    uint8_t                now_data = data_ptr->case_bat_val;

    bat_state_last = bat_state.box;
    bat_state.box  = bat_state_deal(data_ptr->sys_status.box_charge_status, now_data, 6);

    // 更新当前的RX电量信息
    ioctrl.charge_cmd = HL_CHARGE_CHANGE_BOX_ELEC;

    ioctrl.electric.electric_box = now_data;
    hl_mod_charge_ioctl(&ioctrl);

    ioctrl.electric.electric_box = bat_state_last;

    switch (bat_state.box) {
        case 0:
            ioctrl.charge_cmd = HL_CHARGE_TOP_BAT_COLOR_WHITE;
            hl_mod_charge_ioctl(&ioctrl);
            break;
        case 2:
            ioctrl.charge_cmd = HL_CHARGE_TOP_BAT_COLOR_GREEN;
            hl_mod_charge_ioctl(&ioctrl);
            break;
        default:
            ioctrl.charge_cmd = HL_CHARGE_TOP_BAT_COLOR_WHITE;
            hl_mod_charge_ioctl(&ioctrl);
            break;
    }
    LOG_D("box case bat(%d)(%d)", data_ptr->sys_status.box_charge_status, now_data);
}

#define CASE_VIEW 0x01
#define RX_VIEW 0x02
#define TX1_VIEW 0x04
#define TX2_VIEW 0x08

static void page_bat_update(hl_display_screen_s* data_ptr, hl_display_screen_change_s* flag)
{
    // bit0:盒子 0x01  bit1：RX  0x02 bit2 TX1 0x04 bit3 TX2 0x08
    uint8_t rx_state = 0;

    switch (in_box_state) {
        // RX 未入盒子
        case BOX_CHARGE_RX_NOT:
            rx_state = 0;
            break;
        // RX单独在盒子
        case BOX_CHARGE_RX:
            rx_state = CASE_VIEW | RX_VIEW;
            break;
        // TX1 2 RX都入盒子
        case BOX_CHARGE_RX_TX12:
            rx_state = TX1_VIEW | TX2_VIEW | RX_VIEW | CASE_VIEW;
            break;
        // TX1 RX入盒子
        case BOX_CHARGE_RX_TX1:
            rx_state = TX1_VIEW | RX_VIEW | CASE_VIEW;
            break;
        // TX2 RX入盒子
        case BOX_CHARGE_RX_TX2:
            rx_state = TX2_VIEW | RX_VIEW | CASE_VIEW;
            break;
        default:
            rx_state = 0;
            break;
    }

    if (rx_state & RX_VIEW) {
        if (flag->rx_bat_val || flag->sys_status.rx_charge_status) {
            hl_mod_box_rx_bat_update(data_ptr);
            hl_mod_display_mux_take();
            flag->rx_bat_val                  = 0;
            flag->sys_status.rx_charge_status = 0;
            hl_mod_display_mux_release();
        }
    }

    if (rx_state & CASE_VIEW) {

        if (flag->case_bat_val || flag->sys_status.box_charge_status) {
            hl_mod_box_case_bat_update(data_ptr);
            hl_mod_display_mux_take();
            flag->case_bat_val                 = 0;
            flag->sys_status.box_charge_status = 0;
            hl_mod_display_mux_release();
        }
    }

    if (rx_state & TX1_VIEW) {

        if (flag->tx1_bat_val || flag->sys_status.tx1_charge_status) {
            hl_mod_box_tx1_bat_update(data_ptr);
            hl_mod_display_mux_take();
            flag->tx1_bat_val                  = 0;
            flag->sys_status.tx1_charge_status = 0;
            hl_mod_display_mux_release();
        }
    }

    if (rx_state & TX2_VIEW) {
        if (flag->tx2_bat_val || flag->sys_status.tx2_charge_status) {
            hl_mod_box_tx2_bat_update(data_ptr);
            hl_mod_display_mux_take();
            flag->tx2_bat_val                  = 0;
            flag->sys_status.tx2_charge_status = 0;
            hl_mod_display_mux_release();
        }
    }
}

static void box_bat_create(hl_display_screen_s* data_ptr)
{
    bat_state.box = 0;

    hl_mod_box_case_bat_update(data_ptr);
}

static void rx_bat_create(hl_display_screen_s* data_ptr)
{
    bat_state.rx = 2;

    hl_mod_box_rx_bat_update(data_ptr);
}

static void tx1_bat_create(hl_display_screen_s* data_ptr)
{
    bat_state.tx1 = 2;
    hl_mod_box_tx1_bat_update(data_ptr);
}

static void tx2_bat_create(hl_display_screen_s* data_ptr)
{
    bat_state.tx2 = 2;

    hl_mod_box_tx2_bat_update(data_ptr);
}

static void in_box_page_init(hl_display_box_charge_state in_box, hl_display_screen_s* data_ptr)
{
    hl_lvgl_charge_init_t init;

    in_box_state = in_box;

    switch (in_box_state) {
        // RX 未入盒子
        case BOX_CHARGE_RX_NOT:
            return;
            break;
        // RX单独在盒子
        case BOX_CHARGE_RX:
            init.display_device        = HL_CHARGE_DISPLAY_RX;
            init.electric.electric_box = data_ptr->case_bat_val;
            init.electric.electric_tx1 = 0;
            init.electric.electric_tx2 = 0;
            init.electric.electric_rx  = data_ptr->rx_bat_val;
            hl_mod_charge_init(&init);
            box_bat_create(data_ptr);
            rx_bat_create(data_ptr);
            break;
        // TX1 2 RX都入盒子
        case BOX_CHARGE_RX_TX12:
            init.display_device        = HL_CHARGE_DISPLAY_TX1 | HL_CHARGE_DISPLAY_RX | HL_CHARGE_DISPLAY_TX2;
            init.electric.electric_box = data_ptr->case_bat_val;
            init.electric.electric_tx1 = data_ptr->tx1_bat_val;
            init.electric.electric_tx2 = data_ptr->tx2_bat_val;
            init.electric.electric_rx  = data_ptr->rx_bat_val;
            hl_mod_charge_init(&init);
            box_bat_create(data_ptr);
            rx_bat_create(data_ptr);
            tx1_bat_create(data_ptr);
            tx2_bat_create(data_ptr);
            break;
        // TX1 RX入盒子
        case BOX_CHARGE_RX_TX1:
            init.display_device        = HL_CHARGE_DISPLAY_TX1 | HL_CHARGE_DISPLAY_RX;
            init.electric.electric_box = data_ptr->case_bat_val;
            init.electric.electric_tx1 = data_ptr->tx1_bat_val;
            init.electric.electric_tx2 = 0;
            init.electric.electric_rx  = data_ptr->rx_bat_val;
            hl_mod_charge_init(&init);
            box_bat_create(data_ptr);
            rx_bat_create(data_ptr);
            tx1_bat_create(data_ptr);
            break;
        // TX2 RX入盒子
        case BOX_CHARGE_RX_TX2:
            init.display_device        = HL_CHARGE_DISPLAY_RX | HL_CHARGE_DISPLAY_TX2;
            init.electric.electric_box = data_ptr->case_bat_val;
            init.electric.electric_tx1 = 0;
            init.electric.electric_tx2 = data_ptr->tx2_bat_val;
            init.electric.electric_rx  = data_ptr->rx_bat_val;
            hl_mod_charge_init(&init);
            box_bat_create(data_ptr);
            rx_bat_create(data_ptr);
            tx2_bat_create(data_ptr);
            break;
        default:
            break;
    }
}

static void hl_mod_page_setup(void)
{
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();

    in_box_page_init(data_ptr->in_box_state, data_ptr);
    hl_mod_page_screen_lowbritness_deinit();
    hl_mod_page_inbox_screenoff_init();
}

static void hl_mod_page_exit(void)
{
    // if (in_box_state == BOX_CHARGE_RX_NOT) {
    //     return;
    // }
    hl_lvgl_charge_ioctl_t ioctrl;
    ioctrl.charge_cmd = HL_CHARGE_CHANGE_DELETE_PAGE;
    hl_mod_charge_ioctl(&ioctrl);
    hl_mod_page_inbox_screenoff_close();
    // hl_mod_page_screen_lowbritness_init();
}

static void hl_mod_page_loop(void)
{
    hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();

    page_update(data_ptr, flag);
    page_bat_update(data_ptr, flag);
    hl_mod_page_inbox_screenoff_scan();
}

PAGE_DEC(PAGE_BOX_IN)
{
    bool result;

    result = PageManager_PageRegister(PAGE_BOX_IN, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_TX_LED_BRIGHT init fail\n");
    }
}

#endif
/*
 * EOF
 */
