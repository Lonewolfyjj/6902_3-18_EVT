/**
 * @file hl_util_msg_type.h
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-11-14
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
 * <tr><td>2022-11-14     <td>v1.0     <td>luzhanghao     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_UTIL_MSG_TYPE__
#define __HL_UTIL_MSG_TYPE__

/* Includes ------------------------------------------------------------------*/
#include "hl_util_general_type.h"
#include <rtthread.h>

/* typedef -------------------------------------------------------------------*/
/// 模块id枚举
typedef enum _mode_id_e
{
    APP_MODE = 0x00,
    INPUT_MODE,
    DISPLAY_MODE,
    EXT_COM_MODE,
    UPGRADE_MODE,
    APPLE_AUTH_MODE,
    TELINK_MODE,
    AUDIO_MODE,
    PM_MODE,
} HL_ENUM8(hl_mode_id_e);

/// 模块层发送给APP的消息格式
typedef struct _mode_to_app_msg_t
{
    hl_mode_id_e sender;
    uint8_t      cmd;
    uint16_t     len;
    union {
        uint32_t u32_param;
        int32_t  s32_param;
        void*    ptr;
    } param;
} mode_to_app_msg_t;

/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * 
 * @brief 清除消息队列的内容
 * @param [in] app_mq 消息队列指针
 * @date 2023-01-03
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-01-03      <td>yangxianyun     <td>新建
 * </table>
 */
inline void hl_util_msg_queue_clear(struct rt_messagequeue* app_mq)
{
    mode_to_app_msg_t msg = { 0 };
    rt_err_t          err = rt_mq_recv(app_mq, &msg, sizeof(msg), 1);
    while (!err) {
        err = rt_mq_recv(app_mq, &msg, sizeof(msg), 1);
    }
};

#endif /* __HL_UTIL_MSG_TYPE__ */

/*
 * EOF
 */