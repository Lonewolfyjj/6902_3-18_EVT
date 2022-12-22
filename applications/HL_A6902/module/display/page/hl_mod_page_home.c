/**
 * @file hl_mod_page_home.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-11-30
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
 * <tr><td>2022-11-30     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/

#include "hl_mod_page_common.h"

#if (!HL_IS_TX_DEVICE())
#include "hl_mod_display.h"
#include "lvgl.h"
#include "rtthread.h"
#include "hl_mod_page.h"
#include "page_test.h"
#include "page_top.h"
#include "hl_mod_input.h"
#include "lv_port_indev.h"
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

// static hl_icon_status icon_state = {0};
static hl_top_cmd_t  icon_state[HL_TOP_ICON_CNT] = { 0 };
static hl_rf_state_e main_tx;


static void hl_mod_icon_deal(hl_top_icon_t icon, hl_top_cmd_t deal);
static void hl_mod_icon_deal_init();
static void hl_mod_icon_deal_deinit();
static void hl_mod_main_tx_deal_init(hl_display_screen_s* data_ptr);
static void hl_mod_main_tx_deal_deinit();
static void hl_mod_page_main_init(void);
static void hl_mod_page_top_update(hl_display_screen_change_s* flag, hl_display_screen_s* now);
static void hl_mod_page_home_tx1_update(hl_display_screen_change_s* flag, hl_display_screen_s* now);
static void hl_mod_page_home_tx2_update(hl_display_screen_change_s* flag, hl_display_screen_s* now);
static void hl_mod_page_main_update(hl_display_screen_change_s* flag, hl_display_screen_s* now);
static hl_signal_int_t hl_mod_page_signal_deal(uint8_t data);

// 添加是 1  删除是 0
static void hl_mod_icon_deal(hl_top_icon_t icon, hl_top_cmd_t deal)
{
    hl_lvgl_top_ioctl_t ioctl_top;
    if (deal == HL_TOP_ADD_ICON_CMD) {

        if (icon_state[icon] == HL_TOP_DELETE_ICON_CMD) {
            ioctl_top.top_cmd   = HL_TOP_ADD_ICON_CMD;
            ioctl_top.top_param = icon;
            hl_mod_top_ioctl(&ioctl_top);
            icon_state[icon] = HL_TOP_ADD_ICON_CMD;
            LV_LOG_USER("deal=%d,icon=%d\n", deal, icon);
        }

    } else if (deal == HL_TOP_DELETE_ICON_CMD) {
        if (icon_state[icon] == HL_TOP_ADD_ICON_CMD) {
            ioctl_top.top_cmd   = HL_TOP_DELETE_ICON_CMD;
            ioctl_top.top_param = icon;
            hl_mod_top_ioctl(&ioctl_top);
            icon_state[icon] = HL_TOP_DELETE_ICON_CMD;
            LV_LOG_USER("deal=%d,icon=%d\n", deal, icon);
        }
    }
}

static void hl_mod_icon_deal_init()
{
    for (uint8_t i = 0; i < HL_TOP_ICON_CNT; i++) {
        icon_state[i] = HL_TOP_DELETE_ICON_CMD;
    }
}

static void hl_mod_icon_deal_deinit()
{
    hl_lvgl_top_ioctl_t ioctl_top;

    for (uint8_t i = 0; i < HL_TOP_ICON_CNT; i++) {
        if (icon_state[i] == HL_TOP_ADD_ICON_CMD) {
            ioctl_top.top_cmd   = HL_TOP_DELETE_ICON_CMD;
            ioctl_top.top_param = i;
            hl_mod_top_ioctl(&ioctl_top);
        }
        icon_state[i] = HL_TOP_DELETE_ICON_CMD;
    }
}

static void hl_mod_main_tx_deal_init(hl_display_screen_s* data_ptr)
{
    hl_lvgl_main_init_t data;
    main_tx = data_ptr->rf_net_connect;
    LV_LOG_USER("init%d\n",main_tx);
    switch (main_tx) {
        case HL_RF_LR_CONNECT:
            data.display_tx_device = HL_DISPLAY_DOUBLE;

            data.tx_device_1.electric = data_ptr->tx1_bat_val;
            data.tx_device_1.signal   = hl_mod_page_signal_deal(data_ptr->tx1_signal);
            data.tx_device_1.record   = data_ptr->sys_status.tx1_record_state;
            data.tx_device_1.volume   = data_ptr->tx1_vu;

            data.tx_device_2.electric = data_ptr->tx2_bat_val;
            data.tx_device_2.signal   = hl_mod_page_signal_deal(data_ptr->tx2_signal);
            data.tx_device_2.record   = data_ptr->sys_status.tx2_record_state;
            data.tx_device_2.volume   = data_ptr->tx2_vu;
            hl_mod_main_init(&data);
            break;
        case HL_RF_R_CONNECT:
            data.display_tx_device = HL_DISPLAY_DOUBLE;

            data.tx_device_1.electric = 0;
            data.tx_device_1.signal   = 0;
            data.tx_device_1.record   = 0;
            data.tx_device_1.volume   = 0;

            data.tx_device_2.electric = data_ptr->tx2_bat_val;
            data.tx_device_2.signal   = hl_mod_page_signal_deal(data_ptr->tx2_signal);
            data.tx_device_2.record   = data_ptr->sys_status.tx2_record_state;
            data.tx_device_2.volume   = data_ptr->tx2_vu;
            hl_mod_main_init(&data);
            break;
        case HL_RF_L_CONNECT:
            data.display_tx_device = HL_DISPLAY_DOUBLE;

            data.tx_device_1.electric = data_ptr->tx1_bat_val;
            data.tx_device_1.signal   = hl_mod_page_signal_deal(data_ptr->tx1_signal);
            data.tx_device_1.record   = data_ptr->sys_status.tx1_record_state;
            data.tx_device_1.volume   = data_ptr->tx1_vu;

            data.tx_device_2.electric = 0;
            data.tx_device_2.signal   = 0;
            data.tx_device_2.record   = 0;
            data.tx_device_2.volume   = 0;
            hl_mod_main_init(&data);
            break;
        case HL_RF_UNCONNECT:
            data.display_tx_device = HL_DISPLAY_DOUBLE;

            data.tx_device_1.electric = 0;
            data.tx_device_1.signal   = HL_NO_SIGNAL;
            data.tx_device_1.record   = HL_NO_RECODE;
            data.tx_device_1.volume   = 0;

            data.tx_device_2.electric = 0;
            data.tx_device_2.signal   = HL_NO_SIGNAL;
            data.tx_device_2.record   = HL_NO_RECODE;
            data.tx_device_2.volume   = 0;
            hl_mod_main_init(&data);
            break;
        case HL_RF_PAIRING:
            data.display_tx_device = HL_DISPLAY_DOUBLE;

            data.tx_device_1.electric = 0;
            data.tx_device_1.signal   = HL_NO_SIGNAL;
            data.tx_device_1.record   = HL_NO_RECODE;
            data.tx_device_1.volume   = 0;

            data.tx_device_2.electric = 0;
            data.tx_device_2.signal   = HL_NO_SIGNAL;
            data.tx_device_2.record   = HL_NO_RECODE;
            data.tx_device_2.volume   = 0;
            hl_mod_main_init(&data);
            break;
        default:
            break;
    }
}

static void hl_mod_main_tx12_deal_update(hl_display_screen_s* data_ptr)
{
    hl_lvgl_main_ioctl_t data;
    main_tx = data_ptr->rf_net_connect;
    LV_LOG_USER("tx12up%d\n", main_tx);
    switch (main_tx) {
        case HL_RF_LR_CONNECT:

            data.cmd                  = HL_CHANGE_TX1_ELEC;
            data.tx_device_1.electric = data_ptr->tx1_bat_val;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX1_SIGNAL;
            data.tx_device_1.signal = hl_mod_page_signal_deal(data_ptr->tx1_signal);
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX1_VOL;
            data.tx_device_1.volume = data_ptr->tx1_vu;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX1_REC;
            data.tx_device_1.record = (hl_record_status_t)data_ptr->sys_status.tx1_record_state;
            hl_mod_main_ioctl(&data);
            //TX2
            data.cmd                  = HL_CHANGE_TX2_ELEC;
            data.tx_device_2.electric = data_ptr->tx2_bat_val;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX2_SIGNAL;
            data.tx_device_2.signal = hl_mod_page_signal_deal(data_ptr->tx2_signal);
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX2_VOL;
            data.tx_device_2.volume = data_ptr->tx2_vu;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX2_REC;
            data.tx_device_2.record = (hl_record_status_t)data_ptr->sys_status.tx2_record_state;
            hl_mod_main_ioctl(&data);
            break;
        case HL_RF_R_CONNECT:
            data.cmd                  = HL_CHANGE_TX1_ELEC;
            data.tx_device_1.electric = 0;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX1_SIGNAL;
            data.tx_device_1.signal = 0;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX1_VOL;
            data.tx_device_1.volume = 0;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX1_REC;
            data.tx_device_1.record =0;
            hl_mod_main_ioctl(&data);
            //TX2
            data.cmd                  = HL_CHANGE_TX2_ELEC;
            data.tx_device_2.electric = data_ptr->tx2_bat_val;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX2_SIGNAL;
            data.tx_device_2.signal = hl_mod_page_signal_deal(data_ptr->tx2_signal);
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX2_VOL;
            data.tx_device_2.volume = data_ptr->tx2_vu;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX2_REC;
            data.tx_device_2.record = (hl_record_status_t)data_ptr->sys_status.tx2_record_state;
            hl_mod_main_ioctl(&data);
            break;
        case HL_RF_L_CONNECT:
            data.cmd                  = HL_CHANGE_TX1_ELEC;
            data.tx_device_1.electric = data_ptr->tx1_bat_val;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX1_SIGNAL;
            data.tx_device_1.signal = hl_mod_page_signal_deal(data_ptr->tx1_signal);
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX1_VOL;
            data.tx_device_1.volume = data_ptr->tx1_vu;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX1_REC;
            data.tx_device_1.record = (hl_record_status_t)data_ptr->sys_status.tx1_record_state;
            hl_mod_main_ioctl(&data);
            //TX2
            data.cmd                  = HL_CHANGE_TX2_ELEC;
            data.tx_device_2.electric = 0;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX2_SIGNAL;
            data.tx_device_2.signal = 0;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX2_VOL;
            data.tx_device_2.volume = 0;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX2_REC;
            data.tx_device_2.record = 0;
            hl_mod_main_ioctl(&data);
            break;
        case HL_RF_UNCONNECT:
            data.cmd                  = HL_CHANGE_TX1_ELEC;
            data.tx_device_1.electric = 0;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX1_SIGNAL;
            data.tx_device_1.signal = 0;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX1_VOL;
            data.tx_device_1.volume = 0;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX1_REC;
            data.tx_device_1.record =0;
            hl_mod_main_ioctl(&data);
            //TX2
            data.cmd                  = HL_CHANGE_TX2_ELEC;
            data.tx_device_2.electric = 0;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX2_SIGNAL;
            data.tx_device_2.signal = 0;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX2_VOL;
            data.tx_device_2.volume = 0;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX2_REC;
            data.tx_device_2.record = 0;
            hl_mod_main_ioctl(&data);
            break;
        case HL_RF_PAIRING:
            data.cmd                  = HL_CHANGE_TX1_ELEC;
            data.tx_device_1.electric = 0;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX1_SIGNAL;
            data.tx_device_1.signal = 0;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX1_VOL;
            data.tx_device_1.volume = 0;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX1_REC;
            data.tx_device_1.record =0;
            hl_mod_main_ioctl(&data);
            //TX2
            data.cmd                  = HL_CHANGE_TX2_ELEC;
            data.tx_device_2.electric = 0;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX2_SIGNAL;
            data.tx_device_2.signal = 0;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX2_VOL;
            data.tx_device_2.volume = 0;
            hl_mod_main_ioctl(&data);

            data.cmd                = HL_CHANGE_TX2_REC;
            data.tx_device_2.record = 0;
            hl_mod_main_ioctl(&data);
            break;
        default:
            break;
    }
}
static void hl_mod_main_tx_deal_deinit()
{
    hl_lvgl_main_ioctl_t main_ctl;
    LV_LOG_USER("deinit%d\n",main_tx);

    switch (main_tx) {
        case HL_RF_LR_CONNECT:
            main_ctl.cmd = HL_CHANGE_DELETE_DOUBLE;
            hl_mod_main_ioctl(&main_ctl);
            break;
        case HL_RF_R_CONNECT:
            main_ctl.cmd = HL_CHANGE_DELETE_DOUBLE;
            hl_mod_main_ioctl(&main_ctl);
            break;
        case HL_RF_L_CONNECT:
            main_ctl.cmd = HL_CHANGE_DELETE_DOUBLE;
            hl_mod_main_ioctl(&main_ctl);
            break;
        case HL_RF_UNCONNECT:
            main_ctl.cmd = HL_CHANGE_DELETE_DOUBLE;
            hl_mod_main_ioctl(&main_ctl);
            break;
        case HL_RF_PAIRING:
            main_ctl.cmd = HL_CHANGE_DELETE_DOUBLE;
            hl_mod_main_ioctl(&main_ctl);
            break;
        default:
            break;
    }

    main_ctl.cmd = HL_CHANGE_DELETE_STYLE;
    hl_mod_main_ioctl(&main_ctl);
}

static hl_signal_int_t hl_mod_page_signal_deal(uint8_t data)
{
    uint8_t signal;
    signal = data / 25;
    if (data % 25) {
        signal += 1;
    }
    if (signal >= 4)
        signal = 4;
    return signal;
}
static void hl_mod_main_two_init()
{
    hl_lvgl_main_init_t data;
    main_tx = HL_RF_UNCONNECT;
    LV_LOG_USER("twoinit%d\n", main_tx);

    data.display_tx_device = HL_DISPLAY_DOUBLE;

    data.tx_device_1.electric = 0;
    data.tx_device_1.signal   = HL_NO_SIGNAL;
    data.tx_device_1.record   = HL_NO_RECODE;
    data.tx_device_1.volume   = 0;

    data.tx_device_2.electric = 0;
    data.tx_device_2.signal   = HL_NO_SIGNAL;
    data.tx_device_2.record   = HL_NO_RECODE;
    data.tx_device_2.volume   = 0;
    hl_mod_main_init(&data);
}
// top层初始化
static void hl_mod_page_top_init(void)
{

    hl_lvgl_top_init_t top_init;

    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();


    // 更新当前的RX电量信息
    top_init.electric_top = data_ptr->rx_bat_val;
    hl_mod_top_init(&top_init);

    if (data_ptr->sound_module == STEREO) {

        hl_mod_icon_deal(HL_TOP_ICON_TRACK_MOD, HL_TOP_ADD_ICON_CMD);
    }

    if (data_ptr->sys_status.tx1_noise || data_ptr->sys_status.tx2_noise) {

        hl_mod_icon_deal(HL_TOP_ICON_NOISE, HL_TOP_ADD_ICON_CMD);
    }

    if (data_ptr->sys_status.screen_lock) {

        hl_mod_icon_deal(HL_TOP_ICON_LOCK, HL_TOP_ADD_ICON_CMD);
    }

    if (data_ptr->sys_status.line_out_in) {

        hl_mod_icon_deal(HL_TOP_ICON_LINEOUT, HL_TOP_ADD_ICON_CMD);
    }

    if (data_ptr->sys_status.usb_in) {

        hl_mod_icon_deal(HL_TOP_ICON_TYPEC, HL_TOP_ADD_ICON_CMD);
    }

    if (data_ptr->sys_status.monitor_in) {

        hl_mod_icon_deal(HL_TOP_ICON_HEATSET, HL_TOP_ADD_ICON_CMD);
    }
}
static void hl_mod_page_top_update(hl_display_screen_change_s* flag, hl_display_screen_s* now)
{
    hl_lvgl_top_ioctl_t ioctl_top;

    if (flag->rx_bat_val) {
        ioctl_top.top_cmd      = HL_TOP_BAT_VAL;
        ioctl_top.electric_top = now->rx_bat_val;
        hl_mod_top_ioctl(&ioctl_top);
        flag->rx_bat_val = 0;
    }
    // 降噪一起的

    if (flag->sys_status.tx1_noise || flag->sys_status.tx2_noise) {

        if (now->sys_status.tx1_noise || now->sys_status.tx2_noise) {

            hl_mod_icon_deal(HL_TOP_ICON_NOISE, HL_TOP_ADD_ICON_CMD);
        } else if (!now->sys_status.tx1_noise && !now->sys_status.tx2_noise) {
            hl_mod_icon_deal(HL_TOP_ICON_NOISE, HL_TOP_DELETE_ICON_CMD);
        }
        flag->sys_status.tx1_noise = 0;
        flag->sys_status.tx2_noise = 0;
    }

    // 锁屏
    if (flag->sys_status.screen_lock) {

        hl_mod_icon_deal(HL_TOP_ICON_LOCK, (hl_top_cmd_t)now->sys_status.screen_lock);

        flag->sys_status.screen_lock = 0;
    }

    // line out
    if (flag->sys_status.line_out_in) {

        hl_mod_icon_deal(HL_TOP_ICON_LINEOUT, (hl_top_cmd_t)now->sys_status.line_out_in);
        flag->sys_status.line_out_in = 0;
    }

    // usb
    if (flag->sys_status.usb_in) {

        hl_mod_icon_deal(HL_TOP_ICON_TYPEC, (hl_top_cmd_t)now->sys_status.usb_in);
        flag->sys_status.usb_in = 0;
    }

    if (flag->sys_status.monitor_in) {

        hl_mod_icon_deal(HL_TOP_ICON_HEATSET, (hl_top_cmd_t)now->sys_status.monitor_in);
        flag->sys_status.monitor_in = 0;
    }
}

static void hl_mod_page_home_tx1_update(hl_display_screen_change_s* flag, hl_display_screen_s* now)
{
    hl_lvgl_main_ioctl_t data1;

    if (flag->tx1_bat_val) {
        data1.cmd                  = HL_CHANGE_TX1_ELEC;
        data1.tx_device_1.electric = now->tx1_bat_val;
        hl_mod_main_ioctl(&data1);

        flag->tx1_bat_val = 0;
    }

    if (flag->tx1_signal) {
        data1.cmd                = HL_CHANGE_TX1_SIGNAL;
        data1.tx_device_1.signal = hl_mod_page_signal_deal(now->tx1_signal);
        hl_mod_main_ioctl(&data1);

        flag->tx1_signal = 0;
    }

    //vu值
    if (flag->tx1_vu) {
        data1.cmd                = HL_CHANGE_TX1_VOL;
        data1.tx_device_1.volume = now->tx1_vu;
        hl_mod_main_ioctl(&data1);

        flag->tx1_vu = 0;
    }

    if (flag->sys_status.tx1_record_state) {
        data1.cmd                = HL_CHANGE_TX1_REC;
        data1.tx_device_1.record = (hl_record_status_t)now->sys_status.tx1_record_state;
        hl_mod_main_ioctl(&data1);

        flag->sys_status.tx1_record_state = 0;
    }
}

static void hl_mod_page_home_tx2_update(hl_display_screen_change_s* flag, hl_display_screen_s* now)
{
    hl_lvgl_main_ioctl_t data2;

    if (flag->tx2_bat_val) {
        data2.cmd                  = HL_CHANGE_TX2_ELEC;
        data2.tx_device_2.electric = now->tx2_bat_val;
        hl_mod_main_ioctl(&data2);

        flag->tx2_bat_val = 0;
    }

    if (flag->tx2_signal) {
        data2.cmd                = HL_CHANGE_TX2_SIGNAL;
        data2.tx_device_2.signal = hl_mod_page_signal_deal(now->tx2_signal);
        hl_mod_main_ioctl(&data2);

        flag->tx2_signal = 0;
    }

    //vu值
    if (flag->tx2_vu) {
        data2.cmd                = HL_CHANGE_TX2_VOL;
        data2.tx_device_2.volume = now->tx2_vu;
        hl_mod_main_ioctl(&data2);

        flag->tx2_vu = 0;
    }

    if (flag->sys_status.tx2_record_state) {
        data2.cmd                = HL_CHANGE_TX2_REC;
        data2.tx_device_2.record = (hl_record_status_t)now->sys_status.tx2_record_state;
        hl_mod_main_ioctl(&data2);

        flag->sys_status.tx2_record_state = 0;
    }
}


static void hl_mod_page_main_update(hl_display_screen_change_s* flag, hl_display_screen_s* data_ptr)
{
    if (flag->rf_net_connect) {
        // hl_mod_main_tx_deal_deinit();
        // hl_mod_main_tx_deal_init(data_ptr);
        hl_mod_main_tx12_deal_update(data_ptr);
        LV_LOG_USER("update%d\n",main_tx);
        switch (main_tx) {
            case HL_RF_LR_CONNECT:

                hl_mod_page_home_tx1_update(flag, data_ptr);
                hl_mod_page_home_tx2_update(flag, data_ptr);

                break;
            case HL_RF_R_CONNECT:

                hl_mod_page_home_tx2_update(flag, data_ptr);
                break;
            case HL_RF_L_CONNECT:
                // 一个tx1
                hl_mod_page_home_tx1_update(flag, data_ptr);
                break;
            case HL_RF_UNCONNECT:
                break;
            case HL_RF_PAIRING:
                break;
            default:
                break;
        }
        flag->rf_net_connect = 0;
    } else {
        
        switch (main_tx) {
            case HL_RF_LR_CONNECT:

                hl_mod_page_home_tx1_update(flag, data_ptr);
                hl_mod_page_home_tx2_update(flag, data_ptr);

                break;
            case HL_RF_R_CONNECT:
                // 一个tx2

                hl_mod_page_home_tx2_update(flag, data_ptr);
                break;
            case HL_RF_L_CONNECT:
                // 一个tx1

                hl_mod_page_home_tx1_update(flag, data_ptr);
                break;
            case HL_RF_UNCONNECT:
                break;
            case HL_RF_PAIRING:
                break;
            default:
                break;
        }

    }
}
static void hl_mod_page_home_update(void)
{

    // 这一次的数据
    hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();
    hl_mod_page_main_update(flag, data_ptr);
    hl_mod_page_top_update(flag, data_ptr);
}

static void hl_mod_page_setup(void)
{
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();
    // hl_mod_main_two_init();
    hl_mod_icon_deal_init();
    hl_mod_main_tx_deal_init(data_ptr);
    hl_mod_page_top_init();
    // hl_mod_page_home_update();
}

static void hl_mod_page_exit(void)
{
    hl_mod_main_tx_deal_deinit();
    hl_mod_icon_deal_deinit();
    //删除TOP
    hl_lvgl_top_ioctl_t ctl_data = {
        .top_cmd = HL_TOP_ALL_DEL,
    };
    hl_mod_top_ioctl(&ctl_data);
}

// static void hl_mod_page_event(void* btn, int event)
// {

// }

static void hl_mod_page_run(void)
{
    uint8_t key_event;

    //单机确定键进入菜单
    key_event = hl_mod_get_knob_okkey_val();

    if (key_event == HL_KEY_EVENT_SHORT) {
        PageManager_PagePush(PAGE_MAIN_MENU);
    }

    // 单机触摸键进入快捷界面
    hl_mod_menu_goto_fast_config_scan();

    // 快捷设置
    // hl_mod_menu_goto_quickset_scan();

    // 菜单的页面更新
    hl_mod_page_home_update();
}

// void hl_mod_page_home_init(void)
PAGE_DEC(PAGE_HOME)
{
    bool result;

    result = PageManager_PageRegister(PAGE_HOME, hl_mod_page_setup, hl_mod_page_run, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_HOME init fail\n");
    }
}
#endif
/*
 * EOF
 */