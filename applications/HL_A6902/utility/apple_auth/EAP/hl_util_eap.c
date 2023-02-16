#include "hl_util_eap.h"

int hl_util_eap_init(hl_util_apple_p apple)
{
    if (NULL == apple) {
        apple->log("[%s][line:%d]param(apple) is NULL!\n", __func__, __LINE__);
        return -1;
    }
    if (apple->eap.eap_init_flag) {
        apple->log("[%s][line:%d]EAP Inited\r\n", __FUNCTION__, __LINE__);
        return -2;
    }

    apple->eap.eap_init_flag = 1;
    return 0;
}

int hl_util_eap_deinit(hl_util_apple_p apple)
{
    if (!apple->eap.eap_init_flag) {
        apple->log("[%s][line:%d]Not Init EAP, Deinit ERROR!", __FUNCTION__, __LINE__);
        return -1;
    }

    apple->eap.eap_init_flag = 0;
    return 0;
}

int hl_util_eap_oneshot(hl_util_apple_p apple)
{
    if (NULL == apple) {
        apple->log("[%s:%d] error parameter!\n", __func__, __LINE__);
        return -2;
    }

    int result = -1;

    return -1;
}
