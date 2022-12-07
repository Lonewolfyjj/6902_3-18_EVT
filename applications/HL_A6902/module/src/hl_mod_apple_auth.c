#include "rtdevice.h"
#include "hl_mod_apple_auth.h"
#include "hl_util_iap2.h"
#include "hl_drv_usb_vendor_class_com.h"

/// IAP2线程句柄
static struct rt_thread hl_mod_apple_auth_iap2_thread;
/// EAP线程句柄
static struct rt_thread hl_mod_apple_auth_eap_thread;

/// IAP2线程栈数组
static char iap2_thread_stack[IAP2_THREAD_STACK_SIZE];
/// EAP线程栈数组
static char eap_thread_stack[EAP_THREAD_STACK_SIZE];

/// iap2 util功能句柄
static st_iap2_protocol_p iap2_handle;
/// iap2 util功能句柄
static func_handle iap2_func_handle;
/// iap2 IIC chips
static struct rt_i2c_bus_device *apple_auth_chip_iic = NULL;

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
    rt_kprintf("%s:%d\r\n", __func__, ret);
    for (i = 0; i < ret; i++) {
        rt_kprintf("%02x ", read_data_addr[i]);
    }
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
    return 0;
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
    size = rt_i2c_transfer(apple_auth_chip_iic, &msgs[0], 1);
    rt_kprintf("i2c read msg[0] = %d \r\n", size);
    while ( size != 1 && test_time--) {
        size = rt_i2c_transfer(apple_auth_chip_iic, &msgs[0], 1);
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
        return RT_ERROR;
    }

    msgs[1].addr  = 0x10;
    msgs[1].flags = RT_I2C_RD | RT_I2C_IGNORE_NACK; // | RT_I2C_NO_READ_ACK
    msgs[1].buf   = read_data_addr;
    msgs[1].len   = read_data_len;

    size = rt_i2c_transfer(apple_auth_chip_iic, &msgs[1], 1);
    rt_kprintf("i2c read msg[1] = %d \r\n", size);
    test_time  = 3;
    while ( size != 1 && test_time--) {
        size = rt_i2c_transfer(apple_auth_chip_iic, &msgs[1], 1);
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
        return RT_ERROR;
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
    
    size = rt_i2c_transfer(apple_auth_chip_iic, &msgs, 1);
    
    rt_kprintf("i2c write msg[0] size = %d \r\n", size);

    while (size != 1) {
        rt_kprintf("i2c write err!\n");
        size = rt_i2c_transfer(apple_auth_chip_iic, &msgs, 1);
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

    return 0;
}

static void _hl_mod_apple_auth_iap2_delay(uint16_t usec)
{
    rt_thread_mdelay(usec / 1000);
    // return 0;
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
    // -----------------
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
    // -----------------
    rt_kprintf("\neap_thread enter\n");
    while (1) {
        // rt_kprintf("\nwhile eap_thread enter\n");
        hl_iap2_process_main_oneshot(iap2_handle);
        rt_thread_mdelay(1);
    }
}

int hl_mod_apple_auth_init()
{
    int i = 5;
    // while(i--) {
    //     rt_thread_mdelay(1000);
    // }
    rt_err_t result;
    int      ret = 0;

    apple_auth_chip_iic = (struct rt_i2c_bus_device *)rt_device_find("i2c1");
    RT_ASSERT(apple_auth_chip_iic);

    ret         = hl_drv_usb_vendor_class_com_init();
    rt_kprintf("\nhl_drv_usb_vendor_class_com_init ret = %d\n", ret);
    iap2_handle = rt_malloc(sizeof(st_iap2_protocol_t));
    rt_memset(iap2_handle, 0, sizeof(st_iap2_protocol_t));

    iap2_func_handle.delay_usec_func = _hl_mod_apple_auth_iap2_delay;
    iap2_func_handle.iap2_usb_read   = _hl_mod_apple_auth_iap2_usb_read;
    iap2_func_handle.iap2_usb_write  = _hl_mod_apple_auth_iap2_usb_write;
    iap2_func_handle.iap2_iic_read   = _hl_mod_apple_auth_iap2_cp_read;
    iap2_func_handle.iap2_iic_write  = _hl_mod_apple_auth_iap2_cp_write;
    iap2_func_handle.iap2_printf     = rt_kprintf;

    hl_iap2_protocol_init(iap2_handle, iap2_func_handle);

    result =
        rt_thread_init(&hl_mod_apple_auth_iap2_thread, "apple_auth_iap2", hl_mod_apple_auth_iap2_thread_entry, RT_NULL,
                       &iap2_thread_stack[0], sizeof(iap2_thread_stack), IAP2_THREAD_PRIORITY, IAP2_THREAD_TIMESLICE);

    result =
        rt_thread_init(&hl_mod_apple_auth_eap_thread, "apple_auth_eap", hl_mod_apple_auth_eap_thread_entry, RT_NULL,
                       &eap_thread_stack[0], sizeof(eap_thread_stack), EAP_THREAD_PRIORITY, EAP_THREAD_TIMESLICE);

    hl_mod_apple_auth_start();

    if (result == RT_EOK) {
        rt_kprintf("hl_mod_apple_auth_thread init succeed!\n");
    } else {
        rt_kprintf("hl_mod_apple_auth_thread init faild!\n");
    }

    return 0;
}

int hl_mod_apple_auth_deinit()
{
    if (iap2_handle) {
        rt_free(iap2_handle);
    }
    return 0;
}

int hl_mod_apple_auth_start()
{
    rt_thread_startup(&hl_mod_apple_auth_iap2_thread);
    rt_thread_startup(&hl_mod_apple_auth_eap_thread);

    return 0;
}

int hl_mod_apple_auth_stop()
{
    return 0;
}

INIT_APP_EXPORT(hl_mod_apple_auth_init);
