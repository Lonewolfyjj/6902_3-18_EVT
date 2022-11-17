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
#include "hl_mod_pm.h"
#include "hl_app_audio_msg_pro.h"
#include "hl_app_com_msg_pro.h"
#include "hl_app_disp_msg_pro.h"
#include "hl_app_iap2_msg_pro.h"
#include "hl_app_input_msg_pro.h"
#include "hl_app_pm_msg_pro.h"
#include "hl_app_rf_msg_pro.h"
#include "hl_app_upgrade_msg_pro.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/// 应用层消息队列，用于接收模块的消息
struct rt_messagequeue hl_app_mq;
/// 应用层消息队列池
static char            hl_app_msg_pool[2048];

/// 模块层发送给APP的消息格式
#if HL_GET_DEVICE_TYPE()
/// tx app 状态信息
tx_app_info_t tx_info = {0};
#else
/// rx app 状态信息
rx_app_info_t rx_info = {0};
#endif

/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void hl_app_msg_thread(void* parameter)
{
    mode_to_app_msg_t msg = { 0 };

    rt_memset(&msg, 0, sizeof(msg));
    while (1) {
        if (rt_mq_recv(&hl_app_mq, &msg, sizeof(msg), RT_WAITING_FOREVER) == RT_EOK) {
            // rt_kprintf("[%s][line:%d] recv msg sender:%d, cmd:%d, param:%d !!! \r\n", __FUNCTION__, __LINE__, msg.sender, msg.cmd, msg.param);
            switch (msg.sender) {
                case INPUT_MODE:
                    hl_app_input_msg_pro(&msg);
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
                    rt_kprintf("[%s][line:%d] sender(%d) unkown!!! \r\n", __FUNCTION__, __LINE__, msg.sender);
                    break;
            }
        }
    }
}

void hl_app_mng_init(void)
{
    rt_err_t    ret;
	rt_thread_t app_task_tid   = RT_NULL;

	ret = rt_mq_init(&hl_app_mq, "AppMsg", &hl_app_msg_pool[0], 128, sizeof(hl_app_msg_pool), RT_IPC_FLAG_FIFO);
    if (ret != RT_EOK) {
        rt_kprintf("[%s][line:%d] message queuecreate init err!!! \r\n", __FUNCTION__, __LINE__);
        return;
    }

	hl_mod_input_init(&hl_app_mq);
    hl_mod_display_init(&hl_app_mq);
    hl_mod_audio_init(&hl_app_mq);
    hl_mod_telink_init(&hl_app_mq);
    hl_mod_telink_start();
    hl_mod_pm_init(&hl_app_mq);
    hl_mod_pm_start();

	app_task_tid = rt_thread_create("app_task", hl_app_msg_thread, RT_NULL, 2048, 20, 2);
    if (app_task_tid) {
        rt_thread_startup(app_task_tid);
    } else {
        rt_kprintf("[%s][line:%d] thread create err!!! \r\n", __FUNCTION__, __LINE__);
    }
        
}

INIT_APP_EXPORT(hl_app_mng_init);
/*
 * EOF
 */