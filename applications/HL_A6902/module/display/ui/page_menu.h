//#pragma once
#ifndef _PAGE_MENU_H
#define _PAGE_MENU_H

#include "lvgl.h"
#include "stdio.h"

#define MAX_MENU_NUMBER     12
#define ADD_IMG_DATA(OBJ,LAB,SRC,PTR) {.obj = OBJ,.lab = LAB,.pic_src = SRC,.ptr = PTR}

typedef struct _menu_data_t{
    lv_obj_t *obj;
    lv_obj_t *lab;
    const lv_img_dsc_t *pic_src;
    const char *ptr;
}menu_data_t;

typedef void(*event_cb)(uint32);

lv_obj_t * hl_menu_obj_get(uint8_t num);
void lv_set_icon_postion(uint8_t pos);
void page_menu_init(menu_data_t *data,uint8_t menu_num,event_cb func_cb);
void lv_menu_exit(void);

#endif
