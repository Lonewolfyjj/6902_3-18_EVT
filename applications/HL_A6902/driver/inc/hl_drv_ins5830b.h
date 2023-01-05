/**
 * @file hl_drv_ins5830b.h
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief 
 * @version 1.0
 * @date 2023-01-05
 * 
 * ██╗  ██╗ ██████╗ ██╗     ██╗  ██╗   ██╗██╗      █████╗ ███╗   ██╗██████╗ 
 * ██║  ██║██╔═══██╗██║     ██║  ╚██╗ ██╔╝██║     ██╔══██╗████╗  ██║██╔══██╗
 * ███████║██║   ██║██║     ██║   ╚████╔╝ ██║     ███████║██╔██╗ ██║██║  ██║
 * ██╔══██║██║   ██║██║     ██║    ╚██╔╝  ██║     ██╔══██║██║╚██╗██║██║  ██║
 * ██║  ██║╚██████╔╝███████╗███████╗██║   ███████╗██║  ██║██║ ╚████║██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝
 * @copyright Copyright (c) 2023 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2023-01-05     <td>v1.0     <td>dujunjie       <td>初次发布
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
#ifndef __HL_DRV_INS5830B_H
#define __HL_DRV_INS5830B_H

#include <rtthread.h>
#include <rtdevice.h>
#include "hl_config.h"


/* 函数返回状态 */
#define HL_SUCCESS 0
#define HL_FAILED 1

/* 使用I2C */
#if HL_IS_TX_DEVICE()
#define INS5830B_IIC_NAME "i2c1"
#else
#define INS5830B_IIC_NAME "i2c0"
#endif

#define INS5830B_DEVICE_ADDRESS 0x32

#define INS5830B_RTC_FIRST_REG 0x10
#define INS5830B_INIEN_SET_REG 0x1F

#define INS5830B_RTC_SET_TIME_CMD   0
#define INS5830B_RTC_GET_TIME_CMD   1

typedef struct _HL_INS5830B_SECOND_T
{
    uint8_t sec_low : 4;  //

    uint8_t sec_high : 3; //

    uint8_t Reserved0 : 1;
}HL_SECOND_T;

typedef struct _HL_INS5830B_MINUTE_T
{
    uint8_t min_low : 4;  //

    uint8_t min_high : 3; //

    uint8_t Reserved1 : 1;
}HL_MINUTE_T;

typedef struct _HL_INS5830B_HOUR_T
{
    uint8_t hour_low : 4;  //

    uint8_t hour_high : 2; //

    uint8_t Reserved2 : 2;
}HL_HOUR_T;

typedef struct _HL_INS5830B_DAY_T
{
    uint8_t day_low : 4;  //

    uint8_t day_high : 2; //

    uint8_t Reserved3 : 2;
}HL_DAY_T;

typedef struct _HL_INS5830B_MONTH_T
{
    uint8_t month_low : 4;  //

    uint8_t month_high : 1; //

    uint8_t Reserved4 : 3;
}HL_MONTH_T;

typedef struct _HL_INS5830B_YEAR_T
{
    uint8_t year_low : 4;  //

    uint8_t year_high : 4; //
}HL_YEAR_T;

typedef enum _HL_INS5830B_WEEK_T
{
    //周一
    HL_WEEK_SUNDAY = 0x01,
    HL_WEEK_MONDAY = 0x02,
    HL_WEEK_TUESDAY = 0x04,
    HL_WEEK_WEDNESDAY = 0x08,
    HL_WEEK_THURSDAY = 0x10,
    HL_WEEK_FIRDAY = 0x20,
    HL_WEEK_SATURDAY = 0x40,
}HL_INS5830B_WEEK_T;

typedef struct _HL_INS5830B_RTC_T
{
    HL_SECOND_T rtc_sec;
    HL_MINUTE_T rtc_min;
    HL_HOUR_T rtc_hour;
    uint8_t rtc_week; 
    HL_DAY_T rtc_day;
    HL_MONTH_T rtc_month;
    HL_YEAR_T rtc_year;
}__attribute__((page)) HL_INS5830B_RTC_T;



/// @brief 外部调用
typedef struct _HL_INS5830B_RTC_IOCTL_T
{
    /// @brief 年 2000 - 2099
    uint16_t year;
    /// @brief 月 1 - 12
    uint8_t month;
    /// @brief 日
    uint8_t day;
    /// @brief 时 0 - 23
    uint8_t hour;
    /// @brief 分 0 - 59
    uint8_t min;
    /// @brief 秒 0 - 59
    uint8_t sec;
}HL_INS5830B_RTC_IOCTL_T;

int hl_drv_ins5830b_init(void);
int hl_drv_ins5830b_io_ctrl(uint8_t cmd, void* ptr, uint8_t len);
#endif