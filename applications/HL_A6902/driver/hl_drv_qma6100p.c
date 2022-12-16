/**
 * @file HL_drv_QMA6100p.c
 * @author libo (rd46@hollyland-tech.com)
 * @brief 
 * @version 1.0
 * @date 2022-09-21
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-21     <td>v1.0     <td>libo     <td>内容
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
#include "hl_drv_qma6100p.h"
#include "math.h"


#define HL_QMA6100P_I2C_BUS_NAME "i2c1"                    /* 传感器连接的I2C总线设备名称 */
static struct rt_i2c_bus_device* i2c_handle = RT_NULL;     /* I2C总线设备句柄 */

static const qst_convert_t qst_map[] = { { { 1, 1, 1 }, { 0, 1, 2 } }, { { -1, 1, 1 }, { 1, 0, 2 } },
                                       { { -1, -1, 1 }, { 0, 1, 2 } }, { { 1, -1, 1 }, { 1, 0, 2 } },
                                       { { -1, 1, -1 }, { 0, 1, 2 } }, { { 1, 1, -1 }, { 1, 0, 2 } },
                                       { { 1, -1, -1 }, { 0, 1, 2 } }, { { -1, -1, -1 }, { 1, 0, 2 } } };

static HL_QMA6100_data_t g_QMA6100;

/**
 * 
 * @brief 延时函数
 * @param [in] delay 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static void HL_QMA6100_delay(uint32_t delay)
{
    int32_t i, j, z;
    for (i = 0; i < delay; i++) {
        for (j = 0; j < 1000; j++) {
            z++;
        }
    }
}

/**
 * 
 * @brief 获取i2c句柄
 * @return uint8_t 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static uint8_t HL_drv_get_i2c_handle_init(void)
{
    if (i2c_handle != RT_NULL) {
        return 0;
    }

    i2c_handle = (struct rt_i2c_bus_device*)rt_device_find(HL_QMA6100P_I2C_BUS_NAME);
    if (i2c_handle == RT_NULL) {
        rt_kprintf(" HL_drv_get_i2c_handle_init  fail\n");
        return 1;
    }
    return 0;
}

/**
 * 
 * @brief i2c写寄存器数据 
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
static rt_err_t HL_i2c_write_reg(struct rt_i2c_bus_device* bus, rt_uint8_t addr, rt_uint8_t reg, rt_uint8_t* data,
                          rt_uint8_t len)
{
    int16_t ret = 0;
    if(RT_NULL == bus){
        rt_kprintf("QMA6100P  HL_i2c_write_reg  bus is null\n");
        return -2;
    }

    rt_uint8_t        buf[20];
    struct rt_i2c_msg msgs;
    rt_uint32_t       buf_size = len + 1;

    buf[0] = reg;  //cmd

    rt_memcpy(&buf[1], data, len);

    msgs.addr  = addr;
    msgs.flags = RT_I2C_WR;
    msgs.buf   = buf;
    msgs.len   = buf_size;

    /* 调用I2C设备接口传输数据 */
    ret = rt_i2c_transfer(bus, &msgs, 1);
    if (ret == 1) {
        return RT_EOK;
    } else {
        rt_kprintf("HL_i2c_write_reg  error ret : %d\n",ret);
        return -RT_ERROR;
    }
}

/**
 * 
 * @brief i2c写一个字节
 * @param [in] addr 
 * @param [in] reg 
 * @param [in] data 
 * @return int32_t 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static int32_t HL_I2C_ByteWrite(uint8_t addr, uint8_t reg, uint8_t data)
{
    if (HL_drv_get_i2c_handle_init()) {
        rt_kprintf("get_i2c_handle  error\n");
        return 1;
    }
    if (HL_i2c_write_reg(i2c_handle, addr, reg, &data, 1)) {
        return 1;
    }
    return 0;
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
// static rt_err_t HL_i2c_read_regs(struct rt_i2c_bus_device* bus, rt_uint8_t addr, rt_uint8_t* buf, rt_uint8_t len)
// {
//     if(RT_NULL == bus){
//         rt_kprintf("QMA6100P HL_i2c_read_regs  bus is null\n");
//         return -2;
//     }    
//     struct rt_i2c_msg msgs;

//     msgs.addr  = addr;
//     msgs.flags = RT_I2C_RD;
//     msgs.buf   = buf;
//     msgs.len   = len;

//     /* 调用I2C设备接口传输数据 */
//     if (rt_i2c_transfer(bus, &msgs, 1) == 1) {
//         //rt_kprintf("HL_i2c_read_regs  ok\n");
//         return RT_EOK;
//     } else {
//         rt_kprintf("HL_i2c_read_regs  error\n");
//         return -RT_ERROR;
//     }
// }

static rt_err_t hl_i2c_read_reg(struct rt_i2c_bus_device* bus,rt_uint8_t addr, rt_uint8_t reg, rt_uint8_t* rbuf,rt_uint8_t len)
{
    rt_uint8_t        buf[4];
    struct rt_i2c_msg msgs[2];

    msgs[0].addr  = addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &reg;
    msgs[0].len   = 1;

    msgs[1].addr  = addr;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = rbuf;
    msgs[1].len   = len;

    // 调用I2C设备接口传输数据
    if (rt_i2c_transfer(bus, msgs, 2) == 2) {
        return RT_EOK;
    } else
        return -RT_ERROR;
}

/**
 * 
 * @brief i2c读取寄存器
 * @param [in] addr 设备地址
 * @param [in] reg 寄存器地址
 * @param [in] data 数据句柄
 * @param [in] num 长度
 * @return int32_t 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static int32_t HL_I2C_BufferRead(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t num)
{
    if (HL_drv_get_i2c_handle_init()) {
        rt_kprintf("get_i2c_handle  error\n");
        return 1;
    }
    hl_i2c_read_reg(i2c_handle,addr,reg,data,num);
    // if (HL_i2c_write_reg(i2c_handle, addr, reg, data, 0)) {
    //     return 1;
    // }
    // if (HL_i2c_read_regs(i2c_handle, addr, data, num)) {
    //     return 1;
    // }
    return 0;
}

/**
 * 
 * @brief QMA写寄存器
 * @param [in] reg_add 寄存器地址
 * @param [in] reg_dat 数据
 * @return int32_t 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static int32_t HL_QMA6100_write_reg(uint8_t reg_add, uint8_t reg_dat)
{
    int32_t ret         = HL_QMA6100_FAIL;
    int32_t ret_i2c     = 0;
    uint32_t retry      = 0;

    while ((ret == HL_QMA6100_FAIL) && (retry++ < 5)) {
        ret_i2c = (int32_t)HL_I2C_ByteWrite(g_QMA6100.slave, reg_add, reg_dat);

        if (ret_i2c == 0){
            return HL_QMA6100_SUCCESS;
        }  
        else{
             ret = HL_QMA6100_FAIL;
        }           
    }
    return ret;
}

/**
 * 
 * @brief QMA读取寄存器
 * @param [in] reg_add 寄存器地址
 * @param [in] buf 数据句柄
 * @param [in] num 长度
 * @return int32_t 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static int32_t HL_QMA6100_read_reg(uint8_t reg_add, uint8_t* buf, uint16_t num)
{
    int32_t ret         = HL_QMA6100_FAIL;
    int32_t ret_i2c     = 0;
    uint32_t retry      = 0;

    while ((ret == HL_QMA6100_FAIL) && (retry++ < 5)) {
        ret_i2c = (int32_t)HL_I2C_BufferRead(g_QMA6100.slave, reg_add, buf, (uint16_t)num);

        if (ret_i2c == 0){
            return HL_QMA6100_SUCCESS;
        }
        else{
            ret = HL_QMA6100_FAIL;
        }     
    }
    return ret;
}

/**
 * 
 * @brief 获取设备id
 * @return uint8_t 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static uint8_t HL_QMA6100_chip_id()
{
    uint8_t  chip_id = 0x00;
    int32_t  ret     = HL_QMA6100_FAIL;

    ret = HL_QMA6100_read_reg(HL_QMA6100_CHIP_ID, &chip_id, 1);
    HL_QMA6100_LOG("HL_QMA6100_chip_id =0x%x ret=%d\n", chip_id, ret);

    return chip_id;
}

/**
 * 
 * @brief 
 * @return uint8_t 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static uint8_t HL_QMA6100_die_wafe_id(void)
{
    uint8_t      die_buf_id[2] = {0x00};
    uint8_t      wafe_id       = 0x00;
    uint16_t     dieid         = 0;

    int32_t ret = HL_QMA6100_FAIL;

    ret   = HL_QMA6100_read_reg(0x47, die_buf_id, 2);
    dieid = die_buf_id[1] << 8 | die_buf_id[0];

    ret = HL_QMA6100_read_reg(0x5a, &wafe_id, 1);

    HL_QMA6100_LOG("dieid =0x%x wafe_id=0x%x\n", dieid, wafe_id);

    return ret;
}

/**
 * 
 * @brief 设置加速度量程
 * @param [in] range 
 * @return int32_t 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static int32_t HL_QMA6100_set_range(uint8_t range)
{
    int32_t ret = 0;

    if (range == HL_QMA6100_RANGE_4G)
        g_QMA6100.lsb_1g = 2048;
    else if (range == HL_QMA6100_RANGE_8G)
        g_QMA6100.lsb_1g = 1024;
    else if (range == HL_QMA6100_RANGE_16G)
        g_QMA6100.lsb_1g = 512;
    else if (range == HL_QMA6100_RANGE_32G)
        g_QMA6100.lsb_1g = 256;
    else
        g_QMA6100.lsb_1g = 4096;

    ret = HL_QMA6100_write_reg(HL_QMA6100_REG_RANGE, range);

    return ret;
}

/**
 * 
 * @brief HL_QMA6100_set_mode_odr
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static int32_t HL_QMA6100_set_mode_odr(int32_t mode, int32_t mclk, int32_t div, int32_t lpf)
{
    int32_t ret      = 0;
    uint8_t  mclk_reg = (uint8_t)mclk;
    uint8_t  odr_reg  = (uint8_t)div;
    uint8_t  lpf_reg  = (uint8_t)lpf;  //|0x80;

    if (mode >= HL_QMA6100_MODE_ACTIVE) {
        ret = HL_QMA6100_write_reg(HL_QMA6100_REG_POWER_CTL, 0x80 | mclk_reg);
        ret = HL_QMA6100_write_reg(HL_QMA6100_REG_BW_ODR, lpf_reg | odr_reg);
    } else {
        ret = HL_QMA6100_write_reg(HL_QMA6100_REG_POWER_CTL, 0x00);
    }

    return ret;
}

/**
 * 
 * @brief HL_QMA6100_dump_reg
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static void HL_QMA6100_dump_reg(void)
{
    uint8_t  reg_data  = 0;
    int32_t  i         = 0;
    uint8_t  reg_map[] = { 0x0f, 0x10, 0x11, 0x09, 0x0a, 0x0b, 0x0c, 0x16, 0x17, 0x18, 0x19, 0x1a,
                      0x1b, 0x1c, 0x20, 0x21, 0x30, 0x31, 0x37, 0x3e, 0x4a, 0x50, 0x56 };

    HL_QMA6100_LOG("HL_QMA6100_dump_reg\n");
    for (i = 0; i < sizeof(reg_map) / sizeof(reg_map[0]); i++) {
        HL_QMA6100_read_reg(reg_map[i], &reg_data, 1);
        HL_QMA6100_LOG("0x%x = 0x%x\n", reg_map[i], reg_data);
    }
}

/**
 * 
 * @brief 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
#if defined(HL_QMA6100_DATA_READY)
static void HL_QMA6100_drdy_config(int32_t int_map, int32_t enable)
{
    uint8_t reg_17 = 0;
    uint8_t reg_1a = 0;
    uint8_t reg_1c = 0;

    HL_QMA6100_LOG("HL_QMA6100_drdy_config %d\n", enable);
    HL_QMA6100_read_reg(0x17, &reg_17, 1);
    HL_QMA6100_read_reg(0x1a, &reg_1a, 1);
    HL_QMA6100_read_reg(0x1c, &reg_1c, 1);

    if (enable) {
        reg_17 |= 0x10;
        reg_1a |= 0x10;
        reg_1c |= 0x10;
        HL_QMA6100_write_reg(0x17, reg_17);
        if (int_map == HL_QMA6100_MAP_INT1)
            HL_QMA6100_write_reg(0x1a, reg_1a);
        else if (int_map == HL_QMA6100_MAP_INT2)
            HL_QMA6100_write_reg(0x1c, reg_1c);
    } else {
        reg_17 &= (~0x10);
        reg_1a &= (~0x10);
        reg_1c &= (~0x10);
        HL_QMA6100_write_reg(0x17, reg_17);
        HL_QMA6100_write_reg(0x1a, reg_1a);
        HL_QMA6100_write_reg(0x1c, reg_1c);
    }
}
#endif

#if defined(HL_QMA6100_FIFO_FUNC)
static uint8_t HL_QMA6100_fifo_reg[64 * 6];

/**
 * 
 * @brief 
 * @param [in] int_map 
 * @param [in] enable 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static void HL_QMA6100_fifo_config(int32_t int_map, int32_t enable)
{
    uint8_t reg_17, reg_1a, reg_1c = 0;

    HL_QMA6100_LOG("HL_QMA6100_fifo_config enable:%d\n", enable);
    HL_QMA6100_read_reg(0x17, &reg_17, 1);
    HL_QMA6100_read_reg(0x1a, &reg_1a, 1);
    HL_QMA6100_read_reg(0x1c, &reg_1c, 1);

    if (enable) {
        g_QMA6100.fifo_mode = HL_QMA6100_FIFO_MODE_FIFO;
        if (g_QMA6100.fifo_mode == HL_QMA6100_FIFO_MODE_FIFO) {
            //HL_QMA6100_write_reg(0x31, 0x40);
            HL_QMA6100_write_reg(0x3E, 0x47);  //bit[6:7] 0x00:BYPASS 0x40:FIFO 0x80:STREAM
            HL_QMA6100_write_reg(0x17, reg_17 | 0x20);
            if (int_map == HL_QMA6100_MAP_INT1)
                HL_QMA6100_write_reg(0x1a, reg_1a | 0x20);
            else if (int_map == HL_QMA6100_MAP_INT2)
                HL_QMA6100_write_reg(0x1c, reg_1c | 0x20);
        } else if (g_QMA6100.fifo_mode == HL_QMA6100_FIFO_MODE_STREAM) {
            HL_QMA6100_write_reg(0x31, 0x20);  // 0x3f
            HL_QMA6100_write_reg(0x3E, 0x87);  //bit[6:7] 0x00:BYPASS 0x40:FIFO 0x80:STREAM
            HL_QMA6100_write_reg(0x17, reg_17 | 0x40);
            if (int_map == HL_QMA6100_MAP_INT1)
                HL_QMA6100_write_reg(0x1a, reg_1a | 0x40);
            else if (int_map == HL_QMA6100_MAP_INT2)
                HL_QMA6100_write_reg(0x1c, reg_1c | 0x40);
        } else if (g_QMA6100.fifo_mode == HL_QMA6100_FIFO_MODE_BYPASS) {
            HL_QMA6100_write_reg(0x3E, 0x07);  //bit[6:7] 0x00:BYPASS 0x40:FIFO 0x80:STREAM
            HL_QMA6100_write_reg(0x17, reg_17 | 0x20);
            if (int_map == HL_QMA6100_MAP_INT1)
                HL_QMA6100_write_reg(0x1a, reg_1a | 0x20);
            else if (int_map == HL_QMA6100_MAP_INT2)
                HL_QMA6100_write_reg(0x1c, reg_1c | 0x20);
        }
    } else {
        g_QMA6100.fifo_mode = HL_QMA6100_FIFO_MODE_NONE;
        reg_17 &= (~0x60);
        reg_1a &= (~0x60);
        reg_1c &= (~0x60);
        HL_QMA6100_write_reg(0x17, reg_17);
        HL_QMA6100_write_reg(0x1a, reg_1a);
        HL_QMA6100_write_reg(0x1c, reg_1c);
    }
}

/**
 * 
 * @brief 
 * @param [in] fifo_buf 
 * @return int32_t 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static int32_t HL_QMA6100_read_fifo(uint8_t* fifo_buf)
{
    int32_t ret = 0;
    uint8_t  databuf[2];

#if defined(HL_QMA6100_INT_LATCH)
    ret = HL_QMA6100_read_reg(HL_QMA6100_INT_STAT2, databuf, 1);
#endif
    ret = HL_QMA6100_read_reg(HL_QMA6100_FIFO_STATE, databuf, 1);
    if (ret != HL_QMA6100_SUCCESS) {
        HL_QMA6100_ERR("HL_QMA6100_read_fifo state error\n");
        return ret;
    }
    g_QMA6100.fifo_len = databuf[0] & 0x7f;
    if (g_QMA6100.fifo_len > 64) {
        HL_QMA6100_ERR("HL_QMA6100_read_fifo depth(%d) error\n", g_QMA6100.fifo_len);
        return HL_QMA6100_FAIL;
    }
#if 1
    HL_QMA6100_read_reg(0x3f, fifo_buf, g_QMA6100.fifo_len * 6);
#else
    for (int icount = 0; icount < g_QMA6100.fifo_len; icount++) {
        HL_QMA6100_read_reg(0x3f, &fifo_buf[icount * 6], 6);
    }
#endif
    if (g_QMA6100.fifo_mode == HL_QMA6100_FIFO_MODE_FIFO) {
        ret = HL_QMA6100_write_reg(0x3e, 0x47);
    } else if (g_QMA6100.fifo_mode == HL_QMA6100_FIFO_MODE_STREAM) {
        ret = HL_QMA6100_write_reg(0x3e, 0x87);
    } else if (g_QMA6100.fifo_mode == HL_QMA6100_FIFO_MODE_BYPASS) {
        ret = HL_QMA6100_write_reg(0x3e, 0x07);
    }
    // log fifo
    return ret;
}

/**
 * 
 * @brief 
 * @param [in] fifo_buf 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static void HL_QMA6100_exe_fifo(uint8_t* fifo_buf)
{
    int32_t icount;
    int16_t raw_data[3];
    //float acc_data[3];

    HL_QMA6100_ERR("fifo_depth=%d\n", g_QMA6100.fifo_len);
    // log fifo
    for (icount = 0; icount < g_QMA6100.fifo_len; icount++) {
        raw_data[0] = (int16_t)(((int16_t)(fifo_buf[1 + icount * 6] << 8)) | (fifo_buf[0 + icount * 6]));
        raw_data[1] = (int16_t)(((int16_t)(fifo_buf[3 + icount * 6] << 8)) | (fifo_buf[2 + icount * 6]));
        raw_data[2] = (int16_t)(((int16_t)(fifo_buf[5 + icount * 6] << 8)) | (fifo_buf[4 + icount * 6]));
        raw_data[0] = raw_data[0] >> 2;
        raw_data[1] = raw_data[1] >> 2;
        raw_data[2] = raw_data[2] >> 2;
        HL_QMA6100_LOG("%d:%d	%d	%d	", icount, raw_data[0], raw_data[1], raw_data[2]);
        if ((icount % 4 == 0)) {
            HL_QMA6100_LOG("\r\n");
        }
        //acc_data[0] = (raw_data[0]*9.807f)/(g_QMA6100.lsb_1g);			//GRAVITY_EARTH_1000
        //acc_data[1] = (raw_data[1]*9.807f)/(g_QMA6100.lsb_1g);
        //acc_data[2] = (raw_data[2]*9.807f)/(g_QMA6100.lsb_1g);
    }
    HL_QMA6100_LOG("\r\n");
    // log fifo
}
#endif

/**
 * 
 * @brief 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
#if defined(HL_QMA6100_STEPCOUNTER)
static uint32_t HL_QMA6100_read_stepcounter(void)
{
    uint8_t         data[3];
    uint8_t         ret;
    uint32_t        step_num;
    int32_t        step_dif;
    static uint32_t step_last = 0;

    ret = HL_QMA6100_read_reg(HL_QMA6100_STEP_CNT_L, data, 2);
    if (ret != HL_QMA6100_SUCCESS) {
        step_num = step_last;
        return HL_QMA6100_SUCCESS;
    }
    ret = HL_QMA6100_read_reg(HL_QMA6100_STEP_CNT_M, &data[2], 1);
    if (ret != HL_QMA6100_SUCCESS) {
        step_num = step_last;
        return HL_QMA6100_SUCCESS;
    }

    step_num = (uint32_t)(((uint32_t)data[2] << 16) | ((uint32_t)data[1] << 8) | data[0]);

#if 1  //defined(HL_QMA6100_CHECK_ABNORMAL_DATA)
    step_dif = (int32_t)(step_num - step_last);
    if (HL_QMA6100_ABS(step_dif) > 100) {
        uint32_t step_num_temp[3];

        ret              = HL_QMA6100_read_reg(HL_QMA6100_STEP_CNT_L, data, 2);
        ret              = HL_QMA6100_read_reg(HL_QMA6100_STEP_CNT_M, &data[2], 1);
        step_num_temp[0] = (uint32_t)(((uint32_t)data[2] << 16) | ((uint32_t)data[1] << 8) | data[0]);
        HL_QMA6100_delay(2);

        ret              = HL_QMA6100_read_reg(HL_QMA6100_STEP_CNT_L, data, 2);
        ret              = HL_QMA6100_read_reg(HL_QMA6100_STEP_CNT_M, &data[2], 1);
        step_num_temp[1] = (uint32_t)(((uint32_t)data[2] << 16) | ((uint32_t)data[1] << 8) | data[0]);
        HL_QMA6100_delay(2);

        ret              = HL_QMA6100_read_reg(HL_QMA6100_STEP_CNT_L, data, 2);
        ret              = HL_QMA6100_read_reg(HL_QMA6100_STEP_CNT_M, &data[2], 1);
        step_num_temp[2] = (uint32_t)(((uint32_t)data[2] << 16) | ((uint32_t)data[1] << 8) | data[0]);
        HL_QMA6100_delay(2);
        if ((step_num_temp[0] == step_num_temp[1]) && (step_num_temp[1] == step_num_temp[2])) {
            HL_QMA6100_LOG("HL_QMA6100 check data, confirm!\n");
            step_num = step_num_temp[0];
        } else {
            HL_QMA6100_LOG("HL_QMA6100 check data, abnormal!\n");
            return step_last;
        }
    }
#endif
    step_last = step_num;

    return step_num;
}

/**
 * 
 * @brief 
 * @param [in] enable 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static void HL_QMA6100_stepcounter_config(int32_t enable)
{
    //  int32_t odr = 27;   // 100Hz
    uint8_t reg_12 = 0x00;
    uint8_t reg_14 = 0x00;
    uint8_t reg_15 = 0x00;
    uint8_t reg_1e = 0x00;

    HL_QMA6100_write_reg(0x13, 0x80);
    HL_QMA6100_delay(1);
    HL_QMA6100_write_reg(0x13, 0x7f);  //  0x7f(P2P/16), 0.977*16*LSB
    // Windows time
    if (enable) {
#if defined(HL_QMA6100_ODR_27)
        reg_12 = 0x84;
        reg_14 = 0x05;  //((200*odr)/(1000));      // about:200 ms
        reg_15 = 0x07;  //(((2200/8)*odr)/1000);   // 2000 ms
#elif defined(HL_QMA6100_ODR_55)
        reg_12 = 0x89;  //0x89
        reg_14 = 0x0b;  //0x0c  //((200*odr)/(1000));
        reg_15 = 0x0c;  //0x0e //(((2200/8)*odr)/1000);   // 2000 ms
#elif defined(HL_QMA6100_ODR_100)
        reg_12 = 0x8f;
        reg_14 = 0x1c;  //((280*odr)/(1000));      // about:280 ms
        reg_15 = 0x19;  //(((2200/8)*odr)/1000);   // 2000 ms
#endif
        HL_QMA6100_IRQ_LOG("step time config 0x14=0x%x 0x15=0x%x\n", reg_14, reg_15);
        HL_QMA6100_write_reg(0x12, reg_12);
        HL_QMA6100_write_reg(0x14, reg_14);
        HL_QMA6100_write_reg(0x15, reg_15);
        // lpf
        HL_QMA6100_read_reg(0x1e, &reg_1e, 1);
        reg_1e &= 0x3f;
        HL_QMA6100_write_reg(0x1e, (uint8_t)(reg_1e | HL_QMA6100_STEP_LPF_0));  // default 0x08
        // start count, p2p, fix peak
        HL_QMA6100_write_reg(0x1f, (uint8_t)HL_QMA6100_STEP_START_24 | 0x08);  // 0x10
    }
}

/**
 * 
 * @brief 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static void HL_QMA6100_clear_step(void)
{
    HL_QMA6100_write_reg(0x13, 0x80);  // clear step
    HL_QMA6100_delay(10);
    HL_QMA6100_write_reg(0x13, 0x80);  // clear step
    HL_QMA6100_delay(10);
    HL_QMA6100_write_reg(0x13, 0x80);  // clear step
    HL_QMA6100_delay(10);
    HL_QMA6100_write_reg(0x13, 0x80);  // clear step
    HL_QMA6100_write_reg(0x13, 0x7f);  // clear step
}

/**
 * 
 * @brief 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
#if defined(HL_QMA6100_STEP_INT)
static void HL_QMA6100_step_int_config(int32_t int_map, int32_t enable)
{
    uint8_t reg_16 = 0;
    uint8_t reg_19 = 0;
    uint8_t reg_1b = 0;

    HL_QMA6100_read_reg(0x16, &reg_16, 1);
    HL_QMA6100_read_reg(0x19, &reg_19, 1);
    HL_QMA6100_read_reg(0x1b, &reg_1b, 1);
    if (enable) {
        reg_16 |= 0x08;
        reg_19 |= 0x08;
        reg_1b |= 0x08;
        HL_QMA6100_write_reg(0x16, reg_16);
        if (int_map == HL_QMA6100_MAP_INT1)
            HL_QMA6100_write_reg(0x19, reg_19);
        else if (int_map == HL_QMA6100_MAP_INT2)
            HL_QMA6100_write_reg(0x1b, reg_1b);
    } else {
        reg_16 &= (~0x08);
        reg_19 &= (~0x08);
        reg_1b &= (~0x08);

        HL_QMA6100_write_reg(0x16, reg_16);
        HL_QMA6100_write_reg(0x19, reg_19);
        HL_QMA6100_write_reg(0x1b, reg_1b);
    }
}
#endif

/**
 * 
 * @brief 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
#if defined(HL_QMA6100_SIGNIFICANT_STEP_INT)
static void HL_QMA6100_sigstep_int_config(int32_t int_map, int32_t enable)
{
    uint8_t reg_16 = 0;
    uint8_t reg_19 = 0;
    uint8_t reg_1b = 0;

    HL_QMA6100_read_reg(0x16, &reg_16, 1);
    HL_QMA6100_read_reg(0x19, &reg_19, 1);
    HL_QMA6100_read_reg(0x1b, &reg_1b, 1);

    HL_QMA6100_write_reg(0x1d, 0x1a);
    if (enable) {
        reg_16 |= 0x40;
        reg_19 |= 0x40;
        reg_1b |= 0x40;
        HL_QMA6100_write_reg(0x16, reg_16);
        if (int_map == HL_QMA6100_MAP_INT1)
            HL_QMA6100_write_reg(0x19, reg_19);
        else if (int_map == HL_QMA6100_MAP_INT2)
            HL_QMA6100_write_reg(0x1b, reg_1b);
    } else {
        reg_16 &= (~0x40);
        reg_19 &= (~0x40);
        reg_1b &= (~0x40);

        HL_QMA6100_write_reg(0x16, reg_16);
        HL_QMA6100_write_reg(0x19, reg_19);
        HL_QMA6100_write_reg(0x1b, reg_1b);
    }
}
#endif

#endif

/**
 * 
 * @brief 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
#if defined(HL_QMA6100_ANY_MOTION)
static void HL_QMA6100_anymotion_config(int32_t int_map, int32_t enable)
{
    uint8_t reg_0x18 = 0;
    uint8_t reg_0x1a = 0;
    uint8_t reg_0x1c = 0;
    uint8_t reg_0x2c = 0;
#if defined(HL_QMA6100_SIGNIFICANT_MOTION)
    uint8_t reg_0x19 = 0;
    uint8_t reg_0x1b = 0;
#endif

    HL_QMA6100_LOG("HL_QMA6100_anymotion_config %d\n", enable);
    HL_QMA6100_read_reg(0x18, &reg_0x18, 1);
    HL_QMA6100_read_reg(0x1a, &reg_0x1a, 1);
    HL_QMA6100_read_reg(0x1c, &reg_0x1c, 1);
    HL_QMA6100_read_reg(0x2c, &reg_0x2c, 1);
    reg_0x2c |= 0x00;

    HL_QMA6100_write_reg(0x2c, reg_0x2c);
    HL_QMA6100_write_reg(0x2e, 0x09);  // 0.488*16*32 = 250mg  //定义中断阈值

    HL_QMA6100_write_reg(0x30, 0x80 | 0x3f);  // default 0x3f //绕过LPF过滤

    if (enable) {
        reg_0x18 |= 0x07;
        reg_0x1a |= 0x01;
        reg_0x1c |= 0x01;

        HL_QMA6100_write_reg(0x18, reg_0x18);   //enable any motion 任何运动 xyz 轴中断使能
        if (int_map == HL_QMA6100_MAP_INT1)
            HL_QMA6100_write_reg(0x1a, reg_0x1a); //将任何运动中断映射到INT1引脚
        else if (int_map == HL_QMA6100_MAP_INT2)
            HL_QMA6100_write_reg(0x1c, reg_0x1c); //将任何运动中断映射到INT2引脚
    } else {
        reg_0x18 &= (~0x07);
        reg_0x1a &= (~0x01);
        reg_0x1c &= (~0x01);

        HL_QMA6100_write_reg(0x18, reg_0x18);  //对照上面反过来
        HL_QMA6100_write_reg(0x1a, reg_0x1a);
        HL_QMA6100_write_reg(0x1c, reg_0x1c);
    }

#if defined(HL_QMA6100_SIGNIFICANT_MOTION)
    HL_QMA6100_read_reg(0x19, &reg_0x19, 1);
    HL_QMA6100_read_reg(0x1b, &reg_0x1b, 1);

    HL_QMA6100_write_reg(0x2f, 0x01);  // bit0: selecat significant motion
    if (enable) {
        reg_0x19 |= 0x01;
        reg_0x1b |= 0x01;
        if (int_map == HL_QMA6100_MAP_INT1)
            HL_QMA6100_write_reg(0x19, reg_0x19);
        else if (int_map == HL_QMA6100_MAP_INT2)
            HL_QMA6100_write_reg(0x1b, reg_0x1b);
    } else {
        reg_0x19 &= (~0x01);
        reg_0x1b &= (~0x01);
        HL_QMA6100_write_reg(0x19, reg_0x19);
        HL_QMA6100_write_reg(0x1b, reg_0x1b);
    }
#endif
}
#endif

/**
 * 
 * @brief 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
#if defined(HL_QMA6100_NO_MOTION)
static void HL_QMA6100_nomotion_config(int32_t int_map, int32_t enable)
{
    uint8_t reg_0x18 = 0;
    uint8_t reg_0x1a = 0;
    uint8_t reg_0x1c = 0;
    uint8_t reg_0x2c = 0;

    HL_QMA6100_LOG("HL_QMA6100_nomotion_config %d\n", enable);

    HL_QMA6100_read_reg(0x18, &reg_0x18, 1);
    HL_QMA6100_read_reg(0x1a, &reg_0x1a, 1);
    HL_QMA6100_read_reg(0x1c, &reg_0x1c, 1);
    HL_QMA6100_read_reg(0x2c, &reg_0x2c, 1);
    reg_0x2c |= 0x24;  // 10s
    //reg_0x2c |= 0xc0; 		// 100s

    HL_QMA6100_write_reg(0x2c, reg_0x2c);
    HL_QMA6100_write_reg(0x2d, 0x14);
    if (enable) {
        reg_0x18 |= 0xe0;
        reg_0x1a |= 0x80;
        reg_0x1c |= 0x80;
        HL_QMA6100_write_reg(0x18, reg_0x18);
        if (int_map == HL_QMA6100_MAP_INT1)
            HL_QMA6100_write_reg(0x1a, reg_0x1a);
        else if (int_map == HL_QMA6100_MAP_INT2)
            HL_QMA6100_write_reg(0x1c, reg_0x1c);
    } else {
        reg_0x18 &= (~0xe0);
        reg_0x1a &= (~0x80);
        reg_0x1c &= (~0x80);

        HL_QMA6100_write_reg(0x18, reg_0x18);
        HL_QMA6100_write_reg(0x1a, reg_0x1a);
        HL_QMA6100_write_reg(0x1c, reg_0x1c);
    }
}
#endif

/**
 * 
 * @brief 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
#if defined(HL_QMA6100_TAP_FUNC)
static void HL_QMA6100_tap_config(int32_t tap_type, int32_t int_map, int32_t enable)
{
    uint8_t reg_16, reg_19, reg_1a, reg_1b, reg_1c;
    uint8_t tap_reg = (uint8_t)tap_type;

    HL_QMA6100_read_reg(0x16, &reg_16, 1);
    HL_QMA6100_read_reg(0x19, &reg_19, 1);
    HL_QMA6100_read_reg(0x1a, &reg_1a, 1);
    HL_QMA6100_read_reg(0x1b, &reg_1b, 1);
    HL_QMA6100_read_reg(0x1c, &reg_1c, 1);

    //HL_QMA6100_write_reg(0x1e, 0x03);		// TAP_QUIET_TH 31.25*8 = 250mg
    //HL_QMA6100_write_reg(0x2a, 0x43);		// tap config1
    //HL_QMA6100_write_reg(0x2b, (0xc0+6));		// tap config2
    // add by yang, tep counter, raise wake, and tap detector,any motion by pass LPF
    //HL_QMA6100_write_reg(0x30, 0x80|0x40|0x3f);	// default 0x3f
    // add by yang, tep counter, raise wake, and tap detector,any motion by pass LPF

    HL_QMA6100_write_reg(0x1e, TAP_QUIET_TH);  // TAP_QUIET_TH 31.25*8 = 250mg
    /*single tap param*/
    //HL_QMA6100_write_reg(0x2a, TAP_QUIET_TIME_20MS|TAP_SHOCK_TIME_50MS|TAP_TRIPLE_NOT_WAIT_QUAD_DELAY_Y|TAP_EARIN_N|TAP_DUR_250MS);
    HL_QMA6100_write_reg(0x2a, TAP_QUIET_TIME_30MS | TAP_SHOCK_TIME_75MS | TAP_TRIPLE_NOT_WAIT_QUAD_DELAY_Y | TAP_EARIN_N
                               | TAP_DUR_500MS);        // tap config1
    HL_QMA6100_write_reg(0x2b, TAP_AXIS_Z | TAP_SHOCK_TH);  // tap config2

    if (enable) {
        reg_16 |= tap_reg;
        HL_QMA6100_write_reg(0x16, reg_16);

        if (int_map == HL_QMA6100_MAP_INT1) {
            if (tap_type & HL_QMA6100_TAP_QUARTER) {
                reg_1a |= 0x02;
                HL_QMA6100_write_reg(0x1a, reg_1a);
            }

            reg_19 |= tap_reg;
            HL_QMA6100_write_reg(0x19, reg_19);

        } else if (int_map == HL_QMA6100_MAP_INT2) {
            if (tap_type & HL_QMA6100_TAP_QUARTER) {
                reg_1c |= 0x02;
                HL_QMA6100_write_reg(0x1c, reg_1c);
            }

            reg_1b |= tap_reg;
            HL_QMA6100_write_reg(0x1b, reg_1b);
        }
    } else {
        reg_16 &= (~tap_reg);
        HL_QMA6100_write_reg(0x16, reg_16);

        if (int_map == HL_QMA6100_MAP_INT1) {
            if (tap_type & HL_QMA6100_TAP_QUARTER) {
                reg_1a &= (~0x02);
                HL_QMA6100_write_reg(0x1a, reg_1a);
            }

            reg_19 &= (~tap_reg);
            HL_QMA6100_write_reg(0x19, reg_19);

        } else if (int_map == HL_QMA6100_MAP_INT2) {
            if (tap_type & HL_QMA6100_TAP_QUARTER) {
                reg_1c &= (~0x02);
                HL_QMA6100_write_reg(0x1c, reg_1c);
            }

            reg_1b &= (~tap_reg);
            HL_QMA6100_write_reg(0x1b, reg_1b);
        }
    }
}
#endif

/**
 * 
 * @brief 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
#if defined(HL_QMA6100_HAND_RAISE_DOWN)
static void HL_QMA6100_set_hand_up_down(int layout)
{
#if 1  //defined(QMA7981_SWAP_XY)
    unsigned char reg_0x42 = 0;
#endif
    unsigned char reg_0x1e  = 0;
    unsigned char reg_0x34  = 0;
    unsigned char yz_th_sel = 4;
    char          y_th      = -3;  //-2;				// -16 ~ 15
    unsigned char x_th      = 6;   // 0--7.5
    char          z_th      = 6;   // -8--7

#if 1  //defined(QMA7981_SWAP_XY)	// swap xy
    if (layout % 2) {
        HL_QMA6100_read_reg(0x42, &reg_0x42, 1);
        reg_0x42 |= 0x80;  // 0x42 bit 7 swap x and y
        HL_QMA6100_write_reg(0x42, reg_0x42);
    }
#endif

    if ((layout >= 0) && (layout <= 3)) {
        z_th = 3;
        if ((layout == 2) || (layout == 3))
            y_th = 3;
        else if ((layout == 0) || (layout == 1))
            y_th = -3;
    } else if ((layout >= 4) && (layout <= 7)) {
        z_th = -3;

        if ((layout == 6) || (layout == 7))
            y_th = 3;
        else if ((layout == 4) || (layout == 5))
            y_th = -3;
    }

    // 0x34 YZ_TH_SEL[7:5]	Y_TH[4:0], default 0x9d  (YZ_TH_SEL   4   9.0 m/s2 | Y_TH  -3  -3 m/s2)
    //qmaX981_writereg(0x34, 0x9d);	//|yz|>8 m/s2, y>-3 m/m2
    if ((y_th & 0x80)) {
        reg_0x34 |= yz_th_sel << 5;
        reg_0x34 |= (y_th & 0x0f) | 0x10;
        HL_QMA6100_write_reg(0x34, reg_0x34);
    } else {
        reg_0x34 |= yz_th_sel << 5;
        reg_0x34 |= y_th;
        HL_QMA6100_write_reg(0x34, reg_0x34);  //|yz|>8m/s2, y<3 m/m2
    }
    //Z_TH<7:4>: -8~7, LSB 1 (unit : m/s2)	X_TH<3:0>: 0~7.5, LSB 0.5 (unit : m/s2)
    //qmaX981_writereg(0x1e, 0x68);	//6 m/s2, 4 m/m2

    HL_QMA6100_write_reg(0x22, (0x19 | (0x03 << 6)));  // 12m/s2 , 0.5m/s2
    HL_QMA6100_write_reg(0x23, (0x7c | (0x03 >> 2)));
    //qmaX981_writereg(0x2a, (0x19|(0x02<<6)));			// 12m/s2 , 0.5m/s2
    //qmaX981_writereg(0x2b, (0x7c|(0x02)));
    HL_QMA6100_write_reg(0x25, 0x0e);  //50
    //qmaX981_readreg(0x1e, &reg_0x1e, 1);
    if ((z_th & 0x80)) {
        reg_0x1e |= (x_th & 0x0f);
        reg_0x1e |= ((z_th << 4) | 0x80);
        HL_QMA6100_write_reg(0x35, reg_0x1e);
    } else {
        reg_0x1e |= (x_th & 0x0f);
        reg_0x1e |= (z_th << 4);
        HL_QMA6100_write_reg(0x35, reg_0x1e);
    }
}

/**
 * 
 * @brief 
 * @param [in] int_map 
 * @param [in] enable 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static void HL_QMA6100_hand_raise_down(int32_t int_map, int32_t enable)
{
    uint8_t reg_16, reg_19, reg_1b;

    HL_QMA6100_set_hand_up_down(0);

    HL_QMA6100_read_reg(0x16, &reg_16, 1);
    HL_QMA6100_read_reg(0x19, &reg_19, 1);
    HL_QMA6100_read_reg(0x1b, &reg_1b, 1);

    // 0x24: RAISE_WAKE_TIMEOUT_TH<7:0>: Raise_wake_timeout_th[11:0] * ODR period = timeout count
    // 0x25: RAISE_WAKE_PERIOD<7:0>: Raise_wake_period[10:0] * ODR period = wake count
    // 0x26:
    // RAISE_MODE: 0:raise wake function, 1:ear-in function
    // RAISE_WAKE_PERIOD<10:8>: Raise_wake_period[10:0] * ODR period = wake count
    // RAISE_WAKE_TIMEOUT_TH<11:8>: Raise_wake_timeout_th[11:0] * ODR period = timeout count

    if (enable) {
        reg_16 |= (0x02);
        reg_19 |= (0x02);
        reg_1b |= (0x02);
        HL_QMA6100_write_reg(0x16, reg_16);
        if (int_map == HL_QMA6100_MAP_INT1)
            HL_QMA6100_write_reg(0x19, reg_19);
        else if (int_map == HL_QMA6100_MAP_INT2)
            HL_QMA6100_write_reg(0x1b, reg_1b);
    } else {
        reg_16 &= ~((0x02 | 0x04));
        reg_19 &= ~((0x02 | 0x04));
        reg_1b &= ~((0x02 | 0x04));
        HL_QMA6100_write_reg(0x16, reg_16);
        HL_QMA6100_write_reg(0x19, reg_19);
        HL_QMA6100_write_reg(0x1b, reg_1b);
    }
}
#endif

/**
 * 
 * @brief 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static void HL_QMA6100_irq_hdlr(void)
{
    uint8_t  ret = HL_QMA6100_FAIL;
    uint8_t  databuf[4];
    int32_t retry = 0;

    while ((ret == HL_QMA6100_FAIL) && (retry++ < 10)) {
        ret = HL_QMA6100_read_reg(HL_QMA6100_INT_STAT0, databuf, 4);
        if (ret == HL_QMA6100_SUCCESS) {
            break;
        }
    }
    if (ret == HL_QMA6100_FAIL) {
        HL_QMA6100_LOG("HL_QMA6100_irq_hdlr read status fail!\n");
        return;
    } else {
        HL_QMA6100_LOG("irq [0x%x 0x%x 0x%x 0x%x]\n", databuf[0], databuf[1], databuf[2], databuf[3]);
    }

#if defined(HL_QMA6100_DATA_READY)
    if (databuf[2] & 0x10) {
        HL_QMA6100_read_raw_xyz(g_QMA6100.raw);
        HL_QMA6100_LOG("drdy	%d	%d	%d\n", g_QMA6100.raw[0], g_QMA6100.raw[1], g_QMA6100.raw[2]);
    }
#endif
#if defined(HL_QMA6100_FIFO_FUNC)
    if (databuf[2] & 0x20) {
        HL_QMA6100_LOG("FIFO FULL\n");
        HL_QMA6100_read_fifo(HL_QMA6100_fifo_reg);
        HL_QMA6100_exe_fifo(HL_QMA6100_fifo_reg);
    }
    if (databuf[2] & 0x40) {
        //HL_QMA6100_LOG("FIFO WMK\n");
        HL_QMA6100_read_fifo(HL_QMA6100_fifo_reg);
        HL_QMA6100_exe_fifo(HL_QMA6100_fifo_reg);
    }
#endif
#if defined(HL_QMA6100_ANY_MOTION)
    if (databuf[0] & 0x07) {
        HL_QMA6100_LOG("any motion!\n");
    }
#if defined(HL_QMA6100_SIGNIFICANT_MOTION)
    if (databuf[1] & 0x01) {
        HL_QMA6100_LOG("significant motion!\n");
    }
#endif
#endif
#if defined(HL_QMA6100_NO_MOTION)
    if (databuf[0] & 0x80) {
        HL_QMA6100_LOG("no motion!\n");
    }
#endif
#if defined(HL_QMA6100_STEP_INT)
    if (databuf[1] & 0x08) {
        HL_QMA6100_LOG("step int!\n");
    }
#endif
#if defined(HL_QMA6100_SIGNIFICANT_STEP_INT)
    if (databuf[1] & 0x40) {
        HL_QMA6100_LOG("significant step int!\n");
    }
#endif
#if defined(HL_QMA6100_TAP_FUNC)
    if (databuf[1] & 0x80) {
        HL_QMA6100_LOG("SINGLE tap int!\n");
    }
    if (databuf[1] & 0x20) {
        HL_QMA6100_LOG("DOUBLE tap int!\n");
    }
    if (databuf[1] & 0x10) {
        HL_QMA6100_LOG("TRIPLE tap int!\n");
    }
    if (databuf[2] & 0x01) {
        HL_QMA6100_LOG("QUARTER tap int!\n");
    }
#endif
#if defined(HL_QMA6100_HAND_RAISE_DOWN)
    if (databuf[1] & 0x02) {
        HL_QMA6100_LOG("hand raise!\n");
    }
    if (databuf[1] & 0x04) {
        HL_QMA6100_LOG("hand down!\n");
    }
#endif
}

/**
 * 
 * @brief 
 * @param [in] data 
 * @return uint8_t 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static uint8_t HL_QMA6100_drdy_read_raw_xyz(int32_t* data)
{
    uint8_t  databuf[6] = { 0 };
    int16_t raw_data[3];
    uint8_t  drdy = 0;
    int32_t ret;

    while (!drdy) {
        ret = HL_QMA6100_read_reg(HL_QMA6100_XOUTL, databuf, 6);
        if (ret == HL_QMA6100_FAIL) {
            HL_QMA6100_ERR("read xyz error!!!");
            return 0;
        }
        drdy = (databuf[0] & 0x01) + (databuf[2] & 0x01) + (databuf[4] & 0x01);
        //HL_QMA6100_LOG("drdy 0x%d \n",drdy);
    }

    raw_data[0] = (int16_t)(((int16_t)(databuf[1] << 8)) | (databuf[0]));
    raw_data[1] = (int16_t)(((int16_t)(databuf[3] << 8)) | (databuf[2]));
    raw_data[2] = (int16_t)(((int16_t)(databuf[5] << 8)) | (databuf[4]));
    data[0]     = raw_data[0] >> 2;
    data[1]     = raw_data[1] >> 2;
    data[2]     = raw_data[2] >> 2;

    return HL_QMA6100_SUCCESS;
}

/**
 * 
 * @brief 
 * @param [in] data 
 * @return int32_t 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static int32_t HL_QMA6100_read_raw_xyz(int32_t* data)
{
    uint8_t  databuf[6] = { 0 };
    int16_t raw_data[3];
    int32_t ret;

    ret = HL_QMA6100_read_reg(HL_QMA6100_XOUTL, databuf, 6);
    if (ret == HL_QMA6100_FAIL) {
        HL_QMA6100_ERR("read xyz error!!!");
        return HL_QMA6100_FAIL;
    }

    raw_data[0] = (int16_t)(((int16_t)(databuf[1] << 8)) | (databuf[0]));
    raw_data[1] = (int16_t)(((int16_t)(databuf[3] << 8)) | (databuf[2]));
    raw_data[2] = (int16_t)(((int16_t)(databuf[5] << 8)) | (databuf[4]));
    data[0]     = raw_data[0] >> 2;
    data[1]     = raw_data[1] >> 2;
    data[2]     = raw_data[2] >> 2;

    return HL_QMA6100_SUCCESS;
}

/**
 * 
 * @brief 
 * @param [in] accData 
 * @return int32_t 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static int32_t HL_QMA6100_read_acc_xyz(int32_t* accData)
{
    int32_t ret;
    int32_t rawData[3];

    ret = HL_QMA6100_read_raw_xyz(rawData);
    if (ret == HL_QMA6100_SUCCESS) {
        accData[g_QMA6100.cvt.map[0]] = g_QMA6100.cvt.sign[0] * rawData[0];
        accData[g_QMA6100.cvt.map[1]] = g_QMA6100.cvt.sign[1] * rawData[1];
        accData[g_QMA6100.cvt.map[2]] = g_QMA6100.cvt.sign[2] * rawData[2];

        accData[0] = (accData[0] * GRAVITY_EARTH_1000) / (g_QMA6100.lsb_1g);  //GRAVITY_EARTH_1000
        accData[1] = (accData[1] * GRAVITY_EARTH_1000) / (g_QMA6100.lsb_1g);
        accData[2] = (accData[2] * GRAVITY_EARTH_1000) / (g_QMA6100.lsb_1g);
        
    }

    return ret;
}

/**
 * 
 * @brief 
 * @return int32_t 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static int32_t HL_QMA6100_soft_reset(void)
{
    uint8_t  reg_0x11 = 0;
    uint8_t  reg_0x33 = 0;
    uint32_t retry    = 0;

    HL_QMA6100_LOG("HL_QMA6100_soft_reset");
    HL_QMA6100_write_reg(0x36, 0xb6);    //软复位所有寄存器
    HL_QMA6100_delay(5);
    HL_QMA6100_write_reg(0x36, 0x00);    //软复位后，用户应写入0x00
    HL_QMA6100_delay(100);

    retry = 0;
    while (reg_0x11 != 0x84) {
        HL_QMA6100_write_reg(0x11, 0x84);  //开机后默认模式为待机模式，设置为活动模式
        HL_QMA6100_delay(2);
        HL_QMA6100_read_reg(0x11, &reg_0x11, 1);
        HL_QMA6100_LOG("confirm read 0x11 = 0x%x\n", reg_0x11);
        if (retry++ > 100){
            break;
        }    
    }

    // load otp 0x33是内部寄存器 按照demo去初始化 默认值0x05
    HL_QMA6100_write_reg(0x33, 0x08);  //开启NVM映射 
    HL_QMA6100_delay(5);

    retry = 0;
    while ((reg_0x33 & 0x05) != 0x05) {
        HL_QMA6100_read_reg(0x33, &reg_0x33, 1);
        HL_QMA6100_LOG("confirm read 0x33 = 0x%x\n", reg_0x33);
        HL_QMA6100_delay(2);
        if (retry++ > 100){
             break;
        }           
    }
    
    return 0;
}

/**
 * 
 * @brief 
 * @return int32_t 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static int32_t HL_QMA6100_initialize(void)
{
#if defined(HL_QMA6100_INT_LATCH)
    uint8_t reg_data[4];
#endif

    HL_QMA6100_soft_reset();

    HL_QMA6100_write_reg(0x4a, 0x20);
    HL_QMA6100_write_reg(0x50, 0x51);  //0x49  0x51
    HL_QMA6100_write_reg(0x56, 0x01);

    HL_QMA6100_set_range(HL_QMA6100_RANGE_4G);  //设置加速度量程
#if defined(HL_QMA6100_ODR_27)
    HL_QMA6100_write_reg(0x10, 0x06);
#elif defined(HL_QMA6100_ODR_55)
    HL_QMA6100_write_reg(0x10, 0x05); 
#elif defined(HL_QMA6100_ODR_100)
    HL_QMA6100_write_reg(0x10, 0x00);
#endif
    HL_QMA6100_write_reg(0x11, 0x84);   //进入活动模式

    HL_QMA6100_write_reg(0x4A, 0x28);   // force  I2C interface
    HL_QMA6100_delay(5);
    HL_QMA6100_write_reg(0x20, 0x05);   //上拉推挽，中断低
    HL_QMA6100_delay(5);
    HL_QMA6100_write_reg(0x5F, 0x80);
    HL_QMA6100_delay(5);
    HL_QMA6100_write_reg(0x5F, 0x00);
    HL_QMA6100_delay(5);

#if defined(HL_QMA6100_DATA_READY)
    HL_QMA6100_drdy_config(HL_QMA6100_MAP_INT1, HL_QMA6100_ENABLE);
#endif

#if defined(HL_QMA6100_FIFO_FUNC)
    HL_QMA6100_fifo_config(HL_QMA6100_MAP_INT1, HL_QMA6100_ENABLE);
#endif

#if defined(HL_QMA6100_STEPCOUNTER)
    HL_QMA6100_stepcounter_config(HL_QMA6100_ENABLE);
#if defined(HL_QMA6100_STEP_INT)
    HL_QMA6100_step_int_config(HL_QMA6100_MAP_INT1, HL_QMA6100_ENABLE);
#endif
#if defined(HL_QMA6100_SIGNIFICANT_STEP_INT)
    HL_QMA6100_sigstep_int_config(HL_QMA6100_MAP_INT1, HL_QMA6100_ENABLE);
#endif
#endif

#if defined(HL_QMA6100_ANY_MOTION)
    HL_QMA6100_anymotion_config(HL_QMA6100_MAP_INT1, HL_QMA6100_ENABLE);
#endif
#if defined(HL_QMA6100_NO_MOTION)
    HL_QMA6100_nomotion_config(HL_QMA6100_MAP_INT1, HL_QMA6100_ENABLE);
#endif

#if defined(HL_QMA6100_TAP_FUNC)
    HL_QMA6100_tap_config(HL_QMA6100_TAP_DOUBLE | HL_QMA6100_TAP_TRIPLE | HL_QMA6100_TAP_QUARTER, HL_QMA6100_MAP_INT1, HL_QMA6100_ENABLE);
#endif

#if defined(HL_QMA6100_HAND_RAISE_DOWN)
    HL_QMA6100_hand_raise_down(HL_QMA6100_MAP_INT1, HL_QMA6100_ENABLE);
#endif

#if defined(HL_QMA6100_INT_LATCH)
    HL_QMA6100_write_reg(0x21, 0x03);  // default 0x1c, step latch mode
    HL_QMA6100_read_reg(0x09, reg_data, 4);
    HL_QMA6100_LOG("read status=[0x%x 0x%x 0x%x 0x%x] \n", reg_data[0], reg_data[1], reg_data[2], reg_data[3]);
#endif
    ///HL_QMA6100_dump_reg();

    return HL_QMA6100_SUCCESS;
}

/**
 * 
 * @brief 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static void HL_QMA6100_power_down(void)
{
    HL_QMA6100_write_reg(0x11, 0x87);
}

/**
 * 
 * @brief 
 * @return int32_t 
 * @date 2022-09-26
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-26      <td>libo     <td>新建
 * </table>
 */
static int32_t HL_QMA6100_init(void)
{
    int32_t ret             = 0;
    uint8_t  slave_addr[2]  = {HL_QMA6100_I2C_SLAVE_ADDR, HL_QMA6100_I2C_SLAVE_ADDR2};
    uint8_t  index          = 0;

    for (index = 0; index < 2; index++) {
        g_QMA6100.chip_id = 0;
        g_QMA6100.slave   = slave_addr[index];
        g_QMA6100.chip_id = HL_QMA6100_chip_id();
        if ((g_QMA6100.chip_id == 0xfa) || ((g_QMA6100.chip_id & 0xF0) == 0x90)) {
            HL_QMA6100_LOG("HL_QMA6100 find \n");
            break;
        }
    }

#if defined(HL_QMA6100_FIX_i2c)
    HL_QMA6100_write_reg(0x20, 0x45);
    g_QMA6100.slave = HL_QMA6100_I2C_SLAVE_ADDR;
#endif

    if ((g_QMA6100.chip_id == 0xfa) || ((g_QMA6100.chip_id & 0xF0) == 0x90)){
        ret = HL_QMA6100_initialize();
    }else{
        ret = HL_QMA6100_FAIL;
    }
        
    //g_QMA6100.layout = 3;
    g_QMA6100.layout      = 0;
    g_QMA6100.cvt.map[0]  = qst_map[g_QMA6100.layout].map[0];
    g_QMA6100.cvt.map[1]  = qst_map[g_QMA6100.layout].map[1];
    g_QMA6100.cvt.map[2]  = qst_map[g_QMA6100.layout].map[2];
    g_QMA6100.cvt.sign[0] = qst_map[g_QMA6100.layout].sign[0];
    g_QMA6100.cvt.sign[1] = qst_map[g_QMA6100.layout].sign[1];
    g_QMA6100.cvt.sign[2] = qst_map[g_QMA6100.layout].sign[2];

    return ret;
}


uint8_t hl_drv_qma6100p_init(void)
{
    uint8_t ret = 0;
    ret = HL_QMA6100_init();
    rt_thread_mdelay(1000);  //初始化后不能立即获取 要等待寄存器设置完成这段时间
    return ret;
}


uint8_t hl_drv_qma6100p_deinit(void)
{
    if(HL_QMA6100_write_reg(0x36, 0xb6)){ //软复位所有寄存器 默认启动为待机模式
        return 1;
    }
    return 0;

}

/**
 * 
 * @brief 获取欧拉角（注：要在相对静止条件下获取）
 * @param [in] ptr 
 * @return uint8_t 
 * @date 2022-09-29
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-29      <td>libo     <td>新建
 * </table>
 */
static uint8_t HL_QMA6100P_get_Euler_Angle(void* ptr)
{
    if(ptr == NULL){
        rt_kprintf(" hl_drv_qma6100p_io_ctrl  param  error\n");
        return 1;
    }

    euler_angle_t* param = (euler_angle_t*)ptr;
    int32_t acc_data[3];
    
    if(HL_QMA6100_read_acc_xyz(acc_data)){
        return 1;
    }
    
    // if(HL_QMA6100_ABS(acc_data[0])+HL_QMA6100_ABS(acc_data[1])+HL_QMA6100_ABS(acc_data[2]) > 18000){  //加速度之和超过18000 被认为在非静止条件下 后续可根据需求更改
    //     rt_kprintf(" HL_QMA6100P_get_Euler_Angle  no static status!\n");
    //     return 2;
    // }

    float64_t av = sqrtf(acc_data[0]*acc_data[0]+acc_data[1]*acc_data[1]+acc_data[2]*acc_data[2]);
    float64_t pitch = asinf(-acc_data[1]/av)*R2D;
    float64_t roll = asinf(acc_data[0]/av)*R2D;

    param->pitch = (int32_t)pitch;
    param->roll = (int32_t)roll;
    param->z = acc_data[2];

    return 0;
}


uint8_t hl_drv_qma6100p_io_ctrl(uint8_t cmd, void * ptr, uint16_t len)
{
    if(ptr == NULL){
        rt_kprintf(" hl_drv_qma6100p_io_ctrl  param  error\n");
        return 1;
    }
    
    switch (cmd) {
        case QMA6100_GET_EULWER_ANGLE:
            if (HL_QMA6100P_get_Euler_Angle(ptr)) {
                rt_kprintf("QMA6100  get Euler Angle  fail\n");
                return -1;
            }
            break;
        default:
            break;
    }    
    return 0;
}





