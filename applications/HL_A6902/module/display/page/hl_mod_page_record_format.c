
/**
 * @file hl_mod_page_record_format.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
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
#include "hl_util_general_type.h"
#include "page_language.h"

#define TX_TIME_REMAINING_PERCENT(hour) \
    (uint8_t)((((float)STROGE_MAX_USED_TIME - (float)hour) * (float)100) / (float)STROGE_MAX_USED_TIME)

static int16_t            knob_choose;
static hl_storage_check_t display_choose;

// 格式化界面
static void hl_storage_test_cb(hl_storage_check_t event_num)
{
    uint32_t                    value = 0;
    hl_display_screen_change_s* flag  = hl_mod_page_get_screen_change_flag();

    LOG_D("event_num = %d\n", event_num);

    if (display_choose == event_num) {

        switch (event_num) {
            case HL_STORAGE_CHECK_LEFT:
                // TX1格式化
                flag->channel_format_flag = 0;
                break;
            case HL_STORAGE_CHECK_RIGHT:
                // TX2格式化
                flag->channel_format_flag = 1;
                break;
            default:
                break;
        }
        PageManager_PagePush(PAGE_FORMAT);

    } else {
        display_choose = event_num;
        knob_choose    = (int16_t)event_num;
    }
}
// static void tx1_time_remaining_update(char* str, uint8_t percentage)
// {
//     float decimal;
//     int   num;
//     decimal    = (uint8_t)((float)9 * (float)percentage / 100);
//     num        = (int)decimal;
//     decimal    = decimal - (float)num;
//     percentage = decimal < 0.4 ? num : num + 1;

//     rt_sprintf(str,"%dh可用",percentage);
// }

static void tx_time_remaining_update(char* str, uint8_t remain_hour)
{
    a6902_language_typ_t* page_ptr = (a6902_language_typ_t *)hl_a6902_language_ptr_get();
    rt_sprintf(str, "%dh%s", remain_hour,page_ptr->storage_page_ptr->page_storage->ptr_storage_use);
}
// 实时更新当前使用量
// static void hl_stroge_used_update();
// {
//     uint8_t time;
//     char str[10]={0};
//     hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();
//     hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();

//     if (flag->tx1_remained_record_time) {

//         hl_mod_display_mux_take();
//         flag->tx1_remained_record_time = 0;
//         time = data_ptr->tx1_remained_record_time;
//         hl_mod_display_mux_release();

//         tx_time_remaining_update(str,time);
//         data_ptr->tx1_remained_record_time = str;
//     }
// }

static void hl_mod_page_setup(void)
{
    uint8_t              data1;
    uint8_t              data2;
    char                 time1_remaining[32];
    char                 time2_remaining[32];
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();

    data1 = data_ptr->tx1_remained_record_time;
    data2 = data_ptr->tx2_remained_record_time;

    knob_choose = (int16_t)display_choose;

    hl_lvgl_storage_init_t init = {
        .func_cb        = hl_storage_test_cb,
        .ptr_time_tx1   = NULL,
        .ptr_time_tx2   = NULL,
        .storage_choose = display_choose,
        .used_tx1       = 100,
        .used_tx2       = 100,
    };

    tx_time_remaining_update(time1_remaining, data1);
    init.ptr_time_tx1 = time1_remaining;

    init.used_tx1 = TX_TIME_REMAINING_PERCENT(data1);
    LOG_D("tx1 stroge used=%d\n", init.used_tx1);
    tx_time_remaining_update(time2_remaining, data2);
    init.ptr_time_tx2 = time2_remaining;
    init.used_tx2     = TX_TIME_REMAINING_PERCENT(data2);
    LOG_D("tx2 stroge used=%d\n", init.used_tx2);
    hl_mod_storage_init(&init);

    hl_lvgl_storage_ioctl_t storage_ctl = {
        .storage_choose = display_choose,
    };
    hl_mod_storage_ioctl(&storage_ctl);
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_storage_ioctl_t storage_ctl = {
        .storage_choose = HL_STORAGE_CHOOSE_EXIT,
    };
    hl_mod_storage_ioctl(&storage_ctl);
    // storage_ctl.storage_choose = HL_STORAGE_CHOOSE_DEL_STYLE;
    // hl_mod_storage_ioctl(&storage_ctl);
}

static void hl_mod_page_loop(void)
{
    // OK按键
    uint8_t ok_btn = hl_mod_get_knob_okkey_val();
    // 返回按键
    uint8_t back_btn = hl_mod_keypad_touchkey_read();

    // 触摸返回
    if (1 == back_btn) {

        PageManager_PagePop();
    }

    // 旋钮对配置的更改
    if (hl_mod_knob_select_val_change(&knob_choose, 0, 1, true)) {
        LOG_E("knob choose chg=%d\n", knob_choose);
        hl_lvgl_storage_ioctl_t storage_ctl;
        storage_ctl.storage_choose = (hl_storage_choose_t)knob_choose;
        hl_mod_storage_ioctl(&storage_ctl);
    }

    // OK按键
    if (ok_btn == HL_KEY_EVENT_SHORT) {
        hl_lvgl_storage_ioctl_t storage_ctl;
        storage_ctl.storage_choose = (hl_storage_choose_t)knob_choose;
        hl_mod_storage_ioctl(&storage_ctl);
    }

    // 上层对配置的更改
    // hl_stroge_used_update();
}

PAGE_DEC(PAGE_RECORD_FORMAT)
{
    bool result;

    result = PageManager_PageRegister(PAGE_RECORD_FORMAT, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("page_record_format init fail\n");
    }
}

#endif
/*
 * EOF
 */
