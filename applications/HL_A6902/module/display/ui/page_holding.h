#ifndef _PAGE_HOLDING_H
#define _PAGE_HOLDING_H

#include "lvgl.h"
#include "stdio.h"

typedef enum _hl_lvgl_holding_cmd_t
{
    HL_HOLDING_RX_ELEC,

    HL_HOLDING_RX_BAT_COLOR_GREEN,
    HL_HOLDING_RX_BAT_COLOR_WHITE,

    HL_HOLDING_RX_ICON_HIDE,
    HL_HOLDING_RX_ICON_DISHIDE,

    HL_HOLDING_DELETE_PAGE,
    HL_HOLDING_DELETE_STYLE
}hl_lvgl_holding_cmd_t;

typedef struct __hl_lvgl_holding_ioctl_t
{
    hl_lvgl_holding_cmd_t holding_cmd;//
    int16_t electric;
}hl_lvgl_holding_ioctl_t;

typedef struct __hl_lvgl_holding_init_t
{
    int16_t electric;//电量 0 - 100 %
}hl_lvgl_holding_init_t;

void hl_mod_holding_init(void * init_data);
void hl_mod_holding_ioctl(void * ctl_data);
#endif