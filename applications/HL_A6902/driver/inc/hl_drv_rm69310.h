#ifndef APPLICATIONS_DRV_INC_HL_DRV_RM69310_H_
#define APPLICATIONS_DRV_INC_HL_DRV_RM69310_H_

#include "rtdef.h"

#define DEFAULT_BACKLIGHT 0x80

#define OLED_WIDTH 240
#define OLED_HEIGHT 120

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
};

uint8_t hl_drv_rm69310_init(void);

uint8_t hl_drv_rm69310_deinit(void);

uint8_t hl_drv_rm69310_display_write(uint8_t x_start, uint8_t x_end, uint8_t y_start, uint8_t y_end,
                                     const uint8_t* p_pic);

uint8_t hl_drv_rm69310_io_ctrl(uint8_t cmd, void* ptr, uint16_t len);

#endif /* APPLICATIONS_DRV_INC_HL_DRV_RM69310_H_ */
