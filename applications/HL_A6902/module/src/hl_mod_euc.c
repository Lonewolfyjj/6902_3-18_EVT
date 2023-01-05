/**
 * @file hl_mod_euc.c
 * @author lilin (lin.li@hollyland-tech.com)
 * @brief （extern uart communication）外部串口通信模块源文件
 * @version 1.0
 * @date 2022-10-08
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
 * <tr><td>2022-10-08     <td>v1.0     <td>lilin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/

#include "hl_mod_euc.h"
#include "hl_util_hup.h"
#include "hl_util_msg_type.h"

#define DBG_SECTION_NAME "euc"
#define DBG_LEVEL DBG_INFO
#include "rtdbg.h"

/* typedef -------------------------------------------------------------------*/

#if HL_IS_TX_DEVICE() == 1

typedef enum _hl_mod_extcom_hup_cmd_e
{
    HL_HUP_CMD_PROBE            = 0x01,
    HL_HUP_CMD_GET_BAT_INFO     = 0x02,
    HL_HUP_CMD_GET_PAIR_INFO    = 0x05,
    HL_HUP_CMD_SET_PAIR_INFO    = 0x06,
    HL_HUP_CMD_GET_MAC_ADDR     = 0x07,
    HL_HUP_CMD_GET_CHARGE_STATE = 0x0A,
} hl_mod_extcom_hup_cmd_e;

#else

typedef enum _hl_mod_extcom_hup_cmd_e
{
    HL_HUP_CMD_PROBE             = 0x01,
    HL_HUP_CMD_GET_BAT_INFO      = 0x02,
    HL_HUP_CMD_SET_BAT_INFO      = 0x03,
    HL_HUP_CMD_TX_IN_BOX_STATE   = 0x04,
    HL_HUP_CMD_SET_PAIR_INFO     = 0x06,
    HL_HUP_CMD_GET_MAC_ADDR      = 0x07,
    HL_HUP_CMD_PAIR_START        = 0x08,
    HL_HUP_CMD_PAIR_STOP         = 0x09,
    HL_HUP_CMD_GET_CHARGE_STATE  = 0x0A,
    HL_HUP_CMD_SET_CHARGE_STATE  = 0x0B,
    HL_HUP_CMD_SET_RTC_TIME      = 0x0C,
    HL_HUP_CMD_SET_RTC_TIME_BACK = 0x0D,
    HL_HUP_CMD_SET_BOX_LID_STATE = 0x0E,
} hl_mod_extcom_hup_cmd_e;

#endif

typedef enum _hl_euc_mod_dev_e
{
    HL_EUC_MOD_DEV_TX1 = 1,
    HL_EUC_MOD_DEV_TX2 = 2,
    HL_EUC_MOD_DEV_BOX = 3,
} hl_euc_mod_dev_e;

typedef struct _hl_mod_euc
{
    bool                       init_flag;
    bool                       start_flag;
    uint8_t                    tx1_bat_info;
    uint8_t                    tx2_bat_info;
    uint8_t                    box_bat_info;
    hl_mod_euc_charge_state_e  tx1_charge_state;
    hl_mod_euc_charge_state_e  tx2_charge_state;
    hl_mod_euc_charge_state_e  box_charge_state;
    hl_mod_euc_box_lid_state_e box_lid_state;
    bool                       tx1_in_box_flag;
    bool                       tx2_in_box_flag;
    bool                       tx1_pair_ok_flag;
    bool                       tx2_pair_ok_flag;
    struct rt_timer            timer;
    rt_thread_t                euc_thread;
    int                        thread_exit_flag;
    hl_util_hup_t              uart_hup;
    hl_util_hup_t              hid_hup;
    rt_mq_t                    msg_handle;
    rt_device_t                uart_dev;
    rt_device_t                hid_dev;
    struct serial_configure    uart_config;
} hl_mod_euc_st;

/* define --------------------------------------------------------------------*/

#define HL_MOD_EUC_UART_NAME "uart1"

#define HUP_PROT_SIZE 6

#define HL_MOD_EUC_UART_BUFSZ 256
#define HL_MOD_EUC_UART_HUP_BUFSZ 256
#define HL_MOD_EUC_HID_HUP_BUFSZ 256

#define HL_MOD_EUC_MAX_UART_HUP_SEND_BUFSZ 256
#define HL_MOD_EUC_MAX_HID_HUP_SEND_BUFSZ 256

/* variables -----------------------------------------------------------------*/

static hl_mod_euc_st _euc_mod = { .init_flag        = false,
                                  .start_flag       = false,
                                  .euc_thread       = NULL,
                                  .thread_exit_flag = 0,
                                  .uart_hup         = { 0 },
                                  .hid_hup          = { 0 },
                                  .msg_handle       = NULL,
                                  .uart_dev         = NULL,
                                  .hid_dev          = NULL,
                                  .uart_config      = {
                                      .baud_rate = BAUD_RATE_115200,
                                      .data_bits = DATA_BITS_8,
                                      .parity    = PARITY_NONE,
                                      .stop_bits = STOP_BITS_1,
                                      .invert    = NRZ_NORMAL,
                                      .flow_ctrl = RT_SERIAL_AUTO_FLOW_DISABLE,
                                      .bit_order = BIT_ORDER_LSB,
                                      .bufsz     = HL_MOD_EUC_UART_BUFSZ,       //在 open 串口之后不可改变
                                  }, };

static uint8_t uart_hup_buf[HL_MOD_EUC_UART_HUP_BUFSZ] = { 0 };
static uint8_t hid_hup_buf[HL_MOD_EUC_HID_HUP_BUFSZ]   = { 0 };

#if HL_IS_TX_DEVICE() == 1
//tx
static uint8_t rx_mac_addr[6] = { 0 };

#else
//rx
static uint8_t tx1_mac_addr[6] = { 0 };
static uint8_t tx2_mac_addr[6] = { 0 };

static hl_mod_euc_rtc_st _rtc_time_box = { 0 };
static hl_mod_euc_rtc_st _rtc_time_rx  = { 0 };

static bool _record_hid_cmd_send_flag = false;
#endif
/* Private function(only *.c)  -----------------------------------------------*/

static int _mod_msg_send(uint8_t cmd, void* param, uint16_t len)
{
    if (_euc_mod.msg_handle == NULL) {
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    mode_to_app_msg_t msg;
    rt_err_t          rt_err;

    msg.sender    = EXT_COM_MODE;
    msg.cmd       = cmd;
    msg.param.ptr = param;
    msg.len       = len;

    rt_err = rt_mq_send(_euc_mod.msg_handle, (void*)&msg, sizeof(msg));
    if (RT_EOK != rt_err) {
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    return HL_MOD_EUC_FUNC_RET_OK;
}

static int _uart_send_hup_data(char cmd, char* buf, int len)
{
    if (len + HUP_PROT_SIZE > HL_MOD_EUC_MAX_UART_HUP_SEND_BUFSZ) {
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    int      size;
    uint32_t write_size;
    uint8_t  buf_send[HL_MOD_EUC_MAX_UART_HUP_SEND_BUFSZ] = { 0 };

    size = hl_util_hup_encode(_euc_mod.uart_hup.hup_handle.role, cmd, buf_send, sizeof(buf_send), buf, len);
    if (size == -1) {
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    rt_device_write(_euc_mod.uart_dev, -1, buf_send, size);

    return HL_MOD_EUC_FUNC_RET_OK;
}

static int _hid_send_hup_data(char cmd, char* buf, int len)
{
    if (len + HUP_PROT_SIZE > HL_MOD_EUC_MAX_HID_HUP_SEND_BUFSZ) {
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    int      size;
    uint32_t write_size;
    uint8_t  buf_send[HL_MOD_EUC_MAX_HID_HUP_SEND_BUFSZ] = { 0 };

    size = hl_util_hup_encode(_euc_mod.hid_hup.hup_handle.role, cmd, buf_send, sizeof(buf_send), buf, len);
    if (size == -1) {
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    rt_device_write(_euc_mod.hid_dev, 0, buf_send, size);

    return HL_MOD_EUC_FUNC_RET_OK;
}

#if HL_IS_TX_DEVICE() == 1

static void uart_hup_success_handle_func(hup_protocol_type_t hup_frame)
{
    uint16_t len    = ((uint16_t)(hup_frame.data_len_h) << 8) | hup_frame.data_len_l;
    uint8_t  tx_num = 1;
    uint8_t  result = 0;

    switch (hup_frame.cmd) {
        case HL_HUP_CMD_PROBE: {
            _uart_send_hup_data(HL_HUP_CMD_PROBE, &tx_num, sizeof(tx_num));
            _mod_msg_send(HL_IN_BOX_IND, NULL, 0);
        } break;
        case HL_HUP_CMD_GET_BAT_INFO: {
            _mod_msg_send(HL_GET_SOC_REQ_IND, NULL, 0);
            rt_timer_start(&(_euc_mod.timer));
        } break;
        case HL_HUP_CMD_GET_PAIR_INFO: {
            _mod_msg_send(HL_GET_PAIR_MAC_REQ_IND, NULL, 0);
        } break;
        case HL_HUP_CMD_SET_PAIR_INFO: {
            rt_memcpy(rx_mac_addr, hup_frame.data_addr, sizeof(rx_mac_addr));
            _uart_send_hup_data(HL_HUP_CMD_SET_PAIR_INFO, &result, sizeof(result));
            _mod_msg_send(HL_SET_PAIR_MAC_REQ_IND, rx_mac_addr, sizeof(rx_mac_addr));
        } break;
        case HL_HUP_CMD_GET_MAC_ADDR: {
            _mod_msg_send(HL_GET_MAC_REQ_IND, NULL, 0);
        } break;
        case HL_HUP_CMD_GET_CHARGE_STATE: {
            _mod_msg_send(HL_GET_CHARGE_STATE_REQ_IND, NULL, 0);
        } break;
        default:
            break;
    }
}

static void hid_hup_success_handle_func(hup_protocol_type_t hup_frame)
{
    uint16_t len = ((uint16_t)(hup_frame.data_len_h) << 8) | hup_frame.data_len_l;

    switch (hup_frame.cmd) {
        default:
            break;
    }
}

#else

static void uart_hup_success_handle_func(hup_protocol_type_t hup_frame)
{
    uint16_t len    = ((uint16_t)(hup_frame.data_len_h) << 8) | hup_frame.data_len_l;
    uint8_t  rx_num = 0;
    uint8_t  result = 0;

    switch (hup_frame.cmd) {
        case HL_HUP_CMD_PROBE: {
            _uart_send_hup_data(HL_HUP_CMD_PROBE, &rx_num, sizeof(rx_num));
            _mod_msg_send(HL_IN_BOX_IND, NULL, 0);
        } break;
        case HL_HUP_CMD_GET_BAT_INFO: {
            _mod_msg_send(HL_GET_SOC_REQ_IND, NULL, 0);
            rt_timer_start(&(_euc_mod.timer));
        } break;
        case HL_HUP_CMD_SET_BAT_INFO: {
            if (hup_frame.data_addr[0] == HL_EUC_MOD_DEV_TX1) {
                _euc_mod.tx1_bat_info = hup_frame.data_addr[1];
                _mod_msg_send(HL_TX1_BAT_INFO_UPDATE_IND, &(_euc_mod.tx1_bat_info), sizeof(_euc_mod.tx1_bat_info));
            } else if (hup_frame.data_addr[0] == HL_EUC_MOD_DEV_TX2) {
                _euc_mod.tx2_bat_info = hup_frame.data_addr[1];
                _mod_msg_send(HL_TX2_BAT_INFO_UPDATE_IND, &(_euc_mod.tx2_bat_info), sizeof(_euc_mod.tx2_bat_info));
            } else if (hup_frame.data_addr[0] == HL_EUC_MOD_DEV_BOX) {
                _euc_mod.box_bat_info = hup_frame.data_addr[1];
                _mod_msg_send(HL_BOX_BAT_INFO_UPDATE_IND, &(_euc_mod.box_bat_info), sizeof(_euc_mod.box_bat_info));
            } else {
                break;
            }
        } break;
        case HL_HUP_CMD_TX_IN_BOX_STATE: {
            if (hup_frame.data_addr[0] == HL_EUC_MOD_DEV_TX1) {
                _euc_mod.tx1_in_box_flag = hup_frame.data_addr[1];
                _mod_msg_send(HL_TX1_IN_BOX_STATE_IND, &(_euc_mod.tx1_in_box_flag), sizeof(_euc_mod.tx1_in_box_flag));
            } else if (hup_frame.data_addr[0] == HL_EUC_MOD_DEV_TX2) {
                _euc_mod.tx2_in_box_flag = hup_frame.data_addr[1];
                _mod_msg_send(HL_TX2_IN_BOX_STATE_IND, &(_euc_mod.tx2_in_box_flag), sizeof(_euc_mod.tx2_in_box_flag));
            } else {
                break;
            }

            _uart_send_hup_data(HL_HUP_CMD_TX_IN_BOX_STATE, hup_frame.data_addr, len);
        } break;
        case HL_HUP_CMD_SET_PAIR_INFO: {
            if (hup_frame.data_addr[0] == HL_EUC_MOD_DEV_TX1) {
                rt_memcpy(tx1_mac_addr, hup_frame.data_addr + 1, sizeof(tx1_mac_addr));
                _mod_msg_send(HL_SET_PAIR_MAC_TX1_REQ_IND, tx1_mac_addr, sizeof(tx1_mac_addr));
            } else if (hup_frame.data_addr[0] == HL_EUC_MOD_DEV_TX2) {
                rt_memcpy(tx2_mac_addr, hup_frame.data_addr + 1, sizeof(tx2_mac_addr));
                _mod_msg_send(HL_SET_PAIR_MAC_TX2_REQ_IND, tx2_mac_addr, sizeof(tx2_mac_addr));
            } else {
                break;
            }

            _uart_send_hup_data(HL_HUP_CMD_SET_PAIR_INFO, hup_frame.data_addr, 1);
        } break;
        case HL_HUP_CMD_GET_MAC_ADDR: {
            _mod_msg_send(HL_GET_MAC_REQ_IND, NULL, 0);
        } break;
        case HL_HUP_CMD_PAIR_START: {
            if (hup_frame.data_addr[0] == HL_EUC_MOD_DEV_TX1) {
                _mod_msg_send(HL_TX1_PAIR_START_IND, NULL, 0);
            } else if (hup_frame.data_addr[0] == HL_EUC_MOD_DEV_TX2) {
                _mod_msg_send(HL_TX2_PAIR_START_IND, NULL, 0);
            } else {
                break;
            }
        } break;
        case HL_HUP_CMD_PAIR_STOP: {
            if (hup_frame.data_addr[0] == HL_EUC_MOD_DEV_TX1) {
                _euc_mod.tx1_pair_ok_flag = hup_frame.data_addr[1];
                _mod_msg_send(HL_TX1_PAIR_STOP_IND, &(_euc_mod.tx1_pair_ok_flag), sizeof(_euc_mod.tx1_pair_ok_flag));
            } else if (hup_frame.data_addr[0] == HL_EUC_MOD_DEV_TX2) {
                _euc_mod.tx2_pair_ok_flag = hup_frame.data_addr[1];
                _mod_msg_send(HL_TX2_PAIR_STOP_IND, &(_euc_mod.tx2_pair_ok_flag), sizeof(_euc_mod.tx2_pair_ok_flag));
            } else {
                break;
            }

            _uart_send_hup_data(HL_HUP_CMD_PAIR_STOP, hup_frame.data_addr, 1);
        } break;
        case HL_HUP_CMD_GET_CHARGE_STATE: {
            _mod_msg_send(HL_GET_CHARGE_STATE_REQ_IND, NULL, 0);
        } break;
        case HL_HUP_CMD_SET_CHARGE_STATE: {
            if (hup_frame.data_addr[0] == HL_EUC_MOD_DEV_TX1) {
                _euc_mod.tx1_charge_state = hup_frame.data_addr[1];
                _mod_msg_send(HL_TX1_CHARGE_STATE_IND, &(_euc_mod.tx1_charge_state), sizeof(_euc_mod.tx1_charge_state));
            } else if (hup_frame.data_addr[0] == HL_EUC_MOD_DEV_TX2) {
                _euc_mod.tx2_charge_state = hup_frame.data_addr[1];
                _mod_msg_send(HL_TX2_CHARGE_STATE_IND, &(_euc_mod.tx2_charge_state), sizeof(_euc_mod.tx2_charge_state));
            } else if (hup_frame.data_addr[0] == HL_EUC_MOD_DEV_BOX) {
                _euc_mod.box_charge_state = hup_frame.data_addr[1];
                _mod_msg_send(HL_BOX_CHARGE_STATE_IND, &(_euc_mod.box_charge_state), sizeof(_euc_mod.box_charge_state));
            } else {
                break;
            }
        } break;
        case HL_HUP_CMD_SET_RTC_TIME: {
            _rtc_time_box.second  = hup_frame.data_addr[0];
            _rtc_time_box.minute  = hup_frame.data_addr[1];
            _rtc_time_box.hour    = hup_frame.data_addr[2];
            _rtc_time_box.day     = hup_frame.data_addr[3];
            _rtc_time_box.weekday = hup_frame.data_addr[4];
            _rtc_time_box.month   = hup_frame.data_addr[5];
            _rtc_time_box.year    = hup_frame.data_addr[6];

            result = 0;

            _uart_send_hup_data(HL_HUP_CMD_SET_RTC_TIME, &result, sizeof(result));
        } break;
        case HL_HUP_CMD_SET_BOX_LID_STATE: {
            if (hup_frame.data_addr[0] == 0) {
                _euc_mod.box_lid_state = HL_MOD_EUC_BOX_LID_CLOSE;
            } else {
                _euc_mod.box_lid_state = HL_MOD_EUC_BOX_LID_OPEN;
            }

            _mod_msg_send(HL_BOX_LID_STATE_UPDATE_IND, &(_euc_mod.box_lid_state), sizeof(_euc_mod.box_lid_state));

            result = 0;

            _uart_send_hup_data(HL_HUP_CMD_SET_BOX_LID_STATE, &result, sizeof(result));
        } break;
        default:
            break;
    }
}

static void hid_hup_success_handle_func(hup_protocol_type_t hup_frame)
{
    uint16_t len = ((uint16_t)(hup_frame.data_len_h) << 8) | hup_frame.data_len_l;

    switch (hup_frame.cmd) {
        default:
            break;
    }
}

#endif

static inline int _hl_mod_euc_hup_init(void)
{
    int ret;

    _euc_mod.uart_hup.hup_handle.frame_data_len = sizeof(uart_hup_buf);
    _euc_mod.uart_hup.hup_handle.role           = EM_HUP_ROLE_SLAVE;
    _euc_mod.uart_hup.hup_handle.timer_state    = EM_HUP_TIMER_DISABLE;

    ret = hl_util_hup_init(&(_euc_mod.uart_hup), uart_hup_buf, NULL, uart_hup_success_handle_func);
    if (ret == -1) {
        LOG_E("uart_hup init err!");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    _euc_mod.hid_hup.hup_handle.frame_data_len = sizeof(hid_hup_buf);
    _euc_mod.hid_hup.hup_handle.role           = EM_HUP_ROLE_SLAVE;
    _euc_mod.hid_hup.hup_handle.timer_state    = EM_HUP_TIMER_DISABLE;

    ret = hl_util_hup_init(&(_euc_mod.hid_hup), hid_hup_buf, NULL, hid_hup_success_handle_func);
    if (ret == -1) {
        LOG_E("hid_hup init err!");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    return HL_MOD_EUC_FUNC_RET_OK;
}

static int _hid_init(void)
{
    rt_err_t rt_err;

    _euc_mod.hid_dev = rt_device_find("hidd");
    if (_euc_mod.hid_dev == RT_NULL) {
        LOG_E("hid dev find err, can not find hid dev!");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    rt_err = rt_device_open(_euc_mod.hid_dev, RT_DEVICE_FLAG_RDWR);
    if (rt_err != RT_EOK) {
        LOG_E("can not open hid dev!");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    return HL_MOD_EUC_FUNC_RET_OK;
}

static inline void _hl_mod_euc_hup_deinit(void)
{
    hl_util_hup_deinit(&(_euc_mod.uart_hup));
    hl_util_hup_deinit(&(_euc_mod.hid_hup));
}

static int uart_init(void)
{
    rt_device_t uart_dev;
    rt_err_t    rt_err;

    uart_dev = rt_device_find(HL_MOD_EUC_UART_NAME);
    if (uart_dev == NULL) {
        LOG_E("can not find dev:%s", HL_MOD_EUC_UART_NAME);
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    rt_err = rt_device_control(uart_dev, RT_DEVICE_CTRL_CONFIG, &(_euc_mod.uart_config));
    if (rt_err != RT_EOK) {
        LOG_E("can not control dev:%s", HL_MOD_EUC_UART_NAME);
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    rt_err = rt_device_open(uart_dev, RT_DEVICE_FLAG_INT_RX);
    if (rt_err != RT_EOK) {
        LOG_E("can not open dev:%s", HL_MOD_EUC_UART_NAME);
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    _euc_mod.uart_dev = uart_dev;

    return HL_MOD_EUC_FUNC_RET_OK;
}

static void uart_deinit()
{
    rt_device_t uart_dev;
    rt_err_t    rt_err;

    uart_dev = _euc_mod.uart_dev;

    rt_err = rt_device_close(uart_dev);
    if (rt_err != RT_EOK) {
        LOG_E("can not close dev:%s", HL_MOD_EUC_UART_NAME);
        return;
    }

    _euc_mod.uart_dev = NULL;
}

static inline void uart_data_process()
{
    uint8_t buf[256];
    int     size;

    size = rt_device_read(_euc_mod.uart_dev, -1, buf, sizeof(buf));
    if (size <= 0) {
        return;
    }

    for (int i = 0; i < size; i++) {
        hl_util_hup_decode(&(_euc_mod.uart_hup), buf[i]);
    }
}

static void _hid_data_process()
{
    uint8_t buf[64];
    int     size;

    size = rt_device_read(_euc_mod.hid_dev, 0, buf, sizeof(buf));
    if (size <= 0) {
        return;
    }

    for (int i = 0; i < size; i++) {
        hl_util_hup_decode(&(_euc_mod.hid_hup), buf[i]);
    }
}

#if HL_IS_TX_DEVICE() == 1

#else
static void _record_hid_cmd_send_poll(void)
{
    char buf_send[2];

    if (_record_hid_cmd_send_flag == true) {
        buf_send[0] = 0x04;
        buf_send[1] = 0x20;
        rt_device_write(_euc_mod.hid_dev, 0, buf_send, sizeof(buf_send));

        rt_thread_mdelay(10);

        buf_send[0] = 0x04;
        buf_send[1] = 0x00;
        rt_device_write(_euc_mod.hid_dev, 0, buf_send, sizeof(buf_send));

        _record_hid_cmd_send_flag = false;
    }
}
#endif

static void _timer_timeout_handle(void* arg)
{
    LOG_I("euc timeout");
    _mod_msg_send(HL_OUT_BOX_IND, NULL, 0);
}

static void _euc_thread_entry(void* arg)
{
    while (_euc_mod.thread_exit_flag == 0) {
        uart_data_process();
        _hid_data_process();

#if HL_IS_TX_DEVICE() == 1

#else
        _record_hid_cmd_send_poll();
#endif

        rt_thread_mdelay(10);
    }

    _euc_mod.thread_exit_flag = -1;
}

/* Exported functions --------------------------------------------------------*/
int hl_mod_euc_init(rt_mq_t msg_hd)
{
    int ret;

    if (_euc_mod.init_flag == true) {
        LOG_W("euc already inited!");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    ret = _hl_mod_euc_hup_init();
    if (ret == HL_MOD_EUC_FUNC_RET_ERR) {
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    ret = _hid_init();
    if (ret == HL_MOD_EUC_FUNC_RET_ERR) {
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    rt_timer_init(&(_euc_mod.timer), "euc_timer", _timer_timeout_handle, RT_NULL, 1500,
                  RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);

    _euc_mod.msg_handle = msg_hd;

    LOG_D("euc init success");

    _euc_mod.init_flag = true;

    return HL_MOD_EUC_FUNC_RET_OK;
}

int hl_mod_euc_deinit(void)
{
    if (_euc_mod.init_flag == false) {
        LOG_W("euc not init!");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    hl_mod_euc_stop();

    _hl_mod_euc_hup_deinit();

    rt_timer_detach(&(_euc_mod.timer));

    LOG_D("euc deinit success");

    _euc_mod.init_flag = false;

    return HL_MOD_EUC_FUNC_RET_OK;
}

int hl_mod_euc_start(void)
{
    int ret;

    if (_euc_mod.init_flag == false) {
        LOG_E("euc not init!");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    if (_euc_mod.start_flag == true) {
        hl_mod_euc_stop();
    }

    _euc_mod.thread_exit_flag = 0;

    _euc_mod.euc_thread = rt_thread_create("hl_mod_euc_thread", _euc_thread_entry, RT_NULL, 1024, 25, 10);
    if (_euc_mod.euc_thread == RT_NULL) {
        LOG_E("euc thread create failed");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    rt_thread_startup(_euc_mod.euc_thread);

    ret = uart_init();
    if (ret == HL_MOD_EUC_FUNC_RET_ERR) {
        rt_thread_delete(_euc_mod.euc_thread);
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    LOG_I("euc start success");

    _euc_mod.start_flag = true;

    return HL_MOD_EUC_FUNC_RET_OK;
}

int hl_mod_euc_stop(void)
{
    if (_euc_mod.init_flag == false) {
        LOG_E("euc not init!");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    if (_euc_mod.start_flag == false) {
        LOG_W("euc not start");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    uart_deinit();

    _euc_mod.thread_exit_flag = 1;

    LOG_I("wait euc thread exit");

    while (_euc_mod.thread_exit_flag != -1) {
        rt_thread_mdelay(10);
    }

    LOG_I("euc stop success");

    _euc_mod.start_flag = false;

    return HL_MOD_EUC_FUNC_RET_OK;
}

#if HL_IS_TX_DEVICE() == 1

int hl_mod_euc_ctrl(hl_mod_euc_cmd_e cmd, void* arg, int arg_size)
{
    uint8_t charge_state;

    if (_euc_mod.init_flag == false) {
        LOG_E("euc not init!");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    if (_euc_mod.start_flag == false) {
        LOG_E("euc not start!");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    switch (cmd) {
        case HL_SET_SOC_CMD: {
            if (arg_size != sizeof(uint8_t)) {
                LOG_E("size err, ctrl arg need <uint8_t> type pointer!");
                return HL_MOD_EUC_FUNC_RET_ERR;
            }

            _uart_send_hup_data(HL_HUP_CMD_GET_BAT_INFO, (char*)arg, sizeof(uint8_t));
        } break;
        case HL_SET_PAIR_MAC_CMD: {
            if (arg_size != sizeof(uint8_t[6])) {
                LOG_E("size err, ctrl arg need <uint8_t[6]> type pointer!");
                return HL_MOD_EUC_FUNC_RET_ERR;
            }

            _uart_send_hup_data(HL_HUP_CMD_GET_PAIR_INFO, (char*)arg, sizeof(uint8_t[6]));
        } break;
        case HL_SET_MAC_CMD: {
            if (arg_size != sizeof(uint8_t[6])) {
                LOG_E("size err, ctrl arg need <uint8_t[6]> type pointer!");
                return HL_MOD_EUC_FUNC_RET_ERR;
            }

            _uart_send_hup_data(HL_HUP_CMD_GET_MAC_ADDR, (char*)arg, sizeof(uint8_t[6]));
        } break;
        case HL_SET_CHARGE_STATE_CMD: {
            if (arg_size != sizeof(hl_mod_euc_charge_state_e)) {
                LOG_E("size err, ctrl arg need <hl_mod_euc_charge_state_e> type pointer!");
                return HL_MOD_EUC_FUNC_RET_ERR;
            }

            charge_state = *(hl_mod_euc_charge_state_e*)arg;

            _uart_send_hup_data(HL_HUP_CMD_GET_CHARGE_STATE, &charge_state, sizeof(charge_state));
        } break;
        default:
            break;
    }

    return HL_MOD_EUC_FUNC_RET_OK;
}

#else

int hl_mod_euc_ctrl(hl_mod_euc_cmd_e cmd, void* arg, int arg_size)
{
    uint8_t dev_num;
    uint8_t charge_state;

    if (_euc_mod.init_flag == false) {
        LOG_E("euc not init!");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    if (_euc_mod.start_flag == false) {
        LOG_E("euc not start!");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    switch (cmd) {
        case HL_SET_SOC_CMD: {
            if (arg_size != sizeof(uint8_t)) {
                LOG_E("size err, ctrl arg need <uint8_t> type pointer!");
                return HL_MOD_EUC_FUNC_RET_ERR;
            }

            _uart_send_hup_data(HL_HUP_CMD_GET_BAT_INFO, (uint8_t*)arg, sizeof(uint8_t));
        } break;
        case HL_SET_MAC_CMD: {
            if (arg_size != sizeof(uint8_t[6])) {
                LOG_E("size err, ctrl arg need <uint8_t[6]> type pointer!");
                return HL_MOD_EUC_FUNC_RET_ERR;
            }

            _uart_send_hup_data(HL_HUP_CMD_GET_MAC_ADDR, (char*)arg, sizeof(uint8_t[6]));
        } break;
        case HL_START_TX1_PAIR_CMD: {
            dev_num = HL_EUC_MOD_DEV_TX1;
            _uart_send_hup_data(HL_HUP_CMD_PAIR_START, &dev_num, sizeof(dev_num));
        } break;
        case HL_START_TX2_PAIR_CMD: {
            dev_num = HL_EUC_MOD_DEV_TX2;
            _uart_send_hup_data(HL_HUP_CMD_PAIR_START, &dev_num, sizeof(dev_num));
        } break;
        case HL_SET_CHARGE_STATE_CMD: {
            if (arg_size != sizeof(hl_mod_euc_charge_state_e)) {
                LOG_E("size err, ctrl arg need <hl_mod_euc_charge_state_e> type pointer!");
                return HL_MOD_EUC_FUNC_RET_ERR;
            }

            charge_state = *(hl_mod_euc_charge_state_e*)arg;

            _uart_send_hup_data(HL_HUP_CMD_GET_CHARGE_STATE, &charge_state, sizeof(charge_state));
        } break;
        case HL_SET_RTC_TIME_CMD: {
            if (arg_size != sizeof(hl_mod_euc_rtc_st)) {
                LOG_E("size err, ctrl arg need <hl_mod_euc_rtc_st> type pointer!");
                return HL_MOD_EUC_FUNC_RET_ERR;
            }

            rt_memcpy(&_rtc_time_rx, arg, arg_size);
        } break;
        case HL_HID_START_RECORD_CMD: {
            _record_hid_cmd_send_flag = true;
        } break;
        default:
            break;
    }

    return HL_MOD_EUC_FUNC_RET_OK;
}

#endif
/*
 * EOF
 */