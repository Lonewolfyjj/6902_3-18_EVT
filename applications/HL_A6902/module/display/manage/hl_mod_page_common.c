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

// 系统当前的状态和类别
static hl_display_screen_s hl_screendata = {
    .display_fault_code       = 0,
    .monitor_category         = 0,
    .voice_module             = HIGH_FIDELITY,
    .sound_module             = STEREO,
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
    .uac_in_volume            = 0,
    .uac_out_volume           = 0,
    .tx1_gain_volume          = 0,
    .tx2_gain_volume          = 0,
    .monitor_volume           = 0,
    .led_britness             = 127,
    .tx1_remained_record_time = 10,
    .tx2_remained_record_time = 10,
    .ota_upgrade_progress     = 0,
    .tx1_ver                  = "V0.0.0.0",
    .tx2_ver                  = "V0.0.0.0",
#if A6902_RX_HL_EN || A6902_RX_HL_CH
    .rx_ver                   = A6902_VERSION,
#else
    .rx_ver                   = "V0.0.0.0",
#endif

    .sn = "00000000000000000000",

    .sys_status = { 0 },
    .systime    = { 2022, 12, 16, 0, 0 },
};
// 參數變更信息
static hl_display_screen_change_s change_flag = { 0 };
// 下发的输入事件数据
static hl_scr_in_data_t in_data = { 0 };

// 菜单当前的居中的图标
static uint8_t now_center_icon = 0;


// 返回触摸按键消抖时间
static hl_timeout_t backtouchkey_debance;

// 获取当前页面
hl_screen_page_e hl_mod_display_scr_get_page(void)
{
    hl_screendata.page_id = PageManager_GetNowPage();
    return hl_screendata.page_id;
}

uint8_t hl_mod_menu_icon_event(uint32_t current)
{
    rt_kprintf("Page:%x\n", current);

    //未锁屏回主界面
    if (!(current & 0x80)) {
        return 1;
    } else {
        now_center_icon = current & 0x7F;
        return 0;
    }
    return 0;
}

void hl_mod_menu_icon_init()
{
    now_center_icon = 0;
}
uint32_t hl_mod_menu_get_icon()
{
    return now_center_icon;
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
// 返回触摸按键消抖功能实现
uint8_t hl_mod_keypad_touchkey_read()
{
    static uint8_t last_key = 0;
    uint8_t        res;
    uint8_t        press = 0;

    res = hl_drv_ztw523a_botton_status();

    switch (last_key) {
        case 0:
            if (res == 1) {
                // 出现下降沿，重新计数
                hl_util_timeout_set(&backtouchkey_debance, BACKTOUCHKEY_DEBANCE_TIMER);
                last_key = 1;
            }
            break;
        case 1:
            if (hl_util_timeout_judge(&backtouchkey_debance) == RT_TRUE) {
                //20s内没有上升沿，按键按下
                if (res == 1) {
                    LV_LOG_USER("touchkey_en");
                    press    = 1;
                    last_key = 0;
                    hl_util_timeout_close(&backtouchkey_debance);
                } else {
                    hl_util_timeout_close(&backtouchkey_debance);
                    last_key = 0;
                    press    = 0;
                }
            } else {
                // 出现上升沿，则清定时器
                if (res == 0) {
                    hl_util_timeout_close(&backtouchkey_debance);
                    last_key = 0;
                    press    = 0;
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
    int8_t data = hl_mod_get_rx_knob_val();
    if (data != 0) {
        PageManager_PagePush(PAGE_QUICK_SETTINGS);
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
    return data;
}
void hl_mod_menu_icon_set(uint32_t num)
{
    now_center_icon = num;
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

void hl_mod_menu_knob_icon_change(int8_t center, uint8_t maxnum)
{
    int8_t data = hl_mod_get_rx_knob_val();

    // LV_LOG_USER("knob1=%d\n",data);
    if (data != 0) {
        center += data;
        if (center >= maxnum) {
            center = maxnum - 1;
        } else if (center < 0) {
            center = 0;
        } 
        LV_LOG_USER("icon_pos=%d\n", center);
        lv_set_icon_postion(center, false);
        hl_mod_menu_icon_set(center);
    }
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

        data = keypad_knob_ok;
        switch (keypad_knob_ok) {

            case HL_KEY_EVENT_SHORT:

                LV_LOG_USER("LV_KEY_ENTER\n");
                break;
            case HL_KEY_EVENT_LONG:

                LV_LOG_USER("LV_KEY_NEXT\n");

                break;
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

void hl_mod_display_send_msg(hl_out_msg_e msg_cmd, void* param, uint32_t len)
{
    rt_err_t res;

    if (hl_mod_display.msg_hander == RT_NULL) {
        LOG_E("msg no init\n");
    }
    hl_mod_display.msg.sender = DISPLAY_MODE;
    hl_mod_display.msg.cmd    = msg_cmd;
    if (len == 0) {
        hl_mod_display.msg.param.u32_param = *(uint32_t*)param;
        LOG_E("msg[%d][%d]\n",hl_mod_display.msg.cmd,hl_mod_display.msg.param.u32_param);
    } else {
        hl_mod_display.msg.param.ptr = param;
        LOG_E("msg[%d]\n",msg_cmd);
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
            // case MSG_RX_PWR_KEY:
            //     hl_app_rx_pwr_key_pro(p_msg->param.u32_param);
            //     LOG_D("MSG_RX_PWR_KEY:(%d) \r\n", p_msg->param.u32_param);
            //     break;

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
}

void lvgl2rtt_init(void)
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
}
void hl_mod_page_all_init(void)
{
    PageManager_Init(PAGE_MAX, 8);
    hl_mod_page_cb_reg();

    PageManager_PagePush(PAGE_HOME);
}
#endif
/*
 * EOF
 */