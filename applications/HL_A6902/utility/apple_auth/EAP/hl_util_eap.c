#include "hl_util_eap.h"

int hl_util_eap_init(hl_util_apple_p apple)
{
    return -1;
}

int hl_util_eap_deinit(hl_util_apple_p apple)
{
    return -1;
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
