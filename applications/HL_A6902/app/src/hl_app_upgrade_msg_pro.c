/**
 * @file hl_app_upgrade_msg_pro.c
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
/* Includes ------------------------------------------------------------------*/
#include <rtdevice.h>
#include "hl_config.h"
#include "hl_util_msg_type.h"
#include "hl_app_mng.h"
#include "hl_mod_upgrade.h"
#include "hl_mod_telink.h"
#include "hl_mod_audio.h"
#include "hl_mod_display.h"
#include "hl_app_disp_msg_pro.h"
#include "hl_util_nvram.h"
#define DBG_SECTION_NAME "app_upgrade"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
#if HL_IS_TX_DEVICE()
static void hl_app_upgrade_state(hl_mod_upgrade_state upgrade_state)
{
    switch (upgrade_state) {
        case HL_UPGRADE_IDLE_STATE:  /// 空闲升级状态
            tx_info.upgrade_flag = 0;
            break;
        case HL_UPGRADE_START_STATE:  /// 升级开始状态
            tx_info.upgrade_flag = 1;
            break;
        case HL_UPGRADE_UPGRADE_STATE:  /// 升级中状态
            tx_info.upgrade_flag = 1;
            break;
        case HL_UPGRADE_SUCCEED_STATE:  /// 升级成功状态
            tx_info.upgrade_flag = 2;
            // hl_mod_telink_start();
            // hl_mod_audio_init();
            break;
        case HL_UPGRADE_FAIL_STATE:  /// 升级失败状态
            tx_info.upgrade_flag = 3;
            // hl_mod_telink_start();
            // hl_mod_audio_init();
            break;
        default:
            break;
    }

    hl_app_disp_state_led_set();
}
#else
static void hl_app_upgrade_state(hl_mod_upgrade_state upgrade_state)
{
    hl_upgrade_status status;
    switch (upgrade_state) {
        case HL_UPGRADE_IDLE_STATE:  /// 空闲升级状态
            break;
        case HL_UPGRADE_START_STATE:  /// 开始升级状态
            status = HL_UPGRADE_STATUS_UPGRADE;
            LOG_D("HL_UPGRADE_STATUS_UPGRADE");
            hl_mod_display_io_ctrl(UPDATE_STATE_CMD, &status, 0);
            break;
        case HL_UPGRADE_UPGRADE_STATE:  /// 升级中状态
            status = HL_UPGRADE_STATUS_UPGRADE;
            LOG_D("HL_UPGRADE_STATUS_UPGRADE");
            hl_mod_display_io_ctrl(UPDATE_STATE_CMD, &status, 0);
            break;
        case HL_UPGRADE_SUCCEED_STATE:  /// 升级成功状态
            // hl_mod_telink_start();
            // hl_mod_audio_init();
            hl_util_nvram_param_set("MSC_OPEN", "0");
            hl_util_nvram_param_save();
            status = HL_UPGRADE_STATUS_SUCCESS;
            LOG_D("HL_UPGRADE_STATUS_SUCCESS");
            hl_mod_display_io_ctrl(UPDATE_STATE_CMD, &status, 0);
            break;
        case HL_UPGRADE_FAIL_STATE:  /// 升级失败状态

            status = HL_UPGRADE_STATUS_FAIL;
            LOG_D("HL_UPGRADE_STATUS_FAIL");
            hl_mod_display_io_ctrl(UPDATE_STATE_CMD, &status, 0);
            // hl_mod_telink_start();
            // hl_mod_audio_init();
            break;
        default:
            break;
    }
}
#endif
/* Exported functions --------------------------------------------------------*/
#if HL_IS_TX_DEVICE()
void hl_app_upgrade_msg_pro(mode_to_app_msg_t* p_msg)
{
    switch (p_msg->cmd) {
        case HL_UPGRADE_FIND_FW_MSG:  /// 找到升级固件包
            hl_mod_audio_deinit();
            hl_mod_telink_stop();
            // hl_mod_telink_deinit();
            hl_mod_upgrade_io_ctrl(HL_UPGRADE_START_CMD, NULL, 0);
            break;
        case HL_UPGRADE_STATE_MSG:  /// 获取升级状态
            hl_app_upgrade_state(p_msg->param.u32_param);
            break;
        case HL_UPGRADE_VERSIONS_MSG:  /// 获取升级包里面的设备版本号
            /**版本号比较**/
            break;
        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}
#else
void hl_app_upgrade_msg_pro(mode_to_app_msg_t* p_msg)
{
    LOG_I("cmd(%d) !!! ", p_msg->cmd);
    switch (p_msg->cmd) {
        case HL_UPGRADE_FIND_FW_MSG:  /// 找到升级固件包
            // hl_mod_audio_deinit();
            hl_mod_telink_stop();   
            // hl_mod_telink_deinit();        
            hl_mod_upgrade_io_ctrl(HL_UPGRADE_START_CMD, NULL, 0);
            break;
        case HL_UPGRADE_STATE_MSG:  /// 获取升级状态
            hl_app_upgrade_state(p_msg->param.u32_param);
            break;
        case HL_UPGRADE_VERSIONS_MSG:  /// 获取升级包里面的设备版本号
            /**版本号比较**/
            break;
        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}
#endif
/*
 * EOF
 */