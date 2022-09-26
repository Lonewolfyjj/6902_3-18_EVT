/**
 * @file hl_drv_aw2016a.h
 * @author lilin (lin.li@hollyland-tech.com)
 * @brief LED驱动头文件
 * @version 1.0
 * @date 2022-09-06
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-06     <td>v1.0     <td>lilin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __HL_DRV_AW2016A_H__
#define __HL_DRV_AW2016A_H__

/* Includes ------------------------------------------------------------------*/

#include "rtthread.h"
#include "rtdevice.h"
#include "hl_config.h"

/* typedef -------------------------------------------------------------------*/

typedef enum _hl_drv_aw2016a_op
{
    ///get aw2016a chip id, type of parameter is <uint8_t> pointer
    HL_DRV_AW2016A_GET_CHIP_ID,
    ///set aw2016a work mode, type of parameter is <uint8_t> pointer, see <hl_drv_aw2016a_work_mode_e>
    HL_DRV_AW2016A_SET_WORK_MODE,
    ///set aw2016a Global Max Output Current, type of parameter is <uint8_t> pointer, see <hl_drv_aw2016a_max_output_current_e>
    HL_DRV_AW2016A_SET_GLOBAL_MAX_OUTPUT_CURRENT,
    ///open aw2016a led channel, type of parameter is <uint8_t> pointer, see <hl_drv_aw2016a_led_channel_e>
    HL_DRV_AW2016A_OPEN_LED_CHANNEL,
    ///close aw2016a led channel, type of parameter is <uint8_t> pointer, see <hl_drv_aw2016a_led_channel_e>
    HL_DRV_AW2016A_CLOSE_LED_CHANNEL,
    ///set aw2016a pattern mode, type of parameter is <uint8_t> pointer, see <hl_drv_aw2016a_led_channel_e>
    HL_DRV_AW2016A_SET_PATTERN_MODE,
    ///set aw2016a pattern mode param, type of parameter is <hl_drv_aw2016a_pattern_param_st> pointer
    HL_DRV_AW2016A_SET_PATTERN_MODE_PARAM,
    ///set aw2016a manual mode, type of parameter is <uint8_t> pointer, see <hl_drv_aw2016a_led_channel_e>
    HL_DRV_AW2016A_SET_MANUAL_MODE,
    ///set aw2016a led channel output current, type of parameter is <hl_drv_aw2016a_output_current_st> pointer
    HL_DRV_AW2016A_SET_LED_CHANNEL_OUTPUT_CURRENT,
    ///set aw2016a led channel pwm level, type of parameter is <hl_drv_aw2016a_pwm_level_st> pointer
    HL_DRV_AW2016A_SET_LED_CHANNEL_PWM_LEVEL,
    ///dump aw2016a register value, type of parameter is <uint8_t> pointer
    HL_DRV_AW2016A_DUMP_REGISTER_VALUE,
} hl_drv_aw2016a_op_t;

/**
 * 
 * @brief LED编号枚举
 * @date 2022-09-19
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 该枚举描述了LED的编号，每一个LED对应一个aw2016a设备，因为可能有两个aw2016a芯片
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-19      <td>lilin     <td>新建
 * </table>
 */
typedef enum _hl_drv_aw2016a_led_num
{
    HL_DRV_AW2016A_LED0,
    HL_DRV_AW2016A_LED1,
} hl_drv_aw2016a_led_num_e;

typedef enum _hl_drv_aw2016a_work_mode
{
    HL_DRV_AW2016A_STANDBY_MODE = 0x00,
    HL_DRV_AW2016A_ACTIVE_MODE  = 0x01,
} hl_drv_aw2016a_work_mode_e;

/**
 * 
 * @brief 全局最大输出电流枚举
 * @date 2022-09-19
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 该枚举描述了led通道的全局最大输出电流IMAX，IMAX决定最大亮度
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-19      <td>lilin     <td>新建
 * </table>
 */
typedef enum _hl_drv_aw2016a_max_output_current
{
    HL_DRV_AW2016A_IMAX_15MA = 0x00,
    HL_DRV_AW2016A_IMAX_30MA = 0x01,
    HL_DRV_AW2016A_IMAX_5MA  = 0x10,
    HL_DRV_AW2016A_IMAX_10MA = 0x11,
} hl_drv_aw2016a_max_output_current_e;

/**
 * 
 * @brief led通道枚举
 * @date 2022-09-19
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 该枚举描述了led的三个通道
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-19      <td>lilin     <td>新建
 * </table>
 */
typedef enum _hl_drv_aw2016a_led_channel
{
    ///r通道，对应6902上的红灯
    HL_DRV_AW2016A_LED_CHANNEL1 = 0x01,
    ///g通道，对应6902上的绿灯
    HL_DRV_AW2016A_LED_CHANNEL2 = 0x02,
    ///b通道，对应6902上的蓝灯
    HL_DRV_AW2016A_LED_CHANNEL3 = 0x04,
} hl_drv_aw2016a_led_channel_e;

/**
 * 
 * @brief 呼吸时间枚举
 * @date 2022-09-19
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 该枚举描述了呼吸参数可以设置的时间
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-19      <td>lilin     <td>新建
 * </table>
 */
typedef enum _hl_drv_aw2016a_pattern_time
{
    ///T1、T3：0秒，T2、T4、T0：0.04秒
    HL_DRV_AW2016A_PATTERN_0S_OR_0S04 = 0x00,
    HL_DRV_AW2016A_PATTERN_0S13,
    HL_DRV_AW2016A_PATTERN_0S26,
    HL_DRV_AW2016A_PATTERN_0S38,
    HL_DRV_AW2016A_PATTERN_0S51,
    HL_DRV_AW2016A_PATTERN_0S77,
    HL_DRV_AW2016A_PATTERN_1S04,
    HL_DRV_AW2016A_PATTERN_1S60,
    HL_DRV_AW2016A_PATTERN_2S10,
    HL_DRV_AW2016A_PATTERN_2S60,
    HL_DRV_AW2016A_PATTERN_3S10,
    HL_DRV_AW2016A_PATTERN_4S20,
    HL_DRV_AW2016A_PATTERN_5S20,
    HL_DRV_AW2016A_PATTERN_6S20,
    HL_DRV_AW2016A_PATTERN_7S30,
    HL_DRV_AW2016A_PATTERN_8S30,
} hl_drv_aw2016a_pattern_time_e;

/**
 * 
 * @brief 设置自动呼吸效果结构体
 * @date 2022-09-19
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 该结构体描述了灯的呼吸过程
 *           /-----------\
 *          /      |      \
 *         /|      |      |\
 *        / |      |      | \-----------
 *          |hold_time_ms |      |
 *          |             |      |
 *    rise_time_ms  fall_time_ms |
 *                          off_time_ms
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-19      <td>lilin     <td>新建
 * </table>
 */
typedef struct _hl_drv_aw2016a_pattern_param
{
    ///led通道：r/g/b
    hl_drv_aw2016a_led_channel_e led_chan;
    ///开始呼吸的前置延迟时间
    hl_drv_aw2016a_pattern_time_e t0;
    ///rise_time_ms
    hl_drv_aw2016a_pattern_time_e t1;
    ///hold_time_ms
    hl_drv_aw2016a_pattern_time_e t2;
    ///fall_time_ms
    hl_drv_aw2016a_pattern_time_e t3;
    ///off_time_ms
    hl_drv_aw2016a_pattern_time_e t4;
    ///循环次数，[0 - 15]，其中0表示一直循环
    uint8_t repeat;
} hl_drv_aw2016a_pattern_param_st;

/**
 * 
 * @brief 设置灯颜色结构体
 * @date 2022-09-19
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 输出电流决定了灯的颜色
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-19      <td>lilin     <td>新建
 * </table>
 */
typedef struct _hl_drv_aw2016a_output_current
{
    ///led通道：r/g/b
    hl_drv_aw2016a_led_channel_e led_chan;
    ///输出电流，[0 - 15]
    uint8_t current;
} hl_drv_aw2016a_output_current_st;

/**
 * 
 * @brief 设置亮度等级结构体
 * @date 2022-09-19
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details pwm等级决定了灯的亮度，最大亮度受IMAX的影响
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-19      <td>lilin     <td>新建
 * </table>
 */
typedef struct _hl_drv_aw2016a_pwm_level
{
    ///led通道：r/g/b
    hl_drv_aw2016a_led_channel_e led_chan;
    ///pwm等级，最大255
    uint8_t pwm_level;
} hl_drv_aw2016a_pwm_level_st;

/* define --------------------------------------------------------------------*/

#define AW2016A_FUNC_RET_OK 0
#define AW2016A_FUNC_RET_ERR 1

#define AW2016A_CHIP_ID 0x09

/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

int hl_drv_aw2016a_init(void);

int hl_drv_aw2016a_deinit(void);

int hl_drv_aw2016a_ctrl(uint8_t led_num, uint8_t op, void* arg, int32_t arg_size);

#endif
/*
 * EOF
 */