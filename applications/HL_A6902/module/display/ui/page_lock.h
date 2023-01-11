//#pragma once
#ifndef _PAGE_LOCK_H
#define _PAGE_LOCK_H

#include "lvgl.h"
#include "stdio.h"

typedef enum _hl_lvgl_lock_cmd_t
{
    HL_LOCK_ICON_SHOW = 0,
    HL_LOCK_ICON_HIDE,
    HL_UNLOCK_ICON_SHOW,
    HL_UNLOCK_ICON_HIDE,
    HL_LOCK_PAGE_INIT,
    HL_LOCK_PAGE_EXIT,
}hl_lvgl_lock_cmd_t;

typedef struct __hl_lvgl_lock_ioctl_t
{
    hl_lvgl_lock_cmd_t cmd;//
}hl_lvgl_lock_ioctl_t;

void hl_mod_lock_ioctl(void * ctl_data);
#endif
