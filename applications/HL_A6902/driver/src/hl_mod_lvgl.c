#include "hl_config.h"
#if !HL_GET_DEVICE_TYPE()

#include <rtthread.h>
#include <rtdevice.h>
#include "lv_conf.h"
#include "lvgl.h"
#include "hl_mod_lvgl.h"
#include "lv_port_indev.h"

static int lvgl_mod_init(void)
{
    uint8_t i = 0;
    rt_kprintf("lvgl_init \n");
    lv_init();
    // lv_port_disp_init();
    lv_port_indev_init();
    // lv_user_gui_init();

    /* handle the tasks of LVGL */
    while(i<20)
    {
        i++;
        rt_kprintf("lvgl_init \n");
        lv_task_handler();
        rt_thread_mdelay(LV_DISP_DEF_REFR_PERIOD);
    }
    return 0;
}

MSH_CMD_EXPORT(lvgl_mod_init, run lvgl_mod_init);

#endif