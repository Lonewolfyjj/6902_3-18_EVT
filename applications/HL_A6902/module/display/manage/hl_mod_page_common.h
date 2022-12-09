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

//下发的命令
typedef struct _hl_scr_in_cmd_t
{
    hl_screen_page_e page_id;

} hl_scr_in_cmd_t;

// 下发的消息
typedef struct _hl_scr_in_msg_t
{
    uint8_t  tx_bat_1;
    uint8_t  tx_bat_2;
    uint16_t tx_uv_1;
    uint16_t tx_uv_2;
    uint8_t  tx_signal_intensity_1;
    uint8_t  tx_signal_intensity_2;
} hl_scr_in_msg_t;

typedef struct _hl_scr_indev_msg_t
{
    uint8_t keypad_touchkey;
    uint8_t keypad_knob_ok;
    int8_t  encoder_knob_diff;
} hl_scr_indev_msg_t;

typedef struct _hl_scr_in_data_t
{
    hl_scr_in_cmd_t    in_cmd;
    hl_scr_in_msg_t    in_msg;
    hl_scr_indev_msg_t in_inputdev;
} hl_scr_in_data_t;

typedef struct _hl_display_screen_s
{
    uint32_t display_fault_code:4;
    uint32_t screen_lock:1;
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
    uint32_t auto_record_portect:1;
    uint32_t tx1_mute_switch:1;
    uint32_t tx2_mute_switch:1;
    
    uint32_t screen_off_status:1;
    uint32_t in_box_state:1;
    /// @brief 恢复出厂设置
    uint32_t restore_state:1;
    /// 监听类别

    hl_display_voice_monitor_e monitor_category;
    hl_display_vocie_mode_e voice_module;
    hl_display_sound_module_e sound_module;
    hl_display_low_cut_e low_cut;
    hl_screen_page_e page_id;

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
    int8_t tx1_line_out_volume;
    int8_t tx2_line_out_volume;
    int8_t uac_in_volume;
    int8_t uac_out_volume;
    int8_t tx1_gain_volume;
    int8_t tx2_gain_volume;
    uint8_t led_britness;
    uint8_t tx1_remained_record_time;
    uint8_t tx2_remained_record_time;
    uint8_t ota_upgrade_progress;

    uint8_t tx1_ver[10];
    uint8_t tx2_ver[10];
    uint8_t rx_ver[10];
    uint8_t sn[36];
    uint8_t device_fault_code[20];
    hl_display_systime_s systime;

}hl_display_screen_s;

uint8_t          hl_mod_display_scr_page_incmd(void);
void             hl_mod_display_scr_set_page(uint32_t now_page);
hl_screen_page_e hl_mod_display_scr_get_page(void);

void hl_mod_page_delete(lv_obj_t* obj);
void hl_mod_indev_val_get(mode_to_app_msg_t* p_msg);
uint8_t hl_mod_get_knob_okkey_val(void);
int8_t hl_mod_get_rx_knob_val(void);
void hl_mod_menu_knob_icon_change(uint8_t *center, uint8_t maxnum);

bool hl_mod_next_menu_enter(uint8_t * tab, uint8_t num,uint8_t max_num);
void hl_mod_rx_knob_val_pro(struct _lv_indev_drv_t* drv, lv_indev_data_t* data);
void hl_mod_page_event_btn_init(lv_event_cb_t event_cb);
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