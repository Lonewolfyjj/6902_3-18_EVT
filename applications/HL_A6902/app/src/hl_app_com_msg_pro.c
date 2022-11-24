/**
 * @file hl_app_com_msg_pro.c
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

#include "hl_mod_euc.h"

#define DBG_SECTION_NAME "app_com"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#if HL_IS_TX_DEVICE()
void hl_app_com_msg_pro(mode_to_app_msg_t* p_msg)
{
    uint8_t  bat_soc_temp     = 50;
    uint8_t  pair_mac_temp[6] = { 0xf4, 0x4e, 0x35, 0x46, 0xff, 0x34 };
    uint8_t  dev_mac_temp[6]  = { 0xf4, 0x4e, 0x35, 0x46, 0xff, 0x35 };
    uint8_t* rx_mac_addr;

    switch (p_msg->cmd) {
        case HL_IN_BOX_IND: {
            LOG_I("in box!\n");
        } break;
        case HL_GET_SOC_REQ_IND: {
            hl_mod_euc_ctrl(HL_SET_SOC_CMD, &bat_soc_temp, sizeof(bat_soc_temp));
        } break;
        case HL_GET_PAIR_MAC_REQ_IND: {  // 获取已经配对的mac地址
            hl_mod_euc_ctrl(HL_SET_PAIR_MAC_CMD, pair_mac_temp, sizeof(pair_mac_temp));
        } break;
        case HL_SET_PAIR_MAC_REQ_IND: {  // 设置rx的mac地址，为有线配对的流程
            rx_mac_addr = (uint8_t*)p_msg->param.ptr;
            LOG_I("rx mac addr write success! [%02x] [%02x] [%02x] [%02x] [%02x] [%02x]\n", rx_mac_addr[0],
                  rx_mac_addr[1], rx_mac_addr[2], rx_mac_addr[3], rx_mac_addr[4], rx_mac_addr[5]);
        } break;
        case HL_GET_MAC_REQ_IND: {  // 获取自己的mac地址
            hl_mod_euc_ctrl(HL_SET_MAC_CMD, dev_mac_temp, sizeof(dev_mac_temp));
        } break;
        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}
#else
void hl_app_com_msg_pro(mode_to_app_msg_t* p_msg)
{
    switch (p_msg->cmd) {
        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}
#endif
/*
 * EOF
 */