/**
 * @file hl_mod_page_menu_frame.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-12-03
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
 * <tr><td>2022-12-03     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/


#include "hl_mod_page_menu_frame.h"
#include "lvgl.h"

LV_FONT_DECLARE(language);

#define CHECK_POS_CENTRE(X,Y)    ((X >= 125) && (X < 175) )//&& (Y > 186) && (Y < 286)
#define CHECK_POS_LR(X,Y)    (X) < (125) ? 0 : 1 //0:lift 1:right
#define LAB_ADD(X)  lab##X

#define MENU_CRUNT_WIDE 200
#define MENU_CRUNT_HIGH 100
// #define ICON_NUMBER     6
#define CENTRE_CHECK    1
#define RIGHT_CHECK     2
#define LIFT_CHECK      3

static event_cb func_cb;
static uint8_t ICON_NUMBER = 0;
static lv_obj_t *cont_row;
static lv_obj_t *lab_obj[MAX_MENU_NUMBER];

static void lv_lab_hide_set(uint8_t lab_cur)
{    
    uint8_t i;
    for(i=0;i<ICON_NUMBER;i++){
        lv_obj_add_flag(lab_obj[i],LV_OBJ_FLAG_HIDDEN);
    }    
    lv_obj_clear_flag(lab_obj[lab_cur],LV_OBJ_FLAG_HIDDEN);
}

static lv_coord_t get_distance(lv_coord_t data)
{
    if(data > 128){
        return 0;
    }
    return (128 - (data));
}

static void icon_pos_changed(lv_coord_t current,lv_coord_t x,lv_coord_t y)
{
    uint8_t check_pos;
    if(CHECK_POS_CENTRE(x,y)){
        check_pos = CENTRE_CHECK;
    }
    else if(CHECK_POS_LR(x,y)){//0:lift 1:right
        check_pos = RIGHT_CHECK;
    }else{
        check_pos = LIFT_CHECK;
    }
    if(check_pos == CENTRE_CHECK){
        // printf("Check centre icon event\n");
        func_cb(current);
    }
    if(check_pos == RIGHT_CHECK && current < (ICON_NUMBER - 1)){
        current+=1;
        lv_obj_scroll_to_view(lv_obj_get_child(cont_row, current), LV_ANIM_ON);
        // lv_lab_hide_set(current);
    }
    if(check_pos == LIFT_CHECK && current > 0){
        current-=1;
        lv_obj_scroll_to_view(lv_obj_get_child(cont_row, current), LV_ANIM_ON);
        // lv_lab_hide_set(current);
    }
    // printf("current = %d  check_pos = %d\n",current,check_pos);
}


static void lv_icon_event_cb(lv_event_t * e)
{
    uint8_t i;
    static uint32_t child_cnt = 0;
    lv_area_t cont_a;
    lv_area_t child_a;
    lv_obj_t * child;
    lv_coord_t diff_y;
    lv_point_t pos;
    lv_coord_t child_x_center;
    static lv_coord_t cont_x_center = 0;
    lv_event_code_t code = lv_event_get_code(e);
    void * param = lv_event_get_param(e);
    if(code == LV_EVENT_SCROLL){
        if(!cont_x_center){
            lv_obj_get_coords(cont_row, &cont_a);//复制对象
            cont_x_center = cont_a.x1 + lv_area_get_width(&cont_a) / 2;//获取Y方向中心轴坐标    
            child_cnt = lv_obj_get_child_cnt(cont_row);//获取子对象数量
        }
        for(i = 0; i < child_cnt; i++) {
            child = lv_obj_get_child(cont_row, i);        
            lv_obj_get_coords(child, &child_a);//复制子对象

            child_x_center = child_a.x1 + lv_area_get_width(&child_a) / 2;//获取子对象Y方向中心轴坐标
            diff_y = cont_x_center - child_x_center;//获取子对象与父对象中心轴坐标差值
            diff_y = LV_ABS(diff_y);//取绝对值
            lv_img_set_zoom(child,128 + get_distance(diff_y));
            if(diff_y < 10){//中心图标
                lv_lab_hide_set(i);
            }
        }  
    }
    if(code == LV_EVENT_CLICKED){    
        // printf("current = %d  check_pos = %d\n",0,0);    
        lv_indev_get_point(param, &pos);        
        for(i = 0; i < child_cnt; i++) {
            child = lv_obj_get_child(cont_row, i);        
            lv_obj_get_coords(child, &child_a);//复制子对象

            child_x_center = child_a.x1 + lv_area_get_width(&child_a) / 2;//获取子对象Y方向中心轴坐标
            diff_y = cont_x_center - child_x_center;//获取子对象与父对象中心轴坐标差值
            diff_y = LV_ABS(diff_y);//取绝对值
            if(diff_y == 0){//中心图标
                icon_pos_changed(i,pos.x,pos.y);
                break;
            }
        }
    }
}

static lv_style_t screen_style;
static lv_style_t style_label;
static void lv_style_menu_init(void)
{    
    lv_style_init(&screen_style);
    lv_style_set_bg_color(&screen_style, lv_color_black());
    lv_style_set_radius(&screen_style, 0);
    lv_style_set_border_width(&screen_style, 0);
    lv_obj_add_style(lv_scr_act(), &screen_style, 0);

    lv_style_init(&style_label);
    lv_style_set_bg_opa(&style_label, LV_OPA_TRANSP);
    lv_style_set_text_opa(&style_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_label, lv_color_white());
}

static lv_obj_t * lv_lab_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,lv_align_t align,lv_coord_t x_offset,lv_coord_t y_offset,uint8_t hide,const char * ptr)
{
    lv_obj_t * lab = lv_label_create(src_obj);
    lv_obj_add_style(lab, &style_label, LV_PART_MAIN);
    lv_obj_set_style_text_font(lab, &language, 0);
    lv_label_set_text(lab,ptr);
    lv_obj_align_to(lab,align_obj,align,x_offset,y_offset);
    if(hide){
        lv_obj_add_flag(lab,LV_OBJ_FLAG_HIDDEN);
    }
    return lab;
}

/**
 * Show transformations (zoom and rotation) using a pivot point.
 */
static void lv_icon_list_init(int pic_num,menu_data_t *picdata)
{
    uint8_t i;
    /*Now create the actual image*/
    cont_row = lv_obj_create(lv_scr_act());
    lv_obj_add_style(cont_row,&screen_style,0);
    lv_obj_set_size(cont_row, MENU_CRUNT_WIDE, MENU_CRUNT_HIGH);
    lv_obj_align(cont_row, LV_ALIGN_CENTER, 0, -10);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_scroll_snap_x(cont_row, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(cont_row, LV_SCROLLBAR_MODE_OFF);
    // lv_obj_add_event_cb(cont_row, lv_icon_event_cb, LV_EVENT_ALL, NULL);
    
    for(i = 0; i <pic_num; i++){
        picdata[i].obj = lv_img_create(cont_row);
        lv_img_set_src(picdata[i].obj,picdata[i].pic_src);
        lv_img_set_zoom(picdata[i].obj,128);
        picdata[i].lab = lv_lab_creat_fun(lv_scr_act(),cont_row,LV_ALIGN_OUT_BOTTOM_MID,0,0,1,picdata[i].ptr);
    }    
    lv_img_set_zoom(picdata[0].obj,256);   
    // lv_event_send(cont_row, LV_EVENT_SCROLL, NULL); 
    lv_obj_scroll_to_view(lv_obj_get_child(cont_row, 0), LV_ANIM_OFF);
    lv_obj_add_flag(cont_row, LV_OBJ_FLAG_SCROLL_ONE);
    lv_obj_clear_flag(picdata[0].lab,LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(cont_row, lv_icon_event_cb, LV_EVENT_ALL, NULL);
}

void page_menu_init(menu_data_t *data,uint8_t menu_num,event_cb func)
{
    uint8_t i;
    ICON_NUMBER = menu_num;
    func_cb = func;
    lv_style_menu_init();    
    lv_icon_list_init(ICON_NUMBER,data);
    for(i=0;i<ICON_NUMBER;i++){
        lab_obj[i] = data[i].lab;
    }
    lv_event_send(cont_row, LV_EVENT_SCROLL, NULL);
}
/* Includes ------------------------------------------------------------------*/
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/*
 * EOF
 */