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
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void hl_app_larksound_msg_pro(mode_to_app_msg_t* p_msg)
{
#if !HL_IS_TX_DEVICE()
    uint8_t val = 0;

    switch (p_msg->cmd) {
        case LARKSOUND_GET_SOFT_VERSION_CMD:
            LOG_I("app get(%d) larksound get soft version\n", p_msg->cmd);
            break;

        case LARKSOUND_GET_HARD_VERSION_CMD:
            LOG_I("app get(%d) larksound get hard version\n", p_msg->cmd);
            break;

        case LARKSOUND_GET_CONTECT_STATE_CMD:
            LOG_I("app get(%d) larksound get RX connect state\n", p_msg->cmd);
            break;

        case LARKSOUND_GET_MAC_CMD:
            LOG_I("app get(%d) larksound get RX MAC\n", p_msg->cmd);
            break;

        case LARKSOUND_GET_SOC_CMD:
            LOG_I("app get(%d) larksound get Soc\n", p_msg->cmd);
            break;

        case LARKSOUND_GET_UV_CMD:
            LOG_I("app get(%d) larksound get UV\n", p_msg->cmd);
            break;

        case LARKSOUND_GET_DEV_NUB_CMD:
            LOG_I("app get(%d) larksound get Device Number\n", p_msg->cmd);
            break;

        case LARKSOUND_GET_TX_GAIN_CMD:
            LOG_I("app get(%d) larksound get TX Gain\n", p_msg->cmd);
            break;

        case LARKSOUND_SET_TX_GAIN_CMD:
            LOG_I("app get(%d) larksound set TX Gain\n", p_msg->cmd);
            break;

        case LARKSOUND_GET_DENOISE_CMD:
            LOG_I("app get(%d) larksound get Denoise\n", p_msg->cmd);
            break;

        case LARKSOUND_SET_DENOISE_CMD:
            LOG_I("app get(%d) larksound set Denoise\n", p_msg->cmd);
            break;

        case LARKSOUND_GET_LOWCUT_CMD:
            LOG_I("app get(%d) larksound get Lowcut\n", p_msg->cmd);
            break;

        case LARKSOUND_SET_LOWCUT_CMD:
            LOG_I("app get(%d) larksound set Lowcut\n", p_msg->cmd);
            break;

        case LARKSOUND_GET_MUTE_CMD:
            LOG_I("app get(%d) larksound get Mute\n", p_msg->cmd);
            break;

        case LARKSOUND_SET_MUTE_CMD:
            LOG_I("app get(%d) larksound set Mute\n", p_msg->cmd);
            break;

        case LARKSOUND_GET_SOUND_EFFECT_CMD:
            LOG_I("app get(%d) larksound get Sound Effect\n", p_msg->cmd);
            break;

        case LARKSOUND_SET_SOUND_EFFECT_CMD:
            LOG_I("app get(%d) larksound set Sound Effect\n", p_msg->cmd);
            break;

        case LARKSOUND_GET_PHONE_OUT_CMD:
            LOG_I("app get(%d) larksound get Phone Out\n", p_msg->cmd);
            break;

        case LARKSOUND_SET_PHONE_OUT_CMD:
            LOG_I("app get(%d) larksound set Phone Out\n", p_msg->cmd);
            break;

        case LARKSOUND_GET_UAC_GAIN_CMD:
            LOG_I("app get(%d) larksound get UAC Gain\n", p_msg->cmd);
            break;

        case LARKSOUND_SET_UAC_GAIN_CMD:
            LOG_I("app get(%d) larksound set UAC Gain\n", p_msg->cmd);
            break;

        case LARKSOUND_GET_RX_GAIN_CMD:
            LOG_I("app get(%d) larksound get RX Gain\n", p_msg->cmd);
            break;

        case LARKSOUND_SET_RX_GAIN_CMD:
            LOG_I("app get(%d) larksound set RX Gain\n", p_msg->cmd);
            break;

        case LARKSOUND_GET_CAM_GAIN_CMD:
            LOG_I("app get(%d) larksound get Cam Gain\n", p_msg->cmd);
            break;

        case LARKSOUND_SET_CAM_GAIN_CMD:
            LOG_I("app get(%d) larksound set Cam Gain\n", p_msg->cmd);
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