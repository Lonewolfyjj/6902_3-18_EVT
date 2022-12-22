//#pragma once
#ifndef _PAGE_UPGRADE_H
#define _PAGE_UPGRADE_H

#include "lvgl.h"
#include "stdio.h"

typedef struct _hl_lvgl_upgrade_init_t
{
    uint8_t upgrade_progress;//升级进度 0 - 100
}hl_lvgl_upgrade_init_t;

typedef enum _hl_upgrade_ioctl_t
{
    HL_UPGRADE_PARAM_CMD = 0,
    HL_UPGRADE_SUCCESS_CMD,
    HL_UPGRADE_FAIL_CMD,
    HL_UPGRADE_CLEAR_CMD,
    HL_UPGRADE_DEL_STYLE_CMD,
}hl_upgrade_ioctl_t;

typedef struct _hl_lvgl_upgrade_ioctl_t
{
    uint8_t upgrade_progress;//升级进度 0 - 100
    hl_upgrade_ioctl_t upgrade_ioctl;
}hl_lvgl_upgrade_ioctl_t;


void hl_mod_lvgl_upgrade_ioctl(void * ctl_data);
void hl_mod_lvgl_upgrade_init(void * init_data);

#endif