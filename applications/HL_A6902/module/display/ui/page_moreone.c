/**
 * @file page_moreone.c
 * @author your name (you@domain.com)
 * @brief 录制、降噪开关、静音多合一界面
 * @version 0.1
 * @date 2022-12-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "page_moreone.h"
// #include "language.h"

#define MAX_OBJ_NUMBER     7

static lv_obj_t *valid_obj[MAX_OBJ_NUMBER] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};

static uint8_t btn_left_1 = 0,btn_right_1 = 0;
static uint8_t btn_left_2 = 0,btn_right_2 = 0;
static uint8_t btn_left_3 = 0,btn_right_3 = 0;

static char * lock = "btn";
static char * ulock = "ubtn";

static lv_style_t style_choose_main,style_not_choose_main;
static lv_style_t style_choose_lr_main,style_not_choose_lr_main;
static lv_style_t style_label;
static lv_style_t style_screen;

static lv_obj_t * btn1_l,*btn1_r;
static lv_obj_t * btn2_l,*btn2_r;
static lv_obj_t * btn3_l,*btn3_r;
static lv_obj_t * btn11,*btn12;
static lv_obj_t * btn21,*btn22;
static lv_obj_t * btn31,*btn32;
static lv_obj_t * lab11,*lab12,*lab13;
static lv_obj_t * lab21,*lab22,*lab23;
static lv_obj_t * lab31,*lab32,*lab33;
static lv_obj_t * con1,*con2,*con3,*con_src;

static hl_moreone_event_cb hl_moreone_func;

static void lv_style_page4_init(void)
{
    lv_obj_enable_style_refresh(true);

    lv_style_init(&style_screen);
    lv_style_set_bg_color(&style_screen, lv_color_black());
    lv_style_set_border_width(&style_screen, 0);
    lv_style_set_outline_width(&style_screen,0);
    lv_style_set_radius(&style_screen, 0);
    // lv_obj_add_style(lv_scr_act(), &style_screen, 0);

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

static lv_obj_t * lv_btn_lr_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,lv_align_t align,lv_coord_t x_offset,lv_coord_t y_offset,\
                                        lv_coord_t width, lv_coord_t high,uint8_t status)
{
    lv_obj_t * btn;
    btn = lv_obj_create(src_obj);
    if(status){
        lv_obj_add_style(btn,&style_choose_lr_main,LV_PART_MAIN);
    }else{
        lv_obj_add_style(btn,&style_not_choose_lr_main,LV_PART_MAIN);
    }
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
    // lv_obj_move_background(btn);
    lv_obj_set_size(btn,width,high);    
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_align_to(btn, align_obj,align, x_offset, y_offset);
    return btn;
}

static lv_obj_t * lv_btn_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,lv_align_t align,lv_event_cb_t event_cb,\
                                    lv_coord_t x_offset,lv_coord_t y_offset,lv_coord_t width, lv_coord_t high,uint8_t status)
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

static void lv_obj_style_set(lv_obj_t * obj1,lv_obj_t * obj2, lv_style_t * style1,lv_style_t * style2)
{
    lv_obj_remove_style(obj1,style1,LV_PART_MAIN);
    lv_obj_remove_style(obj2,style2,LV_PART_MAIN);
    lv_obj_add_style(obj1,style1,LV_PART_MAIN);
    lv_obj_add_style(obj2,style2,LV_PART_MAIN);  
}

static void btn11_cb(lv_event_t * e)
{
    char * ptr = (char *)lv_event_get_param(e);
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {        
        if(strcmp(ptr,lock)){
            btn_right_1=0;
            btn_left_1++;
            if(btn_left_1 > 1){
                LV_LOG_USER("btn_left1 Clicked\n");
                hl_moreone_func(HL_MOREONE_CHECK_ONE_LEFT);
            } 
            lv_obj_style_set(btn11,btn1_l,&style_choose_main,&style_choose_lr_main);
            lv_event_send(btn12,LV_EVENT_CLICKED,lock);
        }else{
            lv_obj_style_set(btn11,btn1_l,&style_not_choose_main,&style_not_choose_lr_main);
        }
    }
}

static void btn12_cb(lv_event_t * e)
{
    char * ptr = (char *)lv_event_get_param(e);
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        if(strcmp(ptr,lock)){
            btn_right_1++;
            btn_left_1 = 0;
            if(btn_right_1 > 1){
                LV_LOG_USER("btn_right1 Clicked\n");
                hl_moreone_func(HL_MOREONE_CHECK_ONE_RIGHT);
            }
            lv_obj_style_set(btn12,btn1_r,&style_choose_main,&style_choose_lr_main);
            lv_event_send(btn11,LV_EVENT_CLICKED,lock);
        }else{
            lv_obj_style_set(btn12,btn1_r,&style_not_choose_main,&style_not_choose_lr_main);
        }
    }
}

static void btn21_cb(lv_event_t * e)
{
    char * ptr = (char *)lv_event_get_param(e);
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {        
        if(strcmp(ptr,lock)){
            btn_right_2=0;
            btn_left_2++;
            if(btn_left_2 > 1){
                LV_LOG_USER("btn_left2 Clicked\n");
                hl_moreone_func(HL_MOREONE_CHECK_TWO_LEFT);
            }
            lv_obj_style_set(btn21,btn2_l,&style_choose_main,&style_choose_lr_main);
            lv_event_send(btn22,LV_EVENT_CLICKED,lock);
        }else{
            lv_obj_style_set(btn21,btn2_l,&style_not_choose_main,&style_not_choose_lr_main);
        }
    }
}

static void btn22_cb(lv_event_t * e)
{
    char * ptr = (char *)lv_event_get_param(e);
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        if(strcmp(ptr,lock)){
            btn_right_2++;
            btn_left_2 = 0;
            if(btn_right_2 > 1){
                LV_LOG_USER("btn_right2 Clicked\n");
                hl_moreone_func(HL_MOREONE_CHECK_TWO_RIGHT);
            }
            lv_obj_style_set(btn22,btn2_r,&style_choose_main,&style_choose_lr_main);
            lv_event_send(btn21,LV_EVENT_CLICKED,lock);
        }else{
            lv_obj_style_set(btn22,btn2_r,&style_not_choose_main,&style_not_choose_lr_main);
        }
    }
}

static void btn31_cb(lv_event_t * e)
{
    char * ptr = (char *)lv_event_get_param(e);
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {        
        if(strcmp(ptr,lock)){
            btn_right_3=0;
            btn_left_3++;
            if(btn_left_3 > 1){
                LV_LOG_USER("btn_left3 Clicked\n");
                hl_moreone_func(HL_MOREONE_CHECK_THREE_LEFT);
            }
            lv_obj_style_set(btn31,btn3_l,&style_choose_main,&style_choose_lr_main);
            lv_event_send(btn32,LV_EVENT_CLICKED,lock);
        }else{
            lv_obj_style_set(btn31,btn3_l,&style_not_choose_main,&style_not_choose_lr_main);
        }
    }
}

static void btn32_cb(lv_event_t * e)
{
    char * ptr = (char *)lv_event_get_param(e);
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        if(strcmp(ptr,lock)){
            btn_right_3++;
            btn_left_3 = 0;
            if(btn_right_3 > 1){
                LV_LOG_USER("btn_right3 Clicked\n");
                hl_moreone_func(HL_MOREONE_CHECK_THREE_RIGHT);
            }
            lv_obj_style_set(btn32,btn3_r,&style_choose_main,&style_choose_lr_main);
            lv_event_send(btn31,LV_EVENT_CLICKED,lock);
        }else{
            lv_obj_style_set(btn32,btn3_r,&style_not_choose_main,&style_not_choose_lr_main);
        }
    }
}

static lv_obj_t * lv_con_scr_creat(lv_coord_t x_size,lv_coord_t y_size)
{
    lv_obj_t * con;
    con = lv_obj_create(lv_scr_act());
    lv_obj_add_style(con,&style_screen,0);
    lv_obj_align(con, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_flex_flow(con, LV_FLEX_FLOW_ROW);
    lv_obj_set_scroll_snap_x(con, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(con, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(con,x_size,y_size);    
    return con;
}

static lv_obj_t * lv_con_child_creat(lv_obj_t *src_obj,lv_coord_t x_size,lv_coord_t y_size)
{
    lv_obj_t * con;
    con = lv_obj_create(src_obj);
    lv_obj_add_style(con,&style_screen,0);
    lv_obj_clear_flag(con, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(con,LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_size(con,x_size,y_size);
    return con;
}

static void lv_page_1_init(lv_obj_t *con_obj,hl_moreone_choose_t opt)
{   
    if(opt == HL_MOREONE_CHOOSE_LEFT){
        btn_left_1 = 1;
        btn_right_1 = 0;
        btn1_l = lv_btn_lr_creat_fun(con_obj,con_obj,LV_ALIGN_LEFT_MID,-12,13,15,72,1);
        btn11 = lv_btn_creat_fun(con_obj,btn1_l,LV_ALIGN_OUT_RIGHT_MID,btn11_cb,-10,0,108,72,1);
        btn12 = lv_btn_creat_fun(con_obj,btn11,LV_ALIGN_OUT_RIGHT_MID,btn12_cb,0,0,108,72,0);
        btn1_r = lv_btn_lr_creat_fun(con_obj,btn12,LV_ALIGN_OUT_RIGHT_MID,-10,0,15,72,0);
    }
    if(opt == HL_MOREONE_CHOOSE_RIGHT){
        btn_left_1 = 0;
        btn_right_1 = 1;
        btn1_l = lv_btn_lr_creat_fun(con_obj,con_obj,LV_ALIGN_LEFT_MID,-12,13,15,72,0);
        btn11 = lv_btn_creat_fun(con_obj,btn1_l,LV_ALIGN_OUT_RIGHT_MID,btn11_cb,-10,0,108,72,0);
        btn12 = lv_btn_creat_fun(con_obj,btn11,LV_ALIGN_OUT_RIGHT_MID,btn12_cb,0,0,108,72,1);
        btn1_r = lv_btn_lr_creat_fun(con_obj,btn12,LV_ALIGN_OUT_RIGHT_MID,-10,0,15,72,1);
    }

    lab11 = lv_lab_creat_fun(btn11,btn11,LV_ALIGN_CENTER,0,0,"ON");
    lab12 = lv_lab_creat_fun(btn12,btn12,LV_ALIGN_CENTER,0,0,"OFF");
    lab13 = lv_lab_creat_fun(con_obj,con_obj,LV_ALIGN_TOP_MID,-10,-10,"录制开关");
}

static void lv_page_2_init(lv_obj_t *con_obj,hl_moreone_choose_t opt)
{   
    if(opt == HL_MOREONE_CHOOSE_LEFT){
        btn_left_2 = 1;
        btn_right_2 = 0;
        btn2_l = lv_btn_lr_creat_fun(con_obj,con_obj,LV_ALIGN_LEFT_MID,-12,13,15,72,1);
        btn21 = lv_btn_creat_fun(con_obj,btn2_l,LV_ALIGN_OUT_RIGHT_MID,btn21_cb,-10,0,108,72,1);
        btn22 = lv_btn_creat_fun(con_obj,btn21,LV_ALIGN_OUT_RIGHT_MID,btn22_cb,0,0,108,72,0);
        btn2_r = lv_btn_lr_creat_fun(con_obj,btn22,LV_ALIGN_OUT_RIGHT_MID,-10,0,15,72,0);
    }
    if(opt == HL_MOREONE_CHOOSE_RIGHT){
        btn_left_2 = 0;
        btn_right_2 = 1;
        btn2_l = lv_btn_lr_creat_fun(con_obj,con_obj,LV_ALIGN_LEFT_MID,-12,13,15,72,0);
        btn21 = lv_btn_creat_fun(con_obj,btn2_l,LV_ALIGN_OUT_RIGHT_MID,btn21_cb,-10,0,108,72,0);
        btn22 = lv_btn_creat_fun(con_obj,btn21,LV_ALIGN_OUT_RIGHT_MID,btn22_cb,0,0,108,72,1);
        btn2_r = lv_btn_lr_creat_fun(con_obj,btn22,LV_ALIGN_OUT_RIGHT_MID,-10,0,15,72,1);
    }

    lab21 = lv_lab_creat_fun(btn21,btn21,LV_ALIGN_CENTER,0,0,"ON");
    lab22 = lv_lab_creat_fun(btn22,btn22,LV_ALIGN_CENTER,0,0,"OFF");
    lab23 = lv_lab_creat_fun(con_obj,con_obj,LV_ALIGN_TOP_MID,-10,-10,"MUTE");
}

static void lv_page_3_init(lv_obj_t *con_obj,hl_moreone_choose_t opt)
{    
    if(opt == HL_MOREONE_CHOOSE_LEFT){
        btn_left_3 = 1;
        btn_right_3 = 0;
        btn3_l = lv_btn_lr_creat_fun(con_obj,con_obj,LV_ALIGN_LEFT_MID,-12,13,15,72,1);
        btn31 = lv_btn_creat_fun(con_obj,btn3_l,LV_ALIGN_OUT_RIGHT_MID,btn31_cb,-10,0,108,72,1);
        btn32 = lv_btn_creat_fun(con_obj,btn31,LV_ALIGN_OUT_RIGHT_MID,btn32_cb,0,0,108,72,0);
        btn3_r = lv_btn_lr_creat_fun(con_obj,btn32,LV_ALIGN_OUT_RIGHT_MID,-10,0,15,72,0);
    } 
    if(opt == HL_MOREONE_CHOOSE_RIGHT){
        btn_left_3 = 0;
        btn_right_3 = 1;
        btn3_l = lv_btn_lr_creat_fun(con_obj,con_obj,LV_ALIGN_LEFT_MID,-12,13,15,72,0);
        btn31 = lv_btn_creat_fun(con_obj,btn3_l,LV_ALIGN_OUT_RIGHT_MID,btn31_cb,-10,0,108,72,0);
        btn32 = lv_btn_creat_fun(con_obj,btn31,LV_ALIGN_OUT_RIGHT_MID,btn32_cb,0,0,108,72,1);
        btn3_r = lv_btn_lr_creat_fun(con_obj,btn32,LV_ALIGN_OUT_RIGHT_MID,-10,0,15,72,1);
    }    

    lab31 = lv_lab_creat_fun(btn31,btn31,LV_ALIGN_CENTER,0,0,"ON");
    lab32 = lv_lab_creat_fun(btn32,btn32,LV_ALIGN_CENTER,0,0,"OFF");
    lab33 = lv_lab_creat_fun(con_obj,con_obj,LV_ALIGN_TOP_MID,-10,-10,"降噪开关");
}

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
    lv_style_reset(&style_choose_main);
    lv_style_reset(&style_not_choose_main);
    lv_style_reset(&style_choose_lr_main);
    lv_style_reset(&style_not_choose_lr_main);
    lv_style_reset(&style_label);
    lv_style_reset(&style_screen);
}

static void hl_option_mid_set(hl_moreone_mid_opt_t opt,lv_anim_enable_t anim_en)
{
    switch(opt){
        case HL_MOREONE_OPTION_ONE:
            lv_obj_scroll_to_view(con1, anim_en);
            break;
        case HL_MOREONE_OPTION_TWO:
            lv_obj_scroll_to_view(con2, anim_en);
            break;
        case HL_MOREONE_OPTION_THREE:
            lv_obj_scroll_to_view(con3, anim_en);
            break;
        default:
            break;
    }
}

static void hl_postion_cmd_deal(hl_moreone_mid_opt_t pos)
{
    switch (pos) {
        case HL_MOREONE_OPTION_ONE:
        case HL_MOREONE_OPTION_TWO:
        case HL_MOREONE_OPTION_THREE:
            hl_option_mid_set(pos,LV_ANIM_ON);
            break;
        case HL_MOREONE_OPTION_EXTI:  
            hl_obj_delete(lv_scr_act(),false);
            break;
        case HL_MOREONE_OPTION_DELETE_STYLE:  
            lv_delete_style();
            break;
        default:
            break;      
    }
}

static void hl_option_cmd_deal(hl_moreone_mid_opt_t curr_opt,hl_moreone_choose_t opt_cho)
{
    switch (curr_opt) {
        case HL_MOREONE_OPTION_ONE:
            if(opt_cho == HL_MOREONE_CHOOSE_LEFT){
                lv_event_send(btn11,LV_EVENT_CLICKED,ulock);
            }else{
                lv_event_send(btn12,LV_EVENT_CLICKED,ulock);
            }
            break;
        case HL_MOREONE_OPTION_TWO:
            if(opt_cho == HL_MOREONE_CHOOSE_LEFT){
                lv_event_send(btn21,LV_EVENT_CLICKED,ulock);
            }else{
                lv_event_send(btn22,LV_EVENT_CLICKED,ulock);
            }
            break;
        case HL_MOREONE_OPTION_THREE:
            if(opt_cho == HL_MOREONE_CHOOSE_LEFT){
                lv_event_send(btn31,LV_EVENT_CLICKED,ulock);
            }else{
                lv_event_send(btn32,LV_EVENT_CLICKED,ulock);
            }
            break;
        default:
            break;      
    }
}

void hl_mod_moreone_ioctl(void * ctl_data)
{
    hl_lvgl_moreone_ioctl_t * ptr = (hl_lvgl_moreone_ioctl_t *)ctl_data;
    switch(ptr->cmd_type){
        case HL_MOREONE_POSTION_CMD:
            hl_postion_cmd_deal(ptr->moreone_choose_opt);
            break;
        case HL_MOREONE_OPTION_CMD:
            hl_option_cmd_deal(ptr->moreone_choose_opt,ptr->moreone_choose);
            break;
        default:
            break;
    }
}

void hl_mod_moreone_init(void * init_data)
{
    hl_lvgl_moreone_init_t * ptr = (hl_lvgl_moreone_init_t *)init_data;
    hl_moreone_func = ptr->func_cb;
    lv_style_page4_init();

    con_src = lv_con_scr_creat(270,140);
    lv_obj_set_flex_align(con_src,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scroll_dir(con_src,LV_DIR_HOR);
    con1 = lv_con_child_creat(con_src,260,120);
    con2 = lv_con_child_creat(con_src,260,120);
    con3 = lv_con_child_creat(con_src,260,120);
    lv_page_1_init(con1,ptr->moreone_choose_opt.option_one);
    lv_page_2_init(con2,ptr->moreone_choose_opt.option_two);
    lv_page_3_init(con3,ptr->moreone_choose_opt.option_three);
    hl_option_mid_set(ptr->moreone_mid_opt,LV_ANIM_OFF);
    valid_obj[0] = btn11;
    valid_obj[1] = btn12;
    valid_obj[2] = btn21;
    valid_obj[3] = btn22;
    valid_obj[4] = btn31;
    valid_obj[5] = btn32;
}

lv_obj_t * hl_moreone_obj_get(uint8_t num)
{
    return valid_obj[num];
}

//测试接口
// void page_moreone_test(void)
// {
    
//     lv_page_init();
// }