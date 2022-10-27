/**
 * @file hl_RTC_PCF85063ATL.c
 * @author libo (rd46@hollyland-tech.com)
 * @brief 
 * @version 1.0
 * @date 2022-09-06
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-06     <td>v1.0     <td>libo     <td>内容
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

#include "hl_drv_pcf85063atl.h"

#define RTC_PCF85063_I2C_BUS_NAME "i2c1"               /* 传感器连接的I2C总线设备名称 */
#define RTC_PCF85063_ADDR 0x51                         /* 从机地址 */
static struct rt_i2c_bus_device* i2c_handle = RT_NULL; /* I2C总线设备句柄 */


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
    if(RT_NULL == bus){
        rt_kprintf("RTC_PCF85063ATL  write_reg  bus is null\n");
        return -2;
    }
    rt_uint8_t        buf[20];
    struct rt_i2c_msg msgs;
    rt_uint32_t       buf_size = len+1;

    buf[0] = reg;  //cmd

    rt_memcpy(&buf[1], data, len);

    msgs.addr  = RTC_PCF85063_ADDR;
    msgs.flags = RT_I2C_WR;
    msgs.buf   = buf;
    msgs.len   = buf_size;

    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(bus, &msgs, 1) == 1) {
        //rt_kprintf("RTC_PCF85063ATL  write_reg  ok\n");
        return RT_EOK;
    } else {
        rt_kprintf("RTC_PCF85063ATL  write_reg  error %d\n", rt_i2c_transfer(bus, &msgs, 1));
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
    if(RT_NULL == bus){
        rt_kprintf("RTC_PCF85063ATL  read_regs  bus is null\n");
        return -2;
    }
    struct rt_i2c_msg msgs;

    msgs.addr  = RTC_PCF85063_ADDR;
    msgs.flags = RT_I2C_RD;
    msgs.buf   = buf;
    msgs.len   = len;

    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(bus, &msgs, 1) == 1) {
        //rt_kprintf("RTC_PCF85063ATL  read_regs  ok\n");
        return RT_EOK;
    } else {
        rt_kprintf("RTC_PCF85063ATL  read_regs  error\n");
        return -RT_ERROR;
    }
}

/**
 * 
 * @brief 10进制转16进制 （只改变进制 不变数值）
 * @param [in] data 
 * @return u_int8_t 
 * @date 2022-10-14
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-14      <td>libo     <td>新建
 * </table>
 */
static uint8_t DecimalToBCD(uint8_t data)
{
    uint8_t high = 0;

    while(data >= 10){
        high++;
        data -= 10;
    }

    return((high<<4)|data);
}

/**
 * 
 * @brief 16进制转10进制 （只改变进制 不变数值）
 * @param [in] data 
 * @return u_int8_t 
 * @date 2022-10-14
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-14      <td>libo     <td>新建
 * </table>
 */
static uint8_t BCDToDecimal(uint8_t data)
{
    uint8_t tenweight,oneweight,result;
    oneweight =  data&0x0f;
    tenweight = (data&0xf0)>>4;
    result = tenweight*10 + oneweight*1;
    return result;
}

uint8_t hl_drv_rtc_pcf85063_init()
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


uint8_t hl_drv_rtc_pcf85063_deinit()
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
 * @brief 锁住内部计时时钟
 * @return uint8_t 
 * @date 2022-10-11
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-11      <td>libo     <td>新建
 * </table>
 */
static uint8_t hl_lock_rtc_clock(void)
{
    rt_uint8_t reg0x0 = 0x00;
    if (write_reg(i2c_handle, RTC_PCF85063_Control_1, NULL, 0)) {
        rt_kprintf("RTC_PCF85063ATL  write_reg  RTC_PCF85063_Control_1 fail\n");
        return -1;
    }

    if (read_regs(i2c_handle, &reg0x0, 1)) {
        rt_kprintf("RTC_PCF85063ATL  read_regs  RTC_PCF85063_Control_1 fail\n");
        return -1;
    }
    reg0x0 |=0x20;

    if (write_reg(i2c_handle, RTC_PCF85063_Control_1, &reg0x0, 1)) {
        rt_kprintf("RTC_PCF85063ATL  write_reg RTC_PCF85063_Control_1  fail\n");
        return -1;
    }


    return 0;

}

/**
 * 
 * @brief 
 * @return uint8_t 
 * @date 2022-10-11
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-11      <td>libo     <td>新建
 * </table>
 */
static uint8_t hl_unlock_rtc_clock(void)
{
    rt_uint8_t reg0x0 = 0x00;
    if (write_reg(i2c_handle, RTC_PCF85063_Control_1, NULL, 0)) {
        rt_kprintf("RTC_PCF85063ATL  write_reg RTC_PCF85063_Control_1  fail\n");
        return -1;
    }

    if (read_regs(i2c_handle, &reg0x0, 1)) {
        rt_kprintf("RTC_PCF85063ATL  read_regs  RTC_PCF85063_Control_1 fail\n");
        return -1;
    }

    reg0x0 &= 0xDF;
    if (write_reg(i2c_handle, RTC_PCF85063_Control_1, &reg0x0, 1)) {
        rt_kprintf("RTC_PCF85063ATL  write_reg RTC_PCF85063_Control_1  fail\n");
        return -1;
    }


    return 0;
}

/**
 * 
 * @brief 设置时间
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
    if(ptr == NULL){
        rt_kprintf(" hl_set_rtc_time  param  error\n");
        return 1;
    }

    if(hl_lock_rtc_clock()){
        rt_kprintf("RTC_PCF85063ATL  hl_lock_rtc_clock  fail\n");
        return -1;
    }
    
    if (write_reg(i2c_handle, RTC_PCF85063_Seconds, (rt_uint8_t*)ptr, 7)) {
        rt_kprintf("RTC_PCF85063ATL  write_reg RTC_PCF85063_Seconds  fail\n");
        return -1;
    }

    if(hl_unlock_rtc_clock()){
        rt_kprintf("RTC_PCF85063ATL  hl_lock_rtc_clock  fail\n");
        return -1;
    }

    return 0;
}


uint8_t hl_drv_rtc_pcf85063_io_ctrl(uint8_t cmd, void* ptr, uint16_t len)
{
    if(ptr == NULL){
        rt_kprintf(" hl_drv_rtc_pcf85063_io_ctrl  param  error\n");
        return 1;
    }

    uint8_t* ptr1 = (uint8_t*) ptr;
    
    /*将输入参数10进制转换成bcd码*/
    for (size_t i = 0; i < len; i++){
        ptr1[i]= DecimalToBCD(ptr1[i]);
    }
    
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

    
    /*将输出参数bcd码转换成10进制*/
    for (size_t i = 0; i < len; i++){
         ptr1[i]= BCDToDecimal(ptr1[i]);
    }

    return 0;
}