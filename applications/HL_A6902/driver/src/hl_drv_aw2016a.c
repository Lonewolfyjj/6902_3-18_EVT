/**
 * @file hl_drv_aw2016a.c
 * @author lilin (lin.li@hollyland-tech.com)
 * @brief LED驱动源文件
 * @version 1.0
 * @date 2022-09-06
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-06     <td>v1.0     <td>lilin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/

#include "hl_drv_aw2016a.h"

#define DBG_SECTION_NAME "aw2016"
#define DBG_LEVEL DBG_INFO
#include "rtdbg.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/

#if HL_IS_TX_DEVICE() == 1
#define AW2016_IIC_BUS_0_NAME "i2c0"
#define AW2016_IIC_BUS_1_NAME "i2c0"
#else
#define AW2016_IIC_BUS_0_NAME "i2c1"
#define AW2016_IIC_BUS_1_NAME "i2c1"
#endif

/* chip info */
#define AW2016A_IIC_DEV_ADDR 0x64

/* AW2016A reg base addr Macro */
#define AW2016A_REG_RSTR 0x00
#define AW2016A_REG_GCR1 0x01
#define AW2016A_REG_ISR 0x02
#define AW2016A_REG_PATST 0x03
#define AW2016A_REG_GCR2 0x04
#define AW2016A_REG_LCTR 0x30
#define AW2016A_REG_LCFG1 0x31
#define AW2016A_REG_LCFG2 0x32
#define AW2016A_REG_LCFG3 0x33
#define AW2016A_REG_PWM1 0x34
#define AW2016A_REG_PWM2 0x35
#define AW2016A_REG_PWM3 0x36
#define AW2016A_REG_LED1_T1_T2 0x37
#define AW2016A_REG_LED1_T3_T4 0x38
#define AW2016A_REG_LED1_T0_REPEAT 0x39
#define AW2016A_REG_LED2_T1_T2 0x3A
#define AW2016A_REG_LED2_T3_T4 0x3B
#define AW2016A_REG_LED2_T0_REPEAT 0x3C
#define AW2016A_REG_LED3_T1_T2 0x3D
#define AW2016A_REG_LED3_T3_T4 0x3E
#define AW2016A_REG_LED3_T0_REPEAT 0x3F

/* variables -----------------------------------------------------------------*/

static struct rt_i2c_bus_device* _p_i2c_bus_0 = NULL;
static struct rt_i2c_bus_device* _p_i2c_bus_1 = NULL;
static uint8_t                   _init_flag   = 0;

/* Private function(only *.c)  -----------------------------------------------*/

static inline int aw_read(struct rt_i2c_bus_device* p_i2c_bus, uint8_t PointReg, uint8_t* pData)
{
    struct rt_i2c_msg msgs[2] = { 0 };

    msgs[0].addr  = AW2016A_IIC_DEV_ADDR;
    msgs[0].flags = RT_I2C_WR;  // | RT_I2C_NO_STOP;
    msgs[0].buf   = &PointReg;
    msgs[0].len   = 1;

    msgs[1].addr  = AW2016A_IIC_DEV_ADDR;
    msgs[1].flags = RT_I2C_RD | RT_I2C_NO_READ_ACK;
    msgs[1].buf   = pData;
    msgs[1].len   = 1;

    if (rt_i2c_transfer(p_i2c_bus, msgs, 2) == 2) {
        return AW2016A_FUNC_RET_OK;
    } else {
        LOG_E("i2c read err!");
        return AW2016A_FUNC_RET_ERR;
    }
}

static inline int aw_write(struct rt_i2c_bus_device* p_i2c_bus, uint8_t PointReg, uint8_t* pData)
{
    struct rt_i2c_msg msgs[1] = { 0 };
    uint8_t           buf[2];

    buf[0] = PointReg;
    buf[1] = *pData;

    msgs[0].addr  = AW2016A_IIC_DEV_ADDR;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = buf;
    msgs[0].len   = 2;

    if (rt_i2c_transfer(p_i2c_bus, msgs, 1) == 1) {
        return AW2016A_FUNC_RET_OK;
    } else {
        LOG_E("i2c write err!");
        return AW2016A_FUNC_RET_ERR;
    }
}

static inline int aw_read_nbyte(struct rt_i2c_bus_device* p_i2c_bus, uint8_t point_reg, uint8_t* r_pdata, uint8_t len)
{
    struct rt_i2c_msg msgs[2] = { 0 };

    msgs[0].addr  = AW2016A_IIC_DEV_ADDR;
    msgs[0].flags = RT_I2C_WR;  //| RT_I2C_NO_STOP;
    msgs[0].buf   = &point_reg;
    msgs[0].len   = 1;

    msgs[1].addr  = AW2016A_IIC_DEV_ADDR;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = r_pdata;
    msgs[1].len   = len;

    if (rt_i2c_transfer(p_i2c_bus, msgs, 2) == 2) {
        return AW2016A_FUNC_RET_OK;
    } else {
        return AW2016A_FUNC_RET_ERR;
    }
}

static int get_chip_id(struct rt_i2c_bus_device* p_i2c_bus, uint8_t* p_chip_id)
{
    int ret;

    ret = aw_read(p_i2c_bus, AW2016A_REG_RSTR, p_chip_id);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int check_chip_status(struct rt_i2c_bus_device* p_i2c_bus, hl_drv_aw2016a_chip_status_e* p_param)
{
    int     ret;
    uint8_t reg_val;

    ret = aw_read(p_i2c_bus, AW2016A_REG_ISR, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    *p_param = 0;

    if (reg_val & HL_DRV_AW2016A_OTPIS) {
        *p_param |= HL_DRV_AW2016A_OTPIS;
    }

    if (reg_val & HL_DRV_AW2016A_UVLOIS) {
        *p_param |= HL_DRV_AW2016A_UVLOIS;
    }

    if (reg_val & HL_DRV_AW2016A_PUIS) {
        *p_param |= HL_DRV_AW2016A_PUIS;
    }

    return AW2016A_FUNC_RET_OK;
}

/**
 * 
 * @brief reset internal logic and register 
 * @param [in] p_i2c_bus 
 * @return int 
 * @date 2022-09-09
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-09      <td>lilin     <td>新建
 * </table>
 */
static int soft_reset(struct rt_i2c_bus_device* p_i2c_bus)
{
    int     ret;
    uint8_t data = 0x55;

    ret = aw_write(p_i2c_bus, AW2016A_REG_RSTR, &data);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int set_charge_indicator(struct rt_i2c_bus_device* p_i2c_bus, uint8_t* disable)
{
    int     ret;
    uint8_t reg_val;

    ret = aw_read(p_i2c_bus, AW2016A_REG_GCR1, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    reg_val = (reg_val & (~(1 << 1))) | (*disable << 1);

    ret = aw_write(p_i2c_bus, AW2016A_REG_GCR1, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int set_work_mode(struct rt_i2c_bus_device* p_i2c_bus, uint8_t* p_work_mode)
{
    int     ret;
    uint8_t reg_val;

    ret = aw_read(p_i2c_bus, AW2016A_REG_GCR1, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    reg_val = (reg_val & (~(1 << 0))) | *p_work_mode;

    ret = aw_write(p_i2c_bus, AW2016A_REG_GCR1, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int set_global_max_output_current(struct rt_i2c_bus_device* p_i2c_bus, uint8_t* p_max_current)
{
    int     ret;
    uint8_t reg_val;

    ret = aw_read(p_i2c_bus, AW2016A_REG_GCR2, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    reg_val = (reg_val & (~(0x03 << 0))) | *p_max_current;

    ret = aw_write(p_i2c_bus, AW2016A_REG_GCR2, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int open_led_channel(struct rt_i2c_bus_device* p_i2c_bus, uint8_t* p_led_chan)
{
    int     ret;
    uint8_t reg_val;

    ret = aw_read(p_i2c_bus, AW2016A_REG_LCTR, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    reg_val |= (*p_led_chan);

    ret = aw_write(p_i2c_bus, AW2016A_REG_LCTR, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int close_led_channel(struct rt_i2c_bus_device* p_i2c_bus, uint8_t* p_led_chan)
{
    int     ret;
    uint8_t reg_val;

    ret = aw_read(p_i2c_bus, AW2016A_REG_LCTR, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    reg_val &= (~(*p_led_chan));

    ret = aw_write(p_i2c_bus, AW2016A_REG_LCTR, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int set_pattern_mode(struct rt_i2c_bus_device* p_i2c_bus, uint8_t* p_led_chan)
{
    int     ret;
    uint8_t reg_val;

    if (*p_led_chan & HL_DRV_AW2016A_LED_CHANNEL1) {
        ret = aw_read(p_i2c_bus, AW2016A_REG_LCFG1, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        reg_val |= (1 << 4);

        ret = aw_write(p_i2c_bus, AW2016A_REG_LCFG1, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    if (*p_led_chan & HL_DRV_AW2016A_LED_CHANNEL2) {
        ret = aw_read(p_i2c_bus, AW2016A_REG_LCFG2, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        reg_val |= (1 << 4);

        ret = aw_write(p_i2c_bus, AW2016A_REG_LCFG2, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    if (*p_led_chan & HL_DRV_AW2016A_LED_CHANNEL3) {
        ret = aw_read(p_i2c_bus, AW2016A_REG_LCFG3, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        reg_val |= (1 << 4);

        ret = aw_write(p_i2c_bus, AW2016A_REG_LCFG3, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    return AW2016A_FUNC_RET_OK;
}

static int set_pattern_mode_param(struct rt_i2c_bus_device* p_i2c_bus, hl_drv_aw2016a_breath_param_st* p_param)
{
    int     ret;
    uint8_t LEDx_T0;  //t1 t2
    uint8_t LEDx_T1;  //t3 t4
    uint8_t LEDx_T2;  //t0 repeat

    LEDx_T0 = ((p_param->t1 << 4) | p_param->t2);
    LEDx_T1 = ((p_param->t3 << 4) | p_param->t4);
    LEDx_T2 = ((p_param->t0 << 4) | p_param->repeat);

    if (p_param->led_chan & HL_DRV_AW2016A_LED_CHANNEL1) {
        ret = aw_write(p_i2c_bus, AW2016A_REG_LED1_T1_T2, &LEDx_T0);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        ret = aw_write(p_i2c_bus, AW2016A_REG_LED1_T3_T4, &LEDx_T1);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        ret = aw_write(p_i2c_bus, AW2016A_REG_LED1_T0_REPEAT, &LEDx_T2);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    if (p_param->led_chan & HL_DRV_AW2016A_LED_CHANNEL2) {
        ret = aw_write(p_i2c_bus, AW2016A_REG_LED2_T1_T2, &LEDx_T0);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        ret = aw_write(p_i2c_bus, AW2016A_REG_LED2_T3_T4, &LEDx_T1);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        ret = aw_write(p_i2c_bus, AW2016A_REG_LED2_T0_REPEAT, &LEDx_T2);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    if (p_param->led_chan & HL_DRV_AW2016A_LED_CHANNEL3) {
        ret = aw_write(p_i2c_bus, AW2016A_REG_LED3_T1_T2, &LEDx_T0);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        ret = aw_write(p_i2c_bus, AW2016A_REG_LED3_T3_T4, &LEDx_T1);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        ret = aw_write(p_i2c_bus, AW2016A_REG_LED3_T0_REPEAT, &LEDx_T2);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    return AW2016A_FUNC_RET_OK;
}

static int set_manual_mode(struct rt_i2c_bus_device* p_i2c_bus, uint8_t* p_led_chan)
{
    int     ret;
    uint8_t reg_val;

    if (*p_led_chan & HL_DRV_AW2016A_LED_CHANNEL1) {
        ret = aw_read(p_i2c_bus, AW2016A_REG_LCFG1, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        reg_val &= (~(1 << 4));

        ret = aw_write(p_i2c_bus, AW2016A_REG_LCFG1, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    if (*p_led_chan & HL_DRV_AW2016A_LED_CHANNEL2) {
        ret = aw_read(p_i2c_bus, AW2016A_REG_LCFG2, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        reg_val &= (~(1 << 4));

        ret = aw_write(p_i2c_bus, AW2016A_REG_LCFG2, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    if (*p_led_chan & HL_DRV_AW2016A_LED_CHANNEL3) {
        ret = aw_read(p_i2c_bus, AW2016A_REG_LCFG3, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        reg_val &= (~(1 << 4));

        ret = aw_write(p_i2c_bus, AW2016A_REG_LCFG3, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    return AW2016A_FUNC_RET_OK;
}

static int set_output_current(struct rt_i2c_bus_device* p_i2c_bus, hl_drv_aw2016a_output_current_st* p_param)
{
    int     ret;
    uint8_t reg_val;

    if (p_param->current > 15) {
        p_param->current = 15;
    }

    if (p_param->led_chan & HL_DRV_AW2016A_LED_CHANNEL1) {
        ret = aw_read(p_i2c_bus, AW2016A_REG_LCFG1, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        reg_val = (reg_val & (~(0x0F << 0))) | p_param->current;

        ret = aw_write(p_i2c_bus, AW2016A_REG_LCFG1, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    if (p_param->led_chan & HL_DRV_AW2016A_LED_CHANNEL2) {
        ret = aw_read(p_i2c_bus, AW2016A_REG_LCFG2, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        reg_val = (reg_val & (~(0x0F << 0))) | p_param->current;

        ret = aw_write(p_i2c_bus, AW2016A_REG_LCFG2, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    if (p_param->led_chan & HL_DRV_AW2016A_LED_CHANNEL3) {
        ret = aw_read(p_i2c_bus, AW2016A_REG_LCFG3, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        reg_val = (reg_val & (~(0x0F << 0))) | p_param->current;

        ret = aw_write(p_i2c_bus, AW2016A_REG_LCFG3, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    return AW2016A_FUNC_RET_OK;
}

static int set_pwm_level(struct rt_i2c_bus_device* p_i2c_bus, hl_drv_aw2016a_pwm_level_st* p_param)
{
    int ret;

    if (p_param->led_chan & HL_DRV_AW2016A_LED_CHANNEL1) {
        ret = aw_write(p_i2c_bus, AW2016A_REG_PWM1, &(p_param->pwm_level));
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    if (p_param->led_chan & HL_DRV_AW2016A_LED_CHANNEL2) {
        ret = aw_write(p_i2c_bus, AW2016A_REG_PWM2, &(p_param->pwm_level));
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    if (p_param->led_chan & HL_DRV_AW2016A_LED_CHANNEL3) {
        ret = aw_write(p_i2c_bus, AW2016A_REG_PWM3, &(p_param->pwm_level));
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    return AW2016A_FUNC_RET_OK;
}

static int dump_register_value(struct rt_i2c_bus_device* p_i2c_bus)
{
#if 1
    int     ret;
    uint8_t reg_val;

    ret = aw_read(p_i2c_bus, AW2016A_REG_RSTR, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_RSTR, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_GCR1, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_GCR1, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_ISR, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_ISR, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_PATST, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_PATST, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_GCR2, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_GCR2, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_LCTR, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_LCTR, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_LCFG1, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_LCFG1, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_LCFG2, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_LCFG2, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_LCFG3, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_LCFG3, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_PWM1, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_PWM1, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_PWM2, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_PWM2, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_PWM3, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_PWM3, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_LED1_T1_T2, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_LED1_T1_T2, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_LED1_T3_T4, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_LED1_T3_T4, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_LED1_T0_REPEAT, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_LED1_T0_REPEAT, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_LED2_T1_T2, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_LED2_T1_T2, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_LED2_T3_T4, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_LED2_T3_T4, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_LED2_T0_REPEAT, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_LED2_T0_REPEAT, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_LED3_T1_T2, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_LED3_T1_T2, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_LED3_T3_T4, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_LED3_T3_T4, reg_val);

    ret = aw_read(p_i2c_bus, AW2016A_REG_LED3_T0_REPEAT, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_I("reg:%02x,value:%02x", AW2016A_REG_LED3_T0_REPEAT, reg_val);
#endif
    return AW2016A_FUNC_RET_OK;
}

static int set_led_light_effect(struct rt_i2c_bus_device* p_i2c_bus, hl_drv_aw2016a_light_st* p_param)
{
    int                              ret;
    hl_drv_aw2016a_output_current_st output_current;
    hl_drv_aw2016a_pwm_level_st      pwm_level;

    output_current.led_chan = HL_DRV_AW2016A_LED_CHANNEL1;
    output_current.current  = p_param->r;

    ret = set_output_current(p_i2c_bus, &output_current);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    output_current.led_chan = HL_DRV_AW2016A_LED_CHANNEL2;
    output_current.current  = p_param->g;

    ret = set_output_current(p_i2c_bus, &output_current);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    output_current.led_chan = HL_DRV_AW2016A_LED_CHANNEL3;
    output_current.current  = p_param->b;

    ret = set_output_current(p_i2c_bus, &output_current);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pwm_level.led_chan  = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    pwm_level.pwm_level = p_param->brightness;

    ret = set_pwm_level(p_i2c_bus, &pwm_level);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int set_sync_control_mode(struct rt_i2c_bus_device* p_i2c_bus, bool* p_param)
{
    int     ret;
    uint8_t reg_val;

    ret = aw_read(p_i2c_bus, AW2016A_REG_LCFG1, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    reg_val &= (~(1 << 7));
    reg_val |= ((*p_param) << 7);

    ret = aw_write(p_i2c_bus, AW2016A_REG_LCFG1, &reg_val);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int set_fade_out_enable(struct rt_i2c_bus_device* p_i2c_bus, hl_drv_aw2016a_set_fade_in_out_st* p_param)
{
    int     ret;
    uint8_t reg_val;

    if (p_param->led_chan & HL_DRV_AW2016A_LED_CHANNEL1) {
        ret = aw_read(p_i2c_bus, AW2016A_REG_LCFG1, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        reg_val &= (~(1 << 6));
        reg_val |= ((p_param->enable) << 6);

        ret = aw_write(p_i2c_bus, AW2016A_REG_LCFG1, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    if (p_param->led_chan & HL_DRV_AW2016A_LED_CHANNEL2) {
        ret = aw_read(p_i2c_bus, AW2016A_REG_LCFG2, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        reg_val &= (~(1 << 6));
        reg_val |= ((p_param->enable) << 6);

        ret = aw_write(p_i2c_bus, AW2016A_REG_LCFG2, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    if (p_param->led_chan & HL_DRV_AW2016A_LED_CHANNEL3) {
        ret = aw_read(p_i2c_bus, AW2016A_REG_LCFG3, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        reg_val &= (~(1 << 6));
        reg_val |= ((p_param->enable) << 6);

        ret = aw_write(p_i2c_bus, AW2016A_REG_LCFG3, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    return AW2016A_FUNC_RET_OK;
}

static int set_fade_in_enable(struct rt_i2c_bus_device* p_i2c_bus, hl_drv_aw2016a_set_fade_in_out_st* p_param)
{
    int     ret;
    uint8_t reg_val;

    if (p_param->led_chan & HL_DRV_AW2016A_LED_CHANNEL1) {
        ret = aw_read(p_i2c_bus, AW2016A_REG_LCFG1, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        reg_val &= (~(1 << 5));
        reg_val |= ((p_param->enable) << 5);

        ret = aw_write(p_i2c_bus, AW2016A_REG_LCFG1, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    if (p_param->led_chan & HL_DRV_AW2016A_LED_CHANNEL2) {
        ret = aw_read(p_i2c_bus, AW2016A_REG_LCFG2, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        reg_val &= (~(1 << 5));
        reg_val |= ((p_param->enable) << 5);

        ret = aw_write(p_i2c_bus, AW2016A_REG_LCFG2, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    if (p_param->led_chan & HL_DRV_AW2016A_LED_CHANNEL3) {
        ret = aw_read(p_i2c_bus, AW2016A_REG_LCFG3, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }

        reg_val &= (~(1 << 5));
        reg_val |= ((p_param->enable) << 5);

        ret = aw_write(p_i2c_bus, AW2016A_REG_LCFG3, &reg_val);
        if (ret == AW2016A_FUNC_RET_ERR) {
            return AW2016A_FUNC_RET_ERR;
        }
    }

    return AW2016A_FUNC_RET_OK;
}

static int _breath_mode_slow(struct rt_i2c_bus_device* p_i2c_bus)
{
    int                            ret;
    uint8_t                        led_chan;
    hl_drv_aw2016a_breath_param_st pattern_param;

    led_chan = 0xFF;

    ret = close_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;

    ret = set_manual_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = led_chan;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 0;
    pattern_param.t1       = 9;
    pattern_param.t2       = 3;
    pattern_param.t3       = 9;
    pattern_param.t4       = 0;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = set_pattern_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = open_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int _breath_mode_fast(struct rt_i2c_bus_device* p_i2c_bus)
{
    int                            ret;
    uint8_t                        led_chan;
    hl_drv_aw2016a_breath_param_st pattern_param;

    led_chan = 0xFF;

    ret = close_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;

    ret = set_manual_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = led_chan;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 0;
    pattern_param.t1       = 5;
    pattern_param.t2       = 2;
    pattern_param.t3       = 5;
    pattern_param.t4       = 0;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = set_pattern_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = open_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int _breath_mode_keep(struct rt_i2c_bus_device* p_i2c_bus)
{
    int                            ret;
    uint8_t                        led_chan;
    hl_drv_aw2016a_breath_param_st pattern_param;

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;

    ret = set_manual_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int _breath_mode_rgb_slow(struct rt_i2c_bus_device* p_i2c_bus)
{
    int                            ret;
    uint8_t                        led_chan;
    hl_drv_aw2016a_breath_param_st pattern_param;

    led_chan = 0xFF;

    ret = close_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;

    ret = set_manual_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL3;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 0;
    pattern_param.t1       = 9;
    pattern_param.t2       = 3;
    pattern_param.t3       = 9;
    pattern_param.t4       = 8;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL2;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 6;
    pattern_param.t1       = 9;
    pattern_param.t2       = 3;
    pattern_param.t3       = 9;
    pattern_param.t4       = 8;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL1;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 8;
    pattern_param.t1       = 9;
    pattern_param.t2       = 3;
    pattern_param.t3       = 9;
    pattern_param.t4       = 8;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = set_pattern_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = open_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int _breath_mode_rgb_fast(struct rt_i2c_bus_device* p_i2c_bus)
{
    int                            ret;
    uint8_t                        led_chan;
    hl_drv_aw2016a_breath_param_st pattern_param;

    led_chan = 0xFF;

    ret = close_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;

    ret = set_manual_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL3;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 0;
    pattern_param.t1       = 5;
    pattern_param.t2       = 2;
    pattern_param.t3       = 5;
    pattern_param.t4       = 6;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL2;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 4;
    pattern_param.t1       = 5;
    pattern_param.t2       = 2;
    pattern_param.t3       = 5;
    pattern_param.t4       = 6;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL1;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 6;
    pattern_param.t1       = 5;
    pattern_param.t2       = 2;
    pattern_param.t3       = 5;
    pattern_param.t4       = 6;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = set_pattern_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = open_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int _breath_mode_rg_slow(struct rt_i2c_bus_device* p_i2c_bus)
{
    int                            ret;
    uint8_t                        led_chan;
    hl_drv_aw2016a_breath_param_st pattern_param;

    led_chan = 0xFF;

    ret = close_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;

    ret = set_manual_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL3;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 0;
    pattern_param.t1       = 9;
    pattern_param.t2       = 3;
    pattern_param.t3       = 9;
    pattern_param.t4       = 6;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL2;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 6;
    pattern_param.t1       = 9;
    pattern_param.t2       = 3;
    pattern_param.t3       = 9;
    pattern_param.t4       = 6;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;

    ret = set_pattern_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = open_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int _breath_mode_rg_fast(struct rt_i2c_bus_device* p_i2c_bus)
{
    int                            ret;
    uint8_t                        led_chan;
    hl_drv_aw2016a_breath_param_st pattern_param;

    led_chan = 0xFF;

    ret = close_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;

    ret = set_manual_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL3;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 0;
    pattern_param.t1       = 5;
    pattern_param.t2       = 2;
    pattern_param.t3       = 5;
    pattern_param.t4       = 4;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL2;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 4;
    pattern_param.t1       = 5;
    pattern_param.t2       = 2;
    pattern_param.t3       = 5;
    pattern_param.t4       = 4;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;

    ret = set_pattern_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = open_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int _breath_mode_rb_slow(struct rt_i2c_bus_device* p_i2c_bus)
{
    int                            ret;
    uint8_t                        led_chan;
    hl_drv_aw2016a_breath_param_st pattern_param;

    led_chan = 0xFF;

    ret = close_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;

    ret = set_manual_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL3;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 0;
    pattern_param.t1       = 9;
    pattern_param.t2       = 3;
    pattern_param.t3       = 9;
    pattern_param.t4       = 6;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL1;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 6;
    pattern_param.t1       = 9;
    pattern_param.t2       = 3;
    pattern_param.t3       = 9;
    pattern_param.t4       = 6;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL3;

    ret = set_pattern_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = open_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int _breath_mode_rb_fast(struct rt_i2c_bus_device* p_i2c_bus)
{
    int                            ret;
    uint8_t                        led_chan;
    hl_drv_aw2016a_breath_param_st pattern_param;

    led_chan = 0xFF;

    ret = close_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;

    ret = set_manual_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL3;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 0;
    pattern_param.t1       = 5;
    pattern_param.t2       = 2;
    pattern_param.t3       = 5;
    pattern_param.t4       = 4;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL1;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 4;
    pattern_param.t1       = 5;
    pattern_param.t2       = 2;
    pattern_param.t3       = 5;
    pattern_param.t4       = 4;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL3;

    ret = set_pattern_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = open_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int _breath_mode_gb_slow(struct rt_i2c_bus_device* p_i2c_bus)
{
    int                            ret;
    uint8_t                        led_chan;
    hl_drv_aw2016a_breath_param_st pattern_param;

    led_chan = 0xFF;

    ret = close_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;

    ret = set_manual_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL2;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 0;
    pattern_param.t1       = 9;
    pattern_param.t2       = 3;
    pattern_param.t3       = 9;
    pattern_param.t4       = 6;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL1;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 6;
    pattern_param.t1       = 9;
    pattern_param.t2       = 3;
    pattern_param.t3       = 9;
    pattern_param.t4       = 6;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2;

    ret = set_pattern_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = open_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int _breath_mode_gb_fast(struct rt_i2c_bus_device* p_i2c_bus)
{
    int                            ret;
    uint8_t                        led_chan;
    hl_drv_aw2016a_breath_param_st pattern_param;

    led_chan = 0xFF;

    ret = close_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;

    ret = set_manual_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL2;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 0;
    pattern_param.t1       = 5;
    pattern_param.t2       = 2;
    pattern_param.t3       = 5;
    pattern_param.t4       = 4;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL1;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 4;
    pattern_param.t1       = 5;
    pattern_param.t2       = 2;
    pattern_param.t3       = 5;
    pattern_param.t4       = 4;

    ret = set_pattern_mode_param(p_i2c_bus, &pattern_param);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2;

    ret = set_pattern_mode(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = open_led_channel(p_i2c_bus, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int _set_color_red(struct rt_i2c_bus_device* p_i2c_bus)
{
    int                              ret;
    hl_drv_aw2016a_output_current_st current_st;

    current_st.led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2;
    current_st.current  = 0;

    ret = set_output_current(p_i2c_bus, &current_st);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    current_st.led_chan = HL_DRV_AW2016A_LED_CHANNEL3;
    current_st.current  = 15;

    ret = set_output_current(p_i2c_bus, &current_st);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int _set_color_green(struct rt_i2c_bus_device* p_i2c_bus)
{
    int                              ret;
    hl_drv_aw2016a_output_current_st current_st;

    current_st.led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL3;
    current_st.current  = 0;

    ret = set_output_current(p_i2c_bus, &current_st);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    current_st.led_chan = HL_DRV_AW2016A_LED_CHANNEL2;
    current_st.current  = 15;

    ret = set_output_current(p_i2c_bus, &current_st);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int _set_color_blue(struct rt_i2c_bus_device* p_i2c_bus)
{
    int                              ret;
    hl_drv_aw2016a_output_current_st current_st;

    current_st.led_chan = HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    current_st.current  = 0;

    ret = set_output_current(p_i2c_bus, &current_st);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    current_st.led_chan = HL_DRV_AW2016A_LED_CHANNEL1;
    current_st.current  = 15;

    ret = set_output_current(p_i2c_bus, &current_st);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int _set_color_white(struct rt_i2c_bus_device* p_i2c_bus)
{
    int                              ret;
    hl_drv_aw2016a_output_current_st current_st;

    current_st.led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    current_st.current  = 15;

    ret = set_output_current(p_i2c_bus, &current_st);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int _set_color_orange(struct rt_i2c_bus_device* p_i2c_bus)
{
    int                              ret;
    hl_drv_aw2016a_output_current_st current_st;

    current_st.led_chan = HL_DRV_AW2016A_LED_CHANNEL3;
    current_st.current  = 12;

    ret = set_output_current(p_i2c_bus, &current_st);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    current_st.led_chan = HL_DRV_AW2016A_LED_CHANNEL2;
    current_st.current  = 1;

    ret = set_output_current(p_i2c_bus, &current_st);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    current_st.led_chan = HL_DRV_AW2016A_LED_CHANNEL1;
    current_st.current  = 0;

    ret = set_output_current(p_i2c_bus, &current_st);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int _set_color_pink(struct rt_i2c_bus_device* p_i2c_bus)
{
    int                              ret;
    hl_drv_aw2016a_output_current_st current_st;

    current_st.led_chan = HL_DRV_AW2016A_LED_CHANNEL3;
    current_st.current  = 15;

    ret = set_output_current(p_i2c_bus, &current_st);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    current_st.led_chan = HL_DRV_AW2016A_LED_CHANNEL2;
    current_st.current  = 0;

    ret = set_output_current(p_i2c_bus, &current_st);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    current_st.led_chan = HL_DRV_AW2016A_LED_CHANNEL1;
    current_st.current  = 6;

    ret = set_output_current(p_i2c_bus, &current_st);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int _set_color_black(struct rt_i2c_bus_device* p_i2c_bus)
{
    int                              ret;
    hl_drv_aw2016a_output_current_st current_st;

    current_st.led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    current_st.current  = 0;

    ret = set_output_current(p_i2c_bus, &current_st);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return AW2016A_FUNC_RET_OK;
}

static int _user_func_led_ctrl(struct rt_i2c_bus_device* p_i2c_bus, hl_drv_aw2016a_led_ctrl_st* p_param)
{
    int ret;

    switch (p_param->breath_mode) {
        case HL_DRV_AW2016A_BREATH_MODE_SKIP: {

        } break;
        case HL_DRV_AW2016A_BREATH_MODE_SLOW: {
            ret = _breath_mode_slow(p_i2c_bus);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_BREATH_MODE_FAST: {
            ret = _breath_mode_fast(p_i2c_bus);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_BREATH_MODE_KEEP: {
            ret = _breath_mode_keep(p_i2c_bus);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_BREATH_MODE_RGB_SLOW: {
            ret = _breath_mode_rgb_slow(p_i2c_bus);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_BREATH_MODE_RGB_FAST: {
            ret = _breath_mode_rgb_fast(p_i2c_bus);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_BREATH_MODE_RG_SLOW: {
            ret = _breath_mode_rg_slow(p_i2c_bus);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_BREATH_MODE_RG_FAST: {
            ret = _breath_mode_rg_fast(p_i2c_bus);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_BREATH_MODE_RB_SLOW: {
            ret = _breath_mode_rb_slow(p_i2c_bus);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_BREATH_MODE_RB_FAST: {
            ret = _breath_mode_rb_fast(p_i2c_bus);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_BREATH_MODE_GB_SLOW: {
            ret = _breath_mode_gb_slow(p_i2c_bus);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_BREATH_MODE_GB_FAST: {
            ret = _breath_mode_gb_fast(p_i2c_bus);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        default:
            break;
    }

    switch (p_param->color) {
        case HL_DRV_AW2016A_COLOR_SKIP: {

        } break;
        case HL_DRV_AW2016A_COLOR_RED: {
            ret = _set_color_red(p_i2c_bus);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_COLOR_GREEN: {
            ret = _set_color_green(p_i2c_bus);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_COLOR_BLUE: {
            ret = _set_color_blue(p_i2c_bus);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_COLOR_WHITE: {
            ret = _set_color_white(p_i2c_bus);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_COLOR_ORANGE: {
            ret = _set_color_orange(p_i2c_bus);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_COLOR_PINK: {
            ret = _set_color_pink(p_i2c_bus);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_COLOR_BLACK: {
            ret = _set_color_black(p_i2c_bus);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        default:
            break;
    }

    return AW2016A_FUNC_RET_OK;
}

/* Exported functions --------------------------------------------------------*/

int hl_drv_aw2016a_init(void)
{
    int                         ret;
    uint8_t                     chip_id;
    uint8_t                     flag;
    uint8_t                     work_mode;
    uint8_t                     current;
    uint8_t                     led_chan;
    hl_drv_aw2016a_pwm_level_st pwm_level;

    if (_init_flag == 1) {
        LOG_W("LED is already inited!");
        return AW2016A_FUNC_RET_ERR;
    }

    _p_i2c_bus_0 = (struct rt_i2c_bus_device*)rt_device_find(AW2016_IIC_BUS_0_NAME);
    if (_p_i2c_bus_0 == NULL) {
        LOG_E("i2c dev not found!:%s", AW2016_IIC_BUS_0_NAME);
        return AW2016A_FUNC_RET_ERR;
    }

    _p_i2c_bus_1 = (struct rt_i2c_bus_device*)rt_device_find(AW2016_IIC_BUS_1_NAME);
    if (_p_i2c_bus_1 == NULL) {
        LOG_E("i2c dev not found!:%s", AW2016_IIC_BUS_1_NAME);
        return AW2016A_FUNC_RET_ERR;
    }

    ret = get_chip_id(_p_i2c_bus_0, &chip_id);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    if (chip_id != AW2016A_CHIP_ID) {
        LOG_E("aw2016a chip0 id err: %02x!", chip_id);
        return AW2016A_FUNC_RET_ERR;
    }

    ret = get_chip_id(_p_i2c_bus_1, &chip_id);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    if (chip_id != AW2016A_CHIP_ID) {
        LOG_E("aw2016a chip1 id err: %02x!", chip_id);
        return AW2016A_FUNC_RET_ERR;
    }

    flag = 1;

    ret = set_charge_indicator(_p_i2c_bus_0, &flag);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = set_charge_indicator(_p_i2c_bus_1, &flag);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    work_mode = HL_DRV_AW2016A_ACTIVE_MODE;

    ret = set_work_mode(_p_i2c_bus_0, &work_mode);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = set_work_mode(_p_i2c_bus_1, &work_mode);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    current = HL_DRV_AW2016A_IMAX_15MA;

    ret = set_global_max_output_current(_p_i2c_bus_0, &current);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = set_global_max_output_current(_p_i2c_bus_1, &current);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;

    ret = open_led_channel(_p_i2c_bus_0, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = open_led_channel(_p_i2c_bus_1, &led_chan);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pwm_level.led_chan  = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    pwm_level.pwm_level = 30;

    ret = set_pwm_level(_p_i2c_bus_0, &pwm_level);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = set_pwm_level(_p_i2c_bus_1, &pwm_level);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_D("LED init success!");

    _init_flag = 1;

    return AW2016A_FUNC_RET_OK;
}

int hl_drv_aw2016a_deinit(void)
{
    int     ret;
    uint8_t work_mode;

    if (_init_flag == 0) {
        LOG_W("LED is not inited!");
        return AW2016A_FUNC_RET_ERR;
    }

    work_mode = HL_DRV_AW2016A_STANDBY_MODE;

    ret = set_work_mode(_p_i2c_bus_0, &work_mode);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    ret = set_work_mode(_p_i2c_bus_1, &work_mode);
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    LOG_D("LED deinit success!");

    _init_flag = 0;

    return AW2016A_FUNC_RET_OK;
}

int hl_drv_aw2016a_ctrl(hl_drv_aw2016a_led_num_e led_num, hl_drv_aw2016a_op_t op, void* arg, int32_t arg_size)
{
    int                       ret;
    struct rt_i2c_bus_device* p_i2c_bus;

    if (_init_flag == 0) {
        LOG_E("LED is not inited!");
        return AW2016A_FUNC_RET_ERR;
    }

    if (led_num == HL_DRV_AW2016A_LED0) {
        p_i2c_bus = _p_i2c_bus_0;
    } else if (led_num == HL_DRV_AW2016A_LED1) {
        p_i2c_bus = _p_i2c_bus_1;
    } else {
        LOG_E("LED NUM is not 0 or 1!");
        return AW2016A_FUNC_RET_ERR;
    }

    switch (op) {
        case HL_DRV_AW2016A_GET_CHIP_ID: {
            if (arg_size != sizeof(uint8_t)) {
                LOG_E("size err, ctrl arg need <uint8_t> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = get_chip_id(p_i2c_bus, (uint8_t*)arg);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_CHECK_CHIP_STATUS: {
            if (arg_size != sizeof(hl_drv_aw2016a_chip_status_e)) {
                LOG_E("size err, ctrl arg need <hl_drv_aw2016a_chip_status_e> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = check_chip_status(p_i2c_bus, (hl_drv_aw2016a_chip_status_e*)arg);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_SET_WORK_MODE: {
            if (arg_size != sizeof(uint8_t)) {
                LOG_E("size err, ctrl arg need <uint8_t> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = set_work_mode(p_i2c_bus, (uint8_t*)arg);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_SET_GLOBAL_MAX_OUTPUT_CURRENT: {
            if (arg_size != sizeof(uint8_t)) {
                LOG_E("size err, ctrl arg need <uint8_t> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = set_global_max_output_current(p_i2c_bus, (uint8_t*)arg);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_OPEN_LED_CHANNEL: {
            if (arg_size != sizeof(uint8_t)) {
                LOG_E("size err, ctrl arg need <uint8_t> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = open_led_channel(p_i2c_bus, (uint8_t*)arg);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_CLOSE_LED_CHANNEL: {
            if (arg_size != sizeof(uint8_t)) {
                LOG_E("size err, ctrl arg need <uint8_t> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = close_led_channel(p_i2c_bus, (uint8_t*)arg);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_SET_PATTERN_MODE: {
            if (arg_size != sizeof(uint8_t)) {
                LOG_E("size err, ctrl arg need <uint8_t> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = set_pattern_mode(p_i2c_bus, (uint8_t*)arg);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_SET_BREATH_PARAM: {
            if (arg_size != sizeof(hl_drv_aw2016a_breath_param_st)) {
                LOG_E("size err, ctrl arg need <hl_drv_aw2016a_breath_param_st> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = set_pattern_mode_param(p_i2c_bus, (hl_drv_aw2016a_breath_param_st*)arg);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_SET_MANUAL_MODE: {
            if (arg_size != sizeof(uint8_t)) {
                LOG_E("size err, ctrl arg need <uint8_t> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = set_manual_mode(p_i2c_bus, (uint8_t*)arg);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_SET_LED_CHANNEL_OUTPUT_CURRENT: {
            if (arg_size != sizeof(hl_drv_aw2016a_output_current_st)) {
                LOG_E("size err, ctrl arg need <hl_drv_aw2016a_output_current_st> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = set_output_current(p_i2c_bus, (hl_drv_aw2016a_output_current_st*)arg);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_SET_LED_CHANNEL_PWM_LEVEL: {
            if (arg_size != sizeof(hl_drv_aw2016a_pwm_level_st)) {
                LOG_E("size err, ctrl arg need <hl_drv_aw2016a_pwm_level_st> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = set_pwm_level(p_i2c_bus, (hl_drv_aw2016a_pwm_level_st*)arg);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_DUMP_REGISTER_VALUE: {
            ret = dump_register_value(p_i2c_bus);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_SET_LED_LIGHT_EFFECT: {
            if (arg_size != sizeof(hl_drv_aw2016a_light_st)) {
                LOG_E("size err, ctrl arg need <hl_drv_aw2016a_light_st> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = set_led_light_effect(p_i2c_bus, (hl_drv_aw2016a_light_st*)arg);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_SET_SYNC_CONTROL_MODE: {
            if (arg_size != sizeof(bool)) {
                LOG_E("size err, ctrl arg need <bool> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = set_sync_control_mode(p_i2c_bus, (bool*)arg);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_SET_FADE_OUT_ENABLE: {
            if (arg_size != sizeof(hl_drv_aw2016a_set_fade_in_out_st)) {
                LOG_E("size err, ctrl arg need <hl_drv_aw2016a_set_fade_in_out_st> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = set_fade_out_enable(p_i2c_bus, (hl_drv_aw2016a_set_fade_in_out_st*)arg);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_SET_FADE_IN_ENABLE: {
            if (arg_size != sizeof(hl_drv_aw2016a_set_fade_in_out_st)) {
                LOG_E("size err, ctrl arg need <hl_drv_aw2016a_set_fade_in_out_st> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = set_fade_in_enable(p_i2c_bus, (hl_drv_aw2016a_set_fade_in_out_st*)arg);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_LED_CTRL: {
            if (arg_size != sizeof(hl_drv_aw2016a_led_ctrl_st)) {
                LOG_E("size err, ctrl arg need <hl_drv_aw2016a_led_ctrl_st> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = _user_func_led_ctrl(p_i2c_bus, (hl_drv_aw2016a_led_ctrl_st*)arg);
            if (ret == AW2016A_FUNC_RET_ERR) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        default:
            break;
    }

    return AW2016A_FUNC_RET_OK;
}

/*
 * EOF
 */