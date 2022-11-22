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
#include "hl_app_mng.h"
#include "hl_mod_display.h"

#define DBG_SECTION_NAME "app_disp"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#if HL_IS_TX_DEVICE()
void hl_app_disp_msg_pro(mode_to_app_msg_t *p_msg)
{
    switch (p_msg->cmd) {
        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}

void hl_app_disp_state_led_set(void)
{
    hl_led_mode     led_ctrl;

    if (tx_info.denoise_flag == 0) {
        switch (tx_info.rf_state) {
            case HL_RF_STATE_IDLE:
                led_ctrl = LED_MODE_RECONNECTION;
                break;
            case HL_RF_STATE_RELEASE:
                led_ctrl = LED_MODE_RECONNECTION;
                break;
            case HL_RF_STATE_PAIR:
                led_ctrl = LED_MODE_PAIR;
                break;
            case HL_RF_STATE_CONNECT:
                led_ctrl = LED_MODE_CONNECTED;
                break;
            case HL_RF_STATE_STOP:
                led_ctrl = LED_MODE_RECONNECTION;
                break;
            default:
                break;
        }
    } else {
        switch (tx_info.rf_state) {
            case HL_RF_STATE_IDLE:
                led_ctrl = LED_MODE_DENOISE_RECONNECTION;
                break;
            case HL_RF_STATE_RELEASE:
                led_ctrl = LED_MODE_DENOISE_RECONNECTION;
                break;
            case HL_RF_STATE_PAIR:
                led_ctrl = LED_MODE_PAIR;
                break;
            case HL_RF_STATE_CONNECT:
                led_ctrl = LED_MODE_DENOISE_CONNECTED;
                break;
            case HL_RF_STATE_STOP:
                led_ctrl = LED_MODE_DENOISE_RECONNECTION;
                break;
            default:
                break;
        }
    }

    hl_mod_display_io_ctrl(MSG_STATE_LED_MODE_CMD, &led_ctrl, sizeof(led_ctrl));
}

#else
void hl_app_disp_msg_pro(mode_to_app_msg_t *p_msg)
{
    switch (p_msg->cmd) {
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