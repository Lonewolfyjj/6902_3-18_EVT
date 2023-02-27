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
#include "hl_util_apple.h"
#include "hl_util_apple_type.h"
#include "hl_config.h"
#include "stdbool.h"

/* typedef -------------------------------------------------------------------*/

typedef enum _hl_mod_appleauth_ctrl_cmd
{
    /// 开始苹果认证流程：类型无
    HL_APPLE_AUTH_START_CMD = 0x00,
    /// 结束苹果认证流程：类型无
    HL_APPLE_AUTH_STOP_CMD = 0x01,
} HL_ENUM8(hl_mod_appleauth_ctrl_cmd);

typedef enum _hl_mod_appleauth_ctrl_ind
{
    /// 返回认证结果：成功
    HL_APPLE_AUTH_SUCCEED_IND = 0x00,
    /// 返回认证结果：失败
    HL_APPLE_AUTH_FAILED_IND = 0x01,
} HL_ENUM8(hl_mod_appleauth_ctrl_ind);

/* define --------------------------------------------------------------------*/

#define MFI_IC_IIC_ADDR 0x10
#define IAP2_THREAD_STACK_SIZE 4096
#define IAP2_THREAD_PRIORITY 10
#define IAP2_THREAD_TIMESLICE 10
#define EAP_THREAD_STACK_SIZE 4096
#define EAP_THREAD_PRIORITY 10
#define EAP_THREAD_TIMESLICE 10

/* variables -----------------------------------------------------------------*/

typedef struct
{
    /// 线程初始化状态标志
    uint8_t init_flag;
    /// iAP2线程启动状态标志
    uint8_t iap2_start_flag;
    /// EAP线程启动状态标志
    uint8_t eap_start_flag;
    /// 线程运行状态标志
    rt_bool_t iap2_thread_flag;
    /// 线程运行状态标志
    rt_bool_t eap_thread_flag;
    /// APP层消息队列
    rt_mq_t* app_msq;
    /// apple功能句柄
    hl_util_apple_t apple;
    /// apple注册函数句柄
    apple_func_handle func_handle;
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
 * hl_mod_appleauth_ioctl
 * @brief 通过命令控制AppleAuth模块
 * @param [in] cmd 命令
 * @return uint8_t 成功 0 | 失败 1
 * @date 2023-01-11
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-01-11      <td>lisonglin     <td>新建
 * </table>
 */
uint8_t hl_mod_appleauth_ioctl(hl_mod_appleauth_ctrl_cmd cmd);

#endif

/*
 * EOF
 */
