#include "hl_util_xor_calc.h"

uint8_t hl_util_xor_calc_xor8(uint8_t* data_buf, uint16_t data_len)
{
    uint8_t xor = 0;
    int i       = 0;
    for (i = 0; i < data_len; i++) {
        xor ^= data_buf[i];
    }

    return xor;
}

uint16_t hl_util_xor_calc_xor16(uint8_t* data_buf, uint16_t data_len)
{
    uint16_t xor = 0;
    int i        = 0;
    for (i = 0; i < data_len; i++) {
        xor ^= data_buf[i];
    }

    return xor;
}
