/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  ******************************************************************************
  * @file    nau88l25b.h
  * @author  Xiaotan Luo
  * @version v0.1
  * @date    2019.10.16
  * @brief   The rt-thread codec driver for Rockchip
  ******************************************************************************
  */

#ifndef __HL_DRV_NAU88L25B_H__
#define __HL_DRV_NAU88L25B_H__
#include "stdint.h"

/* 函数返回状态 */
#define HL_SUCCESS 0
#define HL_FAILED 1

/* 使用I2C */
#define NAU88L25B_IIC_NAME "i2c0"
#define NAU88L25B_DEVICE_ADDRESS 0x1A

/* NAU88L25B_REGISTER NAME_REG_REGISTER ADDRESS */

#define NAU88L25B_RESET_REG00 0x00 /*reset digital,csm,clock manager etc.*/

/* Clock Scheme Register definition */

#define NAU88L25B_CLK_MANAGER_REG01 0x01 /* select clk src for mclk, enable clock for codec */
#define NAU88L25B_CLK_MANAGER_REG02 0x02 /* clk divider and clk multiplier */
#define NAU88L25B_CLK_MANAGER_REG03 0x03 /* adc fsmode and osr  */
#define NAU88L25B_CLK_MANAGER_REG04 0x04 /* dac osr */
#define NAU88L25B_CLK_MANAGER_REG05 0x05 /* clk divier for adc and dac */
#define NAU88L25B_CLK_MANAGER_REG06 0x06 /* bclk inverter and divider */
#define NAU88L25B_CLK_MANAGER_REG07 0x07 /* tri-state, lrck divider */
#define NAU88L25B_CLK_MANAGER_REG08 0x08 /* lrck divider */
#define NAU88L25B_SDPIN_REG09 0x09       /* dac serial digital port */
#define NAU88L25B_SDPIN_REG09_DACWL_MASK (7 << 2)
#define NAU88L25B_SDPIN_REG09_DACWL_SHIFT 2
#define NAU88L25B_SDPOUT_REG0A 0x0A /* adc serial digital port */
#define NAU88L25B_SDPOUT_REG0A_ADCWL_MASK (7 << 2)
#define NAU88L25B_SDPOUT_REG0A_ADCWL_SHIFT 2
#define NAU88L25B_SYSTEM_REG0B 0x0B /* system */
#define NAU88L25B_SYSTEM_REG0C 0x0C /* system */
#define NAU88L25B_SYSTEM_REG0D 0x0D /* system, power up/down */
#define NAU88L25B_SYSTEM_REG0E 0x0E /* system, power up/down */
#define NAU88L25B_SYSTEM_REG0F 0x0F /* system, low power */
#define NAU88L25B_SYSTEM_REG10 0x10 /* system */
#define NAU88L25B_SYSTEM_REG11 0x11 /* system */
#define NAU88L25B_SYSTEM_REG12 0x12 /* system, Enable DAC */
#define NAU88L25B_SYSTEM_REG13 0x13 /* system */
#define NAU88L25B_SYSTEM_REG14 0x14 /* system, select DMIC, select analog pga gain */
#define NAU88L25B_ADC_REG15 0x15    /* ADC, adc ramp rate, dmic sense */
#define NAU88L25B_ADC_REG16 0x16    /* ADC */
#define NAU88L25B_ADC_REG17 0x17    /* ADC, volume */
#define NAU88L25B_ADC_REG18 0x18    /* ADC, alc enable and winsize */
#define NAU88L25B_ADC_REG19 0x19    /* ADC, alc maxlevel */
#define NAU88L25B_ADC_REG1A 0x1A    /* ADC, alc automute */
#define NAU88L25B_ADC_REG1B 0x1B    /* ADC, alc automute, adc hpf s1 */
#define NAU88L25B_ADC_REG1C 0x1C    /* ADC, equalizer, hpf s2 */
#define NAU88L25B_DAC_REG31 0x31    /* DAC, mute */
#define NAU88L25B_DAC_REG32 0x32    /* DAC, volume */
#define NAU88L25B_DAC_REG33 0x33    /* DAC, offset */
#define NAU88L25B_DAC_REG34 0x34    /* DAC, drc enable, drc winsize */
#define NAU88L25B_DAC_REG35 0x35    /* DAC, drc maxlevel, minilevel */
#define NAU88L25B_DAC_REG37 0x37    /* DAC, ramprate */
#define NAU88L25B_GPIO_REG44 0x44   /* GPIO, dac2adc for test */
#define NAU88L25B_GP_REG45 0x45     /* GP CONTROL */
#define NAU88L25B_CHD1_REGFD 0xFD   /* CHIP ID1 */
#define NAU88L25B_CHD2_REGFE 0xFE   /* CHIP ID2 */
#define NAU88L25B_CHVER_REGFF 0xFF  /* VERSION */
#define NAU88L25B_CHD1_REGFD 0xFD   /* CHIP ID1 */
#define NAU88L25B_MAX_REGISTER 0xFF

uint8_t hl_drv_nau88l25b_init(void);
uint8_t hl_drv_nau88l25b_deinit(void);
uint8_t hl_drv_nau88l25b_io_ctrl(uint8_t cmd, uint8_t* ptr, uint8_t len);

#endif /* __NAU88L25B_H__ */
