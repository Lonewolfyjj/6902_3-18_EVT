#include "hl_config.h"
#if !HL_IS_TX_DEVICE()
#if 1
#include <rtthread.h>
#include <rtdevice.h>
#include "lvgl.h"
#include "stdio.h"
#include "lv_port_indev.h"
#include "lv_port_disp.h"

// extern const lv_img_dsc_t test_pic;

static rt_thread_t  lvgl_tid1 = RT_NULL, lvgl_tid2 = RT_NULL, lvgl_tid3 = RT_NULL;
static lv_obj_t *   scr, *btn_1, *btn_label, *btn_label1;
static lv_style_t   h_style, style_2;
static lv_fs_drv_t* test_f;
// static lv_disp_t * ptr = RT_NULL;
static int hl_mod_lvgl_init(void)
{
    lv_init();
    lv_port_disp_init();
    // lv_port_indev_init();
    return 0;
}

void hl_mod_lvgl_style_1(void)
{
    lv_style_init(&style_2);
    // lv_style_set_radius(&style_2, 5);
    // lv_style_set_width(&style_2, 50);
    // lv_style_set_height(&style_2, 80);

    lv_style_set_bg_opa(&style_2, LV_OPA_20);
    lv_style_set_bg_color(&style_2, lv_palette_lighten(LV_PALETTE_GREEN, 1));
    // lv_style_set_bg_grad_color(&style_2, lv_palette_main(LV_PALETTE_BLUE));
    // lv_style_set_bg_grad_dir(&style_2, LV_GRAD_DIR_VER);

    // lv_style_set_bg_main_stop(&style_2, 50);
    // lv_style_set_bg_grad_stop(&style_2, 150);

    lv_style_set_text_color(&style_2, lv_palette_main(LV_PALETTE_RED));
}

static void hl_mod_lvgl_creat_btn(void)
{
    btn_1 = lv_btn_create(scr);
    lv_obj_add_style(btn_1, &style_2, LV_PART_MAIN);
    lv_obj_set_pos(btn_1,0,0);
    lv_obj_set_size(btn_1, 126, 126);
    lv_obj_align(btn_1, LV_ALIGN_TOP_LEFT, 0, 0);

    // btn_label = lv_label_create(btn_1);
    // // lv_obj_set_size(btn_label,80,20);
    // lv_label_set_text(btn_label, "btn1 !");
    // // lv_label_set_long_mode(btn_label, LV_LABEL_LONG_DOT);
    // lv_obj_align(btn_label, LV_ALIGN_TOP_LEFT, 0, 0);

    // btn_label1 = lv_label_create(btn_1);
    // lv_obj_set_size(btn_label1, 80, 20);
    // lv_obj_add_style(btn_label1, &style_2, LV_PART_MAIN);
    // lv_label_set_text(btn_label1, "btn2 !");
    // lv_label_set_long_mode(btn_label1, LV_LABEL_LONG_SCROLL_CIRCULAR);
    // lv_obj_align(btn_label1, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
}

static lv_obj_t* imga;
static lv_disp_t* screen_ptr;
// LV_IMG_DECLARE(ImgMountain);
LV_IMG_DECLARE(red_60x60);
static void hl_mod_lvgl_creat_btn_l(void)
{
    LV_IMG_DECLARE(red_60x60);
    lv_obj_t * img1 = lv_img_create(lv_scr_act());
    lv_img_set_src(img1, &red_60x60);
    lv_obj_align(img1, LV_ALIGN_CENTER, 0, -10);
    lv_obj_set_size(img1, 100, 40);

    lv_obj_t * img2 = lv_label_create(lv_scr_act());
    // lv_img_set_src(img2, LV_SYMBOL_OK "Accept");
    lv_label_set_text(img2,"Accept");
    lv_obj_set_pos(img1, 0, 0);

    lv_obj_t * img3 = lv_label_create(lv_scr_act());
    lv_label_set_text(img3,"data");
    lv_obj_set_pos(img3, 200, 0);
    //lv_obj_align_to(img2, img1, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    screen_ptr = lv_disp_get_default();
}

// void ddd(void)
// {
//     lv_btn_create()
// }


static void anim_x_cb(void * var, int32_t v)
{
    lv_obj_set_x(var, v);
}

static void anim_size_cb(void * var, int32_t v)
{
    lv_obj_set_size(var, v, v);
}

/**
 * Create a playback animation
 */
void lv_example_anim_2(void)
{

    lv_obj_t * obj = lv_obj_create(lv_scr_act());
    lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, 0);

    lv_obj_align(obj, LV_ALIGN_LEFT_MID, 10, 0);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_values(&a, 10, 50);
    lv_anim_set_time(&a, 1000);
    lv_anim_set_playback_delay(&a, 100);
    lv_anim_set_playback_time(&a, 300);
    lv_anim_set_repeat_delay(&a, 500);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);

    lv_anim_set_exec_cb(&a, anim_size_cb);
    lv_anim_start(&a);
    lv_anim_set_exec_cb(&a, anim_x_cb);
    lv_anim_set_values(&a, 10, 240);
    lv_anim_start(&a);
}

static void hl_mod_lvgl_thread_fun(void* parameter)
{
    scr = lv_scr_act();
    // hl_mod_lvgl_style_1();
    // hl_mod_lvgl_creat_btn();

    // lv_example_btn_1();
lv_example_anim_2();

    while (1) {
        lv_task_handler();
        rt_thread_mdelay(LV_DISP_DEF_REFR_PERIOD);
    }
}

// static void hl_mod_lvgl_thread_timer(void* parameter)
// {
//     while (1) {
//         lv_tick_inc(LV_INDEV_DEF_READ_PERIOD);
//         rt_thread_mdelay(LV_INDEV_DEF_READ_PERIOD);
//     }
// }

static void hl_mod_lvgl_thread_picture(void* parameter)
{
    // FILE *f;
    // test_f = lv_fs_get_drv(LV_FS_STDIO_LETTER);
    // f = fopen("/mnt/sdcard/lvgl/test.txt","w");
    // f = test_f->open_cb(RT_NULL,"test.txt",LV_FS_MODE_WR);
    // LV_IMG_DECLARE(test_pic);
    // lv_obj_t*  pic        = lv_img_create(scr);
    // lv_disp_t* screen_ptr = lv_disp_get_default();
    // lv_img_set_src(pic, &test_pic);
    // lv_obj_align(pic, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    hl_mod_lvgl_creat_btn_l();
    while (1) {
        // f = test_f.open_cb(RT_NULL,"./test.txt",LV_FS_MODE_WR | LV_FS_MODE_RD);
        // test_f.seek_cb(RT_NULL,f,0,LV_FS_SEEK_END);lv_disp_get_default
        rt_thread_mdelay(3000);
        rt_kprintf("testlvglstart\n");
        lv_disp_set_rotation(screen_ptr, LV_DISP_ROT_90);
        rt_kprintf("testlvgl1\n");
        rt_thread_mdelay(3000);
        lv_disp_set_rotation(screen_ptr, LV_DISP_ROT_NONE);
        rt_kprintf("testlvgl2\n");
    }
}

static int lvgl_test_thread(int argc, char** argv)
{
    hl_mod_lvgl_init();
    lvgl_tid1 = rt_thread_create("lvgl_1", hl_mod_lvgl_thread_fun, RT_NULL, 0xB000, 11, 10);

    // lvgl_tid2 = rt_thread_create("lvgl_2", hl_mod_lvgl_thread_timer, RT_NULL, 20480, 11, 10);

    // lvgl_tid3 = rt_thread_create("lvgl_3", hl_mod_lvgl_thread_picture, RT_NULL, 0xB00, 18, 10);

    if (lvgl_tid1 != RT_NULL) {
        rt_kprintf("Lvgl thread 1 init success !\n");
        rt_thread_startup(lvgl_tid1);
    }

    // if (lvgl_tid2 != RT_NULL) {
    //     rt_kprintf("Lvgl thread 2 init success !\n");
    //     rt_thread_startup(lvgl_tid2);
    // }

    // if (lvgl_tid3 != RT_NULL) {
    //     rt_kprintf("Lvgl thread 3 init success !\n");
    //     rt_thread_startup(lvgl_tid3);
    // }
    return RT_EOK;
}

MSH_CMD_EXPORT(lvgl_test_thread, lvgl test thread);
#endif
#endif
