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
#include "drv_heap.h"
#include "dma.h"
#include "hal_base.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/

#if (!HL_IS_TX_DEVICE())

#define OLED_PWR_ON() hl_hal_gpio_high(GPIO_OLED_SWIRE)
#define OLED_PWR_OFF() hl_hal_gpio_low(GPIO_OLED_SWIRE)

#define OLED_RST_H() hl_hal_gpio_high(GPIO_OLED_RST)
#define OLED_RST_L() hl_hal_gpio_low(GPIO_OLED_RST)





#define MAKE_RGB_INFO(rl, ro, gl, go, bl, bo, al, ao) \
    .rgb = { { (rl), (ro) }, { (gl), (go) }, { (bl), (bo) }, { (al), (ao) } }

#define DRV_ARRAY_SIZE(a) (sizeof((a)) / sizeof((a)[0]))

struct color_rgb24
{
    uint32_t value : 24;
} __attribute__((__packed__));

/* variables -----------------------------------------------------------------*/
typedef struct _hl_drv_mipi_screen
{
    struct rt_device*           g_display_dev;
    struct CRTC_WIN_STATE*      win_config;
    struct VOP_POST_SCALE_INFO* post_scale;
    struct rt_device_graphic_info dgi;
    uint32_t mipi_data_len;
} hl_drv_mipi_screen;


static hl_drv_mipi_screen mipi_screen;
/* Private function(only *.c)  -----------------------------------------------*/

// #if HL_SCREEN_IS_BIG_ENDIAN  1

struct hl_util_color_component
{
    uint16_t length;
    uint16_t offset;
};

struct hl_util_rgb_info
{
    struct hl_util_color_component red;
    struct hl_util_color_component green;
    struct hl_util_color_component blue;
    struct hl_util_color_component alpha;
};

struct hl_util_format_info
{
    uint32_t                      format;
    const struct hl_util_rgb_info rgb;
};

#define HL_MAKE_RGBA(rgb, r, g, b, a)                              \
    ((((b) >> (8 - (rgb)->red.length)) << (rgb)->red.offset)       \
     | (((g) >> (8 - (rgb)->green.length)) << (rgb)->green.offset) \
     | (((r) >> (8 - (rgb)->blue.length)) << (rgb)->blue.offset)   \
     | (((a) >> (8 - (rgb)->alpha.length)) << (rgb)->alpha.offset))

#define MAKE_RGB24(rgb, r, g, b)               \
    {                                          \
        .value = HL_MAKE_RGBA(rgb, r, g, b, 0) \
    }

static const struct hl_util_format_info format_info[] = {
    { RTGRAPHIC_PIXEL_FORMAT_BGR565, MAKE_RGB_INFO(5, 0, 6, 5, 5, 11, 0, 0) },
    { RTGRAPHIC_PIXEL_FORMAT_RGB565, MAKE_RGB_INFO(5, 0, 6, 5, 5, 11, 0, 0) },
    { RTGRAPHIC_PIXEL_FORMAT_RGB888, MAKE_RGB_INFO(8, 16, 8, 8, 8, 0, 0, 0) },
    { RTGRAPHIC_PIXEL_FORMAT_ARGB888, MAKE_RGB_INFO(8, 16, 8, 8, 8, 0, 8, 24) },
    { RTGRAPHIC_PIXEL_FORMAT_ABGR888, MAKE_RGB_INFO(8, 0, 8, 8, 8, 16, 8, 24) },
};

const struct hl_util_format_info* hl_util_format_info_find(uint32_t format)
{
    uint32_t i;

    for (i = 0; i < DRV_ARRAY_SIZE(format_info); i++)
        if (format_info[i].format == format)
            return &format_info[i];

    return NULL;
}

#define HL_MAKE_RGB24(rgb, r, g, b) HL_MAKE_RGBA(rgb, r, g, b, 0)
#define HL_MAKE_RGB16(rgb, r, g, b) HL_MAKE_RGBA(rgb, r, g, b, 0)

static uint8_t get_color_format_byte(uint8_t format);
static void hl_drv_rm690a0_hardware_rst(void);
static void hl_fill_pix_rgb(uint8_t* buf1, uint8_t* data, uint32_t size, uint32_t byte)
{
    uint32_t x;
    uint32_t y;

    rt_kprintf("add4 = %x \n", buf1);

    for (x = 0; x < size; x++) {
        for (y = 0; y < byte; y++) {
            buf1[y + byte * x] = data[y];
        }
    }
}

static void hl_color_fill_pattern(const struct hl_util_format_info* info, hl_drv_fill_pattern_t* pattern)
{
    uint8_t  format_btye;
    uint8_t* buf;
    format_btye = get_color_format_byte(pattern->format);

    switch (info->format) {

        case RTGRAPHIC_PIXEL_FORMAT_BGR565: {

        } break;
        case RTGRAPHIC_PIXEL_FORMAT_RGB565: {
            uint16_t rgb16 = { HL_MAKE_RGB16(&(info->rgb), pattern->r, pattern->g, pattern->b) };

            buf = (uint8_t*)(pattern->buf);

            rt_kprintf("r=%d,g=%d,b=%d\n", pattern->r, pattern->g, pattern->b);
            rt_kprintf("size = %d \n", pattern->win_size);
            rt_kprintf("add2 = %x \n", buf);
            hl_fill_pix_rgb((uint8_t*)buf, (uint8_t*)&rgb16, pattern->win_size, format_btye);

        }

        break;
        case RTGRAPHIC_PIXEL_FORMAT_RGB888: {

            struct color_rgb24 rgb24 = { HL_MAKE_RGB24(&(info->rgb), pattern->r, pattern->g, pattern->b) };

            buf = (uint8_t*)(pattern->buf);

            rt_kprintf("r=%d,g=%d,b=%d\n", pattern->r, pattern->g, pattern->b);
            rt_kprintf("size = %d \n", pattern->win_size);
            rt_kprintf("add3 = %x \n", buf);

            hl_fill_pix_rgb((uint8_t*)buf, (uint8_t*)&rgb24, pattern->win_size, format_btye);
        }

        break;
        case RTGRAPHIC_PIXEL_FORMAT_ARGB888: {

        }

        break;
        case RTGRAPHIC_PIXEL_FORMAT_ABGR888: {

        } break;
        default:
            break;
    }
}

static void hl_drv_color_fill_pattern(hl_drv_fill_pattern_t* pattern)
{
    const struct hl_util_format_info* info;

    info = hl_util_format_info_find(pattern->format);
    if (info == NULL)
        return;
    hl_color_fill_pattern(info, pattern);
}

static int32_t AlignDown(int32_t value, int32_t align)
{
    return value / align * align;
}


void hl_drv_rm690a0_poweron_seq(void)
{
    // 初始化放在RTTHREEAD的INIT_PREV_EXPORT部分
    // 屏幕上电时序 屏幕相关代码初始化
    hl_hal_gpio_init(GPIO_OLED_RST);
    hl_hal_gpio_low(GPIO_OLED_RST);
    hl_hal_gpio_init(GPIO_OLED_SWIRE);
    hl_hal_gpio_low(GPIO_OLED_SWIRE);
    
    hl_hal_gpio_init(GPIO_OLED_DCX);
    hl_hal_gpio_low(GPIO_OLED_DCX);

    rt_pin_mode(GPIO1_C5, PIN_MODE_OUTPUT);
    rt_pin_write(GPIO1_C5, PIN_LOW);

    rt_pin_mode(GPIO1_C6, PIN_MODE_OUTPUT);
    rt_pin_write(GPIO1_C6, PIN_LOW);
    rt_pin_mode(GPIO1_C4, PIN_MODE_OUTPUT);
    rt_pin_write(GPIO1_C4, PIN_LOW);
}

INIT_PREV_EXPORT(hl_drv_rm690a0_poweron_seq);

static void hl_drv_rm690a0_gpio_init(void)
{    
    rt_thread_mdelay(100);
    hl_drv_rm690a0_hardware_rst();
    rt_thread_mdelay(100);  
}

static void hl_drv_rm690a0_hardware_rst(void)
{
    rt_thread_mdelay(100);
    hl_hal_gpio_low(GPIO_OLED_RST);
    rt_thread_mdelay(400);
    hl_hal_gpio_high(GPIO_OLED_RST);
}

static uint8_t get_color_format_byte(uint8_t format)
{
    uint8_t res = 0;

    switch (format) {
        case RTGRAPHIC_PIXEL_FORMAT_ARGB888:
        case RTGRAPHIC_PIXEL_FORMAT_ABGR888:
            res = 4;
            break;
        case RTGRAPHIC_PIXEL_FORMAT_RGB888:
            res = 3;
            break;
        case RTGRAPHIC_PIXEL_FORMAT_BGR565:
            res = 2;
            break;
        case RTGRAPHIC_PIXEL_FORMAT_RGB565:
            res = 2;
            break;

        default:
            rt_kprintf("get_stride unsupport format:%d\n", format);
            res = 4;
            break;
    }

    return res;
}
static void hl_drv_rm690a0_gpio_deinit(void)
{
    hl_hal_gpio_init(GPIO_OLED_SWIRE);
    hl_hal_gpio_init(GPIO_OLED_RST);

    hl_hal_gpio_low(GPIO_OLED_SWIRE);
    hl_hal_gpio_low(GPIO_OLED_RST);
}

static uint32_t framebuffer_alloc(rt_size_t size)
{
#if defined(RT_USING_LARGE_HEAP)
    return (uint32_t)rt_dma_malloc_large(size);
#else
    return 0;
#endif
}

static void framebuffer_free(void* base)
{
#if defined(RT_USING_LARGE_HEAP)
    rt_dma_free_large(base);
#endif
}

// 显存缓存
void hl_mod_lvgl_video_memory(hl_mod_lvgl_video_mem_t* video_mem_p)
{
    // uint32_t src_w = video_mem_p->x2 - video_mem_p->x1 + 1;
    // uint32_t src_h = video_mem_p->y2 - video_mem_p->y1 + 1;
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
static void hl_drv_vop_win_init(struct CRTC_WIN_STATE* win_config, struct VOP_POST_SCALE_INFO* post_scale)
{

    win_config->winEn  = 1;
    win_config->winId  = 0;
    win_config->zpos   = 0;
    win_config->format = MIPI_OLED_DATA_FMT;

    win_config->yrgbAddr = RT_NULL;
    win_config->cbcrAddr = RT_NULL;

    win_config->yrgbLength = 0;
    win_config->cbcrLength = 0;
    win_config->xVir       = MIPI_OLED_WIDTH;

    win_config->srcX = 0;
    win_config->srcY = 0;
    win_config->srcW = MIPI_OLED_WIDTH;
    win_config->srcH = MIPI_OLED_HEIGHT;

    win_config->crtcX = 0;
    win_config->crtcY = 0;
    win_config->crtcW = MIPI_OLED_WIDTH;
    win_config->crtcH = MIPI_OLED_HEIGHT;

    win_config->xLoopOffset = 0;
    win_config->yLoopOffset = 0;

    win_config->alphaEn     = 0;
    win_config->alphaMode = VOP_ALPHA_MODE_PER_PIXEL;
    win_config->alphaPreMul = VOP_NON_PREMULT_ALPHA;

    post_scale->srcW = win_config->srcW;
    post_scale->srcH = win_config->srcH;
    post_scale->dstX = win_config->srcX;
    post_scale->dstY = win_config->srcY;
    post_scale->dstW = win_config->srcW;
    post_scale->dstH = win_config->srcH;

    rt_device_control(mipi_screen.g_display_dev, RK_DISPLAY_CTRL_SET_SCALE, post_scale);

    win_config->winEn = 0;
    win_config->winUpdate = 1;
    rt_device_control(mipi_screen.g_display_dev, RK_DISPLAY_CTRL_SET_PLANE, win_config);
    
    rt_device_control(mipi_screen.g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);
    
    rt_thread_delay(33);
}

static uint8_t hl_drv_rm690a0_set_bl(uint16_t val)
{
    uint8_t  ret        = RT_ERROR;
    uint16_t brightness = val;
    if (mipi_screen.g_display_dev == RT_NULL) {
        rt_kprintf("mipi set bl err!\r\n");
        return RT_ERROR;
    }
    ret = rt_device_control(mipi_screen.g_display_dev, RK_DISPLAY_CTRL_UPDATE_BL, &brightness);

    return ret;
}

static int32_t hl_drv_rm690a0_malloc(rt_size_t size)
{
#if defined(RT_USING_LARGE_HEAP)
    return (uint32_t)rt_dma_malloc_large(size);
#else
    return 0;
#endif
}
static void hl_drv_rm690a0_free(uint32_t* base)
{
#if defined(RT_USING_LARGE_HEAP)
    rt_dma_free_large((void*)base);
#endif
}
/* Exported functions --------------------------------------------------------*/
#if 0
uint8_t hl_drv_rm690a0_write(uint16_t x_start, uint16_t x_end, uint16_t y_start, uint16_t y_end, const uint8_t* p_pic)
{
    uint8_t ret = RT_ERROR;

    if (p_pic == RT_NULL) {
        rt_kprintf("mipi drv write err!\r\n");
        rt_kprintf("xs1 =%x\r\n", p_pic);
        return RT_ERROR;
    }

    if (mipi_screen.g_display_dev == RT_NULL) {
        rt_kprintf("mipi drv write err!\r\n");
        return RT_ERROR;
    }

    if ((x_start >= x_end) || (y_start >= y_end) || (x_end >= MIPI_OLED_WIDTH) || (y_end >= MIPI_OLED_HEIGHT)) {
        rt_kprintf("mipi drv write err!\r\n");
        rt_kprintf("xs =\r\n");
        return RT_ERROR;
    }

    mipi_screen.win_config->winId = 0;
    mipi_screen.win_config->winEn = 1;

    mipi_screen.win_config->xLoopOffset = 0;
    mipi_screen.win_config->yLoopOffset = 0;
    mipi_screen.win_config->xVir        = MIPI_OLED_WIDTH;
    mipi_screen.win_config->yrgbAddr    = (uint32_t)p_pic;
    mipi_screen.win_config->yrgbLength =
        (uint32_t)(x_end - x_start + 1) * (y_end - y_start + 1) * get_color_format_byte(MIPI_OLED_DATA_FMT);

    
    mipi_screen.win_config->srcX = x_start;
    mipi_screen.win_config->srcY = y_start;
    mipi_screen.win_config->srcW = x_end - x_start + 1;
    mipi_screen.win_config->srcH = y_end - y_start + 1;

    mipi_screen.post_scale->srcW = MIPI_OLED_WIDTH;
    mipi_screen.post_scale->srcH = mipi_screen.win_config->srcH;
    mipi_screen.post_scale->dstX = 0;
    mipi_screen.post_scale->dstY = mipi_screen.win_config->srcY;
    mipi_screen.post_scale->dstW = MIPI_OLED_WIDTH;
    mipi_screen.post_scale->dstH = mipi_screen.win_config->srcH;

    ret |= rt_device_control(mipi_screen.g_display_dev, RK_DISPLAY_CTRL_SET_SCALE, mipi_screen.post_scale);

    ret |= rt_device_control(mipi_screen.g_display_dev, RK_DISPLAY_CTRL_SET_PLANE, mipi_screen.win_config);

    ret |= rt_device_control(mipi_screen.g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);

    struct display_state *disp_state = (struct display_state *)mipi_screen.g_display_dev->user_data;
    if (disp_state->panel_state.display_mode == DISPLAY_CMD_MODE)
    {
		struct display_sync display_sync_data;

        // rt_kprintf(" sync\n");
        display_sync_data.cmd = DISPLAY_SYNC;
        ret = rt_device_control(mipi_screen.g_display_dev, RK_DISPLAY_CTRL_DISPLAY_SYNC, &display_sync_data);
        if (ret != RT_EOK)
            rt_kprintf("rt_display_sync_hook time out\n");
    }

    rt_thread_mdelay(5);

    return ret;
}
#endif 
uint8_t hl_drv_rm690a0_write(hl_mod_lvgl_video_mem_t* video_mem_p)
{
    uint8_t ret = RT_ERROR;

    hl_mod_lvgl_video_memory(video_mem_p);
    mipi_screen.win_config->winId = 0;
    mipi_screen.win_config->winEn = 1;

    mipi_screen.win_config->yrgbAddr    = (uint32_t)video_mem_p->dst;
    mipi_screen.win_config->yrgbLength = mipi_screen.mipi_data_len;

    ret |= rt_device_control(mipi_screen.g_display_dev, RK_DISPLAY_CTRL_SET_SCALE, mipi_screen.post_scale);

    ret |= rt_device_control(mipi_screen.g_display_dev, RK_DISPLAY_CTRL_SET_PLANE, mipi_screen.win_config);

    ret |= rt_device_control(mipi_screen.g_display_dev, RK_DISPLAY_CTRL_COMMIT, NULL);

// 优化写入时间，先注释
#if 0
    struct display_state *disp_state = (struct display_state *)mipi_screen.g_display_dev->user_data;
    if (disp_state->panel_state.display_mode == DISPLAY_CMD_MODE)
    {
		struct display_sync display_sync_data;

        // rt_kprintf(" sync\n");
        display_sync_data.cmd = DISPLAY_SYNC;
        ret = rt_device_control(mipi_screen.g_display_dev, RK_DISPLAY_CTRL_DISPLAY_SYNC, &display_sync_data);
        if (ret != RT_EOK)
            rt_kprintf("rt_display_sync_hook time out\n");
    }
#endif

    return ret;
}

uint8_t hl_drv_rm690a0_io_ctrl(uint8_t cmd, void* ptr, uint32_t len)
{
    uint8_t result = RT_ERROR;
    uint8_t num    = 0;
    switch (cmd) {
        case SET_MIPI_BACKLIGHT_CMD: {
            uint8_t val = *(uint8_t*)ptr;
            if (val > 255) {
                result = RT_EINVAL;
            } else {
                result = hl_drv_rm690a0_set_bl(val);
                result = RT_EOK;
            }
        }

        break;
        // case DISPLAY_FULL_COLOR_CMD: {
        //     hl_drv_fill_pattern_t* color_patten = (hl_drv_fill_pattern_t*)ptr;
        //     if (color_patten != RT_NULL && color_patten->buf != RT_NULL) {

        //         hl_drv_color_fill_pattern(color_patten);

        //         result = hl_drv_rm690a0_write(0, MIPI_OLED_WIDTH - 1, 0, MIPI_OLED_HEIGHT - 1,
        //                                       (const uint8_t*)(color_patten->buf));

        //     } else {
        //         rt_kprintf("rm690a0 err! need create buf first\r\n");
        //         result = RT_ERROR;
        //     }
        // }

        // break;
        case FRAMEBUF_MALLOC_CMD: {

            uint32_t framebufferaddr;
            framebufferaddr = hl_drv_rm690a0_malloc(len);

            if (framebufferaddr == RT_NULL) {
                result          = RT_ERROR;
                *(uint32_t*)ptr = RT_NULL;
            } else {
                result          = RT_EOK;
                *(uint32_t*)ptr = framebufferaddr;
            }
            rt_kprintf("FRAMEBUF_MALLOC_CMD=%x\r\n", framebufferaddr);
        } break;
        case FRAMEBUF_FREE_CMD: {
            if (ptr == RT_NULL) {
                result = RT_EINVAL;
            } else {
                hl_drv_rm690a0_free(ptr);
                result = RT_EOK;
            }
        }

        break;
        default:
            break;
    }

    return result;
}

uint8_t hl_drv_rm690a0_deinit(void)
{
    uint8_t ret = RT_ERROR;

    if (mipi_screen.g_display_dev == RT_NULL) {
        rt_kprintf("mipi drv deinit err!\r\n");
        return RT_ERROR;
    }

    ret = rt_device_control(mipi_screen.g_display_dev, RTGRAPHIC_CTRL_POWEROFF, NULL);

    if (ret != RT_EOK) {
        rt_kprintf("mipi drv deinit err!\r\n");
        return RT_ERROR;
    }

    if (mipi_screen.post_scale == RT_NULL) {
        rt_kprintf("mipi drv rm690a0 err!\r\n");
        return RT_ERROR;
    }

    if (mipi_screen.win_config == RT_NULL) {
        rt_kprintf("mipi drv deinit err!\r\n");
        return RT_ERROR;
    }

    rt_free(mipi_screen.post_scale);
    rt_free(mipi_screen.win_config);

    hl_drv_rm690a0_gpio_deinit();

    return ret;
}

uint8_t hl_drv_rm690a0_init(void)
{
    uint8_t ret = RT_NULL;
    int path = 0;
    struct display_state *state;

    hl_drv_rm690a0_gpio_init();

    mipi_screen.g_display_dev = rt_device_find("lcd");

    if (mipi_screen.g_display_dev == RT_NULL) {
        rt_kprintf("mipi init err 1!\r\n");
        return RT_ERROR;
    }

    ret = rt_device_open(mipi_screen.g_display_dev, RT_DEVICE_OFLAG_RDWR);

    if (ret != RT_EOK) {
        rt_kprintf("mipi init err 2!\r\n");
        return RT_ERROR;
    }

    state = (struct display_state *)mipi_screen.g_display_dev->user_data;
    mipi_screen.dgi = state->graphic_info;

    mipi_screen.dgi.bits_per_pixel = 16;
    mipi_screen.dgi.width          = RT_LV_HOR_RES;
    mipi_screen.dgi.height         = RT_LV_VER_RES;
    
    ret |= rt_device_control(mipi_screen.g_display_dev, RK_DISPLAY_CTRL_AP_COP_MODE, (uint8_t*)1);
    
    if (ret != RT_EOK) {
        rt_kprintf("mipi init err 3!\r\n");
        return RT_ERROR;
    }
	ret = rt_device_control(mipi_screen.g_display_dev, RK_DISPLAY_CTRL_ENABLE, NULL);
	path = SWITCH_TO_INTERNAL_DPHY;
	ret = rt_device_control(mipi_screen.g_display_dev, RK_DISPLAY_CTRL_MIPI_SWITCH, &path);

    if (ret != RT_EOK) {
        rt_kprintf("mipi init err 3!\r\n");
        return RT_ERROR;
    }
    ret = rt_device_control(mipi_screen.g_display_dev, RK_DISPLAY_CTRL_DISABLE, NULL);

	ret = rt_device_control(mipi_screen.g_display_dev, RK_DISPLAY_CTRL_AP_COP_MODE, (uint8_t *)0);
	ret = rt_device_control(mipi_screen.g_display_dev, RTGRAPHIC_CTRL_POWERON, NULL);
    if (ret != RT_EOK) {
        rt_kprintf("mipi init err 3!\r\n");
        return RT_ERROR;
    }

    // 开启正负电压供电
    rt_thread_mdelay(100);
    OLED_PWR_ON();

	mipi_screen.win_config = (struct CRTC_WIN_STATE *)rt_calloc(1, sizeof(struct CRTC_WIN_STATE));



    if (mipi_screen.win_config == RT_NULL) {
        rt_kprintf("mipi init err 6!\r\n");
        return RT_ERROR;
    }


    mipi_screen.post_scale = (struct VOP_POST_SCALE_INFO*)rt_calloc(1, sizeof(struct VOP_POST_SCALE_INFO));

    if (mipi_screen.post_scale == RT_NULL) {
        rt_kprintf("mipi init err 7!\r\n");
        return RT_ERROR;
    }
    

    hl_drv_vop_win_init(mipi_screen.win_config, mipi_screen.post_scale);

    // BUF长度计算：
    mipi_screen.mipi_data_len = (uint32_t)MIPI_OLED_WIDTH*MIPI_OLED_HEIGHT*(uint32_t)get_color_format_byte(MIPI_OLED_DATA_FMT);


    return RT_EOK;
}
#endif /* (!HL_IS_TX_DEVICE()) */
       /*
 * EOF
 */