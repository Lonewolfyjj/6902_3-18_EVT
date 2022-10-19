/**
 * @file hl_drv_touch_test.c
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief  触屏验证函数
 * @version 1.0
 * @date 2022-10-19
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
 * <tr><td>2022-10-19     <td>v1.0     <td>dujunjie     <td>内容
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
#include <rtthread.h>
#include <rtdevice.h>
#include <touch.h>
#include "hl_drv_touch_test.h"
#include "hl_drv_ft3169.h"

static rt_thread_t         touch_tid1 = RT_NULL, touch_tid2 = RT_NULL;
static rt_device_t         touch_dev = RT_NULL;
static struct rt_semaphore touch_sem;

/**
 * 
 * @brief 获取触控屏数据
 * @param [in] parameter 
 * @date 2022-10-19
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-19      <td>dujunjie     <td>新建
 * </table>
 */
static void touch_thread_fun(void* parameter)
{
    struct fts_ts_event touch_pos;
    while (1) {
        rt_sem_take(&touch_sem, (1 << 31));
        rt_device_read(touch_dev, 0, &touch_pos, 1);
        rt_kprintf("touch_pos.type = %d\ttouch_pos.x = %d\ttouch_pos.y = %d\n", touch_pos.type, touch_pos.x, touch_pos.y);
    }
}

/**
 * 
 * @brief 检测触屏设备是否死机，1s一次
 * @param [in] parameter 
 * @date 2022-10-19
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-19      <td>dujunjie     <td>新建
 * </table>
 */
static void touch_thread_esd(void* parameter)
{
    while (1) {
        rt_thread_mdelay(1000);
        rt_device_control(touch_dev, 11, RT_NULL);
    }
}

/**
 * 
 * @brief 触屏事件回调函数
 * @param [in] dev 
 * @param [in] size 
 * @return rt_err_t 
 * @date 2022-10-19
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 不能在中断中使用锁，所以使用信号量记录
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-19      <td>dujunjie     <td>新建
 * </table>
 */
static rt_err_t touch_rx_int_callback(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&touch_sem);
    return RT_EOK;
}

/**
 * 
 * @brief 信号量初始化
 * @date 2022-10-19
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-19      <td>dujunjie     <td>新建
 * </table>
 */
static void touch_sem_init(void)
{
    if (rt_sem_init(&touch_sem, "sem_touch", 0, RT_IPC_FLAG_PRIO) != RT_EOK) {
        rt_kprintf("Sem init fail !\n");
        return;
    }
}

/**
 * 
 * @brief 触屏设备初始化
 * @date 2022-10-19
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-19      <td>dujunjie     <td>新建
 * </table>
 */
static void touch_dev_init(void)
{
    touch_dev = rt_device_find("FT3169");

    if (touch_dev == RT_NULL) {
        rt_kprintf("Can't find touch device FT3169\n");
        return;
    }
    if (rt_device_init(touch_dev) != RT_EOK) {
        rt_kprintf("open touch device init failed!");
        return;
    }
    if (rt_device_open(touch_dev, RT_DEVICE_FLAG_INT_RX) != RT_EOK) {
        rt_kprintf("open touch device failed!");
        return;
    }
    if (rt_device_set_rx_indicate(touch_dev, touch_rx_int_callback) != RT_EOK) {
        rt_kprintf("Touch device int failed!");
        return;
    }
}

/**
 * 
 * @brief 程序入口
 * @param [in] argc 
 * @param [in] argv 
 * @return int 
 * @date 2022-10-19
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-19      <td>dujunjie     <td>新建
 * </table>
 */
static int touch_test_thread(int argc, char** argv)
{
    touch_sem_init();
    touch_dev_init();
    touch_tid1 = rt_thread_create("touch_thread", touch_thread_fun, RT_NULL, 4096, 18, 10);

    touch_tid2 = rt_thread_create("touch_thread_esd", touch_thread_esd, RT_NULL, 4096, 18, 10);

    if (touch_tid1 != RT_NULL) {
        rt_kprintf("Touch thread 1 init success !\n");
        rt_thread_startup(touch_tid1);
    }

    if (touch_tid2 != RT_NULL) {
        rt_kprintf("Touch thread 2 init success !\n");
        rt_thread_startup(touch_tid2);
    }
    return RT_EOK;
}

MSH_CMD_EXPORT(touch_test_thread, touch test thread);