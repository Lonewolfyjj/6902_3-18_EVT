/**
 * @file hl_util_iap2.h
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * @brief IMF认证流程(嵌套状态机)
 * @version 0.1
 * @date 2022-08-31
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-08-31     <td>v1.0     <td>lisonglin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_UTIL_IAP2_H__
#define __HL_UTIL_IAP2_H__

/* Includes ------------------------------------------------------------------*/
#include "hl_util_iap2_func.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * hl_util_iap2_oneshot
 * @brief 循环执行嵌套状态机每个状态，完成苹果认证
 * @param [in] iap2 iap2句柄
 * @return int 成功 0 | 失败 非0
 * @date 2022-09-01
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lisonglin     <td>新建
 * </table>
 */
int hl_util_iap2_oneshot(hl_util_apple_p apple);

/**
 * hl_util_iap2_init
 * @brief IAP2认证
 * @param [in] handle 函数指针句柄
 * @return int 成功 0 | 失败 非0
 * @date 2022-09-01
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lisonglin     <td>新建
 * </table>
 */
int hl_util_iap2_init(hl_util_apple_p apple);

/**
 * hl_util_iap2_deinit
 * @brief IAP2认证去初始化
 * @return int 成功 0 | 失败 非0
 * @date 2022-09-01
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lisonglin     <td>新建
 * </table>
 */
int hl_util_iap2_deinit(hl_util_apple_p apple);

#endif
/*
 * EOF
 */
