#ifndef __RM690A0_PANEL_CFG_H__
#define __RM690A0_PANEL_CFG_H__

#define RT_HW_LCD_XRES 126         /* LCD PIXEL WIDTH             */
#define RT_HW_LCD_YRES 294         /* LCD PIXEL HEIGHT            */
#define RT_HW_LCD_PIXEL_CLOCK 9000 /* pixel clock in khz          */
#define RT_HW_LCD_LANE_MBPS 240    /* DSI per lane Mbps           */
#define RT_HW_LCD_LEFT_MARGIN 40   /* Horizontal back porch       */
#define RT_HW_LCD_RIGHT_MARGIN 40  /* Horizontal front porch      */
#define RT_HW_LCD_UPPER_MARGIN 2   /* Vertical back porch         */
#define RT_HW_LCD_LOWER_MARGIN 6   /* Vertical front porch        */
#define RT_HW_LCD_HSYNC_LEN 10     /* Horizontal synchronization  */
#define RT_HW_LCD_VSYNC_LEN 2      /* Vertical synchronization    */

#define RT_HW_LCD_CONN_TYPE RK_DISPLAY_CONNECTOR_DSI
#define RT_HW_LCD_BUS_FORMAT MEDIA_BUS_FMT_RGB888_1X24
#define RT_HW_LCD_INIT_CMD_TYPE CMD_TYPE_DEFAULT
#define RT_HW_LCD_DSI_LANES 1
#define RT_HW_LCD_VMODE_FLAG                                                                                      \
    DSI_MODE_VIDEO | VIDEO_MODE_FLAG_NHSYNC | VIDEO_MODE_FLAG_NVSYNC | DSI_MODE_VIDEO_BURST | DSI_MODE_EOT_PACKET \
        | DSI_MODE_LPM
#define RT_HW_LCD_DISPLAY_MODE DISPLAY_VIDEO_MODE
#define RT_HW_LCD_AREA_DISPLAY DISABLE_AREA_DISPLAY
#define RT_HW_LCD_DCS_BACKLIGHT 1
#define RT_HW_LCD_MAX_BRIGHTNESS 255

#define RT_HW_LCD_XACT_ALIGN 1
#define RT_HW_LCD_YACT_ALIGN 1
#define RT_HW_LCD_XPOS_ALIGN 1
#define RT_HW_LCD_YPOS_ALIGN 1


/// RM690A0 INIT USED CODE
#define RM690A0_CMD_COLMOD 0x3A

/// RM69310_CMD_COLMOD = 0x3A：
/// 24bit 屏幕接口默认只有24bit，因为RK硬件只输出RGB888
#define COLOR_FORMAT_24BIT 0x77

/// 背光设置
#define RM690A0_CMD_WRDISBV 0x51

///  默认背光值
#define BRIGHTNESS_DEFAULT_VALUE 0x80

const static struct rockchip_cmd cmd_on[] = {
    { 0x15, 0x00, 0x02, { 0xFE, 0x01 } },
    { 0x15, 0x00, 0x02, { 0x36, 0x00 } },
    { 0x15, 0x00, 0x02, { 0x6E, 0x00 } },
    { 0x15, 0x00, 0x02, { 0xFE, 0x00 } },
    // {0x15, 0x00, 0x02, {0xC4, 0x80}},
    { 0x39, 0x00, 0x05, { 0x2A, 0x00, 0x00, 0x00, 0x7D } },
    { 0x15, 0x00, 0x02, { RM690A0_CMD_COLMOD, COLOR_FORMAT_24BIT } },
    { 0x15, 0x00, 0x02, { 0x35, 0x00 } },
    { 0x15, 0x00, 0x02, { RM690A0_CMD_WRDISBV, BRIGHTNESS_DEFAULT_VALUE } },
    { 0x05, 0x78, 0x01, { 0x11 } },
    { 0x05, 0x50, 0x01, { 0x29 } },
};



const static struct rockchip_cmd cmd_off[] = {};

#endif /* __RM690A0_PANEL_CFG_H__ */
