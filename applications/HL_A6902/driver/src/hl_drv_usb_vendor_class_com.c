/**
 * @file hl_drv_usb_vendor_class_com.c
 * @author yangxianyun (rd52@hollyland-tech.com)
 * @brief 该文件实现了具体的USB厂商驱动的读写和初始化相关的内容
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
#include "hl_drv_usb_vendor_class_com.h"
#include <rtthread.h>
#if defined(RT_USB_DEVICE_WINUSB)

#include <stdio.h>
#include <math.h>
#include <dfs_posix.h>
#include "drv_heap.h"
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/

static hl_drv_usb_vendor_class_com_t* handle = NULL;

/* Private function(only *.c)  -----------------------------------------------*/

static rt_err_t _hl_drv_usb_vendor_class_com_rx_ind(rt_device_t dev, rt_size_t size)
{

    // rt_kprintf("_hl_drv_usb_vendor_class_com_rx_ind size: %d \n", size);
    if (size) {
        /*release the sem */
        rt_sem_release(handle->rx_notice);
    }

    return RT_EOK;
}

static rt_err_t _hl_drv_usb_vendor_class_com_tx_ind(rt_device_t dev, void* buffer)
{
    rt_sem_release(handle->tx_notice);
    return RT_EOK;
}
/* Exported functions --------------------------------------------------------*/

uint8_t hl_drv_usb_vendor_class_com_init()
{
    rt_err_t result;

    if (handle) {
        return 1;
    }

    handle         = rt_malloc(sizeof(hl_drv_usb_vendor_class_com_t));
    handle->device = rt_device_find("winusb");
    if (NULL == handle->device) {
        // can't find device
        hl_drv_usb_vendor_class_com_init();
        return 2;
    }

    result = rt_device_open(handle->device, RT_DEVICE_OFLAG_RDWR);
    RT_ASSERT(result == RT_EOK);

    handle->rx_notice = rt_sem_create("winusb_rx", 1, RT_IPC_FLAG_FIFO);
    handle->tx_notice = rt_sem_create("winusb_tx", 0, RT_IPC_FLAG_FIFO);
    if (handle->rx_notice == RT_NULL || handle->tx_notice == RT_NULL) {
        rt_kprintf("create dynamic semaphore failed.\n");
        return 3;
    } else {
        rt_kprintf("create done. dynamic semaphore value = 0.\n");
    }

    rt_device_set_tx_complete(handle->device, _hl_drv_usb_vendor_class_com_tx_ind);
    rt_device_set_rx_indicate(handle->device, _hl_drv_usb_vendor_class_com_rx_ind);

    handle->is_init = 1;
    return 0;
}

uint8_t hl_drv_usb_vendor_class_com_read(uint8_t* data, uint8_t data_len, uint16_t timeout)
{
    int size = rt_device_read(handle->device, 0, data, data_len);
    int ret  = rt_sem_take(handle->rx_notice, timeout);

    if (ret != 0) {
        // rt_kprintf("read data error = %d\r\n", data_len);
        return 0;
    }

    return (uint8_t)size;
}

uint8_t hl_drv_usb_vendor_class_com_ioctl(uint8_t cmd, uint8_t* data, uint16_t len)
{
    return 0;
}

uint8_t hl_drv_usb_vendor_class_com_write(uint8_t* data, uint8_t data_len)
{
    uint8_t result = 0;
    if (rt_device_write(handle->device, 0, data, data_len) == data_len) {
        /*Must wait the usb send data finish */
        result = rt_sem_take(handle->tx_notice, RT_WAITING_FOREVER);
    } else {
        result = RT_ERROR;
    }
    return result;
}

uint8_t hl_drv_usb_vendor_class_com_deinit()
{
    if (RT_NULL == handle) {
        return 1;
    }

    rt_free(handle);
    handle          = NULL;
    handle->is_init = 0;
    if (handle->device) {
        rt_device_close(handle->device);
    }

    if (handle->rx_notice) {
        rt_sem_delete(handle->rx_notice);
    }

    if (handle->tx_notice) {
        rt_sem_delete(handle->tx_notice);
    }

    return 0;
}

#endif
/*
 * EOF
 */