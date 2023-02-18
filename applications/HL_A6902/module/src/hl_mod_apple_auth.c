#include "hl_mod_apple_auth.h"
#include "hl_util_msg_type.h"
#include "hl_drv_usb_vendor_class_com.h"
#include "hl_util_nvram.h"

#define DBG_SECTION_NAME "mod_apple"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

/// IAP2线程句柄
static rt_thread_t hl_mod_apple_auth_iap2_thread;
/// EAP线程句柄
static rt_thread_t hl_mod_apple_auth_eap_thread;
/// appleauth控制句柄
static hl_mod_apple_auth_t s_apple = { 0 };
/// 消息队列结构体
static mode_to_app_msg_t app_msg_t;

/**
 * _hl_mod_apple_usb_read
 * @brief USB读数据操作
 * @param [in] read_data_addr 读数据缓冲区
 * @param [in] read_data_len 读数据长度
 * @param [in] timeout 超时时间
 * @return int 成功 读取数据长度 | 失败 0
 * @date 2023-01-12
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-01-12      <td>lisonglin     <td>新建
 * </table>
 */
static int _hl_mod_apple_usb_read(uint8_t* read_data_addr, uint16_t read_data_len, uint16_t timeout)
{
    int ret = 0;

    if (read_data_len > 64) {
        for (uint16_t i = 0; i < read_data_len / 64; i++) {
            ret += hl_drv_usb_vendor_class_com_read(read_data_addr + (i * 64), 64, timeout);
        }
    } else {
        ret += hl_drv_usb_vendor_class_com_read(read_data_addr, read_data_len, timeout);
    }

    rt_thread_mdelay(10);

#if LOG_APPLE_USB_OPEN
    rt_kprintf("\n-->[iAP2]USB Read Len:%d", ret);
    for (uint16_t i = 0; i < ret; i++) {
        if (i % 20 == 0) {
            rt_kprintf("\n");
        }
        rt_kprintf("%02x ", read_data_addr[i]);
    }
    rt_kprintf("\n");
    rt_kprintf("\n");
#endif
    return ret;
}

/**
 * _hl_mod_apple_usb_write
 * @brief USB写数据操作
 * @param [in] write_data_addr 写数据内容
 * @param [in] write_data_len 写数据长度
 * @return int 成功 0
 * @date 2023-01-12
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-01-12      <td>lisonglin     <td>新建
 * </table>
 */
static int _hl_mod_apple_usb_write(uint8_t* write_data_addr, uint16_t write_data_len)
{
    uint8_t ret = 0;
    uint8_t len = 64;

    if (write_data_len > 64) {
        uint8_t len_left = write_data_len % 64 == 0 ? 0 : 1;
        for (uint16_t i = 0; i < (write_data_len / 64) + len_left; i++) {
            if ((write_data_len - i * 64) > 64) {
                len = 64;
            } else {
                len = (write_data_len - i * 64);
            }
            ret += hl_drv_usb_vendor_class_com_write(write_data_addr + (i * 64), len);
        }
    } else {
        ret += hl_drv_usb_vendor_class_com_write(write_data_addr, write_data_len);
    }

    rt_thread_mdelay(10);

#if LOG_APPLE_USB_OPEN
    rt_kprintf("\n-->[iAP2]USB Write Len:%d", write_data_len);
    rt_kprintf("\n");
    rt_kprintf("\n");
#endif

    return RT_EOK;
}

/**
 * _hl_mod_apple_i2c_read
 * @brief IIC读数据操作
 * @param [in] reg_addr 读取寄存器
 * @param [in] read_data_addr 读数据缓冲区
 * @param [in] read_data_len 读数据长度
 * @param [in] timeout 超时时间
 * @return int 成功 读数据长度 | 失败 0
 * @date 2023-01-12
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-01-12      <td>lisonglin     <td>新建
 * </table>
 */
static int _hl_mod_apple_i2c_read(uint8_t reg_addr, uint8_t* read_data_addr, uint16_t read_data_len, uint16_t timeout)
{
    struct rt_i2c_msg msgs[2]  = { 0 };
    rt_size_t         size     = 0;
    uint8_t           try_time = 5;

    msgs[0].addr  = MFI_IC_IIC_ADDR;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &reg_addr;
    msgs[0].len   = 1;

    do {
        size = rt_i2c_transfer(s_apple.mfi_chip_iic, &msgs[0], 1);
        try_time--;
        if (!try_time) {
            rt_kprintf("i2c[%02X] read1 err!\n", reg_addr);
            return 0;
        }
#if LOG_APPLE_I2C_OPEN
        rt_kprintf("i2c read msg[0] = %d\n", size);
#endif
        rt_thread_mdelay(5);
    } while (size != 1);

#if LOG_APPLE_I2C_OPEN
    for (size_t i = 0; i < msgs[0].len; i++) {
        rt_kprintf("%02x ", msgs[0].buf[i]);
    }
#endif
    try_time = 5;

    msgs[1].addr  = MFI_IC_IIC_ADDR;
    msgs[1].flags = RT_I2C_RD | RT_I2C_IGNORE_NACK;  // | RT_I2C_NO_READ_ACK
    msgs[1].buf   = read_data_addr;
    msgs[1].len   = read_data_len;

    do {
        size = rt_i2c_transfer(s_apple.mfi_chip_iic, &msgs[1], 1);
        try_time--;
        if (!try_time) {
            rt_kprintf("i2c read2 err!\n");
            return 0;
        }
#if LOG_APPLE_I2C_OPEN
        rt_kprintf("i2c read msg[1] = %d\n", size);
#endif
        rt_thread_mdelay(5);
    } while (size != 1);

#if LOG_APPLE_I2C_OPEN
    rt_kprintf("\n-->[iAP2]IIC Read Data:[");
    for (uint16_t i = 0; i < read_data_len; i++) {
        if (i % 20 == 0) {
            rt_kprintf("\n");
        }
        rt_kprintf("%02x ", read_data_addr[i]);
    }
    rt_kprintf("\n");
    rt_kprintf("\n");
#endif

    return read_data_len;
}

/**
 * _hl_mod_apple_i2c_write
 * @brief IIC写数据操作
 * @param [in] reg_addr 写数据寄存器
 * @param [in] write_data_addr 写数据内容
 * @param [in] write_data_len 写数据长度
 * @return int 成功 0 | 失败 1
 * @date 2023-01-12
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-01-12      <td>lisonglin     <td>新建
 * </table>
 */
static int _hl_mod_apple_i2c_write(uint8_t reg_addr, uint8_t* write_data_addr, uint16_t write_data_len)
{
    struct rt_i2c_msg msgs        = { 0 };
    uint8_t           buffer[128] = { 0 };
    rt_size_t         size        = 0;
    uint8_t           try_time    = 5;

    buffer[0] = reg_addr;
    memcpy(buffer + 1, write_data_addr, write_data_len);

    msgs.addr  = MFI_IC_IIC_ADDR;
    msgs.flags = RT_I2C_WR;
    msgs.buf   = buffer;
    msgs.len   = write_data_len + 1;

    do {
        size = rt_i2c_transfer(s_apple.mfi_chip_iic, &msgs, 1);
        try_time--;
        if (!try_time) {
            rt_kprintf("i2c[%02X] write err!\n", reg_addr);
            return RT_ERROR;
        }
#if LOG_APPLE_I2C_OPEN
        rt_kprintf("i2c write msg[0] size = %d\n", size);
#endif
    } while (size != 1);

#if LOG_APPLE_I2C_OPEN
    rt_kprintf("\n-->[iAP2]IIC Write Data:");
    for (uint16_t i = 0; i < msgs.len; i++) {
        if (i % 20 == 0) {
            rt_kprintf("\n");
        }
        rt_kprintf("%02x ", msgs.buf[i]);
    }
    rt_kprintf("\n");
    rt_kprintf("\n");
#endif

    return RT_EOK;
}

/**
 * _hl_mod_apple_delay_usec
 * @brief 延时函数
 * @param [in] usec 延时时间（微秒）
 * @date 2023-01-12
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-01-12      <td>lisonglin     <td>新建
 * </table>
 */
static void _hl_mod_apple_delay_usec(uint32_t usec)
{
    rt_thread_mdelay(usec / 1000);
}

/**
 * hl_mod_apple_auth_iap2_thread_entry
 * @brief 苹果认证iap2协议流程
 * @param [in] parameter 
 * @date 2022-09-02
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-02      <td>lisonglin     <td>新建
 * </table>
 */
static void hl_mod_apple_auth_iap2_thread_entry(void* parameter)
{
    int result = 0;

    while (RT_TRUE == s_apple.iap2_thread_flag) {
        result = hl_util_apple_oneshot(&s_apple.apple, EM_HL_IAP2);
        if (result >= 0) {
            app_msg_t.cmd = result;
            app_msg_t.len = 0;
            // 上报透传数据
            result = rt_mq_send(s_apple.app_msq, (void*)&app_msg_t, sizeof(app_msg_t));
            // 判断消息队列上传结果
            if (RT_EOK != result) {
                LOG_I("[%s][line:%d](%d)mq_send failed!!! \r\n", __func__, __LINE__, result);
            }
        }
        rt_thread_mdelay(10);
    }
}

/**
 * hl_mod_apple_auth_eap_thread_entry
 * @brief 苹果外部接口通信协议处理流程
 * @param [in] parameter 
 * @date 2022-09-02
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-02      <td>lisonglin     <td>新建
 * </table>
 */
static void hl_mod_apple_auth_eap_thread_entry(void* parameter)
{
    int result = 0;

    while (RT_TRUE == s_apple.eap_thread_flag) {
        result = hl_util_apple_oneshot(&s_apple.apple, EM_HL_EAP);
        if (result >= 0) {
            app_msg_t.cmd = result;
            app_msg_t.len = 0;
            // 上报透传数据
            result = rt_mq_send(s_apple.app_msq, (void*)&app_msg_t, sizeof(app_msg_t));
            // 判断消息队列上传结果
            if (RT_EOK != result) {
                LOG_I("[%s][line:%d](%d)mq_send failed!!! \r\n", __func__, __LINE__, result);
            }
        }
        rt_thread_mdelay(10);
    }
}

int hl_mod_apple_auth_init(rt_mq_t* input_msq)
{
    if (NULL == input_msq) {
        LOG_E("[%s][line:%d]Input_Msq is NULL!!! \r\n", __FUNCTION__, __LINE__, input_msq);
        return 1;
    }
    if (s_apple.init_flag) {
        LOG_E("[%s][line:%d]AppleAuth Mod Inited\r\n", __FUNCTION__, __LINE__);
        return 2;
    }

    uint8_t ret    = 0;
    char*   dev_sn = NULL;

    // 获取并赋值APP层下发的消息队列指针
    s_apple.app_msq          = input_msq;
    s_apple.iap2_thread_flag = RT_TRUE;
    s_apple.eap_thread_flag  = RT_FALSE;
    // Telink消息队列结构体赋初值
    app_msg_t.sender = APPLE_AUTH_MODE;

    // 获取苹果认证芯片I2C设备句柄
    s_apple.mfi_chip_iic = (struct rt_i2c_bus_device*)rt_device_find("i2c1");
    RT_ASSERT(s_apple.mfi_chip_iic);

    // USB通信初始化
    ret = hl_drv_usb_vendor_class_com_init();
    LOG_I("\nhl_drv_usb_vendor_class_com_init ret = %d\n", ret);

    // 获取设备SN码
    dev_sn = s_apple.apple.iap2.dev_sn;
    rt_memset(dev_sn, 0, sizeof(s_apple.apple.iap2.dev_sn));
    strcpy(dev_sn, "xxxxxxxxxxxxxxx");
    if (!hl_util_nvram_param_get("SN", dev_sn, dev_sn, sizeof(s_apple.apple.iap2.dev_sn))) {
        LOG_I("\nMFI Get SN Number: [%s][%d]\n", dev_sn, rt_strlen(dev_sn));
    }

    // 函数注册
    s_apple.func_handle.delay_usec = _hl_mod_apple_delay_usec;
    s_apple.func_handle.usb_read   = _hl_mod_apple_usb_read;
    s_apple.func_handle.usb_write  = _hl_mod_apple_usb_write;
    s_apple.func_handle.iic_read   = _hl_mod_apple_i2c_read;
    s_apple.func_handle.iic_write  = _hl_mod_apple_i2c_write;
    s_apple.func_handle.log        = rt_kprintf;

    hl_util_apple_init(&s_apple.apple, s_apple.func_handle);

    // 置模块开关标志位
    s_apple.init_flag = 1;

    return 0;
}

int hl_mod_apple_auth_deinit()
{
    if (!s_apple.init_flag) {
        LOG_E("[%s][line:%d]Not Init AppleAuth Module, Deinit ERROR!", __FUNCTION__, __LINE__);
        return 1;
    }

    hl_drv_usb_vendor_class_com_deinit();
    hl_util_apple_deinit(&s_apple.apple);

    // 置模块开关标志位
    s_apple.init_flag = 0;

    return 0;
}

int hl_mod_apple_auth_start()
{
    if (s_apple.start_flag) {
        LOG_E("[%s][line:%d]AppleAuth Started\r\n", __FUNCTION__, __LINE__);
        return 1;
    }
    if (!s_apple.init_flag) {
        LOG_E("[%s][line:%d]AppleAuth Not Init, Can't Start AppleAuth Mod!\r\n", __FUNCTION__, __LINE__);
        return 2;
    }

    // 状态机状态初始化
    s_apple.apple.iap2.main_status = EM_HL_IAP2_STM_MAIN_IDLE;

    // 创建线程
    hl_mod_apple_auth_iap2_thread =
        rt_thread_create("apple_auth_iap2", hl_mod_apple_auth_iap2_thread_entry, RT_NULL, IAP2_THREAD_STACK_SIZE,
                         IAP2_THREAD_PRIORITY, IAP2_THREAD_TIMESLICE);
    if (RT_NULL == hl_mod_apple_auth_iap2_thread) {
        LOG_I("hl_mod_apple_auth_iap2_thread create faild!\n");
        return 3;
    }

    hl_mod_apple_auth_eap_thread = rt_thread_create("apple_auth_eap", hl_mod_apple_auth_eap_thread_entry, RT_NULL,
                                                    EAP_THREAD_STACK_SIZE, EAP_THREAD_PRIORITY, EAP_THREAD_TIMESLICE);
    if (RT_NULL == hl_mod_apple_auth_eap_thread) {
        LOG_I("hl_mod_apple_auth_eap_thread create faild!\n");
        return 4;
    }

    // 启动线程
    rt_thread_startup(hl_mod_apple_auth_iap2_thread);
    rt_thread_startup(hl_mod_apple_auth_eap_thread);

    s_apple.start_flag = 1;

    return 0;
}

int hl_mod_apple_auth_stop()
{
    if (!s_apple.start_flag) {
        LOG_E("[%s][line:%d]AppleAuth Not Start, Can't Stop AppleAuth Mod!\r\n", __FUNCTION__, __LINE__);
        return 1;
    }
    rt_err_t result;

    if (RT_NULL == hl_mod_apple_auth_iap2_thread) {
        LOG_I("[%s][line:%d]delete return!!! \r\n", __func__, __LINE__);
        return 0;
    }
    // 脱离Telink线程
    result = rt_thread_delete(hl_mod_apple_auth_iap2_thread);
    if (RT_EOK != result) {
        LOG_I("[%s][line:%d]hl_mod_apple_auth_iap2_thread delete faild!!! \r\n", __func__, __LINE__);
        return 1;
    }

    if (RT_NULL == hl_mod_apple_auth_eap_thread) {
        LOG_I("[%s][line:%d]delete return!!! \r\n", __func__, __LINE__);
        return 0;
    }
    // 脱离Telink线程
    result = rt_thread_delete(hl_mod_apple_auth_eap_thread);
    if (RT_EOK != result) {
        LOG_I("[%s][line:%d]hl_mod_apple_auth_eap_thread delete faild!!! \r\n", __func__, __LINE__);
        return 1;
    }

    s_apple.start_flag = 0;

    return 0;
}

uint8_t hl_mod_appleauth_ioctl(hl_mod_appleauth_ctrl_cmd cmd)
{
    switch (cmd) {
        case HL_APPLE_AUTH_START_CMD:
            LOG_I("\n\n\n*********iAP2 START*********\n\n\n");
            s_apple.apple.iap2.main_status        = EM_HL_IAP2_STM_MAIN_DETECT;
            s_apple.apple.iap2.detect_status      = EM_HL_IAP2_STM_DETECT_SEND;
            s_apple.apple.iap2.link_status        = EM_HL_IAP2_STM_LINK_SEND_SYN;
            s_apple.apple.iap2.identify_status    = EM_HL_IAP2_STM_IDENTIFY_REQ_AUTH;
            s_apple.apple.iap2.powerupdate_status = EM_HL_IAP2_STM_POWERUPDATE_SEND_POWER;
            s_apple.apple.iap2.retry_time         = 3;
            s_apple.apple.packet_arg.seq_num      = 0x00;
            s_apple.apple.packet_arg.ack_num      = 0x2B;
            s_apple.apple.packet_arg.session_id   = 0x00;
            break;

        case HL_APPLE_AUTH_STOP_CMD:
            LOG_I("\n\n\n*********iAP2 STOP*********\n\n\n");
            s_apple.apple.iap2.main_status = EM_HL_IAP2_STM_MAIN_IDLE;
            break;

        default:
            break;
    }

    return 0;
}

static void apple_show_val(void)
{
    LOG_I("\n");
    LOG_I("main_status = %d", s_apple.apple.iap2.main_status);
    LOG_I("detect_status = %d", s_apple.apple.iap2.detect_status);
    LOG_I("link_status = %d", s_apple.apple.iap2.link_status);
    LOG_I("identify_status = %d", s_apple.apple.iap2.identify_status);
    LOG_I("powerupdate_status = %d", s_apple.apple.iap2.powerupdate_status);
    LOG_I("\n");
}
MSH_CMD_EXPORT(apple_show_val, apple show val cmd);
