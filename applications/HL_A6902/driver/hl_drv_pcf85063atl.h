#ifndef __HL_DRV_PCF85063ATL_H__
#define __HL_DRV_PCF85063ATL_H__

#include<stdint.h>
#include "drivers/i2c.h"

#define RTC_PCF85063_Control_1      0x00
#define RTC_PCF85063_Control_2      0x01
#define RTC_PCF85063_Offset         0x02
#define RTC_PCF85063_RAM_byte       0x03
#define RTC_PCF85063_Seconds        0x04
#define RTC_PCF85063_Minutes        0x05
#define RTC_PCF85063_Hours          0x06
#define RTC_PCF85063_Days           0x07
#define RTC_PCF85063_Weekdays       0x08
#define RTC_PCF85063_Months         0x09
#define RTC_PCF85063_Years          0x0a
#define RTC_PCF85063_Second_alarm   0x0b
#define RTC_PCF85063_Minute_alarm   0x0c
#define RTC_PCF85063_Hour_alarm     0x0d
#define RTC_PCF85063_Day_alarm      0x0e
#define RTC_PCF85063_Weekday_alarm  0x0f
#define RTC_PCF85063_Timer_value    0x10
#define RTC_PCF85063_Timer_mode     0x11

enum RTC_CTRL_CMD {
    /// @brief 枚举 获取时间
    RTC_GET_TIME,
    /// @brief 枚举 设置时间
    RTC_SET_TIME,
};

typedef struct rtc_time_t
{
    /// 秒  后7bit有效
    uint8_t second;   
    /// 分  后7bit有效
    uint8_t minute;   
    /// 时  12h制 后5bit有效 24h制 后6bit有效
    uint8_t hour;     
    /// 天  后6bit有效
    uint8_t day;      
    /// 周几    后3bit有效
    uint8_t weekday;  
    ///月   后5bit有效
    uint8_t month;    
    ///年
    uint8_t year;
} rtc_time;

/**
 * 
 * @brief 初始化
 * @return uint8_t 
 * @date 2022-09-06
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-06      <td>libo     <td>新建
 * </table>
 */
uint8_t hl_drv_rtc_pcf85063_init();

/**
 * 
 * @brief 去初始化
 * @return uint8_t 
 * @date 2022-09-06
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-06      <td>libo     <td>新建
 * </table>
 */
uint8_t hl_drv_rtc_pcf85063_deinit();

/**
 * 
 * @brief 控制函数
 * @param [in] cmd 
 * @param [in] ptr 
 * @param [in] len 
 * @return uint8_t 
 * @date 2022-09-06
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-06       <td>libo           <td>新建
 * </table>
 */
uint8_t hl_drv_rtc_pcf85063_io_ctrl(uint8_t cmd, void *ptr, uint16_t len);

#endif