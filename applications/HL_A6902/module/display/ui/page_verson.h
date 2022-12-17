//#pragma once
#ifndef _PAGE_VERSON_H
#define _PAGE_VERSON_H

#include "lvgl.h"
#include "stdio.h"

typedef enum _hl_verson_mid_opt_t
{
    HL_VERSON_OPTION_ONE = 0,
    HL_VERSON_OPTION_TWO,
    HL_VERSON_OPTION_THREE,
    HL_VERSON_OPTION_DEL_STYLE,
    HL_VERSON_OPTION_EXTI
}hl_verson_mid_opt_t;

typedef struct __hl_lvgl_verson_init_t
{
    const char * hl_verson_sn_number;
    const char * hl_verson_firmware;
    const char * hl_verson_tx1;
    const char * hl_verson_tx2;
}hl_lvgl_verson_init_t;

typedef struct __hl_lvgl_verson_ioctl_t
{
    hl_verson_mid_opt_t verson_choose_opt;
}hl_lvgl_verson_ioctl_t;

void page_verson_test(void);
void hl_mod_verson_ioctl(void * ctl_data);
void hl_mod_verson_init(void * init_data);

#endif