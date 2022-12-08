//#pragma once
#ifndef _PAGE_TOP_H
#define _PAGE_TOP_H

#include "lvgl.h"
#include "stdio.h"

#define POSTION_IS_NULL 0xFF

 typedef struct __icon_pos_t
{
    uint8_t duf_pos;//默认位置
    uint8_t cur_pos;//当前位置
    lv_align_t align;// 对齐方式
    lv_obj_t * icon;//图片对象
    const lv_img_dsc_t * icon_data;//图片数据指针
}icon_pos_t;

void page_top_test(void);

#endif