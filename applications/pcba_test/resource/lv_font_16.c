#include "lvgl/lvgl.h"

/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts:
 ******************************************************************************/

#ifndef LV_FONT_16
#define LV_FONT_16 1
#endif

#if LV_FONT_16

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t gylph_bitmap[] =
{
    /* U+20 " " */
    0x0,

    /* U+21 "!" */
    0xff, 0xb0,

    /* U+22 "\"" */
    0xbb, 0xb0,

    /* U+23 "#" */
    0x24, 0x48, 0x91, 0x2f, 0xe4, 0x8a, 0x24, 0xfe,
    0x91, 0x22, 0x40,

    /* U+2A "*" */
    0x11, 0x27, 0x59, 0xc7, 0xda, 0xc4, 0x8,

    /* U+2B "+" */
    0x10, 0x20, 0x47, 0xf1, 0x2, 0x4, 0x0,

    /* U+2C "," */
    0xe0,

    /* U+2D "-" */
    0xfe,

    /* U+2E "." */
    0x80,

    /* U+2F "/" */
    0x0, 0x2, 0x2, 0x4, 0x4, 0x8, 0x8, 0x10,
    0x20, 0x20, 0x40, 0x40, 0x80,

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
    0x81,

    /* U+3B ";" */
    0x81, 0xc0,

    /* U+3F "?" */
    0x7b, 0x38, 0x61, 0xc, 0x63, 0x8, 0x20, 0x0,
    0x8,

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
    0xcf, 0xf0,

    /* U+6A "j" */
    0x11, 0x0, 0x11, 0x11, 0x11, 0x11, 0x1e,

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
    0x46, 0x28, 0xb3, 0xcc,

    /* U+3001 "、" */
    0xc4, 0x63, 0x20,

    /* U+3002 "。" */
    0xf, 0x9e,

    /* U+4E2D "中" */
    0x4, 0x0, 0x80, 0x10, 0x7f, 0xf8, 0x43, 0x8,
    0x61, 0xc, 0x21, 0xff, 0xf0, 0x84, 0x10, 0x2,
    0x0, 0x40, 0x8, 0x1, 0x0,

    /* U+4E8E "于" */
    0x3f, 0xf8, 0x2, 0x0, 0x4, 0x0, 0x8, 0x0,
    0x10, 0x1f, 0xff, 0xc0, 0x40, 0x0, 0x80, 0x1,
    0x0, 0x2, 0x0, 0x4, 0x0, 0x8, 0x0, 0x70,
    0x0,

    /* U+4EF6 "件" */
    0x10, 0x40, 0xc1, 0x2, 0x24, 0x9, 0x90, 0x67,
    0xfb, 0xb1, 0xa, 0x84, 0x8, 0x10, 0x2f, 0xfc,
    0x81, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x81,
    0x0,

    /* U+5149 "光" */
    0x3, 0x0, 0xc, 0x3, 0x31, 0x4, 0xcc, 0x1b,
    0x60, 0xc, 0xf, 0xff, 0xc1, 0x20, 0x4, 0x80,
    0x12, 0x0, 0x88, 0x2, 0x21, 0x30, 0x8f, 0x83,
    0xe0, 0x0, 0x0,

    /* U+5173 "关" */
    0x18, 0xc0, 0x22, 0x0, 0x88, 0x1f, 0xfc, 0x2,
    0x0, 0x8, 0x0, 0x20, 0x0, 0x80, 0xff, 0xf8,
    0xc, 0x0, 0x50, 0x3, 0x30, 0x18, 0x73, 0x80,
    0xe0, 0x0, 0x80,

    /* U+51FA "出" */
    0x2, 0x0, 0x10, 0x10, 0x84, 0x84, 0x24, 0x21,
    0x21, 0x9, 0xff, 0xc0, 0x40, 0x82, 0x1c, 0x10,
    0xe0, 0x87, 0x4, 0x3f, 0xff, 0x80, 0xc, 0x0,
    0x60,

    /* U+5217 "列" */
    0xff, 0x84, 0x40, 0x11, 0x2, 0x4c, 0x9, 0x3f,
    0x24, 0x8c, 0x96, 0x22, 0x76, 0x89, 0xe, 0x24,
    0x10, 0x90, 0xc0, 0x4e, 0x1, 0xe0, 0x1d, 0x0,
    0x20,

    /* U+5220 "删" */
    0x0, 0x5, 0xce, 0x15, 0x2a, 0x54, 0xa9, 0x52,
    0xa5, 0x4a, 0x9f, 0xfe, 0x54, 0xa9, 0x52, 0xa5,
    0x4a, 0x95, 0x2a, 0x54, 0xa1, 0x96, 0x86, 0xf6,
    0x7a, 0x11, 0x80,

    /* U+5316 "化" */
    0x8, 0x80, 0x31, 0x0, 0x42, 0x31, 0x84, 0xc3,
    0xb, 0xa, 0x1c, 0x14, 0x30, 0x8, 0xc0, 0x16,
    0x80, 0x29, 0x0, 0x42, 0x10, 0x84, 0x21, 0xf,
    0xc0,

    /* U+5347 "升" */
    0x3, 0x20, 0x1e, 0x40, 0xf0, 0x80, 0x21, 0x0,
    0x42, 0x0, 0x84, 0x3f, 0xff, 0x82, 0x10, 0x4,
    0x20, 0x10, 0x40, 0x20, 0x80, 0x81, 0x6, 0x2,
    0x8, 0x4, 0x0,

    /* U+5355 "单" */
    0x0, 0x40, 0x23, 0x0, 0x88, 0xf, 0xfc, 0x22,
    0x10, 0xff, 0xc2, 0x21, 0x8, 0x84, 0x3f, 0xf0,
    0x8, 0x0, 0x20, 0x3f, 0xff, 0x2, 0x0, 0x8,
    0x0, 0x20, 0x0,

    /* U+5382 "厂" */
    0x3f, 0xfc, 0x80, 0x2, 0x0, 0x8, 0x0, 0x20,
    0x0, 0x80, 0x2, 0x0, 0x8, 0x0, 0x20, 0x1,
    0x80, 0x4, 0x0, 0x10, 0x0, 0xc0, 0x0,

    /* U+53D6 "取" */
    0xff, 0xf9, 0x12, 0x24, 0x48, 0x9f, 0x26, 0x44,
    0x91, 0x12, 0x47, 0xcd, 0x11, 0x1c, 0x45, 0x61,
    0x3d, 0x8f, 0x4f, 0x21, 0x26, 0x5, 0xc, 0x10,
    0x0,

    /* U+53F7 "号" */
    0x1f, 0xe0, 0x40, 0x81, 0x2, 0x7, 0xf8, 0x0,
    0x3, 0xff, 0xf0, 0x80, 0x2, 0x0, 0x1f, 0xf0,
    0x0, 0xc0, 0x3, 0x0, 0x8, 0x0, 0x20, 0x7,
    0x80,

    /* U+5426 "否" */
    0x7f, 0xfc, 0x6, 0x0, 0x1c, 0x0, 0x7b, 0x83,
    0xb1, 0xdc, 0x60, 0xc0, 0xc0, 0x1f, 0xfc, 0x30,
    0x18, 0x60, 0x30, 0xc0, 0x61, 0xff, 0xc3, 0x1,
    0x80,

    /* U+5668 "器" */
    0x3e, 0xf8, 0x45, 0x10, 0xfb, 0xe0, 0x19, 0x7,
    0xff, 0xc0, 0xd0, 0x7, 0x1f, 0xb0, 0x6, 0x3e,
    0xf8, 0x45, 0x10, 0x8a, 0x21, 0xf7, 0xc2, 0x28,
    0x80,

    /* U+590D "复" */
    0x0, 0x0, 0x20, 0x1, 0xff, 0x8c, 0x0, 0x7f,
    0xf0, 0x40, 0x41, 0xff, 0x4, 0x4, 0x1f, 0xf0,
    0x30, 0x1, 0xff, 0xf, 0x18, 0x66, 0xc0, 0xe,
    0x7, 0xef, 0xd0, 0x1,

    /* U+5F0F "式" */
    0x1, 0x0, 0x4, 0x80, 0x12, 0x3f, 0xfe, 0x1,
    0x0, 0x4, 0x0, 0x10, 0x3f, 0x20, 0x10, 0x80,
    0x42, 0x1, 0xc, 0x4, 0x92, 0x7c, 0x6f, 0x0,
    0xe0, 0x1, 0x80,

    /* U+5F55 "录" */
    0x3f, 0xf0, 0x0, 0x41, 0xff, 0x0, 0x4, 0x0,
    0x13, 0xff, 0xf0, 0x10, 0xc, 0x46, 0x19, 0xa0,
    0x1f, 0x1, 0x96, 0x38, 0x47, 0x47, 0xc, 0x8,
    0x0,

    /* U+5FAA "循" */
    0x10, 0xc, 0x6f, 0xf9, 0x90, 0x86, 0xaf, 0xf1,
    0x42, 0x6, 0x84, 0x19, 0x7f, 0x32, 0x82, 0xa5,
    0xfc, 0x4a, 0x8, 0xb7, 0xf1, 0x68, 0x22, 0x9f,
    0xc5, 0x20, 0x80,

    /* U+5FEB "快" */
    0x20, 0x80, 0x82, 0x2, 0x8, 0xf, 0xfc, 0xe8,
    0x92, 0xa2, 0x4a, 0x9, 0x9, 0xff, 0x20, 0xc0,
    0x87, 0x2, 0x12, 0x8, 0xcc, 0x22, 0x18, 0xb0,
    0x32, 0x80, 0x80,

    /* U+6062 "恢" */
    0x21, 0x80, 0x4f, 0xfa, 0xe4, 0x5, 0x49, 0xa,
    0xe2, 0x14, 0x55, 0xa9, 0xfa, 0x16, 0xbc, 0x28,
    0x60, 0x71, 0xa0, 0x86, 0x61, 0x18, 0x62, 0x60,
    0x40,

    /* U+64AD "播" */
    0x20, 0x18, 0x4f, 0xf0, 0x89, 0x27, 0x8a, 0x82,
    0x7f, 0xc4, 0x3e, 0xc, 0xd6, 0x3b, 0x27, 0xaf,
    0xf8, 0x44, 0x90, 0x8f, 0xe1, 0x12, 0x42, 0x24,
    0x8c, 0x7f, 0x18, 0x82, 0x0,

    /* U+653E "放" */
    0x20, 0x40, 0x43, 0x0, 0x8, 0x3f, 0xbf, 0x21,
    0x98, 0x86, 0x63, 0xd9, 0x89, 0x34, 0x24, 0x50,
    0x91, 0xc6, 0x42, 0x11, 0x1c, 0xdd, 0xda, 0x2c,
    0x30,

    /* U+6587 "文" */
    0x6, 0x0, 0x8, 0x0, 0x20, 0x3f, 0xff, 0x10,
    0x40, 0x41, 0x0, 0x84, 0x2, 0x20, 0x4, 0x80,
    0x1c, 0x0, 0x30, 0x3, 0x70, 0x38, 0x7b, 0x80,
    0x70,

    /* U+65B0 "新" */
    0x10, 0xc, 0x21, 0xfb, 0xfa, 0x2, 0x24, 0x2,
    0x88, 0x1f, 0xff, 0xc4, 0x22, 0x8, 0x44, 0xff,
    0x88, 0xa1, 0x11, 0x52, 0x26, 0xb4, 0x49, 0x10,
    0x86, 0x21, 0xc, 0xc2, 0x0,

    /* U+65E5 "日" */
    0xff, 0xe0, 0x18, 0x6, 0x1, 0x80, 0x7f, 0xf8,
    0x6, 0x1, 0x80, 0x60, 0x18, 0x7, 0xff, 0x80,
    0x40,

    /* U+662F "是" */
    0x1f, 0xf0, 0x20, 0x20, 0x7f, 0xc0, 0x80, 0x81,
    0xff, 0x0, 0x0, 0x1f, 0xff, 0x0, 0x80, 0x19,
    0x0, 0x23, 0xf0, 0xe4, 0x1, 0x78, 0x6, 0x1f,
    0xc0,

    /* U+66F2 "曲" */
    0x8, 0x80, 0x44, 0x2, 0x21, 0xff, 0xf8, 0x88,
    0xc4, 0x46, 0x22, 0x3f, 0xff, 0x88, 0x8c, 0x44,
    0x62, 0x23, 0x11, 0x1f, 0xff, 0xc0, 0x4,

    /* U+66F4 "更" */
    0x7f, 0xfc, 0x4, 0x3, 0xff, 0x8, 0x44, 0x21,
    0x10, 0xff, 0xc2, 0x11, 0xf, 0xfc, 0x11, 0x0,
    0x2c, 0x0, 0x60, 0x3, 0x70, 0xf8, 0x7c, 0x0,
    0x0,

    /* U+671F "期" */
    0x22, 0x0, 0x89, 0xf7, 0xf4, 0x48, 0x91, 0x22,
    0x44, 0xf9, 0xf2, 0x24, 0x4f, 0x91, 0x22, 0x7f,
    0xfd, 0x12, 0x4, 0x48, 0xa1, 0x42, 0x85, 0x4,
    0x30,

    /* U+672C "本" */
    0x1, 0x0, 0x2, 0x0, 0x4, 0x3, 0xff, 0xe0,
    0x38, 0x0, 0xf0, 0x1, 0x50, 0x6, 0xa0, 0x9,
    0x20, 0x32, 0x60, 0xc4, 0x63, 0x7f, 0x64, 0x10,
    0x40, 0x20, 0x0, 0x40, 0x0,

    /* U+683C "格" */
    0x10, 0x80, 0x21, 0x0, 0x47, 0xe3, 0xfc, 0xc1,
    0x29, 0x6, 0xc, 0xe, 0x18, 0x1a, 0xce, 0x57,
    0xc, 0xa3, 0xf0, 0x44, 0x20, 0x88, 0x41, 0x1f,
    0x82, 0x21, 0x0,

    /* U+6A21 "模" */
    0x10, 0xd8, 0x27, 0xfc, 0x43, 0x60, 0x80, 0x7,
    0xdf, 0xc2, 0x20, 0x8c, 0x7f, 0x1e, 0x82, 0x75,
    0xfc, 0xa0, 0x40, 0x4f, 0xf8, 0x82, 0x81, 0xc,
    0x82, 0xe0, 0xc0,

    /* U+6D88 "消" */
    0x0, 0x43, 0x31, 0x36, 0x64, 0x89, 0x94, 0x0,
    0x43, 0x1f, 0xe6, 0x40, 0x81, 0xfe, 0x24, 0x8,
    0x90, 0x26, 0x7f, 0x91, 0x2, 0x44, 0xb, 0x10,
    0x64, 0x41, 0x80,

    /* U+7248 "版" */
    0x49, 0xf9, 0x24, 0x4, 0x90, 0x12, 0x40, 0x7d,
    0xf9, 0x7, 0x24, 0x14, 0x9e, 0x52, 0x49, 0x51,
    0x2d, 0xc4, 0xa3, 0x32, 0x9c, 0x8b, 0xcc, 0x0,
    0x0,

    /* U+73AF "环" */
    0x7d, 0xfe, 0x20, 0x60, 0x40, 0x80, 0x81, 0x7,
    0xc2, 0x82, 0xd, 0x4, 0x39, 0x8, 0x53, 0x1d,
    0xa2, 0xf6, 0x41, 0x8, 0x80, 0x1, 0x0, 0x2,
    0x0, 0x4, 0x0,

    /* U+786E "确" */
    0x0, 0x80, 0x3, 0x7, 0xdf, 0x4, 0x44, 0x12,
    0x30, 0x8f, 0xf3, 0xd2, 0x59, 0x49, 0x65, 0xfd,
    0x94, 0x92, 0x52, 0x49, 0x7f, 0x3d, 0x24, 0x9c,
    0x90, 0x22, 0xc0,

    /* U+79D2 "秒" */
    0x0, 0x20, 0x38, 0x41, 0xc0, 0x80, 0x85, 0x41,
    0x1a, 0x5f, 0xb4, 0xc4, 0x48, 0x1f, 0x92, 0x36,
    0x2c, 0xe0, 0x13, 0x40, 0x64, 0x81, 0x81, 0xe,
    0x2, 0xf0, 0x4, 0x80, 0x0,

    /* U+7EA7 "级" */
    0x20, 0x0, 0x9f, 0xc2, 0x11, 0x13, 0x44, 0x49,
    0x33, 0xe4, 0x89, 0x13, 0x88, 0x42, 0x61, 0x8b,
    0xef, 0x60, 0x25, 0x6, 0x8c, 0xe4, 0x70, 0x33,
    0x60, 0xb0, 0xc0,

    /* U+7F6E "置" */
    0x7f, 0xf9, 0x22, 0x67, 0xff, 0x80, 0x80, 0xff,
    0xf8, 0xff, 0x82, 0x2, 0xf, 0xf8, 0x20, 0x20,
    0xff, 0x82, 0x2, 0xf, 0xf8, 0xff, 0xfc,

    /* U+80CC "背" */
    0x4, 0x40, 0x8, 0x89, 0xf1, 0xf0, 0x22, 0x7,
    0xc4, 0x38, 0x8f, 0xc0, 0x0, 0xf, 0xfc, 0x10,
    0x8, 0x3f, 0xf0, 0x40, 0x20, 0xff, 0xc1, 0x0,
    0x82, 0x7, 0x4, 0xc, 0x0,

    /* U+82F1 "英" */
    0x18, 0x43, 0xff, 0xf1, 0x84, 0x0, 0x80, 0x3f,
    0xf0, 0x88, 0xc2, 0x23, 0x8, 0x8c, 0xff, 0xfc,
    0x1e, 0x0, 0x4c, 0x6, 0x18, 0xf0, 0x3d, 0x0,
    0x0,

    /* U+8868 "表" */
    0x2, 0x0, 0x8, 0x7, 0xff, 0x80, 0x80, 0x3f,
    0xf0, 0x8, 0xf, 0xff, 0xc3, 0x0, 0x19, 0x30,
    0xe5, 0x8e, 0x8c, 0x22, 0x98, 0xe, 0x3c, 0x20,
    0x20,

    /* U+8A00 "言" */
    0x6, 0x0, 0x10, 0x3f, 0xfe, 0x0, 0x3, 0xfe,
    0x0, 0x0, 0x0, 0x7, 0xfc, 0x0, 0x1, 0xff,
    0x8, 0x8, 0x40, 0x43, 0xfe, 0x10, 0x10,

    /* U+8BA4 "认" */
    0x20, 0x80, 0xc2, 0x1, 0x88, 0x0, 0x20, 0x0,
    0x83, 0x82, 0x2, 0x8, 0x8, 0x30, 0x21, 0x40,
    0xa5, 0x3, 0xb2, 0xc, 0x8c, 0x24, 0x18, 0x60,
    0x30, 0x0, 0x0,

    /* U+8BBE "设" */
    0x0, 0x1, 0x8f, 0x83, 0x22, 0x0, 0x88, 0x6,
    0x23, 0x90, 0xf2, 0x0, 0x9, 0xfc, 0x22, 0x10,
    0x88, 0x82, 0x96, 0xe, 0x70, 0x31, 0xc0, 0x9c,
    0xf0, 0xc0, 0xc0,

    /* U+8BED "语" */
    0x4f, 0xf8, 0x82, 0x0, 0x7f, 0x0, 0x44, 0xe1,
    0x10, 0xbf, 0xf2, 0x0, 0x8, 0x0, 0x23, 0xf8,
    0xe8, 0x63, 0x21, 0x88, 0x86, 0x63, 0xf8, 0x8,
    0x60,

    /* U+8FDB "进" */
    0x1, 0x21, 0x84, 0x82, 0x12, 0xb, 0xff, 0x1,
    0x23, 0x84, 0x8e, 0x12, 0xb, 0xff, 0x21, 0x20,
    0x8c, 0x82, 0x22, 0x9, 0x88, 0x5c, 0x22, 0x1f,
    0xf0,

    /* U+9000 "退" */
    0x67, 0xf8, 0x48, 0x10, 0x90, 0x21, 0x3f, 0xc0,
    0x40, 0x80, 0x81, 0x39, 0xfe, 0x12, 0x46, 0x24,
    0xd8, 0x48, 0xe0, 0x9e, 0x61, 0x30, 0x67, 0x80,
    0x10, 0xff, 0x80,

    /* U+91CF "量" */
    0x3f, 0xf0, 0xff, 0xc2, 0x1, 0x7, 0xf8, 0xff,
    0xfc, 0x0, 0x3, 0xff, 0xf, 0xfc, 0x23, 0x10,
    0x7f, 0x87, 0xff, 0x0, 0xc0, 0xff, 0xfc,

    /* U+95ED "闭" */
    0x20, 0x3, 0xbf, 0xc0, 0x3, 0x2, 0x18, 0x10,
    0xdf, 0xe6, 0xc, 0x30, 0xe1, 0x8d, 0xc, 0xc8,
    0x6c, 0x43, 0x42, 0x18, 0x30, 0xc0, 0x1c,

    /* U+9664 "除" */
    0x0, 0x83, 0xc6, 0x9, 0x14, 0x24, 0x98, 0xa6,
    0x3a, 0xbf, 0xea, 0x8, 0x24, 0x20, 0x97, 0xfa,
    0x42, 0xb, 0x2a, 0x21, 0xac, 0x8c, 0x9a, 0x22,
    0x28, 0x18, 0x0,

    /* U+97F3 "音" */
    0x2, 0x0, 0xc, 0x7, 0xff, 0x80, 0x10, 0x8,
    0x40, 0x21, 0xf, 0xff, 0xc0, 0x0, 0x3f, 0xf0,
    0xc0, 0xc3, 0xff, 0xc, 0xc, 0x30, 0x30, 0xff,
    0xc3, 0x3, 0x0,

    /* U+FF01 "！" */
    0xfe, 0x60,

    /* U+FF0C "，" */
    0xf7,

    /* U+FF1A "：" */
    0xf0, 0xf0,

    /* U+FF1B "；" */
    0xf0, 0xf7,

    /* U+FF1F "？" */
    0x33, 0x68, 0x82, 0x18, 0x42, 0x8, 0x60, 0x6,
    0x18
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] =
{
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 128, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 128, .box_w = 1, .box_h = 12, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 128, .box_w = 4, .box_h = 3, .ofs_x = 2, .ofs_y = 9},
    {.bitmap_index = 5, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 16, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 23, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 30, .adv_w = 128, .box_w = 1, .box_h = 3, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 31, .adv_w = 128, .box_w = 7, .box_h = 1, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 32, .adv_w = 128, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 33, .adv_w = 128, .box_w = 8, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 46, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 57, .adv_w = 128, .box_w = 3, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 62, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 71, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 82, .adv_w = 128, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 94, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 105, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 116, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 127, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 138, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 149, .adv_w = 128, .box_w = 1, .box_h = 8, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 150, .adv_w = 128, .box_w = 1, .box_h = 10, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 152, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 161, .adv_w = 128, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 173, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 182, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 193, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 202, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 211, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 220, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 229, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 238, .adv_w = 128, .box_w = 1, .box_h = 12, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 240, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 249, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 260, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 269, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 280, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 289, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 300, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 309, .adv_w = 128, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 321, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 330, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 339, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 348, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 357, .adv_w = 128, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 369, .adv_w = 128, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 381, .adv_w = 128, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 393, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 404, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 413, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 420, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 429, .adv_w = 128, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 435, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 444, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 451, .adv_w = 128, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 462, .adv_w = 128, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 471, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 480, .adv_w = 128, .box_w = 1, .box_h = 12, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 482, .adv_w = 128, .box_w = 4, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 489, .adv_w = 128, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 498, .adv_w = 128, .box_w = 1, .box_h = 12, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 500, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 507, .adv_w = 128, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 513, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 520, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 528, .adv_w = 128, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 536, .adv_w = 128, .box_w = 4, .box_h = 8, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 540, .adv_w = 128, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 546, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 556, .adv_w = 128, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 562, .adv_w = 128, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 568, .adv_w = 128, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 576, .adv_w = 128, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 582, .adv_w = 128, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 591, .adv_w = 128, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 597, .adv_w = 256, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = 9},
    {.bitmap_index = 599, .adv_w = 256, .box_w = 6, .box_h = 5, .ofs_x = 9, .ofs_y = 9},
    {.bitmap_index = 603, .adv_w = 256, .box_w = 4, .box_h = 5, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 606, .adv_w = 256, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 608, .adv_w = 256, .box_w = 11, .box_h = 15, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 629, .adv_w = 256, .box_w = 15, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 654, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 679, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 706, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 733, .adv_w = 256, .box_w = 13, .box_h = 15, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 758, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 783, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 810, .adv_w = 256, .box_w = 15, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 835, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 862, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 889, .adv_w = 256, .box_w = 14, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 912, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 937, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 962, .adv_w = 256, .box_w = 15, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 987, .adv_w = 256, .box_w = 15, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1012, .adv_w = 256, .box_w = 14, .box_h = 16, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1040, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1067, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1092, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1119, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1146, .adv_w = 256, .box_w = 15, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1171, .adv_w = 256, .box_w = 15, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1200, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1225, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1250, .adv_w = 256, .box_w = 15, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1279, .adv_w = 256, .box_w = 10, .box_h = 13, .ofs_x = 3, .ofs_y = -1},
    {.bitmap_index = 1296, .adv_w = 256, .box_w = 15, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1321, .adv_w = 256, .box_w = 13, .box_h = 14, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 1344, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1369, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1394, .adv_w = 256, .box_w = 15, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1423, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1450, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1477, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1504, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1529, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1556, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1583, .adv_w = 256, .box_w = 15, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1612, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1639, .adv_w = 256, .box_w = 14, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1662, .adv_w = 256, .box_w = 15, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1691, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1716, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1741, .adv_w = 256, .box_w = 13, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1764, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1791, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1818, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1843, .adv_w = 256, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1868, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1895, .adv_w = 256, .box_w = 14, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1918, .adv_w = 256, .box_w = 13, .box_h = 14, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 1941, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1968, .adv_w = 256, .box_w = 14, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1995, .adv_w = 256, .box_w = 1, .box_h = 11, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 1997, .adv_w = 256, .box_w = 2, .box_h = 4, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 1998, .adv_w = 256, .box_w = 2, .box_h = 6, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 2000, .adv_w = 256, .box_w = 2, .box_h = 8, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 2002, .adv_w = 256, .box_w = 6, .box_h = 12, .ofs_x = 2, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint8_t glyph_id_ofs_list_0[] =
{
    0, 1, 2, 3, 0, 0, 0, 0,
    0, 0, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17,
    18, 19, 20, 21, 0, 0, 0, 22
};

static const uint16_t unicode_list_3[] =
{
    0x0, 0x1f6c, 0x2f51, 0x2f52, 0x4d7d, 0x4dde, 0x4e46, 0x5099,
    0x50c3, 0x514a, 0x5167, 0x5170, 0x5266, 0x5297, 0x52a5, 0x52d2,
    0x5326, 0x5347, 0x5376, 0x55b8, 0x585d, 0x5e5f, 0x5ea5, 0x5efa,
    0x5f3b, 0x5fb2, 0x63fd, 0x648e, 0x64d7, 0x6500, 0x6535, 0x657f,
    0x6642, 0x6644, 0x666f, 0x667c, 0x678c, 0x6971, 0x6cd8, 0x7198,
    0x72ff, 0x77be, 0x7922, 0x7df7, 0x7ebe, 0x801c, 0x8241, 0x87b8,
    0x8950, 0x8af4, 0x8b0e, 0x8b3d, 0x8f2b, 0x8f50, 0x911f, 0x953d,
    0x95b4, 0x9743, 0xfe51, 0xfe5c, 0xfe6a, 0xfe6b, 0xfe6f
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 32, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = glyph_id_ofs_list_0, .list_length = 32, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL
    },
    {
        .range_start = 65, .range_length = 26, .glyph_id_start = 24,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 97, .range_length = 26, .glyph_id_start = 50,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 176, .range_length = 65136, .glyph_id_start = 76,
        .unicode_list = unicode_list_3, .glyph_id_ofs_list = NULL, .list_length = 63, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

/*Store all the custom data of the font*/
static lv_font_fmt_txt_dsc_t font_dsc =
{
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
lv_font_t lv_font_16 =
{
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 16,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};

#endif /*#if LV_FONT_16*/

