#include "hl_mod_telink.h"
#include <stdlib.h>

/// 线程句柄
static struct rt_thread telink_thread;
/// 线程栈数组
static char telink_thread_stack[TELINK_THREAD_STACK_SIZE];
// telink模块句柄
static hl_mod_telink_t s_telink;

// 串口接收临时缓冲区
static uint8_t s_uart_recv_buf[TELINK_UART_BUF_SIZE];
// util数据缓冲区
static uint8_t* s_telink_hup_buf;
static uint8_t* s_telink_fifo_buf;

static void _telink_hup_success_handle_cb(hup_protocol_type_t hup_frame)
{
    rt_err_t ret;
    uint16_t data_len = 0;

    data_len |= (hup_frame.data_len_h << 8);
    data_len |= hup_frame.data_len_l;
    rt_kprintf("telink_hup_success_handle_cb [ CMD = %d | LEN = %02x ]\n", hup_frame.cmd, data_len);

    // ret = rt_mq_send(*s_telink.app_msq, hup_frame.data_addr, data_len);
    // if (RT_EOK != ret) {
    //     rt_kprintf("[ERROR] Telink hup_success_cb send msg_queue failed\n");
    // }
}

/* 串口接收数据回调函数 */
static rt_err_t _telink_uart_receive_cb(rt_device_t dev, rt_size_t size)
{
    uint32_t ret = 0;

    ret = rt_device_read(s_telink.serial, 0, s_uart_recv_buf, TELINK_UART_BUF_SIZE);
    ret = hl_util_fifo_write(&s_telink.fifo, s_uart_recv_buf, ret);

    return RT_EOK;
}

static void hl_mod_telink_thread_entry(void* parameter)
{
    uint32_t data     = 0;
    uint8_t  buf[256] = { 0 };
    uint8_t  temp[5]  = { 0x11, 0x22, 0x33, 0x44, 0x55 };

    hl_mod_telink_ioctl(0x01, temp, 5);

    while (1) {
        data = hl_util_fifo_data_size(&s_telink.fifo);
        if (data > 0) {
            data = hl_util_fifo_read(&s_telink.fifo, buf, data);
            for (uint32_t i = 0; i < data; i++) {
                hl_util_hup_decode(&s_telink.hup, buf[i]);
            }
        }
        rt_thread_mdelay(10);
    }
}

static int _hl_mod_telink_serial_init(void)
{
    // 查找系统中的串口设备
    s_telink.serial = rt_device_find(TELINK_UART_DEV_NAME);
    if (RT_NULL == s_telink.serial) {
        rt_kprintf("[ERROR] Telink find %s failed!\n", TELINK_UART_DEV_NAME);
        return RT_ERROR;
    }

    // 以中断接收及轮询发送模式打开串口设备
    rt_device_open(s_telink.serial, RT_DEVICE_FLAG_INT_RX);
    // 设置接收回调函数
    rt_device_set_rx_indicate(s_telink.serial, _telink_uart_receive_cb);

    return 0;
}

uint8_t hl_mod_telink_init(rt_mq_t* input_msq)
{
    if (NULL == input_msq) {
        rt_kprintf("[ERROR] telink input_msq is null!\n");
        return -1;
    }

    rt_err_t result;

    // 申请缓冲区内存空间
    s_telink_hup_buf  = (uint8_t*)malloc(sizeof(uint8_t) * TELINK_HUP_BUF_SIZE);
    s_telink_fifo_buf = (uint8_t*)malloc(sizeof(uint8_t) * TELINK_FIFO_BUF_SIZE);
    if ((NULL == s_telink_hup_buf) || (NULL == s_telink_fifo_buf)) {
        rt_kprintf("[ERROR] telink malloc buf failed!\n");
        return -1;
    }

    // Telink获取并赋值APP层下发的消息队列指针
    s_telink.app_msq = input_msq;

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
        rt_kprintf("[ERROR] hl_mod_telink_thread init failed!\n");
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
    free(s_telink_hup_buf);
    free(s_telink_fifo_buf);

    return 0;
}

uint8_t hl_mod_telink_start(void)
{
    rt_err_t result;

    result = rt_thread_startup(&telink_thread);
    if (RT_EOK != result) {
        rt_kprintf("[ERROR] hl_mod_telink_thread startup failed!\n");
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
    uint8_t frame_buf[TELINK_UART_BUF_SIZE] = { 0 };

    uint16_t frame_len =
        hl_util_hup_encode(s_telink.hup.hup_handle.role, cmd, frame_buf, TELINK_UART_BUF_SIZE, data_addr, data_len);

    rt_device_write(s_telink.serial, 0, frame_buf, frame_len);

    return 0;
}

static void telink_test(void)
{
    rt_mq_t mq;

    hl_mod_telink_init(&mq);
    hl_mod_telink_start();
}
MSH_CMD_EXPORT(telink_test, test cmd);
