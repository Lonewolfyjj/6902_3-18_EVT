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
#include "hl_mod_euc.h"
#include "hl_mod_input.h"
#include "hl_mod_audio.h"
#include "hl_mod_apple_auth.h"
#include "hl_app_audio_msg_pro.h"
#include "hl_mod_wdog.h"

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
static hl_charger_power_on_stm_e sg_stm_charger_pwr_key_state = EM_CHARGER_POWER_ON_STM_IDLE;
static uint8_t                   charger_alive                = 1;
/* Private function(only *.c)  -----------------------------------------------*/
static void _hl_app_mng_charger_power_on_stm()
{
    static uint16_t hold_times = 0;
    switch (sg_stm_charger_pwr_key_state) {
        case EM_CHARGER_POWER_ON_STM_IDLE:
            if (hl_hal_gpio_read(GPIO_PWR_KEY) == PIN_LOW) {
                hold_times = 50;
                sg_stm_charger_pwr_key_state++;
            }
            break;
        case EM_CHARGER_POWER_ON_STM_CATCH:
            LOG_D("charge catch\r\n");
            if (hl_hal_gpio_read(GPIO_PWR_KEY) == PIN_LOW) {
                sg_stm_charger_pwr_key_state++;
            } else {
                sg_stm_charger_pwr_key_state = EM_CHARGER_POWER_ON_STM_IDLE;
            }
            break;
        case EM_CHARGER_POWER_ON_STM_HOLD:
            if (hl_hal_gpio_read(GPIO_PWR_KEY) == PIN_LOW) {
                hold_times--;
                if (!hold_times) {
                    sg_stm_charger_pwr_key_state++;
                }
            } else {
                sg_stm_charger_pwr_key_state = EM_CHARGER_POWER_ON_STM_IDLE;
            }
            break;
        case EM_CHARGER_POWER_ON_STM_PROCESS:
            LOG_D("charge process\r\n");
#if HL_IS_TX_DEVICE()
            tx_info.on_off_flag = 1;
#else
            uint8_t param = OUTBOX_OFFCHARGE_LOGO;
            // 告诉显示模块正常开机
            hl_mod_display_io_ctrl(OUT_BOX_CHARGER_SWITCH_CMD, &param, 1);
            rx_info.on_off_flag = 1;
#endif
            hl_app_mng_powerOn();
            charger_alive                = 0;
            sg_stm_charger_pwr_key_state = EM_CHARGER_POWER_ON_STM_IDLE;
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
        state = OUTBOX_OFFCHARGE_OFFPAGE;
    } else if (charge_state == HL_CHARGE_STATE_CHARGING) {
        LOG_D("charging!");
        rx_info.charge_flag = 1;
        state = OUTBOX_OFFCHARGE_OFFPAGE;
    } else if (charge_state == HL_CHARGE_STATE_CHARGE_DONE) {
        LOG_D("charge done!");
        rx_info.charge_flag = 2;
        state = OUTBOX_OFFCHARGE_OFFPAGE;
    }

    if (hl_hal_gpio_read(GPIO_PWR_KEY) == PIN_HIGH) {
        hl_mod_display_io_ctrl(OUT_BOX_CHARGER_SWITCH_CMD, &state, 1);
    }
    if (rx_info.charge_flag == 1) {
        state = 1;
    } else {
        state = 0;
    }
    hl_mod_display_io_ctrl(RX_CHARGE_STATUS_SWITCH_CMD, &state, 1);
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

static void _hl_app_mng_charger_input_process(mode_to_app_msg_t* p_msg)
{
    uint8_t usb_state      = 0;
    uint8_t hp_state       = 0;
    uint8_t cam_plug_state = 0;

    switch (p_msg->cmd) {
#if HL_IS_TX_DEVICE()
        case MSG_TX_VBUS_DET: {
            if (p_msg->param.u32_param == 0) {
                tx_info.usb_plug = 0;
            } else {
                tx_info.usb_plug = 1;
            }
            hl_mod_pm_ctrl(HL_PM_SET_VBUS_C_STATE_CMD, &(p_msg->param.u32_param), sizeof(uint8_t));
        } break;
        case MSG_TX_PVBUS_DET: {
            if (p_msg->param.u32_param == 0) {
                // to be done
            } else {
                // to be done
            }
            hl_mod_pm_ctrl(HL_PM_SET_VBUS_P_STATE_CMD, &(p_msg->param.u32_param), sizeof(uint8_t));
        } break;
#else
        case MSG_RX_VBUS_DET:
            if (p_msg->param.u32_param == 0) {
                rx_info.usb_plug = 0;
                usb_state = 0;
                rx_info.uac_link_flag = 0;
                hl_mod_audio_io_ctrl(HL_USB_MSTORAGE_DISABLE_CMD, NULL, 0);
                rx_info.mstorage_plug = 0;
                hl_mod_appleauth_ioctl(HL_APPLE_AUTH_STOP_CMD);
            } else {
                rx_info.usb_plug = 1;
                usb_state = 1;
                hl_mod_appleauth_ioctl(HL_APPLE_AUTH_START_CMD);
            }
            hl_app_audio_stream_updata();
            hl_mod_display_io_ctrl(USB_IN_SWITCH_CMD, &usb_state, 1);
            usb_state = 1;
            hl_mod_display_io_ctrl(SCREEN_OFF_STATUS_SWITCH_CMD, &usb_state, 1);
            hl_mod_pm_ctrl(HL_PM_SET_VBUS_C_STATE_CMD, &(p_msg->param.u32_param), sizeof(uint8_t));
            LOG_D("MSG_RX_VBUS_DET:(%d) \r\n", p_msg->param.u32_param);
            break;

        case MSG_RX_HP_DET:
            if (p_msg->param.u32_param == 0) {
                rx_info.hp_spk_plug = 0;
                hp_state = 0;

            } else {
                rx_info.hp_spk_plug = 1;
                hp_state = 1;
            }
            hl_mod_display_io_ctrl(MONITOR_IN_SWITCH_CMD, &hp_state, 1);
            hp_state = 1;
            hl_mod_display_io_ctrl(SCREEN_OFF_STATUS_SWITCH_CMD, &hp_state, 1);
            LOG_D("MSG_RX_HP_DET:(%d) \r\n", p_msg->param.u32_param);
            break;

        case MSG_RX_CAM_DET:
            if (p_msg->param.u32_param == 0) {
                rx_info.cam_spk_plug = 0;
                cam_plug_state = 0;
            } else {
                rx_info.cam_spk_plug = 1;
                cam_plug_state = 1;
            }
            hl_mod_display_io_ctrl(LINE_OUT_IN_SWITCH_CMD, &cam_plug_state, 1);
            cam_plug_state = 1;
            hl_mod_display_io_ctrl(SCREEN_OFF_STATUS_SWITCH_CMD, &cam_plug_state, 1);
            LOG_D("MSG_RX_CAM_DET:(%d) \r\n", p_msg->param.u32_param);
            break;

        case MSG_RX_PVBUS_DET:
            if (p_msg->param.u32_param == 0) {
                // to be done
            } else {
                // to be done
            }
            hl_mod_pm_ctrl(HL_PM_SET_VBUS_P_STATE_CMD, &(p_msg->param.u32_param), sizeof(uint8_t));
            break;
#endif

        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}

static void hl_app_mng_charger_goto_power_on()
{
    sg_stm_charger_pwr_key_state = EM_CHARGER_POWER_ON_STM_PROCESS;
}

static bool _in_box_flag    = false;
static bool _shut_down_flag = false;

#if HL_IS_TX_DEVICE()
static void _hl_app_mng_charger_euc_process(mode_to_app_msg_t* p_msg)
{
    uint8_t                   dev_num;
    uint8_t                   bat_soc_temp      = 50;
    hl_mod_euc_charge_state_e charge_state_temp = HL_MOD_EUC_CHARGE_STATE_CHARGING;
    uint8_t                   turn_on_state;

    switch (p_msg->cmd) {
        case HL_IN_BOX_IND: {
            _in_box_flag = true;
            dev_num      = *(uint8_t*)p_msg->param.ptr;
            LOG_I("in box! dev_num:%d", dev_num);
        } break;
        case HL_OUT_BOX_IND: {
            _in_box_flag = false;
            LOG_I("out box!");
            if (_shut_down_flag == false) {
                hl_app_mng_charger_goto_power_on();
            }
        } break;
        case HL_GET_SOC_REQ_IND: {  //请求获取电量
            bat_soc_temp = tx_info.soc;
            hl_mod_euc_ctrl(HL_SET_SOC_CMD, &bat_soc_temp, sizeof(bat_soc_temp));
        } break;
        case HL_GET_CHARGE_STATE_REQ_IND: {  // 请求获取充电状态
            charge_state_temp = tx_info.charge_flag + 1;
            hl_mod_euc_ctrl(HL_SET_CHARGE_STATE_CMD, &charge_state_temp, sizeof(charge_state_temp));
        } break;
        case HL_GET_TURN_ON_STATE_REQ_IND: {
            turn_on_state = 0;
            hl_mod_euc_ctrl(HL_SET_TURN_ON_STATE_CMD, &turn_on_state, sizeof(turn_on_state));
        } break;
        case HL_SHUT_DOWN_REQ_IND: {
            _shut_down_flag = true;
        } break;
        default:
            break;
    }
}
#else
static void _hl_app_mng_charger_euc_process(mode_to_app_msg_t* p_msg)
{
    uint8_t dev_num;
    uint8_t bat_soc_temp = 50;
    hl_mod_euc_charge_state_e charge_state_temp = HL_MOD_EUC_CHARGE_STATE_CHARGING;
    uint8_t turn_on_state;

    switch (p_msg->cmd) {
        case HL_IN_BOX_IND: {
            _in_box_flag = true;
            dev_num = *(uint8_t*)p_msg->param.ptr;
            LOG_I("in box! dev_num:%d", dev_num);
        } break;
        case HL_OUT_BOX_IND: {
            _in_box_flag = false;
            LOG_I("out box!");
            if (_shut_down_flag == false) {
                hl_app_mng_charger_goto_power_on();
            }
        } break;
        case HL_GET_SOC_REQ_IND: {  //请求获取电量
            bat_soc_temp = rx_info.soc;
            hl_mod_euc_ctrl(HL_SET_SOC_CMD, &bat_soc_temp, sizeof(bat_soc_temp));
        } break;
        case HL_GET_CHARGE_STATE_REQ_IND: {  // 请求获取充电状态
            charge_state_temp = rx_info.charge_flag + 1;
            hl_mod_euc_ctrl(HL_SET_CHARGE_STATE_CMD, &charge_state_temp, sizeof(charge_state_temp));
        } break;
        case HL_GET_TURN_ON_STATE_REQ_IND: {
            turn_on_state = 0;
            hl_mod_euc_ctrl(HL_SET_TURN_ON_STATE_CMD, &turn_on_state, sizeof(turn_on_state));
        } break;
        case HL_SHUT_DOWN_REQ_IND: {
            _shut_down_flag = true;
        } break;
        default:
            break;
    }
}
#endif

MSH_CMD_EXPORT(hl_app_mng_charger_goto_power_on, startup the device);

static bool _hl_app_mng_check_power_on_state(void)
{
    if (hl_hal_gpio_read(GPIO_VBUS_DET) == PIN_HIGH && hl_hal_gpio_read(GPIO_PBUS_DET) == PIN_HIGH
        && hl_hal_gpio_read(GPIO_PWR_KEY) == PIN_HIGH) {
        return true;
    } else {
        return false;
    }
}
/* Exported functions --------------------------------------------------------*/
void hl_app_mng_charger_entry(void* msg_q)
{
    struct rt_messagequeue* app_mq = msg_q;
    mode_to_app_msg_t       msg    = { 0 };

    while (charger_alive) {
        hl_mod_feed_dog();
        if (_hl_app_mng_check_power_on_state()) {
            if (_in_box_flag == false || _shut_down_flag == true) {
                hl_app_mng_powerOn();
                hl_app_mng_powerOff();
            }
        }

        if (rt_mq_recv(app_mq, &msg, sizeof(msg), 10) == RT_EOK) {
            // LOG_D("recv msg sender:%d, cmd:%d, param:%d !!!", msg.sender, msg.cmd, msg.param);
            switch (msg.sender) {
                case PM_MODE:
                    _hl_app_mng_charger_pm_process(&msg);
                    break;
                case INPUT_MODE:
                    _hl_app_mng_charger_input_process(&msg);
                    break;
                case EXT_COM_MODE:
                    _hl_app_mng_charger_euc_process(&msg);
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