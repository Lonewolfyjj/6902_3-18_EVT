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

#include "hl_mod_wdog.h"

#include "cJSON/cJSON.h"

#define DBG_SECTION_NAME "mod_up"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>
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

typedef struct _upgrade_json
{
    uint8_t name[20];
    uint8_t type;
    uint8_t version[20];
    uint8_t hardversion[20];
} upgrade_json;

typedef struct _upgrade_pack_info_t
{
    uint8_t type;
    uint8_t version[20];
    uint8_t hardversion[20];
    uint8_t packcount;
    upgrade_json pack[HL_UPGRADE_MAX];
} upgrade_pack_info_t;

typedef struct _hl_mod_upgrade_telink_t
{
    /// 升级数据包
    upgrade_pack upgrade_pack;
    /// 升级文件
    int upgrade_file;
    /// telink升级文件的开始地址
    uint32_t file_start_addr;
} hl_mod_upgrade_telink_t;

typedef struct _hl_mod_upgrade_pack_t
{
    /// 升级包名字
    char file_name[50];
    /// 升级包句柄
    int file_fd;
    /// 文件数量
    uint8_t type_num;
    /// 文件长度（+1 一个JSON配置表文件）
    long type_len[HL_UPGRADE_MAX+1];
} hl_mod_upgrade_pack_t;

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
    /// 升级包
    hl_mod_upgrade_pack_t pack;
    /// TELINK版本号
    uint8_t telink_version[20];
} hl_mod_upgrade_t;

/* define --------------------------------------------------------------------*/
#define HL_UPGRADE_BUF_SIZE (1048)

#define HL_UPGRADE_TELINK_UART_DEV_NAME "uart2"
#define HL_UPGRADE_TELINK_UART_BUF_SIZE (HL_UPGRADE_BUF_SIZE * 3)
#define HL_UPGRADE_TELINK_FIFO_BUF_SIZE (HL_UPGRADE_BUF_SIZE * 3)
#define HL_UPGRADE_TELINK_HUP_BUF_SIZE 256

#define HL_UPGRADE_TELINK_VERSION_CMD 0x00
#define HL_UPGRADE_TELINK_RESET_CMD 0xA5
#define HL_UPGRADE_TELINK_START_CMD 0xA1
#define HL_UPGRADE_TELINK_SIZE_CMD 0xA2
#define HL_UPGRADE_TELINK_PACK_CMD 0xA3
#define HL_UPGRADE_TELINK_STOP_CMD 0xA4

#if HL_IS_TX_DEVICE()
#define HL_UPGRADE_FILE_NAME_TELINK "/mnt/sdcard/telink.bin"
#define HL_UPGRADE_FILE_NAME_RK "/mnt/sdcard/mcu.img"
#define HL_UPGRADE_FILE_PATH "/mnt/sdcard/"
#define HL_UPGRADE_PACK_NAME "A6902_Upgrade_Firmware_TX"
#else
#define HL_UPGRADE_FILE_NAME_TELINK "telink.bin"
#define HL_UPGRADE_FILE_NAME_RK "mcu.img"
#define HL_UPGRADE_FILE_PATH "/"
#define HL_UPGRADE_PACK_NAME "A6902_Upgrade_Firmware_RX"
#endif

#define HL_UPGRADE_NAME_RK "rk2108"
#define HL_UPGRADE_NAME_TELINK "telink"

#define HL_UPGADE_JSON_NAME "name"
#define HL_UPGADE_JSON_TYPE "type"
#define HL_UPGADE_JSON_SOFTVER "softver"
#define HL_UPGADE_JSON_HARDVER "hardver"
#define HL_UPGADE_JSON_PACKCOUNT "packcount"
#define HL_UPGADE_JSON_PACKLIST "packlist"

/* variables -----------------------------------------------------------------*/
// 串口数据包
static hl_mod_upgrade_telink_t s_upgrade_telink = { 0 };

// upgrade模块句柄
static hl_mod_upgrade_t s_upgrade = { 0 };

// 升级包的信息
static upgrade_pack_info_t s_pack_info = {0};

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
// 开始升级
static void _upgrade_start(void);

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
            if (s_upgrade.telink_state == HL_UPGRADE_START_STATE) {
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
            if (s_upgrade.telink_state == HL_UPGRADE_UPGRADE_STATE) {
                hl_mod_upgrade_telink_pack();
            } else {
                LOG_E("telink upgrade state (%d = %d)! ", s_upgrade.telink_state, HL_UPGRADE_IDLE_STATE);
            }
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
        
        case HL_UPGRADE_TELINK_VERSION_CMD:
            memset(&s_upgrade.telink_version[0], 0, 20);
            rt_sprintf(&s_upgrade.telink_version[0], "V%d.%d.%d.%d", hup_frame.data_addr[0], hup_frame.data_addr[1], hup_frame.data_addr[2], hup_frame.data_addr[3]);
            LOG_D("telink version %s ", s_upgrade.telink_version); 
            if(rt_strstr(s_pack_info.pack[1].version, s_upgrade.telink_version) == RT_NULL) {
                if(s_upgrade.telink_state == HL_UPGRADE_IDLE_STATE) {
                    s_upgrade.telink_state = HL_UPGRADE_START_STATE;
                    hl_mod_upgrade_uart_send(HL_UPGRADE_TELINK_START_CMD, hup_frame.data_addr, 0);
                } else {
                    LOG_E("telink upgrade state (%d = %d)! ", s_upgrade.telink_state, HL_UPGRADE_IDLE_STATE);
                }
            } else {
                _upgrade_telink_stop();
                unlink(HL_UPGRADE_FILE_NAME_TELINK);
                s_upgrade.telink_state = HL_UPGRADE_SUCCEED_STATE;
                LOG_D("telink upgrade finish (%s = %s)! ", s_pack_info.pack[1].version, s_upgrade.telink_version);                
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

    LOG_I("%s telink start !\n", __func__);

    s_upgrade.telink_task_thread = rt_thread_create("TelinkUp", _upgrade_telink_do, RT_NULL, 3072, 15, 20);

    if (!s_upgrade.telink_task_thread) {
        LOG_E("telink task create failed");
        return;
    } else {
        rt_thread_startup(s_upgrade.telink_task_thread);
    }        
    
    upgrade_param = _upgrade_telink_start();
    if (upgrade_param < 0) {
        LOG_E("upgrade file error");
    } else {
        for(i = 0; i<5; i++) {
            if (s_upgrade.telink_state != HL_UPGRADE_IDLE_STATE) {
                break;
            }                       
            hl_mod_upgrade_uart_send(HL_UPGRADE_TELINK_VERSION_CMD, &upgrade_param, 0);
            LOG_I("get telink version(%d) ", i);
            rt_thread_mdelay(500);
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
    rt_kprintf("%s ota start !\n", __func__);
    s_upgrade.ota_task_thread = rt_thread_create("OtaUp", hl_mod_upgrade_ota, RT_NULL, 8192 * 2, 15, 20);

    if (!s_upgrade.ota_task_thread) {
        rt_kprintf("ota test task create failed");
        return;
    } else {
        rt_thread_startup(s_upgrade.ota_task_thread);
    }
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
        LOG_D("upgrade seek file num:(%d)! ", s_upgrade.task.type_num);
        _upgrade_start(); // _upgrade_send_msg(HL_UPGRADE_FIND_FW_MSG, s_upgrade.task.type_num);        
    } else {
        LOG_E("upgrade seek file seek null(%d)! ", s_upgrade.task.type_num);
    }
    
}

static bool _upgrade_seek_(void)
{
    DIR* dir = opendir(HL_UPGRADE_FILE_PATH);

    struct dirent* dirent;

    do {
        dirent = readdir(dir);
        if (dirent == RT_NULL) {
            LOG_E("%s read dir error", __func__);
            break;
        }
#if HL_IS_TX_DEVICE()
        if ((rt_strstr(dirent->d_name, "HLD_A6902_") != RT_NULL) && (rt_strstr(dirent->d_name, "_TX.ota") != RT_NULL)) {
#else
        if ((rt_strstr(dirent->d_name, "HLD_A6902_") != RT_NULL) && (rt_strstr(dirent->d_name, "_RX.ota") != RT_NULL)) {
#endif
            if(strlen(dirent->d_name) > 50) {
                LOG_D("seek upgrade file: (%s) name error ", dirent->d_name);
                break;
            }
            memcpy(s_upgrade.pack.file_name, dirent->d_name, strlen(dirent->d_name));
            LOG_D("seek upgrade file: %s ", s_upgrade.pack.file_name);
            closedir(dir);
            
            _upgrade_send_msg(HL_UPGRADE_FIND_FW_MSG, 1);

            return true;
        }
    } while (dirent != RT_NULL);
    closedir(dir);

    return false;
}

static bool _upgrade_nuzip_head_(void)
{
    char buff[255] = {0};
    int ret = 0;
    uint8_t i = 0;
    uint8_t num = 0;
    long len = 0;

    len = lseek(s_upgrade.pack.file_fd, 0, SEEK_END);

    if(len < 128) {
        LOG_E("pack head size error (%ld)", len); 
        return false;
    }

    lseek(s_upgrade.pack.file_fd, len-1, SEEK_SET);
    ret = read(s_upgrade.pack.file_fd, &buff[0], 1);
    s_upgrade.pack.type_num = buff[0] / 8;
    if((s_upgrade.pack.type_num*8) != buff[0]) {
        LOG_E("(%d == %d)pack head size error ", s_upgrade.pack.type_num*8, buff[0]); 
        return false;
    }
    LOG_D("pack head size:%d ", buff[0]); 
    lseek(s_upgrade.pack.file_fd, len-(buff[0]+1), SEEK_SET);
    ret = read(s_upgrade.pack.file_fd, &buff[0], buff[0]);

    for(num = 0; num < s_upgrade.pack.type_num; num++) {
        s_upgrade.pack.type_len[num]  = ((long)buff[i++]<<56)&((long)0xFF<<56);
        s_upgrade.pack.type_len[num] += ((long)buff[i++]<<48)&((long)0xFF<<48);
        s_upgrade.pack.type_len[num] += ((long)buff[i++]<<40)&((long)0xFF<<40);
        s_upgrade.pack.type_len[num] += ((long)buff[i++]<<32)&((long)0xFF<<32);
        s_upgrade.pack.type_len[num] += ((long)buff[i++]<<24)&((long)0xFF<<24);
        s_upgrade.pack.type_len[num] += ((long)buff[i++]<<16)&((long)0xFF<<16);
        s_upgrade.pack.type_len[num] += ((long)buff[i++]<<8)&((long)0xFF<<8); 
        s_upgrade.pack.type_len[num] += ((long)buff[i++])&((long)0xFF);      
        LOG_D("pack(%d) file len: %ld ", num, s_upgrade.pack.type_len[num]);      
    }

    if(num == 0) {
        return false;
    }
    return true;
}

static rt_bool_t _upgrad_param_get(cJSON* json_paramaters, char* param_key, char* param_value, uint16_t value_len)
{
    if (NULL == param_key || NULL == param_value || value_len == 0) {
        LOG_E("error: _upgrad_param_get\r\n");
        return false;
    }

    cJSON*     item;
    cJSON_bool check_ret = cJSON_False;

    check_ret = cJSON_HasObjectItem(json_paramaters, param_key);

    if (check_ret) {
        item = cJSON_GetObjectItem(json_paramaters, param_key);
        if (value_len < strlen(item->valuestring) + 1) {            
            LOG_E("error: buffer len no enough bufferlen = %d, value_len = %d\r\n", value_len, strlen(item->valuestring) + 1);
            return false;
        }
        strcpy(param_value, item->valuestring);
    } else {
        LOG_E("error: upgrade no item %s\r\n", param_key);
        return false;
    }

    return true;
}

uint8_t _upgrad_param_get_integer(cJSON* json_paramaters, char* param_key, int* param_value)
{
    if (NULL == param_key || NULL == param_value) {
        LOG_E("error: _upgrad_param_get_integer");
        return 1;
    }

    cJSON*     item;
    cJSON_bool check_ret = cJSON_False;

    check_ret = cJSON_HasObjectItem(json_paramaters, param_key);
    if (check_ret) {
        item = cJSON_GetObjectItem(json_paramaters, param_key);
        *param_value = atoi(item->valuestring);
        LOG_I("integer: %d ", *param_value);
    } else {
        LOG_E("error: nvram have no item %s", param_key);
        return false;
    }

    return true;
}

static void _upgrade_nuzip_json_show(void)
{
    int i = 0;

    rt_kprintf("==============upgrade info================\n");
    rt_kprintf("upgrade param:\n");
    rt_kprintf("{ \n%s: %d \n", HL_UPGADE_JSON_TYPE, s_pack_info.type);
    rt_kprintf("%s: %s \n", HL_UPGADE_JSON_SOFTVER, s_pack_info.version);
    rt_kprintf("%s: %s \n", HL_UPGADE_JSON_HARDVER, s_pack_info.hardversion);
    rt_kprintf("%s: %d\n}\n", HL_UPGADE_JSON_PACKCOUNT, s_pack_info.packcount);

    rt_kprintf("[\n");
    for(i = 0; i<s_pack_info.packcount; i++) {
        rt_kprintf("    { \n    %s: %d \n", HL_UPGADE_JSON_TYPE, s_pack_info.pack[i].type);
        rt_kprintf("    %s: %s \n", HL_UPGADE_JSON_NAME, s_pack_info.pack[i].name);        
        rt_kprintf("    %s: %s \n", HL_UPGADE_JSON_SOFTVER, s_pack_info.pack[i].version);
        rt_kprintf("    %s: %s \n    }\n", HL_UPGADE_JSON_HARDVER, s_pack_info.pack[i].hardversion);       
    }
    rt_kprintf("]\n");
    rt_kprintf("===========================================\n");    
}

static rt_bool_t _upgrade_nuzip_json(long len)
{
    int       fd;
    char*     json_buff;
    char      name_buff[50] = {0};
    char      pack_name_key[4][20] = {0};
    int       read_len  = 0;
    int       write_len = 0;
    int       i = 0;
    cJSON*    json_param = NULL;

    LOG_D("read json upgrade config ");

    if(len > (1024*10) != len == 0) {
        LOG_E("upgrade pack json len error %ld ", len);
        return false;
    }
    json_buff = rt_malloc(len);

    lseek(s_upgrade.pack.file_fd, s_upgrade.pack.type_len[0]+s_upgrade.pack.type_len[1], SEEK_SET);
    read_len = read(s_upgrade.pack.file_fd, json_buff, len);
    if(read_len != len) {
        LOG_E("upgrade pack json read len error %ld == %ld ", len, read_len);
        return false;
    }

    rt_kprintf("\n");
    for (i = 0; i<len; i++)
        rt_kprintf("%c", json_buff[i]);
    rt_kprintf("\n");
    json_param = cJSON_Parse(json_buff);
    
    _upgrad_param_get(json_param, HL_UPGADE_JSON_NAME, name_buff, 50);
    if(rt_strstr(name_buff, HL_UPGRADE_PACK_NAME) == RT_NULL) {
        LOG_E("(%s)upgrade pack name error", name_buff);
        return false;
    }
    _upgrad_param_get_integer(json_param, HL_UPGADE_JSON_TYPE, &s_pack_info.type);
    _upgrad_param_get(json_param, HL_UPGADE_JSON_SOFTVER, s_pack_info.version, 20);
    _upgrad_param_get(json_param, HL_UPGADE_JSON_HARDVER, s_pack_info.hardversion, 20);
    _upgrad_param_get_integer(json_param, HL_UPGADE_JSON_PACKCOUNT, &s_pack_info.packcount);
    if(s_pack_info.packcount <= 0 || s_pack_info.packcount > HL_UPGRADE_MAX) {
        LOG_E("(%d)upgrade pack count error", s_pack_info.packcount);
        return false;
    }

    for(i = 0; i<s_pack_info.packcount; i++) {
        memset(pack_name_key[0], 0, 20);
        memset(pack_name_key[2], 0, 20);
        memset(pack_name_key[3], 0, 20);
        memset(pack_name_key[4], 0, 20);
        rt_sprintf(pack_name_key[0], "%s%d", HL_UPGADE_JSON_TYPE, i);
        rt_sprintf(pack_name_key[1], "%s%d", HL_UPGADE_JSON_NAME, i);
        rt_sprintf(pack_name_key[2], "%s%d", HL_UPGADE_JSON_SOFTVER, i);
        rt_sprintf(pack_name_key[3], "%s%d", HL_UPGADE_JSON_HARDVER, i);
        
        _upgrad_param_get_integer(json_param, pack_name_key[0], &s_pack_info.pack[i].type);
        _upgrad_param_get(json_param, pack_name_key[1], s_pack_info.pack[i].name, 20);        
        _upgrad_param_get(json_param, pack_name_key[2], s_pack_info.pack[i].version, 20);
        _upgrad_param_get(json_param, pack_name_key[3], s_pack_info.pack[i].hardversion, 20);       
    }    

    _upgrade_nuzip_json_show();
    cJSON_Delete(json_param);
    free(json_buff);
    return true;
}

static rt_bool_t _upgrade_nuzip_file(char* path, long len)
{
    int       fd;
    char      buff[HL_UPGRADE_DATA_SIZE];
    int       read_len  = 0;
    int       write_len = 0;
    rt_bool_t is_exist  = RT_FALSE;
    long      file_len = 0;
    struct statfs stat = {0};

    LOG_D("Create %s ", path);

    fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0);
    if (fd < 0) {
        LOG_E("open file for check failed(%s) \r\n", path);
        return is_exist;
    }    

    statfs("/", &stat);
    LOG_D("root size  %ld - %ld - %ld ", stat.f_bfree, stat.f_blocks, stat.f_bsize);
    lseek(fd, 0, SEEK_SET);
    do {
        if((len - file_len) > HL_UPGRADE_DATA_SIZE) {
            read_len = read(s_upgrade.pack.file_fd, buff, HL_UPGRADE_DATA_SIZE);
        } else {
            read_len = read(s_upgrade.pack.file_fd, buff, (len - file_len));
        }
        
        if (read_len <= 0) {
            close(fd);
            LOG_E("read file failed.\n");
            return false;
        }
        write_len = write(fd, buff, read_len);
        if (read_len != write_len) {
            close(fd);
            LOG_E("write file failed.\n");
            return false;
        }
        file_len += read_len;
        if(file_len >= len) {
            break;
        }

        if((file_len % 400384) == 0) {
            rt_thread_mdelay(100);
        }
        rt_kprintf("size :%ld (%ld) \n", file_len, len);

    } while (read_len == HL_UPGRADE_DATA_SIZE);

    close(fd);
    LOG_D("Create %s succeed\r\n", path);

    return true;
}

static rt_bool_t _upgrade_nuzip_file_mcu(void)
{
    int       fd;
    char      buff[HL_UPGRADE_DATA_SIZE];
    int       read_len  = 0;
    int       write_len = 0;
    rt_bool_t is_exist  = RT_FALSE;
    long      file_len = 0;
    long      len = 0xB00*512;

    LOG_D("Repair %s ", HL_UPGRADE_FILE_NAME_RK);

    fd = open(HL_UPGRADE_FILE_NAME_RK, O_RDWR | O_APPEND, 0);
    if (fd < 0) {
        LOG_E("open file for check failed(%s) \r\n", HL_UPGRADE_FILE_NAME_RK);
        return is_exist;
    }
    
    do {
        lseek(fd, 0x100*512+file_len, SEEK_SET);
        if((len - file_len) > HL_UPGRADE_DATA_SIZE) {
            read_len = read(fd, buff, HL_UPGRADE_DATA_SIZE);
        } else {
            read_len = read(fd, buff, (len - file_len));
        }        
        
        if (read_len <= 0) {
            close(fd);
            LOG_E("read file failed.\n");
            return false;
        }

        lseek(fd, 0, SEEK_END);
        write_len = write(fd, buff, read_len);
        if (read_len != write_len) {
            close(fd);
            LOG_E("write file failed.\n");
            return false;
        }
        file_len += read_len;
        if(file_len >= len) {
            break;
        }
        if((file_len % 20480) == 0) {
            rt_thread_mdelay(100);
        }
        rt_kprintf("size :%ld (%ld) \n", file_len, len);
        
    } while (read_len == HL_UPGRADE_DATA_SIZE);

    close(fd);
    LOG_D("Repair %s succeed\r\n", HL_UPGRADE_FILE_NAME_RK);

    return true;
}

static bool _upgrade_nuzip(void)
{
    int i = 0;
    char packname[128];
    char szcmd[128];
    char szbuf[128] = {0};
    long file_len = 0;
    rt_bool_t ret = true;

    /*modify file name*/
    rt_sprintf(packname, "%s%s", HL_UPGRADE_FILE_PATH, s_upgrade.pack.file_name);
    LOG_I(" %s ", packname);

    s_upgrade.pack.file_fd = open(packname, O_RDONLY, 0);

    if(s_upgrade.pack.file_fd < 0) {
        LOG_E("%s", packname);
        return false;
    }

    /*pack upgrade file ---> info.json*/
    ret = _upgrade_nuzip_head_();
    if (ret != true){
        LOG_E("_upgrade_nuzip_head_ error");
        return false;
    }
    
    ret = _upgrade_nuzip_json(s_upgrade.pack.type_len[2]);
    if (ret != true){
        LOG_E("_upgrade_nuzip_json error");
        return false;
    }

    /*pack upgrade file  telink.bin*/
    file_len += s_upgrade.pack.type_len[0];
    if(rt_strstr(s_pack_info.pack[1].name, HL_UPGRADE_NAME_TELINK) != RT_NULL) { 
        lseek(s_upgrade.pack.file_fd, file_len, SEEK_SET);
        ret = _upgrade_nuzip_file(HL_UPGRADE_FILE_NAME_TELINK, s_upgrade.pack.type_len[1]);
        if (ret != true) {
            LOG_E("_upgrade_nuzip_file %s error", HL_UPGRADE_FILE_NAME_TELINK);
            return false;
        }
    } else {
        LOG_E("upgrade %s hardversion %s error", s_pack_info.pack[1].name, s_pack_info.pack[1].hardversion);
    }

    ret = close(s_upgrade.pack.file_fd);
    if(ret < 0) {
        LOG_E("close error%s", packname);
        return false;
    }
    LOG_I("close file (%s) ", packname);

    /*pack upgrade file  muc.img*/
    if((rt_strstr(s_pack_info.pack[0].name, HL_UPGRADE_NAME_RK) != RT_NULL) && \
        (rt_strstr(s_pack_info.pack[0].version, A6902_VERSION) == RT_NULL)) {
        ret = rename(packname, HL_UPGRADE_FILE_NAME_RK);
        if(ret < 0) {
            LOG_E("close error%s", packname);
            return false;
        }
        LOG_I("set name file (%s)to(%s) ", packname, HL_UPGRADE_FILE_NAME_RK);
    } else {
        ret = unlink(packname);
        if(ret < 0) {
            LOG_E("unlink error%s", packname);
            return false;
        }      
        LOG_I("delete file (%s) ", packname);
        LOG_E("upgrade %s hardversion %s error", s_pack_info.pack[0].name, s_pack_info.pack[0].hardversion);
    }
    
    _upgrade_seek();

    return true;
}

static void hl_mod_upgrade_guard(void)
{
    while(1){
        hl_mod_feed_dog();
        rt_thread_mdelay(1000);
    }
}

void hl_mod_upgrade_guard_start(void)
{
    rt_kprintf("%s guard start!\n", __func__);
    s_upgrade.ota_task_thread = rt_thread_create("WDog", hl_mod_upgrade_guard, RT_NULL, 255, 5, 1);

    if (!s_upgrade.ota_task_thread) {
        rt_kprintf("guard start task create failed");
        return;
    } else {
        rt_thread_startup(s_upgrade.ota_task_thread);
    }
}

static void _upgrade_start(void)
{
    uint32_t i = 0;

    // 初始化Telink模块串口设备
    hl_mod_upgrade_uart_init();
    hl_mod_upgrade_guard_start();

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
    static uint8_t flag = 0;
    int8_t ret = 0;

    LOG_D("[line:%d] cmd(%d)", __LINE__, cmd);
    switch (cmd) {
        case HL_UPGRADE_OPEN_CMD:  /// 进行升级文件查找
            if ((flag == 1)||(s_upgrade.telink_state != HL_UPGRADE_IDLE_STATE)) {
                break;
            }
            flag = 1;
            rt_thread_mdelay(5000);     
            _upgrade_seek_();      
            flag = 0;
            break;
        case HL_UPGRADE_CLOSE_CMD:  /// 关闭升级进程
            _upgrade_stop();
            break;
        case HL_UPGRADE_START_CMD:  /// 开始升级
            if ((flag == 1)||(s_upgrade.telink_state != HL_UPGRADE_IDLE_STATE)) {
                break;
            }
            flag = 1;
            _upgrade_nuzip();
            flag = 0;
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
    struct statfs stat = {0};
    rt_bool_t ret = true;

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
    } else if (!strcmp("test", argv[1])) {
        ret = _upgrade_seek_();
            if (ret != true){
            LOG_E("_upgrade_seek_ error");
            return false;
        }
        ret = _upgrade_nuzip();
            if (ret != true){
            LOG_E("_upgrade_nuzip error");
            return false;
        }

        LOG_D("test finish");
    } else if (!strcmp("stat", argv[1])) {
        statfs(argv[2], &stat);
        LOG_D("root size  %ld - %ld - %ld ", stat.f_bfree, stat.f_blocks, stat.f_bsize);
    } else if (!strcmp("start", argv[1])) {
        ret = _upgrade_nuzip();
        if (ret != true){
            LOG_E("_upgrade_nuzip error");
            return false;
        }
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