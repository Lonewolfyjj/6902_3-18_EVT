#ifndef __HL_RTC_PCF85063ATL_H__
#define __HL_RTC_PCF85063ATL_H__

#include<stdint.h>
#include "drivers/i2c.h"

enum RTC_CTRL_CMD {
    RTC_GET_TIME,

};
    

uint8_t hl_drv_rtc_pcf85063_init();

uint8_t hl_drv_rtc_pcf85063_deinit();

uint8_t hl_drv_rtc_pcf85063_io_ctrl(uint8_t cmd, void *ptr, uint16_t len);

void iic_test();

#endif