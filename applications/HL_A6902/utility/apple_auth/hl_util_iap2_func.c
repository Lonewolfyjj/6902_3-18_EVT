#include "hl_util_iap2_func.h"
#include "hl_util_iap2_packet.h"

// 挑战请求的长度
static uint16_t  challenge_req_len;
static uint16_t challenge_resp_len;

/**
 * hl_iap2_challange_response_process
 * @brief  处理挑战应答结果(状态机)
 * @param  iap2             iap2结构体
 * @return int 成功 0 | 失败 非0
 * @author lisl (songlin.li@hollyland-tech.com)
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-04-07      <td>lisl     <td>新建
 * </table>
 */
static int hl_iap2_challange_response_process(st_iap2_protocol_p iap2)
{
    uint8_t status = EM_HL_CHALLENGE_RESP_STM_WRITE_DATA;
    uint8_t ret    = 0;
    uint8_t val    = 0x01;

    while (1) {
        switch (status) {
            case EM_HL_CHALLENGE_RESP_STM_WRITE_DATA:
                // Write Challenge Data
                iap2->iap2_iic_write(CP_CHALLENGE_DATA, iap2->recv_buffer, challenge_req_len);
                status = EM_HL_CHALLENGE_RESP_STM_READ_DATA_LEN;
                break;

            case EM_HL_CHALLENGE_RESP_STM_READ_DATA_LEN:
                // Read Challenge Data length
                iap2->iap2_iic_read(CP_CHALLENGE_DATA_LEN, iap2->recv_buffer, sizeof(uint16_t), TIMEOUT_US);
                status = EM_HL_CHALLENGE_RESP_STM_WRITE_CTRL;
                break;

            case EM_HL_CHALLENGE_RESP_STM_WRITE_CTRL:
                // Write Autientication Control
                iap2->iap2_iic_write(CP_AUTHENTICATION_CONTROL_STATUES, &val, sizeof(uint8_t));
                status = EM_HL_CHALLENGE_RESP_STM_WRITE_CTRL;
                break;

            case EM_HL_CHALLENGE_RESP_STM_READ_CTRL:
                // Read Authentication Status
                iap2->iap2_iic_read(CP_AUTHENTICATION_CONTROL_STATUES, &val, sizeof(uint8_t), TIMEOUT_US);
                if (!(val & 0x01)) {
                    iap2->iap2_printf("[ERROR] [EM_HL_CHALLENGE_RESP_STM_READ_CTRL] receive status!\n");
                    status = EM_HL_CHALLENGE_RESP_STM_WRITE_DATA;
                } else {
                    status = EM_HL_CHALLENGE_RESP_STM_READ_RESP_LEN;
                }
                break;

            case EM_HL_CHALLENGE_RESP_STM_READ_RESP_LEN:
                // Read Challenge Response Data Length
                ret = iap2->iap2_iic_read(CP_CHALLENGE_RESPONSE_DATA_LEN, &challenge_resp_len, sizeof(uint16_t),
                                          TIMEOUT_US);
                challenge_resp_len = EXCHANGE_HIGH_LOW_BYTE(challenge_resp_len);
                status             = EM_HL_CHALLENGE_RESP_STM_READ_RESP_DATA;
                break;

            case EM_HL_CHALLENGE_RESP_STM_READ_RESP_DATA:
                // Read Challenge Response Data
                ret =
                    iap2->iap2_iic_read(CP_CHALLENGE_RESPONSE_DATA, iap2->recv_buffer, challenge_resp_len, TIMEOUT_US);
                return 0;

            default:
                break;
        }
    }
}

int hl_check_usb_insert(st_iap2_protocol_p iap2)
{
    iap2->main_status = EM_HL_IAP2_STM_MAIN_DETECT;
    return 0;
}

int hl_iap2_detect_send(st_iap2_protocol_p iap2)
{
    uint8_t len = 0;
    len         = hl_iap2_detect_packet_encode(iap2->send_buffer);
    iap2->iap2_usb_write(iap2->send_buffer, len);
    return 0;
}

int hl_iap2_detect_recv(st_iap2_protocol_p iap2)
{
    int     result = -1;
    uint8_t ret    = 0;

    ret = iap2->iap2_usb_read(iap2->recv_buffer, DETECT_FRAME_SIZE, TIMEOUT_US);
    if (ret == DETECT_FRAME_SIZE) {
        result = hl_iap2_detect_packet_decode(iap2->recv_buffer);
    }

    return result;
}

int hl_iap2_link_send_sync(st_iap2_protocol_p iap2)
{
    st_iap2_sync_packet_t iap2_sync_packet = { 0 };
    int                   result           = 0;
    uint8_t               ret              = 0;
    uint8_t               len              = 0;

    uint8_t* ptr_packet  = (uint8_t*)&iap2_sync_packet;
    uint8_t* ptr_header  = (uint8_t*)&iap2_sync_packet.packet_header;
    uint8_t* ptr_payload = (uint8_t*)&iap2_sync_packet.sync_payload;

    uint8_t size_packet  = sizeof(iap2_sync_packet);
    uint8_t size_payload = sizeof(iap2_sync_packet.sync_payload);

    // packet header
    ret = hl_iap2_packet_header_encode(ptr_header, size_packet, LINK_CONTROL_SYN, iap2->packet_arg);
    iap2->iap2_printf("\r\n%s->%02x:%02x:%02x\r\n", __func__, iap2->packet_arg.seq_num, iap2->packet_arg.ack_num,
                      iap2->packet_arg.session_id);
    if (ret > 0) {
        len += ret;
    } else {
        result = -1;
    }
    // packet payload
    ret = hl_iap2_packet_sync_payload_encode(ptr_payload, size_payload);
    if (ret > 0) {
        len += ret;
    } else {
        result = -1;
    }

    memcpy(iap2->send_buffer, ptr_packet, len);
    iap2->iap2_usb_write(iap2->send_buffer, len);

    return result;
}

int hl_iap2_link_recv_sync_ack(st_iap2_protocol_p iap2)
{
    st_iap2_sync_packet_t* iap2_sync_packet = NULL;

    int     result      = -1;
    uint8_t ret         = 0;
    uint8_t len         = 0;
    uint8_t size_packet = sizeof(st_iap2_sync_packet_t);

    memset(iap2->recv_buffer, 0, RECV_BUFFER_SIZE);
    ret = iap2->iap2_usb_read(iap2->recv_buffer, size_packet, 5000);
    if (ret == size_packet) {
        result = hl_iap2_packet_header_decode(iap2->recv_buffer, &len, LINK_CONTROL_ACK, &iap2->packet_arg);
        iap2->packet_arg.ack_num -= 1;
    }
    iap2->iap2_printf("%s->%02x:%02x:%02x:%02x", __func__, iap2->recv_buffer[4], LINK_CONTROL_ACK, iap2->recv_buffer[6],
                      iap2->packet_arg.ack_num);

    return result;
}

int hl_iap2_link_send_ack(st_iap2_protocol_p iap2)
{
    st_iap2_packet_header_t iap2_packet_header;

    int      result     = -1;
    uint8_t  ret        = 0;
    uint8_t  len        = 0;
    uint8_t* ptr_header = (uint8_t*)&iap2_packet_header;

    // packet header
    ret = hl_iap2_packet_header_encode(&iap2_packet_header, PACKET_HEADER_SIZE + 1, LINK_CONTROL_ACK, iap2->packet_arg);
    iap2->iap2_printf("\r\n%s->%02x:%02x:%02x\r\n", __func__, iap2->packet_arg.seq_num, iap2->packet_arg.ack_num,
                      iap2->packet_arg.session_id);
    if (ret > 0) {
        len += ret;
    } else {
        result = -1;
    }

    memcpy(iap2->send_buffer, ptr_header, len);
    iap2->iap2_usb_write(iap2->send_buffer, len);

    return result;
}

int hl_iap2_identify_req_auth(st_iap2_protocol_p iap2)
{
    int      result     = -1;
    uint8_t  ret        = 0;
    uint8_t  len        = 0;
    uint16_t message_id = 0;

    iap2->iap2_printf("\r\n%s:%d:%d\r\n", __func__, __LINE__, ret);
    ret   = iap2->iap2_usb_read(iap2->recv_buffer, IDENTIFY_FRAME_SIZE, TIMEOUT_US);
    int i = 0;
    for (i = 0; i < ret; i++) {
        iap2->iap2_printf("%02x ", iap2->recv_buffer[i]);
    }
    iap2->iap2_printf("\r\n%s:%d:%d\r\n", __func__, __LINE__, ret);
    if (ret == IDENTIFY_FRAME_SIZE) {
        message_id = hl_iap2_ctrl_packet_get_message_id(iap2->recv_buffer);
        // st_iap2_ctrl_packet_t* ptr_packet = (st_iap2_ctrl_packet_t*)iap2->recv_buffer;
        iap2->iap2_printf("\r\n%s:%d:0x%04x:%02x:%02x\r\n", __func__, __LINE__, message_id, iap2->packet_arg.ack_num,
                          iap2->recv_buffer[6]);
        ret = hl_iap2_packet_header_decode(iap2->recv_buffer, &len, LINK_CONTROL_ACK, &iap2->packet_arg);
        iap2->iap2_printf("\r\n%s:%d:%d\r\n", __func__, __LINE__, ret);
        if (!ret && message_id == SESSION_ID_REQUESTAUTHCERTIFICATE) {
            result = 0;
        }
    }
    iap2->iap2_printf("\r\n%s:%d:%d\r\n", __func__, __LINE__, result);
    return result;
}

int hl_iap2_identify_ack_auth(st_iap2_protocol_p iap2)
{
    st_iap2_ctrl_packet_t iap2_ctrl_packet;

    int      result            = 0;
    uint16_t ret               = 0;
    uint8_t  payload_check_sum = 0;
    uint16_t ctrl_packet_len   = 0;
    uint16_t ctrl_message_len  = 0;
    uint16_t ctrl_param_len    = 0;
    uint16_t certification_len = 0;
    uint16_t len               = 0;
    iap2->iap2_printf("\r\n%s:%d:%d\r\n", __func__, __LINE__, result);

    // Read Accessory Certificate Data Length
    ret = iap2->iap2_iic_read(CP_ACCESSORY_CERTIFICATION_DATA_LEN, iap2->recv_buffer, sizeof(uint16_t), TIMEOUT_US);
    iap2->iap2_printf("\r\n%s:%d:%d\r\n", __func__, __LINE__, ret);
    while (ret != sizeof(uint16_t)) {
        iap2->iap2_printf("error ret\n");
        ret = iap2->iap2_iic_read(CP_ACCESSORY_CERTIFICATION_DATA_LEN, iap2->recv_buffer, sizeof(uint16_t), TIMEOUT_US);
        iap2->iap2_printf("\r\n%s:%d:%d\r\n", __func__, __LINE__, ret);
        // return -1;
    }

    iap2->iap2_printf("\r\n%s:%d:%d\r\n", __func__, __LINE__, ret);

    certification_len = *((uint16_t*)iap2->recv_buffer);
    iap2->iap2_printf("%s: cer len  = %04x\r\n", __func__, certification_len);
    certification_len = EXCHANGE_HIGH_LOW_BYTE(certification_len);
    iap2->iap2_printf("%s: cer len  = %04x\r\n", __func__, certification_len);

    ctrl_packet_len =
        PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE + PACKET_PARAM_HEADER_SIZE + certification_len + 1;
    ctrl_message_len = PACKET_CTRL_HEADER_SIZE + PACKET_PARAM_HEADER_SIZE + certification_len;
    ctrl_param_len   = PACKET_PARAM_HEADER_SIZE + certification_len;
    iap2->iap2_printf("\r\n%s:%d:%d\r\n", __func__, __LINE__, ctrl_packet_len);

    // packet header
    ret = hl_iap2_packet_header_encode(&iap2_ctrl_packet.packet_header, ctrl_packet_len, LINK_CONTROL_ACK,
                                       iap2->packet_arg);
    if (ret > 0) {
        len += ret;
    } else {
        result = -1;
    }

    // packet payload
    ret = hl_iap2_ctrl_payload_encode(&iap2_ctrl_packet.ctrl_payload, ctrl_message_len,
                                      SESSION_ID_AUTHENTICATION_CERTIFICATE);
    if (ret > 0) {
        len += ret;
    } else {
        result = -1;
    }

    memcpy(iap2->send_buffer, &iap2_ctrl_packet, len);

    // Read Accessory Certificate Data
    ret = iap2->iap2_iic_read(CP_ACCESSORY_CERTIFICATION_DATA_1, iap2->recv_buffer, certification_len, TIMEOUT_US);
    while (ret != certification_len) {
        iap2->iap2_printf("error certification length = %d: %d\n", ret, certification_len);
        ret = iap2->iap2_iic_read(CP_ACCESSORY_CERTIFICATION_DATA_1, iap2->recv_buffer, certification_len, TIMEOUT_US);
    }
    if (ret != certification_len) {
        result = -1;
    }
    iap2->iap2_printf("\r\n%s:%d:%d\r\n", __func__, __LINE__, len);

    // packet param
    ret = hl_iap2_ctrl_add_param(iap2->send_buffer + len, ctrl_param_len, 0x00, iap2->recv_buffer);
    if (ret > 0) {
        len += ret;
    } else {
        result = -1;
    }
    iap2->iap2_printf("\r\n%s:%d:%d\r\n", __func__, __LINE__, len);

    // payload checksum
    payload_check_sum =
        hl_checksum_calculation(iap2->send_buffer, PACKET_HEADER_SIZE + 1, ctrl_packet_len - PACKET_HEADER_SIZE - 2);
    iap2->send_buffer[len++] = payload_check_sum;
    iap2->iap2_printf("\r\n%s:%d:%d\r\n", __func__, __LINE__, len);
    iap2->iap2_usb_write(iap2->send_buffer, len);
    iap2->iap2_printf("\r\n%s:%d:%d\r\n", __func__, __LINE__, len);

    return result;
}

int hl_iap2_identify_req_challenge(st_iap2_protocol_p iap2)
{
    int     result     = -1;
    uint8_t ret        = 0;
    uint8_t len        = 0;
    uint16_t message_id = 0;

    // Request Authentication Challenge Response
    ret = iap2->iap2_usb_read(iap2->recv_buffer, 64, TIMEOUT_US);
    if (ret >= 0) {
        message_id = hl_iap2_ctrl_packet_get_message_id(iap2->recv_buffer);
        ret        = hl_iap2_packet_header_decode(iap2->recv_buffer, &len, LINK_CONTROL_ACK, &iap2->packet_arg);
        iap2->iap2_printf("\r\n%s:%d:->%d:%04x:%04x\r\n", __func__, __LINE__, ret, message_id, SESSION_ID_REQUEST_AUTH_CHALLENG_RESP);
        if (!ret && message_id == SESSION_ID_REQUEST_AUTH_CHALLENG_RESP) {
            challenge_req_len = hl_iap2_ctrl_packet_get_param_len(iap2->recv_buffer);
            iap2->iap2_printf("\r\n%s:%d:->%d:%04x:%04x\r\n", __func__, __LINE__, ret, message_id, SESSION_ID_REQUEST_AUTH_CHALLENG_RESP);
            result            = 0;
        }
    }
    iap2->iap2_printf("\r\n%s:%d:->%d:%04x:%04x\r\n", __func__, __LINE__, ret, message_id, SESSION_ID_REQUEST_AUTH_CHALLENG_RESP);
    

    return result;
}

int hl_iap2_identify_ack_challenge(st_iap2_protocol_p iap2)
{
    st_iap2_ctrl_packet_t iap2_ctrl_packet;

    int      result            = 0;
    uint8_t  ret               = 0;
    uint8_t  payload_check_sum = 0;
    uint16_t ctrl_packet_len   = 0;
    uint16_t ctrl_message_len  = 0;
    uint16_t ctrl_param_len    = 0;
    uint16_t len               = 0;

    ctrl_packet_len =
        PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE + PACKET_PARAM_HEADER_SIZE + challenge_resp_len + 1;
    ctrl_message_len = PACKET_CTRL_HEADER_SIZE + PACKET_PARAM_HEADER_SIZE + challenge_resp_len;
    ctrl_param_len   = PACKET_PARAM_HEADER_SIZE + challenge_resp_len;

    // request challenge response
    ret = hl_iap2_challange_response_process(iap2);
    if (ret != 0) {
        iap2->iap2_printf("[error][hl_iap2_challange_response_process] ret val\n");
        result = -1;
    }

    // Authentication Response
    // packet header
    ret = hl_iap2_packet_header_encode(&iap2_ctrl_packet.packet_header, ctrl_packet_len, LINK_CONTROL_ACK,
                                       iap2->packet_arg);
    if (ret > 0) {
        len += ret;
    } else {
        result = -1;
    }

    // packet payload
    ret = hl_iap2_ctrl_payload_encode(&iap2_ctrl_packet.ctrl_payload, ctrl_message_len, SESSION_ID_AUTH_RESPONSE);
    if (ret > 0) {
        len += ret;
    } else {
        result = -1;
    }

    memcpy(iap2->send_buffer, &iap2_ctrl_packet, len);

    // packet param
    len += hl_iap2_ctrl_add_param(iap2->send_buffer + len, ctrl_param_len, 0x00, iap2->recv_buffer);
    if (ret > 0) {
        len += ret;
    } else {
        result = -1;
    }

    // payload checksum
    payload_check_sum =
        hl_checksum_calculation(iap2->send_buffer, PACKET_HEADER_SIZE + 1, ctrl_packet_len - PACKET_HEADER_SIZE - 2);
    iap2->send_buffer[len++] = payload_check_sum;

    iap2->iap2_usb_write(iap2->send_buffer, len);

    return result;
}

int hl_iap2_identify_succedd(st_iap2_protocol_p iap2)
{
    int     result = 0;
    uint8_t ret    = 0;
    uint8_t len    = 0;

    // receive identify succedd message
    ret = iap2->iap2_usb_read(iap2->recv_buffer, IDENTIFY_FRAME_SIZE, TIMEOUT_US);
    if (ret == IDENTIFY_FRAME_SIZE) {
        result = hl_iap2_packet_header_decode(iap2->recv_buffer, &len, LINK_CONTROL_ACK, &iap2->packet_arg);
    }

    return result;
}

int hl_iap2_identify_start_identification(st_iap2_protocol_p iap2)
{
    st_iap2_packet_header_t iap2_ack_packet;
    st_iap2_ctrl_packet_t*  ptr_packet = NULL;

    int     result         = -1;
    uint8_t ret            = 0;
    uint8_t len            = 0;
    uint8_t message_id     = 0;
    uint8_t ack_packet_len = PACKET_HEADER_SIZE + 1;

    // encode packet for ack identify succedd message
    ret = hl_iap2_packet_header_encode(&iap2_ack_packet, ack_packet_len, LINK_CONTROL_ACK, iap2->packet_arg);
    if (ret > 0) {
        len += ret;
    } else {
        result = -1;
    }

    // read start identification message
    iap2->iap2_usb_read(iap2->recv_buffer, IDENTIFY_FRAME_SIZE, TIMEOUT_US);
    result     = hl_iap2_packet_header_decode(iap2->recv_buffer, &len, LINK_CONTROL_ACK, &iap2->packet_arg);
    message_id = hl_iap2_ctrl_packet_get_message_id(iap2->recv_buffer);

    if (message_id != SESSION_ID_START_IDENTIFICATION) {
        iap2->iap2_printf("[ERROR][start_identification] receive message id\n");
        result = -1;
    }

    // send identify succedd message ack
    memcpy(iap2->send_buffer, &iap2_ack_packet, ack_packet_len);
    iap2->iap2_usb_write(iap2->send_buffer, ack_packet_len);

    return result;
}

int hl_iap2_identify_identification_info(st_iap2_protocol_p iap2)
{
    st_iap2_ctrl_packet_t iap2_ctrl_packet;

    uint8_t data_1[6] = { 0xae, 0x00, 0xae, 0x02, 0xae, 0x03 };
    uint8_t data_2[2] = { 0xae, 0x01 };
    uint8_t data_3    = 0x02;
    uint8_t data_4[2] = { 0x00, 0x64 };
    uint8_t data_5[2] = { 0x00, 0x00 };

    int      result            = -1;
    uint8_t  ret               = 0;
    uint8_t  payload_check_sum = 0;
    uint16_t ctrl_packet_len   = 0;
    uint16_t ctrl_message_len  = 0;
    uint16_t param_len         = 0;

    uint8_t* addr = iap2->send_buffer + PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE;

    // packet param < 1 >
    param_len += hl_iap2_ctrl_add_param(addr, 5 + strlen(IAP2_DEVICE_NAME), 0x0000, IAP2_DEVICE_NAME);
    addr[param_len++] = '\0';
    addr += param_len;

    // packet param < 2 >
    param_len += hl_iap2_ctrl_add_param(addr, 5 + strlen(IAP2_MODEID), 0x0001, IAP2_MODEID);
    addr[param_len++] = '\0';
    addr += param_len;

    // packet param < 3 >
    param_len += hl_iap2_ctrl_add_param(addr, 5 + strlen(IAP2_MANUFATORY), 0x0002, IAP2_MANUFATORY);
    addr[param_len++] = '\0';
    addr += param_len;

    // packet param < 4 >
    param_len += hl_iap2_ctrl_add_param(addr, 5 + strlen(IAP2_SERIAL_NAME), 0x0003, IAP2_SERIAL_NAME);
    addr[param_len++] = '\0';
    addr += param_len;

    // packet param < 5 >
    param_len += hl_iap2_ctrl_add_param(addr, 5 + strlen(IAP2_FIRMWAREVERSION), 0x0004, IAP2_FIRMWAREVERSION);
    addr[param_len++] = '\0';
    addr += param_len;

    // packet param < 6 >
    param_len += hl_iap2_ctrl_add_param(addr, 5 + strlen(IAP2_HARDWAREVERSION), 0x0005, IAP2_HARDWAREVERSION);
    addr[param_len++] = '\0';
    addr += param_len;

    // packet param < 7 >
    param_len += hl_iap2_ctrl_add_param(addr, 5 + strlen(IAP2_PUID), 0x0022, IAP2_PUID);
    addr[param_len++] = '\0';
    addr += param_len;

    // packet param < 8 >
    param_len += hl_iap2_ctrl_add_param(addr, 4 + sizeof(data_1), 0x0006, data_1);
    addr += param_len;

    // packet param < 9 >
    param_len += hl_iap2_ctrl_add_param(addr, 4 + sizeof(data_2), 0x0007, data_2);
    addr += param_len;

    // packet param < 10 >
    param_len += hl_iap2_ctrl_add_param(addr, 4 + sizeof(data_3), 0x0008, data_3);
    addr += param_len;

    // packet param < 11 >
    param_len += hl_iap2_ctrl_add_param(addr, 4 + sizeof(data_4), 0x0009, &data_4);
    addr += param_len;

    // packet param < 12 >
    param_len += hl_iap2_ctrl_add_param(addr, 5 + strlen("en"), 0x000c, "en");
    addr[param_len++] = '\0';
    addr += param_len;

    // packet param < 13 >
    param_len += hl_iap2_ctrl_add_param(addr, 5 + strlen("en"), 0x000d, "en");
    addr[param_len++] = '\0';
    addr += param_len;

    // packet param < 14 >
    param_len += hl_iap2_ctrl_add_param(addr, 0x002c, 0x0010, NULL);
    addr += 4;

    // packet param < 15 >
    param_len += hl_iap2_ctrl_add_param(addr, 5 + strlen(IAP2_UHOST_COMPONENT), 0x0001, data_5);
    addr[param_len++] = '\0';
    addr += param_len;

    // packet param < 16 >
    param_len += hl_iap2_ctrl_add_param(addr, 4 + sizeof(data_5), 0x0000, data_5);
    addr += param_len;

    // packet param < 17 >
    param_len += hl_iap2_ctrl_add_param(addr, 0x0004, 0x0002, NULL);
    addr += 4;

    ctrl_packet_len  = PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE + param_len + 1;
    ctrl_message_len = PACKET_CTRL_HEADER_SIZE + param_len;

    // packet header
    ret = hl_iap2_packet_header_encode(&iap2_ctrl_packet, ctrl_packet_len, LINK_CONTROL_ACK, iap2->packet_arg);
    if (ret < 0) {
        iap2->iap2_printf("[ERROR][hl_iap2_identify_identification_info] packet header encode\n");
        result = -1;
    }

    // packet payload
    ret = hl_iap2_ctrl_payload_encode(&iap2_ctrl_packet.ctrl_payload, ctrl_message_len, SESSION_ID_IDENTIFICATION_INFO);
    if (ret < 0) {
        iap2->iap2_printf("[ERROR][hl_iap2_identify_identification_info] packet payload encode\n");
        result = -1;
    }

    memcpy(iap2->send_buffer, &iap2_ctrl_packet, PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE);

    // payload checksum
    payload_check_sum = hl_checksum_calculation(iap2->send_buffer, PACKET_HEADER_SIZE + 1, ctrl_message_len);
    iap2->send_buffer[ctrl_packet_len - 1] = payload_check_sum;

    iap2->iap2_usb_write(iap2->send_buffer, ctrl_packet_len);

    return result;
}

int hl_iap2_identify_identification_accepted(st_iap2_protocol_p iap2)
{
    st_iap2_packet_header_t iap2_ack_packet;
    st_iap2_ctrl_packet_t*  ptr_packet = NULL;

    int     result     = -1;
    uint8_t ret        = 0;
    uint8_t len        = 0;
    uint8_t message_id = 0;

    ret = iap2->iap2_usb_read(iap2->recv_buffer, IDENTIFY_FRAME_SIZE, TIMEOUT_US);
    if (ret == IDENTIFY_FRAME_SIZE) {
        message_id = hl_iap2_ctrl_packet_get_message_id(iap2->recv_buffer);
        ret        = hl_iap2_packet_header_decode(iap2->recv_buffer, &len, LINK_CONTROL_ACK, &iap2->packet_arg);
        if (!ret && message_id == SESSION_ID_IDEN_ACCEPTED) {
            result = 0;
        }
    }

    return result;
}

int hl_iap2_powerupdate_send_power(st_iap2_protocol_p iap2)
{
    st_iap2_ctrl_packet_t iap2_ctrl_packet;

    int      result            = 0;
    uint8_t  ret               = 0;
    uint8_t  param_len         = 0;
    uint8_t  payload_check_sum = 0;
    uint16_t ctrl_packet_len   = 0;
    uint16_t ctrl_message_len  = 0;

    uint8_t* addr = iap2->send_buffer + PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE;

    // packet param < 1 >
    param_len += hl_iap2_ctrl_add_param(addr, 0x0004, 0x0000, NULL);
    addr += param_len;

    // packet param < 2 >
    param_len += hl_iap2_ctrl_add_param(addr, 0x0004, 0x0001, NULL);
    addr += param_len;

    // packet param < 3 >
    param_len += hl_iap2_ctrl_add_param(addr, 0x0004, 0x0004, NULL);
    addr += param_len;

    // packet param < 4 >
    param_len += hl_iap2_ctrl_add_param(addr, 0x0004, 0x0005, NULL);
    addr += param_len;

    // packet param < 5 >
    param_len += hl_iap2_ctrl_add_param(addr, 0x0004, 0x0006, NULL);
    addr += param_len;

    ctrl_packet_len  = PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE + param_len + 1;
    ctrl_message_len = PACKET_CTRL_HEADER_SIZE + param_len;

    // packet header
    ret = hl_iap2_packet_header_encode(&iap2_ctrl_packet, ctrl_packet_len, LINK_CONTROL_ACK, iap2->packet_arg);
    if (ret < 0) {
        iap2->iap2_printf("[ERROR][hl_iap2_powerupdate_send_power] packet header encode\n");
        result = -1;
    }

    // packet payload
    ret = hl_iap2_ctrl_payload_encode(&iap2_ctrl_packet.ctrl_payload, ctrl_message_len, MESSAGE_ID_START_POWER_UPDATES);
    if (ret < 0) {
        iap2->iap2_printf("[ERROR][hl_iap2_powerupdate_send_power] packet payload encode\n");
        result = -1;
    }

    memcpy(iap2->send_buffer, &iap2_ctrl_packet, PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE);

    // payload checksum
    payload_check_sum = hl_checksum_calculation(iap2->send_buffer, PACKET_HEADER_SIZE + 1, ctrl_message_len);
    iap2->send_buffer[ctrl_packet_len - 1] = payload_check_sum;

    iap2->iap2_usb_write(iap2->send_buffer, ctrl_packet_len);

    return result;
}

int hl_iap2_powerupdate_recv_update(st_iap2_protocol_p iap2)
{
    int     result     = -1;
    uint8_t ret        = 0;
    uint8_t len        = 0;
    uint8_t param_id   = 0;
    uint8_t message_id = 0;

    // recv power update id=0x0004
    ret = iap2->iap2_usb_read(iap2->recv_buffer, POWERUPDATE_FRAME_SIZE, TIMEOUT_US);
    if (ret == POWERUPDATE_FRAME_SIZE) {
        message_id = hl_iap2_ctrl_packet_get_message_id(iap2->recv_buffer);
        param_id   = hl_iap2_ctrl_packet_get_param_id(iap2->recv_buffer);
        ret        = hl_iap2_packet_header_decode(iap2->recv_buffer, &len, LINK_CONTROL_ACK, &iap2->packet_arg);
        if (ret != 0 || message_id != MESSAGE_ID_POWER_UPDATE || param_id != 0x0004) {
            iap2->iap2_printf("[ERROR][hl_iap2_powerupdate_recv_update] recv power update id:0x0004\n");
            result = -1;
        }
    }

    // recv power update id=0x0005
    ret = iap2->iap2_usb_read(iap2->recv_buffer, POWERUPDATE_FRAME_SIZE, TIMEOUT_US);
    if (ret == POWERUPDATE_FRAME_SIZE) {
        message_id = hl_iap2_ctrl_packet_get_message_id(iap2->recv_buffer);
        param_id   = hl_iap2_ctrl_packet_get_param_id(iap2->recv_buffer);
        ret        = hl_iap2_packet_header_decode(iap2->recv_buffer, &len, LINK_CONTROL_ACK, &iap2->packet_arg);
        if (ret != 0 || message_id != MESSAGE_ID_POWER_UPDATE || param_id != 0x0005) {
            iap2->iap2_printf("[ERROR][hl_iap2_powerupdate_recv_update] recv power update id:0x0005\n");
            result = -1;
        }
    }

    // recv power update id=0x0006
    ret = iap2->iap2_usb_read(iap2->recv_buffer, POWERUPDATE_FRAME_SIZE, TIMEOUT_US);
    if (ret == POWERUPDATE_FRAME_SIZE) {
        message_id = hl_iap2_ctrl_packet_get_message_id(iap2->recv_buffer);
        param_id   = hl_iap2_ctrl_packet_get_param_id(iap2->recv_buffer);
        ret        = hl_iap2_packet_header_decode(iap2->recv_buffer, &len, LINK_CONTROL_ACK, &iap2->packet_arg);
        if (ret != 0 || message_id != MESSAGE_ID_POWER_UPDATE || param_id != 0x0006) {
            iap2->iap2_printf("[ERROR][hl_iap2_powerupdate_recv_update] recv power update id:0x0006\n");
            result = -1;
        }
    }

    return result;
}

int hl_iap2_powerupdate_send_power_source(st_iap2_protocol_p iap2)
{
    st_iap2_ctrl_packet_t iap2_ctrl_packet;

    int      result            = 0;
    uint8_t  ret               = 0;
    uint8_t  param_len         = 0;
    uint8_t  payload_check_sum = 0;
    uint16_t ctrl_packet_len   = 0;
    uint16_t ctrl_message_len  = 0;

    uint8_t  DeviceBatteryShouldChargeIfPowerIsPresent = 0x01;
    uint16_t AvailableCurrentForDeviceVal              = 0x0BB8;

    uint8_t* addr = iap2->send_buffer + PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE;

    // packet param < 1 >
    param_len += hl_iap2_ctrl_add_param(addr, 0x0006, 0x0000, &AvailableCurrentForDeviceVal);
    addr += param_len;

    // packet param < 2 >
    param_len += hl_iap2_ctrl_add_param(addr, 0x0005, 0x0001, &DeviceBatteryShouldChargeIfPowerIsPresent);
    addr += param_len;

    ctrl_packet_len  = PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE + param_len + 1;
    ctrl_message_len = PACKET_CTRL_HEADER_SIZE + param_len;

    // packet header
    ret = hl_iap2_packet_header_encode(&iap2_ctrl_packet, ctrl_packet_len, LINK_CONTROL_ACK, iap2->packet_arg);
    if (ret < 0) {
        iap2->iap2_printf("[ERROR][hl_iap2_powerupdate_send_power_source] packet header encode\n");
        result = -1;
    }

    // packet payload
    ret = hl_iap2_ctrl_payload_encode(&iap2_ctrl_packet.ctrl_payload, ctrl_message_len, MESSAGE_ID_POWEERSOURCE_UPDATE);
    if (ret < 0) {
        iap2->iap2_printf("[ERROR][hl_iap2_powerupdate_send_power_source] packet payload encode\n");
        result = -1;
    }

    memcpy(iap2->send_buffer, &iap2_ctrl_packet, PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE);

    // payload checksum
    payload_check_sum = hl_checksum_calculation(iap2->send_buffer, PACKET_HEADER_SIZE + 1, ctrl_message_len);
    iap2->send_buffer[ctrl_packet_len - 1] = payload_check_sum;

    iap2->iap2_usb_write(iap2->send_buffer, ctrl_packet_len);

    return result;
}

int hl_iap2_eap_handle_msg(st_iap2_protocol_p iap2)
{
    if (iap2 == NULL) {
        iap2->iap2_printf("[hl_iap2_eap_handle_msg] error parameter!\n");
        return -1;
    }

    int     result     = -1;
    uint8_t ret        = 0;
    uint8_t len        = 0;
    uint8_t message_id = 0;

    iap2->iap2_usb_read(iap2->recv_buffer, 64, TIMEOUT_US);
    result = hl_iap2_packet_header_decode(iap2->recv_buffer, &len, LINK_CONTROL_ACK, &iap2->packet_arg);

    switch (iap2->packet_arg.session_id) {
        case SESSION_ID_CTRL: {
            message_id = hl_iap2_ctrl_packet_get_message_id(iap2->recv_buffer);
            switch (message_id) {
                case MESSAGE_ID_START_EAP:
                    /* code */
                    break;

                case MESSAGE_ID_STOP_EAP:
                    /* code */
                    break;

                case MESSAGE_ID_POWER_UPDATE:
                    /* code */
                    break;

                default:
                    break;
            }
            hl_iap2_link_send_ack(iap2);
        } break;

        case SESSION_ID_EA:
            // send msg to eap_event_handle_thread by msg_queue
            break;

        default:
            break;
    }

    return 0;
}

int hl_iap2_eap_check_power_insert(st_iap2_protocol_p iap2)
{
    if (iap2 == NULL) {
        iap2->iap2_printf("[hl_iap2_eap_check_power_insert] error parameter!\n");
        return -1;
    }

    return 0;
}
