/**
 * @file page_lock.c
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief 锁屏或者插入时动画界面
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
#include "page_lock.h"
#include "page_style_bit.h"

LV_IMG_DECLARE(Other_lock);//
LV_IMG_DECLARE(Other_unlock);

LV_IMG_DECLARE(Other_line_out);     //
LV_IMG_DECLARE(Other_heatset);  //
LV_IMG_DECLARE(Other_usb_c);    //

#define LOCK_HOLD_TIME  2000
#define LOCK_ANIM_TIME  500

#define IMG_ROOM_MAX    256
#define IMG_ROOM_MIN    128

static lv_style_t style_area_main;
static lv_obj_t * img_lock,*img_unlock, * area;
static lv_timer_t * timer;

static uint8_t lock_flag = 0;

static hl_lock_event_cb page_lock_cb;

static lv_obj_t * lv_lock_img_creat_fun(lv_obj_t *align_obj,const void * src,lv_coord_t x_offset,lv_coord_t y_offset,uint16_t zoom)
{
    lv_obj_t * img = lv_img_create(align_obj);
    lv_img_set_src(img,src);
    lv_img_set_zoom(img, zoom);
    lv_obj_align_to(img,align_obj,LV_ALIGN_CENTER,x_offset,y_offset);
    lv_obj_add_flag(img,LV_OBJ_FLAG_HIDDEN);
    return img;
}

static lv_obj_t * lv_area_creat_fun(lv_align_t align,lv_event_cb_t event_cb,lv_coord_t x_offset,lv_coord_t y_offset,lv_coord_t width, lv_coord_t high)
{
    lv_obj_t * null_area;
    null_area = lv_obj_create(lv_scr_act());
    lv_obj_add_style(null_area, &style_area_main, LV_PART_MAIN);
    lv_obj_clear_flag(null_area, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(null_area, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(null_area,width,high);
    lv_obj_align(null_area,align,x_offset,y_offset);
    lv_obj_add_event_cb(null_area, event_cb, LV_EVENT_CLICKED, NULL);
    return null_area;
}

static void lv_lock_style_init(void)
{
    //lv_area_creat_fun函数使用
    lv_style_init(&style_area_main);
    lv_style_set_bg_opa(&style_area_main, LV_OPA_TRANSP);
    // lv_style_set_bg_color(&style_area_main, lv_palette_darken(LV_PALETTE_GREY,4));
    lv_style_set_border_width(&style_area_main,0);
    lv_style_set_outline_width(&style_area_main,0);
    lv_style_set_radius(&style_area_main, 0);
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

static void lock_timer(lv_timer_t * timer)
{
    if(lock_flag){
        lv_obj_add_flag(img_lock,LV_OBJ_FLAG_HIDDEN);
        page_lock_cb(HL_LOCK_STATUS_HIDE);
    }else{
        lv_timer_del(timer);
        hl_obj_delete(area,true);
        page_lock_cb(HL_UNLOCK_STATUS_HIDE);
    }
}

static void lock_cb(lv_event_t * e)
{    
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        lv_obj_clear_flag(img_lock,LV_OBJ_FLAG_HIDDEN);
        lv_timer_reset(timer);
    } 
}

typedef void (* anim_start)(void);

typedef struct _icon_anim_obj_t{
    lv_obj_t *hand_obj;
    lv_obj_t *target_obj;
    lv_anim_t *anim_obj;
}icon_anim_obj_t;

typedef struct _icon_pos_ani_t{
    lv_point_t start_pos;
    lv_point_t end_pos;
}icon_pos_ani_t;

typedef struct _icon_anim_data_t{
    int32_t anim_size_min;
    int32_t anim_size_max;
    lv_anim_exec_xcb_t anim_size_cb;
    lv_anim_exec_xcb_t anim_x_cb;
    lv_anim_exec_xcb_t anim_y_cb;
    icon_anim_obj_t obj;
    icon_pos_ani_t pos;
    anim_start anim_start_cb;
    const void * img_src;
}icon_anim_data_t;

static lv_anim_t line_out_anim,heatset_anim,usb_c_anim,lock_anim;

static icon_anim_data_t icon_data[4] = 
{
    {.img_src = &Other_line_out,.obj.anim_obj = &line_out_anim},
    {.img_src = &Other_heatset,.obj.anim_obj = &heatset_anim},
    {.img_src = &Other_usb_c,.obj.anim_obj = &usb_c_anim},
    {.img_src = &Other_lock,.obj.anim_obj = &lock_anim},
};

static void anim_x_cb(void * var, int32_t v)
{
    lv_obj_set_x(var, v);
}

static void anim_y_cb(void * var, int32_t v)
{
    lv_obj_set_y(var, v);
}

static void anim_size_cb(void * var, int32_t v)
{
    lv_img_set_zoom(var, v);//缩放
    if(v == IMG_ROOM_MIN){
        lv_obj_del_async(var);
    }
}

static void get_icon_coord(lv_obj_t *obj,lv_point_t *coord)
{
    coord->x = obj->coords.x1;
    coord->y = obj->coords.y1;
}

/**
 * Create a playback animation
 */
static lv_obj_t * lv_creat_anim(icon_anim_data_t *data)
{
    lv_coord_t x_offset,y_offset;
    lv_point_t target;
    lv_obj_t * img = lv_img_create(lv_scr_act());
    lv_img_set_src(img, data->img_src);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_zoom(img, 256);
    lv_anim_init(data->obj.anim_obj);
    lv_anim_set_var(data->obj.anim_obj, img);
    lv_anim_set_time(data->obj.anim_obj, LOCK_ANIM_TIME);
    lv_anim_set_delay(data->obj.anim_obj, LOCK_HOLD_TIME);

    lv_obj_update_layout(lv_scr_act());
    get_icon_coord(img,&data->pos.start_pos);
    get_icon_coord(data->obj.target_obj,&data->pos.end_pos);

    x_offset = data->pos.end_pos.x + (lv_obj_get_width(data->obj.target_obj) - lv_obj_get_width(img))/2 - data->pos.start_pos.x;
    y_offset = data->pos.end_pos.y + (lv_obj_get_height(data->obj.target_obj) - lv_obj_get_height(img))/2 - data->pos.start_pos.y;
    data->pos.start_pos.x = 0;
    data->pos.start_pos.y = 0;

    lv_anim_set_exec_cb(data->obj.anim_obj, data->anim_size_cb);
    lv_anim_set_values(data->obj.anim_obj, data->anim_size_max, data->anim_size_min);
    lv_anim_start(data->obj.anim_obj);
    lv_anim_set_exec_cb(data->obj.anim_obj, data->anim_x_cb);
    lv_anim_set_values(data->obj.anim_obj, data->pos.start_pos.x, x_offset);
    lv_anim_start(data->obj.anim_obj);
    lv_anim_set_exec_cb(data->obj.anim_obj, data->anim_y_cb);
    lv_anim_set_values(data->obj.anim_obj, data->pos.start_pos.y, y_offset);
    lv_anim_start(data->obj.anim_obj);
    return img;
}

static void lv_creat_icon_anim(icon_anim_data_t *data,lv_obj_t *tar_obj)
{
    data->obj.target_obj = tar_obj;    
    data->anim_size_max = IMG_ROOM_MAX;
    data->anim_size_min = IMG_ROOM_MIN;
    data->anim_size_cb = anim_size_cb;
    data->anim_x_cb = anim_x_cb;
    data->anim_y_cb = anim_y_cb;
    data->obj.hand_obj = lv_creat_anim(data);
}

static void lv_lock_icon_anmi_init(lv_obj_t *obj,hl_lock_icon_t icon_typ)
{
    switch(icon_typ){
        case HL_LOCK_ICON_LINEOUT:
            lv_creat_icon_anim(&icon_data[0],obj);
            break;
        case HL_LOCK_ICON_TYPEC:
            lv_creat_icon_anim(&icon_data[2],obj);
            break;
        case HL_LOCK_ICON_HEATSET:
            lv_creat_icon_anim(&icon_data[1],obj);
            break;
        case HL_LOCK_ICON_LOCK:
            lv_creat_icon_anim(&icon_data[3],obj);
            break;
        default:
            break;
    }
}

static void hl_mod_lock_init(void)
{
    if (!page_style_bit.page_lock){
        lv_lock_style_init();
        page_style_bit.page_lock = 1;
    }    
    if(lock_flag == 0){
        area = lv_area_creat_fun(LV_ALIGN_CENTER,lock_cb,0,0,294,126);
        img_lock = lv_lock_img_creat_fun(area,&Other_lock,0,0,IMG_ROOM_MAX);
        img_unlock = lv_lock_img_creat_fun(area,&Other_unlock,0,0,IMG_ROOM_MAX);
        lv_obj_clear_flag(img_lock,LV_OBJ_FLAG_HIDDEN);
        timer = lv_timer_create(lock_timer, LOCK_HOLD_TIME,  NULL);
        lock_flag = 1;
    }    
} 

static void hl_mod_lock_deinit(void)
{
    if(lock_flag == 1){
        lv_obj_clear_flag(img_unlock,LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(img_lock,LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_event_cb(area,lock_cb);
        lv_timer_reset(timer);
        lock_flag = 0;
    }
}

void hl_mod_lock_ioctl(void * ctl_data)
{
    hl_lvgl_lock_ioctl_t * ptr = (hl_lvgl_lock_ioctl_t *)ctl_data;
    switch(ptr->cmd){
        case HL_LOCK_STATUS_INTO:
            page_lock_cb = ptr->lock_event_cb;
            hl_mod_lock_init();
            break;
        case HL_LOCK_STATUS_BACK:
            hl_mod_lock_deinit();
            break;
        case HL_LOCK_ICON_FRONT:
            if(lock_flag == 1){
                lv_obj_move_foreground(area);
            }
            break;
        case HL_LOCK_ICON_ANIM:
            lv_lock_icon_anmi_init(ptr->icon_obj,ptr->icon_typ);
            break;
        default:
            break;
    }    
}

static void page_lock_test_cb(hl_lvgl_lock_sta_t sta)
{
    ///
}

void page_lock_test_ctl(int argc, char** argv)
{
	hl_lvgl_lock_ioctl_t tmp;
    tmp.lock_event_cb = page_lock_test_cb;
	if (!strcmp("lock", argv[1])) {
        tmp.cmd = HL_LOCK_STATUS_INTO;
        hl_mod_lock_ioctl(&tmp);
    }else if (!strcmp("unlock", argv[1])){
        tmp.cmd = HL_LOCK_STATUS_BACK;
        hl_mod_lock_ioctl(&tmp);
	}else if (!strcmp("front", argv[1])){
        tmp.cmd = HL_LOCK_ICON_FRONT;
        hl_mod_lock_ioctl(&tmp);
	}
}

MSH_CMD_EXPORT(page_lock_test_ctl, run page_lock_test_ctl);
