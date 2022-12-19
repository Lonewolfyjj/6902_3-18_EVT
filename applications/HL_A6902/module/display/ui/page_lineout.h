//#pragma once
#ifndef _PAGE_LINEOUT_H
#define _PAGE_LINEOUT_H

#include "lvgl.h"
#include "stdio.h"

typedef enum _hl_lineout_check_t
{
    HL_LINEOUT_CHECK_LEFT = 0,
    HL_LINEOUT_CHECK_RIGHT
}hl_lineout_check_t;

typedef enum _hl_lineout_choose_t
{
    HL_LINEOUT_CHOOSE_LEFT = 0,
    HL_LINEOUT_CHOOSE_RIGHT,
    HL_LINEOUT_DELETE_PAGE,
    HL_LINEOUT_DELETE_STYLE
}hl_lineout_choose_t;

typedef void(*hl_lineout_event_cb)(hl_lineout_check_t);

typedef struct __hl_lvgl_lineout_init_t
{
    hl_lineout_choose_t lineout_choose;//
    int16_t left_volume;//左声道音量 -40 - 60 db
    int16_t right_volume;//右声道音量 -40 - 60 db
    hl_lineout_event_cb func_cb;
}hl_lvgl_lineout_init_t;

typedef struct __hl_lvgl_lineout_ioctl_t
{
    hl_lineout_choose_t lineout_choose;//
}hl_lvgl_lineout_ioctl_t;

void page_lineout_test(void);
void hl_mod_lineout_ioctl(void * ctl_data);
void hl_mod_lineout_init(void * init_data);
lv_obj_t * hl_lineout_obj_get(uint8_t num);

#endif
