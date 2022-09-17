/**
 * @file hl_drv_aw21009.c
 * @author lilin (lin.li@hollyland-tech.com)
 * @brief LED驱动源文件
 * @version 1.0
 * @date 2022-09-17
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
 * <tr><td>2022-09-17     <td>v1.0     <td>lilin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/

#include "hl_drv_aw21009.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/

/*****************************************************
* define macro
*****************************************************/
#define DBG_LOG rt_kprintf

#define HL_DRV_AW21009_I2C_NAME "i2c2"

#if HL_GET_DEVICE_TYPE() == 0
///Rx
#define HL_DRV_AW21009_OTHER_DEV_ADDR (0x21)
#endif

#define AW210XX_I2C_ADDR (0x20)
#define AW210XX_I2C_RW_RETRIES (5)
#define AW210XX_READ_CHIPID_RETRIES (5)
#define AW210XX_REGADD_SIZE_8BIT (1)
#define AW210XX_REGDATA_SIZE_8BIT (1)

/*****************************************************
* register about led mode
*****************************************************/
#define AW210XX_REG_GCR (0x20)
#define AW210XX_REG_BR00L (0x21)
#define AW210XX_REG_BR00H (0x22)
#define AW210XX_REG_BR01L (0x23)
#define AW210XX_REG_BR01H (0x24)
#define AW210XX_REG_BR02L (0x25)
#define AW210XX_REG_BR02H (0x26)
#define AW210XX_REG_BR03L (0x27)
#define AW210XX_REG_BR03H (0x28)
#define AW210XX_REG_BR04L (0x29)
#define AW210XX_REG_BR04H (0x2A)
#define AW210XX_REG_BR05L (0x2B)
#define AW210XX_REG_BR05H (0x2C)
#define AW210XX_REG_BR06L (0x2D)
#define AW210XX_REG_BR06H (0x2E)
#define AW210XX_REG_BR07L (0x2F)
#define AW210XX_REG_BR07H (0x30)
#define AW210XX_REG_BR08L (0x31)
#define AW210XX_REG_BR08H (0x32)
#define AW210XX_REG_BR09L (0x33)
#define AW210XX_REG_BR09H (0x34)
#define AW210XX_REG_BR10L (0x35)
#define AW210XX_REG_BR10H (0x36)
#define AW210XX_REG_BR11L (0x37)
#define AW210XX_REG_BR11H (0x38)
#define AW210XX_REG_BR12L (0x39)
#define AW210XX_REG_BR12H (0x3A)
#define AW210XX_REG_BR13L (0x3B)
#define AW210XX_REG_BR13H (0x3C)
#define AW210XX_REG_BR14L (0x3D)
#define AW210XX_REG_BR14H (0x3E)
#define AW210XX_REG_BR15L (0x3F)
#define AW210XX_REG_BR15H (0x40)
#define AW210XX_REG_BR16L (0x41)
#define AW210XX_REG_BR16H (0x42)
#define AW210XX_REG_BR17L (0x43)
#define AW210XX_REG_BR17H (0x44)
#define AW210XX_REG_UPDATE (0x45)
#define AW210XX_REG_SL00 (0x46)
#define AW210XX_REG_SL01 (0x47)
#define AW210XX_REG_SL02 (0x48)
#define AW210XX_REG_SL03 (0x49)
#define AW210XX_REG_SL04 (0x4A)
#define AW210XX_REG_SL05 (0x4B)
#define AW210XX_REG_SL06 (0x4C)
#define AW210XX_REG_SL07 (0x4D)
#define AW210XX_REG_SL08 (0x4E)
#define AW210XX_REG_SL09 (0x4F)
#define AW210XX_REG_SL10 (0x50)
#define AW210XX_REG_SL11 (0x51)
#define AW210XX_REG_SL12 (0x52)
#define AW210XX_REG_SL13 (0x53)
#define AW210XX_REG_SL14 (0x54)
#define AW210XX_REG_SL15 (0x55)
#define AW210XX_REG_SL16 (0x56)
#define AW210XX_REG_SL17 (0x57)
#define AW210XX_REG_GCCR (0x58)
#define AW210XX_REG_PHCR (0x59)
#define AW210XX_REG_OSDCR (0x5A)
#define AW210XX_REG_OSST0 (0x5B)
#define AW210XX_REG_OSST1 (0x5C)
#define AW210XX_REG_OSST2 (0x5D)
#define AW210XX_REG_OTCR (0x5E)
#define AW210XX_REG_SSCR (0x5F)
#define AW210XX_REG_UVCR (0x60)
#define AW210XX_REG_GCR2 (0x61)
#define AW210XX_REG_GCR3 (0x62)
#define AW210XX_REG_RESET (0x70)
#define AW210XX_REG_ABMCFG (0x80)
#define AW210XX_REG_ABMGO (0x81)
#define AW210XX_REG_ABMT0 (0x82)
#define AW210XX_REG_ABMT1 (0x83)
#define AW210XX_REG_ABMT2 (0x84)
#define AW210XX_REG_ABMT3 (0x85)
#define AW210XX_REG_GBRH (0x86)
#define AW210XX_REG_GBRL (0x87)
#define AW210XX_REG_GSLR (0x88)
#define AW210XX_REG_GSLG (0x89)
#define AW210XX_REG_GSLB (0x8A)
#define AW210XX_REG_GCFG (0x8B)

/*****************************************************
 * define register Detail
*****************************************************/
#define AW210XX_BIT_APSE_MASK (~(1 << 7))
#define AW210XX_BIT_APSE_ENABLE (1 << 7)
#define AW210XX_BIT_APSE_DISENA (0 << 7)
#define AW210XX_BIT_CHIPEN_MASK (~(1 << 0))
#define AW210XX_BIT_CHIPEN_ENABLE (1 << 0)
#define AW210XX_BIT_CHIPEN_DISENA (0 << 0)
#define AW210XX_BIT_UVPD_MASK (~(1 << 1))
#define AW210XX_BIT_UVPD_ENABLE (0 << 1)
#define AW210XX_BIT_UVPD_DISENA (1 << 1)
#define AW210XX_BIT_UVDIS_MASK (~(1 << 0))
#define AW210XX_BIT_UVDIS_ENABLE (0 << 0)
#define AW210XX_BIT_UVDIS_DISENA (1 << 0)
#define AW210XX_BIT_ABME_MASK (~(1 << 0))
#define AW210XX_BIT_ABME_ENABLE (1 << 0)
#define AW210XX_BIT_ABME_DISENA (0 << 0)
#define AW210XX_BIT_GSLDIS_MASK (~(1 << 6))
#define AW210XX_BIT_GSLDIS_ENABLE (0 << 6)
#define AW210XX_BIT_GSLDIS_DISENA (1 << 6)
#define AW210XX_BIT_RGBMD_MASK (~(1 << 0))
#define AW210XX_BIT_RGBMD_ENABLE (1 << 0)
#define AW210XX_BIT_RGBMD_DISENA (0 << 0)
#define AW210XX_BIT_SBMD_MASK (~(1 << 1))
#define AW210XX_BIT_SBMD_ENABLE (1 << 1)
#define AW210XX_BIT_SBMD_DISENA (0 << 1)
#define AW210XX_BIT_CLKFRQ_MASK (~(7 << 4))
#define AW210XX_BIT_CLKFRQ_16MH (0 << 4)
#define AW210XX_BIT_CLKFRQ_8MH (1 << 4)
#define AW210XX_BIT_CLKFRQ_4MH (2 << 4)
#define AW210XX_BIT_CLKFRQ_2MH (3 << 4)
#define AW210XX_BIT_CLKFRQ_1MH (4 << 4)
#define AW210XX_BIT_CLKFRQ_512KH (5 << 4)
#define AW210XX_BIT_CLKFRQ_256KH (6 << 4)
#define AW210XX_BIT_CLKFRQ_125KH (7 << 4)

#define AW210XX_BIT_PWMRES_MASK (~(3 << 1))
#define AW210XX_BIT_PWMRES_8BIT (0 << 1)
#define AW210XX_BIT_PWMRES_9BIT (1 << 1)
#define AW210XX_BIT_PWMRES_12BIT (2 << 1)
#define AW210XX_BIT_PWMRES_9_AND_3_BIT (3 << 1)

#define AW210XX_DCPWM_SET (7 << 5)
#define AW210XX_DCPWM_CLEAN (0x00)
#define AW210XX_DCPWM_SET_MASK ~(7 << 5)
#define AW210XX_OPEN_THRESHOLD_SET (1 << 3)
#define AW210XX_OPEN_THRESHOLD_SET_MASK ~(1 << 3)
#define AW210XX_SHORT_THRESHOLD_SET (1 << 2)
#define AW210XX_SHORT_THRESHOLD_SET_MASK ~(1 << 2)

/*****************************************************
 * define register data
*****************************************************/
#define AW210XX_RESET_CHIP (0x00)
#define AW210XX_UPDATE_BR_SL (0x00)
#define AW21018_GROUP_ENABLE (0x3F)
#define AW21018_GROUP_DISABLE (0x40)
#define AW21012_GROUP_ENABLE (0x0F)
#define AW21012_GROUP_DISABLE (0x40)
#define AW21009_GROUP_ENABLE (0x07)
#define AW21009_GROUP_DISABLE (0x40)
#define AW210XX_GLOBAL_DEFAULT_SET (0x66)
#define AW210XX_GBRH_DEFAULT_SET (0x60)
#define AW210XX_GBRL_DEFAULT_SET (0x00)
#define AW210XX_ABMT0_SET (0x99)
#define AW210XX_ABMT1_SET (0x99)
#define AW210XX_ABMT2_SET (0x00)
#define AW210XX_ABMT3_SET (0x00)
#define AW210XX_ABMCFG_SET (0x03)
#define AW210XX_ABMGO_SET (0x01)

/*****************************************************
* define chipid info
*****************************************************/
#define AW21018_CHIPID (0x02)
#define AW21012_CHIPID (0x22)
#define AW21009_CHIPID (0x12)

/*****************************************************
* define return value
*****************************************************/
#define AW210XX_RETURN_OK (0)
#define AW210XX_CHIPID_FAILD (1)
#define AW210XX_DATA_ERROR (2)
#define AW210XX_INPUT_ERROR (3)
#define AW210XX_NO_DECTECT (4)
#define AW210XX_NO_GPIO_INFO (5)
#define AW210XX_NO_ACCESS (6)
#define AW210XX_WRITE_FAIL (7)
#define AW210XX_EFFECT_MODE_UNSUPPORT (8)
#define AW210XX_CLK_MODE_UNSUPPORT (9)
#define AW210XX_MALLOC_FAILEDT (10)

/***********************************************
 * define register Detail
 ***********************************************/
#define REG_NONE_ACCESS (0)
#define REG_RD_ACCESS (1 << 0)
#define REG_WR_ACCESS (1 << 1)
#define AW210XX_REG_MAX (0x8C)
#define AW210XX_OPEN_DETECT_EN (0x03)
#define AW210XX_SHORT_DETECT_EN (0x02)
#define AW210XX_OPEN_SHORT_DIS (0x00)

#define AW210XX_DRIVER_VERSION "V0.3.0"

/* variables -----------------------------------------------------------------*/

static struct rt_i2c_bus_device* _p_i2c_bus = NULL;
static uint8_t                   _init_flag = 0;

/* Private function(only *.c)  -----------------------------------------------*/

static inline int aw_read(uint8_t dev_addr, uint8_t PointReg, uint8_t* pData)
{
    struct rt_i2c_msg msgs[2] = { 0 };

    msgs[0].addr  = dev_addr;
    msgs[0].flags = RT_I2C_WR;  // | RT_I2C_NO_STOP;
    msgs[0].buf   = &PointReg;
    msgs[0].len   = 1;

    msgs[1].addr  = dev_addr;
    msgs[1].flags = RT_I2C_RD | RT_I2C_NO_READ_ACK;
    msgs[1].buf   = pData;
    msgs[1].len   = 1;

    if (rt_i2c_transfer(_p_i2c_bus, msgs, 2) == 2) {
        return AW21009_FUNC_RET_OK;
    } else {
        return AW21009_FUNC_RET_ERR;
    }
}

static inline int aw_write(uint8_t dev_addr, uint8_t PointReg, uint8_t* pData)
{
    struct rt_i2c_msg msgs[1] = { 0 };
    uint8_t           buf[2];

    buf[0] = PointReg;
    buf[1] = *pData;

    msgs[0].addr  = dev_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = buf;
    msgs[0].len   = 2;

    if (rt_i2c_transfer(_p_i2c_bus, msgs, 1) == 1) {
        return AW21009_FUNC_RET_OK;
    } else {
        return AW21009_FUNC_RET_ERR;
    }
}

static uint8_t led_soft_i2c_readreg(uint8_t Dev_Addr, uint8_t Regis_Addr)
{
    uint8_t reg_val = 0xFF;

    aw_read(Dev_Addr, Regis_Addr, &reg_val);

    return reg_val;
}

static uint8_t led_soft_i2c_writereg(uint8_t Dev_Addr, uint8_t Regis_Addr, uint8_t Data)
{
    int     ret;
    uint8_t reg_val = Data;

    ret = aw_write(Dev_Addr, Regis_Addr, &reg_val);
    if (ret == AW21009_FUNC_RET_ERR) {
        return -1;
    }

    return 0;
}

static int32_t aw210xx_write_bits(uint8_t base_addr, uint8_t reg_addr, uint32_t reg_mask, uint8_t reg_data)
{
    uint8_t val;

    val = led_soft_i2c_readreg(base_addr, reg_addr);
    val &= reg_mask;
    val |= reg_data;
    led_soft_i2c_writereg(base_addr, reg_addr, val);

    return 0;
}

static void aw210xx_global_set(uint8_t dev_addr, uint8_t data)
{
    led_soft_i2c_writereg(dev_addr, AW210XX_REG_GCCR, data);
}

/*****************************************************
* aw210xx led function set
*****************************************************/
static int32_t aw210xx_clk_pwm_set(uint8_t dev_addr, clk_pwm_e clk_pwm)
{
    switch (clk_pwm) {
        case CLK_FRQ_16M:
            // AWLOGD("%s:osc is 16MHz!\n", __func__);
            aw210xx_write_bits(dev_addr, AW210XX_REG_GCR, AW210XX_BIT_CLKFRQ_MASK, AW210XX_BIT_CLKFRQ_16MH);
            break;
        case CLK_FRQ_8M:
            // AWLOGD("%s:osc is 8MHz!\n", __func__);
            aw210xx_write_bits(dev_addr, AW210XX_REG_GCR, AW210XX_BIT_CLKFRQ_MASK, AW210XX_BIT_CLKFRQ_8MH);
            break;
        case CLK_FRQ_4M:
            // AWLOGD("%s:osc is 4MHz!\n", __func__);
            aw210xx_write_bits(dev_addr, AW210XX_REG_GCR, AW210XX_BIT_CLKFRQ_MASK, AW210XX_BIT_CLKFRQ_4MH);
            break;
        case CLK_FRQ_2M:
            // AWLOGD("%s:osc is 2MHz!\n", __func__);
            aw210xx_write_bits(dev_addr, AW210XX_REG_GCR, AW210XX_BIT_CLKFRQ_MASK, AW210XX_BIT_CLKFRQ_2MH);
            break;
        case CLK_FRQ_1M:
            // AWLOGD("%s:osc is 1MHz!\n", __func__);
            aw210xx_write_bits(dev_addr, AW210XX_REG_GCR, AW210XX_BIT_CLKFRQ_MASK, AW210XX_BIT_CLKFRQ_1MH);
            break;
        case CLK_FRQ_512K:
            // AWLOGD("%s:osc is 512KHz!\n", __func__);
            aw210xx_write_bits(dev_addr, AW210XX_REG_GCR, AW210XX_BIT_CLKFRQ_MASK, AW210XX_BIT_CLKFRQ_512KH);
            break;
        case CLK_FRQ_256K:
            // AWLOGD("%s:osc is 256KHz!\n", __func__);
            aw210xx_write_bits(dev_addr, AW210XX_REG_GCR, AW210XX_BIT_CLKFRQ_MASK, AW210XX_BIT_CLKFRQ_256KH);
            break;
        case CLK_FRQ_125K:
            // AWLOGD("%s:osc is 125KHz!\n", __func__);
            aw210xx_write_bits(dev_addr, AW210XX_REG_GCR, AW210XX_BIT_CLKFRQ_MASK, AW210XX_BIT_CLKFRQ_125KH);
            break;
        default:
            // AWLOGD("%s:this clk_pwm is unsupported!\n",
            //  __func__);
            return -AW210XX_CLK_MODE_UNSUPPORT;
    }
    return 0;
}

static void aw210xx_chipen_set(uint8_t dev_addr, bool flag)
{
    if (flag) {
        aw210xx_write_bits(dev_addr, AW210XX_REG_GCR, AW210XX_BIT_CHIPEN_MASK, AW210XX_BIT_CHIPEN_ENABLE);
        rt_thread_mdelay(1);
    } else {
        aw210xx_write_bits(dev_addr, AW210XX_REG_GCR, AW210XX_BIT_CHIPEN_MASK, AW210XX_BIT_CHIPEN_DISENA);
    }
}

static void aw210xx_sbmd_set(uint8_t dev_addr, bool flag)
{
    if (flag) {
        aw210xx_write_bits(dev_addr, AW210XX_REG_GCR2, AW210XX_BIT_SBMD_MASK, AW210XX_BIT_SBMD_ENABLE);

    } else {
        aw210xx_write_bits(dev_addr, AW210XX_REG_GCR2, AW210XX_BIT_SBMD_MASK, AW210XX_BIT_SBMD_DISENA);
    }
}

static void aw210xx_rgbmd_set(uint8_t dev_addr, bool flag)
{
    if (flag) {
        aw210xx_write_bits(dev_addr, AW210XX_REG_GCR2, AW210XX_BIT_RGBMD_MASK, AW210XX_BIT_RGBMD_ENABLE);

    } else {
        aw210xx_write_bits(dev_addr, AW210XX_REG_GCR2, AW210XX_BIT_RGBMD_MASK, AW210XX_BIT_RGBMD_DISENA);
    }
}

static int32_t aw210xx_br_pwm_set(uint8_t dev_addr, br_pwm_e br_pwm)
{

    switch (br_pwm) {
        case BR_RESOLUTION_8BIT:
            // AWLOGD("%s:br resolution select 8bit!\n", __func__);
            aw210xx_write_bits(dev_addr, AW210XX_REG_GCR, AW210XX_BIT_PWMRES_MASK, AW210XX_BIT_PWMRES_8BIT);
            break;
        case BR_RESOLUTION_9BIT:
            // AWLOGD("%s:br resolution select 9bit!\n", __func__);
            aw210xx_write_bits(dev_addr, AW210XX_REG_GCR, AW210XX_BIT_PWMRES_MASK, AW210XX_BIT_PWMRES_9BIT);
            break;
        case BR_RESOLUTION_12BIT:
            // AWLOGD("%s:br resolution select 12bit!\n", __func__);
            aw210xx_write_bits(dev_addr, AW210XX_REG_GCR, AW210XX_BIT_PWMRES_MASK, AW210XX_BIT_PWMRES_12BIT);
            break;
        case BR_RESOLUTION_9_AND_3_BIT:
            // AWLOGD("%s:br resolution select 9+3bit!\n", __func__);
            aw210xx_write_bits(dev_addr, AW210XX_REG_GCR, AW210XX_BIT_PWMRES_MASK, AW210XX_BIT_PWMRES_9_AND_3_BIT);
            break;
        default:
            // AWLOGD("%s:this br_pwm is unsupported!\n", __func__);
            return -AW210XX_CLK_MODE_UNSUPPORT;
    }
    return 0;
}

static void aw210xx_apse_set(uint8_t dev_addr, bool flag)
{
    if (flag) {
        aw210xx_write_bits(dev_addr, AW210XX_REG_GCR, AW210XX_BIT_APSE_MASK, AW210XX_BIT_APSE_ENABLE);
    } else {
        aw210xx_write_bits(dev_addr, AW210XX_REG_GCR, AW210XX_BIT_APSE_MASK, AW210XX_BIT_APSE_DISENA);
    }
}

static void aw210xx_uvlo_set(uint8_t dev_addr, bool flag)
{
    if (flag) {
        aw210xx_write_bits(dev_addr, AW210XX_REG_UVCR, AW210XX_BIT_UVPD_MASK, AW210XX_BIT_UVPD_DISENA);
        aw210xx_write_bits(dev_addr, AW210XX_REG_UVCR, AW210XX_BIT_UVDIS_MASK, AW210XX_BIT_UVDIS_DISENA);
    } else {
        aw210xx_write_bits(dev_addr, AW210XX_REG_UVCR, AW210XX_BIT_UVPD_MASK, AW210XX_BIT_UVPD_ENABLE);
        aw210xx_write_bits(dev_addr, AW210XX_REG_UVCR, AW210XX_BIT_UVDIS_MASK, AW210XX_BIT_UVDIS_ENABLE);
    }
}

static void aw210xx_led_init(uint8_t base_addr)
{
    /* chip enable */
    aw210xx_chipen_set(base_addr, true);
    /* sbmd enable */
    aw210xx_sbmd_set(base_addr, true);
    /* rgbmd enable */
    aw210xx_rgbmd_set(base_addr, true);
    /* clk_pwm selsect */
    aw210xx_clk_pwm_set(base_addr, CLK_FRQ_16M);
    /* br_pwm select */
    aw210xx_br_pwm_set(base_addr, BR_RESOLUTION_8BIT);
    /* global set */
    aw210xx_global_set(base_addr, AW210XX_GLOBAL_DEFAULT_SET);
    /* under voltage lock out */
    aw210xx_uvlo_set(base_addr, true);
    /* apse enable */
    aw210xx_apse_set(base_addr, true);
}

/*****************************************************
* aw210xx check chipid
*****************************************************/
static uint8_t aw210xx_read_chipid(uint8_t base_addr)
{
    uint8_t chipid = 0;

    chipid = led_soft_i2c_readreg(base_addr, AW210XX_REG_RESET);

    return chipid;
}

static int32_t aw210xx_init(uint8_t dev_addr)
{
    uint8_t chip_id;

    chip_id = aw210xx_read_chipid(dev_addr);
    if (chip_id != AW21009_CHIPID) {
        DBG_LOG("aw2016a chip id err: %02x!", chip_id);
        return -1;
    }

    aw210xx_led_init(dev_addr);

    /*light effect*/
    // aw210xx_effect_select(AW210XX_I2C_ADDR, AW21018_GROUP_BREATH_LEDS_ON);
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x20, 0x01);
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x58, 0x60);
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x80, 0x00);
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x81, 0x00);
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x8b, 0x07);
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x88, 0x00);
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x89, 0x00);
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x8a, 0x60);
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x86, 0x60);
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x87, 0x60);
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x45, 0x00);

    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x58, 0x60);  //电流控制
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x80, 0x03);  //patten config breath mode
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x8b, 0x41);  //工作模式
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x88, 0x60);  //RGB R
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x89, 0x60);  //RGB G
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x8a, 0x60);  //RGB B
    // led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x8a, 0x60);
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x46, 0x60);
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x47, 0x00);
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x48, 0x00);

    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x49, 0x00);
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x4a, 0x60);
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x4b, 0x60);

    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x86, 0x60);  //brightness/fade
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x87, 0x00);  //brightness/fade
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x82, 0x44);  //T0T1
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x83, 0x44);  //T2T3
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x84, 0x00);  //LOOP conf
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x85, 0x00);  //LOOP config
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x45, 0x00);  //refresh SL BR
    led_soft_i2c_writereg(AW210XX_I2C_ADDR, 0x81, 0x01);  //start patten

    return 0;
}

/* Exported functions --------------------------------------------------------*/

int hl_drv_aw21009_init(void)
{
    int ret;

    if (_init_flag != 0) {
        DBG_LOG("aw21009 is already inited!\n");
        return AW21009_FUNC_RET_ERR;
    }

    _p_i2c_bus = (struct rt_i2c_bus_device*)rt_device_find(HL_DRV_AW21009_I2C_NAME);
    if (_p_i2c_bus == NULL) {
        DBG_LOG("i2c dev not found!:%s", HL_DRV_AW21009_I2C_NAME);
        return AW21009_FUNC_RET_ERR;
    }

    ret = aw210xx_init(AW210XX_I2C_ADDR);
    if (ret == -1) {
        DBG_LOG("aw21009 init failed!");
    }

#if HL_GET_DEVICE_TYPE() == 0
    ///Rx
    ret = aw210xx_init(HL_DRV_AW21009_OTHER_DEV_ADDR);
    if (ret == -1) {
        DBG_LOG("aw21009 init failed!");
    }
#endif

    DBG_LOG("\naw21009 init success!\n");

    _init_flag = 1;

    return AW21009_FUNC_RET_OK;
}

int hl_drv_aw21009_deinit(void)
{
    int ret;
    if (_init_flag != 1) {
        DBG_LOG("aw21009 is not inited!\n");
        return AW21009_FUNC_RET_ERR;
    }

    _init_flag = 0;

    return AW21009_FUNC_RET_OK;
}

int hl_drv_aw21009_ctrl(uint8_t led_num, uint8_t op, void* arg, int32_t arg_size)
{
    int ret;
    if (_init_flag != 1) {
        DBG_LOG("aw21009 is not inited!\n");
        return AW21009_FUNC_RET_ERR;
    }

    return AW21009_FUNC_RET_OK;
}

/*
 * EOF
 */