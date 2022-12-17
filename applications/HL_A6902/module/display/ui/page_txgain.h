//#pragma once
#ifndef _PAGE_TXGAIN_H
#define _PAGE_TXGAIN_H

#include "lvgl.h"
#include "stdio.h"

typedef enum _hl_txgain_check_t
{
    HL_TX_GAIN_CHECK_LEFT = 0,
    HL_TX_GAIN_CHECK_RIGHT
}hl_txgain_check_t;

typedef enum _hl_txgain_choose_t
{
    HL_TX_GAIN_CHOOSE_LEFT = 0,
    HL_TX_GAIN_CHOOSE_RIGHT,
    HL_TX_GAIN_DELETE_STYLE,
    HL_TX_GAIN_DELETE_PAGE
}hl_txgain_choose_t;

typedef void(*hl_txgain_event_cb)(hl_txgain_check_t);

typedef struct __hl_lvgl_txgain_init_t
{
    hl_txgain_choose_t txgain_choose;//
    int16_t left_volume;//左声道音量 -40 - 60 db
    int16_t right_volume;//右声道音量 -40 - 60 db
    hl_txgain_event_cb func_cb;
}hl_lvgl_txgain_init_t;

typedef struct __hl_lvgl_txgain_ioctl_t
{
    hl_txgain_choose_t txgain_choose;//
}hl_lvgl_txgain_ioctl_t;

void page_txgain_test(void);
void hl_mod_txgain_ioctl(void * ctl_data);
void hl_mod_txgain_init(void * init_data);
lv_obj_t * hl_txgain_obj_get(uint8_t num);

#endif
