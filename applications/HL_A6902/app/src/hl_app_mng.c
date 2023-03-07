/**
 * @file hl_app_mng.c
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
#include "hl_mod_input.h"
#include "hl_mod_display.h"
#include "hl_mod_audio.h"
#include "hl_mod_telink.h"
#include "hl_mod_apple_auth.h"
#include "hl_mod_pm.h"
#include "hl_mod_upgrade.h"
#include "hl_mod_euc.h"
#include "hl_app_audio_msg_pro.h"
#include "hl_app_com_msg_pro.h"
#include "hl_app_disp_msg_pro.h"
#include "hl_app_iap2_msg_pro.h"
#include "hl_app_input_msg_pro.h"
#include "hl_app_pm_msg_pro.h"
#include "hl_app_rf_msg_pro.h"
#include "hl_app_upgrade_msg_pro.h"
#include "hl_util_general_type.h"
#include "hl_mod_wdog.h"
#include "hl_util_nvram.h"
#include "hl_board_commom.h"
#include "hl_hal_gpio.h"
#include "hl_drv_aw2016a.h"
#include "hl_drv_rm690a0.h"

#define DBG_SECTION_NAME "app_mng"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/// 应用层消息队列，用于接收模块的消息
struct rt_messagequeue hl_app_mq;
/// 应用层消息队列池
static char hl_app_msg_pool[2048];

/// 模块层发送给APP的消息格式
#if HL_IS_TX_DEVICE()
/// tx app 状态信息
tx_app_info_t tx_info = { 0 };
#else
/// rx app 状态信息
rx_app_info_t rx_info = { 0 };
#endif

#if BQB_TEST
/// BQB RK透传串口描述符
rt_device_t rk_bqb_serial = NULL;
/// BQB Telink透传串口描述符
rt_device_t tl_bqb_serial = NULL;

uint8_t bqb_send_buf[1024] = { 0 };
uint8_t bqb_recv_buf[1024] = { 0 };
/// BQB RK接收数据FIFO
static hl_util_fifo_t sg_bqb_rk_rev_fifo;
/// BQB Telink接收数据FIFO
static hl_util_fifo_t sg_bqb_tl_rev_fifo;

/// BQB FIFO接收缓冲区
static uint8_t sg_bqb_rk_fifo_buf[1024] = { 0 };
static uint8_t sg_bqb_tl_fifo_buf[1024] = { 0 };

/// BQB 串口中断接收临时缓冲区
static uint8_t sg_bqb_rk_uart_recv_buf[1024] = { 0 };
static uint8_t sg_bqb_tl_uart_recv_buf[1024] = { 0 };
#endif

/* Private function(only *.c)  -----------------------------------------------*/
int hl_app_info(int argc, char** argv);

#if HL_IS_TX_DEVICE()
void hl_app_param_loader(void)
{
    uint8_t param = 0;

    hl_util_nvram_param_get_integer("MUTE_OPEN", &tx_info.mute_flag, 0);
    hl_util_nvram_param_get_integer("REC_PROTECT_OPEN", &tx_info.rec_protect_flag, 0);
    hl_util_nvram_param_get_integer("REC_AUTO_OPEN", &tx_info.rec_auto_flag, 0);
    hl_util_nvram_param_get_integer("DENOISE_PROTECT_OPEN", &tx_info.denoise_protect_flag, 0);
    hl_util_nvram_param_get_integer("DENOISE_AUTO_OPEN", &tx_info.denoise_auto_flag, 0);
    hl_util_nvram_param_get_integer("DENOISE_OPEN", &param, 0);   
    tx_info.denoise_flag = param;
    hl_util_nvram_param_get_integer("DENOISE_CLASS", &tx_info.denoise_class, 0);

    hl_util_nvram_param_get_integer("TX_GAIN", &tx_info.gain, 0);
    hl_util_nvram_param_get_integer("TX_UAC_GAIN", &tx_info.uac_gain, 0);
}

void hl_app_param_fun(void)
{
    hl_switch_e param_switch = HL_SWITCH_OFF;

    if (tx_info.mute_flag == 1) {
        param_switch = HL_SWITCH_ON;
        hl_mod_audio_io_ctrl(HL_AUDIO_SET_MUTE_CMD, &param_switch, 1);
    }

    if (tx_info.rec_auto_flag == 1) {
        tx_info.rec_flag = 1;
        param_switch     = HL_SWITCH_ON;
        hl_mod_audio_io_ctrl(HL_AUDIO_RECORD_CMD, &param_switch, 1);
        hl_app_pm_timer_set();
    }

    if (tx_info.denoise_flag == 1) {
        //tx_info.denoise_flag = 1;
        param_switch         = HL_SWITCH_ON;
        // 降噪等级设置 。。。
        hl_mod_audio_io_ctrl(HL_AUDIO_SET_DENOISE_CMD, &param_switch, 1);
        //hl_mod_audio_io_ctrl(HL_AUDIO_SET_DENOISE_CMD, &param_switch, 1);
    }
    hl_app_disp_state_led_set();
}

void hl_app_param_reset(void)
{
    uint8_t param = 0;
    hl_switch_e param_switch = HL_SWITCH_OFF;

    tx_info.mute_flag = 0;
    hl_util_nvram_param_set_integer("MUTE_OPEN", tx_info.mute_flag);
    tx_info.rec_protect_flag =0;
    hl_util_nvram_param_set_integer("REC_PROTECT_OPEN", tx_info.rec_protect_flag);
    tx_info.rec_auto_flag =0;
    hl_util_nvram_param_set_integer("REC_AUTO_OPEN", tx_info.rec_auto_flag);
    tx_info.denoise_protect_flag =0;
    hl_util_nvram_param_set_integer("DENOISE_PROTECT_OPEN", tx_info.denoise_protect_flag);
    tx_info.denoise_auto_flag =0;
    hl_util_nvram_param_set_integer("DENOISE_AUTO_OPEN", tx_info.denoise_auto_flag);
    param = 0;
    tx_info.denoise_flag = param;
    hl_util_nvram_param_set_integer("DENOISE_OPEN", param);       
    tx_info.denoise_class = 0;
    hl_util_nvram_param_set_integer("DENOISE_CLASS", tx_info.denoise_class);

    tx_info.gain = 0;
    hl_util_nvram_param_set_integer("TX_GAIN", tx_info.gain);
    tx_info.uac_gain = 0;
    hl_util_nvram_param_set_integer("TX_UAC_GAIN", tx_info.uac_gain);


    if (tx_info.mute_flag == 0) {
        param_switch = HL_SWITCH_OFF;
        hl_mod_audio_io_ctrl(HL_AUDIO_SET_MUTE_CMD, &param_switch, 1);
    }

    if (tx_info.denoise_flag == 0) {
        param_switch = HL_SWITCH_OFF;
        // 降噪等级设置 。。。
        hl_mod_audio_io_ctrl(HL_AUDIO_SET_DENOISE_CMD, &param_switch, 1);
    }

    hl_app_audio_gain(tx_info.gain);
    hl_app_audio_gain_uac(tx_info.uac_gain);    
}

#else
void hl_app_param_loader(void)
{
    hl_util_nvram_param_get_integer("RX_HP_L_GAIN", &rx_info.hp_gain, 6);
    // hl_util_nvram_param_get_integer("RX_HP_R_GAIN", &rx_info.hp_gain, 6);
    hl_util_nvram_param_get_integer("RX_CAM_L_GAIN", &rx_info.cam_gain_l, 0);
    hl_util_nvram_param_get_integer("RX_CAM_R_GAIN", &rx_info.cam_gain_r, 0);
    hl_util_nvram_param_get_integer("RX_UAC_GAIN", &rx_info.uac_gain, 0);
}

void hl_app_param_fun(void)
{
    int8_t param = 0;

    // 读取TX的配置 发送配置给屏幕
    hl_util_nvram_param_get_integer("MUTE_OPEN", &param, 0);
    hl_mod_display_io_ctrl(TX1_MUTE_SWITCH_SWITCH_CMD, &param, 1);
    hl_mod_display_io_ctrl(TX2_MUTE_SWITCH_SWITCH_CMD, &param, 1);

    hl_util_nvram_param_get_integer("REC_PROTECT_OPEN", &param, 0);
    hl_mod_display_io_ctrl(AUTO_RECORD_PORTECT_SWITCH_CMD, &param, 1);

    hl_util_nvram_param_get_integer("REC_AUTO_OPEN", &param, 0);
    hl_mod_display_io_ctrl(AUTO_RECORD_SWITCH_CMD, &param, 1);

    hl_util_nvram_param_get_integer("TX_GAIN", &param, 0);
    hl_mod_display_io_ctrl(TX1_GAIN_VAL_CMD, &param, 1);
    hl_util_nvram_param_get_integer("TX_GAIN2", &param, 0);
    hl_mod_display_io_ctrl(TX2_GAIN_VAL_CMD, &param, 1);    

    param = rx_info.uac_gain; 
    hl_mod_display_io_ctrl(UAC_OUT_VOLUME_VAL_CMD, &param, 1);   
    hl_mod_audio_io_ctrl(HL_AUDIO_SET_GAIN_UAC_CMD, &rx_info.uac_gain, 4);

    param = rx_info.cam_gain_l / 2;
    hl_mod_display_io_ctrl(TX1_LINE_OUT_VOLUME_VAL_CMD, &param, 1);

    param = rx_info.cam_gain_r / 2;
    hl_mod_display_io_ctrl(TX2_LINE_OUT_VOLUME_VAL_CMD, &param, 1);

    param = rx_info.hp_gain / 2;
    hl_mod_display_io_ctrl(MONITOR_VOLUME_VAL_CMD, &param, 1);
}

void hl_app_param_reset(void)
{
    uint8_t param = 0;
    hl_switch_e param_switch = HL_SWITCH_OFF;

    param = 0;
    hl_util_nvram_param_set_integer("MUTE_OPEN", param);
    hl_mod_display_io_ctrl(TX1_MUTE_SWITCH_SWITCH_CMD, &param, 1);
    hl_mod_display_io_ctrl(TX2_MUTE_SWITCH_SWITCH_CMD, &param, 1);

    param = 0;
    hl_util_nvram_param_set_integer("REC_PROTECT_OPEN", param);
    hl_mod_display_io_ctrl(AUTO_RECORD_PORTECT_SWITCH_CMD, &param, 1);

    param = 0;
    hl_util_nvram_param_set_integer("REC_AUTO_OPEN", param);
    hl_mod_display_io_ctrl(AUTO_RECORD_SWITCH_CMD, &param, 1);

    param = 0;
    hl_util_nvram_param_set_integer("TX_GAIN", param);
    hl_mod_display_io_ctrl(TX1_GAIN_VAL_CMD, &param, 1);
    hl_util_nvram_param_set_integer("TX_GAIN2", param);
    hl_mod_display_io_ctrl(TX2_GAIN_VAL_CMD, &param, 1);
    
    param = 0;
    rx_info.uac_gain = 0;
    hl_util_nvram_param_set_integer("RX_UAC_GAIN", param);        
    hl_mod_display_io_ctrl(UAC_OUT_VOLUME_VAL_CMD, &param, 1);
    hl_mod_audio_io_ctrl(HL_AUDIO_SET_GAIN_UAC_CMD, &rx_info.uac_gain, 4);

    rx_info.hp_gain = 0;
    hl_util_nvram_param_set_integer("RX_HP_L_GAIN", rx_info.hp_gain);
    // hl_util_nvram_param_set_integer("RX_HP_R_GAIN", rx_info.hp_gain);
    hl_mod_audio_io_ctrl(HL_AUDIO_SET_HP_GAIN_L_CMD, &rx_info.hp_gain, 4);
    hl_mod_audio_io_ctrl(HL_AUDIO_SET_HP_GAIN_R_CMD, &rx_info.hp_gain, 4);

    rx_info.cam_gain_l = 0;
    rx_info.cam_gain_r = 0;
    hl_util_nvram_param_set_integer("RX_CAM_L_GAIN", rx_info.cam_gain_l);
    hl_util_nvram_param_set_integer("RX_CAM_R_GAIN", rx_info.cam_gain_r);
    hl_mod_audio_io_ctrl(HL_AUDIO_SET_CAM_GAIN_L_CMD, &rx_info.cam_gain_l, 4);
    hl_mod_audio_io_ctrl(HL_AUDIO_SET_CAM_GAIN_R_CMD, &rx_info.cam_gain_r, 4);

    param = rx_info.cam_gain_l/2;
    hl_mod_display_io_ctrl(TX1_LINE_OUT_VOLUME_VAL_CMD, &param, 1);
    param = rx_info.cam_gain_r/2;
    hl_mod_display_io_ctrl(TX2_LINE_OUT_VOLUME_VAL_CMD, &param, 1);

    param = rx_info.hp_gain/2;
    hl_mod_display_io_ctrl(MONITOR_VOLUME_VAL_CMD, &param, 1);
}
#endif
/* Exported functions --------------------------------------------------------*/
void hl_app_msg_thread(void* parameter)
{
    hl_mod_feed_dog();
    mode_to_app_msg_t msg = { 0 };

    hl_app_mng_charger_entry(&hl_app_mq);

    hl_mod_upgrade_init(&hl_app_mq);

#if HL_IS_TX_DEVICE()
    if (tx_info.mstorage_plug == 0) {
#else
    if (rx_info.mstorage_plug == 0) {
#endif
        hl_mod_audio_io_ctrl(HL_AUDIO_CHECK_DFS_CMD, NULL, 0);
        hl_mod_upgrade_io_ctrl(HL_UPGRADE_OPEN_CMD, NULL, 0);
    }
    hl_app_param_fun();

    //退出charger app后重新通知pm上报一次电量和充电状态信息，因为第一次上报的信息在charger app中可能被清除了，没有处理
    hl_mod_pm_ctrl(HL_PM_BAT_INFO_UPDATE_CMD, RT_NULL, 0);
    hl_mod_input_io_ctrl(HL_INPUT_RESET_CMD, RT_NULL, 0);

    rt_memset(&msg, 0, sizeof(msg));
    while (1) {
        if (rt_mq_recv(&hl_app_mq, &msg, sizeof(msg), -1) == RT_EOK) {
            // LOG_D("recv msg sender:%d, cmd:%d, param:%d !!!", msg.sender, msg.cmd, msg.param);
            switch (msg.sender) {
                case INPUT_MODE:
                    hl_app_input_msg_pro(&msg);
#if !HL_IS_TX_DEVICE()
                    hl_mod_display_io_ctrl(MSG_INPUT_UPDATE_CMD, &msg, 0);
#endif
                    break;
                case DISPLAY_MODE:
                    hl_app_disp_msg_pro(&msg);
                    break;
                case EXT_COM_MODE:
                    hl_app_com_msg_pro(&msg);
                    break;
                case UPGRADE_MODE:
                    hl_app_upgrade_msg_pro(&msg);
                    break;
                case APPLE_AUTH_MODE:
                    hl_app_iap2_msg_pro(&msg);
                    break;
                case TELINK_MODE:
                    hl_app_rf_msg_pro(&msg);
                    break;
                case AUDIO_MODE:
                    hl_app_audio_msg_pro(&msg);
                    break;
                case PM_MODE:
                    hl_app_pm_msg_pro(&msg);
                    break;
                default:
                    LOG_E("sender(%d) unkown!!!", msg.sender);
                    break;
            }
        }
        // rt_thread_mdelay(20);
        hl_mod_feed_dog();
    }
}

#if BQB_TEST
void hl_app_telink_bqb_bypass(void* parameter)
{
    uint32_t rk_data_size    = 0;
    uint32_t tl_data_size    = 0;
    uint8_t  rk_rev_buf[512] = { 0 };
    uint8_t  tl_rev_buf[512] = { 0 };

    while (1) {
        rk_data_size = hl_util_fifo_data_size(&sg_bqb_rk_rev_fifo);
        rk_data_size = ((rk_data_size > 256) ? 256 : rk_data_size);
        if (rk_data_size > 0) {
            rk_data_size = hl_util_fifo_read(&sg_bqb_rk_rev_fifo, rk_rev_buf, rk_data_size);
            rt_device_write(tl_bqb_serial, 0, rk_rev_buf, rk_data_size);
        }

        tl_data_size = hl_util_fifo_data_size(&sg_bqb_tl_rev_fifo);
        tl_data_size = ((tl_data_size > 256) ? 256 : tl_data_size);
        if (tl_data_size > 0) {
            tl_data_size = hl_util_fifo_read(&sg_bqb_tl_rev_fifo, tl_rev_buf, tl_data_size);
            rt_device_write(rk_bqb_serial, 0, tl_rev_buf, tl_data_size);
        }
        hl_mod_feed_dog();
    }
}

static rt_err_t _tl_uart_receive_cb(rt_device_t dev, rt_size_t size)
{
    rt_size_t data_size = 0;

    // 读出串口数据
    data_size = rt_device_read(tl_bqb_serial, 0, sg_bqb_tl_uart_recv_buf, size);
    // 将串口数据写入FIFO
    hl_util_fifo_write(&sg_bqb_tl_rev_fifo, sg_bqb_tl_uart_recv_buf, data_size);

    return RT_EOK;
}

static rt_err_t _rk_uart_receive_cb(rt_device_t dev, rt_size_t size)
{
    rt_size_t data_size = 0;

    // 读出串口数据
    data_size = rt_device_read(rk_bqb_serial, 0, sg_bqb_rk_uart_recv_buf, size);

    // 将串口数据写入FIFO
    hl_util_fifo_write(&sg_bqb_rk_rev_fifo, sg_bqb_rk_uart_recv_buf, data_size);

    return RT_EOK;
}

static rt_err_t _hl_bqb_serial_init(void)
{
    rt_err_t result = RT_EOK;
    int      ret    = 0;

    // 初始化fifo
    ret |= hl_util_fifo_init(&sg_bqb_rk_rev_fifo, sg_bqb_rk_fifo_buf, 1024);
    ret |= hl_util_fifo_init(&sg_bqb_tl_rev_fifo, sg_bqb_tl_fifo_buf, 1024);
    if (ret) {
        LOG_E("[%s][line:%d] fifo init faild!!! \r\n", __FUNCTION__, __LINE__);
        return RT_ERROR;
    }

    // 查找系统中的串口设备
    rk_bqb_serial = rt_device_find("uart0");
    tl_bqb_serial = rt_device_find("uart2");
    if ((RT_NULL == rk_bqb_serial) || (RT_NULL == tl_bqb_serial)) {
        LOG_E("[%s][line:%d] serial find faild!!! \r\n", __FUNCTION__, __LINE__);
        return RT_ERROR;
    }

    // 以中断接收及轮询发送模式打开串口设备
    result = rt_device_open(rk_bqb_serial, RT_DEVICE_FLAG_INT_RX);
    if (RT_EOK != result) {
        LOG_E("[%s][line:%d] rt_device_open faild!!! \r\n", __FUNCTION__, __LINE__);
        return RT_ERROR;
    }
    rt_device_set_rx_indicate(rk_bqb_serial, _rk_uart_receive_cb);

    result = rt_device_open(tl_bqb_serial, RT_DEVICE_FLAG_INT_RX);
    if (RT_EOK != result) {
        LOG_E("[%s][line:%d] rt_device_open faild!!! \r\n", __FUNCTION__, __LINE__);
        return RT_ERROR;
    }
    // 设置接收回调函数
    rt_device_set_rx_indicate(tl_bqb_serial, _tl_uart_receive_cb);

    return RT_EOK;
}

void hl_ble_bqb_test_start(void)
{
    rt_err_t    result       = RT_EOK;
    rt_thread_t app_task_tid = RT_NULL;

    // 清空fifo等资源
    hl_util_fifo_clear(&sg_bqb_rk_rev_fifo);
    hl_util_fifo_clear(&sg_bqb_tl_rev_fifo);

    app_task_tid = rt_thread_create("hl_app_telink_bqb_bypass", hl_app_telink_bqb_bypass, RT_NULL, 10240, 15, 5);

    if (app_task_tid) {
        LOG_E("telink_bypass_thread start!!!");
        rt_thread_startup(app_task_tid);
    } else {
        LOG_E("thread create err!!!");
    }
}
#endif

void hl_app_mng_init(void)
{
    rt_err_t    ret          = 0;
    uint8_t     bqb_flag     = 0;
    rt_thread_t app_task_tid = RT_NULL;

    ret = rt_mq_init(&hl_app_mq, "AppMsg", &hl_app_msg_pool[0], sizeof(mode_to_app_msg_t), sizeof(hl_app_msg_pool),
                     RT_IPC_FLAG_FIFO);
    if (ret != RT_EOK) {
        LOG_E("message queuecreate init err!!!");
        return;
    }

    hl_board_nvram_init();

#if BQB_TEST
    ret = hl_util_nvram_param_get_integer("BQB", &bqb_flag, 0);
    LOG_E("----BQB[%d]RET[%d]----\r\n", bqb_flag, ret);
    if (!ret && bqb_flag) {
#if HL_IS_TX_DEVICE()
        // 使能TX相关GPIO引脚
        // hl_hal_gpio_init(GPIO_PWR_EN);
        hl_hal_gpio_init(GPIO_DC3V3_EN);
        hl_hal_gpio_init(GPIO_2831P_EN);
        hl_hal_gpio_init(GPIO_RF_PWR_EN);
        hl_hal_gpio_init(GPIO_ALL_POWER);
        // hl_hal_gpio_high(GPIO_PWR_EN);
        hl_hal_gpio_high(GPIO_DC3V3_EN);
        hl_hal_gpio_high(GPIO_2831P_EN);
        hl_hal_gpio_high(GPIO_RF_PWR_EN);
        hl_hal_gpio_high(GPIO_ALL_POWER);

        hl_drv_aw2016a_led_ctrl_st led;
        led.breath_mode = HL_DRV_AW2016A_BREATH_MODE_KEEP;
        led.color       = HL_DRV_AW2016A_COLOR_WHITE;
        hl_drv_aw2016a_init();
        hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_LED_CTRL, &led, sizeof(led));
#else
        // 使能RX相关GPIO引脚
        hl_hal_gpio_init(GPIO_ALL_POWER);
        hl_hal_gpio_high(GPIO_ALL_POWER);
        hl_hal_gpio_init(GPIO_RF_PWR_EN);
        hl_hal_gpio_high(GPIO_RF_PWR_EN);

        uint16_t color = 0xFFFF;
        hl_drv_rm690a0_init();
        hl_drv_rm690a0_io_ctrl(FULL_COLOR_DISPLAY_INIT, NULL, 0);
        hl_drv_rm690a0_io_ctrl(FULL_COLOR_DISPLAY_CMD, &color, 2);
#endif
        hl_util_nvram_param_set_integer("BQB", 0);
        hl_util_nvram_param_save();
        ret = _hl_bqb_serial_init();
        rt_thread_mdelay(5000);

        app_task_tid = rt_thread_create("hl_app_telink_bqb_bypass", hl_app_telink_bqb_bypass, RT_NULL, 10240, 15, 5);
        if (app_task_tid) {
            LOG_E("\r\ntelink_bypass_thread start!!!\r\n");
            rt_thread_startup(app_task_tid);
        } else {
            LOG_E("thread create err!!!");
        }
    } else {
#endif
        hl_app_param_loader();
        hl_mod_input_init(&hl_app_mq);
        hl_mod_display_init(&hl_app_mq);
        hl_mod_pm_init(&hl_app_mq);
        hl_mod_pm_start();
        hl_mod_euc_init(&hl_app_mq);
        hl_mod_euc_start();

        app_task_tid = rt_thread_create("app_task", hl_app_msg_thread, RT_NULL, 10240, 10, 5);
        if (app_task_tid) {
            rt_thread_startup(app_task_tid);
        } else {
            LOG_E("app task thread create err!!!");
        }

        hl_app_info(0, NULL);
#if BQB_TEST
    }
#endif
}

// 开机，初始化模块
void hl_app_mng_powerOn(void)
{
    uint8_t              value = 0;
    int                  msc_open_flag;
    uint8_t              ret;
    hl_rf_bypass_state_t bypass_state;

    LOG_I("power on");
    hl_mod_pm_ctrl(HL_PM_POWER_UP_CMD, NULL, 0);
    // hl_mod_display_init(&hl_app_mq);
    hl_mod_telink_init(&hl_app_mq);
    hl_mod_telink_start();

    hl_mod_audio_init(&hl_app_mq);
#if HL_IS_TX_DEVICE()
    //开机同步数据
    bypass_state.chn   = tx_info.rf_chn;
    bypass_state.state = tx_info.charge_flag == 1 ? HL_RF_ON : HL_RF_OFF;
    hl_mod_telink_ioctl(HL_RF_BYPASS_CHARGE_CMD, &bypass_state, sizeof(bypass_state));
#else
    ret = hl_util_nvram_param_get_integer("MSC_OPEN", &msc_open_flag, 0);
    if (ret == 1) {
        rt_kprintf("nvram be used before not init\n");
        hl_board_nvram_init();
        ret = hl_util_nvram_param_get_integer("MSC_OPEN", &msc_open_flag, 0);
    }

    LOG_D("msc_open_flag = %d ,ret = %d ", msc_open_flag, ret);
    if (msc_open_flag == 1) {
        hl_mod_audio_deinit();  //hl_mod_audio_init(&hl_app_mq);
    }

    hl_mod_apple_auth_init(&hl_app_mq);
#endif

    hl_mod_euc_init(&hl_app_mq);
    hl_mod_euc_start();

#if HL_IS_TX_DEVICE()
#else
    hl_mod_audio_io_ctrl(HL_AUDIO_SET_GAIN_CMD, &rx_info.cur_volume_r, 2);
    if (rx_info.charge_flag == 0) {
        value = 0;
    } else if (rx_info.charge_flag == 1) {
        value = 1;
    } else if (rx_info.charge_flag == 2) {
        value = 0;
    } else if (rx_info.charge_flag == 3) {
        value = 0;
    }
    LOG_D("rx_info.charge_flag=%d ", rx_info.charge_flag);
    hl_mod_display_io_ctrl(RX_CHARGE_STATUS_SWITCH_CMD, &value, 1);
#endif
}

// 关机，去初始化模块
void hl_app_mng_powerOff(void)
{
    int     msc_open_flag;
    uint8_t ret;

    LOG_I("power off");
    hl_util_nvram_param_save();
#if HL_IS_TX_DEVICE()
    hl_hal_gpio_low(GPIO_DC3V3_EN);
#else
    hl_hal_gpio_high(GPIO_CODEC_EN);
#endif
    hl_mod_euc_stop();
    hl_mod_euc_deinit();

#if !HL_IS_TX_DEVICE()
    ret = hl_util_nvram_param_get_integer("MSC_OPEN", &msc_open_flag, 0);
    if (ret == 1) {
        LOG_E("nvram be used before not init");
        hl_board_nvram_init();
        ret = hl_util_nvram_param_get_integer("MSC_OPEN", &msc_open_flag, 0);
    }

    LOG_D("msc_open_flag = %d ,ret = %d ", msc_open_flag, ret);
    if (msc_open_flag == 0) {
        hl_mod_audio_deinit();
    }
#endif
    hl_mod_telink_stop();
    hl_mod_telink_deinit();
    hl_mod_input_deinit();
    hl_mod_display_deinit();
    hl_mod_pm_ctrl(HL_PM_POWER_DOWN_CMD, NULL, 0);
    //hl_mod_pm_stop();
    //hl_mod_pm_deinit();
}

// hl_app_info [no param]
int hl_app_info(int argc, char** argv)
{
#if HL_IS_TX_DEVICE()
    LOG_I("------show tx app info------");
    LOG_I("on_off_flag = %d", tx_info.on_off_flag);
    LOG_I("usb_plug = %d ", tx_info.usb_plug);
    LOG_I("uac_link_flag = %d ", tx_info.uac_link_flag);
    LOG_I("mstorage_plug = %d ", tx_info.mstorage_plug);
    LOG_I("ex_mic_plug = %d ", tx_info.ex_mic_plug);
    LOG_I("rec_flag = %d ", tx_info.rec_flag);
    LOG_I("denoise_flag = %d ", tx_info.denoise_flag);
    LOG_I("charge_flag = %d ", tx_info.charge_flag);
    LOG_I("mute_flag = %d ", tx_info.mute_flag);
    LOG_I("rf_state = %d ", tx_info.rf_state);
    LOG_I("tx_info.soc = %d ", tx_info.soc);
    LOG_I("tx_info.gain = %d ", tx_info.gain);
    LOG_I("tx_info.uac_gain = %d ", tx_info.uac_gain);
#else
    LOG_I("------show rx app info------");
    LOG_I("on_off_flag = %d", rx_info.on_off_flag);
    LOG_I("usb_plug = %d ", rx_info.usb_plug);
    LOG_I("uac_link_flag = %d ", rx_info.uac_link_flag);
    LOG_I("mstorage_plug = %d ", rx_info.mstorage_plug);
    LOG_I("hp_spk_plug = %d ", rx_info.hp_spk_plug);
    LOG_I("cam_spk_plug = %d ", rx_info.cam_spk_plug);
    LOG_I("charge_flag = %d ", rx_info.charge_flag);
    LOG_I("rf_state = %d ", rx_info.rf_state);
    LOG_I("rx_info.soc = %d ", rx_info.soc);
    LOG_I("cam_gain_l = %d ", rx_info.cam_gain_l);
    LOG_I("cam_gain_r = %d ", rx_info.cam_gain_r);
    LOG_I("hp_gain = %d ", rx_info.hp_gain);
#endif
    return 0;
}

INIT_APP_EXPORT(hl_app_mng_init);
MSH_CMD_EXPORT(hl_app_info, show app info cmd);


MSH_CMD_EXPORT(hl_app_param_reset, show app info cmd);

static void _hl_et_sche_hook(rt_thread_t from, rt_thread_t to)
{
    rt_kprintf("%s[%d]->%s[%d]\r\n", from->name, from->remaining_tick, to->name, to->remaining_tick);
}

static int set_sche_hook(int argc, char **argv)
{
    rt_scheduler_sethook(_hl_et_sche_hook);
    return 0;
}

MSH_CMD_EXPORT(set_sche_hook, set_sche_hook);

void hl_rf_BQB_test(int argc, char** argv)
{
    if (argc != 5) {
        rt_kprintf("\n\n\n");
        rt_kprintf("\n[ERROR]!!! input arg error !!!\n");
        rt_kprintf("\n>>>PS:hl_rf_enter_BQB_test + Ant + RMode + TMode + PowerLevel)\n\n");
        rt_kprintf("Arg[1] Select Antenna\n");
        rt_kprintf("\t\t1-ant1 | 2-ant2\n");
        rt_kprintf("Arg[2] Receive Work Mode\n");
        rt_kprintf("\t\t0-LNA Mode\n");
        rt_kprintf("\t\t1-Bypass Mode\n");
        rt_kprintf("Arg[3] Transmit Work Mode\n");
        rt_kprintf("\t\t0-Low Power Mode\n");
        rt_kprintf("\t\t1-High Power Mode\n");
        rt_kprintf("\t\t2-Bypass Mode\n");
        rt_kprintf("Arg[4] RF Power Level\n");
        rt_kprintf("\t\t8- 3.3dbm\t16- -2dbm\n");
        rt_kprintf("\t\t9- 2.8dbm\t17- -3.4dbm\n");
        rt_kprintf("\t\t10- 2.3dbm\t18- -4.8dbm\n");
        rt_kprintf("\t\t11- 1.7dbm\t19- -6.5dbm\n");
        rt_kprintf("\t\t12- 0.8dbm\t20- -8.8dbm\n");
        rt_kprintf("\t\t13- 0dbm\t21- -12.1dbm\n");
        rt_kprintf("\t\t14- -0.5dbm\t22- -17.8dbm\n");
        rt_kprintf("\t\t15- -1.4dbm\t23- -23.5dbm\n");
        rt_kprintf("Default Set: Ant0 LNA+HighPower 0dbm\n");
        rt_kprintf("\n\n\n");
        return;
    }
    uint8_t buffer[4] = { 0 };

    buffer[0] = (uint8_t)atoi(argv[1]);
    buffer[1] = (uint8_t)atoi(argv[2]);
    buffer[2] = (uint8_t)atoi(argv[3]);
    buffer[3] = (uint8_t)atoi(argv[4]);
    rt_kprintf("antenna = %d\n", buffer[0]);
    rt_kprintf("receive_mode = %d\n", buffer[1]);
    rt_kprintf("transmit_mode = %d\n", buffer[2]);
    rt_kprintf("power_level = %d\n", buffer[3]);

    hl_mod_telink_ioctl(HL_RF_ENTER_BQB_CMD, buffer, sizeof(buffer));
    hl_util_nvram_param_set_integer("BQB", 1);
    hl_util_nvram_param_save();
}
MSH_CMD_EXPORT(hl_rf_BQB_test, RF Enter BQB Test cmd);

/*
 * EOF
 */