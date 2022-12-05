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
#ifndef __HL_MOD_PAGE_H__
#define __HL_MOD_PAGE_H__

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

void hl_mod_page_home_init(void);
void hl_mod_page_menu_init(void);
void hl_mod_page_txconf_init(void);

uint8_t          hl_mod_display_scr_page_incmd(void);
void             hl_mod_display_scr_set_page(uint32_t now_page);
hl_screen_page_e hl_mod_display_scr_get_page(void);

void hl_mod_page_delete(lv_obj_t* obj);
void hl_mod_rx_knob_key_pro(struct _lv_indev_drv_t* drv, lv_indev_data_t* data);
void hl_mod_rx_knob_val_pro(struct _lv_indev_drv_t* drv, lv_indev_data_t* data);
void hl_mod_indev_val_get(mode_to_app_msg_t* p_msg);
#ifdef __cplusplus
} /* extern "C" */
#endif

/* Includes ------------------------------------------------------------------*/
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#endif /* __HL_MOD_PAGE_H__ */
/*
 * EOF
 */