/**
 * @file hl_drv_nau88l25b.c
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief nau88l25b驱动文件
 * @version 1.0
 * @date 2022-09-08
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-08     <td>v1.0     <td>dujunjie     <td>内容
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
#include "hl_drv_nau88l25b.h"

#define NAU88L25B_DEBUG
#ifdef NAU88L25B_DEBUG
#define nau_printf(...) rt_kprintf(__VA_ARGS__)
#else
#define nau_printf(...)
#endif

typedef uint8_t (*hl_reg_ctl_fun)(uint16_t cmd, uint8_t cmd_typ, void* param);
static hl_reg_ctl_fun            fun_arr[NAU_REG_NUM + NAU_SPEREG_NUM];
static uint16_t                  reg_arr[NAU_REG_NUM + NAU_SPEREG_NUM];
static struct rt_i2c_bus_device* i2c_bus = RT_NULL; /* I2C总线设备句柄 */

/* NAU88L25B init the register value */
typedef struct _NAU88L25B_REG_T
{
    uint16_t reg_addr;
    uint16_t reg_value;
} NAU88L25B_REG_T;

static NAU88L25B_REG_T s_nau88l25b_param[] = {
    { 0x0000, 0x0000 }, { 0x0001, 0x07D4 }, { 0x0002, 0x0000 }, { 0x0003, 0x0050 }, { 0x0004, 0x0081 },
    { 0x0005, 0x624D }, { 0x0006, 0x0008 }, { 0x0007, 0x0410 }, { 0x0008, 0x1000 }, { 0x0009, 0x6000 },
    { 0x000A, 0xF13C }, { 0x000C, 0x0048 }, { 0x000D, 0x0000 }, { 0x000F, 0x0000 }, { 0x0010, 0x0000 },
    { 0x0011, 0x0000 }, { 0x0012, 0xFFFF }, { 0x0013, 0x0015 }, { 0x0014, 0x0110 }, { 0x0015, 0x0000 },
    { 0x0016, 0x0000 }, { 0x0017, 0x0000 }, { 0x0018, 0x0000 }, { 0x0019, 0x0000 }, { 0x001A, 0x0000 },
    { 0x001B, 0x0000 }, { 0x001C, 0x0002 }, { 0x001D, 0x2011 }, { 0x001E, 0x0000 }, { 0x001F, 0x0000 },
    { 0x0020, 0x0000 }, { 0x0021, 0x0000 }, { 0x0022, 0x0000 }, { 0x0023, 0x0000 }, { 0x0024, 0x0000 },
    { 0x0025, 0x0000 }, { 0x0025, 0x0000 }, { 0x0025, 0x0000 }, { 0x0025, 0x0000 }, { 0x0025, 0x0000 },
    { 0x0026, 0x0000 }, { 0x0027, 0x0000 }, { 0x0028, 0x0000 }, { 0x0029, 0x0000 }, { 0x002A, 0x0000 },
    { 0x002B, 0x00D2 }, { 0x002C, 0x0080 }, { 0x002D, 0x0000 }, { 0x002F, 0x0000 }, { 0x0030, 0x00DB },
    { 0x0031, 0x1000 }, { 0x0032, 0x0000 }, { 0x0033, 0x00CF }, { 0x0034, 0x02CF }, { 0x0038, 0x1486 },
    { 0x0039, 0x0F12 }, { 0x003A, 0x25FF }, { 0x003B, 0x3457 }, { 0x0045, 0x1486 }, { 0x0046, 0x0F12 },
    { 0x0047, 0x25F9 }, { 0x0048, 0x3457 }, { 0x004C, 0x0000 }, { 0x0050, 0x2007 }, { 0x0051, 0x0000 },
    { 0x0055, 0x0000 }, { 0x0058, 0x1A14 }, { 0x0059, 0x00FF }, { 0x0066, 0x0060 }, { 0x0068, 0xC300 },
    { 0x0069, 0x0000 }, { 0x006A, 0x0083 }, { 0x0071, 0x0011 }, { 0x0072, 0x0260 }, { 0x0073, 0x332C },
    { 0x0074, 0x4002 }, { 0x0076, 0x3140 }, { 0x0077, 0x0000 }, { 0x007F, 0x553F }, { 0x0080, 0x0420 },
    { 0x0081, 0x002C }, { 0x0082, 0x0060 },
};

/**
 * 
 * @brief i2c总线写数据
 * @param [in] bus i2c总线句柄
 * @param [in] cfg_opt 操作的寄存器
 * @param [in] data 数据
 * @return rt_err_t 0成功，1失败
 * @date 2022-09-08
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-08      <td>dujunjie     <td>新建
 * </table>
 */
static rt_err_t hl_i2c_write_reg(struct rt_i2c_bus_device* bus, rt_uint16_t reg, rt_uint16_t* data)
{
    rt_err_t          ret;
    rt_uint8_t        buf[4];
    struct rt_i2c_msg msgs[2];

    buf[0] = (reg >> 8);        // reg
    buf[1] = (reg & 0xFF);      //
    buf[2] = (data[0] >> 8);    // data
    buf[3] = (data[0] & 0xFF);  //

    msgs[0].addr  = NAU88L25B_DEVICE_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = buf;
    msgs[0].len   = 4;
    /*
    msgs[1].addr  = NAU88L25B_DEVICE_ADDRESS;
    msgs[1].flags = RT_I2C_WR | RT_I2C_NO_START;
    msgs[1].buf   = &buf[2];
    msgs[1].len   = 2;
    */
    /* 调用I2C设备接口传输数据 */
    nau_printf("%s : buf[2] = %X buf[3] = %X \n", __FUNCTION__, buf[2], buf[3]);
    ret = rt_i2c_transfer(bus, msgs, 1);
    nau_printf("write ret = %d \n", ret);
    if (ret == 1)
        return HL_SUCCESS;
    else
        return HL_FAILED;
}

/**
 * 
 * @brief i2c总线读数据
 * @param [in] bus i2c总线句柄
 * @param [in] cfg_opt 操作的寄存器
 * @param [in] data 数据
 * @return rt_err_t 0成功，1失败
 * @date 2022-09-08
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-08      <td>dujunjie     <td>新建
 * </table>
 */
static rt_err_t hl_i2c_read_reg(struct rt_i2c_bus_device* bus, rt_uint16_t reg, rt_uint16_t* rbuf)
{
    rt_err_t          ret    = 0;
    rt_uint8_t        buf[4] = { 0, 0, 0, 0 };
    struct rt_i2c_msg msgs[2];

    buf[0] = (reg >> 8);      // reg
    buf[1] = (reg & 0x00FF);  //

    msgs[0].addr  = NAU88L25B_DEVICE_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = buf;
    msgs[0].len   = 2;

    msgs[1].addr  = NAU88L25B_DEVICE_ADDRESS;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = &buf[2];
    msgs[1].len   = 2;

    ret = rt_i2c_transfer(bus, msgs, 2);
    nau_printf("read ret = %d \n", ret);
    /* 调用I2C设备接口传输数据 */
    if (ret == 2) {
        rbuf[0] = ((buf[2] << 8) + buf[3]);
        nau_printf("%s : buf[0] = %X buf[1] = %X buf[2] = %X buf[3] = %X \n", __FUNCTION__, buf[0], buf[1], buf[2],
                   buf[3]);
        return HL_SUCCESS;
    } else
        return HL_FAILED;
}

/**
 * 
 * @brief 将增益设置参数转换为配置寄存器所需要的值
 * @param [in] data 需要转换的增益
 * @return uint8_t 0成功，1失败
 * @date 2022-09-08
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-08      <td>dujunjie     <td>新建
 * </table>
 */
static uint8_t set_gain_tran(int data)
{
    uint8_t gain = 0;
    if (data < -103)
        gain = 0x00;
    else if (data > 24)
        gain = 0xFF;
    else
        gain = (data * 2) + 207;
    return gain;
}

/**
 * 
 * @brief 软件重置芯片
 * @return uint8_t 0成功，1失败
 * @date 2022-09-08
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-08      <td>dujunjie     <td>新建
 * </table>
 */
static uint8_t hl_reg00_ctl(void)
{
    uint16_t reset = 0x0000;
    if (hl_i2c_write_reg(i2c_bus, NAU88L25B_RESET_REG00, &reset)) {
        nau_printf("%s reset fail !\n", __FUNCTION__);
        return HL_FAILED;
    }
    return HL_SUCCESS;
}
/**
 * 
 * @brief 0x33寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
 * @date 2022-09-08
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-08      <td>dujunjie     <td>新建
 * </table>
 */
static uint8_t hl_reg33_ctl(uint16_t cmd, uint8_t cmd_typ, void* param)
{
    int*                 tmp = (int*)param;
    HL_nau88l25b_REG33_T reg33;
    if (hl_i2c_read_reg(i2c_bus, NAU_REG33_ADDR, (uint16_t*)&reg33)) {
        nau_printf("%s read fail !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    if (cmd_typ == NAU_READ_REG_CMD) {
        switch (cmd) {
            case OPT_DACL_CTRL_DAC_MIXER:
                tmp[0] = reg33.DAC_MIXER;
                break;
            case OPT_DACL_CTRL_DAC_CH_SEL0:
                tmp[0] = reg33.DAC_CH_SEL0;
                break;
            case OPT_DACL_CTRL_DGAINL_DAC:
                tmp[0] = (reg33.DGAINL_DAC - 207) / 2;
                if (reg33.DGAINL_DAC == 0) {
                    tmp[0] = 0;
                }
                break;
            default:
                nau_printf("%s cmd_typ read fail !\n", __FUNCTION__);
                goto CTL_ERR;
                break;
        }
        nau_printf("%s read success : tmp[0] = %d  reg33.DGAINL_DAC = %d!\n", __FUNCTION__, tmp[0], reg33.DGAINL_DAC);
        return HL_SUCCESS;
    } else {
        switch (cmd) {
            case OPT_DACL_CTRL_DAC_MIXER:
                reg33.DAC_MIXER = tmp[0];
                break;
            case OPT_DACL_CTRL_DAC_CH_SEL0:
                reg33.DAC_CH_SEL0 = tmp[0];
                break;
            case OPT_DACL_CTRL_DGAINL_DAC:
                reg33.DGAINL_DAC = set_gain_tran(tmp[0]);
                break;
            default:
                nau_printf("%s cmd_typ write fail !\n", __FUNCTION__);
                goto CTL_ERR;
                break;
        }
    }
    if (hl_i2c_write_reg(i2c_bus, NAU_REG33_ADDR, (uint16_t*)&reg33)) {
        nau_printf("%s write fail !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    nau_printf("%s write success : tmp[0] = %d reg33.DGAINL_DAC = %X!\n", __FUNCTION__, tmp[0], reg33.DGAINL_DAC);
    return HL_SUCCESS;
CTL_ERR:
    nau_printf("%s fail !\n", __FUNCTION__);
    return HL_FAILED;
}

/**
 * 
 * @brief 0x34寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
 * @date 2022-09-08
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-08      <td>dujunjie     <td>新建
 * </table>
 */
static uint8_t hl_reg34_ctl(uint16_t cmd, uint8_t cmd_typ, void* param)
{
    int*                 tmp = (int*)param;
    HL_nau88l25b_REG34_T reg34;
    if (hl_i2c_read_reg(i2c_bus, NAU_REG34_ADDR, (uint16_t*)&reg34)) {
        nau_printf("%s read fail !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    if (cmd_typ == NAU_READ_REG_CMD) {
        switch (cmd) {
            case OPT_DACR_CTRL_DAC_CH_SEL1:
                tmp[0] = reg34.DAC_CH_SEL1;
                break;
            case OPT_DACR_CTRL_DGAINR_DAC:
                tmp[0] = (reg34.DGAINR_DAC - 207) / 2;
                if (reg34.DGAINR_DAC == 0) {
                    tmp[0] = 0;
                }
                break;
            default:
                nau_printf("%s cmd_typ read fail !\n", __FUNCTION__);
                goto CTL_ERR;
                break;
        }
        nau_printf("%s read success : tmp[0] = %d  reg34.DGAINR_DAC = %d!\n", __FUNCTION__, tmp[0], reg34.DGAINR_DAC);
        return HL_SUCCESS;
    } else {
        switch (cmd) {
            case OPT_DACR_CTRL_DAC_CH_SEL1:
                reg34.DAC_CH_SEL1 = tmp[0];
                break;
            case OPT_DACR_CTRL_DGAINR_DAC:
                reg34.DGAINR_DAC = set_gain_tran(tmp[0]);
                break;
            default:
                nau_printf("%s cmd_typ write fail !\n", __FUNCTION__);
                goto CTL_ERR;
                break;
        }
    }
    if (hl_i2c_write_reg(i2c_bus, NAU_REG34_ADDR, (uint16_t*)&reg34)) {
        nau_printf("%s write fail !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    nau_printf("%s write success : tmp[0] = %d reg34.DGAINR_DAC = %X!\n", __FUNCTION__, tmp[0], reg34.DGAINR_DAC);
    return HL_SUCCESS;
CTL_ERR:
    nau_printf("%s fail !\n", __FUNCTION__);
    return HL_FAILED;
}

/**
 * 
 * @brief 增益寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
 * @return uint8_t 
 * @date 2022-09-08
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-08      <td>dujunjie     <td>新建
 * </table>
 */
static uint8_t hl_gain_ctl(uint16_t cmd, uint8_t cmd_typ, void* param)
{
    int  gainl = 0, gainr = 0;
    int* gain = (int*)param;
    switch (cmd_typ) {
        case NAU_READ_REG_CMD:
            if (hl_reg33_ctl(OPT_DACL_CTRL_DGAINL_DAC, cmd_typ, &gainl))
                goto CTL_ERR;
            if (hl_reg34_ctl(OPT_DACR_CTRL_DGAINR_DAC, cmd_typ, &gainr))
                goto CTL_ERR;
            gain[0] = (gainl + gainr) / 2;
            break;
        case NAU_WRITE_REG_CMD:
            if (hl_reg33_ctl(OPT_DACL_CTRL_DGAINL_DAC, cmd_typ, param))
                goto CTL_ERR;
            if (hl_reg34_ctl(OPT_DACR_CTRL_DGAINR_DAC, cmd_typ, param))
                goto CTL_ERR;
            break;
        default:
            goto CTL_ERR;
            break;
    }
    return HL_SUCCESS;
CTL_ERR:
    nau_printf("%s fail ! cmd = %X cmd_typ = %X gain[0] = %d\n", __FUNCTION__, cmd, cmd_typ, gain[0]);
    return HL_FAILED;
}

/**
 * 
 * @brief 增益寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
 * @date 2022-09-08
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-08      <td>dujunjie     <td>新建
 * </table>
 */
static uint8_t hl_mute_ctl(uint16_t cmd, uint8_t cmd_typ, void* param)
{
    int  mutel = 0, muter = 0, mutea = -104;
    int* mute = (int*)param;
    switch (cmd_typ) {
        case NAU_READ_REG_CMD:
            if (hl_reg33_ctl(OPT_DACL_CTRL_DGAINL_DAC, cmd_typ, &mutel))
                goto CTL_ERR;
            if (hl_reg34_ctl(OPT_DACR_CTRL_DGAINR_DAC, cmd_typ, &muter))
                goto CTL_ERR;
            mute[0] = (mutel + muter) / 2;
            break;
        case NAU_WRITE_REG_CMD:
            if (hl_reg33_ctl(OPT_DACL_CTRL_DGAINL_DAC, cmd_typ, &mutea))
                goto CTL_ERR;
            if (hl_reg34_ctl(OPT_DACR_CTRL_DGAINR_DAC, cmd_typ, &mutea))
                goto CTL_ERR;
            break;
        default:
            goto CTL_ERR;
            break;
    }
    return HL_SUCCESS;
CTL_ERR:
    nau_printf("%s fail ! cmd = %X cmd_typ = %X mute[0] = %d\n", __FUNCTION__, cmd, cmd_typ, mute[0]);
    return HL_FAILED;
}
/**
 * 
 * @brief 注册寄存器操作函数
 * @date 2022-09-05
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-05      <td>dujunjie     <td>新建
 * </table>
 */
static void hl_reg_ctl_fun_init(void)
{
    fun_arr[0] = hl_reg33_ctl;
    fun_arr[1] = hl_reg34_ctl;
    fun_arr[2] = hl_gain_ctl;
    fun_arr[3] = hl_mute_ctl;
}

/**
 * 
 * @brief 注销已注册的寄存器操作函数
 * @date 2022-09-08
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-08      <td>dujunjie     <td>新建
 * </table>
 */
static void hl_reg_ctl_fun_deinit(void)
{
    uint8_t i;
    for (i = 0; i < NAU_REG_NUM + NAU_SPEREG_NUM; i++)
        fun_arr[i] = NULL;
}

/**
 * 
 * @brief 初始化寄存器数组
 * @date 2022-09-08
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-08      <td>dujunjie     <td>新建
 * </table>
 */
static void hl_reg_arr_init(void)
{
    uint8_t i;
    for (i = 0; i < NAU_REG_NUM; i++)
        reg_arr[i] = NAU_REG33_ADDR + i;

    for (i = NAU_REG_NUM; i < NAU_REG_NUM + NAU_SPEREG_NUM; i++)
        reg_arr[i] = NAU_REGGAIN_ADDR + i - NAU_REG_NUM;
}

/**
 * 
 * @brief 注销寄存器数组
 * @date 2022-09-08
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-08      <td>dujunjie     <td>新建
 * </table>
 */
static void hl_reg_arr_deinit(void)
{
    uint8_t i;
    for (i = 0; i < NAU_REG_NUM + NAU_SPEREG_NUM; i++)
        reg_arr[i] = 0;
}

/**
 * 
 * @brief 获取寄存器地址
 * @param [in] cfg_opt 带解析的寄存器指令
 * @return uint8_t 寄存器地址
 * @date 2022-09-08
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-08      <td>dujunjie     <td>新建
 * </table>
 */
static uint8_t hl_get_reg_serial(uint16_t cfg_opt)
{
    uint8_t reg_ser;
    switch (cfg_opt) {
        //reg33
        case OPT_DACL_CTRL_DAC_MIXER:
        case OPT_DACL_CTRL_DAC_CH_SEL0:
        case OPT_DACL_CTRL_DGAINL_DAC:
            reg_ser = NAU_REG33_ADDR;
            break;
        //reg34
        case OPT_DACR_CTRL_DAC_CH_SEL1:
        case OPT_DACR_CTRL_DGAINR_DAC:
            reg_ser = NAU_REG34_ADDR;
            break;
        case OPT_GAIN:
            reg_ser = NAU_REGGAIN_ADDR;
            break;
        case OPT_MUTE:
            reg_ser = NAU_REGMUTE_ADDR;
            break;
        default:
            nau_printf("Register : [ %X ] overrun !\n", cfg_opt);
            return 0xFF;
            break;
    }
    return reg_ser;
}

/**
 * 
 * @brief 寄存器操作入口函数
 * @param [in] reg_ser 寄存器
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 
 * @date 2022-09-05
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 操作参数只有第一字节生效
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-05      <td>dujunjie     <td>新建
 * </table>
 */
static uint8_t hl_reg_ctl(uint16_t reg_ser, uint8_t cmd, uint8_t cmd_typ, void* param)
{
    uint8_t i;
    for (i = 0; i < NAU_REG_NUM + NAU_SPEREG_NUM; i++) {
        if (reg_arr[i] == reg_ser) {
            if (fun_arr[i](cmd, cmd_typ, param))
                return HL_FAILED;
            return HL_SUCCESS;
        }
    }
    return HL_FAILED;
}

/**
 * 
 * @brief nau88l25b配置操作函数接口
 * @param [in] cmd 配置项
 * @param [in] ptr 配置参数指针，类型应该为 HL_NAU_INPUT_PARAM_T
 * @param [in] len 配置参数个数
 * @return uint8_t 0成功，1失败
 * @date 2022-09-08
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-08      <td>dujunjie     <td>新建
 * </table>
 */
uint8_t hl_drv_nau88l25b_io_ctrl(uint8_t cmd, void* ptr, uint16_t len)
{
    uint8_t               cmd_typ = 0, reg_serial = 0;
    uint16_t              cmd_reg = 0;
    HL_NAU_INPUT_PARAM_T* param   = NULL;
    if (ptr == NULL || len != 1) {
        nau_printf("hl_drv_nau88l25b_io_ctrl param err !\n");
        goto CTL_ERR;
    }
    param      = (HL_NAU_INPUT_PARAM_T*)ptr;
    cmd_typ    = cmd;
    cmd_reg    = param->cfg_opt;
    reg_serial = hl_get_reg_serial(param->cfg_opt);

    if (hl_reg_ctl(reg_serial, cmd_reg, cmd_typ, &param->param))
        return HL_FAILED;
    return HL_SUCCESS;
CTL_ERR:
    nau_printf("hl_drv_nau88l25b_io_ctrl fail !\n");
    return HL_FAILED;
}

/**
 * 
 * @brief nau88l25b初始化函数
 * @return uint8_t 0成功，1失败
 * @date 2022-09-08
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-08      <td>dujunjie     <td>新建
 * </table>
 */
uint8_t hl_drv_nau88l25b_init(void)
{
    uint16_t i, j = sizeof(s_nau88l25b_param) / 4;
    nau_printf("nau cfg_opt num : %d \n", j);
    /* 查找I2C总线设备，获取I2C总线设备句柄 */
    i2c_bus = (struct rt_i2c_bus_device*)rt_device_find(NAU88L25B_IIC_NAME);

    if (i2c_bus == RT_NULL) {
        nau_printf("can't find %s device!\n", NAU88L25B_IIC_NAME);
        goto INIT_ERR;
    }
    hl_reg_arr_init();
    hl_reg_ctl_fun_init();
    hl_reg00_ctl();
    for (i = 0; i < j; i++) {
        if (hl_i2c_write_reg(i2c_bus, s_nau88l25b_param[i].reg_addr, &s_nau88l25b_param[i].reg_value)) {
            nau_printf("nau cfg_opt init fail !\n");
            goto INIT_ERR;
        }
    }
    nau_printf("hl_drv_nau88l25b_init success !\n");
    return HL_SUCCESS;
INIT_ERR:
    nau_printf("hl_drv_nau88l25b_init fail !\n");
    hl_reg_arr_deinit();
    hl_reg_ctl_fun_deinit();
    hl_reg00_ctl();
    return HL_FAILED;
}

void tt25(int argc, char** argv)
{
    if (argc != 4) {
        nau_printf("argc param err : %d \n", argc);
        return;
    }

    uint16_t rw      = atoi(argv[1]);
    uint16_t cfg_opt = atoi(argv[2]);
    int      data    = atoi(argv[3]);

    HL_NAU_INPUT_PARAM_T par;
    par.cfg_opt = cfg_opt;
    par.param   = data;

    i2c_bus = (struct rt_i2c_bus_device*)rt_device_find(NAU88L25B_IIC_NAME);

    hl_reg_arr_init();
    hl_reg_ctl_fun_init();
    // hl_reg00_ctl();

    if (i2c_bus == RT_NULL) {
        nau_printf("can't find %s device!\n", NAU88L25B_IIC_NAME);
    }
    if (hl_drv_nau88l25b_io_ctrl(rw, &par, 1) == HL_FAILED) {
        nau_printf("hl_drv_nau88l25b_io_ctrl fail!\n");
    }
    nau_printf("param = %d \n", par.param);
}

MSH_CMD_EXPORT(tt25, run tt25);
