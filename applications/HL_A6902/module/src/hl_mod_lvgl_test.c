#include "hl_config.h"
#if !HL_GET_DEVICE_TYPE()
#include <rtthread.h>
#include <rtdevice.h>
#include "lvgl.h"
#include "lv_port_indev.h"
#include "lv_port_disp.h"

static rt_thread_t lvgl_tid1 = RT_NULL, lvgl_tid2 = RT_NULL;
static lv_obj_t *scr,*btn_1,*btn_label,*btn_label1;
static lv_style_t h_style,style_2;
static uint8_t X,Y;

static int hl_mod_lvgl_init(void)
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    return 0;
}

void lv_style_1(void)
{
    lv_style_init(&style_2);
    // lv_style_set_radius(&style_2, 5);
    // lv_style_set_width(&style_2, 50);
    // lv_style_set_height(&style_2, 80);

    lv_style_set_bg_opa(&style_2, LV_OPA_100);
    lv_style_set_bg_color(&style_2, lv_palette_lighten(LV_PALETTE_GREY, 1));
    // lv_style_set_bg_grad_color(&style_2, lv_palette_main(LV_PALETTE_BLUE));
    // lv_style_set_bg_grad_dir(&style_2, LV_GRAD_DIR_VER);

    // lv_style_set_bg_main_stop(&style_2, 50);
    // lv_style_set_bg_grad_stop(&style_2, 150);

    lv_style_set_text_color(&style_2,lv_palette_main(LV_PALETTE_RED));
}

static void creat_btn(void)
{  
    btn_1 = lv_btn_create(scr);
    lv_obj_add_style(btn_1,&style_2,LV_PART_MAIN);
    lv_obj_set_size(btn_1,140,60);
    lv_obj_align(btn_1,LV_ALIGN_CENTER,0,0);

    btn_label = lv_label_create(btn_1);
    lv_obj_set_size(btn_label,80,20);
    lv_label_set_text(btn_label,"This is the first btn !"); 
    lv_label_set_long_mode(btn_label, LV_LABEL_LONG_DOT);
    lv_obj_align(btn_label,LV_ALIGN_TOP_LEFT,0,0);

    btn_label1 = lv_label_create(scr);
    lv_obj_set_size(btn_label1,X,Y);
    lv_obj_add_style(btn_1,&style_2,LV_PART_MAIN);
    lv_label_set_text(btn_label1,"This is the first btn !                              "); 
    lv_label_set_long_mode(btn_label1, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_align(btn_label1,LV_ALIGN_TOP_LEFT,0,0);
}

static void lvgl_thread_fun(void* parameter)
{
    scr = lv_scr_act();
    lv_style_1();
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
    X = atoi(argv[1]);
    Y = atoi(argv[2]);

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