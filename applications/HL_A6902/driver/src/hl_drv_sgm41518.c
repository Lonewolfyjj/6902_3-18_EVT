/**
 * @file hl_drv_sgm41518.c
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief sgm41518芯片驱动.c文件
 * @version 1.0
 * @date 2022-09-27
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
 * <tr><td>2022-09-27     <td>v1.0     <td>dujunjie     <td>内容
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
#if 0
#include <rtthread.h>
#include <rtdevice.h>
#include "hl_drv_sgm41518.h"

#define SMG41518_DEBUG

#ifdef SMG41518_DEBUG
#define smg_printf(...) rt_kprintf(__VA_ARGS__)
#else
#define smg_printf(...)
#endif

typedef uint8_t (*hl_reg_ctl_fun)(uint8_t cmd, uint8_t cmd_typ, uint8_t* param);
static hl_reg_ctl_fun            fun_arr[SGM_REG_NUM];
static uint8_t                   reg_arr[SGM_REG_NUM];
static struct rt_i2c_bus_device* i2c_bus = RT_NULL; /* I2C总线设备句柄 */

/**
 * 
 * @brief i2c总线写数据
 * @param [in] bus i2c总线句柄
 * @param [in] cfg_opt 操作的寄存器
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
static rt_err_t hl_i2c_write_reg(struct rt_i2c_bus_device* bus, rt_uint8_t cfg_opt, rt_uint8_t* data)
{
    rt_uint8_t        buf[4];
    struct rt_i2c_msg msgs[2];

    buf[0] = cfg_opt;  // cfg_opt
    buf[1] = data[0];  // data

    msgs[0].addr  = SGM41518_DEVICE_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = buf;
    msgs[0].len   = 2;
    /*
    msgs[1].addr  = SGM41518_DEVICE_ADDRESS;
    msgs[1].flags = RT_I2C_WR | RT_I2C_NO_START;
    msgs[1].buf   = &buf[1];
    msgs[1].len   = 1;
    */
    // 调用I2C设备接口传输数据
    if (rt_i2c_transfer(bus, msgs, 1) == 1)
        return HL_SUCCESS;
    else
        return HL_FAILED;
}

/**
 * 
 * @brief i2c总线读数据
 * @param [in] bus i2c总线句柄
 * @param [in] cfg_opt 操作的寄存器
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
static rt_err_t hl_i2c_read_reg(struct rt_i2c_bus_device* bus, rt_uint8_t cfg_opt, rt_uint8_t* rbuf)
{
    rt_uint8_t        buf[4];
    struct rt_i2c_msg msgs[2];

    msgs[0].addr  = SGM41518_DEVICE_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &cfg_opt;
    msgs[0].len   = 1;

    msgs[1].addr  = SGM41518_DEVICE_ADDRESS;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = buf;
    msgs[1].len   = 1;

    // 调用I2C设备接口传输数据
    if (rt_i2c_transfer(bus, msgs, 2) == 2) {
        rbuf[0] = buf[0];
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
static uint8_t hl_reg00_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SGM_REG00_ADDR, (uint8_t*)&reg_all.reg00)) {
        smg_printf("hl_reg00_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == SGM_READ_CMD) {
        switch (cmd) {
            case RW_EN_HIZ_MOD:
                param[0] = reg_all.reg00.EN_HIZ;
                break;
            case RW_EN_ICHG_MON:
                param[0] = reg_all.reg00.EN_ICHG_MON;
                break;
            case RW_IINDPM_SET:
                param[0] = reg_all.reg00.IINDPM;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
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
    if (hl_i2c_write_reg(i2c_bus, SGM_REG00_ADDR, (uint8_t*)&reg_all.reg00)) {
        smg_printf("hl_reg00_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] hl_reg00_ctl fail !\n", param[0]);
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
static uint8_t hl_reg01_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SGM_REG01_ADDR, (uint8_t*)&reg_all.reg01)) {
        smg_printf("hl_reg01_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == SGM_READ_CMD) {
        switch (cmd) {
            case RW_EN_PFM:
                param[0] = reg_all.reg01.PFM_DIS;
                break;
            case RW_WDG_RST:
                param[0] = reg_all.reg01.WD_RST;
                break;
            case RW_EN_OTG:
                param[0] = reg_all.reg01.OTG_CONFIG;
                break;
            case RW_EN_BAT_CHARGING:
                param[0] = reg_all.reg01.CHG_CONFIG;
                break;
            case RW_SYS_MIN:
                param[0] = reg_all.reg01.SYS_MIN;
                break;
            case RW_MIN_BAT_SEL:
                param[0] = reg_all.reg01.MIN_BAT_SEL;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case RW_EN_PFM:
            reg_all.reg01.PFM_DIS = param[0];
            break;
        case RW_WDG_RST:
            reg_all.reg01.WD_RST = param[0];
            break;
        case RW_EN_OTG:
            reg_all.reg01.OTG_CONFIG = param[0];
            break;
        case RW_EN_BAT_CHARGING:
            reg_all.reg01.CHG_CONFIG = param[0];
            break;
        case RW_SYS_MIN:
            reg_all.reg01.SYS_MIN = param[0];
            break;
        case RW_MIN_BAT_SEL:
            reg_all.reg01.MIN_BAT_SEL = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (hl_i2c_write_reg(i2c_bus, SGM_REG01_ADDR, (uint8_t*)&reg_all.reg01)) {
        smg_printf("hl_reg01_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] hl_reg01_ctl fail !\n", param[0]);
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
static uint8_t hl_reg02_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SGM_REG02_ADDR, (uint8_t*)&reg_all.reg02)) {
        smg_printf("hl_reg02_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == SGM_READ_CMD) {
        switch (cmd) {
            case RW_Q1_FULLON:
                param[0] = reg_all.reg02.Q1_FULLON;
                break;
            case RW_CHARGE_CURRENT:
                param[0] = reg_all.reg02.ICHG;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
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
    if (hl_i2c_write_reg(i2c_bus, SGM_REG02_ADDR, (uint8_t*)&reg_all.reg02)) {
        smg_printf("hl_reg02_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] hl_reg02_ctl fail !\n", param[0]);
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
static uint8_t hl_reg03_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SGM_REG03_ADDR, (uint8_t*)&reg_all.reg03)) {
        smg_printf("hl_reg03_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == SGM_READ_CMD) {
        switch (cmd) {
            case RW_PRECHARGE_CURRENT_LIMIT:
                param[0] = reg_all.reg03.IPRECHG;
                break;
            case RW_TER_CURRENT_LIMIT:
                param[0] = reg_all.reg03.ITERM;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
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
    if (hl_i2c_write_reg(i2c_bus, SGM_REG03_ADDR, (uint8_t*)&reg_all.reg03)) {
        smg_printf("hl_reg03_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] hl_reg03_ctl fail !\n", param[0]);
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
static uint8_t hl_reg04_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SGM_REG04_ADDR, (uint8_t*)&reg_all.reg04)) {
        smg_printf("hl_reg04_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == SGM_READ_CMD) {
        switch (cmd) {
            case RW_CHARGE_VOLTAGE_LIMIT:
                param[0] = reg_all.reg04.VREG;
                break;
            case RW_TOPOFF_TIME:
                param[0] = reg_all.reg04.TOPOFF_TIMER;
                break;
            case RW_RECHARGE_THRESHOLD:
                param[0] = reg_all.reg04.VRECHG;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
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
    if (hl_i2c_write_reg(i2c_bus, SGM_REG04_ADDR, (uint8_t*)&reg_all.reg04)) {
        smg_printf("hl_reg04_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] hl_reg04_ctl fail !\n", param[0]);
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
static uint8_t hl_reg05_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SGM_REG05_ADDR, (uint8_t*)&reg_all.reg05)) {
        smg_printf("hl_reg05_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == SGM_READ_CMD) {
        switch (cmd) {
            case RW_CHARGE_TER_ENABLE:
                param[0] = reg_all.reg05.EN_TERM;
                break;
            case RW_WDOG_TIMER_CHG:
                param[0] = reg_all.reg05.WATCHDOG;
                break;
            case RW_CHARGE_SAFETY_TIMER_ENABLE:
                param[0] = reg_all.reg05.EN_TIMER;
                break;
            case RW_CHARGE_SAFETY_TIMER_CHG:
                param[0] = reg_all.reg05.CHG_TIMER;
                break;
            case RW_TER_REGULATION_THRESHOLD:
                param[0] = reg_all.reg05.TREG;
                break;
            case RW_JEITA_CHARGING_CURRENT:
                param[0] = reg_all.reg05.JEITA_ISET;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case RW_CHARGE_TER_ENABLE:
            reg_all.reg05.EN_TERM = param[0];
            break;
        case RW_WDOG_TIMER_CHG:
            reg_all.reg05.WATCHDOG = param[0];
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
    if (hl_i2c_write_reg(i2c_bus, SGM_REG05_ADDR, (uint8_t*)&reg_all.reg05)) {
        smg_printf("hl_reg05_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] hl_reg05_ctl fail !\n", param[0]);
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
static uint8_t hl_reg06_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SGM_REG06_ADDR, (uint8_t*)&reg_all.reg06)) {
        smg_printf("hl_reg06_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == SGM_READ_CMD) {
        switch (cmd) {
            case RW_OVP_CHG:
                param[0] = reg_all.reg06.OVP;
                break;
            case RW_BOOSTV_CHG:
                param[0] = reg_all.reg06.BOOSTV;
                break;
            case RW_VINDPM_CHG:
                param[0] = reg_all.reg06.VINDPM;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
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
    if (hl_i2c_write_reg(i2c_bus, SGM_REG06_ADDR, (uint8_t*)&reg_all.reg06)) {
        smg_printf("hl_reg06_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] hl_reg06_ctl fail !\n", param[0]);
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
 * @details 注意：启用一次输入电流检测，检测结束后所有寄存器将会复位！！！
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-05      <td>dujunjie     <td>新建
 * </table>
 */
static uint8_t hl_reg07_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SGM_REG07_ADDR, (uint8_t*)&reg_all.reg07)) {
        smg_printf("hl_reg07_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == SGM_READ_CMD) {
        switch (cmd) {
            case RW_INPUT_CRRENT_LIMIT_DETECTION:
                param[0] = reg_all.reg07.IINDET_EN;
                break;
            case RW_TMR2X_EN:
                param[0] = reg_all.reg07.TMR2X_EN;
                break;
            case RW_BATFET_DIS:
                param[0] = reg_all.reg07.BATFET_DIS;
                break;
            case RW_JEITA_VSET_H:
                param[0] = reg_all.reg07.JEITA_VSET;
                break;
            case RW_BATFET_DLY:
                param[0] = reg_all.reg07.BATFET_DLY;
                break;
            case RW_BATFET_RST_EN:
                param[0] = reg_all.reg07.BATFET_RST_EN;
                break;
            case RW_VDPM_BAT_TRACK:
                param[0] = reg_all.reg07.VDPM_BAT_TRACK;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
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
    if (hl_i2c_write_reg(i2c_bus, SGM_REG07_ADDR, (uint8_t*)&reg_all.reg07)) {
        smg_printf("hl_reg07_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] hl_reg07_ctl fail !\n", param[0]);
    return HL_FAILED;
}

/**
 * 
 * @brief 0x08寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读
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
static uint8_t hl_reg08_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SGM_REG08_ADDR, (uint8_t*)&reg_all.reg08)) {
        smg_printf("hl_reg08_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == SGM_READ_CMD) {
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
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] hl_reg08_ctl fail !\n", param[0]);
    return HL_FAILED;
}

/**
 * 
 * @brief 0x09寄存器操作函数
 * @param [in] cmd 具体操作的功能
 * @param [in] cmd_typ 功能类型：读
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
static uint8_t hl_reg09_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SGM_REG09_ADDR, (uint8_t*)&reg_all.reg09)) {
        smg_printf("hl_reg09_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == SGM_READ_CMD) {
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
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] hl_reg09_ctl fail !\n", param[0]);
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
static uint8_t hl_reg0A_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SGM_REG0A_ADDR, (uint8_t*)&reg_all.reg0A)) {
        smg_printf("hl_reg0A_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == SGM_READ_CMD) {
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
    if (hl_i2c_write_reg(i2c_bus, SGM_REG0A_ADDR, (uint8_t*)&reg_all.reg0A)) {
        smg_printf("hl_reg0A_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] hl_reg0A_ctl fail !\n", param[0]);
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
static uint8_t hl_reg0B_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SGM_REG0B_ADDR, (uint8_t*)&reg_all.reg0B)) {
        smg_printf("hl_reg0B_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == SGM_READ_CMD) {
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
    if (hl_i2c_write_reg(i2c_bus, SGM_REG0B_ADDR, (uint8_t*)&reg_all.reg0B)) {
        smg_printf("hl_reg0B_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] hl_reg0B_ctl fail !\n", param[0]);
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
static uint8_t hl_reg0C_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SGM_REG0C_ADDR, (uint8_t*)&reg_all.reg0C)) {
        smg_printf("hl_reg0C_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == SGM_READ_CMD) {
        switch (cmd) {
            case RW_JEITA_VSET_L:
                param[0] = reg_all.reg0C.JEITA_VSET_L;
                break;
            case RW_JEITA_ISET_L_EN:
                param[0] = reg_all.reg0C.JEITA_ISET_L_EN;
                break;
            case RW_JEITA_ISET_H:
                param[0] = reg_all.reg0C.JEITA_ISET_H;
                break;
            case RW_JEITA_VT2:
                param[0] = reg_all.reg0C.JEITA_VT2;
                break;
            case RW_JEITA_VT3:
                param[0] = reg_all.reg0C.JEITA_VT3;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
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
    if (hl_i2c_write_reg(i2c_bus, SGM_REG0C_ADDR, (uint8_t*)&reg_all.reg0C)) {
        smg_printf("hl_reg0C_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] hl_reg0C_ctl fail !\n", param[0]);
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
static uint8_t hl_reg0D_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SGM_REG0D_ADDR, (uint8_t*)&reg_all.reg0D)) {
        smg_printf("hl_reg0D_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == SGM_READ_CMD) {
        switch (cmd) {
            case RW_JEITA_EN:
                param[0] = reg_all.reg0D.JEITA_EN;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case RW_JEITA_EN:
            reg_all.reg0D.JEITA_EN = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (hl_i2c_write_reg(i2c_bus, SGM_REG0D_ADDR, (uint8_t*)&reg_all.reg0D)) {
        smg_printf("hl_reg0D_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] hl_reg0D_ctl fail !\n", param[0]);
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
static uint8_t hl_reg0E_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
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
static uint8_t hl_reg0F_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SGM41518_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SGM_REG0F_ADDR, (uint8_t*)&reg_all.reg0F)) {
        smg_printf("hl_reg0F_ctl read err !\n");
        goto CTL_ERR;
    }
    if (cmd_typ == SGM_READ_CMD) {
        switch (cmd) {
            case RW_VREG_FT:
                param[0] = reg_all.reg0F.VREG_FT;
                break;
            case RW_STAT_SET:
                param[0] = reg_all.reg0F.STATSET;
                break;
            case RW_VINDPM_OS:
                param[0] = reg_all.reg0F.VINDPM_OS;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
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
    if (hl_i2c_write_reg(i2c_bus, SGM_REG0F_ADDR, (uint8_t*)&reg_all.reg0F)) {
        smg_printf("hl_reg0F_ctl write err !\n");
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    smg_printf("[ %X ] hl_reg0F_ctl fail !\n", param[0]);
    return HL_FAILED;
}

/**
 * 
 * @brief 获取寄存器地址
 * @param [in] cfg_opt 带解析的寄存器指令
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
static uint8_t hl_get_reg_serial(uint8_t cfg_opt)
{
    uint8_t reg_ser;

    switch (cfg_opt) {
        // cfg_opt 00
        case RW_EN_HIZ_MOD:
        case RW_EN_ICHG_MON:
        case RW_IINDPM_SET:
            reg_ser = SGM_REG00_ADDR;
            break;
        // cfg_opt 01
        case RW_EN_PFM:
        case RW_WDG_RST:
        case RW_EN_OTG:
        case RW_EN_BAT_CHARGING:
        case RW_SYS_MIN:
        case RW_MIN_BAT_SEL:
            reg_ser = SGM_REG01_ADDR;
            break;
        // cfg_opt 02
        case RW_Q1_FULLON:
        case RW_CHARGE_CURRENT:
            reg_ser = SGM_REG02_ADDR;
            break;
        // cfg_opt 03
        case RW_PRECHARGE_CURRENT_LIMIT:
        case RW_TER_CURRENT_LIMIT:
            reg_ser = SGM_REG03_ADDR;
            break;
        // cfg_opt 04
        case RW_CHARGE_VOLTAGE_LIMIT:
        case RW_TOPOFF_TIME:
        case RW_RECHARGE_THRESHOLD:
            reg_ser = SGM_REG04_ADDR;
            break;
        // cfg_opt 05
        case RW_CHARGE_TER_ENABLE:
        case RW_WDOG_TIMER_CHG:
        case RW_CHARGE_SAFETY_TIMER_ENABLE:
        case RW_CHARGE_SAFETY_TIMER_CHG:
        case RW_TER_REGULATION_THRESHOLD:
        case RW_JEITA_CHARGING_CURRENT:
            reg_ser = SGM_REG05_ADDR;
            break;
        // cfg_opt 06
        case RW_OVP_CHG:
        case RW_BOOSTV_CHG:
        case RW_VINDPM_CHG:
            reg_ser = SGM_REG06_ADDR;
            break;
        // cfg_opt 07
        case RW_INPUT_CRRENT_LIMIT_DETECTION:
        case RW_TMR2X_EN:
        case RW_BATFET_DIS:
        case RW_JEITA_VSET_H:
        case RW_BATFET_DLY:
        case RW_BATFET_RST_EN:
        case RW_VDPM_BAT_TRACK:
            reg_ser = SGM_REG07_ADDR;
            break;
        // cfg_opt 08
        case R_VBUS_STAT:
        case R_CHRG_STAT:
        case R_PG_STAT:
        case R_THERM_STAT:
        case R_VSYS_STAT:
            reg_ser = SGM_REG08_ADDR;
            break;
        // cfg_opt 09
        case R_WDG_FAULT:
        case R_BOOST_FAULT:
        case R_CHRG_FAULT:
        case R_BAT_FAULT:
        case R_NTC_FAULT:
            reg_ser = SGM_REG09_ADDR;
            break;
        // cfg_opt 0A
        case RW_VINDPM_INT_MASK:
        case RW_IINDPM_INT_MASK:
        case R_VBUS_GD:
        case R_VINDPM_STAT:
        case R_IINDPM_STAT:
        case R_TOPOFF_ACTIVE:
        case R_ACOV_STAT:
            reg_ser = SGM_REG0A_ADDR;
            break;
        // cfg_opt 0B
        case RW_REG_RST:
        case R_DEVICE_ID:
            reg_ser = SGM_REG0B_ADDR;
            break;
        // cfg_opt 0C
        case RW_JEITA_VSET_L:
        case RW_JEITA_ISET_L_EN:
        case RW_JEITA_ISET_H:
        case RW_JEITA_VT2:
        case RW_JEITA_VT3:
            reg_ser = SGM_REG0C_ADDR;
            break;
        // cfg_opt 0D
        case RW_JEITA_EN:
            reg_ser = SGM_REG0D_ADDR;
            break;
        // cfg_opt 0F
        case RW_VREG_FT:
        case RW_STAT_SET:
        case RW_VINDPM_OS:
            reg_ser = SGM_REG0F_ADDR;
            break;
        default:
            smg_printf("Register : [ %X ] overrun !\n", cfg_opt);
            return 0xFF;
            break;
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
static void hl_reg_ctl_fun_init(void)
{
    fun_arr[0]  = hl_reg00_ctl;
    fun_arr[1]  = hl_reg01_ctl;
    fun_arr[2]  = hl_reg02_ctl;
    fun_arr[3]  = hl_reg03_ctl;
    fun_arr[4]  = hl_reg04_ctl;
    fun_arr[5]  = hl_reg05_ctl;
    fun_arr[6]  = hl_reg06_ctl;
    fun_arr[7]  = hl_reg07_ctl;
    fun_arr[8]  = hl_reg08_ctl;
    fun_arr[9]  = hl_reg09_ctl;
    fun_arr[10] = hl_reg0A_ctl;
    fun_arr[11] = hl_reg0B_ctl;
    fun_arr[12] = hl_reg0C_ctl;
    fun_arr[13] = hl_reg0D_ctl;
    fun_arr[14] = hl_reg0E_ctl;
    fun_arr[15] = hl_reg0F_ctl;
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
static void hl_reg_ctl_fun_deinit(void)
{
    uint8_t i;
    for (i = 0; i < SGM_REG_NUM; i++)
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
static void hl_reg_arr_init(void)
{
    uint8_t i;
    for (i = 0; i < SGM_REG_NUM; i++)
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
static void hl_reg_arr_deinit(void)
{
    uint8_t i;
    for (i = 0; i < SGM_REG_NUM; i++)
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
static uint8_t hl_reg_ctl(uint8_t reg_ser, uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    uint8_t i;
    for (i = 0; i < SGM_REG_NUM; i++) {
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
static uint8_t hl_reg_type_get(uint8_t cmd)
{
    if (cmd < RW_CNT_MAX)
        return SGM_WRITE_CMD;
    return SGM_READ_CMD;
}

/**
 * 
 * @brief smg41518配置操作函数接口
 * @param [in] cmd 配置项
 * @param [in] ptr 配置参数指针，类型应该为 HL_SGM_INPUT_PARAM_T
 * @param [in] len 配置参数个数
 * @return uint8_t 0成功，1失败
 * @date 2022-09-05
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 只支持一字节长度，每次只能配置一个功能参数
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-05      <td>dujunjie     <td>新建
 * </table>
 */
uint8_t hl_drv_sgm41518_io_ctrl(uint8_t cmd, void* ptr, uint8_t len)
{
    uint8_t               reg_cmd, reg_serial, cmd_typ;
    HL_SGM_INPUT_PARAM_T* param = (HL_SGM_INPUT_PARAM_T*)ptr;
    if (ptr == NULL || len != 1) {
        smg_printf("Param err ! len : [ %X ]\n", len);
        return HL_FAILED;
    }
    cmd_typ = cmd;
    if ((cmd_typ != hl_reg_type_get(param->cfg_opt)) && (cmd_typ == SGM_WRITE_CMD)) {
        smg_printf("Register0 : [ %X ] type err !\n", cmd);
        return HL_FAILED;
    }
    reg_cmd    = param->cfg_opt;
    reg_serial = hl_get_reg_serial(reg_cmd);
    if (cmd_typ == SGM_WRITE_CMD && (reg_cmd == SGM_REG08_ADDR || reg_cmd == SGM_REG09_ADDR)) {
        smg_printf("Register1 : [ %X ] type err !\n", cmd);
        return HL_FAILED;
    }

    if (hl_reg_ctl(reg_serial, reg_cmd, cmd_typ, &param->param)) {
        smg_printf("hl_reg_ctl fail !\n");
        return HL_FAILED;
    }
    return HL_SUCCESS;
}

/**
 * 
 * @brief 初始化结果检测函数
 * @param [in] data 配置参数
 * @return uint8_t 
 * @date 2022-10-12
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-12      <td>dujunjie     <td>新建
 * </table>
 */
static uint8_t hl_drv_sgm41518_init_test(uint8_t* data)
{
    uint8_t reg_config_bak[16] = { 0 }, i = 0;
    if (hl_i2c_read_reg(i2c_bus, SGM_REG00_ADDR, &reg_config_bak[0])) {
        smg_printf("cfg_opt %d read err !\n", SGM_REG00_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SGM_REG01_ADDR, &reg_config_bak[1])) {
        smg_printf("cfg_opt %d read err !\n", SGM_REG01_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SGM_REG02_ADDR, &reg_config_bak[2])) {
        smg_printf("cfg_opt %d read err !\n", SGM_REG02_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SGM_REG03_ADDR, &reg_config_bak[3])) {
        smg_printf("cfg_opt %d read err !\n", SGM_REG03_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SGM_REG04_ADDR, &reg_config_bak[4])) {
        smg_printf("cfg_opt %d read err !\n", SGM_REG04_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SGM_REG05_ADDR, &reg_config_bak[5])) {
        smg_printf("cfg_opt %d read err !\n", SGM_REG05_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SGM_REG06_ADDR, &reg_config_bak[6])) {
        smg_printf("cfg_opt %d read err !\n", SGM_REG06_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SGM_REG07_ADDR, &reg_config_bak[7])) {
        smg_printf("cfg_opt %d read err !\n", SGM_REG07_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SGM_REG0C_ADDR, &reg_config_bak[0x0C])) {
        smg_printf("cfg_opt %d read err !\n", SGM_REG0C_ADDR);
        goto INIT_ERR;
    }

    for (i = 0; i < 16; i++) {
        if (reg_config_bak[i] != data[i]) {
            smg_printf("smg41518 reg %X init fail!\n", i);
            smg_printf("reg_config_bak[%d] = %X data[%d] = %X!\n", i, reg_config_bak[i], i, data[i]);
            goto INIT_ERR;
        }
    }
    smg_printf("smg41518 reg init success!\n");
    return HL_SUCCESS;
INIT_ERR:
    smg_printf("smg41518 reg init fail!\n");
    return HL_FAILED;
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
    uint8_t              reg_config[16] = { 0 };
    HL_SGM41518_REGALL_T reg_all;
    /* 查找I2C总线设备，获取I2C总线设备句柄 */
    i2c_bus = (struct rt_i2c_bus_device*)rt_device_find(SGM41518_IIC_NAME);
    smg_printf("smg41518 init !\n");
    hl_reg_arr_init();
    hl_reg_ctl_fun_init();

    if (i2c_bus == RT_NULL) {
        smg_printf("can't find %s device!\n", SGM41518_IIC_NAME);
        goto INIT_ERR;
    }

    if (hl_i2c_read_reg(i2c_bus, SGM_REG00_ADDR, (uint8_t*)&reg_all.reg00)) {
        smg_printf("cfg_opt %d read err !\n", SGM_REG00_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SGM_REG01_ADDR, (uint8_t*)&reg_all.reg01)) {
        smg_printf("cfg_opt %d read err !\n", SGM_REG01_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SGM_REG02_ADDR, (uint8_t*)&reg_all.reg02)) {
        smg_printf("cfg_opt %d read err !\n", SGM_REG02_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SGM_REG03_ADDR, (uint8_t*)&reg_all.reg03)) {
        smg_printf("cfg_opt %d read err !\n", SGM_REG03_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SGM_REG04_ADDR, (uint8_t*)&reg_all.reg04)) {
        smg_printf("cfg_opt %d read err !\n", SGM_REG04_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SGM_REG05_ADDR, (uint8_t*)&reg_all.reg05)) {
        smg_printf("cfg_opt %d read err !\n", SGM_REG05_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SGM_REG06_ADDR, (uint8_t*)&reg_all.reg06)) {
        smg_printf("cfg_opt %d read err !\n", SGM_REG06_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SGM_REG07_ADDR, (uint8_t*)&reg_all.reg07)) {
        smg_printf("cfg_opt %d read err !\n", SGM_REG07_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SGM_REG0C_ADDR, (uint8_t*)&reg_all.reg0C)) {
        smg_printf("cfg_opt %d read err !\n", SGM_REG0C_ADDR);
        goto INIT_ERR;
    }

    reg_all.reg00.IINDPM      = IINDPM_SET(31);            // 3200mA
    reg_all.reg00.EN_ICHG_MON = FOLLOE_CHARG_STAT_ENABLE;  //STAT引脚状态由充电状态决定
    reg_all.reg00.EN_HIZ      = HIZ_DISABLE;               //HIZ模式禁用
    reg_config[0]             = *(uint8_t*)&reg_all.reg00;

    reg_all.reg01.MIN_BAT_SEL = MIN_BOOST_VOLTAGE_2v95;  //升压模式最小系统电压2.95V
    reg_all.reg01.SYS_MIN     = SYS_MIN_3V5;             //默认最小系统电压3.5V
    reg_all.reg01.CHG_CONFIG  = CHARGE_ENABLE;           //充电使能
    reg_all.reg01.OTG_CONFIG  = OTG_DISABLE;             //关闭OTG充电功能
    reg_all.reg01.WD_RST      = WDG_TIMEOUT;             //看门狗初始化复位
    reg_all.reg01.PFM_DIS     = PFM_DISABLE;             //禁用PFM模式
    reg_config[1]             = *(uint8_t*)&reg_all.reg01;

    reg_all.reg02.ICHG = FAST_CHARGE_CURRENT_SET(63);  // 1280mA
    reg_all.reg02.Q1_FULLON = Q1_USE_HIGHER_RDSON;  //控制开关阻抗，输入电流<700ma时建议设置成 0，通用设置为1
    reg_config[2] = *(uint8_t*)&reg_all.reg02;

    reg_all.reg03.ITERM   = TER_CURRENT_SET(0);  // 设置终止电流20mA
    reg_all.reg03.IPRECHG = PRE_CHARGE_SET(12);  // 设置预充电电流260mA
    reg_config[3]         = *(uint8_t*)&reg_all.reg03;

    reg_all.reg04.VRECHG       = RECHARGE_THRESHOLD_100;    //充电开始阈值：VREG - 100mv
    reg_all.reg04.TOPOFF_TIMER = TOP_TIMER_DISABLE;         //关闭充电延长时间
    reg_all.reg04.VREG         = CHARGE_VOLTAGE_LIMIT(12);  // 充电电压限制4.24V
    reg_config[4]              = *(uint8_t*)&reg_all.reg04;

    reg_all.reg05.JEITA_ISET = JEITA_CURRENT_SET_L_50;      //低温时（0℃ - 10 ℃）充电电流 I = 50% * ICHG
    reg_all.reg05.TREG       = THERNAL_REGT_THRESHOLD_120;  //设置热调节阈值 120℃
    reg_all.reg05.CHG_TIMER  = CHARGE_SAFET_TIMER_SET_10H;  //充电安全计数器时间设置为10h
    reg_all.reg05.EN_TIMER   = CHARGE_SAFET_TIMER_ENABLE;   //使能充电安全计数器
    reg_all.reg05.WATCHDOG   = WDG_TIMER_DISABLE;           //禁用看门狗
    reg_all.reg05.EN_TERM    = CHARGE_TER_ENABLE;           //充电终止使能
    reg_config[5]            = *(uint8_t*)&reg_all.reg05;

    reg_all.reg06.VINDPM = VINDPM_THRESHOLD(6);       //VINDPM电压阈值 : 4.5V
    reg_all.reg06.BOOSTV = BOOST_VOL_THRESHOLD_5V15;  //boost模式调压设置5.15V
    reg_all.reg06.OVP    = VAC_OVP_THRESHOLD_6V5;     //设置VAC检测输入电压阈值 6.5V（5V输入时）
    reg_config[6]        = *(uint8_t*)&reg_all.reg06;

    reg_all.reg07.VDPM_BAT_TRACK = VDPM_BAT_TRACK_DISABLE;  //关闭动态VUNDPM跟踪功能
    reg_all.reg07.BATFET_RST_EN  = BATFET_RST_ENABLE;       //启用BATFET重置功能
    reg_all.reg07.BATFET_DLY     = BATFET_TSM_DLY_ENABLE;   //关闭BATFET时，延时Tsm_dly时间后再关闭
    reg_all.reg07.JEITA_VSET     = JEITA_VSET_H_SET_VREG;   //设置充电电压为VREG（45℃ - 60℃）
    reg_all.reg07.BATFET_DIS     = BATFET_ENABLE;           //允许BATFET打开
    reg_all.reg07.TMR2X_EN       = TMR2X_ENABLE;            //开启安全计数器减速计数模式
    reg_all.reg07.IINDET_EN      = IINDET_DISABLE;           //使能输入电流限制检测
    reg_config[7]                = *(uint8_t*)&reg_all.reg07;

    reg_all.reg0C.JEITA_VT3       = JEITA_WARM_VT3_44C5;    //温暖阈值T3设置为 44.5℃
    reg_all.reg0C.JEITA_VT2       = JEITA_COOL_VT2_10C;     //寒冷阈值T2设置为 10℃
    reg_all.reg0C.JEITA_ISET_H    = JEITA_WARM_ISET_100;    //温暖温度设置充电电流 100% * ICHG
    reg_all.reg0C.JEITA_ISET_L_EN = JEITA_ISET_L_ENABLE;    //开启低温充电
    reg_all.reg0C.JEITA_VSET_L    = JEITA_VSET_L_SET_VREG;  //设置低温区间（0℃ - 10℃）充电电压等于VREG
    reg_config[0x0C]              = *(uint8_t*)&reg_all.reg0C;
    
    if (hl_i2c_write_reg(i2c_bus, SGM_REG00_ADDR, (uint8_t*)&reg_all.reg00)) {
        smg_printf("cfg_opt %d write err !\n", SGM_REG00_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_write_reg(i2c_bus, SGM_REG01_ADDR, (uint8_t*)&reg_all.reg01)) {
        smg_printf("cfg_opt %d write err !\n", SGM_REG01_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_write_reg(i2c_bus, SGM_REG02_ADDR, (uint8_t*)&reg_all.reg02)) {
        smg_printf("cfg_opt %d write err !\n", SGM_REG02_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_write_reg(i2c_bus, SGM_REG03_ADDR, (uint8_t*)&reg_all.reg03)) {
        smg_printf("cfg_opt %d write err !\n", SGM_REG03_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_write_reg(i2c_bus, SGM_REG04_ADDR, (uint8_t*)&reg_all.reg04)) {
        smg_printf("cfg_opt %d write err !\n", SGM_REG04_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_write_reg(i2c_bus, SGM_REG05_ADDR, (uint8_t*)&reg_all.reg05)) {
        smg_printf("cfg_opt %d write err !\n", SGM_REG05_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_write_reg(i2c_bus, SGM_REG06_ADDR, (uint8_t*)&reg_all.reg06)) {
        smg_printf("cfg_opt %d write err !\n", SGM_REG06_ADDR);
        goto INIT_ERR;
    }    
    if (hl_i2c_write_reg(i2c_bus, SGM_REG07_ADDR, (uint8_t*)&reg_all.reg07)) {
        smg_printf("cfg_opt %d write err !\n", SGM_REG07_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_write_reg(i2c_bus, SGM_REG0C_ADDR, (uint8_t*)&reg_all.reg0C)) {
        smg_printf("cfg_opt %d write err !\n", SGM_REG0C_ADDR);
        goto INIT_ERR;
    }

    if (hl_drv_sgm41518_init_test(reg_config) == HL_FAILED) {
        smg_printf("smg41518 reg init fail!\n");
        goto INIT_ERR;
    }
    smg_printf("smg41518 init success !\n");
    return HL_SUCCESS;
INIT_ERR:
    smg_printf("smg41518 init fail !\n");
    hl_reg_arr_deinit();
    hl_reg_ctl_fun_deinit();
    return HL_FAILED;
}

/**
 * 
 * @brief 
 * @param [in] argc 
 * @param [in] argv 按顺序为：读写  配置选项  选项参数值（参考功能选项参数宏）
 * @date 2022-10-08
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-08      <td>dujunjie     <td>新建
 * </table>
 */
void hl_drv_sgm41518_test(int argc, char** argv)
{
    HL_SGM_INPUT_PARAM_T par;
    if (argc != 4) {
        smg_printf("argc param err : %d \n", argc);
        return;
    }

    uint8_t cmd     = atoi(argv[1]);
    uint8_t cfg_opt = atoi(argv[2]);
    uint8_t data    = atoi(argv[3]);

    hl_reg_arr_init();
    hl_reg_ctl_fun_init();

    par.cfg_opt = cfg_opt;
    par.param   = data;

    i2c_bus = (struct rt_i2c_bus_device*)rt_device_find(SGM41518_IIC_NAME);

    if (i2c_bus == RT_NULL) {
        smg_printf("can't find %s device!\n", SGM41518_IIC_NAME);
    }

    if (hl_drv_sgm41518_io_ctrl(cmd, &par, 1) != HL_SUCCESS) {
        smg_printf("hl_drv_sgm41518_io_ctrl fail!\n");
    }

    smg_printf("par.param = 0x%02X \n", par.param);
}
// INIT_ENV_EXPORT(hl_drv_sgm41518_init);
MSH_CMD_EXPORT(hl_drv_sgm41518_test, run hl_drv_sgm41518_test);
MSH_CMD_EXPORT(hl_drv_sgm41518_init, run hl_drv_sgm41518_init);
#endif