/**
 * @file page_charge.c
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief 充电界面
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
#include "page_charge.h"
#include "page_style_bit.h"

LV_IMG_DECLARE(Main_charging);//充电图标
LV_IMG_DECLARE(Main_bat);//电池图标
LV_IMG_DECLARE(Main_box_bat);//大电池图标

#define LV_BAR_GREEN    0
#define LV_BAR_WHITE    1

static lv_obj_t * area_tx1,*area_tx2,*area_rx;

static lv_obj_t * img1,*img2,*img3,*img4;
static lv_obj_t * img11,*img21,*img31;

static lv_obj_t * lab_case;
static lv_obj_t * lab_tx1,*lab_rx,*lab_tx2;
static lv_obj_t * lab_power1,*lab_power2,*lab_power3,*lab_power4;

static lv_obj_t * bar_power_tx1,*bar_power_rx,*bar_power_tx2,*bar_power4;

static lv_style_t style_area_main;
static lv_style_t style_label;
static lv_style_t style_bar_top_indicator_green,style_bar_top_indicator_white,style_bar_main;
static lv_style_t style_bar_tx1_indicator_green,style_bar_tx1_indicator_white;
static lv_style_t style_bar_rx_indicator_green,style_bar_rx_indicator_white;
static lv_style_t style_bar_tx2_indicator_green,style_bar_tx2_indicator_white;

static hl_lvgl_charge_init_t charge_init;

static void lv_style_page_init(void)
{
    lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);

    // static lv_style_t style;
    // lv_style_init(&style);
    // lv_style_set_bg_color(&style, lv_color_black());
    // lv_style_set_border_width(&style, 0);
    //lv_obj_add_style(lv_scr_act(), &style, 0);

    lv_style_init(&style_area_main);
    lv_style_set_bg_opa(&style_area_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_area_main, lv_palette_darken(LV_PALETTE_GREY,4));
    lv_style_set_border_width(&style_area_main,0);
    lv_style_set_outline_width(&style_area_main,0);
    lv_style_set_radius(&style_area_main, 5);

    lv_style_init(&style_bar_top_indicator_green);
    lv_style_set_bg_opa(&style_bar_top_indicator_green, LV_OPA_COVER);
    lv_style_set_bg_color(&style_bar_top_indicator_green, lv_palette_main(LV_PALETTE_LIGHT_GREEN));
    lv_style_set_radius(&style_bar_top_indicator_green, 0);

    lv_style_init(&style_bar_tx1_indicator_green);
    lv_style_set_bg_opa(&style_bar_tx1_indicator_green, LV_OPA_COVER);
    lv_style_set_bg_color(&style_bar_tx1_indicator_green, lv_palette_main(LV_PALETTE_LIGHT_GREEN));
    lv_style_set_radius(&style_bar_tx1_indicator_green, 0);

    lv_style_init(&style_bar_rx_indicator_green);
    lv_style_set_bg_opa(&style_bar_rx_indicator_green, LV_OPA_COVER);
    lv_style_set_bg_color(&style_bar_rx_indicator_green, lv_palette_main(LV_PALETTE_LIGHT_GREEN));
    lv_style_set_radius(&style_bar_rx_indicator_green, 0);

    lv_style_init(&style_bar_tx2_indicator_green);
    lv_style_set_bg_opa(&style_bar_tx2_indicator_green, LV_OPA_COVER);
    lv_style_set_bg_color(&style_bar_tx2_indicator_green, lv_palette_main(LV_PALETTE_LIGHT_GREEN));
    lv_style_set_radius(&style_bar_tx2_indicator_green, 0);

    lv_style_init(&style_bar_top_indicator_white);
    lv_style_set_bg_opa(&style_bar_top_indicator_white, LV_OPA_COVER);
    lv_style_set_bg_color(&style_bar_top_indicator_white, lv_color_white());
    lv_style_set_radius(&style_bar_top_indicator_white, 0);

    lv_style_init(&style_bar_tx1_indicator_white);
    lv_style_set_bg_opa(&style_bar_tx1_indicator_white, LV_OPA_COVER);
    lv_style_set_bg_color(&style_bar_tx1_indicator_white, lv_color_white());
    lv_style_set_radius(&style_bar_tx1_indicator_white, 0);

    lv_style_init(&style_bar_rx_indicator_white);
    lv_style_set_bg_opa(&style_bar_rx_indicator_white, LV_OPA_COVER);
    lv_style_set_bg_color(&style_bar_rx_indicator_white, lv_color_white());
    lv_style_set_radius(&style_bar_rx_indicator_white, 0);

    lv_style_init(&style_bar_tx2_indicator_white);
    lv_style_set_bg_opa(&style_bar_tx2_indicator_white, LV_OPA_COVER);
    lv_style_set_bg_color(&style_bar_tx2_indicator_white, lv_color_white());
    lv_style_set_radius(&style_bar_tx2_indicator_white, 0);

    lv_style_init(&style_bar_main);
    lv_style_set_bg_opa(&style_bar_main, LV_OPA_TRANSP);
    lv_style_set_radius(&style_bar_main, 0);

    lv_style_init(&style_label);
    lv_style_set_bg_opa(&style_label, LV_OPA_TRANSP);
    lv_style_set_text_opa(&style_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_label, lv_color_white());
}

static lv_obj_t * lv_area_creat_fun(lv_align_t align,lv_coord_t x_offset,lv_coord_t y_offset,lv_coord_t width, lv_coord_t high)
{
    lv_obj_t * null_area;
    null_area = lv_obj_create(lv_scr_act());
    lv_obj_add_style(null_area, &style_area_main, LV_PART_MAIN);
    lv_obj_clear_flag(null_area, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(null_area, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(null_area,width,high);
    lv_obj_align(null_area,align,x_offset,y_offset);
    return null_area;
}

static lv_obj_t * lv_power_bar_creat_fun(lv_obj_t *align_obj,lv_style_t * style_green,lv_style_t * style_white,lv_coord_t x_offset,lv_coord_t y_offset,lv_coord_t width, lv_coord_t high,\
                                        uint8_t color,int32_t init_value)
{
    lv_obj_t * bar = lv_bar_create(align_obj);
    lv_obj_add_style(bar, &style_bar_main, LV_PART_MAIN);
    if(color == LV_BAR_GREEN){
        lv_obj_add_style(bar, style_green, LV_PART_INDICATOR);
    }else{
        lv_obj_add_style(bar, style_white, LV_PART_INDICATOR);
    }    
    lv_obj_set_size(bar, width, high);
    lv_obj_align_to(bar,align_obj,LV_ALIGN_LEFT_MID,x_offset,y_offset);
    lv_bar_set_value(bar, init_value, LV_ANIM_ON);
    lv_bar_set_range(bar,0,100);
    lv_obj_move_background(bar);
    return bar;
}

static lv_obj_t * lv_img_creat_fun(lv_obj_t *align_obj,lv_align_t align,const void * src,lv_coord_t x_offset,lv_coord_t y_offset)
{
    lv_obj_t * img = lv_img_create(align_obj);
    lv_img_set_src(img,src);
    lv_obj_align(img,align,x_offset,y_offset);
    return img;
}

static lv_obj_t * lv_power_lab_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,lv_obj_t *bar_obj,lv_align_t align,lv_coord_t x_offset,lv_coord_t y_offset)
{
    char buf[8] = {0,0,0,0,0,0,0,0};
    lv_obj_t * lab = lv_label_create(src_obj);
    lv_obj_add_style(lab, &style_label, LV_PART_MAIN);
    lv_snprintf(buf, sizeof(buf), "%d%%", lv_bar_get_value(bar_obj));
    lv_label_set_text(lab,buf);
    lv_obj_align_to(lab,align_obj,align,x_offset,y_offset);
    return lab;
}

static lv_obj_t * lv_device_lab_creat_fun(lv_obj_t *align_obj,lv_align_t align,lv_coord_t x_offset,lv_coord_t y_offset,const char *ptr)
{
    lv_obj_t * lab = lv_label_create(align_obj);
    lv_obj_add_style(lab, &style_label, LV_PART_MAIN);
    lv_label_set_text(lab,ptr);
    lv_obj_align_to(lab,align_obj,align,x_offset,y_offset);
    return lab;
}

static void lv_device_top_creat(int16_t value)
{
    img4 = lv_img_creat_fun(lv_scr_act(),LV_ALIGN_TOP_RIGHT,&Main_bat,-6,6);
    bar_power4 = lv_power_bar_creat_fun(img4,&style_bar_top_indicator_green,&style_bar_top_indicator_white,3,0,25,14,LV_BAR_GREEN,value);
    lab_power4 = lv_power_lab_creat_fun(lv_scr_act(),img4,bar_power4,LV_ALIGN_OUT_LEFT_MID,-12,0);

    lab_case = lv_device_lab_creat_fun(lv_scr_act(),LV_ALIGN_TOP_LEFT,6,3,"CASE");
}

static void lv_tx1_device_creat(int16_t value)
{
    area_tx1 = lv_area_creat_fun(LV_ALIGN_BOTTOM_LEFT,6,-6,90,94);
    img1 = lv_img_creat_fun(area_tx1,LV_ALIGN_CENTER,&Main_box_bat,2,0);
    img11 = lv_img_creat_fun(img1,LV_ALIGN_CENTER,&Main_charging,-1,0);
    bar_power_tx1 = lv_power_bar_creat_fun(img1,&style_bar_tx1_indicator_green,&style_bar_tx1_indicator_white,4,-1,46,21,LV_BAR_GREEN,value);
    lab_tx1 = lv_device_lab_creat_fun(area_tx1,LV_ALIGN_TOP_MID,0,-5,"TX");
    lab_power1 = lv_power_lab_creat_fun(area_tx1,img1,bar_power_tx1,LV_ALIGN_OUT_BOTTOM_MID,1,0);
}

static void lv_rx_device_creat(int16_t value)
{
    area_rx = lv_area_creat_fun(LV_ALIGN_BOTTOM_LEFT,102,-6,90,94);
    img2 = lv_img_creat_fun(area_rx,LV_ALIGN_CENTER,&Main_box_bat,2,0);
    img21 = lv_img_creat_fun(img2,LV_ALIGN_CENTER,&Main_charging,-1,0);
    bar_power_rx = lv_power_bar_creat_fun(img2,&style_bar_rx_indicator_green,&style_bar_rx_indicator_white,4,-1,46,21,LV_BAR_GREEN,value);
    lab_rx = lv_device_lab_creat_fun(area_rx,LV_ALIGN_TOP_MID,0,-5,"RX");
    lab_power2 = lv_power_lab_creat_fun(area_rx,img2,bar_power_rx,LV_ALIGN_OUT_BOTTOM_MID,1,0);
}

static void lv_tx2_device_creat(int16_t value)
{
    area_tx2 = lv_area_creat_fun(LV_ALIGN_BOTTOM_LEFT,198,-6,90,94);
    img3 = lv_img_creat_fun(area_tx2,LV_ALIGN_CENTER,&Main_box_bat,2,0);
    img31 = lv_img_creat_fun(img3,LV_ALIGN_CENTER,&Main_charging,-1,0);
    bar_power_tx2 = lv_power_bar_creat_fun(img3,&style_bar_tx2_indicator_green,&style_bar_tx2_indicator_white,4,-1,46,21,LV_BAR_GREEN,value);
    lab_tx2 = lv_device_lab_creat_fun(area_tx2,LV_ALIGN_TOP_MID,0,-5,"TX");
    lab_power3 = lv_power_lab_creat_fun(area_tx2,img3,bar_power_tx2,LV_ALIGN_OUT_BOTTOM_MID,1,0);
}

// static void hl_obj_delete(lv_obj_t *obj,bool obj_typ)
// {
//     uint32_t child_cnt = 0,i;
//     child_cnt = lv_obj_get_child_cnt(obj);
//     if(child_cnt == 0){
//         lv_obj_add_flag(obj,LV_OBJ_FLAG_HIDDEN);
//         lv_obj_del_delayed(obj,0);
//     }else{
//         for(i=0;i<child_cnt;i++){
//             hl_obj_delete(lv_obj_get_child(obj, i),true);            
//         }
//         if(obj_typ){
//             lv_obj_add_flag(obj,LV_OBJ_FLAG_HIDDEN);
//             lv_obj_del_delayed(obj,0);
//         }        
//     }
// }

static void lv_delete_style(void)
{
    lv_style_reset(&style_area_main);
    lv_style_reset(&style_label);
    lv_style_reset(&style_bar_top_indicator_green);
    lv_style_reset(&style_bar_top_indicator_white);
    lv_style_reset(&style_bar_tx1_indicator_green);
    lv_style_reset(&style_bar_tx1_indicator_white);
    lv_style_reset(&style_bar_rx_indicator_green);
    lv_style_reset(&style_bar_rx_indicator_white);
    lv_style_reset(&style_bar_tx2_indicator_green);
    lv_style_reset(&style_bar_tx2_indicator_white);
    lv_style_reset(&style_bar_main);
}

void hl_mod_charge_ioctl(void * ctl_data)
{
    char buf[8] = {0,0,0,0,0,0,0,0};
    hl_lvgl_charge_ioctl_t * ptr = (hl_lvgl_charge_ioctl_t *)ctl_data;
    switch(ptr->charge_cmd){
        case HL_DELETE_TX1_DEVICE:
            if(charge_init.display_device & HL_CHARGE_DISPLAY_TX1){
                // hl_obj_delete(area_tx1,true);
                charge_init.display_device &= ~HL_CHARGE_DISPLAY_TX1;
                lv_obj_del(area_tx1);
                rt_thread_mdelay(5);                 
            }            
            break;
        case HL_DELETE_RX_DEVICE:
            if(charge_init.display_device & HL_CHARGE_DISPLAY_RX){
                // hl_obj_delete(area_rx,true);
                charge_init.display_device &= ~HL_CHARGE_DISPLAY_RX;
                lv_obj_del(area_rx);
                rt_thread_mdelay(5);                 
            }  
            break;
        case HL_DELETE_TX2_DEVICE:
            if(charge_init.display_device & HL_CHARGE_DISPLAY_TX2){
                // hl_obj_delete(area_tx2,true);
                charge_init.display_device &= ~HL_CHARGE_DISPLAY_TX2;
                lv_obj_del(area_tx2);
                rt_thread_mdelay(5); 
            }  
            break;
        
        case HL_ADD_TX1_DEVICE:
            if(!(charge_init.display_device & HL_CHARGE_DISPLAY_TX1)){
                lv_tx1_device_creat(ptr->electric.electric_tx1);
                charge_init.display_device |= HL_CHARGE_DISPLAY_TX1;
            }            
            break;
        case HL_ADD_RX_DEVICE:
            if(!(charge_init.display_device & HL_CHARGE_DISPLAY_RX)){
                lv_rx_device_creat(ptr->electric.electric_rx);
                charge_init.display_device |= HL_CHARGE_DISPLAY_RX;
            }   
            break;
        case HL_ADD_TX2_DEVICE:
            if(!(charge_init.display_device & HL_CHARGE_DISPLAY_TX2)){
                lv_tx2_device_creat(ptr->electric.electric_tx2);
                charge_init.display_device |= HL_CHARGE_DISPLAY_TX2;
            }   
            break;

        case HL_CHARGE_CHANGE_BOX_ELEC:
            lv_bar_set_value(bar_power4, ptr->electric.electric_box, LV_ANIM_ON);
            lv_snprintf(buf, sizeof(buf), "%d%%", ptr->electric.electric_box);
            lv_label_set_text(lab_power4,buf);
            break;
        case HL_CHARGE_CHANGE_TX1_ELEC:
            lv_bar_set_value(bar_power_tx1, ptr->electric.electric_tx1, LV_ANIM_ON);
            lv_snprintf(buf, sizeof(buf), "%d%%", ptr->electric.electric_tx1);
            lv_label_set_text(lab_power1,buf);
            break;
        case HL_CHARGE_CHANGE_RX_ELEC:
            lv_bar_set_value(bar_power_rx, ptr->electric.electric_rx, LV_ANIM_ON);
            lv_snprintf(buf, sizeof(buf), "%d%%", ptr->electric.electric_rx);
            lv_label_set_text(lab_power2,buf);
            break;
        case HL_CHARGE_CHANGE_TX2_ELEC:
            lv_bar_set_value(bar_power_tx2, ptr->electric.electric_tx2, LV_ANIM_ON);
            lv_snprintf(buf, sizeof(buf), "%d%%", ptr->electric.electric_tx2);
            lv_label_set_text(lab_power3,buf);
            break;

        case HL_CHARGE_TOP_BAT_COLOR_GREEN:
            lv_obj_remove_style(bar_power4,&style_bar_top_indicator_green,LV_PART_INDICATOR);
            lv_obj_add_style(bar_power4,&style_bar_top_indicator_green,LV_PART_INDICATOR);  
            break;
        case HL_CHARGE_TOP_BAT_COLOR_WHITE:
            lv_obj_remove_style(bar_power4,&style_bar_top_indicator_white,LV_PART_INDICATOR);
            lv_obj_add_style(bar_power4,&style_bar_top_indicator_white,LV_PART_INDICATOR);  
            break;
        case HL_CHARGE_TX1_BAT_COLOR_GREEN:
            lv_obj_remove_style(bar_power_tx1,&style_bar_tx1_indicator_green,LV_PART_INDICATOR);
            lv_obj_add_style(bar_power_tx1,&style_bar_tx1_indicator_green,LV_PART_INDICATOR);  
            break;
        case HL_CHARGE_TX1_BAT_COLOR_WHITE:
            lv_obj_remove_style(bar_power_tx1,&style_bar_tx1_indicator_white,LV_PART_INDICATOR);
            lv_obj_add_style(bar_power_tx1,&style_bar_tx1_indicator_white,LV_PART_INDICATOR);  
            break;
        case HL_CHARGE_TX2_BAT_COLOR_GREEN:
            lv_obj_remove_style(bar_power_tx2,&style_bar_tx2_indicator_green,LV_PART_INDICATOR);
            lv_obj_add_style(bar_power_tx2,&style_bar_tx2_indicator_green,LV_PART_INDICATOR);  
            break;
        case HL_CHARGE_TX2_BAT_COLOR_WHITE:
            lv_obj_remove_style(bar_power_tx2,&style_bar_tx2_indicator_white,LV_PART_INDICATOR);
            lv_obj_add_style(bar_power_tx2,&style_bar_tx2_indicator_white,LV_PART_INDICATOR);  
            break;
        case HL_CHARGE_RX_BAT_COLOR_GREEN:
            lv_obj_remove_style(bar_power_rx,&style_bar_rx_indicator_green,LV_PART_INDICATOR);
            lv_obj_add_style(bar_power_rx,&style_bar_rx_indicator_green,LV_PART_INDICATOR);  
            break;
        case HL_CHARGE_RX_BAT_COLOR_WHITE:
            lv_obj_remove_style(bar_power_rx,&style_bar_rx_indicator_white,LV_PART_INDICATOR);
            lv_obj_add_style(bar_power_rx,&style_bar_rx_indicator_white,LV_PART_INDICATOR);  
            break;
        
        case HL_CHARGE_TX1_ICON_HIDE:
            lv_obj_add_flag(img11,LV_OBJ_FLAG_HIDDEN);
            break;
        case HL_CHARGE_RX_ICON_HIDE:
            lv_obj_add_flag(img21,LV_OBJ_FLAG_HIDDEN);
            break;
        case HL_CHARGE_TX2_ICON_HIDE:
            lv_obj_add_flag(img31,LV_OBJ_FLAG_HIDDEN);
            break;
        case HL_CHARGE_TX1_ICON_DISHIDE:
            lv_obj_clear_flag(img11,LV_OBJ_FLAG_HIDDEN);
            break;
        case HL_CHARGE_RX_ICON_DISHIDE:
            lv_obj_clear_flag(img21,LV_OBJ_FLAG_HIDDEN);
            break;
        case HL_CHARGE_TX2_ICON_DISHIDE:
            lv_obj_clear_flag(img31,LV_OBJ_FLAG_HIDDEN);
            break;

        case HL_CHARGE_CHANGE_DELETE_PAGE:
            // hl_obj_delete(lv_scr_act(),false);  
            lv_obj_clean(lv_scr_act());
            rt_thread_mdelay(10);          
            break;
        case HL_CHARGE_CHANGE_DELETE_STYLE:
            lv_delete_style();            
            break;
    }
}

void hl_mod_charge_init(void * init_data)
{
    hl_lvgl_charge_init_t * ptr = (hl_lvgl_charge_init_t *)init_data;
    memcpy(&charge_init,ptr,sizeof(hl_lvgl_charge_init_t));
    if (!page_style_bit.page_charge) {
        page_style_bit.page_charge = 1;
        lv_style_page_init();
    }
    
    lv_device_top_creat(ptr->electric.electric_box);

    if(ptr->display_device & HL_CHARGE_DISPLAY_TX1){
        lv_tx1_device_creat(ptr->electric.electric_tx1);
    }
    if(ptr->display_device & HL_CHARGE_DISPLAY_RX){
        lv_rx_device_creat(ptr->electric.electric_rx);
    }
    if(ptr->display_device & HL_CHARGE_DISPLAY_TX2){
        lv_tx2_device_creat(ptr->electric.electric_tx2);
    }
}
