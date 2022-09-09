/**
 * @file hl_drv_sgm41518.h
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief sgm41518芯片驱动头文件
 * @version 1.0
 * @date 2022-09-02
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-02     <td>v1.0     <td>dujunjie     <td>内容
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

#ifndef __HL_DRV_SGM41518_H
#define __HL_DRV_SGM41518_H
#include "stdint.h"

/**************************************************************************
 *                                基本宏                                     *
 **************************************************************************/

/* 函数返回状态 */
#define HL_SUCCESS 0
#define HL_FAILED 1

/* 使用I2C */
#define SGM41518_IIC_NAME "i2c0"
#define SGM41518_DEVICE_ADDRESS 0x38

/* 寄存器地址 */
#define SGM_REG00_ADDR 0x00
#define SGM_REG01_ADDR 0x01
#define SGM_REG02_ADDR 0x02
#define SGM_REG03_ADDR 0x03
#define SGM_REG04_ADDR 0x04
#define SGM_REG05_ADDR 0x05
#define SGM_REG06_ADDR 0x06
#define SGM_REG07_ADDR 0x07
#define SGM_REG08_ADDR 0x08
#define SGM_REG09_ADDR 0x09
#define SGM_REG0A_ADDR 0x0A
#define SGM_REG0B_ADDR 0x0B
#define SGM_REG0C_ADDR 0x0C
#define SGM_REG0D_ADDR 0x0D
#define SGM_REG0E_ADDR 0x0E
#define SGM_REG0F_ADDR 0x0F

#define SGM_REG_NUM    16

/* 读写操作 */
#define SGM_READ_CMD   0x00
#define SGM_WRITE_CMD  0x01

/**************************************************************************
 *                               功能选项参数宏                            *
 **************************************************************************/
/*
    因为部分寄存器的Enable 由 1 表示，部分Enable由 0 表示，所以在此使用宏定义统一
*/
//reg 00
#define HIZ_ENABLE 1
#define HIZ_DISABLE 0

#define FOLLOE_CHARG_STAT_ENABLE 0  //STAT引脚状态由充电状态决定
#define FOLLOE_STAT_SET_ENABLE 1    //STAT引脚状态由STAT_SET[1:0]决定
#define STAT_DISABLE 3              //禁用STAT引脚功能

#define IINDPM_SET(x) (x > 31) ? (31) : (x)  //IINDPM[4:0] 输入电流限制,参数 n <= 31 , Ilimit = 100mA + (100 * n)mA

//reg 01
#define PFM_ENABLE 0
#define PFM_DISABLE 1

#define WDG_TIMEOUT 0  //看门狗超时
#define WDG_FEED 1     //喂狗

#define OTG_ENABLE 1   //启用OTG
#define OTG_DISABLE 0  //禁用OTG

#define CHARGE_ENABLE 1   //使能充电
#define CHARGE_DISABLE 0  //禁用充电

#define SYS_MIN_2V6 0  //设置最小系统电压为2.6V
#define SYS_MIN_2V8 1  //设置最小系统电压为2.8V
#define SYS_MIN_3V0 2  //设置最小系统电压为3.0V
#define SYS_MIN_3V2 3  //设置最小系统电压为3.2V
#define SYS_MIN_3V4 4  //设置最小系统电压为3.4V
#define SYS_MIN_3V5 5  //设置最小系统电压为3.5V
#define SYS_MIN_3V6 6  //设置最小系统电压为3.6V
#define SYS_MIN_3V7 7  //设置最小系统电压为3.7V

#define MIN_BOOST_VOLTAGE_2v95 0  //Boost模式下最小电池电压2.95V
#define MIN_BOOST_VOLTAGE_2v6 1   //Boost模式下最小电池电压2.6V

//reg 02
#define Q1_USE_HIGHER_RDSON 0  //高精度检测（INDPM < 700 mA）
#define Q1_USE_LOWER_RDSON 1   //高效率检测（Q1完全开启）

#define FAST_CHARGE_CURRENT_SET(x) (x > 63) ? (63) : (x)  //ICHG[5:0] 快充电流限制,参数 n <= 63 , ICHG = (20 * n)mA

//reg 03
#define PRE_CHARGE_SET(x) (x > 12) ? (12) : (x)  //Pre-charge[7:4] 预充电电流，参数 n <= 12 , I = 20mA + (20 * n)mA
#define TER_CURRENT_SET(x) (x > 15) ? (15) : (x)  //TER_CURRENT[3:0] 终止电流，参数 n <= 12 , I = 20mA + (20 * n)mA

//reg 04
#define CHARGE_VOLTAGE_LIMIT(x)   ((x > 24) & (x != 15)) ? (15) : (x)  //VREG[7:3] 充电电流限制，参数 n <= 24 & n != 15 
                                                                        // V = 3856mV + (32 * n)mV,n = 15,V = 4352mV
#define TOP_TIMER_DISABLE 0       //关闭充电延长时间
#define TOP_TIMER_15MIN 1         //充电延长时间15分钟
#define TOP_TIMER_30MIN 2         //充电延长时间30分钟
#define TOP_TIMER_45MIN 3         //充电延长时间45分钟

#define RECHARGE_THRESHOLD_100 0  //电池充电阈值 VREG - 100mV
#define RECHARGE_THRESHOLD_200 1  //电池充电阈值 VREG - 200mV

//reg 05
#define CHARGE_TER_ENABLE 1   //充电终止使能
#define CHARGE_TER_DISABLE 0  //充电终止失能

#define WDG_TIMER_DISABLE 0   //禁用看门狗
#define WDG_TIMER_SET_40S 1   //看门狗复位时间40秒
#define WDG_TIMER_SET_80S 2   //看门狗复位时间80秒
#define WDG_TIMER_SET_160S 3  //看门狗复位时间160秒

#define CHARGE_SAFET_TIMER_ENABLE 1   //使能充电安全计数器
#define CHARGE_SAFET_TIMER_DISABLE 0  //失能充电安全计数器

#define CHARGE_SAFET_TIMER_SET_20H 0  //安全计数器设置20小时
#define CHARGE_SAFET_TIMER_SET_10H 1  //安全计数器设置0小时

#define THERNAL_REGT_THRESHOLD_80 0   //buck模式热调节阈值80℃
#define THERNAL_REGT_THRESHOLD_120 1  //buck模式热调节阈值120℃

#define JEITA_CURRENT_SET_L_50 0  //低温区间（0℃ - 10℃）充电电流 I = 50% * ICHG
#define JEITA_CURRENT_SET_L_20 1  //低温区间（0℃ - 10℃）充电电流 I = 20% * ICHG

//reg 06
#define VAC_OVP_THRESHOLD_5V5 0   //OVP输入电压阈值5.5V
#define VAC_OVP_THRESHOLD_6V5 1   //OVP输入电压阈值6.5V
#define VAC_OVP_THRESHOLD_10V5 2  //OVP输入电压阈值10.5V
#define VAC_OVP_THRESHOLD_14V 3   //OVP输入电压阈值14V

#define BOOST_VOL_THRESHOLD_4V85 0  //boost模式调压设置4.85V
#define BOOST_VOL_THRESHOLD_5V 1    //boost模式调压设置5V
#define BOOST_VOL_THRESHOLD_5V15 2  //boost模式调压设置5.15V
#define BOOST_VOL_THRESHOLD_5V30 3  //boost模式调压设置5.30V

#define VINDPM_THRESHOLD(x) \
    (x > 15) ? (15) : (x)  //VINDPM[3:0] VINDPM电压阈值，参数 n <= 15 , V = VINDPM_OS + (0.1 * n)mV

//reg 07
#define IINDET_ENABLE 1   //使能输入电流检测
#define IINDET_DISABLE 0  //失能输入电流检测

#define TMR2X_ENABLE 1   //使能半时钟安全计数器
#define TMR2X_DISABLE 1  //失能半时钟安全计数器

#define BATFET_ENABLE 0   //使能关闭BATFET
#define BATFET_DISABLE 1  //失能关闭BATFET

#define JEITA_VSET_H_SET_4V1 0   //设置高温区间（45℃ - 60℃）充电电压4.1V
#define JEITA_VSET_H_SET_VREG 1  //设置高温区间（45℃ - 60℃）充电电压等于VREG

#define BATFET_TSM_DLY_ENABLE 1   //关闭BATFET时，延时Tsm_dly时间后再关闭
#define BATFET_TSM_DLY_DISABLE 0  //关闭BATFET时，立即关闭

#define BATFET_RST_ENABLE 1   //使能BATFET重置功能
#define BATFET_RST_DISABLE 0  //失能BATFET重置功能

#define VDPM_BAT_TRACK_DISABLE 0  //禁止VINDPM
#define VDPM_BAT_TRACK_200 1      //Vbat + 200mV
#define VDPM_BAT_TRACK_250 2      //Vbat + 250mV
#define VDPM_BAT_TRACK_300 3      //Vbat + 300mV

//reg 08
#define VBUS_STAT_NO_INPUT 0       //无输入
#define VBUS_STAT_USB_INPUT 1      //USB供电
#define VBUS_STAT_ADAPTER_INPUT 3  //适配器供电
#define VBUS_STAT_OTG_MOD 7        //OTG模式

#define CHARGE_DISABLE 0       //充电禁止
#define PRE_CHARGE 1           //预充电
#define FAST_CHARGING 2        //快速充电
#define CHARGING_TERMINATED 3  //充电终止

#define INPUT_POWER_SOURCE_GOOD 1      //正常输入源
#define INPUT_POWER_SOURCE_NOT_GOOD 0  //异常输入源

#define NOT_THERMAL_REGULATION 0  //未使用热调节中
#define THERMAL_REGULATION 1      //使用热调节

#define NOT_VSYSMIN_REGULATION 0  //未使用系统电压调节
#define VSYSMIN_REGULATION 1      //使用系统电压调节

//reg 09
#define WDG_NORMAL 0         //看门狗正常工作
#define WDG_TIMER_EXPIRED 1  //定时器过期

#define BOOST_NORMAL 0    //Boost正常工作
#define BOOST_ABNORMAL 1  //Boost异常

#define CHARGING_NORMAL 0                //正常充电
#define CHARGING_INPUT_FAULT 1           //输入异常
#define CHARGING_THERMAL_SHUTDOWN 2      //热关机
#define CHARGING_SAFETY_TIMER_EXPIRED 3  //安全计数器超时

#define BATTERY_NORMAL 0        //电池正常
#define BATTERY_OVER_VOLTAGE 1  //电池过压

#define NTC_NORMAL 0  //温度正常
#define NTC_WARM 2    //温暖
#define NTC_COOL 3    //冷
#define NTC_COLD 5    //寒冷
#define NTC_HOT 0x0C  //高温

//reg 0A
#define GOOD_VBUS_IS_NOT_ATTACHED 0  //输入源异常
#define GOOD_VBUS_IS_ATTACHED 1      //输入源正常

#define NOT_USE_VINDPM 0  //未使用VINDPM
#define USE_VINDPM 1      //使用VINDPM

#define NOT_USE_IINDPM 0  //未使用IINDPM
#define USE_IINDPM 1      //使用IINDPM

#define TOP_OFF_TIMER_NOT_COUNTING 0  //top-off不计数
#define TOP_OFF_TIMER_COUNTING 1      //top-off计数

#define NO_OVER_VOLTAGE 0        //没有过压（no ACOV）
#define OVER_VOLTAGE_DETECTED 1  //检测到过压（ACOV）

#define ALLOW_VINDPM_INT 0  //允许VINDPM触发INT引脚
#define MASK_VINDPM_INT 1   //禁止VINDPM触发INT引脚

#define ALLOW_IINDPM_INT 0  //允许IINDPM触发INT引脚
#define MASK_IINDPM_INT 1   //禁止IINDPM触发INT引脚

//reg 0B
#define RESET_NO_EFFECT 0  //保持当前寄存器设置
#define RESET_ALL_REG 1    //重置所有寄存器

#define SGM41518 0x0C  //设备号
#define SGM41519 0x0D

//reg 0C
#define JEITA_VSET_L_SET_4V1 1   //设置低温区间（0℃ - 10℃）充电电压4.1V
#define JEITA_VSET_L_SET_VREG 0  //设置低温区间（0℃ - 10℃）充电电压等于VREG

#define JEITA_ISET_L_ENABLE 1   //允许低温区间（0℃ - 10℃）充电
#define JEITA_ISET_L_DISABLE 0  //禁止低温区间（0℃ - 10℃）充电

#define JEITA_WARM_ISET_0 0    //温暖区间（45℃ - 60℃）充电电流设置 I = ICHG * 0%
#define JEITA_WARM_ISET_20 1   //温暖区间（45℃ - 60℃）充电电流设置 I = ICHG * 20%
#define JEITA_WARM_ISET_50 2   //温暖区间（45℃ - 60℃）充电电流设置 I = ICHG * 50%
#define JEITA_WARM_ISET_100 3  //温暖区间（45℃ - 60℃）充电电流设置 I = ICHG * 100%

#define JEITA_COOL_VT2_5C5 0  //寒冷区间阈值 5.5℃
#define JEITA_COOL_VT2_10C 1  //寒冷区间阈值 10℃
#define JEITA_COOL_VT2_15C 2  //寒冷区间阈值 15℃
#define JEITA_COOL_VT2_20C 3  //寒冷区间阈值 20℃

#define JEITA_WARM_VT3_40C 0   //温暖区间阈值 40℃
#define JEITA_WARM_VT3_44C5 1  //温暖区间阈值 44.5℃
#define JEITA_WARM_VT3_50C5 2  //温暖区间阈值 50.5℃
#define JEITA_WARM_VT3_54C5 3  //温暖区间阈值 54.5℃

//reg 0D
#define JEITA_ENABLE 0   //使能JEITA
#define JEITA_DISABLE 1  //失能JEITA

//reg 0F
#define VREG_FINE_TUNING_DISABLE 0   //禁止微调VREG
#define VREG_FINE_TUNING_ADD_8MV 1   //VREG + 8mV
#define VREG_FINE_TUNING_SUB_8MV 2   //VREG - 8mV
#define VREG_FINE_TUNING_SUB_16MV 3  //VREG - 16mV

#define STAT_PIN_OFF 0               //引脚高阻态
#define STAT_PIN_ON 1                //引脚低电平
#define STAT_PIN_BLINK_1SON_1SOFF 2  //引脚拉低一秒拉高一秒
#define STAT_PIN_BLINK_1SON_3SOFF 3  //引脚拉低一秒拉高三秒

#define VINDPM_OS_SET_3V9 0   //设置VINDPM Offset为3.9V
#define VINDPM_OS_SET_5V9 1   //设置VINDPM Offset为5.9V
#define VINDPM_OS_SET_7V5 2   //设置VINDPM Offset为7.5V
#define VINDPM_OS_SET_10V5 3  //设置VINDPM Offset为10.5V

/**************************************************************************
 *                                功能选项枚举                             *
 **************************************************************************/

/* 可读写 */
enum hl_rw18_param
{
    // reg 00
    RW_EN_HIZ_MOD = 0,
    RW_EN_ICHG_MON,
    RW_IINDPM_SET,

    // reg 01
    RW_EN_PFM,
    RW_WDG_RST,  //喂狗
    RW_EN_OTG,
    RW_EN_BAT_CHARGING,
    RW_SYS_MIN,
    RW_MIN_BAT_SEL,

    // reg 02
    RW_Q1_FULLON,
    RW_CHARGE_CURRENT,  //充电电流

    // reg 03
    RW_PRECHARGE_CURRENT_LIMIT,  //预充电电流限制
    RW_TER_CURRENT_LIMIT,        //终止电流

    // reg 04
    RW_CHARGE_VOLTAGE_LIMIT,  //充电电压限制
    RW_TOPOFF_TIME,           //充电延长时间
    RW_RECHARGE_THRESHOLD,    //电池充电阈值

    // reg 05
    RW_CHARGE_TER_ENABLE,           //充电终止使能
    RW_WDOG_TIMER_CHG,              //看门狗设置
    RW_CHARGE_SAFETY_TIMER_ENABLE,  //充电安全时间使能
    RW_CHARGE_SAFETY_TIMER_CHG,     //充电安全时间设置
    RW_TER_REGULATION_THRESHOLD,    //buck热调节阈值
    RW_JEITA_CHARGING_CURRENT,      //低温充电电流设置

    // reg 06
    RW_OVP_CHG,     //输入电源的OVP阈值
    RW_BOOSTV_CHG,  //boost模式电压检测设置
    RW_VINDPM_CHG,  //输入电压阈值设置

    // reg 07
    RW_INPUT_CRRENT_LIMIT_DETECTION,  //输入电流限制检测
    RW_TMR2X_EN,                      //使能半时钟安全计时器
    RW_BATFET_DIS,                    //禁用BATFET
    RW_JEITA_VSET_H,                  //JEITA充电电压设置（45℃-60℃）
    RW_BATFET_DLY,                    //关闭BATFET后延时设置
    RW_BATFET_RST_EN,                 //使能BATFET重置功能
    RW_VDPM_BAT_TRACK,                //动态跟踪VINDPM

    // reg 0A
    RW_VINDPM_INT_MASK,  //VINDPM中断事件掩码
    RW_IINDPM_INT_MASK,  //IINDPM中断事件掩码

    // reg 0B
    RW_REG_RST,  //芯片寄存器复位

    // reg 0C
    RW_JEITA_VSET_L,     //JEITA充电电压设置（0℃-10℃）
    RW_JEITA_ISET_L_EN,  //使能低温充电功能（0℃-10℃）
    RW_JEITA_ISET_H,     //高温充电电流设置（45℃-60℃）
    RW_JEITA_VT2,        //JEITA低温区间判定阈值
    RW_JEITA_VT3,        //JEITA高温区间判定阈值

    // reg 0D
    RW_JEITA_EN,  //JEITA使能

    // reg 0F
    RW_VREG_FT,    //微调VREG参数
    RW_STAT_SET,   //STAT引脚状态设置
    RW_VINDPM_OS,  //VINDPM偏移设置

    RW_CNT_MAX
};

/*  只读 */
enum hl_r18_param
{
    // reg 08
    R_VBUS_STAT = RW_CNT_MAX,  //VBUS状态寄存器
    R_CHRG_STAT,               //充电状态
    R_PG_STAT,                 //输入电源状态
    R_THERM_STAT,              //热调节状态
    R_VSYS_STAT,               //系统电压调节状态

    // reg 09
    R_WDG_FAULT,    //看门狗异常状态
    R_BOOST_FAULT,  //BOOST模块异常状态
    R_CHRG_FAULT,   //充电故障状态
    R_BAT_FAULT,    //电池故障状态
    R_NTC_FAULT,    //电池温度状态

    // reg 0A
    R_VBUS_GD,        //良好输入源检测
    R_VINDPM_STAT,    //输入电压调节功能状态
    R_IINDPM_STAT,    //输入电流调节功能状态
    R_TOPOFF_ACTIVE,  //top-off计数器状态
    R_ACOV_STAT,      //输入过压状态

    // reg 0B
    R_DEVICE_ID,  //设备ID

    R_CNT_MAX
};

/**************************************************************************
 *                             数据类型                                    *
 **************************************************************************/
// 参数选项
typedef struct _HL_SGM_INPUT_PARAM_T
{
    uint8_t reg;
    uint8_t param;
}HL_SGM_INPUT_PARAM_T;


// REG00 TYPE
typedef struct _HL_SGM41518_REG00_T
{
    uint8_t IINDPM : 5;  // Input Current Limit Value (n: 5 bits):= 100 + 100n (mA) Default: 2400mA (10111), not typical
                         // IINDPM[4]  	1 = 1600mA
                         // IINDPM[3]	1 = 800mA
                         // IINDPM[2]	1 = 400mA
                         // IINDPM[1]	1 = 200mA
                         // IINDPM[0]	1 = 100mA

    uint8_t EN_ICHG_MON : 2;  // Enable STAT Pin Function
                              // 00 = Enable (default)
                              // 01 = Reserved
                              // 10 = Reserved
                              // 11 = Disable (float pin)

    uint8_t EN_HIZ : 1;  // Enable HIZ Mode
                         // 0 = Disable (default)
                         // 1 = Enable
} HL_SGM41518_REG00_T;

// REG01 TYPE
typedef struct _HL_SGM41518_REG01_T
{
    uint8_t MIN_BAT_SEL : 1;  // Minimum Battery Voltage for OTG Mode
                              // 0 = 2.8V VBAT falling (default)
                              // 1 = 2.5V VBAT falling

    uint8_t SYS_MIN : 3;  // Minimum System Voltage
                          // 000 = 2.6V	001 = 2.8V	 010 = 3V	011 = 3.2V
                          // 100 = 3.4V	101 = 3.5V (default)  110 = 3.6V  111 = 3.7V

    uint8_t CHG_CONFIG : 1;  // Enable Battery Charging
                             // 0 = Charge disable
                             // 1 = Charge enable (default)

    uint8_t OTG_CONFIG : 1;  // Enable OTG
                             // 0 = OTG disable (default)
                             // 1 = OTG enable

    uint8_t WD_RST : 1;  // I2C Watchdog Timer Reset
                         // 0 = Normal (default)
                         // 1 = Reset

    uint8_t PFM_DIS : 1;  // Enable PFM Mode
                          // 0 = Enable (default)
                          // 1 = Disable
} HL_SGM41518_REG01_T;

// REG02 TYPE
typedef struct _HL_SGM41518_REG02_T
{
    uint8_t ICHG : 6;  // Fast Charge Current Value (n: 6 bits): = 60n (mA) (n ≤ 50) Default: 2040mA (100010)
                       // ICHG[5] 1 = 1920mA
                       // ICHG[4] 1 = 960mA
                       // ICHG[3] 1 = 480mA
                       // ICHG[2] 1 = 240mA
                       // ICHG[1] 1 = 120mA
                       // ICHG[0] 1 = 60mA

    uint8_t Q1_FULLON : 1;  // VBUS FET SWITCH (Q1)
                            // 0 = Use higher R-DSON if I-INDPM < 700mA (for better accuracy)
                            // 1 = Use lower R-DSON always (fully ON for better efficiency)

    uint8_t BOOST_LIM : 1;  // Boost Mode Current Limit
                            // 0 = 0.5A
                            // 1 = 1.2A (default)
} HL_SGM41518_REG02_T;

// REG03 TYPE
typedef struct _HL_SGM41518_REG03_T
{
    uint8_t ITERM : 4;  // Termination Current Limit (n: 4 bits):= 60 + 60n (mA)  Default: 180mA (0010)
                        // ITERM[3] 1 = 480mA
                        // ITERM[2] 1 = 240mA
                        // ITERM[1] 1 = 120mA
                        // ITERM[0] 1 = 60mA

    uint8_t IPRECHG : 4;  // Pre-Charge Current Limit (n: 4 bits):= 60 + 60n (mA) (n ≤ 12) Default: 180mA (0010)
                          // IPRECHG[3] 1 = 480mA
                          // IPRECHG[2] 1 = 240mA
                          // IPRECHG[1] 1 = 120mA
                          // IPRECHG[0] 1 = 60mA
} HL_SGM41518_REG03_T;

// REG04 TYPE
typedef struct _HL_SGM41518_REG04_T
{
    uint8_t VRECHG : 1;  // Battery Recharge Threshold
                         // 0 = 100mV below VREG[4:0] (default)
                         // 1 = 200mV below VREG[4:0]

    uint8_t TOPOFF_TIMER : 2;  // Top-Off Timer(The charge extension time added after the termination condition is detected.)
                           // 00 = Disabled (default)
                           // 01 = 15 minutes
                           // 10 = 30 minutes
                           // 11 = 45 minutes

    uint8_t VREG : 5;  // Charge Voltage Limit (n: 5 bits): Default: 4.208V (01011)
                       //	 = 3856 + 32n (mV) if n ≤ 24, n≠15;
                       //	 = 4.352V if n = 15
                       // VREG[4]	1 = 512mV
                       // VREG[3]	1 = 256mV
                       // VREG[2]	1 = 128mV
                       // VREG[1]	1 = 64mV
                       // VREG[0]	1 = 32mV
} HL_SGM41518_REG04_T;

// REG05 TYPE
typedef struct _HL_SGM41518_REG05_T
{
    uint8_t JEITA_ISET : 1;  // JEITA Charging Current(0℃ - 10℃)
                             // 0 = 50% of ICHG
                             // 1 = 20% of ICHG (default)

    uint8_t TREG : 1;  // Thermal Regulation Threshold
                       // 0 = 80℃
                       // 1 = 120℃ (default)

    uint8_t CHG_TIMER : 1;  // Charge Safety Timer Setting
                            // 0 = 4hrs
                            // 1 = 6hrs (default)

    uint8_t EN_TIMER : 1;  // Charge Safety Timer Enable
                           // 0 = Disable
                           // 1 = Enable (default)

    uint8_t WATCHDOG : 2;  // Watchdog Timer Setting
                           // 00 = Disable watchdog timer
                           // 01 = 40s (default)
                           // 10 = 80s
                           // 11 = 160s

    uint8_t Reserved : 1;  // Reserved

    uint8_t EN_TERM : 1;  // Charging Termination Enable
                          // 0 = Disable
                          // 1 = Enable (default)
} HL_SGM41518_REG05_T;

// REG06 TYPE
typedef struct _HL_SGM41518_REG06_T
{
    uint8_t VINDPM : 4;  // VINDPM Threshold (n: 4 bits): = 3.9V + 0.1n (V) Default: 4.5V (0110)
                         // VINDPM[3]	1 = 800mV
                         // VINDPM[2]	1 = 400mV
                         // VINDPM[1]	1 = 200mV
                         // VINDPM[0]	1 = 100mV

    uint8_t BOOSTV : 2;  // Boost Mode Voltage Regulation
                         // 00 = 4.85V
                         // 01 = 5.00V
                         // 10 = 5.15V (default)
                         // 11 = 5.30V

    uint8_t OVP : 2;  // VAC Pin OVP Threshold
                      // 00 = 5.5V
                      // 01 = 6.5V (5V input) (default)
                      // 10 = 10.5V (9V input)
                      // 11 = 14V (12V input)

} HL_SGM41518_REG06_T;

// REG07 TYPE
typedef struct _HL_SGM41518_REG07_T
{
    uint8_t VDPM_BAT_TRACK : 2;  // Dynamic VINDPM Tracking
                                 // 00 = Disable (VINDPM set by register)
                                 // 01 = VBAT + 200mV
                                 // 10 = VBAT + 250mV
                                 // 11 = VBAT + 300mV

    uint8_t BATFET_RST_EN : 1;  // Enable BATFET Reset
                                // 0 = Disable BATFET reset
                                // 1 = Enable BATFET reset (default)

    uint8_t BATFET_DLY : 1;  // BATFET Turn Off Delay Control
                             // 0 = Turn off BATFET immediately
                             // 1 = Turn off BATFET after tSM_DLY(default)

    uint8_t JEITA_VSET : 1;  // JEITA Charging Voltage
                             // 0 = Set charge voltage to 4.1V (MAX) (default)
                             // 1 = Set charge voltage to V-REG

    uint8_t BATFET_DIS : 1;  // Disable BATFET
                             // 0 = Allow BATFET (Q4) to turn on(default)
                             // 1 = Turn off BATFET (Q4) after a t-SM_DLY delay time (REG07 D[3])

    uint8_t TMR2X_EN : 1;  // Enable Half Clock Rate Safety Timer
                           // 0 = Disable
                           // 1 = Safety timer slow down during DPM, JEITA cool, or thermal regulation (default)

    uint8_t IINDET_EN : 1;  // Input Current Limit Detection
                            // 0 = Not in input current limit detection(default)
                            // 1 = Force input current limit detection when VBUS is present
} HL_SGM41518_REG07_T;

// REG08 TYPE
typedef struct _HL_SGM41518_REG08_T
{
    uint8_t VSYS_STAT : 1;  // System Voltage Regulation Status
                            // 0 = Not in VSYSMIN regulation (VBAT > VSYS_MIN)
                            // 1 = In VSYSMIN regulation (VBAT < VSYS_MIN)

    uint8_t THERM_STAT : 1;  // Thermal Regulation Status
                             // 0 = Not in thermal regulation
                             // 1 = In thermal regulation

    uint8_t PG_STAT : 1;  // Input Power Status (VBUS in good voltage range and not poor)
                          // 0 = Input power source is not good
                          // 1 = Input power source is good

    uint8_t CHRG_STAT : 2;  // Charging Status
                            // 00 = Charge disable
                            // 01 = Pre-charge (VBAT < VBATLOW)
                            // 10 = Fast charging (constant current or voltage)
                            // 11 = Charging terminated

    uint8_t VBUS_STAT : 3;  // VBUS Status Register (SGM41518)
                            // 000 = No input
                            // 001 = USB host SDP (500mA) → PSEL HIGH
                            // 010 = Adapter 2.4A → PSEL LOW
                            // 111 = OTG
                            // Other values are reserved.
} HL_SGM41518_REG08_T;

// REG09 TYPE
typedef struct _HL_SGM41518_REG09_T
{
    uint8_t NTC_FAULT : 3;  // JEITA Condition Based on Battery NTC Temperature Measurement
                            // 000 = Normal
                            // 010 = Warm
                            // 011 = Cool (buck mode only)
                            // 101 = Cold
                            // 110 = Hot

    uint8_t BAT_FAULT : 1;  // Battery Fault Status
                            // 0 = Normal
                            // 1 = Battery over-voltage (BATOVP)

    uint8_t CHRG_FAULT : 2;  // Charging Fault Status
                             // 00 = Normal
                             // 01 = Input fault (VAC OVP or VBAT < VVBUS < 3.8V)
                             // 10 = Thermal shutdown
                             // 11 = Charge safety timer expired

    uint8_t BOOST_FAULT : 1;  // Boost Mode Fault Status
                              // 0 = Normal
                              // 1 = VBUS overloaded in OTG, or VBUS OVP, or battery voltage too low
                              //  (any condition that prevents boost starting)

    uint8_t WATCHDOG_FAULT : 1;  // Watchdog Fault Status
                                 // 0 = Normal (no fault)
                                 // 1 = Watchdog timer expired
} HL_SGM41518_REG09_T;

// REG0A TYPE
typedef struct _HL_SGM41518_REG0A_T
{
    uint8_t IINDPM_INT_MASK : 1;  // IINDPM Event Detection Mask
                                  // 0 = Allow IINDPM to send INT pulse
                                  // 1 = Mask IINDPM INT pulse

    uint8_t VINDPM_INT_MASK : 1;  // VINDPM Event Detection Interrupt Mask
                                  // 0 = Allow VINDPM INT pulse
                                  // 1 = Mask VINDPM INT pulse

    uint8_t ACOV_STAT : 1;  // Input Over-Voltage Status (AC adaptor is the input source)
                            // 0 = No over-voltage (no ACOV)
                            // 1 = Over-voltage detected (ACOV)

    uint8_t TOPOFF_ACTIVE : 1;  // Active Top-Off Timer Counting Status
                                // 0 = Top-off timer not counting
                                // 1 = Top-off timer counting

    uint8_t Reserved : 1;  //

    uint8_t IINDPM_STAT : 1;  // Input Current Regulation (Dynamic Power Management)
                              // 0 = Not in IINDPM
                              // 1 = In IINDPM

    uint8_t VINDPM_STAT : 1;  // Input Voltage Regulation (Dynamic Power Management)
                              // 0 = Not in VINDPM
                              // 1 = In VINDPM

    uint8_t VBUS_GD : 1;  // Good Input Source Detected
                          // 0 = A good VBUS is not attached
                          // 1 = A good VBUS attached
} HL_SGM41518_REG0A_T;

// REG0B TYPE
typedef struct _HL_SGM41518_REG0B_T
{
    uint8_t DEV_REV : 2;  // Revision(版本)

    uint8_t SGMPART : 1;  // NA

    uint8_t PN : 4;  // Part ID 0010 = SGM41518

    uint8_t REG_RST : 1;  // Register Reset
                          // 0 = No effect (Keep current register settings)
                          // 1 = Reset R/W bits of all registers to the default and reset safety timer
                          // (It also resets itself to 0 after register reset is completed.)
} HL_SGM41518_REG0B_T;

// REG0C TYPE
typedef struct _HL_SGM41518_REG0C_T
{
    uint8_t JEITA_VT3 : 2;  // R/W 进入高温区间阈值
                            // 0：VT3 = 40℃
                            // 1：VT3 = 44.5℃
                            // 2：VT3 = 50.5℃
                            // 3：VT3 = 54.5℃

    uint8_t JEITA_VT2 : 2;  // R/W 进入低温区间阈值
                            // 0：VT2 = 5.5℃
                            // 1：VT2 = 10℃
                            // 2：VT2 = 15℃
                            // 3：VT2 = 20℃

    uint8_t JEITA_ISET_H : 2;  // R/W 高温区间充电电流限制
                               // 充电电流 = 0：0% * ICHG
                               //           1：20% * ICHG
                               //           2：50% * ICHG
                               //           3：100% * ICHG

    uint8_t JEITA_ISET_L_EN : 1;  // R/W 低温区间充电使能
                                  // 0：失能
                                  // 1：使能

    uint8_t JEITA_VSET_L : 1;  // R/W JEITA充电电压设置
                               // 0：设置电压为 VREG
                               // 1：设置为4.1v

} HL_SGM41518_REG0C_T;

// REG0D TYPE
typedef struct _HL_SGM41518_REG0D_T
{
    uint8_t JEITA_EN : 1;  // R/W JEITA使能设置
                           // 0 ：失能
                           // 1 ：使能

    uint8_t RESERVED : 7;  // R/W 保留

} HL_SGM41518_REG0D_T;

// REG0E TYPE
typedef struct _HL_SGM41518_REG0E_T
{
    uint8_t RESERVED;  // R		:2; //

} HL_SGM41518_REG0E_T;

// REG0FTYPE
typedef struct _HL_SGM41518_REG0F_T
{
    uint8_t VINDPM_OS : 2;  // R/W 输入电压偏移设置
                            // 0 ：3.9v（默认）
                            // 1 ：5.9v
                            // 2 ：7.5v
                            // 3 ：10.5v

    uint8_t STATSET : 2;  // R/W STAT引脚状态获取
                          // 0 ：LED关闭（HIZ模式，默认）
                          // 1 ：LED打开
                          // 2 ：LED亮一秒灭一秒
                          // 3 ：LED亮一秒灭三秒

    uint8_t RESERVED : 2;  // R/W 保留

    uint8_t VREG_FT : 2;  // R/W VREG修正设置
                          // 0 ：不修正（默认）
                          // 1 ：VREG + 8mV
                          // 2 ：VREG - 8mV
                          // 3 ：VREG - 16mV

} HL_SGM41518_REG0F_T;

typedef struct _HL_SGM41518_REGALL_T
{
    HL_SGM41518_REG00_T reg00;
    HL_SGM41518_REG01_T reg01;
    HL_SGM41518_REG02_T reg02;
    HL_SGM41518_REG03_T reg03;
    HL_SGM41518_REG04_T reg04;
    HL_SGM41518_REG05_T reg05;
    HL_SGM41518_REG06_T reg06;
    HL_SGM41518_REG07_T reg07;
    HL_SGM41518_REG08_T reg08;
    HL_SGM41518_REG09_T reg09;
    HL_SGM41518_REG0A_T reg0A;
    HL_SGM41518_REG0B_T reg0B;
    HL_SGM41518_REG0C_T reg0C;
    HL_SGM41518_REG0D_T reg0D;
    HL_SGM41518_REG0F_T reg0F;
} HL_SGM41518_REGALL_T;

/**************************************************************************
 *                               常量                                       *
 **************************************************************************/

//

/**************************************************************************
 *                             全局变量                                       *
 **************************************************************************/

//

/**************************************************************************
 *                            局部函数原型                                      *
 **************************************************************************/

//

/**************************************************************************
 *                            全局函数实现                                      *
 **************************************************************************/

uint8_t hl_drv_sgm41518_init(void);
uint8_t hl_drv_sgm41518_deinit(void);
uint8_t hl_drv_sgm41518_io_ctrl(uint8_t cmd, void* ptr, uint8_t len);
#endif /* end of #ifndef __HL_DRV_SGM41518_H */
