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
#include "hl_mod_input.h"
#include "lvgl.h"

#include "hl_mod_page.h"
#include "lv_port_indev.h"
#include "page_test.h"
#include "page_menu.h"
#include "hl_mod_input.h"
#include "page_lineout.h"

#define SET_PAGE 1
#define BACK_PAGE 2

static uint8_t page_state = SET_PAGE;
// 0 表示左 1表示 右
static int16_t now_sound_channal = (int16_t)HL_LINEOUT_CHOOSE_LEFT;
//当前的音量
static int16_t now_volume = 0;


LV_IMG_DECLARE(Other_voice);

static void page_s_init(hl_display_screen_s*        data, hl_display_screen_change_s* flag);
static void page_voc_bar1_init(hl_display_screen_s*        data, hl_display_screen_change_s* flag);

static void hl_bar_test_cb(int16_t bar_num)
{
    printf("bar_num = %d\n", bar_num);
}

static void hl_bar1_test_cb(int16_t bar_num)
{
    printf("bar_num = %d\n", bar_num);
}
static void hl_bar2_test_cb(int16_t bar_num)
{
    printf("bar_num = %d\n", bar_num);
}

static void page_twolineout_init()
{
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();
    hl_display_screen_s*        data = hl_mod_page_get_screen_data_ptr();

    //  立体声
    if (flag->sound_module == STEREO) {
        page_state = BACK_PAGE;
        hl_mod_knob_select_val_set(&now_sound_channal,(int16_t)HL_LINEOUT_CHOOSE_LEFT);
        page_s_init(data, flag);
    // 单身道
    } else if (flag->sound_module == MONO) {
        // page_voc_bar1_init(data, flag)
    } else if (flag->sound_module == SAFE_TRACK) {

    }
}

// 立体声
static void page_s_init(hl_display_screen_s*        data, hl_display_screen_change_s* flag)
{
    hl_lvgl_lineout_init_t lineout;

    lineout.lineout_choose = HL_LINEOUT_CHOOSE_LEFT;
    lineout.left_volume = data->tx1_line_out_volume;
    lineout.right_volume = data->tx2_line_out_volume;
    lineout.func_cb = hl_bar_test_cb;
    hl_mod_lineout_init(&lineout);
}
// 立体声反初始化
static void page_s_deinit(hl_display_screen_s*        data, hl_display_screen_change_s* flag)
{
    hl_lvgl_lineout_ioctl_t lineout;

    lineout.lineout_choose = HL_LINEOUT_DELETE_PAGE;
    hl_mod_lineout_ioctl(&lineout);
}

// 立体声反初始化
static void page_deinit()
{
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();
    hl_display_screen_s*        data = hl_mod_page_get_screen_data_ptr();
    //  立体声
    if (flag->sound_module == STEREO) {
        page_s_deinit(data, flag);
    // 单身道
    } else if (flag->sound_module == MONO) {
        // page_voc_bar1_init(data, flag)
    } else if (flag->sound_module == SAFE_TRACK) {

    }
}

//左声道设置界面
static void page_voc_bar1_init(hl_display_screen_s*        data, hl_display_screen_change_s* flag)
{
    hl_lvgl_barset_init_t bar_test = 
    {
        .func_cb = hl_bar1_test_cb,
        .icontyp = HL_SINGLE_ICON,
        .init_value = data->tx1_line_out_volume,
        .ptr = "左声道",
        .range_max = 40,
        .range_min = -60,
        .src = &Other_voice,
    };
    hl_mod_barset_init(&bar_test);
}

// 进入立体声声道设置音量页面
static void page_s_voc_bar_init()
{
    hl_lvgl_barset_init_t bar_test;
    hl_display_screen_s*        data = hl_mod_page_get_screen_data_ptr();

        
        bar_test.icontyp = HL_STEREO_ICON;
        
        if (now_sound_channal == 0) {
            bar_test.func_cb = hl_bar1_test_cb;
            bar_test.init_value = data->tx1_line_out_volume;
            bar_test.ptr = "左声道";
        } else {
            bar_test.func_cb = hl_bar2_test_cb;
            bar_test.init_value = data->tx2_line_out_volume;
            bar_test.ptr = "右声道";
        }
        
        bar_test.range_max = 40;
        bar_test.range_min = -60;
        bar_test.src = &Other_voice;

    hl_mod_barset_init(&bar_test);
}

// //右声道设置界面
// static void page_voc_bar2_init(void)
// {
//     hl_lvgl_barset_init_t bar_test = 
//     {
//         .func_cb = hl_bar_test_cb,
//         .icontyp = HL_STEREO_ICON,
//         .init_value = -16,
//         .ptr = "右声道",
//         .range_max = 40,
//         .range_min = -60,
//         .src = &Other_voice,
//     };
//     hl_mod_barset_init(&bar_test);
// }

static void hl_mod_page_setup(void)
{
    page_twolineout_init();
}

static void hl_mod_page_exit(void)
{
    switch (page_state)
    {
    case BACK_PAGE:
        // 返回主页
        page_deinit();
        break;
    case SET_PAGE:
        // 返回显示界面
        page_state = BACK_PAGE;
        break;
    default:
        break;
    }
}

static void btn_scan()
{
    int8_t knob_val = 0;
    hl_lvgl_lineout_ioctl_t ctrl;
    // 返回按键
    uint8_t back_btn  = hl_mod_keypad_touchkey_read();
    // OK按键
    uint8_t ok_btn  = hl_mod_get_knob_okkey_val();

    switch (page_state)
    {
    case BACK_PAGE:
        // 返回主页
        if (back_btn) {
                page_state = BACK_PAGE;
                PageManager_PagePop();
                page_s_voc_bar_init();
        }
        //进入调试
        if (ok_btn == HL_KEY_EVENT_SHORT) {
            page_deinit();
            //重新创建
            page_s_voc_bar_init();
            page_state = SET_PAGE;
        }
        // 选择当前的页面
        
        knob_val = hl_mod_knob_select_val_change(&now_sound_channal,0,1);
        if (knob_val) {
            ctrl.lineout_choose = (hl_lineout_choose_t)knob_val;
            //hl_mod_lineout_ioctl(&ctrl);
        }

        break;
    case SET_PAGE:
        // 返回首界面
        if (back_btn || ok_btn == HL_KEY_EVENT_SHORT) 
            page_state = BACK_PAGE;
        break;
    default:
        break;
    }
}

static void hl_mod_page_loop(void)
{
    btn_scan();
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



