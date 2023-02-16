#include "hl_util_apple.h"

int hl_util_apple_oneshot(hl_util_apple_p apple, hl_apple_action_e action)
{
    if (NULL == apple) {
        apple->log("[%s][line:%d]param(apple) is NULL!\n", __func__, __LINE__);
        return -2;
    }

    int result = -2;

    switch (action) {
        case EM_HL_IAP2:
            result = hl_util_iap2_oneshot(apple);
            break;

        case EM_HL_EAP:
            result = hl_util_eap_oneshot(apple);
            break;

        default:
            break;
    }

    return result;
}

int hl_util_apple_init(hl_util_apple_p apple, apple_func_handle handle)
{
    if (NULL == apple) {
        apple->log("[%s][line:%d]param(apple) is NULL!\n", __func__, __LINE__);
        return -1;
    }
    if ((NULL == handle.delay_usec) || (NULL == handle.usb_read) || (NULL == handle.usb_write)
        || (NULL == handle.iic_read) || (NULL == handle.iic_write)) {
        apple->log("[%s][line:%d]param(handle) is NULL!\n", __func__, __LINE__);
        return -2;
    }
    if (apple->init_flag) {
        apple->log("[%s][line:%d]Apple Inited\r\n", __FUNCTION__, __LINE__);
        return -3;
    }

    int result = 0;

    // 注册函数
    apple->delay_usec = handle.delay_usec;
    apple->usb_read   = handle.usb_read;
    apple->usb_write  = handle.usb_write;
    apple->iic_read   = handle.iic_read;
    apple->iic_write  = handle.iic_write;
    apple->log        = handle.log;

    // 申请内存资源
    apple->fifo_buf = (uint8_t*)rt_malloc(sizeof(uint8_t) * APPLE_FIFO_BUF_SIZE);
    if (NULL == apple->fifo_buf) {
        rt_kprintf("[%s][line:%d]fifo_buf rt_malloc failed!!!\n", __func__, __LINE__);
        return -3;
    }

    // 初始化FIFO
    result = hl_util_fifo_init(&apple->fifo, apple->fifo_buf, APPLE_FIFO_BUF_SIZE);
    if (result) {
        rt_kprintf("[%s][line:%d]fifo_init_func failed!!!\n", __func__, __LINE__);
        return -4;
    }

    // 初始化iAP2相关资源
    result = hl_util_iap2_init(apple);
    if (result) {
        rt_kprintf("[%s][line:%d](%d)iap2_init_func failed!!!\n", __func__, __LINE__, result);
        return -5;
    }

    apple->init_flag = 1;

    return 0;
}

int hl_util_apple_deinit(hl_util_apple_p apple)
{
    if (!apple->init_flag) {
        apple->log("[%s][line:%d]Not Init Apple, Deinit ERROR!", __FUNCTION__, __LINE__);
        return -1;
    }

    hl_util_iap2_deinit(apple);

    // 去初始化FIFO
    hl_util_fifo_deinit(&apple->fifo);
    // 释放内存资源
    rt_free(apple->fifo_buf);

    apple->init_flag = 0;

    return 0;
}
