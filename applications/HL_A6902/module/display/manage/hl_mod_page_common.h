/**
 * @file hl_mod_page_common.h
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-11-30
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
 * <tr><td>2022-11-30     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_MOD_PAGE_COMMON_H__
#define __HL_MOD_PAGE_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hl_config.h"
#include "hl_mod_display_mng.h"
#include "rtthread.h"
#include "hl_mod_display.h"
#include "lvgl.h"
#include "hl_util_msg_type.h"
#include "hl_util_timeout.h"
#include "page_b_two_in_one.h"

#define DBG_SECTION_NAME "display"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

// 屏定时熄屏的计时时间(ms)
#define HL_SCREEN_AUTO_LOWBRIGHTNESS_TIME 60000
// 屏定时熄屏的亮度
#define SCREEN_LOWBRIGHTNESS 0x30
#define SCREEN_BRIGHTNESS_DEFAULT_VALUE     0x80
// led的亮度值
#define NORMAL_BRIGTNESS 30
#define LOWVAL_BRIGTNESS 15
/// TX录制最大使用时间
#define STROGE_MAX_USED_TIME 16

// bool ：true表示定时器超时的操作，false 表示定时器重新更新计数时间时的操作
typedef void (*screen_trigfunc)(bool);

typedef struct _hl_screenofftime_t
{
    hl_timeout_t   timer;
    screen_trigfunc trigfunc;//如果是RT_NULL,就不会运行更新函数
    uint32_t        outtime;
} hl_screenofftime_t;

typedef enum hl_mod_screen_lock_status_e
{
    /// 未锁屏状态
    HL_SCREEN_UNLOCKED,
    /// @brief 已锁屏状态
    HL_SCREEN_LOCKED,
    /// @brief 锁屏动画显示期间
    HL_SCREEN_LOCKING,
    /// @brief 未锁屏动画期间
    HL_SCREEN_UNLOCKING
} HL_ENUM8(hl_mod_screen_lock_status);

//下发的命令
typedef struct _hl_display_status{
    /// 屏幕锁屏状态，默认是未锁屏 聚体见<hl_mod_screen_lock_status>
    uint32_t screen_lock:2;
    uint32_t tx1_noise:1;
    uint32_t tx2_noise:1;
    uint32_t tx1_record_state:1;
    uint32_t tx2_record_state:1;
    uint32_t tx1_low_electricity:1;
    uint32_t tx2_low_electricity:1;
    uint32_t rx_low_electricity:1;
    uint32_t case_low_electricity:1;
    uint32_t tx1_charge_status:1;
    uint32_t tx2_charge_status:1;
    uint32_t rx_charge_status:1;
    uint32_t box_charge_status:1;
    uint32_t usb_in:1;
    uint32_t line_out_in:1;
    uint32_t monitor_in:1;
    uint32_t auto_record:1;
    /// 外放设置 （1: 开启 0:关闭）
    uint32_t soundout_setting:1;
    /// 自动录制状态 （1: 开启 0:关闭）
    uint32_t auto_record_portect:1;
    uint32_t tx1_mute_switch:1;
    uint32_t tx2_mute_switch:1;
    /// 熄屏状态 （1：熄屏(或者盒子关闭) 0 正常亮）
    uint32_t screen_off_status:1;

    /// @brief 恢复出厂设置
    uint32_t restore_state:1; 
    /// @brief 背景变暗标志位
    uint32_t lowbrightness_flag:1; 
    /// @brief 开启升级设置标志位 1表示已下发 0 表示未下发
    /// @brief 数据部分 1成功 0 表示失败
    uint32_t upgrade_setting_flag:1;
    /// @brief 苹果认证标志，1：苹果认证成功 0 ：苹果认证失败
    uint32_t apple_auth_flag:1; 
}hl_display_status;

typedef struct _hl_display_screen_s
{
    // 显示模块错误信息
    uint8_t display_fault_code;
    /// 监听类别
    hl_display_voice_monitor_e monitor_category;
    hl_display_vocie_mode_e voice_module;
    hl_display_sound_module_e down_sound_module;
    hl_display_sound_module_e now_sound_module;
    hl_display_low_cut_e low_cut;
    hl_screen_page_e page_id;
    hl_display_box_charge_state in_box_state;
    hl_rf_state_e rf_net_connect;
    hl_display_out_box_charge_state out_box_poweroff_charge;
    uint8_t tx1_bat_val;
    uint8_t tx2_bat_val;
    uint8_t rx_bat_val;
    uint8_t case_bat_val;
    uint8_t tx1_vu;
    uint8_t tx2_vu;
    uint8_t tx1_signal;
    uint8_t tx2_signal;
    /// 降噪等级设置
    uint8_t tx_noise_level;
    /// @brief 立体声的左声道的音量
    int8_t tx1_line_out_volume;
    /// @brief 立体声的右声道的音量
    int8_t tx2_line_out_volume;
    /// @brief 单声道的音量
    int8_t mono_line_out_volume;
    /// @brief 安全音轨的音量
    int8_t  track_line_out_volume;
    int8_t uac_in_volume;
    int8_t uac_out_volume;
    int8_t tx1_gain_volume;
    int8_t tx2_gain_volume;
    int8_t monitor_volume;
    uint8_t led_britness;
    uint8_t tx1_remained_record_time;
    uint8_t tx2_remained_record_time;
    // OTA升级进度
    uint8_t upgrade_progress;
    // 自动关机参数  0表示永不自动关机；有数据表示设定的关机时间(单位为min)
    uint16_t auto_poweroff;
    hl_upgrade_status upgrade_status;
    
    char tx1_ver[10];
    char tx2_ver[10];
    char rx_ver[10];
    char case_ver[10];
    char sn[36];
    char device_fault_code[20];
    // 显示系统当前状态信息
    hl_display_status sys_status;
    hl_display_systime_s systime;
}hl_display_screen_s;

// 下发信息变更标志,如果多个页面都存在这个状态，那么不适用这个flag判断参数是否变更
typedef struct _hl_display_screen_change_s{
    hl_display_status sys_status;
    uint32_t rf_net_connect:1;
    uint32_t monitor_category:1;
    uint32_t sound_module:1;
    uint32_t low_cut:1;
    uint32_t page_id:1;
    uint32_t in_box_state:1;
    uint32_t tx1_bat_val:1;
    uint32_t tx2_bat_val:1;
    uint32_t rx_bat_val:1;
    uint32_t case_bat_val:1;
    uint32_t tx1_vu:1;
    uint32_t tx2_vu:1;
    uint32_t tx1_signal:1;
    uint32_t tx2_signal:1;
    /// 降噪等级设置
    uint32_t tx_noise_level:1;
    uint32_t tx1_line_out_volume:1;
    uint32_t tx2_line_out_volume:1;
    uint32_t mono_line_out_volume:1;
    uint32_t track_line_out_volume:1;
    uint32_t uac_in_volume:1;
    uint32_t uac_out_volume:1;
    uint32_t tx1_gain_volume:1;
    uint32_t tx2_gain_volume:1;
    uint32_t led_britness:1;
    uint32_t tx1_remained_record_time:1;
    uint32_t tx2_remained_record_time:1;
    uint32_t upgrade_progress:1;
    uint32_t upgrade_status:1;
    uint32_t device_fault_code:1;
    // 菜单复位时的状态
    uint32_t menu_defaut:1;
    uint32_t systime:1;
    uint32_t tx1_ver:1;
    uint32_t tx2_ver:1;
    uint32_t rx_ver:1;
    uint32_t case_ver:1;
    uint32_t rx_sn:1;
    uint32_t auto_poweroff:1;
    // 格式化页面TX声道标志,0表示格式化左声道，1表示格式化右声道
    uint32_t channel_format_flag:1;
    uint32_t voice_module:1;
}hl_display_screen_change_s;


typedef struct _hl_scr_indev_msg_t
{
    uint8_t keypad_touchkey;
    uint8_t keypad_knob_ok;
    int8_t  encoder_knob_diff;
    /// @brief 开关机按键
    uint8_t power_key;
} hl_scr_indev_msg_t;

typedef struct _hl_scr_in_data_t
{
    hl_scr_indev_msg_t in_inputdev;
} hl_scr_in_data_t;

typedef enum _device_pose_t
{
    DEVICE_REVERSE_POSE = 0,
    DEVICE_FORWARD_POSE,
}HL_ENUM8(device_pose_t);
 
void hl_mod_display_mux_init(void);
void hl_mod_display_mux_take(void);
void hl_mod_display_mux_release(void);
void hl_b_two_in_one_trg(hl_b_two_in_one_check_t choose);
hl_screen_page_e hl_mod_display_scr_get_page(void);
hl_display_screen_s* hl_mod_page_get_screen_data_ptr();
void hl_mod_page_delete(lv_obj_t* obj);
void hl_mod_indev_val_get(mode_to_app_msg_t* p_msg);
uint8_t hl_mod_get_knob_okkey_val(void);
int8_t hl_mod_get_rx_knob_val(void);
void hl_mod_menu_knob_icon_change(int8_t center, uint8_t maxnum);

void hl_mod_knob_select_val_set(int16_t* ptr, int16_t num);
int16_t hl_mod_knob_select_val_get(int16_t* ptr);
int16_t hl_mod_knob_select_val_change(int16_t* ptr, int16_t left, int16_t right, bool en);

uint8_t hl_mode_report_event(hl_timeout_t* timer, uint32_t time, int16_t bar_num);
hl_display_screen_change_s* hl_mod_page_get_screen_change_flag();
void hl_mod_display_send_msg(hl_out_msg_e msg_cmd, void *param, uint32_t len);
bool hl_mod_next_menu_enter(uint8_t * tab, uint8_t num,uint8_t max_num);
void hl_mod_rx_knob_val_pro(struct _lv_indev_drv_t* drv, lv_indev_data_t* data);
uint8_t hl_mod_keypad_touchkey_read();
void hl_mod_menu_enterbtn_scan(uint8_t num);
void hl_mod_menu_goto_home_page(void);
void hl_mod_menu_backbtn_scan();
void hl_mod_menu_goto_fast_config_scan();
void hl_mod_menu_goto_quickset_scan();
void hl_mod_page_screen_lowbritness_init(void);
void hl_mod_page_screen_lowbritness_update(void);
void hl_mod_page_screen_lowbritness_deinit(void);
void hl_mod_page_screen_lowbritness_scan(void);
void hl_mod_page_inbox_screenoff_init(void);
void hl_mod_page_inbox_screenoff_close(void);
void hl_mod_page_inbox_screenoff_update(void);
void hl_mod_page_inbox_screenoff_scan(void);
void hl_mod_page_goto_box_scan(void);
void hl_mod_display_upgrade_enter(void);
void hl_mod_page_event_btn_init(lv_event_cb_t event_cb);
void hl_mod_outbox_offcharge_scan(void);
void hl_mod_page_screenofftimer_close(hl_screenofftime_t *timer);
void hl_mod_page_screenofftimer_scan(hl_screenofftime_t *timer);
void hl_mod_page_screenofftimer_update(hl_screenofftime_t* timer);
void hl_mod_page_screenofftimer_init(hl_screenofftime_t* timer);
uint8_t hl_mod_display_msq_set(rt_mq_t msq);
uint8_t hl_mod_get_power_key_val(void);
void hl_mod_page_all_init(void);
void lvgl2rtt_init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

/* Includes ------------------------------------------------------------------*/
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#endif /* __HL_MOD_PAGE_COMMON_H__ */
/*
 * EOF
 */