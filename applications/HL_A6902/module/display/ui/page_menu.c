/**
 * @file page_menu.c
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief 菜单界面
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
#include "page_menu.h"
#include "language.h"
#include "page_style_bit.h"
#include <rtthread.h>
#include <rtdevice.h>
#define CHECK_POS_CENTRE(X,Y)    ((X >= 125) && (X < 175) )//&& (Y > 186) && (Y < 286)
#define CHECK_POS_LR(X,Y)    (X) < (125) ? 0 : 1 //0:lift 1:right

#define ICON_ZOOM_SET(X)    (X < 128 ? 128 : X)

#define LAB_ADD(X)  lab##X

#define ROLLER_TIMEOUT    70

#define MENU_CRUNT_WIDE 240
#define MENU_CRUNT_HIGH 80
// #define ICON_NUMBER     6
#define CENTRE_CHECK    1
#define RIGHT_CHECK     2
#define LIFT_CHECK      3

static event_cb func_cb;
static uint8_t ICON_NUMBER = 0,ICON_START_STATUS,ICON_END_STATUS;
static lv_obj_t *cont_row = NULL;
static lv_obj_t *lab_obj[MAX_MENU_NUMBER] = {RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL};
static lv_obj_t *pic_obj[MAX_MENU_NUMBER] = {RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL};
static int img_obj_xcoor[MAX_MENU_NUMBER];
static lv_style_t screen_style;
static lv_style_t style_label;

static lv_timer_t * timer;

static void lv_lab_hide_set(uint8_t lab_cur)
{    
    uint8_t i;
    for(i=0;i<ICON_NUMBER;i++){
        lv_obj_add_flag(lab_obj[i],LV_OBJ_FLAG_HIDDEN);
    }    
    lv_obj_clear_flag(lab_obj[lab_cur],LV_OBJ_FLAG_HIDDEN);
}

static uint8_t lv_center_icon_get(void)
{
    uint8_t i,l=0; 
    uint16_t j=0,k=0xFFFF; 
    lv_area_t tmp_t;    
    for(i = 0;i<ICON_NUMBER;i++){
        lv_obj_get_coords(pic_obj[i], &tmp_t);//复制子对象
        j = tmp_t.x1 + lv_area_get_width(&tmp_t) / 2;//获取对象Y方向中心轴坐标
        j = LV_ABS(j - 147);
        if(j < k){
            l = i;
            k = j;
        }
    }
    return l;
}

static void lv_icon_alin_mode(uint8_t num)
{
    if(num < 2){
        lv_obj_set_scroll_snap_x(cont_row, ICON_END_STATUS);
    }else{
        lv_obj_set_scroll_snap_x(cont_row, ICON_START_STATUS);
    }  
}
static int lv_icon_pos_get(uint8_t num)
{
    lv_area_t tmp_t;
    lv_obj_get_coords(pic_obj[num], &tmp_t);//复制子对象
    return (tmp_t.x1 + lv_area_get_width(&tmp_t) / 2);//获取对象Y方向中心轴坐标
}

static void menu_timer(lv_timer_t * timer)
{    
    int pos;
    static int pos_bak = 0;    
    uint8_t center = lv_center_icon_get();
    pos = lv_icon_pos_get(center);
    if(pos == pos_bak){
        lv_obj_set_scroll_snap_x(cont_row, LV_SCROLL_SNAP_CENTER);
        lv_obj_scroll_to_view(pic_obj[center], LV_ANIM_ON);
        lv_icon_alin_mode(center);     
        lv_timer_pause(timer); 
        lv_lab_hide_set(center);
    }else{
        pos_bak = pos;
    }
       
}

static void lv_inon_zoom_set(uint8_t num)
{
    int zoom;
    lv_area_t tmp_t;
    lv_obj_get_coords(pic_obj[num], &tmp_t);//复制子对象
    img_obj_xcoor[num] = tmp_t.x1 + lv_area_get_width(&tmp_t) / 2;//获取对象Y方向中心轴坐标
    zoom = ICON_ZOOM_SET(256 - (LV_ABS(img_obj_xcoor[num] - 147) * 2));
    lv_img_set_zoom(pic_obj[num],zoom);
}

static void lv_icon_check(lv_coord_t current,lv_coord_t x)
{
    uint8_t check_pos;
    if(x < 107){
        check_pos = LIFT_CHECK;
    }
    else if(x < 187){//0:lift 1:right
        check_pos = CENTRE_CHECK;
    }else{
        check_pos = RIGHT_CHECK;
    }
    if(check_pos == CENTRE_CHECK){
        func_cb(current);
    }
    if(check_pos == RIGHT_CHECK && current < (ICON_NUMBER - 1)){
        current+=1;       
        lv_obj_set_scroll_snap_x(cont_row, LV_SCROLL_SNAP_CENTER); 
        lv_obj_scroll_to_view(pic_obj[current], LV_ANIM_ON);   
        lv_icon_alin_mode(current);     
    }
    if(check_pos == LIFT_CHECK && current > 0){
        current-=1;     
        lv_obj_set_scroll_snap_x(cont_row, LV_SCROLL_SNAP_CENTER);    
        lv_obj_scroll_to_view(pic_obj[current], LV_ANIM_ON);
        lv_icon_alin_mode(current);  
    }
}

// FALSE 表示切换居中的图标，  true表示选中图标  都会触发中断
void lv_set_icon_postion(uint8_t pos, bool en)
{
    if (en == true) {
        lv_event_send(cont_row, LV_EVENT_KEY, NULL);
    } else {
        lv_obj_set_scroll_snap_x(cont_row, LV_SCROLL_SNAP_CENTER);    
        lv_obj_scroll_to_view(pic_obj[pos], LV_ANIM_ON);
        lv_icon_alin_mode(pos); 
    }    
}

static void lv_icon_event_cb(lv_event_t * e)
{    
    uint8_t i;    
    lv_point_t pos;
    lv_event_code_t code = lv_event_get_code(e);
    void * param = lv_event_get_param(e);
    if(code == LV_EVENT_SCROLL){
        for(i = 0;i<ICON_NUMBER;i++){
            lv_inon_zoom_set(i);
        }
        lv_lab_hide_set(lv_center_icon_get());
    }
    else if(code == LV_EVENT_SCROLL_END){
        lv_timer_resume(timer);
        lv_timer_reset(timer);
    }else if(code == LV_EVENT_CLICKED){
        lv_indev_get_point(param, &pos);  
        lv_icon_check(lv_center_icon_get(),pos.x);
    }else if (code == LV_EVENT_KEY){
        func_cb(lv_center_icon_get());
    }
}

static void lv_style_menu_init(void)
{    
    lv_style_init(&screen_style);
    lv_style_set_bg_color(&screen_style, lv_color_black());
    lv_style_set_radius(&screen_style, 0);
    lv_style_set_border_width(&screen_style, 0);
    // lv_obj_add_style(lv_scr_act(), &screen_style, 0);

    lv_style_init(&style_label);
    lv_style_set_bg_opa(&style_label, LV_OPA_TRANSP);
    lv_style_set_text_opa(&style_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_label, lv_color_white());
}

static lv_obj_t * lv_lab_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,lv_align_t align,lv_coord_t x_offset,lv_coord_t y_offset,uint8_t hide,const char * ptr)
{
    lv_obj_t * lab = lv_label_create(src_obj);
    lv_obj_add_style(lab, &style_label, LV_PART_MAIN);
    lv_obj_set_style_text_font(lab, &language24, 0);
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
static void lv_icon_list_init(int pic_num,menu_data_t *picdata,    int8_t center)
{
    uint8_t i;
    /*Now create the actual image*/
    cont_row = lv_obj_create(lv_scr_act());
    lv_obj_add_style(cont_row,&screen_style,0);
    lv_obj_set_size(cont_row, MENU_CRUNT_WIDE, MENU_CRUNT_HIGH);
    lv_obj_align(cont_row, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_flex_align(cont_row,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(cont_row,20,0);
    lv_obj_set_scroll_dir(cont_row,LV_DIR_HOR);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_scroll_snap_x(cont_row, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(cont_row, LV_SCROLLBAR_MODE_OFF);
    // lv_obj_clear_flag(cont_row,LV_OBJ_FLAG_SCROLL_MOMENTUM);
    for(i = 0; i <pic_num; i++){
        if(picdata[i].obj == RT_NULL){
            picdata[i].obj = lv_img_create(cont_row);
            if(picdata[i].obj == RT_NULL){
                LOG_E("img_obj[%d] creat fail\n",i);
            }else{
                lv_img_set_src(picdata[i].obj,picdata[i].pic_src);
            }
        }else{
            LOG_E("img_obj[%d] is not null\n",i);
        }        
        
        if(picdata[i].lab == RT_NULL){
            picdata[i].lab = lv_lab_creat_fun(lv_scr_act(),cont_row,LV_ALIGN_OUT_BOTTOM_MID,0,15,1,picdata[i].ptr);
            if(picdata[i].lab == RT_NULL){
                LOG_E("lab_obj[%d] creat fail\n",i);
            }
        }else{
            LOG_E("lab_obj[%d] is not null\n",i);
        }
        
    }
    lv_obj_scroll_to_view(lv_obj_get_child(cont_row, center), LV_ANIM_OFF);
    lv_obj_clear_flag(picdata[0].lab,LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(cont_row, lv_icon_event_cb, LV_EVENT_ALL, NULL);
}

void lv_menu_exit(void)
{  
    uint8_t i;
    for(i = 0;i < ICON_NUMBER;i++){
        if(pic_obj[i] == RT_NULL){
             LOG_E("img_obj[%d] is null\n",i);
        }
        if(lab_obj[i] == RT_NULL){
            LOG_E("lab_obj[%d] is null\n",i);
        }
    }
    lv_timer_del(timer);
    lv_obj_clean(lv_scr_act());
    rt_thread_mdelay(10);
    for(i = 0;i < ICON_NUMBER;i++){
        pic_obj[i] = RT_NULL;
        lab_obj[i] = RT_NULL;
    }
}

void page_menu_init(menu_data_t *data,uint8_t menu_num,event_cb func,int8_t center)
{
    uint8_t i;
    ICON_NUMBER = menu_num;
    func_cb = func;
    if (!page_style_bit.page_menu) {
        page_style_bit.page_menu = 1;
        lv_style_menu_init();          
    }
    lv_icon_list_init(ICON_NUMBER,data,center);
    for(i=0;i<ICON_NUMBER;i++){
        lab_obj[i] = data[i].lab;
        pic_obj[i] = data[i].obj;        
    }
    if(ICON_NUMBER > 3){
        ICON_START_STATUS = LV_SCROLL_SNAP_START;
        ICON_END_STATUS = LV_SCROLL_SNAP_END;
    }else{
        ICON_START_STATUS = LV_SCROLL_SNAP_CENTER;
        ICON_END_STATUS = LV_SCROLL_SNAP_CENTER;
    }
    lv_event_send(cont_row, LV_EVENT_SCROLL, NULL);
    lv_obj_set_scroll_snap_x(cont_row, ICON_START_STATUS);
    timer = lv_timer_create(menu_timer, ROLLER_TIMEOUT,  NULL);
    lv_timer_set_repeat_count(timer, -1);
    lv_timer_pause(timer);
}

lv_obj_t * hl_menu_obj_get(uint8_t num)
{
    return lab_obj[num];
}