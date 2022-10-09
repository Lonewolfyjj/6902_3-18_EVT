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
#include "hl_hal_gpio.h"

/* typedef -------------------------------------------------------------------*/

typedef struct _hl_mod_pm_bat_info
{
    hl_st_drv_guage_soc_t  soc;
    uint16_t               voltage;
    int32_t                current;
    hl_st_drv_guage_temp_t temp;
    uint8_t                soh;
    uint32_t               cycle;
} hl_mod_pm_bat_info_st;

typedef struct _hl_mod_pm
{
    bool                  init_flag;
    bool                  start_flag;
    bool                  soc_update_flag;
    void*                 msg_hd;
    rt_thread_t           pm_thread;
    int                   thread_exit_flag;
    hl_mod_pm_bat_info_st bat_info;
} hl_mod_pm_st;

/* define --------------------------------------------------------------------*/

#define DBG_LOG rt_kprintf

/* variables -----------------------------------------------------------------*/

static hl_mod_pm_st _pm_mod = { .init_flag        = false,
                                .start_flag       = false,
                                .soc_update_flag  = false,
                                .msg_hd           = NULL,
                                .pm_thread        = NULL,
                                .thread_exit_flag = 0,
                                .bat_info         = {
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

static void _soc_gpio_irq_handle(void* args)
{
    _pm_mod.soc_update_flag = true;
}

static inline void _guage_soc_gpio_irq_init()
{
    hl_hal_gpio_init(GPIO_GAUGE_INT);
    hl_hal_gpio_attach_irq(GPIO_GAUGE_INT, PIN_IRQ_MODE_FALLING, _soc_gpio_irq_handle, NULL);
    hl_hal_gpio_irq_enable(GPIO_GAUGE_INT, PIN_IRQ_DISABLE);
}

static inline void _guage_soc_gpio_irq_deinit()
{
    hl_hal_gpio_irq_enable(GPIO_GAUGE_INT, PIN_IRQ_DISABLE);
    hl_hal_gpio_deinit(GPIO_GAUGE_INT);
}

static inline void _guage_soc_gpio_irq_enable(bool flag)
{
    if (flag) {
        hl_hal_gpio_irq_enable(GPIO_GAUGE_INT, PIN_IRQ_ENABLE);
    } else {
        hl_hal_gpio_irq_enable(GPIO_GAUGE_INT, PIN_IRQ_DISABLE);
    }
}

static void _pm_update_bat_info(void)
{
    hl_mod_pm_bat_info_st* p_bat_info;

    p_bat_info = &(_pm_mod.bat_info);

    DBG_LOG("update bat info\n");

    hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_SOC, &(p_bat_info->soc), sizeof(p_bat_info->soc));
    DBG_LOG("soc:%d . %d\n", p_bat_info->soc.soc, p_bat_info->soc.soc_d);
    hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_VOLTAGE, &(p_bat_info->voltage), sizeof(p_bat_info->voltage));
    DBG_LOG("voltage:%d\n", p_bat_info->voltage);
    hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_CURRENT, &(p_bat_info->current), sizeof(p_bat_info->current));
    DBG_LOG("current:%d\n", p_bat_info->current);
    hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_TEMP, &(p_bat_info->temp), sizeof(p_bat_info->temp));
    DBG_LOG("temp:%d . %d\n", p_bat_info->temp.temp, p_bat_info->temp.temp_d);
    hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_SOH, &(p_bat_info->soh), sizeof(p_bat_info->soh));
    DBG_LOG("soh:%d\n", p_bat_info->soh);
    hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_CYCLE_COUNT, &(p_bat_info->cycle), sizeof(p_bat_info->cycle));
    DBG_LOG("cycle:%d\n", p_bat_info->cycle);
}

static void _pm_update_bat_info_when_irq(void)
{
    hl_drv_guage_check_it_flag_st it_flag;
    uint8_t                       flag;

    it_flag.it_flag = HL_DRV_GUAGE_IT_FLAG_SOC;
    it_flag.ret     = 0;

    hl_drv_cw2215_ctrl(HL_DRV_GUAGE_CHECK_IT_FLAG, &it_flag, sizeof(it_flag));

    if (it_flag.ret == 1) {
        flag = HL_DRV_GUAGE_IT_FLAG_SOC;

        hl_drv_cw2215_ctrl(HL_DRV_GUAGE_CLEAR_IT_FLAG, &flag, sizeof(flag));

        _pm_update_bat_info();
    }
}

static void _pm_thread_entry(void* arg)
{
    _pm_update_bat_info();

    while (_pm_mod.thread_exit_flag == 0) {
        if (_pm_mod.soc_update_flag == true) {
            _pm_update_bat_info_when_irq();

            _pm_mod.soc_update_flag = false;
        }

        rt_thread_mdelay(10);
    }

    _pm_mod.thread_exit_flag = -1;
}

/* Exported functions --------------------------------------------------------*/

int hl_mod_pm_init(void* msg_hd)
{
    int ret;

    if (_pm_mod.init_flag == true) {
        DBG_LOG("pm is already inited!\n");
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    ret = hl_drv_cw2215_init();
    if (ret == CW2215_FUNC_RET_ERR) {
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    _guage_soc_gpio_irq_init();

    _pm_mod.msg_hd = msg_hd;

    DBG_LOG("pm init success!\n");

    _pm_mod.init_flag = true;

    return HL_MOD_PM_FUNC_RET_OK;
}

int hl_mod_pm_deinit(void)
{
    int ret;

    if (_pm_mod.init_flag == false) {
        DBG_LOG("pm is already deinited!\n");
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    hl_mod_pm_stop();

    _guage_soc_gpio_irq_deinit();

    ret = hl_drv_cw2215_deinit();
    if (ret == CW2215_FUNC_RET_ERR) {
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    _pm_mod.msg_hd = NULL;

    DBG_LOG("pm deinit success!\n");

    _pm_mod.init_flag = false;

    return HL_MOD_PM_FUNC_RET_OK;
}

int hl_mod_pm_start(void)
{
    int ret;

    if (_pm_mod.init_flag == false) {
        DBG_LOG("pm is not init!\n");
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    if (_pm_mod.start_flag == true) {
        ret = hl_mod_pm_stop();
        if (ret == HL_MOD_PM_FUNC_RET_ERR) {
            DBG_LOG("pm try stop failed\n");
            return HL_MOD_PM_FUNC_RET_ERR;
        }
    }

    _pm_mod.soc_update_flag = false;

    _guage_soc_gpio_irq_enable(true);

    _pm_mod.thread_exit_flag = 0;

    _pm_mod.pm_thread = rt_thread_create("hl_mod_pm_thread", _pm_thread_entry, RT_NULL, 1024, 25, 10);
    if (_pm_mod.pm_thread == RT_NULL) {
        DBG_LOG("pm thread create failed\n");
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    rt_thread_startup(_pm_mod.pm_thread);

    DBG_LOG("pm start success\n");

    _pm_mod.start_flag = true;

    return HL_MOD_PM_FUNC_RET_OK;
}

int hl_mod_pm_stop(void)
{
    if (_pm_mod.init_flag == false) {
        DBG_LOG("pm is not init!\n");
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    if (_pm_mod.start_flag == false) {
        DBG_LOG("pm already stop\n");
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    _guage_soc_gpio_irq_enable(false);

    _pm_mod.thread_exit_flag = 1;

    DBG_LOG("wait pm thread exit\n");

    while (_pm_mod.thread_exit_flag != -1) {
        rt_thread_mdelay(10);
    }

    DBG_LOG("pm stop success\n");

    _pm_mod.start_flag = false;

    return HL_MOD_PM_FUNC_RET_OK;
}

int hl_mod_pm_ctrl(int op, void* arg, int arg_size)
{
    if (_pm_mod.init_flag == false) {
        DBG_LOG("pm is not init!\n");
        return HL_MOD_PM_FUNC_RET_ERR;
    }

    return HL_MOD_PM_FUNC_RET_OK;
}

/*
 * EOF
 */