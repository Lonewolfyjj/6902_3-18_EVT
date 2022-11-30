// /**
//  * @file hl_mod_display_lvgl.c
//  * @author liujie (jie.liu@hollyland-tech.com)
//  * @brief lvgl和RTthread的对接文件，包含输入、输出接口、创建线程接口
//  * @version V1.0
//  * @date 2022-10-11
//  * 
//  * ██╗  ██╗ ██████╗ ██╗     ██╗  ██╗   ██╗██╗      █████╗ ███╗   ██╗██████╗ 
//  * ██║  ██║██╔═══██╗██║     ██║  ╚██╗ ██╔╝██║     ██╔══██╗████╗  ██║██╔══██╗
//  * ███████║██║   ██║██║     ██║   ╚████╔╝ ██║     ███████║██╔██╗ ██║██║  ██║
//  * ██╔══██║██║   ██║██║     ██║    ╚██╔╝  ██║     ██╔══██║██║╚██╗██║██║  ██║
//  * ██║  ██║╚██████╔╝███████╗███████╗██║   ███████╗██║  ██║██║ ╚████║██████╔╝
//  * ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝
//  * @copyright Copyright (c) 2022 hollyland
//  * 
//  * @par 修改日志:
//  * <table>
//  * <tr><th>Date           <th>Version  <th>Author         <th>Description
//  * <tr><td>2022-10-11     <td>v1.0     <td>liujie     <td>内容
//  * </table>
//  * 
//  */ 
// /* Define to prevent recursive inclusion -------------------------------------*/

// /* Includes ------------------------------------------------------------------*/

// #include "hl_mod_display_lvgl.h"
// #if  defined(RT_USING_LITTLEVGL2RTT) && (!HL_GET_DEVICE_TYPE)
// #include "hl_drv_rm69310.h"
// #include "lvgl.h"
// #include "hl_drv_knob.h"
// #include <rtthread.h> 
// #include <rtdevice.h>
// #include "lvgl2rtt.h"
// /* typedef -------------------------------------------------------------------*/
// /* define --------------------------------------------------------------------*/
// #define BUF1_SIZE RT_LV_VER_RES * RT_LV_HOR_RES

// /* variables -----------------------------------------------------------------*/
// static lv_disp_draw_buf_t draw_buf_dsc_1;
// static lv_color_t buf_1[BUF1_SIZE];
// volatile bool disp_flush_enabled = true;

// static lv_indev_t * key_l_vol_indev;
// static lv_indev_t * key_r_vol_indev;
// static lv_indev_t * knob_left_indev;
// static lv_indev_t * knob_right_indev;
// static lv_indev_t * power_key_indev;



// /* Private function(only *.c)  -----------------------------------------------*/
// // 屏幕驱动初始化
// static void disp_init(void);
// // 刷屏的数据初始化
// static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
// static void lv_port_disp_init(void);

// static void keypad_init(void);
// static void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);


// static void encoder_init(void);
// static void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
// static void encoder_handler(void);

// static void touchpad_init(void);
// static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
// static bool touchpad_is_pressed(void);
// static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y);

// // lvgl线程相关参数
// static struct rt_thread lvgl_thread;
// static ALIGN(8) rt_uint8_t lvgl_thread_stack[PKG_LVGL_THREAD_STACK_SIZE];






// #if HL_DISPLAY_SCREEN_HARDWARE
// // SPI屏幕
// // 显示
// static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
// {
//     if (disp_flush_enabled) {
//         hl_drv_rm69310_display_write(area->x1, area->x2, area->y1, area->y2, (const uint8_t *)color_p);
//     }

//     /*IMPORTANT!!!
//      *Inform the graphics library that you are ready with the flushing*/
//     lv_disp_flush_ready(disp_drv);
// }




// #else
// // MIPI屏幕
// // 显示

// #endif


// // 按键初始化
// static void keypad_init(void)
// {

// }

// // 按键扫描函数
// static void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
// {

// }

// // 旋钮初始化
// static void encoder_init(void)
// {

// }
// static void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
// {
//     // hl_drv_knob_read();
//     // data->state = (lv_indev_state_t)(
//     //     g_mousewheel_pressed ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL);
//     // data->enc_diff = g_mousewheel_value;
//     // g_mousewheel_value = 0;
// }

// static void touchpad_init(void)
// {

// }

// static void lv_port_disp_init(void)
// {
//     disp_init();

//     lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, BUF1_SIZE);   /*Initialize the display buffer*/

//     /*-----------------------------------
//      * Register the display in LVGL
//      *----------------------------------*/

//     /*Descriptor of a display driver*/
//     static lv_disp_drv_t disp_drv;   
//     /*Basic initialization*/                      
//     lv_disp_drv_init(&disp_drv);                    

//     /*Set the resolution of the display*/
//     disp_drv.hor_res = RT_LV_HOR_RES;
//     disp_drv.ver_res = RT_LV_VER_RES;

//     /*Used to copy the buffer's content to the display*/
//     disp_drv.flush_cb = disp_flush;

//     /*Set a display buffer*/
//     disp_drv.draw_buf = &draw_buf_dsc_1;

//     /*Finally register the driver*/
//     lv_disp_drv_register(&disp_drv);
// }



// static void lv_port_indev_init(void)
// {
//     static lv_indev_drv_t indev_drv;

    
//     keypad_init();
//     lv_indev_drv_init(&indev_drv); 
//     indev_drv.type = LV_INDEV_TYPE_KEYPAD;      // 按键输入事件
//     indev_drv.read_cb = keypad_read;
//     key_l_vol_indev = lv_indev_drv_register(&indev_drv);

//     encoder_init();

//     touchpad_init();
// }

// static void lv_rtt_user_gui_init(void)
// {
//     lv_obj_t * btn = lv_btn_create(lv_scr_act());
//     lv_obj_set_pos(btn, 10, 10);
//     lv_obj_set_height(btn, 50);
//     lv_obj_set_width(btn, 120);
//     lv_obj_t * lable_test = lv_label_create(btn);
//     lv_label_set_text(lable_test, "hollyland");
// }

// /* Exported functions --------------------------------------------------------*/

// void hl_mod_display_enable_update(void)
// {
//     disp_flush_enabled = true;
// }


// void hl_mod_display_disable_update(void)
// {
//     disp_flush_enabled = false;
// }

// uint8_t hl_mod_display_lvgl_init(void)
// {
//     lvgl2rtt_port_disp_reg(lv_port_disp_init);
//     lvgl2rtt_port_indev_reg(lv_port_indev_init);
//     lvgl2rtt_port_usergui_reg(lv_rtt_user_gui_init);


//     return RT_EOK;
// }

// uint8_t hl_mod_display_lvgl_deinit(void)
// {
//     return RT_EOK;
// }

// #endif
// /*
//  * EOF
//  */