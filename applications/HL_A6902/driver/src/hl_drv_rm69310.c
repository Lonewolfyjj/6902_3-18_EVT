/**
 * @file hl_drv_rm69310.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief spi oled显示屏驱动，功能：显示单帧画面，控制背光
 * @version 0.1
 * @date 2022-08-30
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-08-30     <td>v1.0     <td>liujie     <td>内容
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
#include "hl_drv_rm69310.h"
#include <rtdevice.h>
#include "hal_base.h"
#include "hl_hal_gpio.h"
#include "rtdef.h"

#if (!HL_IS_TX_DEVICE())

#define OLED_PWR_ON() hl_hal_gpio_high(GPIO_OLED_SWIRE)
#define OLED_PWR_OFF() hl_hal_gpio_low(GPIO_OLED_SWIRE)

#define OLED_RST_H() hl_hal_gpio_high(GPIO_OLED_RST)
#define OLED_RST_L() hl_hal_gpio_low(GPIO_OLED_RST)

#define OLED_DCX_DATA() hl_hal_gpio_high(GPIO_OLED_DCX)
#define OLED_DCX_CMD() hl_hal_gpio_low(GPIO_OLED_DCX)

#define OLED_READ_TE() hl_hal_gpio_read(GPIO_OLED_TE)

/// 显示屏使用的SPI 设备
#define OLED_SPI_DEVICE "spi2_0"
#define HL_FAILED 1
#define HL_SUCCESS 0

#define X_DEVIATION 0
#define Y_DEVIATION 4

#define hl_util_delay_sys_ms rt_thread_mdelay
#define HL_PRINTF rt_kprintf

/* page0 user cmd list */
#define RM69310_CMD_NOP 0x00
#define RM69310_CMD_SWRESET 0x01

#define RM69310_CMD_RDDID 0x04
#define RM69310_CMD_RDNUMED 0x05

#define RM69310_CMD_RDDPM 0x0A
#define RM69310_CMD_RDDMADCTR 0x0B
#define RM69310_CMD_RDDCOLMOD 0x0C
#define RM69310_CMD_RDDIM 0x0D
#define RM69310_CMD_RDDSM 0x0E
#define RM69310_CMD_RDDSDR 0x0F
#define RM69310_CMD_SLPIN 0x10
#define RM69310_CMD_SLPOUT 0x11
#define RM69310_CMD_PTLON 0x12
#define RM69310_CMD_NORON 0x13

#define RM69310_CMD_INVOFF 0x20
#define RM69310_CMD_INVON 0x21
#define RM69310_CMD_INVON 0x21
#define RM69310_CMD_ALLPOFF 0x22
#define RM69310_CMD_ALLPON 0x23

#define RM69310_CMD_DISPOFF 0x28
#define RM69310_CMD_DISPON 0x29
#define RM69310_CMD_CASET 0x2A
#define RM69310_CMD_RASET 0x2B
#define RM69310_CMD_RAMWR 0x2C

#define RM69310_CMD_PTLAR 0x30
#define RM69310_CMD_VPTLAR 0x31

#define RM69310_CMD_TEOFF 0x34  // 垂直同步信号（TE）关闭
#define RM69310_CMD_TEON 0x35   // 垂直同步信号（TE）打开
#define RM69310_CMD_MADCTR 0x36

#define RM69310_CMD_IDMOFF 0x38
#define RM69310_CMD_IDMON 0x39
#define RM69310_CMD_COLMOD 0x3A

#define RM69310_CMD_RAMWRC 0x3C

#define RM69310_CMD_STESL 0x44
#define RM69310_CMD_GSL 0x45

#define RM69310_CMD_DSTBON 0x4F

#define RM69310_CMD_WRDISBV 0x51
#define RM69310_CMD_RDDISBV 0x52
#define RM69310_CMD_WRCTRLD 0x53
#define RM69310_CMD_RDCTRLD 0x54

#define RM69310_CMD_RDDDBS 0xA1

#define RM69310_CMD_RDDDBC 0xA8

#define RM69310_CMD_RDFCS 0xAA

#define RM69310_CMD_RDCCS 0xAF

#define RM69310_CMD_SetDSIMode 0xC2

#define RM69310_CMD_SetSPIMode 0xC4

#define RM69310_CMD_SetPage 0xFE

/* RM69310_CMD_TEON = 0x35 */
#define TE_OUT_CONSISTS_VBlanking 0x00  // 只打开帧同步
#define TE_OUT_CONSISTS_VHBlanking 0x01 // 打开行同步和帧同步

/* RM69310_CMD_MADCTR = 0x36 */
#define ROW_ADDR_INCREMENT 0x00
#define ROW_ADDR_DECREMENT 0x80

#define COLUMN_ADDR_INCREMENT 0x00
#define COLUMN_ADDR_DECREMENT 0x40

#define ROW_COLUMN_EXCHANGE 0x00
#define ROW_COLUMN_NORMAL 0x20

#define REFRESH_TOP_TO_BOTTOM 0x00
#define REFRESH_BOTTOM_TO_TOP 0x10

#define COLOR_CONTROL_RGB 0x00
#define COLOR_CONTROL_BGR 0x08

#define HORIZONTAL_NORMAL 0x00
#define HORIZONTAL_FLIPPED 0x02

#define VERTICAL_DIR_NORMAL 0x00
#define VERTICAL_DIR_REVERSE 0x01

/* RM69310_CMD_COLMOD = 0x3A */
#define COLOR_FORMAT_16BIT 0x75
#define COLOR_FORMAT_18BIT 0x76
#define COLOR_FORMAT_24BIT 0x77

/* RM69310_CMD_DSTBON = 0x4F */
#define DEEP_STANDBY_MODE_ON 0x01

/* RM69310_CMD_WRDISBV = 0x51 */
#define BRIGHTNESS_DEFAULT_VALUE 0xFF

/* RM69310_CMD_WRCTRLD = 0x53 */
#define BRIGHNESS_ENABLE 0x20
#define DIMMING_ENABLE 0x08

/* RM69310_CMD_SetDSIMode = 0xC2 */
#define INTERNAL_TIMING 0x00
#define VSYNC_ALIGN_MODE 0x02
#define EXTERNAL_TIMING 0x03

/* RM69310_CMD_SetSPIMode = C4 */
#define SPI_WRITE_SRAM_ENABLE 0x80
#define SPI_WRITE_SRAM_DISABLE 0x00

#define DAUL_SPI_1P1T_1WIRE 0x00
#define DAUL_SPI_1P1T_2WIRE 0x20
#define DAUL_SPI_2P3T_2WIRE 0x30

#define DAUL_SPI_MODE_DISABLE 0x00
#define DAUL_SPI_MODE_ENABLE 0x01

typedef struct _RM69310_PARAM_T
{
    uint8_t reg_addr;
    uint8_t reg_value;
} RM69310_PARAM_T;

static struct rt_spi_device* oled_spi_device = RT_NULL;

/// 初始化命令
RM69310_PARAM_T rm69310_init_param[] = {
    // 切换到1页（工厂命令页），如下参数是芯片厂调好的不允许更改
    { RM69310_CMD_SetPage, 0x01 },
    { RM69310_CMD_SetPage, 0x01 },
    { 0x04, 0xA0 },
    { 0x05, 0x70 },
    { 0x06, 0x3C },
    { 0x25, 0x06 },
    { 0x26, 0x57 },
    { 0x27, 0x12 },
    { 0x28, 0x12 },
    { 0x2A, 0x06 },
    { 0x2B, 0x57 },
    { 0x2D, 0x12 },
    { 0x2F, 0x12 },
    { 0x37, 0x0C },
    { 0x6D, 0x18 },
    { 0x29, 0x01 },
    { 0x30, 0x41 },
    //{0x17, 0x44},
    { 0x3A, 0x1D },
    { 0x3B, 0x00 },
    { 0x3D, 0x16 },
    { 0x3F, 0x2D },
    { 0x40, 0x14 },
    { 0x41, 0x0D },
    { 0x42, 0x63 },
    { 0x43, 0x36 },
    { 0x44, 0x41 },
    { 0x45, 0x14 },
    { 0x46, 0x52 },
    { 0x47, 0x25 },
    { 0x48, 0x63 },
    { 0x49, 0x36 },
    { 0x4A, 0x41 },
    { 0x4B, 0x14 },
    { 0x4C, 0x52 },
    { 0x4D, 0x25 },
    { 0x4E, 0x63 },
    { 0x4F, 0x36 },
    { 0x50, 0x41 },
    { 0x51, 0x14 },
    { 0x52, 0x52 },
    { 0x53, 0x25 },
    { 0x54, 0x63 },
    { 0x55, 0x36 },
    { 0x56, 0x41 },
    { 0x57, 0x14 },
    { 0x58, 0x52 },
    { 0x59, 0x25 },
    { 0x66, 0x10 },
    //{0x66, 0x90},
    { 0x67, 0x40 },
    { 0x70, 0xA5 },
    { 0x72, 0x1A },
    { 0x73, 0x15 },
    { 0x74, 0x0C },
    { 0x6A, 0x1F },

    // 切换到4页（工厂命令页），如下参数是芯片厂调好的不允许更改
    { RM69310_CMD_SetPage, 0x04 },
    { 0x00, 0xDC },
    { 0x01, 0x00 },
    { 0x02, 0x02 },
    { 0x03, 0x00 },
    { 0x04, 0x00 },
    { 0x05, 0x01 },
    { 0x06, 0x09 },
    { 0x07, 0x0A },
    { 0x08, 0x00 },
    { 0x09, 0xDC },
    { 0x0A, 0x00 },
    { 0x0B, 0x02 },
    { 0x0C, 0x00 },
    { 0x0D, 0x00 },
    { 0x0E, 0x00 },
    { 0x0F, 0x09 },
    { 0x10, 0x0A },
    { 0x11, 0x00 },
    { 0x12, 0xDC },
    { 0x13, 0x00 },
    { 0x14, 0x02 },
    { 0x15, 0x00 },
    { 0x16, 0x08 },
    { 0x17, 0x01 },
    { 0x18, 0xA3 },
    { 0x19, 0x00 },
    { 0x1A, 0x00 },
    { 0x1B, 0xDC },
    { 0x1C, 0x00 },
    { 0x1D, 0x02 },
    { 0x1E, 0x00 },
    { 0x1F, 0x08 },
    { 0x20, 0x00 },
    { 0x21, 0xA3 },
    { 0x22, 0x00 },
    { 0x23, 0x00 },
    { 0x4C, 0x89 },
    { 0x4D, 0x00 },
    { 0x4E, 0x01 },
    { 0x4F, 0x08 },
    { 0x50, 0x01 },
    { 0x51, 0x85 },
    { 0x52, 0x7C },
    { 0x53, 0x8A },
    { 0x54, 0x50 },
    { 0x55, 0x02 },
    { 0x56, 0x48 },
    { 0x58, 0x34 },
    { 0x59, 0x00 },
    { 0x5E, 0xBB },
    { 0x5F, 0xBB },
    { 0x60, 0x09 },
    { 0x61, 0xB1 },
    { 0x62, 0xBB },
    { 0x65, 0x05 },
    { 0x66, 0x04 },
    { 0x67, 0x00 },
    { 0x78, 0xBB },
    { 0x79, 0x8B },
    { 0x7A, 0x32 },

    // 切换到1页（工厂命令页），如下参数是芯片厂调好的不允许更改
    { RM69310_CMD_SetPage, 0x01 },
    { 0x0E, 0x85 },
    { 0x0F, 0x85 },
    { 0x10, 0x11 },
    { 0x11, 0xA0 },
    { 0x12, 0xA0 },
    { 0x13, 0x81 },
    { 0x14, 0x81 },
    { 0x15, 0x82 },
    { 0x16, 0x82 },
    { 0x18, 0x55 },
    { 0x19, 0x33 },
    { 0x1E, 0x02 },
    { 0x5B, 0x10 },

    // {0x5E, 0x17},
    // {0x5F, 0x17},
    { 0x62, 0x15 },
    { 0x63, 0x15 },
    { 0x6A, 0x00 },
    { 0x70, 0x55 },
    { 0x1D, 0x02 },
    { 0x89, 0xF8 },
    { 0x8A, 0x80 },
    { 0x8B, 0x01 },

    { RM69310_CMD_SetPage, 0x00 },  // 切换到0页（客户命令页）
    { RM69310_CMD_TEON, TE_OUT_CONSISTS_VHBlanking },    // 打开帧同步信号（TE），默认每帧有一个上升沿产生
    { RM69310_CMD_MADCTR, ROW_COLUMN_NORMAL | HORIZONTAL_FLIPPED },
    //{RM69310_CMD_MADCTR, ROW_COLUMN_NORMAL},
    //{RM69310_CMD_MADCTR, ROW_COLUMN_NORMAL | VERTICAL_DIR_REVERSE |
    // REFRESH_BOTTOM_TO_TOP}, {RM69310_CMD_COLMOD, COLOR_FORMAT_24BIT},
    { RM69310_CMD_COLMOD, COLOR_FORMAT_16BIT },
    { RM69310_CMD_WRCTRLD, BRIGHNESS_ENABLE | DIMMING_ENABLE },
    { RM69310_CMD_SetSPIMode, SPI_WRITE_SRAM_ENABLE },
    { RM69310_CMD_WRDISBV, BRIGHTNESS_DEFAULT_VALUE },
};

static uint8_t hl_drv_rm69310_write_cmd(uint8_t cmd);
static uint8_t hl_drv_rm69310_write_data(uint8_t data);
static void    hl_drv_rm69310_gpio_init(void);
static void    hl_drv_rm69310_gpio_deinit(void);
static uint8_t hl_drv_rm69310_spi_init(void);
static uint8_t hl_drv_rm69310_spi_deinit(void);
static uint8_t hl_drv_rm69310_sleep_set(rt_bool_t state);
static uint8_t hl_drv_rm69310_display_set(rt_bool_t state);
static uint8_t hl_drv_rm69310_config(void);
static uint8_t hl_drv_rm69310_read_data(uint8_t data);
static uint8_t hl_drv_rm69310_te_set(rt_bool_t state, uint8_t param);
static uint8_t hl_drv_rm69310_display_color(uint16_t color);
/*******************************************************************************
 *                            全局函数实现 *
 *******************************************************************************/

/**
 * 
 * @brief 读取oledid
 * @return uint8_t HL_SUCCESS:成功  HL_FAILED:失败
 * @date 2022-08-30
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-30      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_drv_rm69310_read_id(void)
{
    uint8_t result[3] = { 0 };

    hl_drv_rm69310_write_cmd(RM69310_CMD_SetPage);

    hl_drv_rm69310_write_data(0x01);

    result[0] = hl_drv_rm69310_read_data(RM69310_CMD_RDDID);

    result[1] = hl_drv_rm69310_read_data(RM69310_CMD_RDDID);

    result[2] = hl_drv_rm69310_read_data(RM69310_CMD_RDDID);

    HL_PRINTF("oled_id1=%x\r\n", result[0]);
    HL_PRINTF("oled_id2=%x\r\n", result[1]);
    HL_PRINTF("oled_id3=%x\r\n", result[2]);
    return HL_SUCCESS;
}

/**
 * 
 * @brief 设置屏幕亮度
 * @param [in] value 亮度值(0x00~0xff)
 * @return uint8_t HL_SUCCESS:成功  HL_FAILED:失败
 * @date 2022-08-30
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-30      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_drv_rm69310_set_brightness(uint8_t value)
{
    uint8_t result = HL_SUCCESS;

    result = hl_drv_rm69310_write_cmd(RM69310_CMD_WRDISBV);
    result |= hl_drv_rm69310_write_data(value);
    return result = (result != HL_SUCCESS) ? HL_FAILED : HL_SUCCESS;
}

/**
 * 
 * @brief 设置要显示的图像的区域
 * @param [in] x_start x坐标起始
 * @param [in] x_end x坐标结束
 * @param [in] y_start y坐标起始
 * @param [in] y_end y坐标结束
 * @return uint8_t 
 * @date 2022-08-30
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-30      <td>liujie     <td>新建
 * </table>
 */
static uint8_t hl_drv_rm69310_set_area(uint8_t x_start, uint8_t x_end, uint8_t y_start, uint8_t y_end)
{
    uint8_t result = HL_SUCCESS;

    // if ((x_end <= x_start) || (x_end >= OLED_WIDTH)) {
    //     HL_PRINTF("err: picture width err\r\n");
    //     return HL_FAILED;
    // }

    // if ((y_end <= y_start) || (y_end >= OLED_HEIGHT)) {
    //     HL_PRINTF("err: picture height err\r\n");
    //     return HL_FAILED;
    // }

    result = hl_drv_rm69310_write_cmd(RM69310_CMD_CASET);
    result |= hl_drv_rm69310_write_data(0x00);
    result |= hl_drv_rm69310_write_data(x_start + X_DEVIATION);
    result |= hl_drv_rm69310_write_data(0x00);
    result |= hl_drv_rm69310_write_data(x_end + X_DEVIATION);

    result |= hl_drv_rm69310_write_cmd(RM69310_CMD_RASET);
    result |= hl_drv_rm69310_write_data(0x00);
    result |= hl_drv_rm69310_write_data(y_start + Y_DEVIATION);
    result |= hl_drv_rm69310_write_data(0x00);
    result |= hl_drv_rm69310_write_data(y_end + Y_DEVIATION);

    return result = (result != HL_SUCCESS) ? HL_FAILED : HL_SUCCESS;
}

/**
 * 
 * @brief 写memory开始命令
 * @return uint8_t 
 * @date 2022-08-30
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-30      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_drv_rm69310_start_write_mem(void)
{
    uint8_t res = HL_SUCCESS;
    res         = hl_drv_rm69310_write_cmd(0x2C);
    OLED_DCX_DATA();
    return res;
}

/**
 *
 * @brief 将图片数据写到RM69310的memory中
 * @param [in] p_data 
 * @param [in] size 
 * @return uint8_t 
 * @date 2022-08-30
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-30      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_drv_rm69310_write_mem(const uint8_t* p_data, uint32_t size)
{
    struct rt_spi_message msg1;
    struct rt_spi_message msg2;
    uint8_t               res = HL_SUCCESS;

    if (size > 86400) {
        HL_PRINTF("err: size no more then 86400\r\n");
        return HL_FAILED;
    }

    if (p_data == NULL) {
        HL_PRINTF("err: p_pic can't be NULL\r\n");
        return HL_FAILED;
    }

    if (size > 65535) {
        msg1.send_buf   = p_data;
        msg1.recv_buf   = RT_NULL;
        msg1.length     = size / 2;
        msg1.cs_take    = 1;
        msg1.cs_release = 0;
        msg1.next       = &msg2;

        msg2.send_buf   = p_data += size / 2;
        msg2.recv_buf   = RT_NULL;
        msg2.length     = size - (size / 2);
        msg2.cs_take    = 0;
        msg2.cs_release = 1;
        msg2.next       = RT_NULL;

        if (RT_NULL != rt_spi_transfer_message(oled_spi_device, &msg1)) {
            res = HL_FAILED;
        }
    } else {
        msg1.send_buf   = p_data;
        msg1.recv_buf   = RT_NULL;
        msg1.length     = size;
        msg1.cs_take    = 1;
        msg1.cs_release = 1;
        msg1.next       = RT_NULL;

        if (RT_NULL != rt_spi_transfer_message(oled_spi_device, &msg1)) {
            res = HL_FAILED;
        }
    }

    return res;
}

/**
 * 
 * @brief 写入命令
 * @param [in] cmd 
 * @return uint8_t 
 * @date 2022-08-30
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-30      <td>liujie     <td>新建
 * </table>
 */
static uint8_t hl_drv_rm69310_write_cmd(uint8_t cmd)
{
    OLED_DCX_CMD();
    if (1 == rt_spi_send(oled_spi_device, (const void*)&cmd, 1))
        return HL_SUCCESS;
    else
        return HL_FAILED;
}

/**
 * 
 * @brief 吸入数据到屏幕
 * @param [in] data 
 * @return uint8_t 
 * @date 2022-08-30
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-30      <td>liujie     <td>新建
 * </table>
 */
static uint8_t hl_drv_rm69310_write_data(uint8_t data)
{
    OLED_DCX_DATA();
    if (1 == rt_spi_send(oled_spi_device, (const void*)&data, 1))
        return HL_SUCCESS;
    else
        return HL_FAILED;
}

/**
 * 
 * @brief 屏幕信息读取
 * @param [in] data 
 * @return uint8_t 
 * @date 2022-08-30
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-30      <td>liujie     <td>新建
 * </table>
 */
static uint8_t hl_drv_rm69310_read_data(uint8_t data)
{
    uint8_t result = 0xFF;

    OLED_DCX_CMD();

    OLED_DCX_DATA();

    return result;
}

/**
 * 
 * @brief 屏幕管脚初始化
 * @date 2022-08-30
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-30      <td>liujie     <td>新建
 * </table>
 */
static void hl_drv_rm69310_gpio_init(void)
{

    /* 初始化SPI管脚 */
    hl_hal_gpio_init(GPIO_OLED_DCX);

    hl_hal_gpio_init(GPIO_OLED_RST);

    //hl_hal_gpio_init(GPIO_OLED_SWIRE);

    hl_hal_gpio_init(GPIO_OLED_TE);

    // hl_hal_gpio_attach_irq(GPIO_OLED_TE, PIN_IRQ_MODE_RISING, hl_hal_gpio_test_irq_process, "te");
    // hl_hal_gpio_irq_enable(GPIO_OLED_TE, PIN_IRQ_ENABLE);

    OLED_DCX_DATA();
    hl_util_delay_sys_ms(10);
    OLED_PWR_ON(); // 延时不要去，1线去掉延时容易被识别成脉冲

    hl_util_delay_sys_ms(10);

    OLED_RST_H();
    hl_util_delay_sys_ms(10);

    OLED_RST_L();
    hl_util_delay_sys_ms(100);  // 10ms

    OLED_RST_H();
    hl_util_delay_sys_ms(500);  // 400ms
}
/**
 * 
 * @brief 屏幕引脚去初始化
 * @date 2022-08-30
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-30      <td>liujie     <td>新建
 * </table>
 */
static void hl_drv_rm69310_gpio_deinit(void)
{
    hl_util_delay_sys_ms(20);
    hl_hal_gpio_low(GPIO_OLED_SWIRE);
    hl_util_delay_sys_ms(50);

    hl_hal_gpio_deinit(GPIO_OLED_DCX);
    hl_hal_gpio_deinit(GPIO_OLED_RST);
    hl_hal_gpio_deinit(GPIO_OLED_TE);
    
}

/**
 * 
 * @brief oled驱动SPI初始化
 * @return uint8_t 
 * @date 2022-08-30
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-30      <td>liujie     <td>新建
 * </table>
 */
static uint8_t hl_drv_rm69310_spi_init(void)
{
    struct rt_spi_configuration cfg;

    oled_spi_device = (struct rt_spi_device*)rt_device_find(OLED_SPI_DEVICE);
    if (oled_spi_device == RT_NULL) {
        rt_kprintf("Did not find device: %s....\n", OLED_SPI_DEVICE);
        return HL_FAILED;
    }

    cfg.data_width = 8; /* 配置8bits传输模式 */
    cfg.mode       = RT_SPI_MASTER | RT_SPI_MSB | RT_SPI_MODE_1;
    cfg.max_hz     = 50000000; /* 配置频率 50Mhz */

    if (RT_EOK != rt_spi_configure(oled_spi_device, &cfg)) {
        return HL_FAILED;
    }

    return HL_SUCCESS;
}

/**
 * 
 * @brief 反初始化SPI
 * @return uint8_t 
 * @date 2022-08-30
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-30      <td>liujie     <td>新建
 * </table>
 */

static uint8_t hl_drv_rm69310_spi_deinit(void)
{
    // if () {
    //     HL_PRINTF("rm69310 spi1 deinit!!!\r\n");
    //     return HL_FAILED;
    // }
    return HL_SUCCESS;
}
/**
 * 
 * @brief 开关oled休眠模式
 * @param [in] state RT_TRUE进入休眠  RT_FALSE退出休眠
 * @return uint8_t 
 * @date 2022-08-30
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-30      <td>liujie     <td>新建
 * </table>
 */

static uint8_t hl_drv_rm69310_sleep_set(rt_bool_t state)
{

    if (RT_TRUE == state) {
        return hl_drv_rm69310_write_cmd(RM69310_CMD_SLPIN);
    } else {
        return hl_drv_rm69310_write_cmd(RM69310_CMD_SLPOUT);
    }
}

/**
 * 
 * @brief 开关OLED显示模式
 * @param [in] state 
 * @return uint8_t HL_TRUE打开显示  false关闭显示
 * @date 2022-08-30
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-30      <td>liujie     <td>新建
 * </table>
 */
static uint8_t hl_drv_rm69310_display_set(rt_bool_t state)
{
    if (RT_TRUE == state) {
        return hl_drv_rm69310_write_cmd(RM69310_CMD_DISPON);
    } else {
        return hl_drv_rm69310_write_cmd(RM69310_CMD_DISPOFF);
    }
}

/**
 * @brief 设置TE垂直同步功能
 * @param [in] state RT_TRUE 开启  RT_FALSE 关闭
 * @param [in] param TE_OUT_CONSISTS_VHBlanking 开启帧同步和行同步
 *                   TE_OUT_CONSISTS_VBlanking 只开帧同步
 * @return uint8_t 
 * @date 2022-09-24
 * @author liujie (jie.liu@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-24      <td>luzhanghao     <td>新建
 * </table>
 */
static uint8_t hl_drv_rm69310_te_set(rt_bool_t state, uint8_t param)
{
    uint8_t res = HL_FAILED;

    if (RT_TRUE == state) {
        res = hl_drv_rm69310_write_cmd(RM69310_CMD_TEON);
        if (param > TE_OUT_CONSISTS_VHBlanking) {
            return res;
        }
        res |= hl_drv_rm69310_write_data(param);
        return res;
    } else {
        res = hl_drv_rm69310_write_cmd(RM69310_CMD_TEOFF);
        return res;
    }
}
/**
 * 
 * @brief 上电配置rm69310
 * @return uint8_t 
 * @date 2022-08-30
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-30      <td>liujie     <td>新建
 * </table>
 */
static uint8_t hl_drv_rm69310_config(void)
{
    uint32_t iLoop, cmd_num;
    uint8_t  result = HL_SUCCESS;

    cmd_num = sizeof(rm69310_init_param) / sizeof(rm69310_init_param[0]);

    for (iLoop = 0; iLoop < cmd_num; iLoop++) {
        result |= hl_drv_rm69310_write_cmd(rm69310_init_param[iLoop].reg_addr);
        result |= hl_drv_rm69310_write_data(rm69310_init_param[iLoop].reg_value);
    }

    result |= hl_drv_rm69310_set_brightness(DEFAULT_BACKLIGHT);

    result |= hl_drv_rm69310_te_set(RT_TRUE, TE_OUT_CONSISTS_VBlanking); // 默认开启帧同步

    result |= hl_drv_rm69310_sleep_set(RT_FALSE);

    hl_util_delay_sys_ms(500);

    result |= hl_drv_rm69310_display_set(RT_TRUE);

    //result |= hl_drv_rm69310_display_color(RGB565_BLACK);
    //hl_util_delay_sys_ms(30);


    return result = (result != HL_SUCCESS) ? HL_FAILED : HL_SUCCESS;
}

void hl_drv_rm69310_mem_deal(const uint8_t* buf,const uint8_t* data,uint16_t vor,uint16_t size)
{
    uint16_t i = 0, j = 0, k = 0;
    size = size * 2;
    for(i = 0;i < vor;i++){
        memcpy(&buf[k],&data[j],size);
        j = j + size + 2;
        k = (i+1) * size;
    }
}

/**
 * 
 * @brief 显示单帧图像
 * @param [in] x_start x坐标起始
 * @param [in] x_end x坐标结束
 * @param [in] y_start y坐标起始
 * @param [in] y_end y坐标结束
 * @param [in] p_pic 缓存地址
 * @return uint8_t 
 * @date 2022-08-30
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-30      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_drv_rm69310_display_write(uint8_t x_start, uint8_t x_end, uint8_t y_start, uint8_t y_end,
                                     const uint8_t* p_pic)
{
    uint8_t column, row,cs, ce;
    uint8_t result = HL_SUCCESS;
    uint8_t buf[OLED_MEM_SIZE],*p = RT_NULL;    
    p = buf;

    column = x_end - x_start + 1;
    row    = y_end - y_start + 1;

    cs = (x_start & 0x01); 
    ce = (x_end & 0x01); 

    if(cs == 1 && ce == 1){//奇 奇
        x_start -= 1;
        x_end -= 2;
        hl_drv_rm69310_mem_deal(buf,p_pic,(y_end - y_start + 1),(x_end - x_start + 1));
    }

    if(cs == 0 && ce == 0){//偶 偶
        x_end -= 1;
        hl_drv_rm69310_mem_deal(buf,p_pic,(y_end - y_start + 1),(x_end - x_start + 1));
    }

    if(cs == 1 && ce == 0){//奇 偶
        x_start -= 1;
        x_end -= 1;
        cs = (x_start & 0x01); 

        x_start -= cs;

        column = x_end - x_start + 1;

        ce = (column & 0x01); 

        x_end -= ce;

        p = p_pic;
    }

    if(cs == 0 && ce == 1){//偶 奇

        cs = (x_start & 0x01); 

        x_start -= cs;

        column = x_end - x_start + 1;

        ce = (column & 0x01); 

        x_end -= ce;

        p = p_pic;
    }

    column = x_end - x_start + 1;
    row    = y_end - y_start + 1;

    if (HL_FAILED == hl_drv_rm69310_set_area(x_start, x_end, y_start, y_end)) {
        HL_PRINTF("err: set area fail!\r\n");
        p = RT_NULL;
        return HL_FAILED;
    }

    result = hl_drv_rm69310_start_write_mem();

    if (HL_FAILED == hl_drv_rm69310_write_mem(p, column * row * 2)) {
        HL_PRINTF("err: write memory fail!\r\n");
        p = RT_NULL;
        return HL_FAILED;
    }
    p = RT_NULL;
    return result;
}

/**
 * @brief 设置屏幕单一颜色
 * @param [in] color 
 * @return uint8_t 颜色：
 *              RGB565_BLUE
 *              RGB565_GREEN
 *              RGB565_RED
 * @date 2022-09-24
 * @author liujie (jie.liu@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-24      <td>liujie     <td>新建
 * </table>
 */
static uint8_t hl_drv_rm69310_display_color(uint16_t color)
{
    uint8_t i, j;
    uint8_t res     = HL_SUCCESS;
    struct rt_spi_message msg1;

    if (oled_spi_device == RT_NULL) {
        return HL_FAILED;
    }
    res = hl_drv_rm69310_set_area(0, OLED_WIDTH - 1, 0, OLED_HEIGHT - 1);

    res |= hl_drv_rm69310_write_cmd(0x2C);

    if (RT_EOK != rt_spi_take_bus(oled_spi_device)) {
        return HL_FAILED;
    }

    rt_spi_take(oled_spi_device);


    OLED_DCX_DATA();

    for (i = 0; i < OLED_HEIGHT; i++) {
        for (j = 0; j < OLED_WIDTH; j++) {

            msg1.send_buf = &color;
            msg1.recv_buf = RT_NULL;
            msg1.cs_take    = 0;
            msg1.cs_release = 0;
            msg1.length   = 2;
            msg1.next   = RT_NULL;
            if (RT_EOK != rt_spi_transfer_message(oled_spi_device, &msg1))
                res |= HL_FAILED;
        }
    }

    rt_spi_release(oled_spi_device);
    rt_spi_release_bus(oled_spi_device);

    return res;
}

/**
 * 
 * @brief 配置rm69310背光参数，获取状态id
 * @param [in] cmd 
 * @param [in] ptr 
 * @param [in] len 
 * @return uint8_t 
 * @date 2022-08-30
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-30      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_drv_rm69310_io_ctrl(uint8_t cmd, void* ptr, uint16_t len)
{
    uint8_t result = HL_SUCCESS;

    switch (cmd) {
        case SET_BACKLIGHT: {
            uint8_t val = *(uint8_t*)ptr;
            if (val > 255) {
               result = HL_FAILED; 
            } else {
                result = hl_drv_rm69310_set_brightness(val);
            }
            
        } break;
        case READ_STATE: {

        } break;
        case CTRL_POWER: {
            uint8_t val = *(uint8_t*)ptr;
            if (val == 1) {
                hl_util_delay_sys_ms(20);
                OLED_PWR_ON();
                hl_util_delay_sys_ms(20);
            } else {
                hl_util_delay_sys_ms(20);
                OLED_PWR_OFF();
                hl_util_delay_sys_ms(20);
            }
        } break;
        case READ_OLED_ID: {
            result = hl_drv_rm69310_read_id();
        } break;
        // case SET_COLOR: {
        //     if (len != 2) {
        //         HL_PRINTF("color format rgb565!\r\n");
        //         result = HL_FAILED;
        //     } else {
        //         uint16_t color = *(uint16_t*)ptr;
        //         result = hl_drv_rm69310_display_color(color);
        //     }
        // } break;
        default:{

        } break;
    }

    return result;
}

/**
 * 
 * @brief 屏幕初始化
 * @return uint8_t HL_SUCCESS:成功  HL_FAILED:失败
 * @date 2022-08-30
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-30      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_drv_rm69310_init(void)
{
    if (HL_FAILED == hl_drv_rm69310_spi_init()) {
        HL_PRINTF("oled spi spi init fail!\r\n");
        return HL_FAILED;
    }

    hl_drv_rm69310_gpio_init();

    if (HL_FAILED == hl_drv_rm69310_config()) {
        HL_PRINTF("oled spi cfg  fail!\r\n");
        return HL_FAILED;
    }

    return HL_SUCCESS;
}

/**
 * 
 * @brief 反初始化显示屏
 * @return uint8_t HL_SUCCESS:成功  HL_FAILED:失败
 * @date 2022-08-30
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志: 
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-30      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_drv_rm69310_deinit(void)
{
    hl_drv_rm69310_gpio_deinit();

    if (HL_FAILED == hl_drv_rm69310_spi_deinit()) {
        HL_PRINTF("err: rm69310 deinit fail!\r\n");
        return HL_FAILED;
    }

    HL_PRINTF("rm69310 deinit successful!\r\n");
    return HL_SUCCESS;
}
#endif