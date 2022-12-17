//#pragma once
#ifndef _PAGE_CHARGE_H
#define _PAGE_CHARGE_H

#include "lvgl.h"
#include "stdio.h"

typedef enum _hl_charge_device_display_t
{
    //显示TX1
    HL_CHARGE_DISPLAY_TX1 = (1<<0),
    //显示RX
    HL_CHARGE_DISPLAY_RX = (1<<1),
    //显示TX2
    HL_CHARGE_DISPLAY_TX2 = (1<<2),
}hl_charge_device_display_t;


typedef enum _hl_lvgl_charge_cmd_type_t
{
    HL_DELETE_TX1_DEVICE = 0,
    HL_DELETE_RX_DEVICE,
    HL_DELETE_TX2_DEVICE,    

    HL_ADD_TX1_DEVICE,
    HL_ADD_RX_DEVICE,
    HL_ADD_TX2_DEVICE,
    
    HL_CHARGE_CHANGE_BOX_ELEC,
    HL_CHARGE_CHANGE_TX1_ELEC,
    HL_CHARGE_CHANGE_RX_ELEC,
    HL_CHARGE_CHANGE_TX2_ELEC,

    HL_CHARGE_CHANGE_DELETE_PAGE,
    HL_CHARGE_CHANGE_DELETE_STYLE
}hl_lvgl_charge_cmd_type_t;

typedef struct __hl_lvgl_charge_elec_t
{
    int16_t electric_box;//电量 0 - 100 %
    int16_t electric_tx1;//电量 0 - 100 %
    int16_t electric_rx;//电量 0 - 100 %
    int16_t electric_tx2;//电量 0 - 100 %
}hl_lvgl_charge_elec_t;

// typedef struct __hl_lvgl_charge_init_t

typedef struct __hl_lvgl_charge_ioctl_t
{
    hl_lvgl_charge_cmd_type_t charge_cmd;//
    hl_lvgl_charge_elec_t electric;
}hl_lvgl_charge_ioctl_t;

typedef struct __hl_lvgl_charge_init_t
{
    hl_charge_device_display_t display_device;//
    hl_lvgl_charge_elec_t electric;
}hl_lvgl_charge_init_t;

// void page_charge_test(void);
void hl_mod_charge_ioctl(void * ctl_data);
void hl_mod_charge_init(void * init_data);
#endif