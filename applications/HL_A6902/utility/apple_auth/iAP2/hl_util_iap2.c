#include "hl_util_iap2.h"

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
static int _hl_iap2_detect_process(hl_util_apple_p apple)
{
    int result = -1;

    switch (apple->iap2.detect_status) {
        case EM_HL_IAP2_STM_DETECT_SEND:
            result = hl_iap2_detect_send(apple);
            if (!result) {
                apple->iap2.detect_status = EM_HL_IAP2_STM_DETECT_RECV;
            } else {
                apple->log("[ERROR][%s][line:%d] send detect message!\n", __func__, __LINE__);
                apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_FAILED;
            }
            break;

        case EM_HL_IAP2_STM_DETECT_RECV:
            result = hl_iap2_detect_recv(apple);
            if (!result) {
                apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_LINK;
                apple->iap2.retry_time  = 5;
            } else if (result && !apple->iap2.retry_time) {
                apple->log("[ERROR][%s][line:%d] receive detect message!\n", __func__, __LINE__);
                apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_FAILED;
            } else {
                apple->iap2.detect_status = EM_HL_IAP2_STM_DETECT_SEND;
                apple->iap2.retry_time--;
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
static int _hl_iap2_link_process(hl_util_apple_p apple)
{
    int result = -1;

    switch (apple->iap2.link_status) {
        case EM_HL_IAP2_STM_LINK_SEND_SYN:
            result = hl_iap2_link_send_sync(apple);
            if (!result) {
                apple->iap2.link_status = EM_HL_IAP2_STM_LINK_RECV_SYN_ACK;
            } else {
                apple->log("[ERROR][%s][line:%d] send link syn!\n", __func__, __LINE__);
                apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_FAILED;
            }
            break;

        case EM_HL_IAP2_STM_LINK_RECV_SYN_ACK:
            result = hl_iap2_link_recv_sync_ack(apple);
            if (!result) {
                apple->iap2.link_status = EM_HL_IAP2_STM_LINK_SEND_ACK;
            } else if (result && !apple->iap2.retry_time) {
                apple->log("[ERROR][%s][line:%d] recv link ack!\n", __func__, __LINE__);
                apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_FAILED;
            } else {
                apple->iap2.link_status = EM_HL_IAP2_STM_LINK_SEND_SYN;
                apple->iap2.retry_time--;
            }
            break;

        case EM_HL_IAP2_STM_LINK_SEND_ACK:
            result                  = hl_iap2_link_send_ack(apple);
            apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_IDENTIFY;
            apple->iap2.retry_time  = 3;
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
static int _hl_iap2_identify_process(hl_util_apple_p apple)
{
    if (NULL == apple) {
        apple->log("[ERROR][%s][line:%d] ipa2 is null!\n", __func__, __LINE__);
        return -1;
    }

    int result = -1;

    switch (apple->iap2.identify_status) {
        case EM_HL_IAP2_STM_IDENTIFY_REQ_AUTH:
            result = hl_iap2_identify_req_auth(apple);
            if (!result) {
                apple->iap2.identify_status = EM_HL_IAP2_STM_IDENTIFY_ACK_AUTH;
            } else {
                apple->log("[ERROR][%s][line:%d] recv auth req message!\n", __func__, __LINE__);
                apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_FAILED;
            }
            break;

        case EM_HL_IAP2_STM_IDENTIFY_ACK_AUTH:
            result = hl_iap2_identify_ack_auth(apple);
            if (!result) {
                apple->iap2.identify_status = EM_HL_IAP2_STM_IDENTIFY_REQ_CHALLENGE;
            } else {
                apple->log("[ERROR][%s][line:%d] receive ack auth message!\n", __func__, __LINE__);
                apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_FAILED;
            }
            break;

        case EM_HL_IAP2_STM_IDENTIFY_REQ_CHALLENGE:
            result = hl_iap2_identify_req_challenge(apple);
            if (!result) {
                apple->iap2.identify_status = EM_HL_IAP2_STM_IDENTIFY_ACK_CHALLENGE;
            } else {
                apple->log("[ERROR][%s][line:%d][%d] recv challenge resp message!\n", __func__, __LINE__, result);
                apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_FAILED;
            }
            break;

        case EM_HL_IAP2_STM_IDENTIFY_ACK_CHALLENGE:
            result = hl_iap2_identify_ack_challenge(apple);
            if (!result) {
                apple->iap2.identify_status = EM_HL_IAP2_STM_IDENTIFY_SUCCEDD;
            } else {
                apple->log("[ERROR][%s][line:%d][%d] ack challenge resp!\n", __func__, __LINE__, result);
                apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_FAILED;
            }
            break;

        case EM_HL_IAP2_STM_IDENTIFY_SUCCEDD:
            result = hl_iap2_identify_succedd(apple);
            if (!result) {
                apple->iap2.identify_status = EM_HL_IAP2_STM_IDENTIFY_START_IDENTIFICATION;
            } else {
                apple->log("[ERROR][%s][line:%d][%d] identify succedd!\n", __func__, __LINE__, result);
                apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_FAILED;
            }
            break;

        case EM_HL_IAP2_STM_IDENTIFY_START_IDENTIFICATION:
            result = hl_iap2_identify_start_identification(apple);
            if (!result) {
                apple->iap2.identify_status = EM_HL_IAP2_STM_IDENTIFY_IDENTIFICATION_INFO;
            } else {
                apple->log("[ERROR][%s][line:%d][%d] start identify!\n", __func__, __LINE__, result);
                apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_FAILED;
            }
            break;

        case EM_HL_IAP2_STM_IDENTIFY_IDENTIFICATION_INFO:
            result = hl_iap2_identify_identification_info(apple);
            if (!result) {
                apple->iap2.identify_status = EM_HL_IAP2_STM_IDENTIFY_IDENTIFICATION_ACCEPTED;
            } else {
                apple->log("[ERROR][%s][line:%d][%d] identify set info!\n", __func__, __LINE__, result);
                apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_FAILED;
            }
            break;

        case EM_HL_IAP2_STM_IDENTIFY_IDENTIFICATION_ACCEPTED:
            result = hl_iap2_identify_identification_accepted(apple);
            if (!result) {
                apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_POWER_UPDATE;
            } else {
                apple->log("[ERROR][%s][line:%d][%d] identify set info!\n", __func__, __LINE__, result);
                apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_FAILED;
            }
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
static int _hl_iap2_power_update_process(hl_util_apple_p apple)
{
    if (NULL == apple) {
        apple->log("[%s][line:%d]param(apple) is NULL!\n", __func__, __LINE__);
        return -1;
    }

    switch (apple->iap2.powerupdate_status) {
        case EM_HL_IAP2_STM_POWERUPDATE_SEND_POWER:
            hl_iap2_powerupdate_send_power(apple);
            apple->iap2.powerupdate_status = EM_HL_IAP2_STM_POWERUPDATE_RECV_POWER_UPDATE;
            break;

        case EM_HL_IAP2_STM_POWERUPDATE_RECV_POWER_UPDATE:
            hl_iap2_powerupdate_recv_update(apple);
            apple->iap2.powerupdate_status = EM_HL_IAP2_STM_POWERUPDATE_SEND_POWER_SOURCE;
            break;

        case EM_HL_IAP2_STM_POWERUPDATE_SEND_POWER_SOURCE:
#if POWERSOURCEUPDATE_OPEN
            hl_iap2_powerupdate_send_power_source(apple);
#endif
            apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_SUCCEED;
            break;

        default:
            break;
    }

    return 0;
}

/**
 * 
 * @brief 
 * @param [in] iap2 iap2句柄
 * @return int 成功 0 | 失败 非0
 * @date 2023-02-07
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-02-07      <td>lisonglin     <td>新建
 * </table>
 */
static int _hl_iap2_ack_ctrl_process(hl_util_apple_p apple)
{
    if (NULL == apple) {
        apple->log("[%s][line:%d]param(apple) is NULL!\n", __func__, __LINE__);
        return -1;
    }

    int      result     = -1;
    int      read_len   = 0;
    uint8_t  ret        = 0;
    uint8_t  len        = 0;
    uint16_t message_id = 0;
    uint16_t param_id   = 0;

    read_len = apple->usb_read(apple->iap2.recv_buffer, 27, TIMEOUT_US);

    result = hl_iap2_packet_header_decode(apple->iap2.recv_buffer, &len, LINK_CONTROL_ACK, &apple->packet_arg);
    apple->log("[%s:%d]USB Read SessionID = %X\n", __func__, __LINE__, apple->packet_arg.session_id);

    switch (apple->packet_arg.session_id) {
        case SESSION_ID_CTRL: {
            message_id = hl_iap2_ctrl_packet_get_message_id(apple->iap2.recv_buffer);
            param_id   = hl_iap2_ctrl_packet_get_param_id(apple->iap2.recv_buffer);
            apple->log("%s:%d:xxx usb MessageID = %X\n", __func__, __LINE__, message_id);
            apple->log("%s:%d:xxx usb ParamID = %04X\n\n", __func__, __LINE__, param_id);
            switch (message_id) {
                case MESSAGE_ID_START_EAP:
                    apple->log("[Apple]Get EAP Start!!!\n");
                    break;

                case MESSAGE_ID_STOP_EAP:
                    apple->log("[Apple]Get EAP Stop!!!\n");
                    break;

                case MESSAGE_ID_POWER_UPDATE:
                    apple->log("[Apple]Get Power Update!!!\n");
                    break;

                default:
                    break;
            }
        } break;

        case SESSION_ID_EA:
            apple->log("[Apple]Get EA Message!!!\n");
            break;

        default:
            break;
    }

    hl_iap2_link_send_ack(apple);

    return 0;
}

int hl_util_iap2_oneshot(hl_util_apple_p apple)
{
    if (NULL == apple) {
        apple->log("[%s][line:%d] error parameter!\n", __func__, __LINE__);
        return -2;
    }

    int result = -1;

    switch (apple->iap2.main_status) {
        case EM_HL_IAP2_STM_MAIN_IDLE:
            apple->delay_usec(10000);
            break;

        case EM_HL_IAP2_STM_MAIN_DETECT:
            _hl_iap2_detect_process(apple);
            break;

        case EM_HL_IAP2_STM_MAIN_LINK:
            _hl_iap2_link_process(apple);
            break;

        case EM_HL_IAP2_STM_MAIN_IDENTIFY:
            _hl_iap2_identify_process(apple);
            break;

        case EM_HL_IAP2_STM_MAIN_POWER_UPDATE:
            _hl_iap2_power_update_process(apple);
            break;

        case EM_HL_IAP2_STM_MAIN_ACK_CTRL:
            _hl_iap2_ack_ctrl_process(apple);
            break;

        case EM_HL_IAP2_STM_MAIN_SUCCEED:
            apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_ACK_CTRL;
            result                  = EM_HL_IAP2_SUCCEED;
            break;

        case EM_HL_IAP2_STM_MAIN_FAILED:
            apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_IDLE;
            result                  = EM_HL_IAP2_FAILED;
            break;

        default:
            break;
    }

    return result;
}

int hl_util_iap2_init(hl_util_apple_p apple)
{
    if (NULL == apple) {
        apple->log("[%s][line:%d]param(apple) is NULL!\n", __func__, __LINE__);
        return -1;
    }

    apple->packet_arg.seq_num    = 0x00;
    apple->packet_arg.ack_num    = 0x2B;
    apple->packet_arg.session_id = 0x00;

    apple->iap2.send_buffer = (uint8_t*)rt_malloc(sizeof(uint8_t) * SEND_BUFFER_SIZE);
    apple->iap2.recv_buffer = (uint8_t*)rt_malloc(sizeof(uint8_t) * RECV_BUFFER_SIZE);
    if ((NULL == apple->iap2.send_buffer) || (NULL == apple->iap2.recv_buffer)) {
        apple->log("[%s][line:%d]buffer rt_malloc failed!!!\n", __func__, __LINE__);
        return -2;
    }

    apple->iap2.main_status = EM_HL_IAP2_STM_MAIN_IDLE;
    apple->iap2.retry_time  = 0;

    return 0;
}

int hl_util_iap2_deinit(hl_util_apple_p apple)
{
    free(apple->iap2.send_buffer);
    free(apple->iap2.recv_buffer);

    return 0;
}
