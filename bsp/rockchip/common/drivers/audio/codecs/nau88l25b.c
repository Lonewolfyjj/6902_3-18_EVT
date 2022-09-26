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

#include <rtdevice.h>
#include <rtthread.h>

//#define RT_USING_CODEC_NAU88L25B //< temp flag

#ifdef RT_USING_CODEC_NAU88L25B

#include "board.h"
#include "rk_audio.h"
#include "drv_codecs.h"
#include "hal_bsp.h"
#include "nau88l25b.h"
#include "hl_config.h"

/* Vendor suggest the startup delay should >100ms */
#define PA_CTL_DELAY_MS        120
#define NAU88L25B_PA_GPIO_BANK PA_GPIO_BANK
#define NAU88L25B_PA_GPIO      PA_GPIO
#define NAU88L25B_PA_PIN       PA_PIN
#define NAU88L25B_PA_PIN_FUNC_GPIO PA_PIN_FUNC_GPIO

#define UAN88L25B_REG_SOFT_WARE_RST         (0x0000)    
#define UAN88L25B_REG_ENA_CTRL              (0x0001)   
#define UAN88L25B_REG_I2C_ADDR_SET          (0x0002)   
#define UAN88L25B_REG_CLK_DIVIDER           (0x0003)   
#define UAN88L25B_REG_FLL1                  (0x0004)   
#define UAN88L25B_REG_FLL2                  (0x0005)   
#define UAN88L25B_REG_FLL3                  (0x0006)   
#define UAN88L25B_REG_FLL4                  (0x0007)   
#define UAN88L25B_REG_FLL5                  (0x0008)   
#define UAN88L25B_REG_FLL6                  (0x0009)   
#define UAN88L25B_REG_FLL_VCO_RSV           (0x000A)       
#define UAN88L25B_REG_HSD_CTRL              (0x000C)       
#define UAN88L25B_REG_JACK_DET_CTRL         (0x000D)       
#define UAN88L25B_REG_INTERRUPT_MASK        (0x000F)           
#define UAN88L25B_REG_IRQ_STATUS            (0x0010)       
#define UAN88L25B_REG_INT_CLR_KEY_STATU     (0x0011)       
#define UAN88L25B_REG_INTERRUPT_DIS_CTRL    (0x0012)       
#define UAN88L25B_REG_SAR_CTRL              (0x0013)                       
#define UAN88L25B_REG_KEYDET_CTRL           (0x0014)                       
#define UAN88L25B_REG_VDET_THRESHOLD_1      (0x0015)                   
#define UAN88L25B_REG_VDET_THRESHOLD_2      (0x0016)               
#define UAN88L25B_REG_VDET_THRESHOLD_3      (0x0017)                               
#define UAN88L25B_REG_VDET_THRESHOLD_4      (0x0018)                               
#define UAN88L25B_REG_GPIO34_CTRL           (0x0019)                               
#define UAN88L25B_REG_GPIO12_CTRL           (0x001A)                       
#define UAN88L25B_REG_TDM_CTRL              (0x001B)                       
#define UAN88L25B_REG_I2S_PCM_CTRL1         (0x001C)                   
#define UAN88L25B_REG_I2S_PCM_CTRL2         (0x001D)                   
#define UAN88L25B_REG_LEFT_TIME_SLOT        (0x001E)                   
#define UAN88L25B_REG_RIGHT_TIME_SLOT       (0x001F)                           
#define UAN88L25B_REG_BIQ_CTRL              (0x0020)                           
#define UAN88L25B_REG_BIQ_COF1              (0x0021)               
#define UAN88L25B_REG_BIQ_COF2              (0x0022)                   
#define UAN88L25B_REG_BIQ_COF3              (0x0023)                       
#define UAN88L25B_REG_BIQ_COF4              (0x0024)           
#define UAN88L25B_REG_BIQ_COF5              (0x0025)                   
#define UAN88L25B_REG_BIQ_COF6              (0x0026)                           
#define UAN88L25B_REG_BIQ_COF7              (0x0027)                                   
#define UAN88L25B_REG_BIQ_COF8              (0x0028)  
#define UAN88L25B_REG_BIQ_COF9              (0x0029)     
#define UAN88L25B_REG_BIQ_COF10             (0x002A)     
#define UAN88L25B_REG_ADC_RATE              (0x002B)     
#define UAN88L25B_REG_DAC_CTRL1             (0x002C)     
#define UAN88L25B_REG_DAC_CTRL2             (0x002D)     
#define UAN88L25B_REG_DAC_DGAIN_CTRL        (0x002F)         
#define UAN88L25B_REG_ADC_DGAIN_CTRL        (0x0030)         
#define UAN88L25B_REG_MUTE_CTRL             (0x0031)     
#define UAN88L25B_REG_HSVOL_CTRL            (0x0032)     
#define UAN88L25B_REG_DACL_CTRL             (0x0033)     
#define UAN88L25B_REG_DACR_CTRL             (0x0034)     
#define UAN88L25B_REG_ADC_DRC_KNEE_IP12     (0x0038)             
#define UAN88L25B_REG_ADC_DRC_KNEE_IP34     (0x0039)             
#define UAN88L25B_REG_ADC_DRC_SLOPES        (0x003A)         
#define UAN88L25B_REG_ADC_DRC_ATKDCY        (0x003B)         
#define UAN88L25B_REG_DAC_DRC_KNEE_IP12     (0x0045)             
#define UAN88L25B_REG_DAC_DRC_KNEE_IP34     (0x0046)             
#define UAN88L25B_REG_DAC_DRC_SLOPES        (0x0047)         
#define UAN88L25B_REG_DAC_DRC_ATKDCY        (0x0048)         
#define UAN88L25B_REG_MODE_CTRL             (0x004C)     
#define UAN88L25B_REG_CLASSG_CTRL           (0x0050)     
#define UAN88L25B_REG_OPT_EFUSE_CTRL        (0x0051)         
#define UAN88L25B_REG_MISC_CTRL             (0x0055)     
#define UAN88L25B_REG_I2C_DEVICE_ID         (0x0058)         
#define UAN88L25B_REG_SARDOUT_RAM_STATUS    (0x0059)             
#define UAN88L25B_REG_BIAS_ADJ              (0x0066)     
#define UAN88L25B_REG_TRIM_SETTINGS         (0x0068)         
#define UAN88L25B_REG_ANALOG_CONTROL_1      (0x0069)             
#define UAN88L25B_REG_ANALOG_CONTROL_2      (0x006A)             
#define UAN88L25B_REG_ANALOG_ADC_1          (0x0071)         
#define UAN88L25B_REG_ANALOG_ADC_2          (0x0072)         
#define UAN88L25B_REG_RDAC                  (0x0073) 
#define UAN88L25B_REG_MIC_BIAS              (0x0074)     
#define UAN88L25B_REG_BOOST                 (0x0076) 
#define UAN88L25B_REG_FEPGA                 (0x0077) 
#define UAN88L25B_REG_POWER_UP_CONTROL      (0x007F)             
#define UAN88L25B_REG_CHARGE_PUMP_AND_POWER_DOWN_CONTROL      (0x0080)                      
#define UAN88L25B_REG_CHARGE_PUMP_INPUT_READ                  (0x0081)                       
#define UAN88L25B_REG_GENERAL_STATUS                          (0x0082)               


struct nau88l25b_priv
{
    struct audio_codec codec;
    struct rt_i2c_client *i2c_client;
    uint32_t work_cnt;
};

/* NAU88L25B init the register value */
typedef struct _NAU88L25B_REG_T
{
    uint16_t reg_addr;
    uint16_t reg_value;
}NAU88L25B_REG_T;

static struct rt_mutex lock;

#define HL_CODEC_SET 0
//#define RT_CODEC_NAU88L21 1
NAU88L25B_REG_T s_nau88l25b_param[] = {
#ifdef RT_CODEC_NAU88L21
    {0x0000, 0x0000}, 
    {0x0001, 0x0FD6},
    {0x0003, 0x0410},
    {0x0004, 0x0000},
    {0x0005, 0x00BC},
    {0x0006, 0x0008},
    {0x0007, 0x0010},
    {0x0008, 0x4000},
    {0x0009, 0x6900},
    {0x000A, 0x0031},
    {0x000B, 0x26E9},
    {0x000D, 0xC000},
    {0x000F, 0x0000},
    {0x0010, 0x0000},
    {0x0011, 0x0000},
    {0x0012, 0xFFFF},
    {0x0013, 0x0000},
    {0x001A, 0x0000},
    {0x001B, 0x0000},
    {0x001C, 0x000A},
    {0x001D, 0x0000},
    {0x001E, 0x2000},
    {0x001F, 0x0000},
    {0x0021, 0x23CE},
    {0x0022, 0x0006},
    {0x0023, 0xDE80},
    {0x0024, 0x0000},
    {0x0025, 0xEEB0},
    {0x0026, 0x0000},
    {0x0027, 0x22A0},
    {0x0028, 0x0006},
    {0x0029, 0xEEB0},
    {0x002A, 0x0008},
    {0x002B, 0x0003},
    {0x002C, 0x0082},
    {0x002D, 0x0000},
    {0x002F, 0x0000},
    {0x0030, 0x0000},
    {0x0031, 0x0000},
    {0x0032, 0x0000},
    {0x0034, 0xCFCF},
    {0x0035, 0xF3F3},
    {0x0036, 0x1486},
    {0x0037, 0x0F12},
    {0x0038, 0x25FF},
    {0x0039, 0x3457},
    {0x003A, 0x1486},
    {0x003B, 0x0F12},
    {0x003C, 0x25F9},
    {0x003D, 0x3457},
    {0x0041, 0x0000},
    {0x0042, 0x0000},
    {0x0043, 0x0000},
    {0x0044, 0x0000},
    {0x0045, 0x0000},
    {0x0046, 0x0000},
    {0x0047, 0x0000},
    {0x0048, 0x0000},
    {0x0049, 0x0000},
    {0x004A, 0x0000},
    {0x004B, 0x2007},
    {0x004C, 0x0000},
    {0x004D, 0x0000},
    {0x0053, 0xB0E8},
    {0x0054, 0x0000},
    {0x0055, 0x0000},
    {0x0058, 0x1B23},
    {0x0059, 0x2000},
    {0x0066, 0x0060},
    {0x0068, 0x0000},
    {0x0069, 0x0000},
    {0x006A, 0x1003},
    {0x006B, 0x0000},
    {0x0071, 0x0011},
    {0x0072, 0x0170},
    {0x0073, 0x3308},
    {0x0074, 0x1506},
    {0x0076, 0x7140},
    {0x0077, 0x0020},
    {0x007E, 0x1111},
    {0x007F, 0xC03F},
    {0x0080, 0x0720},
    {0x0081, 0x002C},
    {0x0082, 0x0060},

#else
    /*{0x0000, 0x0000}, 
    {0x0001, 0x07FF},
    {0x0002, 0x0000},
    {0x0003, 0x8050},
    {0x0004, 0x0081},
    {0x0005, 0x86C2},
    {0x0006, 0x0807},
    {0x0007, 0x0010},
    {0x0008, 0x1000},
    {0x0009, 0x0000},
    {0x000A, 0xF13C},
    {0x000C, 0x0048},
    {0x000D, 0x0000},
    {0x000F, 0x0000},
    {0x0010, 0x0000},
    {0x0011, 0x0000},
    {0x0012, 0xFFFF},
    {0x0013, 0x0015},
    {0x0014, 0x0110},
    {0x0015, 0x0000},
    {0x0016, 0x0000},
    {0x0017, 0x0000},
    {0x0018, 0x0000},
    {0x0019, 0x0000},
    {0x001A, 0x0000},
    {0x001B, 0x0000},
    {0x001C, 0x000A},
    {0x001D, 0x2012},
    {0x001E, 0x0000},
    {0x001F, 0x0000},
    {0x0020, 0x0000},
    {0x0021, 0x0000},
    {0x0022, 0x0000},
    {0x0023, 0x0000},
    {0x0024, 0x0000},
    {0x0025, 0x0000},
    {0x0026, 0x0000},
    {0x0027, 0x0000},
    {0x0028, 0x0000},
    {0x0029, 0x0000},
    {0x002A, 0x0000},
    {0x002B, 0x0012},
    {0x002C, 0x0080},
    {0x002D, 0x0000},
    {0x002F, 0x0000},
    {0x0030, 0x00CF},
    {0x0031, 0x1000},
    {0x0032, 0x0000},
    {0x0033, 0x00CF},
    {0x0034, 0x02CF},
    {0x0038, 0x1486},
    {0x0039, 0x0F12},
    {0x003A, 0x25FF},
    {0x003B, 0x3457},
    {0x0045, 0x1486},
    {0x0046, 0x0F12},
    {0x0047, 0x25F9},
    {0x0048, 0x3457},
    {0x004C, 0x0000},
    {0x0050, 0x2007},
    {0x0051, 0x0000},
    {0x0055, 0x0000},
    {0x0058, 0x1A14},
    {0x0059, 0x00FF},
    {0x0066, 0x0060},
    {0x0068, 0xC300},
    {0x0069, 0x0000},
    {0x006A, 0x0083},
    {0x0071, 0x0011},
    {0x0072, 0x0260},
    {0x0073, 0x332C},
    {0x0074, 0x4102},
    {0x0076, 0x3140},
    {0x0077, 0x0000},
    {0x007F, 0x403F},
    {0x0080, 0x0420},
    {0x0081, 0x002C},
    {0x0082, 0x0060},*/
	
    /*{0x0000, 0x0000}, /// 偏置电压1.5*
    {0x0001, 0x07FF},
    {0x0002, 0x0000},
    {0x0003, 0x8050},
    {0x0004, 0x0081},
    {0x0005, 0x86C2},
    {0x0006, 0x0807},
    {0x0007, 0x0010},
    {0x0008, 0x1000},
    {0x0009, 0x0000},
    {0x000A, 0xF13C},
    {0x000C, 0x0048},
    {0x000D, 0x0000},
    {0x000F, 0x0000},
    {0x0010, 0x0000},
    {0x0011, 0x0000},
    {0x0012, 0xFFFF},
    {0x0013, 0x0015},
    {0x0014, 0x0110},
    {0x0015, 0x0000},
    {0x0016, 0x0000},
    {0x0017, 0x0000},
    {0x0018, 0x0000},
    {0x0019, 0x0000},
    {0x001A, 0x0000},
    {0x001B, 0x0000},
    {0x001C, 0x000A},
    {0x001D, 0x2012},
    {0x001E, 0x0000},
    {0x001F, 0x0000},
    {0x0020, 0x0000},
    {0x0021, 0x0000},
    {0x0022, 0x0000},
    {0x0023, 0x0000},
    {0x0024, 0x0000},
    {0x0025, 0x0000},
    {0x0026, 0x0000},
    {0x0027, 0x0000},
    {0x0028, 0x0000},
    {0x0029, 0x0000},
    {0x002A, 0x0000},
    {0x002B, 0x0012},
    {0x002C, 0x0080},
    {0x002D, 0x0000},
    {0x002F, 0x0000},
    {0x0030, 0x00CF},
    {0x0031, 0x1000},
    {0x0032, 0x0000},
    {0x0033, 0x00CF},
    {0x0034, 0x02CF},
    {0x0038, 0x1486},
    {0x0039, 0x0F12},
    {0x003A, 0x25FF},
    {0x003B, 0x3457},
    {0x0045, 0x1486},
    {0x0046, 0x0F12},
    {0x0047, 0x25F9},
    {0x0048, 0x3457},
    {0x004C, 0x0000},
    {0x0050, 0x2007},
    {0x0051, 0x0000},
    {0x0055, 0x0000},
    {0x0058, 0x1A14},
    {0x0059, 0x00FF},
    {0x0066, 0x0060},
    {0x0068, 0xC300},
    {0x0069, 0x0000},
    {0x006A, 0x0083},
    {0x0071, 0x0011},
    {0x0072, 0x0260},
    {0x0073, 0x332C},
    {0x0074, 0x4107},
    {0x0076, 0x3140},
    {0x0077, 0x0000},
    {0x007F, 0x403F},
    {0x0080, 0x0420},
    {0x0081, 0x002C},
    {0x0082, 0x0060},*/
#if  0//HL_GET_DEVICE_TYPE()
    {0x0000, 0x0000},/// +mclk
    {0x0001, 0x07FF},
    {0x0002, 0x0000},
    {0x0003, 0x0050},
    {0x0004, 0x0081},
    {0x0005, 0x0000},
    {0x0006, 0x0408},
    {0x0007, 0x0010},
    {0x0008, 0x1000},
    {0x0009, 0x0000},
    {0x000A, 0xF13C},
    {0x000C, 0x0048},
    {0x000D, 0x0000},
    {0x000F, 0x0000},
    {0x0010, 0x0000},
    {0x0011, 0x0000},
    {0x0012, 0xFFFF},
    {0x0013, 0x0015},
    {0x0014, 0x0110},
    {0x0015, 0x0000},
    {0x0016, 0x0000},
    {0x0017, 0x0000},
    {0x0018, 0x0000},
    {0x0019, 0x0000},
    {0x001A, 0x0000},
    {0x001B, 0x0000},
    {0x001C, 0x000A},
    {0x001D, 0x1015},
    {0x001E, 0x0000},
    {0x001F, 0x0000},
    {0x0020, 0x0000},
    {0x0021, 0x0000},
    {0x0022, 0x0000},
    {0x0023, 0x0000},
    {0x0024, 0x0000},
    {0x0025, 0x0000},
    {0x0026, 0x0000},
    {0x0027, 0x0000},
    {0x0028, 0x0000},
    {0x0029, 0x0000},
    {0x002A, 0x0000},
    {0x002B, 0x0012},
    {0x002C, 0x0080},
    {0x002D, 0x0000},
    {0x002F, 0x0000},
    {0x0030, 0x00CF},
    {0x0031, 0x1000},
    {0x0032, 0x0000},
    {0x0033, 0x00CF},
    {0x0034, 0x02CF},
    {0x0038, 0x1486},
    {0x0039, 0x0F12},
    {0x003A, 0x25FF},
    {0x003B, 0x3457},
    {0x0045, 0x1486},
    {0x0046, 0x0F12},
    {0x0047, 0x25F9},
    {0x0048, 0x3457},
    {0x004C, 0x0000},
    {0x0050, 0x2007},
    {0x0051, 0x0000},
    {0x0055, 0x0000},
    {0x0058, 0x1A14},
    {0x0059, 0x00FF},
    {0x0066, 0x0060},
    {0x0068, 0xC300},
    {0x0069, 0x0000},
    {0x006A, 0x0083},
    {0x0071, 0x0011},
    {0x0072, 0x0260},
    {0x0073, 0x332C},
    {0x0074, 0x4107},
    {0x0076, 0x3140},
    {0x0077, 0x0000},
    {0x007F, 0x403F},
    {0x0080, 0x0420},
    {0x0081, 0x0008},
    {0x0082, 0x0C60},
#else
    {0x0000, 0x0000},// RX
    {0x0001, 0x07D4},
    {0x0002, 0x0000},
    {0x0003, 0x0040},
    {0x0004, 0x0001},
    {0x0005, 0x0000},
    {0x0006, 0x0408},
    {0x0007, 0x0010},
    {0x0008, 0xC000},
    {0x0009, 0x6000},
    {0x000A, 0xF13C},
    {0x000C, 0x0048},
    {0x000D, 0x0000},
    {0x000F, 0x0000},
    {0x0010, 0x0000},
    {0x0011, 0x0000},
    {0x0012, 0xFFFF},
    {0x0013, 0x0015},
    {0x0014, 0x0110},
    {0x0015, 0x0000},
    {0x0016, 0x0000},
    {0x0017, 0x0000},
    {0x0018, 0x0000},
    {0x0019, 0x0000},
    {0x001A, 0x0000},
    {0x001B, 0x0000},
    {0x001C, 0x000A},
    {0x001D, 0x1015},
    {0x001E, 0x0000},
    {0x001F, 0x0000},
    {0x0020, 0x0000},
    {0x0021, 0x0000},
    {0x0022, 0x0000},
    {0x0023, 0x0000},
    {0x0024, 0x0000},
    {0x0025, 0x0000},
    {0x0026, 0x0000},
    {0x0027, 0x0000},
    {0x0028, 0x0000},
    {0x0029, 0x0000},
    {0x002A, 0x0000},
    {0x002B, 0x00D2},
    {0x002C, 0xC0A1},
    {0x002D, 0x0000},
    {0x002F, 0x0000},
    {0x0030, 0x00D0},
    {0x0031, 0x1004},
    {0x0032, 0x0000},
    {0x0033, 0x00CF},
    {0x0034, 0x02CF},
    {0x0038, 0x1486},
    {0x0039, 0x0F12},
    {0x003A, 0x25FF},
    {0x003B, 0x3457},
    {0x0045, 0x1486},
    {0x0046, 0x0F12},
    {0x0047, 0x25F9},
    {0x0048, 0x3457},
    {0x004C, 0x0000},
    {0x0050, 0x2007},
    {0x0051, 0x0000},
    {0x0055, 0x0000},
    {0x0058, 0x1A14},
    {0x0059, 0x00FF},
    {0x0066, 0x0060},
    {0x0068, 0x8100},
    {0x0069, 0x0000},
    {0x006A, 0x0083},
    {0x0071, 0x0011},
    {0x0072, 0x0160},
    {0x0073, 0x332C},
    {0x0074, 0x4502},
    {0x0076, 0x7040},
    {0x0077, 0x0000},
    {0x007F, 0x413F},
    {0x0080, 0x0430},
    {0x0081, 0x0008},
    {0x0082, 0x0C60},
    #endif
    
    #endif
	};

rt_inline struct nau88l25b_priv *to_nau88l25b_priv(struct audio_codec *codec)
{
    return HAL_CONTAINER_OF(codec, struct nau88l25b_priv, codec);
}

static rt_err_t es_wr_reg(struct rt_i2c_client *i2c_client,
                          uint16_t reg16, uint16_t data16)
{
#if HL_CODEC_SET
    return RT_EOK;  // LX
#else
    struct rt_i2c_msg msgs[1];
    uint8_t data_buf[4];
    rt_err_t ret;

    data_buf[0] = (reg16>>8) & 0xff;
    data_buf[1] = reg16 & 0xff;
    data_buf[2] = (data16>>8) & 0xff;
    data_buf[3] = data16 & 0xff;

    msgs[0].addr = i2c_client->client_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf = data_buf;
    msgs[0].len = 4;

    ret = rt_i2c_transfer(i2c_client->bus, msgs, 1);
    if (ret != 1)
    {
        rt_kprintf("ERR: %s: failed: (%d)\n", __func__, ret);
        return ret;
    }

    rt_thread_mdelay(10);

    return RT_EOK;
#endif
}

static rt_err_t es_rd_reg(struct rt_i2c_client *i2c_client,
                          uint16_t reg16, uint16_t *data16)
{
#if HL_CODEC_SET
    return RT_EOK;  // LX
#else
    struct rt_i2c_msg msgs[2];
    uint8_t data_buf_send[2];
    uint8_t data_buf_rev[2];
    rt_err_t ret;

    data_buf_send[0] = (reg16>>8) & 0xff;
    data_buf_send[1] = reg16 & 0xff;

    msgs[0].addr = i2c_client->client_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf = data_buf_send;
    msgs[0].len = 2;

    msgs[1].addr = i2c_client->client_addr;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf = data_buf_rev;
    msgs[1].len = 2;

    ret = rt_i2c_transfer(i2c_client->bus, msgs, 2);
    if (ret != 2)
    {
        rt_kprintf("ERR: %s: failed: (%d)\n", __func__, ret);
        return ret;
    }
    *data16 = ((uint16_t)data_buf_rev[0]<<8) | ((uint16_t)data_buf_rev[1]);

    return RT_EOK;
#endif
}

static rt_err_t es_pa_ctl(bool on)
{
    if (on)
    {
        HAL_GPIO_SetPinLevel(NAU88L25B_PA_GPIO, NAU88L25B_PA_PIN, GPIO_HIGH);
        rt_thread_mdelay(PA_CTL_DELAY_MS);
    }
    else
    {
        HAL_GPIO_SetPinLevel(NAU88L25B_PA_GPIO, NAU88L25B_PA_PIN, GPIO_LOW);
    }

    return RT_EOK;
}

static rt_err_t es_update_bits(struct rt_i2c_client *i2c_client,
                               uint16_t reg, uint8_t mask, uint8_t val)
{
    uint16_t old, new;
    int ret;

    ret = es_rd_reg(i2c_client, reg, &old);
    if (ret < 0)
        goto err;

    new = (old & ~mask) | (val & mask);
    ret = es_wr_reg(i2c_client, reg, new);

err:
    return ret;
}

static rt_err_t nau88l25b_set_gain(struct audio_codec *codec,
                                      eAUDIO_streamType stream,
                                      struct AUDIO_DB_CONFIG *db_config)
{
    struct nau88l25b_priv *nau88l25b = to_nau88l25b_priv(codec);
    uint16_t reg_gain = 0x00;

    RT_ASSERT(db_config);

    if(db_config->dB < -103) { 
        reg_gain = 0x00; // 小于103则默认mute
    } else {
        reg_gain = db_config->dB * 2 + 207;
    }
    
    es_wr_reg(nau88l25b->i2c_client, UAN88L25B_REG_DACL_CTRL, reg_gain);
    es_wr_reg(nau88l25b->i2c_client, UAN88L25B_REG_DACR_CTRL, reg_gain);

    return RT_EOK;
}

static rt_err_t nau88l25b_get_gain(struct audio_codec *codec,
                                      eAUDIO_streamType stream,
                                      struct AUDIO_DB_CONFIG *db_config)
{
    struct nau88l25b_priv *nau88l25b = to_nau88l25b_priv(codec);
    unsigned short val, volume;

    RT_ASSERT(db_config);

    es_rd_reg(nau88l25b->i2c_client, UAN88L25B_REG_DACL_CTRL, &val);
    es_rd_reg(nau88l25b->i2c_client, UAN88L25B_REG_DACR_CTRL, &val);
    volume = val >> 8;

    /* The min volume is -100dB, register value is 200. */
    if (volume > 200)
        volume = 200;

    db_config->dB = (volume - 207) / 2;

    return RT_EOK;
}


static rt_err_t nau88l25b_reg_init(struct nau88l25b_priv *nau88l25b)
{
    rt_err_t ret = RT_EOK;

    /* config codec */
    uint8_t i;
	uint8_t data;
	NAU88L25B_REG_T *nau88l25b_param = NULL; 
	nau88l25b_param = &s_nau88l25b_param[0]; 

	rt_kprintf("NAU88L25B start init config (%d)\r\n", (sizeof(s_nau88l25b_param)/2));
	for(i = 0; i < (sizeof(s_nau88l25b_param)/4); i++) {
        ret |= es_wr_reg(nau88l25b->i2c_client, nau88l25b_param->reg_addr, nau88l25b_param->reg_value);
		if (ret != RT_EOK) {
			rt_kprintf("NAU88L25B error\r\n");
			return ret;
		}
		nau88l25b_param++;
	}
	rt_kprintf("NAU88L25B end init config\r\n");
    
    return ret;
}

static rt_err_t nau88l25b_codec_standby(struct nau88l25b_priv *nau88l25b)
{
    rt_err_t ret = RT_EOK;

    return ret;
}

static rt_err_t nau88l25b_init(struct audio_codec *codec, struct AUDIO_INIT_CONFIG *config)
{
    rt_kprintf("nau88l25b_init\n");
    return RT_EOK;
}

static rt_err_t nau88l25b_deinit(struct audio_codec *codec)
{    
    return RT_EOK;
}

static rt_err_t nau88l25b_set_clk(struct audio_codec *codec, eAUDIO_streamType stream, uint32_t freq)
{
    return RT_EOK;
}

static rt_err_t nau88l25b_config(struct audio_codec *codec, eAUDIO_streamType stream, struct AUDIO_PARAMS *params)
{

    struct nau88l25b_priv *nau88l25b = to_nau88l25b_priv(codec);
    int wl;
    rt_err_t ret = RT_EOK;

    /* Vendor suggest  that after bclk output and init codec */
    if (nau88l25b->work_cnt <= 0)
        ret |= nau88l25b_reg_init(nau88l25b);
#if 0
    switch (params->sampleBits)
    {
    case 16:
        wl = 3;
        break;
    case 18:
        wl = 2;
        break;
    case 20:
        wl = 1;
        break;
    case 24:
        wl = 0;
        break;
    case 32:
        wl = 4;
        break;
    default:
        return -RT_EINVAL;
    }

    ret |= es_update_bits(nau88l25b->i2c_client, NAU88L25B_SDPIN_REG09,
                          NAU88L25B_SDPIN_REG09_DACWL_MASK,
                          wl << NAU88L25B_SDPIN_REG09_DACWL_SHIFT);

    ret |= es_update_bits(nau88l25b->i2c_client, NAU88L25B_SDPOUT_REG0A,
                          NAU88L25B_SDPOUT_REG0A_ADCWL_MASK,
                          wl << NAU88L25B_SDPOUT_REG0A_ADCWL_SHIFT);

    if (stream == AUDIO_STREAM_PLAYBACK)
        es_pa_ctl(1);
#endif
    if (ret != RT_EOK)
        rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);

    return ret;
}

static rt_err_t nau88l25b_start(struct audio_codec *codec, eAUDIO_streamType stream)
{
    struct nau88l25b_priv *nau88l25b = to_nau88l25b_priv(codec);
	//rt_kprintf("111\n");
    rt_mutex_take(&lock, RT_WAITING_FOREVER);
    nau88l25b->work_cnt++;
    rt_mutex_release(&lock);
	//rt_kprintf("222\n");
    return RT_EOK;
}

static rt_err_t nau88l25b_stop(struct audio_codec *codec, eAUDIO_streamType stream)
{
    struct nau88l25b_priv *nau88l25b = to_nau88l25b_priv(codec);
    rt_err_t ret = RT_EOK;

	//rt_kprintf("333\n");
    rt_mutex_take(&lock, RT_WAITING_FOREVER);
    if (nau88l25b->work_cnt)
        nau88l25b->work_cnt--;
    rt_mutex_release(&lock);
	//rt_kprintf("444\n");
    if (nau88l25b->work_cnt <= 0)
        nau88l25b_codec_standby(nau88l25b);
    if (stream == AUDIO_STREAM_PLAYBACK)
        es_pa_ctl(0);


    return ret;
}

static const struct audio_codec_ops nau88l25b_ops =
{
    .init = nau88l25b_init,
    .deinit = nau88l25b_deinit,
    .set_clk = nau88l25b_set_clk,
    .config = nau88l25b_config,
    .start = nau88l25b_start,
    .stop = nau88l25b_stop,
};

static int misc_prepare(void)
{
    return RT_EOK;
}

int rt_hw_codec_nau88l25b_init(void)
{
    struct codec_desc *codec_dev = (struct codec_desc *)&codec_nau88l25b;
    struct nau88l25b_priv *nau88l25b;
    rt_err_t ret = RT_EOK;
    uint8_t chip_id1;
    uint8_t chip_id2;
    
    misc_prepare();
    es_pa_ctl(0);

    nau88l25b = rt_calloc(1, sizeof(struct nau88l25b_priv));
    RT_ASSERT(nau88l25b);
    nau88l25b->i2c_client = rt_calloc(1, sizeof(struct rt_i2c_client));
    RT_ASSERT(nau88l25b->i2c_client);
    rt_mutex_init(&lock, "lock", RT_IPC_FLAG_FIFO);

    nau88l25b->i2c_client->bus = rt_i2c_bus_device_find(codec_dev->i2c_bus);
    RT_ASSERT(nau88l25b->i2c_client->bus);
    nau88l25b->i2c_client->client_addr = codec_dev->i2c_addr;
    nau88l25b->codec.ops = &nau88l25b_ops;
    nau88l25b->codec.id = (uint32_t)codec_dev;
    nau88l25b->work_cnt = 0;
    
    ret |= rk_audio_register_codec(&nau88l25b->codec);
    if (ret != RT_EOK)
    {
        rt_kprintf("ERR: %s, something wrong: %d\n", __func__, ret);
        goto err;
    }

    /* Vendor suggest  that after bclk output and init codec lx*/
    if (nau88l25b->work_cnt <= 0)
        ret |= nau88l25b_reg_init(nau88l25b);

    rt_kprintf("TAG: register codec nau88l25b success\n");
    return RT_EOK;

err:
    rt_kprintf("ERR: %s, register codec nau88l25b failed: %d\n", __func__, ret);
    if (nau88l25b)
        rt_free(nau88l25b);
    return ret;
}

INIT_DEVICE_EXPORT(rt_hw_codec_nau88l25b_init);

#endif
