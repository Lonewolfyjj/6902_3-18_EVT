// #include "page_three_in_one.h"
// #include "page_b_two_in_one.h"
// #include "page_s_two_in_one.h"
// #include "page_two_in_one.h"
// #include "page_barset.h"
// #include "page_test.h"
// // #include "language.h"
// #include "page_main.h"
// #include "page_charge.h"
// #include "page_storage.h"
// #include "page_moreone.h"
// #include "page_verson.h"
// #include "page_upgrade.h"

// LV_IMG_DECLARE(Other_mic_black);
// LV_IMG_DECLARE(Other_voice);

// static void hl_bar_test_cb(int16_t bar_num)
// {
//     printf("bar_num = %d\n", bar_num);
// }

// //左声道设置界面
// void page_bar1_init(void)
// {
//     hl_lvgl_barset_init_t bar_test = 
//     {
//         .func_cb = hl_bar_test_cb,
//         .icontyp = HL_STEREO_ICON,
//         .init_value = -16,
//         .ptr = "左声道",
//         .range_max = 40,
//         .range_min = -60,
//         .src = &Other_voice,
//     };
//     hl_mod_barset_init(&bar_test);
// }

// //右声道设置界面
// void page_bar2_init(void)
// {
//     hl_lvgl_barset_init_t bar_test = 
//     {
//         .func_cb = hl_bar_test_cb,
//         .icontyp = HL_STEREO_ICON,
//         .init_value = -16,
//         .ptr = "右声道",
//         .range_max = 40,
//         .range_min = -60,
//         .src = &Other_voice,
//     };
//     hl_mod_barset_init(&bar_test);
// }

// //TX1增益设置界面
// void page_bar3_init(void)
// {
//     hl_lvgl_barset_init_t bar_test = 
//     {
//         .func_cb = hl_bar_test_cb,
//         .icontyp = HL_NO_ICON,
//         .init_value = -16,
//         .ptr = "TX1",
//         .range_max = 40,
//         .range_min = -60,
//         .src = &Other_mic_black,
//     };
//     hl_mod_barset_init(&bar_test);
// }

// //TX2增益设置界面
// void page_bar4_init(void)
// {
//     hl_lvgl_barset_init_t bar_test = 
//     {
//         .func_cb = hl_bar_test_cb,
//         .icontyp = HL_NO_ICON,
//         .init_value = -16,
//         .ptr = "TX2",
//         .range_max = 40,
//         .range_min = -60,
//         .src = &Other_mic_black,
//     };
//     hl_mod_barset_init(&bar_test);
// }

// //监听音量设置界面
// void page_bar5_init(void)
// {
//     hl_lvgl_barset_init_t bar_test = 
//     {
//         .func_cb = hl_bar_test_cb,
//         .icontyp = HL_NO_ICON,
//         .init_value = -16,
//         .ptr = "监听音量",
//         .range_max = 40,
//         .range_min = -60,
//         .src = &Other_mic_black,
//     };
//     hl_mod_barset_init(&bar_test);
// }

// //UAC输入音量设置界面
// void page_bar6_init(void)
// {
//     hl_lvgl_barset_init_t bar_test = 
//     {
//         .func_cb = hl_bar_test_cb,
//         .icontyp = HL_NO_ICON,
//         .init_value = -16,
//         .ptr = "UAC输入音量",
//         .range_max = 40,
//         .range_min = -60,
//         .src = &Other_mic_black,
//     };
//     hl_mod_barset_init(&bar_test);
// }

// //UAC输出音量设置界面
// void page_bar7_init(void)
// {
//     hl_lvgl_barset_init_t bar_test = 
//     {
//         .func_cb = hl_bar_test_cb,
//         .icontyp = HL_NO_ICON,
//         .init_value = -16,
//         .ptr = "UAC输出音量",
//         .range_max = 40,
//         .range_min = -60,
//         .src = &Other_mic_black,
//     };
//     hl_mod_barset_init(&bar_test);
// }

// //降噪等级设置界面
// void page_bar8_init(void)
// {
//     hl_lvgl_barset_init_t bar_test = 
//     {
//         .func_cb = hl_bar_test_cb,
//         .icontyp = HL_NO_ICON,
//         .init_value = -16,
//         .ptr = "降噪调节",
//         .range_max = 40,
//         .range_min = -60,
//         .src = &Other_mic_black,
//     };
//     hl_mod_barset_init(&bar_test);
// }

// //低切界面
// static void hl_diqie_test_cb(hl_three_in_one_check_t event_num)
// {
//     printf("event_num = %d\n", event_num);
// }
// static void diqie_test(void)
// {
//     hl_lvgl_three_in_one_init_t three_in_one_test = 
//     {
//         .func_cb = hl_diqie_test_cb,
//         .ptr_lift = "无低切",
//         .ptr_mid = "75HZ",
//         .ptr_right = "150HZ",
//         .ptr_top = "低切",
//         .three_in_one_choose = HL_THREE_ONE_CHOOSE_LEFT,
//     };
//     hl_mod_three_in_one_init(&three_in_one_test);

//     hl_lvgl_three_in_one_ioctl_t three_in_one_test_ctl = 
//     {
//         .three_in_one_choose = HL_THREE_ONE_CHOOSE_LEFT,
//     };

//     hl_mod_three_in_one_ioctl(&three_in_one_test_ctl);
// }

// //自动关机界面
// static void hl_autopoweroff_test_cb(hl_three_in_one_check_t event_num)
// {
//     printf("event_num = %d\n", event_num);
// }
// static void autopoweroff_test(void)
// {
//     hl_lvgl_three_in_one_init_t three_in_one_test = 
//     {
//         .func_cb = hl_autopoweroff_test_cb,
//         .ptr_lift = "永不",
//         .ptr_mid = "15min",
//         .ptr_right = "30min",
//         .ptr_top = "自动关机",
//         .three_in_one_choose = HL_THREE_ONE_CHOOSE_LEFT,
//     };
//     hl_mod_three_in_one_init(&three_in_one_test);

//     hl_lvgl_three_in_one_ioctl_t three_in_one_test_ctl = 
//     {
//         .three_in_one_choose = HL_THREE_ONE_CHOOSE_LEFT,
//     };

//     hl_mod_three_in_one_ioctl(&three_in_one_test_ctl);
// }

// //自动录制界面
// static void hl_autorecode_test_cb(hl_two_in_one_check_t event_num)
// {
//     printf("event_num = %d\n", event_num);
// }
// static void autorecode_test(void)
// {
//     hl_lvgl_two_in_one_init_t two_in_one_test = 
//     {
//         .func_cb = hl_autorecode_test_cb,
//         .ptr_lift = "ON",
//         .ptr_right = "OFF",
//         .ptr_top = "自动录制",
//         .two_in_one_choose = HL_TWO_ONE_CHOOSE_LEFT,
//     };
//     hl_mod_two_in_one_init(&two_in_one_test);

//     hl_lvgl_two_in_one_ioctl_t two_in_one_test_ctl = 
//     {
//         .two_in_one_choose = HL_TWO_ONE_CHOOSE_LEFT,
//     };

//     hl_mod_two_in_one_ioctl(&two_in_one_test_ctl);
// }

// //录制保护界面
// static void hl_recodeprotect_test_cb(hl_two_in_one_check_t event_num)
// {
//     printf("event_num = %d\n", event_num);
// }
// static void recodeprotect_test(void)
// {
//     hl_lvgl_two_in_one_init_t two_in_one_test = 
//     {
//         .func_cb = hl_recodeprotect_test_cb,
//         .ptr_lift = "ON",
//         .ptr_right = "OFF",
//         .ptr_top = "录制保护",
//         .two_in_one_choose = HL_TWO_ONE_CHOOSE_LEFT,
//     };
//     hl_mod_two_in_one_init(&two_in_one_test);

//     hl_lvgl_two_in_one_ioctl_t two_in_one_test_ctl = 
//     {
//         .two_in_one_choose = HL_TWO_ONE_CHOOSE_LEFT,
//     };

//     hl_mod_two_in_one_ioctl(&two_in_one_test_ctl);
// }

// //亮度调节界面
// LV_IMG_DECLARE(Other_led_high_black);// 11
// LV_IMG_DECLARE(Other_led_high_white);// 12
// LV_IMG_DECLARE(Other_led_low_black);//  21
// LV_IMG_DECLARE(Other_led_low_white);//  22

// static void hl_light_test_cb(hl_b_two_in_one_check_t event_num)
// {
//     printf("event_num = %d\n", event_num);
// }
// static void light_test(void)
// {
//     hl_lvgl_b_two_in_one_init_t two_in_one_test = 
//     {
//         .func_cb = hl_light_test_cb,
//         .src11 = &Other_led_high_black,
//         .src12 = &Other_led_high_white,
//         .src21 = &Other_led_low_black,
//         .src22 = &Other_led_low_white,
//         .ptr_lift = "正常",
//         .ptr_right = "低亮",
//         .ptr_top = "状态灯调节",
//         .b_two_in_one_choose = HL_TWO_ONE_CHOOSE_LEFT,
//     };
//     hl_mod_b_two_in_one_init(&two_in_one_test);

//     hl_lvgl_b_two_in_one_ioctl_t two_in_one_test_ctl = 
//     {
//         .b_two_in_one_choose = HL_TWO_ONE_CHOOSE_LEFT,
//     };

//     hl_mod_b_two_in_one_ioctl(&two_in_one_test_ctl);
// }


// //音效模式界面
// LV_IMG_DECLARE(Other_high_true_black);//高保真
// LV_IMG_DECLARE(Other_high_true_white);//高保真
// LV_IMG_DECLARE(Other_sound_black);//声音增强
// LV_IMG_DECLARE(Other_sound_white);//声音增强

// static void hl_soundeffect_test_cb(hl_b_two_in_one_check_t event_num)
// {
//     printf("event_num = %d\n", event_num);
// }
// static void soundeffect_test(void)
// {
//     hl_lvgl_b_two_in_one_init_t two_in_one_test = 
//     {
//         .func_cb = hl_soundeffect_test_cb,
//         .src11 = &Other_high_true_black,
//         .src12 = &Other_high_true_white,
//         .src21 = &Other_sound_black,
//         .src22 = &Other_sound_white,
//         .ptr_lift = "高保真",
//         .ptr_right = "声音增强",
//         .ptr_top = "音效模式",
//         .b_two_in_one_choose = HL_B_TWO_ONE_CHOOSE_LEFT,
//     };
//     hl_mod_b_two_in_one_init(&two_in_one_test);

//     hl_lvgl_b_two_in_one_ioctl_t two_in_one_test_ctl = 
//     {
//         .b_two_in_one_choose = HL_B_TWO_ONE_CHOOSE_LEFT,
//     };

//     hl_mod_b_two_in_one_ioctl(&two_in_one_test_ctl);
// }

// //监听设置界面
// LV_IMG_DECLARE(Other_monitor_uac_black);//
// LV_IMG_DECLARE(Other_monitor_uac_white);//
// LV_IMG_DECLARE(Other_monitor_tx_black);//
// LV_IMG_DECLARE(Other_monitor_tx_white);//

// static void hl_monitor_test_cb(hl_b_two_in_one_check_t event_num)
// {
//     printf("event_num = %d\n", event_num);
// }
// static void monitor_test(void)
// {
//     hl_lvgl_b_two_in_one_init_t two_in_one_test = 
//     {
//         .func_cb = hl_monitor_test_cb,
//         .src11 = &Other_monitor_uac_black,
//         .src12 = &Other_monitor_uac_white,
//         .src21 = &Other_monitor_tx_black,
//         .src22 = &Other_monitor_tx_white,
//         .ptr_lift = "UAC输入",
//         .ptr_right = "TX输入",
//         .ptr_top = "监听设置",
//         .b_two_in_one_choose = HL_B_TWO_ONE_CHOOSE_LEFT,
//     };
//     hl_mod_b_two_in_one_init(&two_in_one_test);

//     hl_lvgl_b_two_in_one_ioctl_t two_in_one_test_ctl = 
//     {
//         .b_two_in_one_choose = HL_B_TWO_ONE_CHOOSE_LEFT,
//     };

//     hl_mod_b_two_in_one_ioctl(&two_in_one_test_ctl);
// }


// //格式化界面
// static void hl_formatting_test_cb(hl_s_two_in_one_check_t event_num)
// {
//     printf("event_num = %d\n", event_num);
// }
// static void formatting_test(void)
// {
//     hl_lvgl_s_two_in_one_init_t s_two_in_one_test = 
//     {
//         .func_cb = hl_formatting_test_cb,
//         .ptr_lift = "取消",
//         .ptr_right = "确定",
//         .ptr_top = "是否格式化TX1",
//         .s_two_in_one_choose = HL_S_TWO_ONE_CHOOSE_LEFT,
//     };
//     hl_mod_s_two_in_one_init(&s_two_in_one_test);

//     hl_lvgl_s_two_in_one_ioctl_t s_two_in_one_test_ctl = 
//     {
//         .s_two_in_one_choose = HL_S_TWO_ONE_CHOOSE_LEFT,
//     };

//     hl_mod_s_two_in_one_ioctl(&s_two_in_one_test_ctl);
// }

// //配对界面
// static void hl_pair_test_cb(hl_s_two_in_one_check_t event_num)
// {
//     printf("event_num = %d\n", event_num);
// }
// static void pair_test(void)
// {
//     hl_lvgl_s_two_in_one_init_t s_two_in_one_test = 
//     {
//         .func_cb = hl_pair_test_cb,
//         .ptr_lift = "取消",
//         .ptr_right = "确定",
//         .ptr_top = "是否进入配对",
//         .s_two_in_one_choose = HL_S_TWO_ONE_CHOOSE_LEFT,
//     };
//     hl_mod_s_two_in_one_init(&s_two_in_one_test);

//     hl_lvgl_s_two_in_one_ioctl_t s_two_in_one_test_ctl = 
//     {
//         .s_two_in_one_choose = HL_S_TWO_ONE_CHOOSE_LEFT,
//     };

//     hl_mod_s_two_in_one_ioctl(&s_two_in_one_test_ctl);
// }

// //恢复出厂设置界面
// static void hl_resetfactory_test_cb(hl_s_two_in_one_check_t event_num)
// {
//     printf("event_num = %d\n", event_num);
// }
// static void resetfactory_test(void)
// {
//     hl_lvgl_s_two_in_one_init_t s_two_in_one_test = 
//     {
//         .func_cb = hl_resetfactory_test_cb,
//         .ptr_lift = "取消",
//         .ptr_right = "确定",
//         .ptr_top = "是否进入配对",
//         .s_two_in_one_choose = HL_S_TWO_ONE_CHOOSE_LEFT,
//     };
//     hl_mod_s_two_in_one_init(&s_two_in_one_test);

//     hl_lvgl_s_two_in_one_ioctl_t s_two_in_one_test_ctl = 
//     {
//         .s_two_in_one_choose = HL_S_TWO_ONE_CHOOSE_LEFT,
//     };

//     hl_mod_s_two_in_one_ioctl(&s_two_in_one_test_ctl);
// }

// //主界面
// void page_main_test(void)
// {    
//     hl_lvgl_main_init_t data = 
//     {
//         .display_tx_device = HL_DISPLAY_DOUBLE,
//         .tx_device_1.electric = 20,
//         .tx_device_1.signal = HL_ONE_SIGNAL,
//         .tx_device_1.record = HL_RECODING,
//         .tx_device_1.volume = -13,

//         .tx_device_2.electric = 56,
//         .tx_device_2.signal = HL_TWO_SIGNAL,
//         .tx_device_2.record = HL_NO_RECODE,
//         .tx_device_2.volume = 17,
//     };
//     hl_mod_main_init(&data);

//     hl_lvgl_main_ioctl_t main_ctl = 
//     {
//         .cmd = HL_CHANGE_TX2_SIGNAL,
//         .tx_device_2.signal = HL_FOUR_SIGNAL,
//     };
//     hl_mod_main_ioctl(&main_ctl);
// }

// //充电界面
// static void page_charge_test(void)
// {    
//     hl_lvgl_charge_init_t data = 
//     {
//         .display_device = HL_CHARGE_DISPLAY_TX1 | HL_CHARGE_DISPLAY_RX | HL_CHARGE_DISPLAY_TX2,
//         .electric.electric_box = 69,
//         .electric.electric_rx = 36,
//         .electric.electric_tx1 = 27,
//         .electric.electric_tx2 = 100,
//     };
//     hl_mod_charge_init(&data);

//     hl_lvgl_charge_ioctl_t charge_ctl = 
//     {
//         .charge_cmd = HL_CHARGE_CHANGE_BOX_ELEC,
//         .electric.electric_box = 83,
//     };
//     hl_mod_charge_ioctl(&charge_ctl);
// }


// //格式化界面
// static void hl_storage_test_cb(hl_storage_check_t event_num)
// {
//     printf("event_num = %d\n", event_num);
// }

// void page_storage_test(void)
// {    
//     hl_lvgl_storage_init_t data = 
//     {
//         .func_cb = hl_storage_test_cb,
//         .ptr_time_tx1 = "9h可用",
//         .ptr_time_tx2 = "3h可用",
//         .storage_choose = HL_STORAGE_CHOOSE_RIGHT,
//         .used_tx1 = 15,
//         .used_tx2 = 68,
//     };
//     hl_mod_storage_init(&data);

//     hl_lvgl_storage_ioctl_t storage_ctl = 
//     {
//         .storage_choose = HL_STORAGE_CHOOSE_RIGHT,
//     };
//     hl_mod_storage_ioctl(&storage_ctl);
// }

// //录制开关、静音、降噪开关界面
// static void hl_moreone_test_cb(hl_storage_check_t event_num)
// {
//     printf("event_num = %d\n", event_num);
// }

// void page_moreone_test(void)
// {    
//     hl_lvgl_moreone_init_t data = 
//     {
//         .func_cb = hl_moreone_test_cb,
//         .moreone_choose_opt.option_one = HL_MOREONE_CHOOSE_LEFT,
//         .moreone_choose_opt.option_two = HL_MOREONE_CHOOSE_RIGHT,
//         .moreone_choose_opt.option_three = HL_MOREONE_CHOOSE_LEFT,
//         .moreone_mid_opt = HL_MOREONE_OPTION_THREE
//     };
//     hl_mod_moreone_init(&data);

//     hl_lvgl_moreone_ioctl_t moreone_ctl = 
//     {
//         .cmd_type = HL_MOREONE_OPTION_CMD,
//         .moreone_choose_opt = HL_MOREONE_OPTION_TWO,
//         .moreone_choose = HL_MOREONE_CHOOSE_LEFT
//     };
//     hl_mod_moreone_ioctl(&moreone_ctl);
// }

// //版本界面
// void page_verson_test(void)
// {    
//     hl_lvgl_verson_init_t data = 
//     {
//         .hl_verson_firmware = "v1.0.0.0",
//         .hl_verson_sn_number = "ME234576",
//         .hl_verson_tx1 = "v1.0.0.1",
//         .hl_verson_tx2 = "v1.0.0.2"
//     };
//     hl_mod_verson_init(&data);

//     hl_lvgl_verson_ioctl_t verson_ctl = 
//     {
//         .verson_choose_opt = HL_VERSON_OPTION_TWO,
//     };
//     hl_mod_verson_ioctl(&verson_ctl);
// }

// static void page_upgrade_init(void)
// {
//     hl_lvgl_upgrade_init_t upgrade_init;
//     upgrade_init.upgrade_progress = 32;
//     hl_mod_lvgl_upgrade_init(&upgrade_init);
// }

// static void page_upgrade_ioctl(void)
// {
//     hl_lvgl_upgrade_ioctl_t upgrade_ioctl;
//     upgrade_ioctl.upgrade_ioctl = HL_UPGRADE_SUCCESS_CMD;
//     hl_mod_lvgl_upgrade_ioctl(&upgrade_ioctl);
// }

// void page_test(void)
// {
//     page_verson_test();
// }