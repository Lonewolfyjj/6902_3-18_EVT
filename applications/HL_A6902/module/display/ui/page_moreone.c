/**
 * @file page_moreone.c
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief 
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
#include "page_moreone.h"
#include "page_style_bit.h"

#define MAX_OBJ_NUMBER     7

#define  LAB_COLOR_WHITE    0
#define  LAB_COLOR_BLACK    1

static lv_obj_t *valid_obj[MAX_OBJ_NUMBER] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};

static uint8_t btn_left_1 = 0,btn_right_1 = 0;
static uint8_t btn_left_2 = 0,btn_right_2 = 0;
static uint8_t btn_left_3 = 0,btn_right_3 = 0;

static char * lock = "btn";
static char * ulock = "ubtn";

static lv_style_t style_choose_main,style_not_choose_main;
static lv_style_t style_choose_lr_main,style_not_choose_lr_main;
static lv_style_t style_label_white,style_label_black;
static lv_style_t style_screen;
static lv_style_t style_white_dot;
static lv_style_t style_grey_dot;

static lv_obj_t * device_lab;
static lv_obj_t * btn1_l,*btn1_r;
static lv_obj_t * btn2_l,*btn2_r;
static lv_obj_t * btn3_l,*btn3_r;
static lv_obj_t * btn11,*btn12;
static lv_obj_t * btn21,*btn22;
static lv_obj_t * btn31,*btn32;
static lv_obj_t * lab11,*lab12,*lab13;
static lv_obj_t * lab21,*lab22,*lab23;
static lv_obj_t * lab31,*lab32,*lab33;
static lv_obj_t * dot1,*dot2,*dot3;
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
    // lv_style_set_bg_color(&style_not_choose_main, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_bg_color(&style_not_choose_main, lv_palette_darken(LV_PALETTE_GREY,3));
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
    // lv_style_set_bg_color(&style_not_choose_lr_main, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_bg_color(&style_not_choose_lr_main, lv_palette_darken(LV_PALETTE_GREY,3));
    lv_style_set_border_width(&style_not_choose_lr_main,0);
    lv_style_set_outline_width(&style_not_choose_lr_main,0);
    lv_style_set_radius(&style_not_choose_lr_main, 10);

    lv_style_init(&style_label_white);
    lv_style_set_bg_opa(&style_label_white, LV_OPA_TRANSP);
    lv_style_set_text_opa(&style_label_white, LV_OPA_COVER);
    lv_style_set_text_color(&style_label_white, lv_color_white());

    lv_style_init(&style_label_black);
    lv_style_set_bg_opa(&style_label_black, LV_OPA_TRANSP);
    lv_style_set_text_opa(&style_label_black, LV_OPA_COVER);
    lv_style_set_text_color(&style_label_black, lv_color_black());

    lv_style_init(&style_white_dot);
    lv_style_set_bg_opa(&style_white_dot, LV_OPA_COVER);
    lv_style_set_bg_color(&style_white_dot, lv_color_white());//LV_RADIUS_CIRCLE
    lv_style_set_radius(&style_white_dot, LV_RADIUS_CIRCLE);

    lv_style_init(&style_grey_dot);
    lv_style_set_bg_opa(&style_grey_dot, LV_OPA_COVER);
    lv_style_set_bg_color(&style_grey_dot, lv_palette_main(LV_PALETTE_GREY));//LV_RADIUS_CIRCLE
    lv_style_set_radius(&style_grey_dot, LV_RADIUS_CIRCLE);
}

static void lv_current_dot(uint8_t id)
{ 
    switch(id) {
        case 1:
            lv_obj_remove_style(dot2,&style_grey_dot,LV_PART_MAIN);
            lv_obj_add_style(dot2,&style_grey_dot,LV_PART_MAIN); 
            lv_obj_remove_style(dot3,&style_grey_dot,LV_PART_MAIN);
            lv_obj_add_style(dot3,&style_grey_dot,LV_PART_MAIN); 

            lv_obj_remove_style(dot1,&style_white_dot,LV_PART_MAIN);
            lv_obj_add_style(dot1,&style_white_dot,LV_PART_MAIN); 
            break;
        case 2:
            lv_obj_remove_style(dot1,&style_grey_dot,LV_PART_MAIN);
            lv_obj_add_style(dot1,&style_grey_dot,LV_PART_MAIN); 
            lv_obj_remove_style(dot3,&style_grey_dot,LV_PART_MAIN);
            lv_obj_add_style(dot3,&style_grey_dot,LV_PART_MAIN); 

            lv_obj_remove_style(dot2,&style_white_dot,LV_PART_MAIN);
            lv_obj_add_style(dot2,&style_white_dot,LV_PART_MAIN); 
            break;
        case 3:
            lv_obj_remove_style(dot2,&style_grey_dot,LV_PART_MAIN);
            lv_obj_add_style(dot2,&style_grey_dot,LV_PART_MAIN); 
            lv_obj_remove_style(dot1,&style_grey_dot,LV_PART_MAIN);
            lv_obj_add_style(dot1,&style_grey_dot,LV_PART_MAIN); 

            lv_obj_remove_style(dot3,&style_white_dot,LV_PART_MAIN);
            lv_obj_add_style(dot3,&style_white_dot,LV_PART_MAIN); 
            break;
        default:
            break;
    }
}

static lv_obj_t * lv_moreone_dot_creat_fun(lv_obj_t *align_obj,lv_coord_t x_offset,lv_coord_t y_offset,uint8_t color)
{
    lv_obj_t * dot = lv_bar_create(align_obj);
    if(color == 0){
        lv_obj_add_style(dot, &style_grey_dot, LV_PART_MAIN);
    }else{
        lv_obj_add_style(dot, &style_white_dot, LV_PART_MAIN);
    }    
    lv_obj_set_size(dot, 10, 10);
    lv_obj_align_to(dot,align_obj,LV_ALIGN_BOTTOM_MID,x_offset,y_offset);
    return dot;
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

static void lv_obj_style_set(lv_obj_t * obj0,lv_obj_t * obj1,lv_obj_t * obj2, lv_style_t * style0,lv_style_t * style1,lv_style_t * style2)
{
    lv_obj_remove_style(obj0,style0,LV_PART_MAIN);
    lv_obj_remove_style(obj1,style1,LV_PART_MAIN);
    lv_obj_remove_style(obj2,style2,LV_PART_MAIN);
    lv_obj_add_style(obj0,style0,LV_PART_MAIN);
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
            if(btn_left_1 == 1){
                hl_moreone_func(HL_MOREONE_CHG_ONE_LEFT);
            }
            if(btn_left_1 > 1){
                LV_LOG_USER("btn_left1 Clicked\n");
                hl_moreone_func(HL_MOREONE_CHECK_ONE_LEFT);
            } 
            lv_obj_style_set(lab11,btn11,btn1_l,&style_label_black,&style_choose_main,&style_choose_lr_main);
            lv_event_send(btn12,LV_EVENT_CLICKED,lock);
        }else{
            lv_obj_style_set(lab11,btn11,btn1_l,&style_label_white,&style_not_choose_main,&style_not_choose_lr_main);
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
            if(btn_right_1 == 1){
                hl_moreone_func(HL_MOREONE_CHG_ONE_RIGHT);
            }
            if(btn_right_1 > 1){
                LV_LOG_USER("btn_right1 Clicked\n");
                hl_moreone_func(HL_MOREONE_CHECK_ONE_RIGHT);
            }
            lv_obj_style_set(lab12,btn12,btn1_r,&style_label_black,&style_choose_main,&style_choose_lr_main);
            lv_event_send(btn11,LV_EVENT_CLICKED,lock);
        }else{
            lv_obj_style_set(lab12,btn12,btn1_r,&style_label_white,&style_not_choose_main,&style_not_choose_lr_main);
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
            if(btn_left_2 == 1){
                hl_moreone_func(HL_MOREONE_CHG_TWO_LEFT);
            }
            if(btn_left_2 > 1){
                LV_LOG_USER("btn_left2 Clicked\n");
                hl_moreone_func(HL_MOREONE_CHECK_TWO_LEFT);
            }
            lv_obj_style_set(lab21,btn21,btn2_l,&style_label_black,&style_choose_main,&style_choose_lr_main);
            lv_event_send(btn22,LV_EVENT_CLICKED,lock);
        }else{
            lv_obj_style_set(lab21,btn21,btn2_l,&style_label_white,&style_not_choose_main,&style_not_choose_lr_main);
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
            if(btn_right_2 == 1){
                hl_moreone_func(HL_MOREONE_CHG_TWO_RIGHT);
            }
            if(btn_right_2 > 1){
                LV_LOG_USER("btn_right2 Clicked\n");
                hl_moreone_func(HL_MOREONE_CHECK_TWO_RIGHT);
            }
            lv_obj_style_set(lab22,btn22,btn2_r,&style_label_black,&style_choose_main,&style_choose_lr_main);
            lv_event_send(btn21,LV_EVENT_CLICKED,lock);
        }else{
            lv_obj_style_set(lab22,btn22,btn2_r,&style_label_white,&style_not_choose_main,&style_not_choose_lr_main);
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
            if(btn_left_3 == 1){
                hl_moreone_func(HL_MOREONE_CHG_THREE_LEFT);
            }
            if(btn_left_3 > 1){
                LV_LOG_USER("btn_left3 Clicked\n");
                hl_moreone_func(HL_MOREONE_CHECK_THREE_LEFT);
            }
            lv_obj_style_set(lab31,btn31,btn3_l,&style_label_black,&style_choose_main,&style_choose_lr_main);
            lv_event_send(btn32,LV_EVENT_CLICKED,lock);
        }else{
            lv_obj_style_set(lab31,btn31,btn3_l,&style_label_white,&style_not_choose_main,&style_not_choose_lr_main);
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
            if(btn_right_3 == 1){
                hl_moreone_func(HL_MOREONE_CHG_THREE_RIGHT);
            }
            if(btn_right_3 > 1){
                LV_LOG_USER("btn_right3 Clicked\n");
                hl_moreone_func(HL_MOREONE_CHECK_THREE_RIGHT);
            }
            lv_obj_style_set(lab32,btn32,btn3_r,&style_label_black,&style_choose_main,&style_choose_lr_main);
            lv_event_send(btn31,LV_EVENT_CLICKED,lock);
        }else{
            lv_obj_style_set(lab32,btn32,btn3_r,&style_label_white,&style_not_choose_main,&style_not_choose_lr_main);
        }
    }
}

static void lv_con_event_cb(lv_event_t * e)
{
    uint8_t i;
    lv_coord_t child_x_center;
    lv_point_t pos;
    lv_coord_t diff_y;
    lv_obj_t * child;
    lv_area_t child_a;
    static lv_coord_t cont_x_center = 0;
    lv_event_code_t code = lv_event_get_code(e);
    void * param = lv_event_get_param(e);
    if(code == LV_EVENT_SCROLL){
        if(!cont_x_center){
            if(con_src == NULL){
                printf("LV_EVENT_SCROLL cont_row is NULL\n");    
                return ;
            }
            lv_obj_get_coords(con_src, &child_a);//复制对象
            cont_x_center = child_a.x1 + lv_area_get_width(&child_a) / 2;//获取Y方向中心轴坐标    
        }
        lv_indev_get_point(param, &pos);        
        for(i = 0; i < 3; i++) {
            if(con_src == NULL){
                printf("LV_EVENT_CLICKED cont_row is NULL\n");    
                return ;
            }
            child = lv_obj_get_child(con_src, i);   
            if(child == NULL){
                printf("LV_EVENT_CLICKED child is NULL\n");    
                return ;
            }       
            lv_obj_get_coords(child, &child_a);//复制子对象

            child_x_center = child_a.x1 + lv_area_get_width(&child_a) / 2;//获取子对象Y方向中心轴坐标
            diff_y = cont_x_center - child_x_center;//获取子对象与父对象中心轴坐标差值
            diff_y = LV_ABS(diff_y);//取绝对值
            if(diff_y < 80){//中心图标
                lv_current_dot(i+1);
                break;
            }
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
    lv_obj_add_event_cb(con, lv_con_event_cb, LV_EVENT_SCROLL, NULL);  
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

        lab11 = lv_lab_creat_fun(btn11,btn11,LV_ALIGN_CENTER,0,0,"ON",LAB_COLOR_BLACK);
        lab12 = lv_lab_creat_fun(btn12,btn12,LV_ALIGN_CENTER,0,0,"OFF",LAB_COLOR_WHITE);
    }
    if(opt == HL_MOREONE_CHOOSE_RIGHT){
        btn_left_1 = 0;
        btn_right_1 = 1;
        btn1_l = lv_btn_lr_creat_fun(con_obj,con_obj,LV_ALIGN_LEFT_MID,-12,13,15,72,0);
        btn11 = lv_btn_creat_fun(con_obj,btn1_l,LV_ALIGN_OUT_RIGHT_MID,btn11_cb,-10,0,108,72,0);
        btn12 = lv_btn_creat_fun(con_obj,btn11,LV_ALIGN_OUT_RIGHT_MID,btn12_cb,0,0,108,72,1);
        btn1_r = lv_btn_lr_creat_fun(con_obj,btn12,LV_ALIGN_OUT_RIGHT_MID,-10,0,15,72,1);
        
        lab11 = lv_lab_creat_fun(btn11,btn11,LV_ALIGN_CENTER,0,0,"ON",LAB_COLOR_WHITE);
        lab12 = lv_lab_creat_fun(btn12,btn12,LV_ALIGN_CENTER,0,0,"OFF",LAB_COLOR_BLACK);
    }
    
    lab13 = lv_lab_creat_fun(con_obj,con_obj,LV_ALIGN_TOP_MID,-10,-10,"录制开关",LAB_COLOR_WHITE);
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

        lab21 = lv_lab_creat_fun(btn21,btn21,LV_ALIGN_CENTER,0,0,"ON",LAB_COLOR_BLACK);
        lab22 = lv_lab_creat_fun(btn22,btn22,LV_ALIGN_CENTER,0,0,"OFF",LAB_COLOR_WHITE);
    }
    if(opt == HL_MOREONE_CHOOSE_RIGHT){
        btn_left_2 = 0;
        btn_right_2 = 1;
        btn2_l = lv_btn_lr_creat_fun(con_obj,con_obj,LV_ALIGN_LEFT_MID,-12,13,15,72,0);
        btn21 = lv_btn_creat_fun(con_obj,btn2_l,LV_ALIGN_OUT_RIGHT_MID,btn21_cb,-10,0,108,72,0);
        btn22 = lv_btn_creat_fun(con_obj,btn21,LV_ALIGN_OUT_RIGHT_MID,btn22_cb,0,0,108,72,1);
        btn2_r = lv_btn_lr_creat_fun(con_obj,btn22,LV_ALIGN_OUT_RIGHT_MID,-10,0,15,72,1);

        lab21 = lv_lab_creat_fun(btn21,btn21,LV_ALIGN_CENTER,0,0,"ON",LAB_COLOR_WHITE);
        lab22 = lv_lab_creat_fun(btn22,btn22,LV_ALIGN_CENTER,0,0,"OFF",LAB_COLOR_BLACK);
    }

    lab23 = lv_lab_creat_fun(con_obj,con_obj,LV_ALIGN_TOP_MID,-10,-10,"MUTE",LAB_COLOR_WHITE);
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

        lab31 = lv_lab_creat_fun(btn31,btn31,LV_ALIGN_CENTER,0,0,"ON",LAB_COLOR_BLACK);
        lab32 = lv_lab_creat_fun(btn32,btn32,LV_ALIGN_CENTER,0,0,"OFF",LAB_COLOR_WHITE);
    } 
    if(opt == HL_MOREONE_CHOOSE_RIGHT){
        btn_left_3 = 0;
        btn_right_3 = 1;
        btn3_l = lv_btn_lr_creat_fun(con_obj,con_obj,LV_ALIGN_LEFT_MID,-12,13,15,72,0);
        btn31 = lv_btn_creat_fun(con_obj,btn3_l,LV_ALIGN_OUT_RIGHT_MID,btn31_cb,-10,0,108,72,0);
        btn32 = lv_btn_creat_fun(con_obj,btn31,LV_ALIGN_OUT_RIGHT_MID,btn32_cb,0,0,108,72,1);
        btn3_r = lv_btn_lr_creat_fun(con_obj,btn32,LV_ALIGN_OUT_RIGHT_MID,-10,0,15,72,1);

        lab31 = lv_lab_creat_fun(btn31,btn31,LV_ALIGN_CENTER,0,0,"ON",LAB_COLOR_WHITE);
        lab32 = lv_lab_creat_fun(btn32,btn32,LV_ALIGN_CENTER,0,0,"OFF",LAB_COLOR_BLACK);
    }    
    
    lab33 = lv_lab_creat_fun(con_obj,con_obj,LV_ALIGN_TOP_MID,-10,-10,"降噪开关",LAB_COLOR_WHITE);
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
    lv_style_reset(&style_screen);
    lv_style_reset(&style_white_dot);
    lv_style_reset(&style_grey_dot);
}

static void hl_option_mid_set(hl_moreone_mid_opt_t opt,lv_anim_enable_t anim_en)
{
    switch(opt){
        case HL_MOREONE_OPTION_ONE:
            lv_obj_scroll_to_view(con1, anim_en);
            lv_current_dot(1);
            break;
        case HL_MOREONE_OPTION_TWO:
            lv_obj_scroll_to_view(con2, anim_en);
            lv_current_dot(2);
            break;
        case HL_MOREONE_OPTION_THREE:
            lv_obj_scroll_to_view(con3, anim_en);
            lv_current_dot(3);
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
    if (!page_style_bit.page_moreone) {
        page_style_bit.page_moreone = 1;
        lv_style_page4_init();
    }

    con_src = lv_con_scr_creat(270,140);
    lv_obj_set_flex_align(con_src,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scroll_dir(con_src,LV_DIR_HOR);
    con1 = lv_con_child_creat(con_src,260,120);
    con2 = lv_con_child_creat(con_src,260,120);
    con3 = lv_con_child_creat(con_src,260,120);
    dot1 = lv_moreone_dot_creat_fun(lv_scr_act(),-20,0,0);
    dot2 = lv_moreone_dot_creat_fun(lv_scr_act(),0,0,0);
    dot3 = lv_moreone_dot_creat_fun(lv_scr_act(),20,0,0);
    lv_page_1_init(con1,ptr->moreone_choose_opt.option_one);
    lv_page_2_init(con2,ptr->moreone_choose_opt.option_two);
    lv_page_3_init(con3,ptr->moreone_choose_opt.option_three);
    hl_option_mid_set(ptr->moreone_mid_opt,LV_ANIM_OFF);
    device_lab = lv_lab_creat_fun(lv_scr_act(),lv_scr_act(),LV_ALIGN_TOP_LEFT,0,0,ptr->device_ptr,LAB_COLOR_WHITE);
    valid_obj[0] = btn11;
    valid_obj[1] = btn12;
    valid_obj[2] = btn21;
    valid_obj[3] = btn22;
    valid_obj[4] = btn31;
    valid_obj[5] = btn32;
}