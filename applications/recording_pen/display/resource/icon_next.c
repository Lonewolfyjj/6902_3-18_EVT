#include "lvgl/lvgl.h"

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_ICON_NEXT
#define LV_ATTRIBUTE_IMG_ICON_NEXT
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_IMG_ICON_NEXT uint8_t icon_next_map[] = {
#if LV_COLOR_DEPTH == 1 || LV_COLOR_DEPTH == 8
  /*Pixel format: Alpha 8 bit, Red: 3 bit, Green: 3 bit, Blue: 2 bit*/
  0x52, 0xb7, 0x52, 0x30, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0xb7, 0x52, 0x30, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0xff, 0x00, 0xff, 0x00, 
  0x52, 0xff, 0x52, 0xfc, 0x52, 0x9c, 0x52, 0x1f, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0xff, 0x52, 0xfc, 0x52, 0x9c, 0x52, 0x1f, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0xff, 0x00, 0xff, 0x00, 
  0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xf7, 0x52, 0x84, 0x52, 0x10, 0x52, 0x00, 0x52, 0x00, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xf7, 0x52, 0x84, 0x52, 0x10, 0x52, 0x00, 0x52, 0x00, 0xff, 0x00, 0xff, 0x00, 
  0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xeb, 0x52, 0x6b, 0x52, 0x07, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xeb, 0x52, 0x6b, 0x52, 0x07, 0xff, 0x00, 0xff, 0x00, 
  0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xdb, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xdb, 0x52, 0x53, 0x52, 0x00, 
  0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0x8f, 
  0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xdb, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xdb, 0x52, 0x50, 0xff, 0x00, 
  0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xeb, 0x52, 0x6b, 0x52, 0x04, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xeb, 0x52, 0x6b, 0x52, 0x04, 0xff, 0x00, 0xff, 0x00, 
  0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xf7, 0x52, 0x84, 0x52, 0x0c, 0x52, 0x00, 0x52, 0x00, 0x52, 0xff, 0x52, 0xff, 0x52, 0xff, 0x52, 0xf7, 0x52, 0x84, 0x52, 0x0c, 0x52, 0x00, 0x52, 0x00, 0xff, 0x00, 0xff, 0x00, 
  0x52, 0xff, 0x52, 0xfc, 0x52, 0x9c, 0x52, 0x1b, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0xff, 0x52, 0xfc, 0x52, 0x9c, 0x52, 0x1b, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0xff, 0x00, 0xff, 0x00, 
  0x52, 0xb7, 0x52, 0x2c, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0xb7, 0x52, 0x2c, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0x52, 0x00, 0xff, 0x00, 0xff, 0x00, 
#endif
#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP == 0
  /*Pixel format: Alpha 8 bit, Red: 5 bit, Green: 6 bit, Blue: 5 bit*/
  0x92, 0x4b, 0xb7, 0x92, 0x4b, 0x30, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0xb7, 0x92, 0x4b, 0x30, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0x92, 0x4b, 0xff, 0x92, 0x4b, 0xfc, 0x92, 0x4b, 0x9c, 0x92, 0x4b, 0x1f, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xfc, 0x92, 0x4b, 0x9c, 0x92, 0x4b, 0x1f, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xf7, 0x92, 0x4b, 0x84, 0x92, 0x4b, 0x10, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xf7, 0x92, 0x4b, 0x84, 0x92, 0x4b, 0x10, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xeb, 0x92, 0x4b, 0x6b, 0x92, 0x4b, 0x07, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xeb, 0x92, 0x4b, 0x6b, 0x92, 0x4b, 0x07, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xdb, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xdb, 0x92, 0x4b, 0x53, 0x92, 0x4b, 0x00, 
  0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0x8f, 
  0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xdb, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xdb, 0x92, 0x4b, 0x50, 0xff, 0xff, 0x00, 
  0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xeb, 0x92, 0x4b, 0x6b, 0x92, 0x4b, 0x04, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xeb, 0x92, 0x4b, 0x6b, 0x92, 0x4b, 0x04, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xf7, 0x92, 0x4b, 0x84, 0x92, 0x4b, 0x0c, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xf7, 0x92, 0x4b, 0x84, 0x92, 0x4b, 0x0c, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0x92, 0x4b, 0xff, 0x92, 0x4b, 0xfc, 0x92, 0x4b, 0x9c, 0x92, 0x4b, 0x1b, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0xff, 0x92, 0x4b, 0xfc, 0x92, 0x4b, 0x9c, 0x92, 0x4b, 0x1b, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0x92, 0x4b, 0xb7, 0x92, 0x4b, 0x2c, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0xb7, 0x92, 0x4b, 0x2c, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0x92, 0x4b, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
#endif
#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP != 0
  /*Pixel format: Alpha 8 bit, Red: 5 bit, Green: 6 bit, Blue: 5 bit  BUT the 2  color bytes are swapped*/
  0x4b, 0x92, 0xb7, 0x4b, 0x92, 0x30, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0xb7, 0x4b, 0x92, 0x30, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0x4b, 0x92, 0xff, 0x4b, 0x92, 0xfc, 0x4b, 0x92, 0x9c, 0x4b, 0x92, 0x1f, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xfc, 0x4b, 0x92, 0x9c, 0x4b, 0x92, 0x1f, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xf7, 0x4b, 0x92, 0x84, 0x4b, 0x92, 0x10, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xf7, 0x4b, 0x92, 0x84, 0x4b, 0x92, 0x10, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xeb, 0x4b, 0x92, 0x6b, 0x4b, 0x92, 0x07, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xeb, 0x4b, 0x92, 0x6b, 0x4b, 0x92, 0x07, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xdb, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xdb, 0x4b, 0x92, 0x53, 0x4b, 0x92, 0x00, 
  0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0x8f, 
  0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xdb, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xdb, 0x4b, 0x92, 0x50, 0xff, 0xff, 0x00, 
  0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xeb, 0x4b, 0x92, 0x6b, 0x4b, 0x92, 0x04, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xeb, 0x4b, 0x92, 0x6b, 0x4b, 0x92, 0x04, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xf7, 0x4b, 0x92, 0x84, 0x4b, 0x92, 0x0c, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xf7, 0x4b, 0x92, 0x84, 0x4b, 0x92, 0x0c, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0x4b, 0x92, 0xff, 0x4b, 0x92, 0xfc, 0x4b, 0x92, 0x9c, 0x4b, 0x92, 0x1b, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0xff, 0x4b, 0x92, 0xfc, 0x4b, 0x92, 0x9c, 0x4b, 0x92, 0x1b, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
  0x4b, 0x92, 0xb7, 0x4b, 0x92, 0x2c, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0xb7, 0x4b, 0x92, 0x2c, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0x4b, 0x92, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 
#endif
#if LV_COLOR_DEPTH == 32
  0x93, 0x71, 0x4c, 0xb7, 0x93, 0x71, 0x4c, 0x30, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0xb7, 0x93, 0x71, 0x4c, 0x30, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 
  0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xfc, 0x93, 0x71, 0x4c, 0x9c, 0x93, 0x71, 0x4c, 0x1f, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xfc, 0x93, 0x71, 0x4c, 0x9c, 0x93, 0x71, 0x4c, 0x1f, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 
  0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xf7, 0x93, 0x71, 0x4c, 0x84, 0x93, 0x71, 0x4c, 0x10, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xf7, 0x93, 0x71, 0x4c, 0x84, 0x93, 0x71, 0x4c, 0x10, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 
  0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xeb, 0x93, 0x71, 0x4c, 0x6b, 0x93, 0x71, 0x4c, 0x07, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xeb, 0x93, 0x71, 0x4c, 0x6b, 0x93, 0x71, 0x4c, 0x07, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 
  0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xdb, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xdb, 0x93, 0x71, 0x4c, 0x53, 0x93, 0x71, 0x4c, 0x00, 
  0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0x8f, 
  0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xdb, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xdb, 0x93, 0x71, 0x4c, 0x50, 0xff, 0xff, 0xff, 0x00, 
  0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xeb, 0x93, 0x71, 0x4c, 0x6b, 0x93, 0x71, 0x4c, 0x04, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xeb, 0x93, 0x71, 0x4c, 0x6b, 0x93, 0x71, 0x4c, 0x04, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 
  0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xf7, 0x93, 0x71, 0x4c, 0x84, 0x93, 0x71, 0x4c, 0x0c, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xf7, 0x93, 0x71, 0x4c, 0x84, 0x93, 0x71, 0x4c, 0x0c, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 
  0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xfc, 0x93, 0x71, 0x4c, 0x9c, 0x93, 0x71, 0x4c, 0x1b, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0xff, 0x93, 0x71, 0x4c, 0xfc, 0x93, 0x71, 0x4c, 0x9c, 0x93, 0x71, 0x4c, 0x1b, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 
  0x93, 0x71, 0x4c, 0xb7, 0x93, 0x71, 0x4c, 0x2c, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0xb7, 0x93, 0x71, 0x4c, 0x2c, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0x93, 0x71, 0x4c, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 
#endif
};

const lv_img_dsc_t icon_next = {
  .header.always_zero = 0,
  .header.w = 18,
  .header.h = 11,
  .data_size = 198 * LV_IMG_PX_SIZE_ALPHA_BYTE,
  .header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,
  .data = icon_next_map,
};

