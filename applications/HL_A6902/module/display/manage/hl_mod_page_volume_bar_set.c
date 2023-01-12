/**
 * @file hl_mod_page_volume_bar_set.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-12-20
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
 * <tr><td>2022-12-20     <td>v1.0     <td>liujie     <td>内容
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

#include "hl_util_general_type.h"
#include "lvgl.h"
#include "hl_util_timeout.h"
#include "hl_mod_page.h"
#include "lv_port_indev.h"
#include "page_barset.h"
#include "hl_mod_page_volume_bar_set.h"

//当前的音量
static int16_t last_volume = 0;

static int16_t now_volume = 0;

static hl_timeout_t volume_timeout;

static hl_timeout_t silder_trg_time;

static void hl_bar_goto_homepage_deal()
{
    PageManager_PagePop();
}

void hl_mod_page_volume_init(int16_t volume)
{
    // 设置当前音量
    hl_mod_knob_select_val_set(&now_volume, volume);
    hl_mod_knob_select_val_set(&last_volume, volume);
    hl_util_timeout_set(&volume_timeout, AUTO_GOTO_HOME_TIMER);
}

void hl_mod_page_volume_exit(void)
{
    hl_lvgl_barset_ioctl_t bar_test;
    bar_test.barset_value = HL_EXTI;
    hl_mod_barset_ioctl(&bar_test);
    // bar_test.barset_value = HL_DELETE_STYLE;
    // hl_mod_barset_ioctl(&bar_test);
}

void hl_mod_page_volume_update(int16_t volume)
{
    LOG_E("bar_num=%d\n", volume);
    hl_mod_knob_select_val_set(&now_volume, volume);
    hl_mod_knob_select_val_set(&last_volume, volume);
    hl_util_timeout_set(&volume_timeout, AUTO_GOTO_HOME_TIMER);
}

int16_t hl_mod_page_volume_get(void)
{
    return hl_mod_knob_select_val_get(&now_volume);
}

void hl_mod_page_volume_loop(hl_out_msg_e msg, save_func func, int16_t left, int16_t right)
{
    int8_t                 knob_val = 0;
    hl_lvgl_barset_ioctl_t ctrl;

    int16_t data;

    // OK按键
    uint8_t ok_btn = hl_mod_get_knob_okkey_val();
    // 旋钮选择TX配置通道
    knob_val = hl_mod_knob_select_val_change(&now_volume, left, right, false);

    // 设置触发点击事件
    if (ok_btn == HL_KEY_EVENT_SHORT) {
        // hl_bar_goto_homepage_deal();
        hl_util_timeout_set(&volume_timeout, AUTO_GOTO_HOME_TIMER);
    }

    if (knob_val) {
        ctrl.barset_value = HL_VALUE;
        ctrl.value        = hl_mod_knob_select_val_get(&now_volume);
        hl_mod_barset_ioctl(&ctrl);

        hl_util_timeout_set(&volume_timeout, AUTO_GOTO_HOME_TIMER);
    }

    if (hl_util_timeout_judge(&volume_timeout)) {
        // 超时回主菜单
        hl_util_timeout_close(&volume_timeout);
        hl_bar_goto_homepage_deal();
    }

    // 返回按键
    if (1 == hl_mod_keypad_touchkey_read()) {
        func();
        PageManager_PagePop();
    }

    if (hl_mod_page_volume_send(msg, &data)) {
        func();
    }
    // hl_mod_menu_backbtn_scan();
}

uint8_t hl_mod_page_volume_send(hl_out_msg_e msg_cmd,int16_t *get_data)
{
    int32_t volume;
    if (hl_mode_report_event(&silder_trg_time, AUTO_SILDER_TRG_TIME, last_volume)) {
        LOG_D("c=[%d]volume=[%d]\n",msg_cmd, last_volume);
        *get_data = last_volume;
        volume = last_volume;
        hl_mod_display_send_msg(msg_cmd, &volume, 0);
        return 1;
    }
    return 0;
}
/*
 * EOF
 */
