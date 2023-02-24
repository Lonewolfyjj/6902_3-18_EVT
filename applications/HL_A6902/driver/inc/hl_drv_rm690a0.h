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
#include "drv_display.h"

#if (!HL_IS_TX_DEVICE())
/* typedef -------------------------------------------------------------------*/

typedef struct _hl_drv_color_t
{
    uint8_t  r;
    uint8_t  g;
    uint8_t  b;
    uint8_t  a;
    uint32_t buf;
    uint32_t win_size;
    uint32_t format;
} hl_drv_fill_pattern_t;

typedef struct _hl_mod_lvgl_video_mem_t
{
    uint16_t format_byte;
    uint16_t hor_max;
    uint16_t vor_max;
    uint16_t x1;
    uint16_t x2;
    uint16_t y1;
    uint16_t y2;
    const uint8_t * src;
    uint8_t * dst;
} hl_mod_lvgl_video_mem_t;

/* define --------------------------------------------------------------------*/
enum
{
    /// 设置背光
    SET_MIPI_BACKLIGHT_CMD,
    // /// 设置屏幕显示纯色
    // DISPLAY_FULL_COLOR_CMD,
    /// 关闭屏幕的正负电压
    CLOSE_MIPI_SCREENPOWER_CMD,
    /// @brief  打开屏幕的正负电压
    OPEN_MIPI_SCREENPOWER_CMD,
    /// 申请缓冲区
    FRAMEBUF_MALLOC_CMD,
    /// 释放缓冲区
    FRAMEBUF_FREE_CMD,
};

#define MIPI_OLED_DATA_FMT RTGRAPHIC_PIXEL_FORMAT_RGB565


#define MIPI_OLED_WIDTH RT_LV_HOR_RES
#define MIPI_OLED_HEIGHT RT_LV_VER_RES
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

uint8_t hl_drv_rm690a0_init(void);

uint8_t hl_drv_rm690a0_deinit(void);

// uint8_t hl_drv_rm690a0_write(uint16_t x_start, uint16_t x_end, uint16_t y_start, uint16_t y_end, const uint8_t* p_pic);
uint8_t hl_drv_rm690a0_write(hl_mod_lvgl_video_mem_t* video_mem_p);
uint8_t hl_drv_rm690a0_io_ctrl(uint8_t cmd, void* ptr, uint32_t len);
uint8_t hl_mipi_screen_sta(void);
#endif

#endif /*__HL_DRV_MIPI_RM690A0__H_*/
/*
 * EOF
 */