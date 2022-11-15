#include "hl_hal_gpio.h"
#include <rtthread.h>
#include "hl_drv_rm69310.h"
#include "hl_two_img.h"
#include "hal_base.h"
#include "hl_one_img.h"
#include <rtdevice.h>
// #include "hl_drv_rm69310.h"
// #include "hl_mod_display_lvgl.h"

#if (!HL_GET_DEVICE_TYPE())
#if 0
static uint16_t databuf[OLED_WIDTH*OLED_HEIGHT] = {0};
static int hl_rx_oled_test_init(void);
static int hl_rx_oled_test_deinit(void);

int hl_mod_test_rx_init(void)
{
    uint32_t i = 0;
    rt_kprintf("test_rx_init/r/n");

    // SYS_3.3V
    hl_hal_gpio_init(GPIO_PWR_EN);
    hl_hal_gpio_init(GPIO_OLED_SWIRE);
    hl_hal_gpio_low(GPIO_PWR_EN);
    hl_hal_gpio_low(GPIO_OLED_SWIRE);
    //rt_thread_mdelay(5);
    for (i=0;i<2000000;i++ ) {
        
    }
    hl_hal_gpio_high(GPIO_PWR_EN);
    for (i=0;i<2000000;i++ ) {

    }
    //rt_thread_mdelay(5); // board init时不能用这个
    //hl_hal_gpio_high(GPIO_OLED_SWIRE);



    // 2831p初始化
    hl_hal_gpio_init(GPIO_ATS_PWR_EN);

    hl_hal_gpio_init(GPIO_RF_PWR_EN);


    hl_hal_gpio_high(GPIO_ATS_PWR_EN);
    hl_hal_gpio_high(GPIO_RF_PWR_EN);

    return RT_EOK;
}

INIT_BOARD_EXPORT(hl_mod_test_rx_init);


static void hl_hal_gpio_test_irq_process(void *args)
{
    //uint8_t data = *(uint8_t *)args;
    rt_kprintf("%d\r\n", rt_tick_get());
}
static hl_gpio_pin_e pinnum = 11;

int oled_test(int argc, char** argv)
{
    //uint32_t i = 0;
    uint16_t color;
    
    if (argc == 1) {
        rt_kprintf("oled_write \r\n");
        hl_drv_rm69310_init();
        hl_hal_gpio_init(GPIO_OLED_TE);
        hl_hal_gpio_attach_irq(GPIO_OLED_TE, PIN_IRQ_MODE_RISING, hl_hal_gpio_test_irq_process, &pinnum);
        hl_hal_gpio_irq_enable(GPIO_OLED_TE, PIN_IRQ_DISABLE);
        rt_kprintf("gpio set irq completed\r\n");

        // color = RGB565_BLUE;
        // hl_drv_rm69310_io_ctrl(SET_COLOR, &color, 2);
        // rt_thread_delay(2000);

        // color = RGB565_GREEN;
        // hl_drv_rm69310_io_ctrl(SET_COLOR, &color, 2);
        // rt_thread_delay(2000);

        // color = RGB565_RED;
        // hl_drv_rm69310_io_ctrl(SET_COLOR, &color, 2);
        // rt_thread_delay(2000);

        // color = RGB565_WHITE;
        // hl_drv_rm69310_io_ctrl(SET_COLOR, &color, 2);
        // rt_thread_delay(2000);

        // color = RGB565_BLACK;
        // hl_drv_rm69310_io_ctrl(SET_COLOR, &color, 2);
        // rt_thread_delay(2000);

        hl_drv_rm69310_display_write(0, 239, 0, 119, gImage_hl_two_img);
        rt_thread_delay(2000);
        hl_drv_rm69310_display_write(0, 239, 0, 119, gImage_hl_one_img);
        rt_thread_delay(2000);
        hl_drv_rm69310_display_write(0, 239, 0, 119, gImage_hl_two_img);  

    } else if (!strcmp(argv[1], "close")) {
        // hl_hal_gpio_low(GPIO_PWR_EN);
        // for (i=0;i<2000000;i++ ) {
        
        // }
        // hl_hal_gpio_low(GPIO_OLED_SWIRE);
        hl_drv_rm69310_deinit();
    } else if (!strcmp(argv[1], "off")) {
        color = 0;
        hl_drv_rm69310_io_ctrl(CTRL_POWER, &color, 0);
    } else if (!strcmp(argv[1], "on")) {
        color = 1;
        hl_drv_rm69310_io_ctrl(CTRL_POWER, &color, 0);
    } else if (!strcmp(argv[1], "bl")) {
        color = atoi(argv[2]);
        hl_drv_rm69310_io_ctrl(SET_BACKLIGHT, &color, 1);
    } else if (!strcmp(argv[1], "pc")) {
        hl_drv_rm69310_display_write(0, 239, 0, 119, gImage_hl_one_img);
    } else if (!strcmp(argv[1], "thread")) {
        rt_kprintf("oled_test thread\r\n");
        hl_rx_oled_test_init();
    } else if (!strcmp(argv[1], "threaddel")) {
        
        hl_rx_oled_test_deinit();
    } else if (!strcmp(argv[1], "lvgl_init")) {
        // hl_mod_display_lvgl_init();
    } else {
        rt_kprintf("wrong parameter, please type: oled_test \r\n");
    }
    return RT_EOK;
}
MSH_CMD_EXPORT(oled_test, oled test);



static void screem_rgb_display(void)
{
    static uint16_t now_color = RGB565_RED;


    for (uint32_t i = 0; i < OLED_WIDTH*OLED_HEIGHT; i++) {
        databuf[i] = now_color;
    }

    // hl_drv_rm69310_display_write(0, OLED_WIDTH - 1, 0, OLED_HEIGHT - 1, (const uint8_t*)databuf);
    hl_drv_rm69310_display_write(0, 10, 0, 10, (const uint8_t*)databuf);

    hl_drv_rm69310_display_write(200, 230, 80, 110, (const uint8_t*)databuf);

    hl_drv_rm69310_display_write(120, 130, 60, 70, (const uint8_t*)databuf);

    hl_drv_rm69310_display_write(140, 145, 20, 25, (const uint8_t*)databuf);

    hl_drv_rm69310_display_write(15, 30, 100, 115, (const uint8_t*)databuf);

    hl_drv_rm69310_display_write(OLED_WIDTH - 10, OLED_WIDTH - 1, 0, 10, (const uint8_t*)databuf);

    if (now_color == RGB565_RED) {
        now_color = RGB565_GREEN;
    } else if (now_color == RGB565_GREEN) {
        now_color = RGB565_BLUE;        
    } else if (now_color == RGB565_BLUE) {
        now_color = RGB565_RED;
    }
}

#define PM_THREAD_PRIORITY 20
#define PM_THREAD_STACK_SIZE 512
#define PM_THREAD_TIMESLICE 10

static void hl_mod_test_display_thread_init(void* param)
{
    hl_drv_rm69310_init();
    rt_thread_mdelay(100);

    for (uint32_t i = 0; i < OLED_WIDTH*OLED_HEIGHT; i++) {
        databuf[i] = RGB565_BLACK;
    }

    hl_drv_rm69310_display_write(0, OLED_WIDTH - 1, 0, OLED_HEIGHT - 1, (const uint8_t*)databuf);

    while (1) {

        screem_rgb_display();
        rt_thread_mdelay(2000);
    }
}

static rt_thread_t pm_tid =RT_NULL;
static int hl_rx_oled_test_init(void)
{
    pm_tid = rt_thread_create("oled_thread", \
                                hl_mod_test_display_thread_init, \
                                RT_NULL, \
                                PM_THREAD_STACK_SIZE, \
                                PM_THREAD_PRIORITY, \
                                PM_THREAD_TIMESLICE);

    if (pm_tid != RT_NULL) {
        if (pm_tid->error != RT_EOK) {
            rt_kprintf("oled thread init err!");
            rt_thread_delete(pm_tid);
        }
        rt_thread_startup(pm_tid);
    } else {
        rt_kprintf("oled thread init err!");
        return -1;
    }
    return RT_EOK;
}
static int hl_rx_oled_test_deinit(void)
{
    hl_drv_rm69310_deinit();

    if (pm_tid != RT_NULL) {
       
        rt_kprintf("oled thread close !");
        rt_thread_delete(pm_tid);
    } else {
        rt_kprintf("oled thread init err!");
        return -1;
    }
    return RT_EOK;
}

static int spi_cfg_init(void)
{
    hl_drv_rm69310_init();
    rt_thread_mdelay(100);
    rt_kprintf("spi_cfg_init success \n\n");
    return 0;
}

INIT_APP_EXPORT(spi_cfg_init);

#endif

#endif