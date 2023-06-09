/**
 * @file hl_mod_page_common.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-12-01
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
 * <tr><td>2022-12-01     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "hl_mod_page_common.h"
#include "hl_mod_display_mng.h"
#include "hl_util_general_type.h"
#include "hl_mod_input.h"
#include "hl_mod_page.h"
#include "lv_port_indev.h"
#include "lv_port_disp.h"
#include "hl_util_timeout.h"
#if !HL_IS_TX_DEVICE()
/* typedef -------------------------------------------------------------------*/
#include "page_menu.h"
#include "page_test.h"
#include "lvgl.h"
#include "hl_drv_ztw523a.h"
#include "math.h"
#include "page_style_bit.h"
#include "hl_drv_rm690a0.h"
#include "page_top.h"
/* define --------------------------------------------------------------------*/
#define DBG_SECTION_NAME "display"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#define BACKTOUCHKEY_DEBANCE_TIMER 20

typedef struct _hl_display_msg_t
{
    /// 消息队列句柄
    rt_mq_t msg_hander;
    ///  消息
    mode_to_app_msg_t msg;

} hl_display_msg_t;

static hl_display_msg_t hl_mod_display;

static hl_screenofftime_t hl_lowbrightness_time;
static hl_screenofftime_t screenoff_timer;
// 系统当前的状态和类别
static hl_display_screen_s hl_screendata = {
    .display_fault_code       = 0,
    .monitor_category         = 0,
    .voice_module             = HIGH_FIDELITY,
    .now_sound_module         = STEREO,
    .down_sound_module        = STEREO,
    .low_cut                  = LOW_CUT_OFF,
    .page_id                  = PAGE_NONE,
    .tx1_bat_val              = 0,
    .tx2_bat_val              = 0,
    .rx_bat_val               = 0,
    .case_bat_val             = 0,
    .tx1_vu                   = 0,
    .tx2_vu                   = 0,
    .tx1_signal               = 0,
    .tx2_signal               = 0,
    .tx_noise_level           = 4,
    .tx1_line_out_volume      = 0,
    .tx2_line_out_volume      = 0,
    .mono_line_out_volume     = 0,
    .track_line_out_volume    = 0,
    .uac_in_volume            = 0,
    .uac_out_volume           = 0,
    .tx1_gain_volume          = 0,
    .tx2_gain_volume          = 0,
    .monitor_volume           = 0,
    .scr_bl                   = SCREEN_BRIGHTNESS_DEFAULT_VALUE,
    .led_britness             = NORMAL_BRIGTNESS,
    .tx1_remained_record_time = STROGE_MAX_USED_TIME,
    .tx2_remained_record_time = STROGE_MAX_USED_TIME,
    .upgrade_progress         = 0,
    .upgrade_status           = HL_UPGRADE_STATUS_NORMAL,
    .tx1_ver                  = "V0.0.0.0",
    .tx2_ver                  = "V0.0.0.0",
#if A6902_RX_HL_EN || A6902_RX_HL_CH || A6902_RX_JF_CH || A6902_RX_HL_EN
    .rx_ver = A6902_VERSION,
#else
    .rx_ver = "V0.0.0.0",
#endif

    .sn = "00000000000000000000",

    .sys_status    = { 0 },
    .systime       = { 2022, 12, 16, 0, 0 },
    .auto_poweroff = 0,
};
// 參數變更信息
static hl_display_screen_change_s change_flag = { 0 };
// 下发的输入事件数据
static hl_scr_in_data_t in_data = { 0 };

static rt_mutex_t display_mux = RT_NULL;

// 返回触摸按键消抖时间
static hl_timeout_t backtouchkey_debance;

void hl_mod_display_mux_init(void)
{
    display_mux = rt_mutex_create("display_mutex", RT_IPC_FLAG_FIFO);
}

void hl_mod_display_mux_take(void)
{
    if (display_mux) {
        rt_mutex_take(display_mux, 1000);
    }
}

void hl_mod_display_mux_release(void)
{
    if (display_mux) {
        rt_mutex_release(display_mux);
    }
}

// 获取当前页面
hl_screen_page_e hl_mod_display_scr_get_page(void)
{
    hl_screendata.page_id = PageManager_GetNowPage();
    return hl_screendata.page_id;
}





hl_display_screen_s* hl_mod_page_get_screen_data_ptr()
{
    return &hl_screendata;
}

hl_display_screen_change_s* hl_mod_page_get_screen_change_flag()
{
    return &change_flag;
}


// 设置参数不变更了再发送事件
uint8_t hl_mode_report_event(hl_timeout_t* timer, uint32_t time, int16_t bar_num)
{

    static int16_t last_data = 0;

    uint8_t changeflag = 0;

    if (last_data != bar_num) {

        hl_util_timeout_set(timer, time);
        changeflag = 0;
        last_data  = bar_num;
    } else {
        if (hl_util_timeout_judge(timer) == RT_TRUE) {
            changeflag = 1;
            hl_util_timeout_close(timer);
        }
    }

    return changeflag;
}

// 2:充电中   1 ：欠压中   0 正常
uint8_t bat_state_deal(uint8_t charge_state, uint8_t bat_val, uint8_t thresho)
{
    uint8_t ret;

    if (charge_state) {
        ret = 2;
    } else if (charge_state == 0 && bat_val > thresho) {
        ret = 0;
    } else if (charge_state == 0 && bat_val <= thresho) {
        ret = 1;
    } else {
        ret = 0;
    }

    return ret;
}

void hl_b_two_in_one_trg(hl_b_two_in_one_check_t choose)
{    hl_lvgl_b_two_in_one_ioctl_t two_b_in_one_test_ctl;

    two_b_in_one_test_ctl.b_two_in_one_choose = choose;
    hl_mod_b_two_in_one_ioctl(&two_b_in_one_test_ctl);
}

// 返回触摸按键消抖功能实现
uint8_t hl_mod_keypad_touchkey_read()
{
    static uint8_t last_state = 0;

    uint8_t        res;
    uint8_t        press = 0;

    res = hl_drv_ztw523a_botton_status();

    switch (last_state) {
        case 0:
            if (res == 1) {
                
                hl_util_timeout_set(&backtouchkey_debance, BACKTOUCHKEY_DEBANCE_TIMER);
                last_state = 1;

            }
            break;
        case 1:
            if (hl_util_timeout_judge(&backtouchkey_debance) == RT_TRUE) {
                //20s内没有上升沿，按键按下
                if (res == 1) {
                    LV_LOG_USER("touchkey_down");
                    press    = 0;
                    last_state = 2;
                    hl_util_timeout_close(&backtouchkey_debance);
                } else {
                    hl_util_timeout_close(&backtouchkey_debance);
                    last_state = 0;
                    press    = 0;
                }
            } else {
                // 出现上升沿，则清定时器
                if (res == 0) {
                    hl_util_timeout_close(&backtouchkey_debance);
                    last_state = 0;
                    press    = 0;
                }
            }
            break;
        case 2:
            if (res == 0) {
                // 出现下降沿，重新计数
                hl_util_timeout_set(&backtouchkey_debance, BACKTOUCHKEY_DEBANCE_TIMER);
                last_state = 3;
            }
        break;
        case 3:
            if (hl_util_timeout_judge(&backtouchkey_debance) == RT_TRUE) {
                //20s内没有被再次压下
                if (res == 0) {
                    LOG_E("touchkey_up");
                    press      = 1;
                    hl_mod_page_screen_lowbritness_update();
                    hl_mod_page_inbox_screenoff_update();
                    last_state = 0;
                    hl_util_timeout_close(&backtouchkey_debance);
                } else {
                    hl_util_timeout_close(&backtouchkey_debance);
                    last_state = 2;
                    press      = 0;
                }
            } else {
                // 又被压下，则清定时器
                if (res == 1) {
                    hl_util_timeout_close(&backtouchkey_debance);
                    last_state = 2;
                    press      = 0;
                }
            }
        break;
        default:
            press = 0;
            break;
    }
    return press;
}

// 返回按键扫描，（如果触发，则返回上一级菜单）
void hl_mod_menu_goto_fast_config_scan()
{
    if (1 == hl_mod_keypad_touchkey_read()) {
        PageManager_PagePush(PAGE_FAST_TX_CONFIG);
    }
}

// 旋钮进入快捷LINW OUT设置
void hl_mod_menu_goto_quickset_scan()
{
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();
    hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();

    int8_t knob = hl_mod_get_rx_knob_val();

    if (knob != 0) {
        switch (data_ptr->now_sound_module) {
            case STEREO:
                LOG_D("STEREO\n");
                PageManager_PagePush(PAGE_QUICK_SETTINGS);
                break;
            case MONO:
                LOG_D("MONO\n");
                PageManager_PagePush(PAGE_LINE_OUT_STEREO_LEFT);
                break;
            case SAFE_TRACK:
                LOG_D("SAFE_TRACK\n");
                PageManager_PagePush(PAGE_LINE_OUT_STEREO_LEFT);
                break;
            default:
                break;
        }
    }
}

// 选择按键处理
void hl_mod_page_s_choose_two_one_scan(uint8_t maxnum)
{
    static int8_t                choose = 0;
    hl_lvgl_s_two_in_one_ioctl_t s_two_in_one_test_ctl;

    int8_t data = hl_mod_get_rx_knob_val();

    if (1 == hl_mod_keypad_touchkey_read()) {
    }

    // LV_LOG_USER("knob1=%d\n",data);
    if (data != 0) {
        choose += data;
        if (choose >= maxnum) {
            choose = maxnum - 1;
        } else if (choose < 0) {
            choose = 0;
        }
        LV_LOG_USER("choose=%d\n", choose);

        s_two_in_one_test_ctl.s_two_in_one_choose = (hl_b_two_in_one_choose_t)choose;

        hl_mod_s_two_in_one_ioctl(&s_two_in_one_test_ctl);
    }
}

// 选择按键处理
void hl_mod_page_b_choose_two_one_scan(uint8_t maxnum)
{
    static int8_t                choose;
    hl_lvgl_s_two_in_one_ioctl_t s_two_in_one_test_ctl;

    int8_t data = hl_mod_get_rx_knob_val();

    if (1 == hl_mod_keypad_touchkey_read()) {

        page_two_in_one_click(choose);
    }

    // LV_LOG_USER("knob1=%d\n",data);
    if (data != 0) {
        choose += data;
        if (choose >= maxnum) {
            choose = maxnum - 1;
        } else if (choose < 0) {
            choose = 0;
        }
        LV_LOG_USER("choose=%d\n", choose);

        s_two_in_one_test_ctl.s_two_in_one_choose = (hl_b_two_in_one_choose_t)choose;

        hl_mod_s_two_in_one_ioctl(&s_two_in_one_test_ctl);
    }
}

// 返回按键扫描，（如果触发，则返回上一级菜单）
void hl_mod_menu_backbtn_scan()
{
    if (1 == hl_mod_keypad_touchkey_read()) {
        PageManager_PagePop();
    }
}

static int8_t now_knob_data = 0;
void          hl_mod_rx_knob_val_pro(struct _lv_indev_drv_t* drv, lv_indev_data_t* data)
{
    static int8_t encoder_knob_diff = 0;

    if (in_data.in_inputdev.encoder_knob_diff != encoder_knob_diff) {

        // LV_LOG_USER("encoder_knob=%d\r\n", in_data.in_inputdev.encoder_knob_diff);
        // LV_LOG_USER("-now=%d\r\n", encoder_knob_diff);

        data->enc_diff = in_data.in_inputdev.encoder_knob_diff - encoder_knob_diff;
        now_knob_data  = data->enc_diff;
        LV_LOG_USER("-knob_val=%d\r\n", now_knob_data);
        encoder_knob_diff = in_data.in_inputdev.encoder_knob_diff;
    }
}

int8_t hl_mod_get_rx_knob_val(void)
{
    int8_t data;
    
    data          = now_knob_data;
    now_knob_data = 0;
    if (data != 0 ) {
        hl_mod_page_screen_lowbritness_update();
        hl_mod_page_inbox_screenoff_update();
    }

    return data;
}


// 设定旋钮的值
void hl_mod_knob_select_val_set(int16_t* ptr, int16_t num)
{
    *ptr = num;
}

// 获取旋钮的当前选定值
int16_t hl_mod_knob_select_val_get(int16_t* ptr)
{
    return *ptr;
}


// 更新旋钮的当前值 left right 分别是上下限 true表示开启循环选择， FALSE 表示关闭循环选择
int16_t hl_mod_knob_select_val_change(int16_t* ptr, int16_t left, int16_t right, bool en)
{
    int16_t center = hl_mod_knob_select_val_get(ptr);
    int8_t  data   = hl_mod_get_rx_knob_val();
    int16_t delta  = right - left + 1;
    if (data != 0) {
        if (en == false) {
            center += data;

            if (center >= right) {
                center = right;
            } else if (center <= left) {
                center = left;
            }
        } else {

            center += data;
            if (center > right) {
                center = left;
            } else if (center < left) {
                center = right;
            }
        }

        LV_LOG_USER("knob_val=%d\n", center);
        hl_mod_knob_select_val_set(ptr, center);
        return 1;
    }
    // 无数据变更
    return 0;
}

// 运行屏幕亮度更新的函数，相关旋钮和返回键的操作都不执行
void hl_mod_page_backlight_update(void)
{
    // OK按键
    hl_mod_get_knob_okkey_val();
    // 返回按键
    hl_mod_keypad_touchkey_read();
    hl_mod_get_rx_knob_val();
}

void hl_mod_page_event_btn_init(lv_event_cb_t event_cb)
{
    lv_obj_t* btn = lv_btn_create(lv_scr_act());
    // lv_obj_add_flag(btn,LV_OBJ_FLAG_HIDDEN);
    lv_group_add_obj(btn, lv_group_get_default());
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_KEY, NULL);
}

uint8_t hl_mod_get_knob_okkey_val(void)
{
    static uint8_t keypad_knob_ok = HL_KEY_EVENT_IDLE;
    uint8_t        data;

    if (keypad_knob_ok != in_data.in_inputdev.keypad_knob_ok) {

        // LV_LOG_USER("keypad_knob_ok=%d\r\n", keypad_knob_ok);
        keypad_knob_ok = in_data.in_inputdev.keypad_knob_ok;

        switch (keypad_knob_ok) {

            case HL_KEY_EVENT_SHORT:
                hl_mod_page_inbox_screenoff_update();
                hl_mod_page_screen_lowbritness_update();
                LOG_E("LV_KEY_ENTER\n");
                data = keypad_knob_ok;
                break;
            case HL_KEY_EVENT_DOUBLE:
                hl_mod_page_inbox_screenoff_update();
                hl_mod_page_screen_lowbritness_update();
                LOG_E("LV_KEY_ENTER\n");
                data = HL_KEY_EVENT_IDLE;
                break;
            case HL_KEY_EVENT_LONG:
                hl_mod_page_inbox_screenoff_update();
                hl_mod_page_screen_lowbritness_update();
                LOG_E("LV_KEY_NEXT\n");
                data = HL_KEY_EVENT_IDLE;
                break;
            case HL_KEY_EVENT_RELEASE:
                LOG_E("LV_KEY_REL\n");
                hl_mod_page_inbox_screenoff_update();
                hl_mod_page_screen_lowbritness_update();
                data = HL_KEY_EVENT_IDLE;
            default:
                data = HL_KEY_EVENT_IDLE;
                LV_LOG_USER("def\n");
                break;
        }
    } else {
        data = HL_KEY_EVENT_IDLE;
    }

    return data;
}

void hl_mod_menu_enterbtn_scan(uint8_t num)
{
    uint8_t key_event;

    key_event = hl_mod_get_knob_okkey_val();

    //菜单点击按键
    if (key_event == HL_KEY_EVENT_SHORT) {
        LV_LOG_USER("icon_enter\n");
        //lv_event_send(hl_menu_obj_get(num),LV_EVENT_KEY,NULL);
        lv_set_icon_postion(num, true);
    }
}

void hl_mod_menu_goto_home_page(void)
{
    PageManager_PageStackClear();
    PageManager_PagePush(PAGE_HOME);
}

void hl_mod_rx_knob_key_pro(struct _lv_indev_drv_t* drv, lv_indev_data_t* data)
{

    static uint32_t last_key = 0;
    uint8_t         key_event;

    key_event = hl_mod_get_knob_okkey_val();
    if (key_event) {

        switch (key_event) {

            case HL_KEY_EVENT_SHORT:
                last_key = LV_KEY_ENTER;
                LV_LOG_USER("LV_KEY_ENTER\n");
                data->state = LV_INDEV_STATE_PR;
                data->key   = last_key;
                break;
            case HL_KEY_EVENT_LONG:
                last_key = LV_KEY_NEXT;
                LV_LOG_USER("LV_KEY_NEXT\n");
                data->state = LV_INDEV_STATE_PR;
                data->key   = last_key;
                break;
            default:
                last_key    = 0;
                data->state = LV_INDEV_STATE_REL;
                data->key   = last_key;
                LV_LOG_USER("def\n");
                break;
        }

    } else {
        data->state = LV_INDEV_STATE_REL;
        data->key   = last_key;
    }
}

bool hl_mod_next_menu_enter(uint8_t* tab, uint8_t num, uint8_t max_num)
{
    if (num < max_num) {
        return PageManager_PagePush(tab[num]);
    } else {
        return false;
    }
}

// len参数为0表示是u32_param，0xFFFFFFFF表示是s32_param 如果大于0小于0xFF表示是使用的指针
void hl_mod_display_send_msg(hl_out_msg_e msg_cmd, void* param, uint32_t len)
{
    rt_err_t res;

    if (hl_mod_display.msg_hander == RT_NULL) {
        LOG_E("msg no init\n");
    }

    rt_memset(&hl_mod_display.msg, 0, sizeof(hl_mod_display.msg));
    hl_mod_display.msg.sender = DISPLAY_MODE;
    hl_mod_display.msg.cmd    = msg_cmd;

    if (len == 0) {

        hl_mod_display.msg.param.u32_param = *(uint32_t*)param;
        LOG_E("+msg[%d][%x]\n", hl_mod_display.msg.cmd, *(uint32_t*)param);

    } else if (len == 0xFFFFFFFF) {

        hl_mod_display.msg.param.s32_param = *(int32_t*)param;
        LOG_E("-msg[%d][%x]\n", hl_mod_display.msg.cmd, *(uint32_t*)param);

    } else {
        hl_mod_display.msg.param.ptr = param;
        LOG_E("pmsg[%d]\n", msg_cmd);
    }
    hl_mod_display.msg.len = len;

    res = rt_mq_send(hl_mod_display.msg_hander, (void*)(&hl_mod_display.msg), sizeof(mode_to_app_msg_t));
    if (res == -RT_EFULL) {
        LOG_E("DISP_MODE msgq full\n");
    } else if (res == -RT_ERROR) {
        LOG_E("DISP_MODE msgq err\n");
    }
}

void hl_mod_indev_val_get(mode_to_app_msg_t* p_msg)
{
    switch (p_msg->cmd) {
        case MSG_RX_PWR_KEY:
            in_data.in_inputdev.power_key = p_msg->param.u32_param;
            LOG_D("MSG_RX_PWR_KEY:(%d) \r\n", p_msg->param.u32_param);
            break;

        case MSG_RX_OK_VOL:
            in_data.in_inputdev.keypad_knob_ok = p_msg->param.u32_param;
            LOG_D("MSG_RX_OK_VOL:(%d) \r\n", in_data.in_inputdev.keypad_knob_ok);

            break;

        case MSG_RX_A_VOL:
            // + 左
            in_data.in_inputdev.encoder_knob_diff += p_msg->param.u32_param;
            LOG_D("MSG_RX_L_VOL_KEY:(%d) \r\n", in_data.in_inputdev.encoder_knob_diff);
            break;

        case MSG_RX_B_VOL:
            // - 右
            in_data.in_inputdev.encoder_knob_diff -= p_msg->param.u32_param;
            LOG_D("MSG_RX_R_VOL_KEY:(%d) \r\n", in_data.in_inputdev.encoder_knob_diff);
            break;

            // case MSG_RX_VBUS_DET:
            //     hl_app_rx_usb_plug_pro(p_msg->param.u32_param);
            //     LOG_D("MSG_RX_VBUS_DET:(%d) \r\n", p_msg->param.u32_param);
            //     break;

            // case MSG_RX_HP_DET:
            //     hl_app_rx_hp_plug_pro(p_msg->param.u32_param);
            //     LOG_D("MSG_RX_HP_DET:(%d) \r\n", p_msg->param.u32_param);
            //     break;

            // case MSG_RX_CAM_DET:
            //     hl_app_rx_cam_plug_pro(p_msg->param.u32_param);
            //     LOG_D("MSG_RX_CAM_DET:(%d) \r\n", p_msg->param.u32_param);
            //     break;

        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}

// 开关机按键功能函数，返回值：    0：表示未出现变更，1表示出现状态变更
uint8_t hl_mod_get_power_key_val(void)
{
    static uint8_t power_key = HL_KEY_EVENT_IDLE;
    uint8_t        data      = 0;

    if (power_key != in_data.in_inputdev.power_key) {

        // LV_LOG_USER("power_key=%d\r\n", power_key);
        power_key = in_data.in_inputdev.power_key;

        switch (power_key) {

            case HL_KEY_EVENT_SHORT:
                hl_mod_page_inbox_screenoff_update();
                hl_mod_page_screen_lowbritness_update();
                data = HL_KEY_EVENT_SHORT;
                break;
            case HL_KEY_EVENT_DOUBLE:
                hl_mod_page_inbox_screenoff_update();
                hl_mod_page_screen_lowbritness_update();
                data = HL_KEY_EVENT_IDLE;
                break;
            case HL_KEY_EVENT_LONG:
                hl_mod_page_inbox_screenoff_update();
                hl_mod_page_screen_lowbritness_update();

                data = HL_KEY_EVENT_IDLE;
                break;
            case HL_KEY_EVENT_RELEASE:
                hl_mod_page_inbox_screenoff_update();
                hl_mod_page_screen_lowbritness_update();
                data = HL_KEY_EVENT_IDLE;
            default:
                data = HL_KEY_EVENT_IDLE;
                LV_LOG_USER("def\n");
                break;
        }
    } else {
        data = HL_KEY_EVENT_IDLE;
    }

    return data;
}
// 在
void hl_mod_outbox_offcharge_scan(void)
{
    static hl_display_out_box_charge_state last_status = OUTBOX_OFFCHARGE_IDLE;
    hl_display_screen_s*                   data_ptr    = hl_mod_page_get_screen_data_ptr();
    hl_display_out_box_charge_state        new_status;
    if (last_status == OUTBOX_OFFCHARGE_LOGO) {
        return;
    }
    new_status = data_ptr->out_box_poweroff_charge;

    switch (last_status) {
        case OUTBOX_OFFCHARGE_IDLE:
            if (new_status == OUTBOX_OFFCHARGE_OFFPAGE) {
                PageManager_PagePush(PAGE_POWEROFF_CHARGE);
            } else if (new_status == OUTBOX_OFFCHARGE_LOGO) {
                PageManager_PagePush(PAGE_LOGO);
            }
            break;
        case OUTBOX_OFFCHARGE_OFFPAGE:
            if (new_status == OUTBOX_OFFCHARGE_LOGO) {
                PageManager_PageStackClear();
                PageManager_PagePush(PAGE_LOGO);
            }
            break;
        case OUTBOX_OFFCHARGE_LOGO:
            break;
        default:
            break;
    }
    last_status = new_status;
}



// 循环扫描是否进入盒子
void hl_mod_page_goto_box_scan(void)
{
    static hl_display_box_charge_state last_status;
    hl_display_screen_s*               data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_display_box_charge_state        new_status;

    new_status = data_ptr->in_box_state;

    if (last_status != new_status) {

        if (new_status != BOX_CHARGE_RX_NOT && last_status == BOX_CHARGE_RX_NOT) {
            PageManager_PagePush(PAGE_BOX_IN);
        }
        last_status = new_status;
    }
}

void hl_mod_page_screenofftimer_init(hl_screenofftime_t* timer)
{
    if (timer->trigfunc == RT_NULL) {
        rt_kprintf("err func_null!\n");
        return;
    }
    hl_util_timeout_set(&timer->timer, timer->outtime);
    timer->trigfunc(false);
}

// 重新更新定时器
void hl_mod_page_screenofftimer_update(hl_screenofftime_t* timer)
{
    if (timer->trigfunc == RT_NULL) {
        return;
    }
    hl_util_timeout_set(&timer->timer, timer->outtime);
    timer->trigfunc(false);
}

// updateflag = 1表示重新计时熄屏时间，
void hl_mod_page_screenofftimer_scan(hl_screenofftime_t *timer)
{
    if (timer->trigfunc == RT_NULL) {
        return;
    }
    if (hl_util_timeout_judge(&timer->timer)) {
        LOG_E("timer triger");
        timer->trigfunc(true);
        hl_util_timeout_close(&timer->timer);
    } 
}

void hl_mod_page_screenofftimer_close(hl_screenofftime_t *timer)
{
    timer->trigfunc = RT_NULL;
    hl_util_timeout_close(&timer->timer);
}

static void hl_mod_page_screen_lowbritness_trigfunc(bool flag)
{
    uint8_t brightness;
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();

    if (flag == true) {
        // 暗屏的亮度
        brightness = SCREEN_LOWBRIGHTNESS;
    } else {
        // 屏幕恢复原亮度
        brightness = data_ptr->scr_bl;
    }
    hl_drv_rm690a0_io_ctrl(SET_MIPI_BACKLIGHT_CMD, &brightness, 1);
}

// 定时降低屏幕亮度功能
void hl_mod_page_screen_lowbritness_init(void)
{
    LOG_E("init low br timer");
    hl_lowbrightness_time.outtime  = HL_SCREEN_AUTO_LOWBRIGHTNESS_TIME;
    hl_lowbrightness_time.trigfunc = hl_mod_page_screen_lowbritness_trigfunc;
    hl_mod_page_screenofftimer_init(&hl_lowbrightness_time);
}

// 降低屏幕亮度定时器重新计数
void hl_mod_page_screen_lowbritness_update(void)
{
    // LOG_E("update low br timer");
    hl_mod_page_screenofftimer_update(&hl_lowbrightness_time);
}

// 降低屏幕定时器完全关闭
void hl_mod_page_screen_lowbritness_deinit(void)
{
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();
    
    LOG_E("close low br timer");
    hl_mod_page_screenofftimer_close(&hl_lowbrightness_time);
    hl_drv_rm690a0_io_ctrl(SET_MIPI_BACKLIGHT_CMD, &data_ptr->scr_bl, 1);
}

static void hl_mod_page_inbox_screenoff_trig(bool flag)
{
    if (flag == true) {
        LOG_E("close");
        rt_thread_mdelay(100);
        hl_drv_rm690a0_io_ctrl(CLOSE_MIPI_SCREENPOWER_CMD, NULL, 0);
    } else {
        LOG_E("open");
        rt_thread_mdelay(100);
        hl_drv_rm690a0_io_ctrl(OPEN_MIPI_SCREENPOWER_CMD, NULL, 0);
        
    }
}

// 盒子内定时熄屏定时器初始化
void hl_mod_page_inbox_screenoff_init(void)
{
    LOG_E("init inbox screenoff timer");
    screenoff_timer.outtime  = 60000;
    screenoff_timer.trigfunc = hl_mod_page_inbox_screenoff_trig;

    hl_mod_page_screenofftimer_init(&screenoff_timer);
}

// 关闭屏幕熄屏定时器
void hl_mod_page_inbox_screenoff_close(void)
{
    LOG_E("close inbox screenoff timer");
    hl_mod_page_screenofftimer_close(&screenoff_timer);
    hl_mod_page_inbox_screenoff_trig(false);
}

// 关闭屏幕熄屏定时器
void hl_mod_page_inbox_screenoff_update(void)
{
    // LOG_E("update");
    hl_mod_page_screenofftimer_update(&screenoff_timer);
}

// 关闭屏幕熄屏定时器
void hl_mod_page_inbox_screenoff_scan(void)
{
    uint8_t                     data   = 0;
    hl_display_screen_s*        data_p = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag   = hl_mod_page_get_screen_change_flag();

    if (screenoff_timer.trigfunc == RT_NULL) {
        return;
    }

    // 按键的操作重新开启屏幕
    hl_mod_keypad_touchkey_read();
    hl_mod_get_rx_knob_val();
    hl_mod_get_knob_okkey_val();

    if (flag->sys_status.screen_off_status) {
        hl_mod_display_mux_take();
        flag->sys_status.screen_off_status = 0;
        data                               = data_p->sys_status.screen_off_status;
        hl_mod_display_mux_release();

        // 盒子的开关盖的处理
        if (data == 1) {
            hl_mod_page_inbox_screenoff_trig(true);
        } else {
            hl_mod_page_inbox_screenoff_update();
        }
    }

    hl_mod_page_screenofftimer_scan(&screenoff_timer);
}

// 屏幕锁屏功能扫描程序 只会在快捷音量设置页面和快捷TX设置、主页面,1表示锁屏的新状态，0表示解锁的新状态
uint8_t hl_mod_page_screen_lock_scan()
{
#if 0
    static uint8_t screen;

    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();

    hl_display_screen_change_s* flag = hl_mod_page_get_screen_change_flag();

    // 发生锁屏变更
    if (HL_KEY_EVENT_SHORT == hl_mod_get_power_key_val()) {

        // 开始运行锁屏
        if (data_ptr->sys_status.screen_lock == 0) {
            data_ptr->sys_status.screen_lock = 1;

            // 锁屏状态
            return 1;
        } else {
            data_ptr->sys_status.screen_lock = 0;

            // 解锁状态
            return 0;
        }
    }
#endif
    return 0;
}

void hl_mod_page_screen_lowbritness_scan(void)
{
    uint8_t                     data   = 0;
    hl_display_screen_s*        data_p = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag   = hl_mod_page_get_screen_change_flag();

    if (hl_lowbrightness_time.trigfunc == RT_NULL) {
        return;
    }

    if (flag->sys_status.lowbrightness_flag) {
        hl_mod_display_mux_take();
        flag->sys_status.lowbrightness_flag = 0;
        data                                = data_p->sys_status.lowbrightness_flag;
        hl_mod_display_mux_release();

        if (data == 1) {
            hl_mod_page_screen_lowbritness_update();
            hl_mod_page_inbox_screenoff_update();
        }
    }

    hl_mod_page_screenofftimer_scan(&hl_lowbrightness_time);
}

void hl_mod_display_upgrade_enter(void)
{
    hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();

    if (PAGE_UPGRADE == PageManager_GetNowPage()) {
        return;
    }

    if (flag->upgrade_status) {
        hl_mod_display_mux_take();
        flag->upgrade_status = 0;
        rt_kprintf("upgrade");
        if (data_ptr->upgrade_status != HL_UPGRADE_STATUS_NORMAL) {
            PageManager_PagePush(PAGE_UPGRADE);
        }
        hl_mod_display_mux_release();
    }
}

uint8_t hl_mod_display_msq_set(rt_mq_t msq)
{
    if (msq == NULL) {
        LOG_E("msghander err!");
        return HL_DISPLAY_FAILED;
    }

    hl_mod_display.msg_hander = (rt_mq_t)msq;
    return HL_DISPLAY_SUCCESS;
}


void hl_mod_page_cb_reg(void)
{
    PAGE_REG(PAGE_SOUND_MODULE);
    PAGE_REG(PAGE_HOME);
    PAGE_REG(PAGE_MAIN_MENU);
    PAGE_REG(PAGE_AUTO_POWEROFF);
    PAGE_REG(PAGE_AUTO_RECORD);
    PAGE_REG(PAGE_FAST_TX_CONFIG);
    PAGE_REG(PAGE_TX_LED_BRIGHT);
    PAGE_REG(PAGE_TX_LOW_CUT);
    PAGE_REG(PAGE_MONITOR_SET);
    PAGE_REG(PAGE_NOISE_REDUCTION_INTENSITY);
    PAGE_REG(PAGE_OTHER_SET);
    PAGE_REG(PAGE_PAIR);
    PAGE_REG(PAGE_RECORD_FORMAT);
    PAGE_REG(PAGE_RECORD_PROTECT);
    PAGE_REG(PAGE_RESTORE);
    PAGE_REG(PAGE_SOUND_EFFECT_MODE);
    PAGE_REG(PAGE_SYS_TIME_SET);
    PAGE_REG(PAGE_TX_CONF_MENU);
    PAGE_REG(PAGE_TX_GAIN_CONF);
    PAGE_REG(PAGE_VERSION);
    PAGE_REG(PAGE_VOLUME_MENU);
    PAGE_REG(PAGE_MONITOR_VOLUME_SET);
    // PAGE_REG(PAGE_UACIN_VOLUME_SET);
    PAGE_REG(PAGE_UACOUT_VOLUME_SET);
    PAGE_REG(PAGE_QUICK_SETTINGS);
    PAGE_REG(PAGE_LINE_OUT_STEREO_LEFT);
    PAGE_REG(PAGE_LINE_OUT_STEREO_RIGHT);
    PAGE_REG(PAGE_TX_GAIN_TX1);
    PAGE_REG(PAGE_TX_GAIN_TX2);
    PAGE_REG(PAGE_BOX_IN);
    PAGE_REG(PAGE_LOGO);
    PAGE_REG(PAGE_POWEROFF_CHARGE);
    PAGE_REG(PAGE_PARING);
    PAGE_REG(PAGE_UPGRADE);
    PAGE_REG(PAGE_FAST_TX2_CONFIG);
    PAGE_REG(PAGE_FORMAT);
    PAGE_REG(PAGE_SOUNDOUT_SETTING);
    PAGE_REG(PAGE_OPEN_MSC);
}

void lvgl2rtt_init(void)
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    // 上电清屏
    hl_mod_display_clear_screen();
}
void hl_mod_page_all_init(void)
{
    hl_page_style_bit_init();
    PageManager_Init(PAGE_MAX, 10);
    hl_mod_page_cb_reg();
}
#endif
/*
 * EOF
 */