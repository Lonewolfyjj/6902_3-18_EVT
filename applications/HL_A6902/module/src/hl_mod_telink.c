/**
 * @file hl_mod_telink.c
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * @brief 负责与Telink无线芯片命令交互功能模块源文件
 * @version 1.0
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
 * <tr><td>2022-11-14     <td>v1.0     <td>lisonglin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "hl_mod_telink.h"
#include "hl_hal_gpio.h"
#include "hl_util_msg_type.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DBG_SECTION_NAME "mod_telink"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

/* typedef -------------------------------------------------------------------*/

/// 无线模块交互信息结构体
typedef struct _hl_s_rf_info_t
{
    uint8_t                local_mac[6];
    hl_rf_remote_mac_t     remote_mac;
    hl_rf_version_t        version;
    hl_rf_pair_info_t      mac;
    hl_rf_rssi_t           rssi;
    hl_rf_bypass_time_t    time;
    hl_rf_bypass_version_t remote_version;
    hl_rf_bypass_state_t   mute;
    hl_rf_bypass_state_t   denoise;
    hl_rf_bypass_state_t   low_cut;
    hl_rf_bypass_state_t   record;
    hl_rf_bypass_state_t   charge;
    hl_rf_bypass_state_t   sound_mix;
    hl_rf_bypass_state_t   auto_record;
    hl_rf_bypass_state_t   record_protect;
    hl_rf_bypass_value_t   status_led;
    hl_rf_bypass_value_t   uac_gain;
    hl_rf_bypass_value_t   tx_gain;
    hl_rf_bypass_value_t   battery;
    hl_rf_bypass_value_t   auto_poweroff;
    hl_rf_bypass_value_t   sound_effect;
} hl_rf_info_t;

typedef struct
{
    /// 配对状态
    hl_rf_state_e pair_state;
    /// RSSI值
    hl_rf_rssi_t remote_rssi;
} hl_rf_telink_info_t;

/* define --------------------------------------------------------------------*/

#define TELINK_THREAD_STACK_SIZE 1024
#define TELINK_THREAD_PRIORITY 20
#define TELINK_THREAD_TIMESLICE 10
#define TELINK_UART_DEV_NAME "uart2"
#define TELINK_UART_BUF_SIZE 1024
#define TELINK_FIFO_BUF_SIZE 1024
#define TELINK_HUP_BUF_SIZE 1024

/* variables -----------------------------------------------------------------*/

/// Telink模块句柄
static hl_mod_telink_t s_telink = { 0 };
/// 串口接收临时缓冲区
static uint8_t s_uart_recv_buf[TELINK_UART_BUF_SIZE] = { 0 };
/// hup接收缓冲区
static uint8_t s_telink_hup_buf[TELINK_HUP_BUF_SIZE] = { 0 };
/// fifo接收缓冲区
static uint8_t s_telink_fifo_buf[TELINK_FIFO_BUF_SIZE] = { 0 };
/// 上层APP消息队列结构体
static mode_to_app_msg_t app_msg_t = { 0 };
/// rf暂存hup解析数据结构体
static hl_rf_info_t s_rf_info = { 0 };
/// rf串口上报信息结构体指针
static hl_rf_telink_info_t* telink_info = NULL;

/// 记录上次配对状态
static uint8_t old_pair_info = 0x1f;
/// 记录最新配对状态
static uint8_t new_pair_info = 0x1f;

/* Private function(only *.c)  -----------------------------------------------*/

static void telink_show_val(void)
{
    LOG_I("\n\nold_pair_info = %02x\n", old_pair_info);
    LOG_I("new_pair_info = %02x\n", new_pair_info);
}
MSH_CMD_EXPORT(telink_show_val, telink show val cmd);

/**
 * 
 * @brief Telink成功解析hup命令回调函数
 * @param [in] hup_frame hup句柄
 * @date 2022-11-14
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-14      <td>lisonglin     <td>新建
 * </table>
 */
static void _telink_hup_success_handle_cb(hup_protocol_type_t hup_frame)
{
    rt_err_t ret = RT_EOK;

    // 计算hup中实际数据的长度
    uint16_t data_len = 0;
    data_len |= (hup_frame.data_len_h << 8);
    data_len |= hup_frame.data_len_l;

    app_msg_t.cmd = hup_frame.cmd;

    switch (hup_frame.cmd) {
        case HL_RF_VERSION_IND:
            s_rf_info.version.major_ver = ((hl_rf_version_t*)hup_frame.data_addr)->major_ver;
            s_rf_info.version.mioor_bug = ((hl_rf_version_t*)hup_frame.data_addr)->mioor_bug;
            s_rf_info.version.miror_rev = ((hl_rf_version_t*)hup_frame.data_addr)->miror_rev;
            s_rf_info.version.miror_tes = ((hl_rf_version_t*)hup_frame.data_addr)->miror_tes;
            app_msg_t.len               = sizeof(s_rf_info.version);
            app_msg_t.param.ptr         = (uint8_t*)&s_rf_info.version;
            break;

        case HL_RF_APP_INFO_IND:
            telink_info = (hl_rf_telink_info_t*)hup_frame.data_addr;
            // 更新Telink模块工作状态和配对状态
            new_pair_info = telink_info->pair_state;
            memcpy(&s_rf_info.rssi, &telink_info->remote_rssi, sizeof(hl_rf_rssi_t));
            app_msg_t.cmd       = HL_RF_RSSI_IND;
            app_msg_t.len       = sizeof(hl_rf_rssi_t);
            app_msg_t.param.ptr = &s_rf_info.rssi;
            break;

        case HL_RF_PAIR_STATE_IND:
            // 更新Telink模块工作状态和配对状态
            new_pair_info = (uint8_t)hup_frame.data_addr[0];
            break;

        case HL_RF_RSSI_IND:
            memcpy(&s_rf_info.rssi, (hl_rf_rssi_t*)hup_frame.data_addr, sizeof(hl_rf_rssi_t));
            app_msg_t.len       = sizeof(hl_rf_rssi_t);
            app_msg_t.param.ptr = &s_rf_info.rssi;
            break;

        case HL_RF_GET_LOCAL_MAC_IND:
            LOG_I("\n\n[Telink Local MAC Addr]:");
            for (int i = 0; i < 6; i++) {
                LOG_I("%02X ", hup_frame.data_addr[i]);
            }
            LOG_I("\n\n");
            memcpy(s_rf_info.local_mac, (uint8_t*)hup_frame.data_addr, 6);
            app_msg_t.len       = sizeof(s_rf_info.local_mac);
            app_msg_t.param.ptr = s_rf_info.local_mac;
            break;

        case HL_RF_GET_REMOTE_MAC_IND:
#if HL_IS_TX_DEVICE()
            memcpy(s_rf_info.remote_mac.rx_mac, ((hl_rf_remote_mac_t*)hup_frame.data_addr)->rx_mac, 6);
            LOG_I("\n\nTelink Remote MAC Addr:\n[RX]: ");
            for (int i = 0; i < 6; i++) {
                LOG_I("%02X ", hup_frame.data_addr[i]);
            }
            LOG_I("\n\n\n");
#else
            memcpy(s_rf_info.remote_mac.tx1_mac, ((hl_rf_remote_mac_t*)hup_frame.data_addr)->tx1_mac, 6);
            memcpy(s_rf_info.remote_mac.tx2_mac, ((hl_rf_remote_mac_t*)hup_frame.data_addr)->tx2_mac, 6);
            LOG_I("\n\nTelink Remote MAC Addr:\n[TX_L]: ");
            for (int i = 0; i < 12; i++) {
                LOG_I("%02X ", hup_frame.data_addr[i]);
                if (i == 5) {
                    LOG_I("\n[TX_R]: ");
                }
            }
            LOG_I("\n\n\n");
#endif
            app_msg_t.len       = sizeof(s_rf_info.remote_mac);
            app_msg_t.param.ptr = (uint8_t*)&s_rf_info.remote_mac;
            break;

        case HL_RF_BYPASS_MUTE_IND:
            s_rf_info.mute.chn   = ((hl_rf_bypass_state_t*)hup_frame.data_addr)->chn;
            s_rf_info.mute.state = ((hl_rf_bypass_state_t*)hup_frame.data_addr)->state;
            app_msg_t.len        = sizeof(s_rf_info.mute);
            app_msg_t.param.ptr  = (uint8_t*)&s_rf_info.mute;
            break;

        case HL_RF_BYPASS_DENOISE_IND:
            s_rf_info.denoise.chn   = ((hl_rf_bypass_state_t*)hup_frame.data_addr)->chn;
            s_rf_info.denoise.state = ((hl_rf_bypass_state_t*)hup_frame.data_addr)->state;
            app_msg_t.len           = sizeof(s_rf_info.denoise);
            app_msg_t.param.ptr     = (uint8_t*)&s_rf_info.denoise;
            break;

        case HL_RF_BYPASS_RECORD_IND:
            s_rf_info.record.chn   = ((hl_rf_bypass_state_t*)hup_frame.data_addr)->chn;
            s_rf_info.record.state = ((hl_rf_bypass_state_t*)hup_frame.data_addr)->state;
            app_msg_t.len          = sizeof(s_rf_info.record);
            app_msg_t.param.ptr    = (uint8_t*)&s_rf_info.record;
            break;

        case HL_RF_BYPASS_RECORD_PROTECT_IND:
            s_rf_info.record_protect.chn   = ((hl_rf_bypass_state_t*)hup_frame.data_addr)->chn;
            s_rf_info.record_protect.state = ((hl_rf_bypass_state_t*)hup_frame.data_addr)->state;
            app_msg_t.len                  = sizeof(s_rf_info.record_protect);
            app_msg_t.param.ptr            = (uint8_t*)&s_rf_info.record_protect;
            break;

        case HL_RF_BYPASS_AUTO_RECORD_IND:
            s_rf_info.auto_record.chn   = ((hl_rf_bypass_state_t*)hup_frame.data_addr)->chn;
            s_rf_info.auto_record.state = ((hl_rf_bypass_state_t*)hup_frame.data_addr)->state;
            app_msg_t.len               = sizeof(s_rf_info.auto_record);
            app_msg_t.param.ptr         = (uint8_t*)&s_rf_info.auto_record;
            break;

        case HL_RF_BYPASS_LOWCUT_IND:
            s_rf_info.low_cut.chn   = ((hl_rf_bypass_state_t*)hup_frame.data_addr)->chn;
            s_rf_info.low_cut.state = ((hl_rf_bypass_state_t*)hup_frame.data_addr)->state;
            app_msg_t.len           = sizeof(s_rf_info.low_cut);
            app_msg_t.param.ptr     = (uint8_t*)&s_rf_info.low_cut;
            break;

        case HL_RF_BYPASS_CHARGE_IND:
            s_rf_info.charge.chn   = ((hl_rf_bypass_state_t*)hup_frame.data_addr)->chn;
            s_rf_info.charge.state = ((hl_rf_bypass_state_t*)hup_frame.data_addr)->state;
            app_msg_t.len          = sizeof(s_rf_info.charge);
            app_msg_t.param.ptr    = (uint8_t*)&s_rf_info.charge;
            break;

        case HL_RF_BYPASS_BATTERY_IND:
            s_rf_info.battery.chn = ((hl_rf_bypass_value_t*)hup_frame.data_addr)->chn;
            s_rf_info.battery.val = ((hl_rf_bypass_value_t*)hup_frame.data_addr)->val;
            app_msg_t.len         = sizeof(s_rf_info.battery);
            app_msg_t.param.ptr   = (uint8_t*)&s_rf_info.battery;
            break;

        case HL_RF_BYPASS_REFACTORY_IND:
        case HL_RF_BYPASS_FORMAT_DISK_IND:
            app_msg_t.len             = sizeof(uint32_t);
            app_msg_t.param.u32_param = (uint32_t)hup_frame.data_addr[0];
            break;

        case HL_RF_BYPASS_AUTO_POWEROFF_IND:
            s_rf_info.auto_poweroff.chn = ((hl_rf_bypass_value_t*)hup_frame.data_addr)->chn;
            s_rf_info.auto_poweroff.val = ((hl_rf_bypass_value_t*)hup_frame.data_addr)->val;
            app_msg_t.len               = sizeof(s_rf_info.auto_poweroff);
            app_msg_t.param.ptr         = (uint8_t*)&s_rf_info.auto_poweroff;
            break;

        case HL_RF_BYPASS_VERSION_IND:
            s_rf_info.remote_version.chn     = ((hl_rf_bypass_version_t*)hup_frame.data_addr)->chn;
            s_rf_info.remote_version.mcu_ver = ((hl_rf_bypass_version_t*)hup_frame.data_addr)->mcu_ver;
            s_rf_info.remote_version.rf_ver  = ((hl_rf_bypass_version_t*)hup_frame.data_addr)->rf_ver;
            app_msg_t.len                    = sizeof(s_rf_info.remote_version);
            app_msg_t.param.ptr              = (uint8_t*)&s_rf_info.remote_version;
            break;

        case HL_RF_BYPASS_STATUS_LED_IND:
            s_rf_info.status_led.chn = ((hl_rf_bypass_value_t*)hup_frame.data_addr)->chn;
            s_rf_info.status_led.val = ((hl_rf_bypass_value_t*)hup_frame.data_addr)->val;
            app_msg_t.len            = sizeof(s_rf_info.status_led);
            app_msg_t.param.ptr      = (uint8_t*)&s_rf_info.status_led;
            break;

        case HL_RF_BYPASS_SOUND_MIX_IND:
            s_rf_info.sound_mix.chn   = ((hl_rf_bypass_state_t*)hup_frame.data_addr)->chn;
            s_rf_info.sound_mix.state = ((hl_rf_bypass_state_t*)hup_frame.data_addr)->state;
            app_msg_t.len             = sizeof(s_rf_info.sound_mix);
            app_msg_t.param.ptr       = (uint8_t*)&s_rf_info.sound_mix;
            break;

        case HL_RF_BYPASS_UAC_GAIN_IND:
            s_rf_info.uac_gain.chn = ((hl_rf_bypass_value_t*)hup_frame.data_addr)->chn;
            s_rf_info.uac_gain.val = ((hl_rf_bypass_value_t*)hup_frame.data_addr)->val;
            app_msg_t.len          = sizeof(s_rf_info.uac_gain);
            app_msg_t.param.ptr    = (uint8_t*)&s_rf_info.uac_gain;
            break;

        case HL_RF_BYPASS_TX_GAIN_IND:
            s_rf_info.tx_gain.chn = ((hl_rf_bypass_value_t*)hup_frame.data_addr)->chn;
            s_rf_info.tx_gain.val = ((hl_rf_bypass_value_t*)hup_frame.data_addr)->val;
            app_msg_t.len         = sizeof(s_rf_info.tx_gain);
            app_msg_t.param.ptr   = (uint8_t*)&s_rf_info.tx_gain;
            break;

        case HL_RF_BYPASS_SOUND_EFFECT_IND:
            s_rf_info.sound_effect.chn = ((hl_rf_bypass_value_t*)hup_frame.data_addr)->chn;
            s_rf_info.sound_effect.val = ((hl_rf_bypass_value_t*)hup_frame.data_addr)->val;
            app_msg_t.len              = sizeof(s_rf_info.sound_effect);
            app_msg_t.param.ptr        = (uint8_t*)&s_rf_info.sound_effect;
            break;

        case HL_RF_BYPASS_TIME_IND:
            rt_memcpy(&s_rf_info.time, (hl_rf_bypass_time_t*)&hup_frame.data_addr, sizeof(hl_rf_bypass_time_t));
            app_msg_t.len       = sizeof(s_rf_info.time);
            app_msg_t.param.ptr = (uint8_t*)&s_rf_info.time;
            break;

        default:
            break;
    }

    if (HL_RF_PAIR_STATE_IND != hup_frame.cmd) {
        // 上报透传数据
        ret = rt_mq_send(s_telink.app_msq, (void*)&app_msg_t, sizeof(app_msg_t));
        // 打印信息
        // LOG_I("\n\n[Telink Bypass Cmd]:%02X\n", hup_frame.cmd);
        // LOG_I("[Telink Bypass Info]:\n");
        // for (uint16_t i = 0; i < data_len; i++) {
        //     LOG_I(" %02X ", hup_frame.data_addr[i]);
        // }
        // LOG_I("\n\n");
        // 判断消息队列上传结果
        if (RT_EOK != ret) {
            LOG_E("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FUNCTION__, __LINE__, ret);
        }
    }
}

/**
 * 
 * @brief Telink串口接收数据回调函数
 * @param [in] dev 设备句柄
 * @param [in] size 串口接收数据大小
 * @return rt_err_t RT_EOK
 * @date 2022-11-14
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-14      <td>lisonglin     <td>新建
 * </table>
 */
static rt_err_t _telink_uart_receive_cb(rt_device_t dev, rt_size_t size)
{
    rt_size_t data_size = 0;

    // 读出串口数据
    data_size = rt_device_read(s_telink.serial, 0, s_uart_recv_buf, size);
    // 将串口数据写入FIFO
    hl_util_fifo_write(&s_telink.fifo, s_uart_recv_buf, data_size);

    return RT_EOK;
}

/**
 * 
 * @brief Telink模块线程处理函数
 * @param [in] parameter 传入参数
 * @date 2022-11-14
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-14      <td>lisonglin     <td>新建
 * </table>
 */
static void hl_mod_telink_thread_entry(void* parameter)
{
    rt_err_t result    = RT_EOK;
    uint16_t count     = 1;
    uint32_t data_size = 0;
    uint8_t  buf[256]  = { 0 };

    while (RT_TRUE == s_telink.thread_flag) {
        // 读取数据并解析
        data_size = hl_util_fifo_data_size(&s_telink.fifo);
        data_size = ((data_size > 256) ? 256 : data_size);
        if (data_size > 0) {
            data_size = hl_util_fifo_read(&s_telink.fifo, buf, data_size);
            for (uint32_t i = 0; i < data_size; i++) {
                hl_util_hup_decode(&s_telink.hup, buf[i]);
            }
        }

        // 延时5ms
        rt_thread_mdelay(5);

        // LED配对状态灯更新间隔1秒
        count++;
        count %= 200;

        // 更新配对状态
        if ((old_pair_info != new_pair_info) || (!count)) {
            if (old_pair_info != new_pair_info) {
                app_msg_t.cmd = HL_RF_PAIR_STATE_IND;
            } else {
                app_msg_t.cmd = HL_RF_REFRESH_STATE_IND;
            }
            // 编辑消息结构体
            app_msg_t.len       = 1;
            app_msg_t.param.ptr = &new_pair_info;
            // 将消息结构体上传至APP层消息队列
            result = rt_mq_send(s_telink.app_msq, (void*)&app_msg_t, sizeof(app_msg_t));
            if (RT_ERROR != result) {
                // 更新配对状态
                old_pair_info = new_pair_info;
            } else {
                LOG_E("[%s][line:%d] Send MSG(pair info) To APP Error!!! \r\n", __FUNCTION__, __LINE__, result);
            }
        } else {
            continue;
        }
    }
}

/**
 * 
 * @brief Telink初始化串口
 * @return rt_err_t 成功/失败
 * @date 2022-11-14
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-14      <td>lisonglin     <td>新建
 * </table>
 */
static rt_err_t _hl_mod_telink_serial_init(void)
{
    rt_err_t result = RT_EOK;

    // 查找系统中的串口设备
    s_telink.serial = rt_device_find(TELINK_UART_DEV_NAME);
    if (RT_NULL == s_telink.serial) {
        LOG_E("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FUNCTION__, __LINE__, s_telink.serial);
        return RT_ERROR;
    }

    // 以中断接收及轮询发送模式打开串口设备
    result = rt_device_open(s_telink.serial, RT_DEVICE_FLAG_INT_RX);
    if (RT_EOK != result) {
        LOG_E("[%s][line:%d] rt_device_open faild!!! \r\n", __FUNCTION__, __LINE__);
        return RT_ERROR;
    }
    // 设置接收回调函数
    rt_device_set_rx_indicate(s_telink.serial, _telink_uart_receive_cb);

    return RT_EOK;
}

/**
 * 
 * @brief Telink去初始化串口，关闭串口设备描述符
 * @return rt_err_t 成功/失败
 * @date 2023-02-02
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-02-02      <td>lisonglin     <td>新建
 * </table>
 */
static rt_err_t _hl_mod_telink_serial_deinit(void)
{
    rt_err_t result = RT_EOK;

    if (RT_NULL == s_telink.serial) {
        return RT_ERROR;
    }

    result = rt_device_close(s_telink.serial);
    if (RT_EOK != result) {
        LOG_E("[%s][line:%d] close faild!!! \r\n", __FUNCTION__, __LINE__);
        return RT_ERROR;
    }
    s_telink.serial = NULL;

    return RT_EOK;
}

/* Exported functions --------------------------------------------------------*/
uint8_t hl_mod_telink_init(rt_mq_t* input_msq)
{
    if (NULL == input_msq) {
        LOG_E("[%s][line:%d]Input_Msq is NULL!!! \r\n", __FUNCTION__, __LINE__, input_msq);
        return 1;
    }
    if (s_telink.init_flag) {
        LOG_E("[%s][line:%d]Telink Inited\r\n", __FUNCTION__, __LINE__);
        return 2;
    }

    rt_err_t result = RT_EOK;

#if HL_IS_TX_DEVICE()
    // 使能TX相关GPIO引脚
    // hl_hal_gpio_init(GPIO_PWR_EN);
    hl_hal_gpio_init(GPIO_DC3V3_EN);
    hl_hal_gpio_init(GPIO_2831P_EN);
    hl_hal_gpio_init(GPIO_RF_PWR_EN);
    // hl_hal_gpio_high(GPIO_PWR_EN);
    hl_hal_gpio_high(GPIO_DC3V3_EN);
    hl_hal_gpio_high(GPIO_2831P_EN);
    hl_hal_gpio_high(GPIO_RF_PWR_EN);
#else
    // 使能RX相关GPIO引脚
    hl_hal_gpio_init(GPIO_RF_PWR_EN);
    hl_hal_gpio_high(GPIO_RF_PWR_EN);
#endif

    // Telink获取并赋值APP层下发的消息队列指针
    s_telink.app_msq     = input_msq;
    s_telink.thread_flag = RT_TRUE;
    // Telink消息队列结构体赋初值
    app_msg_t.sender = TELINK_MODE;

    /* 初始化Telink模块串口交互所需的资源 */
    // 初始化hup
    s_telink.hup.hup_handle.role           = EM_HUP_ROLE_MASTER;
    s_telink.hup.hup_handle.timer_state    = EM_HUP_TIMER_DISABLE;
    s_telink.hup.hup_handle.frame_data_len = TELINK_HUP_BUF_SIZE;
    hl_util_hup_init(&s_telink.hup, s_telink_hup_buf, NULL, _telink_hup_success_handle_cb);

    // 初始化fifo
    hl_util_fifo_init(&s_telink.fifo, s_telink_fifo_buf, TELINK_FIFO_BUF_SIZE);

    // 初始化Telink模块串口设备
    result = _hl_mod_telink_serial_init();
    if (RT_EOK != result) {
        LOG_E("[%s][line:%d] result(%d)!!! \r\n", __FUNCTION__, __LINE__, result);
        return 3;
    }

    // 置模块开关标志位
    s_telink.init_flag = 1;

    return 0;
}

uint8_t hl_mod_telink_deinit(void)
{
    if (!s_telink.init_flag) {
        LOG_E("[%s][line:%d]Not Init Telink Module, Deinit ERROR!", __FUNCTION__, __LINE__);
        return 1;
    }

    rt_err_t result = RT_EOK;

    // 去初始化Telink模块串口设备
    result = _hl_mod_telink_serial_deinit();
    if (RT_EOK != result) {
        LOG_E("[%s][line:%d] result(%d)!!! \r\n", __FUNCTION__, __LINE__, result);
        return 2;
    }

    // 去初始化hup、fifo工具
    hl_util_hup_deinit(&s_telink.hup);
    hl_util_fifo_deinit(&s_telink.fifo);

    // 置模块开关标志位
    s_telink.init_flag = 0;

    return 0;
}

uint8_t hl_mod_telink_start(void)
{
    if (s_telink.start_flag) {
        LOG_E("[%s][line:%d]Telink Started\r\n", __FUNCTION__, __LINE__);
        return 1;
    }
    if (!s_telink.init_flag) {
        LOG_E("[%s][line:%d]Telink Not Init, Can't Start Telink Mod!\r\n", __FUNCTION__, __LINE__);
        return 2;
    }

    rt_err_t result = RT_EOK;

    // 清空fifo等资源
    hl_util_fifo_clear(&s_telink.fifo);

    // 初始化Telink线程资源
    s_telink.thread_id = rt_thread_create("telink", hl_mod_telink_thread_entry, RT_NULL, TELINK_THREAD_STACK_SIZE,
                                          TELINK_THREAD_PRIORITY, TELINK_THREAD_TIMESLICE);
    if (RT_EOK != result) {
        LOG_E("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FUNCTION__, __LINE__, result);
        return 3;
    }

    // 启动Telink线程
    result = rt_thread_startup(s_telink.thread_id);
    if (RT_EOK != result) {
        LOG_E("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FUNCTION__, __LINE__, result);
        return 4;
    }

    s_telink.start_flag = 1;

    return 0;
}
MSH_CMD_EXPORT(hl_mod_telink_start, telink start cmd);

uint8_t hl_mod_telink_stop(void)
{
    if (!s_telink.start_flag) {
        LOG_E("[%s][line:%d]Telink Not Start, Can't Stop Telink Mod!\r\n", __FUNCTION__, __LINE__);
        return 1;
    }
    rt_err_t result = RT_EOK;

    // 脱离Telink线程
    result = rt_thread_delete(s_telink.thread_id);
    if (RT_EOK != result) {
        LOG_E("[%s][line:%d] delete faild!!! \r\n", __FUNCTION__, __LINE__);
        return 2;
    }

    s_telink.start_flag = 0;

    return 0;
}
MSH_CMD_EXPORT(hl_mod_telink_stop, telink stop cmd);

uint8_t hl_mod_telink_ioctl(uint8_t cmd, uint8_t* data_addr, uint16_t data_len)
{
    if (TELINK_UART_BUF_SIZE < data_len) {
        LOG_E("\n[%s][line:%d] data_len is too long!!! \n", __FUNCTION__, __LINE__);
        return 1;
    } else if (RT_NULL == s_telink.serial) {
        LOG_E("\n[%s][line:%d] telink serial is NULL!!! \n", __FUNCTION__, __LINE__);
        return 2;
    }
    uint8_t frame_buf[TELINK_UART_BUF_SIZE] = { 0 };

    uint16_t frame_len =
        hl_util_hup_encode(s_telink.hup.hup_handle.role, cmd, frame_buf, TELINK_UART_BUF_SIZE, data_addr, data_len);

    frame_len = rt_device_write(s_telink.serial, 0, frame_buf, frame_len);

    return 0;
}

void telink_send_cmd(int argc, char** argv)
{
    uint8_t cmd  = (uint8_t)atoi(argv[1]);
    uint8_t data = (uint8_t)atoi(argv[2]);
    uint8_t len  = (uint8_t)atoi(argv[3]);

    if (cmd == 6 && data < 5) {
        LOG_E("[ERROR]telink set rf power too high!\n\n");
        return;
    }

    LOG_I("send data = %d\n", data);

    hl_mod_telink_ioctl(cmd, &data, len);
}
MSH_CMD_EXPORT(telink_send_cmd, telink io ctrl cmd);

void telink_bypass(int argc, char** argv)
{
    if (argc != 3) {
        LOG_E("bypass send arg (telink_bypass + CMD + DATA)\n");
        return;
    }

    uint8_t cmd          = (uint8_t)atoi(argv[1]);
    uint8_t send_buf[50] = { 0 };

#if HL_IS_TX_DEVICE()
    rt_strncpy(send_buf, argv[2], rt_strlen(argv[2]));
#else
    rt_strncpy(&send_buf[1], argv[2], rt_strlen(argv[2]));
    send_buf[0] = 0x02;
#endif
    hl_mod_telink_ioctl(cmd, send_buf, rt_strlen(send_buf));
    LOG_I("bypass send data = %s\n", send_buf);
    LOG_I("bypass send data len = %d\n", rt_strlen(send_buf));
}
MSH_CMD_EXPORT(telink_bypass, telink send bypass cmd);

void telink_get_local_mac(void)
{
    uint8_t data = 0;
    hl_mod_telink_ioctl(HL_RF_GET_LOCAL_MAC_CMD, &data, 0);
}
MSH_CMD_EXPORT(telink_get_local_mac, telink get local mac cmd);

void telink_get_remote_mac(void)
{
    uint8_t data = 0;

    hl_mod_telink_ioctl(HL_RF_GET_REMOTE_MAC_CMD, &data, 0);
}
MSH_CMD_EXPORT(telink_get_remote_mac, telink get remote mac cmd);

static uint8_t _hl_str2hex(uint8_t* str)
{
    if (rt_strlen(str) < 2) {
        return 0;
    }

    uint8_t ret  = 0;
    uint8_t data = 0;

    for (int a = 0; a < 2; a++) {
        data = atoi(str[a]);
        if (isalpha(str[a])) {
            data = str[a] - 'a' + 10;
        } else {
            data = str[a] - '0';
        }

        ret |= (data << (4 * (1 - a)));
    }

    return ret;
}

void telink_set_remote_mac(int argc, char** argv)
{
    if (argc != 3) {
        LOG_E("\n[error]set remote mac(telink_set_remote_mac + channel + MAC)\n");
    }
    hl_rf_pair_info_t info;

    info.chn    = (uint8_t)atoi(argv[1]);
    info.mac[0] = _hl_str2hex((uint8_t*)argv[2]);
    info.mac[1] = _hl_str2hex((uint8_t*)argv[3]);
    info.mac[2] = _hl_str2hex((uint8_t*)argv[4]);
    info.mac[3] = _hl_str2hex((uint8_t*)argv[5]);
    info.mac[4] = _hl_str2hex((uint8_t*)argv[6]);
    info.mac[5] = _hl_str2hex((uint8_t*)argv[7]);

    LOG_I("\nSet MAC:");
    for (int i = 0; i < 6; i++) {
        LOG_I("%02X ", info.mac[i]);
    }
    LOG_I("\n");
    hl_mod_telink_ioctl(HL_RF_SET_REMOTE_MAC_CMD, (uint8_t*)&info, sizeof(info));
}
MSH_CMD_EXPORT(telink_set_remote_mac, telink set remote mac cmd);

/*
 * EOF
 */
