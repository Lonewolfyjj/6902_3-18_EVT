/**
 * @file page_date.c
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief 日期设置界面
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
#include "page_date.h"
#include "language.h"
#include "page_style_bit.h"

#define ROLLER_YEAR  -96
#define ROLLER_MONTH  2
#define ROLLER_DAY  100
#define ROLLER_HOUR  198
#define ROLLER_MIN  296

#define COMMON_YEAR 0
#define LEAP_YEAR 1

#define INIT_TYPE 0
#define IOCTL_TYPE 1

#define CENTER_ERR_RANGE    40
#define ROLLER_TIMEOUT    250
//平年的月份日期表
static uint8_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

static uint8_t date_year[256];
static uint8_t date_month[300];
static uint8_t date_day[256];
static uint8_t date_hour[256];
static uint8_t date_min[350];

static lv_coord_t pos;
static lv_style_t style_black,style_grey;
static lv_obj_t *roller_year,*roller_month,*roller_day,*roller_hour,*roller_min;
static lv_obj_t * con;
static lv_timer_t * timer;
static char * lock = "btn";
static char * ulock = "ubtn";
static hl_date_event_cb hl_date_func;

static uint8_t leap_flag = COMMON_YEAR;//0 :平年 1：闰年
static uint8_t month_sta = 0,timer_flag = 0;
/**
 * Add a fade mask to roller.
 */

static void lv_year_init(void)
{
    char buf[10] = {0,0,0,0,0,0,0,0,0,0};
    uint16_t i;
    for(i=2000;i<2051;i++){
        if(i == 2050){
            lv_snprintf(buf, sizeof(buf), "%d", i);
        }else{
            lv_snprintf(buf, sizeof(buf), "%d\n", i);
        }  
        strcat(date_year,buf);
    }
}
static void lv_month_init(void)
{
    char buf[10] = {0,0,0,0,0,0,0,0,0,0};
    uint16_t i;
    for(i=1;i<13;i++){
        if(i == 12){
            lv_snprintf(buf, sizeof(buf), "%d月", i);
        }else{
            lv_snprintf(buf, sizeof(buf), "%d月\n", i);
        }  
        strcat(date_month,buf);
    }
}
static void lv_day_init(uint8_t day)
{
    char buf[10] = {0,0,0,0,0,0,0,0,0,0};
    uint16_t i;
    memset(date_day, 0, sizeof(date_day));
    for(i=1;i<(day + 1);i++){
        if(i == day){
            lv_snprintf(buf, sizeof(buf), "%d日", i);
        }else{
            lv_snprintf(buf, sizeof(buf), "%d日\n", i);
        }  
        strcat(date_day,buf);
    }
}
static void lv_hour_init(void)
{
    char buf[10] = {0,0,0,0,0,0,0,0,0,0};
    uint16_t i;
    for(i=0;i<24;i++){
        if(i == 23){
            lv_snprintf(buf, sizeof(buf), "%d时", i);
        }else{
            lv_snprintf(buf, sizeof(buf), "%d时\n", i);
        }  
        strcat(date_hour,buf);
    }
}
static void lv_min_init(void)
{
    char buf[10] = {0,0,0,0,0,0,0,0,0,0};
    uint16_t i;
    for(i=0;i<60;i++){
        if(i == 59){
            lv_snprintf(buf, sizeof(buf), "%d分", i);
        }else{
            lv_snprintf(buf, sizeof(buf), "%d分\n", i);
        }        
        strcat(date_min,buf);
    }
}

static void lv_date_init(void)
{
    lv_year_init();
    lv_month_init();
    lv_day_init(30);
    lv_hour_init();
    lv_min_init();
}

//0 :平年 1：闰年
static uint8_t lv_leap_year(uint16_t year)
{			  
	if(year%4==0){//必须能被4整除
		if(year%100==0) { 
			if(year%400==0)return LEAP_YEAR;//如果以00结尾,还要能被400整除 	   
			else return COMMON_YEAR;   
		}else return LEAP_YEAR;   
	}else return COMMON_YEAR;	
}

static void lv_get_cal_day_num(hl_date_choose_t opt,uint8_t data)
{
    uint16_t tmp_data = data;
    uint8_t pos;
    if(opt == HL_DATE_YEAR){
        tmp_data += 2000;
        leap_flag = lv_leap_year(tmp_data);
        if(leap_flag){
            mon_table[1] = 29;
        }else{
            mon_table[1] = 28;
        }
    }
    if(opt == HL_DATE_MONTH){
        month_sta = data;
    }
    lv_day_init(mon_table[month_sta-1]);
    pos = lv_roller_get_selected(roller_day);    
    lv_roller_set_options(roller_day,date_day,LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(roller_day,pos,LV_ANIM_ON);
}

static void lv_style_page6_init(void)
{    
    lv_style_init(&style_black);
    lv_style_set_bg_opa(&style_black, LV_OPA_COVER);
    lv_style_set_bg_color(&style_black, lv_color_black());
    lv_style_set_border_width(&style_black, 0);
    lv_style_set_pad_all(&style_black, 0);
    lv_obj_add_style(lv_scr_act(), &style_black, 0);
    lv_obj_set_scrollbar_mode(lv_scr_act(), LV_SCROLLBAR_MODE_OFF);  

    lv_style_init(&style_grey);
    lv_style_set_bg_opa(&style_grey, LV_OPA_COVER);
    lv_style_set_bg_color(&style_grey, lv_palette_darken(LV_PALETTE_GREY,2));
    lv_style_set_border_width(&style_grey,0);
    lv_style_set_outline_width(&style_grey,0);
    lv_style_set_radius(&style_grey, 3);
}

static lv_obj_t * lv_creat_roller(lv_obj_t *src_obj,lv_event_cb_t event_cb,const char * options,lv_coord_t x_offset,lv_coord_t y_offset)
{ 
    lv_obj_t *roller = lv_roller_create(src_obj);
    lv_obj_add_style(roller, &style_grey, 0);    
    lv_obj_set_style_bg_opa(roller, LV_OPA_TRANSP, LV_PART_SELECTED);
    lv_obj_set_style_bg_opa(roller, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_size(roller,90,120); 
    // lv_obj_align(roller, LV_ALIGN_CENTER, x_offset, y_offset);
// #if LV_FONT_MONTSERRAT_22
    lv_obj_set_style_text_font(roller, &language24, LV_PART_SELECTED);   
    lv_obj_set_style_text_color(roller,lv_color_white(),LV_PART_SELECTED); 
// #endif
    // lv_obj_set_style_text_font(roller, &language, LV_PART_MAIN);
    lv_obj_set_style_text_color(roller,lv_palette_main(LV_PALETTE_GREY),LV_PART_MAIN); 
    lv_roller_set_options(roller,options,LV_ROLLER_MODE_NORMAL);
    lv_obj_set_align(roller,LV_ALIGN_CENTER);
    // lv_roller_set_visible_row_count(roller, 3);
    lv_obj_clear_state(roller,LV_STATE_FOCUS_KEY);
    lv_obj_clear_flag(roller,LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_add_event_cb(roller, event_cb, LV_EVENT_ALL, NULL);    
    return roller;
}

static void roller_year_cb(lv_event_t * e)
{    
    char * ptr = (char *)lv_event_get_param(e);
    lv_event_code_t code = lv_event_get_code(e);
    lv_coord_t pos = lv_roller_get_selected(roller_year);

    if(code == LV_EVENT_CLICKED) {        
        if(strcmp(ptr,lock)){
            lv_obj_scroll_to_view(roller_year, LV_ANIM_ON);    
            lv_obj_set_style_bg_opa(roller_year, LV_OPA_COVER, LV_PART_MAIN);
            lv_event_send(roller_min,LV_EVENT_CLICKED,lock);
            lv_event_send(roller_month,LV_EVENT_CLICKED,lock);
            lv_event_send(roller_day,LV_EVENT_CLICKED,lock);
            lv_event_send(roller_hour,LV_EVENT_CLICKED,lock);
            lv_get_cal_day_num(HL_DATE_YEAR,pos);
            hl_date_func(HL_DATE_YEAR,pos+2000);
        }else{      
            lv_obj_set_style_bg_opa(roller_year, LV_OPA_TRANSP, LV_PART_MAIN);
        }
    }    
}
static void roller_month_cb(lv_event_t * e)
{    
    char * ptr = (char *)lv_event_get_param(e);
    lv_event_code_t code = lv_event_get_code(e);
    lv_coord_t pos = lv_roller_get_selected(roller_month);

    if(code == LV_EVENT_CLICKED) {        
        if(strcmp(ptr,lock)){
            lv_obj_scroll_to_view(roller_month, LV_ANIM_ON);    
            lv_obj_set_style_bg_opa(roller_month, LV_OPA_COVER, LV_PART_MAIN);
            lv_event_send(roller_year,LV_EVENT_CLICKED,lock);
            lv_event_send(roller_min,LV_EVENT_CLICKED,lock);
            lv_event_send(roller_day,LV_EVENT_CLICKED,lock);
            lv_event_send(roller_hour,LV_EVENT_CLICKED,lock);
            lv_get_cal_day_num(HL_DATE_MONTH,pos);
            hl_date_func(HL_DATE_MONTH,pos+1);
        }else{      
            lv_obj_set_style_bg_opa(roller_month, LV_OPA_TRANSP, LV_PART_MAIN);
        }
    } 
}
static void roller_day_cb(lv_event_t * e)
{    
    char * ptr = (char *)lv_event_get_param(e);
    lv_event_code_t code = lv_event_get_code(e);
    lv_coord_t pos = lv_roller_get_selected(roller_day);

    if(code == LV_EVENT_CLICKED) {        
        if(strcmp(ptr,lock)){
            lv_obj_scroll_to_view(roller_day, LV_ANIM_ON);    
            lv_obj_set_style_bg_opa(roller_day, LV_OPA_COVER, LV_PART_MAIN);
            lv_event_send(roller_year,LV_EVENT_CLICKED,lock);
            lv_event_send(roller_month,LV_EVENT_CLICKED,lock);
            lv_event_send(roller_min,LV_EVENT_CLICKED,lock);
            lv_event_send(roller_hour,LV_EVENT_CLICKED,lock);
            hl_date_func(HL_DATE_DAY,pos+1);
        }else{      
            lv_obj_set_style_bg_opa(roller_day, LV_OPA_TRANSP, LV_PART_MAIN);
        }
    }
}
static void roller_hour_cb(lv_event_t * e)
{  
    char * ptr = (char *)lv_event_get_param(e);
    lv_event_code_t code = lv_event_get_code(e);
    lv_coord_t pos = lv_roller_get_selected(roller_hour);

    if(code == LV_EVENT_CLICKED) {        
        if(strcmp(ptr,lock)){
            lv_obj_scroll_to_view(roller_hour, LV_ANIM_ON);    
            lv_obj_set_style_bg_opa(roller_hour, LV_OPA_COVER, LV_PART_MAIN);
            lv_event_send(roller_year,LV_EVENT_CLICKED,lock);
            lv_event_send(roller_month,LV_EVENT_CLICKED,lock);
            lv_event_send(roller_day,LV_EVENT_CLICKED,lock);
            lv_event_send(roller_min,LV_EVENT_CLICKED,lock);
            hl_date_func(HL_DATE_HOUR,pos);
        }else{      
            lv_obj_set_style_bg_opa(roller_hour, LV_OPA_TRANSP, LV_PART_MAIN);
        }
    }  
}
static void roller_min_cb(lv_event_t * e)
{    
    char * ptr = (char *)lv_event_get_param(e);
    lv_event_code_t code = lv_event_get_code(e);
    lv_coord_t pos = lv_roller_get_selected(roller_min);

    if(code == LV_EVENT_CLICKED) {        
        if(strcmp(ptr,lock)){
            lv_obj_scroll_to_view(roller_min, LV_ANIM_ON);    
            lv_obj_set_style_bg_opa(roller_min, LV_OPA_COVER, LV_PART_MAIN);
            lv_event_send(roller_year,LV_EVENT_CLICKED,lock);
            lv_event_send(roller_month,LV_EVENT_CLICKED,lock);
            lv_event_send(roller_day,LV_EVENT_CLICKED,lock);
            lv_event_send(roller_hour,LV_EVENT_CLICKED,lock);
            hl_date_func(HL_DATE_MIN,pos);
        }else{      
            lv_obj_set_style_bg_opa(roller_min, LV_OPA_TRANSP, LV_PART_MAIN);
        }
    } 
}

static void roller_timer(lv_timer_t * timer)
{
    if(timer_flag){
        if(LV_ABS(ROLLER_YEAR - pos) < CENTER_ERR_RANGE){
            lv_event_send(roller_year,LV_EVENT_CLICKED,ulock);
        }
        if(LV_ABS(ROLLER_MONTH - pos) < CENTER_ERR_RANGE){
            lv_event_send(roller_month,LV_EVENT_CLICKED,ulock);
        }
        if(LV_ABS(ROLLER_DAY - pos) < CENTER_ERR_RANGE){
            lv_event_send(roller_day,LV_EVENT_CLICKED,ulock);
        }
        if(LV_ABS(ROLLER_HOUR - pos) < CENTER_ERR_RANGE){
            lv_event_send(roller_hour,LV_EVENT_CLICKED,ulock);
        }
        if(LV_ABS(ROLLER_MIN - pos) < CENTER_ERR_RANGE){
            lv_event_send(roller_min,LV_EVENT_CLICKED,ulock);
        }
        timer_flag = 0;
    }
}

static void roller_con_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    pos = lv_obj_get_scroll_x(con);
    lv_timer_reset(timer);
    if(code == LV_EVENT_SCROLL_END){
        timer_flag = 1;
    }
}

static void hl_current_option_set(hl_date_choose_t opt)
{
    switch (opt){
        case HL_DATE_YEAR:
            lv_event_send(roller_year,LV_EVENT_CLICKED,ulock);
            break;
        case HL_DATE_MONTH:
            lv_event_send(roller_month,LV_EVENT_CLICKED,ulock);
            break;
        case HL_DATE_DAY:
            lv_event_send(roller_day,LV_EVENT_CLICKED,ulock);
            break;
        case HL_DATE_HOUR:
            lv_event_send(roller_hour,LV_EVENT_CLICKED,ulock);
            break;
        case HL_DATE_MIN:
            lv_event_send(roller_min,LV_EVENT_CLICKED,ulock);
            break;
        default:
            break;
    }
}

static void hl_opt_value_set(hl_date_choose_t opt,int16_t value,lv_anim_enable_t anim,uint8_t sta)
{
    switch (opt){
        case HL_DATE_YEAR:
            lv_roller_set_selected(roller_year,value - 2000,anim);
            break;
        case HL_DATE_MONTH:
            lv_roller_set_selected(roller_month,value-sta,anim);
            break;
        case HL_DATE_DAY:
            lv_roller_set_selected(roller_day,value-sta,anim);
            break;
        case HL_DATE_HOUR:
            lv_roller_set_selected(roller_hour,value,anim);
            break;
        case HL_DATE_MIN:
            lv_roller_set_selected(roller_min,value,anim);
            break;
        default:
            break;
    }
}

static void hl_mod_date_init_cfg(hl_lvgl_date_init_t * data)
{
    hl_current_option_set(data->current_choose);
    month_sta = data->month;    
    lv_get_cal_day_num(HL_DATE_YEAR,data->year);
    hl_opt_value_set(HL_DATE_YEAR,data->year,LV_ANIM_OFF,INIT_TYPE);
    hl_opt_value_set(HL_DATE_MONTH,data->month - 1,LV_ANIM_OFF,INIT_TYPE);
    hl_opt_value_set(HL_DATE_DAY,data->day-1,LV_ANIM_OFF,INIT_TYPE);
    hl_opt_value_set(HL_DATE_HOUR,data->hour,LV_ANIM_OFF,INIT_TYPE);
    hl_opt_value_set(HL_DATE_MIN,data->min,LV_ANIM_OFF,INIT_TYPE);
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

void hl_mod_date_ioctl(void * ctl_data)
{
    hl_lvgl_date_ioctl_t * ptr = (hl_lvgl_date_ioctl_t *)ctl_data;
    switch (ptr->date_cmd){
        case HL_DATE_SET_OPT_CMD:
            hl_current_option_set(ptr->opt);
            break;
        case HL_DATE_SET_VALUE_CMD:
            hl_opt_value_set(ptr->opt,ptr->param,LV_ANIM_ON,IOCTL_TYPE);
            break;
        case HL_DATE_EXTI_CMD:
            lv_timer_del(timer);
            hl_obj_delete(lv_scr_act(),false);
            break;
        default:
            break;
    }
}


void hl_mod_date_init(void * init_data)
{
    if (!page_style_bit.page_date) {
        page_style_bit.page_date = 1;
        lv_date_init();
        lv_style_page6_init();
    }

    hl_lvgl_date_init_t * ptr = (hl_lvgl_date_init_t *)init_data;
    hl_date_func = ptr->func_cb;

    con = lv_obj_create(lv_scr_act());
    lv_obj_add_style(con, &style_black, 0);
    lv_obj_set_scroll_dir(con,LV_DIR_HOR);
    lv_obj_set_size(con,282,120);
    lv_obj_align(con, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_flex_flow(con, LV_FLEX_FLOW_ROW);
    lv_obj_set_scroll_snap_x(con, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(con, LV_SCROLLBAR_MODE_OFF);   
    lv_obj_add_event_cb(con, roller_con_cb, LV_EVENT_ALL, NULL);       
    timer = lv_timer_create(roller_timer, ROLLER_TIMEOUT,  NULL);
    lv_timer_set_repeat_count(timer, -1);
    roller_year = lv_creat_roller(con,roller_year_cb,date_year,10,0);
    roller_month = lv_creat_roller(con,roller_month_cb,date_month,20,0);
    roller_day = lv_creat_roller(con,roller_day_cb,date_day,20,0);
    roller_hour = lv_creat_roller(con,roller_hour_cb,date_hour,20,0);
    roller_min = lv_creat_roller(con,roller_min_cb,date_min,20,0);

    hl_mod_date_init_cfg(ptr);
}