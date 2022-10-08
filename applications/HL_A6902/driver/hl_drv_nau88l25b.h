/**
 * @file hl_drv_nau88l25b.h
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief nau88l25b驱动.h文件
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

#ifndef __HL_DRV_NAU88L25B_H__
#define __HL_DRV_NAU88L25B_H__
#include "stdint.h"

/**************************************************************************
 *                                基本宏                                     *
 **************************************************************************/

/* 函数返回状态 */
#define HL_SUCCESS 0
#define HL_FAILED 1

/* 使用I2C */
#define NAU88L25B_IIC_NAME "i2c2"
#define NAU88L25B_DEVICE_ADDRESS 0x1A

/* 寄存器地址 */
#define NAU88L25B_RESET_REG00 0x00 /*reset digital,csm,clock manager etc.*/
#define NAU_REG33_ADDR 0x33
#define NAU_REG34_ADDR 0x34
#define NAU_REG_NUM 2

// 特殊，用于一次性设置左右声道参数
#define NAU_REGGAIN_ADDR 0xF0
#define NAU_REGMUTE_ADDR 0xF1
#define NAU_SPEREG_NUM 2

/**************************************************************************
 *                                功能选项枚举                             *
 **************************************************************************/

/* 基本操作命令 */
#define NAU_READ_REG_CMD 0x00
#define NAU_WRITE_REG_CMD 0x01

/**************************************************************************
 *                               功能选项参数                              *
 **************************************************************************/
//reg33

//参数 DAC_MIXER（混音设置）:
// Data mixing of the two DAC channels
// Bit 15 = 1: DAC Right channel is ½(L + R)
//        =0: Normal DAC Right channel out
// Bit 14 = 1: DAC Left channel is ½(L+R)
//        = 0: Normal DAC Left channel out

//参数 DAC_CH_SEL0:
// DAC Channel 0 source selection
// 0 : from Left Channel
// 1 : from Right Channel

// 参数：DGAINL_DAC（左声道增益）： 
// -104 ~ +24 ，-104时为静音

//reg34

//参数 DAC_CH_SEL1：
// DAC Channel 1 source selection
// 0 : from Left Channel
// 1 : from Right Channel

//参数 DGAINR_DAC（右声道增益）：
// -104 ~ +24 ，-104时为静音

/**************************************************************************
 *                             数据类型                                    *
 **************************************************************************/

/* 可配置项枚举 */
enum hl_nau_param_w
{
    //reg33
    OPT_DACL_CTRL_DAC_MIXER = 0,
    OPT_DACL_CTRL_DAC_CH_SEL0,
    OPT_DACL_CTRL_DGAINL_DAC,
    //reg34
    OPT_DACR_CTRL_DAC_CH_SEL1,
    OPT_DACR_CTRL_DGAINR_DAC,

    // 特殊，用于一次性设置左右声道参数
    OPT_GAIN = 0xF0,
    OPT_MUTE,
    PARAM_CNT_MAX_W
};

/* NAU88L25B init the register value */
typedef struct _NAU88L25B_REG_T
{
    uint16_t reg_addr;
    uint16_t reg_value;
} NAU88L25B_REG_T;

// 配置入口参数
typedef struct _HL_NAU_INPUT_PARAM_T
{
    uint16_t cfg_opt;//参数参考 ： hl_nau_param_w 枚举内容
    int      param;
} HL_NAU_INPUT_PARAM_T;

typedef struct _HL_nau88l25b_REG33_T
{
    uint16_t DGAINL_DAC : 8;  //左声道增益

    uint16_t Reserved1 : 1;

    uint16_t DAC_CH_SEL0 : 1;//0:左声道  1：右声道

    uint16_t Reserved2 : 4;

    uint16_t DAC_MIXER : 2; //混音

} HL_nau88l25b_REG33_T;

// REG34 TYPE
typedef struct _HL_nau88l25b_REG34_T
{
    uint16_t DGAINR_DAC : 8;  //右声道增益

    uint16_t Reserved1 : 1;

    uint16_t DAC_CH_SEL1 : 1;  //0:左声道  1：右声道

    uint16_t Reserved2 : 6;

} HL_nau88l25b_REG34_T;

uint8_t hl_drv_nau88l25b_init(NAU88L25B_REG_T *data,uint16_t datalen);
uint8_t hl_drv_nau88l25b_deinit(void);
uint8_t hl_drv_nau88l25b_io_ctrl(uint8_t cmd, void* ptr, uint16_t len);

#endif /* __NAU88L25B_H__ */
