/**
 * @file hl_drv_sgm41518.c
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief sgm41518芯片驱动.c文件
 * @version 1.0
 * @date 2022-09-05
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-05     <td>v1.0     <td>dujunjie     <td>内容
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
#include "hl_drv_sgm41518.h"

#define SMG41518_DEBUG

#ifdef SMG41518_DEBUG
#define smg_printf(...) rt_kprintf(__VA_ARGS__)
#else
#define smg_printf(...)
#endif

typedef uint8_t (*reg_ctl_fun)(uint8_t cmd, uint8_t cmd_typ, uint8_t* param);
static const uint8_t             READ_CMD  = 0x00;
static const uint8_t             WRITE_CMD = 0x01;
static reg_ctl_fun               fun_arr[0x10];
static uint8_t                   reg_arr[0x10];
static struct rt_i2c_bus_device* i2c_bus = RT_NULL; /* I2C总线设备句柄 */

/**
 * 
 * @brief i2c总线写数据
 * @param [in] bus i2c总线句柄
 * @param [in] reg 操作的寄存器
 * @param [in] data 数据
 * @return rt_err_t 0成功，1失败
 * @date 2022-09-05
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 一次只能写入一字节数据
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-05      <td>dujunjie     <td>新建
 * </table>
 */
static rt_err_t i2c_write_reg(struct rt_i2c_bus_device* bus, rt_uint8_t reg, rt_uint8_t* data)
{
    rt_uint8_t        buf[4];
    struct rt_i2c_msg msgs[2];

    buf[0] = reg;      // reg
    buf[1] = data[0];  // data

    msgs[0].addr  = SGM41518_DEVICE_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = buf;
    msgs[0].len   = 1;

    msgs[1].addr  = SGM41518_DEVICE_ADDRESS;
    msgs[1].flags = RT_I2C_WR | RT_I2C_NO_START;
    msgs[1].buf   = &buf[1];
    msgs[1].len   = 1;

    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(bus, &msgs, 2) == 1)
        return HL_SUCCESS;
    else
        return HL_FAILED;
}

/**
 * 
 * @brief i2c总线读数据
 * @param [in] bus i2c总线句柄
 * @param [in] reg 操作的寄存器
 * @param [in] rbuf 数据存放
 * @return rt_err_t 0成功，1失败
 * @date 2022-09-05
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 一次只能读取一字节数据
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-05      <td>dujunjie     <td>新建
 * </table>
 */
static rt_err_t i2c_read_reg(struct rt_i2c_bus_device* bus, rt_uint8_t reg, rt_uint8_t* rbuf)
{
    rt_uint8_t        buf[4];
    struct rt_i2c_msg msgs[2];

    msgs[0].addr  = SGM41518_DEVICE_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &reg;
    msgs[0].len   = 1;

    msgs[1].addr  = SGM41518_DEVICE_ADDRESS;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = buf;
    msgs[1].len   = 1;

    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(bus, &msgs, 2) == 1) {
        rbuf[0] = buf[1];
        return HL_SUCCESS;
    } else
        return HL_FAILED;
}

/**
 * 
 * @brief 0x00寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
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
static uint8_t reg00_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (i2c_read_reg(i2c_bus, REG00_ADDR, (uint8_t*)&reg_all.reg00)) {
        smg_printf("reg00_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == READ_CMD) {
        smg_printf("reg0 command : [ %X ] type err !\n", cmd);
        goto CTL_ERR;
    }
    switch (cmd) {
        case RW_EN_HIZ_MOD:
            reg_all.reg00.EN_HIZ = param[0];
            break;
        case RW_EN_ICHG_MON:
            reg_all.reg00.EN_ICHG_MON = param[0];
            break;
        case RW_IINDPM_SET:
            reg_all.reg00.IINDPM = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (i2c_write_reg(i2c_bus, REG00_ADDR, (uint8_t*)&reg_all.reg00)) {
        smg_printf("reg00_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] reg00_ctl fail !\n", param[0]);
    return HL_FAILED;
}

/**
 * 
 * @brief 0x01寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
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
static uint8_t reg01_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (i2c_read_reg(i2c_bus, REG01_ADDR, (uint8_t*)&reg_all.reg01)) {
        smg_printf("reg01_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == READ_CMD) {
        smg_printf("reg1 command : [ %X ] type err !\n", cmd);
        goto CTL_ERR;
    }
    switch (cmd) {
        case RW_EN_PFM:
            reg_all.reg01.PFM_DIS = param[0];
            break;
        case RW_WDG_RST:
            reg_all.reg01.WD_RST = param[0];
            break;
        case RW_EN_OTG:
            reg_all.reg01.PFM_DIS = param[0];
            break;
        case RW_EN_BAT_CHARGING:
            reg_all.reg01.PFM_DIS = param[0];
            break;
        case RW_SYS_MIN:
            reg_all.reg01.PFM_DIS = param[0];
            break;
        case RW_MIN_BAT_SEL:
            reg_all.reg01.PFM_DIS = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (i2c_write_reg(i2c_bus, REG01_ADDR, (uint8_t*)&reg_all.reg01)) {
        smg_printf("reg01_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] reg01_ctl fail !\n", param[0]);
    return HL_FAILED;
}

/**
 * 
 * @brief 0x02寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
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
static uint8_t reg02_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (i2c_read_reg(i2c_bus, REG02_ADDR, (uint8_t*)&reg_all.reg02)) {
        smg_printf("reg02_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == READ_CMD) {
        smg_printf("reg2 command : [ %X ] type err !\n", cmd);
        goto CTL_ERR;
    }
    switch (cmd) {
        case RW_Q1_FULLON:
            reg_all.reg02.Q1_FULLON = param[0];
            break;
        case RW_CHARGE_CURRENT:
            reg_all.reg02.ICHG = FAST_CHARGE_CURRENT_SET(param[0]);
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (i2c_write_reg(i2c_bus, REG02_ADDR, (uint8_t*)&reg_all.reg02)) {
        smg_printf("reg02_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] reg02_ctl fail !\n", param[0]);
    return HL_FAILED;
}

/**
 * 
 * @brief 0x03寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
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
static uint8_t reg03_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (i2c_read_reg(i2c_bus, REG03_ADDR, (uint8_t*)&reg_all.reg03)) {
        smg_printf("reg03_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == READ_CMD) {
        smg_printf("reg3 command : [ %X ] type err !\n", cmd);
        goto CTL_ERR;
    }
    switch (cmd) {
        case RW_PRECHARGE_CURRENT_LIMIT:
            reg_all.reg03.IPRECHG = PRE_CHARGE_SET(param[0]);
            break;
        case RW_TER_CURRENT_LIMIT:
            reg_all.reg03.ITERM = TER_CURRENT_SET(param[0]);
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (i2c_write_reg(i2c_bus, REG03_ADDR, (uint8_t*)&reg_all.reg03)) {
        smg_printf("reg03_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] reg03_ctl fail !\n", param[0]);
    return HL_FAILED;
}

/**
 * 
 * @brief 0x04寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
 * @date 2022-09-06
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-06      <td>dujunjie     <td>新建
 * </table>
 */
static uint8_t reg04_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (i2c_read_reg(i2c_bus, REG04_ADDR, (uint8_t*)&reg_all.reg04)) {
        smg_printf("reg04_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == READ_CMD) {
        smg_printf("reg4 command : [ %X ] type err !\n", cmd);
        goto CTL_ERR;
    }
    switch (cmd) {
        case RW_CHARGE_VOLTAGE_LIMIT:
            reg_all.reg04.VREG = CHARGE_VOLTAGE_LIMIT(param[0]);
            break;
        case RW_TOPOFF_TIME:
            reg_all.reg04.TOPOFF_TIMER = param[0];
            break;
        case RW_RECHARGE_THRESHOLD:
            reg_all.reg04.VRECHG = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (i2c_write_reg(i2c_bus, REG04_ADDR, (uint8_t*)&reg_all.reg04)) {
        smg_printf("reg04_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] reg04_ctl fail !\n", param[0]);
    return HL_FAILED;
}

/**
 * 
 * @brief 0x05寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
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
static uint8_t reg05_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (i2c_read_reg(i2c_bus, REG05_ADDR, (uint8_t*)&reg_all.reg05)) {
        smg_printf("reg05_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == READ_CMD) {
        smg_printf("reg5 command : [ %X ] type err !\n", cmd);
        goto CTL_ERR;
    }
    switch (cmd) {
        case RW_CHARGE_TER_ENABLE:
            reg_all.reg05.EN_TERM = param[0];
            break;
        case RW_WDOG_TIMER_CHG:
            reg_all.reg05.CHG_TIMER = param[0];
            break;
        case RW_CHARGE_SAFETY_TIMER_ENABLE:
            reg_all.reg05.EN_TIMER = param[0];
            break;
        case RW_CHARGE_SAFETY_TIMER_CHG:
            reg_all.reg05.CHG_TIMER = param[0];
            break;
        case RW_TER_REGULATION_THRESHOLD:
            reg_all.reg05.TREG = param[0];
            break;
        case RW_JEITA_CHARGING_CURRENT:
            reg_all.reg05.JEITA_ISET = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (i2c_write_reg(i2c_bus, REG05_ADDR, (uint8_t*)&reg_all.reg05)) {
        smg_printf("reg05_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] reg05_ctl fail !\n", param[0]);
    return HL_FAILED;
}

/**
 * 
 * @brief 0x06寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
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
static uint8_t reg06_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (i2c_read_reg(i2c_bus, REG06_ADDR, (uint8_t*)&reg_all.reg06)) {
        smg_printf("reg06_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == READ_CMD) {
        smg_printf("reg6 command : [ %X ] type err !\n", cmd);
        goto CTL_ERR;
    }
    switch (cmd) {
        case RW_OVP_CHG:
            reg_all.reg06.OVP = param[0];
            break;
        case RW_BOOSTV_CHG:
            reg_all.reg06.BOOSTV = param[0];
            break;
        case RW_VINDPM_CHG:
            reg_all.reg06.VINDPM = VINDPM_THRESHOLD(param[0]);
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (i2c_write_reg(i2c_bus, REG06_ADDR, (uint8_t*)&reg_all.reg06)) {
        smg_printf("reg06_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] reg06_ctl fail !\n", param[0]);
    return HL_FAILED;
}

/**
 * 
 * @brief 0x07寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
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
static uint8_t reg07_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (i2c_read_reg(i2c_bus, REG07_ADDR, (uint8_t*)&reg_all.reg07)) {
        smg_printf("reg07_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == READ_CMD) {
        smg_printf("reg7 command : [ %X ] type err !\n", cmd);
        goto CTL_ERR;
    }
    switch (cmd) {
        case RW_INPUT_CRRENT_LIMIT_DETECTION:
            reg_all.reg07.IINDET_EN = param[0];
            break;
        case RW_TMR2X_EN:
            reg_all.reg07.TMR2X_EN = param[0];
            break;
        case RW_BATFET_DIS:
            reg_all.reg07.BATFET_DIS = param[0];
            break;
        case RW_JEITA_VSET_H:
            reg_all.reg07.JEITA_VSET = param[0];
            break;
        case RW_BATFET_DLY:
            reg_all.reg07.BATFET_DLY = param[0];
            break;
        case RW_BATFET_RST_EN:
            reg_all.reg07.BATFET_RST_EN = param[0];
            break;
        case RW_VDPM_BAT_TRACK:
            reg_all.reg07.VDPM_BAT_TRACK = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (i2c_write_reg(i2c_bus, REG07_ADDR, (uint8_t*)&reg_all.reg07)) {
        smg_printf("reg07_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] reg07_ctl fail !\n", param[0]);
    return HL_FAILED;
}

/**
 * 
 * @brief 0x08寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
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
static uint8_t reg08_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (i2c_read_reg(i2c_bus, REG08_ADDR, (uint8_t*)&reg_all.reg08)) {
        smg_printf("reg08_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == READ_CMD) {
        switch (cmd) {
            case R_VBUS_STAT:
                param[0] = reg_all.reg08.VBUS_STAT;
                break;
            case R_CHRG_STAT:
                param[0] = reg_all.reg08.CHRG_STAT;
                break;
            case R_PG_STAT:
                param[0] = reg_all.reg08.PG_STAT;
                break;
            case R_THERM_STAT:
                param[0] = reg_all.reg08.THERM_STAT;
                break;
            case R_VSYS_STAT:
                param[0] = reg_all.reg08.VSYS_STAT;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    } else {
        smg_printf("reg8 command : [ %X ] type err !\n", cmd);
        goto CTL_ERR;
    }
    if (i2c_write_reg(i2c_bus, REG08_ADDR, (uint8_t*)&reg_all.reg08)) {
        smg_printf("reg08_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] reg08_ctl fail !\n", param[0]);
    return HL_FAILED;
}

/**
 * 
 * @brief 0x09寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
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
static uint8_t reg09_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (i2c_read_reg(i2c_bus, REG09_ADDR, (uint8_t*)&reg_all.reg09)) {
        smg_printf("reg09_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == READ_CMD) {
        switch (cmd) {
            case R_WDG_FAULT:
                param[0] = reg_all.reg09.WATCHDOG_FAULT;
                break;
            case R_BOOST_FAULT:
                param[0] = reg_all.reg09.BOOST_FAULT;
                break;
            case R_CHRG_FAULT:
                param[0] = reg_all.reg09.CHRG_FAULT;
                break;
            case R_BAT_FAULT:
                param[0] = reg_all.reg09.BAT_FAULT;
                break;
            case R_NTC_FAULT:
                param[0] = reg_all.reg09.NTC_FAULT;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    } else {
        smg_printf("reg9 command : [ %X ] type err !\n", cmd);
        goto CTL_ERR;
    }
    if (i2c_write_reg(i2c_bus, REG09_ADDR, (uint8_t*)&reg_all.reg09)) {
        smg_printf("reg09_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] reg09_ctl fail !\n", param[0]);
    return HL_FAILED;
}

/**
 * 
 * @brief 0x0A寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
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
static uint8_t reg0A_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (i2c_read_reg(i2c_bus, REG0A_ADDR, (uint8_t*)&reg_all.reg0A)) {
        smg_printf("reg0A_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == READ_CMD) {
        switch (cmd) {
            case RW_VINDPM_INT_MASK:
                param[0] = reg_all.reg0A.VINDPM_INT_MASK;
                break;
            case RW_IINDPM_INT_MASK:
                param[0] = reg_all.reg0A.IINDPM_INT_MASK;
                break;
            case R_VBUS_GD:
                param[0] = reg_all.reg0A.VBUS_GD;
                break;
            case R_VINDPM_STAT:
                param[0] = reg_all.reg0A.VINDPM_STAT;
                break;
            case R_IINDPM_STAT:
                param[0] = reg_all.reg0A.IINDPM_STAT;
                break;
            case R_TOPOFF_ACTIVE:
                param[0] = reg_all.reg0A.TOPOFF_ACTIVE;
                break;
            case R_ACOV_STAT:
                param[0] = reg_all.reg0A.ACOV_STAT;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case RW_VINDPM_INT_MASK:
            reg_all.reg0A.VINDPM_INT_MASK = param[0];
            break;
        case RW_IINDPM_INT_MASK:
            reg_all.reg0A.IINDPM_INT_MASK = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (i2c_write_reg(i2c_bus, REG0A_ADDR, (uint8_t*)&reg_all.reg0A)) {
        smg_printf("reg0A_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] reg0A_ctl fail !\n", param[0]);
    return HL_FAILED;
}

/**
 * 
 * @brief 0x0B寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
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
static uint8_t reg0B_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (i2c_read_reg(i2c_bus, REG0B_ADDR, (uint8_t*)&reg_all.reg0B)) {
        smg_printf("reg0B_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == READ_CMD) {
        switch (cmd) {
            case RW_REG_RST:
                param[0] = reg_all.reg0B.REG_RST;
                break;
            case R_DEVICE_ID:
                param[0] = reg_all.reg0B.PN;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case RW_REG_RST:
            reg_all.reg0B.REG_RST = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (i2c_write_reg(i2c_bus, REG0B_ADDR, (uint8_t*)&reg_all.reg0B)) {
        smg_printf("reg0B_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] reg0B_ctl fail !\n", param[0]);
    return HL_FAILED;
}

/**
 * 
 * @brief 0x0C寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
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
static uint8_t reg0C_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (i2c_read_reg(i2c_bus, REG0C_ADDR, (uint8_t*)&reg_all.reg0C)) {
        smg_printf("reg0C_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == READ_CMD) {
        smg_printf("regC command : [ %X ] type err !\n", cmd);
        goto CTL_ERR;
    }
    switch (cmd) {
        case RW_JEITA_VSET_L:
            reg_all.reg0C.JEITA_VSET_L = param[0];
            break;
        case RW_JEITA_ISET_L_EN:
            reg_all.reg0C.JEITA_ISET_L_EN = param[0];
            break;
        case RW_JEITA_ISET_H:
            reg_all.reg0C.JEITA_ISET_H = param[0];
            break;
        case RW_JEITA_VT2:
            reg_all.reg0C.JEITA_VT2 = param[0];
            break;
        case RW_JEITA_VT3:
            reg_all.reg0C.JEITA_VT3 = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (i2c_write_reg(i2c_bus, REG0C_ADDR, (uint8_t*)&reg_all.reg0C)) {
        smg_printf("reg0C_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] reg0C_ctl fail !\n", param[0]);
    return HL_FAILED;
}

/**
 * 
 * @brief 0x0D寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
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
static uint8_t reg0D_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (i2c_read_reg(i2c_bus, REG0D_ADDR, (uint8_t*)&reg_all.reg0D)) {
        smg_printf("reg0D_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == READ_CMD) {
        smg_printf("regD command : [ %X ] type err !\n", cmd);
        goto CTL_ERR;
    }
    switch (cmd) {
        case RW_JEITA_EN:
            reg_all.reg0D.JEITA_EN = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (i2c_write_reg(i2c_bus, REG0D_ADDR, (uint8_t*)&reg_all.reg0D)) {
        smg_printf("reg0D_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] reg0D_ctl fail !\n", param[0]);
    return HL_FAILED;
}

/**
 * 
 * @brief 0x0E寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
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
static uint8_t reg0E_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    return HL_SUCCESS;
}

/**
 * 
 * @brief 0x0F寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读/写
 * @param [in] param 参数
 * @return uint8_t 0成功，1失败
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
static uint8_t reg0F_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (i2c_read_reg(i2c_bus, REG0F_ADDR, (uint8_t*)&reg_all.reg0F)) {
        smg_printf("reg0F_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == READ_CMD) {
        smg_printf("regF command : [ %X ] type err !\n", cmd);
        goto CTL_ERR;
    }
    switch (cmd) {
        case RW_VREG_FT:
            reg_all.reg0F.VREG_FT = param[0];
            break;
        case RW_STAT_SET:
            reg_all.reg0F.STATSET = param[0];
            break;
        case RW_VINDPM_OS:
            reg_all.reg0F.VINDPM_OS = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (i2c_write_reg(i2c_bus, REG0F_ADDR, (uint8_t*)&reg_all.reg0F)) {
        smg_printf("reg0F_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] reg0F_ctl fail !\n", param[0]);
    return HL_FAILED;
}

/**
 * 
 * @brief 获取寄存器地址
 * @param [in] reg 带解析的寄存器指令
 * @return uint8_t 寄存器地址
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
static uint8_t get_reg_serial(uint8_t reg)
{
    uint8_t reg_ser;
    if (reg < RW_EN_PFM)
        reg_ser = REG00_ADDR;
    else if (reg < RW_Q1_FULLON)
        reg_ser = REG01_ADDR;
    else if (reg < RW_PRECHARGE_CURRENT_LIMIT)
        reg_ser = REG02_ADDR;
    else if (reg < RW_CHARGE_VOLTAGE_LIMIT)
        reg_ser = REG03_ADDR;
    else if (reg < RW_CHARGE_TER_ENABLE)
        reg_ser = REG04_ADDR;
    else if (reg < RW_OVP_CHG)
        reg_ser = REG05_ADDR;
    else if (reg < RW_INPUT_CRRENT_LIMIT_DETECTION)
        reg_ser = REG06_ADDR;
    else if (reg < RW_VINDPM_INT_MASK)
        reg_ser = REG07_ADDR;
    else if (reg < RW_REG_RST)
        reg_ser = REG0A_ADDR;
    else if (reg < RW_JEITA_VSET_L)
        reg_ser = REG0B_ADDR;
    else if (reg < RW_JEITA_EN)
        reg_ser = REG0C_ADDR;
    else if (reg < RW_VREG_FT)
        reg_ser = REG0D_ADDR;
    else if (reg < RW_CNT_MAX)
        reg_ser = REG0F_ADDR;
    else if (reg < R_WDG_FAULT)
        reg_ser = REG08_ADDR;
    else if (reg < R_VBUS_GD)
        reg_ser = REG09_ADDR;
    else if (reg < R_DEVICE_ID)
        reg_ser = REG0A_ADDR;
    else if (reg < R_CNT_MAX)
        reg_ser = REG0B_ADDR;
    else {
        smg_printf("Register : [ %X ] overrun !\n", reg);
        return 0xFF;
    }
    return reg_ser;
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
static void reg_ctl_fun_init(void)
{
    fun_arr[0]  = reg00_ctl;
    fun_arr[1]  = reg01_ctl;
    fun_arr[2]  = reg02_ctl;
    fun_arr[3]  = reg03_ctl;
    fun_arr[4]  = reg04_ctl;
    fun_arr[5]  = reg05_ctl;
    fun_arr[6]  = reg06_ctl;
    fun_arr[7]  = reg07_ctl;
    fun_arr[8]  = reg08_ctl;
    fun_arr[9]  = reg09_ctl;
    fun_arr[10] = reg0A_ctl;
    fun_arr[11] = reg0B_ctl;
    fun_arr[12] = reg0C_ctl;
    fun_arr[13] = reg0D_ctl;
    fun_arr[14] = reg0E_ctl;
    fun_arr[15] = reg0F_ctl;
}

/**
 * 
 * @brief 注销已注册的寄存器操作函数
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
static void reg_ctl_fun_deinit(void)
{
    uint8_t i;
    for (i = 0; i <= 0x0F; i++)
        fun_arr[i] = NULL;
}

/**
 * 
 * @brief 初始化寄存器数组
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
static void reg_arr_init(void)
{
    uint8_t i;
    for (i = 0; i <= 0x0F; i++)
        reg_arr[i] = i;
}

/**
 * 
 * @brief 注销寄存器数组
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
static void reg_arr_deinit(void)
{
    uint8_t i;
    for (i = 0; i <= 0x0F; i++)
        reg_arr[i] = 0;
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
static uint8_t reg_ctl(uint8_t reg_ser, uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    uint8_t i;
    for (i = 0; i <= 0x0F; i++) {
        if (reg_arr[i] == reg_ser) {
            if (fun_arr[i](cmd, cmd_typ, param))
                return HL_FAILED;
        }
    }
    return HL_SUCCESS;
}

/**
 * 
 * @brief 获取命令类型
 * @param [in] cmd 待解析命令
 * @return uint8_t 0读取，1写入
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
static uint8_t command_type_get(uint8_t cmd)
{
    if (cmd < CMD_MASK)
        return READ_CMD;
    return WRITE_CMD;
}

/**
 * 
 * @brief 获取寄存器可操作类型
 * @param [in] cmd 待解析寄存器
 * @return uint8_t 0只读，1读写
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
static uint8_t reg_type_get(uint8_t cmd)
{
    if (cmd < RW_CNT_MAX)
        return WRITE_CMD;
    return READ_CMD;
}

/**
 * 
 * @brief smg41518配置操作函数
 * @param [in] cmd 配置项，输入时请用 W_CMD_SET() 或者 R_CMD_SET() 处理输入
 * @param [in] ptr 配置参数指针
 * @param [in] len 配置参数个数
 * @return uint8_t 0成功，1失败
 * @date 2022-09-05
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 只支持一字节长度，每次只能配置一个功能参数
 *          写命令使用：W_CMD_SET() 处理
 *          读命令使用：R_CMD_SET() 处理
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-05      <td>dujunjie     <td>新建
 * </table>
 */
uint8_t hl_drv_sgm41518_io_ctrl(uint8_t cmd, uint8_t* ptr, uint8_t len)
{
    uint8_t reg_cmd, reg_serial;
    uint8_t cmd_typ = command_type_get(cmd);
    if (ptr == NULL || len != 1) {
        smg_printf("Param err ! len : [ %X ]\n", len);
        return HL_FAILED;
    }
    if (cmd_typ != reg_type_get(cmd)) {
        smg_printf("Register : [ %X ] type err !\n", cmd);
        return HL_FAILED;
    }
    reg_cmd    = cmd & (~CMD_MASK);
    reg_serial = get_reg_serial(reg_cmd);
    if (cmd_typ == WRITE_CMD && (reg_cmd == REG08_ADDR || reg_cmd == REG09_ADDR)) {
        smg_printf("Register : [ %X ] type err !\n", cmd);
        return HL_FAILED;
    }

    if (reg_ctl(reg_serial, reg_cmd, cmd_typ, ptr))
        return HL_FAILED;
    return HL_SUCCESS;
}

/**
 * 
 * @brief sgm41518初始化函数
 * @return uint8_t 0成功，1失败
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
uint8_t hl_drv_sgm41518_init(void)
{
    HL_SGM41518_REGALL_T reg_all;

    /* 查找I2C总线设备，获取I2C总线设备句柄 */
    i2c_bus = (struct rt_i2c_bus_device*)rt_device_find(SGM41518_IIC_NAME);

    reg_arr_init();
    reg_ctl_fun_init();

    if (i2c_bus == RT_NULL) {
        smg_printf("can't find %s device!\n", SGM41518_IIC_NAME);
        goto INIT_ERR;
    }

    if (i2c_read_reg(i2c_bus, REG00_ADDR, (uint8_t*)&reg_all.reg00)) {
        smg_printf("reg %d read err !\n", REG00_ADDR);
        goto INIT_ERR;
    }
    if (i2c_read_reg(i2c_bus, REG01_ADDR, (uint8_t*)&reg_all.reg01)) {
        smg_printf("reg %d read err !\n", REG01_ADDR);
        goto INIT_ERR;
    }
    if (i2c_read_reg(i2c_bus, REG02_ADDR, (uint8_t*)&reg_all.reg02)) {
        smg_printf("reg %d read err !\n", REG02_ADDR);
        goto INIT_ERR;
    }
    if (i2c_read_reg(i2c_bus, REG03_ADDR, (uint8_t*)&reg_all.reg03)) {
        smg_printf("reg %d read err !\n", REG03_ADDR);
        goto INIT_ERR;
    }
    if (i2c_read_reg(i2c_bus, REG04_ADDR, (uint8_t*)&reg_all.reg04)) {
        smg_printf("reg %d read err !\n", REG04_ADDR);
        goto INIT_ERR;
    }
    if (i2c_read_reg(i2c_bus, REG05_ADDR, (uint8_t*)&reg_all.reg05)) {
        smg_printf("reg %d read err !\n", REG05_ADDR);
        goto INIT_ERR;
    }
    if (i2c_read_reg(i2c_bus, REG06_ADDR, (uint8_t*)&reg_all.reg06)) {
        smg_printf("reg %d read err !\n", REG06_ADDR);
        goto INIT_ERR;
    }
    if (i2c_read_reg(i2c_bus, REG07_ADDR, (uint8_t*)&reg_all.reg07)) {
        smg_printf("reg %d read err !\n", REG07_ADDR);
        goto INIT_ERR;
    }
    if (i2c_read_reg(i2c_bus, REG0C_ADDR, (uint8_t*)&reg_all.reg0C)) {
        smg_printf("reg %d read err !\n", REG0C_ADDR);
        goto INIT_ERR;
    }

    reg_all.reg00.IINDPM      = IINDPM_SET(31);            // 3200mA
    reg_all.reg00.EN_ICHG_MON = FOLLOE_CHARG_STAT_ENABLE;  //STAT引脚状态由充电状态决定
    reg_all.reg00.EN_HIZ      = HIZ_DISABLE;               //HIZ模式禁用

    reg_all.reg01.MIN_BAT_SEL = MIN_BOOST_VOLTAGE_2v95;  //升压模式最小系统电压2.95V
    reg_all.reg01.SYS_MIN     = SYS_MIN_3V5;             //默认最小系统电压3.5V
    reg_all.reg01.CHG_CONFIG  = CHARGE_ENABLE;           //充电使能
    reg_all.reg01.OTG_CONFIG  = OTG_DISABLE;             //关闭OTG充电功能
    reg_all.reg01.WD_RST      = WDG_TIMEOUT;             //看门狗初始化复位
    reg_all.reg01.PFM_DIS     = PFM_DISABLE;             //禁用PFM模式

    reg_all.reg02.ICHG = FAST_CHARGE_CURRENT_SET(63);  // 1280mA
    reg_all.reg02.Q1_FULLON = Q1_USE_HIGHER_RDSON;  //控制开关阻抗，输入电流<700ma时建议设置成 0，通用设置为1

    reg_all.reg03.ITERM   = TER_CURRENT_SET(0);  // 设置终止电流20mA
    reg_all.reg03.IPRECHG = PRE_CHARGE_SET(12);  // 设置预充电电流260mA

    reg_all.reg04.VRECHG       = RECHARGE_THRESHOLD_100;    //充电开始阈值：VREG - 100mv
    reg_all.reg04.TOPOFF_TIMER = TOP_TIMER_DISABLE;         //关闭充电延长时间
    reg_all.reg04.VREG         = CHARGE_VOLTAGE_LIMIT(12);  // 充电电压限制4.24V

    reg_all.reg05.JEITA_ISET = JEITA_CURRENT_SET_L_50;      //低温时（0℃ - 10 ℃）充电电流 I = 50% * ICHG
    reg_all.reg05.TREG       = THERNAL_REGT_THRESHOLD_120;  //设置热调节阈值 120℃
    reg_all.reg05.CHG_TIMER  = CHARGE_SAFET_TIMER_SET_10H;  //充电安全计数器时间设置为10h
    reg_all.reg05.EN_TIMER   = CHARGE_SAFET_TIMER_ENABLE;   //使能充电安全计数器
    reg_all.reg05.WATCHDOG   = WDG_TIMER_DISABLE;           //禁用看门狗
    reg_all.reg05.EN_TERM    = CHARGE_TER_ENABLE;           //充电终止使能

    reg_all.reg06.VINDPM = VINDPM_THRESHOLD(6);       //VINDPM电压阈值 : 4.5V
    reg_all.reg06.BOOSTV = BOOST_VOL_THRESHOLD_5V15;  //boost模式调压设置5.15V
    reg_all.reg06.OVP    = VAC_OVP_THRESHOLD_6V5;     //设置VAC检测输入电压阈值 6.5V（5V输入时）

    reg_all.reg07.VDPM_BAT_TRACK = VDPM_BAT_TRACK_DISABLE;  //关闭动态VUNDPM跟踪功能
    reg_all.reg07.BATFET_RST_EN  = BATFET_RST_ENABLE;       //启用BATFET重置功能
    reg_all.reg07.BATFET_DLY     = BATFET_TSM_DLY_ENABLE;   //关闭BATFET时，延时Tsm_dly时间后再关闭
    reg_all.reg07.JEITA_VSET     = JEITA_VSET_H_SET_VREG;   //设置充电电压为VREG（45℃ - 60℃）
    reg_all.reg07.BATFET_DIS     = BATFET_ENABLE;           //允许BATFET打开
    reg_all.reg07.TMR2X_EN       = TMR2X_ENABLE;            //开启安全计数器减速计数模式
    reg_all.reg07.IINDET_EN      = IINDET_ENABLE;           //使能输入电流限制检测

    reg_all.reg0C.JEITA_VT3       = JEITA_WARM_VT3_44C5;    //温暖阈值T3设置为 44.5℃
    reg_all.reg0C.JEITA_VT2       = JEITA_COOL_VT2_10C;     //寒冷阈值T2设置为 10℃
    reg_all.reg0C.JEITA_ISET_H    = JEITA_WARM_ISET_100;    //温暖温度设置充电电流 100% * ICHG
    reg_all.reg0C.JEITA_ISET_L_EN = JEITA_ISET_L_ENABLE;    //开启低温充电
    reg_all.reg0C.JEITA_VSET_L    = JEITA_VSET_L_SET_VREG;  //设置低温区间（0℃ - 10℃）充电电压等于VREG

    if (i2c_write_reg(i2c_bus, REG00_ADDR, (uint8_t*)&reg_all.reg00)) {
        smg_printf("reg %d write err !\n", REG00_ADDR);
        goto INIT_ERR;
    }
    if (i2c_write_reg(i2c_bus, REG01_ADDR, (uint8_t*)&reg_all.reg01)) {
        smg_printf("reg %d write err !\n", REG01_ADDR);
        goto INIT_ERR;
    }
    if (i2c_write_reg(i2c_bus, REG02_ADDR, (uint8_t*)&reg_all.reg02)) {
        smg_printf("reg %d write err !\n", REG02_ADDR);
        goto INIT_ERR;
    }
    if (i2c_write_reg(i2c_bus, REG03_ADDR, (uint8_t*)&reg_all.reg03)) {
        smg_printf("reg %d write err !\n", REG03_ADDR);
        goto INIT_ERR;
    }
    if (i2c_write_reg(i2c_bus, REG04_ADDR, (uint8_t*)&reg_all.reg04)) {
        smg_printf("reg %d write err !\n", REG04_ADDR);
        goto INIT_ERR;
    }
    if (i2c_write_reg(i2c_bus, REG05_ADDR, (uint8_t*)&reg_all.reg05)) {
        smg_printf("reg %d write err !\n", REG05_ADDR);
        goto INIT_ERR;
    }
    if (i2c_write_reg(i2c_bus, REG06_ADDR, (uint8_t*)&reg_all.reg06)) {
        smg_printf("reg %d write err !\n", REG06_ADDR);
        goto INIT_ERR;
    }
    if (i2c_write_reg(i2c_bus, REG07_ADDR, (uint8_t*)&reg_all.reg07)) {
        smg_printf("reg %d write err !\n", REG07_ADDR);
        goto INIT_ERR;
    }
    if (i2c_write_reg(i2c_bus, REG0C_ADDR, (uint8_t*)&reg_all.reg0C)) {
        smg_printf("reg %d write err !\n", REG0C_ADDR);
        goto INIT_ERR;
    }
    rt_thread_mdelay(50);
    if (i2c_write_reg(i2c_bus, REG00_ADDR, (uint8_t*)&reg_all.reg00)) {
        smg_printf("reg %d write err !\n", REG00_ADDR);
        goto INIT_ERR;
    }
    smg_printf("smg41518 init success !\n");
    return HL_SUCCESS;
INIT_ERR:
    smg_printf("smg41518 init fail !\n");
    reg_arr_deinit();
    reg_ctl_fun_deinit();
    return HL_FAILED;
}

INIT_DEVICE_EXPORT(hl_drv_sgm41518_init);
