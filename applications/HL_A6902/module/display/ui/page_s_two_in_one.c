/**
 * @file page_formatting.c
 * @author your name (you@domain.com)
 * @brief 格式化UI界面
 * @version 0.1
 * @date 2022-12-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "page_s_two_in_one.h"
// #include "language.h"
#include "page_style_bit.h"

#define  CHICK_STA_LIFT 0
#define  CHICK_STA_RIGHT 1
LV_IMG_DECLARE(Other_monitor_uac_black);//
LV_IMG_DECLARE(Other_monitor_uac_white);//
LV_IMG_DECLARE(Other_monitor_tx_black);//
LV_IMG_DECLARE(Other_monitor_tx_white);//

#define MAX_OBJ_NUMBER     6

#define  LAB_COLOR_WHITE    0
#define  LAB_COLOR_BLACK    1

static lv_obj_t *valid_obj[MAX_OBJ_NUMBER] = {NULL, NULL, NULL, NULL, NULL, NULL};
static uint8_t btn_left_cnt = 0,btn_right_cnt = 0;

static char * lock = "btn";
static char * ulock = "ubtn";

static lv_style_t style_choose_main,style_not_choose_main;
static lv_style_t style_choose_lr_main,style_not_choose_lr_main;
static lv_style_t style_label_white,style_label_black;

static lv_obj_t * btn_left,*btn_right;
static lv_obj_t * lab1,*lab2,*lab3;

static hl_s_two_in_one_event_cb hl_s_two_in_one_func;

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
    lv_style_set_bg_color(&style_not_choose_main, lv_palette_darken(LV_PALETTE_GREY,3));
    lv_style_set_border_width(&style_not_choose_main,0);
    lv_style_set_outline_width(&style_not_choose_main,0);
    lv_style_set_radius(&style_not_choose_main, 5);    

    lv_style_init(&style_label_white);
    lv_style_set_bg_opa(&style_label_white, LV_OPA_TRANSP);
    lv_style_set_text_opa(&style_label_white, LV_OPA_COVER);
    lv_style_set_text_color(&style_label_white, lv_color_white());

    lv_style_init(&style_label_black);
    lv_style_set_bg_opa(&style_label_black, LV_OPA_TRANSP);
    lv_style_set_text_opa(&style_label_black, LV_OPA_COVER);
    lv_style_set_text_color(&style_label_black, lv_color_black());
}

static lv_obj_t * lv_img_creat_fun(lv_obj_t *src_obj,const void * src,lv_coord_t x_offset,lv_coord_t y_offset)
{
    lv_obj_t * img = lv_img_create(src_obj);
    lv_img_set_src(img,src);
    lv_obj_align_to(img,src_obj,LV_ALIGN_LEFT_MID,x_offset,y_offset);
    lv_obj_add_flag(img,LV_OBJ_FLAG_HIDDEN);
    return img;
}

static lv_obj_t * lv_btn_creat_fun(lv_event_cb_t event_cb,lv_coord_t x_offset,lv_coord_t y_offset,lv_coord_t width, lv_coord_t high,uint8_t status)
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
    lv_obj_align(btn,LV_ALIGN_BOTTOM_LEFT, x_offset, y_offset);
    return btn;
}

static lv_obj_t * lv_lab_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,lv_align_t align,lv_coord_t x_offset,lv_coord_t y_offset,const char * ptr,uint8_t color)
{
    lv_obj_t * lab = lv_label_create(src_obj);
    if(color == LAB_COLOR_WHITE){
        lv_obj_add_style(lab, &style_label_white, LV_PART_MAIN);
    }else{
        lv_obj_add_style(lab, &style_label_black, LV_PART_MAIN);
    }
    
    // lv_obj_set_style_text_font(lab, &language, 0);
    lv_label_set_text(lab,ptr);
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
            if(btn_left_cnt >= 1){
                LV_LOG_USER("btn_left Clicked\n");
                hl_s_two_in_one_func(HL_S_TWO_ONE_CHECK_LEFT);
            } 
            lv_obj_remove_style(lab1,&style_label_black,LV_PART_MAIN);
            lv_obj_remove_style(btn_left,&style_choose_main,LV_PART_MAIN);
            lv_obj_add_style(lab1,&style_label_black,LV_PART_MAIN); 
            lv_obj_add_style(btn_left,&style_choose_main,LV_PART_MAIN);          
            lv_event_send(btn_right,LV_EVENT_CLICKED,lock);
        }else{
            lv_obj_remove_style(lab1,&style_label_white,LV_PART_MAIN);
            lv_obj_remove_style(btn_left,&style_not_choose_main,LV_PART_MAIN);
            lv_obj_add_style(lab1,&style_label_white,LV_PART_MAIN); 
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
            if(btn_right_cnt >= 1){
                LV_LOG_USER("btn_right Clicked\n");
                hl_s_two_in_one_func(HL_S_TWO_ONE_CHECK_RIGHT);                
            }  
            lv_obj_remove_style(lab2,&style_label_black,LV_PART_MAIN);
            lv_obj_remove_style(btn_right,&style_choose_main,LV_PART_MAIN);
            lv_obj_add_style(lab2,&style_label_black,LV_PART_MAIN); 
            lv_obj_add_style(btn_right,&style_choose_main,LV_PART_MAIN);                    
            lv_event_send(btn_left,LV_EVENT_CLICKED,lock);
        }else{
            lv_obj_remove_style(lab2,&style_label_white,LV_PART_MAIN);
            lv_obj_remove_style(btn_right,&style_not_choose_main,LV_PART_MAIN);
            lv_obj_add_style(lab2,&style_label_white,LV_PART_MAIN); 
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
    lv_style_reset(&style_label_white);
    lv_style_reset(&style_label_black);
}

void hl_mod_s_two_in_one_ioctl(void * ctl_data)
{
    hl_lvgl_s_two_in_one_ioctl_t * ptr = (hl_lvgl_s_two_in_one_ioctl_t *)ctl_data;
    switch(ptr->s_two_in_one_choose){
        case HL_S_TWO_ONE_CHOOSE_LEFT:
            lv_event_send(btn_left,LV_EVENT_CLICKED,ulock);
        break;
        case HL_S_TWO_ONE_CHOOSE_RIGHT:
            lv_event_send(btn_right,LV_EVENT_CLICKED,ulock);
        break;
        case HL_S_TWO_ONE_CHOOSE_EXIT:
            hl_obj_delete(lv_scr_act(),false);
        break;
        case HL_S_TWO_ONE_CHOOSE_DEL_STYLE:
            lv_delete_style();
        break;
        default:
        break;
    }
}

void hl_mod_s_two_in_one_init(void * init_data)
{
    hl_lvgl_s_two_in_one_init_t * ptr = (hl_lvgl_s_two_in_one_init_t *)init_data;
    hl_s_two_in_one_func = ptr->func_cb;
    if (!page_style_bit.page_s_two_in_one) {
        page_style_bit.page_s_two_in_one = 1;
        lv_style_page4_init();
    }
    

    if(ptr->s_two_in_one_choose == HL_S_TWO_ONE_CHOOSE_LEFT){
        btn_right_cnt = 0;
        btn_left_cnt = 1;
        btn_left = lv_btn_creat_fun(btn_left_cb,6,-6,138,82,1);
        btn_right = lv_btn_creat_fun(btn_right_cb,150,-6,138,82,0);
        lab1 = lv_lab_creat_fun(btn_left,btn_left,LV_ALIGN_CENTER,0,0,ptr->ptr_lift,LAB_COLOR_BLACK);
        lab2 = lv_lab_creat_fun(btn_right,btn_right,LV_ALIGN_CENTER,0,0,ptr->ptr_right,LAB_COLOR_WHITE);
    }
    if(ptr->s_two_in_one_choose == HL_S_TWO_ONE_CHOOSE_RIGHT){
        btn_right_cnt = 1;
        btn_left_cnt = 0;
        btn_left = lv_btn_creat_fun(btn_left_cb,6,-6,138,82,0);
        btn_right = lv_btn_creat_fun(btn_right_cb,150,-6,138,82,1);
        lab1 = lv_lab_creat_fun(btn_left,btn_left,LV_ALIGN_CENTER,0,0,ptr->ptr_lift,LAB_COLOR_WHITE);
        lab2 = lv_lab_creat_fun(btn_right,btn_right,LV_ALIGN_CENTER,0,0,ptr->ptr_right,LAB_COLOR_BLACK);
    }    
    
    lab3 = lv_lab_creat_fun(lv_scr_act(),lv_scr_act(),LV_ALIGN_TOP_MID,0,5,ptr->ptr_top,LAB_COLOR_WHITE);

    valid_obj[0] = btn_left;
    valid_obj[1] = btn_right;
}

lv_obj_t * hl_formatting_obj_get(uint8_t num)
{
    return valid_obj[num];
}

//测试接口
void page_formatting_test(void)
{
    
    // lv_page_4_init();
}