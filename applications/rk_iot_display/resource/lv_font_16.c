
/*
**********************************************************************************
 * Font characters:  !"#*,.0123456789:;?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz°“、。一七万三为九二五他低你八六关分到十千吩咐器四在好已帮年度开您我打抑日时星晚晨月期水沐浴温热百的秒经至菌补说请调速闭问零高！，：；？
**********************************************************************************
*/

#include <rtthread.h>
#if defined(RT_USING_LITTLEVGL2RTT) && defined(RT_USING_IOT_DISPLAY)
#include "lvgl/lvgl.h"

/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/

#ifndef LV_FONT
#define LV_FONT 1
#endif

#if LV_FONT

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t gylph_bitmap[] = {
    /* U+20 " " */
    0x0,

    /* U+21 "!" */
    0x49, 0x24, 0x92, 0x40, 0x24,

    /* U+22 "\"" */
    0xbb, 0xb0,

    /* U+23 "#" */
    0x24, 0x48, 0x91, 0x2f, 0xe4, 0x8a, 0x24, 0xfe,
    0x91, 0x22, 0x40,

    /* U+2A "*" */
    0x11, 0x27, 0x59, 0xc7, 0xda, 0xc4, 0x8,

    /* U+2C "," */
    0xf6,

    /* U+2E "." */
    0xf0,

    /* U+30 "0" */
    0x38, 0xc9, 0xa, 0x1c, 0x38, 0x50, 0xa1, 0x42,
    0xcc, 0xf0, 0xc0,

    /* U+31 "1" */
    0x2f, 0xd2, 0x49, 0x24, 0x90,

    /* U+32 "2" */
    0x7b, 0xb8, 0x41, 0xc, 0x21, 0x84, 0x31, 0x8c,
    0x3f,

    /* U+33 "3" */
    0x0, 0x79, 0x1a, 0x30, 0x61, 0x87, 0x3, 0x3,
    0x8d, 0x11, 0xc0,

    /* U+34 "4" */
    0x4, 0xc, 0xc, 0x14, 0x34, 0x24, 0x44, 0xc4,
    0xff, 0x4, 0x4, 0x4,

    /* U+35 "5" */
    0x3e, 0x40, 0x83, 0x7, 0xcc, 0x80, 0x81, 0x2,
    0x85, 0xb1, 0xc0,

    /* U+36 "6" */
    0x8, 0x30, 0xc1, 0x7, 0xf, 0xb1, 0xc1, 0x83,
    0x5, 0x11, 0xc0,

    /* U+37 "7" */
    0xfe, 0xc, 0x10, 0x60, 0x83, 0x4, 0x8, 0x30,
    0x40, 0x81, 0x0,

    /* U+38 "8" */
    0x79, 0x9a, 0x14, 0x2c, 0xcf, 0x33, 0x43, 0x87,
    0xf, 0xf3, 0xe0,

    /* U+39 "9" */
    0x38, 0x8b, 0xe, 0x1c, 0x78, 0xdf, 0x6, 0x8,
    0x30, 0x41, 0x80,

    /* U+3A ":" */
    0xf0, 0x3c,

    /* U+3B ";" */
    0xf0, 0x3d, 0x80,

    /* U+3F "?" */
    0x7b, 0x18, 0x61, 0xc, 0x63, 0x8, 0x0, 0x3,
    0xc,

    /* U+41 "A" */
    0x18, 0x18, 0x18, 0x38, 0x24, 0x24, 0x24, 0x7e,
    0x46, 0x42, 0x42, 0xc3,

    /* U+42 "B" */
    0xfa, 0x38, 0x61, 0x8f, 0xe8, 0xe1, 0x86, 0x18,
    0xfe,

    /* U+43 "C" */
    0x38, 0x89, 0xc, 0x18, 0x10, 0x20, 0x41, 0x83,
    0x5, 0x11, 0xc0,

    /* U+44 "D" */
    0xf2, 0x28, 0xe1, 0x86, 0x18, 0x61, 0x86, 0x38,
    0xbc,

    /* U+45 "E" */
    0xfe, 0x8, 0x20, 0x83, 0xe8, 0x20, 0x82, 0x8,
    0x3f,

    /* U+46 "F" */
    0xfe, 0x8, 0x20, 0x82, 0xf, 0xa0, 0x82, 0x8,
    0x20,

    /* U+47 "G" */
    0x39, 0x28, 0x61, 0x82, 0x9, 0xe1, 0x86, 0x14,
    0xcf,

    /* U+48 "H" */
    0x86, 0x18, 0x61, 0x87, 0xf8, 0x61, 0x86, 0x18,
    0x61,

    /* U+49 "I" */
    0xff, 0xf0,

    /* U+4A "J" */
    0x4, 0x10, 0x41, 0x4, 0x10, 0x41, 0x86, 0x1c,
    0xde,

    /* U+4B "K" */
    0x8d, 0x12, 0x45, 0x8e, 0x1c, 0x3c, 0x48, 0x99,
    0x12, 0x34, 0x20,

    /* U+4C "L" */
    0x82, 0x8, 0x20, 0x82, 0x8, 0x20, 0x82, 0x8,
    0x3f,

    /* U+4D "M" */
    0xc7, 0x8f, 0x1f, 0x7e, 0xf5, 0x6a, 0xd5, 0xbb,
    0x76, 0x4c, 0x90,

    /* U+4E "N" */
    0x87, 0x1c, 0x79, 0xa6, 0xdb, 0x65, 0x9e, 0x38,
    0xe1,

    /* U+4F "O" */
    0x38, 0x8a, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x5, 0x11, 0xc0,

    /* U+50 "P" */
    0xfa, 0x38, 0x61, 0x86, 0x3f, 0xa0, 0x82, 0x8,
    0x20,

    /* U+51 "Q" */
    0x38, 0x8a, 0xc, 0x18, 0x30, 0x60, 0xc5, 0x8b,
    0x1d, 0x19, 0xf0, 0x0,

    /* U+52 "R" */
    0xfa, 0x38, 0x61, 0x86, 0x3f, 0xa6, 0x8a, 0x28,
    0xe1,

    /* U+53 "S" */
    0x7a, 0x28, 0x60, 0xc1, 0xc1, 0x83, 0x86, 0x1c,
    0xde,

    /* U+54 "T" */
    0xfc, 0x41, 0x4, 0x10, 0x41, 0x4, 0x10, 0x41,
    0x4,

    /* U+55 "U" */
    0x86, 0x18, 0x61, 0x86, 0x18, 0x61, 0x86, 0x1c,
    0xde,

    /* U+56 "V" */
    0xc3, 0x42, 0x42, 0x46, 0x64, 0x24, 0x24, 0x2c,
    0x38, 0x18, 0x18, 0x18,

    /* U+57 "W" */
    0x9b, 0xdb, 0xda, 0xda, 0x5a, 0x5a, 0x6a, 0x6a,
    0x66, 0x66, 0x64, 0x64,

    /* U+58 "X" */
    0x46, 0x66, 0x24, 0x3c, 0x18, 0x18, 0x18, 0x38,
    0x2c, 0x64, 0x46, 0xc2,

    /* U+59 "Y" */
    0xc6, 0x89, 0x11, 0x42, 0x83, 0x4, 0x8, 0x10,
    0x20, 0x40, 0x80,

    /* U+5A "Z" */
    0xfc, 0x30, 0x86, 0x10, 0x42, 0x8, 0x41, 0x8,
    0x3f,

    /* U+61 "a" */
    0x79, 0x98, 0x33, 0xee, 0x50, 0xa3, 0x3a,

    /* U+62 "b" */
    0x82, 0x8, 0x20, 0xfb, 0x28, 0x61, 0x86, 0x1c,
    0xbe,

    /* U+63 "c" */
    0x39, 0x38, 0x60, 0x82, 0x14, 0xce,

    /* U+64 "d" */
    0x4, 0x10, 0x41, 0x7d, 0x38, 0x61, 0x86, 0x1c,
    0xdf,

    /* U+65 "e" */
    0x38, 0x9a, 0x17, 0xe8, 0x10, 0xd1, 0x1c,

    /* U+66 "f" */
    0x1e, 0x40, 0x81, 0xf, 0xc4, 0x8, 0x10, 0x20,
    0x40, 0x81, 0x0,

    /* U+67 "g" */
    0x36, 0x91, 0x22, 0x47, 0x8, 0x10, 0x3e, 0x84,
    0xf0,

    /* U+68 "h" */
    0x82, 0x8, 0x20, 0xbb, 0x18, 0x61, 0x86, 0x18,
    0x61,

    /* U+69 "i" */
    0xf0, 0xaa, 0xaa,

    /* U+6A "j" */
    0x18, 0xc0, 0x1, 0x8, 0x42, 0x10, 0x84, 0x21,
    0x70,

    /* U+6B "k" */
    0x82, 0x8, 0x20, 0x8a, 0x4a, 0x3c, 0xd2, 0x68,
    0xa3,

    /* U+6C "l" */
    0xff, 0xf0,

    /* U+6D "m" */
    0xff, 0x26, 0x4c, 0x99, 0x32, 0x64, 0xc9,

    /* U+6E "n" */
    0xbb, 0x18, 0x61, 0x86, 0x18, 0x61,

    /* U+6F "o" */
    0x38, 0x8a, 0xc, 0x18, 0x30, 0x51, 0x1c,

    /* U+70 "p" */
    0xfb, 0x28, 0x61, 0x86, 0x1c, 0xbe, 0x82, 0x0,

    /* U+71 "q" */
    0x7f, 0x38, 0x61, 0x86, 0x14, 0xdf, 0x4, 0x10,

    /* U+72 "r" */
    0xbe, 0xc8, 0x88, 0x88,

    /* U+73 "s" */
    0x7a, 0x18, 0x3c, 0x3c, 0x18, 0x5e,

    /* U+74 "t" */
    0x20, 0x40, 0x87, 0xe2, 0x4, 0x8, 0x10, 0x20,
    0x44, 0x78,

    /* U+75 "u" */
    0x86, 0x18, 0x61, 0x86, 0x18, 0xdd,

    /* U+76 "v" */
    0x86, 0x3c, 0x92, 0x59, 0xc3, 0xc,

    /* U+77 "w" */
    0x9b, 0xda, 0x5a, 0x5a, 0x6a, 0x6a, 0x64, 0x24,

    /* U+78 "x" */
    0xcd, 0x27, 0x8c, 0x31, 0xec, 0xa3,

    /* U+79 "y" */
    0xc6, 0x89, 0x13, 0x62, 0x85, 0xe, 0x8, 0x10,
    0xc0,

    /* U+7A "z" */
    0xfc, 0x31, 0x84, 0x21, 0x8c, 0x3f,

    /* U+B0 "°" */
    0x4a, 0x9e,

    /* U+201C "“" */
    0x2, 0xc9, 0x23, 0x66, 0xc0,

    /* U+3001 "、" */
    0xc2, 0x30,

    /* U+3002 "。" */
    0xf, 0x9e,

    /* U+4E00 "一" */
    0x0, 0xff, 0xfc, 0x0,

    /* U+4E03 "七" */
    0x4, 0x0, 0x20, 0x1, 0x0, 0x8, 0xf0, 0x58,
    0x7, 0x1, 0xd0, 0x0, 0x80, 0x4, 0x8, 0x30,
    0xc0, 0xfc,

    /* U+4E07 "万" */
    0x1, 0xf9, 0xf8, 0x0, 0x20, 0x0, 0x90, 0x5,
    0xe0, 0x11, 0x80, 0x84, 0x2, 0x10, 0x10, 0x40,
    0x8a, 0x4, 0x18, 0x0, 0x0,

    /* U+4E09 "三" */
    0x7, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x1e, 0x0, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xfe, 0xfe, 0x0,

    /* U+4E3A "为" */
    0x0, 0x0, 0x8, 0x6, 0x40, 0x12, 0x0, 0x23,
    0x7, 0xe8, 0xc8, 0x40, 0x82, 0x5, 0x90, 0x44,
    0x84, 0x8, 0x40, 0x40, 0xe, 0x0, 0x20,

    /* U+4E5D "九" */
    0x4, 0x0, 0x18, 0x0, 0x40, 0x1, 0x60, 0xe,
    0x80, 0xd2, 0x0, 0x90, 0x2, 0x40, 0x11, 0x4,
    0x44, 0x12, 0x8, 0x50, 0x1f,

    /* U+4E8C "二" */
    0x1, 0xe0, 0x38, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x1, 0x9f, 0xff, 0x0, 0x0,

    /* U+4E94 "五" */
    0x1, 0xe0, 0x1c, 0x0, 0x8, 0x0, 0x10, 0x0,
    0x2c, 0x3, 0xac, 0x1, 0x10, 0x2, 0x20, 0x8,
    0x80, 0x1f, 0xf9, 0xc0, 0x0,

    /* U+4ED6 "他" */
    0x10, 0x80, 0x21, 0x0, 0x42, 0x1, 0x15, 0x82,
    0x2d, 0xa, 0x72, 0x27, 0xb8, 0x9, 0x50, 0x12,
    0x84, 0x24, 0x8, 0x47, 0xf0, 0x80, 0x0, 0x0,
    0x0,

    /* U+4F4E "低" */
    0xc, 0x30, 0xc, 0x20, 0x8, 0xc0, 0x11, 0x40,
    0x11, 0x2c, 0x29, 0x30, 0x49, 0x60, 0x9, 0x10,
    0x9, 0xd0, 0x9, 0x88, 0xb, 0x66, 0x18, 0x2,
    0x0, 0x1,

    /* U+4F60 "你" */
    0x0, 0x40, 0x8, 0xc0, 0x11, 0x0, 0x44, 0x0,
    0x8b, 0xc3, 0x29, 0xa, 0x8, 0x24, 0x10, 0x9,
    0x20, 0x14, 0x58, 0x28, 0x90, 0x41, 0x0, 0x86,
    0x0, 0x0, 0x0,

    /* U+516B "八" */
    0x2, 0x0, 0x2, 0x0, 0x4, 0x0, 0x44, 0x0,
    0x8c, 0x2, 0x8, 0x8, 0x8, 0x20, 0x1c, 0x80,
    0x4,

    /* U+516D "六" */
    0x2, 0x0, 0x4, 0x0, 0x0, 0x0, 0x3f, 0x7f,
    0x0, 0x0, 0x0, 0x8, 0x3, 0x18, 0x18, 0x30,
    0x40, 0x46, 0x0, 0x0,

    /* U+5173 "关" */
    0x0, 0x0, 0x1, 0x80, 0xc4, 0x1, 0x20, 0x1,
    0x40, 0x7e, 0x0, 0x20, 0x0, 0xbe, 0x7f, 0x0,
    0x14, 0x0, 0x48, 0x2, 0x10, 0x10, 0x61, 0x80,
    0xf0,

    /* U+5206 "分" */
    0x2, 0x0, 0x2, 0x0, 0x32, 0x0, 0x42, 0x1,
    0x6, 0x6, 0x6, 0x10, 0x27, 0x5f, 0xe0, 0x4,
    0x40, 0x11, 0x0, 0x22, 0x0, 0x84, 0x2, 0x38,
    0x0, 0x60, 0x0,

    /* U+5230 "到" */
    0x0, 0x10, 0xf0, 0x8a, 0x4, 0x24, 0xa2, 0x25,
    0x3e, 0x28, 0x21, 0x41, 0xca, 0x38, 0x10, 0x78,
    0xbc, 0x4, 0x80, 0x60, 0x3, 0x0, 0x0,

    /* U+5341 "十" */
    0x0, 0x0, 0x4, 0x0, 0x10, 0x0, 0x40, 0x1,
    0x0, 0xf, 0xf7, 0xd0, 0x0, 0x40, 0x1, 0x0,
    0x4, 0x0, 0x10, 0x0, 0x40, 0x1, 0x0, 0x4,
    0x0,

    /* U+5343 "千" */
    0x1, 0x80, 0x10, 0x3, 0x80, 0x4, 0x0, 0x21,
    0xf, 0xff, 0x88, 0x0, 0x40, 0x2, 0x0, 0x10,
    0x0, 0x80, 0x4, 0x0, 0x20, 0x1, 0x0,

    /* U+5429 "吩" */
    0x0, 0x40, 0x0, 0x40, 0x0, 0x20, 0x18, 0x10,
    0x69, 0x98, 0x49, 0xc, 0x52, 0x32, 0x63, 0xd0,
    0x4, 0x90, 0x1, 0x20, 0x2, 0x20, 0x4, 0x20,
    0x0, 0x60, 0x0, 0x0,

    /* U+5490 "咐" */
    0x0, 0x0, 0x2, 0x20, 0x4, 0x40, 0x10, 0x8f,
    0x21, 0x12, 0x87, 0xe5, 0xf4, 0x35, 0x8, 0x12,
    0x90, 0x44, 0x20, 0x8, 0x40, 0x10, 0x80, 0x27,
    0x0, 0x4, 0x0,

    /* U+5668 "器" */
    0x0, 0x20, 0x77, 0xc2, 0x52, 0xb, 0x30, 0x12,
    0x30, 0xf, 0x81, 0xe0, 0x1, 0x60, 0x8, 0x70,
    0xc0, 0x21, 0xff, 0x9, 0x48, 0x2d, 0x60, 0x42,
    0x0,

    /* U+56DB "四" */
    0x0, 0x0, 0x3f, 0xfe, 0x46, 0x92, 0x24, 0x91,
    0x28, 0x69, 0x80, 0x48, 0x16, 0x3f, 0x60, 0x1,
    0x0, 0x0,

    /* U+5728 "在" */
    0x1, 0x0, 0x2, 0x0, 0x4, 0x0, 0x1f, 0x80,
    0xe0, 0x0, 0x88, 0x1, 0x10, 0x4, 0x2c, 0x1b,
    0xe0, 0x50, 0x81, 0x21, 0x4, 0x47, 0xf0, 0xb0,
    0x1, 0x0, 0x0, 0x0, 0x0,

    /* U+597D "好" */
    0x8, 0x0, 0x8, 0x7c, 0x8, 0x98, 0xa, 0x20,
    0xa, 0x20, 0x76, 0x3e, 0x13, 0xd0, 0xc, 0x10,
    0x4, 0x10, 0xa, 0x10, 0x10, 0x10, 0x20, 0x60,
    0x0, 0x20,

    /* U+5DF2 "已" */
    0x4, 0x7, 0xe0, 0x4, 0x8, 0x40, 0x88, 0xf,
    0x0, 0x80, 0x8, 0x0, 0x80, 0x28, 0x2, 0x40,
    0xe3, 0xf0,

    /* U+5E2E "帮" */
    0x0, 0x0, 0x10, 0x20, 0x67, 0x8f, 0x14, 0x1e,
    0x50, 0x1d, 0x27, 0xc5, 0x42, 0x12, 0x11, 0x61,
    0xbe, 0x40, 0x91, 0x2, 0x5c, 0x1, 0x20, 0x4,
    0x0, 0x10, 0x0,

    /* U+5E74 "年" */
    0x2, 0x0, 0x4, 0x0, 0x17, 0x80, 0x54, 0x0,
    0xa, 0x1, 0xf8, 0x1, 0x20, 0x2, 0x7e, 0x3f,
    0x80, 0x81, 0x0, 0x2, 0x0, 0x4, 0x0, 0x8,
    0x0, 0x10, 0x0, 0x0, 0x0,

    /* U+5EA6 "度" */
    0x1, 0x0, 0x2, 0x0, 0xf, 0x0, 0xe2, 0x1,
    0x67, 0x2, 0xf0, 0x4, 0xa0, 0x8, 0x80, 0x17,
    0xc0, 0x49, 0x0, 0x8a, 0x2, 0xc, 0x8, 0x66,
    0x1, 0x7, 0x80,

    /* U+5F00 "开" */
    0x0, 0xe0, 0x7e, 0x0, 0x48, 0x1, 0x20, 0x4,
    0xfd, 0xfe, 0x0, 0x48, 0x2, 0x20, 0x8, 0x80,
    0x42, 0x1, 0x8, 0x0, 0x20, 0x0, 0x80,

    /* U+60A8 "您" */
    0x0, 0x0, 0x26, 0x1, 0x10, 0x8, 0xfc, 0x34,
    0xa1, 0x42, 0x81, 0x49, 0x5, 0x20, 0x11, 0x80,
    0xc, 0x74, 0x10, 0x52, 0x0, 0x44, 0x30, 0xf,
    0xc0,

    /* U+6211 "我" */
    0x3, 0x0, 0x9, 0x0, 0x22, 0x3, 0x40, 0x11,
    0x0, 0x27, 0x0, 0xf2, 0x1e, 0x28, 0x8, 0xc0,
    0xe1, 0xc, 0x9c, 0x2, 0xa, 0x18, 0x18, 0x0,
    0x30,

    /* U+6253 "打" */
    0x8, 0x0, 0x20, 0x0, 0x80, 0x2, 0x0, 0xe,
    0xc, 0xe7, 0xc0, 0xc2, 0x2, 0x8, 0x38, 0x23,
    0x20, 0x80, 0x82, 0xa, 0x8, 0x18, 0xe0, 0x1,
    0x0,

    /* U+6291 "抑" */
    0x0, 0x0, 0x10, 0x0, 0x21, 0x0, 0x46, 0x0,
    0xf3, 0xe7, 0x24, 0xc2, 0x49, 0x4, 0x92, 0x39,
    0xad, 0x92, 0x48, 0x20, 0x80, 0xc1, 0x1, 0x82,
    0x0, 0x4, 0x0, 0x8, 0x0,

    /* U+65E5 "日" */
    0xf, 0x38, 0x90, 0x48, 0x25, 0xd3, 0x9, 0x4,
    0x82, 0x4d, 0x39, 0x80, 0x40, 0x0,

    /* U+65F6 "时" */
    0x0, 0x0, 0x0, 0x40, 0x0, 0x83, 0xe1, 0x4,
    0x82, 0x9, 0x7f, 0x92, 0x8, 0x35, 0x10, 0x49,
    0x20, 0x90, 0x41, 0xe0, 0x82, 0x41, 0x0, 0x6,
    0x0, 0xc, 0x0, 0x0, 0x0,

    /* U+661F "星" */
    0x0, 0xe0, 0x3c, 0xc0, 0xb2, 0x3, 0x18, 0x7,
    0x80, 0x4, 0x0, 0x9e, 0x7, 0xc0, 0x21, 0x0,
    0x3f, 0x0, 0x10, 0x0, 0xff, 0x7c, 0x0,

    /* U+665A "晚" */
    0x1, 0x80, 0x5, 0x7, 0x2c, 0x14, 0xa0, 0x50,
    0xf1, 0xde, 0x85, 0x4a, 0x14, 0xf0, 0x51, 0x81,
    0xca, 0x0, 0x48, 0x42, 0x3f,

    /* U+6668 "晨" */
    0x0, 0x60, 0xf, 0xb0, 0x7, 0xa0, 0x4, 0xe0,
    0x3, 0x0, 0xb, 0xe0, 0xc, 0x40, 0x9, 0x80,
    0x13, 0xf0, 0x1d, 0x30, 0x14, 0xc0, 0x24, 0x60,
    0x47, 0x1c, 0x8c, 0x2, 0x0, 0x0,

    /* U+6708 "月" */
    0x1, 0xc1, 0xc8, 0x21, 0x4, 0x20, 0xe4, 0x10,
    0x82, 0x10, 0xb2, 0x10, 0x42, 0x8, 0x81, 0x20,
    0xe8, 0xc, 0x0, 0x0,

    /* U+671F "期" */
    0x1, 0x0, 0x12, 0x0, 0x24, 0x31, 0xfd, 0xa0,
    0x92, 0x41, 0xa4, 0x82, 0x4f, 0x6, 0x92, 0x9,
    0x24, 0xfd, 0x68, 0x25, 0x10, 0x8a, 0x22, 0x9,
    0x40, 0x11, 0x80, 0x40, 0x0,

    /* U+6C34 "水" */
    0x2, 0x0, 0x4, 0x0, 0x8, 0x80, 0x13, 0x0,
    0xa8, 0x7, 0xe0, 0x2, 0xa0, 0x5, 0x20, 0x12,
    0x60, 0x44, 0x79, 0x8, 0x0, 0x70, 0x0, 0x60,
    0x0, 0x0, 0x0,

    /* U+6C90 "沐" */
    0x1, 0x0, 0x2, 0x0, 0x84, 0x0, 0x88, 0x0,
    0x1f, 0x11, 0xe0, 0x10, 0xe0, 0x1, 0xa0, 0x5,
    0x20, 0x52, 0x31, 0x44, 0x12, 0x8, 0x4, 0x10,
    0x0, 0x20, 0x0,

    /* U+6D74 "浴" */
    0x0, 0x0, 0x44, 0xe0, 0x10, 0x0, 0x48, 0x8,
    0x28, 0x8, 0x48, 0x1, 0x8, 0x14, 0xc, 0x51,
    0xe4, 0x8c, 0xc3, 0x11, 0x2, 0x3f, 0x0, 0x40,
    0x0,

    /* U+6E29 "温" */
    0x0, 0x20, 0xdf, 0x41, 0x23, 0x0, 0xe8, 0x42,
    0xe0, 0xc, 0x0, 0x1, 0xd, 0xfe, 0x25, 0x50,
    0x95, 0x44, 0x59, 0x93, 0x99,

    /* U+70ED "热" */
    0x1, 0x0, 0x22, 0x0, 0x88, 0x3, 0xac, 0x1b,
    0xe0, 0x22, 0x81, 0x94, 0x1a, 0x68, 0xa, 0x28,
    0x70, 0x60, 0x0, 0x9, 0x24, 0x64, 0x89, 0x0,
    0x0,

    /* U+767E "百" */
    0x1, 0xf9, 0xf8, 0x0, 0x20, 0x1, 0x0, 0x1f,
    0xc0, 0x41, 0x1, 0x34, 0x7, 0x10, 0x10, 0x40,
    0x3f, 0x0, 0x4, 0x0, 0x0,

    /* U+7684 "的" */
    0x0, 0x0, 0x86, 0x4, 0x20, 0x41, 0x2, 0x13,
    0x7f, 0x69, 0x30, 0x49, 0x22, 0x68, 0x92, 0x40,
    0x9e, 0x4, 0x91, 0x40, 0x6, 0x0, 0x20,

    /* U+79D2 "秒" */
    0x4, 0x40, 0x31, 0x1, 0x84, 0x2, 0x13, 0x1d,
    0x41, 0xa5, 0x41, 0xc5, 0xa, 0x8, 0x48, 0x22,
    0x21, 0x0, 0x88, 0x2, 0x40, 0x2, 0x0, 0x0,
    0x0,

    /* U+7ECF "经" */
    0x20, 0x0, 0x83, 0x82, 0x94, 0x12, 0x20, 0xf0,
    0xe0, 0x84, 0xe4, 0x60, 0x9c, 0x78, 0x0, 0x80,
    0x42, 0x6, 0xb, 0x83, 0xd0,

    /* U+81F3 "至" */
    0x1, 0xe0, 0xf0, 0x1, 0x80, 0x9, 0x0, 0x86,
    0x1f, 0xd0, 0x88, 0x0, 0x50, 0x1f, 0x40, 0x10,
    0x1, 0xff, 0xf0, 0x0,

    /* U+83CC "菌" */
    0x0, 0xc0, 0x22, 0x3, 0xff, 0xd0, 0x40, 0x0,
    0x0, 0x7f, 0xc2, 0xe1, 0x8, 0xb4, 0x2f, 0x10,
    0xaa, 0x43, 0x21, 0x8, 0x84, 0x3f, 0xd0, 0x80,
    0xc0, 0x0, 0x0,

    /* U+8865 "补" */
    0x8, 0x40, 0x11, 0x0, 0x4, 0x1, 0x10, 0x1c,
    0x41, 0xa1, 0xc1, 0xa5, 0x87, 0x11, 0x2a, 0x41,
    0x21, 0x0, 0x84, 0x2, 0x10, 0x8, 0x40, 0x0,
    0x0,

    /* U+8BF4 "说" */
    0x0, 0x10, 0x64, 0x40, 0x44, 0x0, 0x0, 0x0,
    0x3f, 0x7, 0x24, 0x34, 0x48, 0x8, 0xe0, 0x10,
    0xc0, 0x32, 0x80, 0x65, 0x11, 0x92, 0x10, 0x3,
    0xc0,

    /* U+8BF7 "请" */
    0x0, 0x40, 0x0, 0x80, 0x61, 0xc0, 0xe, 0x80,
    0x1e, 0x7, 0xf, 0xf4, 0xe0, 0x8, 0xf8, 0x11,
    0x50, 0x23, 0x20, 0x67, 0x41, 0x88, 0x80, 0x35,
    0x0, 0x6, 0x0, 0x0, 0x0,

    /* U+8C03 "调" */
    0x30, 0x38, 0xf, 0x20, 0x24, 0x80, 0xba, 0xf2,
    0x78, 0x8a, 0x22, 0x27, 0x88, 0xea, 0x34, 0xe8,
    0xd0, 0x22, 0x80, 0x82, 0xe, 0x0, 0x0,

    /* U+901F "速" */
    0x0, 0x0, 0x1, 0x0, 0x82, 0x0, 0xbf, 0x0,
    0xb, 0x0, 0xfa, 0x1d, 0x2c, 0x11, 0xe0, 0x21,
    0xe0, 0x45, 0x20, 0x92, 0x7, 0xe4, 0x0, 0x3f,
    0xc0, 0x6, 0x0,

    /* U+95ED "闭" */
    0x20, 0x70, 0x9c, 0x90, 0x4, 0x84, 0x24, 0x29,
    0x2f, 0x89, 0x18, 0x49, 0x42, 0x52, 0x13, 0x10,
    0xb1, 0x85, 0x80, 0xe0, 0x3, 0x0, 0x0,

    /* U+95EE "问" */
    0x20, 0x20, 0x9e, 0x90, 0x4, 0x80, 0x24, 0x19,
    0x27, 0x49, 0x24, 0x49, 0x72, 0x44, 0x12, 0x0,
    0x90, 0x4, 0x80, 0x60, 0x1, 0x0, 0x0,

    /* U+96F6 "零" */
    0x0, 0x40, 0x3, 0x80, 0x1, 0xfc, 0x1f, 0x8,
    0x25, 0x60, 0x21, 0x0, 0xd, 0x40, 0x3, 0x0,
    0x4, 0xc0, 0x9, 0x30, 0x10, 0xde, 0x27, 0xc0,
    0x1, 0x0, 0x3, 0x0, 0x1, 0x80,

    /* U+9AD8 "高" */
    0x4, 0x0, 0x1, 0x1f, 0xf0, 0x3, 0x0, 0xe8,
    0x4, 0x80, 0x18, 0x8f, 0xff, 0x41, 0x12, 0x78,
    0x92, 0x44, 0x9c, 0x24, 0x7, 0x0, 0x18,

    /* U+FF01 "！" */
    0x55, 0x54, 0x3c,

    /* U+FF0C "，" */
    0xd9, 0x40,

    /* U+FF1A "：" */
    0xf0, 0x3c,

    /* U+FF1B "；" */
    0xf0, 0x3d, 0x0,

    /* U+FF1F "？" */
    0x33, 0x68, 0x82, 0x18, 0x42, 0x8, 0x60, 0x6,
    0x18
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 128, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 128, .box_w = 3, .box_h = 13, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 6, .adv_w = 128, .box_w = 4, .box_h = 3, .ofs_x = 2, .ofs_y = 9},
    {.bitmap_index = 8, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 19, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 26, .adv_w = 128, .box_w = 2, .box_h = 4, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 27, .adv_w = 128, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 28, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 39, .adv_w = 128, .box_w = 3, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 44, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 53, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 64, .adv_w = 128, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 76, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 87, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 98, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 109, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 120, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 131, .adv_w = 128, .box_w = 2, .box_h = 7, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 133, .adv_w = 128, .box_w = 2, .box_h = 9, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 136, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 145, .adv_w = 128, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 157, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 166, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 177, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 186, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 195, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 204, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 213, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 222, .adv_w = 128, .box_w = 1, .box_h = 12, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 224, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 233, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 244, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 253, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 264, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 273, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 284, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 293, .adv_w = 128, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 305, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 314, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 323, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 332, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 341, .adv_w = 128, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 353, .adv_w = 128, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 365, .adv_w = 128, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 377, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 388, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 397, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 404, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 413, .adv_w = 128, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 419, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 428, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 435, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 446, .adv_w = 128, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 455, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 464, .adv_w = 128, .box_w = 2, .box_h = 12, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 467, .adv_w = 128, .box_w = 5, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 476, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 485, .adv_w = 128, .box_w = 1, .box_h = 12, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 487, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 494, .adv_w = 128, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 500, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 507, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 515, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 523, .adv_w = 128, .box_w = 4, .box_h = 8, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 527, .adv_w = 128, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 533, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 543, .adv_w = 128, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 549, .adv_w = 128, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 555, .adv_w = 128, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 563, .adv_w = 128, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 569, .adv_w = 128, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 578, .adv_w = 128, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 584, .adv_w = 256, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = 9},
    {.bitmap_index = 586, .adv_w = 256, .box_w = 7, .box_h = 5, .ofs_x = 7, .ofs_y = 8},
    {.bitmap_index = 591, .adv_w = 256, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 593, .adv_w = 256, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 595, .adv_w = 256, .box_w = 14, .box_h = 2, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 599, .adv_w = 256, .box_w = 13, .box_h = 11, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 617, .adv_w = 256, .box_w = 14, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 638, .adv_w = 256, .box_w = 15, .box_h = 10, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 657, .adv_w = 256, .box_w = 13, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 680, .adv_w = 256, .box_w = 14, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 701, .adv_w = 256, .box_w = 14, .box_h = 9, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 717, .adv_w = 256, .box_w = 15, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 738, .adv_w = 256, .box_w = 15, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 763, .adv_w = 256, .box_w = 16, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 789, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 816, .adv_w = 256, .box_w = 15, .box_h = 9, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 833, .adv_w = 256, .box_w = 14, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 853, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 878, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 905, .adv_w = 256, .box_w = 13, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 928, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 953, .adv_w = 256, .box_w = 13, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 976, .adv_w = 256, .box_w = 16, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1004, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1031, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1056, .adv_w = 256, .box_w = 13, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1074, .adv_w = 256, .box_w = 15, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1103, .adv_w = 256, .box_w = 16, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1129, .adv_w = 256, .box_w = 12, .box_h = 12, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 1147, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1174, .adv_w = 256, .box_w = 15, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1203, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1230, .adv_w = 256, .box_w = 14, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1253, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1278, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1303, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1328, .adv_w = 256, .box_w = 15, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1357, .adv_w = 256, .box_w = 9, .box_h = 12, .ofs_x = 4, .ofs_y = -1},
    {.bitmap_index = 1371, .adv_w = 256, .box_w = 15, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1400, .adv_w = 256, .box_w = 14, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1423, .adv_w = 256, .box_w = 14, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1444, .adv_w = 256, .box_w = 16, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1474, .adv_w = 256, .box_w = 11, .box_h = 14, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 1494, .adv_w = 256, .box_w = 15, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1523, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1550, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1577, .adv_w = 256, .box_w = 15, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1602, .adv_w = 256, .box_w = 14, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1623, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1648, .adv_w = 256, .box_w = 14, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1669, .adv_w = 256, .box_w = 13, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1692, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1717, .adv_w = 256, .box_w = 14, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1738, .adv_w = 256, .box_w = 13, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1758, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1785, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1810, .adv_w = 256, .box_w = 15, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1835, .adv_w = 256, .box_w = 15, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1864, .adv_w = 256, .box_w = 14, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1887, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1914, .adv_w = 256, .box_w = 13, .box_h = 14, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 1937, .adv_w = 256, .box_w = 13, .box_h = 14, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 1960, .adv_w = 256, .box_w = 16, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1990, .adv_w = 256, .box_w = 13, .box_h = 14, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 2013, .adv_w = 256, .box_w = 2, .box_h = 11, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 2016, .adv_w = 256, .box_w = 3, .box_h = 4, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 2018, .adv_w = 256, .box_w = 2, .box_h = 7, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 2020, .adv_w = 256, .box_w = 2, .box_h = 9, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 2023, .adv_w = 256, .box_w = 6, .box_h = 12, .ofs_x = 2, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint8_t glyph_id_ofs_list_0[] = {
    0, 1, 2, 3, 0, 0, 0, 0,
    0, 0, 4, 0, 5, 0, 6, 0,
    7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 0, 0, 0, 19
};

static const uint16_t unicode_list_3[] = {
    0x0, 0x1f6c, 0x2f51, 0x2f52, 0x4d50, 0x4d53, 0x4d57, 0x4d59,
    0x4d8a, 0x4dad, 0x4ddc, 0x4de4, 0x4e26, 0x4e9e, 0x4eb0, 0x50bb,
    0x50bd, 0x50c3, 0x5156, 0x5180, 0x5291, 0x5293, 0x5379, 0x53e0,
    0x55b8, 0x562b, 0x5678, 0x58cd, 0x5d42, 0x5d7e, 0x5dc4, 0x5df6,
    0x5e50, 0x5ff8, 0x6161, 0x61a3, 0x61e1, 0x6535, 0x6546, 0x656f,
    0x65aa, 0x65b8, 0x6658, 0x666f, 0x6b84, 0x6be0, 0x6cc4, 0x6d79,
    0x703d, 0x75ce, 0x75d4, 0x7922, 0x7e1f, 0x8143, 0x831c, 0x87b5,
    0x8b44, 0x8b47, 0x8b53, 0x8f6f, 0x953d, 0x953e, 0x9646, 0x9a28,
    0xfe51, 0xfe5c, 0xfe6a, 0xfe6b, 0xfe6f
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 32, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = glyph_id_ofs_list_0, .list_length = 32, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL
    },
    {
        .range_start = 65, .range_length = 26, .glyph_id_start = 21,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 97, .range_length = 26, .glyph_id_start = 47,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 176, .range_length = 65136, .glyph_id_start = 73,
        .unicode_list = unicode_list_3, .glyph_id_ofs_list = NULL, .list_length = 69, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

/*Store all the custom data of the font*/
static lv_font_fmt_txt_dsc_t font_dsc = {
    .glyph_bitmap = gylph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 4,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
lv_font_t lv_font_16 = {
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 15,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};

#endif /*#if LV_FONT*/

#endif
