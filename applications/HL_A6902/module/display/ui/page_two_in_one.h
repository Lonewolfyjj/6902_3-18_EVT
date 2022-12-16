//#pragma once
#ifndef _PAGE_TWO_IN_ONE_H
#define _PAGE_TWO_IN_ONE_H

#include "lvgl.h"
#include "stdio.h"

typedef enum _hl_two_in_one_check_t
{
    HL_TWO_ONE_CHECK_LEFT = 0,
    HL_TWO_ONE_CHECK_RIGHT
}hl_two_in_one_check_t;

typedef enum _hl_two_in_one_choose_t
{
    HL_TWO_ONE_CHOOSE_LEFT = 0,
    HL_TWO_ONE_CHOOSE_RIGHT,
    HL_TWO_ONE_CHOOSE_DEL_STYLE,
    HL_TWO_ONE_CHOOSE_EXIT
}hl_two_in_one_choose_t;

typedef void(*hl_two_in_one_event_cb)(hl_two_in_one_check_t);

typedef struct __hl_lvgl_two_in_one_init_t
{
    hl_two_in_one_choose_t two_in_one_choose;//
    char * ptr_lift;
    char * ptr_right;
    char * ptr_top;
    hl_two_in_one_event_cb func_cb;
}hl_lvgl_two_in_one_init_t;

typedef struct __hl_lvgl_two_in_one_ioctl_t
{
    hl_two_in_one_choose_t two_in_one_choose;//
}hl_lvgl_two_in_one_ioctl_t;

void page_two_in_one_test(void);
void hl_mod_two_in_one_init(void * init_data);
void hl_mod_two_in_one_ioctl(void * ctl_data);
lv_obj_t * hl_two_in_one_obj_get(uint8_t num);

#endif
