/**
 * @file hl_app_larksound_msg_pro.c
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * @brief 接收LarkSound和MomaSound APP应用消息处理
 * @version 1.0
 * @date 2023-03-14
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
 * <tr><td>2023-03-14     <td>v1.0     <td>lisonglin     <td>内容
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
#include "hl_util_hap.h"

#define DBG_SECTION_NAME "app_larksound"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/

static uint8_t data_buffer[256] = { 0 };

/* Private function(only *.c)  -----------------------------------------------*/

static int _hl_app_larksound_encode(uint8_t cmd, uint8_t ctrl, uint8_t* data, uint16_t data_size)
{
    if (data_size + 7 > 256) {
        return 0;
    }

    int size = hl_util_hap_encode(EM_HAP_ROLE_SLAVE, cmd, ctrl, data_buffer, sizeof(data_buffer), data, data_size);
    if (size == -1) {
        return 0;
    }

    // android
    hl_mod_euc_ctrl(HL_LARKSOUND_CMD, data_buffer, size);

    return size;
}

/* Exported functions --------------------------------------------------------*/

void hl_app_larksound_msg_pro(mode_to_app_msg_t* p_msg)
{
#if !HL_IS_TX_DEVICE()
    hl_rf_bypass_value_t rf_bypass_value = { 0 };
    hl_rf_bypass_state_t rf_bypass_state = { 0 };
    uint8_t              mute            = 0;
    uint8_t              temp_ret        = 0;
    uint8_t*             ptr             = NULL;
    uint8_t              temp_arr[2]     = { 0 };
    uint8_t              temp_ver[4]     = { 0 };
    uint8_t              temp_devid      = 1;
    uint16_t             temp_uv         = 66;
    uint8_t              temp_txgain[2]  = { 6, 6 };
    uint8_t              ctrl            = 0;
    int                  len             = 0;
    uint8_t              keep_alive[7]   = { 0 };

    temp_ver[0] = 1;
    temp_ver[1] = 2;
    temp_ver[2] = 3;
    temp_ver[3] = 4;

    ctrl = p_msg->len;

    switch (p_msg->cmd) {
        case LARKSOUND_GET_SOFT_VERSION_CMD: {
            if (ctrl & HAP_CTRL_RX) {
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, temp_ver, 4);
                LOG_I("app get(%d) larksound get RX soft version\n", p_msg->cmd);
            }
            if (ctrl & HAP_CTRL_TX1) {
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_TX1, temp_ver, 4);
                LOG_I("app get(%d) larksound get TX1 soft version\n", p_msg->cmd);
            }
            if (ctrl & HAP_CTRL_TX2) {
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_TX2, temp_ver, 4);
                LOG_I("app get(%d) larksound get TX2 soft version\n", p_msg->cmd);
            }
            break;
        }

        case LARKSOUND_GET_HARD_VERSION_CMD: {
            if (ctrl & HAP_CTRL_RX) {
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, temp_ver, 4);
                LOG_I("app get(%d) larksound get RX hard version\n", p_msg->cmd);
            }
            if (ctrl & HAP_CTRL_TX1) {
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_TX1, temp_ver, 4);
                LOG_I("app get(%d) larksound get TX1 hard version\n", p_msg->cmd);
            }
            if (ctrl & HAP_CTRL_TX2) {
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_TX2, temp_ver, 4);
                LOG_I("app get(%d) larksound get TX2 hard version\n", p_msg->cmd);
            }
            break;
        }

        case LARKSOUND_GET_CONTECT_STATE_CMD:
            LOG_I("app get(%d) larksound get RX connect state\n", p_msg->cmd);
            _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, &rx_info.rf_state, 1);
            break;

        case LARKSOUND_GET_MAC_CMD: {
            if (ctrl & HAP_CTRL_RX) {
                LOG_I("app get(%d) larksound get RX MAC\n", p_msg->cmd);
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, rx_info.local_mac, 6);
            }
            if (ctrl & HAP_CTRL_TX1) {
                LOG_I("app get(%d) larksound get TX1 MAC\n", p_msg->cmd);
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_TX1, &rx_info.remote_mac[0], 6);
            }
            if (ctrl & HAP_CTRL_TX2) {
                LOG_I("app get(%d) larksound get TX2 MAC\n", p_msg->cmd);
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_TX2, &rx_info.remote_mac[6], 6);
            }
            break;
        }

        case LARKSOUND_GET_SOC_CMD: {
            if (ctrl & HAP_CTRL_RX) {
                LOG_I("app get(%d) larksound get RX Soc\n", p_msg->cmd);
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, &rx_info.soc, 1);
            }
            if (ctrl & HAP_CTRL_TX1) {
                LOG_I("app get(%d) larksound get TX1 Soc\n", p_msg->cmd);
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_TX1, &rx_info.tx1_soc, 1);
            }
            if (ctrl & HAP_CTRL_TX2) {
                LOG_I("app get(%d) larksound get TX2 Soc\n", p_msg->cmd);
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_TX2, &rx_info.tx2_soc, 1);
            }
            break;
        }

        case LARKSOUND_GET_UV_CMD:
            LOG_I("app get(%d) larksound get UV\n", p_msg->cmd);
            _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, &temp_uv, 2);
            break;

        case LARKSOUND_GET_DEV_NUB_CMD:
            _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, &temp_devid, 1);
            LOG_I("app get(%d) larksound get Device Number\n", p_msg->cmd);
            break;

        case LARKSOUND_GET_TX_GAIN_CMD:
            LOG_I("app get(%d) larksound get TX Gain\n", p_msg->cmd);
            _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, temp_txgain, 2);
            break;

        case LARKSOUND_SET_TX_GAIN_CMD:
            LOG_I("app get(%d) larksound set TX Gain(%08X)\n", p_msg->cmd, p_msg->param.u32_param);
            temp_ret = 1;
            _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, &temp_ret, 1);
            break;

        case LARKSOUND_GET_DENOISE_CMD:
            if (ctrl & HAP_CTRL_TX1) {
                LOG_I("app get(%d) larksound get TX1 Denoise\n", p_msg->cmd);
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_TX1, &rx_info.tx1_denoise, 1);
            }
            if (ctrl & HAP_CTRL_TX2) {
                LOG_I("app get(%d) larksound get TX2 Denoise\n", p_msg->cmd);
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_TX2, &rx_info.tx2_denoise, 1);
            }
            break;

        case LARKSOUND_SET_DENOISE_CMD:
            temp_ret            = (uint8_t)p_msg->param.u32_param;
            rf_bypass_value.val = temp_ret;
            if (rf_bypass_value.val) {
                rf_bypass_value.val = 0xFF;
            } else {
                rf_bypass_value.val = 0x00;
            }
            temp_devid = 1;
            if (ctrl & HAP_CTRL_TX1) {
                rf_bypass_value.chn = HL_RF_LEFT_CHANNEL;
                hl_mod_telink_ioctl(HL_RF_BYPASS_DENOISE_CMD, &rf_bypass_value, sizeof(rf_bypass_value));
                rx_info.tx1_denoise = temp_ret;
                LOG_I("app get(%d) larksound set TX1 Denoise[%d]\n", p_msg->cmd, temp_ret);
                hl_mod_display_io_ctrl(TX1_NOISE_SWITCH_CMD, &temp_ret, 1);
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_TX1, &temp_devid, 1);
            }
            if (ctrl & HAP_CTRL_TX2) {
                rf_bypass_value.chn = HL_RF_RIGHT_CHANNEL;
                hl_mod_telink_ioctl(HL_RF_BYPASS_DENOISE_CMD, &rf_bypass_value, sizeof(rf_bypass_value));
                rx_info.tx2_denoise = temp_ret;
                LOG_I("app get(%d) larksound set TX2 Denoise[%d]\n", p_msg->cmd, temp_ret);
                hl_mod_display_io_ctrl(TX2_NOISE_SWITCH_CMD, &temp_ret, 1);
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_TX2, &temp_devid, 1);
            }
            break;

        case LARKSOUND_GET_LOWCUT_CMD:
            if (ctrl & HAP_CTRL_RX) {
                LOG_I("app get(%d) larksound get Lowcut\n", p_msg->cmd);
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, &rx_info.lowcut, 1);
            }
            break;

        case LARKSOUND_SET_LOWCUT_CMD:
            temp_ret = (uint8_t)p_msg->param.u32_param;
            if (ctrl & HAP_CTRL_RX) {
                rf_bypass_state.chn   = HL_RF_DOUBLE_CHANNEL;
                rf_bypass_state.state = temp_ret;
                hl_mod_telink_ioctl(HL_RF_BYPASS_LOWCUT_CMD, &rf_bypass_state, sizeof(rf_bypass_state));
                LOG_I("app get(%d) larksound set Lowcut\n", p_msg->cmd);
                rx_info.lowcut = temp_ret;
                temp_ret       = 1;
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, &temp_ret, 1);
            }
            break;

        case LARKSOUND_GET_MUTE_CMD:
            if (ctrl & HAP_CTRL_TX1) {
                LOG_I("app get(%d) larksound get TX1 Mute\n", p_msg->cmd);
                mute = rx_info.tx1_mute;
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_TX1, &mute, 1);
            }
            if (ctrl & HAP_CTRL_TX2) {
                LOG_I("app get(%d) larksound get TX2 Mute\n", p_msg->cmd);
                mute = rx_info.tx2_mute;
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_TX2, &mute, 1);
            }
            break;

        case LARKSOUND_SET_MUTE_CMD:
            temp_ret              = (uint8_t)p_msg->param.u32_param;
            rf_bypass_state.state = temp_ret;
            if (ctrl & HAP_CTRL_TX1) {
                rf_bypass_state.chn = HL_RF_LEFT_CHANNEL;
                hl_mod_telink_ioctl(HL_RF_BYPASS_MUTE_CMD, &rf_bypass_state, sizeof(rf_bypass_state));
                LOG_I("app get(%d) larksound set TX1 Mute[%d]\n", p_msg->cmd, temp_ret);
                hl_mod_display_io_ctrl(TX1_MUTE_SWITCH_SWITCH_CMD, &rf_bypass_state.state, 1);
                temp_ret = 1;
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_TX1, &temp_ret, 1);
            }
            if (ctrl & HAP_CTRL_TX2) {
                rf_bypass_state.chn = HL_RF_RIGHT_CHANNEL;
                hl_mod_telink_ioctl(HL_RF_BYPASS_MUTE_CMD, &rf_bypass_state, sizeof(rf_bypass_state));
                LOG_I("app get(%d) larksound set TX2 Mute[%d]\n", p_msg->cmd, temp_ret);
                hl_mod_display_io_ctrl(TX2_MUTE_SWITCH_SWITCH_CMD, &rf_bypass_state.state, 1);
                temp_ret = 1;
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_TX2, &temp_ret, 1);
            }
            break;

        case LARKSOUND_GET_SOUND_EFFECT_CMD:
            if (ctrl & HAP_CTRL_RX) {
                LOG_I("app get(%d) larksound get Sound Effect\n", p_msg->cmd);
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, &rx_info.sound_effect, 1);
            }
            break;

        case LARKSOUND_SET_SOUND_EFFECT_CMD:
            temp_ret = (uint8_t)p_msg->param.u32_param;
            if (ctrl & HAP_CTRL_RX) {
                rf_bypass_value.chn = HL_RF_DOUBLE_CHANNEL;
                rf_bypass_value.val = temp_ret;
                hl_mod_telink_ioctl(HL_RF_BYPASS_SOUND_EFFECT_CMD, &rf_bypass_value, sizeof(rf_bypass_value));
                LOG_I("app get(%d) larksound set Sound Effect[%d]\n", p_msg->cmd, temp_ret);
            }
            break;

        case LARKSOUND_GET_PHONE_OUT_CMD:
            if (ctrl & HAP_CTRL_RX) {
                temp_ret = (uint8_t)rx_info.phone_out;
                LOG_I("app get(%d) larksound get Phone Out\n", p_msg->cmd);
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, &temp_ret, 1);
            }
            break;

        case LARKSOUND_SET_PHONE_OUT_CMD:
            rx_info.phone_out = (uint8_t)p_msg->param.u32_param;
            if (ctrl & HAP_CTRL_RX) {
                LOG_I("app get(%d) larksound set Phone Out[%d]\n", p_msg->cmd, rx_info.phone_out);
            }
            break;

        case LARKSOUND_GET_UAC_GAIN_CMD:
            if (ctrl & HAP_CTRL_RX) {
                temp_ret = (uint8_t)rx_info.uac_gain;
                LOG_I("app get(%d) larksound get UAC Gain\n", p_msg->cmd);
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, &temp_ret, 1);
            }
            break;

        case LARKSOUND_SET_UAC_GAIN_CMD:
            rx_info.uac_gain = (int32_t)p_msg->param.u32_param;
            if (ctrl & HAP_CTRL_RX) {
                LOG_I("app get(%d) larksound set UAC Gain[%d]\n", p_msg->cmd, rx_info.uac_gain);
                hl_mod_audio_io_ctrl(HL_AUDIO_SET_GAIN_UAC_CMD, &rx_info.uac_gain, 4);
                hl_util_nvram_param_set_integer("RX_UAC_GAIN", rx_info.uac_gain);
                temp_ret = 1;
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, temp_ret, 1);
            }
            break;

        case LARKSOUND_GET_RX_GAIN_CMD:
            if (ctrl & HAP_CTRL_RX) {
                temp_ret = (uint8_t)rx_info.hp_gain;
                LOG_I("app get(%d) larksound get Hp Gain\n", p_msg->cmd);
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, &temp_ret, 1);
            }
            break;

        case LARKSOUND_SET_RX_GAIN_CMD:
            rx_info.hp_gain = (int32_t)p_msg->param.u32_param;
            if (ctrl & HAP_CTRL_RX) {
                LOG_I("app get(%d) larksound set Hp Gain[%d]\n", p_msg->cmd, rx_info.hp_gain);
                hl_mod_audio_io_ctrl(HL_AUDIO_SET_HP_GAIN_L_CMD, &rx_info.hp_gain, 4);
                hl_mod_audio_io_ctrl(HL_AUDIO_SET_HP_GAIN_R_CMD, &rx_info.hp_gain, 4);
                hl_util_nvram_param_set_integer("RX_HP_L_GAIN", rx_info.hp_gain);
                hl_util_nvram_param_set_integer("RX_HP_R_GAIN", rx_info.hp_gain);
                temp_ret = 1;
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, temp_ret, 1);
            }
            break;

        case LARKSOUND_GET_CAM_GAIN_CMD:
            if (ctrl & HAP_CTRL_RX) {
                temp_arr[0] = (uint8_t)rx_info.cam_gain_l;
                temp_arr[1] = (uint8_t)rx_info.cam_gain_r;
                LOG_I("app get(%d) larksound get Cam Gain\n", p_msg->cmd);
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, temp_arr, sizeof(temp_arr));
            }
            break;

        case LARKSOUND_SET_CAM_GAIN_CMD:
            ptr = (uint8_t*)&p_msg->param.u32_param;
            if (ctrl & HAP_CTRL_RX) {
                LOG_I("app get(%d) larksound set Cam Gain[%d][%d]\n", p_msg->cmd, ptr[0], ptr[1]);
                rx_info.cam_gain_l = (uint32_t)ptr[0];
                rx_info.cam_gain_r = (uint32_t)ptr[1];
                hl_mod_audio_io_ctrl(HL_AUDIO_SET_CAM_GAIN_L_CMD, &rx_info.cam_gain_l, 4);
                hl_mod_audio_io_ctrl(HL_AUDIO_SET_CAM_GAIN_R_CMD, &rx_info.cam_gain_r, 4);
                hl_util_nvram_param_set_integer("RX_CAM_L_GAIN", rx_info.cam_gain_l);
                hl_util_nvram_param_set_integer("RX_CAM_R_GAIN", rx_info.cam_gain_r);
                temp_ret = 1;
                _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, temp_ret, 1);
            }
            break;

        case LARKSOUND_UPDATE_START_CMD:
            LOG_I("app get(%d) larksound Update Start\n", p_msg->cmd);
            break;

        case LARKSOUND_UPDATE_PACKET_CMD:
            LOG_I("app get(%d) larksound Update Packet\n", p_msg->cmd);
            break;

        case LARKSOUND_UPDATE_STOP_CMD:
            LOG_I("app get(%d) larksound Update Stop\n", p_msg->cmd);
            break;

        case LARKSOUND_KEEP_ALIVE_CMD:
            temp_ret      = (uint8_t)rx_info.charge_flag;
            keep_alive[0] = rx_info.rf_state;
            keep_alive[1] = rx_info.tx1_soc;
            keep_alive[2] = rx_info.tx2_soc;
            keep_alive[3] = rx_info.soc;
            keep_alive[4] = rx_info.tx1_charge & 0x03;
            keep_alive[4] |= (rx_info.tx2_charge & 0x03) << 2;
            keep_alive[4] |= (temp_ret & 0x03) << 4;
            keep_alive[5] = temp_uv & 0xFF00;
            keep_alive[6] = temp_uv & 0xFF;
            _hl_app_larksound_encode(p_msg->cmd, 0x80 | HAP_CTRL_RX, keep_alive, sizeof(keep_alive));
            LOG_I("app get(%d) larksound Keep Alive\n", p_msg->cmd);
            break;

        case LARKSOUND_REBOOT_CMD:
            LOG_I("app get(%d) larksound Reboot\n", p_msg->cmd);
            break;

        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
#endif
}

/*
 * EOF
 */