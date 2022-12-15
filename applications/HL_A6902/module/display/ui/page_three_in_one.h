//#pragma once
#ifndef _PAGE_DIQIE_H
#define _PAGE_DIQIE_H

#include "lvgl.h"
#include "stdio.h"

typedef enum _hl_three_in_one_check_t
{
    HL_THREE_ONE_CHECK_LEFT = 0,
    HL_THREE_ONE_CHECK_MID,
    HL_THREE_ONE_CHECK_RIGHT
}hl_three_in_one_check_t;

typedef enum _hl_three_in_one_choose_t
{
    HL_THREE_ONE_CHOOSE_LEFT = 0,
    HL_THREE_ONE_CHOOSE_MID,
    HL_THREE_ONE_CHOOSE_RIGHT,
    HL_THREE_ONE_CHOOSE_EXIT
}hl_three_in_one_choose_t;

typedef void(*hl_three_in_one_event_cb)(hl_three_in_one_check_t);

typedef struct __hl_lvgl_three_in_one_init_t
{
    hl_three_in_one_choose_t three_in_one_choose;//
    char * ptr_lift;
    char * ptr_mid;
    char * ptr_right;
    char * ptr_top;
    hl_three_in_one_event_cb func_cb;
}hl_lvgl_three_in_one_init_t;

typedef struct __hl_lvgl_three_in_one_ioctl_t
{
    hl_three_in_one_choose_t three_in_one_choose;//
}hl_lvgl_three_in_one_ioctl_t;

void page_diqie_test(void);
void hl_mod_three_in_one_ioctl(void * ctl_data);
void hl_mod_three_in_one_init(void * init_data);
lv_obj_t * hl_diqie_obj_get(uint8_t num);

#endif
