/**
 * @file hl_drv_knob.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 获取当前编码器的增量值
 * @version 1.0
 * @date 2022-09-13
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-13     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "hl_hal_gpio.h"
#include "hl_drv_knob.h"
#include <rtthread.h>
#include <rtdevice.h>
/* typedef -------------------------------------------------------------------*/


typedef struct _hl_drv_knob
{
    /// 旋钮的A相管脚索引
    hl_drv_knob_e knob_a_pin;
    /// 旋钮的B相管脚索引
    hl_drv_knob_e knob_b_pin;
    /// 旋钮的变化值 正值 顺时针旋转    负值：逆时针旋转
    int8_t knob_gain_val;
}hl_drv_knob;

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
static hl_drv_knob knobs[HL_KNOBS] = { 
                                        {GPIO_L_VOL_A, GPIO_L_VOL_B, 0},
                                        {GPIO_R_VOL_A, GPIO_R_VOL_B, 0},
                                     };

/* Private function(only *.c)  -----------------------------------------------*/
static void hl_mod_input_knob_irq_cb(void *args);


static void hl_mod_input_knob_irq_cb(void *args)
{
    int8_t knob_value = 0;                                        
    uint8_t knob_num = *(uint8_t *)args;

    if (hl_hal_gpio_read(knobs[knob_num].knob_a_pin) != 0) {
        if (hl_hal_gpio_read(knobs[knob_num].knob_b_pin) == 0) {
            knob_value += 1;
        } else {
            knob_value -= 1;
        }
    } else {
        if (hl_hal_gpio_read(knobs[knob_num].knob_b_pin) != 0) {
            knob_value += 1;
        } else {
            knob_value -= 1;
        }
    }

    knobs[knob_num].knob_gain_val = knob_value;
}
/* Exported functions --------------------------------------------------------*/

uint8_t hl_drv_knob_init(void)
{
    uint8_t i;
    rt_err_t res;

    for (i = 0;i < HL_KNOBS; i++) {
        hl_hal_gpio_init(knobs[i].knob_a_pin);
        hl_hal_gpio_init(knobs[i].knob_b_pin);

        res = hl_hal_gpio_attach_irq(knobs[i].knob_a_pin, PIN_IRQ_MODE_RISING_FALLING, hl_mod_input_knob_irq_cb, (void*)&i);
        if (res != RT_EOK) {
            return HL_FAILED;
        }
        res = hl_hal_gpio_attach_irq(knobs[i].knob_b_pin, PIN_IRQ_MODE_RISING_FALLING, hl_mod_input_knob_irq_cb, (void*)&i);
        if (res != RT_EOK) {
            return HL_FAILED;
        }
        hl_hal_gpio_irq_enable(knobs[i].knob_a_pin, PIN_IRQ_ENABLE);
        hl_hal_gpio_irq_enable(knobs[i].knob_b_pin, PIN_IRQ_ENABLE);
        
    }
    knobs[KNOB_LEFT].knob_a_pin = GPIO_L_VOL_A;
    knobs[KNOB_LEFT].knob_a_pin = GPIO_L_VOL_B;
    knobs[KNOB_LEFT].knob_gain_val = 0;
    
    knobs[KNOB_RIGHT].knob_a_pin = GPIO_R_VOL_A;
    knobs[KNOB_RIGHT].knob_a_pin = GPIO_R_VOL_B;
    knobs[KNOB_RIGHT].knob_gain_val = 0;
    
    return HL_SUCCESS;
}

uint8_t hl_drv_knob_deinit(void)
{
    uint8_t i;

    for (i = 0;i < HL_KNOBS; i++) {
        hl_hal_gpio_deinit(knobs[i].knob_a_pin);
        hl_hal_gpio_deinit(knobs[i].knob_b_pin);

        if(RT_EOK != hl_hal_gpio_irq_enable(knobs[i].knob_a_pin, PIN_IRQ_DISABLE)) {
            return HL_FAILED;
        }
        if (RT_EOK != hl_hal_gpio_irq_enable(knobs[i].knob_b_pin, PIN_IRQ_DISABLE)) {
            return HL_FAILED;
        }

    }
    return HL_SUCCESS;
}

uint8_t hl_drv_knob_read(uint8_t knob_num, int8_t * val)
{
    if (knob_num >= HL_KNOBS && val == NULL) {
        return HL_FAILED;
    }

    hl_hal_gpio_irq_enable(knobs[knob_num].knob_a_pin, PIN_IRQ_DISABLE);
    hl_hal_gpio_irq_enable(knobs[knob_num].knob_b_pin, PIN_IRQ_DISABLE);

    *val = knobs[knob_num].knob_gain_val;
    knobs[knob_num].knob_gain_val = 0;
    hl_hal_gpio_irq_enable(knobs[knob_num].knob_a_pin, PIN_IRQ_ENABLE);
    hl_hal_gpio_irq_enable(knobs[knob_num].knob_b_pin, PIN_IRQ_ENABLE);

    return HL_SUCCESS;
}




/*
 * EOF
 */