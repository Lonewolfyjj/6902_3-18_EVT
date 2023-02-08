/**
 * @file hl_mod_page_fast_tx_config.c
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

static int16_t                 now_num = (int16_t)HL_MOREONE_OPTION_ONE;
static hl_lvgl_moreone_ioctl_t now_opt;
static hl_moreone_choose_t     opt1_choose;
static hl_moreone_choose_t     opt2_choose;
static hl_moreone_choose_t     opt3_choose;

static hl_moreone_choose_t tx1_record_switch(hl_display_screen_s* data_ptr)
{
    if (data_ptr->sys_status.tx1_record_state) {
        return HL_MOREONE_CHOOSE_LEFT;
    } else {
        return HL_MOREONE_CHOOSE_RIGHT;
    }
}
static hl_moreone_choose_t tx1_mute_switch(hl_display_screen_s* data_ptr)
{
    if (data_ptr->sys_status.tx1_mute_switch) {
        return HL_MOREONE_CHOOSE_LEFT;
    } else {
        return HL_MOREONE_CHOOSE_RIGHT;
    }
}

static hl_moreone_choose_t tx1_noise_switch(hl_display_screen_s* data_ptr)
{
    if (data_ptr->sys_status.tx1_noise) {
        return HL_MOREONE_CHOOSE_LEFT;
    } else {
        return HL_MOREONE_CHOOSE_RIGHT;
    }
}

//录制开关、静音、降噪开关界面
static void hl_moreone_test_cb(hl_moreone_mid_opt_t moreone_mid_opt, hl_moreone_choose_t choose)
{
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();
    uint32_t             value    = 0;

    LOG_E("opt=%d,choose=%d\n", moreone_mid_opt, choose);
    // 上报并更新参数
    switch (moreone_mid_opt) {
        case HL_MOREONE_OPTION_ONE:
            if (opt1_choose != choose) {
                opt1_choose = choose;
                // 上报并更新参数
                switch (choose) {
                    case HL_MOREONE_CHOOSE_LEFT:
                        value = 1;
                        break;
                    case HL_MOREONE_CHOOSE_RIGHT:
                        value = 0;
                        break;
                    default:
                        return;
                        break;
                }

                if (data_ptr->sys_status.tx1_record_state != value) {
                    // 上报并更新参数
                    hl_mod_display_mux_take();
                    data_ptr->sys_status.tx1_record_state = value;
                    flag->sys_status.tx1_record_state = 1;
                    hl_mod_display_mux_release();
                    hl_mod_display_send_msg(TX1_RECORD_STATE_SWITCH_IND, &value, 0);
                }
            }
            break;
        case HL_MOREONE_OPTION_TWO:
            if (opt2_choose != choose) {
                opt2_choose = choose;

                switch (choose) {
                    case HL_MOREONE_CHOOSE_LEFT:
                        value = 1;
                        break;
                    case HL_MOREONE_CHOOSE_RIGHT:
                        value = 0;
                        break;
                    default:
                        return;
                        break;
                }
                if (data_ptr->sys_status.tx1_mute_switch != value) {
                    // 上报并更新参数
                    hl_mod_display_mux_take();
                    data_ptr->sys_status.tx1_mute_switch = value;
                    flag->sys_status.tx1_mute_switch = 1;
                    hl_mod_display_mux_release();
                    hl_mod_display_send_msg(TX1_MUTE_SWITCH_SWITCH_IND, &value, 0);
                }
            }

            break;
        case HL_MOREONE_OPTION_THREE:
            if (opt3_choose != choose) {
                opt3_choose = choose;

                switch (choose) {
                    case HL_MOREONE_CHOOSE_LEFT:
                        value = 1;
                        break;
                    case HL_MOREONE_CHOOSE_RIGHT:
                        value = 0;
                        break;
                    default:
                        return;
                        break;
                }
                if (data_ptr->sys_status.tx1_noise != value) {
                    // 上报并更新参数
                    hl_mod_display_mux_take();
                    data_ptr->sys_status.tx1_noise = value;
                    flag->sys_status.tx1_noise = 1;
                    hl_mod_display_mux_release();
                    hl_mod_display_send_msg(TX1_NOISE_SWITCH_IND, &value, 0);
                }
            }

            break;
        default:
            break;
    }
}

static void hl_mod_page_state_update(void)
{
    hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();
    hl_lvgl_moreone_ioctl_t     ioctrl;
    hl_moreone_choose_t         temp;

    temp = tx1_record_switch(data_ptr);

    if (temp != opt1_choose) {
        opt1_choose               = temp;
        ioctrl.cmd_type           = HL_MOREONE_OPTION_CMD;
        ioctrl.moreone_choose     = temp;
        ioctrl.moreone_choose_opt = HL_MOREONE_OPTION_ONE;
        hl_mod_moreone_ioctl(&ioctrl);

        ioctrl.cmd_type = HL_MOREONE_POSTION_CMD;
        hl_mod_moreone_ioctl(&ioctrl);
    }

    temp = tx1_mute_switch(data_ptr);
    if (temp != opt2_choose) {
        opt2_choose                      = temp;
        ioctrl.cmd_type           = HL_MOREONE_OPTION_CMD;
        ioctrl.moreone_choose     = temp;
        ioctrl.moreone_choose_opt = HL_MOREONE_OPTION_TWO;

        hl_mod_moreone_ioctl(&ioctrl);

        ioctrl.cmd_type = HL_MOREONE_POSTION_CMD;
        hl_mod_moreone_ioctl(&ioctrl);
    }

    temp = tx1_noise_switch(data_ptr);

    if (temp != opt3_choose) {
        opt3_choose = temp;
        ioctrl.cmd_type           = HL_MOREONE_OPTION_CMD;
        ioctrl.moreone_choose     = temp;
        ioctrl.moreone_choose_opt = HL_MOREONE_OPTION_THREE;

        hl_mod_moreone_ioctl(&ioctrl);

        ioctrl.cmd_type = HL_MOREONE_POSTION_CMD;
        hl_mod_moreone_ioctl(&ioctrl);
    }
}

static void hl_mod_page_setup(void)
{
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();

    opt1_choose = tx1_record_switch(data_ptr);
    opt2_choose = tx1_mute_switch(data_ptr);
    opt3_choose = tx1_noise_switch(data_ptr);

    hl_lvgl_moreone_init_t data = {
        .func_cb                         = hl_moreone_test_cb,
        .moreone_choose_opt.option_one   = opt1_choose,
        .moreone_choose_opt.option_two   = opt2_choose,
        .moreone_choose_opt.option_three = opt3_choose,
        .moreone_mid_opt                 = HL_MOREONE_OPTION_ONE,
        .device_ptr                      = "TX1",
    };

    hl_mod_moreone_init(&data);
    now_num = data.moreone_mid_opt;

    hl_mod_knob_select_val_set(&now_num, opt1_choose);
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_moreone_ioctl_t data;
    data.cmd_type           = HL_MOREONE_POSTION_CMD;
    data.moreone_choose     = 0;
    data.moreone_choose_opt = HL_MOREONE_OPTION_EXTI;
    hl_mod_moreone_ioctl(&data);
    rt_thread_mdelay(10);
    // data.cmd_type           = HL_MOREONE_POSTION_CMD;
    // data.moreone_choose     = 0;
    // data.moreone_choose_opt = HL_MOREONE_OPTION_DELETE_STYLE;
    // hl_mod_moreone_ioctl(&data);
}

static void hl_mod_page_loop(void)
{
    hl_moreone_choose_t     moreone_choose;  //
    hl_moreone_mid_opt_t    moreone_choose_opt;
    hl_lvgl_moreone_ioctl_t io_ctrl;
    // OK按键
    uint8_t ok_btn = hl_mod_get_knob_okkey_val();

    hl_mod_page_state_update();

    // 触摸返回
    if (1 == hl_mod_keypad_touchkey_read()) {
        // now_opt.cmd_type = HL_MOREONE_OPTION_CMD;
        // hl_mod_moreone_ioctl(&now_opt);
        PageManager_PagePush(PAGE_FAST_TX2_CONFIG);
        // PageManager_PagePop();
    }

    // 旋钮选择
    if (hl_mod_knob_select_val_change(&now_num, 0, 2, false)) {

        moreone_choose_opt = now_num;

        io_ctrl.cmd_type           = HL_MOREONE_POSTION_CMD;
        io_ctrl.moreone_choose_opt = moreone_choose_opt;

        hl_mod_moreone_ioctl(&io_ctrl);
    }

    // OK按键
    if (ok_btn == HL_KEY_EVENT_SHORT) {
        // now_opt.cmd_type = HL_MOREONE_OPTION_CMD;
        // hl_mod_moreone_ioctl(&now_opt);
    }
}

PAGE_DEC(PAGE_FAST_TX_CONFIG)
{
    bool result;

    result = PageManager_PageRegister(PAGE_FAST_TX_CONFIG, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("page_fast_tx_config init fail\n");
    }
}

#endif
/*
 * EOF
 */
