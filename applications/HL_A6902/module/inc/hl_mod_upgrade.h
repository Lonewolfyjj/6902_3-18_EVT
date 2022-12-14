/**
 * @file hl_mod_upgrade.h
 * @author lixiang (rd37@hollyland-tech.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-22
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-11-22     <td>v1.0     <td>lixiang     <td>内容
 * </table>
 * 
 */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_MOD_UPGRADE_H__
#define __HL_MOD_UPGRADE_H__

/* Includes ------------------------------------------------------------------*/
#include "hl_config.h"


/* typedef -------------------------------------------------------------------*/
typedef enum _hl_mod_upgrade_ctrl_cmd
{
    /// 打开升级进程，进行升级文件查找。 参数<无>
    HL_UPGRADE_OPEN_CMD = 0x00,
    /// 关闭升级进程。 参数<无>
    HL_UPGRADE_CLOSE_CMD,
    /// 开始升级。 参数<hl_mod_upgrade_device>
    HL_UPGRADE_START_CMD,
    /// 获取升级状态。 参数<hl_mod_upgrade_state>
    HL_UPGRADE_STATE_CMD,
    /// 获取升级包里面的设备版本号。 参数<hl_mod_upgrade_device>(需要获取的设备)<uint8_t[4]>(版本号) 
    HL_UPGRADE_GET_VERSIONS_CMD,
} hl_mod_upgrade_ctrl_cmd;

typedef enum _hl_mod_upgrade_msg_cmd
{
    /// 找到升级固件包。 参数<无>
    HL_UPGRADE_FIND_FW_MSG = 0x00,
    /// 获取升级状态。 参数<无>
    HL_UPGRADE_STATE_MSG,
    /// 获取升级包里面的设备版本号。 参数<hl_mod_upgrade_device>(需要获取的设备)<uint8_t[4]>(版本号) 
    HL_UPGRADE_VERSIONS_MSG,
} hl_mod_upgrade_msg_cmd;


typedef enum _hl_mod_upgrade_device
{
    /// 升级RK2108
    HL_UPGRADE_RK = 0x00,
    /// 升级无线telink
    HL_UPGRADE_TELINK,  

    /// 设备升级个数 (不能当成设备使用)
    HL_UPGRADE_MAX,
} hl_mod_upgrade_device;

typedef enum _hl_mod_upgrade_state
{
    /// 空闲升级状态
    HL_UPGRADE_IDLE_STATE = 0x00,
    /// 升级中状态
    HL_UPGRADE_UPGRADE_STATE,  
    /// 升级成功状态
    HL_UPGRADE_SUCCEED_STATE, 
    /// 升级失败状态
    HL_UPGRADE_FAIL_STATE, 
} hl_mod_upgrade_state;



/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/


/**
 * 
 * @brief init upgrade config
 * @param [in] p_msg_handle 上层传入的消息队列句柄
 * @return uint8_t 0:succeed  1:error
 * @date 2022-09-13
 * @author lixiang (rd37@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-13      <td>lixiang     <td>新建
 * </table>
 */
uint8_t hl_mod_upgrade_init(void *p_msg_handle);


/**
 * 
 * @brief deinit upgrade config
 * @return uint8_t 0:succeed  1:error
 * @date 2022-09-13
 * @author lixiang (rd37@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-13      <td>lixiang     <td>新建
 * </table>
 */
uint8_t hl_mod_upgrade_deinit();

/**
 * 
 * @brief upgrade config
 * @param [in] cmd 命令
 * @param [in] ptr 参数指针
 * @param [in] len 参数长度
 * @return uint8_t 0:succeed  1:error
 * @date 2022-09-13
 * @author lixiang (rd37@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-13      <td>lixiang     <td>新建
 * </table>
 */
uint8_t hl_mod_upgrade_io_ctrl(uint8_t cmd, void *ptr, uint16_t len);

#endif /* __HL_MOD_UPGRADE_H__ */
/*
 * EOF
 */