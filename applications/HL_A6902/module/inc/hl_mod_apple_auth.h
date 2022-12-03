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

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
#define IAP2_THREAD_STACK_SIZE 1024
#define IAP2_THREAD_PRIORITY 6
#define IAP2_THREAD_TIMESLICE 10
#define EAP_THREAD_STACK_SIZE 1024
#define EAP_THREAD_PRIORITY 7
#define EAP_THREAD_TIMESLICE 10

/* variables -----------------------------------------------------------------*/
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
int hl_mod_apple_auth_init();

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

#endif

/*
 * EOF
 */
