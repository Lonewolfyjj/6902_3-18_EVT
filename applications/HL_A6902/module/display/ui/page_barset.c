/**
 * @file page_barset.c
 * @author your name (you@domain.com)
 * @brief 进度条UI实现函数
 * @version 0.1
 * @date 2022-12-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "page_barset.h"
#include "language.h"

LV_IMG_DECLARE(Main_stereo);//立体声

static lv_style_t style_indicator;
static lv_style_t style_back;
static lv_style_t style_label;

static lv_obj_t * slider1;
static lv_obj_t * img1,*img2;
static lv_obj_t * lab1,*lab2;

static hl_barset_event_cb hl_barset_func;

static void slider_event_cb1(lv_event_t * e)
{
    char buf[8];
    lv_obj_t * slider = lv_event_get_target(e); 
    int32_t value = lv_slider_get_value(slider);    
    lv_snprintf(buf, sizeof(buf), "%d", value);
    lv_label_set_text(lab2, buf);
    hl_barset_func(value);
}

static lv_obj_t * lv_img_creat_fun(lv_obj_t *src_obj,const void * src,lv_align_t align,lv_coord_t x_offset,lv_coord_t y_offset)
{
    lv_obj_t * img = lv_img_create(src_obj);
    lv_img_set_src(img, src);
    lv_obj_align(img, align, x_offset, y_offset);
    return img;
}

static lv_obj_t * lv_slider_creat_fun(lv_obj_t *src_obj,lv_align_t align,lv_coord_t x_offset,lv_coord_t y_offset,int min,int max,\
                                        int init_data,lv_event_cb_t event_cb)
{
    lv_obj_t * slider = lv_slider_create(src_obj);    
    lv_obj_remove_style_all(slider);        /*Remove the styles coming from the theme*/
    lv_obj_set_size(slider,270,32);
    lv_obj_add_style(slider, &style_back, LV_PART_MAIN);
    lv_obj_add_style(slider, &style_indicator, LV_PART_INDICATOR);
    lv_obj_add_event_cb(slider, event_cb, LV_EVENT_VALUE_CHANGED, NULL);//添加回调
    lv_slider_set_range(slider, min, max);
    lv_slider_set_value(slider, init_data, LV_ANIM_ON);
    lv_obj_align(slider, align, x_offset, y_offset);
    return slider;
}

static lv_obj_t * lv_lab_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,lv_align_t align,lv_coord_t x_offset,lv_coord_t y_offset,const char * ptr)
{
    lv_obj_t * lab = lv_label_create(src_obj);
    lv_obj_add_style(lab,&style_label, LV_PART_MAIN);
    lv_obj_set_style_text_font(lab, &language, 0);
    lv_label_set_text(lab, ptr);
    
    lv_obj_align_to(lab, align_obj, align, x_offset, y_offset);
    return lab;
}

static void lv_style_barset_init(void)
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_color_black());
    lv_style_set_border_width(&style, 0);
    //lv_obj_add_style(lv_scr_act(), &style, 0);

    lv_style_init(&style_back);
    lv_style_set_bg_opa(&style_back, LV_OPA_COVER);
    lv_style_set_bg_color(&style_back, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_radius(&style_back, 3);

    lv_style_init(&style_indicator);
    lv_style_set_bg_opa(&style_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indicator, lv_palette_main(LV_PALETTE_ORANGE));
    lv_style_set_radius(&style_indicator, 3);

    lv_style_init(&style_label);
    lv_style_set_bg_opa(&style_label, LV_OPA_TRANSP);
    lv_style_set_text_opa(&style_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_label, lv_color_white());
}

// static void hl_mod_barset_init(const void * src,const char * ptr1,int min,int max,int init_value,uint8_t top_icon)
// {
//     char buf[8] = {0,0,0,0,0,0,0,0};
//     lv_style_barset_init();
//     lv_snprintf(buf, sizeof(buf), "%d", init_value);
//     slider1 = lv_slider_creat_fun(lv_scr_act(),LV_ALIGN_CENTER,0,0,min,max,init_value,slider_event_cb1);
//     img1 = lv_img_creat_fun(slider1,src,LV_ALIGN_LEFT_MID,10,0);
//     lab1 = lv_lab_creat_fun(lv_scr_act(),slider1,LV_ALIGN_OUT_TOP_LEFT,0,-4,ptr1);
//     lab2 = lv_lab_creat_fun(lv_scr_act(),slider1,LV_ALIGN_OUT_TOP_RIGHT,0,-4,buf);

//     if(top_icon){
//         img2 = lv_img_creat_fun(lv_scr_act(),&Main_stereo,LV_ALIGN_TOP_LEFT,6,5);
//     }
// }

static void hl_obj_delete(lv_obj_t *obj,bool obj_typ)
{
    uint32_t child_cnt = 0,i;
    child_cnt = lv_obj_get_child_cnt(obj);
    if(child_cnt == 0){
        lv_obj_del_delayed(obj,0);
    }else{
        for(i=0;i<child_cnt;i++){
            hl_obj_delete(lv_obj_get_child(obj, i),true);            
        }
        if(obj_typ){
            lv_obj_del_delayed(obj,0);
        }        
    }
}

static void lv_delete_style(void)
{
    lv_style_reset(&style_indicator);
    lv_style_reset(&style_back);
    lv_style_reset(&style_label);
}

void hl_mod_barset_ioctl(void * ctl_data)
{
    char buf[8] = {0,0,0,0,0,0,0,0};
    hl_lvgl_barset_ioctl_t * ptr = (hl_lvgl_barset_ioctl_t *)ctl_data;
    if(ptr->barset_value == HL_VALUE){
        lv_snprintf(buf, sizeof(buf), "%d", ptr->barset_value);
        lv_slider_set_value(slider1, ptr->barset_value, LV_ANIM_ON);
        lv_label_set_text(lab2, ptr);
    }
    if(ptr->barset_value == HL_EXTI){
        hl_obj_delete(lv_scr_act(),false);        
    }   
    if(ptr->barset_value == HL_DELETE_STYLE){
        lv_delete_style();        
    }   
}

void hl_mod_barset_init(void * init_data)
{
    char buf[8] = {0,0,0,0,0,0,0,0};
    hl_lvgl_barset_init_t * ptr = (hl_lvgl_barset_init_t *)init_data;
    hl_barset_func = ptr->func_cb;

    lv_style_barset_init();
    lv_snprintf(buf, sizeof(buf), "%d", ptr->init_value);
    slider1 = lv_slider_creat_fun(lv_scr_act(),LV_ALIGN_CENTER,0,0,ptr->range_min,ptr->range_max,ptr->init_value,slider_event_cb1);
    img1 = lv_img_creat_fun(slider1,ptr->src,LV_ALIGN_LEFT_MID,10,0);
    lab1 = lv_lab_creat_fun(lv_scr_act(),slider1,LV_ALIGN_OUT_TOP_LEFT,0,-4,ptr->ptr);
    lab2 = lv_lab_creat_fun(lv_scr_act(),slider1,LV_ALIGN_OUT_TOP_RIGHT,0,-4,buf);

    switch(ptr->icontyp){
        case HL_NO_ICON:
            break;
        case HL_STEREO_ICON:
            img2 = lv_img_creat_fun(lv_scr_act(),&Main_stereo,LV_ALIGN_TOP_LEFT,6,5);
            break;
        case HL_SAYFT_ICON:
            img2 = lv_img_creat_fun(lv_scr_act(),&Main_stereo,LV_ALIGN_TOP_LEFT,6,5);
            break;
        case HL_SINGLE_ICON:
            img2 = lv_img_creat_fun(lv_scr_act(),&Main_stereo,LV_ALIGN_TOP_LEFT,6,5);
            break;
        default:
            break;
    }
}

lv_obj_t * hl_bar_obj_get(uint8_t num)
{
    return slider1;
}
