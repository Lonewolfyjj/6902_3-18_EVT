/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  ******************************************************************************
  * @file    nau88l25b.c
  * @author  Xiaotan Luo
  * @version v0.1
  * @date    2019.10.16
  * @brief   The rt-thread codec driver for Rockchip
  ******************************************************************************
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

/* Vendor suggest the startup delay should >100ms */
#define PA_CTL_DELAY_MS 120
#define NAU88L25B_PA_GPIO_BANK PA_GPIO_BANK
#define NAU88L25B_PA_GPIO PA_GPIO
#define NAU88L25B_PA_PIN PA_PIN
#define NAU88L25B_PA_PIN_FUNC_GPIO PA_PIN_FUNC_GPIO

#define UAN88L25B_REG_SOFT_WARE_RST (0x0000)
#define UAN88L25B_REG_ENA_CTRL (0x0001)
#define UAN88L25B_REG_I2C_ADDR_SET (0x0002)
#define UAN88L25B_REG_CLK_DIVIDER (0x0003)
#define UAN88L25B_REG_FLL1 (0x0004)
#define UAN88L25B_REG_FLL2 (0x0005)
#define UAN88L25B_REG_FLL3 (0x0006)
#define UAN88L25B_REG_FLL4 (0x0007)
#define UAN88L25B_REG_FLL5 (0x0008)
#define UAN88L25B_REG_FLL6 (0x0009)
#define UAN88L25B_REG_FLL_VCO_RSV (0x000A)
#define UAN88L25B_REG_HSD_CTRL (0x000C)
#define UAN88L25B_REG_JACK_DET_CTRL (0x000D)
#define UAN88L25B_REG_INTERRUPT_MASK (0x000F)
#define UAN88L25B_REG_IRQ_STATUS (0x0010)
#define UAN88L25B_REG_INT_CLR_KEY_STATU (0x0011)
#define UAN88L25B_REG_INTERRUPT_DIS_CTRL (0x0012)
#define UAN88L25B_REG_SAR_CTRL (0x0013)
#define UAN88L25B_REG_KEYDET_CTRL (0x0014)
#define UAN88L25B_REG_VDET_THRESHOLD_1 (0x0015)
#define UAN88L25B_REG_VDET_THRESHOLD_2 (0x0016)
#define UAN88L25B_REG_VDET_THRESHOLD_3 (0x0017)
#define UAN88L25B_REG_VDET_THRESHOLD_4 (0x0018)
#define UAN88L25B_REG_GPIO34_CTRL (0x0019)
#define UAN88L25B_REG_GPIO12_CTRL (0x001A)
#define UAN88L25B_REG_TDM_CTRL (0x001B)
#define UAN88L25B_REG_I2S_PCM_CTRL1 (0x001C)
#define UAN88L25B_REG_I2S_PCM_CTRL2 (0x001D)
#define UAN88L25B_REG_LEFT_TIME_SLOT (0x001E)
#define UAN88L25B_REG_RIGHT_TIME_SLOT (0x001F)
#define UAN88L25B_REG_BIQ_CTRL (0x0020)
#define UAN88L25B_REG_BIQ_COF1 (0x0021)
#define UAN88L25B_REG_BIQ_COF2 (0x0022)
#define UAN88L25B_REG_BIQ_COF3 (0x0023)
#define UAN88L25B_REG_BIQ_COF4 (0x0024)
#define UAN88L25B_REG_BIQ_COF5 (0x0025)
#define UAN88L25B_REG_BIQ_COF6 (0x0026)
#define UAN88L25B_REG_BIQ_COF7 (0x0027)
#define UAN88L25B_REG_BIQ_COF8 (0x0028)
#define UAN88L25B_REG_BIQ_COF9 (0x0029)
#define UAN88L25B_REG_BIQ_COF10 (0x002A)
#define UAN88L25B_REG_ADC_RATE (0x002B)
#define UAN88L25B_REG_DAC_CTRL1 (0x002C)
#define UAN88L25B_REG_DAC_CTRL2 (0x002D)
#define UAN88L25B_REG_DAC_DGAIN_CTRL (0x002F)
#define UAN88L25B_REG_ADC_DGAIN_CTRL (0x0030)
#define UAN88L25B_REG_MUTE_CTRL (0x0031)
#define UAN88L25B_REG_HSVOL_CTRL (0x0032)
#define UAN88L25B_REG_DACL_CTRL (0x0033)
#define UAN88L25B_REG_DACR_CTRL (0x0034)
#define UAN88L25B_REG_ADC_DRC_KNEE_IP12 (0x0038)
#define UAN88L25B_REG_ADC_DRC_KNEE_IP34 (0x0039)
#define UAN88L25B_REG_ADC_DRC_SLOPES (0x003A)
#define UAN88L25B_REG_ADC_DRC_ATKDCY (0x003B)
#define UAN88L25B_REG_DAC_DRC_KNEE_IP12 (0x0045)
#define UAN88L25B_REG_DAC_DRC_KNEE_IP34 (0x0046)
#define UAN88L25B_REG_DAC_DRC_SLOPES (0x0047)
#define UAN88L25B_REG_DAC_DRC_ATKDCY (0x0048)
#define UAN88L25B_REG_MODE_CTRL (0x004C)
#define UAN88L25B_REG_CLASSG_CTRL (0x0050)
#define UAN88L25B_REG_OPT_EFUSE_CTRL (0x0051)
#define UAN88L25B_REG_MISC_CTRL (0x0055)
#define UAN88L25B_REG_I2C_DEVICE_ID (0x0058)
#define UAN88L25B_REG_SARDOUT_RAM_STATUS (0x0059)
#define UAN88L25B_REG_BIAS_ADJ (0x0066)
#define UAN88L25B_REG_TRIM_SETTINGS (0x0068)
#define UAN88L25B_REG_ANALOG_CONTROL_1 (0x0069)
#define UAN88L25B_REG_ANALOG_CONTROL_2 (0x006A)
#define UAN88L25B_REG_ANALOG_ADC_1 (0x0071)
#define UAN88L25B_REG_ANALOG_ADC_2 (0x0072)
#define UAN88L25B_REG_RDAC (0x0073)
#define UAN88L25B_REG_MIC_BIAS (0x0074)
#define UAN88L25B_REG_BOOST (0x0076)
#define UAN88L25B_REG_FEPGA (0x0077)
#define UAN88L25B_REG_POWER_UP_CONTROL (0x007F)
#define UAN88L25B_REG_CHARGE_PUMP_AND_POWER_DOWN_CONTROL (0x0080)
#define UAN88L25B_REG_CHARGE_PUMP_INPUT_READ (0x0081)
#define UAN88L25B_REG_GENERAL_STATUS (0x0082)

static struct rt_i2c_bus_device* i2c_bus = RT_NULL; /* I2C总线设备句柄 */

/* NAU88L25B init the register value */
typedef struct _NAU88L25B_REG_T
{
    uint16_t reg_addr;
    uint16_t reg_value;
} NAU88L25B_REG_T;

static struct rt_mutex lock;

#define HL_CODEC_SET 1

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

static rt_err_t i2c_write_reg(struct rt_i2c_bus_device* bus, rt_uint16_t reg, rt_uint16_t* data)
{
    rt_uint8_t        buf[4];
    struct rt_i2c_msg msgs[2];

    buf[0] = (reg >> 8);        // reg
    buf[1] = (reg & 0xFF);      //
    buf[2] = (data[0] >> 8);    // data
    buf[3] = (data[0] & 0xFF);  //

    msgs[0].addr  = NAU88L25B_DEVICE_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = buf;
    msgs[0].len   = 2;

    msgs[1].addr  = NAU88L25B_DEVICE_ADDRESS;
    msgs[1].flags = RT_I2C_WR | RT_I2C_NO_START;
    msgs[1].buf   = &buf[2];
    msgs[1].len   = 2;

    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(bus, &msgs, 2) == 1)
        return HL_SUCCESS;
    else
        return HL_FAILED;
}

static rt_err_t i2c_read_reg(struct rt_i2c_bus_device* bus, rt_uint16_t reg, rt_uint16_t* rbuf)
{
    rt_uint8_t        buf[4];
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

    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(bus, &msgs, 1) == 1) {
        rbuf[0] = ((buf[2] << 8) + buf[3]);
        return HL_SUCCESS;
    } else
        return HL_FAILED;
}

/*
static rt_err_t nau88l25b_set_gain(struct audio_codec* codec, eAUDIO_streamType stream,
                                   struct AUDIO_DB_CONFIG* db_config)
{
    struct nau88l25b_priv* nau88l25b = to_nau88l25b_priv(codec);
    uint16_t               reg_gain  = 0x00;

    RT_ASSERT(db_config);

    if (db_config->dB < -103) {
        reg_gain = 0x00;  // 小于103则默认mute
    } else {
        reg_gain = db_config->dB * 2 + 207;
    }

    es_wr_reg(nau88l25b->i2c_client, UAN88L25B_REG_DACL_CTRL, reg_gain);
    es_wr_reg(nau88l25b->i2c_client, UAN88L25B_REG_DACR_CTRL, reg_gain);

    return RT_EOK;
}

static rt_err_t nau88l25b_get_gain(struct audio_codec* codec, eAUDIO_streamType stream,
                                   struct AUDIO_DB_CONFIG* db_config)
{
    struct nau88l25b_priv* nau88l25b = to_nau88l25b_priv(codec);
    unsigned short         val, volume;

    RT_ASSERT(db_config);

    es_rd_reg(nau88l25b->i2c_client, UAN88L25B_REG_DACL_CTRL, &val);
    es_rd_reg(nau88l25b->i2c_client, UAN88L25B_REG_DACR_CTRL, &val);
    volume = val >> 8;

    /* The min volume is -100dB, register value is 200. *//*
    if (volume > 200)
        volume = 200;

    db_config->dB = (volume - 207) / 2;

    return RT_EOK;
}
*/

static uint8_t rt_hw_codec_nau88l25b_init(void)
{
    uint16_t i, j = sizeof(s_nau88l25b_param) / 4;
    smg_printf("nau reg num : %d !\n", j);
    /* 查找I2C总线设备，获取I2C总线设备句柄 */
    i2c_bus = (struct rt_i2c_bus_device*)rt_device_find(NAU88L25B_IIC_NAME);
    if (i2c_bus == RT_NULL) {
        smg_printf("can't find %s device!\n", NAU88L25B_IIC_NAME);
        goto INIT_ERR;
    }
    for (i = 0; i < j; i++) {
        if (i2c_write_reg(i2c_bus, s_nau88l25b_param[i].reg_addr, &s_nau88l25b_param[i].reg_value)) {
            smg_printf("nau reg init fail !\n");
            goto INIT_ERR;
        }
    }
    return HL_SUCCESS;
INIT_ERR:
    return HL_FAILED;
}

INIT_DEVICE_EXPORT(rt_hw_codec_nau88l25b_init);
