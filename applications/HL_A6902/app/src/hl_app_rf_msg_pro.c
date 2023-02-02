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
#include "hl_mod_display.h"
#include "hl_mod_telink.h"
#include "hl_mod_euc.h"
#include "hl_mod_audio.h"

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
    uint8_t                p_param;
    uint8_t*               ptr;
    hl_rf_state_e          rf_state;
    hl_rf_channel_e        bypass_chn;
    hl_rf_bypass_version_t bypass_ver;
    hl_rf_bypass_state_t   bypass_state;
    hl_rf_bypass_value_t   bypass_value;
    hl_switch_e            bypass_switch;
    hl_rf_bypass_state_t*  ptr_rf_state;
    hl_rf_bypass_value_t*  ptr_rf_value;

    // LOG_D("hl_app_rf_msg_pro get telink msg(%d)!!! \r\n", p_msg->cmd);
    switch (p_msg->cmd) {
        case HL_RF_VERSION_IND:
            ptr = (uint8_t*)p_msg->param.ptr;
            LOG_D("\n\n--- Telink Version[%d.%d.%d.%d] ---\n\n", ptr[0], ptr[1], ptr[2], ptr[3]);
            break;

        case HL_RF_PAIR_STATE_IND:
            tx_info.rf_state = *(hl_rf_state_e*)p_msg->param.ptr;
            rf_state         = tx_info.rf_state;

            if (HL_RF_L_CONNECT == rf_state || HL_RF_R_CONNECT == rf_state) {
                bypass_ver.chn     = rf_state - 1;
                bypass_state.chn   = rf_state - 1;
                bypass_value.chn   = rf_state - 1;
                bypass_state.state = tx_info.denoise_flag;
                hl_mod_telink_ioctl(HL_RF_BYPASS_DENOISE_CMD, (uint8_t*)&bypass_state, sizeof(bypass_state));
                bypass_state.state = tx_info.rec_flag;
                hl_mod_telink_ioctl(HL_RF_BYPASS_RECORD_CMD, (uint8_t*)&bypass_state, sizeof(bypass_state));
                bypass_value.val = tx_info.soc;
                hl_mod_telink_ioctl(HL_RF_BYPASS_BATTERY_CMD, (uint8_t*)&bypass_value, sizeof(bypass_value));
            } else if (HL_RF_LR_CONNECT == rf_state) {
                bypass_state.chn   = HL_RF_DOUBLE_CHANNEL;
                bypass_value.chn   = HL_RF_DOUBLE_CHANNEL;
                bypass_state.state = tx_info.denoise_flag;
                hl_mod_telink_ioctl(HL_RF_BYPASS_DENOISE_CMD, (uint8_t*)&bypass_state, sizeof(bypass_state));
                bypass_state.state = tx_info.rec_flag;
                hl_mod_telink_ioctl(HL_RF_BYPASS_RECORD_CMD, (uint8_t*)&bypass_state, sizeof(bypass_state));
                bypass_value.val = tx_info.soc;
                hl_mod_telink_ioctl(HL_RF_BYPASS_BATTERY_CMD, (uint8_t*)&bypass_value, sizeof(bypass_value));
            }
            LOG_D("telink info(%02X)", tx_info.rf_state);
            // hl_mod_display_io_ctrl(TX_RF_STATE_VAL_CMD, &rf_state, sizeof(rf_state));
            hl_app_disp_state_led_set();
            break;

        case HL_RF_RSSI_IND:
            p_param = *(uint8_t*)p_msg->param.ptr;
            // LOG_D("\ntelink RSSI(%02X)\r\n", p_param);
            break;

        case HL_RF_BYPASS_AUTO_RECORD_IND:
            ptr_rf_state  = (hl_rf_bypass_state_t*)p_msg->param.ptr;
            bypass_switch = (hl_switch_e)ptr_rf_state->state;
            LOG_D("app get TX%d Auto Record(%d)\r\n", ptr_rf_state->chn, bypass_switch);
            break;

        case HL_RF_BYPASS_RECORD_PROTECT_IND:
            ptr_rf_state  = (hl_rf_bypass_state_t*)p_msg->param.ptr;
            bypass_switch = (hl_switch_e)ptr_rf_state->state;
            LOG_D("app get TX%d Record Protect(%d)\r\n", ptr_rf_state->chn, bypass_switch);
            break;

        case HL_RF_BYPASS_RECORD_IND:
            ptr_rf_state     = (hl_rf_bypass_state_t*)p_msg->param.ptr;
            bypass_switch    = (hl_switch_e)ptr_rf_state->state;
            tx_info.rec_flag = bypass_switch;
            hl_mod_audio_io_ctrl(HL_AUDIO_RECORD_CMD, &bypass_switch, 1);
            LOG_D("app get TX%d Record Switch(%d)\r\n", ptr_rf_state->chn, bypass_switch);
            break;

        case HL_RF_BYPASS_MUTE_IND:
            ptr_rf_state  = (hl_rf_bypass_state_t*)p_msg->param.ptr;
            bypass_switch = (hl_switch_e)ptr_rf_state->state;
            if (tx_info.mute_flag != bypass_switch) {
                tx_info.mute_flag = bypass_switch;
                hl_mod_audio_io_ctrl(HL_AUDIO_SET_MUTE_CMD, &bypass_switch, 1);
                hl_app_disp_state_led_set();
            }
            LOG_D("app get TX%d mute(%d)\r\n", ptr_rf_state->chn, bypass_switch);
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
            LOG_D("app get TX%d Gain Value(%d)", ptr_rf_value->chn, (int)ptr_rf_value->val);
            break;

        case HL_RF_BYPASS_FORMAT_DISK_IND:
            bypass_chn = *(hl_rf_channel_e*)p_msg->param.ptr;
            LOG_D("app get TX%d Format disk", bypass_chn);
            break;

        case HL_RF_BYPASS_UAC_GAIN_IND:
            ptr_rf_value = (hl_rf_bypass_value_t*)p_msg->param.ptr;
            LOG_D("app get TX%d UAC Gain(%d)", ptr_rf_value->chn, ptr_rf_value->val);
            break;

        case HL_RF_BYPASS_LOWCUT_IND:
            ptr_rf_state = (hl_rf_bypass_state_t*)p_msg->param.ptr;
            LOG_D("app get TX%d Low Cut(%d)", ptr_rf_state->chn, ptr_rf_state->state);
            break;

        case HL_RF_BYPASS_DENOISE_IND:
            ptr_rf_value  = (hl_rf_bypass_value_t*)p_msg->param.ptr;
            bypass_switch = (uint8_t)ptr_rf_value->val;
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_DENOISE_CMD, &bypass_switch, sizeof(bypass_switch));
            LOG_D("app get TX%d Denoise(%d)", ptr_rf_value->chn, bypass_switch);
            break;

        case HL_RF_BYPASS_STATUS_LED_IND:
            ptr_rf_value = (hl_rf_bypass_value_t*)p_msg->param.ptr;
            hl_mod_display_io_ctrl(LED_BRIGHT_SET_CMD, &ptr_rf_value->val, sizeof(ptr_rf_value->val));
            LOG_D("app get TX%d LED Status(%d)", ptr_rf_value->chn, ptr_rf_value->val);
            break;

        case HL_RF_BYPASS_AUTO_POWEROFF_IND:
            ptr_rf_state = (hl_rf_bypass_state_t*)p_msg->param.ptr;
            LOG_D("app get TX%d Auto Poweroff(%d)", ptr_rf_state->chn, ptr_rf_state->state);
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
    uint8_t               tx1_rssi;
    uint8_t               tx2_rssi;
    uint8_t*              ptr;
    hl_rf_state_e         rf_state;
    hl_rf_bypass_value_t* ptr_rf_value;
    hl_rf_bypass_state_t  tx_mute;

    // LOG_D("hl_app_rf_msg_pro get telink msg(%d)!!! \r\n", p_msg->cmd);
    switch (p_msg->cmd) {
        case HL_RF_VERSION_IND:
            ptr = (uint8_t*)p_msg->param.ptr;
            LOG_D("\n\n--- Telink Version[%d.%d.%d.%d] ---\n\n", ptr[0], ptr[1], ptr[2], ptr[3]);
            break;

        case HL_RF_PAIR_STATE_IND:
            rx_info.rf_state = *(hl_rf_state_e*)p_msg->param.ptr;
            rf_state         = rx_info.rf_state;
            LOG_D("telink info(%02X)", rx_info.rf_state);
            hl_mod_display_io_ctrl(RX_RF_STATE_VAL_CMD, &rf_state, sizeof(rf_state));
            break;

        case HL_RF_RSSI_IND:
            tx1_rssi = ((uint8_t*)p_msg->param.ptr)[0];
            tx2_rssi = ((uint8_t*)p_msg->param.ptr)[1];
            // LOG_D("telink RSSI(%02X -- %02X)", tx1_rssi, tx2_rssi);
            hl_mod_display_io_ctrl(TX1_SIGNAL_VAL_CMD, &tx1_rssi, 1);
            hl_mod_display_io_ctrl(TX2_SIGNAL_VAL_CMD, &tx2_rssi, 1);
            break;

        case HL_RF_BYPASS_MUTE_IND:
            tx_mute = *(hl_rf_bypass_state_t*)p_msg->param.ptr;
            if (tx_mute.chn == HL_RF_LEFT_CHANNEL) {
                rx_info.tx1_mute = tx_mute.state;
                hl_mod_display_io_ctrl(TX1_MUTE_SWITCH_SWITCH_CMD, &tx_mute.state, 1);
            } else if (tx_mute.chn == HL_RF_RIGHT_CHANNEL) {
                rx_info.tx2_mute = tx_mute.state;
                hl_mod_display_io_ctrl(TX2_MUTE_SWITCH_SWITCH_CMD, &tx_mute.state, 1);
            } else {
                LOG_E("telink mute receive error(%02X -- %02X)", rx_info.tx1_mute, rx_info.tx2_mute);
            }

            hl_mod_display_io_ctrl(TX1_SIGNAL_VAL_CMD, &tx1_rssi, 1);
            hl_mod_display_io_ctrl(TX2_SIGNAL_VAL_CMD, &tx2_rssi, 1);
            break;

        case HL_RF_BYPASS_DENOISE_IND:
            tx_mute = *(hl_rf_bypass_state_t*)p_msg->param.ptr;
            if (tx_mute.chn == HL_RF_LEFT_CHANNEL) {

                hl_mod_display_io_ctrl(TX1_MUTE_SWITCH_SWITCH_CMD, &tx_mute.state, 1);
            } else if (tx_mute.chn == HL_RF_RIGHT_CHANNEL) {

                hl_mod_display_io_ctrl(TX2_MUTE_SWITCH_SWITCH_CMD, &tx_mute.state, 1);
            } else {
                LOG_E("telink mute receive error(%02X -- %02X)", rx_info.tx1_mute, rx_info.tx2_mute);
            }
            LOG_D("app get denoise indicate,ch(%d),v(%d)", tx_mute.chn, tx_mute.state);
            break;

        case HL_RF_BYPASS_VOLUME_IND:
            LOG_D("app get volume indicate");
            break;

        case HL_RF_BYPASS_RECORD_IND:
            hl_mod_euc_ctrl(HL_HID_START_RECORD_CMD, RT_NULL, 0);
            tx_mute = *(hl_rf_bypass_state_t*)p_msg->param.ptr;
            if (tx_mute.chn == HL_RF_LEFT_CHANNEL) {

                hl_mod_display_io_ctrl(TX1_RECORD_STATE_SWITCH_CMD, &tx_mute.state, 1);
            } else if (tx_mute.chn == HL_RF_RIGHT_CHANNEL) {

                hl_mod_display_io_ctrl(TX2_RECORD_STATE_SWITCH_CMD, &tx_mute.state, 1);
            } else {
                LOG_E("telink mute receive error(%02X -- %02X)", rx_info.tx1_mute, rx_info.tx2_mute);
            }
            LOG_D("app get record indicate,ch(%d),v(%d)", tx_mute.chn, tx_mute.state);
            break;

        case HL_RF_BYPASS_SETTING_IND:
            LOG_D("app get setting indicate");
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

        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}
#endif
/*
 * EOF
 */