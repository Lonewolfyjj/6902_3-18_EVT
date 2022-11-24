#include <rtthread.h>
#include <rtdevice.h>
#include <math.h>
#include <rtdbg.h>
#include <stdio.h>
#include "../../components/drivers/usb/usbdevice/class/hid.h"

ALIGN(4) rt_uint8_t report_send_buf[70];
ALIGN(4) rt_uint8_t report_receive_buf[64];
static rt_thread_t hid_thread = RT_NULL;

static void hid_thread_entry(void *parameter)
{
    rt_uint8_t  size,i;
    /* find hidd device */
    rt_device_t device = rt_device_find("hidd");

    if (device == RT_NULL) {
        rt_kprintf("err:no find hidd\r\n");
        hid_thread = RT_NULL;
        return -1;
    }

    /* open hidd device */
    rt_device_open(device, RT_DEVICE_FLAG_RDWR);

    for (i = 0; i < 70; i++) {
        report_send_buf[i] = i;
    }
    rt_device_write(device, 0, report_send_buf, 70);
    rt_device_write(device, 0, report_send_buf, 20);
    while (1) {
        size = rt_device_read(device, 0, report_receive_buf, 64);
        if (size > 0) {
            rt_kprintf("hid receive:\r\n");
            for (i = 0; i < size; i++) {
                rt_kprintf("0x%02x ", report_receive_buf[i]);
                if (i%8 == 7) {
                    rt_kprintf("\r\n");
                }
            }
        }
        rt_thread_mdelay(10);
    }

    rt_device_close(device);
}

// hid_send_test [report_id] [key_value] 
int hid_key_test(int argc, char** argv)
{
    rt_uint8_t report_id;
    rt_uint8_t key_value;
    rt_uint8_t buff[2] = {0};

    rt_uint8_t i;
    rt_uint8_t len;
    char *endptr;

    if (argc != 3) {
        rt_kprintf("wrong parameter, please type: hid_send_test [report_id] [key_value]  \r\n");
        return -1;
    }

    for (i = 0; i < (argc - 1); i++) {
        buff[i] = strtoul(argv[i+1], &endptr, 0);
        if (*endptr) {
            rt_kprintf("Bad <addr> value '%s'\n", argv[i]);
            return 0;
        }
    }
    report_id = buff[0];
    key_value = buff[1];

    /* find hidd device */
    rt_device_t device = rt_device_find("hidd");

    if (device == RT_NULL) {
        rt_kprintf("err:no find hidd\r\n");
        return -1;
    }

    /* open hidd device */
    rt_device_open(device, RT_DEVICE_FLAG_WRONLY);

    // 发送按键按下
    rt_device_write(device, 0, buff, 2);
    // rt_thread_mdelay(10);
    // rt_kprintf("hid key send 0x%x 0x%x\n", report_id, key_value);
    
    // 发送按键释放
    buff[1] = 0x00;
    rt_device_write(device, 0, buff, 2);
    // rt_thread_mdelay(10);
    // rt_kprintf("hid key send 0x%x 0x%x\n", report_id, key_value);

    rt_device_close(device);
    return 0;
}

// hid_send_test data[0] data[1].. data[n]     n <= 64
int hid_send_test(int argc, char** argv)
{
    rt_uint8_t i;
    char *endptr;

    if (argc <= 1) {
        rt_kprintf("wrong parameter, please type: hid_send_test data[0] data[1].. data[n](n <= 64)\r\n");
        return -1;
    }

    for (i = 0; i < (argc - 1); i++) {
        report_send_buf[i] = strtoul(argv[i+1], &endptr, 0);
        if (*endptr) {
            rt_kprintf("Bad <addr> value '%s'\n", argv[i]);
            return 0;
        }
    }

    /* find hidd device */
    rt_device_t device = rt_device_find("hidd");

    if (device == RT_NULL) {
        rt_kprintf("err:no find hidd\r\n");
        return -1;
    }

    /* open hidd device */
    rt_device_open(device, RT_DEVICE_FLAG_WRONLY);

    // 发送按键按下
    rt_device_write(device, report_send_buf[0], &report_send_buf[1], (argc - 2));
    rt_thread_mdelay(10);
    rt_kprintf("hid send data:\n");
    for (i = 0; i < (argc - 1); i++) {
        rt_kprintf("0x%x ", report_send_buf[i]);
        if ((i%8) == 0) {
            rt_kprintf("\n");
        }
    }
    
    rt_device_close(device);
    return 0;
}

// hid_receive_test
int hid_receive_test(int argc, char** argv)
{
    if (argc != 1) {
        rt_kprintf("wrong parameter, please type: hid_receive_test \r\n");
        return -1;
    }

    if (hid_thread != RT_NULL) {
        rt_kprintf("hid_rec is ready\r\n");
        return 0;
    }

    hid_thread = rt_thread_create("hid_rec", hid_thread_entry, RT_NULL, 1024, 20, 1);
    if (hid_thread) {
        rt_thread_startup(hid_thread);
    }
    return 0;
}

MSH_CMD_EXPORT(hid_key_test, hid in test)
MSH_CMD_EXPORT(hid_send_test, hid send test)
MSH_CMD_EXPORT(hid_receive_test, hid receive test)
