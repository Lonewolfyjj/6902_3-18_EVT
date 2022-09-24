/**
 * @file hl_mod_2831pl.c
 * @author lilin (lin.li@hollyland-tech.com)
 * @brief 2831p无线模块源文件
 * @version 1.0
 * @date 2022-09-13
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-13     <td>v1.0     <td>lilin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/

#include "hl_mod_2831pl.h"
#include "hl_util_fifo.h"
#include "hl_util_hup.h"

/* typedef -------------------------------------------------------------------*/

typedef struct _hl_mod_2831pl
{
    int                       init_flag;
    void*                     msg_handle;
    hl_mod_2831pl_work_mode_e work_mode;
    hl_util_hup_t             hup;
    hl_util_fifo_t            fifo;
    rt_thread_t               thread;
    int                       thread_shutdown;
    rt_device_t               uart_dev;
    struct serial_configure   uart_config;
} hl_mod_2831pl_st;

typedef enum _hl_mod_pl_hup_cmd
{
    CMD_GET_VERSION = 0x00,
    CMD_WIRELESS_PAIR,
    CMD_GET_LINK_STATE,
    CMD_SWITCH_SLAVE_MASTER,
    CMD_GET_LOCAL_PAIR_INFO,
    CMD_SET_REMOTE_PAIR_INFO,
    CMD_GET_REMOTE_PAIR_INFO,
    CMD_GET_RSSI,
    CMD_BY_PASS,
    CMD_ASK_L_R_CHANNEL,
    CMD_GET_WORK_MODE,
} hl_mod_pl_hup_cmd_e;

/* define --------------------------------------------------------------------*/

#define HL_MOD_PL_UART_NAME "uart2"

#define DBG_LOG rt_kprintf

#define HL_MOD_PL_UART_BUFSZ 256
#define HL_MOD_PL_FIFO_BUFSZ 256
#define HL_MOD_PL_HUP_BUFSZ 256

/* variables -----------------------------------------------------------------*/

static hl_mod_2831pl_st _pl_mod = { .init_flag       = 0,
                                    .msg_handle      = NULL,
                                    .work_mode       = PL_WORK_MODE_UNKNOWN,
                                    .hup             = { 0 },
                                    .fifo            = { 0 },
                                    .thread          = NULL,
                                    .thread_shutdown = 0,
                                    .uart_dev        = NULL,
                                    .uart_config     = {
                                        .baud_rate = BAUD_RATE_115200,
                                        .data_bits = DATA_BITS_8,
                                        .parity    = PARITY_NONE,
                                        .stop_bits = STOP_BITS_1,
                                        .invert    = NRZ_NORMAL,
                                        .flow_ctrl = RT_SERIAL_AUTO_FLOW_DISABLE,
                                        .bit_order = BIT_ORDER_LSB,
                                        .bufsz     = HL_MOD_PL_UART_BUFSZ,
                                    } };

static uint8_t fifo_buf[HL_MOD_PL_FIFO_BUFSZ] = { 0 };
static uint8_t hup_buf[HL_MOD_PL_HUP_BUFSZ]   = { 0 };

static hl_mod_2831pl_work_mode_e work_mode_temp = PL_WORK_MODE_UNKNOWN;

/* Private function(only *.c)  -----------------------------------------------*/

static void hup_success_handle_func(hup_protocol_type_t hup_frame)
{
    uint16_t len = ((uint16_t)(hup_frame.data_len_h) << 8) | hup_frame.data_len_l;

    switch (hup_frame.cmd) {
        case CMD_GET_VERSION: {

        } break;
        case CMD_WIRELESS_PAIR: {
            DBG_LOG("wireless pair ack: %d\n", hup_frame.data_addr[0]);
        } break;
        case CMD_GET_LINK_STATE: {
            if (_pl_mod.work_mode == PL_WORK_MODE_SLAVE) {
                DBG_LOG("link state: %d\n", hup_frame.data_addr[0]);
            } else if (_pl_mod.work_mode == PL_WORK_MODE_MASTER) {
                DBG_LOG("link state: L: %d R: %d\n", hup_frame.data_addr[0], hup_frame.data_addr[1]);
            } else {
                DBG_LOG("2831pl work mode unknown!");
            }
        } break;
        case CMD_SWITCH_SLAVE_MASTER: {
            if (hup_frame.data_addr[0] == 1) {
                _pl_mod.work_mode = work_mode_temp;
                DBG_LOG("2831pl work mode: %d\n", work_mode_temp);
            } else {
                _pl_mod.work_mode = PL_WORK_MODE_UNKNOWN;
            }
        } break;
        case CMD_GET_LOCAL_PAIR_INFO: {

        } break;
        case CMD_SET_REMOTE_PAIR_INFO: {

        } break;
        case CMD_GET_REMOTE_PAIR_INFO: {

        } break;
        case CMD_GET_RSSI: {

        } break;
        case CMD_BY_PASS: {

        } break;
        case CMD_ASK_L_R_CHANNEL: {

        } break;
        case CMD_GET_WORK_MODE: {
            if (hup_frame.data_addr[0] == 0) {
                _pl_mod.work_mode = PL_WORK_MODE_SLAVE;
            } else {
                _pl_mod.work_mode = PL_WORK_MODE_MASTER;
            }
            DBG_LOG("2831pl work mode: %d\n", _pl_mod.work_mode);
        } break;
        default:
            break;
    }
}

static rt_err_t uart_rx_callback(rt_device_t dev, rt_size_t size)
{
    uint8_t   buf[64];
    rt_size_t len;

    len = rt_device_read(dev, -1, buf, sizeof(buf));
    hl_util_fifo_write(&(_pl_mod.fifo), buf, len);

    return RT_EOK;
}

static int uart_init(void)
{
    rt_device_t uart_dev;
    rt_err_t    rt_err;

    uart_dev = rt_device_find(HL_MOD_PL_UART_NAME);
    if (uart_dev == NULL) {
        DBG_LOG("can not find dev:%s", HL_MOD_PL_UART_NAME);
        return HL_MOD_PL_FUNC_RET_ERR;
    }

    rt_err = rt_device_control(uart_dev, RT_DEVICE_CTRL_CONFIG, &(_pl_mod.uart_config));
    if (rt_err != RT_EOK) {
        DBG_LOG("can not control dev:%s", HL_MOD_PL_UART_NAME);
        return HL_MOD_PL_FUNC_RET_ERR;
    }

    rt_err = rt_device_open(uart_dev, RT_DEVICE_FLAG_INT_RX);
    if (rt_err != RT_EOK) {
        DBG_LOG("can not open dev:%s", HL_MOD_PL_UART_NAME);
        return HL_MOD_PL_FUNC_RET_ERR;
    }

    rt_err = rt_device_set_rx_indicate(uart_dev, uart_rx_callback);
    if (rt_err != RT_EOK) {
        DBG_LOG("can not set rx indicate dev:%s", HL_MOD_PL_UART_NAME);
        return HL_MOD_PL_FUNC_RET_ERR;
    }

    _pl_mod.uart_dev = uart_dev;

    return HL_MOD_PL_FUNC_RET_OK;
}

static void uart_deinit()
{
    rt_device_t uart_dev;
    rt_err_t    rt_err;

    uart_dev = _pl_mod.uart_dev;

    rt_err = rt_device_set_rx_indicate(uart_dev, NULL);
    if (rt_err != RT_EOK) {
        DBG_LOG("can not set rx indicate dev:%s", HL_MOD_PL_UART_NAME);
        return;
    }

    rt_err = rt_device_close(uart_dev);
    if (rt_err != RT_EOK) {
        DBG_LOG("can not close dev:%s", HL_MOD_PL_UART_NAME);
        return;
    }

    _pl_mod.uart_dev = NULL;
}

static int handle_wireless_pair(hl_mod_2831pl_wireless_pair_e* p_param)
{
    int     size;
    uint8_t buf_send[64] = { 0 };
    uint8_t data_buf[1];

    if (*p_param == PL_WIRELESS_PAIR_START) {
        data_buf[0] = 0x01;
        size = hl_util_hup_encode(_pl_mod.hup.hup_handle.role, CMD_WIRELESS_PAIR, buf_send, sizeof(buf_send), data_buf,
                                  sizeof(data_buf));
        rt_device_write(_pl_mod.uart_dev, -1, buf_send, size);
    } else if (*p_param == PL_WIRELESS_PAIR_STOP) {
        data_buf[0] = 0x00;
        size = hl_util_hup_encode(_pl_mod.hup.hup_handle.role, CMD_WIRELESS_PAIR, buf_send, sizeof(buf_send), data_buf,
                                  sizeof(data_buf));
        rt_device_write(_pl_mod.uart_dev, -1, buf_send, size);
    } else {
        return HL_MOD_PL_FUNC_RET_ERR;
    }

    return HL_MOD_PL_FUNC_RET_OK;
}

static int handle_switch_slave_master(hl_mod_2831pl_work_mode_e* p_param)
{
    int     size;
    uint8_t buf_send[64] = { 0 };
    uint8_t data_buf[1];

    if (*p_param == PL_WORK_MODE_SLAVE) {
        data_buf[0] = 0x00;
        size = hl_util_hup_encode(_pl_mod.hup.hup_handle.role, CMD_SWITCH_SLAVE_MASTER, buf_send, sizeof(buf_send),
                                  data_buf, sizeof(data_buf));
        rt_device_write(_pl_mod.uart_dev, -1, buf_send, size);
        work_mode_temp = PL_WORK_MODE_SLAVE;
    } else if (*p_param == PL_WORK_MODE_MASTER) {
        data_buf[0] = 0x01;
        size = hl_util_hup_encode(_pl_mod.hup.hup_handle.role, CMD_SWITCH_SLAVE_MASTER, buf_send, sizeof(buf_send),
                                  data_buf, sizeof(data_buf));
        rt_device_write(_pl_mod.uart_dev, -1, buf_send, size);
        work_mode_temp = PL_WORK_MODE_MASTER;
    } else {
        return HL_MOD_PL_FUNC_RET_ERR;
    }

    return HL_MOD_PL_FUNC_RET_OK;
}

static int handle_get_link_state()
{
    int     size;
    uint8_t buf_send[64] = { 0 };
    uint8_t data_buf[1];

    size = hl_util_hup_encode(_pl_mod.hup.hup_handle.role, CMD_GET_LINK_STATE, buf_send, sizeof(buf_send), data_buf, 0);
    rt_device_write(_pl_mod.uart_dev, -1, buf_send, size);

    return HL_MOD_PL_FUNC_RET_OK;
}

static int handle_get_work_mode()
{
    int     size;
    uint8_t buf_send[64] = { 0 };
    uint8_t data_buf[1];

    size = hl_util_hup_encode(_pl_mod.hup.hup_handle.role, CMD_GET_WORK_MODE, buf_send, sizeof(buf_send), data_buf, 0);
    rt_device_write(_pl_mod.uart_dev, -1, buf_send, size);

    return HL_MOD_PL_FUNC_RET_OK;
}

static void ats2831pl_check(void)
{
    static int timeout_count = 0;

    if (_pl_mod.work_mode == PL_WORK_MODE_UNKNOWN) {
        handle_get_work_mode();
        timeout_count++;
        if (timeout_count == 100) {
            DBG_LOG("find 2831pl timeout!\n");
            timeout_count = 0;
        }
    } else {
        timeout_count = 0;
    }
}

static void uart_data_process()
{
    uint8_t buf[64];
    int     size;

    size = hl_util_fifo_read(&(_pl_mod.fifo), buf, sizeof(buf));
    if (size <= 0) {
        rt_thread_mdelay(50);
        return;
    }

    for (int i = 0; i < size; i++) {
        hl_util_hup_decode(&(_pl_mod.hup), buf[i]);
    }
}

static void serial_thread_entry(void* parameter)
{
    while (_pl_mod.thread_shutdown == 0) {
        ats2831pl_check();  //检测2831pl，并且上报
        uart_data_process();
    }

    _pl_mod.thread_shutdown = -1;
}

/* Exported functions --------------------------------------------------------*/

int hl_mod_2831pl_init(void* msgHd)
{
    int         ret;
    rt_thread_t thread;

    if (_pl_mod.init_flag == 1) {
        DBG_LOG("pl mod already inited!");
        return HL_MOD_PL_FUNC_RET_ERR;
    }

    _pl_mod.hup.hup_handle.frame_data_len = sizeof(hup_buf);
    _pl_mod.hup.hup_handle.role           = EM_HUP_ROLE_MASTER;
    _pl_mod.hup.hup_handle.timer_state    = EM_HUP_TIMER_DISABLE;

    ret = hl_util_hup_init(&(_pl_mod.hup), hup_buf, NULL, hup_success_handle_func);
    if (ret == -1) {
        DBG_LOG("hup init err!");
        return HL_MOD_PL_FUNC_RET_ERR;
    }

    hl_util_fifo_init(&(_pl_mod.fifo), fifo_buf, sizeof(fifo_buf));

    _pl_mod.thread_shutdown = 0;

    thread = rt_thread_create("hl_mod_pl_thread", serial_thread_entry, RT_NULL, 1024, 25, 10);
    if (thread != RT_NULL) {
        rt_thread_startup(thread);
    } else {
        return HL_MOD_PL_FUNC_RET_ERR;
    }

    ret = uart_init();
    if (ret == HL_MOD_PL_FUNC_RET_ERR) {
        return HL_MOD_PL_FUNC_RET_ERR;
    }

    _pl_mod.thread     = thread;
    _pl_mod.msg_handle = msgHd;
    _pl_mod.init_flag  = 1;

    return HL_MOD_PL_FUNC_RET_OK;
}

void hl_mod_2831pl_deinit(void)
{
    if (_pl_mod.init_flag == 0) {
        DBG_LOG("pl mod already deinited!");
        return;
    }

    uart_deinit();

    _pl_mod.thread_shutdown = 1;
    while (_pl_mod.thread_shutdown == -1) {
        rt_thread_mdelay(10);
    }

    hl_util_fifo_deinit(&(_pl_mod.fifo));

    hl_util_hup_deinit(&(_pl_mod.hup));

    return;
}

int hl_mod_2831pl_ctrl(int op, void* arg, int arg_size)
{
    int ret;

    if (_pl_mod.init_flag == 0) {
        DBG_LOG("pl mod is not inited!");
        return HL_MOD_PL_FUNC_RET_ERR;
    }

    if (_pl_mod.work_mode == PL_WORK_MODE_UNKNOWN) {
        DBG_LOG("2831pl is not find yet!");
        return HL_MOD_PL_FUNC_RET_ERR;
    }

    switch (op) {
        case HL_MOD_PL_WIRELESS_PAIR: {
            if (arg_size != sizeof(hl_mod_2831pl_wireless_pair_e)) {
                DBG_LOG("size err, ctrl arg need <hl_mod_2831pl_wireless_pair_e> type pointer!");
            }

            ret = handle_wireless_pair((hl_mod_2831pl_wireless_pair_e*)arg);
            if (ret == HL_MOD_PL_FUNC_RET_ERR) {
                return HL_MOD_PL_FUNC_RET_ERR;
            }
        } break;
        case HL_MOD_PL_SWITCH_SLAVE_MASTER: {
            if (arg_size != sizeof(hl_mod_2831pl_work_mode_e)) {
                DBG_LOG("size err, ctrl arg need <hl_mod_2831pl_work_mode_e> type pointer!");
            }

            ret = handle_switch_slave_master((hl_mod_2831pl_work_mode_e*)arg);
            if (ret == HL_MOD_PL_FUNC_RET_ERR) {
                return HL_MOD_PL_FUNC_RET_ERR;
            }
        } break;
        case HL_MOD_PL_GET_LINK_STATE: {
            ret = handle_get_link_state();
            if (ret == HL_MOD_PL_FUNC_RET_ERR) {
                return HL_MOD_PL_FUNC_RET_ERR;
            }
        } break;
        default:
            break;
    }

    return HL_MOD_PL_FUNC_RET_OK;
}

/*
 * EOF
 */