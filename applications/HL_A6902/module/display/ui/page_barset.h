//#pragma once
#ifndef _PAGE_BARSET_H
#define _PAGE_BARSET_H

#include "lvgl.h"
#include "stdio.h"

typedef enum _hl_barset_top_icontyp_t
{
    HL_NO_ICON = 0,//无图标
    HL_STEREO_ICON,//立体声
    HL_SAYFT_ICON,//安全音轨
    HL_SINGLE_ICON//单声道
}hl_barset_top_icontyp_t;

typedef enum _hl_barset_ioctl_t
{
    HL_VALUE = 0,//
    HL_DELETE_STYLE,
    HL_EXTI
}hl_barset_ioctl_t;

typedef struct __hl_lvgl_barset_ioctl_t
{
    hl_barset_ioctl_t barset_value;//
    int16_t value;
}hl_lvgl_barset_ioctl_t;

typedef void(*hl_barset_event_cb)(int16_t);

typedef struct __hl_lvgl_barset_init_t
{
    int16_t range_min;//
    int16_t range_max;//
    int16_t init_value;//
    const void * src;
    const char * ptr;
    hl_barset_top_icontyp_t icontyp;
    hl_barset_event_cb func_cb;
}hl_lvgl_barset_init_t;

void hl_mod_barset_ioctl(void * ctl_data);
void hl_mod_barset_init(void * init_data);
lv_obj_t * hl_bar_obj_get(uint8_t num);

#endif