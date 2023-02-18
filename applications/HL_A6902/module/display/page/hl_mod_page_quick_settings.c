/**
 * @file hl_mod_page_volume_menu.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief line out 音量设置界面
 * @version V1.0
 * @date 2022-12-12
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
 * <tr><td>2022-12-12     <td>v1.0     <td>liujie     <td>内容
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
#include "hl_util_general_type.h"
#include "lvgl.h"

#include "hl_mod_page.h"
#include "lv_port_indev.h"
#include "page_test.h"
#include "page_menu.h"
#include "hl_util_general_type.h"
#include "page_lineout.h"

#define MAX_LINEOUT_VOLUME 18
#define MIN_LINEOUT_VOLUME -18

// 0 表示左 1表示 右
static int16_t now_sound_channal = (int16_t)HL_LINEOUT_CHOOSE_LEFT;

static hl_display_sound_module_e module = MONO;

LV_IMG_DECLARE(Other_voice);

static void btn_scan();

static void page_deinit();
static void page_s_deinit(hl_display_screen_s* data);
static void page_s_init(hl_display_screen_s* data, hl_display_screen_change_s* flag);
static void page_s_voc_bar_init();
static void hl_bar_test_cb(hl_lineout_check_t bar_num);

// BACK_PAGE 页面回调函数
static void hl_bar_test_cb(hl_lineout_check_t bar_num)
{
    printf("bar_num = %d\n", bar_num);

    now_sound_channal = (int16_t)bar_num;
    page_s_voc_bar_init();
}

static void page_twolineout_init()
{
    hl_display_screen_change_s* flag = hl_mod_page_get_screen_change_flag();
    hl_display_screen_s*        data = hl_mod_page_get_screen_data_ptr();

    //  立体声
    module = data->now_sound_module;

    switch (module) {
        case STEREO:
            // 默认选择声道1
            hl_mod_knob_select_val_set(&now_sound_channal, (int16_t)HL_LINEOUT_CHOOSE_LEFT);
            page_s_init(data, flag);
            break;
        case MONO:
            break;
        case SAFE_TRACK:
            break;
        default:
            break;
    }
}

// 立体声模式的lineout初始化
static void page_s_init(hl_display_screen_s* data, hl_display_screen_change_s* flag)
{
    hl_lvgl_lineout_init_t lineout;
    LOG_D("vol1=[%d]\n", data->tx1_line_out_volume);
    lineout.lineout_choose   = HL_LINEOUT_CHOOSE_LEFT;
    lineout.left_volume      = data->tx1_line_out_volume;
    lineout.right_volume     = data->tx2_line_out_volume;
    lineout.func_cb          = hl_bar_test_cb;
    lineout.left_volume_min  = MIN_LINEOUT_VOLUME;
    lineout.left_volume_max  = MAX_LINEOUT_VOLUME;
    lineout.right_volume_min = MIN_LINEOUT_VOLUME;
    lineout.right_volume_max = MAX_LINEOUT_VOLUME;

    hl_mod_lineout_init(&lineout);
}
// 立体声反初始化
static void page_s_deinit(hl_display_screen_s* data)
{
    hl_lvgl_lineout_ioctl_t lineout;

    lineout.lineout_choose = HL_LINEOUT_DELETE_PAGE;
    hl_mod_lineout_ioctl(&lineout);
    // lineout.lineout_choose = HL_LINEOUT_DELETE_STYLE;
    // hl_mod_lineout_ioctl(&lineout);
}

// 立体声反初始化
static void page_deinit()
{
    hl_display_screen_s* data = hl_mod_page_get_screen_data_ptr();

    switch (module) {
        case STEREO:
            // 默认选择声道1
            page_s_deinit(data);
            break;
        case MONO:

            break;
        case SAFE_TRACK:
            break;
        default:
            break;
    }
}

// 进入立体声声道设置音量页面
static void page_s_voc_bar_init()
{

    if ((hl_lineout_choose_t)now_sound_channal == HL_LINEOUT_CHOOSE_LEFT) {
        PageManager_PagePush(PAGE_LINE_OUT_STEREO_LEFT);

    } else if ((hl_lineout_choose_t)now_sound_channal == HL_LINEOUT_CHOOSE_RIGHT) {
        PageManager_PagePush(PAGE_LINE_OUT_STEREO_RIGHT);
    }
}

static void hl_mod_page_setup(void)
{
    page_twolineout_init();
}

static void hl_mod_page_exit(void)
{
    page_deinit();
}

static void btn_scan()
{
    int8_t                  knob_val = 0;
    hl_lvgl_lineout_ioctl_t ctrl;
    // OK按键
    uint8_t ok_btn = hl_mod_get_knob_okkey_val();

    // 设置触发点击事件
    if (ok_btn == HL_KEY_EVENT_SHORT) {

        ctrl.lineout_choose = (hl_lineout_choose_t)now_sound_channal;
        hl_mod_lineout_ioctl(&ctrl);
    }

    // 旋钮选择TX配置通道
    knob_val = hl_mod_knob_select_val_change(&now_sound_channal, 0, 1,true);
    if (knob_val) {
        ctrl.lineout_choose = (hl_lineout_choose_t)now_sound_channal;
        hl_mod_lineout_ioctl(&ctrl);
    }
}

static void hl_mod_page_loop(void)
{
    btn_scan();
    hl_mod_menu_backbtn_scan();
}

PAGE_DEC(PAGE_QUICK_SETTINGS)
{
    bool result;

    result = PageManager_PageRegister(PAGE_QUICK_SETTINGS, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_QUICK_SETTINGS init fail\n");
    }
}

#endif
/*
 * EOF
 */
