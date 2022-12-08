/**
 * @file lv_port_disp.h
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-12-08
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
 * <tr><td>2022-12-08     <td>v1.0     <td>liujie     <td>内容
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
#ifndef __LV_PORT_DISP_HL_H__
#define __LV_PORT_DISP_HL_H__

#include "hl_config.h"

#if !HL_IS_TX_DEVICE()

#include <rtthread.h>
#include "drv_display.h"
#include "dma.h"
#include "drv_heap.h"
#include "lvgl.h"

typedef struct {
	rt_device_t lcd;
	rt_device_t bl;
	
	struct rt_device_graphic_info* dgi;
	lv_disp_draw_buf_t disp_buf;
	lv_color_t* buf_1;
	lv_color_t* buf_2;

	struct CRTC_WIN_STATE* cfg;
	struct VOP_POST_SCALE_INFO *scale;

	rt_device_t dma;
	struct rt_dma_transfer xfer;
	rt_sem_t sem;
	rt_uint8_t dma_run;

}lv_disp_cntx_t;

rt_err_t lv_port_disp_init(void);
rt_err_t lv_port_disp_deinit(void);

struct rt_device_graphic_info* lv_port_dgi_get(void);

void lv_disp_refer(void);

#endif

#endif
