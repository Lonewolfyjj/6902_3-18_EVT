//#pragma once
#ifndef _PAGE_S_TWO_IN_ONE_H
#define _PAGE_S_TWO_IN_ONE_H

#include "lvgl.h"
#include "stdio.h"

typedef enum _hl_s_two_in_one_check_t
{
    HL_S_TWO_ONE_CHECK_LEFT = 0,
    HL_S_TWO_ONE_CHECK_RIGHT
}hl_s_two_in_one_check_t;

typedef enum _hl_s_two_in_one_choose_t
{
    HL_S_TWO_ONE_CHOOSE_LEFT = 0,
    HL_S_TWO_ONE_CHOOSE_RIGHT,
    HL_S_TWO_ONE_CHOOSE_EXIT
}hl_s_two_in_one_choose_t;

typedef void(*hl_s_two_in_one_event_cb)(hl_s_two_in_one_check_t);

typedef struct __hl_lvgl_s_two_in_one_init_t
{
    hl_s_two_in_one_choose_t s_two_in_one_choose;//
    char * ptr_lift;
    char * ptr_right;
    char * ptr_top;
    hl_s_two_in_one_event_cb func_cb;
}hl_lvgl_s_two_in_one_init_t;

typedef struct __hl_lvgl_s_two_in_one_ioctl_t
{
    hl_s_two_in_one_choose_t s_two_in_one_choose;//
}hl_lvgl_s_two_in_one_ioctl_t;

void page_formatting_test(void);
void hl_mod_s_two_in_one_init(void * init_data);
void hl_mod_s_two_in_one_ioctl(void * ctl_data);
lv_obj_t * hl_formatting_obj_get(uint8_t num);

#endif