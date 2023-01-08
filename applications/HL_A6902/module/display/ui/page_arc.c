#include "page_arc.h"
#include "page_style_bit.h"

static lv_obj_t * arc;
static lv_obj_t * lab;

static lv_style_t style_arc_main,style_arc_indicator,style_arc_label;

static lv_anim_t anim;

static void lv_style_page_arc_init(void)
{
    lv_style_init(&style_arc_main);
    lv_style_set_arc_width(&style_arc_main,10);
    lv_style_set_arc_opa(&style_arc_main,LV_OPA_50);

    lv_style_init(&style_arc_indicator);
    lv_style_set_arc_width(&style_arc_indicator,10);

    lv_style_init(&style_arc_label);
    lv_style_set_bg_opa(&style_arc_label, LV_OPA_TRANSP);
    lv_style_set_text_opa(&style_arc_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_arc_label, lv_color_white());
}

static void set_angle(void * obj, int32_t v)
{
    if(v < 180){
        lv_arc_set_angles(obj,v,v+v/2);
    }else{
        lv_arc_set_angles(obj,2*v - 180,90 + v);
    }  
}

static void lv_arc_page_anim_init(lv_anim_t * anim_obj,lv_obj_t * arc_obj)
{
    lv_anim_init(anim_obj);
    lv_anim_set_var(anim_obj, arc_obj);
    lv_anim_set_exec_cb(anim_obj, set_angle);
    lv_anim_set_time(anim_obj, 1000);
    lv_anim_set_repeat_count(anim_obj, LV_ANIM_REPEAT_INFINITE);
    // lv_anim_set_repeat_delay(anim_obj, 500);
    lv_anim_set_values(anim_obj, 0, 270);
    lv_anim_start(anim_obj);
}

static lv_obj_t * lv_arc_lab_creat_fun(lv_obj_t *align_obj,lv_coord_t x_offset,lv_coord_t y_offset,const char *ptr)
{
    lv_obj_t * lab = lv_label_create(align_obj);
    lv_obj_add_style(lab, &style_arc_label, LV_PART_MAIN);
    lv_label_set_text(lab,ptr);
    lv_obj_align_to(lab,align_obj,LV_ALIGN_BOTTOM_MID,x_offset,y_offset);
    return lab;
}

static lv_obj_t * lv_arc_creat_fun(lv_obj_t * src_obj,lv_coord_t x_offset,lv_coord_t y_offset,lv_coord_t width, lv_coord_t high)
{
    lv_obj_t * arc = lv_arc_create(src_obj);
    lv_obj_add_style(arc,&style_arc_main,LV_PART_MAIN);
    lv_obj_add_style(arc,&style_arc_indicator,LV_PART_INDICATOR);
    lv_arc_set_rotation(arc, 270);
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_obj_set_size(arc,width,high);    
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);  /*To not allow adjusting by click*/
    lv_obj_align(arc,LV_ALIGN_CENTER,x_offset,y_offset);
    return arc;
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

static void lv_delete_style(void)
{
    lv_style_reset(&style_arc_main);
    lv_style_reset(&style_arc_indicator);
    lv_style_reset(&style_arc_label);
}

void hl_mod_arc_ioctl(void * ctl_data)
{
    hl_lvgl_arc_ioctl_t * ptr =  (hl_lvgl_arc_ioctl_t *)ctl_data;
    switch(ptr->ioctl_cmd){
        case HL_ARC_DELETE_PAGE:
            hl_obj_delete(lv_scr_act(),false);
            break;
        case HL_ARC_DELETE_STYLE:
            lv_delete_style();
            break;
        default:
            break;
    }
}

void hl_mod_arc_init(void * init_data)
{
    hl_lvgl_arc_init_t * ptr = (hl_lvgl_arc_init_t *)init_data;
    if (!page_style_bit.page_arc) {
        lv_style_page_arc_init();
        page_style_bit.page_arc = 1;
    }
    arc = lv_arc_creat_fun(lv_scr_act(),0,-10,80,80);
    lab = lv_arc_lab_creat_fun(lv_scr_act(),0,0,ptr->ptr);
    lv_arc_page_anim_init(&anim,arc);
}
