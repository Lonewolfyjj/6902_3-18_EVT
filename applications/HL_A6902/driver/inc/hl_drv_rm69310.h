#ifndef APPLICATIONS_DRV_INC_HL_DRV_RM69310_H_
#define APPLICATIONS_DRV_INC_HL_DRV_RM69310_H_

#include "rtdef.h"
#include "hl_config.h"
#if (!HL_GET_DEVICE_TYPE())
#define DEFAULT_BACKLIGHT 0x80

#define OLED_WIDTH 2//240
#define OLED_HEIGHT 2//120

#define OLED_MEM_SIZE   120 * 240

#define RGB565_BLACK 0x0000
#define RGB565_RED 0x00f8
#define RGB565_GREEN 0xe007
#define RGB565_BLUE 0x1f00
#define RGB565_WHITE 0xffff

enum
{
    /// 设置背光
    SET_BACKLIGHT,
    /// 读取状态
    READ_STATE,
    /// 屏幕电源控制
    CTRL_POWER,
    /// 读取oledid
    READ_OLED_ID,
    // /// 设置oled颜色
    // SET_COLOR,
};

uint8_t hl_drv_rm69310_init(void);

uint8_t hl_drv_rm69310_deinit(void);

uint8_t hl_drv_rm69310_display_write(uint8_t x_start, uint8_t x_end, uint8_t y_start, uint8_t y_end,
                                     const uint8_t* p_pic);

uint8_t hl_drv_rm69310_io_ctrl(uint8_t cmd, void* ptr, uint16_t len);

#endif
#endif /* APPLICATIONS_DRV_INC_HL_DRV_RM69310_H_ */
