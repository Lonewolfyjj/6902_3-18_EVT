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
#include "hl_mod_display.h"
#include "hl_mod_telink.h"

#define DBG_SECTION_NAME "app_com"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/

static bool    _rx_in_box_flag  = false;
static bool    _tx1_in_box_flag = false;
static bool    _tx2_in_box_flag = false;
static uint8_t _dev_num;

/* Private function(only *.c)  -----------------------------------------------*/
#if HL_IS_TX_DEVICE()

#else
static void _display_in_box_state_set(void)
{
    hl_display_box_charge_state display_box_charge_state;

    if (_rx_in_box_flag == false) {
        display_box_charge_state = BOX_CHARGE_RX_NOT;
    } else if (_tx1_in_box_flag == false && _tx2_in_box_flag == false) {
        display_box_charge_state = BOX_CHARGE_RX;
    } else if (_tx1_in_box_flag == true && _tx2_in_box_flag == true) {
        display_box_charge_state = BOX_CHARGE_RX_TX12;
    } else if (_tx1_in_box_flag == true) {
        display_box_charge_state = BOX_CHARGE_RX_TX1;
    } else {
        display_box_charge_state = BOX_CHARGE_RX_TX2;
    }

    hl_mod_display_io_ctrl(IN_BOX_STATE_VAL_CMD, &display_box_charge_state, sizeof(display_box_charge_state));
}
#endif
/* Exported functions --------------------------------------------------------*/
#if HL_IS_TX_DEVICE()
void hl_app_com_msg_pro(mode_to_app_msg_t* p_msg)
{
    uint8_t                   bat_soc_temp     = 50;
    uint8_t                   pair_mac_temp[6] = { 0xf4, 0x4e, 0x35, 0x46, 0xff, 0x34 };
    uint8_t                   dev_mac_temp[6]  = { 0xf4, 0x4e, 0x35, 0x46, 0xff, 0x35 };
    uint8_t*                  rx_mac_addr;
    hl_mod_euc_charge_state_e charge_state_temp = HL_MOD_EUC_CHARGE_STATE_CHARGING;

    hl_rf_work_mode_e telink_work_mode;
    hl_rf_pair_info_t telink_pair_info;

    switch (p_msg->cmd) {
        case HL_IN_BOX_IND: {
            _dev_num = *(uint8_t*)p_msg->param.ptr;
            LOG_I("in box! dev_num:%d", _dev_num);

            telink_work_mode = HL_RF_LOW_POWER;
            hl_mod_telink_ioctl(HL_RF_SET_WORK_MODE_CMD, &telink_work_mode, sizeof(telink_work_mode));

            hl_mod_telink_ioctl(HL_RF_GET_LOCAL_MAC_CMD, &_dev_num, 1);
            hl_mod_telink_ioctl(HL_RF_GET_REMOTE_MAC_CMD, &_dev_num, 1);
        } break;
        case HL_OUT_BOX_IND: {
            LOG_I("out box!");

            telink_work_mode = HL_RF_FULL_POWER;
            hl_mod_telink_ioctl(HL_RF_SET_WORK_MODE_CMD, &telink_work_mode, sizeof(telink_work_mode));
        } break;
        case HL_GET_SOC_REQ_IND: {  //请求获取电量
            bat_soc_temp = tx_info.soc;
            hl_mod_euc_ctrl(HL_SET_SOC_CMD, &bat_soc_temp, sizeof(bat_soc_temp));
        } break;
        case HL_GET_PAIR_MAC_REQ_IND: {  // 请求获取已经配对的mac地址
            rt_memcpy(pair_mac_temp, tx_info.remote_mac, sizeof(pair_mac_temp));

            LOG_I("rx mac addr: [%02x] [%02x] [%02x] [%02x] [%02x] [%02x]", pair_mac_temp[0], pair_mac_temp[1],
                  pair_mac_temp[2], pair_mac_temp[3], pair_mac_temp[4], pair_mac_temp[5]);

            hl_mod_euc_ctrl(HL_SET_PAIR_MAC_CMD, pair_mac_temp, sizeof(pair_mac_temp));
        } break;
        case HL_SET_PAIR_MAC_REQ_IND: {  // 请求设置rx的mac地址，为有线配对的流程
            rx_mac_addr = (uint8_t*)p_msg->param.ptr;

            if (_dev_num == 1) {  // tx1 左声道
                telink_pair_info.chn = HL_RF_LEFT_CHANNEL;
            } else {  // tx2 右声道
                telink_pair_info.chn = HL_RF_RIGHT_CHANNEL;
            }
            rt_memcpy(telink_pair_info.mac, rx_mac_addr, sizeof(telink_pair_info.mac));
            hl_mod_telink_ioctl(HL_RF_SET_REMOTE_MAC_CMD, &telink_pair_info, sizeof(telink_pair_info));

            LOG_I("rx mac addr write success! [%02x] [%02x] [%02x] [%02x] [%02x] [%02x]", rx_mac_addr[0],
                  rx_mac_addr[1], rx_mac_addr[2], rx_mac_addr[3], rx_mac_addr[4], rx_mac_addr[5]);
        } break;
        case HL_GET_MAC_REQ_IND: {  // 请求获取自己的mac地址
            rt_memcpy(dev_mac_temp, tx_info.local_mac, sizeof(dev_mac_temp));

            LOG_I("tx%d mac addr: [%02x] [%02x] [%02x] [%02x] [%02x] [%02x]", _dev_num, dev_mac_temp[0],
                  dev_mac_temp[1], dev_mac_temp[2], dev_mac_temp[3], dev_mac_temp[4], dev_mac_temp[5]);

            hl_mod_euc_ctrl(HL_SET_MAC_CMD, dev_mac_temp, sizeof(dev_mac_temp));
        } break;
        case HL_GET_CHARGE_STATE_REQ_IND: {  // 请求获取充电状态
            charge_state_temp = tx_info.charge_flag + 1;
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
    uint8_t                    bat_soc_temp = 50;
    uint8_t                    tx1_bat_info_temp;
    uint8_t                    tx2_bat_info_temp;
    uint8_t                    box_bat_info_temp;
    uint8_t*                   tx1_mac_addr;
    uint8_t*                   tx2_mac_addr;
    uint8_t                    dev_mac_temp[6] = { 0xf4, 0x4e, 0x35, 0x46, 0xff, 0x33 };
    bool                       tx1_pair_result_temp;
    bool                       tx2_pair_result_temp;
    bool                       tx1_in_box_flag_temp;
    bool                       tx2_in_box_flag_temp;
    hl_mod_euc_charge_state_e  charge_state_temp = HL_MOD_EUC_CHARGE_STATE_CHARGING;
    hl_mod_euc_charge_state_e  tx1_charge_state_temp;
    hl_mod_euc_charge_state_e  tx2_charge_state_temp;
    hl_mod_euc_charge_state_e  box_charge_state_temp;
    hl_mod_euc_rtc_st          rtc_time_temp = { 0 };
    hl_mod_euc_box_lid_state_e box_lid_state_temp;
    uint8_t                    pair_mac_temp[12] = { 0 };

    uint8_t           temp;
    hl_rf_work_mode_e telink_work_mode;
    hl_rf_pair_info_t telink_pair_info;

    switch (p_msg->cmd) {
        case HL_IN_BOX_IND: {
            _rx_in_box_flag = true;
            _dev_num        = *(uint8_t*)p_msg->param.ptr;

            LOG_I("in box! dev_num:%d", _dev_num);

            telink_work_mode = HL_RF_LOW_POWER;
            hl_mod_telink_ioctl(HL_RF_SET_WORK_MODE_CMD, &telink_work_mode, sizeof(telink_work_mode));

            hl_mod_telink_ioctl(HL_RF_GET_LOCAL_MAC_CMD, &_dev_num, 1);
            hl_mod_telink_ioctl(HL_RF_GET_REMOTE_MAC_CMD, &_dev_num, 1);

            _display_in_box_state_set();
        } break;
        case HL_OUT_BOX_IND: {
            LOG_I("out box!");
            _rx_in_box_flag = false;

            telink_work_mode = HL_RF_FULL_POWER;
            hl_mod_telink_ioctl(HL_RF_SET_WORK_MODE_CMD, &telink_work_mode, sizeof(telink_work_mode));

            _display_in_box_state_set();
        } break;
        case HL_GET_SOC_REQ_IND: {  // 请求获取电池电量
            bat_soc_temp = rx_info.soc;
            hl_mod_euc_ctrl(HL_SET_SOC_CMD, &bat_soc_temp, sizeof(bat_soc_temp));
        } break;
        case HL_TX1_BAT_INFO_UPDATE_IND: {  //更新tx1电池电量
            tx1_bat_info_temp = *(uint8_t*)p_msg->param.ptr;
            LOG_I("Tx1 bat soc update: %d", tx1_bat_info_temp);

            hl_mod_display_io_ctrl(TX1_BAT_VAL_VAL_CMD, &tx1_bat_info_temp, sizeof(tx1_bat_info_temp));
        } break;
        case HL_TX2_BAT_INFO_UPDATE_IND: {  //更新tx2电池电量
            tx2_bat_info_temp = *(uint8_t*)p_msg->param.ptr;
            LOG_I("Tx2 bat soc update: %d", tx2_bat_info_temp);

            hl_mod_display_io_ctrl(TX2_BAT_VAL_VAL_CMD, &tx2_bat_info_temp, sizeof(tx2_bat_info_temp));
        } break;
        case HL_BOX_BAT_INFO_UPDATE_IND: {  //更新box电池电量
            box_bat_info_temp = *(uint8_t*)p_msg->param.ptr;
            LOG_I("Box bat soc update: %d", box_bat_info_temp);

            hl_mod_display_io_ctrl(CASE_BAT_VAL_VAL_CMD, &box_bat_info_temp, sizeof(box_bat_info_temp));
        } break;
        case HL_TX1_IN_BOX_STATE_IND: {  //更新Tx1在盒状态
            tx1_in_box_flag_temp = *(bool*)p_msg->param.ptr;
            _tx1_in_box_flag     = tx1_in_box_flag_temp;
            _display_in_box_state_set();
            LOG_I("Tx1 in box state:%d", tx1_in_box_flag_temp);
        } break;
        case HL_TX2_IN_BOX_STATE_IND: {  //更新Tx2在盒状态
            tx2_in_box_flag_temp = *(bool*)p_msg->param.ptr;
            _tx2_in_box_flag     = tx2_in_box_flag_temp;
            _display_in_box_state_set();
            LOG_I("Tx2 in box state:%d", tx2_in_box_flag_temp);
        } break;
        case HL_SET_PAIR_MAC_TX1_REQ_IND: {  //请求设置tx1的mac地址来配对
            tx1_mac_addr = (uint8_t*)p_msg->param.ptr;

            telink_pair_info.chn = HL_RF_LEFT_CHANNEL;
            rt_memcpy(telink_pair_info.mac, tx1_mac_addr, sizeof(telink_pair_info.mac));
            hl_mod_telink_ioctl(HL_RF_SET_REMOTE_MAC_CMD, &telink_pair_info, sizeof(telink_pair_info));

            LOG_I("tx1 mac addr write success! [%02x] [%02x] [%02x] [%02x] [%02x] [%02x]", tx1_mac_addr[0],
                  tx1_mac_addr[1], tx1_mac_addr[2], tx1_mac_addr[3], tx1_mac_addr[4], tx1_mac_addr[5]);
        } break;
        case HL_SET_PAIR_MAC_TX2_REQ_IND: {  //请求设置tx2的mac地址来配对
            tx2_mac_addr = (uint8_t*)p_msg->param.ptr;

            telink_pair_info.chn = HL_RF_RIGHT_CHANNEL;
            rt_memcpy(telink_pair_info.mac, tx2_mac_addr, sizeof(telink_pair_info.mac));
            hl_mod_telink_ioctl(HL_RF_SET_REMOTE_MAC_CMD, &telink_pair_info, sizeof(telink_pair_info));

            LOG_I("tx2 mac addr write success! [%02x] [%02x] [%02x] [%02x] [%02x] [%02x]", tx2_mac_addr[0],
                  tx2_mac_addr[1], tx2_mac_addr[2], tx2_mac_addr[3], tx2_mac_addr[4], tx2_mac_addr[5]);
        } break;
        case HL_GET_MAC_REQ_IND: {  //请求获取自己的Mac地址
            rt_memcpy(dev_mac_temp, rx_info.local_mac, sizeof(dev_mac_temp));

            LOG_I("rx mac addr: [%02x] [%02x] [%02x] [%02x] [%02x] [%02x]", dev_mac_temp[0], dev_mac_temp[1],
                  dev_mac_temp[2], dev_mac_temp[3], dev_mac_temp[4], dev_mac_temp[5]);

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
            charge_state_temp = rx_info.charge_flag + 1;
            hl_mod_euc_ctrl(HL_SET_CHARGE_STATE_CMD, &charge_state_temp, sizeof(charge_state_temp));
        } break;
        case HL_TX1_CHARGE_STATE_IND: {  //更新Tx1的充电状态
            tx1_charge_state_temp = *(hl_mod_euc_charge_state_e*)p_msg->param.ptr;
            LOG_I("Tx1 charge state:%d", tx1_charge_state_temp);

            if (tx1_charge_state_temp == HL_MOD_EUC_CHARGE_STATE_CHARGING) {
                temp = 1;
            } else {
                temp = 0;
            }
            hl_mod_display_io_ctrl(TX1_CHARGE_STATUS_SWITCH_CMD, &temp, sizeof(temp));
        } break;
        case HL_TX2_CHARGE_STATE_IND: {  //更新Tx2的充电状态
            tx2_charge_state_temp = *(hl_mod_euc_charge_state_e*)p_msg->param.ptr;
            LOG_I("Tx2 charge state:%d", tx2_charge_state_temp);

            if (tx2_charge_state_temp == HL_MOD_EUC_CHARGE_STATE_CHARGING) {
                temp = 1;
            } else {
                temp = 0;
            }
            hl_mod_display_io_ctrl(TX2_CHARGE_STATUS_SWITCH_CMD, &temp, sizeof(temp));
        } break;
        case HL_BOX_CHARGE_STATE_IND: {  //更新Box的充电状态
            box_charge_state_temp = *(hl_mod_euc_charge_state_e*)p_msg->param.ptr;
            LOG_I("Box charge state:%d", box_charge_state_temp);

            if (box_charge_state_temp == HL_MOD_EUC_CHARGE_STATE_CHARGING) {
                temp = 1;
            } else {
                temp = 0;
            }
            hl_mod_display_io_ctrl(BOX_CHARGE_STATUS_SWITCH_CMD, &temp, sizeof(temp));
        } break;
        case HL_GET_RTC_TIME_REQ_IND: {  //请求设置RTC时间
            hl_mod_euc_ctrl(HL_SET_RTC_TIME_CMD, &rtc_time_temp, sizeof(rtc_time_temp));
        } break;
        case HL_BOX_LID_STATE_UPDATE_IND: {  //更新Box开关盖状态
            box_lid_state_temp = *(hl_mod_euc_box_lid_state_e*)p_msg->param.ptr;
            LOG_I("Box lid state:%d", box_lid_state_temp);
        } break;
        case HL_GET_PAIR_MAC_REQ_IND: {
            rt_memcpy(pair_mac_temp, rx_info.remote_mac, sizeof(pair_mac_temp));

            LOG_I("rx pair mac addr: [%02x] [%02x] [%02x] [%02x] [%02x] [%02x]  [%02x] [%02x] [%02x] [%02x] [%02x] "
                  "[%02x]",
                  pair_mac_temp[0], pair_mac_temp[1], pair_mac_temp[2], pair_mac_temp[3], pair_mac_temp[4],
                  pair_mac_temp[5], pair_mac_temp[6], pair_mac_temp[7], pair_mac_temp[8], pair_mac_temp[9],
                  pair_mac_temp[10], pair_mac_temp[11]);

            hl_mod_euc_ctrl(HL_SET_PAIR_MAC_CMD, pair_mac_temp, sizeof(pair_mac_temp));
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