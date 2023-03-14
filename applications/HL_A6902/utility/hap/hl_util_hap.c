#include "hl_util_hap.h"
#include "hl_util_xor_calc.h"
#include <stdlib.h>
#include <string.h>

int hl_util_hap_init(hl_util_hap_t* hap_ptr, uint8_t* frame_data_addr, uint32_t (*get_time_stamp)(void),
                     void (*hap_success_handle_cb)(hap_protocol_type_t))
{
    if (NULL == hap_success_handle_cb) {
        HL_UTIL_HAP_DBG("[ERROR] hap init hap_success_handle_cb arg is NULL\n");
        return -1;
    }

    // 检查hap_handle结构体是否赋予初始值
    if ((hap_ptr->hap_handle.role != EM_HAP_ROLE_MASTER) && (hap_ptr->hap_handle.role != EM_HAP_ROLE_SLAVE)) {
        HL_UTIL_HAP_DBG("[ERROR] hap init handle role arg no init value\n");
        return -1;
    }
    if ((hap_ptr->hap_handle.timer_state != EM_HAP_TIMER_ENABLE)
        && (hap_ptr->hap_handle.timer_state != EM_HAP_TIMER_DISABLE)) {
        HL_UTIL_HAP_DBG("[ERROR] hap init handle timer_state arg no init value\n");
        return -1;
    }
    if (hap_ptr->hap_handle.frame_data_len == 0) {
        HL_UTIL_HAP_DBG("[ERROR] hap init handle frame_data_len arg no init value\n");
        return -1;
    }

    // 初始化hap结构体变量值
    hap_ptr->state                 = EM_HAP_STATE_HDR_H;
    hap_ptr->max_timeout           = HAP_MAX_TIMEOUT;
    hap_ptr->get_time_stamp        = get_time_stamp;
    hap_ptr->hap_success_handle_cb = hap_success_handle_cb;
    hap_ptr->hap_frame.data_addr   = frame_data_addr;

    return 0;
}

int hl_util_hap_deinit(hl_util_hap_t* hap_ptr)
{
    if (NULL == hap_ptr) {
        return -1;
    }

    hap_ptr->get_time_stamp        = NULL;
    hap_ptr->hap_success_handle_cb = NULL;

    return 0;
}

int hl_util_hap_encode(hap_role_em role, uint8_t cmd, uint8_t ctrl, uint8_t* frame_buf, uint16_t buf_len, uint8_t* data_addr,
                       uint16_t data_len)
{
    if ((NULL == frame_buf) | (NULL == data_addr)) {
        HL_UTIL_HAP_DBG("[ERROR] hap protocol encode arg\n");
        return -1;
    } else if (buf_len < (data_len + 6)) {
        HL_UTIL_HAP_DBG("[ERROR] hap protocol encode arg\n");
        return -1;
    }

    if (role == EM_HAP_ROLE_MASTER) {
        frame_buf[0] = HAP_FRAME_HOST_HDR_H;
        frame_buf[1] = HAP_FRAME_HOST_HDR_L;
    } else if (role == EM_HAP_ROLE_SLAVE) {
        frame_buf[0] = HAP_FRAME_CLIENT_HDR_H;
        frame_buf[1] = HAP_FRAME_CLIENT_HDR_L;
    }
    frame_buf[2] = cmd;
    frame_buf[3] = ctrl;
    frame_buf[4] = (uint8_t)(0xFF & (data_len >> 8));
    frame_buf[5] = (uint8_t)(0xFF & data_len);
    memcpy(&frame_buf[6], data_addr, data_len);

    frame_buf[6 + data_len] = hl_util_xor_calc_xor8(frame_buf, data_len + 5);

    return data_len + 7;
}

int hl_util_hap_decode(hl_util_hap_t* hap_ptr, uint8_t data_byte)
{
    if (NULL == hap_ptr) {
        HL_UTIL_HAP_DBG("[ERROR] hap protocol decode arg\n");
        return -1;
    }

    if (hap_ptr->hap_handle.timer_state == EM_HAP_TIMER_ENABLE) {
        if (hap_ptr->state == EM_HAP_STATE_HDR_H) {
            hap_ptr->cur_timeout_val  = 0;
            hap_ptr->last_timeout_val = 0;
        } else {
            hap_ptr->cur_timeout_val = hap_ptr->get_time_stamp();
        }

        if ((hap_ptr->cur_timeout_val - hap_ptr->last_timeout_val) > hap_ptr->max_timeout) {
            hap_ptr->state = EM_HAP_STATE_HDR_H;
            HL_UTIL_HAP_DBG("[INFO] hap protocol decode reset\n");
        }
    }

    switch (hap_ptr->state) {
        case EM_HAP_STATE_HDR_H:
            if (((hap_ptr->hap_handle.role == EM_HAP_ROLE_MASTER) && (data_byte == HAP_FRAME_CLIENT_HDR_H))
                || ((hap_ptr->hap_handle.role == EM_HAP_ROLE_SLAVE) && (data_byte == HAP_FRAME_HOST_HDR_H))) {
                hap_ptr->hap_frame.hdr_h = data_byte;
                hap_ptr->state           = EM_HAP_STATE_HDR_L;
                hap_ptr->xor8            = data_byte;
                hap_ptr->read_data_len   = 0;
                if (hap_ptr->hap_handle.timer_state == EM_HAP_TIMER_ENABLE) {
                    hap_ptr->last_timeout_val = hap_ptr->get_time_stamp();
                }
            }
            break;

        case EM_HAP_STATE_HDR_L:
            if (((hap_ptr->hap_handle.role == EM_HAP_ROLE_MASTER) && (data_byte == HAP_FRAME_CLIENT_HDR_L))
                || ((hap_ptr->hap_handle.role == EM_HAP_ROLE_SLAVE) && (data_byte == HAP_FRAME_HOST_HDR_L))) {
                hap_ptr->hap_frame.hdr_l = data_byte;
                hap_ptr->state           = EM_HAP_STATE_CMD;
                hap_ptr->xor8            = hap_ptr->xor8 ^ data_byte;
            } else {
                hap_ptr->state = EM_HAP_STATE_HDR_H;
            }
            break;

        case EM_HAP_STATE_CMD:
            hap_ptr->hap_frame.cmd = data_byte;
            hap_ptr->state         = EM_HAP_STATE_CTRL;
            hap_ptr->xor8          = hap_ptr->xor8 ^ data_byte;
            break;

        case EM_HAP_STATE_CTRL:
            hap_ptr->hap_frame.ctrl = data_byte;
            hap_ptr->state          = EM_HAP_STATE_DATA_LEN_H;
            hap_ptr->xor8           = hap_ptr->xor8 ^ data_byte;
            break;

        case EM_HAP_STATE_DATA_LEN_H:
            hap_ptr->hap_frame.data_len_h = data_byte;
            hap_ptr->frame_data_len       = (data_byte << 8);
            hap_ptr->state                = EM_HAP_STATE_DATA_LEN_L;
            hap_ptr->xor8                 = hap_ptr->xor8 ^ data_byte;
            break;

        case EM_HAP_STATE_DATA_LEN_L:
            hap_ptr->hap_frame.data_len_l = data_byte;
            hap_ptr->frame_data_len       = hap_ptr->frame_data_len | data_byte;
            hap_ptr->xor8                 = hap_ptr->xor8 ^ data_byte;
            if (0 == hap_ptr->frame_data_len) {
                hap_ptr->state = EM_HAP_STATE_CRC;
            } else {
                hap_ptr->state = EM_HAP_STATE_DATA;
            }
            break;

        case EM_HAP_STATE_DATA:
            hap_ptr->hap_frame.data_addr[hap_ptr->read_data_len++] = data_byte;
            hap_ptr->xor8                                          = hap_ptr->xor8 ^ data_byte;
            if (hap_ptr->read_data_len == hap_ptr->frame_data_len) {
                hap_ptr->state = EM_HAP_STATE_CRC;
            }
            break;

        case EM_HAP_STATE_CRC:
            hap_ptr->hap_frame.xor8 = data_byte;
            hap_ptr->state          = EM_HAP_STATE_HDR_H;
            if (hap_ptr->hap_frame.xor8 == hap_ptr->xor8) {
                hap_ptr->hap_success_handle_cb(hap_ptr->hap_frame);
            }
            break;

        default:
            break;
    }
    return 0;
}
