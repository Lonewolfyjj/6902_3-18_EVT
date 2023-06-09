#include "lvgl/lvgl.h"

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_ICON_BT
#define LV_ATTRIBUTE_IMG_ICON_BT
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_IMG_ICON_BT uint8_t icon_bt_map[] = {
#if LV_COLOR_DEPTH == 1 || LV_COLOR_DEPTH == 8
  /*Pixel format: Alpha 8 bit, Red: 3 bit, Green: 3 bit, Blue: 2 bit*/
  0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x2b, 0xff, 0x54, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 
  0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x7f, 0xff, 0xf0, 0xff, 0x6c, 0xff, 0x00, 0xff, 0x00, 
  0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0xd0, 0xff, 0x8b, 0xff, 0x00, 
  0xff, 0x7f, 0xff, 0x0b, 0xff, 0x00, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0x00, 0xff, 0xaf, 0xff, 0xa8, 
  0xff, 0xa8, 0xff, 0xbb, 0xff, 0x20, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0x00, 0xff, 0xaf, 0xff, 0xa8, 
  0xff, 0x00, 0xff, 0x8b, 0xff, 0xc4, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0xcf, 0xff, 0x8b, 0xff, 0x00, 
  0xff, 0x00, 0xff, 0x00, 0xff, 0x68, 0xff, 0xd0, 0xff, 0xef, 0xff, 0x68, 0xff, 0x00, 0xff, 0x00, 
  0xff, 0x00, 0xff, 0x00, 0xff, 0x68, 0xff, 0xcf, 0xff, 0xef, 0xff, 0x68, 0xff, 0x00, 0xff, 0x00, 
  0xff, 0x00, 0xff, 0x8b, 0xff, 0xc4, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0xcf, 0xff, 0x8b, 0xff, 0x00, 
  0xff, 0xa8, 0xff, 0xb8, 0xff, 0x20, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0x00, 0xff, 0xaf, 0xff, 0xa8, 
  0xff, 0x7f, 0xff, 0x0b, 0xff, 0x00, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0x00, 0xff, 0xaf, 0xff, 0xa8, 
  0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0xcf, 0xff, 0x8b, 0xff, 0x00, 
  0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x7f, 0xff, 0xf0, 0xff, 0x70, 0xff, 0x00, 0xff, 0x00, 
  0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x2b, 0xff, 0x58, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 
#endif
#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP == 0
  /*Pixel format: Alpha 8 bit, Red: 5 bit, Green: 6 bit, Blue: 5 bit*/
  0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xdf, 0xff, 0x2b, 0xdf, 0xff, 0x54, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xdf, 0xff, 0x7f, 0xdf, 0xff, 0xf0, 0xdf, 0xff, 0x6c, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xdf, 0xff, 0x7f, 0xdf, 0xff, 0x7f, 0xdf, 0xff, 0xd0, 0xdf, 0xff, 0x8b, 0xff, 0xff, 0x00, 
  0xdf, 0xff, 0x7f, 0xdf, 0xff, 0x0b, 0xff, 0xff, 0x00, 0xdf, 0xff, 0x7f, 0xdf, 0xff, 0x7f, 0xff, 0xff, 0x00, 0xdf, 0xff, 0xaf, 0xdf, 0xff, 0xa8, 
  0xdf, 0xff, 0xa8, 0xdf, 0xff, 0xbb, 0xdf, 0xff, 0x20, 0xdf, 0xff, 0x7f, 0xdf, 0xff, 0x7f, 0xff, 0xff, 0x00, 0xdf, 0xff, 0xaf, 0xdf, 0xff, 0xa8, 
  0xff, 0xff, 0x00, 0xdf, 0xff, 0x8b, 0xdf, 0xff, 0xc4, 0xdf, 0xff, 0x7f, 0xdf, 0xff, 0x7f, 0xdf, 0xff, 0xcf, 0xdf, 0xff, 0x8b, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xdf, 0xff, 0x68, 0xdf, 0xff, 0xd0, 0xdf, 0xff, 0xef, 0xdf, 0xff, 0x68, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xdf, 0xff, 0x68, 0xdf, 0xff, 0xcf, 0xdf, 0xff, 0xef, 0xdf, 0xff, 0x68, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0x00, 0xdf, 0xff, 0x8b, 0xdf, 0xff, 0xc4, 0xdf, 0xff, 0x7f, 0xdf, 0xff, 0x7f, 0xdf, 0xff, 0xcf, 0xdf, 0xff, 0x8b, 0xff, 0xff, 0x00, 
  0xdf, 0xff, 0xa8, 0xdf, 0xff, 0xb8, 0xdf, 0xff, 0x20, 0xdf, 0xff, 0x7f, 0xdf, 0xff, 0x7f, 0xff, 0xff, 0x00, 0xdf, 0xff, 0xaf, 0xdf, 0xff, 0xa8, 
  0xdf, 0xff, 0x7f, 0xdf, 0xff, 0x0b, 0xff, 0xff, 0x00, 0xdf, 0xff, 0x7f, 0xdf, 0xff, 0x7f, 0xff, 0xff, 0x00, 0xdf, 0xff, 0xaf, 0xdf, 0xff, 0xa8, 
  0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xdf, 0xff, 0x7f, 0xdf, 0xff, 0x7f, 0xdf, 0xff, 0xcf, 0xdf, 0xff, 0x8b, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xdf, 0xff, 0x7f, 0xdf, 0xff, 0xf0, 0xdf, 0xff, 0x70, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xdf, 0xff, 0x2b, 0xdf, 0xff, 0x58, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
#endif
#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP != 0
  /*Pixel format: Alpha 8 bit, Red: 5 bit, Green: 6 bit, Blue: 5 bit  BUT the 2  color bytes are swapped*/
  0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xdf, 0x2b, 0xff, 0xdf, 0x54, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xdf, 0x7f, 0xff, 0xdf, 0xf0, 0xff, 0xdf, 0x6c, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xdf, 0x7f, 0xff, 0xdf, 0x7f, 0xff, 0xdf, 0xd0, 0xff, 0xdf, 0x8b, 0xff, 0xff, 0x00, 
  0xff, 0xdf, 0x7f, 0xff, 0xdf, 0x0b, 0xff, 0xff, 0x00, 0xff, 0xdf, 0x7f, 0xff, 0xdf, 0x7f, 0xff, 0xff, 0x00, 0xff, 0xdf, 0xaf, 0xff, 0xdf, 0xa8, 
  0xff, 0xdf, 0xa8, 0xff, 0xdf, 0xbb, 0xff, 0xdf, 0x20, 0xff, 0xdf, 0x7f, 0xff, 0xdf, 0x7f, 0xff, 0xff, 0x00, 0xff, 0xdf, 0xaf, 0xff, 0xdf, 0xa8, 
  0xff, 0xff, 0x00, 0xff, 0xdf, 0x8b, 0xff, 0xdf, 0xc4, 0xff, 0xdf, 0x7f, 0xff, 0xdf, 0x7f, 0xff, 0xdf, 0xcf, 0xff, 0xdf, 0x8b, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xdf, 0x68, 0xff, 0xdf, 0xd0, 0xff, 0xdf, 0xef, 0xff, 0xdf, 0x68, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xdf, 0x68, 0xff, 0xdf, 0xcf, 0xff, 0xdf, 0xef, 0xff, 0xdf, 0x68, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0x00, 0xff, 0xdf, 0x8b, 0xff, 0xdf, 0xc4, 0xff, 0xdf, 0x7f, 0xff, 0xdf, 0x7f, 0xff, 0xdf, 0xcf, 0xff, 0xdf, 0x8b, 0xff, 0xff, 0x00, 
  0xff, 0xdf, 0xa8, 0xff, 0xdf, 0xb8, 0xff, 0xdf, 0x20, 0xff, 0xdf, 0x7f, 0xff, 0xdf, 0x7f, 0xff, 0xff, 0x00, 0xff, 0xdf, 0xaf, 0xff, 0xdf, 0xa8, 
  0xff, 0xdf, 0x7f, 0xff, 0xdf, 0x0b, 0xff, 0xff, 0x00, 0xff, 0xdf, 0x7f, 0xff, 0xdf, 0x7f, 0xff, 0xff, 0x00, 0xff, 0xdf, 0xaf, 0xff, 0xdf, 0xa8, 
  0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xdf, 0x7f, 0xff, 0xdf, 0x7f, 0xff, 0xdf, 0xcf, 0xff, 0xdf, 0x8b, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xdf, 0x7f, 0xff, 0xdf, 0xf0, 0xff, 0xdf, 0x70, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xdf, 0x2b, 0xff, 0xdf, 0x58, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
#endif
#if LV_COLOR_DEPTH == 32
  0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xf9, 0xf9, 0xf9, 0x2b, 0xf9, 0xf9, 0xf9, 0x54, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xf9, 0xf9, 0xf9, 0x7f, 0xf9, 0xf9, 0xf9, 0xf0, 0xf9, 0xf9, 0xf9, 0x6c, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xf9, 0xf9, 0xf9, 0x7f, 0xf9, 0xf9, 0xf9, 0x7f, 0xf9, 0xf9, 0xf9, 0xd0, 0xf9, 0xf9, 0xf9, 0x8b, 0xff, 0xff, 0xff, 0x00, 
  0xf9, 0xf9, 0xf9, 0x7f, 0xf9, 0xf9, 0xf9, 0x0b, 0xff, 0xff, 0xff, 0x00, 0xf9, 0xf9, 0xf9, 0x7f, 0xf9, 0xf9, 0xf9, 0x7f, 0xff, 0xff, 0xff, 0x00, 0xf9, 0xf9, 0xf9, 0xaf, 0xf9, 0xf9, 0xf9, 0xa8, 
  0xf9, 0xf9, 0xf9, 0xa8, 0xf9, 0xf9, 0xf9, 0xbb, 0xf9, 0xf9, 0xf9, 0x20, 0xf9, 0xf9, 0xf9, 0x7f, 0xf9, 0xf9, 0xf9, 0x7f, 0xff, 0xff, 0xff, 0x00, 0xf9, 0xf9, 0xf9, 0xaf, 0xf9, 0xf9, 0xf9, 0xa8, 
  0xff, 0xff, 0xff, 0x00, 0xf9, 0xf9, 0xf9, 0x8b, 0xf9, 0xf9, 0xf9, 0xc4, 0xf9, 0xf9, 0xf9, 0x7f, 0xf9, 0xf9, 0xf9, 0x7f, 0xf9, 0xf9, 0xf9, 0xcf, 0xf9, 0xf9, 0xf9, 0x8b, 0xff, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xf9, 0xf9, 0xf9, 0x68, 0xf9, 0xf9, 0xf9, 0xd0, 0xf9, 0xf9, 0xf9, 0xef, 0xf9, 0xf9, 0xf9, 0x68, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xf9, 0xf9, 0xf9, 0x68, 0xf9, 0xf9, 0xf9, 0xcf, 0xf9, 0xf9, 0xf9, 0xef, 0xf9, 0xf9, 0xf9, 0x68, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0xff, 0x00, 0xf9, 0xf9, 0xf9, 0x8b, 0xf9, 0xf9, 0xf9, 0xc4, 0xf9, 0xf9, 0xf9, 0x7f, 0xf9, 0xf9, 0xf9, 0x7f, 0xf9, 0xf9, 0xf9, 0xcf, 0xf9, 0xf9, 0xf9, 0x8b, 0xff, 0xff, 0xff, 0x00, 
  0xf9, 0xf9, 0xf9, 0xa8, 0xf9, 0xf9, 0xf9, 0xb8, 0xf9, 0xf9, 0xf9, 0x20, 0xf9, 0xf9, 0xf9, 0x7f, 0xf9, 0xf9, 0xf9, 0x7f, 0xff, 0xff, 0xff, 0x00, 0xf9, 0xf9, 0xf9, 0xaf, 0xf9, 0xf9, 0xf9, 0xa8, 
  0xf9, 0xf9, 0xf9, 0x7f, 0xf9, 0xf9, 0xf9, 0x0b, 0xff, 0xff, 0xff, 0x00, 0xf9, 0xf9, 0xf9, 0x7f, 0xf9, 0xf9, 0xf9, 0x7f, 0xff, 0xff, 0xff, 0x00, 0xf9, 0xf9, 0xf9, 0xaf, 0xf9, 0xf9, 0xf9, 0xa8, 
  0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xf9, 0xf9, 0xf9, 0x7f, 0xf9, 0xf9, 0xf9, 0x7f, 0xf9, 0xf9, 0xf9, 0xcf, 0xf9, 0xf9, 0xf9, 0x8b, 0xff, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xf9, 0xf9, 0xf9, 0x7f, 0xf9, 0xf9, 0xf9, 0xf0, 0xf9, 0xf9, 0xf9, 0x70, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xf9, 0xf9, 0xf9, 0x2b, 0xf9, 0xf9, 0xf9, 0x58, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 
#endif
};

const lv_img_dsc_t icon_bt = {
  .header.always_zero = 0,
  .header.w = 8,
  .header.h = 14,
  .data_size = 112 * LV_IMG_PX_SIZE_ALPHA_BYTE,
  .header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,
  .data = icon_bt_map,
};

