/**
 * @file hl_drv_knob.h
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 旋钮驱动程序
 * @version 1.0
 * @date 2022-09-13
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-13     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_DRV_KNOB_H__
#define __HL_DRV_KNOB_H__

#include "hl_config.h"
#if HL_IS_TX_DEVICE()
#else

/* Includes ------------------------------------------------------------------*/
#include "stdbool.h"
#include "stdint.h"
/* typedef -------------------------------------------------------------------*/

typedef enum _hl_drv_knob_e
{
    /// 左旋钮编号
    KNOB_LEFT = 0,
    /// 旋钮数目
    HL_KNOBS,
} hl_drv_knob_e;
/* define --------------------------------------------------------------------*/
#define HL_SUCCESS 0
#define HL_FAILED 1
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * 
 * @brief 初始化旋钮
 * @return uint8_t 成功：0 失败： 1
 * @date 2022-09-13
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-13      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_drv_knob_init(void);

/**
 * 
 * @brief 反初始化 旋钮
 * @return uint8_t 成功：0 失败： 1
 * @date 2022-09-13
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-13      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_drv_knob_deinit(void);

/**
 * 
 * @brief 获取对应的旋钮的值
 * @param [in] 
 *         knob_num： KNOB_LEFT 左旋旋钮
 *                    KNOB_RIGHT 右旋旋钮
 *              val： 参数地址 
 * @return uint8_t 成功 0； 失败 1；
 * @date 2022-09-13
 * @author liujie (jie.liu@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-13      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_drv_knob_read(uint8_t knob_num, int8_t * val);

#endif 

#endif
/*
 * EOF
 */





