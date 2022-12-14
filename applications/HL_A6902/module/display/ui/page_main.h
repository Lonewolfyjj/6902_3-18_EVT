//#pragma once
#ifndef _PAGE_MAIN_H
#define _PAGE_MAIN_H

#include "lvgl.h"
#include "stdio.h"

typedef enum _hl_record_status_t
{
    // 未录制
    HL_NO_RECODE = 0,
    // 录制中
    HL_RECODING
}hl_record_status_t;

typedef enum _hl_signal_int_t
{
    //无信号
    HL_NO_SIGNAL = 0,
    //一格信号
    HL_ONE_SIGNAL,
    //两格信号
    HL_TWO_SIGNAL,
    //三格信号
    HL_THREE_SIGNAL,
    //四格信号
    HL_FOUR_SIGNAL
}hl_signal_int_t;

typedef enum _hl_device_display_t
{
    //只显示TX1
    HL_DISPLAY_TX1 = 0,
    //只显示TX2
    HL_DISPLAY_TX2,
    //显示TX1和TX2
    HL_DISPLAY_DOUBLE
}hl_device_display_t;

typedef struct __device_data_t
{
    hl_record_status_t record;//录制状态 hl_record_status_t
    int16_t volume;//Tx音量 -40 - 60 db
    int16_t electric;//电量 0 - 100 %
    hl_signal_int_t signal;//信号强度 0 - 4
}device_data_t;

typedef enum _hl_lvgl_main_cmd_type_t
{
    HL_CHANGE_TX1_DEVICE = 0,
    HL_CHANGE_TX2_DEVICE,
    HL_CHANGE_DOUBLE_DEVICE,

    HL_CHANGE_TX1_SIGNAL,
    HL_CHANGE_TX1_ELEC,
    HL_CHANGE_TX1_VOL,
    HL_CHANGE_TX1_REC,

    HL_CHANGE_TX2_SIGNAL,
    HL_CHANGE_TX2_ELEC,
    HL_CHANGE_TX2_VOL,
    HL_CHANGE_TX2_REC,

    HL_CHANGE_DELETE_PAGE
}hl_lvgl_main_cmd_type_t;

typedef struct __hl_lvgl_main_init_t
{
    hl_device_display_t display_tx_device;//
    device_data_t tx_device_1;
    device_data_t tx_device_2;
}hl_lvgl_main_init_t;

typedef struct __hl_lvgl_main_ioctl_t
{
    hl_lvgl_main_cmd_type_t cmd;//
    device_data_t tx_device_1;
    device_data_t tx_device_2;
}hl_lvgl_main_ioctl_t;

void page_main_test(void);
void hl_mod_main_ioctl(void * ctl_data);
void hl_mod_main_init(void * init_data);

#endif