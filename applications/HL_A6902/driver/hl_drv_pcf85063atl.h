#ifndef __HL_DRV_PCF85063ATL_H__
#define __HL_DRV_PCF85063ATL_H__

#include<stdint.h>
#include "drivers/i2c.h"

#define RTC_PCF85063_Control_1 0x00
#define RTC_PCF85063_Control_2 0x01
#define RTC_PCF85063_Offset 0x02
#define RTC_PCF85063_RAM_byte 0x03
#define RTC_PCF85063_Seconds 0x04
#define RTC_PCF85063_Minutes 0x05
#define RTC_PCF85063_Hours 0x06
#define RTC_PCF85063_Days 0x07
#define RTC_PCF85063_Weekdays 0x08
#define RTC_PCF85063_Months 0x09
#define RTC_PCF85063_Years 0x0a
#define RTC_PCF85063_Second_alarm 0x0b
#define RTC_PCF85063_Minute_alarm 0x0c
#define RTC_PCF85063_Hour_alarm 0x0d
#define RTC_PCF85063_Day_alarm 0x0e
#define RTC_PCF85063_Weekday_alarm 0x0f
#define RTC_PCF85063_Timer_value 0x10
#define RTC_PCF85063_Timer_mode 0x11

enum RTC_CTRL_CMD {
    RTC_GET_TIME,
    RTC_SET_TIME,
};

typedef struct rtc_time_t
{
    uint8_t second;   //后7bit有效
    uint8_t minute;   //后7bit有效
    uint8_t hour;     //12h制 后5bit有效 24h制 后6bit有效
    uint8_t day;      //后6bit有效
    uint8_t weekday;  //后3bit有效
    uint8_t month;    //后5bit有效
    uint8_t year;
} rtc_time;

uint8_t hl_drv_rtc_pcf85063_init();

uint8_t hl_drv_rtc_pcf85063_deinit();

uint8_t hl_drv_rtc_pcf85063_io_ctrl(uint8_t cmd, void *ptr, uint16_t len);

#endif