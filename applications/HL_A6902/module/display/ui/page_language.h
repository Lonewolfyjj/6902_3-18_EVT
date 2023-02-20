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




typedef struct _other_set_language_ptr_t
{
    char* ptr_pair;
    char* ptr_voice_out;
    char* ptr_time_config;
    char* ptr_verson;
    char* ptr_reset_factory;
    char* ptr_upgrade;
}other_set_language_ptr_t;

typedef struct _sound_module_language_ptr_t
{
    char* ptr_single_voice;
    char* ptr_stereo;
    char* ptr_saft_track;
}sound_module_language_ptr_t;

typedef struct _tx_config_language_ptr_t
{
    char* ptr_tx_gain;
    char* ptr_low_qie;
    char* ptr_auto_recording;
    char* ptr_recording_protection;
    char* ptr_storage;
    char* ptr_auto_poweroff;
    char* ptr_status_led;
}tx_config_language_ptr_t;

typedef struct _volume_menu_language_ptr_t
{
    char* ptr_monitor_voice;
    char* ptr_uac_out;
}volume_menu_language_ptr_t;

typedef struct _menu_language_page_t
{
    other_set_language_ptr_t   page_other_set;
    sound_module_language_ptr_t  page_sound_module;
    tx_config_language_ptr_t  page_tx_config;
    volume_menu_language_ptr_t  page_volume_menu;
}menu_language_page_t;



typedef struct _format_language_ptr_t
{
    char* ptr_left;
    char* ptr_right;
    char* prt_top1;
    char* prt_top2;
}format_language_ptr_t;

typedef struct _open_msc_language_ptr_t
{
    char* ptr_left;
    char* ptr_right;
    char* prt_top;
}open_msc_language_ptr_t;

typedef struct _pair_language_ptr_t
{
    char* ptr_left;
    char* ptr_right;
    char* prt_top;
}pair_language_ptr_t;

typedef struct _restore_language_ptr_t
{
    char* ptr_left;
    char* ptr_right;
    char* prt_top;
}restore_language_ptr_t;

typedef struct _s_two_in_one_language_page_t
{
    format_language_ptr_t page_format;
    open_msc_language_ptr_t page_open_msc;
    pair_language_ptr_t page_pair;
    restore_language_ptr_t page_restore;
}s_two_in_one_language_page_t;




typedef struct _storage_language_ptr_t
{
    char* ptr_storage;
}storage_language_ptr_t;

typedef struct _storage_language_page_t
{
    storage_language_ptr_t page_storage;
}storage_language_page_t;




typedef struct _auto_poweroff_language_ptr_t
{
    char* ptr_lift;
    char* ptr_mid;
    char* ptr_right;
    char* ptr_top;
}auto_poweroff_language_ptr_t;

typedef struct _low_cut_language_ptr_t
{
    char* ptr_lift;
    char* ptr_mid;
    char* ptr_right;
    char* ptr_top;
}low_cut_language_ptr_t;

typedef struct _three_in_one_language_page_t
{
    auto_poweroff_language_ptr_t page_auto_poweroff;
    low_cut_language_ptr_t page_low_cut;
}three_in_one_language_page_t;





typedef struct _auto_record_language_ptr_t
{
    char* ptr_left;
    char* ptr_right;
    char* prt_top;
}auto_record_language_ptr_t;

typedef struct _record_protect_language_ptr_t
{
    char* ptr_left;
    char* ptr_right;
    char* prt_top;
}record_protect_language_ptr_t;

typedef struct _soundout_seting_language_ptr_t
{
    char* ptr_left;
    char* ptr_right;
    char* prt_top;
}soundout_seting_language_ptr_t;

typedef struct _two_in_one_language_page_t
{
    auto_record_language_ptr_t page_auto_record;
    record_protect_language_ptr_t page_record_protect;
    soundout_seting_language_ptr_t page_soundout_seting;
}two_in_one_language_page_t;



typedef struct _upgrade_language_ptr_t
{
    char* ptr_upgrading;
    char* ptr_success;
    char* ptr_fail;
}upgrade_language_ptr_t;

typedef struct _upgrade_language_page_t
{
    upgrade_language_ptr_t page_upgrade;
}upgrade_language_page_t;



typedef struct _verson_language_ptr_t
{
    char* ptr_SN_num;
    char* ptr_firmware_info;
    char* ptr_tx1_info;
    char* ptr_tx2_info;
}verson_language_ptr_t;

typedef struct _verson_language_page_t
{
    verson_language_ptr_t page_verson;
}verson_language_page_t;

#endif