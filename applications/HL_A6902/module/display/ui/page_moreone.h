//#pragma once
#ifndef _PAGE_MOREONE_H
#define _PAGE_MOREONE_H

#include "lvgl.h"
#include "stdio.h"

typedef enum _hl_moreone_check_t
{
    HL_MOREONE_CHECK_ONE_LEFT = 0,
    HL_MOREONE_CHECK_ONE_RIGHT,
    HL_MOREONE_CHECK_TWO_LEFT,
    HL_MOREONE_CHECK_TWO_RIGHT,
    HL_MOREONE_CHECK_THREE_LEFT,
    HL_MOREONE_CHECK_THREE_RIGHT
}hl_moreone_check_t;

typedef enum _hl_moreone_choose_t
{
    HL_MOREONE_CHOOSE_LEFT = 0,
    HL_MOREONE_CHOOSE_RIGHT    
}hl_moreone_choose_t;

typedef enum _hl_moreone_mid_opt_t
{
    HL_MOREONE_OPTION_ONE = 0,
    HL_MOREONE_OPTION_TWO,
    HL_MOREONE_OPTION_THREE,
    HL_MOREONE_OPTION_DELETE_STYLE,
    HL_MOREONE_OPTION_EXTI
}hl_moreone_mid_opt_t;

typedef enum _hl_moreone_cmd_type_t
{
    HL_MOREONE_POSTION_CMD = 0,
    HL_MOREONE_OPTION_CMD    
}hl_moreone_cmd_type_t;

typedef void(*hl_moreone_event_cb)(hl_moreone_check_t);

typedef struct __hl_lvgl_moreone_option_t
{
    hl_moreone_choose_t option_one;
    hl_moreone_choose_t option_two;
    hl_moreone_choose_t option_three;
}hl_lvgl_moreone_option_t;

typedef struct __hl_lvgl_moreone_init_t
{
    hl_lvgl_moreone_option_t moreone_choose_opt;//
    hl_moreone_mid_opt_t moreone_mid_opt;
    hl_moreone_event_cb func_cb;
}hl_lvgl_moreone_init_t;

typedef struct __hl_lvgl_moreone_ioctl_t
{
    hl_moreone_cmd_type_t cmd_type;
    hl_moreone_choose_t moreone_choose;//
    hl_moreone_mid_opt_t moreone_choose_opt;
}hl_lvgl_moreone_ioctl_t;

void page_moreone_test(void);
void hl_mod_moreone_ioctl(void * ctl_data);
void hl_mod_moreone_init(void * init_data);

lv_obj_t * hl_moreone_obj_get(uint8_t num);

#endif