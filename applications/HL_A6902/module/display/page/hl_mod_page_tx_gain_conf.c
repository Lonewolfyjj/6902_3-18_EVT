/**
 * @file hl_mod_page_tx_gain_conf.c
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
#include "page_txgain.h"
#include "hl_util_general_type.h"

// 0 表示左 1表示 右
static int16_t now_sound_channal = (int16_t)HL_TX_GAIN_CHECK_LEFT;

LV_IMG_DECLARE(Other_mic_black);
LV_IMG_DECLARE(Other_voice);

static void hl_bar_test_cb(hl_txgain_check_t bar_num)
{
    printf("bar_num = %d\n", bar_num);
    now_sound_channal = (int16_t)bar_num;
    
    if ((hl_txgain_check_t)now_sound_channal == HL_TX_GAIN_CHECK_LEFT) {
        PageManager_PagePush(PAGE_TX_GAIN_TX1);

    } else if ((hl_txgain_check_t)now_sound_channal == HL_TX_GAIN_CHECK_RIGHT) {
        PageManager_PagePush(PAGE_TX_GAIN_TX2);
    }
}

static void hl_mod_page_setup(void)
{
    hl_display_screen_s* data = hl_mod_page_get_screen_data_ptr();

    hl_lvgl_txgain_init_t txgain;
    txgain.func_cb       = hl_bar_test_cb;
    txgain.txgain_choose = HL_TX_GAIN_CHOOSE_LEFT;
    txgain.left_volume   = data->tx1_gain_volume;
    txgain.right_volume  = data->tx2_gain_volume;

    hl_mod_txgain_init(&txgain);
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_txgain_ioctl_t ioctrl;

    ioctrl.txgain_choose = HL_TX_GAIN_DELETE_PAGE;
    hl_mod_txgain_ioctl(&ioctrl);

    ioctrl.txgain_choose = HL_TX_GAIN_DELETE_STYLE;
    hl_mod_txgain_ioctl(&ioctrl);
}

static void btn_scan()
{
    int8_t                 knob_val = 0;
    hl_lvgl_txgain_ioctl_t ctrl;
    // OK按键
    uint8_t ok_btn = hl_mod_get_knob_okkey_val();

    // 设置触发点击事件
    if (ok_btn == HL_KEY_EVENT_SHORT) {

        ctrl.txgain_choose = (hl_txgain_check_t)now_sound_channal;
        hl_mod_txgain_ioctl(&ctrl);
    }

    // 旋钮选择TX配置通道
    knob_val = hl_mod_knob_select_val_change(&now_sound_channal, 0, 1, true);
    if (knob_val) {
        ctrl.txgain_choose = (hl_txgain_check_t)now_sound_channal;
        hl_mod_txgain_ioctl(&ctrl);
    }
}

static void hl_mod_page_loop(void)
{
    btn_scan();
    // 返回按键
    hl_mod_menu_backbtn_scan();
}

PAGE_DEC(PAGE_TX_GAIN_CONF)
{
    bool result;

    result = PageManager_PageRegister(PAGE_TX_GAIN_CONF, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_TX_GAIN_CONF init fail\n");
    }
}

#endif