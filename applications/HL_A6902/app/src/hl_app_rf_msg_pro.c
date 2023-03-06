/**
 * @file hl_app_rf_msg_pro.c
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-11-14
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
 * <tr><td>2022-11-14     <td>v1.0     <td>luzhanghao     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include <rtdevice.h>
#include "hl_config.h"
#include "hl_util_msg_type.h"
#include "hl_util_general_type.h"
#include "hl_app_mng.h"
#include "hl_app_disp_msg_pro.h"
#include "hl_app_pm_msg_pro.h"
#include "hl_app_audio_msg_pro.h"
#include "hl_mod_display.h"
#include "hl_mod_telink.h"
#include "hl_mod_euc.h"
#include "hl_mod_audio.h"
#include "hl_util_nvram.h"

#define DBG_SECTION_NAME "app_rf"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#if HL_IS_TX_DEVICE()
void hl_app_rf_msg_pro(mode_to_app_msg_t* p_msg)
{
    // hl_led_mode     led_ctrl;
    uint8_t               p_param;
    uint8_t*              ptr;
    hl_rf_channel_e       bypass_chn;
    hl_rf_bypass_string_t bypass_string;
    hl_rf_bypass_state_t  bypass_state;
    hl_rf_bypass_value_t  bypass_value;
    hl_switch_e           bypass_switch;
    hl_rf_bypass_state_t* ptr_rf_state = NULL;
    hl_rf_bypass_value_t* ptr_rf_value = NULL;
    hl_rf_bypass_time_t*  ptr_rf_time  = NULL;
    uint32_t              u32_param;
    hl_rf_tx_info_t       bypass_tx_info;

    // LOG_D("hl_app_rf_msg_pro get telink msg(%d)!!! \r\n", p_msg->cmd);
    switch (p_msg->cmd) {
        case HL_RF_VERSION_IND:
            ptr = (uint8_t*)p_msg->param.ptr;
            LOG_D("\n\n--- Telink Version[%d.%d.%d.%d] ---\n\n", ptr[0], ptr[1], ptr[2], ptr[3]);
            break;

        case HL_RF_PAIR_STATE_IND:
            tx_info.rf_state = *(hl_rf_state_e*)p_msg->param.ptr;
            tx_info.rf_chn   = tx_info.rf_state - 1;

            if (HL_RF_UNCONNECT != tx_info.rf_state && HL_RF_PAIRING != tx_info.rf_state) {
                bypass_string.chn  = tx_info.rf_chn;
                bypass_tx_info.chn = tx_info.rf_chn;
                // 状态信息交互
                bypass_tx_info.mute    = tx_info.mute_flag;
                bypass_tx_info.denoise = tx_info.denoise_flag;
                bypass_tx_info.charge  = tx_info.charge_flag == 1 ? HL_RF_ON : HL_RF_OFF;
                bypass_tx_info.battery = tx_info.soc;
                bypass_tx_info.record  = tx_info.rec_flag;
                hl_mod_telink_ioctl(HL_RF_BYPASS_UPDATE_CMD, (uint8_t*)&bypass_tx_info, sizeof(bypass_tx_info));
                // version
                rt_memset(bypass_string.str, 0, sizeof(bypass_string.str));
                rt_memcpy(bypass_string.str, A6902_VERSION, sizeof(A6902_VERSION));
                hl_mod_telink_ioctl(HL_RF_BYPASS_VERSION_CMD, (uint8_t*)&bypass_string, sizeof(bypass_string));
                LOG_D("telink info show!");
            }
            hl_app_disp_state_led_set();
            hl_app_pm_timer_set();
            LOG_D("telink info(%02X)", tx_info.rf_state);
            break;

        case HL_RF_REFRESH_STATE_IND:
            tx_info.rf_state = *(hl_rf_state_e*)p_msg->param.ptr;
            tx_info.rf_chn   = tx_info.rf_state - 1;
            hl_app_disp_state_led_set();
            break;

        case HL_RF_RSSI_IND:
            p_param = *(uint8_t*)p_msg->param.ptr;
            // LOG_D("\ntelink RSSI(%02X)\r\n", p_param);
            break;

        case HL_RF_BYPASS_AUTO_RECORD_IND:
            ptr_rf_state  = (hl_rf_bypass_state_t*)p_msg->param.ptr;
            bypass_switch = (hl_switch_e)ptr_rf_state->state;
            if (tx_info.rec_auto_flag != bypass_switch) {
                tx_info.rec_auto_flag = bypass_switch;
                hl_util_nvram_param_set_integer("REC_AUTO_OPEN", bypass_switch);
            }
            LOG_I("app get RX Auto Record(%d) ", bypass_switch);
            break;

        case HL_RF_BYPASS_RECORD_PROTECT_IND:
            ptr_rf_state  = (hl_rf_bypass_state_t*)p_msg->param.ptr;
            bypass_switch = (hl_switch_e)ptr_rf_state->state;
            if (tx_info.rec_protect_flag != bypass_switch) {
                tx_info.rec_protect_flag = bypass_switch;
                hl_util_nvram_param_set_integer("REC_PROTECT_OPEN", bypass_switch);
            }
            LOG_I("app get RX Record Protect(%d) ", bypass_switch);
            break;

        case HL_RF_BYPASS_RECORD_IND:
            ptr_rf_state  = (hl_rf_bypass_state_t*)p_msg->param.ptr;
            bypass_switch = (hl_switch_e)ptr_rf_state->state;
            if (tx_info.rec_flag != bypass_switch) {
                tx_info.rec_flag = bypass_switch;
                hl_mod_audio_io_ctrl(HL_AUDIO_RECORD_CMD, &bypass_switch, 1);
                hl_app_disp_state_led_set();
                hl_app_pm_timer_set();
            }
            LOG_I("app get RX Record Switch(%d) ", bypass_switch);
            break;

        case HL_RF_BYPASS_MUTE_IND:
            ptr_rf_state  = (hl_rf_bypass_state_t*)p_msg->param.ptr;
            bypass_switch = (hl_switch_e)ptr_rf_state->state;
            if (tx_info.mute_flag != bypass_switch) {
                tx_info.mute_flag = bypass_switch;
                hl_mod_audio_io_ctrl(HL_AUDIO_SET_MUTE_CMD, &bypass_switch, 1);
                hl_app_disp_state_led_set();
            }
            LOG_I("app get RX mute(%d) ", bypass_switch);
            break;

        case HL_RF_GET_LOCAL_MAC_IND:
            ptr = (uint8_t*)p_msg->param.ptr;
            rt_memcpy(tx_info.local_mac, ptr, sizeof(tx_info.local_mac));
            LOG_I("local mac addr: [%02x] [%02x] [%02x] [%02x] [%02x] [%02x]", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4],
                  ptr[5]);
            break;

        case HL_RF_GET_REMOTE_MAC_IND:
            ptr = (uint8_t*)p_msg->param.ptr;
            rt_memcpy(tx_info.remote_mac, ptr, sizeof(tx_info.remote_mac));
            LOG_I("remote mac addr: [%02x] [%02x] [%02x] [%02x] [%02x] [%02x]", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4],
                  ptr[5]);
            break;

        case HL_RF_BYPASS_TX_GAIN_IND:
            ptr_rf_value = (hl_rf_bypass_value_t*)p_msg->param.ptr;
            tx_info.gain = (int8_t)ptr_rf_value->val;
            tx_info.gain = tx_info.gain * 2;
            hl_app_audio_gain(tx_info.gain);
            hl_util_nvram_param_set_integer("TX_GAIN", tx_info.gain);
            LOG_D("app get TX%d Gain Value(%d)(%d)", ptr_rf_value->chn, (int8_t)ptr_rf_value->val, tx_info.gain);
            break;

        case HL_RF_BYPASS_FORMAT_DISK_IND:
            bypass_chn = *(hl_rf_channel_e*)p_msg->param.ptr;
            hl_mod_audio_io_ctrl(HL_AUDIO_MKFS_DFS_CMD, NULL, 0);
            LOG_D("app get RX Format disk");
            break;

        case HL_RF_BYPASS_UAC_GAIN_IND:
            ptr_rf_value     = (hl_rf_bypass_value_t*)p_msg->param.ptr;
            tx_info.uac_gain = (int)ptr_rf_value->val;
            hl_app_audio_gain_uac(tx_info.uac_gain);
            hl_util_nvram_param_set_integer("TX_UAC_GAIN", tx_info.uac_gain);
            LOG_D("app get TX%d UAC In Gain(%d)", ptr_rf_value->chn, (int8_t)ptr_rf_value->val);
            break;

        case HL_RF_BYPASS_LOWCUT_IND:
            ptr_rf_state = (hl_rf_bypass_state_t*)p_msg->param.ptr;
            LOG_D("app get RX Low Cut(%d)", ptr_rf_state->state);
            break;

        case HL_RF_BYPASS_DENOISE_IND:
            ptr_rf_value  = (hl_rf_bypass_value_t*)p_msg->param.ptr;
            bypass_switch = (uint8_t)ptr_rf_value->val;
            if (tx_info.denoise_flag == 0) {
                bypass_switch        = HL_SWITCH_ON;
                tx_info.denoise_flag = 1;
            } else {
                bypass_switch        = HL_SWITCH_OFF;
                tx_info.denoise_flag = 0;
            }
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_DENOISE_CMD, &bypass_switch, sizeof(bypass_switch));
            hl_app_disp_state_led_set();
            LOG_D("app get RX Denoise(%d)", bypass_switch);
            break;

        case HL_RF_BYPASS_STATUS_LED_IND:
            ptr_rf_value = (hl_rf_bypass_value_t*)p_msg->param.ptr;
            hl_mod_display_io_ctrl(LED_BRIGHT_SET_CMD, &ptr_rf_value->val, sizeof(ptr_rf_value->val));
            LOG_D("app get RX LED Status(%d)", ptr_rf_value->val);
            break;

        case HL_RF_BYPASS_AUTO_POWEROFF_IND:
            ptr_rf_state = (hl_rf_bypass_state_t*)p_msg->param.ptr;
            LOG_D("app get RX Auto Poweroff(%d)", ptr_rf_state->state);
            break;

        case HL_RF_BYPASS_TIME_IND:
            ptr_rf_time = (hl_rf_bypass_time_t*)p_msg->param.ptr;
            // hl_mod_audio_io_ctrl(HL_AUDIO_SET_TIME_CMD, &ptr_rf_time->time, sizeof(ptr_rf_time->time));
            // LOG_D("app get TX%d Time[%d][%d][%d][%d][%d][%d]", ptr_rf_time->chn, ptr_rf_time->time.year,
            //       ptr_rf_time->time.month, ptr_rf_time->time.day, ptr_rf_time->time.hour, ptr_rf_time->time.minute,
            //       ptr_rf_time->time.second);
            break;

        case HL_RF_BYPASS_SOUND_EFFECT_IND:
            ptr_rf_value = (hl_rf_bypass_value_t*)p_msg->param.ptr;
            LOG_D("app get RX Sound Effect(%d)", ptr_rf_value->val);
            break;

        case HL_RF_BYPASS_REFACTORY_IND:
            bypass_chn = *(hl_rf_channel_e*)p_msg->param.ptr;
            LOG_D("app get RX Refactory");
            break;

        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
    hl_app_disp_state_led_set();
}
#else
void hl_app_rf_msg_pro(mode_to_app_msg_t* p_msg)
{
    // hl_led_mode     led_ctrl;
    uint8_t                tx1_rssi;
    uint8_t                tx2_rssi;
    uint8_t*               ptr           = NULL;
    hl_rf_bypass_state_t*  ptr_rf_state  = NULL;
    hl_rf_bypass_value_t*  ptr_rf_value  = NULL;
    hl_rf_bypass_string_t* ptr_rf_string = NULL;
    hl_rf_tx_info_t*       ptr_rf_info   = NULL;
    hl_rf_bypass_state_t   bypass_state;
    hl_rf_bypass_value_t   bypass_value;
    hl_rf_bypass_string_t  bypass_string;
    hl_rf_bypass_time_t    bypass_time;

    // LOG_D("hl_app_rf_msg_pro get telink msg(%d)!!! \r\n", p_msg->cmd);
    switch (p_msg->cmd) {
        case HL_RF_VERSION_IND:
            ptr = (uint8_t*)p_msg->param.ptr;
            LOG_D("\n\n--- Telink Version[%d.%d.%d.%d] ---\n\n", ptr[0], ptr[1], ptr[2], ptr[3]);
            break;

        case HL_RF_PAIR_STATE_IND:
            rx_info.rf_state = *(hl_rf_state_e*)p_msg->param.ptr;
            rx_info.rf_chn   = rx_info.rf_state - 1;
            LOG_D("telink info(%02X)", rx_info.rf_state);
            hl_mod_display_io_ctrl(RX_RF_STATE_VAL_CMD, &rx_info.rf_state, sizeof(rx_info.rf_state));
            bypass_time.chn = rx_info.rf_chn;
            hl_app_pm_timer_set();
            // hl_mod_audio_io_ctrl(HL_AUDIO_GET_RTC_TIME_CMD, &bypass_time.time, sizeof(bypass_time.time));
            // hl_mod_telink_ioctl(HL_RF_BYPASS_TIME_CMD, &bypass_time, sizeof(bypass_time));
            break;

        case HL_RF_REFRESH_STATE_IND:
            rx_info.rf_state = *(hl_rf_state_e*)p_msg->param.ptr;
            rx_info.rf_chn   = rx_info.rf_state - 1;
            hl_app_audio_switch();
            break;

        case HL_RF_RSSI_IND:
            tx1_rssi = ((uint8_t*)p_msg->param.ptr)[0];
            tx2_rssi = ((uint8_t*)p_msg->param.ptr)[1];
            // LOG_D("telink RSSI(%02X -- %02X)", tx1_rssi, tx2_rssi);
            hl_mod_display_io_ctrl(TX1_SIGNAL_VAL_CMD, &tx1_rssi, 1);
            hl_mod_display_io_ctrl(TX2_SIGNAL_VAL_CMD, &tx2_rssi, 1);
            break;

        case HL_RF_BYPASS_VERSION_IND:
            ptr_rf_string = (hl_rf_bypass_string_t*)p_msg->param.ptr;
            if (ptr_rf_string->chn == HL_RF_LEFT_CHANNEL) {
                rt_memcpy(rx_info.tx1_version, ptr_rf_string->str, sizeof(ptr_rf_string->str));
                hl_mod_display_io_ctrl(TX1_VER_STRING_CMD, rx_info.tx1_version, rt_strlen(rx_info.tx1_version));
            } else if (ptr_rf_string->chn == HL_RF_RIGHT_CHANNEL) {
                rt_memcpy(rx_info.tx2_version, ptr_rf_string->str, sizeof(ptr_rf_string->str));
                hl_mod_display_io_ctrl(TX2_VER_STRING_CMD, rx_info.tx2_version, rt_strlen(rx_info.tx2_version));
            } else {
                LOG_E("telink version receive error\n");
            }
            LOG_D("app get TX%d Version(%s)", ptr_rf_string->chn, ptr_rf_string->str);
            break;

        case HL_RF_BYPASS_UPDATE_IND:
            ptr_rf_info = (hl_rf_tx_info_t*)p_msg->param.ptr;
            if (HL_RF_LEFT_CHANNEL == ptr_rf_info->chn) {
                rx_info.tx1_mute = ptr_rf_info->mute;
                hl_mod_display_io_ctrl(TX1_BAT_VAL_VAL_CMD, &ptr_rf_info->battery, sizeof(ptr_rf_info->battery));
                hl_mod_display_io_ctrl(TX1_MUTE_SWITCH_SWITCH_CMD, &ptr_rf_info->mute, 1);
                hl_mod_display_io_ctrl(TX1_NOISE_SWITCH_CMD, &ptr_rf_info->denoise, 1);
                hl_mod_display_io_ctrl(TX1_CHARGE_STATUS_SWITCH_CMD, &ptr_rf_info->charge, 1);
                if (1 == ptr_rf_info->record) {
                    hl_mod_display_io_ctrl(TX1_RECORD_STATE_SWITCH_CMD, &ptr_rf_info->record, 1);
                }
            } else if (HL_RF_RIGHT_CHANNEL == ptr_rf_info->chn) {
                rx_info.tx2_mute = ptr_rf_info->mute;
                hl_mod_display_io_ctrl(TX2_BAT_VAL_VAL_CMD, &ptr_rf_info->battery, sizeof(ptr_rf_info->battery));
                hl_mod_display_io_ctrl(TX2_MUTE_SWITCH_SWITCH_CMD, &ptr_rf_info->mute, 1);
                hl_mod_display_io_ctrl(TX2_NOISE_SWITCH_CMD, &ptr_rf_info->denoise, 1);
                hl_mod_display_io_ctrl(TX2_CHARGE_STATUS_SWITCH_CMD, &ptr_rf_info->charge, 1);
                if (1 == ptr_rf_info->record) {
                    hl_mod_display_io_ctrl(TX2_RECORD_STATE_SWITCH_CMD, &ptr_rf_info->record, 1);
                }
            } else {
                LOG_E("telink update_info receive error");
            }
            LOG_I("RX Telink Get TX%d Update info!", ptr_rf_info->chn);
            break;

        case HL_RF_BYPASS_MUTE_IND:
            ptr_rf_state = (hl_rf_bypass_state_t*)p_msg->param.ptr;
            if (HL_RF_LEFT_CHANNEL == ptr_rf_state->chn) {
                rx_info.tx1_mute = ptr_rf_state->state;
                hl_mod_display_io_ctrl(TX1_MUTE_SWITCH_SWITCH_CMD, &ptr_rf_state->state, 1);
            } else if (HL_RF_RIGHT_CHANNEL == ptr_rf_state->chn) {
                rx_info.tx2_mute = ptr_rf_state->state;
                hl_mod_display_io_ctrl(TX2_MUTE_SWITCH_SWITCH_CMD, &ptr_rf_state->state, 1);
            } else {
                LOG_E("telink mute receive error(%02X -- %02X)", rx_info.tx1_mute, rx_info.tx2_mute);
            }

            LOG_D("app get TX%d MUTE(%d)", ptr_rf_state->chn, ptr_rf_state->state);
            hl_mod_display_io_ctrl(TX1_SIGNAL_VAL_CMD, &tx1_rssi, 1);
            hl_mod_display_io_ctrl(TX2_SIGNAL_VAL_CMD, &tx2_rssi, 1);
            break;

        case HL_RF_BYPASS_DENOISE_IND:
            ptr_rf_state = (hl_rf_bypass_state_t*)p_msg->param.ptr;
            if (ptr_rf_state->chn == HL_RF_LEFT_CHANNEL) {

                hl_mod_display_io_ctrl(TX1_NOISE_SWITCH_CMD, &ptr_rf_state->state, 1);
            } else if (ptr_rf_state->chn == HL_RF_RIGHT_CHANNEL) {

                hl_mod_display_io_ctrl(TX2_NOISE_SWITCH_CMD, &ptr_rf_state->state, 1);
            } else {
                LOG_E("telink mute receive error(%02X -- %02X)", rx_info.tx1_mute, rx_info.tx2_mute);
            }
            LOG_D("app get TX%d Denoise(%d)", ptr_rf_state->chn, ptr_rf_state->state);
            break;

        case HL_RF_BYPASS_CHARGE_IND:
            ptr_rf_state = (hl_rf_bypass_state_t*)p_msg->param.ptr;
            if (ptr_rf_state->chn == HL_RF_LEFT_CHANNEL) {
                hl_mod_display_io_ctrl(TX1_CHARGE_STATUS_SWITCH_CMD, &ptr_rf_state->state, 1);
            } else if (ptr_rf_state->chn == HL_RF_RIGHT_CHANNEL) {
                hl_mod_display_io_ctrl(TX2_CHARGE_STATUS_SWITCH_CMD, &ptr_rf_state->state, 1);
            } else {
                LOG_E("telink Charge receive error(%02X -- %02X)", rx_info.tx1_mute, rx_info.tx2_mute);
            }
            LOG_D("app get TX%d Charge(%d)", ptr_rf_state->chn, ptr_rf_state->state);
            break;

        case HL_RF_BYPASS_RECORD_IND:
            ptr_rf_state = (hl_rf_bypass_state_t*)p_msg->param.ptr;
            if (2 == ptr_rf_state->state) {
                hl_mod_euc_ctrl(HL_HID_START_RECORD_CMD, RT_NULL, 0);
                LOG_D("app get TX%d HID Record(%d)", ptr_rf_state->chn, ptr_rf_state->state);
            } else {
                if (ptr_rf_state->chn == HL_RF_LEFT_CHANNEL) {
                    hl_mod_display_io_ctrl(TX1_RECORD_STATE_SWITCH_CMD, &ptr_rf_state->state, 1);
                } else if (ptr_rf_state->chn == HL_RF_RIGHT_CHANNEL) {
                    hl_mod_display_io_ctrl(TX2_RECORD_STATE_SWITCH_CMD, &ptr_rf_state->state, 1);
                } else {
                    LOG_E("telink mute receive error(%02X -- %02X)", rx_info.tx1_mute, rx_info.tx2_mute);
                }
                LOG_D("app get TX%d Record(%d)", ptr_rf_state->chn, ptr_rf_state->state);
            }
            break;

        case HL_RF_BYPASS_BATTERY_IND:
            ptr_rf_value = (hl_rf_bypass_value_t*)p_msg->param.ptr;
            LOG_D("app get TX%d battery(%d)", ptr_rf_value->chn, ptr_rf_value->val);
            if (ptr_rf_value->chn == 0) {
                hl_mod_display_io_ctrl(TX1_BAT_VAL_VAL_CMD, &ptr_rf_value->val, sizeof(ptr_rf_value->val));
            } else if (ptr_rf_value->chn == 1) {
                hl_mod_display_io_ctrl(TX2_BAT_VAL_VAL_CMD, &ptr_rf_value->val, sizeof(ptr_rf_value->val));
            }
            break;

        case HL_RF_GET_LOCAL_MAC_IND:
            ptr = (uint8_t*)p_msg->param.ptr;
            rt_memcpy(rx_info.local_mac, ptr, sizeof(rx_info.local_mac));
            LOG_I("local mac addr: [%02x] [%02x] [%02x] [%02x] [%02x] [%02x]", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4],
                  ptr[5]);
            break;

        case HL_RF_GET_REMOTE_MAC_IND:
            ptr = (uint8_t*)p_msg->param.ptr;
            rt_memcpy(rx_info.remote_mac, ptr, sizeof(rx_info.remote_mac));
            LOG_I(
                "remote mac addr: [%02x] [%02x] [%02x] [%02x] [%02x] [%02x]  [%02x] [%02x] [%02x] [%02x] [%02x] [%02x]",
                ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7], ptr[8], ptr[9], ptr[10], ptr[11]);
            break;
        case HL_RF_BYPASS_STATUS_LED_IND:
            ptr_rf_value = (hl_rf_bypass_value_t*)p_msg->param.ptr;

            hl_mod_display_io_ctrl(LED_BRITNESS_VAL_CMD, &ptr_rf_value->val, 1);
            break;

        case HL_RF_REBOOT_IND:
            if (!rx_info.usb_pogo_flag) {
                LOG_I("[OK]RX Reboot Pair!!! \r\n");
                hl_mod_telink_ioctl(HL_RF_REBOOT_CMD, &tx1_rssi, 1);
            }
            break;

        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}
#endif
/*
 * EOF
 */