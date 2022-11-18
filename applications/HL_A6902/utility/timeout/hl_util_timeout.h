/**
 * @file hl_util_timeout.h
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-11-16
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
 * <tr><td>2022-11-16     <td>v1.0     <td>luzhanghao     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_UTIL_TIMEOUT_H__
#define __HL_UTIL_TIMEOUT_H__
/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
#include <rtdevice.h>

/* typedef -------------------------------------------------------------------*/
/// 定时器结构体
typedef struct _hl_timeout_t
{
    rt_bool_t       activity;           // timer state (RT_TRUE:act      RT_FALSE:not:inact)
    rt_bool_t       timeout_flag;       // timeout flag(RT_TRUE:timeout  RT_FALSE:not timeout)
    rt_tick_t       timeout_time;       // timeout tick(start time + set time)
} hl_timeout_t;

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * @brief 设置定时器超时时间
 * @param [in] p_timeout 定时器结构体指针
 * @param [in] time 定时时长（ms）
 * @return rt_bool_t RT_TRUE--成功 RT_FALSE--失败  
 * @date 2022-11-16
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-16      <td>luzhanghao     <td>新建
 * </table>
 */
rt_bool_t hl_util_timeout_set(hl_timeout_t *p_timeout, rt_tick_t time);

/**
 * @brief 关闭定时器
 * @param [in] p_timeout 定时器结构体指针
 * @return rt_bool_t RT_TRUE--成功 RT_FALSE--失败  
 * @date 2022-11-16
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-16      <td>luzhanghao     <td>新建
 * </table>
 */
rt_bool_t hl_util_timeout_close(hl_timeout_t *p_timeout);

/**
 * @brief 判断定时器是否超时
 * @param [in] p_timeout 定时器结构体指针
 * @return rt_bool_t RT_TRUE--成功 RT_FALSE--失败  
 * @date 2022-11-16
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-16      <td>luzhanghao     <td>新建
 * </table>
 */
rt_bool_t hl_util_timeout_judge(hl_timeout_t *p_timeout);

#endif /* __HL_UTIL_TIMEOUT_H__ */
/*
 * EOF
 */