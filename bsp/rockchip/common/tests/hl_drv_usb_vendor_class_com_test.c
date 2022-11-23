/**
 * @file hl_drv_usb_vendor_class_com_test.c
 * @author yangxianyun (rd52@hollyland-tech.com)
 * @brief 在该文件的内容是USB厂商通信的应用示例示例
 * @version 0.1
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
 * <tr><td>2022-11-09     <td>v1.0     <td>yangxianyun     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/*
 * EOF
 */
#if 0
#include <rtthread.h>
#if defined(RT_USB_DEVICE_WINUSB)
#include <stdio.h>
#include <math.h>
#include <dfs_posix.h>

#include "drv_heap.h"
#include "hl_drv_usb_vendor_class_com.h"

static void winusb_thread(void* p)
{
    hl_drv_usb_vendor_class_com_init();
    uint8_t data[64]     = { 0 };
    uint8_t data_send[]  = { 0xFF, 0x55, 0x02, 0x00, 0x0E, 0x10 };
    uint8_t data_send2[] = { 0xFF, 0x5A, 0x00, 0x1A, 0x80, 0x2B, 0x00, 0x00, 0xE2, 0x01, 0x04, 0x02, 0x00,
                             0x0B, 0xB8, 0x0B, 0xB8, 0x03, 0x03, 0x01, 0x00, 0x01, 0x02, 0x02, 0x01, 0x66 };

    int ret = 0;
    while (1) {
        ret = hl_drv_usb_vendor_class_com_read(data, 64);
        if (ret) {
            rt_kprintf("data length = %d\r\n", ret);
        }

        ret = hl_drv_usb_vendor_class_com_write(data_send, sizeof(data_send));
        ret = hl_drv_usb_vendor_class_com_write(data_send2, sizeof(data_send2));

        rt_thread_mdelay(1000);
    }
}
/**
 * clock demo init
 */
int winusb_app_init(void)
{
    rt_thread_t thread;

    thread = rt_thread_create("clockdark", winusb_thread, RT_NULL, 4096, 5, 10);
    RT_ASSERT(thread != RT_NULL);

    rt_thread_startup(thread);

    return RT_EOK;
}

INIT_APP_EXPORT(winusb_app_init);
#endif
#endif