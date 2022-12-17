/*********************
 *      INCLUDES
 *********************/

#include "hl_config.h"

#if !HL_IS_TX_DEVICE()

#include "hl_mod_page_common.h"
#include "rtconfig.h"
#include "lv_port_indev.h"
#include "../../lvgl.h"
#ifdef RT_USING_FT3169
#include "hl_drv_ft3169.h"
#endif
#ifdef RT_USING_ZTW523A
#include "hl_drv_ztw523a.h"
#endif

static lv_group_t* key_group;

static lv_indev_t* indev_touchpad_p;
static lv_indev_t* indev_keypad_touchkey_p;
static lv_indev_t* indev_keypad_knob_ok_p;
static lv_indev_t* indev_encoder_knob_p;

static lv_indev_t* indev_keypad_lock_screen_p;

static lv_indev_drv_t indev_touchpad;
static lv_indev_drv_t indev_keypad_touchkey;
static lv_indev_drv_t indev_keypad_knob_ok;
static lv_indev_drv_t indev_encoder_knob;

// static hl_indev_s hl_indev_callback;


static bool touchpad_switch        = false;

// 触摸屏

static void touchpad_init(void);
static void touchpad_read(lv_indev_drv_t* indev_drv, lv_indev_data_t* data);
static bool touchpad_is_pressed(void);
static void touchpad_get_xy(lv_coord_t* x, lv_coord_t* y);

// 返回触摸按键
static void keypad_touchkey_init(void);
static void keypad_touchkey_read(lv_indev_drv_t* indev_drv, lv_indev_data_t* data);
// static uint32_t keypad_touchkey_key(void);

//旋钮OK按键
static void keypad_knob_ok_init(void);
static void keypad_knob_ok_read(lv_indev_drv_t* indev_drv, lv_indev_data_t* data);
// static uint32_t keypad_keypad_knob_ok_key(void);

//旋钮
static void encoder_knob_init(void);
static void encoder_knob_read(lv_indev_drv_t* indev_drv, lv_indev_data_t* data);
static void encoder_knob_handler(void);

// static void hl_lv_indev_read_reg(hl_indev_s hl_indev_callback);

static void             lv_add_all_indev_to_def_group(lv_group_t* group);
static int32_t          encoder_diff;
static lv_indev_state_t encoder_state;

#ifdef RT_USING_FT3169
static struct fts_ts_event touch_pos = {
    .type = 1,
    .x    = 0,
    .y    = 0,
};
#endif
#ifdef RT_USING_ZTW523A
static struct ztw523a_ts_event touch_pos = {
    .type = 1,
    .x    = 0,
    .y    = 0,
};
#endif

void lv_port_indev_init(void)
{

#if 1
    /*Initialize your touchpad if you have*/
    touchpad_init();
    /*Register a touchpad input device*/
    lv_indev_drv_init(&indev_touchpad);
    indev_touchpad.type    = LV_INDEV_TYPE_POINTER;
    indev_touchpad.read_cb = touchpad_read;
    indev_touchpad_p       = lv_indev_drv_register(&indev_touchpad);
#endif

    // keypad_knob_ok_callback = keypad_knob_ok_read;
    // static lv_indev_t * indev_keypad_knob_ok;
    keypad_knob_ok_init();
    lv_indev_drv_init(&indev_keypad_knob_ok);
    indev_keypad_knob_ok.type    = LV_INDEV_TYPE_KEYPAD;
    indev_keypad_knob_ok.read_cb = keypad_knob_ok_read;
    indev_keypad_knob_ok_p       = lv_indev_drv_register(&indev_keypad_knob_ok);

    /*Later you should create group(s) with `lv_group_t * group = lv_group_create()`,
     *add objects to the group with `lv_group_add_obj(group, obj)`
     *and assign this input device to group to navigate in it:
     *`lv_indev_set_group(indev_keypad, group);`*/

    // keypad_touchkey_init();
    // lv_indev_drv_init(&indev_keypad_touchkey);
    // indev_keypad_touchkey.type    = LV_INDEV_TYPE_KEYPAD;
    // indev_keypad_touchkey.read_cb = keypad_touchkey_read;
    // indev_keypad_touchkey_p       = lv_indev_drv_register(&indev_keypad_touchkey);

    encoder_knob_init();
    lv_indev_drv_init(&indev_encoder_knob);
    indev_encoder_knob.type    = LV_INDEV_TYPE_ENCODER;
    indev_encoder_knob.read_cb = encoder_knob_read;
    indev_encoder_knob_p       = lv_indev_drv_register(&indev_encoder_knob);

    lv_add_all_indev_to_def_group(NULL);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lv_add_all_indev_to_def_group(lv_group_t* group)
{
    if (!group) {
        LV_LOG_USER("group\n");
        group = lv_group_get_default();
        if (!group) {
            group = lv_group_create();
            LV_LOG_USER("group\n");
            if (group) {
                lv_group_set_default(group);
            }
        }
    }

    // lv_indev_set_group(indev_touchpad_p, group);
    //group只能添加按键和编码器
    // lv_indev_set_group(indev_keypad_touchkey_p, group);
    // lv_indev_set_group(indev_keypad_knob_ok_p, group);
    // lv_indev_set_group(indev_encoder_knob_p, group);
}

void hl_port_indev_lock_screen(bool en)
{
    
    if (en) {
        // 禁用输入
        lv_indev_enable(indev_touchpad_p, false);
        lv_indev_enable(indev_keypad_touchkey_p, false);
        lv_indev_enable(indev_keypad_knob_ok_p, false);
        //有BUG
        lv_indev_enable(indev_encoder_knob_p, false);


    } else {
        // 使能输入
        lv_indev_enable(indev_touchpad_p, true);
        lv_indev_enable(indev_keypad_touchkey_p, true);
        lv_indev_enable(indev_keypad_knob_ok_p, true);
        //有BUG
        lv_indev_enable(indev_encoder_knob_p, true);

    }
}

lv_indev_t * hl_mod_get_knob_indev()
{
    return indev_encoder_knob_p;
}



lv_indev_t * hl_mod_get_okkey_indev()
{
    return indev_keypad_knob_ok_p;
}

/*------------------
 * Touchpad
 * -----------------*/

/*Initialize your touchpad*/
static void touchpad_init(void)
{
    touchpad_switch = true;
}

/*Will be called by the library to read the touchpad*/
static void touchpad_read(lv_indev_drv_t* indev_drv, lv_indev_data_t* data)
{

    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;
    uint32_t tmp;

    if (touchpad_switch == true) {

        /*Save the pressed coordinates and the state*/
        if (touchpad_is_pressed()) {
            
            touchpad_get_xy(&last_x, &last_y);
            // tmp = last_x;
            // last_x = 294 - last_y;
            // last_y = tmp;

            // rt_kprintf("last_x = %d  last_y = %d \n",last_x,last_y);
            data->state = LV_INDEV_STATE_PR;
        } else {
            data->state = LV_INDEV_STATE_REL;
        }

        /*Set the last pressed coordinates*/
        data->point.x = last_x;
        data->point.y = last_y;
    }
}

/*Return true is the touchpad is pressed*/
static bool touchpad_is_pressed(void)
{
#ifdef RT_USING_FT3169
    /*Your code comes here*/
    hl_drv_touch_dev_read_info(&touch_pos);
    if (touch_pos.type != 1) {
        return true;
    }
    return false;
#endif
#ifdef RT_USING_ZTW523A
    /*Your code comes here*/
    hl_drv_ztw523a_dev_read_info(&touch_pos);
    if (touch_pos.type != 0) {
        return true;
    }
    return false;
#endif
}

/*Get the x and y coordinates if the touchpad is pressed*/
static void touchpad_get_xy(lv_coord_t* x, lv_coord_t* y)
{
    /*Your code comes here*/
    (*x) = touch_pos.x;
    (*y) = touch_pos.y;
}

/*------------------
 * Keypad
 * -----------------*/
static void keypad_knob_ok_init(void)
{
}

static void keypad_knob_ok_read(lv_indev_drv_t* indev_drv, lv_indev_data_t* data)
{
    hl_mod_rx_knob_val_pro(indev_drv, data);
}
void keypad_knob_ok_update(keypad_knob_ok_callback c)
{
    static lv_indev_drv_t new_drv;
    lv_indev_t*           indev_p;

    lv_indev_drv_init(&new_drv);
    new_drv.type    = LV_INDEV_TYPE_KEYPAD;
    new_drv.read_cb = c;
    indev_p         = lv_indev_drv_register(&new_drv);

    lv_indev_drv_update(indev_keypad_knob_ok_p, &new_drv);
    indev_keypad_knob_ok_p = indev_p;
}

static void keypad_touchkey_init(void)
{
    //设备，不用初始化
}

static void keypad_touchkey_read(lv_indev_drv_t* indev_drv, lv_indev_data_t* data)
{
    static uint8_t last_key = 0;

    last_key = hl_drv_ztw523a_botton_status();
    if (last_key != BUTTON_UP) {
        data->state = LV_INDEV_STATE_PR;
        last_key    = LV_KEY_BACKSPACE;
        LV_LOG_USER("touchkey_en");
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
    data->key = last_key;
}
/*------------------
 * Encoder
 * -----------------*/

static void encoder_knob_init(void)
{
    // 初始化了
}

static void encoder_knob_read(lv_indev_drv_t* indev_drv, lv_indev_data_t* data)
{

    data->enc_diff = 0;
    // data->state = LV_INDEV_STATE_REL;
}

void encode_knob_update(keypad_knob_ok_callback c)
{
    static lv_indev_drv_t new_drv;
    lv_indev_t*           indev_p;

    lv_indev_drv_init(&new_drv);
    new_drv.type    = LV_INDEV_TYPE_ENCODER;
    new_drv.read_cb = c;
    indev_p         = lv_indev_drv_register(&new_drv);

    lv_indev_drv_update(indev_encoder_knob_p, &new_drv);
    indev_encoder_knob_p = indev_p;
}

#endif /*!HL_IS_TX_DEVICE()*/
