#include "hl_util_hup.h"
#include "hl_util_xor_calc.h"

hup_protocol_handle_p hl_util_hup_protocol_init(hup_role_em role, hup_timer_state_em timer_state, uint8_t* buf_addr,
                                                uint16_t buf_size, uint32_t (*get_time_stamp)(void),
                                                void (*hup_success_handle_func)(hup_protocol_type_t))
{
    if ((NULL == hup_success_handle_func) || (NULL == buf_addr)) {
        HL_UTIL_HUP_DBG("[ERROR] hup protocol init arg\n");
        return NULL;
    }

    hup_protocol_handle_p hup = (hup_protocol_handle_p)malloc(sizeof(hup_protocol_handle_t));
    memset(hup, 0, sizeof(hup_protocol_handle_t));

    hup->role                    = role;
    hup->state                   = EM_HUP_STATE_HDR_H;
    hup->timer_state             = timer_state;
    hup->max_timeout             = HUP_MAX_TIMEOUT;
    hup->get_time_stamp          = get_time_stamp;
    hup->hup_success_handle_func = hup_success_handle_func;
    hup->buf_len                 = buf_size;
    hup->hup_frame.data_addr     = buf_addr;

    return hup;
}

int hl_util_hup_protocol_deinit(hup_protocol_handle_p hup)
{
    if (NULL == hup) {
        return -1;
    }

    hup->get_time_stamp          = NULL;
    hup->hup_success_handle_func = NULL;
    free(hup);

    return 0;
}

int hl_util_hup_protocol_encode(uint8_t role, uint8_t cmd, uint8_t* frame_buf, uint16_t buf_len, uint8_t* data_addr,
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

int hl_util_hup_protocol_decode(hup_protocol_handle_p hup, uint8_t data_byte)
{
    if (NULL == hup) {
        HL_UTIL_HUP_DBG("[ERROR] hup protocol decode arg\n");
        return -1;
    }

    if (hup->timer_state == EM_HUP_ENABLE) {
        if (hup->state == EM_HUP_STATE_HDR_H) {
            hup->cur_timeout_val  = 0;
            hup->last_timeout_val = 0;
        } else {
            hup->cur_timeout_val = hup->get_time_stamp();
        }

        if ((hup->cur_timeout_val - hup->last_timeout_val) > hup->max_timeout) {
            hup->state = EM_HUP_STATE_HDR_H;
        } else {
            hup->last_timeout_val = hup->cur_timeout_val;
        }
    }

    switch (hup->state) {
        case EM_HUP_STATE_HDR_H:
            if (((hup->role == EM_HUP_ROLE_MASTER) && (data_byte == HUP_FRAME_CLIENT_HDR_H))
                || ((hup->role == EM_HUP_ROLE_SLAVE) && (data_byte == HUP_FRAME_HOST_HDR_H))) {
                hup->hup_frame.hdr_h = data_byte;
                hup->state           = EM_HUP_STATE_HDR_L;
                hup->xor8            = data_byte;
                hup->read_data_len   = 0;
                if (hup->timer_state == EM_HUP_ENABLE) {
                    hup->last_timeout_val = hup->get_time_stamp();
                }
            }
            break;

        case EM_HUP_STATE_HDR_L:
            if (((hup->role == EM_HUP_ROLE_MASTER) && (data_byte == HUP_FRAME_CLIENT_HDR_L))
                || ((hup->role == EM_HUP_ROLE_SLAVE) && (data_byte == HUP_FRAME_HOST_HDR_L))) {
                hup->hup_frame.hdr_l = data_byte;
                hup->state           = EM_HUP_STATE_CMD;
                hup->xor8            = hup->xor8 ^ data_byte;
            } else {
                hup->state = EM_HUP_STATE_HDR_H;
            }
            break;

        case EM_HUP_STATE_CMD:
            hup->hup_frame.cmd = data_byte;
            hup->state         = EM_HUP_STATE_DATA_LEN_H;
            hup->xor8          = hup->xor8 ^ data_byte;
            break;

        case EM_HUP_STATE_DATA_LEN_H:
            hup->hup_frame.data_len = (data_byte << 8);
            hup->state              = EM_HUP_STATE_DATA_LEN_L;
            hup->xor8               = hup->xor8 ^ data_byte;
            break;

        case EM_HUP_STATE_DATA_LEN_L:
            hup->hup_frame.data_len = hup->hup_frame.data_len | data_byte;
            hup->state              = EM_HUP_STATE_DATA;
            hup->xor8               = hup->xor8 ^ data_byte;
            break;

        case EM_HUP_STATE_DATA:
            hup->hup_frame.data_addr[hup->read_data_len++] = data_byte;
            hup->xor8                                      = hup->xor8 ^ data_byte;
            if (hup->read_data_len == hup->hup_frame.data_len) {
                hup->state = EM_HUP_STATE_CRC;
            }
            break;

        case EM_HUP_STATE_CRC:
            hup->hup_frame.xor8 = data_byte;
            hup->state          = EM_HUP_STATE_HDR_H;
            if (hup->hup_frame.xor8 == hup->xor8) {
                hup->hup_success_handle_func(hup->hup_frame);
            }
            break;

        default:
            break;
    }
    return 0;
}
