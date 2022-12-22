/*
 * File      : hid.c
 * COPYRIGHT (C) 2008 - 2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Change Logs:
 * Date           Author       Notes
 * 2017-03-13     Urey         the first version
 * 2017-11-16     ZYH          Update to common hid
 */

#include <rthw.h>
#include <rtthread.h>
#include <rtservice.h>
#include <rtdevice.h>

#include "drivers/usb_common.h"
#include "drivers/usb_device.h"

#include "hid.h"
#include "dma.h"
#include <ipc/ringbuffer.h>
#include "hl_config.h"
#include "hl_util_nvram.h"

#ifdef RT_USB_DEVICE_HID

struct hid_s
{
    // struct rt_device parent;
    struct ufunction *func;
    uep_t ep_in;
    uep_t ep_out;
    int status;
    ALIGN(4) rt_uint16_t protocol;
    ALIGN(4) rt_uint8_t report_buf[MAX_REPORT_SIZE];
    ALIGN(4) rt_uint8_t idle;
    // struct rt_messagequeue hid_mq;
};

struct usb_hid_node
{
    rt_uint8_t buffer[64];
    rt_uint8_t size;
    rt_list_t  list;
};

/// hid设备结构体
struct _hl_hid_dev
{
    /// hid已经创建
    uint8_t    active;
    /// hid写互斥量的指针
    rt_mutex_t write_mutex;
    /// hid 发送空闲链表
    rt_list_t free_list;
    /// hid 就绪待发送链表
    rt_list_t ready_list;
    /// hid数据接收ringbuff
    struct rt_ringbuffer* receive_rb;
    /// hid结构体指针
    struct hid_s *hid_s;
    /// hidd 设备本体               
    struct rt_device  parent;     
};

/// hid rx buff size
#define HID_REC_BUFF_SIZE   512
/// hid tx list node size
#define HID_TX_NODE_SIZE    10

/// hid设备结构体
volatile static struct _hl_hid_dev hid_dev = {0};

ALIGN(4) static struct uhid_descriptor hid_desc_buf;

static char usb_sn[100] = {0};

/* CustomHID_ConfigDescriptor */
ALIGN(4)
const rt_uint8_t _report_desc[]=
{
#ifdef RT_USB_DEVICE_HID_KEYBOARD
    USAGE_PAGE(1),      0x01,
    USAGE(1),           0x06,
    COLLECTION(1),      0x01,
    REPORT_ID(1),       HID_REPORT_ID_KEYBOARD1,

    USAGE_PAGE(1),      0x07,
    USAGE_MINIMUM(1),   0xE0,
    USAGE_MAXIMUM(1),   0xE7,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_SIZE(1),     0x01,
    REPORT_COUNT(1),    0x08,
    INPUT(1),           0x02,
    REPORT_COUNT(1),    0x01,
    REPORT_SIZE(1),     0x08,
    INPUT(1),           0x01,


    REPORT_COUNT(1),    0x05,
    REPORT_SIZE(1),     0x01,
    USAGE_PAGE(1),      0x08,
    USAGE_MINIMUM(1),   0x01,
    USAGE_MAXIMUM(1),   0x05,
    OUTPUT(1),          0x02,
    REPORT_COUNT(1),    0x01,
    REPORT_SIZE(1),     0x03,
    OUTPUT(1),          0x01,


    REPORT_COUNT(1),    0x06,
    REPORT_SIZE(1),     0x08,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x65,
    USAGE_PAGE(1),      0x07,
    USAGE_MINIMUM(1),   0x00,
    USAGE_MAXIMUM(1),   0x65,
    INPUT(1),           0x00,
    END_COLLECTION(0),
#if RT_USB_DEVICE_HID_KEYBOARD_NUMBER>1
    /****keyboard2*****/
    USAGE_PAGE(1),      0x01,
    USAGE(1),           0x06,
    COLLECTION(1),      0x01,
    REPORT_ID(1),       HID_REPORT_ID_KEYBOARD2,

    USAGE_PAGE(1),      0x07,
    USAGE_MINIMUM(1),   0xE0,
    USAGE_MAXIMUM(1),   0xE7,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_SIZE(1),     0x01,
    REPORT_COUNT(1),    0x08,
    INPUT(1),           0x02,
    REPORT_COUNT(1),    0x01,
    REPORT_SIZE(1),     0x08,
    INPUT(1),           0x01,

    REPORT_COUNT(1),    0x06,
    REPORT_SIZE(1),     0x08,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x65,
    USAGE_PAGE(1),      0x07,
    USAGE_MINIMUM(1),   0x00,
    USAGE_MAXIMUM(1),   0x65,
    INPUT(1),           0x00,
    END_COLLECTION(0),
#if RT_USB_DEVICE_HID_KEYBOARD_NUMBER>2
    USAGE_PAGE(1),      0x01,
    USAGE(1),           0x06,
    COLLECTION(1),      0x01,
    REPORT_ID(1),       HID_REPORT_ID_KEYBOARD3,

    USAGE_PAGE(1),      0x07,
    USAGE_MINIMUM(1),   0xE0,
    USAGE_MAXIMUM(1),   0xE7,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_SIZE(1),     0x01,
    REPORT_COUNT(1),    0x08,
    INPUT(1),           0x02,
    REPORT_COUNT(1),    0x01,
    REPORT_SIZE(1),     0x08,
    INPUT(1),           0x01,

    REPORT_COUNT(1),    0x06,
    REPORT_SIZE(1),     0x08,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x65,
    USAGE_PAGE(1),      0x07,
    USAGE_MINIMUM(1),   0x00,
    USAGE_MAXIMUM(1),   0x65,
    INPUT(1),           0x00,
    END_COLLECTION(0),
#if RT_USB_DEVICE_HID_KEYBOARD_NUMBER>3
    USAGE_PAGE(1),      0x01,
    USAGE(1),           0x06,
    COLLECTION(1),      0x01,
    REPORT_ID(1),       HID_REPORT_ID_KEYBOARD4,

    USAGE_PAGE(1),      0x07,
    USAGE_MINIMUM(1),   0xE0,
    USAGE_MAXIMUM(1),   0xE7,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_SIZE(1),     0x01,
    REPORT_COUNT(1),    0x08,
    INPUT(1),           0x02,
    REPORT_COUNT(1),    0x01,
    REPORT_SIZE(1),     0x08,
    INPUT(1),           0x01,

    REPORT_COUNT(1),    0x06,
    REPORT_SIZE(1),     0x08,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x65,
    USAGE_PAGE(1),      0x07,
    USAGE_MINIMUM(1),   0x00,
    USAGE_MAXIMUM(1),   0x65,
    INPUT(1),           0x00,
    END_COLLECTION(0),
#endif
#endif
#endif
#endif
    // Media Control
#ifdef RT_USB_DEVICE_HID_MEDIA
    USAGE_PAGE(1),      0x0C,
    USAGE(1),           0x01,
    COLLECTION(1),      0x01,
    REPORT_ID(1),       HID_REPORT_ID_MEDIA,
    USAGE_PAGE(1),      0x0C,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_SIZE(1),     0x01,
    REPORT_COUNT(1),    0x07,
    USAGE(1),           0xB5,             // Next Track
    USAGE(1),           0xB6,             // Previous Track
    USAGE(1),           0xB7,             // Stop
    USAGE(1),           0xCD,             // Play / Pause
    USAGE(1),           0xE2,             // Mute
    USAGE(1),           0xE9,             // Volume Up
    USAGE(1),           0xEA,             // Volume Down
    INPUT(1),           0x02,             // Input (Data, Variable, Absolute)
    REPORT_COUNT(1),    0x01,
    INPUT(1),           0x01,
    END_COLLECTION(0),
#endif

#ifdef RT_USB_DEVICE_HID_GENERAL
    USAGE_PAGE(1),      0x8c,
    USAGE(1),           0x01,
    COLLECTION(1),      0x01,
    REPORT_ID(1),       HID_REPORT_ID_GENERAL,

    REPORT_COUNT(1),    RT_USB_DEVICE_HID_GENERAL_IN_REPORT_LENGTH,
    USAGE(1),           0x03,
    REPORT_SIZE(1),     0x08,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0xFF,
    INPUT(1),           0x02,

    REPORT_COUNT(1),    RT_USB_DEVICE_HID_GENERAL_OUT_REPORT_LENGTH,
    USAGE(1),           0x04,
    REPORT_SIZE(1),     0x08,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0xFF,
    OUTPUT(1),          0x02,
    END_COLLECTION(0),
#endif
#ifdef RT_USB_DEVICE_HID_MOUSE
    USAGE_PAGE(1),      0x01,           // Generic Desktop
    USAGE(1),           0x02,           // Mouse
    COLLECTION(1),      0x01,           // Application
    USAGE(1),           0x01,           // Pointer
    COLLECTION(1),      0x00,           // Physical
    REPORT_ID(1),       HID_REPORT_ID_MOUSE,
    REPORT_COUNT(1),    0x03,
    REPORT_SIZE(1),     0x01,
    USAGE_PAGE(1),      0x09,           // Buttons
    USAGE_MINIMUM(1),   0x1,
    USAGE_MAXIMUM(1),   0x3,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    INPUT(1),           0x02,
    REPORT_COUNT(1),    0x01,
    REPORT_SIZE(1),     0x05,
    INPUT(1),           0x01,
    REPORT_COUNT(1),    0x03,
    REPORT_SIZE(1),     0x08,
    USAGE_PAGE(1),      0x01,
    USAGE(1),           0x30,           // X
    USAGE(1),           0x31,           // Y
    USAGE(1),           0x38,           // scroll
    LOGICAL_MINIMUM(1), 0x81,
    LOGICAL_MAXIMUM(1), 0x7f,
    INPUT(1),           0x06,
    END_COLLECTION(0),
    END_COLLECTION(0),
#endif
}; /* CustomHID_ReportDescriptor */

ALIGN(4)
static struct udevice_descriptor _dev_desc =
{
    USB_DESC_LENGTH_DEVICE,     //bLength;
    USB_DESC_TYPE_DEVICE,       //type;
    USB_BCD_VERSION,            //bcdUSB;
    USB_CLASS_HID,              //bDeviceClass;
    0x00,                       //bDeviceSubClass;
    0x00,                       //bDeviceProtocol;
    64,                         //bMaxPacketSize0;
    _VENDOR_ID,                 //idVendor;
    _PRODUCT_ID,                //idProduct;
    USB_BCD_DEVICE,             //bcdDevice;
    USB_STRING_MANU_INDEX,      //iManufacturer;
    USB_STRING_PRODUCT_INDEX,   //iProduct;
    USB_STRING_SERIAL_INDEX,    //iSerialNumber;
    USB_DYNAMIC,                //bNumConfigurations;
};

//FS and HS needed
ALIGN(4)
static struct usb_qualifier_descriptor dev_qualifier =
{
    sizeof(dev_qualifier),          //bLength
    USB_DESC_TYPE_DEVICEQUALIFIER,  //bDescriptorType
    0x0200,                         //bcdUSB
    USB_CLASS_MASS_STORAGE,         //bDeviceClass
    0x06,                           //bDeviceSubClass
    0x50,                           //bDeviceProtocol
    64,                             //bMaxPacketSize0
    0x01,                           //bNumConfigurations
    0,
};


/* hid interface descriptor */
ALIGN(4)
const static struct uhid_comm_descriptor _hid_comm_desc =
{
#ifdef RT_USB_DEVICE_COMPOSITE
    /* Interface Association Descriptor */
    {
        USB_DESC_LENGTH_IAD,
        USB_DESC_TYPE_IAD,
        USB_DYNAMIC,
        0x01,
        0x03,                       /* bInterfaceClass: HID */
#if defined(RT_USB_DEVICE_HID_KEYBOARD)||defined(RT_USB_DEVICE_HID_MOUSE)
        USB_HID_SUBCLASS_BOOT,    /* bInterfaceSubClass : 1=BOOT, 0=no boot */
#else
        USB_HID_SUBCLASS_NOBOOT,    /* bInterfaceSubClass : 1=BOOT, 0=no boot */
#endif
#if !defined(RT_USB_DEVICE_HID_KEYBOARD)||!defined(RT_USB_DEVICE_HID_MOUSE)||!defined(RT_USB_DEVICE_HID_MEDIA)
        USB_HID_PROTOCOL_NONE,      /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
#elif !defined(RT_USB_DEVICE_HID_MOUSE)
        USB_HID_PROTOCOL_KEYBOARD,  /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
#else
        USB_HID_PROTOCOL_MOUSE,     /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
#endif
        0x00,
    },
#endif
    /* Interface Descriptor */
    {
        USB_DESC_LENGTH_INTERFACE,
        USB_DESC_TYPE_INTERFACE,
        USB_DYNAMIC,                /* bInterfaceNumber: Number of Interface */
        0x00,                       /* bAlternateSetting: Alternate setting */
        0x02,                       /* bNumEndpoints */
        0x03,                       /* bInterfaceClass: HID */
#if defined(RT_USB_DEVICE_HID_KEYBOARD)||defined(RT_USB_DEVICE_HID_MOUSE)
        USB_HID_SUBCLASS_BOOT,    /* bInterfaceSubClass : 1=BOOT, 0=no boot */
#else
        USB_HID_SUBCLASS_NOBOOT,    /* bInterfaceSubClass : 1=BOOT, 0=no boot */
#endif
#if !defined(RT_USB_DEVICE_HID_KEYBOARD) && !defined(RT_USB_DEVICE_HID_MOUSE) && !defined(RT_USB_DEVICE_HID_MEDIA)
        USB_HID_PROTOCOL_NONE,      /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
#elif !defined(RT_USB_DEVICE_HID_MOUSE)
        USB_HID_PROTOCOL_KEYBOARD,  /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
#else
        USB_HID_PROTOCOL_MOUSE,     /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
#endif
        0,                          /* iInterface: Index of string descriptor */
    },

    /* HID Descriptor */
    {
        HID_DESCRIPTOR_SIZE,        /* bLength: HID Descriptor size */
        HID_DESCRIPTOR_TYPE,        /* bDescriptorType: HID */
        0x0110,                     /* bcdHID: HID Class Spec release number */
        0x00,                       /* bCountryCode: Hardware target country */
        0x01,                       /* bNumDescriptors: Number of HID class descriptors to follow */
        {
            {
                0x22,                       /* bDescriptorType */
                sizeof(_report_desc),       /* wItemLength: Total length of Report descriptor */
            },
        },
    },

    /* Endpoint Descriptor IN */
    {
        USB_DESC_LENGTH_ENDPOINT,
        USB_DESC_TYPE_ENDPOINT,
        USB_DYNAMIC | USB_DIR_IN,
        USB_EP_ATTR_INT,
        0x40,
        0x01,
    },

    /* Endpoint Descriptor OUT */
    {
        USB_DESC_LENGTH_ENDPOINT,
        USB_DESC_TYPE_ENDPOINT,
        USB_DYNAMIC | USB_DIR_OUT,
        USB_EP_ATTR_INT,
        0x40,
        0x01,
    },
};

ALIGN(4)
const static char* _ustring[] =
{
    "Language",
    "Shenzhen Hollyland technology Co.,LTD",
#if HL_IS_TX_DEVICE()
    "Wireless Microphone Tx",
#else
    "Wireless Microphone Rx",
#endif
    usb_sn,
    "Configuration",
    "Interface",
    "iAP Interface",
};

// static void dump_data(uint8_t *data, rt_size_t size)
// {
//     rt_size_t i;
//     for (i = 0; i < size; i++)
//     {
//         rt_kprintf("%02x ", *data++);
//         if ((i + 1) % 8 == 0)
//         {
//             rt_kprintf("\n");
//         }else if ((i + 1) % 4 == 0){
//             rt_kprintf(" ");
//         }
//     }
// }
// static void dump_report(struct hid_report * report)
// {
//     rt_kprintf("\nHID Recived:");
//     rt_kprintf("\nReport ID %02x \n", report->report_id);
//     dump_data(report->report,report->size);
// }

static rt_err_t _ep_out_handler(ufunction_t func, rt_size_t size)
{
    struct hid_s *data;
    // struct hid_report report;
    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL);
    data = (struct hid_s *) func->user_data;

    if(size != 0)
    {
        // rt_memcpy((void *)&report,(void*)data->ep_out->buffer,size);
        // report.size = size-1;
        // rt_mq_send(&data->hid_mq,(void *)&report,sizeof(report));
        rt_ringbuffer_put_force(hid_dev.receive_rb, (void*)data->ep_out->buffer, size);
    }

    data->ep_out->request.buffer = data->ep_out->buffer;
    data->ep_out->request.size = EP_MAXPACKET(data->ep_out);
    data->ep_out->request.req_type = UIO_REQUEST_READ_BEST;
    rt_usbd_io_request(func->device, data->ep_out, &data->ep_out->request);
    return RT_EOK;
}

static rt_err_t _ep_in_handler(ufunction_t func, rt_size_t size)
{
    // struct hid_s *data;
    // RT_ASSERT(func != RT_NULL);
    // RT_ASSERT(func->device != RT_NULL);

    // data = (struct hid_s *) func->user_data;
    // if(data->parent.tx_complete != RT_NULL)
    // {
    //     data->parent.tx_complete(&data->parent,RT_NULL);
    // }
    if(hid_dev.parent.tx_complete != RT_NULL)
    {
        hid_dev.parent.tx_complete(&hid_dev.parent,RT_NULL);
    }

    return RT_EOK;
}

static rt_err_t _hid_set_report_callback(udevice_t device, rt_size_t size)
{
    RT_DEBUG_LOG(RT_DEBUG_USB, ("_hid_set_report_callback\n"));

    if(size != 0)
    {
    }

    dcd_ep0_send_status(device->dcd);

    return RT_EOK;
}

/**
 * This function will handle hid interface bRequest.
 *
 * @param device the usb device object.
 * @param setup the setup bRequest.
 *
 * @return RT_EOK on successful.
 */
static rt_err_t _interface_handler(ufunction_t func, ureq_t setup)
{
    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL);
    RT_ASSERT(setup != RT_NULL);

    struct hid_s *data = (struct hid_s *) func->user_data;
    struct usb_hid_node *p_node;

    switch (setup->bRequest)
    {
    case USB_REQ_GET_DESCRIPTOR:
        if((setup->wValue >> 8) == USB_DESC_TYPE_REPORT)
        {
            rt_usbd_ep0_write(func->device, (void *)(&_report_desc[0]), sizeof(_report_desc));
        }
        else if((setup->wValue >> 8) == USB_DESC_TYPE_HID)
        {
            rt_memcpy((void *)&hid_desc_buf, (void*)(&_hid_comm_desc.hid_desc), sizeof(struct uhid_descriptor));
            rt_usbd_ep0_write(func->device, (void *)&hid_desc_buf, sizeof(struct uhid_descriptor));
        }
        // 清空写链表
        while (!rt_list_isempty(&hid_dev.ready_list))
        {
            p_node = rt_list_first_entry(&hid_dev.ready_list, struct usb_hid_node, list);
            rt_list_remove(&p_node->list);
            rt_list_insert_before(&hid_dev.free_list, &p_node->list);
        }
        break;
    case USB_HID_REQ_GET_REPORT:
        if(setup->wLength == 0)
        {
            rt_usbd_ep0_set_stall(func->device);
            break;
        }
        if((setup->wLength == 0) || (setup->wLength > MAX_REPORT_SIZE))
            setup->wLength = MAX_REPORT_SIZE;
        rt_usbd_ep0_write(func->device, data->report_buf,setup->wLength);

        break;
    case USB_HID_REQ_GET_IDLE:
        rt_usbd_ep0_write(func->device, &data->idle, 1);
        break;
    case USB_HID_REQ_GET_PROTOCOL:
        rt_usbd_ep0_write(func->device, &data->protocol,2);
        break;
    case USB_HID_REQ_SET_REPORT:

        if((setup->wLength == 0) || (setup->wLength > MAX_REPORT_SIZE))
            rt_usbd_ep0_set_stall(func->device);

        rt_usbd_ep0_read(func->device, data->report_buf, setup->wLength, _hid_set_report_callback);
        break;
    case USB_HID_REQ_SET_IDLE:
        data->idle = setup->wValue >> 8;
        dcd_ep0_send_status(func->device->dcd);
        break;
    case USB_HID_REQ_SET_PROTOCOL:
        data->protocol = setup->wValue;

        dcd_ep0_send_status(func->device->dcd);
        break;
    }

    return RT_EOK;
}


/**
 * This function will run cdc function, it will be called on handle set configuration bRequest.
 *
 * @param func the usb function object.
 *
 * @return RT_EOK on successful.
 */
static rt_err_t _function_enable(ufunction_t func)
{
    struct hid_s *data;

    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL);
    data = (struct hid_s *) func->user_data;

    RT_DEBUG_LOG(RT_DEBUG_USB, ("hid function enable\n"));
//
//    _vcom_reset_state(func);
//
    if(data->ep_out->buffer == RT_NULL)
    {
        data->ep_out->buffer        = rt_dma_malloc(HID_RX_BUFSIZE);
    }
    data->ep_out->request.buffer    = data->ep_out->buffer;
    data->ep_out->request.size      = EP_MAXPACKET(data->ep_out);
    data->ep_out->request.req_type  = UIO_REQUEST_READ_BEST;

    rt_usbd_io_request(func->device, data->ep_out, &data->ep_out->request);


    return RT_EOK;
}

/**
 * This function will stop cdc function, it will be called on handle set configuration bRequest.
 *
 * @param func the usb function object.
 *
 * @return RT_EOK on successful.
 */
static rt_err_t _function_disable(ufunction_t func)
{
    struct hid_s *data;

    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL);
    data = (struct hid_s *) func->user_data;

    RT_DEBUG_LOG(RT_DEBUG_USB, ("hid function disable\n"));

    if(data->ep_out->buffer != RT_NULL)
    {
        rt_dma_free(data->ep_out->buffer);
        data->ep_out->buffer = RT_NULL;
    }

    // 失能hid后，清空buff
    // rt_ringbuffer_reset(hid_dev.receive_rb);

    return RT_EOK;
}

static struct ufunction_ops ops =
{
    _function_enable,
    _function_disable,
    RT_NULL,
};




/**
 * This function will configure hid descriptor.
 *
 * @param comm the communication interface number.
 * @param data the data interface number.
 *
 * @return RT_EOK on successful.
 */
static rt_err_t _hid_descriptor_config(uhid_comm_desc_t hid, rt_uint8_t cintf_nr)
{
#ifdef RT_USB_DEVICE_COMPOSITE
    hid->iad_desc.bFirstInterface = cintf_nr;
#endif

    return RT_EOK;
}

static rt_size_t _hid_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    rt_size_t data_size;

    data_size = rt_ringbuffer_get(hid_dev.receive_rb, buffer, size);

    return data_size;
}

static rt_size_t _hid_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    rt_size_t           ret = 0;
    struct usb_hid_node *p_node;
    const  uint8_t      *p_buff;
    uint8_t             list_isempty_flag = 0;
    struct hid_s        *p_hid_s = hid_dev.hid_s;

    if (hid_dev.active != 0)
    if (p_hid_s->func->device->state == USB_STATE_CONFIGURED)
    {
        rt_mutex_take(hid_dev.write_mutex, RT_WAITING_FOREVER);
        if (rt_list_isempty(&hid_dev.free_list)) {
            return 0;
        }

        if (rt_list_isempty(&hid_dev.ready_list))
        {
            list_isempty_flag = 1;
        }

        p_buff = buffer;
        for (; size > 0; ) {
            if (rt_list_isempty(&hid_dev.free_list)) {
                break;
            }
            p_node = rt_list_first_entry(&hid_dev.free_list, struct usb_hid_node, list);

            if (size < 64) {
                ret  += size;
                p_node->size = size;
                size  = 0;
            } else {
                ret         += 64;
                p_node->size = 64;
                size        -= 64;
            }
            rt_memcpy(p_node->buffer, p_buff, p_node->size);
            p_buff += p_node->size;

            rt_list_remove(&p_node->list);
            rt_list_insert_before(&hid_dev.ready_list, &p_node->list);
        }

        if (list_isempty_flag == 1) {
            p_node = rt_list_first_entry(&hid_dev.ready_list, struct usb_hid_node, list);
            p_hid_s->ep_in->request.buffer = (void *)p_node->buffer;
            p_hid_s->ep_in->request.size = (p_node->size) > 64 ? 64 : p_node->size;
            p_hid_s->ep_in->request.req_type = UIO_REQUEST_WRITE;
            rt_usbd_io_request(p_hid_s->func->device, p_hid_s->ep_in, &p_hid_s->ep_in->request);
        }
        rt_mutex_release(hid_dev.write_mutex);
        
        return ret;
    }

    return 0;
}

static rt_err_t _hid_ctrl(rt_device_t dev, int cmd, void *args)
{
    switch (cmd) {
        default:
            return -RT_ERROR;
            break;
    }
    return RT_EOK;
}


rt_err_t _hid_tx_complete(rt_device_t dev, void *buffer)
{
    struct usb_hid_node *p_node;

    rt_mutex_take(hid_dev.write_mutex, RT_WAITING_FOREVER);
    p_node = rt_list_first_entry(&hid_dev.ready_list, struct usb_hid_node, list);
    rt_list_remove(&p_node->list);
    rt_list_insert_before(&hid_dev.free_list, &p_node->list);
    
    if (!rt_list_isempty(&hid_dev.ready_list)) {
        p_node = rt_list_first_entry(&hid_dev.ready_list, struct usb_hid_node, list);
        hid_dev.hid_s->ep_in->request.buffer = (void *)p_node->buffer;
        hid_dev.hid_s->ep_in->request.size = (p_node->size) > 64 ? 64 : p_node->size;
        hid_dev.hid_s->ep_in->request.req_type = UIO_REQUEST_WRITE;
        rt_usbd_io_request(hid_dev.hid_s->func->device, hid_dev.hid_s->ep_in, &hid_dev.hid_s->ep_in->request);
    }
    rt_mutex_release(hid_dev.write_mutex);

    return RT_EOK;
}

// RT_WEAK void HID_Report_Received(hid_report_t report)
// {
//     dump_report(report);
// }
// ALIGN(RT_ALIGN_SIZE)
// static rt_uint8_t hid_thread_stack[512];
// static struct rt_thread hid_thread;

// static void hid_thread_entry(void* parameter)
// {
//     struct hid_report report;
//     struct hid_s *hiddev;
//     hiddev = (struct hid_s *)parameter;
// 	while(1)
// 	{
// 		if(rt_mq_recv(&hiddev->hid_mq, &report, sizeof(report),RT_WAITING_FOREVER) != RT_EOK )
//             continue;
// 		HID_Report_Received(&report);
// 	}
// }

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops hid_device_ops =
{
    RT_NULL,
    RT_NULL,
    RT_NULL,
    _hid_read,
    _hid_write,
    _hid_ctrl,
};
#endif

// static rt_uint8_t hid_mq_pool[(sizeof(struct hid_report)+sizeof(void*))*8];
// static void rt_usb_hid_init(struct ufunction *func)
// {
//     struct hid_s *hiddev;
//     hiddev = (struct hid_s *)func->user_data;
//     rt_memset(&hiddev->parent, 0, sizeof(hiddev->parent));

// #ifdef RT_USING_DEVICE_OPS
//     hiddev->parent.ops   = &hid_device_ops;
// #else
//     hiddev->parent.write = _hid_write;
// #endif
//     hiddev->func = func;
//     hiddev->idle = 1;

//     rt_device_register(&hiddev->parent, "hidd", RT_DEVICE_FLAG_RDWR);
    
//     // rt_mq_init(&hiddev->hid_mq, "hiddmq", hid_mq_pool, sizeof(struct hid_report),
//     //                         sizeof(hid_mq_pool), RT_IPC_FLAG_FIFO);
                            
//     // rt_thread_init(&hid_thread, "hidd", hid_thread_entry, hiddev,
//     //         hid_thread_stack, sizeof(hid_thread_stack), RT_USBD_THREAD_PRIO, 20);
//     // rt_thread_startup(&hid_thread);
// }
static rt_err_t rt_usb_hid_init(void)
{
    uint8_t node_size;
    struct usb_hid_node *list_node;

    /* create rx ringbuffer */
    hid_dev.receive_rb = rt_ringbuffer_create(HID_REC_BUFF_SIZE);
    if (hid_dev.receive_rb == RT_NULL) {
        rt_kprintf("err:create hid_dev.receive_rb failed \r\n");
        goto err0;
    }

    /* 创建tx list互斥量 */
    hid_dev.write_mutex = rt_mutex_create("hidm", RT_IPC_FLAG_PRIO);
    if (hid_dev.write_mutex == RT_NULL)
    {
        rt_kprintf("create hid tx list mutex failed.\n");
        goto err1;
    }

    /* create tx list node */
    rt_list_init(&hid_dev.free_list);
    rt_list_init(&hid_dev.ready_list);
    node_size = sizeof(struct usb_hid_node);
    for (uint8_t i = 0; i < HID_TX_NODE_SIZE; i++)
    {
        list_node = rt_malloc(node_size);
        if (!list_node)
        {
            rt_kprintf("%s: alloc hid tx node fail!\n", __func__);
            goto err2;
        }
        rt_list_insert_before(&hid_dev.free_list, &list_node->list);
    }

#ifdef RT_USING_DEVICE_OPS
    hid_dev.parent.ops   = &hid_device_ops;
#else
    hid_dev.parent.write   = _hid_write;
    hid_dev.parent.read    = _hid_read;
    hid_dev.parent.control = _hid_ctrl;
#endif

    if (RT_EOK != rt_device_register(&hid_dev.parent, "hidd", RT_DEVICE_FLAG_RDWR)) {
        rt_kprintf("hidd rt_device_register fail!\n");
        goto err2;
    }

    if (RT_EOK != rt_device_set_tx_complete(&hid_dev.parent, _hid_tx_complete)) {
        goto err3;
    }

    return RT_EOK;

err3:
    rt_device_unregister(&hid_dev.parent);
err2:
    // 释放链表内存
    while (!rt_list_isempty(&hid_dev.free_list)) {
        list_node = rt_list_first_entry(&hid_dev.ready_list, struct usb_hid_node, list);
        rt_list_remove(&list_node->list);
        rt_free(list_node);
    }
err1:
    rt_ringbuffer_destroy(hid_dev.receive_rb);
err0:
    return RT_ERROR;
}


/**
 * This function will create a hid function instance.
 *
 * @param device the usb device object.
 *
 * @return RT_EOK on successful.
 */
ufunction_t rt_usbd_function_hid_create(udevice_t device)
{
    ufunction_t     func;
    struct hid_s   *data;

    uintf_t         hid_intf;
    ualtsetting_t   hid_setting;
    uhid_comm_desc_t hid_desc;

    /* parameter check */
    RT_ASSERT(device != RT_NULL);

    extern uint8_t hl_board_nvram_init();

    hl_board_nvram_init();

    if (!hl_util_nvram_param_get("HL_SN", usb_sn, usb_sn, sizeof(usb_sn))) {
        rt_kprintf("USB SN : %s\r\n", usb_sn);
    }

    /* set usb device string description */
    rt_usbd_device_set_string(device, _ustring);

    /* create a cdc function */
    func = rt_usbd_function_new(device, &_dev_desc, &ops);
    //not support hs
    //rt_usbd_device_set_qualifier(device, &_dev_qualifier);

    /* allocate memory for cdc vcom data */
    data = (struct hid_s*)rt_malloc(sizeof(struct hid_s));
    rt_memset(data, 0, sizeof(struct hid_s));
    func->user_data = (void*)data;

    /* create an interface object */
    hid_intf = rt_usbd_interface_new(device, _interface_handler);

    /* create an alternate setting object */
    hid_setting = rt_usbd_altsetting_new(sizeof(struct uhid_comm_descriptor));

    /* config desc in alternate setting */
    rt_usbd_altsetting_config_descriptor(hid_setting, &_hid_comm_desc, (rt_off_t)&((uhid_comm_desc_t)0)->intf_desc);

    /* configure the hid interface descriptor */
    _hid_descriptor_config(hid_setting->desc, hid_intf->intf_num);

    /* create endpoint */
    hid_desc = (uhid_comm_desc_t)hid_setting->desc;
    data->ep_out = rt_usbd_endpoint_new(&hid_desc->ep_out_desc, _ep_out_handler);
    data->ep_in  = rt_usbd_endpoint_new(&hid_desc->ep_in_desc, _ep_in_handler);

    /* add the int out and int in endpoint to the alternate setting */
    rt_usbd_altsetting_add_endpoint(hid_setting, data->ep_out);
    rt_usbd_altsetting_add_endpoint(hid_setting, data->ep_in);

    /* add the alternate setting to the interface, then set default setting */
    rt_usbd_interface_add_altsetting(hid_intf, hid_setting);
    rt_usbd_set_altsetting(hid_intf, 0);

    /* add the interface to the mass storage function */
    rt_usbd_function_add_interface(func, hid_intf);

    /* initilize hid */
    // rt_usb_hid_init(func);
    data->func = func;
    data->idle = 1;
    hid_dev.active = 1;
    hid_dev.hid_s  = data;

    return func;
}
struct udclass hid_class = 
{
    .rt_usbd_function_create = rt_usbd_function_hid_create
};

int rt_usbd_hid_class_register(void)
{
    if (RT_EOK != rt_usb_hid_init()) {
        return -1;
    }
    hid_dev.active = 0;

    rt_usbd_class_register(&hid_class);
    return 0;
}
INIT_PREV_EXPORT(rt_usbd_hid_class_register);
#endif /* RT_USB_DEVICE_HID */
