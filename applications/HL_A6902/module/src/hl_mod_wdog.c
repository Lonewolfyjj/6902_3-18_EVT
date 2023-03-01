/**
 * @file hl_mod_wdog.c
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief  看门狗模块
 * @version 1.0
 * @date 2022-11-07
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
 * <tr><td>2022-11-07     <td>v1.0     <td>dujunjie     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
#include <rtdevice.h>
#include "hl_mod_wdog.h"
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
static uint8_t     feed_dog_flag = 0;
static rt_device_t wdg_dev; /* 看门狗设备句柄 */
/* Private function(only *.c)  -----------------------------------------------*/
/**
 * 
 * @brief 看门狗空闲喂狗钩子函数
 * @date 2022-11-07
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-07      <td>dujunjie     <td>新建
 * </table>
 */
static void hl_mod_wdog_idle_hook(void)
{
    /* 在空闲线程的回调函数里喂狗 */
    rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
}

void hl_mod_feed_dog(void)
{
    // rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
}

/**
 * 
 * @brief 看门狗初始化函数，系统自动调用
 * @return int 
 * @date 2022-11-07
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-07      <td>dujunjie     <td>新建
 * </table>
 */
static int hl_mod_wdog_init(void)
{
    rt_err_t    ret     = RT_EOK;
    rt_uint32_t timeout = WDT_DEVICE_TIMEOUT;

    /* 根据设备名称查找看门狗设备，获取设备句柄 */
    wdg_dev = rt_device_find(WDT_DEVICE_NAME);
    if (!wdg_dev) {
        rt_kprintf("find %s failed!\n", WDT_DEVICE_NAME);
        return RT_ERROR;
    }

    /* 初始化看门狗 */
    if (rt_device_init(wdg_dev) != RT_EOK) {
        rt_kprintf("Init wdog %s failed!\n", WDT_DEVICE_NAME);
        return RT_ERROR;
    }

    /* 设置看门狗溢出时间 */
    ret = rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &timeout);
    if (ret != RT_EOK) {
        rt_kprintf("set %s timeout failed!\n", WDT_DEVICE_NAME);
        return RT_ERROR;
    }
    /* 启动看门狗 */
    ret = rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_START, RT_NULL);
    if (ret != RT_EOK) {
        rt_kprintf("start %s failed!\n", WDT_DEVICE_NAME);
        return -RT_ERROR;
    }
    /* 设置空闲线程回调函数 */
    // rt_thread_idle_sethook(hl_mod_wdog_idle_hook);

    return ret;
}

// INIT_ENV_EXPORT(hl_mod_wdog_init);

/**
 * 
 * @brief 看门狗空闲喂狗钩子函数（测试时使用）
 * @date 2022-11-07
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 使用测试函数时应该注释掉 106行 INIT_ENV_EXPORT(hl_mod_wdog_init);
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-07      <td>dujunjie     <td>新建
 * </table>
 */
static void hl_mod_wdog_idle_hook_test(void)
{
    /* 在空闲线程的回调函数里喂狗 */
    if (feed_dog_flag) {
        rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
    }
}

/**
 * 
 * @brief 看门狗初始化函数（测试）
 * @param [in] argc 
 * @param [in] argv 0:关闭喂狗功能  1：开启喂狗功能
 * @return int 
 * @date 2022-11-07
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * 
 * @details 使用测试函数时应该注释掉 106行 INIT_ENV_EXPORT(hl_mod_wdog_init);
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-07      <td>dujunjie     <td>新建
 * </table>
 */
static int hl_mod_wdog_test(int argc, char** argv[])
{
    rt_err_t    ret     = RT_EOK;
    rt_uint32_t timeout = 1; /* 溢出时间，单位：秒 */

    feed_dog_flag = atoi(argv[1]);

    /* 根据设备名称查找看门狗设备，获取设备句柄 */
    wdg_dev = rt_device_find(WDT_DEVICE_NAME);
    if (!wdg_dev) {
        rt_kprintf("find %s failed!\n", WDT_DEVICE_NAME);
        return RT_ERROR;
    }

    /* 初始化看门狗 */
    if (rt_device_init(wdg_dev) != RT_EOK) {
        rt_kprintf("Init wdog %s failed!\n", WDT_DEVICE_NAME);
        return RT_ERROR;
    }

    /* 设置看门狗溢出时间 */
    ret = rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &timeout);
    if (ret != RT_EOK) {
        rt_kprintf("set %s timeout failed!\n", WDT_DEVICE_NAME);
        return RT_ERROR;
    }
    /* 启动看门狗 */
    ret = rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_START, RT_NULL);
    if (ret != RT_EOK) {
        rt_kprintf("start %s failed!\n", WDT_DEVICE_NAME);
        return -RT_ERROR;
    }
    /* 设置空闲线程回调函数 */
    rt_thread_idle_sethook(hl_mod_wdog_idle_hook_test);

    return ret;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(hl_mod_wdog_test, hl_mod_wdog_test);

/* Exported functions --------------------------------------------------------*/
/*
 * EOF
 */