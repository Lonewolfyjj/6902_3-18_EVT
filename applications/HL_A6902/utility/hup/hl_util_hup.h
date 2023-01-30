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
#include <rtthread.h>

/* typedef -------------------------------------------------------------------*/
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
    EM_HUP_ROLE_MASTER = 0x11,
    EM_HUP_ROLE_SLAVE,
} hup_role_em;

typedef enum
{
    EM_HUP_TIMER_ENABLE = 0x21,
    EM_HUP_TIMER_DISABLE,
} hup_timer_state_em;

typedef struct
{
    /// 协议头部高字节
    uint8_t hdr_h;
    /// 协议头部低字节
    uint8_t hdr_l;
    /// 命令控制位
    uint8_t cmd;
    /// 数据内容长度高字节
    uint8_t data_len_h;
    /// 数据内容长度低字节
    uint8_t data_len_l;
    /// 数据内容地址
    uint8_t* data_addr;
    /// 异或校验位
    uint8_t xor8;
} hup_protocol_type_t;

typedef struct
{
    /// 协议角色
    hup_role_em role;
    /// 定时器使能状态
    hup_timer_state_em timer_state;
    /// 已申请存放数据缓冲区长度
    uint16_t frame_data_len;
} hup_handle_type_t;

typedef struct
{
    /// 异或校验值
    uint8_t xor8;
    /// 状态机当前状态
    uint8_t state;
    /// 记录当前帧的数据长度
    uint16_t frame_data_len;
    /// 记录当前已解析的数据长度
    uint16_t read_data_len;
    /// 上次时间戳值
    uint32_t last_timeout_val;
    /// 当前时间戳值
    uint32_t cur_timeout_val;
    /// 最大超时时间
    uint32_t max_timeout;
    /// hup句柄结构体
    hup_handle_type_t hup_handle;
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
     * hup_success_handle_cb
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
    void (*hup_success_handle_cb)(hup_protocol_type_t);
} hl_util_hup_t;

/* define --------------------------------------------------------------------*/
#define HL_UTIL_HUP_DBG rt_kprintf
#define HUP_MAX_TIMEOUT 500

#define HUP_FRAME_HOST_HDR_H 0xAA
#define HUP_FRAME_HOST_HDR_L 0xDD
#define HUP_FRAME_CLIENT_HDR_H 0xBB
#define HUP_FRAME_CLIENT_HDR_L 0xDD

/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/**
 * hl_util_hup_init
 * @brief 初始化hup协议帧结构体，申请内存空间，注册回调函数
 * @param [in] hup_ptr hup结构体指针(传入该参数前，需对成员变量hup_handle结构体赋初值)
 * @param [in] frame_data_addr 协议帧数据存放缓冲区
 * @param [in] get_time_stamp 获取当前时间戳（函数指针）
 * @param [in] hup_success_handle_cb 解析成功后的处理（函数指针）
 * @return int 成功 0 | 失败 -1
 * @date 2022-09-13
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-13      <td>lisonglin     <td>新建
 * </table>
 */
int hl_util_hup_init(hl_util_hup_t* hup_ptr, uint8_t* frame_data_addr, uint32_t (*get_time_stamp)(void),
                     void (*hup_success_handle_cb)(hup_protocol_type_t));

/**
 * hl_util_hup_deinit
 * @brief hup协议帧结构体去初始化，释放内存空间
 * @param [in] hup_ptr hup结构体指针
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
int hl_util_hup_deinit(hl_util_hup_t* hup_ptr);

/**
 * hl_util_hup_encode
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
int hl_util_hup_encode(hup_role_em role, uint8_t cmd, uint8_t* frame_buf, uint16_t buf_len, uint8_t* data_addr,
                       uint16_t data_len);

/**
 * hl_util_hup_decode
 * @brief hup协议解包函数
 * @param [in] hup_ptr hup结构体指针
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
int hl_util_hup_decode(hl_util_hup_t* hup_ptr, uint8_t data_byte);

#endif

/* Exported functions --------------------------------------------------------*/
/*
 * EOF
 */
