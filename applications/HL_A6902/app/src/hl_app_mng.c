/**
 * @file hl_app_mng.c
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
#include "hl_mod_apple_auth.h"
#include "hl_mod_pm.h"
#include "hl_mod_upgrade.h"
#include "hl_mod_euc.h"
#include "hl_app_audio_msg_pro.h"
#include "hl_app_com_msg_pro.h"
#include "hl_app_disp_msg_pro.h"
#include "hl_app_iap2_msg_pro.h"
#include "hl_app_input_msg_pro.h"
#include "hl_app_pm_msg_pro.h"
#include "hl_app_rf_msg_pro.h"
#include "hl_app_upgrade_msg_pro.h"
#include "hl_util_general_type.h"
#include "hl_mod_wdog.h"
#include "hl_util_nvram.h"
#include "hl_board_commom.h"
#include "hl_hal_gpio.h"

#define DBG_SECTION_NAME "app_mng"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/// 应用层消息队列，用于接收模块的消息
struct rt_messagequeue hl_app_mq;
/// 应用层消息队列池
static char hl_app_msg_pool[2048];

/// 模块层发送给APP的消息格式
#if HL_IS_TX_DEVICE()
/// tx app 状态信息
tx_app_info_t tx_info = { 0 };
#else
/// rx app 状态信息
rx_app_info_t rx_info = { 0 };
#endif

/* Private function(only *.c)  -----------------------------------------------*/
int hl_app_info(int argc, char** argv);

#if HL_IS_TX_DEVICE()
void hl_app_param_loader(void)
{
    uint8_t param = 0;

    hl_util_nvram_param_get_integer("MUTE_OPEN", &tx_info.mute_flag, 0);
    hl_util_nvram_param_get_integer("REC_PROTECT_OPEN", &tx_info.rec_protect_flag, 0);
    hl_util_nvram_param_get_integer("REC_AUTO_OPEN", &tx_info.rec_auto_flag, 0);
    hl_util_nvram_param_get_integer("DENOISE_PROTECT_OPEN", &tx_info.denoise_protect_flag, 0);
    hl_util_nvram_param_get_integer("DENOISE_AUTO_OPEN", &tx_info.denoise_auto_flag, 0);
    hl_util_nvram_param_get_integer("DENOISE_OPEN", &param, 0);   
    tx_info.denoise_flag = param;
    hl_util_nvram_param_get_integer("DENOISE_CLASS", &tx_info.denoise_class, 0);

    hl_util_nvram_param_get_integer("TX_GAIN", &tx_info.gain, 0);
    hl_util_nvram_param_get_integer("TX_UAC_GAIN", &tx_info.uac_gain, 0);
}

void hl_app_param_fun(void)
{
    hl_switch_e param_switch = HL_SWITCH_OFF;

    if (tx_info.mute_flag == 1) {
        param_switch = HL_SWITCH_ON;
        hl_mod_audio_io_ctrl(HL_AUDIO_SET_MUTE_CMD, &param_switch, 1);
    }

    if (tx_info.rec_auto_flag == 1) {
        tx_info.rec_flag = 1;
        param_switch     = HL_SWITCH_ON;
        hl_mod_audio_io_ctrl(HL_AUDIO_RECORD_CMD, &param_switch, 1);
        hl_app_pm_timer_set();
    }

    if (tx_info.denoise_flag == 1) {
        //tx_info.denoise_flag = 1;
        param_switch         = HL_SWITCH_ON;
        // 降噪等级设置 。。。
        hl_mod_audio_io_ctrl(HL_AUDIO_SET_DENOISE_CMD, &param_switch, 1);
        //hl_mod_audio_io_ctrl(HL_AUDIO_SET_DENOISE_CMD, &param_switch, 1);
    }
    hl_app_disp_state_led_set();
}

void hl_app_param_reset(void)
{
    uint8_t param = 0;
    hl_switch_e param_switch = HL_SWITCH_OFF;

    tx_info.mute_flag = 0;
    hl_util_nvram_param_set_integer("MUTE_OPEN", tx_info.mute_flag);
    tx_info.rec_protect_flag =0;
    hl_util_nvram_param_set_integer("REC_PROTECT_OPEN", tx_info.rec_protect_flag);
    tx_info.rec_auto_flag =0;
    hl_util_nvram_param_set_integer("REC_AUTO_OPEN", tx_info.rec_auto_flag);
    tx_info.denoise_protect_flag =0;
    hl_util_nvram_param_set_integer("DENOISE_PROTECT_OPEN", tx_info.denoise_protect_flag);
    tx_info.denoise_auto_flag =0;
    hl_util_nvram_param_set_integer("DENOISE_AUTO_OPEN", tx_info.denoise_auto_flag);
    param = 0;
    tx_info.denoise_flag = param;
    hl_util_nvram_param_set_integer("DENOISE_OPEN", param);       
    tx_info.denoise_class = 0;
    hl_util_nvram_param_set_integer("DENOISE_CLASS", tx_info.denoise_class);

    tx_info.gain = 0;
    hl_util_nvram_param_set_integer("TX_GAIN", tx_info.gain);
    tx_info.uac_gain = 0;
    hl_util_nvram_param_set_integer("TX_UAC_GAIN", tx_info.uac_gain);


    if (tx_info.mute_flag == 0) {
        param_switch = HL_SWITCH_OFF;
        hl_mod_audio_io_ctrl(HL_AUDIO_SET_MUTE_CMD, &param_switch, 1);
    }

    if (tx_info.denoise_flag == 0) {
        param_switch = HL_SWITCH_OFF;
        // 降噪等级设置 。。。
        hl_mod_audio_io_ctrl(HL_AUDIO_SET_DENOISE_CMD, &param_switch, 1);
    }

    hl_app_audio_gain(tx_info.gain);
    hl_app_audio_gain_uac(tx_info.uac_gain);    
}

#else
void hl_app_param_loader(void)
{
    // 声道默认是立体声
    rx_info.sound_mod = STEREO;

    hl_util_nvram_param_get_integer("RX_HP_L_GAIN", &rx_info.hp_gain, 6);
    // hl_util_nvram_param_get_integer("RX_HP_R_GAIN", &rx_info.hp_gain, 6);
    hl_util_nvram_param_get_integer("ST_CAM_L_GAIN", &rx_info.st_cam_gain_l, 0);
    // 默认立体声
    // hl_util_nvram_param_get_integer("SOUND_MOD", &rx_info.sound_mod, 1);
    hl_util_nvram_param_get_integer("MONO_CAM_GAIN", &rx_info.mono_cam_gain, 0);
    hl_util_nvram_param_get_integer("SFT_CAM_GAIN", &rx_info.sft_cam_gain, 0);

    hl_util_nvram_param_get_integer("ST_CAM_R_GAIN", &rx_info.st_cam_gain_r, 0);
    hl_util_nvram_param_get_integer("RX_UAC_GAIN", &rx_info.uac_gain, 0);
}

void hl_app_param_fun(void)
{
    int8_t param = 0;

    // 读取TX的配置 发送配置给屏幕
    hl_util_nvram_param_get_integer("MUTE_OPEN", &param, 0);
    hl_mod_display_io_ctrl(TX1_MUTE_SWITCH_SWITCH_CMD, &param, 1);
    hl_mod_display_io_ctrl(TX2_MUTE_SWITCH_SWITCH_CMD, &param, 1);

    hl_util_nvram_param_get_integer("REC_PROTECT_OPEN", &param, 0);
    hl_mod_display_io_ctrl(AUTO_RECORD_PORTECT_SWITCH_CMD, &param, 1);

    hl_util_nvram_param_get_integer("REC_AUTO_OPEN", &param, 0);
    hl_mod_display_io_ctrl(AUTO_RECORD_SWITCH_CMD, &param, 1);

    hl_util_nvram_param_get_integer("TX_GAIN", &param, 0);
    hl_mod_display_io_ctrl(TX1_GAIN_VAL_CMD, &param, 1);
    hl_util_nvram_param_get_integer("TX_GAIN2", &param, 0);
    hl_mod_display_io_ctrl(TX2_GAIN_VAL_CMD, &param, 1);    

    param = rx_info.uac_gain; 
    hl_mod_display_io_ctrl(UAC_OUT_VOLUME_VAL_CMD, &param, 1);   
    hl_mod_audio_io_ctrl(HL_AUDIO_SET_GAIN_UAC_CMD, &rx_info.uac_gain, 4);

    // 声道模式默认开机改变
    // LOG_D("soundmod=(%d)\r\n",rx_info.sound_mod);
    // hl_mod_display_io_ctrl(,&rx_info.sound_mod,1);
    param = rx_info.mono_cam_gain / 2;
    hl_mod_display_io_ctrl(MONO_LINE_OUT_VOLUME_VAL_CMD, &param, 1);

    param = rx_info.sft_cam_gain / 2;
    hl_mod_display_io_ctrl(SFT_LINE_OUT_VOLUME_VAL_CMD, &param, 1);

    param = rx_info.st_cam_gain_l / 2;
    LOG_D("st_cam_gain_l=(%d)\r\n",rx_info.st_cam_gain_l);
    hl_mod_display_io_ctrl(TX1_LINE_OUT_VOLUME_VAL_CMD, &param, 1);

    param = rx_info.st_cam_gain_r / 2;
    LOG_D("st_cam_gain_r=(%d)\r\n",rx_info.st_cam_gain_r);
    hl_mod_display_io_ctrl(TX2_LINE_OUT_VOLUME_VAL_CMD, &param, 1);

    param = rx_info.hp_gain;
    LOG_D("hp_gain = (%d)\r\n",param);
    hl_mod_display_io_ctrl(MONITOR_VOLUME_VAL_CMD, &param, 1);

    // hl_app_audio_hp_gain_update();
    // hl_app_audio_cam_gain_update();
    hl_app_audio_switch();
}

void hl_app_param_reset(void)
{
    uint8_t param = 0;
    hl_switch_e param_switch = HL_SWITCH_OFF;

    param = 0;
    hl_util_nvram_param_set_integer("MUTE_OPEN", param);
    hl_mod_display_io_ctrl(TX1_MUTE_SWITCH_SWITCH_CMD, &param, 1);
    hl_mod_display_io_ctrl(TX2_MUTE_SWITCH_SWITCH_CMD, &param, 1);

    param = 0;
    hl_util_nvram_param_set_integer("REC_PROTECT_OPEN", param);
    hl_mod_display_io_ctrl(AUTO_RECORD_PORTECT_SWITCH_CMD, &param, 1);

    param = 0;
    hl_util_nvram_param_set_integer("REC_AUTO_OPEN", param);
    hl_mod_display_io_ctrl(AUTO_RECORD_SWITCH_CMD, &param, 1);

    param = 0;
    hl_util_nvram_param_set_integer("TX_GAIN", param);
    hl_mod_display_io_ctrl(TX1_GAIN_VAL_CMD, &param, 1);
    hl_util_nvram_param_set_integer("TX_GAIN2", param);
    hl_mod_display_io_ctrl(TX2_GAIN_VAL_CMD, &param, 1);
    
    param = 0;
    rx_info.uac_gain = 0;
    hl_util_nvram_param_set_integer("RX_UAC_GAIN", param);        
    hl_mod_display_io_ctrl(UAC_OUT_VOLUME_VAL_CMD, &param, 1);
    hl_mod_audio_io_ctrl(HL_AUDIO_SET_GAIN_UAC_CMD, &rx_info.uac_gain, 4);

    rx_info.hp_gain = 0;
    hl_util_nvram_param_set_integer("RX_HP_L_GAIN", rx_info.hp_gain);
    // hl_util_nvram_param_set_integer("RX_HP_R_GAIN", rx_info.hp_gain);
    hl_mod_audio_io_ctrl(HL_AUDIO_SET_HP_GAIN_L_CMD, &rx_info.hp_gain, 4);
    hl_mod_audio_io_ctrl(HL_AUDIO_SET_HP_GAIN_R_CMD, &rx_info.hp_gain, 4);

    rx_info.st_cam_gain_l = 0;
    rx_info.st_cam_gain_r = 0;
    hl_util_nvram_param_set_integer("ST_CAM_L_GAIN", rx_info.st_cam_gain_l);
    hl_util_nvram_param_set_integer("ST_CAM_R_GAIN", rx_info.st_cam_gain_r);
    hl_mod_audio_io_ctrl(HL_AUDIO_SET_CAM_GAIN_L_CMD, &rx_info.st_cam_gain_l, 4);
    hl_mod_audio_io_ctrl(HL_AUDIO_SET_CAM_GAIN_R_CMD, &rx_info.st_cam_gain_r, 4);

    rx_info.mono_cam_gain = 0;
    hl_util_nvram_param_set_integer("MONO_CAM_GAIN", rx_info.mono_cam_gain);

    rx_info.sft_cam_gain = 0;
    hl_util_nvram_param_set_integer("SFT_CAM_GAIN", rx_info.sft_cam_gain);

    param = rx_info.st_cam_gain_l/2;
    hl_mod_display_io_ctrl(TX1_LINE_OUT_VOLUME_VAL_CMD, &param, 1);
    param = rx_info.st_cam_gain_r/2;
    hl_mod_display_io_ctrl(TX2_LINE_OUT_VOLUME_VAL_CMD, &param, 1);

    param = rx_info.hp_gain/2;
    hl_mod_display_io_ctrl(MONITOR_VOLUME_VAL_CMD, &param, 1);
}
#endif
/* Exported functions --------------------------------------------------------*/
void hl_app_msg_thread(void* parameter)
{
    hl_mod_feed_dog();
    mode_to_app_msg_t msg = { 0 };

    hl_app_mng_charger_entry(&hl_app_mq);

    hl_mod_upgrade_init(&hl_app_mq);

#if HL_IS_TX_DEVICE()
    rt_thread_mdelay(5);
    if (tx_info.mstorage_plug == 0) {
#else
    if (rx_info.mstorage_plug == 0) {
#endif
        hl_mod_audio_io_ctrl(HL_AUDIO_CHECK_DFS_CMD, NULL, 0);
        hl_mod_upgrade_io_ctrl(HL_UPGRADE_OPEN_CMD, NULL, 0);
    }
    hl_app_param_fun();

    //退出charger app后重新通知pm上报一次电量和充电状态信息，因为第一次上报的信息在charger app中可能被清除了，没有处理
    hl_mod_pm_ctrl(HL_PM_BAT_INFO_UPDATE_CMD, RT_NULL, 0);
    hl_mod_input_io_ctrl(HL_INPUT_RESET_CMD, RT_NULL, 0);

    rt_memset(&msg, 0, sizeof(msg));
    while (1) {
        if (rt_mq_recv(&hl_app_mq, &msg, sizeof(msg), -1) == RT_EOK) {
            // LOG_D("recv msg sender:%d, cmd:%d, param:%d !!!", msg.sender, msg.cmd, msg.param);
            switch (msg.sender) {
                case INPUT_MODE:
                    hl_app_input_msg_pro(&msg);
#if !HL_IS_TX_DEVICE()
                    hl_mod_display_io_ctrl(MSG_INPUT_UPDATE_CMD, &msg, 0);
#endif
                    break;
                case DISPLAY_MODE:
                    hl_app_disp_msg_pro(&msg);
                    break;
                case EXT_COM_MODE:
                    hl_app_com_msg_pro(&msg);
                    break;
                case UPGRADE_MODE:
                    hl_app_upgrade_msg_pro(&msg);
                    break;
                case APPLE_AUTH_MODE:
                    hl_app_iap2_msg_pro(&msg);
                    break;
                case TELINK_MODE:
                    hl_app_rf_msg_pro(&msg);
                    break;
                case AUDIO_MODE:
                    hl_app_audio_msg_pro(&msg);
                    break;
                case PM_MODE:
                    hl_app_pm_msg_pro(&msg);
                    break;
                default:
                    LOG_E("sender(%d) unkown!!!", msg.sender);
                    break;
            }
        }
        // rt_thread_mdelay(20);
        hl_mod_feed_dog();
    }
}

void hl_app_mng_init(void)
{
    rt_err_t    ret;
    rt_thread_t app_task_tid = RT_NULL;

    ret = rt_mq_init(&hl_app_mq, "AppMsg", &hl_app_msg_pool[0], sizeof(mode_to_app_msg_t), sizeof(hl_app_msg_pool),
                     RT_IPC_FLAG_FIFO);
    if (ret != RT_EOK) {
        LOG_E("message queuecreate init err!!!");
        return;
    }

    hl_board_nvram_init();
    hl_app_param_loader();
    hl_mod_input_init(&hl_app_mq);
    hl_mod_display_init(&hl_app_mq);
    hl_mod_pm_init(&hl_app_mq);
    hl_mod_pm_start();
    hl_mod_euc_init(&hl_app_mq);
    hl_mod_euc_start();

    app_task_tid = rt_thread_create("app_task", hl_app_msg_thread, RT_NULL, 10240, 10, 5);
    if (app_task_tid) {
        rt_thread_startup(app_task_tid);
    } else {
        LOG_E("thread create err!!!");
    }

    // rt_thread_mdelay(2000);
    hl_app_info(0, NULL);
}

// 开机，初始化模块
void hl_app_mng_powerOn(void)
{
    uint8_t              value = 0;
    int                  msc_open_flag;
    uint8_t              ret;
    hl_rf_bypass_state_t bypass_state;

    LOG_I("power on");
    hl_mod_pm_ctrl(HL_PM_POWER_UP_CMD, NULL, 0);
    // hl_mod_display_init(&hl_app_mq);
    hl_mod_telink_init(&hl_app_mq);
    hl_mod_telink_start();

    hl_mod_audio_init(&hl_app_mq);
#if HL_IS_TX_DEVICE()
    //开机同步数据
    bypass_state.chn   = tx_info.rf_chn;
    bypass_state.state = tx_info.charge_flag == 1 ? HL_RF_ON : HL_RF_OFF;
    hl_mod_telink_ioctl(HL_RF_BYPASS_CHARGE_CMD, &bypass_state, sizeof(bypass_state));
#else
    ret = hl_util_nvram_param_get_integer("MSC_OPEN", &msc_open_flag, 0);
    if (ret == 1) {
        rt_kprintf("nvram be used before not init\n");
        hl_board_nvram_init();
        ret = hl_util_nvram_param_get_integer("MSC_OPEN", &msc_open_flag, 0);
    }

    LOG_D("msc_open_flag = %d ,ret = %d ", msc_open_flag, ret);
    if (msc_open_flag == 1) {
        hl_mod_audio_deinit();  //hl_mod_audio_init(&hl_app_mq);
    }

    hl_mod_apple_auth_init(&hl_app_mq);
#endif

    hl_mod_euc_init(&hl_app_mq);
    hl_mod_euc_start();

#if HL_IS_TX_DEVICE()
#else
    hl_mod_audio_io_ctrl(HL_AUDIO_SET_GAIN_CMD, &rx_info.cur_volume_r, 2);
    if (rx_info.charge_flag == 0) {
        value = 0;
    } else if (rx_info.charge_flag == 1) {
        value = 1;
    } else if (rx_info.charge_flag == 2) {
        value = 0;
    } else if (rx_info.charge_flag == 3) {
        value = 0;
    }
    LOG_D("rx_info.charge_flag=%d ", rx_info.charge_flag);
    hl_mod_display_io_ctrl(RX_CHARGE_STATUS_SWITCH_CMD, &value, 1);
#endif
}

// 关机，去初始化模块
void hl_app_mng_powerOff(void)
{
    int     msc_open_flag;
    uint8_t ret;

    LOG_I("power off");
    hl_util_nvram_param_save();
#if HL_IS_TX_DEVICE()
    hl_hal_gpio_low(GPIO_DC3V3_EN);
#else
    hl_hal_gpio_high(GPIO_CODEC_EN);
#endif
    hl_mod_euc_stop();
    hl_mod_euc_deinit();

#if !HL_IS_TX_DEVICE()
    ret = hl_util_nvram_param_get_integer("MSC_OPEN", &msc_open_flag, 0);
    if (ret == 1) {
        LOG_E("nvram be used before not init");
        hl_board_nvram_init();
        ret = hl_util_nvram_param_get_integer("MSC_OPEN", &msc_open_flag, 0);
    }

    LOG_D("msc_open_flag = %d ,ret = %d ", msc_open_flag, ret);
    if (msc_open_flag == 0) {
        hl_mod_audio_deinit();
    }
#endif
    hl_mod_telink_stop();
    hl_mod_telink_deinit();
    hl_mod_input_deinit();
    hl_mod_display_deinit();
    hl_mod_pm_ctrl(HL_PM_POWER_DOWN_CMD, NULL, 0);
    //hl_mod_pm_stop();
    //hl_mod_pm_deinit();
}

// hl_app_info [no param]
int hl_app_info(int argc, char** argv)
{
#if HL_IS_TX_DEVICE()
    LOG_I("------show tx app info------");
    LOG_I("on_off_flag = %d", tx_info.on_off_flag);
    LOG_I("usb_plug = %d ", tx_info.usb_plug);
    LOG_I("uac_link_flag = %d ", tx_info.uac_link_flag);
    LOG_I("mstorage_plug = %d ", tx_info.mstorage_plug);
    LOG_I("ex_mic_plug = %d ", tx_info.ex_mic_plug);
    LOG_I("rec_flag = %d ", tx_info.rec_flag);
    LOG_I("denoise_flag = %d ", tx_info.denoise_flag);
    LOG_I("charge_flag = %d ", tx_info.charge_flag);
    LOG_I("mute_flag = %d ", tx_info.mute_flag);
    LOG_I("rf_state = %d ", tx_info.rf_state);
    LOG_I("tx_info.soc = %d ", tx_info.soc);
    LOG_I("tx_info.gain = %d ", tx_info.gain);
    LOG_I("tx_info.uac_gain = %d ", tx_info.uac_gain);
#else
    LOG_I("------show rx app info------");
    LOG_I("on_off_flag = %d", rx_info.on_off_flag);
    LOG_I("usb_plug = %d ", rx_info.usb_plug);
    LOG_I("uac_link_flag = %d ", rx_info.uac_link_flag);
    LOG_I("mstorage_plug = %d ", rx_info.mstorage_plug);
    LOG_I("hp_spk_plug = %d ", rx_info.hp_spk_plug);
    LOG_I("cam_spk_plug = %d ", rx_info.cam_spk_plug);
    LOG_I("charge_flag = %d ", rx_info.charge_flag);
    LOG_I("rf_state = %d ", rx_info.rf_state);
    LOG_I("rx_info.soc = %d ", rx_info.soc);
    LOG_I("st_cam_gain_l = %d ", rx_info.st_cam_gain_l);
    LOG_I("st_cam_gain_r = %d ", rx_info.st_cam_gain_r);
    LOG_I("hp_gain = %d ", rx_info.hp_gain);
#endif
    return 0;
}

INIT_APP_EXPORT(hl_app_mng_init);
MSH_CMD_EXPORT(hl_app_info, show app info cmd);


MSH_CMD_EXPORT(hl_app_param_reset, show app info cmd);

static void _hl_et_sche_hook(rt_thread_t from, rt_thread_t to)
{
    rt_kprintf("%s[%d]->%s[%d]\r\n", from->name, from->remaining_tick, to->name, to->remaining_tick);
}

static int set_sche_hook(int argc, char **argv)
{
    rt_scheduler_sethook(_hl_et_sche_hook);
    return 0;
}

MSH_CMD_EXPORT(set_sche_hook, set_sche_hook);
/*
 * EOF
 */