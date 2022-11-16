/**
 * @file hl_util_timeout.c
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
/* Includes ------------------------------------------------------------------*/
#include "hl_util_timeout.h"

#define DBG_TAG "timeout"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
rt_bool_t hl_util_timeout_set(hl_timeout_t *p_timeout, rt_tick_t time)
{
    if ((p_timeout == RT_NULL) || (time > (RT_TICK_MAX >> 1))) {
        LOG_E("Invalid argument! p_timeout = %d, time = %d !", p_timeout, time);
        return RT_EINVAL;
    }

    p_timeout->timeout_time = rt_tick_get() + time;
    p_timeout->timeout_flag = RT_FALSE;
    p_timeout->activity     = RT_TRUE;

    return RT_TRUE;
}

rt_bool_t hl_util_timeout_close(hl_timeout_t *p_timeout)
{
    if (p_timeout == RT_NULL) {
        LOG_E("Invalid argument! p_timeout = %d!", p_timeout);
        return RT_EINVAL;
    }

    p_timeout->timeout_flag = RT_FALSE;
    p_timeout->activity     = RT_FALSE;

    return RT_TRUE;
}

rt_bool_t hl_util_timeout_judge(hl_timeout_t *p_timeout)
{
    if (p_timeout == RT_NULL) {
        LOG_E("Invalid argument! p_timeout = %d!", p_timeout);
        return RT_FALSE;
    }

    if (p_timeout->activity == RT_FALSE) {
        return RT_FALSE;
    }

    if ((p_timeout->timeout_time - rt_tick_get()) > (RT_TICK_MAX >> 1)) {
        p_timeout->timeout_flag = RT_TRUE;
        p_timeout->activity     = RT_FALSE;
        return RT_TRUE;
    }

    return RT_FALSE;
}

/*
 * EOF
 */