// /**
//  * @file page.c
//  * @author your name (you@domain.com)
//  * @brief 
//  * @version 0.1
//  * @date 2022-12-06
//  * 
//  * @copyright Copyright (c) 2022
//  * 
//  */
// #include "page.h"
// #include "page_menu.h"

// //主菜单页面
// LV_IMG_DECLARE(Menu_single_voice);//单声道
// LV_IMG_DECLARE(Menu_tx_config);//Tx设置
// LV_IMG_DECLARE(Menu_noise_config);//降噪设置
// LV_IMG_DECLARE(Menu_voice_config);//音量调节
// LV_IMG_DECLARE(Menu_monitor_config);//监听设置
// LV_IMG_DECLARE(Menu_common_config);//通用设置

// static void page_7_test_cb(uint32_t current)
// {
//     printf("Page_7 check centre icon event :%d\n",current);
// }

// static void page_7_test(void)
// {
//     menu_data_t pic_list[6] = {
//         ADD_IMG_DATA(NULL,NULL,&Menu_single_voice,"单声道"),
//         // ADD_IMG_DATA(NULL,NULL,&Menu_tx_config,"TX设置"),
//         ADD_IMG_DATA(NULL,NULL,&Menu_noise_config,"降噪设置"),        
//         ADD_IMG_DATA(NULL,NULL,&Menu_voice_config,"音量调节"),
//         ADD_IMG_DATA(NULL,NULL,&Menu_tx_config,"TX设置"),
//         ADD_IMG_DATA(NULL,NULL,&Menu_monitor_config,"监听设置"),
//         ADD_IMG_DATA(NULL,NULL,&Menu_common_config,"通用设置"),
//     };
//     page_menu_init(pic_list,6,page_7_test_cb,0);
// }

// //Tx菜单界面
// LV_IMG_DECLARE(Menu_tx_gain);//TX增益
// LV_IMG_DECLARE(Menu_low_qie);//低切
// LV_IMG_DECLARE(Menu_auto_recording);//自动录制
// LV_IMG_DECLARE(Menu_recording_protection);//录制保护
// LV_IMG_DECLARE(Menu_storage);//存储
// LV_IMG_DECLARE(Menu_auto_poweroff);//自动关机
// LV_IMG_DECLARE(Menu_status_led);//状态灯调节

// static void page_8_test_cb(uint32_t current)
// {
//     printf("Page_8 check centre icon event :%d\n",current);
// }

// static void page_8_test(void)
// {
//     menu_data_t pic_list[7] = {
//         ADD_IMG_DATA(NULL,NULL,&Menu_tx_gain,"TX增益"),
//         ADD_IMG_DATA(NULL,NULL,&Menu_low_qie,"低切"),
//         ADD_IMG_DATA(NULL,NULL,&Menu_auto_recording,"自动录制"),
//         ADD_IMG_DATA(NULL,NULL,&Menu_recording_protection,"录制保护"),
//         ADD_IMG_DATA(NULL,NULL,&Menu_storage,"存储"),
//         ADD_IMG_DATA(NULL,NULL,&Menu_auto_poweroff,"自动关机"),
//         ADD_IMG_DATA(NULL,NULL,&Menu_status_led,"状态灯调节"),
//     };
//     page_menu_init(pic_list,7,page_8_test_cb,0);
// }


// //通用设置菜单界面
// LV_IMG_DECLARE(Menu_pair);//配对
// LV_IMG_DECLARE(Menu_time_config);//时间设置
// LV_IMG_DECLARE(Menu_verson);//版本信息
// LV_IMG_DECLARE(Menu_reset_factory);//恢复出厂设置

// static void page_9_test_cb(uint32_t current)
// {
//     printf("Page_9 check centre icon event :%d\n",current);
// }

// static void page_9_test(void)
// {
//     menu_data_t pic_list[4] = {
//         ADD_IMG_DATA(NULL,NULL,&Menu_pair,"配对"),
//         ADD_IMG_DATA(NULL,NULL,&Menu_time_config,"时间设置"),
//         ADD_IMG_DATA(NULL,NULL,&Menu_verson,"版本信息"),
//         ADD_IMG_DATA(NULL,NULL,&Menu_reset_factory,"恢复出厂设置"),
//     };
//     page_menu_init(pic_list,4,page_9_test_cb,0);
// }


// //模式选择菜单界面
// LV_IMG_DECLARE(Menu_single_voice);//单声道
// LV_IMG_DECLARE(Menu_stereo);//立体声
// LV_IMG_DECLARE(Menu_saft_track);//安全音轨

// static void page_10_test_cb(uint32_t current)
// {
//     printf("Page_10 check centre icon event :%d\n",current);
// }

// static void page_10_test(void)
// {
//     menu_data_t pic_list[3] = {
//         ADD_IMG_DATA(NULL,NULL,&Menu_single_voice,"单声道"),
//         ADD_IMG_DATA(NULL,NULL,&Menu_stereo,"立体声"),
//         ADD_IMG_DATA(NULL,NULL,&Menu_saft_track,"安全音轨"),
//     };
//     page_menu_init(pic_list,3,page_10_test_cb,0);
// }


// //音量调节菜单界面
// LV_IMG_DECLARE(Menu_monitor_voice);//监听音量
// LV_IMG_DECLARE(Menu_uac_out);//UAC输出

// static void page_11_test_cb(uint32_t current)
// {
//     printf("Page_11 check centre icon event :%d\n",current);
// }

// static void page_11_test(void)
// {
//     menu_data_t pic_list[2] = {
//         ADD_IMG_DATA(NULL,NULL,&Menu_monitor_voice,"监听音量"),
//         ADD_IMG_DATA(NULL,NULL,&Menu_uac_out,"UAC输出"),
//     };
//     page_menu_init(pic_list,2,page_11_test_cb,0);
// }


// //测试接口
// void page_meue_test(void)
// {
//     page_11_test();
// }