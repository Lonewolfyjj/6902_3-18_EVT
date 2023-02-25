/**
 * @file page_main.c
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief 主界面
 * @version 1.0
 * @date 2023-01-14
 * 
 * ██╗  ██╗ ██████╗ ██╗     ██╗  ██╗   ██╗██╗      █████╗ ███╗   ██╗██████╗ 
 * ██║  ██║██╔═══██╗██║     ██║  ╚██╗ ██╔╝██║     ██╔══██╗████╗  ██║██╔══██╗
 * ███████║██║   ██║██║     ██║   ╚████╔╝ ██║     ███████║██╔██╗ ██║██║  ██║
 * ██╔══██║██║   ██║██║     ██║    ╚██╔╝  ██║     ██╔══██║██║╚██╗██║██║  ██║
 * ██║  ██║╚██████╔╝███████╗███████╗██║   ███████╗██║  ██║██║ ╚████║██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝
 * @copyright Copyright (c) 2023 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2023-01-14     <td>v1.0     <td>dujunjie       <td>初次发布
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/*
 * EOF
 */
#include "page_main.h"
#include "page_style_bit.h"


LV_IMG_DECLARE(Main_horn);//喇叭
LV_IMG_DECLARE(Main_horn_mute);//喇叭-MUTE
LV_IMG_DECLARE(Main_bat);//电池图标
LV_IMG_DECLARE(Main_volumebar_short);//音量柱——短
LV_IMG_DECLARE(Main_volumebar_long);//音量柱——长
LV_IMG_DECLARE(Main_signal_0);//信号
LV_IMG_DECLARE(Main_signal_1);//信号
LV_IMG_DECLARE(Main_signal_2);//信号
LV_IMG_DECLARE(Main_signal_3);//信号
LV_IMG_DECLARE(Main_signal_4);//信号
LV_IMG_DECLARE(Main_charging);//充电图标

#define VOICE_TOP_BAR_WITH  4
#define VOICE_TOP_BAR_DELAY  1000
#define ANIMAtION_TOP_TIME_DOWN  3000

#define SAFETY_VOICE    0
#define NORMAL_VOICE    1

#define RECORD_DISABLE   0
#define RECORD_ENABLE    1

#define ANIMAtION_TIME_DOWN  400
#define ANIMAtION_TIME_UP    50

static lv_obj_t * area_tx1 = RT_NULL,*area_tx2 = RT_NULL;
static lv_obj_t * voice_bar_tx1 = RT_NULL,*voice_bar_tx2 = RT_NULL;
static lv_obj_t * power_bar_tx1 = RT_NULL,*power_bar_tx2 = RT_NULL;
static lv_obj_t * voice_img_tx1 = RT_NULL,*voice_img_tx2 = RT_NULL;
static lv_obj_t * voice_mute_img_tx1 = RT_NULL,*voice_mute_img_tx2 = RT_NULL;
static lv_obj_t * power_img_tx1 = RT_NULL,*power_img_tx2 = RT_NULL;
static lv_obj_t * voice_lab_tx1 = RT_NULL,*voice_lab_tx2 = RT_NULL,*voice_lab_tx11 = RT_NULL,*voice_lab_tx22 = RT_NULL;
static lv_obj_t * voice_bar_top_tx1 = RT_NULL,*voice_bar_top_tx2 = RT_NULL;
static lv_obj_t * device_lab_tx1 = RT_NULL,*device_lab_tx2 = RT_NULL;
static lv_obj_t * video_dot_tx1 = RT_NULL,*video_dot_tx2 = RT_NULL;
static lv_obj_t * tx1_signal_obj[5] = {RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL};
static lv_obj_t * tx2_signal_obj[5] = {RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL};
static lv_obj_t * imgftx1 = RT_NULL,*imgftx2 = RT_NULL;

static uint8_t sign_1 = 0xFF,sign_2 = 0xFF;
static int16_t tx1_value_max;
static int16_t tx2_value_max;
static int16_t tx1_value_start;
static int16_t tx2_value_start;
static lv_anim_t animation_tx1;
static lv_anim_t animation_tx2;

static lv_anim_t animation_top_tx1;
static lv_anim_t animation_top_tx2;

static lv_style_t style_area_main;
static lv_style_t style_voice_sbar_indicator,style_voice_bar_main;
static lv_style_t style_voice_lbar_indicator,style_voice_bar_main;
static lv_style_t style_power_bar_indicator,style_power_bar_main;
static lv_style_t style_voice_label;
static lv_style_t style_power_label;
static lv_style_t style_device_label;
static lv_style_t style_video_dot,style_power_bar_white_indicator,style_power_bar_red_indicator,style_power_bar_green_indicator;
    
static hl_lvgl_main_init_t main_init;
/***************************************************************************************************/
/*                                       组件创建函数                                               */
/***************************************************************************************************/

static void lv_signal_group1_hide_set(uint8_t hide_num)
{
    uint8_t i;
    for(i=0;i<5;i++){
        lv_obj_add_flag(tx1_signal_obj[i],LV_OBJ_FLAG_HIDDEN);
    }
    lv_obj_clear_flag(tx1_signal_obj[hide_num],LV_OBJ_FLAG_HIDDEN);
}

static void lv_signal_group2_hide_set(uint8_t hide_num)
{
    uint8_t i;
    for(i=0;i<5;i++){
        lv_obj_add_flag(tx2_signal_obj[i],LV_OBJ_FLAG_HIDDEN);
    }
    lv_obj_clear_flag(tx2_signal_obj[hide_num],LV_OBJ_FLAG_HIDDEN);
}

static void lv_style_page1_init(void)
{
    lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);
    //lv_area_creat_fun函数使用
    lv_style_init(&style_area_main);
    lv_style_set_bg_opa(&style_area_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_area_main, lv_palette_darken(LV_PALETTE_GREY,4));
    lv_style_set_border_width(&style_area_main,0);
    lv_style_set_outline_width(&style_area_main,0);
    lv_style_set_radius(&style_area_main, 5);

    //lv_voice_sbar_creat_fun函数使用
    lv_style_init(&style_voice_sbar_indicator);
    lv_style_set_bg_opa(&style_voice_sbar_indicator, LV_OPA_COVER);
    // lv_style_set_bg_color(&style_voice_sbar_indicator, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_bg_img_src(&style_voice_sbar_indicator,&Main_volumebar_short);//使用图片做源
    lv_style_set_radius(&style_voice_sbar_indicator, 0);

    lv_style_init(&style_voice_bar_main);
    lv_style_set_bg_opa(&style_voice_bar_main, LV_OPA_COVER);
    // lv_style_set_bg_color(&style_voice_bar_main, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_bg_color(&style_voice_bar_main, lv_palette_darken(LV_PALETTE_GREY,3));
    lv_style_set_radius(&style_voice_bar_main, 0);

    //lv_voice_lbar_creat_fun函数使用
    lv_style_init(&style_voice_lbar_indicator);
    lv_style_set_bg_opa(&style_voice_lbar_indicator, LV_OPA_COVER);
    // lv_style_set_bg_color(&style_voice_lbar_indicator, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_bg_img_src(&style_voice_lbar_indicator,&Main_volumebar_long);//使用图片做源
    lv_style_set_radius(&style_voice_lbar_indicator, 0);

    // lv_style_init(&style_voice_bar_main);
    // lv_style_set_bg_opa(&style_voice_bar_main, LV_OPA_COVER);
    // lv_style_set_bg_color(&style_voice_bar_main, lv_palette_main(LV_PALETTE_GREY));
    // lv_style_set_bg_color(&style_voice_bar_main, lv_palette_lighten(LV_PALETTE_GREY,1));
    // lv_style_set_radius(&style_voice_bar_main, 0);

    //lv_device_bar_creat_fun函数使用
    lv_style_init(&style_power_bar_indicator);
    lv_style_set_bg_opa(&style_power_bar_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_power_bar_indicator, lv_color_white());
    lv_style_set_radius(&style_power_bar_indicator, 0);

    lv_style_init(&style_power_bar_main);
    lv_style_set_bg_opa(&style_power_bar_main, LV_OPA_TRANSP);
    lv_style_set_radius(&style_power_bar_main, 0);

    //lv_voice_lab_creat_fun函数使用
    lv_style_init(&style_voice_label);
    lv_style_set_bg_opa(&style_voice_label, LV_OPA_TRANSP);
    lv_style_set_bg_color(&style_voice_label, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_text_opa(&style_voice_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_voice_label, lv_color_white());

    //lv_power_lab_creat_fun函数使用
    lv_style_init(&style_power_label);
    lv_style_set_bg_opa(&style_power_label, LV_OPA_TRANSP);
    lv_style_set_bg_color(&style_power_label, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_text_opa(&style_power_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_power_label, lv_color_white());

    //lv_device_lab_creat_fun
    lv_style_init(&style_device_label);
    lv_style_set_bg_opa(&style_device_label, LV_OPA_TRANSP);
    lv_style_set_bg_color(&style_device_label, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_text_opa(&style_device_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_device_label, lv_color_white());

    //lv_video_dot_creat_fun
    lv_style_init(&style_video_dot);
    lv_style_set_bg_opa(&style_video_dot, LV_OPA_COVER);
    lv_style_set_bg_color(&style_video_dot, lv_palette_main(LV_PALETTE_RED));//LV_RADIUS_CIRCLE
    lv_style_set_radius(&style_video_dot, LV_RADIUS_CIRCLE);

    lv_style_init(&style_power_bar_white_indicator);
    lv_style_set_bg_opa(&style_power_bar_white_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_power_bar_white_indicator, lv_color_white());
    lv_style_set_radius(&style_power_bar_white_indicator, 0);

    lv_style_init(&style_power_bar_red_indicator);
    lv_style_set_bg_opa(&style_power_bar_red_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_power_bar_red_indicator, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_radius(&style_power_bar_red_indicator, 0);

    lv_style_init(&style_power_bar_green_indicator);
    lv_style_set_bg_opa(&style_power_bar_green_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_power_bar_green_indicator, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_radius(&style_power_bar_green_indicator, 0);
}

static lv_obj_t * lv_area_creat_fun(lv_align_t align,lv_coord_t x_offset,lv_coord_t y_offset,lv_coord_t width, lv_coord_t high)
{
    lv_obj_t * null_area;
    null_area = lv_obj_create(lv_scr_act());
    lv_obj_add_style(null_area, &style_area_main, LV_PART_MAIN);
    lv_obj_clear_flag(null_area, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(null_area, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(null_area,width,high);
    lv_obj_align(null_area,align,x_offset,y_offset);
    lv_obj_move_background(null_area);
    return null_area;
}

static lv_obj_t * lv_voice_sbar_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,lv_coord_t x_offset,lv_coord_t y_offset,lv_coord_t width, lv_coord_t high,int32_t init_value)
{
    lv_obj_t * bar = lv_bar_create(src_obj);
    lv_obj_add_style(bar, &style_voice_bar_main, LV_PART_MAIN);
    lv_obj_add_style(bar, &style_voice_sbar_indicator, LV_PART_INDICATOR);
    lv_bar_set_range(bar,0,118);
    lv_obj_set_size(bar, width, high);
    lv_obj_align_to(bar,align_obj,LV_ALIGN_OUT_RIGHT_MID,x_offset,y_offset);
    lv_bar_set_value(bar, init_value, LV_ANIM_ON);
    return bar;
}

static lv_obj_t * lv_voice_lbar_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,lv_coord_t x_offset,lv_coord_t y_offset,lv_coord_t width, lv_coord_t high,int32_t init_value)
{
    lv_obj_t * bar = lv_bar_create(src_obj);
    lv_obj_add_style(bar, &style_voice_bar_main, LV_PART_MAIN);
    lv_obj_add_style(bar, &style_voice_lbar_indicator, LV_PART_INDICATOR);
    lv_bar_set_range(bar,0,118);
    lv_obj_set_size(bar, width, high);
    lv_obj_align_to(bar,align_obj,LV_ALIGN_OUT_RIGHT_MID,x_offset,y_offset);
    lv_bar_set_value(bar, init_value, LV_ANIM_ON);
    return bar;
}

static lv_obj_t * lv_voice_stopbar_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,lv_coord_t x_offset,lv_coord_t y_offset,lv_coord_t width, lv_coord_t high,int32_t init_value)
{
    lv_obj_t * bar = lv_bar_create(src_obj);
    lv_obj_add_style(bar, &style_power_bar_main, LV_PART_MAIN);
    lv_obj_add_style(bar, &style_voice_sbar_indicator, LV_PART_INDICATOR);
    lv_bar_set_range(bar,0,118);
    lv_obj_set_size(bar, width, high);
    lv_obj_align_to(bar,align_obj,LV_ALIGN_OUT_RIGHT_MID,x_offset,y_offset);
    lv_bar_set_mode(bar, LV_BAR_MODE_RANGE);
    lv_bar_set_start_value(bar,init_value - VOICE_TOP_BAR_WITH,LV_ANIM_ON);
    lv_bar_set_value(bar, init_value, LV_ANIM_ON);
    return bar;
}

static lv_obj_t * lv_voice_ltopbar_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,lv_coord_t x_offset,lv_coord_t y_offset,lv_coord_t width, lv_coord_t high,int32_t init_value)
{
    lv_obj_t * bar = lv_bar_create(src_obj);
    lv_obj_add_style(bar, &style_power_bar_main, LV_PART_MAIN);
    lv_obj_add_style(bar, &style_voice_lbar_indicator, LV_PART_INDICATOR);
    lv_bar_set_range(bar,0,118);
    lv_obj_set_size(bar, width, high);
    lv_obj_align_to(bar,align_obj,LV_ALIGN_OUT_RIGHT_MID,x_offset,y_offset);
    lv_bar_set_mode(bar, LV_BAR_MODE_RANGE);
    lv_bar_set_start_value(bar,init_value - VOICE_TOP_BAR_WITH,LV_ANIM_ON);
    lv_bar_set_value(bar, init_value, LV_ANIM_ON);
    return bar;
}

static lv_obj_t * lv_power_bar_creat_fun(lv_obj_t *align_obj,lv_coord_t x_offset,lv_coord_t y_offset,lv_coord_t width, lv_coord_t high,int32_t init_value)
{
    lv_obj_t * bar = lv_bar_create(align_obj);
    lv_obj_add_style(bar, &style_power_bar_main, LV_PART_MAIN);
    lv_obj_add_style(bar, &style_power_bar_indicator, LV_PART_INDICATOR);
    lv_obj_set_size(bar, width, high);
    lv_obj_align_to(bar,align_obj,LV_ALIGN_LEFT_MID,x_offset,y_offset);
    lv_bar_set_value(bar, init_value, LV_ANIM_ON);
    lv_bar_set_range(bar,0,100);
    lv_obj_move_background(bar);
    return bar;
}

static lv_obj_t * lv_voice_img_creat_fun(lv_obj_t *align_obj,const void * src,lv_coord_t x_offset,lv_coord_t y_offset,uint16_t zoom)
{
    lv_obj_t * img = lv_img_create(align_obj);
    lv_img_set_src(img,src);
    lv_img_set_zoom(img, zoom);
    lv_obj_align_to(img,align_obj,LV_ALIGN_BOTTOM_LEFT,x_offset,y_offset);
    return img;
}

static lv_obj_t * lv_power_img_creat_fun(lv_obj_t *align_obj,lv_coord_t x_offset,lv_coord_t y_offset,uint16_t zoom)
{
    lv_obj_t * img = lv_img_create(align_obj);
    lv_img_set_src(img,&Main_bat);
    lv_img_set_zoom(img, zoom);
    lv_obj_align_to(img,align_obj,LV_ALIGN_TOP_RIGHT,x_offset,y_offset);
    return img;
}

static lv_obj_t * lv_signal_img_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,const void * picture,lv_coord_t x_offset,lv_coord_t y_offset,uint16_t zoom,uint8_t hide)
{
    lv_obj_t * img = lv_img_create(src_obj);
    lv_img_set_src(img,picture);
    lv_img_set_zoom(img, zoom);
    lv_obj_align_to(img,align_obj,LV_ALIGN_OUT_RIGHT_BOTTOM,x_offset,y_offset);
    if(!hide){
        lv_obj_add_flag(img,LV_OBJ_FLAG_HIDDEN);
    }
    return img;
}

static lv_obj_t * lv_voice_lab_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,rt_int16_t init_value,lv_coord_t x_offset,lv_coord_t y_offset)
{
    char buf[8] = {0,0,0,0,0,0,0,0};
    lv_obj_t * lab = lv_label_create(src_obj);
    lv_obj_add_style(lab, &style_voice_label, LV_PART_MAIN);
    lv_snprintf(buf, sizeof(buf), "%d", init_value);
    lv_label_set_text(lab,buf);
    lv_obj_align_to(lab,align_obj,LV_ALIGN_OUT_TOP_RIGHT,x_offset,y_offset);
    return lab;
}

static lv_obj_t * lv_voice_line_out_lab_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,rt_int16_t init_value,lv_coord_t x_offset,lv_coord_t y_offset)
{
    char buf[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    lv_obj_t * lab = lv_label_create(src_obj);
    lv_obj_add_style(lab, &style_voice_label, LV_PART_MAIN);
    lv_snprintf(buf, sizeof(buf), "Output : %d", init_value);
    lv_label_set_text(lab,buf);
    lv_obj_align_to(lab,align_obj,LV_ALIGN_OUT_TOP_MID,x_offset,y_offset);
    return lab;
}

static lv_obj_t * lv_power_lab_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,lv_obj_t *bar_obj,lv_coord_t x_offset,lv_coord_t y_offset)
{
    char buf[8] = {0,0,0,0,0,0,0,0};
    lv_obj_t * lab = lv_label_create(src_obj);
    lv_obj_add_style(lab, &style_power_label, LV_PART_MAIN);
    lv_snprintf(buf, sizeof(buf), "%d%%", lv_bar_get_value(bar_obj));
    lv_label_set_text(lab,buf);
    lv_obj_align_to(lab,align_obj,LV_ALIGN_OUT_LEFT_MID,x_offset,y_offset);
    return lab;
}

static lv_obj_t * lv_device_lab_creat_fun(lv_obj_t *align_obj,lv_coord_t x_offset,lv_coord_t y_offset,const char *ptr)
{
    lv_obj_t * lab = lv_label_create(align_obj);
    lv_obj_add_style(lab, &style_device_label, LV_PART_MAIN);
    lv_label_set_text(lab,ptr);
    lv_obj_align_to(lab,align_obj,LV_ALIGN_TOP_LEFT,x_offset,y_offset);
    return lab;
}

static lv_obj_t * lv_video_dot_creat_fun(lv_obj_t *align_obj,lv_coord_t x_offset,lv_coord_t y_offset,uint8_t hide)
{
    lv_obj_t * dot = lv_bar_create(align_obj);
    lv_obj_add_style(dot, &style_video_dot, LV_PART_MAIN);
    lv_obj_set_size(dot, 10, 10);
    lv_obj_align_to(dot,align_obj,LV_ALIGN_LEFT_MID,x_offset,y_offset);
    if(!hide){
        lv_obj_add_flag(dot,LV_OBJ_FLAG_HIDDEN);
    }
    return dot;
}


static void lv_topbar_set_value_cb(void * bar, int32_t temp)
{
    if(temp == 0){
        // lv_bar_set_start_value(bar,temp - VOICE_TOP_BAR_WITH,LV_ANIM_ON);
        lv_bar_set_value(bar, temp, LV_ANIM_ON);
    }else{
        lv_bar_set_start_value(bar,temp,LV_ANIM_ON);
        lv_bar_set_value(bar, temp + VOICE_TOP_BAR_WITH, LV_ANIM_ON);
    }
    
    if((lv_obj_t *)bar == voice_bar_top_tx1){
        tx1_value_max = temp;
    }
    if((lv_obj_t *)bar == voice_bar_top_tx2){
        tx2_value_max = temp;
    }
}

static void lv_topbar_anim_init(lv_anim_t * anim_obj,lv_obj_t * bar_obj,int16_t init_value,uint32_t anim_time)
{    
    lv_anim_init(anim_obj);
    lv_anim_set_exec_cb(anim_obj, lv_topbar_set_value_cb);
    lv_anim_set_time(anim_obj, anim_time);
    lv_anim_set_var(anim_obj, bar_obj);
    lv_anim_set_values(anim_obj, init_value, 0);
    lv_anim_set_repeat_count(anim_obj, 0);
    lv_anim_set_delay(anim_obj, VOICE_TOP_BAR_DELAY);
    lv_anim_start(anim_obj);
}

static void lv_topbar_anim_ctl(lv_anim_t * anim_obj,int16_t value_start,int16_t value_end,uint32_t anim_time)
{
    if(anim_obj == &animation_top_tx1 && tx1_value_max < value_start){
        lv_anim_set_values(anim_obj, value_start, 0);
        lv_anim_set_repeat_count(anim_obj, 1);
        lv_anim_set_delay(&animation_top_tx1, anim_time);
        lv_anim_start(&animation_top_tx1);
        lv_bar_set_start_value(voice_bar_top_tx1,tx1_value_start - VOICE_TOP_BAR_WITH,LV_ANIM_ON);
        lv_bar_set_value(voice_bar_top_tx1, tx1_value_start, LV_ANIM_ON);    
        tx1_value_max = value_start;    
    }
    if(anim_obj == &animation_top_tx2 && tx2_value_max < value_start){
        lv_anim_set_values(anim_obj, value_start, 0);
        lv_anim_set_repeat_count(anim_obj, 1);
        lv_anim_set_delay(&animation_top_tx2, anim_time);
        lv_anim_start(&animation_top_tx2);
        lv_bar_set_start_value(voice_bar_top_tx2,tx2_value_start - VOICE_TOP_BAR_WITH,LV_ANIM_ON);
        lv_bar_set_value(voice_bar_top_tx2, tx2_value_start, LV_ANIM_ON);      
        tx2_value_max = value_start;          
    }
}

static void lv_bar_set_value_cb(void * bar, int32_t temp)
{
    lv_bar_set_value(bar, temp, LV_ANIM_ON);
}

static void lv_bar_anim_init(lv_anim_t * anim_obj,lv_obj_t * bar_obj,int16_t init_value,uint32_t anim_time)
{    
    lv_anim_init(anim_obj);
    lv_anim_set_exec_cb(anim_obj, lv_bar_set_value_cb);
    lv_anim_set_time(anim_obj, anim_time);
    // lv_anim_set_playback_time(anim_obj, 3000);
    lv_anim_set_path_cb(anim_obj, lv_anim_path_ease_out);
    lv_anim_set_var(anim_obj, bar_obj);
    lv_anim_set_values(anim_obj, init_value, init_value);
    lv_anim_set_repeat_count(anim_obj, 0);
    lv_anim_start(anim_obj);
}

static void lv_bar_anim_ctl(lv_anim_t * anim_obj,int16_t value_start,int16_t value_end,uint32_t anim_time)
{
    lv_anim_set_time(anim_obj, anim_time);
    lv_anim_set_values(anim_obj, value_start, value_end);
    lv_anim_set_repeat_count(anim_obj, 1);
    lv_anim_start(anim_obj);
}

/***************************************************************************************************/
/*                                       操作回调函数                                               */
/***************************************************************************************************/

static void lv_set_vodeo_dot_status_cb(lv_obj_t * dot_obj,uint8_t hide)
{
    if(!hide){
        lv_obj_add_flag(dot_obj,LV_OBJ_FLAG_HIDDEN);
    }else{
        lv_obj_clear_flag(dot_obj,LV_OBJ_FLAG_HIDDEN);
    }
}

static void lv_set_video_hide_cb(lv_obj_t * obj,uint8_t hide)
{
    if(hide){
        lv_obj_add_flag(obj,LV_OBJ_FLAG_HIDDEN);
    }else{
        lv_obj_clear_flag(obj,LV_OBJ_FLAG_HIDDEN);
    }
}

static void lv_signal_hide_set(uint8_t signal_group,uint8_t hide_num)
{
    switch(signal_group){
        case 1:
        if(sign_1 != hide_num){
            sign_1 = hide_num;
            lv_signal_group1_hide_set(hide_num);
        }        
        break;
        case 2:
        if(sign_2 != hide_num){
            sign_2 = hide_num;
            lv_signal_group2_hide_set(hide_num);
        }          
        break;
        default:
        break;
    }
}

// static void hl_obj_delete(lv_obj_t *obj,bool obj_typ)
// {
//     uint32_t child_cnt = 0,i;
//     sign_1 = 0xFF;
//     sign_2 = 0xFF;
//     child_cnt = lv_obj_get_child_cnt(obj);
//     if(child_cnt == 0){
//         lv_obj_add_flag(obj,LV_OBJ_FLAG_HIDDEN);
//         lv_obj_del_delayed(obj,0);
//     }else{
//         for(i=0;i<child_cnt;i++){
//             hl_obj_delete(lv_obj_get_child(obj, i),true);            
//         }
//         if(obj_typ){
//             lv_obj_add_flag(obj,LV_OBJ_FLAG_HIDDEN);
//             lv_obj_del_delayed(obj,0);
//         }        
//     }
// }

void lv_ptr_set_null(uint8_t typ)
{
    if(typ == 1){
        area_tx1 = RT_NULL;
        voice_bar_tx1 = RT_NULL;
        power_bar_tx1 = RT_NULL;
        voice_img_tx1 = RT_NULL;
        voice_mute_img_tx1 = RT_NULL;
        power_img_tx1 = RT_NULL;
        voice_lab_tx1 = RT_NULL;
        voice_lab_tx11 = RT_NULL;
        voice_bar_top_tx1 = RT_NULL;
        device_lab_tx1 = RT_NULL;
        video_dot_tx1 = RT_NULL;
        imgftx1 = RT_NULL;
        tx1_signal_obj[0] = RT_NULL;
        tx1_signal_obj[1] = RT_NULL;
        tx1_signal_obj[2] = RT_NULL;
        tx1_signal_obj[3] = RT_NULL;
        tx1_signal_obj[4] = RT_NULL;
    }else{
        area_tx2 = RT_NULL;
        voice_bar_tx2 = RT_NULL;
        power_bar_tx2 = RT_NULL;
        voice_img_tx2 = RT_NULL;
        voice_mute_img_tx2 = RT_NULL;
        power_img_tx2 = RT_NULL;
        voice_lab_tx2 = RT_NULL;
        voice_lab_tx22 = RT_NULL;
        voice_bar_top_tx2 = RT_NULL;
        device_lab_tx2 = RT_NULL;
        video_dot_tx2 = RT_NULL;
        imgftx2 = RT_NULL;
        tx2_signal_obj[0] = RT_NULL;
        tx2_signal_obj[1] = RT_NULL;
        tx2_signal_obj[2] = RT_NULL;
        tx2_signal_obj[3] = RT_NULL;
        tx2_signal_obj[4] = RT_NULL;
    }
}

static lv_obj_t * lv_img_creat_fun(lv_obj_t *align_obj,lv_align_t align,const void * src,lv_coord_t x_offset,lv_coord_t y_offset)
{
    lv_obj_t* img = lv_img_create(align_obj);
    lv_img_set_src(img, src);
    lv_obj_align(img, align, x_offset, y_offset);
    lv_img_set_zoom(img, 200);
    lv_obj_set_height(img,18);
    lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
    return img;
}

static void lv_signal_tx1_set(int16_t signal)
{    
    if(tx1_signal_obj[0] == RT_NULL){
        tx1_signal_obj[0] = lv_signal_img_creat_fun(area_tx1,device_lab_tx1,&Main_signal_0,8,-4,256,0);
    }else{
        LOG_E("tx1_signal_obj[0] is not null\n");
    }
    if(tx1_signal_obj[1] == RT_NULL){
        tx1_signal_obj[1] = lv_signal_img_creat_fun(area_tx1,device_lab_tx1,&Main_signal_1,8,-4,256,0);
    }else{
        LOG_E("tx1_signal_obj[1] is not null\n");
    }
    if(tx1_signal_obj[2] == RT_NULL){
        tx1_signal_obj[2] = lv_signal_img_creat_fun(area_tx1,device_lab_tx1,&Main_signal_2,8,-4,256,0);
    }else{
        LOG_E("tx1_signal_obj[2] is not null\n");
    }
    if(tx1_signal_obj[3] == RT_NULL){
        tx1_signal_obj[3] = lv_signal_img_creat_fun(area_tx1,device_lab_tx1,&Main_signal_3,8,-4,256,0);
    }else{
        LOG_E("tx1_signal_obj[3] is not null\n");
    }
    if(tx1_signal_obj[4] == RT_NULL){
        tx1_signal_obj[4] = lv_signal_img_creat_fun(area_tx1,device_lab_tx1,&Main_signal_4,8,-4,256,0);
    }else{
        LOG_E("tx1_signal_obj[4] is not null\n");
    }
    lv_signal_hide_set(1,signal);
}

static void lv_signal_tx2_set(int16_t signal)
{    
    if(tx2_signal_obj[0] == RT_NULL){
        tx2_signal_obj[0] = lv_signal_img_creat_fun(area_tx2,device_lab_tx2,&Main_signal_0,8,-4,256,0);
    }else{
        LOG_E("tx2_signal_obj[0] is not null\n");
    }
    if(tx2_signal_obj[1] == RT_NULL){
        tx2_signal_obj[1] = lv_signal_img_creat_fun(area_tx2,device_lab_tx2,&Main_signal_1,8,-4,256,0);
    }else{
        LOG_E("tx2_signal_obj[1] is not null\n");
    }
    if(tx2_signal_obj[2] == RT_NULL){
        tx2_signal_obj[2] = lv_signal_img_creat_fun(area_tx2,device_lab_tx2,&Main_signal_2,8,-4,256,0);
    }else{
        LOG_E("tx2_signal_obj[2] is not null\n");
    }
    if(tx2_signal_obj[3] == RT_NULL){
        tx2_signal_obj[3] = lv_signal_img_creat_fun(area_tx2,device_lab_tx2,&Main_signal_3,8,-4,256,0);
    }else{
        LOG_E("tx2_signal_obj[3] is not null\n");
    }
    if(tx2_signal_obj[4] == RT_NULL){
        tx2_signal_obj[4] = lv_signal_img_creat_fun(area_tx2,device_lab_tx2,&Main_signal_4,8,-4,256,0);
    }else{
        LOG_E("tx2_signal_obj[4] is not null\n");
    }
    
    lv_signal_hide_set(2,signal);
}

static void lv_display_tx1(device_data_t * init_data)
{
    if(area_tx1 == RT_NULL){
        area_tx1 = lv_area_creat_fun(LV_ALIGN_BOTTOM_LEFT,6,-6,282,94);
    }else{
        LOG_E("area_tx1 is not null\n");
    }
    if(voice_img_tx1 == RT_NULL){
        voice_img_tx1 = lv_voice_img_creat_fun(area_tx1,&Main_horn,-14,6,256);
    }else{
        LOG_E("voice_img_tx1 is not null\n");
    }
    if(voice_mute_img_tx1 == RT_NULL){
        voice_mute_img_tx1 = lv_voice_img_creat_fun(area_tx1,&Main_horn_mute,-14,6,256);
    }else{
        LOG_E("voice_mute_img_tx1 is not null\n");
    }
    if(power_img_tx1 == RT_NULL){
        power_img_tx1 = lv_power_img_creat_fun(area_tx1,7,-10,256);
    }else{
        LOG_E("power_img_tx1 is not null\n");
    }
    if(imgftx1 == RT_NULL){
        imgftx1 = lv_img_creat_fun(power_img_tx1, LV_ALIGN_CENTER, &Main_charging, -2, -2);
    }else{
        LOG_E("imgftx1 is not null\n");
    }
    if(voice_bar_tx1 == RT_NULL){
        voice_bar_tx1 = lv_voice_lbar_creat_fun(area_tx1,voice_img_tx1,4,0,251,14,init_data->volume);
    }else{
        LOG_E("voice_bar_tx1 is not null\n");
    }
    if(power_bar_tx1 == RT_NULL){
        power_bar_tx1 = lv_power_bar_creat_fun(power_img_tx1,3,0,25,14,init_data->electric);
    }else{
        LOG_E("power_bar_tx1 is not null\n");
    }
    if(voice_bar_top_tx1 == RT_NULL){
        voice_bar_top_tx1 = lv_voice_ltopbar_creat_fun(area_tx1,voice_img_tx1,4,0,251,14,init_data->volume);
    }else{
        LOG_E("voice_bar_top_tx1 is not null\n");
    }

    tx1_value_start = init_data->volume;
    lv_bar_anim_init(&animation_tx1,voice_bar_tx1,tx1_value_start,ANIMAtION_TIME_DOWN);
    lv_topbar_anim_init(&animation_top_tx1,voice_bar_top_tx1,tx1_value_start,ANIMAtION_TOP_TIME_DOWN);

    if(voice_lab_tx1 == RT_NULL){
        voice_lab_tx1 = lv_voice_lab_creat_fun(area_tx1,voice_bar_tx1,init_data->tx_gain,0,0);
    }else{
        LOG_E("voice_lab_tx1 is not null\n");
    }
    
    // power_lab_tx1 = lv_power_lab_creat_fun(area_tx1,power_img_tx1,power_bar_tx1,0,0);
    if(device_lab_tx1 == RT_NULL){
        device_lab_tx1 = lv_device_lab_creat_fun(area_tx1,-10,-5,"1");
    }else{
        LOG_E("device_lab_tx1 is not null\n");
    }
    if(voice_lab_tx11 == RT_NULL){
        voice_lab_tx11 = lv_voice_line_out_lab_creat_fun(area_tx1,voice_bar_tx1,init_data->line_out_value,0,-4);
    }else{
        LOG_E("voice_lab_tx11 is not null\n");
    }    

    lv_signal_tx1_set(init_data->signal);

    if(video_dot_tx1 == RT_NULL){
        video_dot_tx1 = lv_video_dot_creat_fun(area_tx1,22,2,init_data->record);
    }else{
        LOG_E("video_dot_tx1 is not null\n");
    }     
}

static void lv_display_tx2(device_data_t * init_data)
{
    if(area_tx2 == RT_NULL){
        area_tx2 = lv_area_creat_fun(LV_ALIGN_BOTTOM_LEFT,6,-6,282,94);
    }else{
        LOG_E("area_tx2 is not null\n");
    }  
    if(voice_img_tx2 == RT_NULL){
        voice_img_tx2 = lv_voice_img_creat_fun(area_tx2,&Main_horn,-14,6,256);
    }else{
        LOG_E("voice_img_tx2 is not null\n");
    }
    if(voice_mute_img_tx2 == RT_NULL){
        voice_mute_img_tx2 = lv_voice_img_creat_fun(area_tx2,&Main_horn_mute,-14,6,256);
    }else{
        LOG_E("voice_mute_img_tx2 is not null\n");
    }
    if(power_img_tx2 == RT_NULL){
        power_img_tx2 = lv_power_img_creat_fun(area_tx2,7,-10,256);
    }else{
        LOG_E("power_img_tx2 is not null\n");
    }
    if(imgftx2 == RT_NULL){
        imgftx2 = lv_img_creat_fun(power_img_tx2, LV_ALIGN_CENTER, &Main_charging, -2, -2);
    }else{
        LOG_E("imgftx2 is not null\n");
    }
    if(voice_bar_tx2 == RT_NULL){
        voice_bar_tx2 = lv_voice_lbar_creat_fun(area_tx2,voice_img_tx2,4,0,251,14,init_data->volume);
    }else{
        LOG_E("voice_bar_tx2 is not null\n");
    }
    if(power_bar_tx2 == RT_NULL){
        power_bar_tx2 = lv_power_bar_creat_fun(power_img_tx2,3,0,25,14,init_data->electric);
    }else{
        LOG_E("power_bar_tx2 is not null\n");
    }
    if(voice_bar_top_tx2 == RT_NULL){
        voice_bar_top_tx2 = lv_voice_ltopbar_creat_fun(area_tx2,voice_img_tx2,4,0,251,14,init_data->volume);
    }else{
        LOG_E("voice_bar_top_tx2 is not null\n");
    }

    tx2_value_start = init_data->volume;
    lv_bar_anim_init(&animation_tx2,voice_bar_tx2,tx2_value_start,ANIMAtION_TIME_DOWN);
    lv_topbar_anim_init(&animation_top_tx2,voice_bar_top_tx2,tx2_value_start,ANIMAtION_TOP_TIME_DOWN);

    if(voice_lab_tx2 == RT_NULL){
        voice_lab_tx2 = lv_voice_lab_creat_fun(area_tx2,voice_bar_tx2,init_data->tx_gain,0,0);
    }else{
        LOG_E("voice_lab_tx2 is not null\n");
    }
    
    // power_lab_tx2 = lv_power_lab_creat_fun(area_tx2,power_img_tx2,power_bar_tx2,0,0);
    if(device_lab_tx2 == RT_NULL){
        device_lab_tx2 = lv_device_lab_creat_fun(area_tx2,-10,-5,"2");
    }else{
        LOG_E("device_lab_tx2 is not null\n");
    }
    if(voice_lab_tx22 == RT_NULL){
        voice_lab_tx22 = lv_voice_line_out_lab_creat_fun(area_tx2,voice_bar_tx2,init_data->line_out_value,0,-4);
    }else{
        LOG_E("voice_lab_tx22 is not null\n");
    }
    lv_signal_tx2_set(init_data->signal);
    if(video_dot_tx2 == RT_NULL){
        video_dot_tx2 = lv_video_dot_creat_fun(area_tx2,22,2,init_data->record);
    }else{
        LOG_E("video_dot_tx2 is not null\n");
    }    
}

static void lv_display_double(device_data_t * init_tx1,device_data_t * init_tx2)
{
    //设备框
    if(area_tx1 == RT_NULL){
        area_tx1 = lv_area_creat_fun(LV_ALIGN_BOTTOM_LEFT,6,-6,138,94);
    }else{
        LOG_E("area_tx1 is not null\n");
    } 
    if(area_tx2 == RT_NULL){
        area_tx2 = lv_area_creat_fun(LV_ALIGN_BOTTOM_LEFT,150,-6,138,94);
    }else{
        LOG_E("area_tx2 is not null\n");
    }   
    
    //音量喇叭图片
    if(voice_img_tx1 == RT_NULL){
        voice_img_tx1 = lv_voice_img_creat_fun(area_tx1,&Main_horn,-14,6,256);
    }else{
        LOG_E("voice_img_tx1 is not null\n");
    } 
    if(voice_img_tx2 == RT_NULL){
        voice_img_tx2 = lv_voice_img_creat_fun(area_tx2,&Main_horn,-14,6,256);
    }else{
        LOG_E("voice_img_tx2 is not null\n");
    } 
    if(voice_mute_img_tx1 == RT_NULL){
        voice_mute_img_tx1 = lv_voice_img_creat_fun(area_tx1,&Main_horn_mute,-14,6,256);
    }else{
        LOG_E("voice_mute_img_tx1 is not null\n");
    } 
    if(voice_mute_img_tx2 == RT_NULL){
        voice_mute_img_tx2 = lv_voice_img_creat_fun(area_tx2,&Main_horn_mute,-14,6,256);
    }else{
        LOG_E("voice_mute_img_tx2 is not null\n");
    } 

    //电池图标图片
    if(power_img_tx1 == RT_NULL){
        power_img_tx1 = lv_power_img_creat_fun(area_tx1,7,-10,256);
    }else{
        LOG_E("power_img_tx1 is not null\n");
    } 
    if(power_img_tx2 == RT_NULL){
        power_img_tx2 = lv_power_img_creat_fun(area_tx2,7,-10,256);
    }else{
        LOG_E("power_img_tx2 is not null\n");
    } 
    if(imgftx1 == RT_NULL){
        imgftx1 = lv_img_creat_fun(power_img_tx1, LV_ALIGN_CENTER, &Main_charging, -2, -2);
    }else{
        LOG_E("imgftx1 is not null\n");
    } 
    if(imgftx2 == RT_NULL){
        imgftx2 = lv_img_creat_fun(power_img_tx2, LV_ALIGN_CENTER, &Main_charging, -2, -2);
    }else{
        LOG_E("imgftx2 is not null\n");
    }   

    //音量大小进度条
    if(voice_bar_tx1 == RT_NULL){
        voice_bar_tx1 = lv_voice_sbar_creat_fun(area_tx1,voice_img_tx1,4,0,105,20,init_tx1->volume);
    }else{
        LOG_E("voice_bar_tx1 is not null\n");
    } 
    if(voice_bar_tx2 == RT_NULL){
        voice_bar_tx2 = lv_voice_sbar_creat_fun(area_tx2,voice_img_tx2,4,0,105,20,init_tx2->volume);
    }else{
        LOG_E("voice_bar_tx2 is not null\n");
    } 
    if(voice_bar_top_tx1 == RT_NULL){
        voice_bar_top_tx1 = lv_voice_stopbar_creat_fun(area_tx1,voice_img_tx1,4,0,105,20,init_tx1->volume);
    }else{
        LOG_E("voice_bar_top_tx1 is not null\n");
    } 
    if(voice_bar_top_tx2 == RT_NULL){
        voice_bar_top_tx2 = lv_voice_stopbar_creat_fun(area_tx2,voice_img_tx2,4,0,105,20,init_tx2->volume);
    }else{
        LOG_E("voice_bar_top_tx2 is not null\n");
    } 

    tx1_value_start = init_tx1->volume;
    tx2_value_start = init_tx2->volume;
    lv_bar_anim_init(&animation_tx1,voice_bar_tx1,tx1_value_start,ANIMAtION_TIME_DOWN);
    lv_bar_anim_init(&animation_tx2,voice_bar_tx2,tx2_value_start,ANIMAtION_TIME_DOWN);

    lv_topbar_anim_init(&animation_top_tx1,voice_bar_top_tx1,tx1_value_start,ANIMAtION_TOP_TIME_DOWN);
    lv_topbar_anim_init(&animation_top_tx2,voice_bar_top_tx2,tx2_value_start,ANIMAtION_TOP_TIME_DOWN);


    //电池电量进度条
    if(power_bar_tx1 == RT_NULL){
        power_bar_tx1 = lv_power_bar_creat_fun(power_img_tx1,3,0,25,14,init_tx1->electric);
    }else{
        LOG_E("power_bar_tx1 is not null\n");
    } 
    if(power_bar_tx2 == RT_NULL){
        power_bar_tx2 = lv_power_bar_creat_fun(power_img_tx2,3,0,25,14,init_tx2->electric);
    }else{
        LOG_E("power_bar_tx2 is not null\n");
    } 

    //音量大小文本
    if(voice_lab_tx1 == RT_NULL){
        voice_lab_tx1 = lv_voice_lab_creat_fun(area_tx1,voice_bar_tx1,init_tx1->tx_gain,0,0);
    }else{
        LOG_E("voice_lab_tx1 is not null\n");
    }
    if(voice_lab_tx2 == RT_NULL){
        voice_lab_tx2 = lv_voice_lab_creat_fun(area_tx2,voice_bar_tx2,init_tx2->tx_gain,0,0);   
    }else{
        LOG_E("voice_lab_tx2 is not null\n");
    }
        
    //电池电量大小文本
    // power_lab_tx1 = lv_power_lab_creat_fun(area_tx1,power_img_tx1,power_bar_tx1,0,0);
    // power_lab_tx2 = lv_power_lab_creat_fun(area_tx2,power_img_tx2,power_bar_tx2,0,0); 
    //设备文本
    if(device_lab_tx1 == RT_NULL){
        device_lab_tx1 = lv_device_lab_creat_fun(area_tx1,-10,-5,"1");
    }else{
        LOG_E("device_lab_tx1 is not null\n");
    }
    if(device_lab_tx2 == RT_NULL){
        device_lab_tx2 = lv_device_lab_creat_fun(area_tx2,-10,-5,"2");
    }else{
        LOG_E("device_lab_tx2 is not null\n");
    }

    //信号图标
    lv_signal_tx1_set(init_tx1->signal);
    lv_signal_tx2_set(init_tx2->signal);

    //录制状态红点
    if(video_dot_tx1 == RT_NULL){
        video_dot_tx1 = lv_video_dot_creat_fun(area_tx1,22,2,init_tx1->record);
    }else{
        LOG_E("video_dot_tx1 is not null\n");
    }
    if(video_dot_tx2 == RT_NULL){
        video_dot_tx2 = lv_video_dot_creat_fun(area_tx2,22,2,init_tx2->record);
    }else{
        LOG_E("video_dot_tx2 is not null\n");
    }
}

static void lv_delete_style(void)
{
    lv_style_reset(&style_area_main);
    lv_style_reset(&style_voice_sbar_indicator);
    lv_style_reset(&style_voice_bar_main);
    lv_style_reset(&style_voice_lbar_indicator);
    lv_style_reset(&style_voice_bar_main);
    lv_style_reset(&style_power_bar_indicator);
    lv_style_reset(&style_power_bar_main);
    lv_style_reset(&style_voice_label);
    lv_style_reset(&style_power_label);
    lv_style_reset(&style_device_label);
    lv_style_reset(&style_video_dot);
    lv_style_reset(&style_power_bar_white_indicator);
    lv_style_reset(&style_power_bar_red_indicator);
    lv_style_reset(&style_power_bar_green_indicator);
}

static void lv_display_mod_change(hl_lvgl_main_init_t * ctl_data)
{
    if(main_init.display_tx_device == ctl_data->display_tx_device){
        // rt_kprintf(" dataf%d \n",ctl_data->display_tx_device);
        // rt_kprintf(" adam%d \n",main_init.display_tx_device);
        return;
    }
    // rt_kprintf(" datam \n");
    switch(main_init.display_tx_device){
        case HL_CHANGE_TX1_DEVICE:
            // hl_obj_delete(area_tx1,true);
            if(area_tx1 == RT_NULL){
                LOG_E("area_tx1 is null\n");
            }
            lv_obj_del(area_tx1);
            rt_thread_mdelay(10); 
            lv_ptr_set_null(1);
            break;
        case HL_CHANGE_TX2_DEVICE:
            // hl_obj_delete(area_tx2,true);
            if(area_tx2 == RT_NULL){
                LOG_E("area_tx2 is null\n");
            }
            lv_obj_del(area_tx2);
            rt_thread_mdelay(10); 
            lv_ptr_set_null(2);
            break;
        case HL_DISPLAY_DOUBLE:
            // hl_obj_delete(area_tx1,true);
            // hl_obj_delete(area_tx2,true);
            if(area_tx1 == RT_NULL){
                LOG_E("area_tx1 is null\n");
            }
            if(area_tx2 == RT_NULL){
                LOG_E("area_tx2 is null\n");
            }
            lv_obj_del(area_tx1);
            lv_obj_del(area_tx2);
            rt_thread_mdelay(10); 
            lv_ptr_set_null(1);
            lv_ptr_set_null(2);
            break;
        default:
            break;
    }    
    switch(ctl_data->display_tx_device){
        case HL_CHANGE_TX1_DEVICE:
            lv_display_tx1(&ctl_data->tx_device_1);
            break;
        case HL_CHANGE_TX2_DEVICE:
            lv_display_tx2(&ctl_data->tx_device_2);
            break;
        case HL_DISPLAY_DOUBLE:
            lv_display_double(&ctl_data->tx_device_1,&ctl_data->tx_device_2);
            break;
        default:
            break;
    }  
    main_init.display_tx_device = ctl_data->display_tx_device;
}


void hl_mod_main_ioctl(void * ctl_data)
{
    char buf[8] = {0,0,0,0,0,0,0,0};
    hl_lvgl_main_ioctl_t * ptr =  (hl_lvgl_main_ioctl_t *)ctl_data;
    switch(ptr->cmd){
        case HL_CHANGE_TX1_DEVICE:
        case HL_CHANGE_TX2_DEVICE:
        case HL_CHANGE_DOUBLE_DEVICE:
            lv_display_mod_change(ctl_data);
            break;


        case HL_CHANGE_TX1_SIGNAL:
            lv_signal_hide_set(1,ptr->tx_device_1.signal);
            main_init.tx_device_1.signal = ptr->tx_device_1.signal;
            break;
        case HL_CHANGE_TX1_ELEC:
            lv_bar_set_value(power_bar_tx1, ptr->tx_device_1.electric, LV_ANIM_ON);
            // lv_snprintf(buf, sizeof(buf), "%d%%", ptr->tx_device_1.electric);
            // lv_label_set_text(power_lab_tx1,buf);
            main_init.tx_device_1.electric = ptr->tx_device_1.electric;
            break;
        case HL_CHANGE_TX1_VOL:           
            if(ptr->tx_device_1.volume > tx1_value_start){
                lv_bar_anim_ctl(&animation_tx1,tx1_value_start,ptr->tx_device_1.volume,ANIMAtION_TIME_UP);                
            }else{
                lv_bar_anim_ctl(&animation_tx1,tx1_value_start,ptr->tx_device_1.volume,ANIMAtION_TIME_DOWN);
                lv_topbar_anim_ctl(&animation_top_tx1,tx1_value_start,ptr->tx_device_1.volume,VOICE_TOP_BAR_DELAY);
            } 
            // lv_snprintf(buf, sizeof(buf), "%d", ptr->tx_device_1.volume);
            // lv_label_set_text(voice_lab_tx1,buf);
            tx1_value_start = ptr->tx_device_1.volume;
            main_init.tx_device_1.volume = ptr->tx_device_1.volume; 
            break;
        case HL_CHANGE_TX1_REC:
            lv_set_vodeo_dot_status_cb(video_dot_tx1,ptr->tx_device_1.record);
            main_init.tx_device_1.record = ptr->tx_device_1.record; 
            break;
        case HL_CHANGE_TX1_BAR_RED:
            lv_obj_remove_style(power_bar_tx1,&style_power_bar_red_indicator,LV_PART_INDICATOR);
            lv_obj_add_style(power_bar_tx1,&style_power_bar_red_indicator,LV_PART_INDICATOR);
            break;
        case HL_CHANGE_TX1_BAR_WHITE:
            lv_obj_remove_style(power_bar_tx1,&style_power_bar_white_indicator,LV_PART_INDICATOR);
            lv_obj_add_style(power_bar_tx1,&style_power_bar_white_indicator,LV_PART_INDICATOR);
            break;
        case HL_CHANGE_TX1_MUTE:
            // if (voice_img_tx1 != RT_NULL){
                lv_set_video_hide_cb(voice_mute_img_tx1,0);
                lv_set_video_hide_cb(voice_img_tx1,1);
                lv_obj_move_foreground(voice_mute_img_tx1);
            // }
                // lv_img_set_src(voice_img_tx1, &Main_horn_mute);
            break;
        case HL_CHANGE_TX1_MUTE_DEL:
            // if (voice_img_tx1 != RT_NULL){
                lv_set_video_hide_cb(voice_mute_img_tx1,1);
                lv_set_video_hide_cb(voice_img_tx1,0);
                lv_obj_move_foreground(voice_img_tx1);
            // }
                // lv_img_set_src(voice_img_tx1, &Main_horn);
            break;
        case HL_CHANGE_TX2_SIGNAL:
            lv_signal_hide_set(2,ptr->tx_device_2.signal);
            main_init.tx_device_2.signal = ptr->tx_device_2.signal;
            break;
        case HL_CHANGE_TX2_ELEC:
            lv_bar_set_value(power_bar_tx2, ptr->tx_device_2.electric, LV_ANIM_ON);
            // lv_snprintf(buf, sizeof(buf), "%d%%", ptr->tx_device_2.electric);
            // lv_label_set_text(power_lab_tx2,buf);
            main_init.tx_device_2.electric = ptr->tx_device_2.electric;
            break;
        case HL_CHANGE_TX2_VOL:
            if(ptr->tx_device_2.volume > tx2_value_start){
                lv_bar_anim_ctl(&animation_tx2,tx2_value_start,ptr->tx_device_2.volume,ANIMAtION_TIME_UP);                
            }else{
                lv_bar_anim_ctl(&animation_tx2,tx2_value_start,ptr->tx_device_2.volume,ANIMAtION_TIME_DOWN);
                lv_topbar_anim_ctl(&animation_top_tx2,tx2_value_start,ptr->tx_device_2.volume,VOICE_TOP_BAR_DELAY);
            } 
            // lv_snprintf(buf, sizeof(buf), "%d", ptr->tx_device_2.volume);
            // lv_label_set_text(voice_lab_tx2,buf);
            tx2_value_start = ptr->tx_device_2.volume;
            main_init.tx_device_2.volume = ptr->tx_device_2.volume; 
            break;
        case HL_CHANGE_TX2_REC:
            lv_set_vodeo_dot_status_cb(video_dot_tx2,ptr->tx_device_2.record);
            main_init.tx_device_2.record = ptr->tx_device_2.record; 
            break;
        case HL_CHANGE_TX2_BAR_RED:
            lv_obj_remove_style(power_bar_tx2,&style_power_bar_red_indicator,LV_PART_INDICATOR);
            lv_obj_add_style(power_bar_tx2,&style_power_bar_red_indicator,LV_PART_INDICATOR);
            break;
        case HL_CHANGE_TX2_BAR_WHITE:
            lv_obj_remove_style(power_bar_tx2,&style_power_bar_white_indicator,LV_PART_INDICATOR);
            lv_obj_add_style(power_bar_tx2,&style_power_bar_white_indicator,LV_PART_INDICATOR);
            break;
        case HL_CHANGE_TX2_MUTE:
            // if (voice_img_tx2 != RT_NULL){
                lv_set_video_hide_cb(voice_mute_img_tx2,0);
                lv_set_video_hide_cb(voice_img_tx2,1);
                lv_obj_move_foreground(voice_mute_img_tx2);
            // }
                // lv_img_set_src(voice_img_tx2, &Main_horn_mute);
            break;
        case HL_CHANGE_TX2_MUTE_DEL:
            // if (voice_img_tx2 != RT_NULL){
                lv_set_video_hide_cb(voice_mute_img_tx2,1);
                lv_set_video_hide_cb(voice_img_tx2,0);
                lv_obj_move_foreground(voice_img_tx2);
            // }
                // lv_img_set_src(voice_img_tx2, &Main_horn);
            break;

        case HL_CHANGE_DELETE_TX1:
            // hl_obj_delete(area_tx1,true);
            if(area_tx1 == RT_NULL){
                LOG_E("area_tx1 is null\n");
            }
            lv_obj_del(area_tx1);
            rt_thread_mdelay(10); 
            lv_ptr_set_null(1);
        break;
        case HL_CHANGE_DELETE_DOUBLE:
            // hl_obj_delete(area_tx1,true);
            // hl_obj_delete(area_tx2,true);
            if(area_tx1 == RT_NULL){
                LOG_E("area_tx1 is null\n");
            }
            if(area_tx2 == RT_NULL){
                LOG_E("area_tx2 is null\n");
            }
            lv_obj_del(area_tx1);
            lv_obj_del(area_tx2);
            rt_thread_mdelay(10); 
            lv_ptr_set_null(1);
            lv_ptr_set_null(2);
        break;
        case HL_CHANGE_DELETE_TX2:
            // hl_obj_delete(area_tx2,true);
            if(area_tx2 == RT_NULL){
                LOG_E("area_tx2 is null\n");
            }
            lv_obj_del(area_tx2);
            rt_thread_mdelay(10); 
            lv_ptr_set_null(2);
            break;
        case HL_CHANGE_DELETE_STYLE:
            lv_delete_style();
            break;
        case HL_CHANGE_TX1_BAR_GREEN:
            lv_obj_remove_style(power_bar_tx1, &style_power_bar_white_indicator, LV_PART_INDICATOR);
            lv_obj_add_style(power_bar_tx1, &style_power_bar_green_indicator, LV_PART_INDICATOR);
            break;
        case HL_CHANGE_TX2_BAR_GREEN:
            lv_obj_remove_style(power_bar_tx2, &style_power_bar_white_indicator, LV_PART_INDICATOR);
            lv_obj_add_style(power_bar_tx2, &style_power_bar_green_indicator, LV_PART_INDICATOR);
            break;
            /// @brief 隐藏TX1设备充电闪电图标
        case HL_CHANGE_TX1_ICON_HIDE:
            lv_obj_add_flag(imgftx1, LV_OBJ_FLAG_HIDDEN);
            break;
        case HL_CHANGE_TX2_ICON_HIDE:
            lv_obj_add_flag(imgftx2, LV_OBJ_FLAG_HIDDEN);
            break;
        case HL_CHANGE_TX1_ICON_DISHIDE:
            lv_obj_clear_flag(imgftx1, LV_OBJ_FLAG_HIDDEN);
            break;
        case HL_CHANGE_TX2_ICON_DISHIDE:
            lv_obj_clear_flag(imgftx2, LV_OBJ_FLAG_HIDDEN);
            break;
        default:
            break;
    }
}

void hl_mod_main_init(void * init_data)
{
    hl_lvgl_main_init_t * ptr = (hl_lvgl_main_init_t *)init_data;
    memcpy(&main_init,ptr,sizeof(hl_lvgl_main_init_t));
    if (!page_style_bit.page_main) {
        lv_style_page1_init();
        page_style_bit.page_main = 1;
    }
    sign_1 = 0xFF;
    sign_2 = 0xFF;
    tx1_value_max = 0;
    tx2_value_max = 0;
    if(ptr->display_tx_device == HL_DISPLAY_TX1){
        lv_display_tx1(&ptr->tx_device_1);
    }
    if(ptr->display_tx_device == HL_DISPLAY_TX2){
        lv_display_tx2(&ptr->tx_device_2);
    }
    if(ptr->display_tx_device == HL_DISPLAY_DOUBLE){
        lv_display_double(&ptr->tx_device_1,&ptr->tx_device_2);
    } 
}
