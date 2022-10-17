/**
 * @file hl_drv_msm8909.h
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief 
 * @version 1.0
 * @date 2022-10-13
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
 * <tr><td>2022-10-13     <td>v1.0     <td>dujunjie     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/*
 * EOF
 */

#ifndef __HL_DRV_MSM8909_H
#define __HL_DRV_MSM8909_H
#include "stdint.h"

/**************************************************************************
 *                                基本宏                                   *
 **************************************************************************/

/* 函数返回状态 */
#define HL_SUCCESS 0
#define HL_FAILED 1

#define MSM8909_IIC_NAME "i2c2"
#define MSM8909_DEVICE_ADDRESS 0x20


/**************************************************************************
 *                                触摸板                                   *
 **************************************************************************/

/*分辨率*/
#define TPD_RES_MAX_X 720
#define TPD_RES_MAX_Y 1280
/*多指个数的定义1~10*/
#define MAX_SUPPORTED_FINGER_NUM 1
/*根据平台是否支持I2c dma mode 定义TPD_HIGH_SPEED_DMA*/
//#define TPD_HIGH_SPEED_DMA
#ifdef TPD_HIGH_SPEED_DMA
#define TOUCH_POINT_MODE 0
#else
#define TOUCH_POINT_MODE 1
#endif
/*Register Map 一些寄存器的定义*/
#define ZINITIX_SWRESET_CMD 0x0000
#define ZINITIX_WAKEUP_CMD 0x0001
#define ZINITIX_IDLE_CMD 0x0004
#define ZINITIX_SLEEP_CMD 0x0005
#define ZINITIX_CLEAR_INT_STATUS_CMD 0x0003
#define ZINITIX_CALIBRATE_CMD 0x0006
#define ZINITIX_SAVE_STATUS_CMD 0x0007
#define ZINITIX_SAVE_CALIBRATION_CMD 0x0008
#define ZINITIX_RECALL_FACTORY_CMD 0x000f
#define ZINITIX_SENSITIVITY 0x0020
#define ZINITIX_I2C_CHECKSUM_WCNT 0x016a
#define ZINITIX_I2C_CHECKSUM_RESULT 0x016c
#define ZINITIX_DEBUG_REG 0x0115  //0~7
#define ZINITIX_TOUCH_MODE 0x0010
#define ZINITIX_CHIP_REVISION 0x0011
#define ZINITIX_FIRMWARE_VERSION 0x0012
#define ZINITIX_MINOR_FW_VERSION 0x0121
#define ZINITIX_DATA_VERSION_REG 0x0013
#define ZINITIX_HW_ID 0x0014
#define ZINITIX_SUPPORTED_FINGER_NUM 0x0015
#define ZINITIX_EEPROM_INFO 0x0018
#define ZINITIX_INITIAL_TOUCH_MODE 0x0019
#define ZINITIX_TOTAL_NUMBER_OF_X 0x0060
#define ZINITIX_TOTAL_NUMBER_OF_Y 0x0061
#define ZINITIX_DELAY_RAW_FOR_HOST 0x007f
#define ZINITIX_BUTTON_SUPPORTED_NUM 0x00B0
#define ZINITIX_BUTTON_SENSITIVITY 0x00B2
#define ZINITIX_X_RESOLUTION 0x00C0
#define ZINITIX_Y_RESOLUTION 0x00C1
#define ZINITIX_POINT_STATUS_REG 0x0080
#define ZINITIX_ICON_STATUS_REG 0x00AA
#define ZINITIX_AFE_FREQUENCY 0x0100
#define ZINITIX_DND_N_COUNT 0x0122
#define ZINITIX_RAWDATA_REG 0x0200
#define ZINITIX_EEPROM_INFO_REG 0x0018
#define ZINITIX_INT_ENABLE_FLAG 0x00f0
#define ZINITIX_PERIODICAL_INTERRUPT_INTERVAL 0x00f1
#define ZINITIX_CHECKSUM_RESULT 0x012c
#define ZINITIX_INIT_FLASH 0x01d0
#define ZINITIX_WRITE_FLASH 0x01d1
#define ZINITIX_READ_FLASH 0x01d2
#define BIT_DOWN 1
#define BIT_MOVE 2
#define BIT_UP 3
#define BIT_PALM 4
#define BIT_PALM_REJECT 5
#define BIT_WAKEUP 6
#define RESERVED_1 7
#define BIT_WEIGHT_CHANGE 8
#define BIT_PT_NO_CHANGE 9
#define BIT_REJECT 10
#define BIT_PT_EXIST 11
#define RESERVED_2 12
#define BIT_MUST_ZERO 13
#define BIT_DEBUG 14
#define BIT_ICON_EVENT 15
#define SUB_BIT_EXIST 0
#define SUB_BIT_DOWN 1
#define SUB_BIT_MOVE 2
#define SUB_BIT_UP 3
#define SUB_BIT_UPDATE 4
#define SUB_BIT_WAIT 5
#define zinitix_bit_set(val, n) ((val) &= ~(1 << (n)), (val) |= (1 << (n)))
#define zinitix_bit_clr(val, n) ((val) &= ~(1 << (n)))
#define zinitix_bit_test(val, n) ((val) & (1 << (n)))
#define zinitix_swap_v(a, b, t) ((t) = (a), (a) = (b), (b) = (t))
#define zinitix_swap_16(s) (((((s)&0xff) << 8) | (((s) >> 8) & 0xff)))


/*报点相关结构体的定义*/
typedef struct _ts_zinitix_coord_t
{
    uint16_t x;
    uint16_t y;
    uint8_t  width;
    uint8_t  sub_status;
} ts_zinitix_coord_t;

typedef struct _ts_zinitix_point_info_t
{
    uint16_t status;
#if TOUCH_POINT_MODE
    uint16_t event_flag;
#else
    uint8_t finger_cnt;
    uint8_t time_stamp;
#endif
    ts_zinitix_coord_t coord[MAX_SUPPORTED_FINGER_NUM];
} ts_zinitix_point_info_t;

rt_size_t hl_drv_get_touch_info(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size);
rt_err_t hl_drv_msm8909_init(rt_device_t dev);

#endif