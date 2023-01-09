//#pragma once
#ifndef _PAGE_ARC_H
#define _PAGE_ARC_H

#include "lvgl.h"
#include "stdio.h"

typedef enum _hl_lvgl_arc_cmd_type_t
{
    HL_ARC_DELETE_PAGE,
    HL_ARC_DELETE_STYLE
}hl_lvgl_arc_cmd_type_t;

typedef struct __hl_lvgl_arc_init_t
{
    char * ptr;
}hl_lvgl_arc_init_t;

typedef struct __hl_lvgl_arc_ioctl_t
{
    hl_lvgl_arc_cmd_type_t ioctl_cmd;
}hl_lvgl_arc_ioctl_t;

void hl_mod_arc_ioctl(void * ctl_data);
void hl_mod_arc_init(void * init_data);

#endif
