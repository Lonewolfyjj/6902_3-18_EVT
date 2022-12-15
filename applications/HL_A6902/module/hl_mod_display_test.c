/**
 * @file hl_mod_display_test.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 显示的测试程序
 * @version V1.0
 * @date 2022-11-09
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
 * <tr><td>2022-11-09     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "hl_hal_gpio.h"
#include <rtthread.h>
#include "hl_drv_rm69310.h"
#include "hl_two_img.h"
#include "hal_base.h"
#include "hl_one_img.h"
#include <rtdevice.h>
#include "hl_drv_rm690a0.h"
#include "hl_mod_display.h"
#include "hl_util_msg_type.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/


// #include "hl_drv_rm69310.h"
// #include "hl_mod_display_lvgl.h"
/// 
/*0 : 默认就是mipi屏幕,1：spi屏幕*/
#define HL_DISPLAY_SCREEN_HARDWARE      0


#if (!HL_IS_TX_DEVICE())

#if (HL_DISPLAY_SCREEN_HARDWARE)


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
    // hl_hal_gpio_init(GPIO_ATS_PWR_EN);

    hl_hal_gpio_init(GPIO_RF_PWR_EN);


    // hl_hal_gpio_high(GPIO_ATS_PWR_EN);
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
#else
// RX MIPI屏幕的测试代码
int hl_mod_display_test_cmd(int argc, char** argv)
{
    static mode_to_app_msg_t msg;

    if (argc == 1) {
        rt_kprintf("display_test \r\n");

    } else if (!strcmp(argv[1], "init")) {
        rt_kprintf("display_test thread init\r\n");
    } else if (!strcmp(argv[1], "cmd")) {
        rt_kprintf("display_test thread\r\n");
        msg.cmd             = atoi(argv[2]);
        msg.len             = 4;
        msg.param.u32_param = atoi(argv[3]);

        rt_kprintf("u32_param=%d\r\n", msg.param.u32_param);
        rt_thread_mdelay(1000);
        hl_mod_display_io_ctrl(msg.cmd, (void*)&msg.param.u32_param, msg.len);

    } else {
        rt_kprintf("wrong parameter, please type: oled_test \r\n");
    }

    return RT_EOK;
}
MSH_CMD_EXPORT(hl_mod_display_test_cmd, display cmd test);
#endif 
#else
// TX display mod test

struct rt_messagequeue hl_mq;
static char            hl_msg_pool[2048];
static rt_thread_t     pm_tid = RT_NULL;

#define PM_THREAD_PRIORITY 20
#define PM_THREAD_STACK_SIZE 512
#define PM_THREAD_TIMESLICE 10

static void hl_mod_display_test_thread_init(void* param)
{
    uint8_t           res         = 0;
    mode_to_app_msg_t hl_msg_test = *(mode_to_app_msg_t*)param;

    rt_kprintf("start display thread!\r\n");

    rt_kprintf("cmd=%d\r\n", hl_msg_test.cmd);
    rt_kprintf("u32_param=%d\r\n", hl_msg_test.param.u32_param);
    rt_thread_mdelay(4000);
    hl_mod_display_io_ctrl(hl_msg_test.cmd, (void*)&hl_msg_test.param, hl_msg_test.len);

    while (1) {

        rt_thread_mdelay(100);
    }
}

int hl_mod_display_test_init(int argc, char** argv)
{
    static mode_to_app_msg_t msg;

    if (argc == 1) {
        rt_kprintf("display_test \r\n");

    } else if (!strcmp(argv[1], "cmd")) {
        rt_kprintf("display_test thread\r\n");
        msg.cmd             = atoi(argv[2]);
        msg.len             = 4;
        msg.param.u32_param = atoi(argv[3]);

    } else {
        rt_kprintf("wrong parameter, please type: oled_test \r\n");
    }

    rt_mq_init(&hl_mq, "AppMsg", &hl_msg_pool[0], 128 - sizeof(void*), sizeof(hl_msg_pool), RT_IPC_FLAG_FIFO);

    hl_mod_display_init((void*)&hl_mq);
    rt_kprintf("test!\r\n");

    pm_tid = rt_thread_create("oled_thread", hl_mod_display_test_thread_init, (void*)&msg, PM_THREAD_STACK_SIZE,
                              PM_THREAD_PRIORITY, PM_THREAD_TIMESLICE);

    if (pm_tid != RT_NULL) {
        rt_kprintf("oled thread init ok!\r\n");
        rt_thread_startup(pm_tid);
    } else {
        rt_kprintf("oled thread init err!\r\n");
        return -1;
    }

    return RT_EOK;
}
MSH_CMD_EXPORT(hl_mod_display_test_init, display test);

int hl_mod_display_test_cmd(int argc, char** argv)
{
    static mode_to_app_msg_t msg;

    if (argc == 1) {
        rt_kprintf("display_test \r\n");

    } else if (!strcmp(argv[1], "init")) {
        rt_kprintf("display_test thread init\r\n");

        rt_mq_init(&hl_mq, "AppMsg", &hl_msg_pool[0], 128 - sizeof(void*), sizeof(hl_msg_pool), RT_IPC_FLAG_FIFO);

        hl_mod_display_init((void*)&hl_mq);

        rt_kprintf("test!\r\n");

    } else if (!strcmp(argv[1], "cmd")) {
        rt_kprintf("display_test thread\r\n");
        msg.cmd             = atoi(argv[2]);
        msg.len             = 4;
        msg.param.u32_param = atoi(argv[3]);

        rt_kprintf("u32_param=%d\r\n", msg.param.u32_param);
        rt_thread_mdelay(1000);
        hl_mod_display_io_ctrl(msg.cmd, (void*)&msg.param.u32_param, msg.len);

    } else if (!strcmp(argv[1], "ui")) {
        rt_kprintf("hl_mod_display_test_cmd ui\r\n");
        msg.cmd             = atoi(argv[2]);
        msg.len             = atoi(argv[3]);
        msg.param.u32_param = atoi(argv[4]);
        hl_mod_display_io_ctrl(msg.cmd, (void*)&msg.param.u32_param, msg.len);

    } else {
        rt_kprintf("wrong parameter, please type: hl_mod_display_test_cmd \r\n");
    }

    return RT_EOK;
}
MSH_CMD_EXPORT(hl_mod_display_test_cmd, display cmd test);

#endif
/*
 * EOF
 */