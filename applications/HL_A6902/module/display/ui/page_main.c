#include "page_main.h"
#include "language.h"

LV_IMG_DECLARE(Main_horn);//喇叭
LV_IMG_DECLARE(Main_bat);//电池图标
LV_IMG_DECLARE(Main_volumebar_short);//音量柱——短
LV_IMG_DECLARE(Main_volumebar_long);//音量柱——长
LV_IMG_DECLARE(Main_signal_0);//信号
LV_IMG_DECLARE(Main_signal_1);//信号
LV_IMG_DECLARE(Main_signal_2);//信号
LV_IMG_DECLARE(Main_signal_3);//信号
LV_IMG_DECLARE(Main_signal_4);//信号

#define SAFETY_VOICE    0
#define NORMAL_VOICE    1

#define RECORD_DISABLE   0
#define RECORD_ENABLE    1

static lv_obj_t * area_tx1,*area_tx2;
static lv_obj_t * voice_bar_tx1,*voice_bar_tx2;
static lv_obj_t * power_bar_tx1,*power_bar_tx2;
static lv_obj_t * voice_img_tx1,*voice_img_tx2;
static lv_obj_t * power_img_tx1,*power_img_tx2;
static lv_obj_t * voice_lab_tx1,*voice_lab_tx2;
static lv_obj_t * power_lab_tx1,*power_lab_tx2;
static lv_obj_t * device_lab_tx1,*device_lab_tx2;
static lv_obj_t * video_dot_tx1,*video_dot_tx2;
static lv_obj_t * tx1_signal_obj[5];
static lv_obj_t * tx2_signal_obj[5];

static lv_style_t style_area_main;
static lv_style_t style_voice_sbar_indicator,style_voice_bar_main;
static lv_style_t style_voice_lbar_indicator,style_voice_bar_main;
static lv_style_t style_power_bar_indicator,style_power_bar_main;
static lv_style_t style_voice_label;
static lv_style_t style_power_label;
static lv_style_t style_device_label;
static lv_style_t style_video_dot;
    
static hl_lvgl_main_init_t main_init;
/***************************************************************************************************/
/*                                       组件创建函数                                               */
/***************************************************************************************************/

static void lv_signal_group1_hide_set(uint8_t hide_num)
{
    uint8_t i;
    for(i=0;i<5;i++){
        lv_obj_add_flag(tx1_signal_obj[i],LV_OBJ_FLAG_HIDDEN);
    }
    lv_obj_clear_flag(tx1_signal_obj[hide_num],LV_OBJ_FLAG_HIDDEN);
}

static void lv_signal_group2_hide_set(uint8_t hide_num)
{
    uint8_t i;
    for(i=0;i<5;i++){
        lv_obj_add_flag(tx2_signal_obj[i],LV_OBJ_FLAG_HIDDEN);
    }
    lv_obj_clear_flag(tx2_signal_obj[hide_num],LV_OBJ_FLAG_HIDDEN);
}

static void lv_style_page1_init(void)
{
    lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);
    //lv_area_creat_fun函数使用
    lv_style_init(&style_area_main);
    lv_style_set_bg_opa(&style_area_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_area_main, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_border_width(&style_area_main,0);
    lv_style_set_outline_width(&style_area_main,0);
    lv_style_set_radius(&style_area_main, 5);

    //lv_voice_sbar_creat_fun函数使用
    lv_style_init(&style_voice_sbar_indicator);
    lv_style_set_bg_opa(&style_voice_sbar_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_voice_sbar_indicator, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_bg_img_src(&style_voice_sbar_indicator,&Main_volumebar_short);//使用图片做源
    lv_style_set_radius(&style_voice_sbar_indicator, 0);

    lv_style_init(&style_voice_bar_main);
    lv_style_set_bg_opa(&style_voice_bar_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_voice_bar_main, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_bg_color(&style_voice_bar_main, lv_palette_lighten(LV_PALETTE_GREY,1));
    lv_style_set_radius(&style_voice_bar_main, 0);

    //lv_voice_lbar_creat_fun函数使用
    lv_style_init(&style_voice_lbar_indicator);
    lv_style_set_bg_opa(&style_voice_lbar_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_voice_lbar_indicator, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_bg_img_src(&style_voice_lbar_indicator,&Main_volumebar_long);//使用图片做源
    lv_style_set_radius(&style_voice_lbar_indicator, 0);

    // lv_style_init(&style_voice_bar_main);
    // lv_style_set_bg_opa(&style_voice_bar_main, LV_OPA_COVER);
    // lv_style_set_bg_color(&style_voice_bar_main, lv_palette_main(LV_PALETTE_GREY));
    // lv_style_set_bg_color(&style_voice_bar_main, lv_palette_lighten(LV_PALETTE_GREY,1));
    // lv_style_set_radius(&style_voice_bar_main, 0);

    //lv_device_bar_creat_fun函数使用
    lv_style_init(&style_power_bar_indicator);
    lv_style_set_bg_opa(&style_power_bar_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_power_bar_indicator, lv_color_white());
    lv_style_set_radius(&style_power_bar_indicator, 0);

    lv_style_init(&style_power_bar_main);
    lv_style_set_bg_opa(&style_power_bar_main, LV_OPA_TRANSP);
    lv_style_set_radius(&style_power_bar_main, 0);

    //lv_voice_lab_creat_fun函数使用
    lv_style_init(&style_voice_label);
    lv_style_set_bg_opa(&style_voice_label, LV_OPA_COVER);
    lv_style_set_bg_color(&style_voice_label, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_text_opa(&style_voice_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_voice_label, lv_color_white());

    //lv_power_lab_creat_fun函数使用
    lv_style_init(&style_power_label);
    lv_style_set_bg_opa(&style_power_label, LV_OPA_COVER);
    lv_style_set_bg_color(&style_power_label, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_text_opa(&style_power_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_power_label, lv_color_white());

    //lv_device_lab_creat_fun
    lv_style_init(&style_device_label);
    lv_style_set_bg_opa(&style_device_label, LV_OPA_COVER);
    lv_style_set_bg_color(&style_device_label, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_text_opa(&style_device_label, LV_OPA_COVER);
    lv_style_set_text_color(&style_device_label, lv_color_white());

    //lv_video_dot_creat_fun
    lv_style_init(&style_video_dot);
    lv_style_set_bg_opa(&style_video_dot, LV_OPA_COVER);
    lv_style_set_bg_color(&style_video_dot, lv_palette_main(LV_PALETTE_RED));//LV_RADIUS_CIRCLE
    lv_style_set_radius(&style_video_dot, LV_RADIUS_CIRCLE);
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

static lv_obj_t * lv_voice_sbar_creat_fun(lv_obj_t *align_obj,lv_coord_t x_offset,lv_coord_t y_offset,lv_coord_t width, lv_coord_t high,int32_t init_value)
{
    lv_obj_t * bar = lv_bar_create(align_obj);
    lv_obj_add_style(bar, &style_voice_bar_main, LV_PART_MAIN);
    lv_obj_add_style(bar, &style_voice_sbar_indicator, LV_PART_INDICATOR);
    lv_bar_set_range(bar,-40,60);
    lv_obj_set_size(bar, width, high);
    lv_obj_align_to(bar,align_obj,LV_ALIGN_BOTTOM_LEFT,x_offset,y_offset);
    lv_bar_set_value(bar, init_value, LV_ANIM_ON);
    // lv_bar_set_range(bar,-40,60);
    return bar;
}

static lv_obj_t * lv_voice_lbar_creat_fun(lv_obj_t *align_obj,lv_coord_t x_offset,lv_coord_t y_offset,lv_coord_t width, lv_coord_t high,int32_t init_value)
{
    lv_obj_t * bar = lv_bar_create(align_obj);
    lv_obj_add_style(bar, &style_voice_bar_main, LV_PART_MAIN);
    lv_obj_add_style(bar, &style_voice_lbar_indicator, LV_PART_INDICATOR);
    lv_bar_set_range(bar,-40,60);
    lv_obj_set_size(bar, width, high);
    lv_obj_align_to(bar,align_obj,LV_ALIGN_BOTTOM_LEFT,x_offset,y_offset);
    lv_bar_set_value(bar, init_value, LV_ANIM_ON);
    // lv_bar_set_range(bar,-40,60);
    return bar;
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

static lv_obj_t * lv_voice_img_creat_fun(lv_obj_t *align_obj,lv_coord_t x_offset,lv_coord_t y_offset,uint16_t zoom)
{
    lv_obj_t * img = lv_img_create(align_obj);
    lv_img_set_src(img,&Main_horn);
    lv_img_set_zoom(img, zoom);
    lv_obj_align_to(img,align_obj,LV_ALIGN_BOTTOM_LEFT,x_offset,y_offset);
    return img;
}

static lv_obj_t * lv_power_img_creat_fun(lv_obj_t *align_obj,lv_coord_t x_offset,lv_coord_t y_offset,uint16_t zoom)
{
    lv_obj_t * img = lv_img_create(align_obj);
    lv_img_set_src(img,&Main_bat);
    lv_img_set_zoom(img, zoom);
    lv_obj_align_to(img,align_obj,LV_ALIGN_TOP_RIGHT,x_offset,y_offset);
    return img;
}

static lv_obj_t * lv_signal_img_creat_fun(lv_obj_t *align_obj,const void * picture,lv_coord_t x_offset,lv_coord_t y_offset,uint16_t zoom,uint8_t hide)
{
    lv_obj_t * img = lv_img_create(align_obj);
    lv_img_set_src(img,picture);
    lv_img_set_zoom(img, zoom);
    lv_obj_align_to(img,align_obj,LV_ALIGN_TOP_LEFT,x_offset,y_offset);
    if(!hide){
        lv_obj_add_flag(img,LV_OBJ_FLAG_HIDDEN);
    }
    return img;
}

static lv_obj_t * lv_voice_lab_creat_fun(lv_obj_t *src_obj,lv_obj_t *align_obj,lv_obj_t *bar_obj,lv_coord_t x_offset,lv_coord_t y_offset)
{
    char buf[8] = {0,0,0,0,0,0,0,0};
    lv_obj_t * lab = lv_label_create(src_obj);
    lv_obj_add_style(lab, &style_voice_label, LV_PART_MAIN);
    lv_snprintf(buf, sizeof(buf), "%d", lv_bar_get_value(bar_obj));
    lv_label_set_text(lab,buf);
    lv_obj_align_to(lab,align_obj,LV_ALIGN_OUT_TOP_MID,x_offset,y_offset);
    return lab;
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

static lv_obj_t * lv_device_lab_creat_fun(lv_obj_t *align_obj,lv_coord_t x_offset,lv_coord_t y_offset,const char *ptr)
{
    lv_obj_t * lab = lv_label_create(align_obj);
    lv_obj_add_style(lab, &style_device_label, LV_PART_MAIN);
    lv_label_set_text(lab,ptr);
    lv_obj_align_to(lab,align_obj,LV_ALIGN_TOP_LEFT,x_offset,y_offset);
    return lab;
}

static lv_obj_t * lv_video_dot_creat_fun(lv_obj_t *align_obj,lv_coord_t x_offset,lv_coord_t y_offset,uint8_t hide)
{
    lv_obj_t * dot = lv_bar_create(align_obj);
    lv_obj_add_style(dot, &style_video_dot, LV_PART_MAIN);
    lv_obj_set_size(dot, 10, 10);
    lv_obj_align_to(dot,align_obj,LV_ALIGN_LEFT_MID,x_offset,y_offset);
    if(!hide){
        lv_obj_add_flag(dot,LV_OBJ_FLAG_HIDDEN);
    }
    return dot;
}


/***************************************************************************************************/
/*                                       操作回调函数                                               */
/***************************************************************************************************/

static void lv_set_vodeo_dot_status_cb(lv_obj_t * dot_obj,uint8_t hide)
{
    if(!hide){
        lv_obj_add_flag(dot_obj,LV_OBJ_FLAG_HIDDEN);
    }else{
        lv_obj_clear_flag(dot_obj,LV_OBJ_FLAG_HIDDEN);
    }
}

static void lv_signal_hide_set(uint8_t signal_group,uint8_t hide_num)
{
    switch(signal_group){
        case 1:
        lv_signal_group1_hide_set(hide_num);
        break;
        case 2:
        lv_signal_group2_hide_set(hide_num);
        break;
        default:
        break;
    }
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

static void lv_signal_tx1_set(int16_t signal)
{    
    tx1_signal_obj[0] = lv_signal_img_creat_fun(area_tx1,&Main_signal_0,0,-10,256,0);
    tx1_signal_obj[1] = lv_signal_img_creat_fun(area_tx1,&Main_signal_1,0,-10,256,0);
    tx1_signal_obj[2] = lv_signal_img_creat_fun(area_tx1,&Main_signal_2,0,-10,256,0);
    tx1_signal_obj[3] = lv_signal_img_creat_fun(area_tx1,&Main_signal_3,0,-10,256,0);
    tx1_signal_obj[4] = lv_signal_img_creat_fun(area_tx1,&Main_signal_4,0,-10,256,0);

    lv_signal_hide_set(1,signal);
}

static void lv_signal_tx2_set(int16_t signal)
{    
    tx2_signal_obj[0] = lv_signal_img_creat_fun(area_tx2,&Main_signal_0,0,-10,256,0);
    tx2_signal_obj[1] = lv_signal_img_creat_fun(area_tx2,&Main_signal_1,0,-10,256,0);
    tx2_signal_obj[2] = lv_signal_img_creat_fun(area_tx2,&Main_signal_2,0,-10,256,0);
    tx2_signal_obj[3] = lv_signal_img_creat_fun(area_tx2,&Main_signal_3,0,-10,256,0);
    tx2_signal_obj[4] = lv_signal_img_creat_fun(area_tx2,&Main_signal_4,0,-10,256,0);

    lv_signal_hide_set(2,signal);
}

static void lv_display_tx1(device_data_t * init_data)
{
    area_tx1 = lv_area_creat_fun(LV_ALIGN_BOTTOM_LEFT,6,-6,282,94);
    voice_img_tx1 = lv_voice_img_creat_fun(area_tx1,-3,-5,256);
    power_img_tx1 = lv_power_img_creat_fun(area_tx1,7,-6,256);

    voice_bar_tx1 = lv_voice_lbar_creat_fun(area_tx1,13,-5,251,14,init_data->volume);
    power_bar_tx1 = lv_power_bar_creat_fun(power_img_tx1,2,0,22,10,init_data->electric);
    
    voice_lab_tx1 = lv_voice_lab_creat_fun(area_tx1,voice_img_tx1,voice_bar_tx1,0,0);
    power_lab_tx1 = lv_power_lab_creat_fun(area_tx1,power_img_tx1,power_bar_tx1,0,0);
    device_lab_tx1 = lv_device_lab_creat_fun(area_tx1,-10,-9,"1");

    lv_signal_tx1_set(init_data->signal);

    video_dot_tx1 = lv_video_dot_creat_fun(area_tx1,16,6,init_data->record);
}

static void lv_display_tx2(device_data_t * init_data)
{
    area_tx2 = lv_area_creat_fun(LV_ALIGN_BOTTOM_LEFT,6,-6,282,94);
    voice_img_tx2 = lv_voice_img_creat_fun(area_tx2,-3,-5,256);
    power_img_tx2 = lv_power_img_creat_fun(area_tx2,7,-6,256);

    voice_bar_tx2 = lv_voice_lbar_creat_fun(area_tx2,13,-5,251,14,init_data->volume);
    power_bar_tx2 = lv_power_bar_creat_fun(power_img_tx2,2,0,22,10,init_data->electric);
    
    voice_lab_tx2 = lv_voice_lab_creat_fun(area_tx2,voice_img_tx2,voice_bar_tx2,0,0);
    power_lab_tx2 = lv_power_lab_creat_fun(area_tx2,power_img_tx2,power_bar_tx2,0,0);
    device_lab_tx2 = lv_device_lab_creat_fun(area_tx2,-10,-9,"2");

    lv_signal_tx2_set(init_data->signal);

    video_dot_tx2 = lv_video_dot_creat_fun(area_tx2,16,6,init_data->record);
}

static void lv_display_double(device_data_t * init_tx1,device_data_t * init_tx2)
{
    //设备框
    area_tx1 = lv_area_creat_fun(LV_ALIGN_BOTTOM_LEFT,6,-6,138,94);
    area_tx2 = lv_area_creat_fun(LV_ALIGN_BOTTOM_LEFT,150,-6,138,94);
    //音量喇叭图片
    voice_img_tx1 = lv_voice_img_creat_fun(area_tx1,-3,-5,256);
    voice_img_tx2 = lv_voice_img_creat_fun(area_tx2,-3,-5,256);
    //电池图标图片
    power_img_tx1 = lv_power_img_creat_fun(area_tx1,7,-6,256);
    power_img_tx2 = lv_power_img_creat_fun(area_tx2,7,-6,256);
    //音量大小进度条
    voice_bar_tx1 = lv_voice_sbar_creat_fun(area_tx1,13,-5,107,14,init_tx1->volume);
    voice_bar_tx2 = lv_voice_sbar_creat_fun(area_tx2,13,-5,107,14,init_tx2->volume);
    //电池电量进度条
    power_bar_tx1 = lv_power_bar_creat_fun(power_img_tx1,2,0,22,10,init_tx1->electric);
    power_bar_tx2 = lv_power_bar_creat_fun(power_img_tx2,2,0,22,10,init_tx2->electric);
    //音量大小文本
    voice_lab_tx1 = lv_voice_lab_creat_fun(area_tx1,voice_img_tx1,voice_bar_tx1,0,0);
    voice_lab_tx2 = lv_voice_lab_creat_fun(area_tx2,voice_img_tx2,voice_bar_tx2,0,0);   
    //电池电量大小文本
    power_lab_tx1 = lv_power_lab_creat_fun(area_tx1,power_img_tx1,power_bar_tx1,0,0);
    power_lab_tx2 = lv_power_lab_creat_fun(area_tx2,power_img_tx2,power_bar_tx2,0,0); 
    //设备文本
    device_lab_tx1 = lv_device_lab_creat_fun(area_tx1,-10,-9,"1");
    device_lab_tx2 = lv_device_lab_creat_fun(area_tx2,-10,-9,"2");
    //信号图标
    lv_signal_tx1_set(init_tx1->signal);
    lv_signal_tx2_set(init_tx2->signal);

    //录制状态红点
    video_dot_tx1 = lv_video_dot_creat_fun(area_tx1,16,6,init_tx1->record);
    video_dot_tx2 = lv_video_dot_creat_fun(area_tx2,16,6,init_tx2->record);
}


static void lv_display_mod_change(hl_lvgl_main_init_t * ctl_data)
{
    if(main_init.display_tx_device == ctl_data->display_tx_device){
        return;
    }
    switch(main_init.display_tx_device){
        case HL_CHANGE_TX1_DEVICE:
            hl_obj_delete(area_tx1,true);
            break;
        case HL_CHANGE_TX2_DEVICE:
            hl_obj_delete(area_tx2,true);
            break;
        case HL_DISPLAY_DOUBLE:
            hl_obj_delete(area_tx1,true);
            hl_obj_delete(area_tx2,true);
            break;
        default:
            break;
    }    
    switch(ctl_data->display_tx_device){
        case HL_CHANGE_TX1_DEVICE:
            lv_display_tx1(&ctl_data->tx_device_1);
            break;
        case HL_CHANGE_TX2_DEVICE:
            lv_display_tx2(&ctl_data->tx_device_2);
            break;
        case HL_DISPLAY_DOUBLE:
            lv_display_double(&ctl_data->tx_device_1,&ctl_data->tx_device_2);
            break;
        default:
            break;
    }  
    main_init.display_tx_device = ctl_data->display_tx_device;
}


void hl_mod_main_ioctl(void * ctl_data)
{
    char buf[8] = {0,0,0,0,0,0,0,0};
    hl_lvgl_main_ioctl_t * ptr =  (hl_lvgl_main_ioctl_t *)ctl_data;
    switch(ptr->cmd){
        case HL_CHANGE_TX1_DEVICE:
        case HL_CHANGE_TX2_DEVICE:
        case HL_CHANGE_DOUBLE_DEVICE:
            lv_display_mod_change(ctl_data);
            break;


        case HL_CHANGE_TX1_SIGNAL:
            lv_signal_tx1_set(ptr->tx_device_1.signal);
            main_init.tx_device_1.signal = ptr->tx_device_1.signal;
            break;
        case HL_CHANGE_TX1_ELEC:
            lv_bar_set_value(power_bar_tx1, ptr->tx_device_1.electric, LV_ANIM_ON);
            lv_snprintf(buf, sizeof(buf), "%d%%", ptr->tx_device_1.electric);
            lv_label_set_text(power_lab_tx1,buf);
            main_init.tx_device_1.electric = ptr->tx_device_1.electric;
            break;
        case HL_CHANGE_TX1_VOL:           
            lv_bar_set_value(voice_bar_tx1, ptr->tx_device_1.volume, LV_ANIM_ON);
            lv_snprintf(buf, sizeof(buf), "%d", ptr->tx_device_1.volume);
            lv_label_set_text(voice_lab_tx1,buf);
            main_init.tx_device_1.volume = ptr->tx_device_1.volume; 
            break;
        case HL_CHANGE_TX1_REC:
            lv_set_vodeo_dot_status_cb(video_dot_tx1,ptr->tx_device_1.record);
            main_init.tx_device_1.record = ptr->tx_device_1.record; 
            break;


        case HL_CHANGE_TX2_SIGNAL:
            lv_signal_tx2_set(ptr->tx_device_2.signal);
            main_init.tx_device_2.signal = ptr->tx_device_2.signal;
            break;
        case HL_CHANGE_TX2_ELEC:
            lv_bar_set_value(power_bar_tx2, ptr->tx_device_2.electric, LV_ANIM_ON);
            lv_snprintf(buf, sizeof(buf), "%d%%", ptr->tx_device_2.electric);
            lv_label_set_text(power_lab_tx2,buf);
            main_init.tx_device_2.electric = ptr->tx_device_2.electric;
            break;
        case HL_CHANGE_TX2_VOL:
            lv_bar_set_value(voice_bar_tx2, ptr->tx_device_2.volume, LV_ANIM_ON);
            lv_snprintf(buf, sizeof(buf), "%d", ptr->tx_device_2.volume);
            lv_label_set_text(voice_lab_tx2,buf);
            main_init.tx_device_2.volume = ptr->tx_device_2.volume; 
            break;
        case HL_CHANGE_TX2_REC:
            lv_set_vodeo_dot_status_cb(video_dot_tx2,ptr->tx_device_2.record);
            main_init.tx_device_2.record = ptr->tx_device_2.record; 
            break;

        
        case HL_CHANGE_DELETE_PAGE:
            hl_obj_delete(area_tx1,true);
            hl_obj_delete(area_tx2,true);
            break;
        default:
            break;
    }
}

void hl_mod_main_init(void * init_data)
{
    hl_lvgl_main_init_t * ptr = (hl_lvgl_main_init_t *)init_data;
    memcpy(&main_init,ptr,sizeof(hl_lvgl_main_init_t));
    lv_style_page1_init();
    if(ptr->display_tx_device == HL_DISPLAY_TX1){
        lv_display_tx1(&ptr->tx_device_1);
    }
    if(ptr->display_tx_device == HL_DISPLAY_TX2){
        lv_display_tx2(&ptr->tx_device_1);
    }
    if(ptr->display_tx_device == HL_DISPLAY_DOUBLE){
        lv_display_double(&ptr->tx_device_1,&ptr->tx_device_2);
    }    
}

/*
static void delete(lv_obj_t *obj,bool obj_typ)
{
    uint32_t child_cnt = 0,i;
    child_cnt = lv_obj_get_child_cnt(obj);
    if(child_cnt == 0){
        printf("child_cnt1 = %d\n",0);
        lv_obj_del_delayed(obj,0);
    }else{
        for(i=0;i<child_cnt;i++){
            delete(lv_obj_get_child(obj, i),true);            
        }
        if(obj_typ){
            printf("child_cnt1 = %d\n",0);
            lv_obj_del_delayed(obj,0);
        }        
    }
}

//使用方法
delete(lv_scr_act(),false);
*/

//测试接口
// void page_main_test(void)
// {    
//     hl_lvgl_main_init_t data;
//     data.display_tx_device = HL_DISPLAY_TX1;
//     data.tx_device_1.electric = 20;
//     data.tx_device_1.signal = HL_ONE_SIGNAL;
//     data.tx_device_1.record = HL_RECODING;
//     data.tx_device_1.volume = -13;
//     hl_mod_lvgl_main_init(&data);
// }