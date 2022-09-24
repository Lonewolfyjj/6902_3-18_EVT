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
    ///获取芯片id，需要<uint8_t>类型的指针作为参数
    HL_DRV_AW21009_GET_CHIP_ID,
    ///检测power-on-reset是否出现，需要<uint8_t>类型的指针作为参数
    HL_DRV_AW21009_CHECK_POR_STATU,
    /**
     * 
     * @brief 设置自动呼吸参数，需要<hl_drv_aw21009_auto_breath_param_st>类型的指针作为参数
     * @date 2022-09-24
     * @author lilin (lin.li@hollyland-tech.com)
     * 
     * @details 需要在自动呼吸模式下用
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2022-09-24      <td>lilin     <td>新建
     * </table>
     */
    HL_DRV_AW21009_SET_AUTO_BREATH_PARAM,
    /**
     * 
     * @brief 设置组控制失能，需要<bool>类型的指针作为参数
     * @date 2022-09-24
     * @author lilin (lin.li@hollyland-tech.com)
     * 
     * @details 失能时，每个在组内的led通道的颜色独立控制，使能时，每个在组内的led通道的颜色共享同一个r/g/b参数
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2022-09-24      <td>lilin     <td>新建
     * </table>
     */
    HL_DRV_AW21009_SET_GROUP_CTRL_DISABLE,
    /**
     * 
     * @brief 设置LED组模式，需要<hl_drv_aw21009_led_group_e>类型的指针作为参数
     * @date 2022-09-24
     * @author lilin (lin.li@hollyland-tech.com)
     * 
     * @details led通道分组：[1-3]、[4-6]、[7-9]
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2022-09-24      <td>lilin     <td>新建
     * </table>
     */
    HL_DRV_AW21009_SET_LED_GROUP_MODE,
    /**
     * 
     * @brief 设置pattern控制模式，需要<hl_drv_aw21009_pattern_mode_e>类型的指针作为参数
     * @date 2022-09-24
     * @author lilin (lin.li@hollyland-tech.com)
     * 
     * @details pattern控制模式下，分为自动呼吸模式和manual模式
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2022-09-24      <td>lilin     <td>新建
     * </table>
     */
    HL_DRV_AW21009_SET_PATTERN_MODE,
    /**
     * 
     * @brief 设置led灯光效果，需要<hl_drv_aw21009_led_light_st>类型的指针作为参数
     * @date 2022-09-24
     * @author lilin (lin.li@hollyland-tech.com)
     * 
     * @details 注意：亮度参数是所有led组共用的，改变亮度会改变所有led组的亮度，而颜色每个led组独立。
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2022-09-24      <td>lilin     <td>新建
     * </table>
     */
    HL_DRV_AW21009_SET_LED_LIGHT_EFFECT,
    /**
     * 
     * @brief 设置led灯光效果，需要<hl_drv_aw21009_led_light_st>类型的指针作为参数
     * @date 2022-09-24
     * @author lilin (lin.li@hollyland-tech.com)
     * 
     * @details led_group参数被忽略，所有led组共享同一个灯光参数
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2022-09-24      <td>lilin     <td>新建
     * </table>
     */
    HL_DRV_AW21009_SET_LED_GROUP_LIGHT_EFFECT,
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
    HL_DRV_AW21009_STOP_POINT_T1,
} hl_drv_aw21009_loop_stop_point_e;

typedef enum _hl_drv_aw21009_loop_start_point
{
    HL_DRV_AW21009_START_POINT_T0 = 0,
    HL_DRV_AW21009_START_POINT_T1,
    HL_DRV_AW21009_START_POINT_T2,
    HL_DRV_AW21009_START_POINT_T3,
} hl_drv_aw21009_loop_start_point_e;

typedef enum _hl_drv_aw21009_led_group
{
    HL_DRV_AW21009_LED_GROUP_NONE = 0x00,
    HL_DRV_AW21009_LED_GROUP_1    = 0x01,
    HL_DRV_AW21009_LED_GROUP_2    = 0x02,
    HL_DRV_AW21009_LED_GROUP_3    = 0x04,
} hl_drv_aw21009_led_group_e;

typedef enum _hl_drv_aw21009_pattern_mode
{
    HL_DRV_AW21009_PAT_MANUAL_MODE      = 0,
    HL_DRV_AW21009_PAT_AUTO_BREATH_MODE = 1,
} hl_drv_aw21009_pattern_mode_e;

typedef struct _hl_drv_aw21009_led_light_param
{
    hl_drv_aw21009_led_group_e led_group;
    uint8_t                    r;
    uint8_t                    g;
    uint8_t                    b;
    uint16_t                   brightness;
} hl_drv_aw21009_led_light_st;

typedef struct _hl_drv_aw21009_auto_breath_param
{
    hl_drv_aw21009_pattern_time_e     rise_time;
    hl_drv_aw21009_pattern_time_e     on_time;
    hl_drv_aw21009_pattern_time_e     fall_time;
    hl_drv_aw21009_pattern_time_e     off_time;
    hl_drv_aw21009_loop_start_point_e start_point;
    hl_drv_aw21009_loop_stop_point_e  stop_point;
    uint16_t                          repeat_times;
} hl_drv_aw21009_auto_breath_param_st;

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
 * @param [in] led_num led设备编号，对应rx上两个aw21009芯片，tx上则没有区别，两个编号都操作同一个芯片, see<hl_drv_aw21009_led_dev_num_e>。 
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