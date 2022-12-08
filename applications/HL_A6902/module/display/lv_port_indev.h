
/**
 * @brief 
 * @date 2022-12-08
 * @author liujie (jie.liu@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-12-08      <td>liujie     <td>新建
 * </table>
 */
#ifndef LV_PORT_INDEV_HL_H
#define LV_PORT_INDEV_HL_H

#include "hl_config.h"
#if !HL_IS_TX_DEVICE()
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
void hl_port_indev_lock_screen(bool en);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /**/

#endif /*LV_PORT_INDEV_HL_H*/


