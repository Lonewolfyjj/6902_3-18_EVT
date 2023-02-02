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
/* Exported functions --------------------------------------------------------*/
void hl_app_msg_thread(void* parameter)
{
    hl_mod_feed_dog();
    mode_to_app_msg_t msg = { 0 };

    hl_app_mng_charger_entry(&hl_app_mq);

    hl_mod_upgrade_init(&hl_app_mq);

#if HL_IS_TX_DEVICE()
    if (tx_info.mstorage_plug == 0) {
#else
    if (rx_info.mstorage_plug == 0) {
#endif
        hl_mod_upgrade_io_ctrl(HL_UPGRADE_OPEN_CMD, NULL, 0);
    }

    rt_memset(&msg, 0, sizeof(msg));
    while (1) {
        if (rt_mq_recv(&hl_app_mq, &msg, sizeof(msg), 500) == RT_EOK) {
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
        rt_thread_mdelay(5);
        hl_mod_feed_dog();
    }
}

void hl_app_mng_init(void)
{
    rt_err_t    ret;
    rt_thread_t app_task_tid = RT_NULL;

    ret = rt_mq_init(&hl_app_mq, "AppMsg", &hl_app_msg_pool[0], 128, sizeof(hl_app_msg_pool), RT_IPC_FLAG_FIFO);
    if (ret != RT_EOK) {
        LOG_E("message queuecreate init err!!!");
        return;
    }

    hl_mod_input_init(&hl_app_mq);
    hl_mod_display_init(&hl_app_mq);
    // hl_mod_audio_init(&hl_app_mq);
    // hl_mod_telink_init(&hl_app_mq);
    // hl_mod_telink_start();
    // hl_mod_apple_auth_init(&hl_app_mq);
    hl_mod_pm_init(&hl_app_mq);
    hl_mod_pm_start();
    hl_mod_euc_init(&hl_app_mq);
    hl_mod_euc_start();  

    app_task_tid = rt_thread_create("app_task", hl_app_msg_thread, RT_NULL, 10240, 15, 5);
    if (app_task_tid) {
        rt_thread_startup(app_task_tid);
    } else {
        LOG_E("thread create err!!!");
    }

    rt_thread_mdelay(2000);
    hl_app_info(0, NULL);
}

// 开机，初始化模块
void hl_app_mng_powerOn(void)
{
    uint8_t value = 0;
    LOG_I("power on");
    hl_mod_pm_ctrl(HL_PM_POWER_UP_CMD, NULL, 0);
    // hl_mod_display_init(&hl_app_mq);
    hl_mod_telink_init(&hl_app_mq);
    hl_mod_telink_start();
    hl_mod_audio_init(&hl_app_mq);
    hl_mod_apple_auth_init(&hl_app_mq);
    hl_mod_apple_auth_start();
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
    }
    rt_kprintf("rx_info.charge_flag=%d\n", rx_info.charge_flag);
    hl_mod_display_io_ctrl(RX_CHARGE_STATUS_SWITCH_CMD, &value, 1);
#endif
}

// 关机，去初始化模块
void hl_app_mng_powerOff(void)
{
    LOG_I("power off");    
    hl_mod_euc_stop();
    hl_mod_euc_deinit();
    hl_mod_audio_deinit();
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
    LOG_I("cur_volume_r = %d ", rx_info.cur_volume_r);
    LOG_I("cur_volume_l = %d ", rx_info.cur_volume_l);
#endif
    return 0;
}

INIT_APP_EXPORT(hl_app_mng_init);
MSH_CMD_EXPORT(hl_app_info, show app info cmd);

/*
 * EOF
 */