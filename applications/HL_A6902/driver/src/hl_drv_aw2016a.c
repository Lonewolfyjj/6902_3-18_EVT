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

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/

#define AW2016_IIC_BUS_0_NAME "i2c0"
#define AW2016_IIC_BUS_1_NAME "i2c1"

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

/* chip info */
#define AW2016A_IIC_DEV_ADDR 0x64

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

static int set_pattern_mode_param(struct rt_i2c_bus_device* p_i2c_bus, hl_drv_aw2016a_pattern_param_st* p_param)
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

/* Exported functions --------------------------------------------------------*/

int hl_drv_aw2016a_init(void)
{
    if (_init_flag == 1) {
        //("LED is already inited!");
        return AW2016A_FUNC_RET_ERR;
    }

    _p_i2c_bus_0 = (struct rt_i2c_bus_device*)rt_device_find(AW2016_IIC_BUS_0_NAME);
    if (_p_i2c_bus_0 == NULL) {
        //("i2c dev not found!:%s", AW2016_IIC_BUS_0_NAME);
        return AW2016A_FUNC_RET_ERR;
    }

    _p_i2c_bus_1 = (struct rt_i2c_bus_device*)rt_device_find(AW2016_IIC_BUS_1_NAME);
    if (_p_i2c_bus_1 == NULL) {
        //("i2c dev not found!:%s", AW2016_IIC_BUS_1_NAME);
        return AW2016A_FUNC_RET_ERR;
    }

    _init_flag = 1;

    return AW2016A_FUNC_RET_OK;
}

int hl_drv_aw2016a_deinit(void)
{
    if (_init_flag == 0) {
        //("LED is not inited!");
        return AW2016A_FUNC_RET_ERR;
    }

    _init_flag = 0;

    return AW2016A_FUNC_RET_OK;
}

int hl_drv_aw2016a_ctrl(uint8_t led_num, uint8_t op, void* arg, int32_t arg_size)
{
    int                       ret;
    struct rt_i2c_bus_device* p_i2c_bus;

    if (_init_flag == 0) {
        //("LED is not inited!");
        return AW2016A_FUNC_RET_ERR;
    }

    if (led_num == HL_DRV_AW2016A_LED0) {
        p_i2c_bus = _p_i2c_bus_0;
    } else if (led_num == HL_DRV_AW2016A_LED1) {
        p_i2c_bus = _p_i2c_bus_1;
    } else {
        //("LED NUM is not 0 or 1!");
        return AW2016A_FUNC_RET_ERR;
    }

    switch (op) {
        case HL_DRV_AW2016A_GET_CHIP_ID: {
            if (arg_size != sizeof(uint8_t)) {
                //("size err, ctrl arg need <uint8_t> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = get_chip_id(p_i2c_bus, (uint8_t*)arg);
            if (ret < 0) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_SET_WORK_MODE: {
            if (arg_size != sizeof(uint8_t)) {
                //("size err, ctrl arg need <uint8_t> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = set_work_mode(p_i2c_bus, (uint8_t*)arg);
            if (ret < 0) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_SET_GLOBAL_MAX_OUTPUT_CURRENT: {
            if (arg_size != sizeof(uint8_t)) {
                //("size err, ctrl arg need <uint8_t> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = set_global_max_output_current(p_i2c_bus, (uint8_t*)arg);
            if (ret < 0) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_OPEN_LED_CHANNEL: {
            if (arg_size != sizeof(uint8_t)) {
                //("size err, ctrl arg need <uint8_t> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = open_led_channel(p_i2c_bus, (uint8_t*)arg);
            if (ret < 0) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_CLOSE_LED_CHANNEL: {
            if (arg_size != sizeof(uint8_t)) {
                //("size err, ctrl arg need <uint8_t> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = close_led_channel(p_i2c_bus, (uint8_t*)arg);
            if (ret < 0) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_SET_PATTERN_MODE: {
            if (arg_size != sizeof(uint8_t)) {
                //("size err, ctrl arg need <uint8_t> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = set_pattern_mode(p_i2c_bus, (uint8_t*)arg);
            if (ret < 0) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_SET_PATTERN_MODE_PARAM: {
            if (arg_size != sizeof(hl_drv_aw2016a_pattern_param_st)) {
                //("size err, ctrl arg need <hl_drv_aw2016a_pattern_param_st> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = set_pattern_mode_param(p_i2c_bus, (hl_drv_aw2016a_pattern_param_st*)arg);
            if (ret < 0) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_SET_MANUAL_MODE: {
            if (arg_size != sizeof(uint8_t)) {
                //("size err, ctrl arg need <uint8_t> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = set_manual_mode(p_i2c_bus, (uint8_t*)arg);
            if (ret < 0) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_SET_LED_CHANNEL_OUTPUT_CURRENT: {
            if (arg_size != sizeof(hl_drv_aw2016a_output_current_st)) {
                //("size err, ctrl arg need <hl_drv_aw2016a_output_current_st> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = set_output_current(p_i2c_bus, (hl_drv_aw2016a_output_current_st*)arg);
            if (ret < 0) {
                return AW2016A_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_AW2016A_SET_LED_CHANNEL_PWM_LEVEL: {
            if (arg_size != sizeof(hl_drv_aw2016a_pwm_level_st)) {
                //("size err, ctrl arg need <hl_drv_aw2016a_pwm_level_st> type pointer!");
                return AW2016A_FUNC_RET_ERR;
            }

            ret = set_pwm_level(p_i2c_bus, (hl_drv_aw2016a_pwm_level_st*)arg);
            if (ret < 0) {
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