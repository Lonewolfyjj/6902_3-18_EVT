/**
 * @file hl_drv_sy6971.c
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief 
 * @version 1.0
 * @date 2022-11-03
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
 * <tr><td>2022-11-03     <td>v1.0     <td>dujunjie     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
#include <rtdevice.h>
#include "hl_drv_sy6971.h"
#include "hl_config.h"
/* typedef -------------------------------------------------------------------*/
typedef uint8_t (*hl_reg_ctl_fun)(uint8_t cmd, uint8_t cmd_typ, uint8_t* param);
/* define --------------------------------------------------------------------*/
#define SY6971_DEBUG
#ifdef SY6971_DEBUG
#define sy_printf(...) rt_kprintf(__VA_ARGS__)
#else
#define sy_printf(...)
#endif
/* variables -----------------------------------------------------------------*/
static hl_reg_ctl_fun            fun_arr[SY_REG_NUM];
static uint8_t                   reg_arr[SY_REG_NUM];
static struct rt_i2c_bus_device* i2c_bus = RT_NULL; /* I2C总线设备句柄 */
/* Private function(only *.c)  -----------------------------------------------*/

static rt_err_t hl_i2c_write_reg(struct rt_i2c_bus_device* bus, rt_uint8_t cfg_opt, rt_uint8_t* data)
{
    rt_size_t       ret;
    rt_uint8_t        buf[4];
    struct rt_i2c_msg msgs[2];

    buf[0] = cfg_opt;  // cfg_opt
    buf[1] = data[0];  // data

    msgs[0].addr  = SY6971_DEVICE_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = buf;
    msgs[0].len   = 2;
    /*
    msgs[1].addr  = SY6971_DEVICE_ADDRESS;
    msgs[1].flags = RT_I2C_WR | RT_I2C_NO_START;
    msgs[1].buf   = &buf[1];
    msgs[1].len   = 1;
    */
    // 调用I2C设备接口传输数据
    ret = rt_i2c_transfer(bus, msgs, 1);
    if (ret == 1)
        return HL_SUCCESS;
    else{
        sy_printf("%s ret = %d !\n", __FUNCTION__,ret);
        return HL_FAILED;
    }   
}

static rt_err_t hl_i2c_read_reg(struct rt_i2c_bus_device* bus, rt_uint8_t cfg_opt, rt_uint8_t* rbuf)
{
    rt_size_t       ret;
    rt_uint8_t        buf[4];
    struct rt_i2c_msg msgs[2];

    msgs[0].addr  = SY6971_DEVICE_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &cfg_opt;
    msgs[0].len   = 1;

    msgs[1].addr  = SY6971_DEVICE_ADDRESS;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = buf;
    msgs[1].len   = 1;

    // 调用I2C设备接口传输数据
    ret = rt_i2c_transfer(bus, msgs, 2);
    if (ret == 2) {
        rbuf[0] = buf[0];
        return HL_SUCCESS;
    } else{
        sy_printf("%s ret = %d !\n", __FUNCTION__,ret);
        return HL_FAILED;
    }  
}

static uint8_t hl_reg00_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    uint8_t            bak_data = 0;
    HL_SY6971_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SY_REG00_ADDR, (uint8_t*)&reg_all.reg00)) {
        sy_printf("%s read err !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    if (cmd_typ == SY_READ_CMD) {
        switch (cmd) {
            case E_IINLIM:
                param[0] = reg_all.reg00.IINLIM;
                break;
            case E_LDO_MODE2:
                param[0] = reg_all.reg00.LDO_MODE2;
                break;
            case E_LDO_MODE1:
                param[0] = reg_all.reg00.LDO_MODE1;
                break;
            case E_EN_HIZ:
                param[0] = reg_all.reg00.EN_HIZ;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case E_IINLIM:
            reg_all.reg00.IINLIM = param[0];
            break;
        case E_LDO_MODE2:
            reg_all.reg00.LDO_MODE2 = param[0];
            break;
        case E_LDO_MODE1:
            reg_all.reg00.LDO_MODE1 = param[0];
            break;
        case E_EN_HIZ:
            reg_all.reg00.EN_HIZ = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (hl_i2c_write_reg(i2c_bus, SY_REG00_ADDR, (uint8_t*)&reg_all.reg00)) {
        goto CTL_ERR;
    }
    hl_i2c_read_reg(i2c_bus, SY_REG00_ADDR, &bak_data);
    if (bak_data != *(uint8_t*)&reg_all.reg00) {
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    sy_printf("%s write err !\n", __FUNCTION__);
    return HL_FAILED;
}

static uint8_t hl_reg01_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    uint8_t            bak_data = 0;
    HL_SY6971_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SY_REG01_ADDR, (uint8_t*)&reg_all.reg01)) {
        sy_printf("%s read err !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    if (cmd_typ == SY_READ_CMD) {
        switch (cmd) {
            case E_OTG_BAT:
                param[0] = reg_all.reg01.OTG_BAT;
                break;
            case E_SYS_MIN:
                param[0] = reg_all.reg01.SYS_MIN;
                break;
            case E_CHG_CONFIG:
                param[0] = reg_all.reg01.CHG_CONFIG;
                break;
            case E_OTG_CONFIG:
                param[0] = reg_all.reg01.OTG_CONFIG;
                break;
            case E_WD_RST:
                param[0] = reg_all.reg01.WD_RST;
                break;
            case E_PFM_DIS:
                param[0] = reg_all.reg01.PFM_DIS;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case E_OTG_BAT:
            reg_all.reg01.OTG_BAT = param[0];
            break;
        case E_SYS_MIN:
            reg_all.reg01.SYS_MIN = param[0];
            break;
        case E_CHG_CONFIG:
            reg_all.reg01.CHG_CONFIG = param[0];
            break;
        case E_OTG_CONFIG:
            reg_all.reg01.OTG_CONFIG = param[0];
            break;
        case E_WD_RST:
            reg_all.reg01.WD_RST = param[0];
            break;
        case E_PFM_DIS:
            reg_all.reg01.PFM_DIS = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (hl_i2c_write_reg(i2c_bus, SY_REG01_ADDR, (uint8_t*)&reg_all.reg01)) {
        goto CTL_ERR;
    }
    hl_i2c_read_reg(i2c_bus, SY_REG01_ADDR, &bak_data);
    if (bak_data != *(uint8_t*)&reg_all.reg01) {
        goto CTL_ERR;
    }
CTL_ERR:
    return HL_SUCCESS;
// CTL_ERR:
//     sy_printf("%s write err !\n", __FUNCTION__);
//     return HL_FAILED;
}

static uint8_t hl_reg02_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    uint8_t            bak_data = 0;
    HL_SY6971_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SY_REG02_ADDR, (uint8_t*)&reg_all.reg02)) {
        sy_printf("%s read err !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    if (cmd_typ == SY_READ_CMD) {
        switch (cmd) {
            case E_ICHG:
                param[0] = reg_all.reg02.ICHG;
                break;
            case E_BATSNS_DIS:
                param[0] = reg_all.reg02.BATSNS_DIS;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case E_ICHG:
            reg_all.reg02.ICHG = param[0];
            break;
        case E_BATSNS_DIS:
            reg_all.reg02.BATSNS_DIS = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (hl_i2c_write_reg(i2c_bus, SY_REG02_ADDR, (uint8_t*)&reg_all.reg02)) {
        goto CTL_ERR;
    }
    hl_i2c_read_reg(i2c_bus, SY_REG02_ADDR, &bak_data);
    if (bak_data != *(uint8_t*)&reg_all.reg02) {
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    sy_printf("%s write err !\n", __FUNCTION__);
    return HL_FAILED;
}

static uint8_t hl_reg03_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    uint8_t            bak_data = 0;
    HL_SY6971_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SY_REG03_ADDR, (uint8_t*)&reg_all.reg03)) {
        sy_printf("%s read err !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    if (cmd_typ == SY_READ_CMD) {
        switch (cmd) {
            case E_IPRECHG:
                param[0] = reg_all.reg03.IPRECHG;
                break;
            case E_IINDPM_INT_MASK:
                param[0] = reg_all.reg03.IINDPM_INT_MASK;
                break;
            case E_VINDPM_INT_MASK:
                param[0] = reg_all.reg03.VINDPM_INT_MASK;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case E_IPRECHG:
            reg_all.reg03.IPRECHG = param[0];
            break;
        case E_IINDPM_INT_MASK:
            reg_all.reg03.IINDPM_INT_MASK = param[0];
            break;
        case E_VINDPM_INT_MASK:
            reg_all.reg03.VINDPM_INT_MASK = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (hl_i2c_write_reg(i2c_bus, SY_REG03_ADDR, (uint8_t*)&reg_all.reg03)) {
        goto CTL_ERR;
    }
    hl_i2c_read_reg(i2c_bus, SY_REG03_ADDR, &bak_data);
    if (bak_data != *(uint8_t*)&reg_all.reg03) {
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    sy_printf("%s write err !\n", __FUNCTION__);
    return HL_FAILED;
}

static uint8_t hl_reg04_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    uint8_t            bak_data = 0;
    HL_SY6971_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SY_REG04_ADDR, (uint8_t*)&reg_all.reg04)) {
        sy_printf("%s read err !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    if (cmd_typ == SY_READ_CMD) {
        switch (cmd) {
            case E_ITERM:
                param[0] = reg_all.reg04.ITERM;
                break;
            case E_ISHORT:
                param[0] = reg_all.reg04.ISHORT;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case E_ITERM:
            reg_all.reg04.ITERM = param[0];
            break;
        case E_ISHORT:
            reg_all.reg04.ISHORT = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (hl_i2c_write_reg(i2c_bus, SY_REG04_ADDR, (uint8_t*)&reg_all.reg04)) {
        goto CTL_ERR;
    }
    hl_i2c_read_reg(i2c_bus, SY_REG04_ADDR, &bak_data);
    if (bak_data != *(uint8_t*)&reg_all.reg04) {
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    sy_printf("%s write err !\n", __FUNCTION__);
    return HL_FAILED;
}

static uint8_t hl_reg05_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    uint8_t            bak_data = 0;
    HL_SY6971_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SY_REG05_ADDR, (uint8_t*)&reg_all.reg05)) {
        sy_printf("%s read err !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    if (cmd_typ == SY_READ_CMD) {
        switch (cmd) {
            case E_VRECHG:
                param[0] = reg_all.reg05.VRECHG;
                break;
            case E_EN_TERM:
                param[0] = reg_all.reg05.EN_TERM;
                break;
            case E_VREG:
                param[0] = reg_all.reg05.VREG;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case E_VRECHG:
            reg_all.reg05.VRECHG = param[0];
            break;
        case E_EN_TERM:
            reg_all.reg05.EN_TERM = param[0];
            break;
        case E_VREG:
            reg_all.reg05.VREG = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (hl_i2c_write_reg(i2c_bus, SY_REG05_ADDR, (uint8_t*)&reg_all.reg05)) {
        goto CTL_ERR;
    }
    hl_i2c_read_reg(i2c_bus, SY_REG05_ADDR, &bak_data);
    if (bak_data != *(uint8_t*)&reg_all.reg05) {
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    sy_printf("%s write err !\n", __FUNCTION__);
    return HL_FAILED;
}

static uint8_t hl_reg06_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    uint8_t            bak_data = 0;
    HL_SY6971_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SY_REG06_ADDR, (uint8_t*)&reg_all.reg06)) {
        sy_printf("%s read err !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    if (cmd_typ == SY_READ_CMD) {
        switch (cmd) {
            case E_JEITA_ISET:
                param[0] = reg_all.reg06.JEITA_ISET;
                break;
            case E_JEITA_VSET:
                param[0] = reg_all.reg06.JEITA_VSET;
                break;
            case E_TREG:
                param[0] = reg_all.reg06.TREG;
                break;
            case E_TMR2X_EN:
                param[0] = reg_all.reg06.TMR2X_EN;
                break;
            case E_CHG_TIMER:
                param[0] = reg_all.reg06.CHG_TIMER;
                break;
            case E_EN_TIMER:
                param[0] = reg_all.reg06.EN_TIMER;
                break;
            case E_WATCHDOG:
                param[0] = reg_all.reg06.WATCHDOG;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case E_JEITA_ISET:
            reg_all.reg06.JEITA_ISET = param[0];
            break;
        case E_JEITA_VSET:
            reg_all.reg06.JEITA_VSET = param[0];
            break;
        case E_TREG:
            reg_all.reg06.TREG = param[0];
            break;
        case E_TMR2X_EN:
            reg_all.reg06.TMR2X_EN = param[0];
            break;
        case E_CHG_TIMER:
            reg_all.reg06.CHG_TIMER = param[0];
            break;
        case E_EN_TIMER:
            reg_all.reg06.EN_TIMER = param[0];
            break;
        case E_WATCHDOG:
            reg_all.reg06.WATCHDOG = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (hl_i2c_write_reg(i2c_bus, SY_REG06_ADDR, (uint8_t*)&reg_all.reg06)) {
        goto CTL_ERR;
    }
    hl_i2c_read_reg(i2c_bus, SY_REG06_ADDR, &bak_data);
    if (bak_data != *(uint8_t*)&reg_all.reg06) {
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    sy_printf("%s write err !\n", __FUNCTION__);
    return HL_FAILED;
}

static uint8_t hl_reg07_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    uint8_t            bak_data = 0;
    HL_SY6971_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SY_REG07_ADDR, (uint8_t*)&reg_all.reg07)) {
        sy_printf("%s read err !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    if (cmd_typ == SY_READ_CMD) {
        switch (cmd) {
            case E_VINDPM:
                param[0] = reg_all.reg07.VINDPM;
                break;
            case E_Q1_EN:
                param[0] = reg_all.reg07.Q1_EN;
                break;
            case E_NTC_JEITA:
                param[0] = reg_all.reg07.NTC_JEITA;
                break;
            case E_OVP:
                param[0] = reg_all.reg07.OVP;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case E_VINDPM:
            reg_all.reg07.VINDPM = param[0];
            break;
        case E_Q1_EN:
            reg_all.reg07.Q1_EN = param[0];
            break;
        case E_NTC_JEITA:
            reg_all.reg07.NTC_JEITA = param[0];
            break;
        case E_OVP:
            reg_all.reg07.OVP = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (hl_i2c_write_reg(i2c_bus, SY_REG07_ADDR, (uint8_t*)&reg_all.reg07)) {
        goto CTL_ERR;
    }
    hl_i2c_read_reg(i2c_bus, SY_REG07_ADDR, &bak_data);
    if (bak_data != *(uint8_t*)&reg_all.reg07) {
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    sy_printf("%s write err !\n", __FUNCTION__);
    return HL_FAILED;
}

static uint8_t hl_reg08_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    uint8_t            bak_data = 0;
    HL_SY6971_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SY_REG08_ADDR, (uint8_t*)&reg_all.reg08)) {
        sy_printf("%s read err !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    if (cmd_typ == SY_READ_CMD) {
        switch (cmd) {
            case E_VDPM_BAT_TRACK:
                param[0] = reg_all.reg08.VDPM_BAT_TRACK;
                break;
            case E_BATFET_RST_EN:
                param[0] = reg_all.reg08.BATFET_RST_EN;
                break;
            case E_BATFET_DLY:
                param[0] = reg_all.reg08.BATFET_DLY;
                break;
            case E_BATFET_DIS:
                param[0] = reg_all.reg08.BATFET_DIS;
                break;
            case E_BCOLD:
                param[0] = reg_all.reg08.BCOLD;
                break;
            case E_BHOT:
                param[0] = reg_all.reg08.BHOT;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case E_VDPM_BAT_TRACK:
            reg_all.reg08.VDPM_BAT_TRACK = param[0];
            break;
        case E_BATFET_RST_EN:
            reg_all.reg08.BATFET_RST_EN = param[0];
            break;
        case E_BATFET_DLY:
            reg_all.reg08.BATFET_DLY = param[0];
            break;
        case E_BATFET_DIS:
            reg_all.reg08.BATFET_DIS = param[0];
            break;
        case E_BCOLD:
            reg_all.reg08.BCOLD = param[0];
            break;
        case E_BHOT:
            reg_all.reg08.BHOT = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (hl_i2c_write_reg(i2c_bus, SY_REG08_ADDR, (uint8_t*)&reg_all.reg08)) {
        goto CTL_ERR;
    }
    hl_i2c_read_reg(i2c_bus, SY_REG08_ADDR, &bak_data);
    if (bak_data != *(uint8_t*)&reg_all.reg08) {
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    sy_printf("%s write err !\n", __FUNCTION__);
    return HL_FAILED;
}

static uint8_t hl_reg09_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    uint8_t            bak_data = 0;
    HL_SY6971_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SY_REG09_ADDR, (uint8_t*)&reg_all.reg09)) {
        sy_printf("%s read err !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    if (cmd_typ == SY_READ_CMD) {
        switch (cmd) {
            case E_TBATFET_RST:
                param[0] = reg_all.reg09.TBATFET_RST;
                break;
            case E_TQON_RST:
                param[0] = reg_all.reg09.TQON_RST;
                break;
            case E_TSM_DLY:
                param[0] = reg_all.reg09.TSM_DLY;
                break;
            case E_TSR_FAULT:
                param[0] = reg_all.reg09.TSR_FAULT;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case E_TBATFET_RST:
            reg_all.reg09.TBATFET_RST = param[0];
            break;
        case E_TQON_RST:
            reg_all.reg09.TQON_RST = param[0];
            break;
        case E_TSM_DLY:
            reg_all.reg09.TSM_DLY = param[0];
            break;
        case E_TSR_FAULT:
            reg_all.reg09.TSR_FAULT = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (hl_i2c_write_reg(i2c_bus, SY_REG09_ADDR, (uint8_t*)&reg_all.reg09)) {
        goto CTL_ERR;
    }
    hl_i2c_read_reg(i2c_bus, SY_REG09_ADDR, &bak_data);
    if (bak_data != *(uint8_t*)&reg_all.reg09) {
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    sy_printf("%s write err !\n", __FUNCTION__);
    return HL_FAILED;
}

static uint8_t hl_reg0A_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    uint8_t            bak_data = 0;
    HL_SY6971_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SY_REG0A_ADDR, (uint8_t*)&reg_all.reg0A)) {
        sy_printf("%s read err !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    if (cmd_typ == SY_READ_CMD) {
        switch (cmd) {
            case E_IDPM_LIM:
                param[0] = reg_all.reg0A.IDPM_LIM;
                break;
            case E_BOOST_LIM:
                param[0] = reg_all.reg0A.BOOST_LIM;
                break;
            case E_NTC_TOFF:
                param[0] = reg_all.reg0A.NTC_TOFF;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case E_IDPM_LIM:
            reg_all.reg0A.IDPM_LIM = param[0];
            break;
        case E_BOOST_LIM:
            reg_all.reg0A.BOOST_LIM = param[0];
            break;
        case E_NTC_TOFF:
            reg_all.reg0A.NTC_TOFF = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (hl_i2c_write_reg(i2c_bus, SY_REG0A_ADDR, (uint8_t*)&reg_all.reg0A)) {
        goto CTL_ERR;
    }
    hl_i2c_read_reg(i2c_bus, SY_REG0A_ADDR, &bak_data);
    if (bak_data != *(uint8_t*)&reg_all.reg0A) {
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    sy_printf("%s write err !\n", __FUNCTION__);
    return HL_FAILED;
}

static uint8_t hl_reg0B_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    uint8_t            bak_data = 0;
    HL_SY6971_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SY_REG0B_ADDR, (uint8_t*)&reg_all.reg0B)) {
        sy_printf("%s read err !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    if (cmd_typ == SY_READ_CMD) {
        switch (cmd) {
            case E_BOOSTV:
                param[0] = reg_all.reg0B.BOOSTV;
                break;
            case E_SYSR_TIMER:
                param[0] = reg_all.reg0B.SYSR_TIMER;
                break;
            case E_SYSR_NTC:
                param[0] = reg_all.reg0B.SYSR_NTC;
                break;
            case E_LDO_HIZ_EN:
                param[0] = reg_all.reg0B.LDO_HIZ_EN;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case E_BOOSTV:
            reg_all.reg0B.BOOSTV = param[0];
            break;
        case E_SYSR_TIMER:
            reg_all.reg0B.SYSR_TIMER = param[0];
            break;
        case E_SYSR_NTC:
            reg_all.reg0B.SYSR_NTC = param[0];
            break;
        case E_LDO_HIZ_EN:
            reg_all.reg0B.LDO_HIZ_EN = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (hl_i2c_write_reg(i2c_bus, SY_REG0B_ADDR, (uint8_t*)&reg_all.reg0B)) {
        goto CTL_ERR;
    }
    hl_i2c_read_reg(i2c_bus, SY_REG0B_ADDR, &bak_data);
    if (bak_data != *(uint8_t*)&reg_all.reg0B) {
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    sy_printf("%s write err !\n", __FUNCTION__);
    return HL_FAILED;
}

static uint8_t hl_reg0C_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    uint8_t            bak_data = 0;
    HL_SY6971_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SY_REG0C_ADDR, (uint8_t*)&reg_all.reg0C)) {
        sy_printf("%s read err !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    if (cmd_typ == SY_READ_CMD) {
        switch (cmd) {
            case E_VLDO1:
                param[0] = reg_all.reg0C.VLDO1;
                break;
            case E_ILDO1:
                param[0] = reg_all.reg0C.ILDO1;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case E_VLDO1:
            reg_all.reg0C.VLDO1 = param[0];
            break;
        case E_ILDO1:
            reg_all.reg0C.ILDO1 = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (hl_i2c_write_reg(i2c_bus, SY_REG0C_ADDR, (uint8_t*)&reg_all.reg0C)) {
        goto CTL_ERR;
    }
    hl_i2c_read_reg(i2c_bus, SY_REG0C_ADDR, &bak_data);
    if (bak_data != *(uint8_t*)&reg_all.reg0C) {
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    sy_printf("%s write err !\n", __FUNCTION__);
    return HL_FAILED;
}

static uint8_t hl_reg0D_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    uint8_t            bak_data = 0;
    HL_SY6971_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SY_REG0D_ADDR, (uint8_t*)&reg_all.reg0D)) {
        sy_printf("%s read err !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    if (cmd_typ == SY_READ_CMD) {
        switch (cmd) {
            case E_VLDO2:
                param[0] = reg_all.reg0D.VLDO2;
                break;
            case E_ILDO2:
                param[0] = reg_all.reg0D.ILDO2;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case E_VLDO2:
            reg_all.reg0D.VLDO2 = param[0];
            break;
        case E_ILDO2:
            reg_all.reg0D.ILDO2 = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (hl_i2c_write_reg(i2c_bus, SY_REG0D_ADDR, (uint8_t*)&reg_all.reg0D)) {
        goto CTL_ERR;
    }
    hl_i2c_read_reg(i2c_bus, SY_REG0D_ADDR, &bak_data);
    if (bak_data != *(uint8_t*)&reg_all.reg0D) {
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    sy_printf("%s write err !\n", __FUNCTION__);
    return HL_FAILED;
}

static uint8_t hl_reg0E_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    uint8_t            bak_data = 0;
    HL_SY6971_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SY_REG0E_ADDR, (uint8_t*)&reg_all.reg0E)) {
        sy_printf("%s read err !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    if (cmd_typ == SY_READ_CMD) {
        switch (cmd) {
            case E_TOPOFF_TIMER:
                param[0] = reg_all.reg0E.TOPOFF_TIMER;
                break;
            case E_Q1_FULLON:
                param[0] = reg_all.reg0E.Q1_FULLON;
                break;
            case E_FORCE_AICL:
                param[0] = reg_all.reg0E.FORCE_AICL;
                break;
            case E_AICL_EN:
                param[0] = reg_all.reg0E.AICL_EN;
                break;
            case E_TQON_LOW:
                param[0] = reg_all.reg0E.TQON_LOW;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case E_TOPOFF_TIMER:
            reg_all.reg0E.TOPOFF_TIMER = param[0];
            break;
        case E_Q1_FULLON:
            reg_all.reg0E.Q1_FULLON = param[0];
            break;
        case E_FORCE_AICL:
            reg_all.reg0E.FORCE_AICL = param[0];
            break;
        case E_AICL_EN:
            reg_all.reg0E.AICL_EN = param[0];
            break;
        case E_TQON_LOW:
            reg_all.reg0E.TQON_LOW = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (hl_i2c_write_reg(i2c_bus, SY_REG0E_ADDR, (uint8_t*)&reg_all.reg0E)) {
        goto CTL_ERR;
    }
    hl_i2c_read_reg(i2c_bus, SY_REG0E_ADDR, &bak_data);
    if (bak_data != *(uint8_t*)&reg_all.reg0E) {
        goto CTL_ERR;
    }
CTL_ERR:
    return HL_SUCCESS;
// CTL_ERR:
    // sy_printf("%s write err !\n", __FUNCTION__);
    // return HL_FAILED;
}

static uint8_t hl_reg0F_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SY6971_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SY_REG0F_ADDR, (uint8_t*)&reg_all.reg0F)) {
        sy_printf("%s read err !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    switch (cmd) {
        case E_VSYS_STAT:
            param[0] = reg_all.reg0F.VSYS_STAT;
            break;
        case E_THERM_STAT:
            param[0] = reg_all.reg0F.THERM_STAT;
            break;
        case E_PG_STAT:
            param[0] = reg_all.reg0F.PG_STAT;
            break;
        case E_CHRG_STAT:
            param[0] = reg_all.reg0F.CHRG_STAT;
            break;
        case E_BUS_STAT:
            param[0] = reg_all.reg0F.BUS_STAT;
            break;
        default:
            goto CTL_ERR;
            break;
    }
    return HL_SUCCESS;
CTL_ERR:
    sy_printf("%s err !\n", __FUNCTION__);
    return HL_FAILED;
}

static uint8_t hl_reg10_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SY6971_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SY_REG10_ADDR, (uint8_t*)&reg_all.reg10)) {
        sy_printf("%s read err !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    switch (cmd) {
        case E_LDO2_FAULT:
            param[0] = reg_all.reg10.LDO2_FAULT;
            break;
        case E_LDO1_FAULT:
            param[0] = reg_all.reg10.LDO1_FAULT;
            break;
        case E_ACOV_STAT:
            param[0] = reg_all.reg10.ACOV_STAT;
            break;
        case E_BAT_FAULT:
            param[0] = reg_all.reg10.BAT_FAULT;
            break;
        case E_CHRG_FAULT:
            param[0] = reg_all.reg10.CHRG_FAULT;
            break;
        case E_BOOST_FAULT:
            param[0] = reg_all.reg10.BOOST_FAULT;
            break;
        case E_WATCHDOG_FAULT:
            param[0] = reg_all.reg10.WATCHDOG_FAULT;
            break;
        default:
            goto CTL_ERR;
            break;
    }
    return HL_SUCCESS;
CTL_ERR:
    sy_printf("%s err !\n", __FUNCTION__);
    return HL_FAILED;
}

static uint8_t hl_reg11_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    HL_SY6971_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SY_REG11_ADDR, (uint8_t*)&reg_all.reg11)) {
        sy_printf("%s read err !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    switch (cmd) {
        case E_NTC_FAULT:
            param[0] = reg_all.reg11.NTC_FAULT;
            break;
        case E_AICL_OPTIMIZED:
            param[0] = reg_all.reg11.AICL_OPTIMIZED;
            break;
        case E_TOPOFF_ACTIVE:
            param[0] = reg_all.reg11.TOPOFF_ACTIVE;
            break;
        case E_BATSNS_STAT:
            param[0] = reg_all.reg11.BATSNS_STAT;
            break;
        case E_IDPM_STAT:
            param[0] = reg_all.reg11.IDPM_STAT;
            break;
        case E_VDPM_STAT:
            param[0] = reg_all.reg11.VDPM_STAT;
            break;
        default:
            goto CTL_ERR;
            break;
    }
    return HL_SUCCESS;
CTL_ERR:
    sy_printf("%s err !\n", __FUNCTION__);
    return HL_FAILED;
}

static uint8_t hl_reg12_ctl(uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    uint8_t            bak_data = 0;
    HL_SY6971_REGALL_T reg_all;
    if (hl_i2c_read_reg(i2c_bus, SY_REG12_ADDR, (uint8_t*)&reg_all.reg12)) {
        sy_printf("%s read err !\n", __FUNCTION__);
        goto CTL_ERR;
    }
    if (cmd_typ == SY_READ_CMD) {
        switch (cmd) {
            case E_DEV_REV:
                param[0] = reg_all.reg12.DEV_REV;
                break;
            case E_PN:
                param[0] = reg_all.reg12.PN;
                break;
            case E_BYPASS_EN:
                param[0] = reg_all.reg12.BYPASS_EN;
                break;
            case E_REG_RST:
                param[0] = reg_all.reg12.REG_RST;
                break;
            default:
                goto CTL_ERR;
                break;
        }
        return HL_SUCCESS;
    }
    switch (cmd) {
        case E_DEV_REV:
            reg_all.reg12.DEV_REV = param[0];
            break;
        case E_PN:
            reg_all.reg12.PN = param[0];
            break;
        case E_BYPASS_EN:
            reg_all.reg12.BYPASS_EN = param[0];
            break;
        case E_REG_RST:
            reg_all.reg12.REG_RST = param[0];
            break;
        default:
            goto CTL_ERR;
            break;
    }
    if (hl_i2c_write_reg(i2c_bus, SY_REG12_ADDR, (uint8_t*)&reg_all.reg12)) {
        goto CTL_ERR;
    }
    hl_i2c_read_reg(i2c_bus, SY_REG12_ADDR, &bak_data);
    if (bak_data != *(uint8_t*)&reg_all.reg12) {
        goto CTL_ERR;
    }
    return HL_SUCCESS;
CTL_ERR:
    sy_printf("%s write err !\n", __FUNCTION__);
    return HL_FAILED;
}

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
    fun_arr[16] = hl_reg10_ctl;
    fun_arr[17] = hl_reg11_ctl;
    fun_arr[18] = hl_reg12_ctl;
}

static void hl_reg_ctl_fun_deinit(void)
{
    uint8_t i;
    for (i = 0; i < SY_REG_NUM; i++)
        fun_arr[i] = NULL;
}

static void hl_reg_arr_init(void)
{
    uint8_t i;
    for (i = 0; i < SY_REG_NUM; i++)
        reg_arr[i] = i;
}

static void hl_reg_arr_deinit(void)
{
    uint8_t i;
    for (i = 0; i < SY_REG_NUM; i++)
        reg_arr[i] = 0;
}

static uint8_t hl_reg_ctl(uint8_t reg_ser, uint8_t cmd, uint8_t cmd_typ, uint8_t* param)
{
    uint8_t i;
    for (i = 0; i < SY_REG_NUM; i++) {
        if (reg_arr[i] == reg_ser) {
            if (fun_arr[i](cmd, cmd_typ, param))
                return HL_FAILED;
            return HL_SUCCESS;
        }
    }
    return HL_FAILED;
}

static uint8_t hl_get_reg_serial(uint8_t cfg_opt)
{
    uint8_t reg_ser;

    switch (cfg_opt) {
        //reg 0x00
        case E_IINLIM:
        case E_LDO_MODE2:
        case E_LDO_MODE1:
        case E_EN_HIZ:
            reg_ser = SY_REG00_ADDR;
            break;
        //reg 0x01
        case E_OTG_BAT:
        case E_SYS_MIN:
        case E_CHG_CONFIG:
        case E_OTG_CONFIG:
        case E_WD_RST:
        case E_PFM_DIS:
            reg_ser = SY_REG01_ADDR;
            break;
        //reg 0x02
        case E_ICHG:
        case E_BATSNS_DIS:
            reg_ser = SY_REG02_ADDR;
            break;
        //reg 0x03
        case E_IPRECHG:
        case E_IINDPM_INT_MASK:
        case E_VINDPM_INT_MASK:
            reg_ser = SY_REG03_ADDR;
            break;
        //reg 0x04
        case E_ITERM:
        case E_Reserved_1:
        case E_ISHORT:
            reg_ser = SY_REG04_ADDR;
            break;
        //reg 0x05
        case E_VRECHG:
        case E_EN_TERM:
        case E_VREG:
            reg_ser = SY_REG05_ADDR;
            break;
        //reg 0x06
        case E_JEITA_ISET:
        case E_JEITA_VSET:
        case E_TREG:
        case E_TMR2X_EN:
        case E_CHG_TIMER:
        case E_EN_TIMER:
        case E_WATCHDOG:
            reg_ser = SY_REG06_ADDR;
            break;
        //reg 0x07
        case E_VINDPM:
        case E_Q1_EN:
        case E_NTC_JEITA:
        case E_OVP:
            reg_ser = SY_REG07_ADDR;
            break;
        //reg 0x08
        case E_VDPM_BAT_TRACK:
        case E_BATFET_RST_EN:
        case E_BATFET_DLY:
        case E_BATFET_DIS:
        case E_BCOLD:
        case E_BHOT:
            reg_ser = SY_REG08_ADDR;
            break;
        //reg 0x09
        case E_TBATFET_RST:
        case E_TQON_RST:
        case E_TSM_DLY:
        case E_TSR_FAULT:
            reg_ser = SY_REG09_ADDR;
            break;
        //reg 0x0A
        case E_IDPM_LIM:
        case E_BOOST_LIM:
        case E_NTC_TOFF:
            reg_ser = SY_REG0A_ADDR;
            break;
        //reg 0x0B
        case E_BOOSTV:
        case E_SYSR_TIMER:
        case E_SYSR_NTC:
        case E_Reserved_2:
        case E_LDO_HIZ_EN:
            reg_ser = SY_REG0B_ADDR;
            break;
        //reg 0x0C
        case E_VLDO1:
        case E_ILDO1:
            reg_ser = SY_REG0C_ADDR;
            break;
        //reg 0x0D
        case E_VLDO2:
        case E_ILDO2:
            reg_ser = SY_REG0D_ADDR;
            break;
        //reg 0x0E
        case E_TOPOFF_TIMER:
        case E_Q1_FULLON:
        case E_FORCE_AICL:
        case E_AICL_EN:
        case E_TQON_LOW:
            reg_ser = SY_REG0E_ADDR;
            break;
        //reg 0x12
        case E_DEV_REV:
        case E_PN:
        case E_BYPASS_EN:
        case E_REG_RST:
            reg_ser = SY_REG12_ADDR;
            break;
        //0x0F
        case E_VSYS_STAT:
        case E_THERM_STAT:
        case E_PG_STAT:
        case E_CHRG_STAT:
        case E_BUS_STAT:
            reg_ser = SY_REG0F_ADDR;
            break;
        //0x10
        case E_LDO2_FAULT:
        case E_LDO1_FAULT:
        case E_ACOV_STAT:
        case E_BAT_FAULT:
        case E_CHRG_FAULT:
        case E_BOOST_FAULT:
        case E_WATCHDOG_FAULT:
            reg_ser = SY_REG10_ADDR;
            break;
        //0x11
        case E_NTC_FAULT:
        case E_AICL_OPTIMIZED:
        case E_TOPOFF_ACTIVE:
        case E_BATSNS_STAT:
        case E_IDPM_STAT:
        case E_VDPM_STAT:
            reg_ser = SY_REG11_ADDR;
            break;
        default:
            sy_printf("Register : [ %X ] overrun !\n", cfg_opt);
            return 0xFF;
            break;
    }
    return reg_ser;
}

static uint8_t hl_reg_type_get(uint8_t cmd)
{
    if (cmd < E_VSYS_STAT)
        return SY_WRITE_CMD;
    return SY_READ_CMD;
}

/* Exported functions --------------------------------------------------------*/
int hl_drv_sy6971_init(void)
{
    uint8_t             reg_config[SY_REG_NUM] = { 0 };
    HL_SY6971_REGALL_T reg_all;
    /* 查找I2C总线设备，获取I2C总线设备句柄 */
    i2c_bus = (struct rt_i2c_bus_device*)rt_device_find(SY6971_IIC_NAME);
    hl_reg_arr_init();
    hl_reg_ctl_fun_init();

    if (i2c_bus == RT_NULL) {
        sy_printf("can't find %s device!\n", SY6971_IIC_NAME);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SY_REG00_ADDR, (uint8_t*)&reg_all.reg00)) {
        sy_printf("cfg_opt %d read err !\n", SY_REG00_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SY_REG01_ADDR, (uint8_t*)&reg_all.reg01)) {
        sy_printf("cfg_opt %d read err !\n", SY_REG01_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SY_REG02_ADDR, (uint8_t*)&reg_all.reg02)) {
        sy_printf("cfg_opt %d read err !\n", SY_REG02_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SY_REG04_ADDR, (uint8_t*)&reg_all.reg04)) {
        sy_printf("cfg_opt %d read err !\n", SY_REG04_ADDR);
        goto INIT_ERR;
    }
    if (hl_i2c_read_reg(i2c_bus, SY_REG05_ADDR, (uint8_t*)&reg_all.reg05)) {
        sy_printf("cfg_opt %d read err !\n", SY_REG05_ADDR);
        goto INIT_ERR;
    }
    // if (hl_i2c_read_reg(i2c_bus, SY_REG06_ADDR, (uint8_t*)&reg_all.reg06)) {
    //     sy_printf("cfg_opt %d read err !\n", SY_REG06_ADDR);
    //     goto INIT_ERR;
    // }
    if (hl_i2c_read_reg(i2c_bus, SY_REG07_ADDR, (uint8_t*)&reg_all.reg07)) {
        sy_printf("cfg_opt %d read err !\n", SY_REG07_ADDR);
        goto INIT_ERR;
    }    

    if (hl_i2c_read_reg(i2c_bus, SY_REG08_ADDR, (uint8_t*)&reg_all.reg08)) {
        sy_printf("cfg_opt %d read err !\n", SY_REG08_ADDR);
        goto INIT_ERR;
    }   
    
    if (hl_i2c_read_reg(i2c_bus, SY_REG0E_ADDR, (uint8_t*)&reg_all.reg0E)) {
        sy_printf("cfg_opt %d read err !\n", SY_REG0E_ADDR);
        goto INIT_ERR;
    }

    reg_all.reg00.IINLIM = 7;
    reg_all.reg01.SYS_MIN = 4;
#if HL_IS_TX_DEVICE()
    reg_all.reg02.ICHG = 14; //Tx
#else
    reg_all.reg02.ICHG = 22; //Rx
#endif
    reg_all.reg04.ITERM = 1;

    reg_all.reg05.VREG = 45;
    reg_all.reg07.NTC_JEITA = 1;

    reg_all.reg08.BHOT = 3;

    reg_all.reg0E.Q1_FULLON = 1;
    // reg_all.reg07.VINDPM = 2;

    // reg_all.reg06.WATCHDOG = 0;

    // reg_all.reg0E.AICL_EN = 1;
    // reg_all.reg0E.FORCE_AICL = 1;

    if (hl_i2c_write_reg(i2c_bus, SY_REG00_ADDR, (uint8_t*)&reg_all.reg00)) {
        sy_printf("cfg_opt %d write err !\n", SY_REG00_ADDR);
        goto INIT_ERR;
    }  
    if (hl_i2c_write_reg(i2c_bus, SY_REG01_ADDR, (uint8_t*)&reg_all.reg01)) {
        sy_printf("cfg_opt %d write err !\n", SY_REG01_ADDR);
        goto INIT_ERR;
    }  
    if (hl_i2c_write_reg(i2c_bus, SY_REG02_ADDR, (uint8_t*)&reg_all.reg02)) {
        sy_printf("cfg_opt %d write err !\n", SY_REG02_ADDR);
        goto INIT_ERR;
    }  
    if (hl_i2c_write_reg(i2c_bus, SY_REG04_ADDR, (uint8_t*)&reg_all.reg04)) {
        sy_printf("cfg_opt %d write err !\n", SY_REG04_ADDR);
        goto INIT_ERR;
    } 
    if (hl_i2c_write_reg(i2c_bus, SY_REG05_ADDR, (uint8_t*)&reg_all.reg05)) {
        sy_printf("cfg_opt %d write err !\n", SY_REG05_ADDR);
        goto INIT_ERR;
    }       
    // if (hl_i2c_write_reg(i2c_bus, SY_REG06_ADDR, (uint8_t*)&reg_all.reg06)) {
    //     sy_printf("cfg_opt %d write err !\n", SY_REG06_ADDR);
    //     goto INIT_ERR;
    // }  
    if (hl_i2c_write_reg(i2c_bus, SY_REG07_ADDR, (uint8_t*)&reg_all.reg07)) {
        sy_printf("cfg_opt %d write err !\n", SY_REG07_ADDR);
        goto INIT_ERR;
    } 
    if (hl_i2c_write_reg(i2c_bus, SY_REG08_ADDR, (uint8_t*)&reg_all.reg08)) {
        sy_printf("cfg_opt %d write err !\n", SY_REG08_ADDR);
        goto INIT_ERR;
    } 
    if (hl_i2c_write_reg(i2c_bus, SY_REG0E_ADDR, (uint8_t*)&reg_all.reg0E)) {
        sy_printf("cfg_opt %d write err !\n", SY_REG0E_ADDR);
        goto INIT_ERR;
    } 

    sy_printf("SY6971 init success !\n");
    return HL_SUCCESS;
INIT_ERR:
    sy_printf("SY6971 init fail !\n");
    hl_reg_arr_deinit();
    hl_reg_ctl_fun_deinit();
    return HL_FAILED;
}

int hl_drv_sy6971_io_ctrl(uint8_t cmd, void* ptr, uint8_t len)
{
    uint8_t              reg_cmd, reg_serial, cmd_typ;
    HL_SY_INPUT_PARAM_T* param = (HL_SY_INPUT_PARAM_T*)ptr;
    if (ptr == NULL || len != 1) {
        sy_printf("Sy6971 param err ! len : [ %X ]\n", len);
        return HL_FAILED;
    }
    cmd_typ = cmd;
    if ((cmd_typ != hl_reg_type_get(param->cfg_opt)) && (cmd_typ == SY_WRITE_CMD)) {
        sy_printf("Sy6971 register0 : [ %X ] type err !\n", cmd);
        return HL_SUCCESS;
    }
    reg_cmd    = param->cfg_opt;
    reg_serial = hl_get_reg_serial(reg_cmd);
    if (cmd_typ == SY_WRITE_CMD && (reg_cmd > E_REG_RST)) {
        sy_printf("Sy6971 register1 : [ %X ] type err !\n", cmd);
        return HL_SUCCESS;
    }

    if (hl_reg_ctl(reg_serial, reg_cmd, cmd_typ, &param->param)) {
        sy_printf("Sy6971 hl_reg_ctl fail !\n");
        return HL_FAILED;
    }
    return HL_SUCCESS;
}

// static uint8_t get_buf[24];
// static uint8_t com_buf[24];

// void sy6971_get_data(int argc, char** argv)
// {
//     uint8_t i = 0;
//     for(i = SY_REG00_ADDR;i<SY_REG_NUM;i++){
//         if(hl_i2c_read_reg(i2c_bus, i, &get_buf[i])){
//             sy_printf("sy6971_get_data read fail ! %d\n",i);
//             return ;
//         }
//     }
//     sy_printf("sy6971_get_data success !\n");
// }

// void sy6971_com_data(int argc, char** argv)
// {
//     uint8_t i = 0;
//     for(i = SY_REG00_ADDR;i<SY_REG_NUM;i++){
//         if(hl_i2c_read_reg(i2c_bus, i, &com_buf[i])){
//             sy_printf("sy6971_com_data read fail ! %d\n",i);
//             return ;
//         }
//     }
//     for(i = SY_REG00_ADDR;i<SY_REG_NUM;i++){
//         if(get_buf[i] != com_buf[i]){
//             sy_printf("get_buf[%d] = %X   com_buf[%d] = %X\n",i,get_buf[i],i,com_buf[i]);
//         }
//     }
// }

// MSH_CMD_EXPORT(sy6971_get_data, run sy6971_get_data);
// MSH_CMD_EXPORT(sy6971_com_data, run sy6971_com_data);

void hl_drv_sy6971_test(int argc, char** argv)
{
    HL_SY_INPUT_PARAM_T par;
    if (argc != 4) {
        sy_printf("argc param err : %d \n", argc);
        return;
    }

    uint8_t cmd     = atoi(argv[1]);
    uint8_t cfg_opt = atoi(argv[2]);
    uint8_t data    = atoi(argv[3]);

    hl_reg_arr_init();
    hl_reg_ctl_fun_init();

    par.cfg_opt = cfg_opt;
    par.param   = data;

    i2c_bus = (struct rt_i2c_bus_device*)rt_device_find(SY6971_IIC_NAME);

    if (i2c_bus == RT_NULL) {
        sy_printf("can't find %s device!\n", SY6971_IIC_NAME);
    }

    if (hl_drv_sy6971_io_ctrl(cmd, &par, 1) != HL_SUCCESS) {
        sy_printf("hl_drv_sy6971_io_ctrl fail!\n");
    }
    sy_printf("par.param = 0x%02X \n", par.param);
}

MSH_CMD_EXPORT(hl_drv_sy6971_init, run hl_drv_sy6971_init);
MSH_CMD_EXPORT(hl_drv_sy6971_test, run hl_drv_sy6971_test);

/*
 * EOF
 */