/**
 * @file hl_drv_ft3169.c
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief FT3169驱动源文件
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

/*****************************************************************************
* Included header files
*****************************************************************************/
#include <rtthread.h>
#include <rtdevice.h>
#include <touch.h>
#include "hl_drv_ft3169.h"
#include "hal_pinctrl.h"

/*****************************************************************************
* Private constant and macro definitions using #define
*****************************************************************************/

#define FT3169_DEBUG

#ifdef FT3169_DEBUG
#define ft_printf(...) rt_kprintf(__VA_ARGS__)
#else
#define ft_printf(...)
#endif

#define FTS_CMD_START_DELAY 12

/* register address */
#define FTS_REG_WORKMODE 0x00
#define FTS_REG_WORKMODE_FACTORY_VALUE 0x40
#define FTS_REG_WORKMODE_SCAN_VALUE 0xC0
#define FTS_REG_FLOW_WORK_CNT 0x91
#define FTS_REG_POWER_MODE 0xA5
#define FTS_REG_GESTURE_EN 0xD0
#define FTS_REG_GESTURE_ENABLE 0x01
#define FTS_REG_GESTURE_OUTPUT_ADDRESS 0xD3

/*Max point numbers of gesture trace*/
#define MAX_POINTS_GESTURE_TRACE 6
/*Length of gesture information*/
#define MAX_LEN_GESTURE_INFO (MAX_POINTS_GESTURE_TRACE * 4 + 2)

/*Max point numbers of touch trace*/
#define MAX_POINTS_TOUCH_TRACE 2
/*Length of touch information*/
#define MAX_LEN_TOUCH_INFO (MAX_POINTS_TOUCH_TRACE * 6 + 2)

/*Max touch points that touch controller supports*/
#define FTS_MAX_POINTS_SUPPORT 10

#define CURRENT_TOUCH_POINT_NUM 1
#define CURRENT_TOUCH_X_RANGE 10
#define CURRENT_TOUCH_Y_RANGE 10
#define CURRENT_TOUCH_DEVICE_NAME "FT3169"
#define CURRENT_TOUCH_IRQ_PIN GPIO1_C7
#define CURRENT_TOUCH_IRQ_MODE PIN_MODE_INPUT_PULLUP

#define CURRENT_TOUCH_RESET_PIN GPIO1_C5
#define CURRENT_TOUCH_RESET_MODE PIN_MODE_OUTPUT
#define CURRENT_TOUCH_RESET_PIN_H PIN_HIGH
#define CURRENT_TOUCH_RESET_PIN_L PIN_LOW

// #define CURRENT_TOUCH_RESET_PIN GPIO1_C5
// #define CURRENT_TOUCH_RESET_MODE PIN_MODE_OUTPUT
// #define CURRENT_TOUCH_RESET_PIN_H PIN_HIGH
// #define CURRENT_TOUCH_RESET_PIN_L PIN_LOW

// static rt_device_t touch_dev  = RT_NULL;
static rt_touch_t touch_hand = RT_NULL;
static rt_device_t         touch_dev = RT_NULL;
static struct rt_i2c_bus_device* i2c_bus = RT_NULL; /* I2C总线设备句柄 */
static int                       fts_ts_suspend(void);
// static rt_err_t                  fts_ts_init(rt_device_t dev);
static rt_err_t                  fts_touch_irq_callback(rt_touch_t touch);
static rt_err_t                  hl_drv_ft3169_io_ctrl(struct rt_touch_device* touch, int cmd, void* arg);
static rt_size_t                 fts_touch_process(struct rt_touch_device* touch, void* buf, rt_size_t touch_num);

/*****************************************************************************
* Private variables/functions
*****************************************************************************/
static struct fts_ts_data _fts_data = {
    .suspended       = 0,
    .gesture_support = FTS_GESTURE_EN,
    .esd_support     = 1,
};

// const static struct rt_device_ops touch_ops = {
//     .init = fts_ts_init,
//     .close = RT_NULL,
//     .control = RT_NULL,
//     .open = RT_NULL,
//     .read = RT_NULL,
//     .write = RT_NULL,
// };

const static struct rt_touch_ops touch_ops_top = {
    .touch_control   = hl_drv_ft3169_io_ctrl,
    .touch_readpoint = fts_touch_process,
};

const static struct rt_touch_info touch_info = {
    .point_num = CURRENT_TOUCH_POINT_NUM,
    .range_x   = CURRENT_TOUCH_X_RANGE,
    .range_y   = CURRENT_TOUCH_Y_RANGE,
    .type      = RT_TOUCH_TYPE_CAPACITANCE,
    .vendor    = RT_TOUCH_VENDOR_FT,
};

const struct rt_touch_config touch_config = {
    .dev_name     = CURRENT_TOUCH_DEVICE_NAME,
    .irq_pin.mode = CURRENT_TOUCH_IRQ_MODE,
    .irq_pin.pin  = CURRENT_TOUCH_IRQ_PIN,
    .user_data    = RT_NULL,
};

/*****************************************************************************
* Global variable or extern global variabls/functions
*****************************************************************************/
struct fts_ts_data* fts_data = &_fts_data;

/*delay, unit: millisecond */
static void fts_msleep(unsigned long msec)
{
    rt_thread_mdelay(msec);
}

/**
 * 
 * @brief 写IIC操作函数
 * @param [in] bus IIC总线
 * @param [in] reg 寄存器
 * @param [in] wbuf 数据
 * @param [in] datalen 数据长度
 * @return rt_err_t 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
static rt_err_t hl_i2c_write_reg(struct rt_i2c_bus_device* bus, rt_uint8_t reg, rt_uint8_t* wbuf, rt_uint16_t datalen)
{
    rt_uint8_t        buf[256];
    struct rt_i2c_msg msgs[2];

    buf[0] = reg;
    memcpy(&buf[1], wbuf, datalen);

    msgs[0].addr  = FT3169_DEVICE_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = buf;
    msgs[0].len   = datalen + 1;

    // 调用I2C设备接口传输数据
    if (rt_i2c_transfer(bus, msgs, 1) == 1)
        return HL_SUCCESS;
    else
        return HL_FAILED;
}

/**
 * 
 * @brief 读IIC操作函数
 * @param [in] bus 总线
 * @param [in] reg 寄存器
 * @param [in] rbuf 数据
 * @param [in] datalen 数据长度
 * @return rt_err_t 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
static rt_err_t hl_i2c_read_reg(struct rt_i2c_bus_device* bus, rt_uint8_t reg, rt_uint8_t* rbuf, rt_uint16_t datalen)
{
    struct rt_i2c_msg msgs[2];

    msgs[0].addr  = FT3169_DEVICE_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &reg;
    msgs[0].len   = 1;

    msgs[1].addr  = FT3169_DEVICE_ADDRESS;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = rbuf;
    msgs[1].len   = datalen;

    // 调用I2C设备接口传输数据
    if (rt_i2c_transfer(bus, msgs, 2) == 2) {
        return HL_SUCCESS;
    } else
        return HL_FAILED;
}

/**
 * 
 * @brief 硬件复位操作
 * @param [in] msec 复位保持时间
 * @return int 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
static int fts_hw_reset(uint32_t msec)
{
    /*firsty. set reset_pin low, and then delay 10ms*/
    /*secondly. set reset_pin high, and then delay 200ms*/
    rt_pin_mode(CURRENT_TOUCH_RESET_PIN, CURRENT_TOUCH_RESET_MODE);
    rt_pin_write(CURRENT_TOUCH_RESET_PIN, CURRENT_TOUCH_RESET_PIN_L);
    fts_msleep(10);
    rt_pin_mode(CURRENT_TOUCH_RESET_PIN, CURRENT_TOUCH_RESET_MODE);
    rt_pin_write(CURRENT_TOUCH_RESET_PIN, CURRENT_TOUCH_RESET_PIN_H);
    fts_msleep(msec);
    return HL_SUCCESS;
}

/**
 * 
 * @brief 初始化IIC
 * @return int 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
static int platform_i2c_init(void)
{
    /*Initialize I2C bus, you should implement it based on your platform*/
    i2c_bus = (struct rt_i2c_bus_device*)rt_device_find(FT3169_IIC_NAME);
    ft_printf("ft3169 init !\n");

    if (i2c_bus == RT_NULL) {
        ft_printf("can't find %s device!\n", FT3169_IIC_NAME);
        return HL_FAILED;
    }
    return HL_SUCCESS;
}

/**
 * 
 * @brief 初始化硬件复位引脚
 * @return int 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
static int platform_reset_pin_cfg(void)
{
    /*Initialize reset_pin,  you should implement it based on your platform*/

    /*firstly,set the reset_pin to output mode*/
    /*secondly,set the reset_pin to low */
    fts_hw_reset(200);
    return HL_SUCCESS;
}

/**
 * 
 * @brief 触摸事件中断引脚初始化
 * @return int 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
static int platform_interrupt_gpio_init(void)
{
    /*Initialize gpio interrupt , and the corresponding interrupt function is fts_gpio_interrupt_handler,
    you should implement it based on your platform*/

    /*firstly,set int_pin to input mode with pull-up*/
    /*secondly,and set int_pin's trigger mode to falling edge trigger */

    return HL_SUCCESS;
}

/**
 * 
 * @brief 开电源
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
static void fts_power_on(void)
{
    /*refer to ic datasheet*/
}

/**
 * 
 * @brief 初始化一个定时器
 * @return int 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 需要一秒钟轮询一次 fts_timer_interrupt_handler 函数
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
// static int platform_interrupt_timer_init(void)
// {

//     /*Initialize timer and set to interrupt mode which period is 1 second,
//     and the corresponding interrupt function is fts_timer_interrupt_handler,
//     you should implement it based on your platform*/
//     return HL_SUCCESS;
// }

/**
 * 
 * @brief 写触摸屏寄存器函数
 * @param [in] addr 寄存器地址
 * @param [in] data 数据
 * @param [in] datalen 数据长度（<256）
 * @return int 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
static int fts_write(uint8_t addr, uint8_t* data, uint16_t datalen)
{
    /*TODO based your platform*/
    int ret = 0;
    // uint8_t  txbuf[256] = { 0 };
    // uint16_t txlen      = 0;
    int i = 0;

    if (datalen >= 256) {
        FTS_ERROR("txlen(%d) fails", datalen);
        return HL_FAILED;
    }

    /*call hl_i2c_write_reg function to transfer I2C package to TP controller
     *hl_i2c_write_reg() is different for different platform, based on your platform.
     */
    for (i = 0; i < 3; i++) {
        ret = hl_i2c_write_reg(i2c_bus, addr, data, datalen);
        if (ret == HL_FAILED) {
            // FTS_ERROR("hl_i2c_write_reg(%d) fails,ret:%d,retry:%d", addr, ret, i);
            continue;
        } else {
            ret = HL_SUCCESS;
            break;
        }
    }
    // if (ret == HL_FAILED) {
    //     FTS_ERROR("hl_i2c_write_reg(%d) fails,ret:%d,retry:%d", addr, ret, i);
    // }
    return ret;
}

/**
 * 
 * @brief 读触摸屏寄存器函数
 * @param [in] addr 寄存器地址
 * @param [in] data 缓存数组
 * @param [in] datalen 数据长度
 * @return int 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
static int fts_read(uint8_t addr, uint8_t* data, uint16_t datalen)
{
    /*TODO based your platform*/
    int ret = 0;
    int i   = 0;

    if (!data || !datalen) {
        FTS_ERROR("data is null, or datalen is 0");
        return HL_FAILED;
    }

    for (i = 0; i < 3; i++) {
        ret = hl_i2c_read_reg(i2c_bus, addr, data, datalen);
        if (ret == HL_FAILED) {
            // FTS_ERROR("hl_i2c_read_reg fails,ret:%d,retry:%d,i:%d,data[0]:%x", addr, ret, i, data[0]);
            continue;
        } else {
            ret = HL_SUCCESS;
            break;
        }
    }

    // if (ret == HL_FAILED) {
    //     FTS_ERROR("hl_i2c_read_reg fails,ret:%d,retry:%d,i:%d,data[0]:%x", addr, ret, i, data[0]);
    // }

    return ret;
}

/**
 * 
 * @brief 写寄存器函数
 * @param [in] addr 寄存器地址
 * @param [in] val 值
 * @return int 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 只能写一字节
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
static int fts_write_reg(uint8_t addr, uint8_t val)
{
    return fts_write(addr, &val, 1);
}

/**
 * 
 * @brief 读寄存器函数
 * @param [in] addr 寄存器地址
 * @param [in] val 缓存
 * @return int 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 只能读一字节
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
int fts_read_reg(uint8_t addr, uint8_t* val)
{
    return fts_read(addr, val, 1);
}

/**
 * 
 * @brief 检测触摸屏设备ID
 * @return int 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
static int fts_check_id(void)
{
    int ret = 0;
    // int     cnt        = 0;
    uint8_t chip_id[2] = { 0 };

    /*delay 200ms,wait fw*/
    fts_msleep(200);

    /*get chip id*/
    fts_read_reg(FTS_REG_CHIP_ID, &chip_id[0]);
    fts_read_reg(FTS_REG_CHIP_ID2, &chip_id[1]);
    if ((FTS_CHIP_IDH == chip_id[0]) && (FTS_CHIP_IDL == chip_id[1])) {
        FTS_INFO("get ic information, chip id = 0x%02x%02x", chip_id[0], chip_id[1]);
        return HL_SUCCESS;
    }

    /*get boot id*/
    FTS_INFO("fw is invalid, need read boot id\t0x%x%x", chip_id[0], chip_id[1]);
    //fts_hw_reset(15);
    ret = fts_write_reg(0x55, 0xAA);
    if (ret == HL_FAILED) {
        FTS_ERROR("start cmd write fail");
        return ret;
    }

    fts_msleep(FTS_CMD_START_DELAY);
    ret = fts_read(FTS_CMD_READ_ID, chip_id, 2);
    if ((ret == HL_SUCCESS) && ((FTS_CHIP_IDH == chip_id[0]) && (FTS_CHIP_IDL == chip_id[1]))) {
        FTS_INFO("get ic information, boot id = 0x%02x%02x", chip_id[0], chip_id[1]);
        ret = HL_SUCCESS;
    } else {
        FTS_ERROR("read boot id fail,read:0x%02x%02x", chip_id[0], chip_id[1]);
        return HL_FAILED;
    }

    return ret;
}

/**
 * 
 * @brief ESD防护函数，应该一秒调用一次
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
static void fts_esdcheck_process(void)
{
    uint8_t        reg_value          = 0;
    static uint8_t flow_work_hold_cnt = 0;
    static uint8_t flow_work_cnt_last = 0;
    FTS_DEBUG("Check esd \n");
    /* 1. check power state, if suspend, no need check esd */
    if (fts_data->suspended == 1) {
        FTS_DEBUG("In suspend, not check esd");
        /* because in suspend state, when upgrade FW, will
         * active ESD check(active = 1); when resume, don't check ESD again
         */
        return;
    }

    /* 2. In factory mode, can't check esd */
    fts_read_reg(FTS_REG_WORKMODE, &reg_value);
    if ((reg_value == FTS_REG_WORKMODE_FACTORY_VALUE) || (reg_value == FTS_REG_WORKMODE_SCAN_VALUE)) {
        FTS_DEBUG("in factory mode(%x), no check esd", reg_value);
        return;
    }

    /* 3. get Flow work cnt: 0x91 If no change for 5 times, then ESD and reset */
    fts_read_reg(FTS_REG_FLOW_WORK_CNT, &reg_value);
    if (flow_work_cnt_last == reg_value)
        flow_work_hold_cnt++;
    else
        flow_work_hold_cnt = 0;

    flow_work_cnt_last = reg_value;

    /* 4. If need hardware reset, then handle it here */
    if (flow_work_hold_cnt >= 5) {
        FTS_DEBUG("ESD, Hardware Reset");
        flow_work_hold_cnt = 0;
        fts_hw_reset(200);
    }
}

/**
 * 
 * @brief 手势判断函数
 * @return int 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 触发中断时，触摸屏处于休眠模式才能进行手势读取操作
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
// static int fts_gesture_process(void)
// {
//     int     ret                       = 0;
//     uint8_t i                         = 0;
//     uint8_t base                      = 0;
//     uint8_t regaddr                   = 0;
//     uint8_t value                     = 0xFF;
//     uint8_t buf[MAX_LEN_GESTURE_INFO] = { 0 };
//     uint8_t gesture_id                = 0;

//     /*Read a byte from register 0xD0 to confirm gesture function in FW is enabled*/
//     ret = fts_read_reg(FTS_REG_GESTURE_EN, &value);
//     if ((ret < 0) || (value != FTS_REG_GESTURE_ENABLE)) {
//         FTS_DEBUG("gesture isn't enable in fw, don't process gesture");
//         return 1;
//     }

//     /*Read 26 bytes from register 0xD3 to get gesture information*/
//     regaddr = FTS_REG_GESTURE_OUTPUT_ADDRESS;
//     ret     = fts_read(regaddr, buf, MAX_LEN_GESTURE_INFO);
//     if (ret < 0) {
//         FTS_DEBUG("read gesture information from reg0xD3 fails");
//         return ret;
//     }

//     /*get gesture_id, and the gestrue_id table provided by our technicians */
//     gesture_id = buf[0];

//     /* Now you have parsed the gesture information, you can recognise the gesture type based on gesture id.
//      * You can do anything you want to do, for example,
//      *     gesture id 0x24, so the gesture type id "Double Tap", now you can inform system to wake up
//      *     from gesture mode.
//      */

//     /*TODO...(report gesture to system)*/

//     return 0;
// }

/**
 * 
 * @brief 打印从寄存器地址0x01读取的触摸缓冲区数据
 * @param [in] buf 
 * @param [in] buflen 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
static void log_touch_buf(uint8_t* buf, uint32_t buflen)
{
    int  i           = 0;
    int  count       = 0;
    char tmpbuf[512] = { 0 };

    for (i = 0; i < buflen; i++) {
        count += snprintf(tmpbuf + count, 1024 - count, "%02X,", buf[i]);
        if (count >= 1024)
            break;
    }
    FTS_DEBUG("point buffer:%s", tmpbuf);
}

/**
 * 
 * @brief 打印此中断的触点信息
 * @param [in] events 触点信息
 * @param [in] event_nums 触点数量
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
static void log_touch_info(struct fts_ts_event* events, uint8_t event_nums)
{
    uint8_t i = 0;

    for (i = 0; i < event_nums; i++) {
        FTS_DEBUG("[%d][%d][%d,%d,%d]%d", events[i].id, events[i].type, events[i].x, events[i].y, events[i].p,
                  events[i].area);
    }
}

/**
 * 
 * @brief 该函数用于读取和解析接触点信息
 * @return int 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 当系统处于显示(正常)状态时，在gpio中断处理程序中调用
 * @note 也可以尝试使用轮询方式
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
static rt_size_t fts_touch_process(struct rt_touch_device* touch, void* buf, rt_size_t touch_num)
{
    int                  ret     = 0;
    uint8_t              i       = 0;
    uint8_t              base    = 0;
    uint8_t              regaddr = 0x01;
    uint8_t              rbuf[MAX_LEN_TOUCH_INFO]; /*A maximum of two points are supported*/
    uint8_t              point_num        = 0;
    uint8_t              touch_event_nums = 0;
    uint8_t              point_id         = 0;
    struct fts_ts_event  events[MAX_POINTS_TOUCH_TRACE]; /* multi-touch */
    struct fts_ts_event* ptr = (struct fts_ts_event*)buf;
    /*read touch information from reg0x01*/
    ret = fts_read(regaddr, rbuf, MAX_LEN_TOUCH_INFO);
    if (ret == HL_FAILED) {
        FTS_DEBUG("Read touch information from reg0x01 fails");
        return ret;
    }

    /*print touch buffer, for debug usage*/
    // log_touch_buf(rbuf, MAX_LEN_TOUCH_INFO);

    if ((rbuf[1] == 0xFF) && (rbuf[2] == 0xFF) && (rbuf[3] == 0xFF)) {
        FTS_INFO("FW initialization, need recovery");
        if (fts_data->gesture_support && fts_data->suspended)
            fts_write_reg(FTS_REG_GESTURE_EN, FTS_REG_GESTURE_ENABLE);
    }

    /*parse touch information based on register map*/
    // memset(events, 0xFF, sizeof(struct fts_ts_event) * FTS_MAX_POINTS_SUPPORT);
    point_num = rbuf[1] & 0x0F;
    if (point_num > FTS_MAX_POINTS_SUPPORT) {
        FTS_DEBUG("invalid point_num(%d)", point_num);
        return HL_FAILED;
    }

    for (i = 0; i < MAX_POINTS_TOUCH_TRACE; i++) {
        base     = 2 + i * 6;
        point_id = rbuf[base + 2] >> 4;
        if (point_id >= MAX_POINTS_TOUCH_TRACE) {
            break;
        }

        events[i].x    = ((rbuf[base] & 0x0F) << 8) + rbuf[base + 1];
        events[i].y    = ((rbuf[base + 2] & 0x0F) << 8) + rbuf[base + 3];
        events[i].id   = point_id;
        events[i].type = (rbuf[base] >> 6) & 0x03;
        events[i].p    = rbuf[base + 4];
        events[i].area = rbuf[base + 5];
        if (((events[i].type == 0) || (events[i].type == 2)) && (point_num == 0)) {
            FTS_DEBUG("abnormal touch data from fw");
            return HL_FAILED;
        }

        touch_event_nums++;
    }

    if (touch_event_nums == 0) {
        FTS_DEBUG("no touch point information(%02x)", rbuf[1]);
        return HL_FAILED;
    }

    /*print touch information*/
    log_touch_info(events, touch_event_nums);

    /*Now you have get the touch information, you can report anything(X/Y coordinates...) you want to system*/
    /*TODO...(report touch information to system)*/
    /*Below sample code is a pseudo code*/
    for (i = 0; i < touch_event_nums; i++) {
        if ((events[i].type == 0) || (events[i].type == 2)) {
            /* The event of point(point id is events[i].id) is down event, the finger of this id stands for is
             * pressing on the screen.*/
            /*TODO...(down event)*/
            memcpy(ptr, &events[i], sizeof(struct fts_ts_event));
        } else {
            /*TODO...(up event)*/
            memcpy(ptr, &events[i], sizeof(struct fts_ts_event));
        }
    }

    return HL_SUCCESS;
}

// static rt_err_t fts_touch_irq_callback(rt_touch_t touch)
// {
//     ft_printf("%s \n", __FUNCTION__);
//     return 0;
// }

// static rt_err_t fts_touch_rx_indicate_callback(rt_device_t dev, rt_size_t size)
// {
// if (fts_data->gesture_support && fts_data->suspended) {
//     /*if gesture is enabled, interrupt handler should process gesture at first*/
//     ret = fts_gesture_process();
//     if (ret == 0) {
//         FTS_DEBUG("success to process gesture.");
//         return HL_SUCCESS;
//     }
// }

/*if gesture isn't enabled, the handler should process touch points*/
// ret = fts_touch_process(dev, pos, buffer, size);
//     ft_printf("%s \n", __FUNCTION__);
//     return 0;
// }

/**
 * 
 * @brief 一秒调用一次
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
// void fts_timer_interrupt_handler(void)
// {
//     /* esd check */
//     fts_esdcheck_process();
// }

/**
 * 
 * @brief 挂起触屏设备，并更新挂起状态
 * @return int 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 进入休眠模式
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
static int fts_ts_suspend(void)
{
    int ret = 0;

    if (fts_data->suspended) {
        FTS_INFO("Already in suspend state");
        return HL_SUCCESS;
    }

    if (fts_data->gesture_support) {
        /*Host writes 0x01 to register address 0xD0 to enable gesture function while system suspends.*/
        ret = fts_write_reg(FTS_REG_GESTURE_EN, FTS_REG_GESTURE_ENABLE);
        if (ret == HL_FAILED)
            FTS_ERROR("enable gesture fails.ret:%d", ret);
        else
            FTS_INFO("enable gesture success.");
    } else {
        /*Host writes 0x03 to register address 0xA5 to enter into sleep mode.*/
        ret = fts_write_reg(FTS_REG_POWER_MODE, 0x03);
        if (ret == HL_FAILED)
            FTS_ERROR("system enter sleep mode fails.ret:%d", ret);
        else
            FTS_INFO("system enter sleep mode success.");
    }

    fts_data->suspended = 1;
    return HL_SUCCESS;
}

/**
 * 
 * @brief 接触挂起状态，更新挂起状态
 * @return int 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 退出休眠模式
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
static int fts_ts_resume(void)
{
    if (!fts_data->suspended) {
        FTS_INFO("Already in resume state");
        return HL_SUCCESS;
    }

    fts_data->suspended = 0;
    fts_hw_reset(200);

    return HL_SUCCESS;
}

/**
 * 
 * @brief 触摸屏控制函数
 * @param [in] dev 设备句柄
 * @param [in] cmd 操作命令，参考：hl_ft3169_cfg 枚举内容
 * @param [in] args 参数
 * @return rt_err_t 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 目前未用到参数选项
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */

static rt_err_t hl_drv_ft3169_io_ctrl(struct rt_touch_device* touch, int cmd, void* arg)
{
    rt_err_t ret = HL_FAILED;
    switch (cmd) {
        case SYS_RESET:
            ret = fts_ts_resume();
            break;
        case SYS_SUSPENDS:
            ret = fts_ts_suspend();
            break;
        case SYS_RESUME:
            ret = fts_ts_resume();
            break;
        case SYS_ESD_CHECK:
            fts_esdcheck_process();
            break;
        case CHECK_DEV_ID:
            ret = fts_check_id();
            break;
        default:
            ft_printf("param err ! \n");
            break;
    }
    return ret;
}
/**
 * 
 * @brief 触摸屏初始化函数
 * @return int 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
static rt_err_t fts_ts_init(void)
{
    int ret = 0;

    /*Initialize I2C*/
    ret = platform_i2c_init();
    if (ret == HL_FAILED) {
        FTS_ERROR("I2C init fails.ret:%d", ret);
        return ret;
    }

    /*reset pin cfg*/
    ret = platform_reset_pin_cfg();
    if (ret == HL_FAILED) {
        FTS_ERROR("reset pin init fails.ret:%d", ret);
        return ret;
    }

    /*tp power on*/
    fts_power_on();

    /*Register gpio interrupt handler,which for touch process or gestrue process*/
    ret = platform_interrupt_gpio_init();
    if (ret == HL_FAILED) {
        FTS_ERROR("Register gpio interrupt handler fails.ret:%d", ret);
        return ret;
    }

    return ret;
}

static int touch_dev_init(void)
{
    touch_dev = rt_device_find("FT3169");

    if (touch_dev == RT_NULL) {
        rt_kprintf("Can't find touch device FT3169\n");
        return HL_FAILED;
    }
    if (rt_device_init(touch_dev) != RT_EOK) {
        rt_kprintf("open touch device init failed!");
        return HL_FAILED;
    }
    if (rt_device_open(touch_dev, RT_DEVICE_FLAG_INT_RX) != RT_EOK) {
        rt_kprintf("open touch device failed!");
        return HL_FAILED;
    }
    return HL_SUCCESS;
}

/**
 * 
 * @brief 初始化注册触屏设备
 * @return int 
 * @date 2022-10-17
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-17      <td>dujunjie     <td>新建
 * </table>
 */
static int touch_device_ft3169_init(void)
{
    rt_err_t ret;
    fts_ts_init();
    // touch_dev  = rt_device_create(RT_Device_Class_Touch, RT_NULL);
    touch_hand = (struct rt_touch_device*)rt_malloc(sizeof(struct rt_touch_device));
    if (touch_hand == RT_NULL) {
        ft_printf("Touch device ft3169 create fail !\n");
        return HL_FAILED;
    }
    // touch_dev->ops         = &touch_ops;
    // touch_dev->rx_indicate = fts_touch_rx_indicate_callback;
    // touch_hand->parent     = *touch_dev;
    touch_hand->config = touch_config;
    touch_hand->info   = touch_info;
    touch_hand->ops    = &touch_ops_top;
    // touch_hand->irq_handle = fts_touch_irq_callback;

    ret = rt_hw_touch_register(touch_hand, CURRENT_TOUCH_DEVICE_NAME, RT_DEVICE_FLAG_RDONLY,
                               RT_NULL);
    if (ret != RT_EOK) {
        ft_printf("Touch device ft3169 register fail !\n");
        return HL_FAILED;
    }

    if(touch_dev_init() == HL_FAILED){
        return HL_FAILED;
    }
    return HL_SUCCESS;
}

rt_err_t touch_info_fun(struct fts_ts_event *touch_pos)
{
    rt_device_read(touch_dev, 0, touch_pos, 1);
    // rt_kprintf("touch_pos.type = %d\ttouch_pos.x = %d\ttouch_pos.y = %d\n", touch_pos.type, touch_pos.x, touch_pos.y);
    return 0;
}

int tt3169(int argc, char** argv)
{
    uint8_t cmd = atoi(argv[1]);
    if (cmd == 0) {
        fts_ts_init();
    }
    if (cmd == 1) {
        fts_touch_process(RT_NULL, RT_NULL, 0);
    }
    if (cmd == 2) {
        fts_check_id();
    }
    if (cmd == 3) {
        fts_esdcheck_process();
    }
    return RT_EOK;
}

MSH_CMD_EXPORT(tt3169, run tt3169);
INIT_DEVICE_EXPORT(touch_device_ft3169_init);
