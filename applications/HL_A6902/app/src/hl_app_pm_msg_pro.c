/**
 * @file hl_app_pm_msg_pro.c
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
#include "hl_board_commom.h"

#include "hl_mod_pm.h"
#include "hl_mod_display.h"
#include "hl_mod_telink.h"

#define DBG_SECTION_NAME "app_pm"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
void hl_app_pm_charge_pro(hl_mod_pm_charge_state_e charge_state)
{
    hl_rf_bypass_state_t bypass_state;

    uint8_t state = 0;
#if HL_IS_TX_DEVICE()
    if (charge_state == HL_CHARGE_STATE_NO_CHARGE) {
        LOG_D("no charge!");
        tx_info.charge_flag = 0;
        state               = 0;
    } else if (charge_state == HL_CHARGE_STATE_CHARGING) {
        LOG_D("charging!");
        tx_info.charge_flag = 1;
        state               = 1;
    } else if (charge_state == HL_CHARGE_STATE_CHARGE_DONE) {
        LOG_D("charge done!");
        tx_info.charge_flag = 2;
        state               = 2;
    } else if (charge_state == HL_CHARGE_STATE_CHARGE_FULL_DONE) {
        LOG_D("charge full done!");
        tx_info.charge_flag = 3;
        state               = 2;
    }

    bypass_state.chn   = tx_info.rf_chn;
    bypass_state.state = tx_info.charge_flag == 1 ? HL_RF_ON : HL_RF_OFF;
    hl_mod_telink_ioctl(HL_RF_BYPASS_CHARGE_CMD, &bypass_state, sizeof(bypass_state));
    hl_mod_display_io_ctrl(LED_CHARGE_STATUS_CMD, &state, 1);
#else
    if (charge_state == HL_CHARGE_STATE_NO_CHARGE) {
        LOG_D("no charge!");
        rx_info.charge_flag = 0;
        state               = 0;
    } else if (charge_state == HL_CHARGE_STATE_CHARGING) {
        LOG_D("charging!");
        rx_info.charge_flag = 1;
        state               = 1;
    } else if (charge_state == HL_CHARGE_STATE_CHARGE_DONE) {
        LOG_D("charge done!");
        rx_info.charge_flag = 2;
        state               = 2;
    } else if (charge_state == HL_CHARGE_STATE_CHARGE_FULL_DONE) {
        LOG_D("charge full done!");
        rx_info.charge_flag = 3;
        state               = 2;
    }

    hl_mod_display_io_ctrl(RX_CHARGE_STATUS_SWITCH_CMD, &state, 1);
#endif
}

static bool _use_no_operation_timer_flag = true;

void hl_app_pm_timer_set(void)
{
    if (_use_no_operation_timer_flag == false) {
        return;
    }

    hl_mod_pm_cmd_e cmd;

#if HL_IS_TX_DEVICE()
    if (tx_info.usb_pogo_flag == true || tx_info.usb_plug == true
        || tx_info.rf_state != HL_RF_UNCONNECT && tx_info.rf_state != HL_RF_PAIRING || tx_info.rec_flag == true) {
        cmd = HL_PM_STOP_SHUTDOWN_TIMER_CMD;
        LOG_I("stop No-Operation-Timer!");
    } else {
        cmd = HL_PM_START_SHUTDOWN_TIMER_CMD;
        LOG_I("start No-Operation-Timer!");
    }

    hl_mod_pm_ctrl(cmd, RT_NULL, 0);
#else
    // if (rx_info.usb_pogo_flag == true || rx_info.usb_plug == true
    //     || rx_info.rf_state != HL_RF_UNCONNECT && rx_info.rf_state != HL_RF_PAIRING) {
    //     cmd = HL_PM_STOP_SHUTDOWN_TIMER_CMD;
    //     LOG_I("stop No-Operation-Timer!");
    // } else {
    //     cmd = HL_PM_START_SHUTDOWN_TIMER_CMD;
    //     LOG_I("start No-Operation-Timer!");
    // }

    // hl_mod_pm_ctrl(cmd, RT_NULL, 0);
#endif
}

static void hl_app_pm_close_no_operation_timer(void)
{
    _use_no_operation_timer_flag = false;
    hl_mod_pm_ctrl(HL_PM_STOP_SHUTDOWN_TIMER_CMD, RT_NULL, 0);
    LOG_I("close no-operation-timer");
}

MSH_CMD_EXPORT(hl_app_pm_close_no_operation_timer, close no operation timer);
/* Exported functions --------------------------------------------------------*/
#if HL_IS_TX_DEVICE()
void hl_app_pm_msg_pro(mode_to_app_msg_t* p_msg)
{
    uint8_t              soc_temp;
    int8_t               temperature_temp;
    hl_rf_bypass_value_t tx_bat;

    switch (p_msg->cmd) {
        case HL_SOC_UPDATE_IND:
            soc_temp    = *(uint8_t*)p_msg->param.ptr;
            tx_info.soc = soc_temp;
            hl_mod_display_io_ctrl(LED_BATTERY_VAL_CMD, &soc_temp, 1);
            tx_bat.chn = tx_info.rf_state - 1;
            tx_bat.val = soc_temp;
            hl_mod_telink_ioctl(HL_RF_BYPASS_BATTERY_CMD, &tx_bat, sizeof(tx_bat));
            // LOG_I("current soc:%d", tx_info.soc);
            break;

        case HL_MAX_TEMP_ALERT_IND:
            temperature_temp = *(int8_t*)p_msg->param.ptr;
            LOG_I("max temp alert:%d", temperature_temp);
            break;

        case HL_MIN_TEMP_ALERT_IND:
            temperature_temp = *(int8_t*)p_msg->param.ptr;
            LOG_I("min temp alert:%d", temperature_temp);
            break;

        case HL_CHARGE_STATE_IND:
            hl_app_pm_charge_pro(*(hl_mod_pm_charge_state_e*)p_msg->param.ptr);
            break;

        case HL_ULTRA_LOWPOWER_IND:
            LOG_I("extreme lowpower! need power off");
            hl_board_reboot();
            break;

        case HL_SHUTDOWN_TIMEOUT_IND:
            LOG_I("No operation! power off");
            hl_board_reboot();
            break;

        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}
#else
void hl_app_pm_msg_pro(mode_to_app_msg_t* p_msg)
{
    uint8_t soc_temp;
    int8_t temperature_temp;

    switch (p_msg->cmd) {
        case HL_SOC_UPDATE_IND:
            soc_temp = *(uint8_t*)p_msg->param.ptr;
            rx_info.soc = soc_temp;
            hl_mod_display_io_ctrl(RX_BAT_VAL_VAL_CMD, &soc_temp, 1);
            // LOG_I("current soc:%d", rx_info.soc);
            break;

        case HL_MAX_TEMP_ALERT_IND:
            temperature_temp = *(int8_t*)p_msg->param.ptr;
            LOG_I("max temp alert:%d", temperature_temp);
            break;

        case HL_MIN_TEMP_ALERT_IND:
            temperature_temp = *(int8_t*)p_msg->param.ptr;
            LOG_I("min temp alert:%d", temperature_temp);
            break;

        case HL_CHARGE_STATE_IND:
            hl_app_pm_charge_pro(*(hl_mod_pm_charge_state_e*)p_msg->param.ptr);
            break;

        case HL_ULTRA_LOWPOWER_IND:
            LOG_I("extreme lowpower! need power off");
            hl_board_reboot();
            break;

        case HL_SHUTDOWN_TIMEOUT_IND:
            LOG_I("No operation! power off");
            hl_board_reboot();
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