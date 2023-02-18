
/**
 * @file hl_mod_page_systime_set.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 时间设置页面
 * @version V1.0
 * @date 2022-12-09
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
 * <tr><td>2022-12-09     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/*
 * EOF
 */
#include "hl_mod_page_common.h"

#if (!HL_IS_TX_DEVICE())
#include "hl_mod_display.h"
#include "lvgl.h"

#include "hl_mod_page.h"
#include "lv_port_indev.h"
#include "page_test.h"
#include "page_menu.h"
#include "page_date.h"
#include "hl_util_general_type.h"

static hl_display_systime_s systime_recode;
static uint8_t              now_time_flag = 0;

//时间设置界面
static void hl_date_test_cb(hl_date_choose_t opt, int16_t data)
{
    rt_kprintf("opt=%d,data=%d\n", opt, data);

    switch (opt) {
        case HL_DATE_YEAR:
            systime_recode.year = data;
            break;
        case HL_DATE_MONTH:
            systime_recode.month = data;
            break;
        case HL_DATE_DAY:
            systime_recode.day = data;
            break;
        case HL_DATE_HOUR:
            systime_recode.hour = data;
            break;
        case HL_DATE_MIN:
            systime_recode.min = data;
            break;
        default:
            return;
            break;
    }
}

// static void hl_mod_page_update()
// {

// }

static void hl_mod_page_setup(void)
{
    hl_mod_display_send_msg(SYSTIME_GET_VAL_IND, NULL, 0);
    now_time_flag = 1;
}

static void hl_mod_page_start(void)
{
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();

    hl_lvgl_date_init_t data = {
        .current_choose = HL_DATE_YEAR,
        .day            = 23,
        .func_cb        = hl_date_test_cb,
        .hour           = 13,
        .min            = 46,
        .month          = 12,
        .year           = 2022,
    };

    data.year  = data_ptr->systime.year;
    data.month = data_ptr->systime.month;
    data.hour  = data_ptr->systime.hour;
    data.day   = data_ptr->systime.day;
    data.min   = data_ptr->systime.min;

    systime_recode.year  = data.year;
    systime_recode.month = data.month;
    systime_recode.day   = data.day;
    systime_recode.hour  = data.hour;
    systime_recode.min   = data.min;

    hl_mod_date_init(&data);

    hl_lvgl_date_ioctl_t date_ctl = {
        .date_cmd = HL_DATE_SET_OPT_CMD,
        .opt      = HL_DATE_YEAR,
    };
    hl_mod_date_ioctl(&date_ctl);

    // rt_thread_mdelay(10);
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_date_ioctl_t date_ctl = {
        .date_cmd = HL_DATE_EXTI_CMD,
        .opt      = HL_DATE_YEAR,
    };
    hl_mod_date_ioctl(&date_ctl);
    // date_ctl.date_cmd = HL_DATE_DELETE_STYLE_CMD;
    // hl_mod_date_ioctl(&date_ctl);
}

static void hl_mod_page_loop(void)
{
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();

    hl_display_screen_change_s* flag = hl_mod_page_get_screen_change_flag();
    // OK按键
    uint8_t ok_btn = hl_mod_get_knob_okkey_val();
    // 返回按键
    uint8_t back_btn = hl_mod_keypad_touchkey_read();

    int8_t knob = hl_mod_get_rx_knob_val();

    if (now_time_flag == 1 && flag->systime) {

        hl_mod_page_start();

        hl_mod_display_mux_take();
        now_time_flag = 2;
        flag->systime = 0;
        hl_mod_display_mux_release();
    }

    if (now_time_flag == 2) {
        if (ok_btn == HL_KEY_EVENT_SHORT) {
            hl_mod_display_send_msg(SYSTIME_SET_VAL_IND, &systime_recode, sizeof(systime_recode));
            LOG_D("y=%d", systime_recode.year);
            LOG_D("m=%d", systime_recode.month);
            LOG_D("d=%d", systime_recode.day);
            LOG_D("h=%d", systime_recode.hour);
            LOG_D("m=%d", systime_recode.min);
            PageManager_PagePop();
        } else if (back_btn == 1) {
            PageManager_PagePop();
        }
    }
}

PAGE_DEC(PAGE_SYS_TIME_SET)
{
    bool result;

    result = PageManager_PageRegister(PAGE_SYS_TIME_SET, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_SYS_TIME_SET init fail\n");
    }
}

#endif
/*
 * EOF
 */
