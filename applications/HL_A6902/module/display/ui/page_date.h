//#pragma once
#ifndef _PAGE_DATE_H
#define _PAGE_DATE_H

#include "lvgl.h"
#include "stdio.h"

typedef enum _hl_date_choose_t
{
    HL_DATE_YEAR = 0,
    HL_DATE_MONTH,
    HL_DATE_DAY,
    HL_DATE_HOUR,
    HL_DATE_MIN
}hl_date_choose_t;

typedef enum _hl_date_cmd_t
{
    HL_DATE_SET_OPT_CMD = 0,
    HL_DATE_SET_VALUE_CMD,
    HL_DATE_DELETE_STYLE_CMD,
    HL_DATE_EXTI_CMD
}hl_date_cmd_t;

typedef void(*hl_date_event_cb)(hl_date_choose_t,int16_t);

typedef struct _hl_lvgl_date_init_t
{
    hl_date_choose_t current_choose;
    int16_t year;
    int8_t month;
    int8_t day;
    int8_t hour;
    int8_t min;
    hl_date_event_cb func_cb;
}hl_lvgl_date_init_t;

typedef struct _hl_lvgl_date_ioctl_t
{
    hl_date_cmd_t date_cmd;//
    hl_date_choose_t opt;
    int16_t param;
}hl_lvgl_date_ioctl_t;

void hl_mod_date_ioctl(void * ctl_data);
void hl_mod_date_init(void * init_data);
void page_date_test(void);
lv_obj_t * hl_date_obj_get(uint8_t num);

#endif