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
#include "hl_drv_knob.h"
#if HL_GET_DEVICE_TYPE()
#else
// RX旋钮配置
/* Includes ------------------------------------------------------------------*/
#include "hl_hal_gpio.h"
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
                                        {GPIO_VOL_A, GPIO_VOL_B, 0},
                                     };
static hl_knob_irq_num[HL_KNOBS]   = {0};
/* Private function(only *.c)  -----------------------------------------------*/
static void hl_mod_input_knob_irq_cb(void *args)
{                                     
    uint8_t knob_num = 0;

    knob_num = *(uint8_t *)args;

    // rt_kprintf("[%s][line:%d]  knob_num = %d!!! \r\n", __FUNCTION__, __LINE__, knob_num);
    if (hl_hal_gpio_read(knobs[knob_num].knob_a_pin) != 0) {
        if (hl_hal_gpio_read(knobs[knob_num].knob_b_pin) == 0) {
            knobs[knob_num].knob_gain_val += 1;
        } else {
            knobs[knob_num].knob_gain_val -= 1;
        }
    } else {
        if (hl_hal_gpio_read(knobs[knob_num].knob_b_pin) != 0) {
            knobs[knob_num].knob_gain_val += 1;
        } else {
            knobs[knob_num].knob_gain_val -= 1;
        }
    }
}
/* Exported functions --------------------------------------------------------*/
uint8_t hl_drv_knob_init(void)
{
    uint8_t i;
    rt_err_t res;

    for (i = 0; i < HL_KNOBS; i++) {
        hl_hal_gpio_init(knobs[i].knob_a_pin);
        hl_hal_gpio_init(knobs[i].knob_b_pin);
        hl_knob_irq_num[i] = i;
        res = hl_hal_gpio_attach_irq(knobs[i].knob_a_pin, PIN_IRQ_MODE_RISING_FALLING, hl_mod_input_knob_irq_cb, (void*)&hl_knob_irq_num[i]);
        if (res != RT_EOK) {
            rt_kprintf("[%s][line:%d]gpio attach err!!! \r\n", __FUNCTION__, __LINE__);
            return HL_FAILED;
        }
        hl_hal_gpio_irq_enable(knobs[i].knob_a_pin, PIN_IRQ_ENABLE);
    }
    
    return HL_SUCCESS;
}

uint8_t hl_drv_knob_deinit(void)
{
    uint8_t i;

    for (i = 0;i < HL_KNOBS; i++) {
        if(RT_EOK != hl_hal_gpio_irq_enable(knobs[i].knob_a_pin, PIN_IRQ_DISABLE)) {
            return HL_FAILED;
        }
        hl_hal_gpio_deinit(knobs[i].knob_a_pin);
        hl_hal_gpio_deinit(knobs[i].knob_b_pin);
    }
    
    return HL_SUCCESS;
}

uint8_t hl_drv_knob_read(uint8_t knob_num, int8_t * val)
{
    if (knob_num >= HL_KNOBS && val == NULL) {
        return HL_FAILED;
    }

    hl_hal_gpio_irq_enable(knobs[knob_num].knob_a_pin, PIN_IRQ_DISABLE);
    *val = knobs[knob_num].knob_gain_val;
    knobs[knob_num].knob_gain_val = 0;
    hl_hal_gpio_irq_enable(knobs[knob_num].knob_a_pin, PIN_IRQ_ENABLE);

    return HL_SUCCESS;
}


#endif

