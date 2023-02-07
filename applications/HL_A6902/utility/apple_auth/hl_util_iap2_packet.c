#include "hl_util_iap2_packet.h"
// #include "rtthread.h"

uint8_t hl_checksum_calculation(uint8_t* buffer, uint16_t start, uint16_t len)
{
    uint8_t  sum = 0;
    uint16_t i   = 0;

    for (i = start; i < (start + len); i++) {
        sum += buffer[i];
    }

    return (uint8_t)(0x100 - sum);
}

int hl_iap2_detect_packet_encode(uint8_t* write_data_addr)
{
    if (write_data_addr == NULL) {
        // printf("NULL Point\n");
        return -1;
    }

    write_data_addr[0] = 0xFF;
    write_data_addr[1] = 0x55;
    write_data_addr[2] = 0x02;
    write_data_addr[3] = 0x00;
    write_data_addr[4] = 0xEE;
    write_data_addr[5] = 0x10;

    return DETECT_FRAME_SIZE;
}

int hl_iap2_detect_packet_decode(uint8_t* read_data_addr)
{
    if (read_data_addr == NULL) {
        return -1;
    }

    int     result        = 0;
    uint8_t detect_arr[6] = { 0xFF, 0x55, 0x02, 0x00, 0xEE, 0x10 };

    result = strncmp(detect_arr, read_data_addr, DETECT_FRAME_SIZE);
    return result;
}

int hl_iap2_packet_header_encode(st_iap2_packet_header_t* packet_header, uint16_t packet_len, uint8_t ctrl_byte,
                                 st_packet_header_arg header_arg)
{
    if (packet_header == NULL) {
        return -1;
    }

    packet_header->StartOfPacket  = EXCHANGE_HIGH_LOW_BYTE(0xFF5A);
    packet_header->PacketLength   = EXCHANGE_HIGH_LOW_BYTE(packet_len);
    packet_header->ControlByte    = ctrl_byte;
    packet_header->PacketSeqNum   = header_arg.ack_num + 1;
    packet_header->PacketAckNum   = header_arg.seq_num;
    packet_header->SessionIdentif = header_arg.session_id;
    packet_header->HeaderCksum    = hl_checksum_calculation(packet_header, 0, PACKET_HEADER_SIZE);

    return PACKET_HEADER_SIZE + 1;
}

int hl_iap2_packet_header_decode(uint8_t* packet_header, uint16_t* packet_len, uint8_t ctrl_byte,
                                 st_packet_header_arg* header_arg)
{
    if (packet_header == NULL) {
        return -1;
    }

    st_iap2_packet_header_t* ptr_header = (st_iap2_packet_header_t*)packet_header;
    // rt_kprintf("\nctrl_byte[%d][%d]\n", ptr_header->ControlByte, ctrl_byte);
    // rt_kprintf("seq_num[%d][%d]\n", ptr_header->PacketSeqNum, header_arg->seq_num);
    // rt_kprintf("ack_num[%d][%d]\n", ptr_header->PacketAckNum, header_arg->ack_num);

    if (!(ptr_header->ControlByte & ctrl_byte)) {
        return -2;
    } else if (ptr_header->PacketAckNum != (header_arg->ack_num + 1)) {
        return -3;
    } else {
        *packet_len            = ptr_header->PacketLength;
        header_arg->seq_num    = ptr_header->PacketSeqNum;
        header_arg->ack_num    = ptr_header->PacketAckNum;
        header_arg->session_id = ptr_header->SessionIdentif;
    }

    return 0;
}

int hl_iap2_packet_sync_payload_encode(st_iap2_sync_payload_t* packet_payload, uint8_t payload_len)
{
    if (packet_payload == NULL) {
        // printf("NULL Point\n");
        return -1;
    }

    packet_payload->LinkVersion               = 0x01;
    packet_payload->MaxNumOfOutstandingPacket = 0x04;
    packet_payload->MaxReceivedPacketLen      = EXCHANGE_HIGH_LOW_BYTE(0x0200);
    packet_payload->RetransmitTimeout         = EXCHANGE_HIGH_LOW_BYTE(0x07D0);
    packet_payload->CumulativeAckTimeout      = EXCHANGE_HIGH_LOW_BYTE(0x03E8);
    packet_payload->MaxNumOfRetransmit        = 0x03;
    packet_payload->MaxCumulativeAckNum       = 0x03;
    packet_payload->SessionId1                = LINK_SESSION1_ID;
    packet_payload->SessionType1              = LINK_SESSION1_TYPE;
    packet_payload->SessionVer1               = LINK_SESSION1_VER;
    packet_payload->SessionId2                = LINK_SESSION2_ID;
    packet_payload->SessionType2              = LINK_SESSION2_TYPE;
    packet_payload->SessionVer2               = LINK_SESSION2_VER;
    packet_payload->PayloadCksum              = hl_checksum_calculation(packet_payload, 0, payload_len - 1);

    return payload_len;
}

int hl_iap2_ctrl_packet_get_message_id(uint8_t* data_addr)
{
    if (data_addr == NULL) {
        // printf("NULL Point\n");
        return -1;
    }

    st_iap2_ctrl_packet_t* ptr_packet = (st_iap2_ctrl_packet_t*)data_addr;

    uint16_t message_id = EXCHANGE_HIGH_LOW_BYTE(ptr_packet->ctrl_payload.MessageId);

    return message_id;
}

int hl_iap2_ctrl_packet_get_param_len(uint8_t* data_addr)
{
    if (data_addr == NULL) {
        // printf("NULL Point\n");
        return -1;
    }

    st_iap2_ctrl_packet_t* ptr_packet = (st_iap2_ctrl_packet_t*)data_addr;
    return EXCHANGE_HIGH_LOW_BYTE(ptr_packet->ctrl_payload.ParamLen) - 4;
}

int hl_iap2_ctrl_packet_get_param_id(uint8_t* data_addr)
{
    if (data_addr == NULL) {
        // printf("NULL Point\n");
        return -1;
    }

    st_iap2_ctrl_packet_t* ptr_packet = (st_iap2_ctrl_packet_t*)data_addr;
    return EXCHANGE_HIGH_LOW_BYTE(ptr_packet->ctrl_payload.ParamId);
}

int hl_iap2_ctrl_payload_encode(st_iap2_ctrl_payload_t* packet_payload, uint16_t message_len, uint16_t message_id)
{
    if (packet_payload == NULL) {
        // printf("NULL Point\n");
        return -1;
    }

    packet_payload->StartOfMessage = EXCHANGE_HIGH_LOW_BYTE(0x4040);
    packet_payload->MessageLen     = EXCHANGE_HIGH_LOW_BYTE(message_len);
    packet_payload->MessageId      = EXCHANGE_HIGH_LOW_BYTE(message_id);

    return PACKET_CTRL_HEADER_SIZE;
}

uint16_t hl_iap2_ctrl_add_param(uint8_t* write_addr, uint16_t param_len, uint16_t param_id, uint8_t* write_data)
{
    if (write_addr == NULL) {
        // printf("NULL Point\n");
        return -1;
    }

    st_iap2_ctrl_param_t* ptr_ctrl_param = (st_iap2_ctrl_param_t*)write_addr;
    ptr_ctrl_param->Len                  = EXCHANGE_HIGH_LOW_BYTE(param_len);
    ptr_ctrl_param->Id                   = EXCHANGE_HIGH_LOW_BYTE(param_id);
    if (write_data != NULL) {
        memcpy(&ptr_ctrl_param->Data, write_data, param_len - 4);
    }

    return param_len;
}
