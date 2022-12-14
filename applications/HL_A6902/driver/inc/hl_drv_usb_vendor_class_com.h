/**
 * @file hl_drv_usb_vendor_class_com.h
 * @author yangxianyun (rd52@hollyland-tech.com)
 * @brief USB厂商驱动的读写接口
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
#ifndef __HL_DRV_USB_VENDOR_CLASS_COM_H__
#define __HL_DRV_USB_VENDOR_CLASS_COM_H__
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(RT_USB_DEVICE_WINUSB)
/* typedef -------------------------------------------------------------------*/
typedef struct _hl_drv_usb_vendor_class_com_t_
{
    uint8_t     is_init;
    rt_device_t device;
    rt_sem_t    rx_notice;
    rt_sem_t    tx_notice;
} hl_drv_usb_vendor_class_com_t;
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * 
 * @brief 初始化USB厂商驱动所需要的资源
 * @return uint8_t 
 * @date 2022-11-09
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-09      <td>yangxianyun     <td>新建
 * </table>
 */
uint8_t hl_drv_usb_vendor_class_com_init();

/**
 * 
 * @brief 对USB厂商驱动进行读取操作
 * @param [in] data 读取的数据地址
 * @param [in] data_len 读取的数据长度，<= 64
 * @return uint8_t 
 * @date 2022-11-09
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-09      <td>yangxianyun     <td>新建
 * </table>
 */
uint8_t hl_drv_usb_vendor_class_com_read(uint8_t* data, uint8_t data_len, uint16_t timeout);

/**
 * 
 * @brief TBD
 * @param [in] cmd 
 * @param [in] data 
 * @param [in] len 
 * @return uint8_t 
 * @date 2022-11-09
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-09      <td>yangxianyun     <td>新建
 * </table>
 */
uint8_t hl_drv_usb_vendor_class_com_ioctl(uint8_t cmd, uint8_t* data, uint16_t len);

/**
 * 
 * @brief USB厂商驱动的写入接口
 * @param [in] data 写入的数据地址
 * @param [in] data_len 写入的数据长度 <= 64
 * @return uint8_t 
 * @date 2022-11-09
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-09      <td>yangxianyun     <td>新建
 * </table>
 */
uint8_t hl_drv_usb_vendor_class_com_write(uint8_t* data, uint8_t data_len);

/**
 * 
 * @brief USB厂商设备的去初始化接口，释放初始化申请的资源
 * @return uint8_t 
 * @date 2022-11-09
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-09      <td>yangxianyun     <td>新建
 * </table>
 */
uint8_t hl_drv_usb_vendor_class_com_deinit();

#endif

#ifdef __cplusplus
}
#endif
#endif  //__HL_DRV_USB_VENDOR_CLASS_COM_H__
/*
 * EOF
 */

