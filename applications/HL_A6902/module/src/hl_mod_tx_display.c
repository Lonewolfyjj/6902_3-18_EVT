// #include <rtthread.h>
// #include <rtdevice.h>
// #include "lvgl.h"
// #include "hl_mod_lvgl.h"
// #include "lv_port_indev.h"

// static int hl_mod_lvgl_init(void)
// {
//     uint8_t i = 0;
//     rt_kprintf("lvgl_init \n");
//     lv_init();
//     // lv_port_disp_init();
//     lv_port_indev_init();
//     // lv_user_gui_init();

//     /* handle the tasks of LVGL */
//     while(i<20)
//     {
//         i++;
//         rt_kprintf("lvgl_init : LV_COLOR_MIX_ROUND_OFS = %d\n",LV_COLOR_MIX_ROUND_OFS);
//         lv_task_handler();
//         rt_thread_mdelay(LV_DISP_DEF_REFR_PERIOD);
//     }
//     return 0;
// }

// MSH_CMD_EXPORT(hl_mod_lvgl_init, run lvgl_mod_init);