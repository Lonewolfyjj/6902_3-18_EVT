//#pragma once
#ifndef _PAGE_STORAGE_H
#define _PAGE_STORAGE_H

#include "lvgl.h"
#include "stdio.h"

typedef enum _hl_storage_check_t
{
    HL_STORAGE_CHECK_LEFT = 0,
    HL_STORAGE_CHECK_RIGHT
}hl_storage_check_t;

typedef enum _hl_storage_choose_t
{
    HL_STORAGE_CHOOSE_LEFT = 0,
    HL_STORAGE_CHOOSE_RIGHT,
    HL_STORAGE_CHOOSE_DEL_STYLE,
    HL_STORAGE_CHOOSE_EXIT
}hl_storage_choose_t;

// typedef enum _hl_storage_cfg_t
// {
//     HL_STORAGE_SET_LEFT_TIME = 0,
//     HL_STORAGE_SET_RIGHT_TIME,
// }hl_storage_cfg_t;

typedef void(*hl_storage_event_cb)(hl_storage_check_t);

typedef struct __hl_lvgl_storage_init_t
{
    hl_storage_choose_t storage_choose;//
    uint8_t used_tx1;//已使用内存大小，0 - 100%
    uint8_t used_tx2;//已使用内存大小，0 - 100%
    char * ptr_time_tx1;
    char * ptr_time_tx2;
    hl_storage_event_cb func_cb;
}hl_lvgl_storage_init_t;

typedef struct __hl_lvgl_storage_ioctl_t
{
    hl_storage_choose_t storage_choose;//
    // hl_storage_cfg_t storage_time_set;
}hl_lvgl_storage_ioctl_t;

void page_storage_test(void);
void hl_mod_storage_ioctl(void * ctl_data);
void hl_mod_storage_init(void * init_data);
lv_obj_t * hl_storage_obj_get(uint8_t num);

#endif
