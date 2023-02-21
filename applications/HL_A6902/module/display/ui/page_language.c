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

static s_two_in_one_language_page_t CHINESE_LANGUAGE_DEFINE(s_two_in_one) = {
    .page_format = &CHINESE_LANGUAGE_DEFINE(format),
    .page_open_msc = &CHINESE_LANGUAGE_DEFINE(open_msc),
    .page_pair = &CHINESE_LANGUAGE_DEFINE(pair),
    .page_restore = &CHINESE_LANGUAGE_DEFINE(restore),
};

static storage_language_ptr_t CHINESE_LANGUAGE_DEFINE(storage_ptr) = {
    .ptr_storage = "格式化",
};

static storage_language_page_t CHINESE_LANGUAGE_DEFINE(storage_page) = {
    .page_storage = &CHINESE_LANGUAGE_DEFINE(storage_ptr),
};

static auto_poweroff_language_ptr_t CHINESE_LANGUAGE_DEFINE(auto_poweroff) = {
    .ptr_lift = "永不",
    .ptr_mid = "15min",
    .ptr_right = "30min",
    .ptr_top = "自动关机",
};

static low_cut_language_ptr_t CHINESE_LANGUAGE_DEFINE(low_cut) = {
    .ptr_lift = "无低切",
    .ptr_mid = "75HZ",
    .ptr_right = "150HZ",
    .ptr_top = "低切",
};

static three_in_one_language_page_t CHINESE_LANGUAGE_DEFINE(three_in_one) = {
    .page_auto_poweroff = &CHINESE_LANGUAGE_DEFINE(auto_poweroff),
    .page_low_cut = &CHINESE_LANGUAGE_DEFINE(low_cut),
};


static auto_record_language_ptr_t CHINESE_LANGUAGE_DEFINE(auto_record) = {
    .prt_top = "自动录制",
    .ptr_left = "ON",
    .ptr_right = "OFF",
};

static record_protect_language_ptr_t CHINESE_LANGUAGE_DEFINE(record_protect) = {
    .prt_top = "录制保护",
    .ptr_left = "ON",
    .ptr_right = "OFF",
};

static soundout_seting_language_ptr_t CHINESE_LANGUAGE_DEFINE(soundout_seting) = {
    .prt_top = "外放设置",
    .ptr_left = "ON",
    .ptr_right = "OFF",
};

static two_in_one_language_page_t CHINESE_LANGUAGE_DEFINE(two_in_one) = {
    .page_auto_record = &CHINESE_LANGUAGE_DEFINE(auto_record),
    .page_record_protect = &CHINESE_LANGUAGE_DEFINE(record_protect),
    .page_soundout_seting = &CHINESE_LANGUAGE_DEFINE(soundout_seting),
};

static upgrade_language_ptr_t CHINESE_LANGUAGE_DEFINE(upgrade_ptr) = {
    .ptr_fail = "升级失败",
    .ptr_reset_device = "请手动重启设备",
    .ptr_success = "升级成功",
    .ptr_upgrading = "设备升级中...",
};

static upgrade_language_page_t CHINESE_LANGUAGE_DEFINE(upgrade_page) = {
    .page_upgrade = &CHINESE_LANGUAGE_DEFINE(upgrade_ptr),
};

static verson_language_ptr_t CHINESE_LANGUAGE_DEFINE(verson_ptr) = {
    .ptr_firmware_info = "固件版本信息",
    .ptr_SN_num = "SN序列号",
    .ptr_tx1_info = "TX1版本信息",
    .ptr_tx2_info = "TX2版本信息",
};

static verson_language_page_t CHINESE_LANGUAGE_DEFINE(verson_page) = {
    .page_verson = &CHINESE_LANGUAGE_DEFINE(verson_ptr),
};


static a6902_language_typ_t CHINESE_LANGUAGE_DEFINE(language) = {
    .arc_page_ptr = NULL,
    .b_two_in_one_page_ptr = &CHINESE_LANGUAGE_DEFINE(b_two_in_one),
    .barset_page_prt = &CHINESE_LANGUAGE_DEFINE(barset),
    .lineout_page_ptr = &CHINESE_LANGUAGE_DEFINE(lineout_page),
    .menu_page_ptr = &CHINESE_LANGUAGE_DEFINE(menu),
    .s_two_in_one_page_ptr = &CHINESE_LANGUAGE_DEFINE(s_two_in_one),
    .storage_page_ptr = &CHINESE_LANGUAGE_DEFINE(storage_page),
    .three_in_one_page_ptr = &CHINESE_LANGUAGE_DEFINE(three_in_one),
    .two_in_one_page_ptr = &CHINESE_LANGUAGE_DEFINE(two_in_one),
    .upgrade_page_ptr = &CHINESE_LANGUAGE_DEFINE(upgrade_page),
    .verson_page_ptr = &CHINESE_LANGUAGE_DEFINE(verson_page),
};











static led_brightness_language_ptr_t ENGLISH_LANGUAGE_DEFINE(led_brightness) = {
    .ptr_lift = "正常",
    .ptr_right = "低亮",
    .ptr_top = "状态灯调节",
};

static monitor_set_language_ptr_t ENGLISH_LANGUAGE_DEFINE(monitor_set) = {
    .ptr_lift = "UAC输入",
    .ptr_right = "TX输入",
    .ptr_top = "监听输入",
};

static sound_effect_language_ptr_t ENGLISH_LANGUAGE_DEFINE(sound_effect) = {
    .ptr_lift = "高保真",
    .ptr_right = "人声增强",
    .ptr_top = "音效模式",
};

static b_two_in_one_language_page_t ENGLISH_LANGUAGE_DEFINE(b_two_in_one) = {
    .page_led = &ENGLISH_LANGUAGE_DEFINE(led_brightness),
    .page_monitor = &ENGLISH_LANGUAGE_DEFINE(monitor_set),
    .page_sound = &ENGLISH_LANGUAGE_DEFINE(sound_effect),
};


static lineout_left_language_ptr_t ENGLISH_LANGUAGE_DEFINE(lineout_left) = {
    .ptr_lift = "左声道",
    .ptr_single = "单声道",
    .ptr_stft = "安全音轨",
};

static lineout_right_language_ptr_t ENGLISH_LANGUAGE_DEFINE(lineout_right) = {
    .ptr_right = "右声道",
};

static monitor_volume_language_ptr_t ENGLISH_LANGUAGE_DEFINE(monitor_volume) = {
    .ptr_monitor = "监听音量",
};

static noise_level_language_ptr_t ENGLISH_LANGUAGE_DEFINE(noise_level) = {
    .ptr_noise = "降噪调节",
};

static uacout_volume_language_ptr_t ENGLISH_LANGUAGE_DEFINE(uacout_volume) = {
    .ptr_uacout = "UAC输出音量",
};

static barset_language_page_t ENGLISH_LANGUAGE_DEFINE(barset) = {
    .page_lineout_left = &ENGLISH_LANGUAGE_DEFINE(lineout_left),
    .page_lineout_right = &ENGLISH_LANGUAGE_DEFINE(lineout_right),
    .page_monitor_volume = &ENGLISH_LANGUAGE_DEFINE(monitor_volume),
    .page_noise_level = &ENGLISH_LANGUAGE_DEFINE(noise_level),
    .page_uacout_volume = &ENGLISH_LANGUAGE_DEFINE(uacout_volume),
};


static lineout_language_ptr_t ENGLISH_LANGUAGE_DEFINE(lineout_ptr) = {
    .ptr_left = "左声道",
    .ptr_right = "右声道",
};

static lineout_language_page_t ENGLISH_LANGUAGE_DEFINE(lineout_page) = {
    .page_lineout = &ENGLISH_LANGUAGE_DEFINE(lineout_ptr),
};


static other_set_language_ptr_t ENGLISH_LANGUAGE_DEFINE(other_set) = {
    .ptr_pair = "配对",
    .ptr_reset_factory = "恢复出厂设置",
    .ptr_time_config = "时间设置",
    .ptr_upgrade = "开启升级",
    .ptr_verson = "版本信息",
    .ptr_voice_out = "外放设置",
};

static sound_module_language_ptr_t ENGLISH_LANGUAGE_DEFINE(sound_module) = {
    .ptr_saft_track = "安全音轨",
    .ptr_single_voice = "单声道",
    .ptr_stereo = "立体声",
};

static tx_config_language_ptr_t ENGLISH_LANGUAGE_DEFINE(tx_config) = {
    .ptr_auto_poweroff = "自动关机",
    .ptr_auto_recording = "自动录制",
    .ptr_low_qie = "低切",
    .ptr_recording_protection = "录制保护",
    .ptr_status_led = "状态灯调节",
    .ptr_storage = "存储",
    .ptr_tx_gain = "TX增益",
};

static volume_menu_language_ptr_t ENGLISH_LANGUAGE_DEFINE(volume_menu) = {
    .ptr_monitor_voice = "监听音量",
    .ptr_uac_out = "UAC输出",
};

static menu_language_page_t ENGLISH_LANGUAGE_DEFINE(menu) = {
    .page_other_set = &ENGLISH_LANGUAGE_DEFINE(other_set),
    .page_sound_module = &ENGLISH_LANGUAGE_DEFINE(sound_module),
    .page_tx_config = &ENGLISH_LANGUAGE_DEFINE(tx_config),
    .page_volume_menu = &ENGLISH_LANGUAGE_DEFINE(volume_menu),
};

static format_language_ptr_t ENGLISH_LANGUAGE_DEFINE(format) = {
    .prt_top1 = "是否格式化TX1",
    .prt_top2 = "是否格式化TX2",
    .ptr_left = "取消",
    .ptr_right = "确定",
};

static open_msc_language_ptr_t ENGLISH_LANGUAGE_DEFINE(open_msc) = {
    .prt_top = "是否开启升级",
    .ptr_left = "确定",
    .ptr_right = "取消",
};

static pair_language_ptr_t ENGLISH_LANGUAGE_DEFINE(pair) = {
    .prt_top = "是否进入配对",
    .ptr_left = "确定",
    .ptr_right = "取消",
};

static restore_language_ptr_t ENGLISH_LANGUAGE_DEFINE(restore) = {
    .prt_top = "取消",
    .ptr_left = "确定",
    .ptr_right = "是否恢复出厂设置",
};

static s_two_in_one_language_page_t ENGLISH_LANGUAGE_DEFINE(s_two_in_one) = {
    .page_format = &ENGLISH_LANGUAGE_DEFINE(format),
    .page_open_msc = &ENGLISH_LANGUAGE_DEFINE(open_msc),
    .page_pair = &ENGLISH_LANGUAGE_DEFINE(pair),
    .page_restore = &ENGLISH_LANGUAGE_DEFINE(restore),
};

static storage_language_ptr_t ENGLISH_LANGUAGE_DEFINE(storage_ptr) = {
    .ptr_storage = "格式化",
};

static storage_language_page_t ENGLISH_LANGUAGE_DEFINE(storage_page) = {
    .page_storage = &ENGLISH_LANGUAGE_DEFINE(storage_ptr),
};

static auto_poweroff_language_ptr_t ENGLISH_LANGUAGE_DEFINE(auto_poweroff) = {
    .ptr_lift = "永不",
    .ptr_mid = "15min",
    .ptr_right = "30min",
    .ptr_top = "自动关机",
};

static low_cut_language_ptr_t ENGLISH_LANGUAGE_DEFINE(low_cut) = {
    .ptr_lift = "无低切",
    .ptr_mid = "75HZ",
    .ptr_right = "150HZ",
    .ptr_top = "低切",
};

static three_in_one_language_page_t ENGLISH_LANGUAGE_DEFINE(three_in_one) = {
    .page_auto_poweroff = &ENGLISH_LANGUAGE_DEFINE(auto_poweroff),
    .page_low_cut = &ENGLISH_LANGUAGE_DEFINE(low_cut),
};


static auto_record_language_ptr_t ENGLISH_LANGUAGE_DEFINE(auto_record) = {
    .prt_top = "自动录制",
    .ptr_left = "ON",
    .ptr_right = "OFF",
};

static record_protect_language_ptr_t ENGLISH_LANGUAGE_DEFINE(record_protect) = {
    .prt_top = "录制保护",
    .ptr_left = "ON",
    .ptr_right = "OFF",
};

static soundout_seting_language_ptr_t ENGLISH_LANGUAGE_DEFINE(soundout_seting) = {
    .prt_top = "外放设置",
    .ptr_left = "ON",
    .ptr_right = "OFF",
};

static two_in_one_language_page_t ENGLISH_LANGUAGE_DEFINE(two_in_one) = {
    .page_auto_record = &ENGLISH_LANGUAGE_DEFINE(auto_record),
    .page_record_protect = &ENGLISH_LANGUAGE_DEFINE(record_protect),
    .page_soundout_seting = &ENGLISH_LANGUAGE_DEFINE(soundout_seting),
};

static upgrade_language_ptr_t ENGLISH_LANGUAGE_DEFINE(upgrade_ptr) = {
    .ptr_fail = "升级失败",
    .ptr_reset_device = "请手动重启设备",
    .ptr_success = "升级成功",
    .ptr_upgrading = "设备升级中...",
};

static upgrade_language_page_t ENGLISH_LANGUAGE_DEFINE(upgrade_page) = {
    .page_upgrade = &ENGLISH_LANGUAGE_DEFINE(upgrade_ptr),
};

static verson_language_ptr_t ENGLISH_LANGUAGE_DEFINE(verson_ptr) = {
    .ptr_firmware_info = "固件版本信息",
    .ptr_SN_num = "SN序列号",
    .ptr_tx1_info = "TX1版本信息",
    .ptr_tx2_info = "TX2版本信息",
};

static verson_language_page_t ENGLISH_LANGUAGE_DEFINE(verson_page) = {
    .page_verson = &ENGLISH_LANGUAGE_DEFINE(verson_ptr),
};


static a6902_language_typ_t ENGLISH_LANGUAGE_DEFINE(language) = {
    .arc_page_ptr = NULL,
    .b_two_in_one_page_ptr = &ENGLISH_LANGUAGE_DEFINE(b_two_in_one),
    .barset_page_prt = &ENGLISH_LANGUAGE_DEFINE(barset),
    .lineout_page_ptr = &ENGLISH_LANGUAGE_DEFINE(lineout_page),
    .menu_page_ptr = &ENGLISH_LANGUAGE_DEFINE(menu),
    .s_two_in_one_page_ptr = &ENGLISH_LANGUAGE_DEFINE(s_two_in_one),
    .storage_page_ptr = &ENGLISH_LANGUAGE_DEFINE(storage_page),
    .three_in_one_page_ptr = &ENGLISH_LANGUAGE_DEFINE(three_in_one),
    .two_in_one_page_ptr = &ENGLISH_LANGUAGE_DEFINE(two_in_one),
    .upgrade_page_ptr = &ENGLISH_LANGUAGE_DEFINE(upgrade_page),
    .verson_page_ptr = &ENGLISH_LANGUAGE_DEFINE(verson_page),
};

void* hl_a6902_language_ptr_get(uint8_t language)
{
    if(language == CHINESE){
        return &CHINESE_LANGUAGE_DEFINE(language);
    }else{
        return &ENGLISH_LANGUAGE_DEFINE(language);
    }
}


