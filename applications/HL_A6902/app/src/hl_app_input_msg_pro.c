/**
 * @file hl_app_input_msg_pro.c
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
#include "hl_mod_input.h"
#include "hl_mod_display.h"
#include "hl_mod_audio.h"
#include "hl_mod_telink.h"
#include "hl_mod_pm.h"

#define DBG_SECTION_NAME "app_input"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

/* typedef -------------------------------------------------------------------*/
typedef enum _hl_knob_dir_e
{
    HL_KNOB_L = 0x00,
    HL_KNOB_R,
} HL_ENUM8(hl_knob_dir_e);

/* define --------------------------------------------------------------------*/
#define VOLUME_MAX       24
#define VOLUME_MIN       -104

/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
#if HL_GET_DEVICE_TYPE()
/// 电源键处理
static void hl_app_tx_pwr_key_pro(hl_key_event_e event)
{
    HL_PMIC_INPUT_PARAM_T pm_crl;

    switch (event) {
        case HL_KEY_EVENT_PRESS:
            break;
        case HL_KEY_EVENT_SHORT:
            break;
        case HL_KEY_EVENT_LONG:
            pm_crl.param = HL_MOD_RK2108_POWER_DOWN_CMD;
            hl_mod_pm_ctrl(1, &pm_crl, 1);
            hl_mod_input_deinit();
            hl_mod_display_deinit();
            hl_mod_audio_deinit();
            hl_mod_telink_stop();
            hl_mod_telink_deinit();
            //hl_mod_pm_stop();
            //hl_mod_pm_deinit();
            break;
        case HL_KEY_EVENT_DOUBLE:
            break;
        case HL_KEY_EVENT_RELEASE:
            break;
        default:
            LOG_E("event(%d) unkown!!! \r\n", event);
            break;
    }
}

/// 配对键处理
static void hl_app_tx_pair_key_pro(hl_key_event_e event)
{
    uint8_t denoise_ctrl;

    switch (event) {
        case HL_KEY_EVENT_PRESS:
            break;
        case HL_KEY_EVENT_SHORT:
            if (tx_info.denoise_flag == 0) {
                denoise_ctrl         = 1;
                tx_info.denoise_flag = 1;
            } else {
                denoise_ctrl         = 0;
                tx_info.denoise_flag = 0;
            }
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_DENOISE_CMD, &denoise_ctrl, 1);
            break;
        case HL_KEY_EVENT_LONG:
            break;
        case HL_KEY_EVENT_DOUBLE:
            break;
        case HL_KEY_EVENT_RELEASE:
            break;
        default:
            LOG_E("event(%d) unkown!!! \r\n", event);
            break;
    }
}

/// 录制键处理
static void hl_app_tx_rec_key_pro(hl_key_event_e event)
{
    uint8_t record_ctrl;

    switch (event) {
        case HL_KEY_EVENT_PRESS:
            break;
        case HL_KEY_EVENT_SHORT:
            if (tx_info.rec_flag == 0) {
                record_ctrl      = 1;
                tx_info.rec_flag = 1;
            } else {
                record_ctrl      = 0;
                tx_info.rec_flag = 0;
            }
            hl_mod_audio_io_ctrl(HL_AUDIO_RECORD_CMD, &record_ctrl, 1);
            break;
        case HL_KEY_EVENT_LONG:
            break;
        case HL_KEY_EVENT_DOUBLE:
            break;
        case HL_KEY_EVENT_RELEASE:
            break;
        default:
            LOG_E("event(%d) unkown!!! \r\n", event);
            break;
    }
}

/// usb连接状态处理
static void hl_app_tx_usb_plug_pro(uint32_t value)
{
    if (value == 0) {
        tx_info.usb_plug = 0;
    } else {
        tx_info.usb_plug = 1;
    }
}

/// 外置mic状态处理
static void hl_app_tx_ex_mic_plug_pro(uint32_t value)
{
    uint8_t mic_select;
    
    if (value == 0) {
        tx_info.ex_mic_plug = 0;
        mic_select          = 0;
    } else {
        tx_info.ex_mic_plug = 1;
        mic_select          = 1;
    }
    hl_mod_audio_io_ctrl(HL_AUDIO_MIC_SWITCH_CMD, &mic_select, 1);
}

#else
/// 电源键处理
static void hl_app_rx_pwr_key_pro(hl_key_event_e event)
{
    HL_PMIC_INPUT_PARAM_T pm_crl;

    switch (event) {
        case HL_KEY_EVENT_PRESS:
            break;
        case HL_KEY_EVENT_SHORT:
            break;
        case HL_KEY_EVENT_LONG:
            pm_crl.param = HL_MOD_RK2108_POWER_DOWN_CMD;
            hl_mod_pm_ctrl(1, &pm_crl, 1);
            hl_mod_input_deinit();
            hl_mod_display_deinit();
            hl_mod_audio_deinit();
            hl_mod_telink_stop();
            hl_mod_telink_deinit();
            //hl_mod_pm_stop();
            //hl_mod_pm_deinit();
            break;
        case HL_KEY_EVENT_DOUBLE:
            break;
        case HL_KEY_EVENT_RELEASE:
            break;
        default:
            LOG_E("event(%d) unkown!!! \r\n", event);
            break;
    }
}

/// 旋钮中间按键处理
static void hl_app_rx_knob_key_pro(hl_key_event_e event)
{
    static hl_screen_color_e screen_color_ctrl = RGB888_BLACK;

    switch (event) {
        case HL_KEY_EVENT_PRESS:
            break;
        case HL_KEY_EVENT_SHORT:
            screen_color_ctrl++;
            screen_color_ctrl %= RGB888_COLOR_CNT;
            hl_mod_display_io_ctrl(MSG_OLED_COLOR_CHANGE_CMD, &screen_color_ctrl, sizeof(screen_color_ctrl));
            break;
        case HL_KEY_EVENT_LONG:
            break;
        case HL_KEY_EVENT_DOUBLE:
            break;
        case HL_KEY_EVENT_RELEASE:
            break;
        default:
            LOG_E("event(%d) unkown!!! \r\n", event);
            break;
    }
}

/// 旋钮转动处理
static void hl_app_rx_knob_pro(hl_knob_dir_e dir, uint32_t value)
{
    if (dir == HL_KNOB_L) {
        rx_info.cur_volume_l += value;
        rx_info.cur_volume_r += value;
        if (VOLUME_MAX < rx_info.cur_volume_l) {
            rx_info.cur_volume_l = VOLUME_MAX;
        }
        if (VOLUME_MAX < rx_info.cur_volume_r) {
            rx_info.cur_volume_r = VOLUME_MAX;
        }
    } else {
        rx_info.cur_volume_l -= value;
        rx_info.cur_volume_r -= value;
        if (VOLUME_MIN > rx_info.cur_volume_l) {
            rx_info.cur_volume_l = VOLUME_MIN;
        }
        if (VOLUME_MIN > rx_info.cur_volume_r) {
            rx_info.cur_volume_r = VOLUME_MIN;
        }
    }
    
    hl_mod_audio_io_ctrl(HL_AUDIO_SET_GAIN_CMD, &rx_info.cur_volume_r, 2);
    LOG_D("volume:(%d)\r\n", rx_info.cur_volume_r);
}

/// usb连接状态处理
static void hl_app_rx_usb_plug_pro(uint32_t value)
{
    if (value == 0) {
        rx_info.usb_plug = 0;
    } else {
        rx_info.usb_plug = 1;
    }
}

/// 监听口状态处理
static void hl_app_rx_hp_plug_pro(uint32_t value)
{
    uint8_t hp_amp_ctrl;

    if (value == 0) {
        rx_info.hp_spk_plug = 0;
        hp_amp_ctrl         = 0;
    } else {
        rx_info.hp_spk_plug = 1;
        hp_amp_ctrl         = 1;
    }
    hl_mod_audio_io_ctrl(HL_AUDIO_SET_HP_AMP_CMD, &hp_amp_ctrl, 1);
}

/// 相机口状态处理
static void hl_app_rx_cam_plug_pro(uint32_t value)
{
    if (value == 0) {
        rx_info.cam_spk_plug = 0;
    } else {
        rx_info.cam_spk_plug = 1;
    }
}

#endif

/// 
/* Exported functions --------------------------------------------------------*/
#if HL_GET_DEVICE_TYPE()
void hl_app_input_msg_pro(mode_to_app_msg_t *p_msg)
{
    switch (p_msg->cmd) {
        case MSG_TX_PWR_KEY:
            hl_app_tx_pwr_key_pro(p_msg->param.u32_param);
            LOG_D("MSG_TX_PWR_KEY:(%d) \r\n", p_msg->param.u32_param);
            break;
        case MSG_TX_PAIR_KEY:
            hl_app_tx_pair_key_pro(p_msg->param.u32_param);
            LOG_D("MSG_TX_PAIR_KEY:(%d) \r\n", p_msg->param.u32_param);
            break;
        case MSG_TX_REC_KEY:
            hl_app_tx_rec_key_pro(p_msg->param.u32_param);
			LOG_D("MSG_TX_REC_KEY:(%d) \r\n", p_msg->param.u32_param);
            break;
        case MSG_TX_VBUS_DET:
            /// usb连接状态处理
            hl_app_tx_usb_plug_pro(p_msg->param.u32_param);
			LOG_D("MSG_TX_VBUS_DET:(%d) \r\n", p_msg->param.u32_param);
            break;
        case MSG_TX_MIC_DET:
            hl_app_tx_ex_mic_plug_pro(p_msg->param.u32_param);
			LOG_D("MSG_TX_MIC_DET:(%d) \r\n", p_msg->param.u32_param);
            break;
        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}
#else
void hl_app_input_msg_pro(mode_to_app_msg_t *p_msg)
{
    switch (p_msg->cmd) {
        case MSG_RX_PWR_KEY:
            hl_app_rx_pwr_key_pro(p_msg->param.u32_param);
            LOG_D("MSG_RX_PWR_KEY:(%d) \r\n", p_msg->param.u32_param);
            break;

        case MSG_RX_OK_VOL:
            hl_app_rx_knob_key_pro(p_msg->param.u32_param);
            LOG_D("MSG_RX_OK_VOL:(%d) \r\n", p_msg->param.u32_param);
            break;

        case MSG_RX_A_VOL:
            hl_app_rx_knob_pro(HL_KNOB_L, p_msg->param.u32_param);
            LOG_D("MSG_RX_L_VOL_KEY:(%d) \r\n", p_msg->param.u32_param);
            break;

        case MSG_RX_B_VOL:
            hl_app_rx_knob_pro(HL_KNOB_R, p_msg->param.u32_param);
            LOG_D("MSG_RX_R_VOL_KEY:(%d) \r\n", p_msg->param.u32_param);
            break;

        case MSG_RX_VBUS_DET:
            hl_app_rx_usb_plug_pro(p_msg->param.u32_param);
            LOG_D("MSG_RX_VBUS_DET:(%d) \r\n", p_msg->param.u32_param);
            break;

        case MSG_RX_HP_DET:
            hl_app_rx_hp_plug_pro(p_msg->param.u32_param);
            LOG_D("MSG_RX_HP_DET:(%d) \r\n", p_msg->param.u32_param);
            break;

        case MSG_RX_CAM_DET:
            hl_app_rx_cam_plug_pro(p_msg->param.u32_param);
            LOG_D("MSG_RX_CAM_DET:(%d) \r\n", p_msg->param.u32_param);
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