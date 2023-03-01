/**
 * @file page_holding.c
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief 接上电源但是未开机时的界面
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
#include "page_holding.h"
#include "page_style_bit.h"

LV_IMG_DECLARE(Main_charging);  //充电图标
LV_IMG_DECLARE(Main_box_bat);   //大电池图标

#define LV_BAR_GREEN 0
#define LV_BAR_WHITE 1

static lv_obj_t * img1, *img2;
static lv_obj_t*  lab_power;
static lv_obj_t*  bar_power;
static lv_style_t style_label, style_bar_main;

static lv_style_t style_bar_rx_indicator_green, style_bar_rx_indicator_white;

static void lv_style_page_init(void)
{
    lv_style_init(&style_bar_main);
    lv_style_set_bg_opa(&style_bar_main, LV_OPA_TRANSP);
    lv_style_set_radius(&style_bar_main, 0);

    lv_style_init(&style_label);
    lv_style_set_bg_opa(&style_label, LV_OPA_TRANSP);
    lv_style_set_text_opa(&style_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_label, lv_color_white());

    lv_style_init(&style_bar_rx_indicator_green);
    lv_style_set_bg_opa(&style_bar_rx_indicator_green, LV_OPA_COVER);
    lv_style_set_bg_color(&style_bar_rx_indicator_green, lv_palette_main(LV_PALETTE_LIGHT_GREEN));
    lv_style_set_radius(&style_bar_rx_indicator_green, 0);

    lv_style_init(&style_bar_rx_indicator_white);
    lv_style_set_bg_opa(&style_bar_rx_indicator_white, LV_OPA_COVER);
    lv_style_set_bg_color(&style_bar_rx_indicator_white, lv_color_white());
    lv_style_set_radius(&style_bar_rx_indicator_white, 0);
}

static lv_obj_t* lv_img_creat_fun(lv_obj_t* align_obj, lv_align_t align, const void* src, lv_coord_t x_offset,
                                  lv_coord_t y_offset)
{
    lv_obj_t* img = lv_img_create(align_obj);
    lv_img_set_src(img, src);
    lv_obj_align(img, align, x_offset, y_offset);
    return img;
}

static lv_obj_t* lv_power_bar_creat_fun(lv_obj_t* align_obj, lv_style_t* style_green, lv_style_t* style_white,
                                        lv_coord_t x_offset, lv_coord_t y_offset, lv_coord_t width, lv_coord_t high,
                                        uint8_t color, int32_t init_value)
{
    lv_obj_t* bar = lv_bar_create(align_obj);
    lv_obj_add_style(bar, &style_bar_main, LV_PART_MAIN);
    if (color == LV_BAR_GREEN) {
        lv_obj_add_style(bar, style_green, LV_PART_INDICATOR);
    } else {
        lv_obj_add_style(bar, style_white, LV_PART_INDICATOR);
    }
    lv_obj_set_size(bar, width, high);
    lv_obj_align_to(bar, align_obj, LV_ALIGN_LEFT_MID, x_offset, y_offset);
    lv_bar_set_value(bar, init_value, LV_ANIM_ON);
    lv_bar_set_range(bar, 0, 100);
    lv_obj_move_background(bar);
    return bar;
}

static lv_obj_t* lv_power_lab_creat_fun(lv_obj_t* src_obj, lv_obj_t* align_obj, lv_obj_t* bar_obj, lv_align_t align,
                                        lv_coord_t x_offset, lv_coord_t y_offset)
{
    char      buf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    lv_obj_t* lab    = lv_label_create(src_obj);
    lv_obj_add_style(lab, &style_label, LV_PART_MAIN);
    lv_snprintf(buf, sizeof(buf), "%d%%", lv_bar_get_value(bar_obj));
    lv_label_set_text(lab, buf);
    lv_obj_align_to(lab, align_obj, align, x_offset, y_offset);
    return lab;
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
    lv_style_reset(&style_label);
    lv_style_reset(&style_bar_main);
    lv_style_reset(&style_bar_rx_indicator_green);
    lv_style_reset(&style_bar_rx_indicator_white);
}

static void rx_bat_style_del(int16_t data)
{
    switch (data) {
        case 0:
            lv_obj_remove_style(bar_power, &style_bar_rx_indicator_white, LV_PART_INDICATOR);
            break;
        case 2:
            lv_obj_remove_style(bar_power, &style_bar_rx_indicator_green, LV_PART_INDICATOR);
            break;
        default:
            lv_obj_remove_style(bar_power, &style_bar_rx_indicator_white, LV_PART_INDICATOR);
            break;
    }
}

void hl_mod_holding_ioctl(void* ctl_data)
{
    char                     buf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    hl_lvgl_holding_ioctl_t* ptr    = (hl_lvgl_holding_ioctl_t*)ctl_data;
    switch (ptr->holding_cmd) {
        case HL_HOLDING_RX_ELEC:
            lv_bar_set_value(bar_power, ptr->electric, LV_ANIM_ON);
            lv_snprintf(buf, sizeof(buf), "%d%%", ptr->electric);
            lv_label_set_text(lab_power, buf);
            break;
        case HL_HOLDING_RX_BAT_COLOR_GREEN:
            rx_bat_style_del(ptr->electric);
            // lv_obj_remove_style(bar_power,&style_bar_rx_indicator_green,LV_PART_INDICATOR);
            lv_obj_add_style(bar_power, &style_bar_rx_indicator_green, LV_PART_INDICATOR);
            break;
        case HL_HOLDING_RX_BAT_COLOR_WHITE:
            rx_bat_style_del(ptr->electric);
            // lv_obj_remove_style(bar_power,&style_bar_rx_indicator_white,LV_PART_INDICATOR);
            lv_obj_add_style(bar_power, &style_bar_rx_indicator_white, LV_PART_INDICATOR);
            break;
        case HL_HOLDING_RX_ICON_HIDE:
            lv_obj_add_flag(img2, LV_OBJ_FLAG_HIDDEN);
            break;
        case HL_HOLDING_RX_ICON_DISHIDE:
            lv_obj_clear_flag(img2, LV_OBJ_FLAG_HIDDEN);
            break;
        case HL_HOLDING_DELETE_PAGE:
            // hl_obj_delete(lv_scr_act(),false);
            lv_obj_clean(lv_scr_act());
            rt_thread_mdelay(10);
            break;
        case HL_HOLDING_DELETE_STYLE:
            lv_delete_style();
            break;
        default:
            break;
    }
}

void hl_mod_holding_init(void* init_data)
{
    hl_lvgl_holding_init_t* ptr = (hl_lvgl_holding_init_t*)init_data;
    if (!page_style_bit.page_holding) {
        page_style_bit.page_holding = 1;
        lv_style_page_init();
    }

    img1      = lv_img_creat_fun(lv_scr_act(), LV_ALIGN_CENTER, &Main_box_bat, 2, 0);
    img2      = lv_img_creat_fun(img1, LV_ALIGN_CENTER, &Main_charging, -1, 0);
    bar_power = lv_power_bar_creat_fun(img1, &style_bar_rx_indicator_green, &style_bar_rx_indicator_white, 4, -1, 46,
                                       21, LV_BAR_GREEN, ptr->electric);
    lab_power = lv_power_lab_creat_fun(lv_scr_act(), img1, bar_power, LV_ALIGN_OUT_BOTTOM_MID, 1, 0);
}