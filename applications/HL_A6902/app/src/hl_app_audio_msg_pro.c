/**
 * @file hl_app_audio_msg_pro.c
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
#include "hl_app_mng.h"
#include "hl_app_audio_msg_pro.h"
#include "hl_app_pm_msg_pro.h"
#include "hl_mod_audio.h"
#include "hl_mod_display.h"
#include "hl_mod_upgrade.h"

#define DBG_SECTION_NAME "app_audio"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#if HL_IS_TX_DEVICE()

/// 大容量状态处理
static void hl_app_tx_mstorage_plug_pro(uint32_t value)
{
    hl_switch_e        record_switch;
    hl_led_switch record_led_ctrl;
    
    if (value == 0) {
        tx_info.mstorage_plug = 0;
        hl_mod_upgrade_io_ctrl(HL_UPGRADE_OPEN_CMD, NULL, 0);
    } else {
        record_switch    = HL_SWITCH_OFF;
        tx_info.mstorage_plug = 1;
        record_led_ctrl  = SWITCH_CLOSE;
        tx_info.rec_flag = 0;
        hl_mod_audio_io_ctrl(HL_AUDIO_RECORD_CMD, &record_switch, 1);
        hl_mod_display_io_ctrl(LED_RECORD_STATE_CMD, &record_led_ctrl, sizeof(record_led_ctrl));
        hl_app_pm_timer_set();
    }
}

void hl_app_audio_msg_pro(mode_to_app_msg_t *p_msg)
{
    hl_led_switch temp = SWITCH_CLOSE;
    switch (p_msg->cmd) {
        case HL_AUDIO_UAC_LINK_IND:
            tx_info.uac_link_flag = 1;
            hl_app_audio_stream_updata();
            LOG_I("app recv uac link indicate");
            break;
        case MSG_USB_MSTORAGE_DET:
            hl_app_tx_mstorage_plug_pro(p_msg->param.u32_param);
			LOG_D("MSG_USB_MSTORAGE_DET:(%d) \r\n", p_msg->param.u32_param);
            break;
        case RECORD_FAULT_MSG_ING:
            if (p_msg->param.u32_param == 1) {
                temp = SWITCH_OPEN;
            } else {
                temp = SWITCH_CLOSE;
            }
            hl_mod_display_io_ctrl(LED_RECORD_FAULT_CMD,&temp,1);
			LOG_D("RECORD_FAULT_MSG_ING:(%d) \r\n", p_msg->param.u32_param);
            break;
        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}

void hl_app_audio_switch(void)
{
    static uint8_t audio_mic_switch = 0;
    int32_t audio_gain = -128;

    LOG_I("hl_app_audio_switch ====(%d)(%d)(%d)", audio_mic_switch, tx_info.ex_mic_plug ,tx_info.uac_link_flag);

    if (tx_info.ex_mic_plug != 0) {
        if(audio_mic_switch == 1) {
            return;
        }
        audio_gain = 0;
        hl_mod_audio_io_ctrl(HL_AUDIO_SET_GAIN_CMD, &audio_gain, 4);
                           
        rt_thread_mdelay(200);
        hl_app_audio_gain(tx_info.gain);

        audio_mic_switch = 1;
    } else if(tx_info.uac_link_flag != 0) {
        if(audio_mic_switch == 2) {
            return;
        }
        audio_gain = -128;
        hl_mod_audio_io_ctrl(HL_AUDIO_SET_MIC_GAIN_CMD, &audio_gain, 4);

        hl_app_audio_gain_uac(tx_info.uac_gain);

        audio_mic_switch = 2;
    } else {
        if(audio_mic_switch == 3) {
            return;
        }
        audio_gain = -128;
        hl_mod_audio_io_ctrl(HL_AUDIO_SET_MIC_GAIN_CMD, &audio_gain, 4);

        hl_app_audio_gain(tx_info.gain);

        audio_mic_switch = 3;
    }    
}

void hl_app_audio_gain(int32_t gain)
{
    tx_info.gain = gain;

    if(tx_info.ex_mic_plug == 1) {          
        if (gain > 0) {
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_MIC_PGA_GAIN_CMD, &gain, 4);
            gain = 0;
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_MIC_GAIN_CMD, &gain, 4);            
        } else if (gain < 0) {
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_MIC_GAIN_CMD, &gain, 4);
            gain = 0;
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_MIC_PGA_GAIN_CMD, &gain, 4);
        } else {
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_MIC_GAIN_CMD, &gain, 4);
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_MIC_PGA_GAIN_CMD, &gain, 4);
        }
    } else {
        gain += 10;
        hl_mod_audio_io_ctrl(HL_AUDIO_SET_GAIN_CMD, &gain, 4);
    }  
}


void hl_app_audio_gain_uac(int32_t uac_gain)
{
    tx_info.uac_gain = uac_gain;

    if((tx_info.uac_link_flag == 1)&&(tx_info.ex_mic_plug == 0)) {       
        hl_mod_audio_io_ctrl(HL_AUDIO_SET_GAIN_CMD, &uac_gain, 4);
    } 
}
#else
/// 大容量状态处理
static void hl_app_rx_mstorage_plug_pro(uint32_t value)
{
    hl_switch_e        record_switch;
    
    if (value == 0) {
        rx_info.mstorage_plug = 0;
        hl_mod_upgrade_io_ctrl(HL_UPGRADE_OPEN_CMD, NULL, 0);
    } else {
        record_switch    = HL_SWITCH_OFF;
        rx_info.mstorage_plug = 1;
    }
}

void hl_app_audio_msg_pro(mode_to_app_msg_t *p_msg)
{
    uint8_t u8temp;

    switch (p_msg->cmd) {
        case HL_AUDIO_UAC_LINK_IND:
            rx_info.uac_link_flag = 1;
            hl_app_audio_stream_updata();
            LOG_I("app recv uac link indicate");
            break;
        case MSG_USB_MSTORAGE_DET:
            hl_app_rx_mstorage_plug_pro(p_msg->param.u32_param);
			LOG_D("MSG_USB_MSTORAGE_DET:(%d)", p_msg->param.u32_param);
            break;

        case HL_AUDIO_L_VU_VAL:
            u8temp = (uint8_t)(p_msg->param.u32_param);
            hl_mod_display_io_ctrl(TX1_VU_VAL_CMD, &u8temp, 1);
            break;
        
        case HL_AUDIO_R_VU_VAL:
            u8temp = (uint8_t)(p_msg->param.u32_param);
            hl_mod_display_io_ctrl(TX2_VU_VAL_CMD, &u8temp, 1);
            break;

        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}

void hl_app_audio_switch(void)
{
    static uint8_t audio_cam_switch = 1;
    static uint8_t audio_hp_switch = 1;
    int32_t audio_gain = -128;

    // 设置cam的开关
    if(audio_cam_switch == 1) {
        if((rx_info.on_off_flag == 0)||(rx_info.cam_spk_plug == 0)||((rx_info.rf_state != HL_RF_L_CONNECT)&&(rx_info.rf_state != HL_RF_R_CONNECT)&&(rx_info.rf_state != HL_RF_LR_CONNECT))) {
            audio_gain = -128;
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_CAM_GAIN_L_CMD, &audio_gain, 4);
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_CAM_GAIN_R_CMD, &audio_gain, 4);
            audio_cam_switch = 0;
        }
    } else {
        if((rx_info.on_off_flag == 1)&&(rx_info.cam_spk_plug == 1)&&((rx_info.rf_state == HL_RF_L_CONNECT)||(rx_info.rf_state == HL_RF_R_CONNECT)||(rx_info.rf_state == HL_RF_LR_CONNECT))) {
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_CAM_GAIN_L_CMD, &rx_info.cam_gain_l, 4);
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_CAM_GAIN_R_CMD, &rx_info.cam_gain_r, 4);
            audio_cam_switch = 1;
        }
    }

    // 设置hp的开关
    if(audio_hp_switch == 1) {
        if((rx_info.on_off_flag == 0)||(rx_info.hp_spk_plug == 0)||((rx_info.rf_state != HL_RF_L_CONNECT)&&(rx_info.rf_state != HL_RF_R_CONNECT)&&(rx_info.rf_state != HL_RF_LR_CONNECT))) {
            audio_gain = -128;
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_HP_GAIN_L_CMD, &audio_gain, 4);
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_HP_GAIN_R_CMD, &audio_gain, 4);
            audio_hp_switch = 0;
        }
    } else {
        if((rx_info.on_off_flag == 1)&&(rx_info.hp_spk_plug == 1)&&((rx_info.rf_state == HL_RF_L_CONNECT)||(rx_info.rf_state == HL_RF_R_CONNECT)||(rx_info.rf_state == HL_RF_LR_CONNECT))) {
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_HP_GAIN_L_CMD, &rx_info.hp_gain, 4);
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_HP_GAIN_R_CMD, &rx_info.hp_gain, 4);
            audio_hp_switch = 1;
        }
    }    
}
#endif

void hl_app_audio_stream_updata(void)
{
    hl_stream_mode_e stream_mode = HL_STREAM_IDLE;

#if HL_IS_TX_DEVICE()
    if (tx_info.ex_mic_plug != 0) {
        stream_mode = HL_STREAM_CAP2PLAY;
        hl_mod_audio_io_ctrl(HL_AUDIO_STREAM_SET_CMD, &stream_mode, sizeof(hl_stream_mode_e));        
    } else if(tx_info.uac_link_flag != 0) {
        stream_mode = HL_STREAM_UAC2PLAY;
        hl_mod_audio_io_ctrl(HL_AUDIO_STREAM_SET_CMD, &stream_mode, sizeof(hl_stream_mode_e)); 
    } else {
        stream_mode = HL_STREAM_PDM2PLAY;
        hl_mod_audio_io_ctrl(HL_AUDIO_STREAM_SET_CMD, &stream_mode, sizeof(hl_stream_mode_e)); 
    }

    hl_app_audio_switch();
#else
    // if (rx_info.uac_link_flag != 0) {
    //     stream_mode = HL_STREAM_CAP2UAC_UAC2PLAY;
    //     // stream_mode = HL_STREAM_CAP2PLAY_CAP2UAC;
    //     hl_mod_audio_io_ctrl(HL_AUDIO_STREAM_SET_CMD, &stream_mode, sizeof(hl_stream_mode_e)); 
    // } else {
    //     stream_mode = HL_STREAM_CAP2PLAY;
    //     hl_mod_audio_io_ctrl(HL_AUDIO_STREAM_SET_CMD, &stream_mode, sizeof(hl_stream_mode_e));         
    // }
#endif
}

/*
 * EOF
 */