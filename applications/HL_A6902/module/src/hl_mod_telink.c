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
#include <stdlib.h>

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
#define TELINK_THREAD_STACK_SIZE 1024
#define TELINK_THREAD_PRIORITY 7
#define TELINK_THREAD_TIMESLICE 10
#define TELINK_UART_DEV_NAME "uart2"
#define TELINK_UART_BUF_SIZE 256
#define TELINK_FIFO_BUF_SIZE 256
#define TELINK_HUP_BUF_SIZE 256

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

static uint8_t old_pair_workmode  = 0xFF;
static uint8_t old_pair_chn_state = 0xFF;
static uint8_t new_pair_workmode  = 0xFF;
static uint8_t new_pair_chn_state = 0xFF;

/* Private function(only *.c)  -----------------------------------------------*/

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

    app_msg_t.sender = TELINK_MODE;

    switch (hup_frame.cmd) {
        case HL_MOD_TELINK_VERSION_IND:
            app_msg_t.cmd       = HL_MOD_TELINK_VERSION_IND;
            app_msg_t.len       = data_len;
            app_msg_t.param.ptr = hup_frame.data_addr;

            // 上报Telink固件版本信息
            ret = rt_mq_send(s_telink.app_msq, (void*)&app_msg_t, sizeof(app_msg_t));
            break;

        case HL_MOD_TELINK_PAIR_INFO_IND:
            // 更新Telink模块工作状态和配对状态
            // rt_kprintf("telink cmd = %d\n", hup_frame.cmd);
            // rt_kprintf("telink len = %d\n[ ", data_len);
            // for(int i=0;i<data_len;i++){
            //     rt_kprintf("%02x ", hup_frame.data_addr[i]);
            // }
            // rt_kprintf("]\n");
            new_pair_workmode  = hup_frame.data_addr[0];
            new_pair_chn_state = hup_frame.data_addr[1] | hup_frame.data_addr[2];
            break;

        case HL_MOD_TELINK_RSSI_IND:
            app_msg_t.cmd       = HL_MOD_TELINK_RSSI_IND;
            app_msg_t.len       = data_len;
            app_msg_t.param.ptr = hup_frame.data_addr;
            // 上报无线模块RSSI值
            ret = rt_mq_send(s_telink.app_msq, (void*)&app_msg_t, sizeof(app_msg_t));
            break;

        default:
            break;
    }

    if (RT_EOK != ret) {
        rt_kprintf("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FILE__, __LINE__, ret);
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
    uint32_t data     = 0;
    uint8_t  buf[256] = { 0 };

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
        rt_thread_mdelay(10);

        // 更新配对状态
        if ((old_pair_workmode == new_pair_workmode) && (new_pair_chn_state == old_pair_chn_state)) {
            continue;
        } else if ((old_pair_workmode != PAIR_START) && (new_pair_workmode == PAIR_START)) {
            old_pair_workmode  = PAIR_START;
            app_msg_t.cmd = HL_MOD_TELINK_PAIR_START_IND;
            app_msg_t.len = 0;
        } else if ((old_pair_workmode != PAIR_STOP) && (new_pair_workmode == PAIR_STOP) && (new_pair_chn_state == PAIR_FAILED)) {
            old_pair_workmode  = PAIR_STOP;
            old_pair_chn_state = PAIR_FAILED;
            app_msg_t.cmd = HL_MOD_TELINK_PAIR_STOP_IND;
            app_msg_t.len = 0;
        } else if ((old_pair_workmode != PAIR_STOP) && (new_pair_chn_state == PAIR_STOP)
                   && (new_pair_chn_state != PAIR_FAILED)) {
            old_pair_workmode        = PAIR_STOP;
            old_pair_chn_state       = new_pair_chn_state;
            app_msg_t.cmd       = HL_MOD_TELINK_PAIR_INFO_IND;
            app_msg_t.len       = 1;
            app_msg_t.param.ptr = &new_pair_chn_state;
        } else if ((old_pair_workmode == PAIR_STOP) && (new_pair_chn_state != old_pair_chn_state)) {
            old_pair_chn_state       = new_pair_chn_state;
            app_msg_t.cmd       = HL_MOD_TELINK_PAIR_INFO_IND;
            app_msg_t.len       = 1;
            app_msg_t.param.ptr = &new_pair_chn_state;
        } else {
            old_pair_workmode  = new_pair_workmode;
            old_pair_chn_state = new_pair_chn_state;
        }
        // 将数据上传至APP层消息队列
        ret = rt_mq_send(s_telink.app_msq, (void*)&app_msg_t, sizeof(app_msg_t));
        if (RT_EOK != ret) {
            rt_kprintf("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FILE__, __LINE__, ret);
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
        rt_kprintf("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FILE__, __LINE__, s_telink.serial);
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
        rt_kprintf("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FILE__, __LINE__, input_msq);
        return -1;
    }

    rt_err_t result;

#if HL_GET_DEVICE_TYPE()
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
        rt_kprintf("[%s][line:%d] rt_malloc failed!!! \r\n", __FILE__, __LINE__);
        return -1;
    }

    // Telink获取并赋值APP层下发的消息队列指针
    s_telink.app_msq     = input_msq;
    s_telink.thread_flag = RT_TRUE;

    // 初始化Telink模块串口交互所需的资源
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
        rt_kprintf("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FILE__, __LINE__, result);
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
        rt_kprintf("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FILE__, __LINE__, result);
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
        rt_kprintf("[%s][line:%d] data_len is too long!!! \r\n", __FILE__, __LINE__);
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
    uint8_t data = (uint8_t)argv[2][0] - '0';
    uint8_t len  = (uint8_t)atoi(argv[3]);
    rt_kprintf("\nSEND CMD:%02x\n", cmd);
    rt_kprintf("SEND DATA:%02x\n\n", data);
    hl_mod_telink_ioctl(cmd, &data, len);
}
MSH_CMD_EXPORT(telink_send_cmd, telink io ctrl cmd);

/*
 * EOF
 */
