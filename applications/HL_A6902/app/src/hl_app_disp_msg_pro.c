/**
 * @file hl_app_disp_msg_pro.c
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
#include "hl_mod_display.h"
#include "hl_mod_audio.h"
#include "hl_mod_telink.h"

#define DBG_SECTION_NAME "app_disp"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#if HL_IS_TX_DEVICE()
void hl_app_disp_msg_pro(mode_to_app_msg_t* p_msg)
{
    switch (p_msg->cmd) {
        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}

void hl_app_disp_state_led_set(void)
{
    hl_led_net_mode led_ctrl;
    hl_led_switch   denoise_ctrl;
    hl_led_switch   record_ctrl;

    // RF
    switch (tx_info.rf_state) {
        case HL_RF_PAIRING:
            led_ctrl = LED_NET_MODE_PAIR;
            break;

        case HL_RF_UNCONNECT:
            led_ctrl = LED_NET_MODE_RECONNECTION;
            break;

        case HL_RF_L_CONNECT:
        case HL_RF_R_CONNECT:
        case HL_RF_LR_CONNECT:
            led_ctrl = LED_NET_MODE_CONNECTED;
            break;

        default:
            break;
    }
    hl_mod_display_io_ctrl(LED_NET_MODLE_CMD, &led_ctrl, sizeof(led_ctrl));

    // DENOISE
    if (tx_info.denoise_flag == 0) {
        denoise_ctrl = SWITCH_CLOSE;
    } else {
        denoise_ctrl = SWITCH_OPEN;
    }
    hl_mod_display_io_ctrl(LED_SWITCH_NOISE_REDUCTION_CMD, &denoise_ctrl, sizeof(denoise_ctrl));

    // record
    if (tx_info.rec_flag == 0) {
        record_ctrl = SWITCH_CLOSE;
    } else {
        record_ctrl = SWITCH_OPEN;
    }
    hl_mod_display_io_ctrl(LED_RECORD_STATE_CMD, &record_ctrl, sizeof(record_ctrl));
}

#else
static uint8_t _hl_app_disp_msg_pro_rf_connect()
{
    uint8_t        ret     = 0;
    static uint8_t channel = 0;

    switch (rx_info.rf_state) {
        case HL_RF_UNCONNECT:
            channel = 0x00;
            hl_mod_telink_ioctl(HL_RF_PAIR_START_CMD, &channel, sizeof(channel));
            LOG_D("conneting left channel\r\n");
            break;
        case HL_RF_L_CONNECT:
            channel = 0x01;
            hl_mod_telink_ioctl(HL_RF_PAIR_START_CMD, &channel, sizeof(channel));
            LOG_D("conneting right channel\r\n");
            break;
        case HL_RF_R_CONNECT:
            channel = 0x00;
            hl_mod_telink_ioctl(HL_RF_PAIR_START_CMD, &channel, sizeof(channel));
            LOG_D("conneting left channel\r\n");
            break;
        case HL_RF_LR_CONNECT:
            ret = 1;
            LOG_D("no need to paire\r\n");
            break;
        case HL_RF_PAIRING:
            ret = 1;
            LOG_D("already connecting\r\n");
            break;
    }

    return ret;
}

void hl_app_disp_msg_pro(mode_to_app_msg_t* p_msg)
{
    hl_rf_bypass_value_t telink_bypass = { 0 };
    switch (p_msg->cmd) {
        case RESTORE_SET_SWITCH_IND:
            // TBD: 恢复NVRAM的值并重启？
            LOG_D("RESTORE_SET_SWITCH_IND\r\n");
            break;
        case AUTO_RECORD_SWITCH_IND:
            // TBD: 透传通道给TX发出音频模块控制自动录制开启状态
            LOG_D("AUTO_RECORD_SWITCH_IND\r\n");
            break;
        case AUTO_RECORD_PORTECT_SWITCH_IND:
            // TBD: 透传通道给TX发出音频模块控制录制保护
            LOG_D("AUTO_RECORD_PORTECT_SWITCH_IND\r\n");
            break;
        case SOUND_MODULE_SET_IND:
            // TBD: 音频模块控制声道模式
            LOG_D("SOUND_MODULE_SET_IND\r\n");
            break;
        case TX1_NOISE_SWITCH_IND:
            // TBD: 透传通道给TX1发出降噪
            telink_bypass.chn = 0;
            telink_bypass.val = p_msg->param.u32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_DENOISE_CMD, &telink_bypass, sizeof(telink_bypass));
            LOG_D("TX1_NOISE_SWITCH_IND\r\n");
            break;
        case TX2_NOISE_SWITCH_IND:
            // TBD: 透传通道给TX2发出降噪
            telink_bypass.chn = 1;
            telink_bypass.val = p_msg->param.u32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_DENOISE_CMD, &telink_bypass, sizeof(telink_bypass));
            LOG_D("TX2_NOISE_SWITCH_IND\r\n");
            break;
        case TX1_RECORD_STATE_SWITCH_IND:
            // TBD: 透传通道给TX1发出录制开始/关闭
            LOG_D("TX1_RECORD_STATE_SWITCH_IND\r\n");
            break;
        case TX2_RECORD_STATE_SWITCH_IND:
            // TBD: 透传通道给TX2发出录制开始/关闭
            LOG_D("TX2_RECORD_STATE_SWITCH_IND\r\n");
            break;
        case TX1_MUTE_SWITCH_SWITCH_IND:
            // 透传通道给TX1发出MUTE使能/失能
            telink_bypass.chn = 0;
            telink_bypass.val = p_msg->param.u32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_MUTE_CMD, &telink_bypass, sizeof(telink_bypass));
            LOG_D("TX1_MUTE_SWITCH_SWITCH_IND\r\n");
            break;
        case TX2_MUTE_SWITCH_SWITCH_IND:
            // 透传通道给TX2发出MUTE使能/失能
            telink_bypass.chn = 1;
            telink_bypass.val = p_msg->param.u32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_MUTE_CMD, &telink_bypass, sizeof(telink_bypass));
            LOG_D("TX2_MUTE_SWITCH_SWITCH_IND\r\n");
            break;
        case TX1_GAIN_VAL_IND:
            // : 透传通道给TX1发出codec增益配置
            telink_bypass.chn = 0;
            telink_bypass.val = p_msg->param.s32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_VOLUME_CMD, &telink_bypass, sizeof(telink_bypass));
            LOG_D("TX1_GAIN_VAL_IND\r\n");
            break;
        case TX2_GAIN_VAL_IND:
            // : 透传通道给TX2发出codec增益配置
            telink_bypass.chn = 1;
            telink_bypass.val = p_msg->param.s32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_VOLUME_CMD, &telink_bypass, sizeof(telink_bypass));
            LOG_D("TX2_GAIN_VAL_IND\r\n");
            break;
        case TX1_FS_FORMAT_VAL_IND:
            // TBD: 透传通道给TX1发出格式化U盘命令
            telink_bypass.chn = 0;
            telink_bypass.val = p_msg->param.s32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_FORMAT_DISK_CMD, &telink_bypass, sizeof(telink_bypass));
            LOG_D("TX1_FS_FORMAT_VAL_IND\r\n");
            break;
        case TX2_FS_FORMAT_VAL_IND:
            // TBD: 透传通道给TX2发出格式化U盘命令
            telink_bypass.chn = 1;
            telink_bypass.val = p_msg->param.s32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_FORMAT_DISK_CMD, &telink_bypass, sizeof(telink_bypass));
            LOG_D("TX2_FS_FORMAT_VAL_IND\r\n");
            break;
        case VOICE_MODULE_VAL_IND:
            // ？？？
            LOG_D("VOICE_MODULE_VAL_IND\r\n");
            break;
        case LOW_CUT_VAL_IND:
            // : 透传通道给TX发出低切命令
            telink_bypass.chn = 2;
            telink_bypass.val = p_msg->param.u32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_LOWCUT_CMD, &telink_bypass, sizeof(telink_bypass));
            LOG_D("LOW_CUT_VAL_IND\r\n");
            break;
        case TX_NOISE_LEVEL_VAL_IND:
            // TBD: 透传通道给TX发出降噪等级命令
            LOG_D("TX_NOISE_LEVEL_VAL_IND\r\n");
            break;
        case TX1_LINE_OUT_VOLUME_VAL_IND:
            LOG_D("TX1_LINE_OUT_VOLUME_VAL_IND\r\n");
            break;
        case TX2_LINE_OUT_VOLUME_VAL_IND:
            LOG_D("TX2_LINE_OUT_VOLUME_VAL_IND\r\n");
            break;
        case MONITOR_CATEGORY_VAL_IND:
            // 监听源切换：MONITOR_TX_IN MONITOR_UAC_IN | typedef enum _hl_stream_mode_e
            if (rx_info.uac_link_flag) {
                if (1) {  // TBD: 需要添加状态，True是UAC到输出声卡， False是输入声卡到输出声卡
                    // UAC SPK => 输出声卡
                    hl_mod_audio_io_ctrl(HL_AUDIO_STREAM_SET_CMD, HL_STREAM_CAP2UAC_UAC2PLAY,
                                         sizeof(HL_STREAM_CAP2UAC_UAC2PLAY));
                } else {
                    // 输入声卡 =》 输出声卡
                    hl_mod_audio_io_ctrl(HL_AUDIO_STREAM_SET_CMD, HL_STREAM_CAP2PLAY_CAP2UAC,
                                         sizeof(HL_STREAM_CAP2PLAY_CAP2UAC));
                }
            } else {
                LOG_D("UAC not link yet\r\n");
            }
            LOG_D("MONITOR_CATEGORY_VAL_IND\r\n");
            break;
        case LINE_OUT_VOLUME_VAL_IND:
            // TBD: audio模块设置相机口音量
            LOG_D("LINE_OUT_VOLUME_VAL_IND\r\n");
            break;
        case MONITOR_VOLUME_VAL_IND:
            // TBD: audio模块设置监听口音量
            LOG_D("MONITOR_VOLUME_VAL_IND\r\n");
            break;
        case LED_BRITNESS_VAL_IND:
            // TBD: 透传通道给TX发出LED亮度调节命令
            LOG_D("LED_BRITNESS_VAL_IND\r\n");
            break;
        case SYSTIME_SET_VAL_IND:
            // 调用音量设置接口设置时间
            LOG_D("SYSTIME_SET_VAL_IND\r\n");
            break;
        case POWEROFF_SET_VAL_IND:
            // 自动关机时间设置
            LOG_D("POWEROFF_SET_VAL_IND\r\n");
            break;
        case DEVICE_PAIR_IND:
            // 进入配对状态
            _hl_app_disp_msg_pro_rf_connect();
            LOG_D("DEVICE_PAIR_IND\r\n");
            break;
        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}

void hl_app_disp_state_led_set(void)
{
}
#endif

/*
 * EOF
 */