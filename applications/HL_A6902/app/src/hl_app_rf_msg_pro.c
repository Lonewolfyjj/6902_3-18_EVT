/**
 * @file hl_app_rf_msg_pro.c
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
#include "hl_app_disp_msg_pro.h"
#include "hl_mod_display.h"
#include "hl_mod_telink.h"

#define DBG_SECTION_NAME "app_rf"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#if HL_IS_TX_DEVICE()
void hl_app_rf_msg_pro(mode_to_app_msg_t *p_msg)
{
    hl_led_mode     led_ctrl;
    uint8_t         *p_param;

    LOG_D("get telink msg(%d)!!! \r\n", p_msg->cmd);
    switch (p_msg->cmd) {
        case HL_MOD_TELINK_PAIR_START_IND:
            tx_info.rf_state = HL_RF_STATE_PAIR;
            break;
        case HL_MOD_TELINK_PAIR_STOP_IND:
            tx_info.rf_state = HL_RF_STATE_RELEASE;
            break;
        case HL_MOD_TELINK_PAIR_INFO_IND:
            p_param  = (uint8_t*)p_msg->param.ptr;
            LOG_D("telink info(%d,%d,%d)\r\n", p_param[0], p_param[1], p_param[2]);
            if (p_param[0] == 0x01) {
                tx_info.rf_state = HL_RF_STATE_PAIR;
            } else if ((p_param[1] == 0) && (p_param[2] == 0)) {
                tx_info.rf_state = HL_RF_STATE_RELEASE;
            } else {
                tx_info.rf_state = HL_RF_STATE_CONNECT;
            }
            break;

        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
    hl_app_disp_state_led_set();
}
#else
void hl_app_rf_msg_pro(mode_to_app_msg_t *p_msg)
{
    hl_led_mode     led_ctrl;
    uint8_t         *p_param;

    LOG_D("get telink msg(%d)!!! \r\n", p_msg->cmd);
    switch (p_msg->cmd) {
        case HL_MOD_TELINK_PAIR_START_IND:
            led_ctrl = LED_MODE_PAIR;
            rx_info.rf_state = HL_RF_STATE_PAIR;
            hl_mod_display_io_ctrl(MSG_STATE_LED_MODE_CMD, &led_ctrl, sizeof(led_ctrl));
            break;
        case HL_MOD_TELINK_PAIR_STOP_IND:
            led_ctrl = LED_MODE_RECONNECTION;
            rx_info.rf_state = HL_RF_STATE_RELEASE;
            hl_mod_display_io_ctrl(MSG_STATE_LED_MODE_CMD, &led_ctrl, sizeof(led_ctrl));
            break;
        case HL_MOD_TELINK_PAIR_INFO_IND:
            p_param  = (uint8_t*)p_msg->param.ptr;
            LOG_D("telink info(%d,%d,%d)\r\n", p_param[0], p_param[1], p_param[2]);
            if (p_param[0] == 0x01) {
                led_ctrl = LED_MODE_PAIR;
                rx_info.rf_state = HL_RF_STATE_PAIR;
            } else if ((p_param[1] == 0) && (p_param[2] == 0)) {
                led_ctrl = LED_MODE_RECONNECTION;
                rx_info.rf_state = HL_RF_STATE_RELEASE;
            } else {
                led_ctrl = LED_MODE_CONNECTED;
                rx_info.rf_state = HL_RF_STATE_CONNECT;
            }
            hl_mod_display_io_ctrl(MSG_STATE_LED_MODE_CMD, &led_ctrl, sizeof(led_ctrl));
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