/**
 * @file hl_drv_aw21009.h
 * @author lilin (lin.li@hollyland-tech.com)
 * @brief LED驱动头文件
 * @version 1.0
 * @date 2022-09-17
 * 
 * ██╗  ██╗ ██████╗ ██╗     ██╗  ██╗   ██╗██╗      █████╗ ███╗   ██╗██████╗ 
 * ██║  ██║██╔═══██╗██║     ██║  ╚██╗ ██╔╝██║     ██╔══██╗████╗  ██║██╔══██╗
 * ███████║██║   ██║██║     ██║   ╚████╔╝ ██║     ███████║██╔██╗ ██║██║  ██║
 * ██╔══██║██║   ██║██║     ██║    ╚██╔╝  ██║     ██╔══██║██║╚██╗██║██║  ██║
 * ██║  ██║╚██████╔╝███████╗███████╗██║   ███████╗██║  ██║██║ ╚████║██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-17     <td>v1.0     <td>lilin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __HL_DRV_AW21009_H__
#define __HL_DRV_AW21009_H__

/* Includes ------------------------------------------------------------------*/

#include "rtthread.h"
#include "rtdevice.h"
#include "hl_config.h"
#include "stdbool.h"

/* typedef -------------------------------------------------------------------*/

typedef enum _hl_drv_aw21009_op
{
    HL_DRV_AW21009_GET_CHIP_ID,
    HL_DRV_AW21009_CHECK_POR_STATU,
    HL_DRV_AW21009_TEMP_LED_OPEN,
    HL_DRV_AW21009_TEMP_LED_CLOSE,
} hl_drv_aw21009_op_e;

typedef enum _hl_drv_aw21009_pattern_time
{
    HL_DRV_AW21009_PAT_TIME_0S00 = 0,
    HL_DRV_AW21009_PAT_TIME_0S13,
} hl_drv_aw21009_pattern_time_e;

typedef enum _hl_drv_aw21009_loop_stop_point
{
    HL_DRV_AW21009_STOP_POINT_T3 = 0,
    HL_DRV_AW21009_STOP_POINT_T0,
} hl_drv_aw21009_loop_stop_point_e;

typedef enum _hl_drv_aw21009_loop_start_point
{
    HL_DRV_AW21009_START_POINT_T0 = 0,
    HL_DRV_AW21009_START_POINT_T1,
    HL_DRV_AW21009_START_POINT_T2,
    HL_DRV_AW21009_START_POINT_T3,
} hl_drv_aw21009_loop_start_point_e;

typedef struct _hl_drv_aw21009_auto_breath_param
{
    uint8_t                           r;
    uint8_t                           g;
    uint8_t                           b;
    uint8_t                           l;
    hl_drv_aw21009_pattern_time_e     rise_time;
    hl_drv_aw21009_pattern_time_e     on_time;
    hl_drv_aw21009_pattern_time_e     fall_time;
    hl_drv_aw21009_pattern_time_e     off_time;
    hl_drv_aw21009_loop_start_point_e start_point;
    hl_drv_aw21009_loop_stop_point_e  stop_point;
    uint16_t                          repeat_times;
} hl_drv_aw21009_auto_breath_param_st;

typedef struct _hl_drv_aw21009_set_auto_breath
{
    hl_drv_aw21009_auto_breath_param_st led1;
    hl_drv_aw21009_auto_breath_param_st led2;
    hl_drv_aw21009_auto_breath_param_st led3;
} hl_drv_aw21009_set_auto_breath_st;

typedef enum _hl_drv_aw21009_led_dev_num
{
    HL_DRV_AW21009_LED_DEV_0,
    HL_DRV_AW21009_LED_DEV_1,
} hl_drv_aw21009_led_dev_num_e;

typedef enum
{
    CLK_FRQ_16M = 1,
    CLK_FRQ_8M,
    CLK_FRQ_4M,
    CLK_FRQ_2M,
    CLK_FRQ_1M,
    CLK_FRQ_512K,
    CLK_FRQ_256K,
    CLK_FRQ_125K,
} clk_pwm_e;

typedef enum
{
    BR_RESOLUTION_8BIT = 1,
    BR_RESOLUTION_9BIT,
    BR_RESOLUTION_12BIT,
    BR_RESOLUTION_9_AND_3_BIT,
} br_pwm_e;

typedef enum
{
    GROUP_ALL_LED_OFF = 0,
    AW21018_GROUP_ALL_LEDS_ON,
    AW21018_GROUP_RED_LEDS_ON,
    AW21018_GROUP_GREEN_LEDS_ON,
    AW21018_GROUP_BLUE_LEDS_ON,
    AW21018_GROUP_BREATH_LEDS_ON,
    AW21012_GROUP_ALL_LEDS_ON,
    AW21012_GROUP_RED_LEDS_ON,
    AW21012_GROUP_GREEN_LEDS_ON,
    AW21012_GROUP_BLUE_LEDS_ON,
    AW21012_GROUP_BREATH_LEDS_ON,
    AW21009_GROUP_ALL_LEDS_ON,
    AW21009_GROUP_RED_LEDS_ON,
    AW21009_GROUP_GREEN_LEDS_ON,
    AW21009_GROUP_BLUE_LEDS_ON,
    AW21009_GROUP_BREATH_LEDS_ON,
} effect_select_e;

/* define --------------------------------------------------------------------*/

#define AW21009_FUNC_RET_OK 0
#define AW21009_FUNC_RET_ERR 1

/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

int hl_drv_aw21009_init(void);

int hl_drv_aw21009_deinit(void);

/**
 * 
 * @brief led 控制函数
 * @param [in] led_num led设备编号，对应rx上两个aw21009芯片, see<hl_drv_aw21009_led_dev_num_e>。 
 * @param [in] op 
 * @param [in] arg 
 * @param [in] arg_size 
 * @return int 
 * @date 2022-09-24
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-24      <td>lilin     <td>新建
 * </table>
 */
int hl_drv_aw21009_ctrl(uint8_t led_num, uint8_t op, void* arg, int32_t arg_size);

#endif
/*
 * EOF
 */