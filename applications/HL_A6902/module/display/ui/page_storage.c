/**
 * @file page_storage.c
 * @author your name (you@domain.com)
 * @brief 存储格式化
 * @version 0.1
 * @date 2022-12-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "page_storage.h"
// #include "language.h"
#include "page_style_bit.h"

#define  CHICK_STA_LIFT 0
#define  CHICK_STA_RIGHT 1

#define MAX_OBJ_NUMBER     6

static lv_obj_t *valid_obj[MAX_OBJ_NUMBER] = {NULL, NULL, NULL, NULL, NULL, NULL};
static uint8_t btn_left_cnt = 0,btn_right_cnt = 0;

static char * lock = "btn";
static char * ulock = "ubtn";

static lv_style_t style_choose_main,style_not_choose_main;
static lv_style_t style_choose_lr_main,style_not_choose_lr_main;
static lv_style_t style_label,style_back;
static lv_style_t style_indicator,style_main;

static lv_obj_t * con1,*con2;
static lv_obj_t * bar1,*bar2;
static lv_obj_t * btn_left,*btn_right;
static lv_obj_t * lab11,*lab12,*lab13;
static lv_obj_t * lab21,*lab22,*lab23;

static hl_storage_event_cb hl_storage_func;

static void lv_style_page4_init(void)
{
    lv_obj_enable_style_refresh(true);

    // static lv_style_t style;
    // lv_style_init(&style);
    // lv_style_set_bg_color(&style, lv_color_black());
    // lv_style_set_border_width(&style, 0);
    //lv_obj_add_style(lv_scr_act(), &style, 0);

    lv_style_init(&style_choose_main);
    lv_style_set_bg_opa(&style_choose_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_choose_main, lv_palette_main(LV_PALETTE_ORANGE));
    lv_style_set_border_width(&style_choose_main,0);
    lv_style_set_outline_width(&style_choose_main,0);
    lv_style_set_radius(&style_choose_main, 5);

    lv_style_init(&style_not_choose_main);
    lv_style_set_bg_opa(&style_not_choose_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_not_choose_main, lv_palette_lighten(LV_PALETTE_GREY,1));
    lv_style_set_border_width(&style_not_choose_main,0);
    lv_style_set_outline_width(&style_not_choose_main,0);
    lv_style_set_radius(&style_not_choose_main, 5);    

    lv_style_init(&style_label);
    lv_style_set_bg_opa(&style_label, LV_OPA_TRANSP);
    lv_style_set_text_opa(&style_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_label, lv_color_white());

    lv_style_init(&style_back);
    lv_style_set_bg_opa(&style_back, LV_OPA_COVER);
    lv_style_set_bg_color(&style_back, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_border_width(&style_back,0);
    lv_style_set_outline_width(&style_back,0);
    lv_style_set_radius(&style_back, 0);

    lv_style_init(&style_main);
    lv_style_set_bg_opa(&style_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_main, lv_palette_lighten(LV_PALETTE_GREY,1));
    lv_style_set_radius(&style_main, 0);

    lv_style_init(&style_indicator);
    lv_style_set_bg_opa(&style_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indicator, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_radius(&style_indicator, 0);
}

static lv_obj_t * lv_img_creat_fun(lv_obj_t *src_obj,const void * src,lv_coord_t x_offset,lv_coord_t y_offset)
{
    lv_obj_t * img = lv_img_create(src_obj);
    lv_img_set_src(img,src);
    lv_obj_align_to(img,src_obj,LV_ALIGN_LEFT_MID,x_offset,y_offset);
    lv_obj_add_flag(img,LV_OBJ_FLAG_HIDDEN);
    return img;
}

static lv_obj_t * lv_con_creat_fun(lv_align_t align,lv_coord_t x_offset,lv_coord_t y_offset,lv_coord_t width, lv_coord_t high)
{
    lv_obj_t * con;
    con = lv_obj_create(lv_scr_act());
    lv_obj_add_style(con,&style_back,LV_PART_MAIN);
    lv_obj_clear_flag(con, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(con,width,high);    
    lv_obj_clear_flag(con, LV_OBJ_FLAG_CLICK_FOCUSABLE);    
    lv_obj_align(con,align, x_offset, y_offset);
    return con;
}

static lv_obj_t * lv_btn_creat_fun(lv_obj_t *src_obj,lv_event_cb_t event_cb,lv_coord_t x_offset,lv_coord_t y_offset,lv_coord_t width, lv_coord_t high,uint8_t status)
{
    lv_obj_t * btn;
    btn = lv_obj_create(src_obj);
    if(status){
        lv_obj_add_style(btn,&style_choose_main,LV_PART_MAIN);
    }else{
        lv_obj_add_style(btn,&style_not_choose_main,LV_PART_MAIN);
    }
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(btn,width,high);    
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICK_FOCUSABLE);    
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_align(btn,LV_ALIGN_BOTTOM_MID, x_offset, y_offset);
    return btn;
}

static lv_obj_t * lv_bar_creat_fun(lv_obj_t *align_obj,lv_coord_t x_offset,lv_coord_t y_offset,lv_coord_t width, lv_coord_t high,int32_t init_value)
{
    lv_obj_t * bar = lv_bar_create(align_obj);
    lv_obj_add_style(bar, &style_main, LV_PART_MAIN);
    lv_obj_add_style(bar, &style_indicator, LV_PART_INDICATOR);
    lv_obj_set_size(bar, width, high);
    lv_bar_set_range(bar,0,100);
    lv_obj_align_to(bar,align_obj,LV_ALIGN_TOP_MID,x_offset,y_offset);
    lv_bar_set_value(bar, init_value, LV_ANIM_ON);    
    return bar;
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

static lv_obj_t * lv_value_lab_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,const char * text,lv_align_t align,lv_coord_t x_offset,lv_coord_t y_offset)
{
    // char buf[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    lv_obj_t * lab = lv_label_create(src_obj);
    lv_obj_add_style(lab, &style_label, LV_PART_MAIN);
    // lv_obj_set_style_text_font(lab, &language, 0);
    // lv_snprintf(buf, sizeof(buf), "%dh可用", lv_bar_get_value(bar_obj));
    lv_label_set_text(lab,text);
    lv_obj_align_to(lab,align_obj,align,x_offset,y_offset);
    return lab;
}

static void btn_left_cb(lv_event_t * e)
{
    char * ptr = (char *)lv_event_get_param(e);
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {        
        if(strcmp(ptr,lock)){
            btn_right_cnt=0;
            btn_left_cnt++;
            if(btn_left_cnt > 1){
                LV_LOG_USER("btn_left Clicked\n");
                hl_storage_func(HL_STORAGE_CHECK_LEFT);
            }   
            lv_obj_remove_style(btn_left,&style_choose_main,LV_PART_MAIN);
            lv_obj_add_style(btn_left,&style_choose_main,LV_PART_MAIN);          
            lv_event_send(btn_right,LV_EVENT_CLICKED,lock);
        }else{
            lv_obj_remove_style(btn_left,&style_not_choose_main,LV_PART_MAIN);
            lv_obj_add_style(btn_left,&style_not_choose_main,LV_PART_MAIN);        
        }
    }
}

static void btn_right_cb(lv_event_t * e)
{
    char * ptr = (char *)lv_event_get_param(e);
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        if(strcmp(ptr,lock)){
            btn_left_cnt=0;
            btn_right_cnt++;
            if(btn_right_cnt > 1){
                LV_LOG_USER("btn_right Clicked\n");
                hl_storage_func(HL_STORAGE_CHECK_RIGHT);                
            }   
            lv_obj_remove_style(btn_right,&style_choose_main,LV_PART_MAIN);
            lv_obj_add_style(btn_right,&style_choose_main,LV_PART_MAIN);        
            lv_event_send(btn_left,LV_EVENT_CLICKED,lock);
        }else{
            lv_obj_remove_style(btn_right,&style_not_choose_main,LV_PART_MAIN);
            lv_obj_add_style(btn_right,&style_not_choose_main,LV_PART_MAIN);         
        }
    }
}

static void hl_obj_delete(lv_obj_t *obj,bool obj_typ)
{
    uint32_t child_cnt = 0,i;
    child_cnt = lv_obj_get_child_cnt(obj);
    if(child_cnt == 0){
        lv_obj_add_flag(obj,LV_OBJ_FLAG_HIDDEN);
        lv_obj_del_delayed(obj,0);
    }else{
        for(i=0;i<child_cnt;i++){
            hl_obj_delete(lv_obj_get_child(obj, i),true);            
        }
        if(obj_typ){
            lv_obj_add_flag(obj,LV_OBJ_FLAG_HIDDEN);
            lv_obj_del_delayed(obj,0);
        }        
    }
}

static void lv_delete_style(void)
{
    lv_style_reset(&style_choose_main);
    lv_style_reset(&style_not_choose_main);
    lv_style_reset(&style_choose_lr_main);
    lv_style_reset(&style_not_choose_lr_main);
    lv_style_reset(&style_label);
    lv_style_reset(&style_back);
    lv_style_reset(&style_indicator);
    lv_style_reset(&style_main);
}

void hl_mod_storage_ioctl(void * ctl_data)
{
    hl_lvgl_storage_ioctl_t * ptr = (hl_lvgl_storage_ioctl_t *)ctl_data;
    switch(ptr->storage_choose){
        case HL_STORAGE_CHOOSE_LEFT:
            lv_event_send(btn_left,LV_EVENT_CLICKED,ulock);
            break;
        case HL_STORAGE_CHOOSE_RIGHT:
            lv_event_send(btn_right,LV_EVENT_CLICKED,ulock);
            break;
        case HL_STORAGE_CHOOSE_EXIT:
            hl_obj_delete(lv_scr_act(),false);
            break;
        case HL_STORAGE_CHOOSE_DEL_STYLE:
            lv_delete_style();
            break;
        default:
            break;
    }
}

void hl_mod_storage_init(void * init_data)
{    
    hl_lvgl_storage_init_t * ptr = (hl_lvgl_storage_init_t *)init_data;
    hl_storage_func = ptr->func_cb;
    if (!page_style_bit.page_storage) {
        page_style_bit.page_storage = 1;
        lv_style_page4_init();
    }
    

    con1 = lv_con_creat_fun(LV_ALIGN_LEFT_MID,0,0,144,126);
    con2 = lv_con_creat_fun(LV_ALIGN_RIGHT_MID,0,0,144,126);
    
    if(ptr->storage_choose == HL_STORAGE_CHOOSE_LEFT){
        btn_left_cnt = 1;
        btn_right_cnt = 0;
        btn_left = lv_btn_creat_fun(lv_scr_act(),btn_left_cb,-75,-10,132,42,1);
        btn_right = lv_btn_creat_fun(lv_scr_act(),btn_right_cb,75,-10,132,42,0);
    }
    if(ptr->storage_choose == HL_STORAGE_CHOOSE_RIGHT){
        btn_left_cnt = 0;
        btn_right_cnt = 1;
        btn_left = lv_btn_creat_fun(lv_scr_act(),btn_left_cb,-75,-10,132,42,0);
        btn_right = lv_btn_creat_fun(lv_scr_act(),btn_right_cb,75,-10,132,42,1);
    }    

    bar1 = lv_bar_creat_fun(con1,0,13,132,25,ptr->used_tx1);
    bar2 = lv_bar_creat_fun(con2,0,13,132,25,ptr->used_tx2);

    lab11 = lv_lab_creat_fun(con1,bar1,LV_ALIGN_OUT_TOP_LEFT,0,-6,"TX1");
    lab21 = lv_lab_creat_fun(con2,bar2,LV_ALIGN_OUT_TOP_LEFT,0,-6,"TX2");

    lab12 = lv_value_lab_creat_fun(con1,lab11,ptr->ptr_time_tx1,LV_ALIGN_OUT_RIGHT_MID,10,0);
    lab22 = lv_value_lab_creat_fun(con2,lab21,ptr->ptr_time_tx2,LV_ALIGN_OUT_RIGHT_MID,10,0);    

    lab13 = lv_lab_creat_fun(btn_left,btn_left,LV_ALIGN_CENTER,0,0,"格式化");
    lab23 = lv_lab_creat_fun(btn_right,btn_right,LV_ALIGN_CENTER,0,0,"格式化");

    valid_obj[0] = btn_left;
    valid_obj[1] = btn_right;
}

lv_obj_t * hl_storage_obj_get(uint8_t num)
{
    return valid_obj[num];
}

//测试接口
// void page_storage_test(void)
// {
    
    // lv_page_4_init();
// }