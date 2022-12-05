
/**
 * @file lv_port_indev_templ.h
 *
 */

/*Copy this file as "lv_port_indev.h" and set this value to "1" to enable content*/
#if 1

#ifndef LV_PORT_INDEV_TEMPL_H
#define LV_PORT_INDEV_TEMPL_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"

/*********************
 *      DEFINES
 *********************/

// typedef void (*keypad_knob_ok_callback)(lv_indev_data_t * data) keypad_knob_ok_callback;

typedef void (*keypad_touchkey_callback)(lv_indev_data_t *);
typedef void (*keypad_knob_ok_callback)(struct _lv_indev_drv_t *, lv_indev_data_t *);
typedef void (*encoder_knob_callback)(void);

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_port_indev_init(void);

void keypad_knob_ok_update(keypad_knob_ok_callback c);
void encode_knob_update(keypad_knob_ok_callback c);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_PORT_INDEV_TEMPL_H*/

#endif /*Disable/Enable content*/
