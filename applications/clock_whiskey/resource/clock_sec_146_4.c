#include <rtthread.h>
#include "image_info.h"

#ifdef CLOCK_WHISKEY_QUALITY_LOW //RGB332
static const unsigned char clock_sec_146_4[584UL] =
{
    0xF6, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1,
    0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1,
    0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1,
    0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xF6, 0xC5, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
    0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
    0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
    0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
    0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC5, 0xC5, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
    0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
    0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
    0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC5, 0xF6, 0xC1,
    0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1,
    0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1,
    0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1,
    0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xF6
};

image_info_t clock_sec_info =
{
    .type  = IMG_TYPE_RAW,
    .pixel = RTGRAPHIC_PIXEL_FORMAT_RGB332,
    .x = 0,
    .y = 0,
    .w = 146,
    .h = 4,
    .size = 584UL,
    .data = (uint8_t *)clock_sec_146_4,
};

#else   //RGB565

static const unsigned char clock_sec_146_4[1168UL] =
{
    0x97, 0xF5, 0x46, 0xD0, 0x25, 0xD0, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8,
    0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8,
    0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8,
    0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8,
    0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8,
    0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8,
    0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8,
    0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x25, 0xD0, 0x46, 0xD0, 0x97, 0xF5, 0xC8, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0,
    0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0,
    0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0,
    0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0,
    0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0,
    0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0,
    0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0,
    0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0xC8, 0xD0, 0xC8, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0,
    0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0,
    0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0,
    0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0,
    0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0,
    0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0,
    0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0,
    0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0xC8, 0xD0, 0x97, 0xF5, 0x46, 0xD0,
    0x25, 0xD0, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8,
    0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8,
    0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8,
    0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8,
    0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8,
    0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8,
    0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8, 0x66, 0xD8,
    0x66, 0xD8, 0x25, 0xD0, 0x46, 0xD0, 0x97, 0xF5
};

image_info_t clock_sec_info =
{
    .type  = IMG_TYPE_RAW,
    .pixel = RTGRAPHIC_PIXEL_FORMAT_RGB565,
    .x = 0,
    .y = 0,
    .w = 146,
    .h = 4,
    .size = 1168UL,
    .data = (uint8_t *)clock_sec_146_4,
};
#endif
