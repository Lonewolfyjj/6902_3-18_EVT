/**
 * @file hl_drv_ft3169.h
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief FT3169驱动头文件
 * @version 1.0
 * @date 2022-10-17
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
 * <tr><td>2022-10-17     <td>v1.0     <td>dujunjie     <td>内容
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

#ifndef __HL_DRV_FT3169_H
#define __HL_DRV_FT3169_H
/*****************************************************************************
* Included header files
*****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/**************************************************************************
 *                                基本宏                                     *
 **************************************************************************/

/* 函数返回状态 */
#define HL_SUCCESS 0
#define HL_FAILED 1

/* 使用I2C */
#define FT3169_IIC_NAME "i2c1"
#define FT3169_DEVICE_ADDRESS 0x38

/*****************************************************************************
* Private constant and macro definitions using #define
*****************************************************************************/
#define HOST_MCU_DRIVER_VERSION                  	"Hollyland MCU V1.0 20220615"

#define FTS_INFO(fmt, ...)							rt_kprintf("[FTS/I]%s:"fmt"\n", __func__, ##__VA_ARGS__)
#define FTS_ERROR(fmt, ...)							rt_kprintf("[FTS/E]%s:"fmt"\n", __func__,  ##__VA_ARGS__)
#define FTS_DEBUG(fmt, ...)	    					rt_kprintf("[FTS/D]%s:"fmt"\n", __func__, ##__VA_ARGS__)

#define INTERVAL_READ_REG                   		200  /* unit:ms */


#define FTS_CMD_READ_ID                     		0x90

/* chip id */
#define FTS_CHIP_IDH								0x52
#define FTS_CHIP_IDL								0x60

/* register address */
#define FTS_REG_CHIP_ID                     		0xA3
#define FTS_REG_CHIP_ID2                    		0x9F
#define FTS_REG_FW_VER                      		0xA6
#define FTS_REG_UPGRADE                         	0xFC

/*
 * Gesture function enable
 * default: disable
 */
#define FTS_GESTURE_EN                          	0

//控制函数参数
enum hl_ft3169_cfg
{
    SYS_RESET = RT_TOUCH_CTRL_ENABLE_INT + 1,
    SYS_SUSPENDS,
    SYS_RESUME,
    SYS_ESD_CHECK,
    CHECK_DEV_ID
};

/*****************************************************************************
* Private enumerations, structures and unions using typedef
*****************************************************************************/
/*
 * Structures of point information
 *
 * @x: X coordinate of this point
 * @y: Y coordinate of this point
 * @p: pressure value of this point
 * @type: event type of this point, 0 means down event,
 *		  1 means up event, 2 means contact event
 * @id: ID of this point
 * @area: touch area of this point
 */
struct fts_ts_event {
    int x;		/*x coordinate */
    int y;		/*y coordinate */
    int p;		/* pressure */
    int type;	/* touch event flag: 0 -- down; 1-- up; 2 -- contact */
    int id; 	/*touch ID */
    int area;   /*touch area*/
};


struct fts_ts_data {
    int suspended;		   /* suspended state, 1: suspended mode, 0:not suspended mode */
    int esd_support;	   /* esd enable or disable, default: disable */
    int gesture_support;   /* gesture enable or disable, default: disable */
};


/*****************************************************************************
* Global variable or extern global variabls/functions
*****************************************************************************/
// extern struct fts_ts_data *fts_data;

// void fts_msleep(unsigned long msec);

/* communication interface */
// int fts_read(uint8_t addr, uint8_t *data, uint16_t datalen);
// int fts_read_reg(uint8_t addr, uint8_t *value);
// int fts_write(uint8_t addr, uint8_t *data, uint16_t datalen);
// int fts_write_reg(uint8_t addr, uint8_t value);

// rt_err_t fts_ts_init(rt_device_t dev);
// int fts_ts_suspend(void);
// int fts_ts_resume(void);

// int fts_check_id(void);
// int fts_hw_reset(uint32_t msec);
// rt_size_t fts_touch_event_handler(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size);
// void fts_timer_interrupt_handler(void);
#endif /* __FOCALTECH_CORE_H__ */
