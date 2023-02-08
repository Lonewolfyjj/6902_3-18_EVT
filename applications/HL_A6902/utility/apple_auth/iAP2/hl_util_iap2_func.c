#include "hl_util_iap2_func.h"

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
static int hl_iap2_challange_response_process(hl_util_apple_p apple)
{
    int      result             = 0;
    uint8_t  flag               = 1;
    uint8_t  ret                = 0;
    uint8_t  try_time           = 5;
    uint8_t  val                = 0x01;
    uint16_t challenga_data_len = 0;

    hl_iap2_challange_response_status_e status = EM_HL_CHALLENGE_RESP_STM_WRITE_DATA;

    while (flag) {
        switch (status) {
            case EM_HL_CHALLENGE_RESP_STM_WRITE_DATA:
                // Write Challenge Data
                apple->iic_write(CP_CHALLENGE_DATA, apple->iap2.challenge_resp_data, apple->iap2.challenge_req_len);
                status = EM_HL_CHALLENGE_RESP_STM_READ_DATA_LEN;
                break;

            case EM_HL_CHALLENGE_RESP_STM_READ_DATA_LEN:
                // Read Challenge Data length
                apple->iic_read(CP_CHALLENGE_DATA_LEN, &challenga_data_len, sizeof(uint16_t), TIMEOUT_US);
                status = EM_HL_CHALLENGE_RESP_STM_WRITE_CTRL;
                break;

            case EM_HL_CHALLENGE_RESP_STM_WRITE_CTRL:
                // Write Autientication Control
                apple->iic_write(CP_AUTHENTICATION_CONTROL_STATUES, &val, sizeof(uint8_t));
                status = EM_HL_CHALLENGE_RESP_STM_READ_CTRL;
                break;

            case EM_HL_CHALLENGE_RESP_STM_READ_CTRL:
                // Read Authentication Status
                apple->delay_usec(500000);
                do {
                    ret = apple->iic_read(CP_AUTHENTICATION_CONTROL_STATUES, &val, sizeof(uint8_t), TIMEOUT_US);
                    try_time--;
                } while (try_time && !ret);

                if (!(val & 0x10)) {
                    status = EM_HL_CHALLENGE_RESP_STM_WRITE_DATA;
                } else {
                    status   = EM_HL_CHALLENGE_RESP_STM_READ_RESP_LEN;
                    try_time = 5;
                }
                break;

            case EM_HL_CHALLENGE_RESP_STM_READ_RESP_LEN:
                // Read Challenge Response Data Length
                do {
                    ret = apple->iic_read(CP_CHALLENGE_RESPONSE_DATA_LEN, &apple->iap2.challenge_resp_len,
                                          sizeof(uint16_t), TIMEOUT_US);
                    try_time--;
                } while (try_time && !ret);

                if (sizeof(uint16_t) == ret) {
                    apple->iap2.challenge_resp_len = EXCHANGE_HIGH_LOW_BYTE(apple->iap2.challenge_resp_len);
                    status                         = EM_HL_CHALLENGE_RESP_STM_READ_RESP_DATA;
                    try_time                       = 5;
                } else {
                    result = -1;
                    apple->log("[ERROR][%s:%d] read status!\n", __func__, __LINE__);
                    apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_FAILED;
                }
                break;

            case EM_HL_CHALLENGE_RESP_STM_READ_RESP_DATA:
                // Read Challenge Response Data
                do {
                    ret = apple->iic_read(CP_CHALLENGE_RESPONSE_DATA, apple->iap2.recv_buffer,
                                          apple->iap2.challenge_resp_len, TIMEOUT_US);
                    try_time--;
                } while (try_time && !ret);

                if (!ret) {
                    result = -2;
                    apple->log("[ERROR][%s:%d] read resp data!\n", __func__, __LINE__);
                    apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_FAILED;
                }
                // jump out loop(while)
                flag = 0;
                break;

            default:
                break;
        }
    }

    return result;
}

int hl_check_usb_insert(hl_util_apple_p apple)
{
    apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_DETECT;
    return 0;
}

int hl_iap2_detect_send(hl_util_apple_p apple)
{
    int     ret      = 0;
    int     len      = 0;
    uint8_t try_time = 10;

    do {
        ret = hl_iap2_detect_packet_encode(apple->iap2.send_buffer);
        try_time--;
    } while (try_time && ret <= 0);

    if (ret >= 0) {
        apple->usb_write(apple->iap2.send_buffer, ret);
        ret = 0;
    } else {
        ret = -1;
    }

    return ret;
}

int hl_iap2_detect_recv(hl_util_apple_p apple)
{
    int     result   = 1;
    int     ret      = 0;
    uint8_t try_time = 3;

    do {
        ret = apple->usb_read(apple->iap2.recv_buffer, DETECT_FRAME_SIZE, TIMEOUT_US);
        if (DETECT_FRAME_SIZE == ret) {
            result = hl_iap2_detect_packet_decode(apple->iap2.recv_buffer);
        }
        try_time--;
    } while (try_time && result);

    return result;
}

int hl_iap2_link_send_sync(hl_util_apple_p apple)
{
    st_iap2_sync_packet_t iap2_sync_packet = { 0 };
    int                   ret              = 0;
    int                   result           = 0;
    uint8_t               try_time         = 10;

    uint8_t* ptr_packet  = (uint8_t*)&iap2_sync_packet;
    uint8_t* ptr_header  = (uint8_t*)&iap2_sync_packet.packet_header;
    uint8_t* ptr_payload = (uint8_t*)&iap2_sync_packet.sync_payload;

    uint8_t  size_payload = sizeof(iap2_sync_packet.sync_payload);
    uint16_t size_packet  = sizeof(iap2_sync_packet);

    do {
        result = 0;
        // packet header
        ret = hl_iap2_packet_header_encode(ptr_header, size_packet, LINK_CONTROL_SYN, apple->packet_arg);
        if (ret < 0) {
            result++;
        }
        // packet payload
        ret += hl_iap2_packet_sync_payload_encode(ptr_payload, size_payload);
        if (ret < 0) {
            result++;
        }
        try_time--;
    } while (try_time && result);

    if (!result) {
        memcpy(apple->iap2.send_buffer, ptr_packet, ret);
        apple->usb_write(apple->iap2.send_buffer, (uint16_t)ret);
        ret = 0;
    } else {
        ret = -1;
    }

    return ret;
}

int hl_iap2_link_recv_sync_ack(hl_util_apple_p apple)
{
    st_iap2_sync_packet_t* iap2_sync_packet = NULL;

    int      ret         = 0;
    uint8_t  try_time    = 3;
    uint16_t len         = 0;
    uint16_t size_packet = sizeof(st_iap2_sync_packet_t);

    do {
        ret = apple->usb_read(apple->iap2.recv_buffer, size_packet, TIMEOUT_US);
        try_time--;
    } while (try_time && !ret);

    if (size_packet != ret) {
        apple->log("[ERROR][%s:%d] read syn ack error!\n", __func__, __LINE__);
        return -1;
    }

    try_time = 10;
    do {
        ret = hl_iap2_packet_header_decode(apple->iap2.recv_buffer, &len, LINK_CONTROL_ACK, &apple->packet_arg);
        try_time--;
    } while (try_time && ret);

    if (ret) {
        apple->log("[ERROR][%s:%d] encode ack packet error!\n", __func__, __LINE__);
        return -1;
    }

    apple->packet_arg.ack_num -= 1;

    return 0;
}

int hl_iap2_link_send_ack(hl_util_apple_p apple)
{
    st_iap2_packet_header_t iap2_packet_header = { 0 };

    int      ret        = 0;
    uint8_t  try_time   = 10;
    uint8_t* ptr_header = (uint8_t*)&iap2_packet_header;

    do {
        ret = hl_iap2_packet_header_encode(&iap2_packet_header, PACKET_HEADER_SIZE + 1, LINK_CONTROL_ACK,
                                           apple->packet_arg);
        try_time--;
    } while (try_time && ret < 0);

    if (ret >= 0) {
        memcpy(apple->iap2.send_buffer, ptr_header, ret);
        apple->usb_write(apple->iap2.send_buffer, ret);
        ret = 0;
    } else {
        ret = -1;
    }

    return ret;
}

int hl_iap2_identify_req_auth(hl_util_apple_p apple)
{
    int      ret        = 0;
    uint8_t  try_time   = 3;
    uint16_t len        = 0;
    uint16_t message_id = 0;

    // apple->log("[ERROR][%s:%d]2222222222\n", __func__, __LINE__);
    do {
        ret = apple->usb_read(apple->iap2.recv_buffer, IDENTIFY_FRAME_SIZE, TIMEOUT_US);
        try_time--;
    } while (try_time && !ret);

    if (IDENTIFY_FRAME_SIZE != ret) {
        apple->log("[ERROR][%s:%d] recv auth req error!\n", __func__, __LINE__);
        return -1;
    }

    try_time   = 10;
    message_id = hl_iap2_ctrl_packet_get_message_id(apple->iap2.recv_buffer);

    do {
        ret = hl_iap2_packet_header_decode(apple->iap2.recv_buffer, &len, LINK_CONTROL_ACK, &apple->packet_arg);
        try_time--;
    } while (try_time && ret);

    if (ret || SESSION_ID_REQUESTAUTHCERTIFICATE != message_id) {
        apple->log("[ERROR][%s:%d] auth req messageid or decode error\n", __func__, __LINE__);
        return -1;
    }

    return 0;
}

int hl_iap2_identify_ack_auth(hl_util_apple_p apple)
{
    st_iap2_ctrl_packet_t iap2_ctrl_packet;

    int      ret               = 0;
    uint8_t  try_time          = 3;
    uint8_t  payload_check_sum = 0;
    uint16_t ctrl_packet_len   = 0;
    uint16_t ctrl_message_len  = 0;
    uint16_t ctrl_param_len    = 0;
    uint16_t certification_len = 0;
    uint16_t len               = 0;

    // (1) I2C Read Certification Data Length
    do {
        ret =
            apple->iic_read(CP_ACCESSORY_CERTIFICATION_DATA_LEN, apple->iap2.recv_buffer, sizeof(uint16_t), TIMEOUT_US);
        try_time--;
    } while (try_time && !ret);

    if (sizeof(uint16_t) != ret) {
        apple->log("[ERROR][%s:%d] iic read cert len error!\n", __func__, __LINE__);
        return -1;
    } else {
        certification_len = *((uint16_t*)apple->iap2.recv_buffer);
        certification_len = EXCHANGE_HIGH_LOW_BYTE(certification_len);
    }

    // (2) I2C Read Certification Data
    try_time = 3;
    do {
        ret =
            apple->iic_read(CP_ACCESSORY_CERTIFICATION_DATA_1, apple->iap2.recv_buffer, certification_len, TIMEOUT_US);
        try_time--;
    } while (try_time && !ret);

    if (certification_len != ret) {
        apple->log("\n[ERROR][%s:%d] iic read cert data error!\n", __func__, __LINE__);
        return -2;
    }

    // (3) USB Send Certification Data
    ctrl_packet_len =
        PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE + PACKET_PARAM_HEADER_SIZE + certification_len + 1;
    ctrl_message_len = PACKET_CTRL_HEADER_SIZE + PACKET_PARAM_HEADER_SIZE + certification_len;
    ctrl_param_len   = PACKET_PARAM_HEADER_SIZE + certification_len;

    // packet header
    try_time = 10;
    do {
        ret = hl_iap2_packet_header_encode(&iap2_ctrl_packet.packet_header, ctrl_packet_len, LINK_CONTROL_ACK,
                                           apple->packet_arg);
        try_time--;
    } while (try_time && ret < 0);

    if (ret >= 0) {
        len = ret;
    } else {
        return -3;
    }

    // packet payload
    try_time = 10;
    do {
        ret = hl_iap2_ctrl_payload_encode(&iap2_ctrl_packet.ctrl_payload, ctrl_message_len,
                                          SESSION_ID_AUTHENTICATION_CERTIFICATE);
        try_time--;
    } while (try_time && ret < 0);

    if (ret >= 0) {
        len += ret;
    } else {
        return -4;
    }

    memcpy(apple->iap2.send_buffer, &iap2_ctrl_packet, len);

    // packet param
    try_time = 10;
    do {
        ret = hl_iap2_ctrl_add_param(apple->iap2.send_buffer + len, ctrl_param_len, 0x00, apple->iap2.recv_buffer);
        try_time--;
    } while (try_time && ret < 0);

    if (ret > 0) {
        len += ret;
    } else {
        return -5;
    }

    // payload checksum
    payload_check_sum              = hl_checksum_calculation(apple->iap2.send_buffer, PACKET_HEADER_SIZE + 1,
                                                ctrl_packet_len - PACKET_HEADER_SIZE - 2);
    apple->iap2.send_buffer[len++] = payload_check_sum;

    apple->usb_write(apple->iap2.send_buffer, len);

    return 0;
}

int hl_iap2_identify_req_challenge(hl_util_apple_p apple)
{
    int      ret        = 0;
    uint8_t  try_time   = 5;
    uint8_t  len        = 0;
    uint16_t message_id = 0;

    // Request Authentication Challenge Response
    do {
        ret = apple->usb_read(apple->iap2.recv_buffer, 52, TIMEOUT_US);
        try_time--;
    } while (try_time && 52 != ret);

    if (52 != ret) {
        apple->log("\n[ERROR][%s:%d] usb read challenge error!\n", __func__, __LINE__);
        return -1;
    }

    message_id = hl_iap2_ctrl_packet_get_message_id(apple->iap2.recv_buffer);
    if (SESSION_ID_REQUEST_AUTH_CHALLENG_RESP != message_id) {
        apple->log("\n[ERROR][%s:%d] challenge resp messageid error!\n", __func__, __LINE__);
        return -2;
    }

    ret = hl_iap2_packet_header_decode(apple->iap2.recv_buffer, &len, LINK_CONTROL_ACK, &apple->packet_arg);
    if (ret) {
        apple->log("\n[ERROR][%s:%d] decode req challenge message error!\n", __func__, __LINE__);
        return -3;
    }

    apple->iap2.challenge_resp_data = &apple->iap2.recv_buffer[19];
    apple->iap2.challenge_req_len   = hl_iap2_ctrl_packet_get_param_len(apple->iap2.recv_buffer);
    apple->log("\r\n%s:%d:->%d:%04x:%04x\r\n", __func__, __LINE__, ret, message_id,
               SESSION_ID_REQUEST_AUTH_CHALLENG_RESP);

    return 0;
}

int hl_iap2_identify_ack_challenge(hl_util_apple_p apple)
{
    st_iap2_ctrl_packet_t iap2_ctrl_packet = { 0 };

    int      ret               = 0;
    uint8_t  payload_check_sum = 0;
    uint16_t ctrl_packet_len   = 0;
    uint16_t ctrl_message_len  = 0;
    uint16_t ctrl_param_len    = 0;
    uint16_t len               = 0;

    // (1) Request Challenge Response
    ret = hl_iap2_challange_response_process(apple);
    if (ret) {
        apple->log("[ERROR][%s:%d] ret val = %d\n", __func__, __LINE__, ret);
        return -1;
    }

    apple->log("[OK]Finish hl_iap2_challange_response_process\n");

    // (2) Authentication Response
    ctrl_packet_len = PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE + PACKET_PARAM_HEADER_SIZE
                      + apple->iap2.challenge_resp_len + 1;
    ctrl_message_len = PACKET_CTRL_HEADER_SIZE + PACKET_PARAM_HEADER_SIZE + apple->iap2.challenge_resp_len;
    ctrl_param_len   = PACKET_PARAM_HEADER_SIZE + apple->iap2.challenge_resp_len;

    // packet header
    ret = hl_iap2_packet_header_encode(&iap2_ctrl_packet.packet_header, ctrl_packet_len, LINK_CONTROL_ACK,
                                       apple->packet_arg);
    if (ret >= 0) {
        len = ret;
    } else {
        return -2;
    }

    // packet payload
    ret = hl_iap2_ctrl_payload_encode(&iap2_ctrl_packet.ctrl_payload, ctrl_message_len, SESSION_ID_AUTH_RESPONSE);
    if (ret >= 0) {
        len += ret;
    } else {
        return -3;
    }

    memcpy(apple->iap2.send_buffer, &iap2_ctrl_packet, len);
    // packet param
    ret = hl_iap2_ctrl_add_param(apple->iap2.send_buffer + len, ctrl_param_len, 0x00, apple->iap2.recv_buffer);
    if (ret > 0) {
        len += ret;
    } else {
        return -4;
    }

    // payload checksum
    payload_check_sum              = hl_checksum_calculation(apple->iap2.send_buffer, PACKET_HEADER_SIZE + 1,
                                                ctrl_packet_len - PACKET_HEADER_SIZE - 2);
    apple->iap2.send_buffer[len++] = payload_check_sum;

    // USB Send Challenge Ack
    apple->usb_write(apple->iap2.send_buffer, len);

    return 0;
}

int hl_iap2_identify_succedd(hl_util_apple_p apple)
{
    if (NULL == apple) {
        apple->log("[ERROR][%s:%d]iap2 is null!\n", __func__, __LINE__);
        return -5;
    }

    int     ret      = 0;
    uint8_t try_time = 5;
    uint8_t len      = 0;

    // apple->log("[ERROR][%s:%d]111111\n", __func__, __LINE__);
    do {
        ret = apple->usb_read(apple->iap2.recv_buffer, IDENTIFY_FRAME_SIZE, TIMEOUT_US);
        try_time--;
    } while (try_time && !ret);

    if (IDENTIFY_FRAME_SIZE != ret) {
        apple->log("[ERROR][%s:%d] usb read identify succedd error\n", __func__, __LINE__);
        return -4;
    }

    try_time = 5;
    do {
        ret = hl_iap2_packet_header_decode(apple->iap2.recv_buffer, &len, LINK_CONTROL_ACK, &apple->packet_arg);
        // apple->log("[%d] 000\n", ret);
        try_time--;
    } while (try_time && ret);

    if (!ret) {
        apple->packet_arg.ack_num -= 1;
    }
    // apple->log("[ERROR][%s:%d]2222222[%d]\n", __func__, __LINE__, ret);
    apple->iap2.identify_status = EM_HL_IAP2_STM_IDENTIFY_START_IDENTIFICATION;

    return ret;
}

int hl_iap2_identify_start_identification(hl_util_apple_p apple)
{
    st_iap2_packet_header_t iap2_ack_packet;

    int      ret            = 0;
    uint8_t  try_time       = 5;
    uint8_t  ack_packet_len = PACKET_HEADER_SIZE + 1;
    uint16_t len            = 0;
    uint16_t message_id     = 0;

    // read start identification message
    do {
        ret = apple->usb_read(apple->iap2.recv_buffer, IDENTIFY_FRAME_SIZE, TIMEOUT_US);
        try_time--;
    } while (try_time && IDENTIFY_FRAME_SIZE != ret);

    if (IDENTIFY_FRAME_SIZE != ret) {
        apple->log("[ERROR][%s:%d] recv start indentification error\n", __func__, __LINE__);
        return -1;
    }

    do {
        ret = hl_iap2_packet_header_decode(apple->iap2.recv_buffer, &len, LINK_CONTROL_ACK, &apple->packet_arg);
        // apple->log("[%d] 4444\n", ret);
        try_time--;
    } while (try_time && ret);

    message_id = hl_iap2_ctrl_packet_get_message_id(apple->iap2.recv_buffer);
    if (ret) {
        apple->log("[ERROR][%s:%d] decode start indentification message error\n", __func__, __LINE__);
        return -2;
    }
    if (SESSION_ID_START_IDENTIFICATION != message_id) {
        apple->log("[ERROR][%s:%d] start indentification messageid error\n", __func__, __LINE__);
        return -3;
    }

    // encode packet for ack identify succedd message
    ret = hl_iap2_packet_header_encode(&iap2_ack_packet, ack_packet_len, LINK_CONTROL_ACK, apple->packet_arg);
    if (ret >= 0) {
        len = ret;
    } else {
        apple->log("[ERROR][%s:%d] encode error\n", __func__, __LINE__, ret);
        return -4;
    }

    // send identify succedd message ack
    memcpy(apple->iap2.send_buffer, &iap2_ack_packet, len);
    apple->usb_write(apple->iap2.send_buffer, ack_packet_len);

    return 0;
}

int hl_iap2_identify_identification_info(hl_util_apple_p apple)
{
    st_iap2_ctrl_packet_t iap2_ctrl_packet;

    uint8_t data_1[4] = { 0xae, 0x00, 0xae, 0x02 };
    uint8_t data_2[6] = { 0xae, 0x01, 0xea, 0x00, 0xea, 0x01 };
    uint8_t data_3    = 0x00;
    uint8_t data_4[2] = { 0x00, 0x64 };
    uint8_t data_5[2] = { 0x00, 0x01 };
    uint8_t data_6    = 0x01;
    uint8_t data_7    = 0x02;

    int      ret               = 0;
    uint8_t  payload_check_sum = 0;
    uint16_t ctrl_packet_len   = 0;
    uint16_t ctrl_message_len  = 0;
    uint16_t param_len         = 0;
    uint16_t temp_len          = 0;

    uint8_t* addr = apple->iap2.send_buffer + PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE;

    // packet param < 1 >
    param_len += hl_iap2_ctrl_add_param(addr + param_len, 5 + strlen(IAP2_DEVICE_NAME), 0x0000, IAP2_DEVICE_NAME);
    addr[param_len] = '\0';

    // packet param < 2 >
    param_len += hl_iap2_ctrl_add_param(addr + param_len, 5 + strlen(IAP2_MODEID), 0x0001, IAP2_MODEID);
    addr[param_len] = '\0';

    // packet param < 3 >
    param_len += hl_iap2_ctrl_add_param(addr + param_len, 5 + strlen(IAP2_MANUFATORY), 0x0002, IAP2_MANUFATORY);
    addr[param_len] = '\0';

    // packet param < 4 >
    param_len +=
        hl_iap2_ctrl_add_param(addr + param_len, 5 + strlen(apple->iap2.dev_sn), 0x0003, (uint8_t*)apple->iap2.dev_sn);
    addr[param_len] = '\0';

    // packet param < 5 >
    param_len +=
        hl_iap2_ctrl_add_param(addr + param_len, 5 + strlen(IAP2_FIRMWAREVERSION), 0x0004, IAP2_FIRMWAREVERSION);
    addr[param_len] = '\0';

    // packet param < 6 >
    param_len +=
        hl_iap2_ctrl_add_param(addr + param_len, 5 + strlen(IAP2_HARDWAREVERSION), 0x0005, IAP2_HARDWAREVERSION);
    addr[param_len] = '\0';

    // packet param < 8 >
    param_len += hl_iap2_ctrl_add_param(addr + param_len, 4 + sizeof(data_1), 0x0006, data_1);

    // packet param < 9 >
    param_len += hl_iap2_ctrl_add_param(addr + param_len, 4 + sizeof(data_2), 0x0007, data_2);

    // packet param < 10 >
    param_len += hl_iap2_ctrl_add_param(addr + param_len, 4 + sizeof(data_3), 0x0008, &data_3);

    // packet param < 11 >
    param_len += hl_iap2_ctrl_add_param(addr + param_len, 4 + sizeof(data_4), 0x0009, data_4);

    // packet param < 11 >
    temp_len = param_len;
    param_len += 4;
    param_len += hl_iap2_ctrl_add_param(addr + param_len, 4 + sizeof(data_6), 0x0000, &data_6);
    param_len += hl_iap2_ctrl_add_param(addr + param_len, 5 + strlen(IAP2_EXTERNAL_ACCESSORY_PROTOCOL_NAME), 0x0001,
                                        IAP2_EXTERNAL_ACCESSORY_PROTOCOL_NAME);
    addr[param_len] = '\0';
    param_len += hl_iap2_ctrl_add_param(addr + param_len, 4 + sizeof(data_7), 0x0002, &data_7);
    hl_iap2_ctrl_add_param(addr + temp_len, param_len - temp_len, 0x000a, NULL);

    // packet param < 12 >
    param_len +=
        hl_iap2_ctrl_add_param(addr + param_len, 5 + strlen(IAP2_APP_MATCH_TEAM_ID), 0x000b, IAP2_APP_MATCH_TEAM_ID);
    addr[param_len] = '\0';

    // packet param < 12 >
    param_len += hl_iap2_ctrl_add_param(addr + param_len, 5 + strlen("en"), 0x000c, "en");
    addr[param_len] = '\0';

    // packet param < 13 >
    param_len += hl_iap2_ctrl_add_param(addr + param_len, 5 + strlen("en"), 0x000d, "en");
    addr[param_len] = '\0';

    // packet param < 14 >
    hl_iap2_ctrl_add_param(addr + param_len, 0x002c, 0x0010, NULL);
    param_len += 4;

    // packet param < 16 >
    param_len += hl_iap2_ctrl_add_param(addr + param_len, 4 + sizeof(data_5), 0x0000, data_5);

    // packet param < 15 >
    param_len +=
        hl_iap2_ctrl_add_param(addr + param_len, 5 + strlen(IAP2_UHOST_COMPONENT), 0x0001, IAP2_UHOST_COMPONENT);
    addr[param_len] = '\0';

    // packet param < 17 >
    param_len += hl_iap2_ctrl_add_param(addr + param_len, 0x0004, 0x0002, NULL);

    // packet param < 7 >
    param_len += hl_iap2_ctrl_add_param(addr + param_len, 5 + strlen(IAP2_PUID), 0x0022, IAP2_PUID);
    addr[param_len] = '\0';

    ctrl_packet_len  = PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE + param_len + 1;
    ctrl_message_len = PACKET_CTRL_HEADER_SIZE + param_len;

    // packet header
    ret = hl_iap2_packet_header_encode(&iap2_ctrl_packet, ctrl_packet_len, LINK_CONTROL_ACK, apple->packet_arg);
    if (ret < 0) {
        apple->log("[ERROR][%s:%d] packet header encode\n", __func__, __LINE__);
        return -1;
    }

    // packet payload
    ret = hl_iap2_ctrl_payload_encode(&iap2_ctrl_packet.ctrl_payload, ctrl_message_len, SESSION_ID_IDENTIFICATION_INFO);
    if (ret < 0) {
        apple->log("[ERROR][%s:%d] packet payload encode\n", __func__, __LINE__);
        return -2;
    }
    memcpy(apple->iap2.send_buffer, &iap2_ctrl_packet, PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE);

    // payload checksum
    payload_check_sum = hl_checksum_calculation(apple->iap2.send_buffer, PACKET_HEADER_SIZE + 1, ctrl_message_len);
    apple->iap2.send_buffer[ctrl_packet_len - 1] = payload_check_sum;

    apple->usb_write(apple->iap2.send_buffer, ctrl_packet_len);

    return 0;
}

int hl_iap2_identify_identification_accepted(hl_util_apple_p apple)
{
    int              ret        = 0;
    uint8_t          try_time   = 3;
    uint16_t         len        = 0;
    uint16_t         message_id = 0;
    volatile uint8_t buf[128]   = { 0 };

    do {
        ret = apple->usb_read(buf, IDENTIFY_FRAME_SIZE, TIMEOUT_US);
        try_time--;
    } while (try_time && IDENTIFY_FRAME_SIZE != ret);

    if (IDENTIFY_FRAME_SIZE != ret) {
        apple->log("[ERROR][%s:%d] usb read identify accept\n", __func__, __LINE__);
        return -1;
    }

    try_time = 5;
    do {
        ret = hl_iap2_packet_header_decode(buf, &len, LINK_CONTROL_ACK, &apple->packet_arg);
        // apple->log("[%d] 555\n", ret);
        try_time--;
    } while (try_time && ret);

    message_id = hl_iap2_ctrl_packet_get_message_id(buf);

    // apple->log("\r\n%s:%d:ret = %d |  message_id = %02X\r\n", __func__, __LINE__, ret, message_id);
    if (ret || SESSION_ID_IDEN_ACCEPTED != message_id) {
        apple->log("[ERROR][%s:%d] receive message id\n", __func__, __LINE__);
        return -2;
    }

    return 0;
}

int hl_iap2_powerupdate_send_power(hl_util_apple_p apple)
{
    st_iap2_ctrl_packet_t iap2_ctrl_packet;

    int      result            = 0;
    uint8_t  ret               = 0;
    uint8_t  param_len         = 0;
    uint8_t  payload_check_sum = 0;
    uint16_t ctrl_packet_len   = 0;
    uint16_t ctrl_message_len  = 0;

    uint8_t* addr = apple->iap2.send_buffer + PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE;

    // // packet param < 1 >
    // param_len += hl_iap2_ctrl_add_param(addr+param_len, 0x0004, 0x0000, NULL);

    // // packet param < 2 >
    // param_len += hl_iap2_ctrl_add_param(addr+param_len, 0x0004, 0x0001, NULL);

    // // packet param < 2 >
    param_len += hl_iap2_ctrl_add_param(addr + param_len, 0x0004, 0x0002, NULL);

    // // packet param < 3 >
    // param_len += hl_iap2_ctrl_add_param(addr+param_len, 0x0004, 0x0004, NULL);

    // packet param < 4 >
    param_len += hl_iap2_ctrl_add_param(addr + param_len, 0x0004, 0x0005, NULL);

    // // packet param < 5 >
    // param_len += hl_iap2_ctrl_add_param(addr+param_len, 0x0004, 0x0006, NULL);

    ctrl_packet_len  = PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE + param_len + 1;
    ctrl_message_len = PACKET_CTRL_HEADER_SIZE + param_len;

    // packet header
    ret = hl_iap2_packet_header_encode(&iap2_ctrl_packet, ctrl_packet_len, LINK_CONTROL_ACK, apple->packet_arg);
    if (ret < 0) {
        apple->log("[ERROR][hl_iap2_powerupdate_send_power] packet header encode\n");
        result = -1;
    }

    // packet payload
    ret = hl_iap2_ctrl_payload_encode(&iap2_ctrl_packet.ctrl_payload, ctrl_message_len, MESSAGE_ID_START_POWER_UPDATES);
    if (ret < 0) {
        apple->log("[ERROR][hl_iap2_powerupdate_send_power] packet payload encode\n");
        result = -1;
    }

    memcpy(apple->iap2.send_buffer, &iap2_ctrl_packet, PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE);

    // payload checksum
    payload_check_sum = hl_checksum_calculation(apple->iap2.send_buffer, PACKET_HEADER_SIZE + 1, ctrl_message_len);
    apple->iap2.send_buffer[ctrl_packet_len - 1] = payload_check_sum;

    apple->usb_write(apple->iap2.send_buffer, ctrl_packet_len);

    return result;
}

int hl_iap2_powerupdate_recv_update(hl_util_apple_p apple)
{
    int      result     = -1;
    uint8_t  ret        = 0;
    uint8_t  len        = 0;
    uint16_t param_id   = 0;
    uint16_t message_id = 0;

    // recv power update id=0x0002
    do {
        ret = apple->usb_read(apple->iap2.recv_buffer, POWERUPDATE_FRAME_SIZE, TIMEOUT_US);
        apple->log("\r\n%s:%d:111 ret = %d\r\n", __func__, __LINE__, ret);
        if (ret == POWERUPDATE_FRAME_SIZE) {
            message_id = hl_iap2_ctrl_packet_get_message_id(apple->iap2.recv_buffer);
            param_id   = hl_iap2_ctrl_packet_get_param_id(apple->iap2.recv_buffer);
            apple->log("\r\n%s:%d:222 message_id = %02X\r\n", __func__, __LINE__, message_id);
            apple->log("\r\n%s:%d:222 param_id = %02X\r\n", __func__, __LINE__, param_id);
            ret =
                hl_iap2_packet_header_decode(apple->iap2.recv_buffer, &len, LINK_CONTROL_ACK, &apple->packet_arg);
            if (ret != 0 || message_id != MESSAGE_ID_POWER_UPDATE || param_id != 0x0002) {
                apple->log("[%d] recv power update id:0x0002\n", __LINE__);
                result = -1;
            } else {
                break;
            }
        }
    } while (1);
    apple->log("\r\n%s:%d:222\r\n", __func__, __LINE__);

    apple->packet_arg.ack_num -= 1;

    // // recv power update id=0x0004
    // ret = apple->usb_read(apple->iap2.recv_buffer, POWERUPDATE_FRAME_SIZE, TIMEOUT_US);
    // if (ret == POWERUPDATE_FRAME_SIZE) {
    //     message_id = hl_iap2_ctrl_packet_get_message_id(apple->iap2.recv_buffer);
    //     param_id   = hl_iap2_ctrl_packet_get_param_id(apple->iap2.recv_buffer);
    //     ret        = hl_iap2_packet_header_decode(apple->iap2.recv_buffer, &len, LINK_CONTROL_ACK, &apple->packet_arg);
    //     if (ret != 0 || message_id != MESSAGE_ID_POWER_UPDATE || param_id != 0x0004) {
    //         apple->log("[ERROR][hl_iap2_powerupdate_recv_update] recv power update id:0x0004\n");
    //         result = -1;
    //     }
    // }

    // recv power update id=0x0005
    do {
        ret = apple->usb_read(apple->iap2.recv_buffer, POWERUPDATE_FRAME_SIZE, TIMEOUT_US);
        apple->log("\r\n%s:%d:333 ret = %d\r\n", __func__, __LINE__, ret);
        if (ret == POWERUPDATE_FRAME_SIZE) {
            message_id = hl_iap2_ctrl_packet_get_message_id(apple->iap2.recv_buffer);
            param_id   = hl_iap2_ctrl_packet_get_param_id(apple->iap2.recv_buffer);
            apple->log("\r\n%s:%d:333 message_id = %02X\r\n", __func__, __LINE__, message_id);
            apple->log("\r\n%s:%d:444 param_id = %02X\r\n", __func__, __LINE__, param_id);
            ret =
                hl_iap2_packet_header_decode(apple->iap2.recv_buffer, &len, LINK_CONTROL_ACK, &apple->packet_arg);
            if (ret != 0 || message_id != MESSAGE_ID_POWER_UPDATE || param_id != 0x0005) {
                apple->log("[%d] recv power update id:0x0005\n", __LINE__);
                result = -1;
            } else {
                break;
            }
        }
    } while (1);
    apple->log("\r\n%s:%d:444\r\n", __func__, __LINE__);

    // // recv power update id=0x0006
    // ret = apple->usb_read(apple->iap2.recv_buffer, POWERUPDATE_FRAME_SIZE, TIMEOUT_US);
    // if (ret == POWERUPDATE_FRAME_SIZE) {
    //     message_id = hl_iap2_ctrl_packet_get_message_id(apple->iap2.recv_buffer);
    //     param_id   = hl_iap2_ctrl_packet_get_param_id(apple->iap2.recv_buffer);
    //     ret        = hl_iap2_packet_header_decode(apple->iap2.recv_buffer, &len, LINK_CONTROL_ACK, &apple->packet_arg);
    //     if (ret != 0 || message_id != MESSAGE_ID_POWER_UPDATE || param_id != 0x0006) {
    //         apple->log("[ERROR][hl_iap2_powerupdate_recv_update] recv power update id:0x0006\n");
    //         result = -1;
    //     }
    // }

    hl_iap2_link_send_ack(apple);

    return result;
}

int hl_iap2_powerupdate_send_power_source(hl_util_apple_p apple)
{
    st_iap2_ctrl_packet_t iap2_ctrl_packet;

    int      result            = 0;
    uint8_t  ret               = 0;
    uint8_t  param_len         = 0;
    uint8_t  payload_check_sum = 0;
    uint16_t ctrl_packet_len   = 0;
    uint16_t ctrl_message_len  = 0;

    uint8_t  DeviceBatteryShouldChargeIfPowerIsPresent = 0x00;
    uint16_t AvailableCurrentForDeviceVal              = EXCHANGE_HIGH_LOW_BYTE(0x0000);

    uint8_t* addr = apple->iap2.send_buffer + PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE;

    // packet param < 1 >
    param_len += hl_iap2_ctrl_add_param(addr + param_len, 0x0006, 0x0000, &AvailableCurrentForDeviceVal);

    // packet param < 2 >
    param_len += hl_iap2_ctrl_add_param(addr + param_len, 0x0005, 0x0001, &DeviceBatteryShouldChargeIfPowerIsPresent);

    ctrl_packet_len  = PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE + param_len + 1;
    ctrl_message_len = PACKET_CTRL_HEADER_SIZE + param_len;

    // packet header
    ret = hl_iap2_packet_header_encode(&iap2_ctrl_packet, ctrl_packet_len, LINK_CONTROL_ACK, apple->packet_arg);
    if (ret < 0) {
        apple->log("[ERROR][hl_iap2_powerupdate_send_power_source] packet header encode\n");
        result = -1;
    }

    // packet payload
    ret = hl_iap2_ctrl_payload_encode(&iap2_ctrl_packet.ctrl_payload, ctrl_message_len, MESSAGE_ID_POWEERSOURCE_UPDATE);
    if (ret < 0) {
        apple->log("[ERROR][hl_iap2_powerupdate_send_power_source] packet payload encode\n");
        result = -1;
    }

    memcpy(apple->iap2.send_buffer, &iap2_ctrl_packet, PACKET_HEADER_SIZE + 1 + PACKET_CTRL_HEADER_SIZE);
    // payload checksum

    payload_check_sum = hl_checksum_calculation(apple->iap2.send_buffer, PACKET_HEADER_SIZE + 1, ctrl_message_len);
    apple->iap2.send_buffer[ctrl_packet_len - 1] = payload_check_sum;

    // apple->usb_write(apple->iap2.send_buffer, ctrl_packet_len);
    ret = apple->usb_read(apple->iap2.recv_buffer, 64, TIMEOUT_US);

    return result;
}

int hl_iap2_eap_check_power_insert(hl_util_apple_p apple)
{
    if (NULL == apple) {
        apple->log("[hl_iap2_eap_check_power_insert] error parameter!\n");
        return -1;
    }

    return 0;
}
