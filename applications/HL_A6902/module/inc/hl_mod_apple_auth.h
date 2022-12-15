/**
 * @file hl_mod_apple_auth.h
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * @brief 
 * @version 1.0
 * @date 2022-09-02
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-02     <td>v1.0     <td>lisonglin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_MOD_APPLE_AUTH_H__
#define __HL_MOD_APPLE_AUTH_H__

/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
#include <rtdevice.h>
#include <stdint.h>
#include "hl_util_iap2.h"
#include "hl_config.h"
#include "stdbool.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/

#define MFI_IC_IIC_ADDR 0x10
#define IAP2_THREAD_STACK_SIZE 1024
#define IAP2_THREAD_PRIORITY 7
#define IAP2_THREAD_TIMESLICE 20
#define EAP_THREAD_STACK_SIZE 1024
#define EAP_THREAD_PRIORITY 7
#define EAP_THREAD_TIMESLICE 20

/* variables -----------------------------------------------------------------*/

typedef struct
{
    /// 线程运行状态标志
    rt_bool_t iap2_thread_flag;
    /// 线程运行状态标志
    rt_bool_t eap_thread_flag;
    /// APP层消息队列
    rt_mq_t* app_msq;
    /// iap2 util功能句柄
    st_iap2_protocol_p iap2_handle;
    /// iap2 util功能句柄
    func_handle iap2_func_handle;
    /// iap2 IIC chips
    struct rt_i2c_bus_device* mfi_chip_iic;
} hl_mod_apple_auth_t;

/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
 * hl_mod_apple_auth_init
 * @brief 苹果认证初始化两个线程，申请相应资源
 * @return int 成功 0 | 失败 非0
 * @date 2022-09-02
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-02      <td>lisonglin     <td>新建
 * </table>
 */
int hl_mod_apple_auth_init(rt_mq_t* input_msq);

/**
 * hl_mod_apple_auth_deinit
 * @brief 苹果认证去初始化，释放资源
 * @return int 成功 0 | 失败 非0
 * @date 2022-09-02
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-02      <td>lisonglin     <td>新建
 * </table>
 */
int hl_mod_apple_auth_deinit();

/**
 * hl_mod_apple_auth_start
 * @brief 启动已初始化的线程，循环执行MFI认证、EAP通信、电源管理
 * @return int 成功 0 | 失败 非0
 * @date 2022-09-02
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-02      <td>lisonglin     <td>新建
 * </table>
 */
int hl_mod_apple_auth_start();

/**
 * hl_mod_apple_auth_stop
 * @brief 停止线程
 * @return int 成功 0 | 失败 非0
 * @date 2022-09-02
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-02      <td>lisonglin     <td>新建
 * </table>
 */
int hl_mod_apple_auth_stop();

void hl_mod_apple_auth_begin();
void hl_mod_apple_auth_end();

#endif

/*
 * EOF
 */
