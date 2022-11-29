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
    uint8_t                   bat_soc_temp     = 50;
    uint8_t                   pair_mac_temp[6] = { 0xf4, 0x4e, 0x35, 0x46, 0xff, 0x34 };
    uint8_t                   dev_mac_temp[6]  = { 0xf4, 0x4e, 0x35, 0x46, 0xff, 0x35 };
    uint8_t*                  rx_mac_addr;
    hl_mod_euc_charge_state_e charge_state_temp = HL_MOD_EUC_CHARGE_STATE_CHARGING;

    switch (p_msg->cmd) {
        case HL_IN_BOX_IND: {
            LOG_I("in box!\n");
        } break;
        case HL_GET_SOC_REQ_IND: {
            hl_mod_euc_ctrl(HL_SET_SOC_CMD, &bat_soc_temp, sizeof(bat_soc_temp));
        } break;
        case HL_GET_PAIR_MAC_REQ_IND: {  // 请求获取已经配对的mac地址
            hl_mod_euc_ctrl(HL_SET_PAIR_MAC_CMD, pair_mac_temp, sizeof(pair_mac_temp));
        } break;
        case HL_SET_PAIR_MAC_REQ_IND: {  // 请求设置rx的mac地址，为有线配对的流程
            rx_mac_addr = (uint8_t*)p_msg->param.ptr;
            LOG_I("rx mac addr write success! [%02x] [%02x] [%02x] [%02x] [%02x] [%02x]\n", rx_mac_addr[0],
                  rx_mac_addr[1], rx_mac_addr[2], rx_mac_addr[3], rx_mac_addr[4], rx_mac_addr[5]);
        } break;
        case HL_GET_MAC_REQ_IND: {  // 请求获取自己的mac地址
            hl_mod_euc_ctrl(HL_SET_MAC_CMD, dev_mac_temp, sizeof(dev_mac_temp));
        } break;
        case HL_GET_CHARGE_STATE_REQ_IND: {  // 请求获取充电状态
            hl_mod_euc_ctrl(HL_SET_CHARGE_STATE_CMD, &charge_state_temp, sizeof(charge_state_temp));
        } break;
        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}
#else
void hl_app_com_msg_pro(mode_to_app_msg_t* p_msg)
{
    uint8_t                   bat_soc_temp = 50;
    uint8_t                   tx1_bat_info_temp;
    uint8_t                   tx2_bat_info_temp;
    uint8_t                   box_bat_info_temp;
    uint8_t*                  tx1_mac_addr;
    uint8_t*                  tx2_mac_addr;
    uint8_t                   dev_mac_temp[6] = { 0xf4, 0x4e, 0x35, 0x46, 0xff, 0x33 };
    bool                      tx1_pair_result_temp;
    bool                      tx2_pair_result_temp;
    hl_mod_euc_charge_state_e charge_state_temp = HL_MOD_EUC_CHARGE_STATE_CHARGING;
    hl_mod_euc_charge_state_e tx1_charge_state_temp;
    hl_mod_euc_charge_state_e tx2_charge_state_temp;
    hl_mod_euc_charge_state_e box_charge_state_temp;

    switch (p_msg->cmd) {
        case HL_IN_BOX_IND: {
            LOG_I("in box!\n");
        } break;
        case HL_GET_SOC_REQ_IND: {  // 请求获取电池电量
            hl_mod_euc_ctrl(HL_SET_SOC_CMD, &bat_soc_temp, sizeof(bat_soc_temp));
        } break;
        case HL_TX1_BAT_INFO_UPDATE_IND: {  //更新tx1电池电量
            tx1_bat_info_temp = *(uint8_t*)p_msg->param.ptr;
            LOG_I("Tx1 bat soc update: %d", tx1_bat_info_temp);
        } break;
        case HL_TX2_BAT_INFO_UPDATE_IND: {  //更新tx2电池电量
            tx2_bat_info_temp = *(uint8_t*)p_msg->param.ptr;
            LOG_I("Tx2 bat soc update: %d", tx2_bat_info_temp);
        } break;
        case HL_BOX_BAT_INFO_UPDATE_IND: {  //更新box电池电量
            box_bat_info_temp = *(uint8_t*)p_msg->param.ptr;
            LOG_I("Box bat soc update: %d", box_bat_info_temp);
        } break;
        case HL_SET_PAIR_MAC_TX1_REQ_IND: {  //请求设置tx1的mac地址来配对
            tx1_mac_addr = (uint8_t*)p_msg->param.ptr;
            LOG_I("tx1 mac addr write success! [%02x] [%02x] [%02x] [%02x] [%02x] [%02x]\n", tx1_mac_addr[0],
                  tx1_mac_addr[1], tx1_mac_addr[2], tx1_mac_addr[3], tx1_mac_addr[4], tx1_mac_addr[5]);
        } break;
        case HL_SET_PAIR_MAC_TX2_REQ_IND: {  //请求设置tx2的mac地址来配对
            tx2_mac_addr = (uint8_t*)p_msg->param.ptr;
            LOG_I("tx2 mac addr write success! [%02x] [%02x] [%02x] [%02x] [%02x] [%02x]\n", tx2_mac_addr[0],
                  tx2_mac_addr[1], tx2_mac_addr[2], tx2_mac_addr[3], tx2_mac_addr[4], tx2_mac_addr[5]);
        } break;
        case HL_GET_MAC_REQ_IND: {  //请求获取自己的Mac地址
            hl_mod_euc_ctrl(HL_SET_MAC_CMD, dev_mac_temp, sizeof(dev_mac_temp));
        } break;
        case HL_TX1_PAIR_START_IND: {  //请求开始Tx1有线配对
            LOG_I("Tx1 pair start!");
            hl_mod_euc_ctrl(HL_START_TX1_PAIR_CMD, NULL, 0);  //通知收纳盒开始Tx1的配对
        } break;
        case HL_TX2_PAIR_START_IND: {  //请求开始Tx2有线配对
            LOG_I("Tx2 pair start!");
            hl_mod_euc_ctrl(HL_START_TX2_PAIR_CMD, NULL, 0);  //通知收纳盒开始Tx2的配对
        } break;
        case HL_TX1_PAIR_STOP_IND: {  //tx1配对结束，结果打印
            tx1_pair_result_temp = *(bool*)p_msg->param.ptr;
            LOG_I("Tx1 pair result:%d!", tx1_pair_result_temp);
        } break;
        case HL_TX2_PAIR_STOP_IND: {  //tx2配对结束，结果打印
            tx2_pair_result_temp = *(bool*)p_msg->param.ptr;
            LOG_I("Tx2 pair result:%d!", tx2_pair_result_temp);
        } break;
        case HL_GET_CHARGE_STATE_REQ_IND: {  //请求获取充电状态
            hl_mod_euc_ctrl(HL_SET_CHARGE_STATE_CMD, &charge_state_temp, sizeof(charge_state_temp));
        } break;
        case HL_TX1_CHARGE_STATE_IND: {  //更新Tx1的充电状态
            tx1_charge_state_temp = *(hl_mod_euc_charge_state_e*)p_msg->param.ptr;
            LOG_I("Tx1 charge state:%d", tx1_charge_state_temp);
        } break;
        case HL_TX2_CHARGE_STATE_IND: {  //更新Tx2的充电状态
            tx2_charge_state_temp = *(hl_mod_euc_charge_state_e*)p_msg->param.ptr;
            LOG_I("Tx2 charge state:%d", tx2_charge_state_temp);
        } break;        
        case HL_BOX_CHARGE_STATE_IND: {  //更新Box的充电状态
            box_charge_state_temp = *(hl_mod_euc_charge_state_e*)p_msg->param.ptr;
            LOG_I("Box charge state:%d", box_charge_state_temp);
        } break;    
        default:
            LOG_E("cmd(%d) unkown!!! \r\n", p_msg->cmd);
            break;
    }
}
#endif
/*
 * EOF
 */