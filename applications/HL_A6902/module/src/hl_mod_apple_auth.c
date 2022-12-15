#include "hl_mod_apple_auth.h"
#include "hl_drv_usb_vendor_class_com.h"

/// IAP2线程句柄
static rt_thread_t hl_mod_apple_auth_iap2_thread;
/// EAP线程句柄
static rt_thread_t hl_mod_apple_auth_eap_thread;
/// appleauth控制句柄
hl_mod_apple_auth_t s_apple_auth;

static int _hl_mod_apple_auth_iap2_usb_read(uint8_t* read_data_addr, uint16_t read_data_len, uint16_t timeout)
{
    int     index = 0;
    uint8_t ret   = 0;
    int     i     = 0;
    if (read_data_len > 64) {
        for (i = 0; i < read_data_len / 64; i++) {
            ret += hl_drv_usb_vendor_class_com_read(read_data_addr + (i * 64), 64, timeout);
        }
    } else {
        ret += hl_drv_usb_vendor_class_com_read(read_data_addr, read_data_len, timeout);
    }
    rt_kprintf("%s:%d\n", __func__, ret);
    for (i = 0; i < ret; i++) {
        rt_kprintf("%02x ", read_data_addr[i]);
    }
    rt_kprintf("\n");
    return ret;
}

static int _hl_mod_apple_auth_iap2_usb_write(uint8_t* write_data_addr, uint16_t write_data_len)
{
    uint8_t ret = 0;
    if (write_data_len > 64) {
        uint8_t len = 64;
        int     i   = 0;
        uint8_t len_left = write_data_len % 64 == 0 ? 0 : 1;
        for (i = 0; i < (write_data_len / 64) + len_left; i++) {
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
    rt_kprintf("%s:%d\r\n", __func__, write_data_len);
    return RT_EOK;
}

static int _hl_mod_apple_auth_iap2_cp_read(uint8_t reg_addr, uint8_t* read_data_addr, uint16_t read_data_len,
                                           uint16_t timeout)
{
    struct rt_i2c_msg msgs[2] = { 0 };
    rt_size_t size = 0;
    uint8_t test_time = 3;

    msgs[0].addr  = 0x10;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &reg_addr;
    msgs[0].len   = 1;
    size = rt_i2c_transfer(s_apple_auth.mfi_chip_iic, &msgs[0], 1);
    rt_kprintf("i2c read msg[0] = %d \r\n", size);
    while ( size != 1 && test_time--) {
        size = rt_i2c_transfer(s_apple_auth.mfi_chip_iic, &msgs[0], 1);
        rt_kprintf("i2c read msg[0] = %d \r\n", size);
    }

    if ( size == 1) {

        int i = 0;
        for (size_t i = 0; i < msgs[0].len; i++)
        {
            rt_kprintf("%02x ", msgs[0].buf[i]);
        }
         
        // return RT_EOK;
    } else {
        rt_kprintf("i2c read err!\n");
        return 0;
    }

    msgs[1].addr  = 0x10;
    msgs[1].flags = RT_I2C_RD | RT_I2C_IGNORE_NACK; // | RT_I2C_NO_READ_ACK
    msgs[1].buf   = read_data_addr;
    msgs[1].len   = read_data_len;

    size = rt_i2c_transfer(s_apple_auth.mfi_chip_iic, &msgs[1], 1);
    rt_kprintf("i2c read msg[1] = %d \r\n", size);
    test_time  = 3;
    while ( size != 1 && test_time--) {
        size = rt_i2c_transfer(s_apple_auth.mfi_chip_iic, &msgs[1], 1);
        rt_kprintf("i2c read msg[1] = %d \r\n", size);
    } 
    
    if (size == 1){
        rt_kprintf("\r\n-->iic read data:[");
        for (uint16_t i = 0; i < read_data_len; i++)
        {
            rt_kprintf("%02x ", read_data_addr[i]);
        }
        rt_kprintf("]\r\n");
         
        return read_data_len;
    }else {
        rt_kprintf("i2c read err!\n");
        return 0;
    }
    
    return 0;
}

static int _hl_mod_apple_auth_iap2_cp_write(uint8_t reg_addr, uint8_t* write_data_addr, uint16_t write_data_len)
{
    uint8_t buffer[128] = { 0 };
    buffer[0] = reg_addr;
    memcpy(buffer + 1, write_data_addr, write_data_len);

    struct rt_i2c_msg msgs = { 0 };
    rt_size_t size = 0;

    msgs.addr  = 0x10;
    msgs.flags = RT_I2C_WR;
    msgs.buf   = buffer;
    msgs.len   = write_data_len + 1;
    
    size = rt_i2c_transfer(s_apple_auth.mfi_chip_iic, &msgs, 1);
    
    rt_kprintf("i2c write msg[0] size = %d \r\n", size);

    while (size != 1) {
        rt_kprintf("i2c write err!\n");
        size = rt_i2c_transfer(s_apple_auth.mfi_chip_iic, &msgs, 1);
    }

    if ( size == 1) {
        for (uint16_t i = 0; i < msgs.len; i++)
        {
            rt_kprintf("%02x ", msgs.buf[i]);
        }
        return RT_EOK;
    } else {
        rt_kprintf("i2c read err!\n");
        return RT_ERROR;
    }

    return RT_ERROR;
}

static void _hl_mod_apple_auth_iap2_delay(uint16_t usec)
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

    rt_kprintf("\n---> eap_thread enter!\n");
    rt_thread_mdelay(200);

    while (s_apple_auth.eap_thread_flag == RT_TRUE) {
        result = hl_iap2_process_main_oneshot(s_apple_auth.iap2_handle);
        rt_thread_mdelay(1);
    }

    rt_kprintf("\n---> eap_thread quit!\n");
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
}

int hl_mod_apple_auth_init(rt_mq_t* input_msq)
{
    if (NULL == input_msq) {
        rt_kprintf("[%s][line:%d] cmd(%d) unkown!!! \r\n", __FUNCTION__, __LINE__, input_msq);
        return 1;
    }

    uint8_t ret = 0;

    // 获取并赋值APP层下发的消息队列指针
    s_apple_auth.app_msq          = input_msq;
    s_apple_auth.iap2_thread_flag = RT_TRUE;
    s_apple_auth.eap_thread_flag  = RT_TRUE;

    // 获取苹果认证芯片I2C设备句柄
    s_apple_auth.mfi_chip_iic = (struct rt_i2c_bus_device*)rt_device_find("i2c1");
    RT_ASSERT(s_apple_auth.mfi_chip_iic);

    // USB通信初始化
    ret = hl_drv_usb_vendor_class_com_init();
    rt_kprintf("\nhl_drv_usb_vendor_class_com_init ret = %d\n", ret);

    // 初始化结构体
    s_apple_auth.iap2_handle = rt_malloc(sizeof(st_iap2_protocol_t));
    rt_memset(s_apple_auth.iap2_handle, 0, sizeof(st_iap2_protocol_t));

    // 函数注册
    s_apple_auth.iap2_func_handle.delay_usec_func = _hl_mod_apple_auth_iap2_delay;
    s_apple_auth.iap2_func_handle.iap2_usb_read   = _hl_mod_apple_auth_iap2_usb_read;
    s_apple_auth.iap2_func_handle.iap2_usb_write  = _hl_mod_apple_auth_iap2_usb_write;
    s_apple_auth.iap2_func_handle.iap2_iic_read   = _hl_mod_apple_auth_iap2_cp_read;
    s_apple_auth.iap2_func_handle.iap2_iic_write  = _hl_mod_apple_auth_iap2_cp_write;
    s_apple_auth.iap2_func_handle.iap2_printf     = rt_kprintf;

    hl_iap2_protocol_init(s_apple_auth.iap2_handle, s_apple_auth.iap2_func_handle);

    return 0;
}

int hl_mod_apple_auth_deinit()
{
    if (s_apple_auth.iap2_handle) {
        rt_free(s_apple_auth.iap2_handle);
    }

    return 0;
}

int hl_mod_apple_auth_start()
{
    rt_err_t result;

    // 状态机状态初始化
    s_apple_auth.iap2_handle->main_status        = EM_HL_IAP2_STM_MAIN_DETECT;
    s_apple_auth.iap2_handle->detect_status      = EM_HL_IAP2_STM_DETECT_SEND;
    s_apple_auth.iap2_handle->link_status        = EM_HL_IAP2_STM_LINK_SEND_SYN;
    s_apple_auth.iap2_handle->identify_status    = EM_HL_IAP2_STM_IDENTIFY_REQ_AUTH;
    s_apple_auth.iap2_handle->powerupdate_status = EM_HL_IAP2_STM_POWERUPDATE_SEND_POWER;

    // 创建线程
    hl_mod_apple_auth_iap2_thread =
        rt_thread_create("apple_auth_iap2", hl_mod_apple_auth_iap2_thread_entry, RT_NULL, IAP2_THREAD_STACK_SIZE,
                         IAP2_THREAD_PRIORITY, IAP2_THREAD_TIMESLICE);
    if (RT_NULL == hl_mod_apple_auth_iap2_thread) {
        rt_kprintf("hl_mod_apple_auth_iap2_thread create faild!\n");
        return 1;
    }

    hl_mod_apple_auth_eap_thread = rt_thread_create("apple_auth_eap", hl_mod_apple_auth_eap_thread_entry, RT_NULL,
                                                    EAP_THREAD_STACK_SIZE, EAP_THREAD_PRIORITY, EAP_THREAD_TIMESLICE);
    if (RT_NULL == hl_mod_apple_auth_eap_thread) {
        rt_kprintf("hl_mod_apple_auth_eap_thread create faild!\n");
        return 1;
    }

    // 启动线程
    rt_thread_startup(hl_mod_apple_auth_iap2_thread);
    rt_thread_startup(hl_mod_apple_auth_eap_thread);

    return 0;
}

int hl_mod_apple_auth_stop()
{
    rt_err_t result;

    if(RT_NULL == hl_mod_apple_auth_iap2_thread) {
        rt_kprintf("[%s][line:%d]delete return!!! \r\n", __FUNCTION__, __LINE__);
        return 0;
    }
    // 脱离Telink线程
    result = rt_thread_delete(hl_mod_apple_auth_iap2_thread);
    if (RT_EOK != result) {
        rt_kprintf("[%s][line:%d]hl_mod_apple_auth_iap2_thread detach faild!!! \r\n", __FUNCTION__, __LINE__);
        return 1;
    }

    return 0;
}

void hl_mod_apple_auth_begin()
{
    s_apple_auth.iap2_handle->main_status = EM_HL_IAP2_STM_MAIN_DETECT;
}

void hl_mod_apple_auth_end()
{
    s_apple_auth.iap2_handle->main_status = EM_HL_IAP2_STM_MAIN_IDLE;
}
