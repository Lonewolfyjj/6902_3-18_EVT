/**
 * @file hl_drv_rm690a0.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief mipi屏幕 rm690a0相关驱动
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
/* Includes ------------------------------------------------------------------*/
#include "hl_drv_rm690a0.h"
#include "rtthread.h"
#include "hal_gpio.h"
#include "rtdevice.h"
#include "hl_hal_gpio.h"
#include "drv_display.h"
#include "drv_heap.h"
#include "dma.h"
#include "hal_base.h"
#include "display_pattern.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/

#if (!HL_IS_TX_DEVICE())

#define SRC_DATA_FMT RTGRAPHIC_PIXEL_FORMAT_RGB888
#define SRC_DATA_W RT_LV_HOR_RES
#define SRC_DATA_H RT_LV_VER_RES
/* variables -----------------------------------------------------------------*/
static struct rt_device*              g_display_dev ;
static struct rt_device_graphic_info* graphic_info  = RT_NULL;
static struct CRTC_WIN_STATE*         win_config    = RT_NULL;
static struct VOP_POST_SCALE_INFO*    post_scale    = RT_NULL;
static uint32_t fb_length = SRC_DATA_W * SRC_DATA_H *3;
static uint32_t rtt_framebuffer_test;
static     uint32_t hlpitches ;
void *planes[3];
/* Private function(only *.c)  -----------------------------------------------*/
static int32_t AlignDown(int32_t value, int32_t align)
{
    return value / align * align;
}

static void hl_drv_rm690a0_gpio_init(void)
{
    hl_hal_gpio_init(GPIO_OLED_RST);

    hl_hal_gpio_high(GPIO_OLED_RST);
}



static uint32_t get_stride(uint32_t format, uint32_t width)
{
    switch (format)
    {
    case RTGRAPHIC_PIXEL_FORMAT_ARGB888:
    case RTGRAPHIC_PIXEL_FORMAT_ABGR888:
        return width * 4;
    case RTGRAPHIC_PIXEL_FORMAT_RGB888:
        return width * 3;
    case RTGRAPHIC_PIXEL_FORMAT_BGR565:
    case RTGRAPHIC_PIXEL_FORMAT_RGB565:
        return width * 2;
    case RTGRAPHIC_PIXEL_FORMAT_YUV420:
        return width;
    default:
        rt_kprintf("get_stride unsupport format:%d\n", format);
        return width * 4;
    }
}
static void hl_drv_rm690a0_gpio_deinit(void)
{
    hl_hal_gpio_init(GPIO_OLED_RST);

    hl_hal_gpio_low(GPIO_OLED_RST);
}


static int32_t framebuffer_alloc(rt_size_t size)
{
#if defined(RT_USING_LARGE_HEAP)
    return (uint32_t)rt_dma_malloc_large(size);
#else
    return 0;
#endif
}


static void framebuffer_free(void *base)
{
#if defined(RT_USING_LARGE_HEAP)
    rt_dma_free_large(base);
#endif
}

static void display_win_init(struct CRTC_WIN_STATE* win_config, struct VOP_POST_SCALE_INFO* post_scale)
{
    win_config->winEn  = true;
    win_config->winId  = 0;
    win_config->zpos   = 0;
    win_config->format = SRC_DATA_FMT;

    win_config->yrgbAddr = (uint32_t)rtt_framebuffer_test;
    win_config->cbcrAddr = (uint32_t)RT_NULL;

    win_config->yrgbLength = 0;
    win_config->cbcrLength = 0;
    win_config->xVir       = 0;

    win_config->srcX = 0;
    win_config->srcY = 0;
    win_config->srcW = SRC_DATA_W;
    win_config->srcH = SRC_DATA_H;

    win_config->crtcX = 0;
    win_config->crtcY = 0;
    win_config->crtcW = SRC_DATA_W;
    win_config->crtcH = SRC_DATA_H;

    win_config->xLoopOffset = 0;
    win_config->yLoopOffset = 0;

    win_config->alphaEn     = 0;
    win_config->alphaMode   = 0;
    win_config->alphaPreMul = 0;

    post_scale->srcW = win_config->srcW;
    post_scale->srcH = win_config->srcH;
    post_scale->dstX = win_config->srcX;
    post_scale->dstY = win_config->srcY;
    post_scale->dstW = win_config->srcW;
    post_scale->dstH = win_config->srcH;
}

static void test_display_clear_screen(struct rt_device *dev,
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

    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_SCALE, post_scale);
    RT_ASSERT(ret == RT_EOK);

    win_config->winEn = 0;
    win_config->winUpdate = 1;
    ret = rt_device_control(g_display_dev,
                            RK_DISPLAY_CTRL_SET_PLANE, win_config);
    RT_ASSERT(ret == RT_EOK);

    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
    RT_ASSERT(ret == RT_EOK);
    rt_kprintf("Display test: test_display_init!\n");
    rt_thread_mdelay(40);
}


static uint8_t hl_drv_rm690a0_set_bl(uint16_t val)
{
    uint8_t  ret        = RT_ERROR;
    uint16_t brightness = val;
    if (g_display_dev == RT_NULL) {
        rt_kprintf("mipi drv bl err!\r\n");
        return RT_ERROR;
    }
    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_UPDATE_BL, &brightness);

    return ret;
}

/* Exported functions --------------------------------------------------------*/



uint8_t hl_drv_rm690a0_write(uint16_t x_start, uint16_t x_end, uint16_t y_start, uint16_t y_end, const uint8_t* p_pic)
{
    uint8_t ret = RT_ERROR;

    if (g_display_dev == RT_NULL) {
        rt_kprintf("mipi drv write err!\r\n");
        return RT_ERROR;
    }
    if (win_config == RT_NULL) {
        rt_kprintf("mipi drv write err!\r\n");
        return RT_ERROR;
    }

    if (graphic_info == RT_NULL) {
        rt_kprintf("mipi drv write err!\r\n");
        return RT_ERROR;
    }
    if (post_scale == RT_NULL) {
        rt_kprintf("mipi drv write err!\r\n");
        return RT_ERROR;
    }
    if (p_pic == RT_NULL) {
        rt_kprintf("mipi drv write err!\r\n");
        return RT_ERROR;
    }

    
    util_fill_pattern(SRC_DATA_FMT, planes, SRC_DATA_W, SRC_DATA_H, hlpitches,*p_pic);
    HAL_DCACHE_CleanByRange((uint32_t)rtt_framebuffer_test, fb_length);
    // rt_memcpy(rtt_framebuffer_test,p_pic ,fb_length);


    rt_kprintf("rtt_framebuffer_test,p_pic!\r\n");
    win_config->winId = 0;
    win_config->winEn = 1;

    win_config->xLoopOffset = 0;
    win_config->yLoopOffset = 0;
    win_config->xVir = 0;
    // win_config->yrgbLength  = (uint32_t*)rtt_framebuffer_test;
    win_config->srcX        = x_start;
    win_config->srcY        = y_start;
    win_config->srcW        = x_end - x_start +1;
    win_config->srcH        = y_end - y_start +1 ;
    rt_kprintf("win_config->srcX=%d\r\n",win_config->srcX);
    rt_kprintf("win_config->srcY=%d\r\n",win_config->srcY);
    rt_kprintf("win_config->srcW=%d\r\n",win_config->srcW);
    rt_kprintf("win_config->srcH=%d\r\n",win_config->srcH);

    post_scale->srcW = graphic_info->width;
    post_scale->srcH = win_config->srcH;
    post_scale->dstX = 0;
    post_scale->dstY = win_config->srcY;
    post_scale->dstW = graphic_info->width;
    post_scale->dstH = win_config->srcH;

    ret |= rt_device_control(g_display_dev, RK_DISPLAY_CTRL_SET_SCALE, post_scale);
    rt_kprintf("rtt_framebuffer_test,p_pic!\r\n");
    ret |= rt_device_control(g_display_dev, RK_DISPLAY_CTRL_SET_PLANE, win_config);
    rt_kprintf("rtt_framebuffer_test,p_pic!\r\n");
    ret |= rt_device_control(g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);

    rt_thread_mdelay(5);

    return ret;
}
uint8_t hl_drv_rm690a0_io_ctrl(uint8_t cmd, void* ptr, uint16_t len)
{
    uint8_t result = RT_ERROR;

    switch (cmd) {
        case SET_MIPI_BACKLIGHT: {
            uint8_t val = *(uint8_t*)ptr;
            if (val > 255) {
                result = RT_EINVAL;
            } else {
                result = hl_drv_rm690a0_set_bl(val);
            }

        } break;
        default: {

        } break;
    }

    return result;
}

uint8_t hl_drv_rm690a0_deinit(void)
{
    uint8_t ret = RT_ERROR;

    if (g_display_dev == RT_NULL) {
        rt_kprintf("mipi drv deinit err!\r\n");
        return RT_ERROR;
    }
    ret = rt_device_control(g_display_dev, RTGRAPHIC_CTRL_POWEROFF, NULL);
    if (ret != RT_EOK) {
        rt_kprintf("mipi drv deinit err!\r\n");
        return RT_ERROR;
    }
    if (post_scale == RT_NULL) {
        rt_kprintf("mipi drv rm690a0 err!\r\n");
        return RT_ERROR;
    }

    rt_free(post_scale);
    rt_free(graphic_info);

    if (win_config == RT_NULL) {
        rt_kprintf("mipi drv err!\r\n");
        return RT_ERROR;
    }

    rt_free(win_config);

    framebuffer_free((void *)rtt_framebuffer_test);
    hl_drv_rm690a0_gpio_deinit();

    return ret;
}




uint8_t         hl_drv_rm690a0_init(void)
{
    uint8_t ret = RT_NULL;
    

    hl_drv_rm690a0_gpio_init();


    fb_length = SRC_DATA_W * SRC_DATA_H *3;
hlpitches = get_stride(SRC_DATA_FMT, SRC_DATA_W);
    rtt_framebuffer_test = framebuffer_alloc(fb_length);

    if (rtt_framebuffer_test == RT_NULL) {
        rt_kprintf("malloc memory: 0x%x failed\n", fb_length);
        return -RT_ENOMEM;
    }
    rt_memset((void *)rtt_framebuffer_test, 0, fb_length);
    
    planes[0] = (void *)rtt_framebuffer_test;
    util_fill_pattern(SRC_DATA_FMT, planes, SRC_DATA_W, SRC_DATA_H, hlpitches, 1);
   
    HAL_DCACHE_CleanByRange((uint32_t)rtt_framebuffer_test, fb_length);
    g_display_dev = rt_device_find("lcd");

    if (g_display_dev == RT_NULL) {
        rt_kprintf("mipi drv err 1!\r\n");
        return RT_ERROR;
    }

    ret = rt_device_open(g_display_dev, RT_DEVICE_FLAG_RDWR);

    if (ret != RT_EOK) {
        rt_kprintf("mipi drv err 2 !\r\n");
        return RT_ERROR;
    }

    ret = rt_device_control(g_display_dev, RK_DISPLAY_CTRL_AP_COP_MODE, (uint8_t*)0);

    ret |= rt_device_control(g_display_dev, RTGRAPHIC_CTRL_POWERON, NULL);

    if (ret != RT_EOK) {
        rt_kprintf("mipi drv err 3!\r\n");
        return RT_ERROR;
    }

    graphic_info = (struct rt_device_graphic_info*)rt_calloc(1, sizeof(struct rt_device_graphic_info));

    if (graphic_info == RT_NULL) {
        rt_kprintf("mipi drv err 4!\r\n");
        return RT_ERROR;
    }

    ret = rt_device_control(g_display_dev, RTGRAPHIC_CTRL_GET_INFO, (void*)graphic_info);

    if (ret != RT_EOK) {
        rt_kprintf("mipi drv err 5!\r\n");
        return RT_ERROR;
    }

    win_config = (struct CRTC_WIN_STATE*)rt_calloc(1, sizeof(struct CRTC_WIN_STATE));

    if (win_config == RT_NULL) {
        rt_kprintf("mipi drv err 6!\r\n");
        return RT_ERROR;
    }

    post_scale = (struct VOP_POST_SCALE_INFO*)rt_calloc(1, sizeof(struct VOP_POST_SCALE_INFO));

    if (post_scale == RT_NULL) {
        rt_kprintf("mipi drv err 7!\r\n");
        return RT_ERROR;
    }

    display_win_init(win_config, post_scale);

rt_thread_mdelay(20);
        test_display_clear_screen(g_display_dev, win_config, post_scale, graphic_info);

    

    return RT_EOK;
}
// INIT_BOARD_EXPORT(hl_drv_rm690a0_init);

#endif /* (!HL_IS_TX_DEVICE()) */
       /*
 * EOF
 */