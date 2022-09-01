/**
 * @file hl_util_hup.h
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * @brief 对串口数据进行hup协议打包解包
 * @version 0.1
 * @date 2022-08-09
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-08-09     <td>v1.0     <td>lisonglin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_UTIL_HUP_H__
#define __HL_UTIL_HUP_H__
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
/* typedef -------------------------------------------------------------------*/
typedef struct
{
    /// 协议头部高字节
    uint8_t hdr_h;
    /// 协议头部低字节
    uint8_t hdr_l;
    /// 命令控制位
    uint8_t cmd;
    /// 数据内容长度
    uint16_t data_len;
    /// 数据内容地址
    uint8_t* data_addr;
    /// 异或校验位
    uint8_t xor8;
} hup_protocol_type_t, *hup_protocol_type_p;

typedef struct
{
    /// 协议角色
    uint8_t role;
    /// 状态机当前状态
    uint8_t state;
    /// 异或校验值
    uint8_t xor8;
    /// 定时器使能状态
    uint8_t timer_state;
    /// 缓冲区可存的长度
    uint16_t buf_len;
    /// 已解析数据的长度
    uint16_t read_data_len;
    /// 上次时间戳值
    uint32_t last_timeout_val;
    /// 当前时间戳值
    uint32_t cur_timeout_val;
    /// 最大超时时间
    uint32_t max_timeout;
    /// hup协议帧结构体
    hup_protocol_type_t hup_frame;
    /**
     * get_time_stamp
     * @brief 获取当前时间戳
     * @date 2022-08-09
     * @author lisonglin (songlin.li@hollyland-tech.com)
     * 
     * @details 
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2022-08-09      <td>lisonglin     <td>新建
     * </table>
     */
    uint32_t (*get_time_stamp)(void);
    /**
     * hup_success_handle_func
     * @brief hup解析成功后的处理函数
     * @date 2022-08-09
     * @author lisonglin (songlin.li@hollyland-tech.com)
     * 
     * @details 
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2022-08-09      <td>lisonglin     <td>新建
     * </table>
     */
    void (*hup_success_handle_func)(hup_protocol_type_t);
} hup_protocol_handle_t, *hup_protocol_handle_p;

typedef enum
{
    EM_HUP_STATE_HDR_H,
    EM_HUP_STATE_HDR_L,
    EM_HUP_STATE_CMD,
    EM_HUP_STATE_DATA_LEN_H,
    EM_HUP_STATE_DATA_LEN_L,
    EM_HUP_STATE_DATA,
    EM_HUP_STATE_CRC,
} hup_protocol_state_em;

typedef enum
{
    EM_HUP_ROLE_MASTER,
    EM_HUP_ROLE_SLAVE,
} hup_role_em;

typedef enum
{
    EM_HUP_ENABLE,
    EM_HUP_DISABLE,
} hup_timer_state_em;

/* define --------------------------------------------------------------------*/
#define HL_UTIL_HUP_DBG printf
#define HUP_MAX_TIMEOUT 10000

#define HUP_FRAME_HOST_HDR_H 0xAA
#define HUP_FRAME_HOST_HDR_L 0xDD
#define HUP_FRAME_CLIENT_HDR_H 0xBB
#define HUP_FRAME_CLIENT_HDR_L 0xDD
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/**
 * hl_util_hup_protocol_init
 * @brief 初始化hup协议帧结构体，申请内存空间，注册回调函数
 * @param [in] role hup当前角色
 * @param [in] timer_state 定时器使能状态
 * @param [in] buf_addr 缓冲区地址
 * @param [in] buf_size 缓冲区大小
 * @param [in] get_time_stamp 获取当前时间戳（函数指针）
 * @param [in] hup_success_handle_func 解析成功后的处理（函数指针）
 * @return hup_protocol_handle_p 成功 返回hup句柄 | 失败 NULL
 * @date 2022-08-09
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-09      <td>lisonglin     <td>新建
 * </table>
 */
hup_protocol_handle_p hl_util_hup_protocol_init(hup_role_em role, hup_timer_state_em timer_state, uint8_t* buf_addr,
                                                uint16_t buf_size, uint32_t (*get_time_stamp)(void),
                                                void (*hup_success_handle_func)(hup_protocol_type_t));

/**
 * hl_util_hup_protocol_deinit
 * @brief hup协议帧结构体去初始化，释放内存空间
 * @param [in] hup hup协议帧结构体
 * @return int 成功 0 | 失败 -1
 * @date 2022-08-09
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-09      <td>lisonglin     <td>新建
 * </table>
 */
int hl_util_hup_protocol_deinit(hup_protocol_handle_p hup);

/**
 * hl_util_hup_protocol_encode
 * @brief hup协议封包函数
 * @param [in] role hup当前角色
 * @param [in] cmd 控制命令
 * @param [in] frame_buf 存放hup协议包的缓冲区
 * @param [in] buf_len 缓冲区大小
 * @param [in] data_addr 数据内容地址
 * @param [in] data_len 数据内容长度
 * @return int 成功 数据包长度 | 失败 -1
 * @date 2022-08-09
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-09      <td>lisonglin     <td>新建
 * </table>
 */
int hl_util_hup_protocol_encode(uint8_t role, uint8_t cmd, uint8_t* frame_buf, uint16_t buf_len, uint8_t* data_addr,
                                uint16_t data_len);

/**
 * hl_util_hup_protocol_decode
 * @brief hup协议解包函数
 * @param [in] hup hup句柄
 * @param [in] data_byte 解析单个字节
 * @return int 成功 0 | 失败 -1
 * @date 2022-08-09
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-09      <td>lisonglin     <td>新建
 * </table>
 */
int hl_util_hup_protocol_decode(hup_protocol_handle_p hup, uint8_t data_byte);

#endif

/* Exported functions --------------------------------------------------------*/
/*
 * EOF
 */
