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
    uint8_t       p_param;
    uint8_t*      ptr;
    hl_rf_state_e rf_state;
    hl_switch_e        mute_switch;

    // LOG_D("hl_app_rf_msg_pro get telink msg(%d)!!! \r\n", p_msg->cmd);
    switch (p_msg->cmd) {
        case HL_RF_VERSION_IND:
            ptr = (uint8_t*)p_msg->param.ptr;
            LOG_D("\n\n--- Telink Version[%d.%d.%d.%d] ---\n\n", ptr[0], ptr[1], ptr[2], ptr[3]);
            break;
        case HL_RF_PAIR_STATE_IND:
            tx_info.rf_state = *(hl_rf_state_e*)p_msg->param.ptr;
            rf_state         = tx_info.rf_state;
            LOG_D("telink info(%02X)", tx_info.rf_state);
            // hl_mod_display_io_ctrl(TX_RF_STATE_VAL_CMD, &rf_state, sizeof(rf_state));
            hl_app_disp_state_led_set();
            break;
        case HL_RF_RSSI_IND:
            p_param  = *(uint8_t*)p_msg->param.ptr;
            // LOG_D("\ntelink RSSI(%02X)\r\n", p_param);
            break;
        case HL_RF_BYPASS_MUTE_IND:   
            mute_switch = *(hl_switch_e*)p_msg->param.ptr;                     
            if (tx_info.mute_flag == mute_switch) {
                LOG_D("telink mute error (%02X)\r\n", mute_switch);  
                break;
            }
            tx_info.mute_flag = mute_switch;
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_MUTE_CMD, &mute_switch, 1);   
            hl_app_disp_state_led_set(); 
            // LOG_D("telink mute(%02X)\r\n", mute_switch);           
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
    uint8_t            tx1_rssi;
    uint8_t            tx2_rssi;
    uint8_t            *ptr;
    hl_rf_state_e      rf_state;
    hl_rf_bypass_value_t *ptr_rf_value;
    hl_rf_bypass_state_t tx_mute;

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
            tx1_rssi  = ((uint8_t*)p_msg->param.ptr)[0];
            tx2_rssi  = ((uint8_t*)p_msg->param.ptr)[1];
            // LOG_D("telink RSSI(%02X -- %02X)", tx1_rssi, tx2_rssi);
            hl_mod_display_io_ctrl(TX1_SIGNAL_VAL_CMD, &tx1_rssi, 1);
            hl_mod_display_io_ctrl(TX2_SIGNAL_VAL_CMD, &tx2_rssi, 1);
            break;

        case HL_RF_BYPASS_MUTE_IND:
            tx_mute = *(hl_rf_bypass_state_t*)p_msg->param.ptr;
            if(tx_mute.chn == HL_RF_LEFT_CHANNEL) {
                rx_info.tx1_mute = tx_mute.state;
            } else if(tx_mute.chn == HL_RF_RIGHT_CHANNEL) {
                rx_info.tx2_mute = tx_mute.state;
            } else {
                LOG_E("telink mute receive error(%02X -- %02X)", rx_info.tx1_mute, rx_info.tx2_mute);
            }
            
            break;

        case HL_RF_BYPASS_DENOISE_IND:
            LOG_D("app get denoise indicate");
            break;

        case HL_RF_BYPASS_VOLUME_IND:
            LOG_D("app get volume indicate");
            break;

        case HL_RF_BYPASS_RECORD_IND:
            hl_mod_euc_ctrl(HL_HID_START_RECORD_CMD, RT_NULL, 0);
            LOG_D("app get record indicate");
            break;

        case HL_RF_BYPASS_SETTING_IND:
            LOG_D("app get setting indicate");
            break;
            
        case HL_RF_BYPASS_BATTERY_IND:
            ptr_rf_value = (hl_rf_bypass_value_t*)p_msg->param.ptr;
            LOG_D("app get TX%d battery(%d)", ptr_rf_value->chn, ptr_rf_value->val);
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