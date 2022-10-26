/**
 * @file hl_drv_msm8909.c
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief 触摸屏驱动函数
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

#include <rtthread.h>
#include <rtdevice.h>
#include "hl_drv_msm8909.h"
#include "string.h"

#if 0

#define MSM8909_DEBUG

#ifdef MSM8909_DEBUG
#define msm_printf(...) rt_kprintf(__VA_ARGS__)
#else
#define msm_printf(...)
#endif

static ts_zinitix_point_info_t   touch_info;//
static struct rt_i2c_bus_device* i2c_bus = RT_NULL; /* I2C总线设备句柄 */
static rt_device_t touch_dev = RT_NULL;

const static struct rt_device_ops touch_ops =
{
    hl_drv_msm8909_init,
    RT_NULL,
    RT_NULL,
    hl_drv_get_touch_info,
    RT_NULL,
    RT_NULL,
};

/**
 * 
 * @brief 初始化注册触屏设备
 * @return int 
 * @date 2022-10-14
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-14      <td>dujunjie     <td>新建
 * </table>
 */
static int touch_device_msm8909_init(void)
{
    rt_err_t ret;
    touch_dev = rt_device_create(RT_Device_Class_Touch, RT_NULL);
    if(touch_dev == RT_NULL){
        msm_printf("Touch device msm8909 create fail !\n");
        return HL_FAILED;
    }
    touch_dev->ops = &touch_ops;
    ret = rt_device_register(touch_dev, "MSM8909", RT_DEVICE_FLAG_RDONLY);
    if(ret != RT_EOK){
        msm_printf("Touch device msm8909 register fail !\n");
        return HL_FAILED;
    }
    return HL_SUCCESS;
}


/**
 * 
 * @brief 写命令函数
 * @param [in] bus IIC总线
 * @param [in] cmd 命令
 * @return rt_err_t 0成功，1失败
 * @date 2022-10-13
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-13      <td>dujunjie     <td>新建
 * </table>
 */
static rt_err_t hl_i2c_write_cmd(struct rt_i2c_bus_device* bus, rt_uint16_t cmd)
{
    rt_err_t          ret;
    rt_uint8_t        buf[4];
    struct rt_i2c_msg msgs[2];

    buf[0] = (cmd & 0x00FF);  //低位在前
    buf[1] = (cmd >> 8);      //

    msgs[0].addr  = MSM8909_DEVICE_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = buf;
    msgs[0].len   = 2;

    /* 调用I2C设备接口传输数据 */
    ret = rt_i2c_transfer(bus, msgs, 1);
    msm_printf("write cmd ret = %d \n", ret);
    if (ret == 1)
        return HL_SUCCESS;
    else
        return HL_FAILED;
}

/**
 * 
 * @brief 写寄存器数据函数
 * @param [in] bus IIC总线
 * @param [in] reg 寄存器
 * @param [in] data 数据
 * @return rt_err_t 0成功，1失败
 * @date 2022-10-13
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-13      <td>dujunjie     <td>新建
 * </table>
 */
static rt_err_t hl_i2c_write_reg(struct rt_i2c_bus_device* bus, rt_uint16_t reg, rt_uint16_t data)
{
    rt_err_t          ret;
    rt_uint8_t        buf[4];
    struct rt_i2c_msg msgs[2];

    buf[0] = (reg & 0x00FF);   ////低位在前
    buf[1] = (reg >> 8);       //
    buf[2] = (data & 0x00FF);  // data
    buf[3] = (data >> 8);      //

    msgs[0].addr  = MSM8909_DEVICE_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = buf;
    msgs[0].len   = 4;

    /* 调用I2C设备接口传输数据 */
    msm_printf("%s : buf[2] = %X buf[3] = %X \n", __FUNCTION__, buf[2], buf[3]);
    ret = rt_i2c_transfer(bus, msgs, 1);
    msm_printf("write reg ret = %d \n", ret);
    if (ret == 1)
        return HL_SUCCESS;
    else
        return HL_FAILED;
}

/**
 * 
 * @brief 读数据函数
 * @param [in] bus IIC总线
 * @param [in] reg 寄存器
 * @param [in] rbuf 消息缓存
 * @param [in] buf_len 需要读取消息数量
 * @return rt_err_t 0成功，1失败
 * @date 2022-10-13
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-13      <td>dujunjie     <td>新建
 * </table>
 */
static rt_err_t hl_i2c_read_reg(struct rt_i2c_bus_device* bus, rt_uint16_t reg, rt_uint8_t* rbuf, rt_int8_t buf_len)
{
    rt_err_t          ret     = 0;
    rt_uint8_t        buf[16] = { 0 };
    struct rt_i2c_msg msgs[2];

    if (buf_len > 8) {
        return HL_FAILED;
    }

    buf[0] = (reg & 0x00FF);  // reg
    buf[1] = (reg >> 8);      //

    msgs[0].addr  = MSM8909_DEVICE_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = buf;
    msgs[0].len   = 2;

    msgs[1].addr  = MSM8909_DEVICE_ADDRESS;
    msgs[1].flags = RT_I2C_RD;
    // msgs[1].buf   = &buf[2];
    msgs[1].buf = rbuf;
    msgs[1].len = buf_len;

    ret = rt_i2c_transfer(bus, msgs, 2);
    msm_printf("read ret = %d \n", ret);
    /* 调用I2C设备接口传输数据 */
    if (ret == 2) {
        // rbuf[0] = ((buf[2] << 8) + buf[3]);
        return HL_SUCCESS;
    } else
        return HL_FAILED;
}

/**
 * 
 * @brief 读取数据函数
 * @date 2022-10-13
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details //应放在中断中触发
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-13      <td>dujunjie     <td>新建
 * </table>
 */
rt_size_t hl_drv_get_touch_info(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    uint8_t  i;
    uint16_t x, y;
    msm_printf("Detection touch !\n");
#ifdef TPD_HIGH_SPEED_DMA
    //I2C dma mode 可以一次读很多个字节，需要平台支持，Android 平台都支持
    hl_i2c_read_reg(i2c_bus, ZINITIX_POINT_STATUS_REG, (uint8_t*)&(touch_info), sizeof(struct _ts_zinitix_point_info));
#else //非dma mode 平台一般只允许最多一次读8 个字节
    //多点触控第一个点的信息
    hl_i2c_read_reg(i2c_bus, ZINITIX_POINT_STATUS_REG, (uint8_t*)(&touch_info), 8);
    hl_i2c_read_reg(i2c_bus, ZINITIX_POINT_STATUS_REG + 4, ((uint8_t*)&touch_info + 8), 2);
#if MAX_SUPPORTED_FINGER_NUM > 1
    //后续点的信息
    for (i = 1; i < MAX_SUPPORTED_FINGER_NUM; i++) {
        if (zinitix_bit_test(touch_info.event_flag, i)) {//
            hl_i2c_read_reg(i2c_bus, ZINITIX_POINT_STATUS_REG + 2 + (i * 4), (uint8_t*)(&touch_info.coord[i]),
                         sizeof(ts_zinitix_point_info_t));
        }
    }
#endif
#endif
    //down up 事件的判断
    for (i = 0; i < MAX_SUPPORTED_FINGER_NUM; i++) {
        if (zinitix_bit_test(touch_info.coord[i].sub_status, SUB_BIT_DOWN)//按下事件
            || zinitix_bit_test(touch_info.coord[i].sub_status, SUB_BIT_MOVE)//移动事件
            || zinitix_bit_test(touch_info.coord[i].sub_status, SUB_BIT_EXIST)) {//松手事件
            x = touch_info.coord[i].x;
            y = touch_info.coord[i].y;
            msm_printf("finger dwon [%02d] x = %d, y = %d \r\n", i, x, y);
            // Send_down_event(i,x,y);/*平台上报down 事件函数*/
        } else if (zinitix_bit_test(touch_info.coord[i].sub_status, SUB_BIT_UP)) {
            msm_printf("finger [%02d] up \r\n", i);
            // Send_up_event(i);/*平台上报up 事件函数*/
        } else {
            memset(&touch_info.coord[i], 0, sizeof(ts_zinitix_coord_t));
        }
    }
    memcpy((uint8_t *)buffer,(uint8_t *)touch_info.coord,(sizeof(ts_zinitix_coord_t) * MAX_SUPPORTED_FINGER_NUM));
    //clear 中断，clear 后中断IO会被拉高
    hl_i2c_write_cmd(i2c_bus, ZINITIX_CLEAR_INT_STATUS_CMD);
    return (sizeof(ts_zinitix_coord_t) * MAX_SUPPORTED_FINGER_NUM);
}

/**
 * 
 * @brief 触摸初始化函数
 * @return int 0成功，1失败
 * @date 2022-10-13
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-13      <td>dujunjie     <td>新建
 * </table>
 */
rt_err_t hl_drv_msm8909_init(rt_device_t dev)
{
    uint8_t  i;
    uint16_t firmware_version = 0, minor_firmware_version = 0;
    /* 查找I2C总线设备，获取I2C总线设备句柄 */
    i2c_bus = (struct rt_i2c_bus_device*)rt_device_find(MSM8909_IIC_NAME);
    hl_i2c_write_cmd(i2c_bus, 0x0000);
    rt_thread_mdelay(10);

    for (i = 0; i < 10; i++) {
        if (hl_i2c_write_cmd(i2c_bus, ZINITIX_SWRESET_CMD) == HL_SUCCESS)
            break; /*return 0 mean write success then break*/
        rt_thread_mdelay(10);
    }

    if(i == 10){
        msm_printf("touch init fail !\n");
        return HL_FAILED;
    }
    //固件主版本号u16 firmware_version;
    hl_i2c_read_reg(i2c_bus, ZINITIX_FIRMWARE_VERSION, (uint8_t *)&firmware_version, 2);
    //固件次版本号u16 minor_firmware_version;
    hl_i2c_read_reg(i2c_bus, ZINITIX_MINOR_FW_VERSION, (uint8_t *)&minor_firmware_version, 2);
    msm_printf("firmware_version = %X minor_firmware_version = %X\n", firmware_version, minor_firmware_version);
    //固件寄存器版本号u16 reg_data_version;
    // hl_i2c_read_reg(i2c_bus, ZINITIX_DATA_VERSION_REG,(uint8_t *)&reg_data_version, 1) ;
    hl_i2c_write_reg(i2c_bus, ZINITIX_INITIAL_TOUCH_MODE, (uint16_t)(TOUCH_POINT_MODE));
    hl_i2c_write_reg(i2c_bus, ZINITIX_TOUCH_MODE, (uint16_t)(TOUCH_POINT_MODE));
    hl_i2c_write_reg(i2c_bus, ZINITIX_SUPPORTED_FINGER_NUM, (uint16_t)(MAX_SUPPORTED_FINGER_NUM));

    hl_i2c_write_reg(i2c_bus, ZINITIX_X_RESOLUTION, (uint16_t)(TPD_RES_MAX_X));
    hl_i2c_write_reg(i2c_bus, ZINITIX_Y_RESOLUTION, (uint16_t)(TPD_RES_MAX_Y));
    hl_i2c_write_cmd(i2c_bus, ZINITIX_CALIBRATE_CMD);
    for (i = 0; i < 10; i++) {
        hl_i2c_write_cmd(i2c_bus, ZINITIX_CLEAR_INT_STATUS_CMD);
        rt_thread_mdelay(1);
    }
    return HL_SUCCESS;
}

INIT_DEVICE_EXPORT(touch_device_msm8909_init);

#endif
