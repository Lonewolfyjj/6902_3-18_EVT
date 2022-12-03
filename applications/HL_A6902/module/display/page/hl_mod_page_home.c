/**
 * @file hl_mod_page_home.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-11-30
 * 
 * ██╗  ██╗ ██████╗ ██╗     ██╗  ██╗   ██╗██╗      █████╗ ███╗   ██╗██████╗ 
 * ██║  ██║██╔═══██╗██║     ██║  ╚██╗ ██╔╝██║     ██╔══██╗████╗  ██║██╔══██╗
 * ███████║██║   ██║██║     ██║   ╚████╔╝ ██║     ███████║██╔██╗ ██║██║  ██║
 * ██╔══██║██║   ██║██║     ██║    ╚██╔╝  ██║     ██╔══██║██║╚██╗██║██║  ██║
 * ██║  ██║╚██████╔╝███████╗███████╗██║   ███████╗██║  ██║██║ ╚████║██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-11-30     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/

#include "hl_mod_page_common.h"

#if (!HL_IS_TX_DEVICE())
#include "hl_mod_display.h"
#include "lvgl.h"
#include "rtthread.h"
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

static lv_obj_t* app_window;

LV_IMG_DECLARE(img_star);           //星星图片
LV_IMG_DECLARE(img_cogwheel_argb);  //齿轮图片

#define SAFETY_VOICE 0
#define NORMAL_VOICE 1

#define RECORD_DISABLE 0
#define RECORD_ENABLE 1

typedef struct __device_data_t
{
    uint8_t record;    //录制状态 0:未录制 1：录制
    int16_t volume;    //Tx音量
    int16_t electric;  //电量
    int16_t signal;    //信号强度
} device_data_t;

typedef struct __page_1_t
{
    uint8_t       voice_mode;  //音轨模式
    int16_t       electric;    //Rx电量
    device_data_t tx_device_1;
    device_data_t tx_device_2;
} page_1_t;

/****************************************************************/
/*                          普通音轨                             */
/****************************************************************/

//音量显示
#define MENU_DEV_AREA_WIDE 300
#define MENU_DEV_AREA_HIGH 150

static lv_obj_t *area_tx1, *area_tx2;
static lv_obj_t *voice_bar_tx1, *voice_bar_tx2;
static lv_obj_t *power_bar_tx1, *power_bar_tx2;
static lv_obj_t *voice_img_tx1, *voice_img_tx2;
static lv_obj_t *power_img_tx1, *power_img_tx2;
static lv_obj_t *voice_lab_tx1, *voice_lab_tx2;
static lv_obj_t *power_lab_tx1, *power_lab_tx2;
static lv_obj_t *device_lab_tx1, *device_lab_tx2;
static lv_obj_t *video_dot_tx1, *video_dot_tx2;

static lv_style_t style_area_main;
static lv_style_t style_voice_bar_indicator, style_voice_bar_main;
static lv_style_t style_power_bar_indicator, style_power_bar_main;
static lv_style_t style_voice_label;
static lv_style_t style_power_label;
static lv_style_t style_device_label;
static lv_style_t style_video_dot;

/***************************************************************************************************/
/*                                       组件创建函数                                               */
/***************************************************************************************************/
static void lv_style_page1_init(void)
{
    //lv_area_creat_fun函数使用
    lv_style_init(&style_area_main);
    lv_style_set_bg_opa(&style_area_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_area_main, lv_palette_main(LV_PALETTE_GREY));
    // lv_style_set_bg_color(&style_area_main, lv_palette_darken(LV_PALETTE_GREY,4));//变暗渐变色
    lv_style_set_border_width(&style_area_main, 0);
    lv_style_set_outline_width(&style_area_main, 0);
    lv_style_set_radius(&style_area_main, 5);

    //lv_voice_bar_creat_fun函数使用
    lv_style_init(&style_voice_bar_indicator);
    lv_style_set_bg_opa(&style_voice_bar_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_voice_bar_indicator, lv_palette_main(LV_PALETTE_GREEN));
    // lv_style_set_bg_img_src(&style_voice_bar_indicator,&img_cogwheel_argb);//使用图片做源
    lv_style_set_radius(&style_voice_bar_indicator, 0);

    lv_style_init(&style_voice_bar_main);
    lv_style_set_bg_opa(&style_voice_bar_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_voice_bar_main, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_bg_color(&style_voice_bar_main, lv_palette_lighten(LV_PALETTE_GREY, 1));
    lv_style_set_radius(&style_voice_bar_main, 0);

    //lv_device_bar_creat_fun函数使用
    lv_style_init(&style_power_bar_indicator);
    lv_style_set_bg_opa(&style_power_bar_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_power_bar_indicator, lv_color_white());
    // lv_style_set_bg_img_src(&style_power_bar_indicator,&img_cogwheel_argb);//使用图片做源
    lv_style_set_radius(&style_power_bar_indicator, 0);

    lv_style_init(&style_power_bar_main);
    lv_style_set_bg_opa(&style_power_bar_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_power_bar_main, lv_color_black());
    // lv_style_set_bg_color(&style_power_bar_main, lv_palette_lighten(LV_PALETTE_GREY,1));
    lv_style_set_radius(&style_power_bar_main, 0);

    //lv_voice_lab_creat_fun函数使用
    lv_style_init(&style_voice_label);
    lv_style_set_bg_opa(&style_voice_label, LV_OPA_COVER);
    lv_style_set_bg_color(&style_voice_label, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_text_opa(&style_voice_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_voice_label, lv_color_white());

    //lv_power_lab_creat_fun函数使用
    lv_style_init(&style_power_label);
    lv_style_set_bg_opa(&style_power_label, LV_OPA_COVER);
    lv_style_set_bg_color(&style_power_label, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_text_opa(&style_power_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_power_label, lv_color_white());

    //lv_device_lab_creat_fun
    lv_style_init(&style_device_label);
    lv_style_set_bg_opa(&style_device_label, LV_OPA_COVER);
    lv_style_set_bg_color(&style_device_label, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_text_opa(&style_device_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_device_label, lv_color_white());

    //lv_video_dot_creat_fun
    lv_style_init(&style_video_dot);
    lv_style_set_bg_opa(&style_video_dot, LV_OPA_COVER);
    lv_style_set_bg_color(&style_video_dot, lv_palette_main(LV_PALETTE_RED));  //LV_RADIUS_CIRCLE
    lv_style_set_radius(&style_video_dot, LV_RADIUS_CIRCLE);
}

static lv_obj_t* lv_area_creat_fun(lv_align_t align, lv_coord_t x_offset, lv_coord_t y_offset, lv_coord_t width,
                                   lv_coord_t high)
{
    lv_obj_t* null_area;
    null_area = lv_obj_create(lv_scr_act());
    lv_obj_add_style(null_area, &style_area_main, LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(null_area, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(null_area, width, high);
    lv_obj_align(null_area, align, x_offset, y_offset);
    return null_area;
}

static lv_obj_t* lv_voice_bar_creat_fun(lv_obj_t* align_obj, lv_coord_t x_offset, lv_coord_t y_offset, lv_coord_t width,
                                        lv_coord_t high, int32_t init_value)
{
    lv_obj_t* bar = lv_bar_create(align_obj);
    lv_obj_add_style(bar, &style_voice_bar_main, LV_PART_MAIN);
    lv_obj_add_style(bar, &style_voice_bar_indicator, LV_PART_INDICATOR);
    lv_obj_set_size(bar, width, high);
    lv_obj_align_to(bar, align_obj, LV_ALIGN_LEFT_MID, x_offset, y_offset);
    lv_bar_set_value(bar, init_value, LV_ANIM_ON);
    lv_bar_set_range(bar, -40, 60);
    return bar;
}

static lv_obj_t* lv_power_bar_creat_fun(lv_obj_t* align_obj, lv_coord_t x_offset, lv_coord_t y_offset, lv_coord_t width,
                                        lv_coord_t high, int32_t init_value)
{
    lv_obj_t* bar = lv_bar_create(align_obj);
    lv_obj_add_style(bar, &style_power_bar_main, LV_PART_MAIN);
    lv_obj_add_style(bar, &style_power_bar_indicator, LV_PART_INDICATOR);
    lv_obj_set_size(bar, width, high);
    lv_obj_align_to(bar, align_obj, LV_ALIGN_LEFT_MID, x_offset, y_offset);
    lv_bar_set_value(bar, init_value, LV_ANIM_ON);
    lv_bar_set_range(bar, 0, 100);
    return bar;
}

static lv_obj_t* lv_voice_img_creat_fun(lv_obj_t* align_obj, lv_coord_t x_offset, lv_coord_t y_offset, uint16_t zoom)
{
    lv_obj_t* img = lv_img_create(align_obj);
    lv_img_set_src(img, &img_star);
    lv_img_set_zoom(img, zoom);
    lv_obj_align_to(img, align_obj, LV_ALIGN_LEFT_MID, x_offset, y_offset);
    return img;
}

static lv_obj_t* lv_power_img_creat_fun(lv_obj_t* align_obj, lv_coord_t x_offset, lv_coord_t y_offset, uint16_t zoom)
{
    lv_obj_t* img = lv_img_create(align_obj);
    lv_img_set_src(img, &img_star);
    lv_img_set_zoom(img, zoom);
    lv_obj_align_to(img, align_obj, LV_ALIGN_TOP_RIGHT, x_offset, y_offset);
    return img;
}

static lv_obj_t* lv_voice_lab_creat_fun(lv_obj_t* src_obj, lv_obj_t* align_obj, lv_obj_t* bar_obj, lv_coord_t x_offset,
                                        lv_coord_t y_offset)
{
    char      buf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    lv_obj_t* lab    = lv_label_create(src_obj);
    lv_obj_add_style(lab, &style_voice_label, LV_PART_MAIN);
    lv_snprintf(buf, sizeof(buf), "%ddb", lv_bar_get_value(bar_obj));
    lv_label_set_text(lab, buf);
    lv_obj_align_to(lab, align_obj, LV_ALIGN_OUT_TOP_MID, x_offset, y_offset);
    return lab;
}

static lv_obj_t* lv_power_lab_creat_fun(lv_obj_t* src_obj, lv_obj_t* align_obj, lv_obj_t* bar_obj, lv_coord_t x_offset,
                                        lv_coord_t y_offset)
{
    char      buf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    lv_obj_t* lab    = lv_label_create(src_obj);
    lv_obj_add_style(lab, &style_power_label, LV_PART_MAIN);
    lv_snprintf(buf, sizeof(buf), "%d%%", lv_bar_get_value(bar_obj));
    lv_label_set_text(lab, buf);
    lv_obj_align_to(lab, align_obj, LV_ALIGN_OUT_LEFT_MID, x_offset, y_offset);
    return lab;
}

static lv_obj_t* lv_device_lab_creat_fun(lv_obj_t* align_obj, lv_coord_t x_offset, lv_coord_t y_offset, const char* ptr)
{
    lv_obj_t* lab = lv_label_create(align_obj);
    lv_obj_add_style(lab, &style_device_label, LV_PART_MAIN);
    lv_label_set_text(lab, ptr);
    lv_obj_align_to(lab, align_obj, LV_ALIGN_TOP_LEFT, x_offset, y_offset);
    return lab;
}

static lv_obj_t* lv_video_dot_creat_fun(lv_obj_t* align_obj, lv_coord_t x_offset, lv_coord_t y_offset)
{
    lv_obj_t* dot = lv_bar_create(align_obj);
    lv_obj_add_style(dot, &style_video_dot, LV_PART_MAIN);
    lv_obj_set_size(dot, 10, 10);
    lv_obj_align_to(dot, align_obj, LV_ALIGN_TOP_LEFT, x_offset, y_offset);
    return dot;
}

/***************************************************************************************************/
/*                                       操作回调函数                                               */
/***************************************************************************************************/
static void lv_set_power_bar_value_cb(lv_obj_t* bar_obj, lv_obj_t* lab_obj, int32_t value)
{
    char buf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    lv_bar_set_value(bar_obj, value, LV_ANIM_ON);
    lv_snprintf(buf, sizeof(buf), "%d%%", lv_bar_get_value(bar_obj));
    lv_label_set_text(lab_obj, buf);
}

static void lv_set_voice_bar_value_cb(lv_obj_t* bar_obj, lv_obj_t* lab_obj, int32_t value)
{
    char buf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    lv_bar_set_value(bar_obj, value, LV_ANIM_ON);
    lv_snprintf(buf, sizeof(buf), "%ddb", lv_bar_get_value(bar_obj));
    lv_label_set_text(lab_obj, buf);
}

static void lv_set_vodeo_dot_status_cb(lv_obj_t* dot_obj, uint8_t hide)
{
    if (hide) {
        lv_obj_add_flag(dot_obj, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_clear_flag(dot_obj, LV_OBJ_FLAG_HIDDEN);
    }
}

static void page_1_init(void)
{
    //设备框
    area_tx1 = lv_area_creat_fun(LV_ALIGN_LEFT_MID, 10, 0, 200, 100);
    area_tx2 = lv_area_creat_fun(LV_ALIGN_LEFT_MID, 220, 0, 200, 100);

    //音量喇叭图片
    voice_img_tx1 = lv_voice_img_creat_fun(area_tx1, -15, 25, 256);
    voice_img_tx2 = lv_voice_img_creat_fun(area_tx2, -15, 25, 256);

    //电池图标图片
    power_img_tx1 = lv_power_img_creat_fun(area_tx1, 15, -15, 256);
    power_img_tx2 = lv_power_img_creat_fun(area_tx2, 15, -15, 256);

    //音量大小进度条
    voice_bar_tx1 = lv_voice_bar_creat_fun(area_tx1, 30, 25, 120, 20, 10);
    voice_bar_tx2 = lv_voice_bar_creat_fun(area_tx2, 30, 25, 120, 20, 50);

    //电池电量进度条
    power_bar_tx1 = lv_power_bar_creat_fun(power_img_tx1, 0, 0, 20, 10, 15);
    power_bar_tx2 = lv_power_bar_creat_fun(power_img_tx2, 0, 0, 20, 10, 50);

    //音量大小文本
    voice_lab_tx1 = lv_voice_lab_creat_fun(area_tx1, voice_img_tx1, voice_bar_tx1, 0, 0);
    voice_lab_tx2 = lv_voice_lab_creat_fun(area_tx2, voice_img_tx2, voice_bar_tx2, 0, 0);

    //电池电量大小文本
    power_lab_tx1 = lv_power_lab_creat_fun(area_tx1, power_img_tx1, power_bar_tx1, 0, 0);
    power_lab_tx2 = lv_power_lab_creat_fun(area_tx2, power_img_tx2, power_bar_tx2, 0, 0);

    //设备文本
    device_lab_tx1 = lv_device_lab_creat_fun(area_tx1, -15, -15, "Tx1");
    device_lab_tx2 = lv_device_lab_creat_fun(area_tx2, -15, -15, "Tx2");

    //录制状态红点
    video_dot_tx1 = lv_video_dot_creat_fun(area_tx1, 10, -12);
    video_dot_tx2 = lv_video_dot_creat_fun(area_tx2, 11, -12);
}

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



static void page_1_test(void)
{
    rt_kprintf("page1!\r\n");
    lv_style_page1_init();
    page_1_init();
    // lv_example_anim_2();
}

static void hl_mod_page_setup(void)
{
    page_1_test();
}

static void hl_mod_page_loop(void)
{
}

static void hl_mod_page_exit(void)
{
    hl_mod_page_delete(lv_scr_act());
}

static void hl_mod_page_event(void* btn, int event)
{
}

void hl_mod_page_home_init(void)
{
    bool result;

    // app_window = lv_obj_create(lv_scr_act());
    // app_window = lv_scr_act();

    result =
        PageManager_PageRegister(PAGE_HOME, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, hl_mod_page_event);

    if (result == false) {
        LV_LOG_USER("PAGE_HOME init fail\n");
    }
}
#endif
/*
 * EOF
 */