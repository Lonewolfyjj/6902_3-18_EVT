#include "hl_mod_telink.h"
#include "hl_hal_gpio.h"
#include "hl_drv_aw2016a.h"
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

static struct rt_thread led_thread;
static char             led_thread_stack[1024];
hl_gpio_pin_e           key_left_gpio;
hl_gpio_pin_e           key_right_gpio;
hl_gpio_pin_e           rx_key_pair_gpio;
uint8_t                 telink_pair_state;
uint8_t                 get_pair_count = 1;

static void hl_hal_gpio_telink_pairleft_irq_process(void* args)
{
    hl_gpio_pin_e gpio_pin_e = *(hl_gpio_pin_e*)args;

    uint8_t data = 0;
    // rt_kprintf("telink pair leftkey\n");
    hl_mod_telink_ioctl(TELINK_CMD_WIRELESS_PAIR_LEFT, &data, 1);
}
static void hl_hal_gpio_telink_pairright_irq_process(void* args)
{
    hl_gpio_pin_e gpio_pin_e = *(hl_gpio_pin_e*)args;

    uint8_t data = 0;
    // rt_kprintf("telink pair rightkey\n");
    hl_mod_telink_ioctl(TELINK_CMD_WIRELESS_PAIR_RIGHT, &data, 1);
}
static void hl_hal_gpio_telink_pairall_irq_process(void* args)
{
    hl_gpio_pin_e gpio_pin_e = *(hl_gpio_pin_e*)args;

    uint8_t data = 0;
    // rt_kprintf("telink pair allkey\n");
    hl_mod_telink_ioctl(TELINK_CMD_WIRELESS_PAIR_ALL, &data, 1);
}

static void _hl_drv_key_init(void)
{
#if HL_GET_DEVICE_TYPE()
    key_left_gpio = GPIO_PAIR_KEY;
    hl_hal_gpio_init(GPIO_PAIR_KEY);
    hl_hal_gpio_attach_irq(GPIO_PAIR_KEY, PIN_IRQ_MODE_FALLING, hl_hal_gpio_telink_pairleft_irq_process, &key_left_gpio);
    hl_hal_gpio_irq_enable(GPIO_PAIR_KEY, PIN_IRQ_ENABLE);

    key_right_gpio = GPIO_REC_KEY;
    hl_hal_gpio_init(GPIO_REC_KEY);
    hl_hal_gpio_attach_irq(GPIO_REC_KEY, PIN_IRQ_MODE_FALLING, hl_hal_gpio_telink_pairright_irq_process, &key_right_gpio);
    hl_hal_gpio_irq_enable(GPIO_REC_KEY, PIN_IRQ_ENABLE);
#else
    rx_key_pair_gpio = GPIO_PWR_KEY;
    hl_hal_gpio_init(GPIO_PWR_KEY);
    hl_hal_gpio_attach_irq(GPIO_PWR_KEY, PIN_IRQ_MODE_FALLING, hl_hal_gpio_telink_pairall_irq_process, &rx_key_pair_gpio);
    hl_hal_gpio_irq_enable(GPIO_PWR_KEY, PIN_IRQ_ENABLE);
#endif
}

static void _hl_drv_led_init(void)
{
    int                             ret;
    uint8_t                         chip_id;
    uint8_t                         work_mode;
    uint8_t                         led_chan;
    hl_drv_aw2016a_pwm_level_st     pwm_param;
    hl_drv_aw2016a_pattern_param_st pattern_param;

    // aw2016a init
    hl_drv_aw2016a_init();

    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_GET_CHIP_ID, &chip_id, sizeof(chip_id));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    work_mode = HL_DRV_AW2016A_ACTIVE_MODE;
    ret       = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_WORK_MODE, &work_mode, sizeof(work_mode));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    work_mode = HL_DRV_AW2016A_IMAX_5MA;
    ret       = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_GLOBAL_MAX_OUTPUT_CURRENT, &work_mode,
                              sizeof(work_mode));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    ret      = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_OPEN_LED_CHANNEL, &led_chan, sizeof(led_chan));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    ret      = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_PATTERN_MODE, &led_chan, sizeof(led_chan));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 0;
    pattern_param.t1       = 9;
    pattern_param.t2       = 3;
    pattern_param.t3       = 9;
    pattern_param.t4       = 0;
    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_PATTERN_MODE_PARAM, &pattern_param,
                              sizeof(pattern_param));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    pwm_param.led_chan  = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    pwm_param.pwm_level = 50;
    ret                 = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_LED_CHANNEL_PWM_LEVEL, &pwm_param,
                              sizeof(pwm_param));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }
}

static void _hl_drv_led_ctrl(uint8_t red, uint8_t green, uint8_t blue)
{
    int                              ret;
    hl_drv_aw2016a_output_current_st output_param;

    output_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL1;
    output_param.current  = red;
    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_LED_CHANNEL_OUTPUT_CURRENT, &output_param,
                              sizeof(output_param));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    output_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL2;
    output_param.current  = green;
    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_LED_CHANNEL_OUTPUT_CURRENT, &output_param,
                              sizeof(output_param));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    output_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL3;
    output_param.current  = blue;
    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_LED_CHANNEL_OUTPUT_CURRENT, &output_param,
                              sizeof(output_param));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }
}


static void _telink_hup_success_handle_cb(hup_protocol_type_t hup_frame)
{
    rt_err_t ret;
    uint16_t data_len = 0;

    data_len |= (hup_frame.data_len_h << 8);
    data_len |= hup_frame.data_len_l;
    // rt_kprintf("telink_hup_success_handle_cb [ CMD = %d | LEN = %02x ]\n", hup_frame.cmd, data_len);

    // ret = rt_mq_send(*s_telink.app_msq, hup_frame.data_addr, data_len);
    // if (RT_EOK != ret) {
    //     // rt_kprintf("[ERROR] Telink hup_success_cb send msg_queue failed\n");
    // }
}

/* 串口接收数据回调函数 */
static rt_err_t _telink_uart_receive_cb(rt_device_t dev, rt_size_t size)
{
    uint32_t ret = 0;

    ret = rt_device_read(s_telink.serial, 0, s_uart_recv_buf, TELINK_UART_BUF_SIZE);
    // ret = hl_util_fifo_write(&s_telink.fifo, s_uart_recv_buf, ret);

    // rt_kprintf("[ Telink Recv Len = %d ] ", ret);
    // rt_kprintf("[cmd] = %02x ", s_uart_recv_buf[0]);
    // rt_kprintf("[pair] = %02x ", s_uart_recv_buf[1]);
    // rt_kprintf("[left] = %02x ", s_uart_recv_buf[2]);
    // rt_kprintf("[right] = %02x ", s_uart_recv_buf[3]);
    // rt_kprintf("[ok] = %02x\n", s_uart_recv_buf[4]);
    switch (s_uart_recv_buf[0]) {
        case TELINK_CMD_GET_PAIR_INFO:
            if (s_uart_recv_buf[1] == 0x00 && s_uart_recv_buf[2] == 0x00 && s_uart_recv_buf[3] == 0x00) {
                telink_pair_state = TELINK_PAIR_FAILED;
            } else if (s_uart_recv_buf[2] > 0) {
                telink_pair_state = TELINK_PAIR_SUCCEDD_L;
            } else if (s_uart_recv_buf[3] > 0) {
                telink_pair_state = TELINK_PAIR_SUCCEDD_R;
            }
            break;

        case TELINK_CMD_WIRELESS_PAIR_LEFT:
        case TELINK_CMD_WIRELESS_PAIR_RIGHT:
            telink_pair_state = TELINK_PAIR_START;
            break;

        default:
            break;
    }

    return RT_EOK;
}

static void hl_mod_telink_thread_entry(void* parameter)
{
    uint32_t data      = 0;
    uint8_t  send_data = 0;
    uint8_t  buf[256]  = { 0 };

    // // 通过串口给Telink发送配对命令(0x03)
    // hl_mod_telink_ioctl(TELINK_CMD_WIRELESS_PAIR_LEFT, &data, 1);

    while (s_telink.thread_flag == RT_TRUE) {
        data = hl_util_fifo_data_size(&s_telink.fifo);
        if (data > 0) {
            data = hl_util_fifo_read(&s_telink.fifo, buf, data);
            for (uint32_t i = 0; i < data; i++) {
                hl_util_hup_decode(&s_telink.hup, buf[i]);
            }
        }
        rt_thread_mdelay(5);
    }
}

static void hl_mod_led_thread_entry(void* parameter)
{
    uint8_t data = 0;

    while (1) {
        if (telink_pair_state == TELINK_PAIR_START) {
            _hl_drv_led_ctrl(0, 0, 15);
            // // rt_kprintf("[OK] start pair\n");
        } else if (telink_pair_state == TELINK_PAIR_FAILED) {
            _hl_drv_led_ctrl(15, 0, 0);
            // // rt_kprintf("[OK] pair failed\n");
        } else if (telink_pair_state == TELINK_PAIR_SUCCEDD_L) {
            _hl_drv_led_ctrl(0, 15, 0);
            // // rt_kprintf("[OK] pair succedd left\n");
        } else if (telink_pair_state == TELINK_PAIR_SUCCEDD_R) {
            _hl_drv_led_ctrl(0, 15, 15);
            // // rt_kprintf("[OK] pair succedd right\n");
        }

        get_pair_count %= 100;
        if (get_pair_count == 0) {
            // 通过串口给Telink发送获取配对信息命令(0x02)
            hl_mod_telink_ioctl(TELINK_CMD_GET_PAIR_INFO, &data, 1);
            // rt_kprintf("[OK] send get pair info\n");
        }

        get_pair_count++;
        rt_thread_mdelay(10);
    }
}

static int _hl_mod_telink_serial_init(void)
{
    // 查找系统中的串口设备
    s_telink.serial = rt_device_find(TELINK_UART_DEV_NAME);
    if (RT_NULL == s_telink.serial) {
        // rt_kprintf("[ERROR] Telink find %s failed!\n", TELINK_UART_DEV_NAME);
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
        // rt_kprintf("[ERROR] telink input_msq is null!\n");
        return -1;
    }

    rt_err_t result;

    // 申请缓冲区内存空间
    s_telink_hup_buf  = (uint8_t*)rt_malloc(sizeof(uint8_t) * TELINK_HUP_BUF_SIZE);
    s_telink_fifo_buf = (uint8_t*)rt_malloc(sizeof(uint8_t) * TELINK_FIFO_BUF_SIZE);
    if ((NULL == s_telink_hup_buf) || (NULL == s_telink_fifo_buf)) {
        // rt_kprintf("[ERROR] telink malloc buf failed!\n");
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
        // rt_kprintf("[ERROR] hl_mod_telink_thread init failed!\n");
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
        // rt_kprintf("[ERROR] hl_mod_telink_thread startup failed!\n");
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
        // rt_kprintf("[ERROR] telink send data len is too long\n");
        return -1;
    }
    uint8_t frame_buf[TELINK_UART_BUF_SIZE] = { 0 };

    uint16_t frame_len =
        hl_util_hup_encode(s_telink.hup.hup_handle.role, cmd, frame_buf, TELINK_UART_BUF_SIZE, data_addr, data_len);

    frame_len = rt_device_write(s_telink.serial, 0, frame_buf, frame_len);
    // rt_kprintf("[ Telink Write Len = %d] [", frame_len);
    for (uint8_t i = 0; i < frame_len; i++) {
        // rt_kprintf(" %02x ", frame_buf[i]);
    }
    // rt_kprintf("]\n");

    return 0;
}

void hl_telink_get_state(void)
{
    uint8_t data = 0;

    // 通过串口给Telink发送获取配对信息命令(0x02)
    hl_mod_telink_ioctl(TELINK_CMD_GET_PAIR_INFO, &data, 1);
}
MSH_CMD_EXPORT(hl_telink_get_state, telink get pair state cmd);

void hl_telink_start_pairleft(void)
{
    uint8_t data = 0;

    // 通过串口给Telink发送配对命令(0x03)
    hl_mod_telink_ioctl(TELINK_CMD_WIRELESS_PAIR_LEFT, &data, 1);
}
MSH_CMD_EXPORT(hl_telink_start_pairleft, telink start pairleft cmd);

void telink_pair_test(void)
{
    rt_mq_t  mq;
    rt_err_t result;

#if HL_GET_DEVICE_TYPE()
    hl_hal_gpio_init(GPIO_EMMC_PWR_EN);
    hl_hal_gpio_init(GPIO_PWR_EN);
    hl_hal_gpio_init(GPIO_DC3V3_EN);
    hl_hal_gpio_init(GPIO_2831P_EN);
    hl_hal_gpio_init(GPIO_RF_PWR_EN);

    hl_hal_gpio_high(GPIO_EMMC_PWR_EN);
    hl_hal_gpio_high(GPIO_PWR_EN);
    hl_hal_gpio_high(GPIO_DC3V3_EN);
    hl_hal_gpio_high(GPIO_2831P_EN);
    hl_hal_gpio_high(GPIO_RF_PWR_EN);

    rt_thread_mdelay(10);

    // LED
    _hl_drv_led_init();

#else
    hl_hal_gpio_init(GPIO_RF_PWR_EN);
    hl_hal_gpio_high(GPIO_RF_PWR_EN);
#endif
    // KEY
    _hl_drv_key_init();

    // telink_thread init
    hl_mod_telink_init(&mq);
    // telink_thread start
    hl_mod_telink_start();


#if HL_GET_DEVICE_TYPE()
    // led_thread init
    result = rt_thread_init(&led_thread, "led", hl_mod_led_thread_entry, RT_NULL, &led_thread_stack[0],
                            sizeof(led_thread_stack), TELINK_THREAD_PRIORITY, TELINK_THREAD_TIMESLICE);
    if (RT_EOK != result) {
        // rt_kprintf("[ERROR] hl_mod_telink_thread init failed!\n");
        return -1;
    }
    // led_thread start
    result = rt_thread_startup(&led_thread);
    if (RT_EOK != result) {
        // rt_kprintf("[ERROR] hl_mod_led_thread startup failed!\n");
        return -1;
    }
#endif

    return 0;
}
INIT_APP_EXPORT(telink_pair_test);
