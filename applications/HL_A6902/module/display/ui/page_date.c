#include "page_date.h"
#include "language.h"

static uint8_t date_year[256];
static uint8_t date_month[256];
static uint8_t date_day[256];
static uint8_t date_hour[256];
static uint8_t date_min[256];

/**
 * Add a fade mask to roller.
 */

static void lv_date_init(void)
{
    char buf[6] = {0,0,0,0,0,0};
    uint16_t i;
    for(i=2000;i<2051;i++){
        if(i == 2050){
            lv_snprintf(buf, sizeof(buf), "%d", i);
        }else{
            lv_snprintf(buf, sizeof(buf), "%d\n", i);
        }  
        lv_snprintf(buf, sizeof(buf), "%d\n", i);
        strcat(date_year,buf);
    }
    for(i=1;i<13;i++){
        lv_snprintf(buf, sizeof(buf), "%d\n", i);
        if(i == 12){
            lv_snprintf(buf, sizeof(buf), "%d", i);
        }else{
            lv_snprintf(buf, sizeof(buf), "%d\n", i);
        }  
        strcat(date_month,buf);
    }
    for(i=1;i<31;i++){
        if(i == 30){
            lv_snprintf(buf, sizeof(buf), "%d", i);
        }else{
            lv_snprintf(buf, sizeof(buf), "%d\n", i);
        }  
        lv_snprintf(buf, sizeof(buf), "%d\n", i);
        strcat(date_day,buf);
    }
    for(i=0;i<25;i++){
        if(i == 24){
            lv_snprintf(buf, sizeof(buf), "%d", i);
        }else{
            lv_snprintf(buf, sizeof(buf), "%d\n", i);
        }  
        lv_snprintf(buf, sizeof(buf), "%d\n", i);
        strcat(date_hour,buf);
    }
    for(i=0;i<61;i++){
        if(i == 60){
            lv_snprintf(buf, sizeof(buf), "%d", i);
        }else{
            lv_snprintf(buf, sizeof(buf), "%d\n", i);
        }        
        strcat(date_min,buf);
    }
}

static lv_style_t style;
static lv_obj_t *roller_year,*roller_month,*roller_day,*roller_hour,*roller_min;

static void lv_style_page6_init(void)
{    
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_color_black());
    lv_style_set_border_width(&style, 0);
    lv_style_set_pad_all(&style, 0);
    // lv_obj_add_style(lv_scr_act(), &style, 0);
}

static lv_obj_t * lv_creat_roller(lv_obj_t *src_obj,lv_obj_t *align_obj,lv_align_t align,const char * options,lv_coord_t x_offset,lv_coord_t y_offset)
{ 
    lv_obj_t *roller = lv_roller_create(src_obj);
    lv_obj_add_style(roller, &style, 0);
    lv_obj_set_style_bg_opa(roller, LV_OPA_TRANSP, LV_PART_SELECTED);

#if LV_FONT_MONTSERRAT_22
    lv_obj_set_style_text_font(roller, &lv_font_montserrat_22, LV_PART_SELECTED);
#endif

    lv_roller_set_options(roller,options,LV_ROLLER_MODE_NORMAL);
    lv_obj_align_to(roller,align_obj,align,x_offset,y_offset);
    lv_roller_set_visible_row_count(roller, 3);
    return roller;
}

static void page_6_test(void)
{
    lv_date_init();
    
    roller_year = lv_creat_roller(lv_scr_act(),lv_scr_act(),LV_ALIGN_LEFT_MID,date_year,10,0);
    roller_month = lv_creat_roller(lv_scr_act(),roller_year,LV_ALIGN_OUT_RIGHT_MID,date_month,20,0);
    roller_day = lv_creat_roller(lv_scr_act(),roller_month,LV_ALIGN_OUT_RIGHT_MID,date_day,20,0);
    roller_hour = lv_creat_roller(lv_scr_act(),roller_day,LV_ALIGN_OUT_RIGHT_MID,date_hour,20,0);
    roller_min = lv_creat_roller(lv_scr_act(),roller_hour,LV_ALIGN_OUT_RIGHT_MID,date_min,20,0);
}

//测试接口
void page_date_test(void)
{
    lv_style_page6_init();
    page_6_test();
}