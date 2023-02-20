#include "page_language.h"

#define CHINESE_LANGUAGE_DEFINE(PTR)    Chinese_##PTR
#define ENGLISH_LANGUAGE_DEFINE(PTR)    English_##PTR


static led_brightness_language_ptr_t CHINESE_LANGUAGE_DEFINE(led_brightness) = {
    .ptr_lift = "正常",
    .ptr_right = "低亮",
    .ptr_top = "状态灯调节",
};

static monitor_set_language_ptr_t CHINESE_LANGUAGE_DEFINE(monitor_set) = {
    .ptr_lift = "UAC输入",
    .ptr_right = "TX输入",
    .ptr_top = "监听输入",
};

static sound_effect_language_ptr_t CHINESE_LANGUAGE_DEFINE(sound_effect) = {
    .ptr_lift = "高保真",
    .ptr_right = "人声增强",
    .ptr_top = "音效模式",
};

static b_two_in_one_language_page_t CHINESE_LANGUAGE_DEFINE(b_two_in_one) = {
    .page_led = &CHINESE_LANGUAGE_DEFINE(led_brightness),
    .page_monitor = &CHINESE_LANGUAGE_DEFINE(monitor_set),
    .page_sound = &CHINESE_LANGUAGE_DEFINE(sound_effect),
};


static lineout_left_language_ptr_t CHINESE_LANGUAGE_DEFINE(lineout_left) = {
    .ptr_lift = "左声道",
    .ptr_single = "单声道",
    .ptr_stft = "安全音轨",
};

static lineout_right_language_ptr_t CHINESE_LANGUAGE_DEFINE(lineout_right) = {
    .ptr_right = "右声道",
};

static monitor_volume_language_ptr_t CHINESE_LANGUAGE_DEFINE(monitor_volume) = {
    .ptr_monitor = "监听音量",
};

static noise_level_language_ptr_t CHINESE_LANGUAGE_DEFINE(noise_level) = {
    .ptr_noise = "降噪调节",
};

static uacout_volume_language_ptr_t CHINESE_LANGUAGE_DEFINE(uacout_volume) = {
    .ptr_uacout = "UAC输出音量",
};

static barset_language_page_t CHINESE_LANGUAGE_DEFINE(barset) = {
    .page_lineout_left = &CHINESE_LANGUAGE_DEFINE(lineout_left),
    .page_lineout_right = &CHINESE_LANGUAGE_DEFINE(lineout_right),
    .page_monitor_volume = &CHINESE_LANGUAGE_DEFINE(monitor_volume),
    .page_noise_level = &CHINESE_LANGUAGE_DEFINE(noise_level),
    .page_uacout_volume = &CHINESE_LANGUAGE_DEFINE(uacout_volume),
};




















// static a6902_language_typ_t Chinese_language = {
    
// }

