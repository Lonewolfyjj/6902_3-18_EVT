#include <rtthread.h>
#if defined(RT_USING_IOT_DISPLAY)
#include "image_info.h"

static const unsigned char date_bkg_80_80[293UL] =
{
    0x03, 0x00, 0xFF, 0xFE, 0xFD, 0xF3, 0x97, 0xFF, 0xDF, 0xBF, 0x7F, 0xFF, 0xFE, 0xFD, 0xFB, 0xF7, 0xCF, 0x7F, 0xFE, 0x7F, 0xFF, 0xF0, 0xE5, 0xF3, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F, 0xF4, 0xF8, 0xF9, 0xF3, 0xD7, 0xDE, 0xDF, 0xBF, 0x7F, 0xFE, 0xE2,
    0xF9, 0xEF, 0xFF, 0xEF, 0x9F, 0x7D, 0x7D, 0xF6, 0xF6, 0xF7, 0xFB, 0xF7, 0x0F, 0x3D, 0x3F, 0x78, 0xFE, 0xFB, 0xFD, 0xC3, 0xD7, 0xBF, 0x7D, 0xFF, 0x7F, 0xFD, 0xD4, 0xF3, 0xEF, 0xFF, 0x8F, 0x9D, 0xFE, 0x7E, 0xFD, 0xFF, 0xA1, 0xB3, 0xD7, 0xAF,
    0xFF, 0x3F, 0x74, 0xDD, 0xFC, 0xFD, 0xDB, 0x07, 0xCF, 0xDF, 0x3F, 0xF9, 0x00, 0xF9, 0xFA, 0xDB, 0x1F, 0xDF, 0xBF, 0x7F, 0x7F, 0x7F, 0xFE, 0xE0, 0x9B, 0xF7, 0x77, 0xDF, 0x9F, 0x3F, 0x7A, 0xFE, 0xFC, 0xC1, 0xF7, 0xE7, 0x8F, 0xBD, 0x3F, 0x7F,
    0xE8, 0xFD, 0xF9, 0x23, 0xEF, 0xCF, 0x1F, 0x78, 0x7F, 0xFE, 0xB8, 0xF9, 0xF3, 0x77, 0xDD, 0x9F, 0xBF, 0x6A, 0xFE, 0xFC, 0x45, 0xF3, 0xE7, 0xAF, 0xB9, 0x3F, 0x7F, 0xDD, 0xD5, 0xFB, 0xE3, 0xEF, 0xCF, 0x1F, 0x7F, 0xFF, 0xD1, 0xF4, 0xF9, 0xB3,
    0xB6, 0xDF, 0xDF, 0x73, 0x7F, 0xFE, 0x7A, 0xEF, 0xF3, 0x67, 0x9B, 0xBF, 0x3F, 0x7F, 0xFE, 0x85, 0xED, 0xEF, 0xE7, 0x5F, 0xBF, 0x7F, 0x7C, 0xFE, 0xEB, 0xFB, 0x9F, 0xE7, 0x1F, 0xBE, 0x7F, 0xF7, 0xFE, 0xD9, 0xF9, 0x4F, 0xEF, 0xFF, 0x9C, 0x7F,
    0xF2, 0xFE, 0xC5, 0xF9, 0xFF, 0xEE, 0xBF, 0x9B, 0x7F, 0xED, 0xFE, 0xB1, 0xF9, 0xAF, 0xEE, 0x7F, 0x9A, 0x7F, 0xE8, 0xFE, 0x9D, 0xF9, 0x5F, 0xEE, 0x3F, 0x99, 0x7F, 0xE3, 0xFE, 0x87, 0xFB, 0x07, 0xEE, 0xFF, 0xD8, 0x38, 0x72, 0xFD, 0x77, 0xFB,
    0x2F, 0xCE, 0x5F, 0x3C, 0x2F, 0x70, 0xD2, 0x70, 0xFB, 0x83, 0xE5, 0xCF, 0xDF, 0xA8, 0xD9, 0xFE, 0x4C, 0xC5, 0xF6, 0x97, 0x9A, 0x96, 0xBF, 0xFD, 0xFE, 0xF8, 0xB3, 0xA2, 0x17, 0x6E, 0xB3, 0xBF, 0xF1, 0x92, 0xFD, 0x95, 0xAB, 0xEC, 0x0F, 0xBD,
    0x7B, 0x48, 0xDA, 0xAA, 0x21, 0xFA, 0xF7, 0xEF, 0x3F, 0x86, 0x19, 0xFF, 0x07
};

image_info_t date_bkg_info =
{
    .type  = IMG_TYPE_COMPRESS,
    .pixel = RTGRAPHIC_PIXEL_FORMAT_RGB332,
    .x = 0,
    .y = 0,
    .w = 80,
    .h = 80,
    .size = 293UL,
    .data = (uint8_t *)date_bkg_80_80,
};
#endif
