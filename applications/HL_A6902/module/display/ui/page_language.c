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


static lineout_language_ptr_t CHINESE_LANGUAGE_DEFINE(lineout_ptr) = {
    .ptr_left = "左声道",
    .ptr_right = "右声道",
};

static lineout_language_page_t CHINESE_LANGUAGE_DEFINE(lineout_page) = {
    .page_lineout = &CHINESE_LANGUAGE_DEFINE(lineout_ptr),
};


static other_set_language_ptr_t CHINESE_LANGUAGE_DEFINE(other_set) = {
    .ptr_pair = "配对",
    .ptr_reset_factory = "恢复出厂设置",
    .ptr_time_config = "时间设置",
    .ptr_upgrade = "开启升级",
    .ptr_verson = "版本信息",
    .ptr_voice_out = "外放设置",
};

static sound_module_language_ptr_t CHINESE_LANGUAGE_DEFINE(sound_module) = {
    .ptr_saft_track = "安全音轨",
    .ptr_single_voice = "单声道",
    .ptr_stereo = "立体声",
};

static tx_config_language_ptr_t CHINESE_LANGUAGE_DEFINE(tx_config) = {
    .ptr_auto_poweroff = "自动关机",
    .ptr_auto_recording = "自动录制",
    .ptr_low_qie = "低切",
    .ptr_recording_protection = "录制保护",
    .ptr_status_led = "状态灯调节",
    .ptr_storage = "存储",
    .ptr_tx_gain = "TX增益",
};

static volume_menu_language_ptr_t CHINESE_LANGUAGE_DEFINE(volume_menu) = {
    .ptr_monitor_voice = "监听音量",
    .ptr_uac_out = "UAC输出",
};

static menu_language_page_t CHINESE_LANGUAGE_DEFINE(menu) = {
    .page_other_set = &CHINESE_LANGUAGE_DEFINE(other_set),
    .page_sound_module = &CHINESE_LANGUAGE_DEFINE(sound_module),
    .page_tx_config = &CHINESE_LANGUAGE_DEFINE(tx_config),
    .page_volume_menu = &CHINESE_LANGUAGE_DEFINE(volume_menu),
};

static format_language_ptr_t CHINESE_LANGUAGE_DEFINE(format) = {
    .prt_top1 = "是否格式化TX1",
    .prt_top2 = "是否格式化TX2",
    .ptr_left = "取消",
    .ptr_right = "确定",
};

static open_msc_language_ptr_t CHINESE_LANGUAGE_DEFINE(open_msc) = {
    .prt_top = "是否开启升级",
    .ptr_left = "确定",
    .ptr_right = "取消",
};

static pair_language_ptr_t CHINESE_LANGUAGE_DEFINE(pair) = {
    .prt_top = "是否进入配对",
    .ptr_left = "确定",
    .ptr_right = "取消",
};

static restore_language_ptr_t CHINESE_LANGUAGE_DEFINE(restore) = {
    .prt_top = "取消",
    .ptr_left = "确定",
    .ptr_right = "是否恢复出厂设置",
};

// static s_two_in_one_language_page_t CHINESE_LANGUAGE_DEFINE(s_two_in_one) = {

// }
















// static a6902_language_typ_t Chinese_language = {
    
// }

