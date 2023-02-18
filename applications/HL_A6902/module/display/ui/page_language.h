#ifndef _PAGE_LANGUAGE_H
#define _PAGE_LANGUAGE_H

#include "lvgl.h"
#include "stdio.h"

typedef struct _arc_language_ptr_t
{
    ///等刘杰确认
}arc_language_ptr_t;

typedef struct _led_brightness_language_ptr_t
{
    char* ptr_lift;
    char* ptr_right;
    char* ptr_top;
}led_brightness_language_ptr_t;

typedef struct _monitor_set_language_ptr_t
{
    char* ptr_lift;
    char* ptr_right;
    char* ptr_top;
}monitor_set_language_ptr_t;

typedef struct _sound_effect_language_ptr_t
{
    char* ptr_lift;
    char* ptr_right;
    char* ptr_top;
}sound_effect_language_ptr_t;


typedef struct _b_two_in_one_language_page_t
{
    led_brightness_language_ptr_t page_led;
    monitor_set_language_ptr_t page_monitor;
    sound_effect_language_ptr_t page_sound;
}b_two_in_one_language_page_t;




typedef struct _lineout_left_language_ptr_t
{
    char* ptr_lift;
    char* ptr_single;
    char* ptr_stft;
}lineout_left_language_ptr_t;

typedef struct _lineout_right_language_ptr_t
{
    char* ptr_right;
}lineout_right_language_ptr_t;

typedef struct _monitor_volume_language_ptr_t
{
    char* ptr_monitor;
}monitor_volume_language_ptr_t;

typedef struct _noise_level_language_ptr_t
{
    char* ptr_noise;
}noise_level_language_ptr_t;

typedef struct _tx1_gain_language_ptr_t
{
    char* ptr_tx1;
}tx1_gain_language_ptr_t;

typedef struct _tx2_gain_language_ptr_t
{
    char* ptr_tx2;
}tx2_gain_language_ptr_t;

typedef struct _uacout_volume_language_ptr_t
{
    char* ptr_uacout;;
}uacout_volume_language_ptr_t;

typedef struct _barset_language_page_t
{
    lineout_left_language_ptr_t page_lineout_left;
    lineout_right_language_ptr_t page_lineout_right;
    monitor_volume_language_ptr_t page_monitor_volume;
    noise_level_language_ptr_t page_noise_level;
    tx1_gain_language_ptr_t page_tx1_gain;
    tx2_gain_language_ptr_t page_tx2_gain;
    uacout_volume_language_ptr_t page_uacout_volume;
}barset_language_page_t;


typedef struct _lineout_language_ptr_t
{
    char* ptr_left;
    char* ptr_right;
}lineout_language_ptr_t;

typedef struct _lineout_language_page_t
{
    lineout_language_ptr_t page_lineout;
}lineout_language_page_t;






#endif