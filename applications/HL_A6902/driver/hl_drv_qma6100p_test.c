/**
 * @file hl_drv_qma6100p_test.c
 * @author libo (rd46@hollyland-tech.com)
 * @brief 
 * @version 1.0
 * @date 2022-10-11
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-10-11     <td>v1.0     <td>libo           <td>内容
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

#include "hl_drv_qma6100p.h"

/**
 * 
 * @brief demo
 * @date 2022-10-13
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-13       <td>libo           <td>新建
 * </table>
 */
void hl_drv_qma6100p_export(void)
{
    euler_angle_t pose;

    hl_drv_qma6100p_init();

    rt_thread_mdelay(1000);  //初始化后不能立即获取 要等待寄存器设置完成这段时间

    hl_drv_qma6100p_io_ctrl(QMA6100_GET_EULWER_ANGLE, (void*)&pose, sizeof(euler_angle_t));
    
    HL_QMA6100_LOG("pitch = %d roll =%d  z=%d\n",pose.pitch, pose.roll, pose.z);
}

MSH_CMD_EXPORT(hl_drv_qma6100p_export, qma6100p ctrl);