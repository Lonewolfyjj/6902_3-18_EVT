//#pragma once
#ifndef _PAGE_TOP_H
#define _PAGE_TOP_H

#include "lvgl.h"
#include "stdio.h"

typedef enum _hl_top_icon_t
{
    HL_TOP_ICON_STEREO_MOD = 0,
    HL_TOP_ICON_SINGLE_MOD,
    HL_TOP_ICON_TRACK_MOD,
    HL_TOP_ICON_NOISE,
    HL_TOP_ICON_LOCK,
    HL_TOP_ICON_LINEOUT,
    HL_TOP_ICON_TYPEC,
    HL_TOP_ICON_HEATSET,
    HL_TOP_ICON_CNT,
}hl_top_icon_t;

typedef enum _hl_top_cmd_t
{
    HL_TOP_DELETE_ICON_CMD = 0,
    HL_TOP_ADD_ICON_CMD,
    HL_TOP_BAT_VAL,
    HL_TOP_BAT_COLOR_GREEN,
    HL_TOP_BAT_COLOR_WHITE,
    HL_TOP_DELETE_STYLE,
    HL_TOP_ALL_DEL
}hl_top_cmd_t;

typedef struct _hl_lvgl_top_ioctl_t
{
    hl_top_cmd_t top_cmd;
    hl_top_icon_t top_param;
    int16_t electric_top;//电量 0 - 100 %
}hl_lvgl_top_ioctl_t;

typedef struct _hl_lvgl_top_init_t
{
    int16_t electric_top;//电量 0 - 100 %
}hl_lvgl_top_init_t;

// void page_top_test(void);
void hl_mod_top_ioctl(void * ctl_data);
void hl_mod_top_init(void * init_data);

#endif