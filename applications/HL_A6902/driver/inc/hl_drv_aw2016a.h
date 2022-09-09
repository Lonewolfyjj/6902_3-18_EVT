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
    HL_DRV_AW2016A_SET_PATTERN_MODE_PARAM,
    ///set aw2016a manual mode, type of parameter is <uint8_t> pointer, see <hl_drv_aw2016a_led_channel_e>
    HL_DRV_AW2016A_SET_MANUAL_MODE,
    HL_DRV_AW2016A_SET_MANUAL_MODE_PARAM,
} hl_drv_aw2016a_op_t;

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

typedef enum _hl_drv_aw2016a_max_output_current
{
    HL_DRV_AW2016A_IMAX_15MA = 0x00,
    HL_DRV_AW2016A_IMAX_30MA = 0x01,
    HL_DRV_AW2016A_IMAX_5MA  = 0x10,
    HL_DRV_AW2016A_IMAX_10MA = 0x11,
} hl_drv_aw2016a_max_output_current_e;

typedef enum _hl_drv_aw2016a_led_channel {
    HL_DRV_AW2016A_LED_CHANNEL1 = 0x01,
    HL_DRV_AW2016A_LED_CHANNEL2 = 0x02,
    HL_DRV_AW2016A_LED_CHANNEL3 = 0x04,
} hl_drv_aw2016a_led_channel_e;

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