/**
 * @file hl_app_mng_charger.c
 * @author yangxianyun (rd52@hollyland-tech.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-03
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
 * <tr><td>2023-01-03     <td>v1.0     <td>yangxianyun     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "hl_app_mng.h"
#include "hl_util_msg_type.h"
#include "rtthread.h"
#include "hl_hal_gpio.h"
#include "rtdevice.h"
#include "hl_mod_pm.h"
#include "hl_mod_display.h"

#define DBG_SECTION_NAME "app_charger"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>
/* typedef -------------------------------------------------------------------*/
typedef enum _charger_powe_on_stm_e_
{
    EM_CHARGER_POWER_ON_STM_IDLE = 0x00,
    EM_CHARGER_POWER_ON_STM_CATCH,
    EM_CHARGER_POWER_ON_STM_HOLD,
    EM_CHARGER_POWER_ON_STM_PROCESS,
} hl_charger_power_on_stm_e;
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
static uint8_t charger_alive = 1;
/* Private function(only *.c)  -----------------------------------------------*/
static void _hl_app_mng_charger_power_on_stm()
{
    uint8_t                          param      = OUTBOX_OFFCHARGE_LOGO;
    static uint16_t                  hold_times = 0;
    static hl_charger_power_on_stm_e state      = EM_CHARGER_POWER_ON_STM_IDLE;
    switch (state) {
        case EM_CHARGER_POWER_ON_STM_IDLE:
            if (hl_hal_gpio_read(GPIO_PWR_KEY) == PIN_LOW) {
                hold_times = 50;
                state++;
            }
            break;
        case EM_CHARGER_POWER_ON_STM_CATCH:
            LOG_D("charge catch\r\n");
            if (hl_hal_gpio_read(GPIO_PWR_KEY) == PIN_LOW) {
                state++;
            } else {
                state = EM_CHARGER_POWER_ON_STM_IDLE;
            }
            break;
        case EM_CHARGER_POWER_ON_STM_HOLD:
            if (hl_hal_gpio_read(GPIO_PWR_KEY) == PIN_LOW) {
                hold_times--;
                if (!hold_times) {
                    state++;
                }
            } else {
                state = EM_CHARGER_POWER_ON_STM_IDLE;
            }
            break;
        case EM_CHARGER_POWER_ON_STM_PROCESS:
            LOG_D("charge process\r\n");
            if (hl_hal_gpio_read(GPIO_PWR_KEY) == PIN_LOW) {
                // 告诉显示模块正常开机
                hl_mod_display_io_ctrl(OUT_BOX_CHARGER_SWITCH_CMD, &param, 1);
                hl_app_mng_powerOn();
#if HL_IS_TX_DEVICE()
                tx_info.on_off_flag = 1;
#else
                rx_info.on_off_flag = 1;
#endif
                charger_alive = 0;
            } else {
            }
            state = EM_CHARGER_POWER_ON_STM_IDLE;
            break;
    }
}

static void _hl_app_mng_charger_charge_pro(hl_mod_pm_charge_state_e charge_state)
{
#if HL_IS_TX_DEVICE()
    hl_charge_mode state = 0;
    if (charge_state == HL_CHARGE_STATE_NO_CHARGE) {
        LOG_D("no charge!");
        tx_info.charge_flag = 0;
        state               = 0;
    } else if (charge_state == HL_CHARGE_STATE_CHARGING) {
        LOG_D("charging!");
        tx_info.charge_flag = 1;
        state               = CHARGING;
    } else if (charge_state == HL_CHARGE_STATE_CHARGE_DONE) {
        LOG_D("charge done!");
        tx_info.charge_flag = 2;
        state               = FULL_CHARGE;
    }
    hl_mod_display_io_ctrl(LED_CHARGE_STATUS_CMD, &state, sizeof(state));
#else
    uint8_t state = 0;
    if (charge_state == HL_CHARGE_STATE_NO_CHARGE) {
        LOG_D("no charge!");
        rx_info.charge_flag = 0;
        state = 0;
    } else if (charge_state == HL_CHARGE_STATE_CHARGING) {
        LOG_D("charging!");
        rx_info.charge_flag = 1;
        state = 1;
    } else if (charge_state == HL_CHARGE_STATE_CHARGE_DONE) {
        LOG_D("charge done!");
        rx_info.charge_flag = 2;
        state = 2;
    }
    state = OUTBOX_OFFCHARGE_OFFPAGE;
    if (hl_hal_gpio_read(GPIO_PWR_KEY) == PIN_HIGH) {
        hl_mod_display_io_ctrl(OUT_BOX_CHARGER_SWITCH_CMD, &state, 1);
    }
#endif
}

static void _hl_app_mng_charger_pm_process(mode_to_app_msg_t* p_msg)
{
    uint8_t soc_temp;
    int8_t  temperature_temp;

    switch (p_msg->cmd) {
        case HL_SOC_UPDATE_IND:
            soc_temp = *(uint8_t*)p_msg->param.ptr;
#if HL_IS_TX_DEVICE()
            tx_info.soc = soc_temp;
            hl_mod_display_io_ctrl(LED_BATTERY_VAL_CMD, &soc_temp, 1);
            LOG_I("current soc:%d", tx_info.soc);
#else
            rx_info.soc = soc_temp;
            hl_mod_display_io_ctrl(RX_BAT_VAL_VAL_CMD, &soc_temp, 1);
            LOG_I("current soc:%d", rx_info.soc);
#endif
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
            _hl_app_mng_charger_charge_pro(*(hl_mod_pm_charge_state_e*)p_msg->param.ptr);
            break;

        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}
/* Exported functions --------------------------------------------------------*/
void hl_app_mng_charger_entry(void* msg_q)
{
    struct rt_messagequeue* app_mq = msg_q;
    mode_to_app_msg_t       msg    = { 0 };

    while (charger_alive) {
        if (hl_hal_gpio_read(GPIO_VBUS_DET) == PIN_HIGH && hl_hal_gpio_read(GPIO_PWR_KEY) == PIN_HIGH) {
            hl_app_mng_powerOn();
            hl_app_mng_powerOff();
        }

        if (rt_mq_recv(app_mq, &msg, sizeof(msg), 10) == RT_EOK) {
            // LOG_D("recv msg sender:%d, cmd:%d, param:%d !!!", msg.sender, msg.cmd, msg.param);
            switch (msg.sender) {
                case PM_MODE:
                    _hl_app_mng_charger_pm_process(&msg);
                    break;
                default:
                    LOG_E("sender(%d) unkown!!!", msg.sender);
                    break;
            }
        }
        _hl_app_mng_charger_power_on_stm();
        rt_thread_mdelay(10);
    }
    LOG_D("charge exit\r\n");
    hl_util_msg_queue_clear(app_mq);
}
/*
 * EOF
 */