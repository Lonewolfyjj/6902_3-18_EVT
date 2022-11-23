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

#define DBG_SECTION_NAME "euc"
#define DBG_LEVEL DBG_INFO
#include "rtdbg.h"

/* typedef -------------------------------------------------------------------*/

typedef struct _hl_mod_euc
{
    bool                    init_flag;
    bool                    start_flag;
    rt_thread_t             euc_thread;
    int                     thread_exit_flag;
    hl_util_hup_t           hup;
    void*                   msg_handle;
    rt_device_t             uart_dev;
    struct serial_configure uart_config;
} hl_mod_euc_st;

/* define --------------------------------------------------------------------*/

#define HL_MOD_EUC_UART_NAME "uart1"

#define HL_MOD_EUC_UART_BUFSZ 256
#define HL_MOD_EUC_HUP_BUFSZ 256

/* variables -----------------------------------------------------------------*/

static hl_mod_euc_st _euc_mod = { .init_flag        = false,
                                  .start_flag       = false,
                                  .euc_thread       = NULL,
                                  .thread_exit_flag = 0,
                                  .hup              = { 0 },
                                  .msg_handle       = NULL,
                                  .uart_dev         = NULL,
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

static uint8_t hup_buf[HL_MOD_EUC_HUP_BUFSZ] = { 0 };

/* Private function(only *.c)  -----------------------------------------------*/

static void hup_success_handle_func(hup_protocol_type_t hup_frame)
{
    uint16_t len = ((uint16_t)(hup_frame.data_len_h) << 8) | hup_frame.data_len_l;

    switch (hup_frame.cmd) {
        default:
            break;
    }
}

static inline int _hl_mod_euc_hup_init(void)
{
    int ret;

    _euc_mod.hup.hup_handle.frame_data_len = sizeof(hup_buf);
    _euc_mod.hup.hup_handle.role           = EM_HUP_ROLE_MASTER;
    _euc_mod.hup.hup_handle.timer_state    = EM_HUP_TIMER_DISABLE;

    ret = hl_util_hup_init(&(_euc_mod.hup), hup_buf, NULL, hup_success_handle_func);
    if (ret == -1) {
        LOG_E("hup init err!");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    return HL_MOD_EUC_FUNC_RET_OK;
}

static inline void _hl_mod_euc_hup_deinit(void)
{
    hl_util_hup_deinit(&(_euc_mod.hup));
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
        hl_util_hup_decode(&(_euc_mod.hup), buf[i]);
    }
}

static void _euc_thread_entry(void* arg)
{
    while (_euc_mod.thread_exit_flag == 0) {
        uart_data_process();

        rt_thread_mdelay(10);
    }

    _euc_mod.thread_exit_flag = -1;
}

/* Exported functions --------------------------------------------------------*/
int hl_mod_euc_init(void* msg_hd)
{
    int ret;

    if (_euc_mod.init_flag == true) {
        LOG_W("euc already inited!\n");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    ret = _hl_mod_euc_hup_init();
    if (ret == HL_MOD_EUC_FUNC_RET_ERR) {
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    _euc_mod.msg_handle = msg_hd;

    LOG_D("euc init success\n");

    _euc_mod.init_flag = true;

    return HL_MOD_EUC_FUNC_RET_OK;
}

int hl_mod_euc_deinit(void)
{
    if (_euc_mod.init_flag == false) {
        LOG_W("euc not init!\n");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    hl_mod_euc_stop();

    _hl_mod_euc_hup_deinit();

    LOG_D("euc deinit success\n");

    _euc_mod.init_flag = false;

    return HL_MOD_EUC_FUNC_RET_OK;
}

int hl_mod_euc_start(void)
{
    int ret;

    if (_euc_mod.init_flag == false) {
        LOG_E("euc not init!\n");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    if (_euc_mod.start_flag == true) {
        hl_mod_euc_stop();
    }

    _euc_mod.thread_exit_flag = 0;

    _euc_mod.euc_thread = rt_thread_create("hl_mod_euc_thread", _euc_thread_entry, RT_NULL, 1024, 25, 10);
    if (_euc_mod.euc_thread == RT_NULL) {
        LOG_E("euc thread create failed\n");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    rt_thread_startup(_euc_mod.euc_thread);

    ret = uart_init();
    if (ret == HL_MOD_EUC_FUNC_RET_ERR) {
        rt_thread_delete(_euc_mod.euc_thread);
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    LOG_I("euc start success\n");

    _euc_mod.start_flag = true;

    return HL_MOD_EUC_FUNC_RET_OK;
}

int hl_mod_euc_stop(void)
{
    if (_euc_mod.init_flag == false) {
        LOG_E("euc not init!\n");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    if (_euc_mod.start_flag == false) {
        LOG_W("euc not start\n");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    uart_deinit();

    _euc_mod.thread_exit_flag = 1;

    LOG_I("wait euc thread exit\n");

    while (_euc_mod.thread_exit_flag != -1) {
        rt_thread_mdelay(10);
    }

    LOG_I("euc stop success\n");

    _euc_mod.start_flag = false;

    return HL_MOD_EUC_FUNC_RET_OK;
}

int hl_mod_euc_ctrl(int op, void* arg, int arg_size)
{
    if (_euc_mod.init_flag == false) {
        LOG_E("euc not init!\n");
        return HL_MOD_EUC_FUNC_RET_ERR;
    }

    return HL_MOD_EUC_FUNC_RET_OK;
}

/*
 * EOF
 */