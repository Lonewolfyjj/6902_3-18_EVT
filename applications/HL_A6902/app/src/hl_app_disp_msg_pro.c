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
#include "hl_util_nvram.h"
#include "hl_app_audio_msg_pro.h"

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
    hl_led_switch   mute_ctrl;
    hl_led_switch   upgrade_ctrl;

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

    // mute
    if (tx_info.mute_flag == 0) {
        mute_ctrl = SWITCH_CLOSE;
    } else {
        mute_ctrl = SWITCH_OPEN;
    }
    hl_mod_display_io_ctrl(LED_SWITCH_MUTE_CMD, &mute_ctrl, sizeof(mute_ctrl));

    if (tx_info.upgrade_flag == 1) {
        upgrade_ctrl = SWITCH_OPEN;
    } else {
        upgrade_ctrl = SWITCH_CLOSE;
    }
    hl_mod_display_io_ctrl(LED_SWITCH_UPDATE_CMD, &upgrade_ctrl, sizeof(upgrade_ctrl));
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
            hl_mod_display_io_ctrl(RX_RF_STATE_VAL_CMD, &rx_info.rf_state, sizeof(rx_info.rf_state));
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
    hl_rf_channel_e           rf_bypass_chn   = 0;
    hl_rf_bypass_value_t      rf_bypass_value = { 0 };
    hl_rf_bypass_state_t      rf_bypass_state = { 0 };
    hl_rf_bypass_time_t       rf_bypass_time  = { 0 };
    uint32_t                  ptr;
    int32_t                    ptr_int32;
    hl_display_sound_module_e sound_module;
    hl_audio_time_t           time;
    hl_display_systime_s      display_time;

    switch (p_msg->cmd) {
        case RESTORE_SET_SWITCH_IND:
            // TBD: 恢复NVRAM的值并重启？
            rf_bypass_chn = HL_RF_DOUBLE_CHANNEL;
            hl_mod_telink_ioctl(HL_RF_BYPASS_REFACTORY_CMD, &rf_bypass_chn, sizeof(rf_bypass_chn));    
            hl_app_param_reset(); // 建议跳入语言设置和时间设置入用户进行重新设置     
            LOG_D("RESTORE_SET_SWITCH_IND\r\n");
            break;
        case AUTO_RECORD_SWITCH_IND:
            // TBD: 透传通道给TX发出音频模块控制自动录制开启状态
            rf_bypass_state.chn   = HL_RF_DOUBLE_CHANNEL;
            rf_bypass_state.state = (uint8_t)p_msg->param.u32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_AUTO_RECORD_CMD, &rf_bypass_state, sizeof(rf_bypass_state));
            LOG_D("AUTO_RECORD_SWITCH_IND\r\n");
            break;
        case AUTO_RECORD_PORTECT_SWITCH_IND:
            // TBD: 透传通道给TX发出音频模块控制录制保护
            rf_bypass_state.chn   = HL_RF_DOUBLE_CHANNEL;
            rf_bypass_state.state = (uint8_t)p_msg->param.u32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_RECORD_PROTECT_CMD, &rf_bypass_state, sizeof(rf_bypass_state));
            LOG_D("AUTO_RECORD_PORTECT_SWITCH_IND\r\n");
            break;
        case SOUND_MODULE_SET_IND:
            ptr          = (uint32_t)p_msg->param.u32_param;
            sound_module = (hl_display_sound_module_e)ptr;
            rx_info.sound_mod = sound_module;
            sound_module = (sound_module == 1 ? 0 : 1);
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_MIX_SWITCH_CMD, &sound_module, 1);
            hl_app_audio_cam_gain_update();
            // TBD: 音频模块控制声道模式
            LOG_D("SOUND_MODULE_SET_IND\r\n");
            break;
        case TX1_NOISE_SWITCH_IND:
            // TBD: 透传通道给TX1发出降噪 uint8_t 0是关闭降噪 1-254 表示降噪的等级   0XFF表示的是开启降噪功能
            rf_bypass_value.chn = HL_RF_LEFT_CHANNEL;
            rf_bypass_value.val = (uint8_t)p_msg->param.u32_param;
            if (rf_bypass_value.val) {
                rf_bypass_value.val = 0xFF;
            } else {
                rf_bypass_value.val = 0x00;
            }
            hl_mod_telink_ioctl(HL_RF_BYPASS_DENOISE_CMD, &rf_bypass_value, sizeof(rf_bypass_value));
            LOG_D("TX1_NOISE_SWITCH_IND\r\n");
            break;
        case TX2_NOISE_SWITCH_IND:
            // TBD: 透传通道给TX2发出降噪
            rf_bypass_value.chn = HL_RF_RIGHT_CHANNEL;
            rf_bypass_value.val = (uint8_t)p_msg->param.u32_param;
            if (rf_bypass_value.val) {
                rf_bypass_value.val = 0xFF;
            } else {
                rf_bypass_value.val = 0x00;
            }
            hl_mod_telink_ioctl(HL_RF_BYPASS_DENOISE_CMD, &rf_bypass_value, sizeof(rf_bypass_value));
            LOG_D("TX2_NOISE_SWITCH_IND\r\n");
            break;
        case TX1_RECORD_STATE_SWITCH_IND:
            // TBD: 透传通道给TX1发出录制开始/关闭
            rf_bypass_state.chn   = HL_RF_LEFT_CHANNEL;
            rf_bypass_state.state = (uint8_t)p_msg->param.u32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_RECORD_CMD, &rf_bypass_state, sizeof(rf_bypass_state));
            LOG_D("TX1_RECORD_STATE_SWITCH_IND %d\r\n",rf_bypass_state.state);
            break;
        case TX2_RECORD_STATE_SWITCH_IND:
            // TBD: 透传通道给TX2发出录制开始/关闭
            rf_bypass_state.chn   = HL_RF_RIGHT_CHANNEL;
            rf_bypass_state.state = (uint8_t)p_msg->param.u32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_RECORD_CMD, &rf_bypass_state, sizeof(rf_bypass_state));
            LOG_D("TX2_RECORD_STATE_SWITCH_IND %d\r\n",rf_bypass_state.state);
            break;
        case TX1_MUTE_SWITCH_SWITCH_IND:
            // 透传通道给TX1发出MUTE使能/失能
            rf_bypass_state.chn   = HL_RF_LEFT_CHANNEL;
            rf_bypass_state.state = (uint8_t)p_msg->param.u32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_MUTE_CMD, &rf_bypass_state, sizeof(rf_bypass_state));
            LOG_D("TX1_MUTE_SWITCH_SWITCH_IND %d\r\n",rf_bypass_state.state);
            break;
        case TX2_MUTE_SWITCH_SWITCH_IND:
            // 透传通道给TX2发出MUTE使能/失能
            rf_bypass_state.chn   = HL_RF_RIGHT_CHANNEL;
            rf_bypass_state.state = (uint8_t)p_msg->param.u32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_MUTE_CMD, &rf_bypass_state, sizeof(rf_bypass_state));
            LOG_D("TX2_MUTE_SWITCH_SWITCH_IND %d\r\n",rf_bypass_state.state);
            break;
        case TX1_GAIN_VAL_IND:
            // : 透传通道给TX1发出codec增益配置
            rf_bypass_value.chn = HL_RF_LEFT_CHANNEL;
            rf_bypass_value.val = (int8_t)p_msg->param.s32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_TX_GAIN_CMD, &rf_bypass_value, sizeof(rf_bypass_value));
            hl_util_nvram_param_set_integer("TX_GAIN", rf_bypass_value.val);
            LOG_D("TX1_GAIN_VAL_IND\r\n");
            break;
        case TX2_GAIN_VAL_IND:
            // : 透传通道给TX2发出codec增益配置
            rf_bypass_value.chn = HL_RF_RIGHT_CHANNEL;
            rf_bypass_value.val = (int8_t)p_msg->param.s32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_TX_GAIN_CMD, &rf_bypass_value, sizeof(rf_bypass_value));
            hl_util_nvram_param_set_integer("TX_GAIN2", rf_bypass_value.val);
            LOG_D("TX2_GAIN_VAL_IND\r\n");
            break;
        case TX1_FS_FORMAT_VAL_IND:
            // TBD: 透传通道给TX1发出格式化U盘命令
            rf_bypass_chn = HL_RF_LEFT_CHANNEL;
            hl_mod_telink_ioctl(HL_RF_BYPASS_FORMAT_DISK_CMD, &rf_bypass_chn, sizeof(rf_bypass_chn));
            LOG_D("TX1_FS_FORMAT_VAL_IND\r\n");
            break;
        case TX2_FS_FORMAT_VAL_IND:
            // TBD: 透传通道给TX2发出格式化U盘命令
            rf_bypass_chn = HL_RF_RIGHT_CHANNEL;
            hl_mod_telink_ioctl(HL_RF_BYPASS_FORMAT_DISK_CMD, &rf_bypass_chn, sizeof(rf_bypass_chn));
            LOG_D("TX2_FS_FORMAT_VAL_IND\r\n");
            break;
        case VOICE_MODULE_VAL_IND:
            // ？？？
            rf_bypass_value.chn = HL_RF_DOUBLE_CHANNEL;
            rf_bypass_value.val = (uint8_t)p_msg->param.u32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_SOUND_EFFECT_CMD, &rf_bypass_value, sizeof(rf_bypass_value));
            LOG_D("VOICE_MODULE_VAL_IND\r\n");
            break;
        case LOW_CUT_VAL_IND:
            // : 透传通道给TX发出低切命令
            rf_bypass_state.chn   = HL_RF_DOUBLE_CHANNEL;
            rf_bypass_state.state = (uint8_t)p_msg->param.u32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_LOWCUT_CMD, &rf_bypass_state, sizeof(rf_bypass_state));
            LOG_D("LOW_CUT_VAL_IND\r\n");
            break;
        case TX_NOISE_LEVEL_VAL_IND:
            // TBD: 透传通道给TX发出降噪等级命令
            rf_bypass_value.chn = HL_RF_DOUBLE_CHANNEL;
            rf_bypass_value.val = p_msg->param.u32_param;
            if (rf_bypass_value.val == 0 || rf_bypass_value.val == 0xFF) {
                rf_bypass_value.val = 1;
            }
            hl_mod_telink_ioctl(HL_RF_BYPASS_DENOISE_CMD, &rf_bypass_value, sizeof(rf_bypass_value));
            LOG_D("TX_NOISE_LEVEL_VAL_IND(%d)\r\n", rf_bypass_value.val);
            break;
        case TX1_LINE_OUT_VOLUME_VAL_IND:
            ptr_int32 = p_msg->param.s32_param*2;
            // hl_mod_audio_io_ctrl(HL_AUDIO_SET_CAM_GAIN_L_CMD, &ptr_int32, 4);
            rx_info.st_cam_gain_l = ptr_int32;
            hl_app_audio_cam_gain_update();
            hl_util_nvram_param_set_integer("ST_CAM_L_GAIN", rx_info.st_cam_gain_l);
            LOG_D("TX1_LINE_OUT_VOLUME_VAL_IND(%d)\r\n",rx_info.st_cam_gain_l);
            break;
        case TX2_LINE_OUT_VOLUME_VAL_IND:
            ptr_int32 = p_msg->param.s32_param*2;
            // hl_mod_audio_io_ctrl(HL_AUDIO_SET_CAM_GAIN_R_CMD, &ptr_int32, 4);
            rx_info.st_cam_gain_r = ptr_int32;
            hl_app_audio_cam_gain_update();
            hl_util_nvram_param_set_integer("ST_CAM_R_GAIN", rx_info.st_cam_gain_r);
            LOG_D("TX2_LINE_OUT_VOLUME_VAL_IND(%d)\r\n",rx_info.st_cam_gain_r);
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
        case UAC_OUT_VOLUME_VAL_IND:
            // TBD: MONO设置UAC音量
            ptr_int32 = p_msg->param.s32_param;
            hl_mod_audio_io_ctrl(HL_AUDIO_SET_GAIN_UAC_CMD, &ptr_int32, 4);
            rx_info.uac_gain = ptr_int32;
            hl_util_nvram_param_set_integer("RX_UAC_GAIN", rx_info.uac_gain);
            LOG_D("UAC_OUT_VOLUME_VAL_IND(%d)",rx_info.uac_gain);
            break;
        case MONO_LINE_OUT_VOLUME_VAL_IND:
            // TBD: MONO设置相机口音量
            ptr_int32 = p_msg->param.s32_param*2;
            // hl_mod_audio_io_ctrl(HL_AUDIO_SET_CAM_GAIN_L_CMD, &ptr_int32, 4);
            // hl_mod_audio_io_ctrl(HL_AUDIO_SET_CAM_GAIN_R_CMD, &ptr_int32, 4);            
            rx_info.mono_cam_gain = ptr_int32;
            hl_app_audio_cam_gain_update();
            hl_util_nvram_param_set_integer("MONO_CAM_GAIN", rx_info.mono_cam_gain);
            LOG_D("MONO_LINE_OUT_VOLUME_VAL_IND(%d)\r\n",rx_info.mono_cam_gain);
            break;
        case SAFETRACK_LINE_OUT_VOLUME_VAL_IND:
            // TBD: SAFETRACK设置相机口音量
            ptr_int32 = p_msg->param.s32_param*2;
            // hl_mod_audio_io_ctrl(HL_AUDIO_SET_CAM_GAIN_L_CMD, &ptr_int32, 4);
            // hl_mod_audio_io_ctrl(HL_AUDIO_SET_CAM_GAIN_R_CMD, &ptr_int32, 4);
            rx_info.sft_cam_gain = ptr_int32;
            hl_app_audio_cam_gain_update();
            hl_util_nvram_param_set_integer("SFT_CAM_GAIN", rx_info.sft_cam_gain);
            LOG_D("SAFETRACK_LINE_OUT_VOLUME_VAL_IND(%d)\r\n",rx_info.sft_cam_gain);
            break;
        case MONITOR_VOLUME_VAL_IND:
            // TBD: audio模块设置监听口音量
            ptr_int32 = p_msg->param.s32_param;
            // hl_mod_audio_io_ctrl(HL_AUDIO_SET_HP_GAIN_L_CMD, &ptr_int32, 4);
            // hl_mod_audio_io_ctrl(HL_AUDIO_SET_HP_GAIN_R_CMD, &ptr_int32, 4);
            rx_info.hp_gain = ptr_int32;
            hl_app_audio_hp_gain_update();
            hl_util_nvram_param_set_integer("RX_HP_L_GAIN", rx_info.hp_gain);
            hl_util_nvram_param_set_integer("RX_HP_R_GAIN", rx_info.hp_gain);
            LOG_D("MONITOR_VOLUME_VAL_IND(%d)\r\n",rx_info.hp_gain);
            break;
        case LED_BRITNESS_VAL_IND:
            // TBD: 透传通道给TX发出LED亮度调节命令
            rx_info.tx_led_britness = (uint8_t)p_msg->param.u32_param;
            rf_bypass_value.chn     = HL_RF_DOUBLE_CHANNEL;
            rf_bypass_value.val     = (uint8_t)p_msg->param.u32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_STATUS_LED_CMD, &rf_bypass_value, sizeof(rf_bypass_value));
            LOG_D("LED_BRITNESS_VAL_IND(%d)\r\n", rx_info.tx_led_britness);
            break;
        case SYSTIME_SET_VAL_IND:
            // 调用音量设置接口设置时间
            LOG_D("SYSTIME_SET_VAL_IND\r\n");
            hl_mod_audio_io_ctrl(HL_AUDIO_GET_RTC_TIME_CMD, &rf_bypass_time.time, sizeof(rf_bypass_time.time));
            rt_kprintf("\nTIME-->[%d][%d][%d][%d][%d][%d]\n", rf_bypass_time.time.year, rf_bypass_time.time.month,
                       rf_bypass_time.time.day, rf_bypass_time.time.hour, rf_bypass_time.time.minute, rf_bypass_time.time.second);
            hl_mod_telink_ioctl(HL_RF_BYPASS_TIME_CMD, &rf_bypass_time, sizeof(rf_bypass_time));
            break;
        case POWEROFF_SET_VAL_IND:
            // 自动关机时间设置
            rf_bypass_value.chn = HL_RF_DOUBLE_CHANNEL;
            rf_bypass_value.val = (uint8_t)p_msg->param.u32_param;
            hl_mod_telink_ioctl(HL_RF_BYPASS_AUTO_POWEROFF_CMD, &rf_bypass_value, sizeof(rf_bypass_value));
            LOG_D("POWEROFF_SET_VAL_IND\r\n");
            break;
        case DEVICE_PAIR_IND:
            // 进入配对状态
            _hl_app_disp_msg_pro_rf_connect();
            LOG_D("DEVICE_PAIR_IND\r\n");
            break;
        case UPGRADE_SETTING_SWITCH_IND:
            // 开启升级的相关设置
            LOG_D("UPGRADE_SETTING_SWITCH_IND\r\n");
            break;
        case SYSTIME_GET_VAL_IND:

            hl_mod_audio_io_ctrl(HL_AUDIO_GET_RTC_TIME_CMD, &time, sizeof(time));
            display_time.year  = time.year;
            display_time.month = time.month;
            display_time.day   = time.day;
            display_time.hour  = time.hour;
            display_time.min   = time.minute;

            hl_mod_display_io_ctrl(SYSTIME_SET_VAL_CMD, &display_time, sizeof(display_time));
            
            LOG_D("SYSTIME_GET_VAL_IND\r\n");
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