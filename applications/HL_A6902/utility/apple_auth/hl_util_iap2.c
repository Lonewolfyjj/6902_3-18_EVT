#include "hl_util_iap2.h"
#include <stdlib.h>

/**
 * _hl_iap2_detect_process
 * @brief detect状态机处理
 * @param [in] iap2 iap2句柄
 * @return int 成功 0 | 失败 非0
 * @date 2022-09-01
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lisonglin     <td>新建
 * </table>
 */
static int _hl_iap2_detect_process(st_iap2_protocol_p iap2)
{
    uint8_t  result = -1;
    uint16_t len    = 0;

    switch (iap2->detect_status) {
        case EM_HL_IAP2_STM_DETECT_SEND:
            result              = hl_iap2_detect_send(iap2);
            iap2->detect_status = EM_HL_IAP2_STM_DETECT_RECV;
            break;

        case EM_HL_IAP2_STM_DETECT_RECV:
            result              = hl_iap2_detect_recv(iap2);
            iap2->detect_status = EM_HL_IAP2_STM_DETECT_SEND;

            if (result == 0) {
                iap2->iap2_printf("[OK] [_hl_iap2_detect_process] receive detect message!\n");
                iap2->main_status = EM_HL_IAP2_STM_MAIN_LINK;
            } else {
                iap2->iap2_printf("[ERROR] [_hl_iap2_detect_process] receive detect message!\n");
                iap2->main_status = EM_HL_IAP2_STM_MAIN_FAILED;
            }
            break;

        default:
            break;
    }

    return result;
}

/**
 * _hl_iap2_link_process
 * @brief link状态机处理
 * @param [in] iap2 iap2句柄
 * @return int 成功 0 | 失败 非0
 * @date 2022-09-01
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lisonglin     <td>新建
 * </table>
 */
static int _hl_iap2_link_process(st_iap2_protocol_p iap2)
{
    uint8_t result = -1;

    switch (iap2->link_status) {
        case EM_HL_IAP2_STM_LINK_SEND_SYN:
            result            = hl_iap2_link_send_sync(iap2);
            iap2->link_status = EM_HL_IAP2_STM_LINK_RECV_SYN_ACK;
            break;

        case EM_HL_IAP2_STM_LINK_RECV_SYN_ACK:
            result = hl_iap2_link_recv_sync_ack(iap2);

            while (result != 0) {
                result = hl_iap2_link_recv_sync_ack(iap2);
                iap2->delay_usec_func(1000);
            }
            
            if (result == 0) {
                iap2->iap2_printf("[OK] [_hl_iap2_link_process] receive link message!\n");
                iap2->link_status = EM_HL_IAP2_STM_LINK_SEND_ACK;
            } else {
                iap2->iap2_printf("[ERROR] [_hl_iap2_link_process] receive link message!\n");
                iap2->main_status = EM_HL_IAP2_STM_MAIN_FAILED;
                iap2->link_status = EM_HL_IAP2_STM_LINK_SEND_SYN;
            }
            break;

        case EM_HL_IAP2_STM_LINK_SEND_ACK:
            result            = hl_iap2_link_send_ack(iap2);
            iap2->main_status = EM_HL_IAP2_STM_MAIN_IDENTIFY;
            iap2->link_status = EM_HL_IAP2_STM_LINK_SEND_SYN;
            break;

        default:
            break;
    }

    return result;
}

/**
 * _hl_iap2_identify_process
 * @brief identify状态机处理
 * @param [in] iap2 iap2句柄
 * @return int 成功 0 | 失败 非0
 * @date 2022-09-01
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lisonglin     <td>新建
 * </table>
 */
static int _hl_iap2_identify_process(st_iap2_protocol_p iap2)
{
    uint8_t result = 0;

    switch (iap2->identify_status) {
        case EM_HL_IAP2_STM_IDENTIFY_REQ_AUTH:
            result = hl_iap2_identify_req_auth(iap2);

            while (result != 0) {
                result = hl_iap2_identify_req_auth(iap2);
                // iap2->delay_usec_func(1000);
            }

            if (result == 0) {
                iap2->iap2_printf("[OK] [_hl_iap2_identify_process] receive req auth message!\n");
                iap2->identify_status = EM_HL_IAP2_STM_IDENTIFY_ACK_AUTH;
            } else {
                iap2->iap2_printf("[ERROR] [_hl_iap2_identify_process] receive req auth message!\n");
                iap2->main_status = EM_HL_IAP2_STM_MAIN_FAILED;
            }
            iap2->iap2_printf("[TEST] [_hl_iap2_identify_process]!\n");
            break;

        case EM_HL_IAP2_STM_IDENTIFY_ACK_AUTH:
            result = hl_iap2_identify_ack_auth(iap2);

            if (result == 0) {
                iap2->iap2_printf("[OK] [_hl_iap2_identify_process] receive ack auth message!\n");
                iap2->identify_status = EM_HL_IAP2_STM_IDENTIFY_REQ_CHALLENGE;
            } else {
                iap2->iap2_printf("[ERROR] [_hl_iap2_identify_process] receive ack auth message!\n");
                iap2->identify_status = EM_HL_IAP2_STM_IDENTIFY_REQ_AUTH;
                iap2->main_status     = EM_HL_IAP2_STM_MAIN_FAILED;
            }
            break;

        case EM_HL_IAP2_STM_IDENTIFY_REQ_CHALLENGE:
            result = hl_iap2_identify_req_challenge(iap2);

            if (result == 0) {
                iap2->iap2_printf("[OK] [_hl_iap2_identify_process] receive req challenge message!\n");
                iap2->identify_status = EM_HL_IAP2_STM_IDENTIFY_ACK_CHALLENGE;
            } else {
                iap2->iap2_printf("[ERROR] [_hl_iap2_identify_process] receive req challenge message!\n");
                iap2->identify_status = EM_HL_IAP2_STM_IDENTIFY_REQ_AUTH;
                iap2->main_status     = EM_HL_IAP2_STM_MAIN_FAILED;
            }
            break;

        case EM_HL_IAP2_STM_IDENTIFY_ACK_CHALLENGE:
            result                = hl_iap2_identify_ack_challenge(iap2);
            iap2->identify_status = EM_HL_IAP2_STM_IDENTIFY_SUCCEDD;
            break;

        case EM_HL_IAP2_STM_IDENTIFY_SUCCEDD:
            result                = hl_iap2_identify_succedd(iap2);
            iap2->identify_status = EM_HL_IAP2_STM_IDENTIFY_START_IDENTIFICATION;
            break;

        case EM_HL_IAP2_STM_IDENTIFY_START_IDENTIFICATION:
            result                = hl_iap2_identify_start_identification(iap2);
            iap2->identify_status = EM_HL_IAP2_STM_IDENTIFY_IDENTIFICATION_INFO;
            break;

        case EM_HL_IAP2_STM_IDENTIFY_IDENTIFICATION_INFO:
            result                = hl_iap2_identify_identification_info(iap2);
            iap2->identify_status = EM_HL_IAP2_STM_IDENTIFY_IDENTIFICATION_ACCEPTED;
            break;

        case EM_HL_IAP2_STM_IDENTIFY_IDENTIFICATION_ACCEPTED:
            result                = hl_iap2_identify_identification_accepted(iap2);
            iap2->identify_status = EM_HL_IAP2_STM_IDENTIFY_REQ_AUTH;
            iap2->main_status     = EM_HL_IAP2_STM_MAIN_POWER_UPDATE;
            break;

        default:
            break;
    }

    return 0;
}

/**
 * _hl_iap2_power_update_process
 * @brief powerupdate处理
 * @param [in] iap2 iap2句柄
 * @return int 成功 0 | 失败 非0
 * @date 2022-09-01
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lisonglin     <td>新建
 * </table>
 */
static int _hl_iap2_power_update_process(st_iap2_protocol_p iap2)
{
    hl_iap2_powerupdate_send_power(iap2);

    iap2->main_status = EM_HL_IAP2_STM_MAIN_EAP;
    // switch (iap2->powerupdate_status) {
    //     case EM_HL_IAP2_STM_POWERUPDATE_SEND_POWER:
    //         hl_iap2_powerupdate_send_power(iap2);
    //         iap2->powerupdate_status = EM_HL_IAP2_STM_POWERUPDATE_RECV_POWER_UPDATE;
    //         break;

    //     case EM_HL_IAP2_STM_POWERUPDATE_RECV_POWER_UPDATE:
    //         hl_iap2_powerupdate_recv_update(iap2);
    //         iap2->powerupdate_status = EM_HL_IAP2_STM_POWERUPDATE_SEND_POWER_SOURCE;
    //         break;

    //     case EM_HL_IAP2_STM_POWERUPDATE_SEND_POWER_SOURCE:
    //         hl_iap2_powerupdate_send_power_source(iap2);
    //         iap2->powerupdate_status = EM_HL_IAP2_STM_POWERUPDATE_SEND_ACK;
    //         break;

    //     case EM_HL_IAP2_STM_POWERUPDATE_SEND_ACK:
    //         hl_iap2_link_send_ack(iap2);
    //         iap2->powerupdate_status = EM_HL_IAP2_STM_POWERUPDATE_SEND_POWER;
    //         iap2->main_status        = EM_HL_IAP2_STM_MAIN_SUCCEED;
    //         break;

    //     default:
    //         break;
    // }

    return 0;
}

static int _hl_iap2_ea_session_process(st_iap2_protocol_p iap2)
{
    hl_iap2_eap_handle_msg(iap2);
    hl_iap2_eap_check_power_insert(iap2);

    // iap2->main_status = EM_HL_IAP2_STM_MAIN_SUCCEED;

    return 0;
}

int hl_iap2_process_main_oneshot(st_iap2_protocol_p iap2)
{
    if (iap2 == NULL) {
        iap2->iap2_printf("[_hl_iap2_process_main] error parameter!\n");
        return -1;
    }

    uint8_t ret = 0;
    // iap2->iap2_printf("[%s]state:%d\n", __func__);

    switch (iap2->main_status) {
        case EM_HL_IAP2_STM_MAIN_IDLE:
            // _hl_check_usb_insert(iap2);
            break;

        case EM_HL_IAP2_STM_MAIN_DETECT:
            _hl_iap2_detect_process(iap2);
            break;

        case EM_HL_IAP2_STM_MAIN_LINK:
            _hl_iap2_link_process(iap2);
            break;

        case EM_HL_IAP2_STM_MAIN_IDENTIFY:
            _hl_iap2_identify_process(iap2);
            break;

        case EM_HL_IAP2_STM_MAIN_POWER_UPDATE:
            _hl_iap2_power_update_process(iap2);
            break;

        case EM_HL_IAP2_STM_MAIN_EAP:
            _hl_iap2_ea_session_process(iap2);
            break;

        case EM_HL_IAP2_STM_MAIN_SUCCEED:
            /* code */
            break;

        case EM_HL_IAP2_STM_MAIN_FAILED:
            /* code */
            break;

        default:
            break;
    }

    return 0;
}

int hl_iap2_protocol_init(st_iap2_protocol_p iap2, func_handle handle)
{
    if (handle.delay_usec_func == NULL || handle.iap2_usb_read == NULL || handle.iap2_usb_write == NULL
        || handle.iap2_iic_read == NULL || handle.iap2_iic_write == NULL) {
        iap2->iap2_printf("[hl_iap2_protocol_init] error parameter!\n");
        return -1;
    }

    iap2->packet_arg.seq_num    = 0x00;
    iap2->packet_arg.ack_num    = 0x2B;
    iap2->packet_arg.session_id = 0x00;

    iap2->send_buffer = (uint8_t*)malloc(sizeof(uint8_t) * SEND_BUFFER_SIZE);
    iap2->recv_buffer = (uint8_t*)malloc(sizeof(uint8_t) * RECV_BUFFER_SIZE);
    if (iap2->send_buffer == NULL || iap2->recv_buffer == NULL) {
        iap2->iap2_printf("[hl_iap2_protocol_init] error malloc buffer!\n");
        return -1;
    }

    // 注册函数
    iap2->delay_usec_func = handle.delay_usec_func;
    iap2->iap2_usb_read   = handle.iap2_usb_read;
    iap2->iap2_usb_write  = handle.iap2_usb_write;
    iap2->iap2_iic_read   = handle.iap2_iic_read;
    iap2->iap2_iic_write  = handle.iap2_iic_write;
    iap2->iap2_printf     = handle.iap2_printf;

    iap2->main_status = EM_HL_IAP2_STM_MAIN_DETECT;

    return 0;
}

int hl_iap2_protocol_deinit(st_iap2_protocol_p iap2)
{
    free(iap2->send_buffer);
    free(iap2->recv_buffer);

    return 0;
}