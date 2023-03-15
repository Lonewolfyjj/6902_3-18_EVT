/**
 * @file hl_mod_pm.c
 * @author lilin (lin.li@hollyland-tech.com)
 * @brief 电源管理模块源文件
 * @version 1.0
 * @date 2022-09-29
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
 * <tr><td>2022-09-29     <td>v1.0     <td>lilin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/

#include "hl_mod_pm.h"
#include "hl_drv_cw2215.h"
#include "hl_drv_sgm41518.h"
#include "hl_drv_sy6971.h"
#include "hl_hal_gpio.h"
#include "hl_config.h"
#include "hl_util_msg_type.h"

#define DBG_SECTION_NAME "pm"
#define DBG_LEVEL DBG_INFO
#include "rtdbg.h"

/* typedef -------------------------------------------------------------------*/
typedef int (*pm_io_ctl)(uint8_t cmd, void* ptr, uint8_t len);

typedef enum _hl_mod_pm_charger_e
{
    HL_MOD_PM_CHARGER_UNKNOWN,
    HL_MOD_PM_CHARGER_SY6971,
    HL_MOD_PM_CHARGER_SGM41518,
} hl_mod_pm_charger_e;

typedef struct _hl_mod_pm_bat_info_st
{
    hl_st_drv_guage_soc_t  soc;
    uint16_t               voltage;
    int32_t                current;
    hl_st_drv_guage_temp_t temp;
    uint8_t                soh;
    uint32_t               cycle;
} hl_mod_pm_bat_info_st;

typedef struct _hl_mod_pm_st
{
    bool                     init_flag;
    bool                     guage_init_flag;
    bool                     guage_reinit_flag;
    bool                     start_flag;
    bool                     interrupt_update_flag;
    hl_mod_pm_charger_e      charger;
    bool                     charger_charge_terminate_flag;
    hl_mod_pm_charge_state_e charge_state;
    uint8_t                  vbus_c_state;
    uint8_t                  vbus_p_state;
    bool                     charge_full_timeout_flag;
    struct rt_timer          charge_full_timer;
    struct rt_timer          shutdown_timer;
    uint32_t                 shutdown_timeout;
    rt_mq_t                  msg_hd;
    rt_thread_t              pm_thread;
    int                      thread_exit_flag;
    hl_mod_pm_bat_info_st    bat_info;
} hl_mod_pm_st;

typedef enum _hl_mod_pm_bat_info_e
{
    HL_MOD_PM_BAT_INFO_SOC = 0,
    HL_MOD_PM_BAT_INFO_VOL,
    HL_MOD_PM_BAT_INFO_CUR,
    HL_MOD_PM_BAT_INFO_TEMP,
    HL_MOD_PM_BAT_INFO_SOH,
    HL_MOD_PM_BAT_INFO_CYCLE,
} hl_mod_pm_bat_info_e;

/* define --------------------------------------------------------------------*/

#define MAX_CHARGE_TEMP 60
#define MIN_CHARGE_TEMP 0

/* variables -----------------------------------------------------------------*/

static hl_mod_pm_st _pm_mod = { .init_flag             = false,
                                .start_flag            = false,
                                .interrupt_update_flag = false,
                                .msg_hd                = NULL,
                                .pm_thread             = NULL,
                                .thread_exit_flag      = 0,
                                .bat_info              = {
                                    .soc.soc     = 0,
                                    .soc.soc_d   = 0,
                                    .current     = 0,
                                    .cycle       = 0,
                                    .soh         = 0,
                                    .temp.temp   = 0,
                                    .temp.temp_d = 0,
                                    .voltage     = 0,
                                } };

static pm_io_ctl pm_ioctl     = RT_NULL;
static uint8_t   power_ic_typ = HL_MOD_PM_CHARGER_SGM41518;
/* Private function(only *.c)  -----------------------------------------------*/

static int _mod_msg_send(uint8_t cmd, void* param, uint16_t len)
{
    if (_pm_mod.msg_hd == NULL) {
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    mode_to_app_msg_t msg;
    rt_err_t          rt_err;

    msg.sender    = PM_MODE;
    msg.cmd       = cmd;
    msg.param.ptr = param;
    msg.len       = len;

    rt_err = rt_mq_send(_pm_mod.msg_hd, (void*)&msg, sizeof(msg));
    if (RT_EOK != rt_err) {
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    return HL_MOD_PM_FUNC_RET_OK;
}

static void _charge_full_timer_timeout_handle(void* arg)
{
    LOG_I("pm charge timeout");
    _pm_mod.charge_full_timeout_flag = true;
}

static void _shutdown_timer_timeout_handle(void* arg)
{
    _mod_msg_send(HL_SHUTDOWN_TIMEOUT_IND, NULL, 0);
}

static void _guage_gpio_irq_handle(void* args)
{
    _pm_mod.interrupt_update_flag = true;
}

static inline void _guage_gpio_irq_init()
{
    hl_hal_gpio_init(GPIO_GAUGE_INT);
    hl_hal_gpio_attach_irq(GPIO_GAUGE_INT, PIN_IRQ_MODE_FALLING, _guage_gpio_irq_handle, NULL);
    hl_hal_gpio_irq_enable(GPIO_GAUGE_INT, PIN_IRQ_DISABLE);
}

static inline void _guage_gpio_irq_deinit()
{
    hl_hal_gpio_irq_enable(GPIO_GAUGE_INT, PIN_IRQ_DISABLE);
    hl_hal_gpio_deinit(GPIO_GAUGE_INT);
}

static inline void _guage_gpio_irq_enable(bool flag)
{
    if (flag) {
        hl_hal_gpio_irq_enable(GPIO_GAUGE_INT, PIN_IRQ_ENABLE);
    } else {
        hl_hal_gpio_irq_enable(GPIO_GAUGE_INT, PIN_IRQ_DISABLE);
    }
}

static void _power_gpio_init(void)
{
    hl_hal_gpio_init(GPIO_ALL_POWER);
}

static void _power_gpio_set(hl_gpio_pin_e power_gpio, bool flag)
{
    if (flag == true) {
        hl_hal_gpio_high(power_gpio);
    } else {
        hl_hal_gpio_low(power_gpio);
    }
}

static void _pm_update_bat_info(hl_mod_pm_bat_info_e type)
{
    hl_mod_pm_bat_info_st* p_bat_info;
    bool                   flag;
    int                    ret;

    ret = hl_drv_cw2215_ctrl(HL_DRV_GUAGE_CHECK_IF_READY, &flag, sizeof(flag));
    if (ret == CW2215_FUNC_RET_ERR || flag == false) {
        LOG_W("guage caculate not ready!");
        return;
    }

    p_bat_info = &(_pm_mod.bat_info);

    switch (type) {
        case HL_MOD_PM_BAT_INFO_SOC: {
            hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_SOC, &(p_bat_info->soc), sizeof(p_bat_info->soc));
        } break;
        case HL_MOD_PM_BAT_INFO_VOL: {
            hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_VOLTAGE, &(p_bat_info->voltage), sizeof(p_bat_info->voltage));
        } break;
        case HL_MOD_PM_BAT_INFO_CUR: {
            hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_CURRENT, &(p_bat_info->current), sizeof(p_bat_info->current));
        } break;
        case HL_MOD_PM_BAT_INFO_TEMP: {
            hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_TEMP, &(p_bat_info->temp), sizeof(p_bat_info->temp));
        } break;
        case HL_MOD_PM_BAT_INFO_SOH: {
            hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_SOH, &(p_bat_info->soh), sizeof(p_bat_info->soh));
        } break;
        case HL_MOD_PM_BAT_INFO_CYCLE: {
            hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_CYCLE_COUNT, &(p_bat_info->cycle), sizeof(p_bat_info->cycle));
        } break;
        default:
            break;
    }
}

static inline void _guage_interrupt_check(void)
{
    hl_drv_guage_it_flag_e it_flag;

    if (_pm_mod.interrupt_update_flag == true) {
        hl_drv_cw2215_ctrl(HL_DRV_GUAGE_CHECK_IT_FLAG, &it_flag, sizeof(it_flag));

        if (it_flag & HL_DRV_GUAGE_IT_FLAG_SOC) {
            _pm_update_bat_info(HL_MOD_PM_BAT_INFO_SOC);

            _mod_msg_send(HL_SOC_UPDATE_IND, &(_pm_mod.bat_info.soc.soc), sizeof(uint8_t));
        }

        if (it_flag & HL_DRV_GUAGE_IT_FLAG_TMAX) {
            _pm_update_bat_info(HL_MOD_PM_BAT_INFO_TEMP);

            _mod_msg_send(HL_MAX_TEMP_ALERT_IND, &(_pm_mod.bat_info.temp.temp), sizeof(uint8_t));
        }

        if (it_flag & HL_DRV_GUAGE_IT_FLAG_TMIN) {
            _pm_update_bat_info(HL_MOD_PM_BAT_INFO_TEMP);

            _mod_msg_send(HL_MIN_TEMP_ALERT_IND, &(_pm_mod.bat_info.temp.temp), sizeof(uint8_t));
        }

        hl_drv_cw2215_ctrl(HL_DRV_GUAGE_CLEAR_IT_FLAG, &it_flag, sizeof(it_flag));

        _pm_mod.interrupt_update_flag = false;
    }
}

static uint8_t _soc_convert(uint8_t soc)
{
    if (soc > 100) {
        return soc;
    } else if (soc >= 96) {
        return 100;
    } else if (soc >= 86) {
        return (soc + 5);
    } else {
        return (soc * 100 / 95);
    }
}

static void _send_soc_msg_to_app(void)
{
    static uint8_t soc = 0;

    soc = _soc_convert(_pm_mod.bat_info.soc.soc);

    _mod_msg_send(HL_SOC_UPDATE_IND, &soc, sizeof(soc));
}

static void _pm_init_state_update(void)
{
    _pm_update_bat_info(HL_MOD_PM_BAT_INFO_SOC);
    _pm_update_bat_info(HL_MOD_PM_BAT_INFO_VOL);
    _pm_update_bat_info(HL_MOD_PM_BAT_INFO_CUR);
    _pm_update_bat_info(HL_MOD_PM_BAT_INFO_TEMP);
    _pm_update_bat_info(HL_MOD_PM_BAT_INFO_SOH);
    _pm_update_bat_info(HL_MOD_PM_BAT_INFO_CYCLE);

    LOG_I("soc:%d . %d", _pm_mod.bat_info.soc.soc, _pm_mod.bat_info.soc.soc_d);
    LOG_I("voltage:%d", _pm_mod.bat_info.voltage);
    LOG_I("current:%d", _pm_mod.bat_info.current);
    LOG_I("temp:%d . %d", _pm_mod.bat_info.temp.temp, _pm_mod.bat_info.temp.temp_d);
    LOG_I("soh:%d", _pm_mod.bat_info.soh);
    LOG_I("cycle:%d", _pm_mod.bat_info.cycle);

    _pm_mod.charge_state = HL_CHARGE_STATE_UNKNOWN;

    _send_soc_msg_to_app();
}

static bool _debug_switch_flag = false;

static void hl_mod_pm_debug_bat_info_5s(void)
{
    if (_debug_switch_flag == true) {
        _debug_switch_flag = false;
    } else {
        _debug_switch_flag = true;
    }
}

MSH_CMD_EXPORT(hl_mod_pm_debug_bat_info_5s, 间隔5s打印电池信息);

static void _guage_state_update()
{
    if (_pm_mod.guage_init_flag == false) {
        return;
    }

    uint8_t soc;
    bool    flag = false;
    int     ret;
    bool    ready_flag;

    ret = hl_drv_cw2215_ctrl(HL_DRV_GUAGE_CHECK_IF_READY, &ready_flag, sizeof(ready_flag));
    if (ret == CW2215_FUNC_RET_ERR || ready_flag == false) {
        LOG_W("guage caculate not ready!");
        return;
    }

    soc = _pm_mod.bat_info.soc.soc;

    _pm_update_bat_info(HL_MOD_PM_BAT_INFO_SOC);
    _pm_update_bat_info(HL_MOD_PM_BAT_INFO_VOL);
    _pm_update_bat_info(HL_MOD_PM_BAT_INFO_CUR);
    _pm_update_bat_info(HL_MOD_PM_BAT_INFO_TEMP);
    _pm_update_bat_info(HL_MOD_PM_BAT_INFO_SOH);
    _pm_update_bat_info(HL_MOD_PM_BAT_INFO_CYCLE);

    if (soc != _pm_mod.bat_info.soc.soc && _pm_mod.bat_info.soc.soc <= 100) {
        _send_soc_msg_to_app();
        flag = true;
    }

    if (flag == true || _debug_switch_flag == true) {
        LOG_I("------bat log------");
        LOG_I("soc:%d . %d", _pm_mod.bat_info.soc.soc, _pm_mod.bat_info.soc.soc_d * 10000 / 256);
        LOG_I("voltage:%d", _pm_mod.bat_info.voltage);
        LOG_I("current:%d", _pm_mod.bat_info.current);
        LOG_I("temp:%d . %d", _pm_mod.bat_info.temp.temp, _pm_mod.bat_info.temp.temp_d);
        LOG_I("soh:%d", _pm_mod.bat_info.soh);
        LOG_I("cycle:%d", _pm_mod.bat_info.cycle);
    }

    if (_pm_mod.bat_info.soc.soc <= 3 && hl_hal_gpio_read(GPIO_VBUS_DET) == PIN_HIGH
        && hl_hal_gpio_read(GPIO_PBUS_DET) == PIN_HIGH) {
        _mod_msg_send(HL_ULTRA_LOWPOWER_IND, NULL, 0);
    }
}

static void _guage_err_monitor(void)
{
    int  ret;
    char chip_id;
    bool flag;

    ret = hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_CHIP_ID, &chip_id, sizeof(chip_id));
    if (ret == CW2215_FUNC_RET_ERR || chip_id != CW2215_CHIP_ID) {  //通信不正常
        LOG_W("guage bad i2c!");
        if (_pm_mod.guage_init_flag == true) {
            _pm_mod.guage_init_flag = false;
            // _mod_msg_send(HL_MOD_PM_GUAGE_ERR_MSG, &(_pm_mod.guage_init_flag), sizeof(bool));
        }
    } else {  //通信正常
        ret = hl_drv_cw2215_ctrl(HL_DRV_GUAGE_CHIP_SELF_CHECK, &flag, sizeof(flag));
        if (ret == CW2215_FUNC_RET_ERR) {
            return;
        }

        if (flag == true) {  //需要重新初始化
            if (_pm_mod.guage_init_flag == true) {
                _pm_mod.guage_init_flag = false;
                // _mod_msg_send(HL_MOD_PM_GUAGE_ERR_MSG, &(_pm_mod.guage_init_flag), sizeof(bool));
            }

            LOG_W("reinit guage!");
            ret = hl_drv_cw2215_deinit();
            if (ret == CW2215_FUNC_RET_OK) {
                ret = hl_drv_cw2215_init();
                if (ret == CW2215_FUNC_RET_OK) {
                    LOG_W("guage need calibration!");
                    _pm_mod.guage_reinit_flag = true;
                }
            }
        } else {  //不需要重新初始化
            if (_pm_mod.guage_init_flag == false) {
                LOG_W("guage good again!");
                _pm_mod.guage_init_flag = true;
                // _mod_msg_send(HL_MOD_PM_GUAGE_ERR_MSG, &(_pm_mod.guage_init_flag), sizeof(bool));
            }
        }
    }
}

static void _guage_state_poll()
{
    static uint16_t count = 0;

    if (count == 0) {
        _guage_err_monitor();
        _guage_state_update();
        count = 20;
    } else {
        count--;
    }
}

static void _guage_reinit_check_poll(void)
{
    if (_pm_mod.guage_reinit_flag == false || _pm_mod.guage_init_flag == false) {  //不需要校准或者电量计不正常
        _pm_mod.guage_reinit_flag = false;
        return;
    }

    int ret;

    if (hl_hal_gpio_read(GPIO_VBUS_DET) != PIN_HIGH || hl_hal_gpio_read(GPIO_PBUS_DET) != PIN_HIGH) {  //插入充电电源
        if (_pm_mod.charger_charge_terminate_flag == false) {  //充电状态未终止
            return;
        }
    }

    LOG_W("calibration guage!");
    ret = hl_drv_cw2215_deinit();
    if (ret == CW2215_FUNC_RET_OK) {
        ret = hl_drv_cw2215_init();
        if (ret == CW2215_FUNC_RET_OK) {
            LOG_W("guage calibration ok!");
        }
    }

    _pm_mod.guage_reinit_flag = false;
}

static void _charger_charge_state_update(void)
{
    HL_SY_INPUT_PARAM_T      sy6971_param;
    HL_SGM_INPUT_PARAM_T     sgm_param;
    hl_mod_pm_charge_state_e charge_state = HL_CHARGE_STATE_UNKNOWN;

    if (_pm_mod.charger == HL_MOD_PM_CHARGER_SY6971) {
        sy6971_param.cfg_opt = E_CHRG_STAT;
        hl_drv_sy6971_io_ctrl(SY_READ_CMD, &sy6971_param, 1);

        if (sy6971_param.param == 0) {
            charge_state = HL_CHARGE_STATE_NO_CHARGE;
        } else if (sy6971_param.param == 1) {
            charge_state = HL_CHARGE_STATE_CHARGING;
        } else if (sy6971_param.param == 2) {
            charge_state = HL_CHARGE_STATE_CHARGING;
        } else if (sy6971_param.param == 3) {
            charge_state = HL_CHARGE_STATE_CHARGE_DONE;
        }
    } else if (_pm_mod.charger == HL_MOD_PM_CHARGER_SGM41518) {
        sgm_param.cfg_opt = R_CHRG_STAT;
        hl_drv_sgm41518_io_ctrl(SGM_READ_CMD, &sgm_param, 1);

        if (sgm_param.param == 0) {
            charge_state = HL_CHARGE_STATE_NO_CHARGE;
        } else if (sgm_param.param == 1) {
            charge_state = HL_CHARGE_STATE_CHARGING;
        } else if (sgm_param.param == 2) {
            charge_state = HL_CHARGE_STATE_CHARGING;
        } else if (sgm_param.param == 3) {
            charge_state = HL_CHARGE_STATE_CHARGE_DONE;
        }
    } else {
        return;
    }

    if (charge_state == HL_CHARGE_STATE_CHARGE_DONE) {
        if (_pm_mod.charger_charge_terminate_flag == false) {
            _pm_mod.charger_charge_terminate_flag = true;
            LOG_I("charger charge terminate!");
        }
    } else {
        if (_pm_mod.charger_charge_terminate_flag == true) {
            _pm_mod.charger_charge_terminate_flag = false;
            LOG_I("charger charge not terminate!");
        }
    }
}

static void _charger_charge_state_poll(void)
{
    static uint8_t count = 0;

    if (count == 0) {
        _charger_charge_state_update();
        count = 10;
    } else {
        count--;
    }
}

static void _charger_fault_state_update(void)
{
}

static void _charger_fault_state_poll(void)
{
    static uint16_t count = 0;

    if (count == 0) {
        _charger_fault_state_update();
        count = 500;
    } else {
        count--;
    }
}

static void _charge_state_judge(void)
{
    static bool              flag         = false;
    hl_mod_pm_charge_state_e charge_state = HL_CHARGE_STATE_UNKNOWN;

    if (_pm_mod.vbus_c_state == 1 || _pm_mod.vbus_p_state == 1) {
        if (_pm_mod.charge_full_timeout_flag == true) {
            charge_state = HL_CHARGE_STATE_CHARGE_FULL_DONE;
        } else {
            if (_pm_mod.charger_charge_terminate_flag == true) {
                if (_pm_mod.bat_info.soc.soc != 100) {
                    // 重新初始化电量计
                }
                charge_state = HL_CHARGE_STATE_CHARGE_FULL_DONE;
            } else {
                if (_pm_mod.bat_info.soc.soc >= 95) {
                    charge_state = HL_CHARGE_STATE_CHARGE_DONE;
                } else {
                    charge_state = HL_CHARGE_STATE_CHARGING;
                }
            }
        }
    } else {
        charge_state                          = HL_CHARGE_STATE_NO_CHARGE;
        _pm_mod.charger_charge_terminate_flag = false;
    }

    if (charge_state != _pm_mod.charge_state) {
        LOG_I("%d %d", charge_state, _pm_mod.charge_state);
        _pm_mod.charge_state = charge_state;
        _mod_msg_send(HL_CHARGE_STATE_IND, &(_pm_mod.charge_state), sizeof(_pm_mod.charge_state));
    }
}

static void _charge_full_timer_set(void)
{
    static bool flag = false;

    if (_pm_mod.vbus_p_state == 1
        && (_pm_mod.guage_init_flag == false
            || (_pm_mod.bat_info.temp.temp > MIN_CHARGE_TEMP && _pm_mod.bat_info.temp.temp < MAX_CHARGE_TEMP))) {
        if (flag == false) {
            flag = true;
            rt_timer_start(&(_pm_mod.charge_full_timer));
            _pm_mod.charge_full_timeout_flag = false;
            LOG_I("start charge-full timer!");
        }
    } else {
        if (flag == true) {
            flag = false;
            rt_timer_stop(&(_pm_mod.charge_full_timer));
            _pm_mod.charge_full_timeout_flag = false;
            LOG_I("stop charge-full timer!");
        }
    }
}

static void _hl_mod_pmwdg(void)
{
    HL_SY_INPUT_PARAM_T wdg_sy = {
        .cfg_opt = E_WD_RST,
        .param   = 1,
    };
    HL_SGM_INPUT_PARAM_T wdg_sgm = {
        .cfg_opt = RW_WDG_RST,
        .param   = 1,
    };
    if (power_ic_typ == HL_MOD_PM_CHARGER_SY6971) {
        pm_ioctl(SY_WRITE_CMD, &wdg_sy, 1);
    }
    if (power_ic_typ == HL_MOD_PM_CHARGER_SGM41518) {
        pm_ioctl(SGM_WRITE_CMD, &wdg_sgm, 1);
    }
}

static void _hl_mod_pm_input_check(void)
{
    if (power_ic_typ == HL_MOD_PM_CHARGER_SGM41518) {
        return;
    }
    HL_SY_INPUT_PARAM_T input_stat = {
        .cfg_opt = E_IDPM_STAT,
    };
    HL_SY_INPUT_PARAM_T input_data = {
        .cfg_opt = E_FORCE_AICL,
        .param   = 1,
    };
    pm_ioctl(SY_READ_CMD, &input_stat, 1);
    if (input_stat.param == 1) {
        pm_ioctl(SY_WRITE_CMD, &input_data, 1);
    }
}

static void _pm_thread_entry(void* arg)
{
    uint16_t delay_time = 0;

    _pm_init_state_update();

    while (_pm_mod.thread_exit_flag == 0) {
#if 0
        _guage_interrupt_check();
#else
        _guage_state_poll();
#endif

        _guage_reinit_check_poll();

        _charger_charge_state_poll();
        // _charger_fault_state_poll();
        _charge_state_judge();
        _charge_full_timer_set();

        rt_thread_mdelay(100);
        if (delay_time++ > 50) {
            delay_time = 0;
            _hl_mod_pmwdg();
            _hl_mod_pm_input_check();
        }
    }

    _pm_mod.thread_exit_flag = -1;
}

/* Exported functions --------------------------------------------------------*/

int hl_mod_pm_init(rt_mq_t msg_hd)
{
    int                  ret;
    HL_SGM_INPUT_PARAM_T sgm_param;
    bool                 flag;

    if (_pm_mod.init_flag == true) {
        LOG_W("pm is already inited!");
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    if (hl_drv_sy6971_init() == HL_SUCCESS) {
        power_ic_typ    = HL_MOD_PM_CHARGER_SY6971;
        _pm_mod.charger = HL_MOD_PM_CHARGER_SY6971;
        pm_ioctl        = hl_drv_sy6971_io_ctrl;
        LOG_I("sy6971 charger init success!");
    } else if (hl_drv_sgm41518_init() == HL_SUCCESS) {
        power_ic_typ    = HL_MOD_PM_CHARGER_SGM41518;
        _pm_mod.charger = HL_MOD_PM_CHARGER_SGM41518;
        pm_ioctl        = hl_drv_sgm41518_io_ctrl;
        LOG_I("sgm41518 charger init success!");

        sgm_param.cfg_opt = RW_EN_BAT_CHARGING;
        sgm_param.param   = 0;
        hl_drv_sgm41518_io_ctrl(SGM_WRITE_CMD, &sgm_param, 1);
        sgm_param.param = 1;
        hl_drv_sgm41518_io_ctrl(SGM_WRITE_CMD, &sgm_param, 1);
    } else {
        _pm_mod.charger = HL_MOD_PM_CHARGER_UNKNOWN;
        LOG_E("all charger init err! please check charger");
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    ret = hl_drv_cw2215_ctrl(HL_DRV_GUAGE_CHIP_SELF_CHECK, &flag, sizeof(flag));
    if (ret != CW2215_FUNC_RET_ERR) {
        if (flag == true) {
            LOG_W("guage need calibration!");
            _pm_mod.guage_reinit_flag = true;
        } else {
            LOG_W("guage no need calibration!");
            _pm_mod.guage_reinit_flag = false;
        }
    }

    ret = hl_drv_cw2215_init();
    if (ret == CW2215_FUNC_RET_ERR) {
        _pm_mod.guage_init_flag = false;
    } else {
        _pm_mod.guage_init_flag = true;
    }

    rt_timer_init(&(_pm_mod.charge_full_timer), "pm_timer", _charge_full_timer_timeout_handle, RT_NULL,
                  1000 * 60 * 60 * 4, RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);

    _pm_mod.shutdown_timeout = 1000 * 60 * 15;

    rt_timer_init(&(_pm_mod.shutdown_timer), "pm_timer", _shutdown_timer_timeout_handle, RT_NULL,
                  _pm_mod.shutdown_timeout, RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);
#if 0
    _guage_gpio_irq_init();
#endif
    _power_gpio_init();

    _pm_mod.msg_hd = msg_hd;

    LOG_D("pm init success!");

    _pm_mod.init_flag = true;

    return HL_MOD_PM_FUNC_RET_OK;
}

int hl_mod_pm_deinit(void)
{
    int ret;

    if (_pm_mod.init_flag == false) {
        LOG_W("pm is already deinited!");
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    hl_mod_pm_stop();

    rt_timer_detach(&(_pm_mod.charge_full_timer));
#if 0
    _guage_gpio_irq_deinit();

    ret = hl_drv_cw2215_deinit();
    if (ret == CW2215_FUNC_RET_ERR) {
        return HL_MOD_PM_FUNC_RET_ERR;
    }
#endif

    _pm_mod.msg_hd = NULL;

    LOG_D("pm deinit success!");

    _pm_mod.init_flag = false;

    return HL_MOD_PM_FUNC_RET_OK;
}

int hl_mod_pm_start(void)
{
    int ret;

    if (_pm_mod.init_flag == false) {
        LOG_E("pm is not init!");
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    if (_pm_mod.start_flag == true) {
        ret = hl_mod_pm_stop();
        if (ret == HL_MOD_PM_FUNC_RET_ERR) {
            LOG_W("pm try stop failed");
            return HL_MOD_PM_FUNC_RET_ERR;
        }
    }

    _pm_mod.interrupt_update_flag         = false;
    _pm_mod.charge_state                  = HL_CHARGE_STATE_UNKNOWN;
    _pm_mod.vbus_c_state                  = 0;
    _pm_mod.vbus_p_state                  = 0;
    _pm_mod.charge_full_timeout_flag      = false;
    _pm_mod.charger_charge_terminate_flag = false;
#if 0
    _guage_gpio_irq_enable(true);
#endif
    _pm_mod.thread_exit_flag = 0;

#if HL_IS_TX_DEVICE()
    rt_timer_start(&(_pm_mod.shutdown_timer));
#endif

    _pm_mod.pm_thread = rt_thread_create("hl_mod_pm_thread", _pm_thread_entry, RT_NULL, 1024, 25, 10);
    if (_pm_mod.pm_thread == RT_NULL) {
        LOG_E("pm thread create failed");
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    rt_thread_startup(_pm_mod.pm_thread);

    LOG_I("pm start success");

    _pm_mod.start_flag = true;

    return HL_MOD_PM_FUNC_RET_OK;
}

int hl_mod_pm_stop(void)
{
    if (_pm_mod.init_flag == false) {
        LOG_E("pm is not init!");
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    if (_pm_mod.start_flag == false) {
        LOG_W("pm already stop");
        return HL_MOD_PM_FUNC_RET_ERR;
    }

#if 0
    _guage_gpio_irq_enable(false);
#endif
    _pm_mod.thread_exit_flag = 1;

    rt_timer_stop(&(_pm_mod.shutdown_timer));

    LOG_I("wait pm thread exit");

    while (_pm_mod.thread_exit_flag != -1) {
        rt_thread_mdelay(10);
    }

    LOG_I("pm stop success");

    _pm_mod.start_flag = false;

    return HL_MOD_PM_FUNC_RET_OK;
}

int hl_mod_pm_ctrl(hl_mod_pm_cmd_e cmd, void* arg, int arg_size)
{
    if (_pm_mod.init_flag == false) {
        LOG_E("pm is not init!");
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    int ret;

    switch (cmd) {
        case HL_PM_POWER_UP_CMD: {
            _power_gpio_set(GPIO_ALL_POWER, true);
        } break;
        case HL_PM_POWER_DOWN_CMD: {
            int i = 100;
            while (_pm_mod.guage_reinit_flag == true && i--) {
                rt_thread_mdelay(10);
            }
            _power_gpio_set(GPIO_ALL_POWER, false);
        } break;
        case HL_PM_BAT_INFO_UPDATE_CMD: {
            _pm_init_state_update();
        } break;
        case HL_PM_SET_VBUS_C_STATE_CMD: {
            _pm_mod.vbus_c_state = *((uint8_t*)arg);
        } break;
        case HL_PM_SET_VBUS_P_STATE_CMD: {
            _pm_mod.vbus_p_state = *((uint8_t*)arg);
        } break;
        case HL_PM_SET_SHUTDOWN_TIMEOUT_CMD: {
            _pm_mod.shutdown_timeout = *((uint32_t*)arg);
            rt_timer_control(&(_pm_mod.shutdown_timer), RT_TIMER_CTRL_SET_TIME, &(_pm_mod.shutdown_timeout));
        } break;
        case HL_PM_START_SHUTDOWN_TIMER_CMD: {
            rt_timer_start(&(_pm_mod.shutdown_timer));
        } break;
        case HL_PM_STOP_SHUTDOWN_TIMER_CMD: {
            rt_timer_stop(&(_pm_mod.shutdown_timer));
        } break;
        default:
            break;
    }

    return HL_MOD_PM_FUNC_RET_OK;
}

/*
 * EOF
 */