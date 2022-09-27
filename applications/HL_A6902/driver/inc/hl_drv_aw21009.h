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
     * @brief 设置pwm精度，需要<hl_drv_aw21009_pwm_resolution_e>类型的指针作为参数
     * @date 2022-09-27
     * @author lilin (lin.li@hollyland-tech.com)
     * 
     * @details pattern 模式下固定8位精度
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2022-09-27      <td>lilin     <td>新建
     * </table>
     */
    HL_DRV_AW21009_SET_PWM_RESOLUTION,
    /**
     * 
     * @brief 设置自动呼吸参数，需要<hl_drv_aw21009_auto_breath_param_st>类型的指针作为参数
     * @date 2022-09-24
     * @author lilin (lin.li@hollyland-tech.com)
     * 
     * @details 需要开启自动呼吸模式，且设置组模式；在manual模式下，可以通过该op设置淡入淡出的时间，由rise_time和fall_time决定。
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
     * @details led通道分组：[1-3]、[4-6]、[7-9]，取消组模式后，led通道亮度变为单独控制，否则组内led亮度统一控制
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
     * @details pattern控制模式下，分为自动呼吸模式和manual模式；需要设置组模式。
     * 自动呼吸模式下：brightness的高8位表示呼吸的最大亮度，低8位表示呼吸的最小亮度
     * manual模式下：brightness的高8位表示led switch on时的亮度，低8位表示led switch off时的亮度
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
     * @details <hl_drv_aw21009_led_light_st>中brightness的高八位表示呼吸的最大亮度，
     * 低八位表示呼吸的最小亮度，颜色每个led组独立；需要设置组模式，且失能组控制模式，开
     * 启自动呼吸模式或者manual模式。
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
     * @details led_group参数被忽略，所有led组共享同一个r/g/b参数; 需要设置组模式，且开启组控制模式。
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2022-09-24      <td>lilin     <td>新建
     * </table>
     */
    HL_DRV_AW21009_SET_LED_GROUP_LIGHT_EFFECT,
    /**
     * 
     * @brief 单独设置led通道的亮度，需要<hl_drv_aw21009_led_chan_brightness_st>类型的指针作为参数
     * @date 2022-09-27
     * @author lilin (lin.li@hollyland-tech.com)
     * 
     * @details 需要退出组模式
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2022-09-27      <td>lilin     <td>新建
     * </table>
     */
    HL_DRV_AW21009_SET_LED_CHAN_BRIGHTNESS,
    /**
     * 
     * @brief 设置亮度单字节模式，需要<bool>类型的指针作为参数
     * @date 2022-09-27
     * @author lilin (lin.li@hollyland-tech.com)
     * 
     * @details 设置单字节模式后，亮度精度变为8位，默认12位，使用12位精度时，建议同步设置pwm的精度为12位。pattern 模式下固定8位精度
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2022-09-27      <td>lilin     <td>新建
     * </table>
     */
    HL_DRV_AW21009_SET_SINGLE_BYTE_MODE,
    /**
     * 
     * @brief 设置rgb模式，需要<bool>类型的指针作为参数
     * @date 2022-09-27
     * @author lilin (lin.li@hollyland-tech.com)
     * 
     * @details 设置rgb模式后，所有led通道亮度都由通道[1 - 3]控制，需要退出组模式。**建议关闭该模式，rgb模式并未测试成功。
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2022-09-27      <td>lilin     <td>新建
     * </table>
     */
    HL_DRV_AW21009_SET_RGB_MODE,
    /**
     * 
     * @brief 单独设置led通道的颜色深浅，需要<hl_drv_aw21009_led_chan_color_st>类型的指针作为参数
     * @date 2022-09-27
     * @author lilin (lin.li@hollyland-tech.com)
     * 
     * @details 需要退出组模式，或者失能组控制模式
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2022-09-27      <td>lilin     <td>新建
     * </table>
     */
    HL_DRV_AW21009_SET_LED_CHAN_COLOR,
    /**
     * 
     * @brief 设置led开关，需要<bool>类型的指针作为参数
     * @date 2022-09-27
     * @author lilin (lin.li@hollyland-tech.com)
     * 
     * @details 切换亮度输出值，由<_hl_drv_aw21009_led_light_param>的brightness高8位决定最大值，低8位决定最小值。
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2022-09-27      <td>lilin     <td>新建
     * </table>
     */
    HL_DRV_AW21009_MANUAL_SET_SWITCH,
    /**
     * 
     * @brief 设置淡入淡出效果，需要<bool>类型的指针作为参数
     * @date 2022-09-27
     * @author lilin (lin.li@hollyland-tech.com)
     * 
     * @details 
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2022-09-27      <td>lilin     <td>新建
     * </table>
     */
    HL_DRV_AW21009_MANUAL_SET_RAMP,
    /**
     * 
     * @brief 进入省电模式，需要<bool>类型的指针作为参数
     * @date 2022-09-27
     * @author lilin (lin.li@hollyland-tech.com)
     * 
     * @details true：进入，false：退出
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2022-09-27      <td>lilin     <td>新建
     * </table>
     */
    HL_DRV_AW21009_SET_POWER_SAVE_MODE,
} hl_drv_aw21009_op_e;

typedef enum _hl_drv_aw21009_pattern_time
{
    HL_DRV_AW21009_PAT_TIME_0S00 = 0,
    HL_DRV_AW21009_PAT_TIME_0S13,
    HL_DRV_AW21009_PAT_TIME_0S26,
    HL_DRV_AW21009_PAT_TIME_0S38,
    HL_DRV_AW21009_PAT_TIME_0S51,
    HL_DRV_AW21009_PAT_TIME_0S77,
    HL_DRV_AW21009_PAT_TIME_1S04,
    HL_DRV_AW21009_PAT_TIME_1S60,
    HL_DRV_AW21009_PAT_TIME_2S10,
    HL_DRV_AW21009_PAT_TIME_2S60,
    HL_DRV_AW21009_PAT_TIME_3S10,
    HL_DRV_AW21009_PAT_TIME_4S20,
    HL_DRV_AW21009_PAT_TIME_5S20,
    HL_DRV_AW21009_PAT_TIME_6S20,
    HL_DRV_AW21009_PAT_TIME_7S30,
    HL_DRV_AW21009_PAT_TIME_8S30,
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
    HL_DRV_AW21009_PAT_MODE_DISABLE     = 2,
} hl_drv_aw21009_pattern_mode_e;

typedef enum _hl_drv_aw21009_led_dev_num
{
    HL_DRV_AW21009_LED_DEV_0,
    HL_DRV_AW21009_LED_DEV_1,
} hl_drv_aw21009_led_dev_num_e;

typedef enum _hl_drv_aw21009_led_channel
{
    ///red color channel
    HL_DRV_AW21009_LED_CHANNEL1 = 0x01,
    ///green color channel
    HL_DRV_AW21009_LED_CHANNEL2 = 0x02,
    ///blue color channel
    HL_DRV_AW21009_LED_CHANNEL3 = 0x04,
    ///red color channel
    HL_DRV_AW21009_LED_CHANNEL4 = 0x08,
    ///green color channel
    HL_DRV_AW21009_LED_CHANNEL5 = 0x10,
    ///blue color channel
    HL_DRV_AW21009_LED_CHANNEL6 = 0x20,
    ///red color channel
    HL_DRV_AW21009_LED_CHANNEL7 = 0x40,
    ///green color channel
    HL_DRV_AW21009_LED_CHANNEL8 = 0x80,
    ///blue color channel
    HL_DRV_AW21009_LED_CHANNEL9 = 0x100,
} hl_drv_aw21009_led_channel_e;

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

typedef struct _hl_drv_aw21009_led_chan_brightness
{
    hl_drv_aw21009_led_channel_e led_chan;
    uint16_t                     brightness;
} hl_drv_aw21009_led_chan_brightness_st;

typedef struct _hl_drv_aw21009_led_chan_color
{
    hl_drv_aw21009_led_channel_e led_chan;
    uint8_t                      color;
} hl_drv_aw21009_led_chan_color_st;

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
} hl_drv_aw21009_pwm_resolution_e;

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