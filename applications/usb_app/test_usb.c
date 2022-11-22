#include <rtthread.h>
#include <rtdevice.h>
#include <math.h>
#include <rtdbg.h>
#include <stdio.h>
#include "../../components/drivers/usb/usbdevice/class/hid.h"

#define KEYBOARD_A 0x4
#define KEYBOARD_B 0x5

struct keyboard
{
    char key_value[7];
} keyboard;

static int keyboard_send_key_release(rt_device_t device)
{
    int i;

    RT_ASSERT(device != RT_NULL);

    /* init input date as key_release */
    for (i = 0; i < 7; i++)
        keyboard.key_value[i] = 0;

    rt_device_write(device, 0, keyboard.key_value, 7);
    return 0;
}

static int keyboard_send_key_value(rt_device_t device, int value)
{
    rt_uint8_t report_id;
    RT_ASSERT(device != RT_NULL);
    keyboard.key_value[1] = value;
    report_id = 0x00;

    rt_device_write(device, report_id, keyboard.key_value, 7);
    return 0;
}

static void usb_send_data(rt_device_t device)
{
    RT_ASSERT(device != RT_NULL);
#if 0
    rt_kprintf("start send hid keyboard data\n");
    /* set key value */
    keyboard_send_key_value(device, KEYBOARD_A);
    rt_thread_mdelay(10);
    keyboard_send_key_release(device);
    rt_thread_mdelay(10);
    keyboard_send_key_value(device, KEYBOARD_B);
    rt_thread_mdelay(10);
    keyboard_send_key_release(device);
    rt_thread_mdelay(10);
    rt_kprintf("complete send hid keyboard data\n");
#else 
    rt_uint8_t report_id;
    char key_value[7];
    report_id = 0x05;
    key_value[0] = 0x11;
    key_value[1] = 0x22;
    key_value[2] = 0x33;
    key_value[3] = 0x44;
    key_value[4] = 0x55;
    key_value[5] = 0x66;
    key_value[6] = 0x77;
    rt_device_write(device, report_id, key_value, 7);
    // rt_thread_mdelay(10);
    // rt_device_write(device, report_id, key_value, 1);
    // rt_thread_mdelay(10);
    // rt_device_write(device, report_id, key_value, 1);
    rt_thread_mdelay(200);

    key_value[0] = 0x01;
    key_value[1] = 0x02;
    key_value[2] = 0x03;
    key_value[3] = 0x04;
    key_value[4] = 0x05;
    key_value[5] = 0x06;
    key_value[6] = 0x07;
    rt_device_write(device, report_id, key_value, 7);
    rt_thread_mdelay(10);
    // rt_device_write(device, report_id, key_value, 1);
    // rt_thread_mdelay(10);
    // rt_device_write(device, report_id, key_value, 1);
    // rt_thread_mdelay(10);

#endif
}

ALIGN(4) rt_uint8_t report_buf[64];
static void usb_thread_entry(void *parameter)
{
    rt_device_t device = (rt_device_t)parameter;
    struct urequest setup;
    int i;

    usb_send_data(device);
#if 0
    while (1)
    {
        rt_memset(&setup, 0, sizeof(setup));
        rt_memset(report_buf, 0, 64);
        rt_device_control(device, RK_USB_HID_SETUP_READ, &setup);
        rt_kprintf("setup ");
        for(i = 0; i < 8; i++)
                rt_kprintf("%02x", *(((rt_uint8_t*)&setup) + i));
        rt_kprintf("\n");
        switch (setup.bRequest)
        {
        case USB_HID_REQ_SET_REPORT:
            rt_device_control(device, RK_USB_HID_REPORT_DATA_READ, report_buf);
            rt_kprintf("report data ");
            for(i = 0; i < setup.wLength; i++)
                rt_kprintf("%02x", *(((rt_uint8_t*)report_buf) + i));
            rt_kprintf("\n");
            break;
        case USB_HID_REQ_GET_REPORT:
            rt_device_control(device, RK_USB_HID_REPORT_DATA_WRITE, report_buf);
            break;
        default:
            break;
        }
    }
#endif
    rt_device_close(device);
}

static int hid_test(void)
{
    static rt_thread_t hid_thread = RT_NULL;
    /* find hidd device */
    rt_device_t device = rt_device_find("hidd");

    if (device == RT_NULL) {
        rt_kprintf("err:no find hidd\r\n");
        return -1;
    }

    /* open hidd device */
    rt_device_open(device, RT_DEVICE_FLAG_WRONLY);

    /* create hid dynamic thread */
    hid_thread = rt_thread_create("hid_test", usb_thread_entry, device,
                                  1024, 10, 20);
    rt_thread_startup(hid_thread);
    return 0;
}

// hid_send_test [report_id] [key_value] 
int hid_send_test(int argc, char** argv)
{
    rt_thread_t hid_thread;
    rt_uint8_t report_id;
    char key_value[7];

    if (argc != 3) {
        rt_kprintf("wrong parameter, please type: hid_send_test [report_id] [key_value]  \r\n");
        return -1;
    }

    /* find hidd device */
    rt_device_t device = rt_device_find("hidd");

    if (device == RT_NULL) {
        rt_kprintf("err:no find hidd\r\n");
        return -1;
    }

    /* open hidd device */
    rt_device_open(device, RT_DEVICE_FLAG_WRONLY);

    report_id    = atoi(argv[1]);
    key_value[0] = atoi(argv[2]);
    rt_device_write(device, report_id, key_value, 1);
    rt_thread_mdelay(10);
    
    key_value[0] = 0x00;
    rt_device_write(device, report_id, key_value, 1);
    rt_thread_mdelay(10);

    rt_device_close(device);
    return 0;
}

MSH_CMD_EXPORT(hid_test, hid in test)
MSH_CMD_EXPORT(hid_send_test, hid send test)
