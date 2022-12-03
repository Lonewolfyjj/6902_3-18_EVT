// /**
//  * @file lvgl2rtt.c
//  * @author liujie (jie.liu@hollyland-tech.com)
//  * @brief lvgl和rtt对接文件
//  * @version V1.0
//  * @date 2022-10-23
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
//  * <tr><td>2022-10-23     <td>v1.0     <td>liujie     <td>内容
//  * </table>
//  * 
//  */ 
// /* Define to prevent recursive inclusion -------------------------------------*/

// #ifndef RT_USING_LITTLEVGL2RTT

// /* Includes ------------------------------------------------------------------*/

// #include <lvgl.h>
// #include <rtthread.h>
// #include "rtconfig.h"
// #include "lvgl2rtt.h"
// /* typedef -------------------------------------------------------------------*/
// /* define --------------------------------------------------------------------*/
// #define DBG_TAG    "LVGL"
// #define DBG_LVL    DBG_INFO
// #include <rtdbg.h>
// #ifndef PKG_LVGL_THREAD_STACK_SIZE
// #define PKG_LVGL_THREAD_STACK_SIZE 8192
// #endif /* PKG_LVGL_THREAD_STACK_SIZE */

// // #ifndef PKG_LVGL_THREAD_PRIO
// // // #define PKG_LVGL_THREAD_PRIO (RT_THREAD_PRIORITY_MAX*2/3)
// // #endif /* PKG_LVGL_THREAD_PRIO */
// #define PKG_LVGL_THREAD_PRIO 18
// /* variables -----------------------------------------------------------------*/
// /* Private function(only *.c)  -----------------------------------------------*/







// static lv_rtt_disp_init lv_port_disp_init = RT_NULL;
// static lv_rtt_indev_init lv_port_indev_init = RT_NULL;
// static lv_rtt_user_gui_init lv_user_gui_init = RT_NULL;


// static struct rt_thread lvgl_thread;
// static ALIGN(8) rt_uint8_t lvgl_thread_stack[PKG_LVGL_THREAD_STACK_SIZE];

// #if LV_USE_LOG
// static void lv_rt_log(const char *buf)
// {
//     LOG_I(buf);
// }
// #endif /* LV_USE_LOG */

// static void lvgl_thread_entry(void *parameter)
// {
// #if LV_USE_LOG
//     lv_log_register_print_cb(lv_rt_log);
// #endif /* LV_USE_LOG */
//     lv_init();
    
//     if (!lv_port_indev_init || !lv_port_disp_init || !lv_user_gui_init) {
//         rt_kprintf("lvgl2rtt err, dont reg callback\r\n");
//     }
//     lv_port_disp_init();
//     lv_port_indev_init();
//     lv_user_gui_init();

//     /* handle the tasks of LVGL */
//     while(1)
//     {
//         lv_task_handler();
//         rt_thread_mdelay(LV_DISP_DEF_REFR_PERIOD);
//     }
// }
// /* Exported functions --------------------------------------------------------*/

// void lvgl2rtt_port_disp_reg(lv_rtt_disp_init func)
// {
//     if (func == RT_NULL) {
//         rt_kprintf("lvgl2rtt err, dont reg  lv_port_disp_init !\r\n");
//     }

//     lv_port_disp_init = func;
// }

// void lvgl2rtt_port_indev_reg(lv_rtt_indev_init func)
// {
//     if (func == RT_NULL) {
//         rt_kprintf("lvgl2rtt err, dont reg  lv_rtt_indev_init !\r\n");
//     }
//     lv_port_indev_init = func;
// }

// void lvgl2rtt_port_usergui_reg(lv_rtt_user_gui_init func)
// {
//     if (func == RT_NULL) {
//         rt_kprintf("lvgl2rtt err, dont reg  lv_rtt_user_gui_init !\r\n");
//     }
//     lv_user_gui_init = func;
// }

// int hl_lvgl2thread_init(void)
// {
//     rt_err_t err;

//     err = rt_thread_init(&lvgl_thread, "LVGL", lvgl_thread_entry, RT_NULL,
//            &lvgl_thread_stack[0], sizeof(lvgl_thread_stack), PKG_LVGL_THREAD_PRIO, 0);
//     if(err != RT_EOK)
//     {
//         LOG_E("Failed to create LVGL thread");
//         return -1;
//     }
//     rt_thread_startup(&lvgl_thread);

//     return 0;
// }

// int hl_lvgl2thread_deinit(void)
// {
//     return rt_thread_delete(&lvgl_thread);
// }
// #endif
// /*
//  * EOF
//  */