/**
 * @file hl_drv_cw2215.h
 * @author lilin (lin.li@hollyland-tech.com)
 * @brief 电量计驱动头文件，对外提供控制电量计的接口。
 * @version 1.0
 * @date 2022-09-01
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-01     <td>v1.0     <td>lilin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __HL_DRV_CW2215_H__
#define __HL_DRV_CW2215_H__

/* Includes ------------------------------------------------------------------*/

#include "rtthread.h"
#include "rtdevice.h"
#include "hl_config.h"

/* typedef -------------------------------------------------------------------*/

/**
 * 
 * @brief hl_drv_cw2215_ctrl的功能枚举
 * @date 2022-09-01
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lilin     <td>新建
 * </table>
 */
typedef enum _hl_drv_guage_op
{
    ///get cw2215 chip id, type of parameter is <char> pointer
    HL_DRV_GUAGE_GET_CHIP_ID = 0,
    ///get bat voltage, type of parameter is <uint16_t> pointer
    HL_DRV_GUAGE_GET_VOLTAGE,
    ///get bat current, param type of parameter is <int32_t> pointer
    HL_DRV_GUAGE_GET_CURRENT,
    ///get bat soc, param type of parameter is <hl_st_drv_guage_soc_t> pointer
    HL_DRV_GUAGE_GET_SOC,
    ///get bat temperature, param type of parameter is <hl_st_drv_guage_temp_t> pointer
    HL_DRV_GUAGE_GET_TEMP,
    ///get bat soh (State of Health), param type of parameter is <uint8_t> pointer
    HL_DRV_GUAGE_GET_SOH,
    ///get bat cycle count, param type of parameter is <uint32_t> pointer
    HL_DRV_GUAGE_GET_CYCLE_COUNT,
    ///check guage interrupt flag, param type of parameter is <hl_drv_guage_it_flag_e> pointer，该命令会返回产生了中断的标志枚举，可以使用 | 来判断产生了哪种中断
    HL_DRV_GUAGE_CHECK_IT_FLAG,
    ///clear guage interrupt flag, param type of parameter is <hl_drv_guage_it_flag_e> pointer
    HL_DRV_GUAGE_CLEAR_IT_FLAG,
    ///dump all register value, no param
    HL_DRV_GUAGE_DUMP_ALL_REGISTER_VALUE,
    ///自检测，无参数
    HL_DRV_GUAGE_CHIP_SELF_CHECK,
} hl_drv_guage_op_t;

/**
 * 
 * @brief 电量计SOC结构体，用来存SOC值，单位为%
 * @date 2022-09-01
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lilin     <td>新建
 * </table>
 */
typedef struct _hl_st_drv_guage_soc
{
    ///整数部分，范围是 0~100
    uint8_t soc;
    ///小数部分，计算方法为 soc_d/256
    uint8_t soc_d;
} hl_st_drv_guage_soc_t;

/**
 * 
 * @brief 电量计温度结构体，用来存储温度值，单位为摄氏度
 * @date 2022-09-02
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-02      <td>lilin     <td>新建
 * </table>
 */
typedef struct _hl_st_drv_guage_temp
{
    ///整数部分，范围是 -40~87
    int8_t temp;
    ///小数部分，值为 [0,5]
    uint8_t temp_d;
} hl_st_drv_guage_temp_t;

/**
 * 
 * @brief 电量计中断标志位枚举
 * @date 2022-09-21
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 该枚举描述了电量计的中断标志位类型
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-21      <td>lilin     <td>新建
 * </table>
 */
typedef enum _hl_drv_guage_it_flag
{
    HL_DRV_GUAGE_IT_FLAG_SOC  = 0x01,
    HL_DRV_GUAGE_IT_FLAG_TMAX = 0x02,
    HL_DRV_GUAGE_IT_FLAG_TMIN = 0x04,
} hl_drv_guage_it_flag_e;

/* define --------------------------------------------------------------------*/

#define CW2215_FUNC_RET_ERR 1
#define CW2215_FUNC_RET_OK 0

#define CW2215_CHIP_ID (0xa0)

/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * 
 * @brief 电量计初始化函数
 * @return int8_t 
 * @date 2022-09-01
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 使电量计进入工作模式
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lilin     <td>新建
 * </table>
 */
int8_t hl_drv_cw2215_init(void);

/**
 * 
 * @brief 电量计解初始化函数
 * @return int8_t 
 * @date 2022-09-01
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 使电量计进入睡眠（关机）模式
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lilin     <td>新建
 * </table>
 */
int8_t hl_drv_cw2215_deinit(void);

/**
 * 
 * @brief 电量计控制函数
 * @param [in] op 详见hl_drv_guage_op_t定义的内容
 * @param [in] arg 同上
 * @param [in] arg_size 同上
 * @return int8_t 成功：0，失败：1
 * @date 2022-09-01
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lilin     <td>新建
 * </table>
 */
int8_t hl_drv_cw2215_ctrl(hl_drv_guage_op_t op, void* arg, int32_t arg_size);

#endif
/*
 * EOF
 */