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
#include "page_lock.h"
#include "hl_util_general_type.h"
#include "lv_port_indev.h"
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#define LOWBAT_THRESHOLD 6
// static hl_icon_status icon_state = {0};
// static hl_top_cmd_t              icon_state[HL_TOP_ICON_CNT] = { 0 };
static hl_rf_state_e             main_tx;
static hl_display_sound_module_e sound_module = MONO;
typedef struct hl_page_home_bat_state_s
{
    /// @brief 0表示当前正常显示电量 ；1表示当前欠压；2表示当前充电中
    uint8_t rx;
    /// @brief 0表示当前正常显示电量 ；1表示当前欠压；2表示当前充电中
    uint8_t tx1;
    /// @brief 0表示当前正常显示电量 ；1表示当前欠压；2表示当前充电中
    uint8_t tx2;
} hl_page_home_bat_state;

static hl_page_home_bat_state bat_state;

static void            hl_mod_icon_deal(hl_top_icon_t icon, hl_top_cmd_t deal);
// static void            hl_mod_icon_deal_init();
static void            hl_mod_icon_deal_deinit();
static void            hl_mod_main_tx_deal_init(hl_display_screen_s* data_ptr);
static void            hl_mod_main_tx_deal_deinit();
static void            hl_mod_page_top_update(hl_display_screen_change_s* flag, hl_display_screen_s* now);
static void            hl_mod_page_home_tx1_update(hl_display_screen_change_s* flag, hl_display_screen_s* now);
static void            hl_mod_page_home_tx2_update(hl_display_screen_change_s* flag, hl_display_screen_s* now);
static void            hl_mod_page_main_update(hl_display_screen_change_s* flag, hl_display_screen_s* now);
static hl_signal_int_t hl_mod_page_signal_deal(uint8_t data);
static void            hl_mod_page_lock_event(hl_lvgl_lock_sta_t state);
// static uint8_t         bat_state_deal(uint8_t charge_state, uint8_t bat_val, uint8_t thresho);
static void            hl_mod_home_rx_bat_update(hl_display_screen_s* data_ptr);
static void            hl_mod_home_tx1_bat_update(hl_display_screen_s* data_ptr);
static void            hl_mod_home_tx2_bat_update(hl_display_screen_s* data_ptr);

static int16_t hl_mod_lineout_volume_get(hl_display_screen_s* data_ptr, uint8_t channl)
{
    int16_t volume_back = 0;

    switch (sound_module) {
        case STEREO:
            //左声道
            if (!channl) {
                volume_back = data_ptr->tx1_line_out_volume;
            } else {
                //右声道
                volume_back = data_ptr->tx2_line_out_volume;
            }
            break;
        case MONO:
            volume_back = data_ptr->mono_line_out_volume;
            break;
        case SAFE_TRACK:
            volume_back = data_ptr->track_line_out_volume;
            break;
        default:
            break;
    }
    return volume_back;
}

// 小状态图标添加是：1；带中间图标的添加是：2；删除图标是 0
static void hl_mod_icon_deal(hl_top_icon_t icon, hl_top_cmd_t deal)
{
    hl_lvgl_top_ioctl_t ioctl_top;

    if (icon >= HL_TOP_ICON_LOCK && icon < HL_TOP_ICON_CNT) {
        ioctl_top.top_cmd   = HL_TOP_CENTER_DEL;
        ioctl_top.top_param = icon;
        hl_mod_top_ioctl(&ioctl_top);
    } else if (icon >= HL_TOP_ICON_STEREO_MOD && icon <= HL_TOP_ICON_APPLE) {

    } else {
    }

    switch (deal) {
        case HL_TOP_DELETE_ICON_CMD:

            ioctl_top.top_cmd   = HL_TOP_DELETE_ICON_CMD;
            ioctl_top.top_param = icon;
            hl_mod_top_ioctl(&ioctl_top);
            break;
        case HL_TOP_ADD_ICON_CMD:

            ioctl_top.top_cmd   = HL_TOP_ADD_ICON_CMD;
            ioctl_top.top_param = icon;
            hl_mod_top_ioctl(&ioctl_top);
            break;
        case HL_TOP_INPUT_CMD:

            if (icon == HL_TOP_ICON_UNLOCK) {

                ioctl_top.top_cmd = HL_TOP_UNKLOCK_TP;

                hl_mod_top_ioctl(&ioctl_top);
            }
            ioctl_top.top_cmd   = HL_TOP_INPUT_CMD;
            ioctl_top.top_param = icon;
            hl_mod_top_ioctl(&ioctl_top);
            break;
        default:
            break;
    };

    LOG_D("deal=%d,icon=%d\n", deal, icon);
}

// // 添加是 1  删除是 0  带动画的 只有插入相关的图标和锁屏有动画
// static void hl_mod_icon_deal_anim(hl_top_icon_t icon, hl_top_cmd_t deal)
// {
//     hl_lvgl_top_ioctl_t  ioctl_top;
//     hl_lvgl_lock_ioctl_t ioctl_lock;
//     hl_lock_icon_t       lock_icon;

//     if (icon > HL_TOP_ICON_HEATSET || icon <= HL_TOP_ICON_LOCK) {
//         return;
//     }

//     switch (icon) {
//         case HL_TOP_ICON_LOCK:
//             lock_icon = HL_LOCK_ICON_LOCK;
//             break;
//         case HL_TOP_ICON_LINEOUT:
//             lock_icon = HL_LOCK_ICON_LINEOUT;
//             break;
//         case HL_TOP_ICON_TYPEC:
//             lock_icon = HL_LOCK_ICON_TYPEC;
//             break;
//         case HL_TOP_ICON_HEATSET:
//             lock_icon = HL_LOCK_ICON_HEATSET;
//             break;
//         default:
//             break;
//     }

//     if (deal == HL_TOP_ADD_ICON_CMD) {

//         if (icon_state[icon] == HL_TOP_DELETE_ICON_CMD) {

//             ioctl_top.top_cmd   = HL_TOP_ADD_ICON_CMD;
//             ioctl_top.top_param = icon;
//             hl_mod_top_ioctl(&ioctl_top);

//             ioctl_top.top_cmd   = HL_TOP_GET_ICON_OBJ_CMD;
//             ioctl_top.top_param = icon;
//             hl_mod_top_ioctl(&ioctl_top);

//             ioctl_lock.cmd      = HL_LOCK_ICON_ANIM;
//             ioctl_lock.icon_typ = lock_icon;
//             ioctl_lock.icon_obj = ioctl_top.icon_obj;
//             hl_mod_lock_ioctl(&ioctl_lock);

//             icon_state[icon] = HL_TOP_ADD_ICON_CMD;
//             LV_LOG_USER("deal=%d,icon=%d\n", deal, icon);
//         }
//     } else if (deal == HL_TOP_DELETE_ICON_CMD) {
//         if (icon_state[icon] == HL_TOP_ADD_ICON_CMD) {
//             ioctl_top.top_cmd   = HL_TOP_DELETE_ICON_CMD;
//             ioctl_top.top_param = icon;
//             hl_mod_top_ioctl(&ioctl_top);
//             icon_state[icon] = HL_TOP_DELETE_ICON_CMD;
//             LV_LOG_USER("deal=%d,icon=%d\n", deal, icon);
//         }
//     }
// }

// static void hl_mod_icon_deal_init()
// {
//     for (uint8_t i = 0; i < HL_TOP_ICON_CNT; i++) {
//         icon_state[i] = HL_TOP_DELETE_ICON_CMD;
//     }
// }

static void hl_mod_icon_deal_deinit()
{
    hl_lvgl_top_ioctl_t ioctl_top;

    for (uint8_t i = 0; i < HL_TOP_ICON_CNT; i++) {
        // if (icon_state[i] == HL_TOP_ADD_ICON_CMD) {

        ioctl_top.top_cmd   = HL_TOP_DELETE_ICON_CMD;
        ioctl_top.top_param = i;
        hl_mod_top_ioctl(&ioctl_top);
        // }
        // icon_state[i] = HL_TOP_DELETE_ICON_CMD;
    }
}

static void hl_mod_home_rx_bat_update(hl_display_screen_s* data_ptr)
{
    hl_lvgl_top_ioctl_t ioctrl;
    int16_t  bat_state_last;
    
    bat_state_last = bat_state.rx;
    bat_state.rx = bat_state_deal(data_ptr->sys_status.rx_charge_status, data_ptr->rx_bat_val, LOWBAT_THRESHOLD);

    // 更新当前的RX电量信息
    ioctrl.top_cmd      = HL_TOP_BAT_VAL;
    ioctrl.electric_top = data_ptr->rx_bat_val;
    hl_mod_top_ioctl(&ioctrl);

    switch (bat_state.rx) {
        case 0:
            ioctrl.electric_top = bat_state_last;
            ioctrl.top_cmd = HL_TOP_BAT_COLOR_WHITE;
            hl_mod_top_ioctl(&ioctrl);

            ioctrl.top_cmd = HL_TOP_BAT_CHARGER_HIDE;
            hl_mod_top_ioctl(&ioctrl);
            break;
        case 1:
            ioctrl.electric_top = bat_state_last;
            ioctrl.top_cmd = HL_TOP_BAT_COLOR_RED;
            hl_mod_top_ioctl(&ioctrl);

            ioctrl.top_cmd = HL_TOP_BAT_CHARGER_HIDE;
            hl_mod_top_ioctl(&ioctrl);
            break;
        case 2:
            ioctrl.electric_top = bat_state_last;
            ioctrl.top_cmd = HL_TOP_BAT_COLOR_GREEN;
            hl_mod_top_ioctl(&ioctrl);

            ioctrl.top_cmd = HL_TOP_BAT_CHARGER_NOT_HIDE;
            hl_mod_top_ioctl(&ioctrl);
            break;
        default:
            break;
    }
}

static void hl_mod_home_tx1_bat_update(hl_display_screen_s* data_ptr)
{
    hl_lvgl_main_ioctl_t ioctrl;
    int16_t  bat_state_last;
    
    bat_state_last = bat_state.tx1;
    bat_state.tx1 = bat_state_deal(data_ptr->sys_status.tx1_charge_status, data_ptr->tx1_bat_val, LOWBAT_THRESHOLD);

    ioctrl.cmd                  = HL_CHANGE_TX1_ELEC;
    ioctrl.tx_device_1.electric = data_ptr->tx1_bat_val;
    hl_mod_main_ioctl(&ioctrl);

    switch (bat_state.tx1) {
        case 0:
            // 删除上一个电量的颜色风格
            ioctrl.tx_device_1.electric =  (int16_t)bat_state_last ;
            ioctrl.cmd = HL_CHANGE_TX1_BAR_WHITE;
            hl_mod_main_ioctl(&ioctrl);

            ioctrl.cmd = HL_CHANGE_TX1_ICON_HIDE;
            hl_mod_main_ioctl(&ioctrl);
            break;
        case 1:
            ioctrl.tx_device_1.electric =  (int16_t)bat_state_last ;
            ioctrl.cmd = HL_CHANGE_TX1_BAR_RED;
            hl_mod_main_ioctl(&ioctrl);

            ioctrl.cmd = HL_CHANGE_TX1_ICON_HIDE;
            hl_mod_main_ioctl(&ioctrl);
            break;
        case 2:
            ioctrl.tx_device_1.electric =  (int16_t)bat_state_last ;
            ioctrl.cmd = HL_CHANGE_TX1_BAR_GREEN;
            hl_mod_main_ioctl(&ioctrl);

            ioctrl.cmd = HL_CHANGE_TX1_ICON_DISHIDE;
            hl_mod_main_ioctl(&ioctrl);
            break;
        default:
            break;
    }
}

static void hl_mod_home_tx1mute_update(hl_display_screen_s* now)
{    
    hl_lvgl_main_ioctl_t data1;

    if (now->sys_status.tx1_mute_switch) {
        data1.cmd = HL_CHANGE_TX1_MUTE;
    } else {
        data1.cmd = HL_CHANGE_TX1_MUTE_DEL;
    }
    hl_mod_main_ioctl(&data1);
    LOG_D("mute1=%d", now->sys_status.tx1_mute_switch);
}

static void hl_mod_home_tx2mute_update(hl_display_screen_s* now)
{
    hl_lvgl_main_ioctl_t data2;

    if (now->sys_status.tx2_mute_switch) {
        data2.cmd = HL_CHANGE_TX2_MUTE;
    } else {
        data2.cmd = HL_CHANGE_TX2_MUTE_DEL;
    }
    hl_mod_main_ioctl(&data2);
    LOG_D("mute1=%d", now->sys_status.tx2_mute_switch);
}

static void hl_mod_home_tx2_bat_update(hl_display_screen_s* data_ptr)
{
    hl_lvgl_main_ioctl_t ioctrl;
    int16_t  bat_state_last;
    
    bat_state_last = bat_state.tx2;
    bat_state.tx2 = bat_state_deal(data_ptr->sys_status.tx2_charge_status, data_ptr->tx2_bat_val, LOWBAT_THRESHOLD);

    ioctrl.cmd                  = HL_CHANGE_TX2_ELEC;
    ioctrl.tx_device_2.electric = data_ptr->tx2_bat_val;
    hl_mod_main_ioctl(&ioctrl);

    switch (bat_state.tx2) {
        case 0:
            // 删除上一个电量的颜色风格
            ioctrl.tx_device_2.electric =  (int16_t)bat_state_last ;
            ioctrl.cmd = HL_CHANGE_TX2_BAR_WHITE;
            hl_mod_main_ioctl(&ioctrl);

            ioctrl.cmd = HL_CHANGE_TX2_ICON_HIDE;
            hl_mod_main_ioctl(&ioctrl);
            break;
        case 1:
            // 删除上一个电量的颜色风格
            ioctrl.tx_device_2.electric =  (int16_t)bat_state_last ;
            ioctrl.cmd = HL_CHANGE_TX2_BAR_RED;
            hl_mod_main_ioctl(&ioctrl);

            ioctrl.cmd = HL_CHANGE_TX2_ICON_HIDE;
            hl_mod_main_ioctl(&ioctrl);
            break;
        case 2:
            // 删除上一个电量的颜色风格
            ioctrl.tx_device_2.electric =  (int16_t)bat_state_last ;
            ioctrl.cmd = HL_CHANGE_TX2_BAR_GREEN;
            hl_mod_main_ioctl(&ioctrl);

            ioctrl.cmd = HL_CHANGE_TX2_ICON_DISHIDE;
            hl_mod_main_ioctl(&ioctrl);
            break;
        default:
            break;
    }
}

static void hl_mod_main_tx_deal_init(hl_display_screen_s* data_ptr)
{
    hl_lvgl_main_init_t data;

    LV_LOG_USER("init%d\n", main_tx);
    switch (main_tx) {
        case HL_RF_LR_CONNECT:
            data.display_tx_device = HL_DISPLAY_DOUBLE;

            data.tx_device_1.electric       = data_ptr->tx1_bat_val;
            data.tx_device_1.line_out_value = hl_mod_lineout_volume_get(data_ptr, 0);
            data.tx_device_1.tx_gain        = data_ptr->tx1_gain_volume;
            data.tx_device_1.signal         = hl_mod_page_signal_deal(data_ptr->tx1_signal);
            data.tx_device_1.record         = data_ptr->sys_status.tx1_record_state;
            data.tx_device_1.volume         = data_ptr->tx1_vu;

            data.tx_device_2.electric       = data_ptr->tx2_bat_val;
            data.tx_device_2.line_out_value = hl_mod_lineout_volume_get(data_ptr, 1);
            data.tx_device_2.tx_gain        = data_ptr->tx2_gain_volume;
            data.tx_device_2.signal         = hl_mod_page_signal_deal(data_ptr->tx2_signal);
            data.tx_device_2.record         = data_ptr->sys_status.tx2_record_state;
            data.tx_device_2.volume         = data_ptr->tx2_vu;
            hl_mod_main_init(&data);

            hl_mod_home_tx1_bat_update(data_ptr);
            hl_mod_home_tx2_bat_update(data_ptr);
            hl_mod_home_tx1mute_update(data_ptr);
            hl_mod_home_tx2mute_update(data_ptr);
            break;
        case HL_RF_R_CONNECT:
            data.display_tx_device = HL_DISPLAY_TX2;

            data.tx_device_2.electric       = data_ptr->tx2_bat_val;
            data.tx_device_2.line_out_value = hl_mod_lineout_volume_get(data_ptr, 1);
            data.tx_device_2.tx_gain        = data_ptr->tx2_gain_volume;
            data.tx_device_2.signal         = hl_mod_page_signal_deal(data_ptr->tx2_signal);
            data.tx_device_2.record         = data_ptr->sys_status.tx2_record_state;
            data.tx_device_2.volume         = data_ptr->tx2_vu;
            hl_mod_main_init(&data);

            hl_mod_home_tx2_bat_update(data_ptr);
            hl_mod_home_tx2mute_update(data_ptr);
            break;
        case HL_RF_L_CONNECT:
            data.display_tx_device = HL_DISPLAY_TX1;

            data.tx_device_1.electric       = data_ptr->tx1_bat_val;
            data.tx_device_1.line_out_value = hl_mod_lineout_volume_get(data_ptr, 0);
            data.tx_device_1.tx_gain        = data_ptr->tx1_gain_volume;
            data.tx_device_1.signal         = hl_mod_page_signal_deal(data_ptr->tx1_signal);
            data.tx_device_1.record         = data_ptr->sys_status.tx1_record_state;
            data.tx_device_1.volume         = data_ptr->tx1_vu;
            hl_mod_main_init(&data);
            hl_mod_home_tx1_bat_update(data_ptr);
            hl_mod_home_tx1mute_update(data_ptr);

            break;

        case HL_RF_UNCONNECT:
            data.display_tx_device = HL_DISPLAY_DOUBLE;

            data.tx_device_1.electric       = 0;
            data.tx_device_1.line_out_value = 0;
            data.tx_device_1.tx_gain        = 0;
            data.tx_device_1.signal         = HL_NO_SIGNAL;
            data.tx_device_1.record         = HL_NO_RECODE;
            data.tx_device_1.volume         = 0;

            data.tx_device_2.electric       = 0;
            data.tx_device_2.line_out_value = 0;
            data.tx_device_2.tx_gain        = 0;
            data.tx_device_2.signal         = HL_NO_SIGNAL;
            data.tx_device_2.record         = HL_NO_RECODE;
            data.tx_device_2.volume         = 0;
            hl_mod_main_init(&data);
            break;
        case HL_RF_PAIRING:
            data.display_tx_device = HL_DISPLAY_DOUBLE;

            data.tx_device_1.electric       = 0;
            data.tx_device_1.line_out_value = 0;
            data.tx_device_1.tx_gain        = 0;
            data.tx_device_1.signal         = HL_NO_SIGNAL;
            data.tx_device_1.record         = HL_NO_RECODE;
            data.tx_device_1.volume         = 0;

            data.tx_device_2.electric       = 0;
            data.tx_device_2.line_out_value = 0;
            data.tx_device_2.tx_gain        = 0;
            data.tx_device_2.signal         = HL_NO_SIGNAL;
            data.tx_device_2.record         = HL_NO_RECODE;
            data.tx_device_2.volume         = 0;
            hl_mod_main_init(&data);
            break;
        default:
            break;
    }
}

// static void hl_mod_main_tx12_deal_update(hl_display_screen_s* data_ptr)
// {
//     hl_lvgl_main_ioctl_t data;

//     LV_LOG_USER("tx12up%d\n", main_tx);
//     switch (main_tx) {
//         case HL_RF_LR_CONNECT:

//             data.cmd                  = HL_CHANGE_TX1_ELEC;
//             data.tx_device_1.electric = data_ptr->tx1_bat_val;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX1_SIGNAL;
//             data.tx_device_1.signal = hl_mod_page_signal_deal(data_ptr->tx1_signal);
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX1_VOL;
//             data.tx_device_1.volume = data_ptr->tx1_vu;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX1_REC;
//             data.tx_device_1.record = (hl_record_status_t)data_ptr->sys_status.tx1_record_state;
//             hl_mod_main_ioctl(&data);
//             //TX2
//             data.cmd                  = HL_CHANGE_TX2_ELEC;
//             data.tx_device_2.electric = data_ptr->tx2_bat_val;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX2_SIGNAL;
//             data.tx_device_2.signal = hl_mod_page_signal_deal(data_ptr->tx2_signal);
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX2_VOL;
//             data.tx_device_2.volume = data_ptr->tx2_vu;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX2_REC;
//             data.tx_device_2.record = (hl_record_status_t)data_ptr->sys_status.tx2_record_state;
//             hl_mod_main_ioctl(&data);
//             break;
//         case HL_RF_R_CONNECT:
//             data.cmd                  = HL_CHANGE_TX1_ELEC;
//             data.tx_device_1.electric = 0;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX1_SIGNAL;
//             data.tx_device_1.signal = 0;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX1_VOL;
//             data.tx_device_1.volume = 0;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX1_REC;
//             data.tx_device_1.record =0;
//             hl_mod_main_ioctl(&data);
//             //TX2
//             data.cmd                  = HL_CHANGE_TX2_ELEC;
//             data.tx_device_2.electric = data_ptr->tx2_bat_val;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX2_SIGNAL;
//             data.tx_device_2.signal = hl_mod_page_signal_deal(data_ptr->tx2_signal);
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX2_VOL;
//             data.tx_device_2.volume = data_ptr->tx2_vu;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX2_REC;
//             data.tx_device_2.record = (hl_record_status_t)data_ptr->sys_status.tx2_record_state;
//             hl_mod_main_ioctl(&data);
//             break;
//         case HL_RF_L_CONNECT:
//             data.cmd                  = HL_CHANGE_TX1_ELEC;
//             data.tx_device_1.electric = data_ptr->tx1_bat_val;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX1_SIGNAL;
//             data.tx_device_1.signal = hl_mod_page_signal_deal(data_ptr->tx1_signal);
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX1_VOL;
//             data.tx_device_1.volume = data_ptr->tx1_vu;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX1_REC;
//             data.tx_device_1.record = (hl_record_status_t)data_ptr->sys_status.tx1_record_state;
//             hl_mod_main_ioctl(&data);
//             //TX2
//             data.cmd                  = HL_CHANGE_TX2_ELEC;
//             data.tx_device_2.electric = 0;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX2_SIGNAL;
//             data.tx_device_2.signal = 0;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX2_VOL;
//             data.tx_device_2.volume = 0;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX2_REC;
//             data.tx_device_2.record = 0;
//             hl_mod_main_ioctl(&data);
//             break;
//         case HL_RF_UNCONNECT:
//             data.cmd                  = HL_CHANGE_TX1_ELEC;
//             data.tx_device_1.electric = 0;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX1_SIGNAL;
//             data.tx_device_1.signal = 0;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX1_VOL;
//             data.tx_device_1.volume = 0;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX1_REC;
//             data.tx_device_1.record =0;
//             hl_mod_main_ioctl(&data);
//             //TX2
//             data.cmd                  = HL_CHANGE_TX2_ELEC;
//             data.tx_device_2.electric = 0;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX2_SIGNAL;
//             data.tx_device_2.signal = 0;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX2_VOL;
//             data.tx_device_2.volume = 0;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX2_REC;
//             data.tx_device_2.record = 0;
//             hl_mod_main_ioctl(&data);
//             break;
//         case HL_RF_PAIRING:
//             data.cmd                  = HL_CHANGE_TX1_ELEC;
//             data.tx_device_1.electric = 0;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX1_SIGNAL;
//             data.tx_device_1.signal = 0;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX1_VOL;
//             data.tx_device_1.volume = 0;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX1_REC;
//             data.tx_device_1.record =0;
//             hl_mod_main_ioctl(&data);
//             //TX2
//             data.cmd                  = HL_CHANGE_TX2_ELEC;
//             data.tx_device_2.electric = 0;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX2_SIGNAL;
//             data.tx_device_2.signal = 0;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX2_VOL;
//             data.tx_device_2.volume = 0;
//             hl_mod_main_ioctl(&data);

//             data.cmd                = HL_CHANGE_TX2_REC;
//             data.tx_device_2.record = 0;
//             hl_mod_main_ioctl(&data);
//             break;
//         default:
//             break;
//     }
// }
static void hl_mod_main_tx_deal_deinit()
{
    hl_lvgl_main_ioctl_t main_ctl;
    LV_LOG_USER("deinit%d\n", main_tx);

    switch (main_tx) {
        case HL_RF_LR_CONNECT:
            main_ctl.cmd = HL_CHANGE_DELETE_DOUBLE;
            hl_mod_main_ioctl(&main_ctl);
            break;
        case HL_RF_R_CONNECT:
            main_ctl.cmd = HL_CHANGE_DELETE_TX2;
            hl_mod_main_ioctl(&main_ctl);
            break;
        case HL_RF_L_CONNECT:
            main_ctl.cmd = HL_CHANGE_DELETE_TX1;
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
}

static hl_signal_int_t hl_mod_page_signal_deal(uint8_t data)
{
    uint8_t signal = 0;
    signal         = data / 25;
    if (data % 25) {
        signal += 1;
    }
    if (signal >= 4)
        signal = 4;
    return signal;
}

// top层初始化
static void hl_mod_page_top_init(void)
{
    hl_display_sound_module_e now;
    hl_lvgl_top_init_t        top_init;
    hl_display_screen_s*      data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();

    // 更新当前的RX电量信息
    top_init.electric_top  = data_ptr->rx_bat_val;
    top_init.lock_event_cb = hl_mod_page_lock_event;
    hl_mod_top_init(&top_init);

    hl_mod_home_rx_bat_update(data_ptr);

    now          = data_ptr->now_sound_module;
    sound_module = now;
    switch (now) {
        case STEREO:
            hl_mod_icon_deal(HL_TOP_ICON_STEREO_MOD, HL_TOP_ADD_ICON_CMD);
            break;

        case MONO:
            hl_mod_icon_deal(HL_TOP_ICON_SINGLE_MOD, HL_TOP_ADD_ICON_CMD);
            break;
        case SAFE_TRACK:
            hl_mod_icon_deal(HL_TOP_ICON_TRACK_MOD, HL_TOP_ADD_ICON_CMD);
            break;
        default:
            break;
    }

    if (data_ptr->sys_status.tx1_noise || data_ptr->sys_status.tx2_noise) {

        hl_mod_icon_deal(HL_TOP_ICON_NOISE, HL_TOP_ADD_ICON_CMD);
    }

    if (data_ptr->sys_status.line_out_in) {
        hl_mod_icon_deal(HL_TOP_ICON_LINEOUT, HL_TOP_ADD_ICON_CMD);
        hl_mod_display_mux_take();
        flag->sys_status.line_out_in = 0;
        hl_mod_display_mux_release();
    }

    if (data_ptr->sys_status.usb_in) {
        hl_mod_icon_deal(HL_TOP_ICON_TYPEC, HL_TOP_ADD_ICON_CMD);
        hl_mod_display_mux_take();
        flag->sys_status.usb_in = 0;
        hl_mod_display_mux_release();
    }

    if (data_ptr->sys_status.monitor_in) {
        hl_mod_icon_deal(HL_TOP_ICON_HEATSET, HL_TOP_ADD_ICON_CMD);
        hl_mod_display_mux_take();
        flag->sys_status.monitor_in = 0;
        hl_mod_display_mux_release();
    }

    if (data_ptr->sys_status.apple_auth_flag) {
        hl_mod_icon_deal(HL_TOP_ICON_APPLE, HL_TOP_ADD_ICON_CMD);
    }

    if (data_ptr->sys_status.screen_lock == HL_SCREEN_LOCKED) {
        hl_mod_icon_deal(HL_TOP_ICON_LOCK, HL_TOP_ADD_ICON_CMD);

        hl_lvgl_top_ioctl_t ioctl_top;

        ioctl_top.top_cmd = HL_TOP_LOCK_TP_FORBIDDEN;
        hl_mod_top_ioctl(&ioctl_top);
    }
}
static void hl_mod_page_top_update(hl_display_screen_change_s* flag, hl_display_screen_s* now)
{

    if (flag->rx_bat_val || flag->sys_status.rx_charge_status) {
        hl_mod_home_rx_bat_update(now);
        hl_mod_display_mux_take();
        flag->rx_bat_val                  = 0;
        flag->sys_status.rx_charge_status = 0;
        hl_mod_display_mux_release();
        LOG_D("update rx bat %d,%d", now->sys_status.rx_charge_status, now->rx_bat_val);
    }

    // 降噪一起的
    if (flag->sys_status.tx1_noise || flag->sys_status.tx2_noise) {
        if (now->sys_status.tx1_noise || now->sys_status.tx2_noise) {

            hl_mod_icon_deal(HL_TOP_ICON_NOISE, HL_TOP_ADD_ICON_CMD);
        } else if (!now->sys_status.tx1_noise && !now->sys_status.tx2_noise) {
            hl_mod_icon_deal(HL_TOP_ICON_NOISE, HL_TOP_DELETE_ICON_CMD);
        }
        hl_mod_display_mux_take();
        flag->sys_status.tx1_noise = 0;
        flag->sys_status.tx2_noise = 0;
        hl_mod_display_mux_release();
    }

    // // 锁屏
    // if (flag->sys_status.screen_lock) {

    //     hl_mod_display_mux_take();
    //     flag->sys_status.screen_lock = 0;
    //     hl_mod_display_mux_release();

    //     hl_mod_icon_deal_anim(HL_TOP_ICON_LOCK, (hl_top_cmd_t)now->sys_status.screen_lock);
    // }

    // line out
    if (flag->sys_status.line_out_in) {

        if (now->sys_status.line_out_in) {
            hl_mod_icon_deal(HL_TOP_ICON_LINEOUT, HL_TOP_INPUT_CMD);
        } else {
            hl_mod_icon_deal(HL_TOP_ICON_LINEOUT, HL_TOP_DELETE_ICON_CMD);
        }

        hl_mod_display_mux_take();
        flag->sys_status.line_out_in = 0;
        hl_mod_display_mux_release();
    }

    if (flag->sys_status.apple_auth_flag) {

        if (now->sys_status.apple_auth_flag) {
            hl_mod_icon_deal(HL_TOP_ICON_APPLE, HL_TOP_ADD_ICON_CMD);
        } else {
            hl_mod_icon_deal(HL_TOP_ICON_APPLE, HL_TOP_DELETE_ICON_CMD);
        }
        hl_mod_display_mux_take();
        flag->sys_status.apple_auth_flag = 0;
        hl_mod_display_mux_release();

        // if (now->sys_status.apple_auth_flag == 1) {
        //     hl_mod_icon_deal(HL_TOP_ICON_APPLE, (hl_top_cmd_t)now->sys_status.apple_auth_flag);
        // }
    }

    // usb
    if (flag->sys_status.usb_in) {

        if (now->sys_status.usb_in) {
            hl_mod_icon_deal(HL_TOP_ICON_TYPEC, HL_TOP_INPUT_CMD);
        } else {
            hl_mod_icon_deal(HL_TOP_ICON_TYPEC, HL_TOP_DELETE_ICON_CMD);
        }

        hl_mod_display_mux_take();
        flag->sys_status.usb_in = 0;
        hl_mod_display_mux_release();

        // if (!now->sys_status.usb_in && now->sys_status.apple_auth_flag) {
        //     hl_mod_icon_deal(HL_TOP_ICON_APPLE, HL_TOP_DELETE_ICON_CMD);
        // }
    }

    if (flag->sys_status.monitor_in) {

        if (now->sys_status.monitor_in) {
            hl_mod_icon_deal(HL_TOP_ICON_HEATSET, HL_TOP_INPUT_CMD);
        } else {
            hl_mod_icon_deal(HL_TOP_ICON_HEATSET, HL_TOP_DELETE_ICON_CMD);
        }

        hl_mod_display_mux_take();
        flag->sys_status.monitor_in = 0;
        hl_mod_display_mux_release();
    }
}

static void hl_mod_page_home_tx1_update(hl_display_screen_change_s* flag, hl_display_screen_s* now)
{
    hl_lvgl_main_ioctl_t data1;

    if (flag->sys_status.tx1_mute_switch) {
        hl_mod_display_mux_take();
        flag->sys_status.tx1_mute_switch = 0;

        if (now->sys_status.tx1_mute_switch) {
            data1.cmd = HL_CHANGE_TX1_MUTE;
        } else {
            data1.cmd = HL_CHANGE_TX1_MUTE_DEL;
        }
        hl_mod_display_mux_release();

        hl_mod_main_ioctl(&data1);
        LOG_D("mute1=%d", now->sys_status.tx1_mute_switch);
    }

    if (flag->tx1_bat_val || flag->sys_status.tx1_charge_status) {
        hl_mod_home_tx1_bat_update(now);
        hl_mod_display_mux_take();
        flag->tx1_bat_val                  = 0;
        flag->sys_status.tx1_charge_status = 0;
        hl_mod_display_mux_release();
        LOG_D("update bat tx1 %d,%d", now->sys_status.tx1_charge_status, now->tx1_bat_val);
    }

    if (flag->tx1_signal) {
        hl_mod_display_mux_take();
        flag->tx1_signal         = 0;
        data1.cmd                = HL_CHANGE_TX1_SIGNAL;
        data1.tx_device_1.signal = hl_mod_page_signal_deal(now->tx1_signal);
        hl_mod_display_mux_release();
        hl_mod_main_ioctl(&data1);
        // LOG_I("tx1 signal = %d\n",data1.tx_device_1.signal);
    }

    //vu值
    if (flag->tx1_vu) {
        hl_mod_display_mux_take();
        flag->tx1_vu             = 0;
        data1.cmd                = HL_CHANGE_TX1_VOL;
        data1.tx_device_1.volume = now->tx1_vu;
        hl_mod_display_mux_release();

        hl_mod_main_ioctl(&data1);
    }

    if (flag->sys_status.tx1_record_state) {
        hl_mod_display_mux_take();
        flag->sys_status.tx1_record_state = 0;
        data1.cmd                         = HL_CHANGE_TX1_REC;
        data1.tx_device_1.record          = (hl_record_status_t)now->sys_status.tx1_record_state;
        hl_mod_display_mux_release();
        hl_mod_main_ioctl(&data1);
    }
}

static void hl_mod_page_home_tx2_update(hl_display_screen_change_s* flag, hl_display_screen_s* now)
{
    hl_lvgl_main_ioctl_t data2;

    if (flag->sys_status.tx2_mute_switch) {
        hl_mod_display_mux_take();
        flag->sys_status.tx2_mute_switch = 0;

        if (now->sys_status.tx2_mute_switch) {
            data2.cmd = HL_CHANGE_TX2_MUTE;
        } else {
            data2.cmd = HL_CHANGE_TX2_MUTE_DEL;
        }
        hl_mod_display_mux_release();

        hl_mod_main_ioctl(&data2);
        LOG_D("mute2=%d", now->sys_status.tx2_mute_switch);
    }
    
    if (flag->tx2_bat_val || flag->sys_status.tx2_charge_status) {
        hl_mod_home_tx2_bat_update(now);
        hl_mod_display_mux_take();
        flag->tx2_bat_val                  = 0;
        flag->sys_status.tx2_charge_status = 0;
        hl_mod_display_mux_release();
        LOG_D("update bat tx2 %d,%d", now->sys_status.tx2_charge_status, now->tx2_bat_val);
    }
    
    if (flag->tx2_signal) {
        // LOG_D("testl4");
        hl_mod_display_mux_take();
        flag->tx2_signal        = 0;
        data2.cmd                = HL_CHANGE_TX2_SIGNAL;
        data2.tx_device_2.signal = hl_mod_page_signal_deal(now->tx2_signal);
        hl_mod_display_mux_release();
        hl_mod_main_ioctl(&data2);
    }
   
    //vu值
    if (flag->tx2_vu) {
        // LOG_D("testl5");
        hl_mod_display_mux_take();
        flag->tx2_vu             = 0;
        data2.cmd                = HL_CHANGE_TX2_VOL;
        data2.tx_device_2.volume = now->tx2_vu;
        hl_mod_display_mux_release();
        hl_mod_main_ioctl(&data2);
    }
    
    if (flag->sys_status.tx2_record_state) {
        hl_mod_display_mux_take();
        flag->sys_status.tx2_record_state = 0;
        data2.cmd                         = HL_CHANGE_TX2_REC;
        data2.tx_device_2.record          = (hl_record_status_t)now->sys_status.tx2_record_state;
        hl_mod_display_mux_release();
        hl_mod_main_ioctl(&data2);
        // LOG_D("testl2");
    }
}

static void hl_mod_page_main_update(hl_display_screen_change_s* flag, hl_display_screen_s* data_ptr)
{
    if (flag->rf_net_connect) {

        hl_mod_main_tx_deal_deinit();

        hl_mod_display_mux_take();
        flag->rf_net_connect = 0;
        main_tx              = data_ptr->rf_net_connect;
        hl_mod_display_mux_release();
        hl_mod_main_tx_deal_init(data_ptr);

        LOG_D("update%d\n", main_tx);
    }
    // hl_mod_main_tx12_deal_update(data_ptr);
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

    main_tx = data_ptr->rf_net_connect;

    // 默认白色
    bat_state.tx1 = 0;
    bat_state.rx  = 0;
    bat_state.tx2 = 0;
    // hl_mod_icon_deal_init();
    hl_mod_page_top_init();
    hl_mod_main_tx_deal_init(data_ptr);

    // hl_mod_page_home_update();
}

static void hl_mod_page_exit(void)
{
    LOG_D("exithome");
    // 删除中间大图标
    hl_lvgl_top_ioctl_t ctl_data;

    // 删除所有小图标
    hl_mod_icon_deal_deinit();

    // 删除TOP
    ctl_data.top_cmd = HL_TOP_ALL_DEL;
    hl_mod_top_ioctl(&ctl_data);
}

// 锁屏显示时间到的回调,该回调不能有hl_mod_icon_deal函数，会崩溃
static void hl_mod_page_lock_event(hl_lvgl_lock_sta_t state)
{
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_lvgl_top_ioctl_t  ioctl_top;

    if (state == HL_LOCK_STATUS_HIDE) {
        if (data_ptr->sys_status.screen_lock == HL_SCREEN_LOCKING) {
            data_ptr->sys_status.screen_lock = HL_SCREEN_LOCKED;
            // hl_mod_icon_deal(HL_TOP_ICON_LOCK, HL_TOP_ADD_ICON_CMD);
            ioctl_top.top_cmd = HL_TOP_ADD_ICON_CMD;
            ioctl_top.top_param = HL_TOP_ICON_LOCK;
            hl_mod_top_ioctl(&ioctl_top);
            ioctl_top.top_cmd = HL_TOP_LOCK_TP_FORBIDDEN;
            hl_mod_top_ioctl(&ioctl_top);
        } else {
            rt_kprintf("lock time out\n");
        }

    } else if (state == HL_UNLOCK_STATUS_HIDE) {
        if (data_ptr->sys_status.screen_lock == HL_SCREEN_UNLOCKING) {
            data_ptr->sys_status.screen_lock = HL_SCREEN_UNLOCKED;
        }
    }
}

// 锁屏状态的情况，只要有按钮被按下，就居中显示锁屏图标
static void hl_mod_page_lock_state_run()
{
    uint8_t key_event;
    uint8_t touchkey;
    int8_t  knob;

    //单机确定键进入菜单
    key_event = hl_mod_get_knob_okkey_val();
    touchkey  = hl_mod_keypad_touchkey_read();
    knob      = hl_mod_get_rx_knob_val();

    if (key_event != HL_KEY_EVENT_IDLE || touchkey != 0 || knob != 0) {

        hl_mod_icon_deal(HL_TOP_ICON_LOCK, HL_TOP_INPUT_CMD);
    }
}

// 解锁状态的情况
static void hl_mod_page_unlock_state_run()
{
    uint8_t                     key_event;
    hl_display_screen_change_s* flag = hl_mod_page_get_screen_change_flag();
    //单机确定键进入菜单
    key_event = hl_mod_get_knob_okkey_val();

    if (key_event == HL_KEY_EVENT_SHORT) {
        flag->menu_defaut = 1;
        PageManager_PagePush(PAGE_MAIN_MENU);
    }

    // 单机触摸键进入快捷界面
    hl_mod_menu_goto_fast_config_scan();

    // 快捷设置
    hl_mod_menu_goto_quickset_scan();

    // // 菜单的页面更新
    // hl_mod_page_home_update();
}

static void hl_mod_page_run(void)
{
    uint8_t              power_key;
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();

    // 菜单的页面更新
    hl_mod_page_home_update();

    power_key = hl_mod_get_power_key_val();

    switch ((hl_mod_screen_lock_status)data_ptr->sys_status.screen_lock) {
        case HL_SCREEN_UNLOCKED:
            if (power_key == HL_KEY_EVENT_SHORT) {
                data_ptr->sys_status.screen_lock = HL_SCREEN_LOCKING;
                hl_mod_icon_deal(HL_TOP_ICON_LOCK, HL_TOP_INPUT_CMD);
                LOG_D("locking\n");
            } else {
                hl_mod_page_unlock_state_run();
            }
            break;
        case HL_SCREEN_LOCKED:
            if (power_key == HL_KEY_EVENT_SHORT) {
                data_ptr->sys_status.screen_lock = HL_SCREEN_UNLOCKING;
                LOG_D("unlocking\n");
                hl_mod_icon_deal(HL_TOP_ICON_UNLOCK, HL_TOP_INPUT_CMD);
            } else {
                hl_mod_page_lock_state_run();
            }
            break;
        case HL_SCREEN_LOCKING:
            if (power_key == HL_KEY_EVENT_SHORT) {
                data_ptr->sys_status.screen_lock = HL_SCREEN_UNLOCKING;
                hl_mod_icon_deal(HL_TOP_ICON_UNLOCK, HL_TOP_INPUT_CMD);
                LOG_D("unclocking\n");
            }
            // 只更新当前的页面，不处理锁屏和解锁功能
            break;
        case HL_SCREEN_UNLOCKING:
            if (power_key == HL_KEY_EVENT_SHORT) {
                data_ptr->sys_status.screen_lock = HL_SCREEN_LOCKING;
                hl_mod_icon_deal(HL_TOP_ICON_LOCK, HL_TOP_INPUT_CMD);
                LOG_D("locking\n");
            }
            // 只更新当前的页面，不处理锁屏和解锁功能
            break;
        default:
            break;
    }
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