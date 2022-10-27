#include "hl_config.h"
#if !HL_GET_DEVICE_TYPE()
#include <rtthread.h>
#include <rtdevice.h>
#include "lvgl.h"
#include "lv_port_indev.h"
#include "lv_port_disp.h"

static rt_thread_t lvgl_tid1 = RT_NULL, lvgl_tid2 = RT_NULL;
static lv_obj_t *scr,*btn_1,*btn_label;
static lv_style_t h_style;

static int hl_mod_lvgl_init(void)
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    return 0;
}

static void hl_mod_style_init(void)
{
    lv_style_init(&h_style);
}

static void creat_btn(void)
{  
    // lv_style_copy(&h_style, &lv_style_transp);
    btn_1 = lv_btn_create(scr);
    lv_obj_set_size(btn_1,40,20);
    lv_obj_align(btn_1,LV_ALIGN_TOP_LEFT,0,0);
    btn_label = lv_label_create(btn_1);
    lv_label_set_text(btn_label,"BTN 1");  
}

static void lvgl_thread_fun(void* parameter)
{
    scr = lv_scr_act();
    creat_btn();
    while(1)
    {
        lv_task_handler();
        rt_thread_mdelay(LV_DISP_DEF_REFR_PERIOD);
    }
}

static void lvgl_thread_timer(void* parameter)
{
    while(1)
    {
        lv_tick_inc(LV_INDEV_DEF_READ_PERIOD);
        rt_thread_mdelay(LV_INDEV_DEF_READ_PERIOD);
    }
}

static int lvgl_test_thread(int argc, char** argv)
{
    hl_mod_lvgl_init();
    lvgl_tid1 = rt_thread_create("touch_thread", lvgl_thread_fun, RT_NULL, 40960, 18, 10);

    lvgl_tid2 = rt_thread_create("touch_thread_esd", lvgl_thread_timer, RT_NULL, 4096, 18, 10);

    if (lvgl_tid1 != RT_NULL) {
        rt_kprintf("Lvgl thread 1 init success !\n");
        rt_thread_startup(lvgl_tid1);
    }

    if (lvgl_tid2 != RT_NULL) {
        rt_kprintf("Lvgl thread 2 init success !\n");
        rt_thread_startup(lvgl_tid2);
    }
    return RT_EOK;
}

MSH_CMD_EXPORT(lvgl_test_thread, lvgl test thread);
#endif