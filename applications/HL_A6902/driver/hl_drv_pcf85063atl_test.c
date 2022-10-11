/**
 * @file hl_drv_pcf85063atl_test.c
 * @author libo (rd46@hollyland-tech.com)
 * @brief 
 * @version 1.0
 * @date 2022-09-19
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-19     <td>v1.0     <td>libo     <td>内容
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
#include <string.h>
#include <rtdevice.h>
#include "drivers/i2c.h"

enum RTC_CTRL_CMD
{
    RTC_GET_TIME,
    RTC_SET_TIME,
};

#define RTC_PCF85063_I2C_BUS_NAME "i2c1"               /* 传感器连接的I2C总线设备名称 */
#define RTC_PCF85063_ADDR 0x51                         /* 从机地址 */
static struct rt_i2c_bus_device* i2c_handle = RT_NULL; /* I2C总线设备句柄 */

#define RTC_PCF85063_Control_1          0x00
#define RTC_PCF85063_Control_2          0x01
#define RTC_PCF85063_Offset             0x02
#define RTC_PCF85063_RAM_byte           0x03
#define RTC_PCF85063_Seconds            0x04
#define RTC_PCF85063_Minutes            0x05
#define RTC_PCF85063_Hours              0x06
#define RTC_PCF85063_Days               0x07
#define RTC_PCF85063_Weekdays           0x08
#define RTC_PCF85063_Months             0x09
#define RTC_PCF85063_Years              0x0a
#define RTC_PCF85063_Second_alarm       0x0b
#define RTC_PCF85063_Minute_alarm       0x0c
#define RTC_PCF85063_Hour_alarm         0x0d
#define RTC_PCF85063_Day_alarm          0x0e
#define RTC_PCF85063_Weekday_alarm      0x0f
#define RTC_PCF85063_Timer_value        0x10
#define RTC_PCF85063_Timer_mode         0x11

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


/**
 * 
 * @brief 写寄存器 
 * @date 2022-09-07
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-07      <td>libo     <td>新建
 * </table>
 */
static rt_err_t write_reg(struct rt_i2c_bus_device* bus, rt_uint8_t reg, rt_uint8_t* data, rt_uint8_t len)
{
    rt_uint8_t        buf[20];
    struct rt_i2c_msg msgs;
    rt_uint32_t       buf_size = len+1;

    buf[0] = reg;  //cmd

    rt_memcpy(&buf[1], data, len);

    // for(int i = 0; i<10;i++){
    //     rt_kprintf("data :%d\n", buf[i]);
    // }
    msgs.addr  = RTC_PCF85063_ADDR;
    msgs.flags = RT_I2C_WR;
    msgs.buf   = buf;
    msgs.len   = buf_size;

    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(bus, &msgs, 1) == 1) {
        rt_kprintf("RTC_PCF85063ATL  write_reg  ok\n");
        return RT_EOK;
    } else {
        rt_kprintf("RTC_PCF85063ATL  write_reg  error\n");
        return -RT_ERROR;
    }
}

/**
 * 
 * @brief 读寄存器数据 
 * @date 2022-09-07
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-07      <td>libo     <td>新建
 * </table>
 */
static rt_err_t read_regs(struct rt_i2c_bus_device* bus, rt_uint8_t* buf, rt_uint8_t len)
{
    struct rt_i2c_msg msgs;

    msgs.addr  = RTC_PCF85063_ADDR;
    msgs.flags = RT_I2C_RD;
    msgs.buf   = buf;
    msgs.len   = len;

    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(bus, &msgs, 1) == 1) {
        rt_kprintf("RTC_PCF85063ATL  read_regs  ok\n");
        return RT_EOK;
    } else {
        rt_kprintf("RTC_PCF85063ATL  read_regs  error\n");
        return -RT_ERROR;
    }
}

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
static uint8_t hl_drv_rtc_pcf85063_init()
{
    rt_uint8_t buf[10];

    i2c_handle = (struct rt_i2c_bus_device*)rt_device_find(RTC_PCF85063_I2C_BUS_NAME);
    if (i2c_handle == RT_NULL) {
        rt_kprintf("RTC_PCF85063ATL  get i2c_handle  fail\n");
        return -1;
    }

    // buf[0] = 0x58;

    // if (write_reg(i2c_handle, RTC_PCF85063_Control_1, buf, 1)) {
    //     rt_kprintf("RTC_PCF85063ATL  write_reg   fail\n");
    // }

    return 0;
}

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
static uint8_t hl_drv_rtc_pcf85063_deinit()
{
    return 0;
}

/**
 * 
 * @brief 获取时间
 * @param [in] ptr 
 * @return uint8_t 
 * @date 2022-09-07
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-07      <td>libo     <td>新建
 * </table>
 */
static uint8_t hl_get_rtc_time(void* ptr)
{
    if (write_reg(i2c_handle, RTC_PCF85063_Seconds, NULL, 0)) {
        rt_kprintf("RTC_PCF85063ATL  write_reg   fail\n");
        return -1;
    }

    rt_thread_mdelay(400);

    if (read_regs(i2c_handle, (char*)ptr, 7)) {
        rt_kprintf("RTC_PCF85063ATL  read_regs   fail\n");
        return -1;
    }
    return 0;
}

/**
 * 
 * @brief 获取时间
 * @param [in] ptr 
 * @return uint8_t 
 * @date 2022-09-19
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-19      <td>libo     <td>新建
 * </table>
 */
static uint8_t hl_set_rtc_time(void* ptr)
{
    if (write_reg(i2c_handle, RTC_PCF85063_Seconds, (rt_uint8_t*)ptr, 7)) {
        rt_kprintf("RTC_PCF85063ATL  write_reg   fail\n");
        return -1;
    }

    return 0;
}

/**
 * 
 * @brief 
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
static uint8_t hl_drv_rtc_pcf85063_io_ctrl(uint8_t cmd, void* ptr, uint16_t len)
{
    switch (cmd) {
        case RTC_GET_TIME:
            if (hl_get_rtc_time(ptr)) {
                rt_kprintf("RTC_PCF85063ATL  read_regs   fail\n");
                return -1;
            }
            break;
        case RTC_SET_TIME:
            if (hl_set_rtc_time(ptr)) {
                rt_kprintf("RTC_PCF85063ATL  RTC_SET_TIME   fail\n");
                return -1;
            }
            break;
        default:
            break;
    }

    return 0;
}
/**
 * 
 * @brief demo
 * @param [in] argc 
 * @param [in] argv 
 * @date 2022-09-22
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-22       <td>libo     <td>新建
 * </table>
 */
void hl_drv_pcf85063atl(int argc, char** argv)
{
    rtc_time time;
    memset(&time, 0, sizeof(rtc_time));

    hl_drv_rtc_pcf85063_init();
    rt_kprintf(" argc :%d argv[0]: %s\n", argc, argv[1]);
    if (argc == 7) {
        if (!strcmp("set", argv[1])) {
            time.year   = atoi(argv[2]);
            time.month  = atoi(argv[3]);
            time.day    = atoi(argv[4]);
            time.hour   = atoi(argv[5]);
            time.minute = atoi(argv[6]);
            hl_drv_rtc_pcf85063_io_ctrl(RTC_SET_TIME, (void*)&time, 0);
        } else {
            rt_kprintf(" parm1 no set or get\n");
        }
    }

    if (argc == 2) {
        if (!strcmp("get", argv[1])) {
            hl_drv_rtc_pcf85063_io_ctrl(RTC_GET_TIME, (void*)&time, 0);
            rt_kprintf(" year: %d\n", time.year);
            rt_kprintf(" month: %d\n", time.month);
            rt_kprintf(" day: %d\n", time.day);
            rt_kprintf(" hour: %d\n", time.hour);
            rt_kprintf(" minute: %d\n", time.minute);
        } else {
            rt_kprintf(" parm1 no  get\n");
        }
    }
}

MSH_CMD_EXPORT(hl_drv_pcf85063atl, parm 1 : set/get parm 2 - 6: year month day hour minute);