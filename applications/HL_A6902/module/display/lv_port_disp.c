/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-10     Chris Zhong      First version
*/

#include "lv_port_disp.h"

// #if LV_USE_FULL_SCREEN == 0
// #error "Please define LV_USE_FULL_SCREEN in lv_conf.h"
// #endif

// #define LV_VER_RES_MAX	LV_VER_RES_MAX

// #if defined(RT_LV_DISP_USE_LARGE_BUF)
#define RT_LV_DISP_BUF_MALLOC	rt_malloc_large
#define RT_LV_DISP_BUF_FREE		rt_free_large

#define BUF_HIGH_LEN         LV_HOR_RES_MAX
// #elif defined(RT_LV_DISP_USE_DTCM_BUF)
// #define RT_LV_DISP_BUF_MALLOC	rt_malloc_dtcm
// #define RT_LV_DISP_BUF_FREE		rt_free_dtcm
// #elif defined(RT_LV_DISP_USE_PSRAM_BUF)
// #define RT_LV_DISP_BUF_MALLOC	rt_malloc_psram
// #define RT_LV_DISP_BUF_FREE		rt_free_psram
// #else
// #define RT_LV_DISP_BUF_MALLOC	rt_malloc
// #define RT_LV_DISP_BUF_FREE		rt_free
// #endif

// 1表示使用显示存储器
#define USB_VIDEO_MEMORY 1

#if USB_VIDEO_MEMORY

static lv_color_t * video_memory_g;

#endif

/* littlevGL Display device interface */
lv_disp_drv_t disp_drv;
lv_disp_cntx_t disp_ctx = {0};

static void lv_bl_enable(bool enable)
{
	disp_ctx.bl = rt_device_find("backlight");
	if(!disp_ctx.bl) {
		printf("%s: get bl fail!\n", __FUNCTION__);
		return;
	}

	int brightness = 50;
	printf("%s: turn on backlight\n", __FUNCTION__);
	rt_device_control(disp_ctx.bl, RTGRAPHIC_CTRL_POWERON, NULL);
	rt_device_control(disp_ctx.bl, RTGRAPHIC_CTRL_RECT_UPDATE, &brightness);
}
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
static hl_mod_lvgl_video_mem_t video_mem_t;

void hl_mod_lvgl_video_memory(hl_mod_lvgl_video_mem_t* video_mem_p)
{
    uint32_t src_w = video_mem_p->x2 - video_mem_p->x1 + 1;
    uint32_t src_h = video_mem_p->y2 - video_mem_p->y1 + 1;
    uint32_t i;
    uint32_t j;
    uint8_t  byte;
    // uint32_t src_index;
    uint32_t dst_index;

    // if (video_mem_p->dst_len < (src_w * src_h * video_mem_p->format_byte)) {
    //     return;
    // }
    for (j = video_mem_p->y1; j <= video_mem_p->y2; j++) {
        for (i = video_mem_p->x1; i <= video_mem_p->x2; i++) {

            // src_index = (j * src_w + i) * video_mem_p->format_byte;
            dst_index = (j * video_mem_p->hor_max + i) * video_mem_p->format_byte;

            for (byte = 0; byte < video_mem_p->format_byte; byte++) {
                video_mem_p->dst[(uint32_t)byte + dst_index] =
                    *video_mem_p->src;  //video_mem_p->src[(uint32_t)byte + src_index];
                video_mem_p->src++;
            }
        }
    }
}

void lv_disp_refer(void)
{
	rt_err_t ret;

    disp_ctx.cfg->winEn = 1;
    disp_ctx.cfg->winId = 0;

    ret = rt_device_control(disp_ctx.lcd, RK_DISPLAY_CTRL_SET_SCALE, disp_ctx.scale);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(disp_ctx.lcd, RK_DISPLAY_CTRL_SET_PLANE, disp_ctx.cfg);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(disp_ctx.lcd, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);

	struct display_state *disp_state = (struct display_state *)disp_ctx.lcd->user_data;
    if (disp_state->panel_state.display_mode == DISPLAY_CMD_MODE)
    {
		struct display_sync display_sync_data;

        // rt_kprintf(" sync\n");
        display_sync_data.cmd = DISPLAY_SYNC;
        ret = rt_device_control(disp_ctx.lcd, RK_DISPLAY_CTRL_DISPLAY_SYNC, &display_sync_data);
        if (ret != RT_EOK)
            printf("rt_display_sync_hook time out\n");
    }
}

static void lvgl_fb_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    
    uint32_t a,b;
    /*Return if the area is out the screen*/
    if (area->x2 < 0) return;
    if (area->y2 < 0) return;
    if (area->x1 > LV_HOR_RES_MAX  - 1) return;
    if (area->y1 > LV_VER_RES_MAX - 1) return;

    rt_kprintf("x1 = %d\n",area->x1);
    rt_kprintf("x2 = %d\n",area->x2);
    rt_kprintf("y1 = %d\n",area->y1);
    rt_kprintf("y2 = %d\n",area->y2);

    video_mem_t.format_byte = disp_ctx.dgi->bits_per_pixel / 8;
    video_mem_t.hor_max = LV_HOR_RES_MAX;
    video_mem_t.vor_max = LV_VER_RES_MAX;
    video_mem_t.src = (const uint8_t*)color_p;
    video_mem_t.dst = (uint8_t *) video_memory_g;
    video_mem_t.x1 = area->x1;
    video_mem_t.x2 = area->x2;
    video_mem_t.y1 = area->y1;
    video_mem_t.y2 = area->y2;

    a = rt_tick_get();
    // rt_kprintf("s==%d\n",rt_tick_get());
    hl_mod_lvgl_video_memory(&video_mem_t);
    
    disp_ctx.cfg->yrgbAddr = (uint32_t)video_memory_g;
    // disp_ctx.cfg->yrgbAddr = (uint32_t)color_p;
    disp_ctx.cfg->yrgbLength = disp_ctx.dgi->height * disp_ctx.dgi->width * disp_ctx.dgi->bits_per_pixel / 8;
//    disp_ctx.cfg->yrgbLength = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1) * disp_ctx.dgi->bits_per_pixel / 8;
    // rt_kprintf("w = %d\n",area->x2 - area->x1 + 1);
    // rt_kprintf("h = %d\n",(area->y2 - area->y1 + 1));

	// disp_ctx.scale->srcW = area->x2 - area->x1 + 1;;
	// disp_ctx.scale->srcH = area->y2 - area->y1 + 1;

	// disp_ctx.scale->dstX = area->x1;
	// disp_ctx.scale->dstY = area->y1;
	// disp_ctx.scale->dstW = area->x2 - area->x1 + 1;
	// disp_ctx.scale->dstH = area->y2 - area->y1 + 1;

    lv_disp_refer();
    b = rt_tick_get();
    rt_kprintf("inc==%d\n",b-a);
    lv_disp_flush_ready(disp_drv);

    return;
}

void lv_dgi_init(void)
{
    disp_ctx.dgi->bits_per_pixel = LV_COLOR_DEPTH;
    disp_ctx.dgi->width          = LV_HOR_RES_MAX;
    disp_ctx.dgi->height         = LV_VER_RES_MAX;
    disp_ctx.dgi->framebuffer    = (rt_uint8_t *)disp_ctx.buf_1;
}

static void lv_win_cfg_init(struct CRTC_WIN_STATE *win_config)
{
    win_config->winEn = 1;
    win_config->winId = 0;
    win_config->zpos = 0;
    win_config->format = RTGRAPHIC_PIXEL_FORMAT_RGB565;

    // win_config->yrgbAddr = (uint32_t)disp_ctx.buf_1;

    win_config->yrgbAddr = (uint32_t)video_memory_g;

    win_config->cbcrAddr = 0;
    win_config->yrgbLength = 0;
    win_config->cbcrLength = 0;
    win_config->xVir = LV_HOR_RES_MAX;

    win_config->srcX = 0;
    win_config->srcY = 0;
    win_config->srcW = LV_HOR_RES_MAX;
    win_config->srcH = LV_VER_RES_MAX;

    win_config->crtcX = 0;
    win_config->crtcY = 0;
    win_config->crtcW = LV_HOR_RES_MAX;
    win_config->crtcH = LV_VER_RES_MAX;

    win_config->xLoopOffset = 0;
    win_config->yLoopOffset = 0;

    win_config->alphaEn = 0;
    win_config->alphaMode = VOP_ALPHA_MODE_PER_PIXEL;
    win_config->alphaPreMul = VOP_NON_PREMULT_ALPHA;
}

static void lv_lcd_clear(struct rt_device *dev,
          struct CRTC_WIN_STATE *win_config,
          struct VOP_POST_SCALE_INFO *post_scale,
          struct rt_device_graphic_info *graphic_info)
{
    rt_err_t ret;

    post_scale->srcW = graphic_info->width;
    post_scale->srcH = graphic_info->height;
    post_scale->dstX = 0;
    post_scale->dstY = 0;
    post_scale->dstW = graphic_info->width;
    post_scale->dstH = graphic_info->height;

    ret = rt_device_control(disp_ctx.lcd, RK_DISPLAY_CTRL_SET_SCALE, post_scale);
    RT_ASSERT(ret == RT_EOK);

    win_config->winEn = 0;
    win_config->winUpdate = 1;
    ret = rt_device_control(disp_ctx.lcd, RK_DISPLAY_CTRL_SET_PLANE, win_config);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(disp_ctx.lcd, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);

    rt_thread_delay(33);
}

void lv_lcd_init(void)
{
	rt_err_t ret;
	int path = 0;

	ret = rt_device_control(disp_ctx.lcd, RK_DISPLAY_CTRL_AP_COP_MODE, (uint8_t *)1);
	RT_ASSERT(ret == RT_EOK);

	ret = rt_device_control(disp_ctx.lcd, RK_DISPLAY_CTRL_ENABLE, NULL);
	path = SWITCH_TO_INTERNAL_DPHY;
	ret = rt_device_control(disp_ctx.lcd, RK_DISPLAY_CTRL_MIPI_SWITCH, &path);
	RT_ASSERT(ret == RT_EOK);
	ret = rt_device_control(disp_ctx.lcd, RK_DISPLAY_CTRL_DISABLE, NULL);

	ret = rt_device_control(disp_ctx.lcd, RK_DISPLAY_CTRL_AP_COP_MODE, (uint8_t *)0);
	ret = rt_device_control(disp_ctx.lcd, RTGRAPHIC_CTRL_POWERON, NULL);
	RT_ASSERT(ret == RT_EOK);

	disp_ctx.cfg = (struct CRTC_WIN_STATE *)rt_calloc(1, sizeof(struct CRTC_WIN_STATE));
	RT_ASSERT(disp_ctx.cfg != RT_NULL);
	lv_win_cfg_init(disp_ctx.cfg);

	disp_ctx.scale = (struct VOP_POST_SCALE_INFO *)rt_calloc(1, sizeof(struct VOP_POST_SCALE_INFO));
	RT_ASSERT(disp_ctx.scale != RT_NULL);

	lv_lcd_clear(disp_ctx.lcd, disp_ctx.cfg, disp_ctx.scale, disp_ctx.dgi);

	disp_ctx.scale->srcW = disp_ctx.cfg->srcW;
	disp_ctx.scale->srcH = disp_ctx.cfg->srcH;
	disp_ctx.scale->dstX = disp_ctx.cfg->srcX;
	disp_ctx.scale->dstY = disp_ctx.cfg->srcY;
	disp_ctx.scale->dstW = disp_ctx.cfg->srcW;
	disp_ctx.scale->dstH = disp_ctx.cfg->srcH;
}

static rt_err_t disp_init(void)
{
	rt_err_t ret;

    uint32_t total, used, maxm;
    rt_memory_info(&total, &used, &maxm);
    printf("sram total=%ld, used=%ld, maxm=%ld", total, used, maxm);

    lv_init();

	disp_ctx.buf_1 = (lv_color_t *)RT_LV_DISP_BUF_MALLOC( BUF_HIGH_LEN* LV_VER_RES_MAX * sizeof(lv_color_t));
	RT_ASSERT(disp_ctx.buf_1 != RT_NULL);


#ifdef RT_LV_DISP_DOUBLE_BUF
    disp_ctx.buf_2 = (lv_color_t *)RT_LV_DISP_BUF_MALLOC(BUF_HIGH_LEN * LV_VER_RES_MAX * sizeof(lv_color_t));
    RT_ASSERT(disp_ctx.buf_2 != RT_NULL);
#endif

#if USB_VIDEO_MEMORY
    video_memory_g = (lv_color_t *)RT_LV_DISP_BUF_MALLOC(LV_HOR_RES_MAX * LV_VER_RES_MAX * sizeof(lv_color_t));
    rt_kprintf("video_mem = 0x%x\n", (int)video_memory_g);
#endif

	lv_disp_draw_buf_init(&disp_ctx.disp_buf, disp_ctx.buf_1, disp_ctx.buf_2, BUF_HIGH_LEN* LV_VER_RES_MAX);

	printf("lvgl buf1 = 0x%x, buf2 = 0x%x", (int)disp_ctx.buf_1, (int)disp_ctx.buf_2);
    

    disp_ctx.lcd = rt_device_find("lcd");
    RT_ASSERT(disp_ctx.lcd != RT_NULL);
    ret = rt_device_open(disp_ctx.lcd, RT_DEVICE_OFLAG_RDWR);
    RT_ASSERT(ret == RT_EOK);

    struct display_state *state = (struct display_state *)disp_ctx.lcd->user_data;
	disp_ctx.dgi = &state->graphic_info;

#ifdef RT_USING_VOP
    lv_dgi_init();
#else
    ret = rt_device_control(disp_ctx.lcd, RTGRAPHIC_CTRL_GET_INFO, disp_ctx.dgi);
    RT_ASSERT(ret == RT_EOK);
#endif

    RT_ASSERT(disp_ctx.dgi->bits_per_pixel ==  8 || disp_ctx.dgi->bits_per_pixel == 16 ||
              disp_ctx.dgi->bits_per_pixel == 24 || disp_ctx.dgi->bits_per_pixel == 32);

    if ((disp_ctx.dgi->bits_per_pixel != LV_COLOR_DEPTH) && (disp_ctx.dgi->bits_per_pixel != 32 && LV_COLOR_DEPTH != 24)) {
        printf("Error: framebuffer color depth mismatch! (Should be %d to match with LV_COLOR_DEPTH)",
                   disp_ctx.dgi->bits_per_pixel);
        return RT_ERROR;
    }

	lv_lcd_init();
	// lv_bl_enable(true);

	return RT_EOK;
}

static void lv_display_driver_rounder_callback(
    lv_disp_drv_t* disp_drv,
    lv_area_t* area)
{
    area->x1 = 0;
    area->x2 = LV_HOR_RES_MAX - 1;
    area->y1 = 0;
    area->y2 = LV_VER_RES_MAX - 1;
}



rt_err_t lv_port_disp_init(void)
{
    rt_err_t ret;

	memset(&disp_ctx, 0x0, sizeof(disp_ctx));

    ret = disp_init();
    RT_ASSERT(ret == RT_EOK);

    lv_disp_drv_init(&disp_drv);

	disp_drv.hor_res = LV_HOR_RES_MAX;
	disp_drv.ver_res = LV_VER_RES_MAX;
	disp_drv.draw_buf = &disp_ctx.disp_buf;
	disp_drv.flush_cb = lvgl_fb_flush;
    disp_drv.sw_rotate = 1;
    disp_drv.rotated = LV_DISP_ROT_90;
    // disp_drv.full_refresh = 1;
	// disp_drv.rounder_cb = lv_display_driver_rounder_callback;
    lv_disp_drv_register(&disp_drv);
    
    return RT_EOK;
}

rt_err_t lv_port_disp_deinit(void)
{
    rt_err_t ret;

	if(disp_ctx.buf_1) {
		RT_LV_DISP_BUF_FREE(disp_ctx.buf_1);
		disp_ctx.buf_1 = RT_NULL;
	}

    if(disp_ctx.buf_2) {
		RT_LV_DISP_BUF_FREE(disp_ctx.buf_2);
		disp_ctx.buf_2 = RT_NULL;
	}

    ret = rt_device_close(disp_ctx.lcd);
    RT_ASSERT(ret == RT_EOK);

    return ret;
}

struct rt_device_graphic_info* lv_port_dgi_get(void)
{
	 return disp_ctx.dgi;
}



