/**
 * @file page_recodeprotect.c
 * @author your name (you@domain.com)
 * @brief 录制保护设置界面
 * @version 0.1
 * @date 2022-12-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "page_recodeprotect.h"
// #include "language.h"

#define MAX_OBJ_NUMBER     6

static lv_obj_t *valid_obj[MAX_OBJ_NUMBER] = {NULL, NULL, NULL, NULL, NULL, NULL};

static char * lock = "btn";

static lv_style_t style_choose_main,style_not_choose_main;
static lv_style_t style_choose_lr_main,style_not_choose_lr_main;
static lv_style_t style_label;

static lv_obj_t * btn_l,*btn_r;
static lv_obj_t * btn1,*btn2;
static lv_obj_t * lab1,*lab2,*lab3;

static void lv_style_page4_init(void)
{
    lv_obj_enable_style_refresh(true);
    lv_style_init(&style_choose_main);
    lv_style_set_bg_opa(&style_choose_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_choose_main, lv_palette_main(LV_PALETTE_ORANGE));
    lv_style_set_border_width(&style_choose_main,0);
    lv_style_set_outline_width(&style_choose_main,0);
    lv_style_set_radius(&style_choose_main, 0);

    lv_style_init(&style_not_choose_main);
    lv_style_set_bg_opa(&style_not_choose_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_not_choose_main, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_bg_color(&style_not_choose_main, lv_palette_lighten(LV_PALETTE_GREY,1));
    lv_style_set_border_width(&style_not_choose_main,0);
    lv_style_set_outline_width(&style_not_choose_main,0);
    lv_style_set_radius(&style_not_choose_main, 0);    

    lv_style_init(&style_choose_lr_main);
    lv_style_set_bg_opa(&style_choose_lr_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_choose_lr_main, lv_palette_main(LV_PALETTE_ORANGE));
    lv_style_set_border_width(&style_choose_lr_main,0);
    lv_style_set_outline_width(&style_choose_lr_main,0);
    lv_style_set_radius(&style_choose_lr_main, 10);

    lv_style_init(&style_not_choose_lr_main);
    lv_style_set_bg_opa(&style_not_choose_lr_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_not_choose_lr_main, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_bg_color(&style_not_choose_lr_main, lv_palette_lighten(LV_PALETTE_GREY,1));
    lv_style_set_border_width(&style_not_choose_lr_main,0);
    lv_style_set_outline_width(&style_not_choose_lr_main,0);
    lv_style_set_radius(&style_not_choose_lr_main, 10);

    lv_style_init(&style_label);
    lv_style_set_bg_opa(&style_label, LV_OPA_TRANSP);
    lv_style_set_text_opa(&style_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_label, lv_color_white());
}

static lv_obj_t * lv_btn_lr_creat_fun(lv_obj_t *align_obj,lv_align_t align,lv_coord_t x_offset,lv_coord_t y_offset,\
                                        lv_coord_t width, lv_coord_t high,uint8_t status)
{
    lv_obj_t * btn;
    btn = lv_obj_create(lv_scr_act());
    if(status){
        lv_obj_add_style(btn,&style_choose_lr_main,LV_PART_MAIN);
    }else{
        lv_obj_add_style(btn,&style_not_choose_lr_main,LV_PART_MAIN);
    }
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_move_background(btn);
    lv_obj_set_size(btn,width,high);    
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_align_to(btn, align_obj,align, x_offset, y_offset);
    return btn;
}

static lv_obj_t * lv_btn_creat_fun(lv_obj_t *align_obj,lv_align_t align,lv_event_cb_t event_cb,\
                                    lv_coord_t x_offset,lv_coord_t y_offset,lv_coord_t width, lv_coord_t high,uint8_t status)
{
    lv_obj_t * btn;
    btn = lv_obj_create(lv_scr_act());
    if(status){
        lv_obj_add_style(btn,&style_choose_main,LV_PART_MAIN);
    }else{
        lv_obj_add_style(btn,&style_not_choose_main,LV_PART_MAIN);
    }
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(btn,width,high);    
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICK_FOCUSABLE);    
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_align_to(btn, align_obj,align, x_offset, y_offset);
    return btn;
}

static lv_obj_t * lv_lab_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,lv_align_t align,lv_coord_t x_offset,lv_coord_t y_offset,const char * ptr)
{
    lv_obj_t * lab = lv_label_create(src_obj);
    lv_obj_add_style(lab, &style_label, LV_PART_MAIN);
    // lv_obj_set_style_text_font(lab, &language, 0);
    lv_label_set_text(lab,ptr);
    lv_obj_align_to(lab,align_obj,align,x_offset,y_offset);
    return lab;
}

static void bt1_cb(lv_event_t * e)
{
    char * ptr = (char *)lv_event_get_param(e);
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {        
        if(strcmp(ptr,lock)){
            LV_LOG_USER("btn1 Clicked\n");
            lv_obj_remove_style(btn1,&style_choose_main,LV_PART_MAIN);
            lv_obj_remove_style(btn_l,&style_choose_lr_main,LV_PART_MAIN);
            lv_obj_add_style(btn1,&style_choose_main,LV_PART_MAIN);
            lv_obj_add_style(btn_l,&style_choose_lr_main,LV_PART_MAIN);            
            lv_event_send(btn2,LV_EVENT_CLICKED,lock);
        }else{
            lv_obj_remove_style(btn1,&style_not_choose_main,LV_PART_MAIN);
            lv_obj_remove_style(btn_l,&style_not_choose_lr_main,LV_PART_MAIN);
            lv_obj_add_style(btn1,&style_not_choose_main,LV_PART_MAIN);
            lv_obj_add_style(btn_l,&style_not_choose_lr_main,LV_PART_MAIN);            
        }
    }
}

static void bt2_cb(lv_event_t * e)
{
    char * ptr = (char *)lv_event_get_param(e);
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        if(strcmp(ptr,lock)){
            LV_LOG_USER("btn2 Clicked\n");
            lv_obj_remove_style(btn2,&style_choose_main,LV_PART_MAIN);
            lv_obj_remove_style(btn_r,&style_choose_lr_main,LV_PART_MAIN);
            lv_obj_add_style(btn2,&style_choose_main,LV_PART_MAIN);
            lv_obj_add_style(btn_r,&style_choose_lr_main,LV_PART_MAIN);            
            lv_event_send(btn1,LV_EVENT_CLICKED,lock);
        }else{
            lv_obj_remove_style(btn2,&style_not_choose_main,LV_PART_MAIN);
            lv_obj_remove_style(btn_r,&style_not_choose_lr_main,LV_PART_MAIN);
            lv_obj_add_style(btn2,&style_not_choose_main,LV_PART_MAIN);
            lv_obj_add_style(btn_r,&style_not_choose_lr_main,LV_PART_MAIN);            
        }
    }
}

static void lv_page_4_init(void)
{
    // static lv_style_t style;
    // lv_style_init(&style);
    // lv_style_set_bg_color(&style, lv_color_black());
    // lv_style_set_border_width(&style, 0);
    //lv_obj_add_style(lv_scr_act(), &style, 0);
    btn_l = lv_btn_lr_creat_fun(lv_scr_act(),LV_ALIGN_LEFT_MID,40,5,15,54,0);
    btn1 = lv_btn_creat_fun(btn_l,LV_ALIGN_OUT_RIGHT_MID,bt1_cb,-10,0,80,54,0);
    btn2 = lv_btn_creat_fun(btn1,LV_ALIGN_OUT_RIGHT_MID,bt2_cb,0,0,80,54,1);
    btn_r = lv_btn_lr_creat_fun(btn2,LV_ALIGN_OUT_RIGHT_MID,-10,0,15,54,1);

    lab1 = lv_lab_creat_fun(btn1,btn1,LV_ALIGN_CENTER,0,0,"ON");
    lab2 = lv_lab_creat_fun(btn2,btn2,LV_ALIGN_CENTER,0,0,"OFF");
    lab3 = lv_lab_creat_fun(lv_scr_act(),lv_scr_act(),LV_ALIGN_TOP_MID,0,15,"录制保护");

    valid_obj[0] = btn1;
    valid_obj[1] = btn2;
}

lv_obj_t * hl_recodeprotect_obj_get(uint8_t num)
{
    return valid_obj[num];
}

//测试接口
void page_recodeprotect_test(void)
{
    // lv_style_page4_init();
    // lv_page_4_init();
}