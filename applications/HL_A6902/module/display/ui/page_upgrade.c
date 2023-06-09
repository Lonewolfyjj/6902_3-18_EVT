/**
 * @file page_upgrade.c
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
#include "page_upgrade.h"
#include "language.h"
#include "page_style_bit.h"
#include "page_language.h"
LV_IMG_DECLARE(Other_upgrade_fail);//
LV_IMG_DECLARE(Other_upgrade_success);//

static lv_style_t style_indicator,style_main,style_label;

static lv_obj_t * bar;
static lv_obj_t * lab,*lab1,*lab2;
static lv_obj_t * img;

static void lv_style_barset_init(void)
{
    lv_style_init(&style_indicator);
    lv_style_set_bg_opa(&style_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indicator, lv_palette_main(LV_PALETTE_ORANGE));
    lv_style_set_radius(&style_indicator, 3);

    lv_style_init(&style_main);
    lv_style_set_bg_opa(&style_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_main, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_radius(&style_main, 3);

    lv_style_init(&style_label);
    lv_style_set_bg_opa(&style_label, LV_OPA_TRANSP);
    lv_style_set_text_opa(&style_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_label, lv_color_white());
}

static void lv_delete_style(void)
{
    lv_style_reset(&style_indicator);
    lv_style_reset(&style_main);
    lv_style_reset(&style_label);
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

static lv_obj_t * lv_bar_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,lv_align_t align,lv_coord_t x_offset,lv_coord_t y_offset,int8_t init_value)
{
    lv_obj_t * bar = lv_bar_create(src_obj);
    lv_obj_add_style(bar,&style_indicator, LV_PART_INDICATOR);
    lv_obj_add_style(bar,&style_main, LV_PART_MAIN);
    lv_obj_set_size(bar, 270, 8);
    lv_bar_set_range(bar,0,100);
    lv_obj_align_to(bar, align_obj, align, x_offset, y_offset);
    lv_bar_set_value(bar, init_value, LV_ANIM_OFF);
    return bar;
}

static lv_obj_t * lv_lab_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,lv_align_t align,lv_coord_t x_offset,lv_coord_t y_offset,const char * ptr)
{
    lv_obj_t * lab = lv_label_create(src_obj);
    lv_obj_add_style(lab,&style_label, LV_PART_MAIN);
    // lv_obj_set_style_text_font(lab, &language24, LV_PART_MAIN);
    lv_obj_set_style_text_color(lab, lv_color_white(), LV_PART_MAIN);
    lv_label_set_text(lab, ptr);    
    lv_obj_align_to(lab, align_obj, align, x_offset, y_offset);
    return lab;
}

static lv_obj_t * lv_img_creat_fun(lv_obj_t *src_obj,const void * src,lv_align_t align,lv_coord_t x_offset,lv_coord_t y_offset)
{
    lv_obj_t * img = lv_img_create(src_obj);
    lv_img_set_src(img, src);
    lv_obj_align(img, align, x_offset, y_offset);
    return img;
}

void hl_mod_lvgl_upgrade_init(void * init_data)
{
    hl_lvgl_upgrade_init_t * ptr = (hl_lvgl_upgrade_init_t *)init_data;
    a6902_language_typ_t* page_ptr = (a6902_language_typ_t *)hl_a6902_language_ptr_get();
    if (!page_style_bit.page_upgrade) {
        page_style_bit.page_upgrade = 1;
        lv_style_barset_init();
    }
       
    bar = lv_bar_creat_fun(lv_scr_act(),lv_scr_act(),LV_ALIGN_CENTER,0,20,ptr->upgrade_progress);
    lab = lv_lab_creat_fun(lv_scr_act(),bar,LV_ALIGN_OUT_TOP_MID,0,-20,page_ptr->upgrade_page_ptr->page_upgrade->ptr_upgrading);
}


void hl_mod_lvgl_upgrade_ioctl(void * ctl_data)
{
    hl_lvgl_upgrade_ioctl_t * ptr = (hl_lvgl_upgrade_ioctl_t *)ctl_data; 
    a6902_language_typ_t* page_ptr = (a6902_language_typ_t *)hl_a6902_language_ptr_get();
    switch(ptr->upgrade_ioctl){
        case HL_UPGRADE_PARAM_CMD:
            lv_bar_set_value(bar, ptr->upgrade_progress, LV_ANIM_ON);
            break;
        case HL_UPGRADE_SUCCESS_CMD:
            img = lv_img_creat_fun(lv_scr_act(),&Other_upgrade_success,LV_ALIGN_TOP_MID,0,8);
            lab1 = lv_lab_creat_fun(lv_scr_act(),img,LV_ALIGN_OUT_BOTTOM_MID,0,8,ptr->ptr);
            lab2 = lv_lab_creat_fun(lv_scr_act(),img,LV_ALIGN_OUT_BOTTOM_MID,0,36,page_ptr->upgrade_page_ptr->page_upgrade->ptr_reset_device);
            break;
        case HL_UPGRADE_FAIL_CMD:
            img = lv_img_creat_fun(lv_scr_act(),&Other_upgrade_fail,LV_ALIGN_TOP_MID,0,8);
            lab1 = lv_lab_creat_fun(lv_scr_act(),img,LV_ALIGN_OUT_BOTTOM_MID,0,8,ptr->ptr);
            lab2 = lv_lab_creat_fun(lv_scr_act(),img,LV_ALIGN_OUT_BOTTOM_MID,0,36,page_ptr->upgrade_page_ptr->page_upgrade->ptr_reset_device);
            break;
        case HL_UPGRADE_CLEAR_CMD:
            // hl_obj_delete(lv_scr_act(),false);
            lv_obj_clean(lv_scr_act());
            rt_thread_mdelay(10);
            break;
        case HL_UPGRADE_DEL_STYLE_CMD:
            lv_delete_style();
            break;
        default:
            break;
    }
}