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
#include "page_menu.h"
#include "hl_util_general_type.h"

static hl_display_box_charge_state in_box_state;

static void in_box_page_init(hl_display_box_charge_state in_box,hl_display_screen_s* data_ptr);

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
                hl_mod_menu_goto_home_page();
            } else {
                hl_lvgl_charge_ioctl_t ioctrl;
                ioctrl.charge_cmd = HL_CHARGE_CHANGE_DELETE_PAGE;
                hl_mod_charge_ioctl(&ioctrl);

                in_box_page_init(new_state,data_ptr);
            }
        }
    }
}
static void rx_update(hl_display_screen_s* data_ptr, hl_display_screen_change_s* flag)
{
    hl_lvgl_charge_ioctl_t ioctrl;
    uint8_t data;

    if (flag->rx_bat_val) {
        hl_mod_display_mux_take();
        flag->rx_bat_val            = 0;
        ioctrl.electric.electric_rx = data_ptr->rx_bat_val;
        hl_mod_display_mux_release();
        ioctrl.charge_cmd = HL_CHARGE_CHANGE_RX_ELEC;

        hl_mod_charge_ioctl(&ioctrl);
    }

    if (flag->sys_status.rx_charge_status) {
        hl_mod_display_mux_take();
        flag->sys_status.rx_charge_status = 0;
        data = data_ptr->sys_status.rx_charge_status;

        hl_mod_display_mux_release();

        if (data) {
            ioctrl.charge_cmd = HL_CHARGE_RX_BAT_COLOR_GREEN;
            hl_mod_charge_ioctl(&ioctrl);

            ioctrl.charge_cmd = HL_CHARGE_RX_ICON_DISHIDE;
            hl_mod_charge_ioctl(&ioctrl);
        } else {
            ioctrl.charge_cmd = HL_CHARGE_RX_BAT_COLOR_WHITE;
            hl_mod_charge_ioctl(&ioctrl);

            ioctrl.charge_cmd = HL_CHARGE_RX_ICON_HIDE;
            hl_mod_charge_ioctl(&ioctrl);
        }
        
    }
}
static void tx1_update(hl_display_screen_s* data_ptr, hl_display_screen_change_s* flag)
{
    hl_lvgl_charge_ioctl_t ioctrl;
    uint8_t data;

    if (flag->tx1_bat_val) {
        hl_mod_display_mux_take();
        flag->tx1_bat_val            = 0;
        ioctrl.electric.electric_tx1 = data_ptr->tx1_bat_val;
        hl_mod_display_mux_release();

        ioctrl.charge_cmd = HL_CHARGE_CHANGE_TX1_ELEC;

        hl_mod_charge_ioctl(&ioctrl);
    }

    if (flag->sys_status.tx1_charge_status) {
        hl_mod_display_mux_take();
        flag->sys_status.tx1_charge_status = 0;
        data = data_ptr->sys_status.tx1_charge_status;

        hl_mod_display_mux_release();
        if (data) {
            ioctrl.charge_cmd = HL_CHARGE_TX1_BAT_COLOR_GREEN;
            hl_mod_charge_ioctl(&ioctrl);

            ioctrl.charge_cmd = HL_CHARGE_TX1_ICON_DISHIDE;
            hl_mod_charge_ioctl(&ioctrl);            
        } else {
            ioctrl.charge_cmd = HL_CHARGE_TX1_BAT_COLOR_WHITE;
            hl_mod_charge_ioctl(&ioctrl);

            ioctrl.charge_cmd = HL_CHARGE_TX1_ICON_HIDE;
            hl_mod_charge_ioctl(&ioctrl);   
        }
        
    }
}
static void tx2_update(hl_display_screen_s* data_ptr, hl_display_screen_change_s* flag)
{
    hl_lvgl_charge_ioctl_t ioctrl;
    uint8_t data;

    if (flag->tx2_bat_val) {
        hl_mod_display_mux_take();
        flag->tx2_bat_val            = 0;
        ioctrl.electric.electric_tx2 = data_ptr->tx2_bat_val;
        hl_mod_display_mux_release();

        ioctrl.charge_cmd = HL_CHARGE_CHANGE_TX2_ELEC;

        hl_mod_charge_ioctl(&ioctrl);
    }

    if (flag->sys_status.tx2_charge_status) {
        hl_mod_display_mux_take();
        flag->sys_status.tx2_charge_status = 0;
        data = data_ptr->sys_status.tx2_charge_status;

        hl_mod_display_mux_release();

        if (data) {
            ioctrl.charge_cmd = HL_CHARGE_TX2_BAT_COLOR_GREEN;
            hl_mod_charge_ioctl(&ioctrl);

            ioctrl.charge_cmd = HL_CHARGE_TX2_ICON_DISHIDE;
            hl_mod_charge_ioctl(&ioctrl);
        } else {
            ioctrl.charge_cmd = HL_CHARGE_TX2_BAT_COLOR_WHITE;
            hl_mod_charge_ioctl(&ioctrl);

            ioctrl.charge_cmd = HL_CHARGE_TX2_ICON_HIDE;
            hl_mod_charge_ioctl(&ioctrl);
        }
    }
}
static void case_bat_update(hl_display_screen_s* data_ptr, hl_display_screen_change_s* flag)
{
    hl_lvgl_charge_ioctl_t ioctrl;

    if (flag->case_bat_val) {
        hl_mod_display_mux_take();
        flag->case_bat_val           = 0;
        ioctrl.electric.electric_box = data_ptr->case_bat_val;
        hl_mod_display_mux_release();

        ioctrl.charge_cmd = HL_CHARGE_CHANGE_BOX_ELEC;

        hl_mod_charge_ioctl(&ioctrl);
    }

    if (flag->sys_status.box_charge_status) {
        hl_mod_display_mux_take();
        flag->sys_status.box_charge_status = 0;
        if (data_ptr->sys_status.box_charge_status) {
            ioctrl.charge_cmd = HL_CHARGE_TOP_BAT_COLOR_GREEN;
        } else {
            ioctrl.charge_cmd = HL_CHARGE_TOP_BAT_COLOR_WHITE;
        }
        hl_mod_display_mux_release();

        hl_mod_charge_ioctl(&ioctrl);
    }
}
static void page_bat_update(hl_display_screen_s* data_ptr, hl_display_screen_change_s* flag)
{
    switch (in_box_state) {
        // RX 未入盒子
        case BOX_CHARGE_RX_NOT: 
            break;
        // RX单独在盒子
        case BOX_CHARGE_RX:
            rx_update(data_ptr, flag);
            case_bat_update(data_ptr, flag);
            break;
        // TX1 2 RX都入盒子
        case BOX_CHARGE_RX_TX12:
            rx_update(data_ptr, flag);
            tx1_update(data_ptr, flag);
            tx2_update(data_ptr, flag);
            case_bat_update(data_ptr, flag);
            break;
        // TX1 RX入盒子
        case BOX_CHARGE_RX_TX1:
            rx_update(data_ptr, flag);
            tx1_update(data_ptr, flag);
            case_bat_update(data_ptr, flag);
            break;
        // TX2 RX入盒子
        case BOX_CHARGE_RX_TX2:
            rx_update(data_ptr, flag);
            tx2_update(data_ptr, flag);
            case_bat_update(data_ptr, flag);
            break;
        default:
            break;
    }
}

static void box_bat_create(hl_display_screen_s* data_ptr)
{
    hl_lvgl_charge_ioctl_t ioctrl;

    if (data_ptr->sys_status.box_charge_status) {
        ioctrl.charge_cmd = HL_CHARGE_TOP_BAT_COLOR_GREEN;
        hl_mod_charge_ioctl(&ioctrl);
    } else {
        ioctrl.charge_cmd = HL_CHARGE_TOP_BAT_COLOR_WHITE;
        hl_mod_charge_ioctl(&ioctrl);
    }
}

static void rx_bat_create(hl_display_screen_s* data_ptr)
{
    hl_lvgl_charge_ioctl_t ioctrl;

    if (data_ptr->sys_status.rx_charge_status) {
        ioctrl.charge_cmd = HL_CHARGE_RX_BAT_COLOR_GREEN;
        hl_mod_charge_ioctl(&ioctrl);

        ioctrl.charge_cmd = HL_CHARGE_RX_ICON_DISHIDE;
        hl_mod_charge_ioctl(&ioctrl);
    } else {
        ioctrl.charge_cmd = HL_CHARGE_RX_BAT_COLOR_WHITE;
        hl_mod_charge_ioctl(&ioctrl);

        ioctrl.charge_cmd = HL_CHARGE_RX_ICON_HIDE;
        hl_mod_charge_ioctl(&ioctrl);
    }
}

static void tx1_bat_create(hl_display_screen_s* data_ptr)
{
    hl_lvgl_charge_ioctl_t ioctrl;

    if (data_ptr->sys_status.tx1_charge_status) {
        ioctrl.charge_cmd = HL_CHARGE_TX1_BAT_COLOR_GREEN;
        hl_mod_charge_ioctl(&ioctrl);

        ioctrl.charge_cmd = HL_CHARGE_TX1_ICON_DISHIDE;
        hl_mod_charge_ioctl(&ioctrl);
    } else {
        ioctrl.charge_cmd = HL_CHARGE_TX1_BAT_COLOR_WHITE;
        hl_mod_charge_ioctl(&ioctrl);

        ioctrl.charge_cmd = HL_CHARGE_TX1_ICON_HIDE;
        hl_mod_charge_ioctl(&ioctrl);
    }
}

static void tx2_bat_create(hl_display_screen_s* data_ptr)
{
    hl_lvgl_charge_ioctl_t ioctrl;

    if (data_ptr->sys_status.tx2_charge_status) {
        ioctrl.charge_cmd = HL_CHARGE_TX2_BAT_COLOR_GREEN;
        hl_mod_charge_ioctl(&ioctrl);

        ioctrl.charge_cmd = HL_CHARGE_TX2_ICON_DISHIDE;
        hl_mod_charge_ioctl(&ioctrl);
    } else {
        ioctrl.charge_cmd = HL_CHARGE_TX2_BAT_COLOR_WHITE;
        hl_mod_charge_ioctl(&ioctrl);

        ioctrl.charge_cmd = HL_CHARGE_TX2_ICON_HIDE;
        hl_mod_charge_ioctl(&ioctrl);
    }
}

static void in_box_page_init(hl_display_box_charge_state in_box,hl_display_screen_s* data_ptr)
{
    hl_lvgl_charge_init_t  init;
    

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
    in_box_page_init(data_ptr->in_box_state,data_ptr);
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
    hl_mod_page_screen_lowbritness_init();
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
