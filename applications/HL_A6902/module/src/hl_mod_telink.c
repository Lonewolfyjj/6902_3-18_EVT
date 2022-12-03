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
#include <ctype.h>

/* typedef -------------------------------------------------------------------*/

typedef struct
{
    uint16_t number;     //升级数据包序号
    uint8_t  data[512];  //升级数据
} hl_rf_upgrade_pack_t;

/* define --------------------------------------------------------------------*/

#define TELINK_THREAD_STACK_SIZE 1024
#define TELINK_THREAD_PRIORITY 7
#define TELINK_THREAD_TIMESLICE 10
#define TELINK_UART_DEV_NAME "uart2"
#define TELINK_UART_BUF_SIZE 1024
#define TELINK_FIFO_BUF_SIZE 1024
#define TELINK_HUP_BUF_SIZE 1024

/* variables -----------------------------------------------------------------*/

/// 线程句柄
static struct rt_thread telink_thread;
/// 线程栈数组
static char telink_thread_stack[TELINK_THREAD_STACK_SIZE];
/// Telink模块句柄
static hl_mod_telink_t s_telink;
/// 串口接收临时缓冲区
static uint8_t s_uart_recv_buf[TELINK_UART_BUF_SIZE];
/// hup接收缓冲区
static uint8_t* s_telink_hup_buf;
/// fifo接收缓冲区
static uint8_t* s_telink_fifo_buf;
/// 消息队列结构体
static mode_to_app_msg_t app_msg_t;

/// 记录上次配对状态
static uint8_t old_pair_info = 0x1f;
/// 记录最新配对状态
static uint8_t new_pair_info = 0x1f;

/* Private function(only *.c)  -----------------------------------------------*/

static void telink_show_val(void)
{
    rt_kprintf("\n\nold_pair_info = %02x\n", old_pair_info);
    rt_kprintf("new_pair_info = %02x\n", new_pair_info);
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

    switch (hup_frame.cmd) {
        case HL_RF_VERSION_IND:
            app_msg_t.cmd       = HL_RF_VERSION_IND;
            app_msg_t.len       = data_len;
            app_msg_t.param.ptr = hup_frame.data_addr;
            // 上报Telink固件版本信息
            ret = rt_mq_send(s_telink.app_msq, (void*)&app_msg_t, sizeof(app_msg_t));
            break;

        case HL_RF_PAIR_STATE_IND:
            // 更新Telink模块工作状态和配对状态
            new_pair_info = (uint8_t)hup_frame.data_addr[0];
            break;

        case HL_RF_RSSI_IND:
            app_msg_t.cmd       = HL_RF_RSSI_IND;
            app_msg_t.len       = data_len;
            app_msg_t.param.ptr = hup_frame.data_addr;
            // 上报无线模块RSSI值
            ret = rt_mq_send(s_telink.app_msq, (void*)&app_msg_t, sizeof(app_msg_t));
            break;

        case HL_RF_GET_LOCAL_PAIR_INFO_IND:
            rt_kprintf("\n\n[Telink Local MAC Addr]:");
            for (int i = 0; i < 6; i++) {
                rt_kprintf("%02X ", hup_frame.data_addr[i]);
            }
            rt_kprintf("\n\n");
            app_msg_t.cmd       = HL_RF_GET_LOCAL_PAIR_INFO_IND;
            app_msg_t.len       = data_len;
            app_msg_t.param.ptr = hup_frame.data_addr;
            // 上报本地MAC地址
            ret = rt_mq_send(s_telink.app_msq, (void*)&app_msg_t, sizeof(app_msg_t));
            break;

        case HL_RF_SET_REMOTE_PAIR_INFO_IND:
            break;

        case HL_RF_GET_REMOTE_PAIR_INFO_IND:
#if HL_IS_TX_DEVICE()
            rt_kprintf("\n\nTelink Remote MAC Addr:\n[RX]: ");
            for (int i = 0; i < 6; i++) {
                rt_kprintf("%02X ", hup_frame.data_addr[i]);
            }
            rt_kprintf("\n\n\n");
#else
            rt_kprintf("\n\nTelink Remote MAC Addr:\n[TX_L]: ");
            for (int i = 0; i < 12; i++) {
                rt_kprintf("%02X ", hup_frame.data_addr[i]);
                if (i == 5) {
                    rt_kprintf("\n[TX_R]: ");
                }
            }
            rt_kprintf("\n\n\n");
#endif
            app_msg_t.cmd       = HL_RF_GET_REMOTE_PAIR_INFO_IND;
            app_msg_t.len       = data_len;
            app_msg_t.param.ptr = hup_frame.data_addr;
            // 上报本地MAC地址
            ret = rt_mq_send(s_telink.app_msq, (void*)&app_msg_t, sizeof(app_msg_t));
            break;

        case HL_RF_BYPASS_MUTE_IND:
        case HL_RF_BYPASS_DENOISE_IND:
        case HL_RF_BYPASS_VOLUME_IND:
        case HL_RF_BYPASS_RECORD_IND:
        case HL_RF_BYPASS_SETTING_IND:
            rt_kprintf("\n\n[Telink Bypass Cmd]:%02X\n", hup_frame.cmd);
            rt_kprintf("[Telink Bypass Info]:%s\n\n", hup_frame.data_addr);
            break;

        default:
            break;
    }

    if (RT_EOK != ret) {
        rt_kprintf("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FUNCTION__, __LINE__, ret);
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
    rt_err_t ret;
    uint8_t  buf[256] = { 0 };
    uint32_t data     = 0;

    while (s_telink.thread_flag == RT_TRUE) {
        // 读取数据并解析
        data = hl_util_fifo_data_size(&s_telink.fifo);
        if (data > 0) {
            data = hl_util_fifo_read(&s_telink.fifo, buf, data);
            for (uint32_t i = 0; i < data; i++) {
                hl_util_hup_decode(&s_telink.hup, buf[i]);
            }
        }

        // 延时5ms
        rt_thread_mdelay(5);

        // 更新配对状态
        if (old_pair_info != new_pair_info) {
            // 更新配对状态
            old_pair_info = new_pair_info;
            // 编辑消息结构体
            app_msg_t.cmd       = HL_RF_PAIR_STATE_IND;
            app_msg_t.len       = 1;
            app_msg_t.param.ptr = &old_pair_info;
            // 将消息结构体上传至APP层消息队列
            ret = rt_mq_send(s_telink.app_msq, (void*)&app_msg_t, sizeof(app_msg_t));
            if (RT_EOK != ret) {
                rt_kprintf("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FUNCTION__, __LINE__, ret);
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
    // 查找系统中的串口设备
    s_telink.serial = rt_device_find(TELINK_UART_DEV_NAME);
    if (RT_NULL == s_telink.serial) {
        rt_kprintf("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FUNCTION__, __LINE__, s_telink.serial);
        return RT_ERROR;
    }

    // 以中断接收及轮询发送模式打开串口设备
    rt_device_open(s_telink.serial, RT_DEVICE_FLAG_INT_RX);
    // 设置接收回调函数
    rt_device_set_rx_indicate(s_telink.serial, _telink_uart_receive_cb);

    return RT_EOK;
}

/* Exported functions --------------------------------------------------------*/
uint8_t hl_mod_telink_init(rt_mq_t* input_msq)
{
    if (NULL == input_msq) {
        rt_kprintf("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FUNCTION__, __LINE__, input_msq);
        return -1;
    }

    rt_err_t result;

#if HL_IS_TX_DEVICE()
    // 使能TX相关GPIO引脚
    hl_hal_gpio_init(GPIO_PWR_EN);
    hl_hal_gpio_init(GPIO_DC3V3_EN);
    hl_hal_gpio_init(GPIO_2831P_EN);
    hl_hal_gpio_init(GPIO_RF_PWR_EN);
    hl_hal_gpio_high(GPIO_PWR_EN);
    hl_hal_gpio_high(GPIO_DC3V3_EN);
    hl_hal_gpio_high(GPIO_2831P_EN);
    hl_hal_gpio_high(GPIO_RF_PWR_EN);
#else
    // 使能RX相关GPIO引脚
    hl_hal_gpio_init(GPIO_RF_PWR_EN);
    hl_hal_gpio_high(GPIO_RF_PWR_EN);
#endif

    // 申请缓冲区内存空间
    s_telink_hup_buf  = (uint8_t*)rt_malloc(sizeof(uint8_t) * TELINK_HUP_BUF_SIZE);
    s_telink_fifo_buf = (uint8_t*)rt_malloc(sizeof(uint8_t) * TELINK_FIFO_BUF_SIZE);
    if ((NULL == s_telink_hup_buf) || (NULL == s_telink_fifo_buf)) {
        rt_kprintf("[%s][line:%d] rt_malloc failed!!! \r\n", __FUNCTION__, __LINE__);
        return -1;
    }

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
    _hl_mod_telink_serial_init();

    // 初始化Telink线程资源
    result = rt_thread_init(&telink_thread, "telink", hl_mod_telink_thread_entry, RT_NULL, &telink_thread_stack[0],
                            sizeof(telink_thread_stack), TELINK_THREAD_PRIORITY, TELINK_THREAD_TIMESLICE);
    if (RT_EOK != result) {
        rt_kprintf("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FUNCTION__, __LINE__, result);
        return -1;
    }

    return 0;
}

uint8_t hl_mod_telink_deinit(void)
{
    // 去初始化hup、fifo工具
    hl_util_hup_deinit(&s_telink.hup);
    hl_util_fifo_deinit(&s_telink.fifo);

    // 释放已申请的缓冲区内存空间
    rt_free(s_telink_hup_buf);
    rt_free(s_telink_fifo_buf);

    return 0;
}

uint8_t hl_mod_telink_start(void)
{
    rt_err_t result;

    result = rt_thread_startup(&telink_thread);
    if (RT_EOK != result) {
        rt_kprintf("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FUNCTION__, __LINE__, result);
        return -1;
    }
    return 0;
}

uint8_t hl_mod_telink_stop(void)
{
    return 0;
}

uint8_t hl_mod_telink_ioctl(uint8_t cmd, uint8_t* data_addr, uint16_t data_len)
{
    if (TELINK_UART_BUF_SIZE < data_len) {
        rt_kprintf("[%s][line:%d] data_len is too long!!! \r\n", __FUNCTION__, __LINE__);
        return -1;
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

    if (cmd == 7 && data < 5) {
        rt_kprintf("\n[ERROR]telink set rf power too high!\n\n");
        return;
    }

    rt_kprintf("\nsend data = %d\n", data);

    hl_mod_telink_ioctl(cmd, &data, len);
}
MSH_CMD_EXPORT(telink_send_cmd, telink io ctrl cmd);

void telink_bypass(int argc, char** argv)
{
    if(argc != 3){
        rt_kprintf("\n[error]bypass send arg (telink_bypass + CMD + DATA)\n");
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
    rt_kprintf("\nbypass send data = %s\n", send_buf);
    rt_kprintf("bypass send data len = %d\n", rt_strlen(send_buf));
}
MSH_CMD_EXPORT(telink_bypass, telink send bypass cmd);

void telink_get_local_mac(void)
{
    uint8_t data = 0;
    hl_mod_telink_ioctl(HL_RF_GET_LOCAL_PAIR_INFO_CMD, &data, 0);
}
MSH_CMD_EXPORT(telink_get_local_mac, telink get local mac cmd);

void telink_get_remote_mac(void)
{
    uint8_t data = 0;

    hl_mod_telink_ioctl(HL_RF_GET_REMOTE_PAIR_INFO_CMD, &data, 0);
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
    if(argc != 3){
        rt_kprintf("\n[error]set remote mac(telink_set_remote_mac + channel + MAC)\n");
    }
    hl_rf_pair_info_t info;

    info.chn    = (uint8_t)atoi(argv[1]);
    info.mac[0] = _hl_str2hex((uint8_t*)argv[2]);
    info.mac[1] = _hl_str2hex((uint8_t*)argv[3]);
    info.mac[2] = _hl_str2hex((uint8_t*)argv[4]);
    info.mac[3] = _hl_str2hex((uint8_t*)argv[5]);
    info.mac[4] = _hl_str2hex((uint8_t*)argv[6]);
    info.mac[5] = _hl_str2hex((uint8_t*)argv[7]);

    rt_kprintf("\nSet MAC:");
    for (int i = 0; i < 6; i++) {
        rt_kprintf("%02X ", info.mac[i]);
    }
    rt_kprintf("\n");
    hl_mod_telink_ioctl(HL_RF_SET_REMOTE_PAIR_INFO_CMD, (uint8_t*)&info, sizeof(info));
}
MSH_CMD_EXPORT(telink_set_remote_mac, telink set remote mac cmd);

/*
 * EOF
 */
