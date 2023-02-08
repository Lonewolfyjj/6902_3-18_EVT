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

#define POSTION_IS_NULL 0xFF

typedef struct __icon_pos_t
{
    uint8_t             duf_pos;    //默认位置
    uint8_t             cur_pos;    //当前位置
    lv_align_t          align;      // 对齐方式
    lv_obj_t*           icon;       //图片对象
    const lv_img_dsc_t* icon_data;  //图片数据指针
} icon_pos_t;

LV_IMG_DECLARE(Main_bat);      //电池图标
LV_IMG_DECLARE(Main_line_in);  //耳机插入
LV_IMG_DECLARE(Main_usb_c);    //USB插入
LV_IMG_DECLARE(Main_stereo);   //立体声
LV_IMG_DECLARE(Main_saft_track);   //安全音轨
LV_IMG_DECLARE(Main_single_voice);   //单声道
LV_IMG_DECLARE(Main_lock);     //锁屏
LV_IMG_DECLARE(Main_heatset);  //监听
LV_IMG_DECLARE(Main_noise);    //降噪
LV_IMG_DECLARE(Main_apple);    // 苹果认证

#define ICON_POS_CURRENT 0
#define ICON_POS_DEFAULT 1
#define ICON_NUM 4
#define ICON_POS_VOR 2
#define ICON_POS_LIFT 0
#define ICON_POS_RIGHT 1

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

static HL_DISPLAY_TOP_T top_icon_sta = {
    .heatset = 0,
    .lineout = 0,
    .lock    = 0,
    .noise   = 0,
    .typec   = 0,
    .voice_mod   = 0,
};

static lv_style_t style_power_bar_white_indicator, style_power_bar_green_indicator,style_power_bar_main,style_power_bar_red_indicator;
static lv_style_t style_power_label;

static lv_obj_t *bat_icon, *bat_bar, *bat_label;

static icon_pos_t icon_list_l[ICON_NUM] = {
    { .duf_pos = 0, .cur_pos = POSTION_IS_NULL, .align = LV_ALIGN_TOP_LEFT, .icon_data = &Main_stereo },
    { .duf_pos = 1, .cur_pos = POSTION_IS_NULL, .align = LV_ALIGN_TOP_LEFT, .icon_data = &Main_noise },
    { .duf_pos = 2, .cur_pos = POSTION_IS_NULL, .align = LV_ALIGN_TOP_LEFT, .icon_data = &Main_lock },
    { .duf_pos = 3, .cur_pos = POSTION_IS_NULL, .align = LV_ALIGN_TOP_LEFT, .icon_data = &Main_apple },
};

static icon_pos_t icon_list_r[ICON_NUM] = {
    { .duf_pos = 0, .cur_pos = POSTION_IS_NULL, .align = LV_ALIGN_TOP_RIGHT, .icon_data = &Main_line_in },
    { .duf_pos = 1, .cur_pos = POSTION_IS_NULL, .align = LV_ALIGN_TOP_RIGHT, .icon_data = &Main_usb_c },
    { .duf_pos = 2, .cur_pos = POSTION_IS_NULL, .align = LV_ALIGN_TOP_RIGHT, .icon_data = &Main_heatset },
};

static uint8_t icon_pos_judge(uint8_t icon_pos, icon_pos_t* icon_list, uint8_t cmd_typ)
{
    uint8_t i;
    if (ICON_POS_DEFAULT == cmd_typ) {  //
        for (i = 0; i < ICON_NUM; i++) {
            if (icon_list[i].cur_pos == icon_pos) {
                return icon_list[i].duf_pos;
            }
        }
    }
    if (ICON_POS_CURRENT == cmd_typ) {
        for (i = 0; i < ICON_NUM; i++) {
            if (icon_list[i].duf_pos == icon_pos) {
                return icon_list[i].cur_pos;
            }
        }
    }
    return 0;
}

static void delete_icon_pos_set(icon_pos_t* icon, icon_pos_t* icon_list, uint8_t duf_pos, uint8_t icon_typ)
{
    uint8_t  i, is_used = 0;
    uint8_t  icon_pos, icon_ar[ICON_NUM];
    int16_t icon_offset,ori;
    if (icon_typ == ICON_POS_LIFT) {
        icon_offset = 0;
        ori = 1;
    }
    if (icon_typ == ICON_POS_RIGHT) {
        icon_offset = -50;
        ori = -1;
    }

    for (i = 0; i < ICON_NUM; i++) {
        if (icon_list[i].cur_pos != POSTION_IS_NULL) {
            is_used++;  //已经绘制的图标个数
        }
    }
    if (is_used == 0) {  //
        return;
    }
    icon_pos = icon_pos_judge(duf_pos, icon_list, ICON_POS_CURRENT);  //获取图标的当前位置
    if (icon_pos + 1 == is_used) {
        lv_obj_del(icon_list[duf_pos].icon);
        icon_list[duf_pos].cur_pos = POSTION_IS_NULL;
        return;
    }
    for (i = 0; i < is_used; i++) {
        icon_ar[i] = icon_pos_judge(i, icon_list, ICON_POS_DEFAULT);  //获取已绘制图标的默认位置排序
    }
    for (i = icon_pos + 1; i < is_used; i++) {
        icon_list[icon_ar[i]].cur_pos -= 1;
        lv_obj_align(icon_list[icon_ar[i]].icon, icon_list[icon_ar[i]].align,
                     icon_list[icon_ar[i]].cur_pos * 20 * ori + icon_offset, ICON_POS_VOR);
    }
    lv_obj_del(icon_list[duf_pos].icon);
    icon_list[duf_pos].cur_pos = POSTION_IS_NULL;
}

static void add_icon_pos_set(icon_pos_t* icon, icon_pos_t* icon_list, uint8_t icon_typ)
{
    uint8_t  i, j, is_used = 0, new_f = 1;
    uint8_t  icon_ar[ICON_NUM];
    int16_t icon_offset,ori,icon_sign;
    if (icon_typ == ICON_POS_LIFT) {
        icon_offset = 10; // 最左图标距离对齐边的距离
        ori = 5; // 间隔
        icon_sign = 1;
    }
    if (icon_typ == ICON_POS_RIGHT) {
        icon_offset = -50;
        ori = 0;
        icon_sign = -1;
    }

    for (i = 0; i < ICON_NUM; i++) {
        if (icon_list[i].cur_pos != POSTION_IS_NULL) {
            is_used++;  //已经绘制的图标个数
        }
    }
    if (is_used == 0) {       //
        icon[0].cur_pos = 0;  //第一个位置绘制图标
        lv_obj_align(icon[0].icon, icon[0].align, 2 + icon_offset, ICON_POS_VOR);
        return;
    }
    for (i = 0; i < is_used; i++) {
        icon_ar[i] = icon_pos_judge(i, icon_list, ICON_POS_DEFAULT);  //获取已绘制图标的默认位置排序
    }
    for (i = 0; i < is_used; i++) {
        if (icon[0].duf_pos > icon_list[icon_ar[i]].duf_pos) {  //优先级低
            continue;
        } else if (icon[0].duf_pos == icon_list[icon_ar[i]].duf_pos) {  //重复绘制
            return;
        } else {
            new_f           = 0;
            icon[0].cur_pos = icon_list[icon_ar[i]].cur_pos;
            lv_obj_align(icon[0].icon, icon[0].align, icon[0].cur_pos * (20 + ori)*icon_sign + icon_offset, ICON_POS_VOR);
            for (j = i; j < is_used; j++) {
                icon_list[icon_ar[j]].cur_pos += 1;
                lv_obj_align(icon_list[icon_ar[j]].icon, icon_list[icon_ar[j]].align,
                             icon_list[icon_ar[j]].cur_pos * (20 + ori)*icon_sign + icon_offset, ICON_POS_VOR);
            }
            return;
        }
    }
    if (new_f) {
        icon[0].cur_pos = is_used;
        lv_obj_align(icon[0].icon, icon[0].align, icon[0].cur_pos * (20 + ori)*icon_sign + icon_offset, ICON_POS_VOR);
    }
}

static void hl_mod_creat_top_icon(icon_pos_t* icon_data, icon_pos_t* icon_list, uint8_t icon_typ)
{
    if (icon_data->icon == NULL) {
        icon_data->icon = lv_img_create(lv_scr_act());
        lv_img_set_src(icon_data->icon, icon_data->icon_data);
        lv_img_set_zoom(icon_data->icon, 256);
        
        // if(&icon_list_l[0] == icon_data){
        //     lv_img_set_zoom(icon_data->icon, 220);
        // }else{
        //     lv_img_set_zoom(icon_data->icon, 256);
        // }


        
        add_icon_pos_set(icon_data, icon_list, icon_typ);
    } else {
        add_icon_pos_set(icon_data, icon_list, icon_typ);
    }
}

static void lv_style_page_top_init(void)
{
    lv_obj_enable_style_refresh(true);
    // static lv_style_t style;
    // lv_style_init(&style);
    // lv_style_set_bg_color(&style, lv_color_black());
    // lv_style_set_border_width(&style, 0);
    //lv_obj_add_style(lv_scr_act(), &style, 0);

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

static void hl_add_top_icon(hl_top_icon_t icon)
{
    switch (icon) {
        case HL_TOP_ICON_STEREO_MOD://
        case HL_TOP_ICON_SINGLE_MOD://
        case HL_TOP_ICON_TRACK_MOD://
            if(top_icon_sta.voice_mod == 1){
                rt_kprintf("voice_mod is exist!\n");
                return ;
            }
            top_icon_sta.voice_mod = 1;
            if(icon == HL_TOP_ICON_STEREO_MOD){
                icon_list_l[0].icon_data = &Main_stereo;
            }
            if(icon == HL_TOP_ICON_SINGLE_MOD){
                icon_list_l[0].icon_data = &Main_single_voice;
            }
            if(icon == HL_TOP_ICON_TRACK_MOD){
                icon_list_l[0].icon_data = &Main_saft_track;
            }
            hl_mod_creat_top_icon(&icon_list_l[0], icon_list_l, ICON_POS_LIFT);
            break;
        case HL_TOP_ICON_NOISE:
            if(top_icon_sta.noise == 1){
                rt_kprintf("Top noise is exist!\n");
                return ;
            }
            top_icon_sta.noise = 1;
            hl_mod_creat_top_icon(&icon_list_l[1], icon_list_l, ICON_POS_LIFT);
            break;
        case HL_TOP_ICON_LOCK:
            if(top_icon_sta.lock == 1){
                rt_kprintf("Top lock is exist!\n");
                return ;
            }
            top_icon_sta.lock = 1;
            hl_mod_creat_top_icon(&icon_list_l[2], icon_list_l, ICON_POS_LIFT);
            break;
        case HL_TOP_ICON_LINEOUT:
            if(top_icon_sta.lineout == 1){
                rt_kprintf("Top lineout is exist!\n");
                return ;
            }
            top_icon_sta.lineout = 1;
            hl_mod_creat_top_icon(&icon_list_r[0], icon_list_r, ICON_POS_RIGHT);
            break;
        case HL_TOP_ICON_TYPEC:
            if(top_icon_sta.typec == 1){
                rt_kprintf("Top typec is exist!\n");
                return ;
            }
            top_icon_sta.typec = 1;
            hl_mod_creat_top_icon(&icon_list_r[1], icon_list_r, ICON_POS_RIGHT);
            break;
        case HL_TOP_ICON_HEATSET:
            if(top_icon_sta.heatset == 1){
                rt_kprintf("Top heatset is exist!\n");
                return ;
            }
            top_icon_sta.heatset = 1;
            hl_mod_creat_top_icon(&icon_list_r[2], icon_list_r, ICON_POS_RIGHT);
            break;
        case HL_TOP_ICON_APPLE:
            if(top_icon_sta.apple == 1){
                rt_kprintf("Top apple is exist!\n");
                return ;
            }
            top_icon_sta.apple = 1;
            hl_mod_creat_top_icon(&icon_list_l[3], icon_list_l, ICON_POS_LIFT);
            break;
        default:
            break;
    }
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
            if(top_icon_sta.voice_mod == 0){
                rt_kprintf("voice_mod is not exist!\n");
                return ;
            }
            top_icon_sta.voice_mod = 0;
            delete_icon_pos_set(&icon_list_l[0], icon_list_l, 0, ICON_POS_LIFT);
            icon_list_l[0].icon = NULL;
            break;
        case HL_TOP_ICON_NOISE:
            if(top_icon_sta.noise == 0){
                rt_kprintf("Top noise is not exist!\n");
                return ;
            }
            top_icon_sta.noise = 0;
            delete_icon_pos_set(&icon_list_l[1], icon_list_l, 1, ICON_POS_LIFT);
            icon_list_l[1].icon = NULL;
            break;
        case HL_TOP_ICON_LOCK:
            if(top_icon_sta.lock == 0){
                rt_kprintf("Top lock is not exist!\n");
                return ;
            }
            top_icon_sta.lock = 0;
            delete_icon_pos_set(&icon_list_l[2], icon_list_l, 2, ICON_POS_LIFT);
            icon_list_l[2].icon = NULL;
            break;
        case HL_TOP_ICON_LINEOUT:
            if(top_icon_sta.lineout == 0){
                rt_kprintf("Top lineout is not exist!\n");
                return ;
            }
            top_icon_sta.lineout = 0;
            delete_icon_pos_set(&icon_list_r[0], icon_list_r, 0, ICON_POS_RIGHT);
            icon_list_r[0].icon = NULL;
            break;
        case HL_TOP_ICON_TYPEC:
            if(top_icon_sta.typec == 0){
                rt_kprintf("Top typec is not exist!\n");
                return ;
            }
            top_icon_sta.typec = 0;
            delete_icon_pos_set(&icon_list_r[1], icon_list_r, 1, ICON_POS_RIGHT);
            icon_list_r[1].icon = NULL;
            break;
        case HL_TOP_ICON_HEATSET:
            if(top_icon_sta.heatset == 0){
                rt_kprintf("Top heatset is not exist!\n");
                return ;
            }
            top_icon_sta.heatset = 0;
            delete_icon_pos_set(&icon_list_r[2], icon_list_r, 2, ICON_POS_RIGHT);
            icon_list_r[2].icon = NULL;
            break;
        case HL_TOP_ICON_APPLE:
            if(top_icon_sta.apple == 0){
                rt_kprintf("Top apple is not exist!\n");
                return ;
            }
            top_icon_sta.apple = 0;
            delete_icon_pos_set(&icon_list_l[3], icon_list_l, 3, ICON_POS_LIFT);
            icon_list_l[3].icon = NULL;
            break;
        default:
            break;
    }
}

static lv_obj_t * hl_mod_icon_obj(hl_top_icon_t icon_typ)
{
    lv_obj_t * obj = NULL;
    switch(icon_typ){
        case HL_TOP_ICON_STEREO_MOD:        
        case HL_TOP_ICON_SINGLE_MOD:
        case HL_TOP_ICON_TRACK_MOD:
            obj = icon_list_l[0].icon;
            break;
        case HL_TOP_ICON_NOISE:
            obj = icon_list_l[1].icon;
            break;
        case HL_TOP_ICON_LOCK:
            obj = icon_list_l[2].icon;
            break;
        case HL_TOP_ICON_LINEOUT:
            obj = icon_list_r[0].icon;
            break;
        case HL_TOP_ICON_TYPEC:
            obj = icon_list_r[1].icon;
            break;
        case HL_TOP_ICON_HEATSET:
            obj = icon_list_r[2].icon;
            break;
        default:
            break;
    }
    return obj;
}

void hl_mod_top_ioctl(void* ctl_data)
{
    // char                 buf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    hl_lvgl_top_ioctl_t* ptr    = (hl_lvgl_top_ioctl_t*)ctl_data;
    switch (ptr->top_cmd) {
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

        case HL_TOP_BAT_COLOR_GREEN:
            lv_obj_remove_style(bat_bar,&style_power_bar_green_indicator,LV_PART_INDICATOR);
            lv_obj_add_style(bat_bar,&style_power_bar_green_indicator,LV_PART_INDICATOR);
            break;

        case HL_TOP_BAT_COLOR_WHITE:
            lv_obj_remove_style(bat_bar,&style_power_bar_white_indicator,LV_PART_INDICATOR);
            lv_obj_add_style(bat_bar,&style_power_bar_white_indicator,LV_PART_INDICATOR);
            break;

        case HL_TOP_BAT_COLOR_RED:
            lv_obj_remove_style(bat_bar,&style_power_bar_red_indicator,LV_PART_INDICATOR);
            lv_obj_add_style(bat_bar,&style_power_bar_red_indicator,LV_PART_INDICATOR);
            break;

        case HL_TOP_GET_ICON_OBJ_CMD:
            ptr->icon_obj = hl_mod_icon_obj(ptr->top_param);
            break;

        case HL_TOP_ALL_DEL:
            *(uint8_t*)&top_icon_sta = 0;
            // hl_obj_delete(lv_scr_act(), false);
            lv_obj_clean(lv_scr_act());
            rt_thread_mdelay(10);
            break;
        case HL_TOP_DELETE_STYLE:
            lv_delete_style();
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
        lv_style_page_top_init();
    }    

    bat_icon  = lv_power_img_creat_fun(lv_scr_act(), 0, 0, 256);
    bat_bar   = lv_power_bar_creat_fun(bat_icon, 3, 0, 25, 14, ptr->electric_top);
}
