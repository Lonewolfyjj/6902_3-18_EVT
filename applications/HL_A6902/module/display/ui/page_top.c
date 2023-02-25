/**
 * @file page_top.c
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
#include "page_top.h"
#include "page_style_bit.h"
#include <rtthread.h>

LV_IMG_DECLARE(Main_bat);           //电池图标
LV_IMG_DECLARE(Main_charging);      //充电图标
LV_IMG_DECLARE(Main_line_in);       //耳机插入
LV_IMG_DECLARE(Main_usb_c);         //USB插入
LV_IMG_DECLARE(Main_stereo);        //立体声
LV_IMG_DECLARE(Main_saft_track);    //安全音轨
LV_IMG_DECLARE(Main_single_voice);  //单声道
LV_IMG_DECLARE(Main_lock);          //锁屏
LV_IMG_DECLARE(Main_heatset);       //监听
LV_IMG_DECLARE(Main_noise);         //降噪
LV_IMG_DECLARE(Main_apple);         // 苹果认证

LV_IMG_DECLARE(Other_lock);    //锁屏
LV_IMG_DECLARE(Other_unlock);  //解锁

LV_IMG_DECLARE(Other_line_out);  //
LV_IMG_DECLARE(Other_heatset);   //
LV_IMG_DECLARE(Other_usb_c);     //

LV_IMG_DECLARE(Other_lock);    //锁屏
LV_IMG_DECLARE(Other_unlock);  //解锁

LV_IMG_DECLARE(Other_line_out);  //
LV_IMG_DECLARE(Other_heatset);   //
LV_IMG_DECLARE(Other_usb_c);     //

#define ICON_POS_LIFT 0
#define ICON_POS_RIGHT 1

#define ICON_POS_VOR 0

#define LIFT_DEFAUTE_OFFSET 1
#define RIGHT_DEFAUTE_OFFSET -35

#define ICON_HOLD_TIME 2000

typedef struct _HL_DISPLAY_CENTER_T
{
    uint8_t lock : 1;  //

    uint8_t unlock : 1;  //

    uint8_t line_out : 1;  //

    uint8_t heatset : 1;

    uint8_t usb_c : 1;

    uint8_t reserve0 : 1;

    uint8_t reserve1 : 1;
    uint8_t reserve2 : 1;
} HL_DISPLAY_CENTER_T;

typedef struct _HL_DISPLAY_TOP_T
{
    uint8_t voice_mod : 1;  //

    uint8_t noise : 1;  //

    uint8_t lock : 1;  //

    uint8_t lineout : 1;

    uint8_t typec : 1;

    uint8_t heatset : 1;

    uint8_t apple : 1;
    uint8_t reserve2 : 1;
} HL_DISPLAY_TOP_T;

static HL_DISPLAY_CENTER_T center_icon_sta = {
    .heatset  = 0,
    .line_out = 0,
    .lock     = 0,
    .unlock   = 0,
    .usb_c    = 0,
};

static HL_DISPLAY_TOP_T top_icon_sta = {
    .heatset   = 0,
    .lineout   = 0,
    .lock      = 0,
    .noise     = 0,
    .typec     = 0,
    .voice_mod = 0,
};

typedef struct _img_info_t
{
    lv_obj_t*   img_obj;
    const void* img_src;
    uint8_t     default_pos;
} img_info_t;

typedef struct _top_list_t
{
    img_info_t*         img_info;
    struct _top_list_t* next;
} top_list_t;
static hl_lock_event_cb page_lock_cb;

static lv_style_t style_area_main;
static lv_style_t style_power_bar_white_indicator, style_power_bar_green_indicator, style_power_bar_main,
    style_power_bar_red_indicator;
static lv_style_t  style_power_label;
static lv_timer_t* timer;
static lv_obj_t *  bat_icon, *bat_bar, *bat_label, *bat_charger_icon;
static lv_obj_t *  center_heatset, *center_line_out, *center_lock, *center_unlock, *center_usb_c;
static top_list_t *head_left_list = NULL, *head_right_list = NULL;
static int8_t      TOP_POS, TOP_OUT_POS, TOP_OFFSET;

static lv_obj_t* area     = RT_NULL;
static lv_obj_t *img_lock = RT_NULL, *img_unlock = RT_NULL;

static img_info_t stereo_icon = { .default_pos = HL_TOP_ICON_STEREO_MOD, .img_src = &Main_stereo, .img_obj = NULL },
                  noise_icon  = { .default_pos = HL_TOP_ICON_NOISE, .img_src = &Main_noise, .img_obj = NULL },
                  lock_icon   = { .default_pos = HL_TOP_ICON_LOCK, .img_src = &Main_lock, .img_obj = NULL },
                  apple_icon  = { .default_pos = HL_TOP_ICON_APPLE,
                                 .img_src     = &Main_apple,
                                 .img_obj     = NULL };  //左侧图标

static img_info_t line_out_icon = { .default_pos = HL_TOP_ICON_LINEOUT, .img_src = &Main_line_in, .img_obj = NULL },
                  usb_c_icon    = { .default_pos = HL_TOP_ICON_TYPEC, .img_src = &Main_usb_c, .img_obj = NULL },
                  heatset_icon  = { .default_pos = HL_TOP_ICON_HEATSET,
                                   .img_src     = &Main_heatset,
                                   .img_obj     = NULL };  //右侧图标

static void hl_delete_top_icon(hl_top_icon_t icon);
static void centert_icon_timer_cb(lv_timer_t* timer);

static void lv_lock_style_init(void)
{
    //lv_area_creat_fun函数使用
    lv_style_init(&style_area_main);
    lv_style_set_bg_opa(&style_area_main, LV_OPA_TRANSP);
    // lv_style_set_bg_color(&style_area_main, lv_palette_darken(LV_PALETTE_GREY,4));
    lv_style_set_border_width(&style_area_main, 0);
    lv_style_set_outline_width(&style_area_main, 0);
    lv_style_set_radius(&style_area_main, 0);
}

static void hl_delete_top_icon(hl_top_icon_t icon);

static void lv_style_page_top_init(void)
{
    lv_obj_enable_style_refresh(true);

    lv_style_init(&style_power_bar_white_indicator);
    lv_style_set_bg_opa(&style_power_bar_white_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_power_bar_white_indicator, lv_color_white());
    lv_style_set_radius(&style_power_bar_white_indicator, 0);

    lv_style_init(&style_power_bar_green_indicator);
    lv_style_set_bg_opa(&style_power_bar_green_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_power_bar_green_indicator, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_radius(&style_power_bar_green_indicator, 0);

    lv_style_init(&style_power_bar_red_indicator);
    lv_style_set_bg_opa(&style_power_bar_red_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_power_bar_red_indicator, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_radius(&style_power_bar_red_indicator, 0);

    lv_style_init(&style_power_bar_main);
    lv_style_set_bg_opa(&style_power_bar_main, LV_OPA_TRANSP);
    lv_style_set_radius(&style_power_bar_main, 0);

    lv_style_init(&style_power_label);
    lv_style_set_bg_opa(&style_power_label, LV_OPA_COVER);
    lv_style_set_bg_color(&style_power_label, lv_color_black());
    lv_style_set_text_opa(&style_power_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_power_label, lv_color_white());
}

static lv_obj_t* lv_power_bar_creat_fun(lv_obj_t* align_obj, lv_coord_t x_offset, lv_coord_t y_offset, lv_coord_t width,
                                        lv_coord_t high, int32_t init_value)
{
    lv_obj_t* bar = lv_bar_create(align_obj);
    lv_obj_add_style(bar, &style_power_bar_main, LV_PART_MAIN);
    lv_obj_add_style(bar, &style_power_bar_white_indicator, LV_PART_INDICATOR);
    lv_obj_set_size(bar, width, high);
    lv_obj_align_to(bar, align_obj, LV_ALIGN_LEFT_MID, x_offset, y_offset);
    lv_bar_set_value(bar, init_value, LV_ANIM_ON);
    lv_bar_set_range(bar, 0, 100);
    return bar;
}

static lv_obj_t* lv_img_creat_fun(lv_obj_t* align_obj, lv_align_t align, const void* src, lv_coord_t x_offset,
                                  lv_coord_t y_offset)
{
    lv_obj_t* img = lv_img_create(align_obj);
    lv_img_set_src(img, src);
    lv_obj_align(img, align, x_offset, y_offset);
    lv_img_set_zoom(img, 200);
    lv_obj_set_height(img, 18);
    lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
    return img;
}

static lv_obj_t* lv_center_img_creat_fun(const void* src, lv_coord_t x_offset, lv_coord_t y_offset)
{
    lv_obj_t* img = lv_img_create(lv_scr_act());
    lv_img_set_src(img, src);
    lv_obj_align(img, LV_ALIGN_CENTER, x_offset, y_offset);
    return img;
}

static lv_obj_t* lv_power_img_creat_fun(lv_obj_t* align_obj, lv_coord_t x_offset, lv_coord_t y_offset, uint16_t zoom)
{
    lv_obj_t* img = lv_img_create(align_obj);
    lv_img_set_src(img, &Main_bat);
    lv_img_set_zoom(img, zoom);
    lv_obj_align_to(img, align_obj, LV_ALIGN_TOP_RIGHT, x_offset, y_offset);
    return img;
}

static lv_obj_t* lv_power_lab_creat_fun(lv_obj_t* src_obj, lv_obj_t* align_obj, lv_obj_t* bar_obj, lv_coord_t x_offset,
                                        lv_coord_t y_offset)
{
    char      buf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    lv_obj_t* lab    = lv_label_create(src_obj);
    lv_obj_add_style(lab, &style_power_label, LV_PART_MAIN);
    lv_snprintf(buf, sizeof(buf), "%d%%", lv_bar_get_value(bar_obj));
    lv_label_set_text(lab, buf);
    lv_obj_align_to(lab, align_obj, LV_ALIGN_OUT_LEFT_MID, x_offset, y_offset);
    return lab;
}

static lv_obj_t* hl_mod_creat_top_icon(const void* img_src, lv_obj_t* align_obj, lv_align_t align, lv_coord_t x_ofs,
                                       lv_coord_t y_ofs)
{
    lv_obj_t* img;
    img = lv_img_create(lv_scr_act());
    lv_img_set_src(img, img_src);
    lv_img_set_zoom(img, 256);
    lv_obj_align_to(img, align_obj, align, x_ofs, y_ofs);
    return img;
}

static void hl_top_icon_init(void)
{
    head_left_list        = (top_list_t*)rt_malloc(sizeof(top_list_t));
    head_left_list->next  = NULL;
    head_right_list       = (top_list_t*)rt_malloc(sizeof(top_list_t));
    head_right_list->next = NULL;
}

static void hl_top_icon_ref(uint8_t typ, top_list_t* head)
{
    uint8_t     i    = 0;
    top_list_t* node = head;
    if (ICON_POS_LIFT == typ) {
        TOP_POS     = LV_ALIGN_TOP_LEFT;
        TOP_OFFSET  = LIFT_DEFAUTE_OFFSET;
        TOP_OUT_POS = LV_ALIGN_OUT_RIGHT_MID;
    } else {
        TOP_POS     = LV_ALIGN_TOP_RIGHT;
        TOP_OFFSET  = RIGHT_DEFAUTE_OFFSET;
        TOP_OUT_POS = LV_ALIGN_OUT_LEFT_MID;
    }
    while (node->next != NULL) {
        if (node != head) {
            if (node->next->img_info->img_obj != NULL) {
                lv_obj_del(node->next->img_info->img_obj);
                node->next->img_info->img_obj =
                    hl_mod_creat_top_icon(node->next->img_info->img_src, node->img_info->img_obj, TOP_OUT_POS, 0, 0);
            } else {
                node->next->img_info->img_obj =
                    hl_mod_creat_top_icon(node->next->img_info->img_src, node->img_info->img_obj, TOP_OUT_POS, 0, 0);
            }
        } else {
            if (node->next->img_info->img_obj == NULL) {
                node->next->img_info->img_obj =
                    hl_mod_creat_top_icon(node->next->img_info->img_src, lv_scr_act(), TOP_POS, TOP_OFFSET, 0);
            } else {
                lv_obj_del(node->next->img_info->img_obj);
                node->next->img_info->img_obj =
                    hl_mod_creat_top_icon(node->next->img_info->img_src, lv_scr_act(), TOP_POS, TOP_OFFSET, 0);
            }
        }
        node = node->next;
    }
}

static void hl_top_icon_add(img_info_t* img_info, uint8_t typ, top_list_t* head)
{
    top_list_t *node    = head, *node_list;
    node_list           = (top_list_t*)rt_malloc(sizeof(top_list_t));
    node_list->img_info = img_info;
    node_list->next     = NULL;
    if (node->next == NULL) {
        node->next = node_list;
        goto display;
    }
    while (node->next != NULL) {
        if (node->next->img_info->default_pos > node_list->img_info->default_pos) {
            node_list->next = node->next;
            node->next      = node_list;
            goto display;
        } else if (node->next->img_info->default_pos == node_list->img_info->default_pos) {
            node->next->img_info = node_list->img_info;
            rt_free(node_list);
            goto display;
        }
        node = node->next;
    }
    node->next = node_list;
display:
    hl_top_icon_ref(typ, head);
}

static lv_obj_t* hl_top_icon_obj_get(hl_top_icon_t icon, top_list_t* head)
{
    top_list_t* node = head;
    while (node->next != NULL) {
        if (node->next->img_info->default_pos == icon) {
            return node->next->img_info->img_obj;
        }
        node = node->next;
    }
    return NULL;
}

static uint8_t hl_top_icon_delete(uint8_t def_num, uint8_t typ, top_list_t* head)
{
    top_list_t *node = head, *del_list;
    del_list         = (top_list_t*)rt_malloc(sizeof(top_list_t));
    top_list_t* del  = del_list;
    while (node->next != NULL) {
        if (node->next->img_info->default_pos == def_num) {
            del_list   = node->next;
            node->next = node->next->next;
            goto del;
        }
        node = node->next;
    }
    return 1;
del:
    lv_obj_del(del_list->img_info->img_obj);
    hl_top_icon_ref(typ, head);
    rt_free(del_list);
    rt_free(del);
    return 0;
}

static void hl_top_list_clean(top_list_t* head)
{
    top_list_t *node = head->next, *last_node;
    last_node        = node->next;
    while (last_node != NULL) {
        rt_free(node);
        node      = last_node;
        last_node = last_node->next;
    }
}

static void hl_add_top_icon(hl_top_icon_t icon)
{
    switch (icon) {
        case HL_TOP_ICON_STEREO_MOD:  //
        case HL_TOP_ICON_SINGLE_MOD:  //
        case HL_TOP_ICON_TRACK_MOD:   //
            if (top_icon_sta.voice_mod == 1) {
                rt_kprintf("voice_mod is exist!\n");
                return;
            }
            top_icon_sta.voice_mod = 1;
            if (icon == HL_TOP_ICON_STEREO_MOD) {
                stereo_icon.img_src = &Main_stereo;
            }
            if (icon == HL_TOP_ICON_SINGLE_MOD) {
                stereo_icon.img_src = &Main_single_voice;
            }
            if (icon == HL_TOP_ICON_TRACK_MOD) {
                stereo_icon.img_src = &Main_saft_track;
            }
            stereo_icon.img_obj = NULL;
            hl_top_icon_add(&stereo_icon, ICON_POS_LIFT, head_left_list);
            break;
        case HL_TOP_ICON_NOISE:
            if (top_icon_sta.noise == 1) {
                rt_kprintf("Top noise is exist!\n");
                return;
            }
            top_icon_sta.noise = 1;
            noise_icon.img_obj = NULL;
            hl_top_icon_add(&noise_icon, ICON_POS_LIFT, head_left_list);
            break;
        case HL_TOP_ICON_LOCK:
            if (top_icon_sta.lock == 1) {
                rt_kprintf("Top lock is exist!\n");
                return;
            }
            top_icon_sta.lock = 1;
            lock_icon.img_obj = NULL;
            hl_top_icon_add(&lock_icon, ICON_POS_LIFT, head_left_list);
            break;
        case HL_TOP_ICON_APPLE:
            if (top_icon_sta.apple == 1) {
                rt_kprintf("Top apple is exist!\n");
                return;
            }
            top_icon_sta.apple = 1;
            apple_icon.img_obj = NULL;
            hl_top_icon_add(&apple_icon, ICON_POS_LIFT, head_left_list);
            break;
        case HL_TOP_ICON_LINEOUT:
            if (top_icon_sta.lineout == 1) {
                rt_kprintf("Top lineout is exist!\n");
                return;
            }
            top_icon_sta.lineout  = 1;
            line_out_icon.img_obj = NULL;
            hl_top_icon_add(&line_out_icon, ICON_POS_RIGHT, head_right_list);
            break;
        case HL_TOP_ICON_TYPEC:
            if (top_icon_sta.typec == 1) {
                rt_kprintf("Top typec is exist!\n");
                return;
            }
            top_icon_sta.typec = 1;
            usb_c_icon.img_obj = NULL;
            hl_top_icon_add(&usb_c_icon, ICON_POS_RIGHT, head_right_list);
            break;
        case HL_TOP_ICON_HEATSET:
            if (top_icon_sta.heatset == 1) {
                rt_kprintf("Top heatset is exist!\n");
                return;
            }
            top_icon_sta.heatset = 1;
            heatset_icon.img_obj = NULL;
            hl_top_icon_add(&heatset_icon, ICON_POS_RIGHT, head_right_list);
            break;
        default:
            break;
    }
}

// static void hl_del_center_icon(void)
// {
//     if (center_icon_sta.lock) {
//         lv_timer_reset(timer);
//         lv_timer_pause(timer);
//         lv_obj_del(center_lock);
//         hl_add_top_icon(HL_TOP_ICON_LOCK);
//         page_lock_cb(HL_LOCK_STATUS_HIDE);
//         center_icon_sta.lock = 0;
//     }
//     if (center_icon_sta.line_out) {
//         lv_timer_reset(timer);
//         lv_timer_pause(timer);
//         lv_obj_del(center_line_out);
//         hl_add_top_icon(HL_TOP_ICON_LINEOUT);
//         center_icon_sta.line_out = 0;
//     }
//     if (center_icon_sta.usb_c) {
//         center_icon_sta.usb_c = 0;
//         lv_timer_reset(timer);
//         lv_timer_pause(timer);
//         lv_obj_del(center_usb_c);
//         hl_add_top_icon(HL_TOP_ICON_TYPEC);
//     }
//     if (center_icon_sta.heatset) {
//         center_icon_sta.heatset = 0;
//         lv_timer_reset(timer);
//         lv_timer_pause(timer);
//         lv_obj_del(center_heatset);
//         hl_add_top_icon(HL_TOP_ICON_HEATSET);
//     }
//     if (center_icon_sta.unlock) {
//         center_icon_sta.unlock = 0;
//         lv_timer_reset(timer);
//         lv_timer_pause(timer);
//         lv_obj_del(center_unlock);
//         page_lock_cb(HL_UNLOCK_STATUS_HIDE);
//     }
// }

static void hl_del_center_icon(void)
{
    if (center_icon_sta.lock) {
        center_icon_sta.lock = 0;
        lv_timer_reset(timer);
        lv_timer_pause(timer);
        lv_obj_del(center_lock);

        page_lock_cb(HL_LOCK_STATUS_HIDE);
    }
    if (center_icon_sta.line_out) {
        center_icon_sta.line_out = 0;
        lv_timer_reset(timer);
        lv_timer_pause(timer);
        lv_obj_del(center_line_out);
        hl_add_top_icon(HL_TOP_ICON_LINEOUT);
    }
    if (center_icon_sta.usb_c) {
        hl_add_top_icon(HL_TOP_ICON_TYPEC);
        center_icon_sta.usb_c = 0;
        lv_timer_reset(timer);
        lv_timer_pause(timer);
        lv_obj_del(center_usb_c);
    }
    if (center_icon_sta.heatset) {
        center_icon_sta.heatset = 0;
        lv_timer_reset(timer);
        lv_timer_pause(timer);
        lv_obj_del(center_heatset);
        hl_add_top_icon(HL_TOP_ICON_HEATSET);
    }
    if (center_icon_sta.unlock) {
        center_icon_sta.unlock = 0;
        lv_timer_reset(timer);
        lv_timer_pause(timer);
        lv_obj_del(center_unlock);
        page_lock_cb(HL_UNLOCK_STATUS_HIDE);
    }
}

static void hl_all_del_center_icon(void)
{
    if (center_icon_sta.lock) {
        center_icon_sta.lock = 0;
        lv_timer_reset(timer);
        lv_timer_pause(timer);
        lv_obj_del(center_lock);
    }
    if (center_icon_sta.line_out) {
        center_icon_sta.line_out = 0;
        lv_timer_reset(timer);
        lv_timer_pause(timer);
        lv_obj_del(center_line_out);
    }
    if (center_icon_sta.usb_c) {
        center_icon_sta.usb_c = 0;
        lv_timer_reset(timer);
        lv_timer_pause(timer);
        lv_obj_del(center_usb_c);
    }
    if (center_icon_sta.heatset) {
        center_icon_sta.heatset = 0;
        lv_timer_reset(timer);
        lv_timer_pause(timer);
        lv_obj_del(center_heatset);
    }
    if (center_icon_sta.unlock) {
        center_icon_sta.unlock = 0;
        lv_timer_reset(timer);
        lv_timer_pause(timer);
        lv_obj_del(center_unlock);
    }
}

static void centert_icon_timer_cb(lv_timer_t* timer)
{
    if (center_icon_sta.lock) {
        center_icon_sta.lock = 0;
        lv_obj_del(center_lock);
        hl_add_top_icon(HL_TOP_ICON_LOCK);
        page_lock_cb(HL_LOCK_STATUS_HIDE);
    }
    if (center_icon_sta.line_out) {
        center_icon_sta.line_out = 0;
        lv_obj_del(center_line_out);
        hl_add_top_icon(HL_TOP_ICON_LINEOUT);
    }
    if (center_icon_sta.usb_c) {
        center_icon_sta.usb_c = 0;
        lv_obj_del(center_usb_c);
        hl_add_top_icon(HL_TOP_ICON_TYPEC);
    }
    if (center_icon_sta.heatset) {
        center_icon_sta.heatset = 0;
        lv_obj_del(center_heatset);
        hl_add_top_icon(HL_TOP_ICON_HEATSET);
    }
    if (center_icon_sta.unlock) {
        center_icon_sta.unlock = 0;
        lv_obj_del(center_unlock);
        page_lock_cb(HL_UNLOCK_STATUS_HIDE);
        // hl_delete_top_icon(HL_TOP_ICON_LOCK);
    }
    lv_timer_pause(timer);
}

static void hl_add_center_icon(hl_top_icon_t icon)
{
    hl_del_center_icon();
    switch (icon) {
        case HL_TOP_ICON_LOCK:
            center_icon_sta.lock = 1;
            lv_timer_reset(timer);
            lv_timer_resume(timer);
            center_lock = lv_center_img_creat_fun(&Other_lock, 0, 0);
            break;
        case HL_TOP_ICON_LINEOUT:
            center_icon_sta.line_out = 1;
            lv_timer_reset(timer);
            lv_timer_resume(timer);
            center_line_out = lv_center_img_creat_fun(&Other_line_out, 0, 0);
            break;
        case HL_TOP_ICON_TYPEC:
            center_icon_sta.usb_c = 1;
            lv_timer_reset(timer);
            lv_timer_resume(timer);
            center_usb_c = lv_center_img_creat_fun(&Other_usb_c, 0, 0);
            break;
        case HL_TOP_ICON_HEATSET:
            center_icon_sta.heatset = 1;
            lv_timer_reset(timer);
            lv_timer_resume(timer);
            center_heatset = lv_center_img_creat_fun(&Other_heatset, 0, 0);
            break;
        case HL_TOP_ICON_UNLOCK:
            center_icon_sta.unlock = 1;
            lv_timer_reset(timer);
            lv_timer_resume(timer);
            hl_delete_top_icon(HL_TOP_ICON_LOCK);
            center_unlock = lv_center_img_creat_fun(&Other_unlock, 0, 0);
            break;
        default:
            break;
    }
}

static void lv_delete_style(void)
{
    lv_style_reset(&style_power_bar_green_indicator);
    lv_style_reset(&style_power_bar_white_indicator);
    lv_style_reset(&style_power_bar_red_indicator);
    lv_style_reset(&style_power_bar_main);
    lv_style_reset(&style_power_label);
}

static void hl_delete_top_icon(hl_top_icon_t icon)
{
    switch (icon) {
        case HL_TOP_ICON_STEREO_MOD:
        case HL_TOP_ICON_SINGLE_MOD:
        case HL_TOP_ICON_TRACK_MOD:
            if (top_icon_sta.voice_mod == 0) {
                rt_kprintf("voice_mod is not exist!\n");
                return;
            }
            top_icon_sta.voice_mod = 0;
            hl_top_icon_delete(HL_TOP_ICON_STEREO_MOD, ICON_POS_LIFT, head_left_list);
            break;
        case HL_TOP_ICON_NOISE:
            if (top_icon_sta.noise == 0) {
                rt_kprintf("Top noise is not exist!\n");
                return;
            }
            top_icon_sta.noise = 0;
            hl_top_icon_delete(icon, ICON_POS_LIFT, head_left_list);
            break;
        case HL_TOP_ICON_LOCK:
            if (top_icon_sta.lock == 0) {
                rt_kprintf("Top lock is not exist!\n");
                return;
            }
            top_icon_sta.lock = 0;
            hl_top_icon_delete(icon, ICON_POS_LIFT, head_left_list);
            break;
        case HL_TOP_ICON_APPLE:
            if (top_icon_sta.apple == 0) {
                rt_kprintf("Top apple is not exist!\n");
                return;
            }
            top_icon_sta.apple = 0;
            hl_top_icon_delete(icon, ICON_POS_LIFT, head_left_list);
            break;
        case HL_TOP_ICON_LINEOUT:
            if (top_icon_sta.lineout == 0) {
                rt_kprintf("Top lineout is not exist!\n");
                return;
            }
            top_icon_sta.lineout = 0;
            hl_top_icon_delete(icon, ICON_POS_RIGHT, head_right_list);
            break;
        case HL_TOP_ICON_TYPEC:
            if (top_icon_sta.typec == 0) {
                rt_kprintf("Top typec is not exist!\n");
                return;
            }
            top_icon_sta.typec = 0;
            hl_top_icon_delete(icon, ICON_POS_RIGHT, head_right_list);
            break;
        case HL_TOP_ICON_HEATSET:
            if (top_icon_sta.heatset == 0) {
                rt_kprintf("Top heatset is not exist!\n");
                return;
            }
            top_icon_sta.heatset = 0;
            hl_top_icon_delete(icon, ICON_POS_RIGHT, head_right_list);
            break;
        default:
            break;
    }
}

static lv_obj_t* hl_mod_icon_obj(hl_top_icon_t icon_typ)
{
    lv_obj_t* obj = NULL;
    switch (icon_typ) {
        case HL_TOP_ICON_STEREO_MOD:
        case HL_TOP_ICON_SINGLE_MOD:
        case HL_TOP_ICON_TRACK_MOD:
            obj = hl_top_icon_obj_get(icon_typ, head_left_list);
            break;
        case HL_TOP_ICON_NOISE:
            obj = hl_top_icon_obj_get(icon_typ, head_left_list);
            break;
        case HL_TOP_ICON_LOCK:
            obj = hl_top_icon_obj_get(icon_typ, head_left_list);
            break;
        case HL_TOP_ICON_LINEOUT:
            obj = hl_top_icon_obj_get(icon_typ, head_right_list);
            break;
        case HL_TOP_ICON_TYPEC:
            obj = hl_top_icon_obj_get(icon_typ, head_right_list);
            break;
        case HL_TOP_ICON_HEATSET:
            obj = hl_top_icon_obj_get(icon_typ, head_right_list);
            break;
        default:
            break;
    }
    return obj;
}

static void lock_cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED) {
        rt_kprintf("reset lock\n");
        hl_add_center_icon(HL_TOP_ICON_LOCK);
    }
}

static lv_obj_t* lv_area_creat_fun(lv_align_t align, lv_event_cb_t event_cb, lv_coord_t x_offset, lv_coord_t y_offset,
                                   lv_coord_t width, lv_coord_t high)
{
    lv_obj_t* null_area;
    null_area = lv_obj_create(lv_scr_act());
    lv_obj_add_style(null_area, &style_area_main, LV_PART_MAIN);
    lv_obj_clear_flag(null_area, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(null_area, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(null_area, width, high);
    lv_obj_align(null_area, align, x_offset, y_offset);
    return null_area;
}

static void lv_area_del()
{
    if (area != RT_NULL) {
        lv_obj_remove_event_cb(area, lock_cb);
        lv_obj_del(area);
        area = RT_NULL;
    }
}

void hl_mod_top_ioctl(void* ctl_data)
{
    char                 buf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    hl_lvgl_top_ioctl_t* ptr    = (hl_lvgl_top_ioctl_t*)ctl_data;
    switch (ptr->top_cmd) {
        case HL_TOP_INPUT_CMD:
            hl_add_center_icon(ptr->top_param);
            break;
        case HL_TOP_ADD_ICON_CMD:
            hl_add_top_icon(ptr->top_param);
            break;
        case HL_TOP_DELETE_ICON_CMD:
            hl_delete_top_icon(ptr->top_param);
            break;
        case HL_TOP_BAT_VAL:
            lv_bar_set_value(bat_bar, ptr->electric_top, LV_ANIM_ON);
            // lv_snprintf(buf, sizeof(buf), "%d%%", ptr->electric_top);
            // lv_label_set_text(bat_label, buf);
            break;

        case HL_TOP_BAT_CHARGER_HIDE:

            lv_obj_add_flag(bat_charger_icon, LV_OBJ_FLAG_HIDDEN);
            break;
        case HL_TOP_BAT_CHARGER_NOT_HIDE:

            lv_obj_clear_flag(bat_charger_icon, LV_OBJ_FLAG_HIDDEN);
            break;
        case HL_TOP_BAT_COLOR_GREEN:
            lv_obj_remove_style(bat_bar, &style_power_bar_green_indicator, LV_PART_INDICATOR);
            lv_obj_add_style(bat_bar, &style_power_bar_green_indicator, LV_PART_INDICATOR);
            break;

        case HL_TOP_BAT_COLOR_WHITE:
            lv_obj_remove_style(bat_bar, &style_power_bar_white_indicator, LV_PART_INDICATOR);
            lv_obj_add_style(bat_bar, &style_power_bar_white_indicator, LV_PART_INDICATOR);
            break;

        case HL_TOP_BAT_COLOR_RED:
            lv_obj_remove_style(bat_bar, &style_power_bar_red_indicator, LV_PART_INDICATOR);
            lv_obj_add_style(bat_bar, &style_power_bar_red_indicator, LV_PART_INDICATOR);
            break;

        case HL_TOP_GET_ICON_OBJ_CMD:
            ptr->icon_obj = hl_mod_icon_obj(ptr->top_param);
            break;

        case HL_TOP_ALL_DEL:
            hl_all_del_center_icon();
            lv_area_del();
            *(uint8_t*)&top_icon_sta    = 0;
            *(uint8_t*)&center_icon_sta = 0;
            lv_timer_del(timer);
            lv_anim_del_all();
            lv_obj_clean(lv_scr_act());
            break;
        case HL_TOP_CENTER_DEL:
            hl_del_center_icon();
            break;
        case HL_TOP_DELETE_STYLE:
            lv_delete_style();
            break;
        case HL_TOP_LOCK_TP_FORBIDDEN:
            if (area != RT_NULL) {
                lv_obj_add_event_cb(area, lock_cb, LV_EVENT_CLICKED, NULL);
            } else {
                rt_kprintf("area not exist\n");
            }
            break;
        case HL_TOP_UNKLOCK_TP:
            if (area != RT_NULL) {
                lv_obj_remove_event_cb(area, lock_cb);
            } else {
                rt_kprintf("area not exist\n");
            }
            break;
        default:
            break;
    }
}

void hl_mod_top_init(void* init_data)
{
    hl_lvgl_top_init_t* ptr = (hl_lvgl_top_init_t*)init_data;
    if (!page_style_bit.page_top) {
        page_style_bit.page_top = 1;
        hl_top_icon_init();
        lv_style_page_top_init();
    }

    bat_icon         = lv_power_img_creat_fun(lv_scr_act(), 0, 0, 256);
    bat_bar          = lv_power_bar_creat_fun(bat_icon, 3, 0, 25, 14, ptr->electric_top);
    bat_charger_icon = lv_img_creat_fun(bat_icon, LV_ALIGN_CENTER, &Main_charging, -2, -2);
}
