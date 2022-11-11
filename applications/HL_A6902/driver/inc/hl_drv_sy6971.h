#ifndef __HL_DRV_SY6971_H
#define __HL_DRV_SY6971_H

#include "stdint.h"


/**************************************************************************
 *                                基本宏                                   *
 **************************************************************************/

/* 函数返回状态 */
#define HL_SUCCESS 0
#define HL_FAILED 1

/* 使用I2C */
#define SY6971_IIC_NAME "i2c0"
#define SY6971_DEVICE_ADDRESS 0x6B

/* 寄存器地址 */
#define SY_REG00_ADDR 0x00
#define SY_REG01_ADDR 0x01
#define SY_REG02_ADDR 0x02
#define SY_REG03_ADDR 0x03
#define SY_REG04_ADDR 0x04
#define SY_REG05_ADDR 0x05
#define SY_REG06_ADDR 0x06
#define SY_REG07_ADDR 0x07
#define SY_REG08_ADDR 0x08
#define SY_REG09_ADDR 0x09
#define SY_REG0A_ADDR 0x0A
#define SY_REG0B_ADDR 0x0B
#define SY_REG0C_ADDR 0x0C
#define SY_REG0D_ADDR 0x0D
#define SY_REG0E_ADDR 0x0E
#define SY_REG0F_ADDR 0x0F
#define SY_REG10_ADDR 0x10
#define SY_REG11_ADDR 0x11
#define SY_REG12_ADDR 0x12

#define SY_REG_NUM    0x13

/* 读写操作 */
#define SY_READ_CMD   0x00
#define SY_WRITE_CMD  0x01

/**************************************************************************
 *                               功能选项参数宏                            *
 **************************************************************************/



/**************************************************************************
 *                                功能选项枚举                             *
 **************************************************************************/

enum hl_sy6971_param
{
    /*  可读写 */

    //reg 0x00
    E_IINLIM = 0,
    E_LDO_MODE2,
    E_LDO_MODE1,
    E_EN_HIZ,

    //reg 0x01
    E_OTG_BAT,
    E_SYS_MIN,
    E_CHG_CONFIG,
    E_OTG_CONFIG,
    E_WD_RST,
    E_PFM_DIS,

    //reg 0x02
    E_ICHG,
    E_BATSNS_DIS,

    //reg 0x03
    E_IPRECHG,
    E_IINDPM_INT_MASK,
    E_VINDPM_INT_MASK,

    //reg 0x04
    E_ITERM,
    E_Reserved_1,
    E_ISHORT,

    //reg 0x05
    E_VRECHG,
    E_EN_TERM,
    E_VREG,

    //reg 0x06
    E_JEITA_ISET,
    E_JEITA_VSET,
    E_TREG,
    E_TMR2X_EN,
    E_CHG_TIMER,
    E_EN_TIMER,
    E_WATCHDOG,

    //reg 0x07
    E_VINDPM,
    E_Q1_EN,
    E_NTC_JEITA,
    E_OVP,

    //reg 0x08
    E_VDPM_BAT_TRACK,
    E_BATFET_RST_EN,
    E_BATFET_DLY,
    E_BATFET_DIS,
    E_BCOLD,
    E_BHOT,

    //reg 0x09
    E_TBATFET_RST,
    E_TQON_RST,
    E_TSM_DLY,
    E_TSR_FAULT,

    //reg 0x0A
    E_IDPM_LIM,
    E_BOOST_LIM,
    E_NTC_TOFF,

    //reg 0x0B
    E_BOOSTV,
    E_SYSR_TIMER,
    E_SYSR_NTC,
    E_Reserved_2,
    E_LDO_HIZ_EN,

    //reg 0x0C
    E_VLDO1,
    E_ILDO1,

    //reg 0x0D
    E_VLDO2,
    E_ILDO2,

    //reg 0x0E
    E_TOPOFF_TIMER,
    E_Q1_FULLON,
    E_FORCE_AICL,
    E_AICL_EN,
    E_TQON_LOW,

    //reg 0x12
    E_DEV_REV,
    E_PN,
    E_BYPASS_EN,
    E_REG_RST,

    /* 只读 */

    //0x0F
    E_VSYS_STAT,
    E_THERM_STAT,
    E_PG_STAT,
    E_CHRG_STAT,
    E_BUS_STAT,

    //0x10
    E_LDO2_FAULT,
    E_LDO1_FAULT,
    E_ACOV_STAT,
    E_BAT_FAULT,
    E_CHRG_FAULT,
    E_BOOST_FAULT,
    E_WATCHDOG_FAULT,

    //0x11
    E_NTC_FAULT,
    E_AICL_OPTIMIZED,
    E_TOPOFF_ACTIVE,
    E_BATSNS_STAT,
    E_IDPM_STAT,
    E_VDPM_STAT,

    E_NUM_MAX
};

/**************************************************************************
 *                             数据类型                                    *
 **************************************************************************/

typedef struct _HL_SY_INPUT_PARAM_T
{
    uint8_t cfg_opt;//参数参考 ： hl_sy6971_param 枚举内容
    uint8_t param; //参数参考 ： 功能选项参数宏
}HL_SY_INPUT_PARAM_T;


// REG00 TYPE
typedef struct _HL_SY6971_REG00_T
{
    uint8_t IINLIM : 5;  // 100mA+100mA*[IINLIM]

    uint8_t LDO_MODE2 : 1; //

    uint8_t LDO_MODE1 : 1; //

    uint8_t EN_HIZ : 1;
}HL_SY6971_REG00_T;

// REG01 TYPE
typedef struct _HL_SY6971_REG01_T
{
    uint8_t OTG_BAT : 1;  //

    uint8_t SYS_MIN : 3; //

    uint8_t CHG_CONFIG : 1; //

    uint8_t OTG_CONFIG : 1;

    uint8_t WD_RST : 1;

    uint8_t PFM_DIS : 1;
}HL_SY6971_REG01_T;

// REG02 TYPE
typedef struct _HL_SY6971_REG02_T
{
    uint8_t ICHG : 7;  //

    uint8_t BATSNS_DIS : 1; //
}HL_SY6971_REG02_T;

// REG03 TYPE
typedef struct _HL_SY6971_REG03_T
{
    uint8_t IPRECHG : 6;  //IPRECHG = 8mA+[ IPRECHG]*8mA

    uint8_t IINDPM_INT_MASK : 1; //

    uint8_t VINDPM_INT_MASK : 1; //
}HL_SY6971_REG03_T;

// REG04 TYPE
typedef struct _HL_SY6971_REG04_T
{
    uint8_t ITERM : 5;  //ITERM = 8mA+[ITERM]*8mA

    uint8_t Reserved : 1; //

    uint8_t ISHORT : 2; //
}HL_SY6971_REG04_T;

// REG05 TYPE
typedef struct _HL_SY6971_REG05_T
{
    uint8_t VRECHG : 1;  //

    uint8_t EN_TERM : 1; //

    uint8_t VREG : 6; //VREG = 4.000V+[VREG]*10mV
}HL_SY6971_REG05_T;

// REG06 TYPE
typedef struct _HL_SY6971_REG06_T
{
    uint8_t JEITA_ISET : 1;  //

    uint8_t JEITA_VSET : 1; //

    uint8_t TREG : 1; //

    uint8_t TMR2X_EN : 1; //

    uint8_t CHG_TIMER : 1;

    uint8_t EN_TIMER : 1;

    uint8_t WATCHDOG : 2;
}HL_SY6971_REG06_T;

// REG07 TYPE
typedef struct _HL_SY6971_REG07_T
{
    uint8_t VINDPM : 4;  //VINDPM = 3.8V+[VINDPM]*100mV

    uint8_t Q1_EN : 1; //

    uint8_t NTC_JEITA : 1; //

    uint8_t OVP : 2; //
}HL_SY6971_REG07_T;

// REG08 TYPE
typedef struct _HL_SY6971_REG08_T
{
    uint8_t VDPM_BAT_TRACK : 2;  //

    uint8_t BATFET_RST_EN : 1; //

    uint8_t BATFET_DLY : 1; //

    uint8_t BATFET_DIS : 1; //

    uint8_t BCOLD : 1; //

    uint8_t BHOT : 2; //
}HL_SY6971_REG08_T;

// REG09 TYPE
typedef struct _HL_SY6971_REG09_T
{
    uint8_t TBATFET_RST : 1;  //

    uint8_t TQON_RST : 3; //

    uint8_t TSM_DLY : 2; //

    uint8_t TSR_FAULT : 2; //
}HL_SY6971_REG09_T;

// REG0A TYPE
typedef struct _HL_SY6971_REG0A_T
{
    uint8_t IDPM_LIM : 5;  //IDPM_LIM = 100mA+[IDPM_LIM] *100mA

    uint8_t BOOST_LIM : 1; //

    uint8_t NTC_TOFF : 2; //
}HL_SY6971_REG0A_T;

// REG0B TYPE
typedef struct _HL_SY6971_REG0B_T
{
    uint8_t BOOSTV : 4;  //BOOSTV = 4.5V+ [ BOOSTV] * 0.1V

    uint8_t SYSR_TIMER : 1; //

    uint8_t SYSR_NTC : 1; //

    uint8_t Reserved : 1; //

    uint8_t LDO_HIZ_EN : 1; //
}HL_SY6971_REG0B_T;

// REG0C TYPE
typedef struct _HL_SY6971_REG0C_T
{
    uint8_t VLDO1 : 6;  //VLDO1 = 1.6V+ [ VLDO1] * 0.1V

    uint8_t ILDO1 : 2; //
}HL_SY6971_REG0C_T;

// REG0D TYPE
typedef struct _HL_SY6971_REG0D_T
{
    uint8_t VLDO2 : 6;  //VLDO2 = 1.6V+ [ VLDO2] *0.1V

    uint8_t ILDO2 : 2; //
}HL_SY6971_REG0D_T;

// REG0E TYPE
typedef struct _HL_SY6971_REG0E_T
{
    uint8_t TOPOFF_TIMER : 4;  //ITOPOFF= [ITOPOFF_TIMER]*5min

    uint8_t Q1_FULLON : 1; //

    uint8_t FORCE_AICL : 1; //

    uint8_t AICL_EN : 1; //

    uint8_t TQON_LOW : 1; //
}HL_SY6971_REG0E_T;


// REG0F TYPE Read only
typedef struct _HL_SY6971_REG0F_T
{
    uint8_t VSYS_STAT : 1;  //

    uint8_t THERM_STAT : 1; //

    uint8_t PG_STAT : 1; //

    uint8_t CHRG_STAT : 2; //

    uint8_t BUS_STAT : 3; //
}HL_SY6971_REG0F_T;

// REG10 TYPE Read only
typedef struct _HL_SY6971_REG10_T
{
    uint8_t LDO2_FAULT : 1;  //

    uint8_t LDO1_FAULT : 1;  //

    uint8_t ACOV_STAT : 1; //

    uint8_t BAT_FAULT : 1; //

    uint8_t CHRG_FAULT : 2; //

    uint8_t BOOST_FAULT : 1; //

    uint8_t WATCHDOG_FAULT : 1; //
}HL_SY6971_REG10_T;

// REG11 TYPE Read only
typedef struct _HL_SY6971_REG11_T
{
    uint8_t NTC_FAULT : 3;  //

    uint8_t AICL_OPTIMIZED : 1;  //

    uint8_t TOPOFF_ACTIVE : 1; //

    uint8_t BATSNS_STAT : 1; //

    uint8_t IDPM_STAT : 1; //

    uint8_t VDPM_STAT : 1; //
}HL_SY6971_REG11_T;

// REG12 TYPE
typedef struct _HL_SY6971_REG12_T
{
    uint8_t DEV_REV : 2;  //

    uint8_t PN : 4;  //

    uint8_t BYPASS_EN : 1; //

    uint8_t REG_RST : 1; //
}HL_SY6971_REG12_T;

typedef struct _HL_SY6971_REGALL_T
{
    HL_SY6971_REG00_T reg00;
    HL_SY6971_REG01_T reg01;
    HL_SY6971_REG02_T reg02;
    HL_SY6971_REG03_T reg03;
    HL_SY6971_REG04_T reg04;
    HL_SY6971_REG05_T reg05;
    HL_SY6971_REG06_T reg06;
    HL_SY6971_REG07_T reg07;
    HL_SY6971_REG08_T reg08;
    HL_SY6971_REG09_T reg09;
    HL_SY6971_REG0A_T reg0A;
    HL_SY6971_REG0B_T reg0B;
    HL_SY6971_REG0C_T reg0C;
    HL_SY6971_REG0D_T reg0D;
    HL_SY6971_REG0E_T reg0E;
    HL_SY6971_REG0F_T reg0F;
    HL_SY6971_REG10_T reg10;
    HL_SY6971_REG11_T reg11;
    HL_SY6971_REG12_T reg12;
}HL_SY6971_REGALL_T;

uint8_t hl_drv_sy6971_init(void);
uint8_t hl_drv_sy6971_io_ctrl(uint8_t cmd, void* ptr, uint8_t len);

#endif