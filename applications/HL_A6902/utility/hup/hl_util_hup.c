#include "hl_util_hup.h"
#include "hl_util_xor_calc.h"
#include <stdlib.h>
#include <string.h>

int hl_util_hup_init(hl_util_hup_t* hup_ptr, uint8_t* frame_data_addr, uint32_t (*get_time_stamp)(void),
                     void (*hup_success_handle_cb)(hup_protocol_type_t))
{
    if (NULL == hup_success_handle_cb) {
        HL_UTIL_HUP_DBG("[ERROR] hup init hup_success_handle_cb arg is NULL\n");
        return -1;
    }

    // 检查hup_handle结构体是否赋予初始值
    if ((hup_ptr->hup_handle.role != EM_HUP_ROLE_MASTER) && (hup_ptr->hup_handle.role != EM_HUP_ROLE_SLAVE)) {
        HL_UTIL_HUP_DBG("[ERROR] hup init handle role arg no init value\n");
        return -1;
    }
    if ((hup_ptr->hup_handle.timer_state != EM_HUP_TIMER_ENABLE)
        && (hup_ptr->hup_handle.timer_state != EM_HUP_TIMER_DISABLE)) {
        HL_UTIL_HUP_DBG("[ERROR] hup init handle timer_state arg no init value\n");
        return -1;
    }
    if (hup_ptr->hup_handle.frame_data_len == 0) {
        HL_UTIL_HUP_DBG("[ERROR] hup init handle frame_data_len arg no init value\n");
        return -1;
    }

    // 初始化hup结构体变量值
    hup_ptr->state                 = EM_HUP_STATE_HDR_H;
    hup_ptr->max_timeout           = HUP_MAX_TIMEOUT;
    hup_ptr->get_time_stamp        = get_time_stamp;
    hup_ptr->hup_success_handle_cb = hup_success_handle_cb;
    hup_ptr->hup_frame.data_addr   = frame_data_addr;

    return 0;
}

int hl_util_hup_deinit(hl_util_hup_t* hup_ptr)
{
    if (NULL == hup_ptr) {
        return -1;
    }

    hup_ptr->get_time_stamp        = NULL;
    hup_ptr->hup_success_handle_cb = NULL;

    return 0;
}

int hl_util_hup_encode(hup_role_em role, uint8_t cmd, uint8_t* frame_buf, uint16_t buf_len, uint8_t* data_addr,
                       uint16_t data_len)
{
    if ((NULL == frame_buf) | (NULL == data_addr)) {
        HL_UTIL_HUP_DBG("[ERROR] hup protocol encode arg\n");
        return -1;
    } else if (buf_len < (data_len + 6)) {
        HL_UTIL_HUP_DBG("[ERROR] hup protocol encode arg\n");
        return -1;
    }

    if (role == EM_HUP_ROLE_MASTER) {
        frame_buf[0] = HUP_FRAME_HOST_HDR_H;
        frame_buf[1] = HUP_FRAME_HOST_HDR_L;
    } else if (role == EM_HUP_ROLE_SLAVE) {
        frame_buf[0] = HUP_FRAME_CLIENT_HDR_H;
        frame_buf[1] = HUP_FRAME_CLIENT_HDR_L;
    }
    frame_buf[2] = cmd;
    frame_buf[3] = (uint8_t)(0xFF & (data_len >> 8));
    frame_buf[4] = (uint8_t)(0xFF & data_len);
    memcpy(&frame_buf[5], data_addr, data_len);

    frame_buf[5 + data_len] = hl_util_xor_calc_xor8(frame_buf, data_len + 5);

    return data_len + 6;
}

int hl_util_hup_decode(hl_util_hup_t* hup_ptr, uint8_t data_byte)
{
    if (NULL == hup_ptr) {
        HL_UTIL_HUP_DBG("[ERROR] hup protocol decode arg\n");
        return -1;
    }

    if (hup_ptr->hup_handle.timer_state == EM_HUP_TIMER_ENABLE) {
        if (hup_ptr->state == EM_HUP_STATE_HDR_H) {
            hup_ptr->cur_timeout_val  = 0;
            hup_ptr->last_timeout_val = 0;
        } else {
            hup_ptr->cur_timeout_val = hup_ptr->get_time_stamp();
        }

        if ((hup_ptr->cur_timeout_val - hup_ptr->last_timeout_val) > hup_ptr->max_timeout) {
            hup_ptr->state = EM_HUP_STATE_HDR_H;
            HL_UTIL_HUP_DBG("[INFO] hup protocol decode reset\n");
        }
    }

    switch (hup_ptr->state) {
        case EM_HUP_STATE_HDR_H:
            if (((hup_ptr->hup_handle.role == EM_HUP_ROLE_MASTER) && (data_byte == HUP_FRAME_CLIENT_HDR_H))
                || ((hup_ptr->hup_handle.role == EM_HUP_ROLE_SLAVE) && (data_byte == HUP_FRAME_HOST_HDR_H))) {
                hup_ptr->hup_frame.hdr_h = data_byte;
                hup_ptr->state           = EM_HUP_STATE_HDR_L;
                hup_ptr->xor8            = data_byte;
                hup_ptr->read_data_len   = 0;
                if (hup_ptr->hup_handle.timer_state == EM_HUP_TIMER_ENABLE) {
                    hup_ptr->last_timeout_val = hup_ptr->get_time_stamp();
                }
            }
            break;

        case EM_HUP_STATE_HDR_L:
            if (((hup_ptr->hup_handle.role == EM_HUP_ROLE_MASTER) && (data_byte == HUP_FRAME_CLIENT_HDR_L))
                || ((hup_ptr->hup_handle.role == EM_HUP_ROLE_SLAVE) && (data_byte == HUP_FRAME_HOST_HDR_L))) {
                hup_ptr->hup_frame.hdr_l = data_byte;
                hup_ptr->state           = EM_HUP_STATE_CMD;
                hup_ptr->xor8            = hup_ptr->xor8 ^ data_byte;
            } else {
                hup_ptr->state = EM_HUP_STATE_HDR_H;
            }
            break;

        case EM_HUP_STATE_CMD:
            hup_ptr->hup_frame.cmd = data_byte;
            hup_ptr->state         = EM_HUP_STATE_DATA_LEN_H;
            hup_ptr->xor8          = hup_ptr->xor8 ^ data_byte;
            break;

        case EM_HUP_STATE_DATA_LEN_H:
            hup_ptr->hup_frame.data_len_h = data_byte;
            hup_ptr->frame_data_len       = (data_byte << 8);
            hup_ptr->state                = EM_HUP_STATE_DATA_LEN_L;
            hup_ptr->xor8                 = hup_ptr->xor8 ^ data_byte;
            break;

        case EM_HUP_STATE_DATA_LEN_L:
            hup_ptr->hup_frame.data_len_l = data_byte;
            hup_ptr->frame_data_len       = hup_ptr->frame_data_len | data_byte;
            hup_ptr->xor8                 = hup_ptr->xor8 ^ data_byte;
            if (0 == hup_ptr->frame_data_len) {
                hup_ptr->state = EM_HUP_STATE_CRC;
            } else {
                hup_ptr->state = EM_HUP_STATE_DATA;
            }
            break;

        case EM_HUP_STATE_DATA:
            hup_ptr->hup_frame.data_addr[hup_ptr->read_data_len++] = data_byte;
            hup_ptr->xor8                                          = hup_ptr->xor8 ^ data_byte;
            if (hup_ptr->read_data_len == hup_ptr->frame_data_len) {
                hup_ptr->state = EM_HUP_STATE_CRC;
            }
            break;

        case EM_HUP_STATE_CRC:
            hup_ptr->hup_frame.xor8 = data_byte;
            hup_ptr->state          = EM_HUP_STATE_HDR_H;
            if (hup_ptr->hup_frame.xor8 == hup_ptr->xor8) {
                hup_ptr->hup_success_handle_cb(hup_ptr->hup_frame);
            }
            break;

        default:
            break;
    }
    return 0;
}
