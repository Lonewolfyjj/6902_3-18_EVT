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
#include "hl_util_msg_type.h"

#define DBG_SECTION_NAME "pm"
#define DBG_LEVEL DBG_INFO
#include "rtdbg.h"

/* typedef -------------------------------------------------------------------*/

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
    bool                  init_flag;
    bool                  start_flag;
    bool                  update_flag;
    bool                  interrupt_update_flag;
    hl_mod_pm_charger_e   charger;
    rt_mq_t               msg_hd;
    rt_thread_t           pm_thread;
    int                   thread_exit_flag;
    hl_mod_pm_bat_info_st bat_info;
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
/* variables -----------------------------------------------------------------*/

static hl_mod_pm_st _pm_mod = { .init_flag             = false,
                                .start_flag            = false,
                                .update_flag           = false,
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

    p_bat_info = &(_pm_mod.bat_info);

    switch (type) {
        case HL_MOD_PM_BAT_INFO_SOC: {
            hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_SOC, &(p_bat_info->soc), sizeof(p_bat_info->soc));
            LOG_I("soc:%d . %d", p_bat_info->soc.soc, p_bat_info->soc.soc_d);
        } break;
        case HL_MOD_PM_BAT_INFO_VOL: {
            hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_VOLTAGE, &(p_bat_info->voltage), sizeof(p_bat_info->voltage));
            LOG_I("voltage:%d", p_bat_info->voltage);
        } break;
        case HL_MOD_PM_BAT_INFO_CUR: {
            hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_CURRENT, &(p_bat_info->current), sizeof(p_bat_info->current));
            LOG_I("current:%d", p_bat_info->current);
        } break;
        case HL_MOD_PM_BAT_INFO_TEMP: {
            hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_TEMP, &(p_bat_info->temp), sizeof(p_bat_info->temp));
            LOG_I("temp:%d . %d", p_bat_info->temp.temp, p_bat_info->temp.temp_d);
        } break;
        case HL_MOD_PM_BAT_INFO_SOH: {
            hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_SOH, &(p_bat_info->soh), sizeof(p_bat_info->soh));
            LOG_I("soh:%d", p_bat_info->soh);
        } break;
        case HL_MOD_PM_BAT_INFO_CYCLE: {
            hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_CYCLE_COUNT, &(p_bat_info->cycle), sizeof(p_bat_info->cycle));
            LOG_I("cycle:%d", p_bat_info->cycle);
        } break;
        default:
            break;
    }
}

static inline void _pm_interrupt_check(void)
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

static void _pm_init_state_update(void)
{
    _pm_update_bat_info(HL_MOD_PM_BAT_INFO_SOC);
    _pm_update_bat_info(HL_MOD_PM_BAT_INFO_VOL);
    _pm_update_bat_info(HL_MOD_PM_BAT_INFO_CUR);
    _pm_update_bat_info(HL_MOD_PM_BAT_INFO_TEMP);
    _pm_update_bat_info(HL_MOD_PM_BAT_INFO_SOH);
    _pm_update_bat_info(HL_MOD_PM_BAT_INFO_CYCLE);

    _mod_msg_send(HL_SOC_UPDATE_IND, &(_pm_mod.bat_info.soc.soc), sizeof(uint8_t));
}

static void _pm_thread_entry(void* arg)
{
    _pm_init_state_update();

    while (_pm_mod.thread_exit_flag == 0) {
        _pm_interrupt_check();

        rt_thread_mdelay(10);
    }

    _pm_mod.thread_exit_flag = -1;
}

/* Exported functions --------------------------------------------------------*/

int hl_mod_pm_init(rt_mq_t msg_hd)
{
    int ret;

    if (_pm_mod.init_flag == true) {
        LOG_W("pm is already inited!");
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    if (hl_drv_sy6971_init() == HL_SUCCESS) {
        _pm_mod.charger = HL_MOD_PM_CHARGER_SY6971;
        LOG_I("sy6971 charger init success!");
    } else if (hl_drv_sgm41518_init() == HL_SUCCESS) {
        _pm_mod.charger = HL_MOD_PM_CHARGER_SGM41518;
        LOG_I("sgm41518 charger init success!");
    } else {
        _pm_mod.charger = HL_MOD_PM_CHARGER_UNKNOWN;
        LOG_E("all charger init err! please check charger");
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    ret = hl_drv_cw2215_init();
    if (ret == CW2215_FUNC_RET_ERR) {
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    _guage_gpio_irq_init();
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

    _guage_gpio_irq_deinit();

    ret = hl_drv_cw2215_deinit();
    if (ret == CW2215_FUNC_RET_ERR) {
        return HL_MOD_PM_FUNC_RET_ERR;
    }

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

    _pm_mod.interrupt_update_flag = false;
    _pm_mod.update_flag     = false;

    _guage_gpio_irq_enable(true);

    _pm_mod.thread_exit_flag = 0;

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

    _guage_gpio_irq_enable(false);

    _pm_mod.thread_exit_flag = 1;

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

    switch (cmd) {
        case HL_PM_POWER_UP_CMD: {
            _power_gpio_set(GPIO_ALL_POWER, true);
        } break;
        case HL_PM_POWER_DOWN_CMD: {
            _power_gpio_set(GPIO_ALL_POWER, false);
        } break;
        default:
            break;
    }

    return HL_MOD_PM_FUNC_RET_OK;
}

/*
 * EOF
 */