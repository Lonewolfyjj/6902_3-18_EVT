/**
 * @file hl_mod_upgrade.c
 * @author lixiang (rd37@hollyland-tech.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-22
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-11-22     <td>v1.0     <td>lixiang     <td>内容
 * </table>
 * 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_MOD_UPGRADE_C__
#define __HL_MOD_UPGRADE_C__

/* Includes ------------------------------------------------------------------*/
#include <rtdevice.h>
#include <rtthread.h>
#include <stdint.h>
#include <ipc/ringbuffer.h>
#include "drv_dsp.h"
#include "string.h"
#include "stdlib.h"

#include <drivers/mtd_nor.h>
#include <dfs_posix.h>
#include "hal_base.h"
#include "../fwmgr/fw_analysis.h"
#include "../fwmgr/rkpart.h"
#include "ota.h"
#include "ota_file.h"
#include "ota_opt.h"

#include "hl_mod_upgrade.h"
#include "hl_hal_gpio.h"
#include "hl_util_hup.h"
#include "hl_util_fifo.h"
#include "hl_util_msg_type.h"
/* typedef -------------------------------------------------------------------*/

#define HL_UPGRADE_DATA_SIZE (512)

typedef struct _upgrade_pack
{
    uint16_t upgrade_num;
    uint8_t  upgrade_data[HL_UPGRADE_DATA_SIZE];
} upgrade_pack;

typedef struct _upgrade_task
{
    uint8_t type_num;
    uint8_t type_task[HL_UPGRADE_MAX];
} upgrade_task;

typedef struct _hl_mod_upgrade_telink_t
{
    /// 升级数据包
    upgrade_pack upgrade_pack;
    /// 升级文件
    int upgrade_file;
    /// telink升级文件的开始地址
    uint32_t file_start_addr;
} hl_mod_upgrade_telink_t;

typedef struct _hl_mod_upgrade_t
{
    rt_bool_t thread_flag;
    /// APP层消息队列
    rt_mq_t* app_msq;
    /// 升级模块消息队列
    mode_to_app_msg_t upgrade_msg;
    /// 串口设备
    rt_device_t serial;
    /// hup结构体
    hl_util_hup_t hup;
    /// fifo结构体
    hl_util_fifo_t fifo;
    /// upgrade类型
    upgrade_task task;
    /// telink升级线程
    rt_thread_t telink_task_thread;
    /// RK2108升级线程
    rt_thread_t ota_task_thread;
    /// telink升级状态
    hl_mod_upgrade_state telink_state;
    /// RK2108升级状态
    hl_mod_upgrade_state ota_state;
} hl_mod_upgrade_t;

/* define --------------------------------------------------------------------*/
#define HL_UPGRADE_BUF_SIZE (1048)

#define HL_UPGRADE_TELINK_UART_DEV_NAME "uart2"
#define HL_UPGRADE_TELINK_UART_BUF_SIZE (HL_UPGRADE_BUF_SIZE * 3)
#define HL_UPGRADE_TELINK_FIFO_BUF_SIZE (HL_UPGRADE_BUF_SIZE * 3)
#define HL_UPGRADE_TELINK_HUP_BUF_SIZE 256

#define HL_UPGRADE_TELINK_RESET_CMD 0xA5
#define HL_UPGRADE_TELINK_START_CMD 0xA1
#define HL_UPGRADE_TELINK_SIZE_CMD 0xA2
#define HL_UPGRADE_TELINK_PACK_CMD 0xA3
#define HL_UPGRADE_TELINK_STOP_CMD 0xA4

#if HL_IS_TX_DEVICE()
#define HL_UPGRADE_FILE_NAME_TELINK "/mnt/sdcard/telink.bin"
#define HL_UPGRADE_FILE_NAME_RK "/mnt/sdcard/mcu.img"
#else
#define HL_UPGRADE_FILE_NAME_TELINK "telink.bin"
#define HL_UPGRADE_FILE_NAME_RK "mcu.img"
#endif
/* variables -----------------------------------------------------------------*/
// 串口数据包
static hl_mod_upgrade_telink_t s_upgrade_telink = { 0 };

// upgrade模块句柄
static hl_mod_upgrade_t s_upgrade = { 0 };

// 串口接收缓冲区
static uint8_t s_uart_recv_buf[HL_UPGRADE_BUF_SIZE];

/// hup接收缓冲区
static uint8_t* s_upgrade_hup_buf = NULL;
/// fifo接收缓冲区
static uint8_t* s_upgrade_fifo_buf = NULL;

/* Private function(only *.c)  -----------------------------------------------*/
/// 串口初始化
static rt_err_t hl_mod_upgrade_uart_init(void);
/// 串口去初始化
static rt_err_t hl_mod_upgrade_uart_deinit(void);
/// 串口发送
static rt_err_t hl_mod_upgrade_uart_send(uint8_t upgrade_cmd, uint8_t* upgrade_data, uint32_t upgrade_size);
/// 串口接收数据回调函数
static rt_err_t _update_uart_receive_cb(rt_device_t dev, rt_size_t size);

static rt_err_t hl_mod_upgrade_uart_init(void)
{
    // 查找系统中的串口设备
    s_upgrade.serial = rt_device_find(HL_UPGRADE_TELINK_UART_DEV_NAME);
    if (RT_NULL == s_upgrade.serial) {
        rt_kprintf("[ERROR] Upgrade find %s failed!\n", HL_UPGRADE_TELINK_UART_DEV_NAME);
        return RT_ERROR;
    }

    // 以中断接收及轮询发送模式打开串口设备
    rt_device_open(s_upgrade.serial, RT_DEVICE_FLAG_INT_RX);
    // 设置接收回调函数
    rt_device_set_rx_indicate(s_upgrade.serial, _update_uart_receive_cb);
    return RT_EOK;
}

static rt_err_t hl_mod_upgrade_uart_deinit(void)
{
    return;
}

static rt_err_t hl_mod_upgrade_uart_send(uint8_t upgrade_cmd, uint8_t* upgrade_data, uint32_t upgrade_size)
{
    static uint8_t send_buf[HL_UPGRADE_BUF_SIZE] = { 0 };
    uint16_t frame_len = hl_util_hup_encode(s_upgrade.hup.hup_handle.role, upgrade_cmd, send_buf, HL_UPGRADE_BUF_SIZE,
                                            upgrade_data, upgrade_size);

    frame_len = rt_device_write(s_upgrade.serial, 0, send_buf, frame_len);
    return;
}

static uint8_t _upgrade_telink_start(void)
{
    s_upgrade_telink.upgrade_file = open(HL_UPGRADE_FILE_NAME_TELINK, O_RDONLY);
    s_upgrade_telink.upgrade_pack.upgrade_num = 0;
    s_upgrade_telink.file_start_addr          = 0;

    if (s_upgrade_telink.upgrade_file < 0) {
        return -1;
    } else {
        return 0;
    }
}

static uint8_t _upgrade_telink_stop(void)
{
    uint8_t  ret  = RT_FALSE;
    
    if (s_upgrade_telink.upgrade_file < 0) {
        rt_kprintf("no open file (%s) \r\n", HL_UPGRADE_FILE_NAME_TELINK);
        return ret;
    } else {
        close(s_upgrade_telink.upgrade_file);
        ret = RT_TRUE;
        rt_kprintf("close file (%s) \r\n", HL_UPGRADE_FILE_NAME_TELINK);
    }
    return ret;
}

static uint8_t hl_mod_upgrade_telink_pack(void)
{
    uint8_t  ret  = 0;
    uint32_t size = 0;

    memset(s_upgrade_telink.upgrade_pack.upgrade_data, NULL, HL_UPGRADE_DATA_SIZE);
    //lseek(s_upgrade_telink.upgrade_file, (s_upgrade_telink.file_start_addr + (s_upgrade_telink.upgrade_pack.upgrade_num * HL_UPGRADE_DATA_SIZE)), SEEK_SET);

    while (size < HL_UPGRADE_DATA_SIZE) {
        ret = read(s_upgrade_telink.upgrade_file, &s_upgrade_telink.upgrade_pack.upgrade_data[size], 128);

        if (ret > 0) {
            size += ret;
        } else if (ret == 0 && size > 0) {
            break;
        } else if (ret == 0) {
            ret = 0;
            hl_mod_upgrade_uart_send(HL_UPGRADE_TELINK_STOP_CMD, &ret, 1);
            rt_kprintf("upgrade telink stop (end) \r\n");
            return 0;
        } else if (ret < 0) {
            ret = 1;
            hl_mod_upgrade_uart_send(HL_UPGRADE_TELINK_STOP_CMD, &ret, 1);
            rt_kprintf("upgrade telink stop (error) \r\n");
            return -1;
        }
    }

    rt_kprintf("upgrade telink file read size(%d) \r\n", size);
    hl_mod_upgrade_uart_send(HL_UPGRADE_TELINK_PACK_CMD, &s_upgrade_telink.upgrade_pack, size + 2);  
    s_upgrade_telink.upgrade_pack.upgrade_num += 1;
    return 1;
}

static void _upgrade_telink_hup_handle_cb(hup_protocol_type_t hup_frame)
{
    rt_err_t ret = RT_EOK;

    // 计算hup中实际数据的长度
    uint16_t data_len = 0;
    data_len |= (hup_frame.data_len_h << 8);
    data_len |= hup_frame.data_len_l;

    switch (hup_frame.cmd) {
        case HL_UPGRADE_TELINK_RESET_CMD:
            break;

        case HL_UPGRADE_TELINK_START_CMD:
            if (s_upgrade.telink_state == HL_UPGRADE_IDLE_STATE) {
                s_upgrade.telink_state = HL_UPGRADE_UPGRADE_STATE;
                hl_mod_upgrade_telink_pack();
                rt_kprintf("telink upgrade start! \r\n");
            } else {
                rt_kprintf("telink upgrade start error! (%d) \r\n", s_upgrade.telink_state);
            }            
            break;

        case HL_UPGRADE_TELINK_SIZE_CMD:
            break;

        case HL_UPGRADE_TELINK_PACK_CMD:
            hl_mod_upgrade_telink_pack();
            break;

        case HL_UPGRADE_TELINK_STOP_CMD:
            hl_mod_upgrade_uart_send(HL_UPGRADE_TELINK_RESET_CMD, &ret, 0);
            if (hup_frame.data_addr[0] == 0) {
                _upgrade_telink_stop();
                unlink(HL_UPGRADE_FILE_NAME_TELINK);
                s_upgrade.telink_state = HL_UPGRADE_SUCCEED_STATE;
                rt_kprintf("telink upgrade succeed! \r\n");                
            } else {
                _upgrade_telink_stop();
                s_upgrade.telink_state = HL_UPGRADE_FAIL_STATE;
                rt_kprintf("telink upgrade error! \r\n");                
            }            
            break;

        default:
            break;
    }

    if (RT_EOK != ret) {
        rt_kprintf("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FUNCTION__, __LINE__, ret);
    }
}

static rt_err_t _update_uart_receive_cb(rt_device_t dev, rt_size_t size)
{
    rt_size_t data_size = 0;

    // 读出串口数据
    data_size = rt_device_read(s_upgrade.serial, 0, s_uart_recv_buf, size);
    // 将串口数据写入FIFO
    hl_util_fifo_write(&s_upgrade.fifo, s_uart_recv_buf, data_size);

    return RT_EOK;
}

static void _upgrade_telink_do(void)
{
    uint8_t  buf[HL_UPGRADE_BUF_SIZE] = { 0 };
    uint32_t size                     = 0;
    uint32_t i                        = 0;

    while (1) {
        if (s_upgrade.thread_flag == RT_TRUE) {
            // 读取Telink数据并解析
            size = hl_util_fifo_data_size(&s_upgrade.fifo);
            if (size > 0) {
                size = hl_util_fifo_read(&s_upgrade.fifo, buf, size);
#if 0
                rt_kprintf("uart msg(%d):\n", size);
                for(i = 0; i<size; i++) {
                    rt_kprintf(" %02x", s_uart_recv_buf[i]);
                }    
                rt_kprintf("\n");
#endif
                for (i = 0; i < size; i++) {
                    hl_util_hup_decode(&s_upgrade.hup, buf[i]);
                }
            }
        }
        if(s_upgrade.telink_state == HL_UPGRADE_SUCCEED_STATE || s_upgrade.telink_state == HL_UPGRADE_FAIL_STATE) {
            s_upgrade.telink_state = HL_UPGRADE_IDLE_STATE;
            break;
        }
        // 延时5ms
        rt_thread_mdelay(5);
    }
}

void hl_mod_upgrade_telink_start(void)
{
    uint8_t upgrade_param = 0;
    uint8_t i = 0;

    rt_kprintf("%s telink start !\n", __func__);

    s_upgrade.telink_task_thread = rt_thread_create("TelinkUp", _upgrade_telink_do, RT_NULL, 3072, 15, 20);

    if (!s_upgrade.telink_task_thread) {
        rt_kprintf("telink task create failed");
        return;
    } else {
        rt_thread_startup(s_upgrade.telink_task_thread);
    }        
    
    upgrade_param = _upgrade_telink_start();
    if (upgrade_param < 0) {
        rt_kprintf("upgrade file error");
    } else {
        for(i = 0; i<5; i++) {
            if (s_upgrade.telink_state != HL_UPGRADE_IDLE_STATE) {
                break;
            }
            rt_thread_mdelay(500);
            hl_mod_upgrade_uart_send(HL_UPGRADE_TELINK_START_CMD, &upgrade_param, 0);
        }
    }
}

void hl_mod_upgrade_telink_stop(void)
{
}

/*********************************OTA升级************************************/
static void hl_mod_upgrade_ota(void* arg)
{
    char*      file_url = NULL;
    ota_status ret;
    uint32_t   slot_boot_idx;

    rt_kprintf("%s Enter...", __func__);

#ifdef RT_USING_OTA_FROM_LOCAL
    rt_kprintf("This is ota # LOCAL # test ....");
    /* fw file in emmc device. */
    file_url = rt_calloc(1, 256);
    if (!file_url) {
        rt_kprintf("malloc memory fail ");
        return;
    }

    /* get current os run seq */
    if (ota_init() != OTA_STATUS_OK)
        goto END;

    if (fw_slot_get_current_running(&slot_boot_idx) != OTA_STATUS_OK)
        goto END;

    /* according os run seq to get local fw path (url)*/
    rt_memcpy(file_url, OTA_FW_LOCAL_PATH, rt_strlen(OTA_FW_LOCAL_PATH));

    fw_slot_reset_flag(slot_boot_idx);
    ret = ota_update_image(OTA_PROTOCOL_FILE, file_url);
    if (OTA_STATUS_OK != ret) {
        rt_kprintf("ota update image Fail!!!\n");
        goto END;
    }

#ifdef RT_USING_OTA_RECOVERY
    /* if enable recovery mode and running in slot 0, should reboot to slot 1 to
     * run ota task, to do upgrade work.
     */
    if (slot_boot_idx == 0 && OTA_STATUS_OK == ret) {
        rt_kprintf("!!! OTA type is recovery mode, will reboot to run Fw2 !!!");
        if (file_url)
            rt_free(file_url);

        if (!fw_slot_change(1 - slot_boot_idx))
            ota_reboot(1 - slot_boot_idx);
    }
#endif  //#ifdef RT_USING_OTA_RECOVERY

#endif  //#ifdef RT_USING_OTA_FROM_LOCAL

    if (file_url) {
        rt_free(file_url);
        unlink(HL_UPGRADE_FILE_NAME_RK);
        return;
    }
            
END:
    if (file_url)
        rt_free(file_url);
}

void hl_mod_upgrade_ota_start(void)
{
#if 1
    rt_kprintf("%s ota start !\n", __func__);
    s_upgrade.ota_task_thread = rt_thread_create("OtaUp", hl_mod_upgrade_ota, RT_NULL, 8192 * 2, 15, 20);

    if (!s_upgrade.ota_task_thread) {
        rt_kprintf("ota test task create failed");
        return;
    } else {
        rt_thread_startup(s_upgrade.ota_task_thread);
    }
        
#else
    uint32_t size = 0;
    uint32_t i    = 0;
    int      df   = 0;

    df = open("tx.img", O_RDONLY);  //fopen("tx.img", "rb");
    if (df < 0) {
        rt_kprintf("upgrade file failed \r\n");
        return -1;
    } else {
        rt_kprintf("upgrade file succeed \r\n");
    }
    size = read(
        df, s_upgrade_telink.upgrade_pack.upgrade_data,
        512);  //fread(&s_upgrade_telink.upgrade_pack.upgrade_data[0], sizeof(uint8_t), 128, s_upgrade_telink.upgrade_file);

    if (size == 0) {
        rt_kprintf("upgrade RK2108 stop (end) \r\n");
        return 0;
    } else if (size < 0) {
        rt_kprintf("upgrade RK2108 stop (error) \r\n");
        return -1;
    }

    rt_kprintf("OTA msg(%d):\n", size);
    for (i = 0; i < size; i++) {
        rt_kprintf(" %02x", s_upgrade_telink.upgrade_pack.upgrade_data[i]);
    }
    rt_kprintf("\n");
#endif
}

static rt_err_t hl_mod_upgrade_config(void)
{
    // 初始化Telink模块串口设备
    hl_mod_upgrade_uart_init();
    return RT_EOK;
}
/**************************************模块间消息**************************************/
static void _upgrade_send_msg(uint8_t msg_cmd, uint32_t param)
{
    rt_err_t res;

    if(s_upgrade.app_msq == NULL) {
        rt_kprintf("%s msq error \r\n", __func__);
        return;
    }

    s_upgrade.upgrade_msg.sender          = UPGRADE_MODE;
    s_upgrade.upgrade_msg.cmd             = msg_cmd;
    s_upgrade.upgrade_msg.len             = 0;
    s_upgrade.upgrade_msg.param.u32_param = param;

    res = rt_mq_send(s_upgrade.app_msq, (void*)(&s_upgrade.upgrade_msg), sizeof(mode_to_app_msg_t));
    if (res == -RT_EFULL) {
        rt_kprintf("upgrade msgq full");
    } else if (res == -RT_ERROR) {
        rt_kprintf("upgrade msgq err");
    }
}
/**************************************升级控制****************************************/
static rt_bool_t _upgrade_seek_fw_file(char* path)
{
    int       fd;
    rt_bool_t is_exist = RT_FALSE;

    rt_kprintf("%s: Enter \r\n", __func__);
    rt_kprintf("Check %s \r\n", path);
    fd = open(path, O_RDONLY, 0);

    if (fd < 0) {
        rt_kprintf("open file for check failed(%s) \r\n", path);
        return is_exist;
    } else {
        close(fd);
        is_exist = RT_TRUE;
        rt_kprintf("Found %s \r\n", path);
    }

    return is_exist;
}

static void _upgrade_switch(hl_mod_upgrade_device type)
{
    switch (type) {
        case HL_UPGRADE_RK:  /// 升级RK2108
            hl_mod_upgrade_ota_start();
            break;
        case HL_UPGRADE_TELINK:  /// 升级无线telink
            hl_mod_upgrade_telink_start();
            break;
        default:
            rt_kprintf("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FUNCTION__, __LINE__, type);
            break;
    }
}

static void _upgrade_seek(void)
{
    rt_bool_t ret = RT_FALSE;

    s_upgrade.task.type_num = 0;

    ret = RT_FALSE;
    ret = _upgrade_seek_fw_file(HL_UPGRADE_FILE_NAME_RK);
    if (ret == RT_TRUE) {
        s_upgrade.task.type_task[s_upgrade.task.type_num] = HL_UPGRADE_RK;
        s_upgrade.task.type_num++;
    }

    ret = RT_FALSE;
    ret = _upgrade_seek_fw_file(HL_UPGRADE_FILE_NAME_TELINK);
    if (ret == RT_TRUE) {
        s_upgrade.task.type_task[s_upgrade.task.type_num] = HL_UPGRADE_TELINK;
        s_upgrade.task.type_num++;
    }

    if (s_upgrade.task.type_num > 0) {
        _upgrade_send_msg(HL_UPGRADE_FIND_FW_MSG, s_upgrade.task.type_num);
        rt_kprintf("upgrade seek file num:(%d)! \r\n", s_upgrade.task.type_num);
    }
}

static void _upgrade_start(void)
{
    uint32_t i = 0;

    // 初始化Telink模块串口设备
    hl_mod_upgrade_uart_init();

    /// 升级任务处理
    for (i = 0; i < s_upgrade.task.type_num; i++) {
        _upgrade_switch(s_upgrade.task.type_task[i]);
    }
}

static void _upgrade_stop(void)
{
    return;
}

/* Exported functions --------------------------------------------------------*/

uint8_t hl_mod_upgrade_init(void* p_msg_handle)
{
    if (NULL == p_msg_handle) {
        rt_kprintf("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FUNCTION__, __LINE__, p_msg_handle);
        return -1;
    } else {
        rt_kprintf("upgrade start init \r\n");
    }

    // 申请缓冲区内存空间
    s_upgrade_hup_buf  = (uint8_t*)rt_malloc(sizeof(uint8_t) * HL_UPGRADE_TELINK_HUP_BUF_SIZE);
    s_upgrade_fifo_buf = (uint8_t*)rt_malloc(sizeof(uint8_t) * HL_UPGRADE_TELINK_FIFO_BUF_SIZE);
    if ((NULL == s_upgrade_hup_buf) || (NULL == s_upgrade_fifo_buf)) {
        rt_kprintf("[%s][line:%d] rt_malloc failed!!! \r\n", __FUNCTION__, __LINE__);
        return -1;
    }
    // 升级状态
    s_upgrade.telink_state = HL_UPGRADE_IDLE_STATE;
    s_upgrade.ota_state = HL_UPGRADE_IDLE_STATE;

    // Telink获取并赋值APP层下发的消息队列指针
    s_upgrade.app_msq     = p_msg_handle;
    s_upgrade.thread_flag = RT_TRUE;
    // Telink消息队列结构体赋初值
    s_upgrade.upgrade_msg.sender = UPGRADE_MODE;

    /* 初始化Telink模块串口交互所需的资源 */
    // 初始化hup
    s_upgrade.hup.hup_handle.role           = EM_HUP_ROLE_MASTER;
    s_upgrade.hup.hup_handle.timer_state    = EM_HUP_TIMER_DISABLE;
    s_upgrade.hup.hup_handle.frame_data_len = HL_UPGRADE_TELINK_HUP_BUF_SIZE;
    hl_util_hup_init(&s_upgrade.hup, s_upgrade_hup_buf, NULL, _upgrade_telink_hup_handle_cb);

    // 初始化fifo
    hl_util_fifo_init(&s_upgrade.fifo, s_upgrade_fifo_buf, HL_UPGRADE_TELINK_FIFO_BUF_SIZE);
    //hl_mod_upgrade_config();

    return 0;
}
void hl_mod_upgrade_temp(void)
{
    hl_mod_upgrade_init(NULL);
}

uint8_t hl_mod_upgrade_deinit()
{
    return 0;
}

uint8_t hl_mod_upgrade_io_ctrl(uint8_t cmd, void* ptr, uint16_t len)
{
    int8_t ret = 0;

    switch (cmd) {
        case HL_UPGRADE_OPEN_CMD:  /// 进行升级文件查找
            _upgrade_seek();
            break;
        case HL_UPGRADE_CLOSE_CMD:  /// 关闭升级进程
            _upgrade_stop();
            break;
        case HL_UPGRADE_START_CMD:  /// 开始升级
            _upgrade_start();
            break;
        case HL_UPGRADE_STATE_CMD:  /// 获取升级状态
            break;
        case HL_UPGRADE_GET_VERSIONS_CMD:  /// 获取升级包里面的设备版本号
            break;
        default:
            rt_kprintf("[%s][line:%d] cmd(%d) error!!! \r\n", __FILE__, __LINE__, cmd);
            break;
    }

    return 0;
}

/**
 * 
 * @brief 
 * @param [in] argc 
 * @param [in] argv 
 * @return int 
 * @date 2022-10-13
 * @author lixiang (rd37@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-10-13      <td>lixiang     <td>新建
 * </table>
 */

int hl_mod_upgrade_test(int argc, char** argv)
{
    uint8_t  upgrade_param = 0;
    uint8_t  buf[10]       = { 0 };
    uint32_t size          = 0;

    if (argc <= 1) {
        rt_kprintf("wrong parameter, please type: hl_mod_upgrade_test [start | stop | send | ota | telink | seek | "
                   "upgrade] [] \r\n");
        return 0;
    }

    if (!strcmp("start", argv[1])) {
        upgrade_param = _upgrade_telink_start();
        if (upgrade_param < 0) {
            rt_kprintf("upgrade file error");
        } else {
            hl_mod_upgrade_uart_send(HL_UPGRADE_TELINK_START_CMD, &buf, 0);
        }
    } else if (!strcmp("stop", argv[1])) {
        hl_mod_upgrade_uart_send(HL_UPGRADE_TELINK_STOP_CMD, &buf, 0);
    } else if (!strcmp("send", argv[1])) {
        hl_mod_upgrade_telink_pack();
    } else if (!strcmp("ota", argv[1])) {
        hl_mod_upgrade_ota_start();
    } else if (!strcmp("telink", argv[1])) {
        hl_mod_upgrade_telink_start();
    } else if (!strcmp("seek", argv[1])) {
        _upgrade_seek();
    } else if (!strcmp("upgrade", argv[1])) {
        _upgrade_start();
    } else {
        rt_kprintf("wrong parameter, please type: hl_mod_upgrade_test cmd error\r\n");
        return 0;
    }

    return 0;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(hl_mod_upgrade_test, upgrade io ctrl cmd);
#endif
//INIT_APP_EXPORT(hl_mod_upgrade_temp);
#endif  // HL_MOD_UPGRADE_C
        /*
 * EOF
 */