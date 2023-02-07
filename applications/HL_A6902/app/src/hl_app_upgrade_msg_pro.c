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
#define DBG_SECTION_NAME "app_upgrade"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
static void hl_app_upgrade_state(hl_mod_upgrade_state upgrade_state)
{
    hl_led_switch led_switch;

    switch (upgrade_state) {
        case HL_UPGRADE_IDLE_STATE:  /// 空闲升级状态
            break;
        case HL_UPGRADE_UPGRADE_STATE:  /// 升级中状态
            led_switch = SWITCH_OPEN;
            hl_mod_display_io_ctrl(LED_SWITCH_UPDATE_CMD, &led_switch, sizeof(led_switch));
            break;
        case HL_UPGRADE_SUCCEED_STATE:  /// 升级成功状态
            led_switch = SWITCH_CLOSE;
            hl_mod_display_io_ctrl(LED_SWITCH_UPDATE_CMD, &led_switch, sizeof(led_switch));
            // hl_mod_telink_start();
            // hl_mod_audio_init();
            break;
        case HL_UPGRADE_FAIL_STATE:  /// 升级失败状态
            led_switch = SWITCH_CLOSE;
            hl_mod_display_io_ctrl(LED_SWITCH_UPDATE_CMD, &led_switch, sizeof(led_switch));
            // hl_mod_telink_start();
            // hl_mod_audio_init();
            break;
        default:
            break;
    }
}
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
#endif
/*
 * EOF
 */