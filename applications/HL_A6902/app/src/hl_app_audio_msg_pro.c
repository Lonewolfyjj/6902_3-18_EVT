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
#include "hl_mod_audio.h"

#define DBG_SECTION_NAME "app_audio"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#if HL_IS_TX_DEVICE()
void hl_app_audio_msg_pro(mode_to_app_msg_t *p_msg)
{
    switch (p_msg->cmd) {
        case HL_AUDIO_UAC_LINK_IND:
            tx_info.uac_link_flag = 1;
            hl_app_audio_stream_updata();
            LOG_I("app recv uac link indicate");
            break;
        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}
#else
void hl_app_audio_msg_pro(mode_to_app_msg_t *p_msg)
{

    switch (p_msg->cmd) {
        case HL_AUDIO_UAC_LINK_IND:
            rx_info.uac_link_flag = 1;
            hl_app_audio_stream_updata();
            LOG_I("app recv uac link indicate");
            break;
        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
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
#else
    if (rx_info.uac_link_flag != 0) {
        stream_mode = HL_STREAM_CAP2UAC_UAC2PLAY;
        // stream_mode = HL_STREAM_CAP2PLAY_CAP2UAC;
        hl_mod_audio_io_ctrl(HL_AUDIO_STREAM_SET_CMD, &stream_mode, sizeof(hl_stream_mode_e)); 
    } else {
        stream_mode = HL_STREAM_CAP2PLAY;
        hl_mod_audio_io_ctrl(HL_AUDIO_STREAM_SET_CMD, &stream_mode, sizeof(hl_stream_mode_e));         
    }
#endif
}

/*
 * EOF
 */