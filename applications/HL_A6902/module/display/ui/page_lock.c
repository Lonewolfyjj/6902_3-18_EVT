#include "page_lock.h"
#include "page_style_bit.h"

LV_IMG_DECLARE(Other_lock);
LV_IMG_DECLARE(Other_unlock);

#define LOCK_TIMEOUT    2000

static lv_style_t style_area_main;
static lv_obj_t * img_lock,*img_unlock, * area;
static lv_timer_t * timer;

static uint8_t lock_flag = 0;

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
        lock_flag = 0;
    }
}

static void lock_cb(lv_event_t * e)
{    
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        lv_obj_clear_flag(img_lock,LV_OBJ_FLAG_HIDDEN);
        lv_timer_reset(timer);
        lock_flag = 1;
    }    
}

static void hl_mod_lock_init(void)
{
    if (!page_style_bit.page_lock){
        lv_lock_style_init();
        page_style_bit.page_lock = 1;
    }    
    area = lv_area_creat_fun(LV_ALIGN_CENTER,lock_cb,0,0,294,126);
    img_lock = lv_lock_img_creat_fun(area,&Other_lock,0,0,256);
    img_unlock = lv_lock_img_creat_fun(area,&Other_unlock,0,0,256);
    timer = lv_timer_create(lock_timer, LOCK_TIMEOUT,  NULL);
} 

void hl_mod_lock_ioctl(void * ctl_data)
{
    hl_lvgl_lock_ioctl_t * ptr = (hl_lvgl_lock_ioctl_t *)ctl_data;
    switch(ptr->cmd){
        case HL_LOCK_ICON_SHOW:
            lv_obj_clear_flag(img_lock,LV_OBJ_FLAG_HIDDEN);
            lv_timer_reset(timer);
            lock_flag = 1;
            break;
        case HL_LOCK_ICON_HIDE:
            lv_obj_add_flag(img_lock,LV_OBJ_FLAG_HIDDEN);
            break;

        case HL_UNLOCK_ICON_SHOW:
            lv_obj_clear_flag(img_unlock,LV_OBJ_FLAG_HIDDEN);
            break;
        case HL_UNLOCK_ICON_HIDE:
            lv_obj_add_flag(img_unlock,LV_OBJ_FLAG_HIDDEN);
            break;

        case HL_LOCK_PAGE_INIT:
            hl_mod_lock_init();
            break;
        case HL_LOCK_PAGE_EXIT:
            lv_timer_del(timer);
            hl_obj_delete(area,true);
            break;
        default:
            break;
    }    
}
