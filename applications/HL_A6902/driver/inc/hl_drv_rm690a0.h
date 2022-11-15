/**
 * @file hl_drv_rm690a0.h
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-11-02
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
 * <tr><td>2022-11-02     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_DRV_MIPI_RM690A0__H_
#define __HL_DRV_MIPI_RM690A0__H_

/* Includes ------------------------------------------------------------------*/

#include "hl_config.h"
#include "rtdef.h"
#if (!HL_GET_DEVICE_TYPE())
/* typedef -------------------------------------------------------------------*/

/* define --------------------------------------------------------------------*/
enum
{
    /// 设置背光
    SET_MIPI_BACKLIGHT,
    /// 读取状态
};

#define MIPI_OLED_WIDTH 126
#define MIPI_OLED_HEIGHT 294
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

uint8_t hl_drv_rm690a0_init(void);

uint8_t hl_drv_rm690a0_deinit(void);

uint8_t hl_drv_rm690a0_write(uint16_t x_start, uint16_t x_end, uint16_t y_start, uint16_t y_end, const uint8_t* p_pic);

uint8_t hl_drv_rm690a0_io_ctrl(uint8_t cmd, void* ptr, uint16_t len);
#endif

#endif /*__HL_DRV_MIPI_RM690A0__H_*/
       /*
 * EOF
 */