/**
 * @file page_verson.c
 * @author your name (you@domain.com)
 * @brief 版本号显示界面
 * @version 0.1
 * @date 2022-12-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "page_verson.h"
#include "language.h"

static lv_style_t style_label;
static lv_style_t style_screen;

static lv_obj_t * lab11,*lab12,*lab13;
static lv_obj_t * lab21,*lab22,*lab23;
static lv_obj_t * lab31,*lab32,*lab33;
static lv_obj_t * con1,*con2,*con3,*con_src;

static void lv_style_page4_init(void)
{
    lv_obj_enable_style_refresh(true);

    lv_style_init(&style_screen);
    lv_style_set_bg_color(&style_screen, lv_color_black());
    lv_style_set_border_width(&style_screen, 0);
    lv_style_set_outline_width(&style_screen,0);
    lv_style_set_radius(&style_screen, 0);
    lv_obj_add_style(lv_scr_act(), &style_screen, 0);

    lv_style_init(&style_label);
    lv_style_set_bg_opa(&style_label, LV_OPA_TRANSP);
    lv_style_set_text_opa(&style_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_label, lv_color_white());
}

static lv_obj_t * lv_lab_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,lv_align_t align,lv_coord_t x_offset,lv_coord_t y_offset,const char * ptr)
{
    lv_obj_t * lab = lv_label_create(src_obj);
    lv_obj_add_style(lab, &style_label, LV_PART_MAIN);
    lv_obj_set_style_text_font(lab, &language, 0);
    lv_label_set_text(lab,ptr);
    lv_obj_align_to(lab,align_obj,align,x_offset,y_offset);
    return lab;
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

static void lv_page_1_init(lv_obj_t *con_obj,const char * text)
{   
    lab11 = lv_lab_creat_fun(con_obj,con_obj,LV_ALIGN_CENTER,0,-15,"SN序列号");
    lab12 = lv_lab_creat_fun(con_obj,con_obj,LV_ALIGN_CENTER,0,15,text);
}

static void lv_page_2_init(lv_obj_t *con_obj,const char * text)
{   
    lab21 = lv_lab_creat_fun(con_obj,con_obj,LV_ALIGN_CENTER,0,-15,"固件版本信息");
    lab22 = lv_lab_creat_fun(con_obj,con_obj,LV_ALIGN_CENTER,0,15,text);
}

static void lv_page_3_init(lv_obj_t *con_obj,const char * text1,const char * text2)
{    
    uint8_t buf1[32] = {"TX1版本信息 "};
    uint8_t buf2[32] = {"TX2版本信息 "};
    strcat(buf1,text1);
    strcat(buf2,text2);
    lab31 = lv_lab_creat_fun(con_obj,con_obj,LV_ALIGN_CENTER,0,-15,buf1);
    lab32 = lv_lab_creat_fun(con_obj,con_obj,LV_ALIGN_CENTER,0,15,buf2);
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

void hl_mod_verson_ioctl(void * ctl_data)
{
    hl_lvgl_verson_ioctl_t * ptr = (hl_lvgl_verson_ioctl_t *)ctl_data;
    switch(ptr->verson_choose_opt){
        case HL_VERSON_OPTION_ONE:
            lv_obj_scroll_to_view(con1, LV_ANIM_ON);
            break;
        case HL_VERSON_OPTION_TWO:
            lv_obj_scroll_to_view(con2, LV_ANIM_ON);
            break;
        case HL_VERSON_OPTION_THREE:
            lv_obj_scroll_to_view(con3, LV_ANIM_ON);
            break;
        case HL_VERSON_OPTION_EXTI:
            hl_obj_delete(lv_scr_act(),false);
            break;
        default:
            break;
    }
}
void hl_mod_verson_init(void * init_data)
{
    hl_lvgl_verson_init_t * ptr = (hl_lvgl_verson_init_t *)init_data;
    lv_style_page4_init();
    con_src = lv_con_scr_creat(180,100);
    con1 = lv_con_child_creat(con_src,172,90);
    con2 = lv_con_child_creat(con_src,172,90);
    con3 = lv_con_child_creat(con_src,172,90);
    lv_page_1_init(con1,ptr->hl_verson_sn_number);
    lv_page_2_init(con2,ptr->hl_verson_firmware);
    lv_page_3_init(con3,ptr->hl_verson_tx1,ptr->hl_verson_tx2);
    lv_obj_scroll_to_view(con1, LV_ANIM_OFF);
}

//测试接口
// void page_verson_test(void)
// {
//     lv_style_page4_init();
//     lv_page_init();
// }