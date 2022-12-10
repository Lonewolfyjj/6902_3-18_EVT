/**
 * @file page_top.c
 * @author your name (you@domain.com)
 * @brief 顶部菜单栏
 * @version 0.1
 * @date 2022-12-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "page_top.h"
#include "language.h"

#define POSTION_IS_NULL 0xFF

 typedef struct __icon_pos_t
{
    uint8_t duf_pos;//默认位置
    uint8_t cur_pos;//当前位置
    lv_align_t align;// 对齐方式
    lv_obj_t * icon;//图片对象
    const lv_img_dsc_t * icon_data;//图片数据指针
}icon_pos_t;

LV_IMG_DECLARE(Main_bat);//电池图标
LV_IMG_DECLARE(Main_line_in);//耳机插入
LV_IMG_DECLARE(Main_usb_c);//USB插入
LV_IMG_DECLARE(Main_stereo);//立体声
LV_IMG_DECLARE(Main_lock);//锁屏
LV_IMG_DECLARE(Main_heatset);//监听
LV_IMG_DECLARE(Main_noise);//降噪

#define ICON_POS_CURRENT      0
#define ICON_POS_DEFAULT      1
#define ICON_NUM    3
#define ICON_POS_VOR      2
#define ICON_POS_LIFT     0
#define ICON_POS_RIGHT    1

static lv_style_t style_power_bar_indicator,style_power_bar_main;
static lv_style_t style_power_label;

static lv_obj_t * bat_icon, * bat_bar, * bat_label;

static icon_pos_t icon_list_l[ICON_NUM] = 
{
    {.duf_pos = 0,.cur_pos = POSTION_IS_NULL,.align = LV_ALIGN_TOP_LEFT,.icon_data = &Main_stereo},
    {.duf_pos = 1,.cur_pos = POSTION_IS_NULL,.align = LV_ALIGN_TOP_LEFT,.icon_data = &Main_noise},
    {.duf_pos = 2,.cur_pos = POSTION_IS_NULL,.align = LV_ALIGN_TOP_LEFT,.icon_data = &Main_lock},
};

static icon_pos_t icon_list_r[ICON_NUM] = 
{
    {.duf_pos = 0,.cur_pos = POSTION_IS_NULL,.align = LV_ALIGN_TOP_LEFT,.icon_data = &Main_line_in},
    {.duf_pos = 1,.cur_pos = POSTION_IS_NULL,.align = LV_ALIGN_TOP_LEFT,.icon_data = &Main_usb_c},
    {.duf_pos = 2,.cur_pos = POSTION_IS_NULL,.align = LV_ALIGN_TOP_LEFT,.icon_data = &Main_heatset},
};

static uint8_t icon_pos_judge(uint8_t icon_pos,icon_pos_t *icon_list,uint8_t cmd_typ)
{
    uint8_t i;
    if(ICON_POS_DEFAULT == cmd_typ){//
        for(i=0;i<ICON_NUM;i++){
            if(icon_list[i].cur_pos == icon_pos){
                return icon_list[i].duf_pos;
            }
        }
    }
    if(ICON_POS_CURRENT == cmd_typ){
        for(i=0;i<ICON_NUM;i++){
            if(icon_list[i].duf_pos == icon_pos){
                return icon_list[i].cur_pos;
            }
        }
    }
    return 0;
}

static void delete_icon_pos_set(icon_pos_t *icon,icon_pos_t *icon_list,uint8_t duf_pos,uint8_t icon_typ)
{
    uint8_t i,is_used = 0;
    uint8_t icon_pos,icon_ar[ICON_NUM];
    uint16_t icon_offset;
    if(icon_typ == ICON_POS_LIFT){
        icon_offset = 0;
    }
    if(icon_typ == ICON_POS_RIGHT){
        icon_offset = 160;
    }

    for(i = 0; i < ICON_NUM;i++){
        if(icon_list[i].cur_pos != POSTION_IS_NULL){
            is_used++;//已经绘制的图标个数
        }
    }  
    if(is_used == 0){//
        return;
    }
    icon_pos = icon_pos_judge(duf_pos,icon_list,ICON_POS_CURRENT);//获取图标的当前位置
    if(icon_pos+1 == is_used){
        lv_obj_del(icon_list[duf_pos].icon);
        icon_list[duf_pos].cur_pos = POSTION_IS_NULL;
        return;
    }
    for(i = 0; i < is_used;i++){
        icon_ar[i] = icon_pos_judge(i,icon_list,ICON_POS_DEFAULT);//获取已绘制图标的默认位置排序
    }
    for(i = icon_pos+1; i < is_used;i++){
        icon_list[icon_ar[i]].cur_pos -= 1; 
        lv_obj_align(icon_list[icon_ar[i]].icon, icon_list[icon_ar[i]].align, icon_list[icon_ar[i]].cur_pos * 20 + icon_offset, ICON_POS_VOR);
    }
    // printf("icon_pos = %d\n",icon_pos);
    lv_obj_del(icon_list[duf_pos].icon);
    icon_list[icon_pos].cur_pos = POSTION_IS_NULL;
}

static void add_icon_pos_set(icon_pos_t *icon,icon_pos_t *icon_list,uint8_t icon_typ)
{
    uint8_t i,j,is_used = 0,new_f = 1;
    uint8_t icon_ar[ICON_NUM];
    uint16_t icon_offset;
    if(icon_typ == ICON_POS_LIFT){
        icon_offset = 0;
    }
    if(icon_typ == ICON_POS_RIGHT){
        icon_offset = 160;
    }

    for(i = 0; i < ICON_NUM;i++){
        if(icon_list[i].cur_pos != POSTION_IS_NULL){
            is_used++;//已经绘制的图标个数
        }
    }   
    if(is_used == 0){//
        icon[0].cur_pos = 0;//第一个位置绘制图标
        lv_obj_align(icon[0].icon, icon[0].align, 2 + icon_offset, ICON_POS_VOR);
        return;
    }
    for(i = 0; i < is_used;i++){
        icon_ar[i] = icon_pos_judge(i,icon_list,ICON_POS_DEFAULT);//获取已绘制图标的默认位置排序
    }
    for(i=0;i<is_used;i++){
        if(icon[0].duf_pos > icon_list[icon_ar[i]].duf_pos){//优先级低
            continue;
        }
        else if(icon[0].duf_pos == icon_list[icon_ar[i]].duf_pos){//重复绘制
            return;
        }
        else{
            new_f = 0;
            icon[0].cur_pos = icon_list[icon_ar[i]].cur_pos;            
            lv_obj_align(icon[0].icon, icon[0].align, icon[0].cur_pos * 20 + icon_offset, ICON_POS_VOR);
            for(j = i;j<is_used;j++){
                icon_list[icon_ar[j]].cur_pos += 1;                          
                lv_obj_align(icon_list[icon_ar[j]].icon, icon_list[icon_ar[j]].align, icon_list[icon_ar[j]].cur_pos * 20 + icon_offset, ICON_POS_VOR);
            }
            return;
        }        
    }
    if(new_f){
        icon[0].cur_pos = is_used;
        lv_obj_align(icon[0].icon, icon[0].align, icon[0].cur_pos * 20 + icon_offset, ICON_POS_VOR);
    }    
}

static void hl_mod_creat_top_icon(icon_pos_t * icon_data,icon_pos_t *icon_list,uint8_t icon_typ)
{
    if(icon_data->icon == NULL){
        icon_data->icon = lv_img_create(lv_scr_act());
        lv_img_set_src(icon_data->icon, icon_data->icon_data);
        lv_img_set_zoom(icon_data->icon, 256);
        add_icon_pos_set(icon_data,icon_list,icon_typ);
    }else{
        add_icon_pos_set(icon_data,icon_list,icon_typ);
    }
}

static void lv_style_page_top_init(void)
{
    static lv_style_t style;    
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_color_black());
    lv_style_set_border_width(&style, 0);
    lv_obj_add_style(lv_scr_act(), &style, 0);

    lv_style_init(&style_power_bar_indicator);
    lv_style_set_bg_opa(&style_power_bar_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_power_bar_indicator, lv_color_white());
    lv_style_set_radius(&style_power_bar_indicator, 0);

    lv_style_init(&style_power_bar_main);
    lv_style_set_bg_opa(&style_power_bar_main, LV_OPA_TRANSP);
    lv_style_set_radius(&style_power_bar_main, 0);

    lv_style_init(&style_power_label);
    lv_style_set_bg_opa(&style_power_label, LV_OPA_COVER);
    lv_style_set_bg_color(&style_power_label, lv_color_black());
    lv_style_set_text_opa(&style_power_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_power_label, lv_color_white());
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
    return bar;
}

static lv_obj_t * lv_power_img_creat_fun(lv_obj_t *align_obj,lv_coord_t x_offset,lv_coord_t y_offset,uint16_t zoom)
{
    lv_obj_t * img = lv_img_create(align_obj);
    lv_img_set_src(img,&Main_bat);
    lv_img_set_zoom(img, zoom);
    lv_obj_align_to(img,align_obj,LV_ALIGN_TOP_RIGHT,x_offset,y_offset);
    return img;
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

// static void lv_creat_top_bat_icon(void)
// {
//     bat_icon = lv_power_img_creat_fun(lv_scr_act(),0,0,256);
//     bat_bar = lv_power_bar_creat_fun(bat_icon,2,0,22,10,100);
//     bat_label = lv_power_lab_creat_fun(lv_scr_act(),bat_bar,bat_bar,-6,0);
// }
/*
    hl_mod_creat_top_icon(&icon_list_l[0],icon_list_l,ICON_POS_LIFT);
    hl_mod_creat_top_icon(&icon_list_l[1],icon_list_l,ICON_POS_LIFT);
    hl_mod_creat_top_icon(&icon_list_l[2],icon_list_l,ICON_POS_LIFT);
    hl_mod_creat_top_icon(&icon_list_r[0],icon_list_r,ICON_POS_RIGHT);
    hl_mod_creat_top_icon(&icon_list_r[1],icon_list_r,ICON_POS_RIGHT);
    hl_mod_creat_top_icon(&icon_list_r[2],icon_list_r,ICON_POS_RIGHT);
*/
static void hl_add_top_icon(hl_top_icon_t icon)
{
    switch(icon){
        case HL_TOP_ICON_VOICE_MOD:
            hl_mod_creat_top_icon(&icon_list_l[0],icon_list_l,ICON_POS_LIFT);
            break;
        case HL_TOP_ICON_NOISE:
            hl_mod_creat_top_icon(&icon_list_l[1],icon_list_l,ICON_POS_LIFT);
            break;
        case HL_TOP_ICON_LOCK:
            hl_mod_creat_top_icon(&icon_list_l[2],icon_list_l,ICON_POS_LIFT);
            break;
        case HL_TOP_ICON_LINEOUT:
            hl_mod_creat_top_icon(&icon_list_r[0],icon_list_r,ICON_POS_RIGHT);
            break;
        case HL_TOP_ICON_TYPEC:
            hl_mod_creat_top_icon(&icon_list_r[1],icon_list_r,ICON_POS_RIGHT);
            break;
        case HL_TOP_ICON_HEATSET:
            hl_mod_creat_top_icon(&icon_list_r[2],icon_list_r,ICON_POS_RIGHT);
            break;
        default:
            break;
    }
}

static void hl_delete_top_icon(hl_top_icon_t icon)
{
    switch(icon){
        case HL_TOP_ICON_VOICE_MOD:
            delete_icon_pos_set(&icon_list_l[0],icon_list_l,0,ICON_POS_LIFT);
            break;
        case HL_TOP_ICON_NOISE:
            delete_icon_pos_set(&icon_list_l[1],icon_list_l,1,ICON_POS_LIFT);
            break;
        case HL_TOP_ICON_LOCK:
            delete_icon_pos_set(&icon_list_l[2],icon_list_l,2,ICON_POS_LIFT);
            break;
        case HL_TOP_ICON_LINEOUT:
            delete_icon_pos_set(&icon_list_r[0],icon_list_r,0,ICON_POS_RIGHT);
            break;
        case HL_TOP_ICON_TYPEC:
            delete_icon_pos_set(&icon_list_r[1],icon_list_r,1,ICON_POS_RIGHT);
            break;
        case HL_TOP_ICON_HEATSET:
            delete_icon_pos_set(&icon_list_r[2],icon_list_r,2,ICON_POS_RIGHT);
            break;
        default:
            break;
    }
}

void hl_mod_top_ioctl(void * ctl_data)
{
    hl_lvgl_top_ioctl_t * ptr = (hl_lvgl_top_ioctl_t *)ctl_data;
    switch(ptr->top_cmd){
        case HL_TOP_ADD_ICON_CMD:
            hl_add_top_icon(ptr->top_param);
            break;
        case HL_TOP_DELETE_ICON_CMD:
            hl_delete_top_icon(ptr->top_param);
            break;
        default:
            break;
    }
}

void hl_mod_top_init(void * init_data)
{ 
    hl_lvgl_top_init_t * ptr = (hl_lvgl_top_init_t *)init_data;
    lv_style_page_top_init();

    bat_icon = lv_power_img_creat_fun(lv_scr_act(),0,0,256);
    bat_bar = lv_power_bar_creat_fun(bat_icon,2,0,22,10,ptr->electric_top);
    bat_label = lv_power_lab_creat_fun(lv_scr_act(),bat_bar,bat_bar,-6,0);

    // lv_creat_top_bat_icon();
    
}


// //测试接口
// void page_top_test(void)
// {
//     hl_mod_top_icon_init();
// }