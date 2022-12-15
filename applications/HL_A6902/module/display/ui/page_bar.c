// /**
//  * @file page_bar.c
//  * @author your name (you@domain.com)
//  * @brief 进度条显示相关
//  * @version 0.1
//  * @date 2022-12-06
//  * 
//  * @copyright Copyright (c) 2022
//  * 
//  */
// #include "page_barset.h"

// LV_IMG_DECLARE(Other_mic_black);
// LV_IMG_DECLARE(Other_voice);

// //左声道设置界面
// void page_bar1_init(void)
// {
//     hl_mod_barset_init(&Other_voice,"左声道",-60,40,-16,1);
// }

// //右声道设置界面
// void page_bar2_init(void)
// {
//     hl_mod_barset_init(&Other_voice,"右声道",-60,40,-16,1);
// }

// //TX1增益设置界面
// void page_bar3_init(void)
// {
//     hl_mod_barset_init(&Other_mic_black,"TX1",-60,40,-16,0);
// }

// //TX2增益设置界面
// void page_bar4_init(void)
// {
//     hl_mod_barset_init(&Other_mic_black,"TX2",-60,40,-16,0);
// }

// //监听音量设置界面
// void page_bar5_init(void)
// {
//     hl_mod_barset_init(&Other_mic_black,"监听音量",-60,40,-16,0);
// }

// //UAC输入音量设置界面
// void page_bar6_init(void)
// {
//     hl_mod_barset_init(&Other_mic_black,"UAC输入音量",-60,40,-16,0);
// }

// //UAC输出音量设置界面
// void page_bar7_init(void)
// {
//     hl_mod_barset_init(&Other_mic_black,"UAC输出音量",-60,40,-16,0);
// }

// //降噪等级设置界面
// void page_bar8_init(void)
// {
//     hl_mod_barset_init(&Other_mic_black,"降噪调节",1,7,2,0);
// }

// //测试接口
// void page_bar_test(void)
// {
//     page_bar8_init();
// }