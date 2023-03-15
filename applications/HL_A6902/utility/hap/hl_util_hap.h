/**
 * @file hl_util_hap.h
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * @brief APP协议解析处理文件
 * @version 1.0
 * @date 2023-03-13
 * 
 * ██╗  ██╗ ██████╗ ██╗     ██╗  ██╗   ██╗██╗      █████╗ ███╗   ██╗██████╗ 
 * ██║  ██║██╔═══██╗██║     ██║  ╚██╗ ██╔╝██║     ██╔══██╗████╗  ██║██╔══██╗
 * ███████║██║   ██║██║     ██║   ╚████╔╝ ██║     ███████║██╔██╗ ██║██║  ██║
 * ██╔══██║██║   ██║██║     ██║    ╚██╔╝  ██║     ██╔══██║██║╚██╗██║██║  ██║
 * ██║  ██║╚██████╔╝███████╗███████╗██║   ███████╗██║  ██║██║ ╚████║██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝
 * @copyright Copyright (c) 2023 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2023-03-13     <td>v1.0     <td>lisonglin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_UTIL_HAP_H__
#define __HL_UTIL_HAP_H__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <rtthread.h>

/* typedef -------------------------------------------------------------------*/
typedef enum
{
    EM_HAP_STATE_HDR_H,
    EM_HAP_STATE_HDR_L,
    EM_HAP_STATE_CMD,
    EM_HAP_STATE_CTRL,
    EM_HAP_STATE_DATA_LEN_H,
    EM_HAP_STATE_DATA_LEN_L,
    EM_HAP_STATE_DATA,
    EM_HAP_STATE_CRC,
} hap_protocol_state_em;

typedef enum
{
    EM_HAP_ROLE_MASTER = 0x11,
    EM_HAP_ROLE_SLAVE,
} hap_role_em;

typedef enum
{
    EM_HAP_TIMER_ENABLE = 0x21,
    EM_HAP_TIMER_DISABLE,
} hap_timer_state_em;

typedef struct
{
    /// 协议头部高字节
    uint8_t hdr_h;
    /// 协议头部低字节
    uint8_t hdr_l;
    /// 命令位
    uint8_t cmd;
    /// 控制位
    uint8_t ctrl;
    /// 数据内容长度高字节
    uint8_t data_len_h;
    /// 数据内容长度低字节
    uint8_t data_len_l;
    /// 数据内容地址
    uint8_t* data_addr;
    /// 异或校验位
    uint8_t xor8;
} hap_protocol_type_t;

typedef struct
{
    /// 协议角色
    hap_role_em role;
    /// 定时器使能状态
    hap_timer_state_em timer_state;
    /// 已申请存放数据缓冲区长度
    uint16_t frame_data_len;
} hap_handle_type_t;

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
    /// hap句柄结构体
    hap_handle_type_t hap_handle;
    /// hap协议帧结构体
    hap_protocol_type_t hap_frame;
    /**
     * get_time_stamp
     * @brief 获取当前时间戳
     * @date 2023-03-13
     * @author lisonglin (songlin.li@hollyland-tech.com)
     * 
     * @details 
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2023-03-13      <td>lisonglin     <td>新建
     * </table>
     */
    uint32_t (*get_time_stamp)(void);
    /**
     * hap_success_handle_cb
     * @brief hap解析成功后的处理函数
     * @date 2023-03-13
     * @author lisonglin (songlin.li@hollyland-tech.com)
     * 
     * @details 
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2023-03-13      <td>lisonglin     <td>新建
     * </table>
     */
    void (*hap_success_handle_cb)(hap_protocol_type_t);
} hl_util_hap_t;

/* define --------------------------------------------------------------------*/
#define HL_UTIL_HAP_DBG rt_kprintf
#define HAP_MAX_TIMEOUT 500

#define HAP_FRAME_HOST_HDR_H 0xAA
#define HAP_FRAME_HOST_HDR_L 0xDD
#define HAP_FRAME_CLIENT_HDR_H 0xBB
#define HAP_FRAME_CLIENT_HDR_L 0xDD

#define HAP_CTRL_RX (1 << 0)
#define HAP_CTRL_TX1 (1 << 1)
#define HAP_CTRL_TX2 (1 << 2)
#define HAP_CTRL_PHONE (1 << 3)

/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/**
 * hl_util_hap_init
 * @brief 初始化hap协议帧结构体，申请内存空间，注册回调函数
 * @param [in] hap_ptr hap结构体指针(传入该参数前，需对成员变量hap_handle结构体赋初值)
 * @param [in] frame_data_addr 协议帧数据存放缓冲区
 * @param [in] get_time_stamp 获取当前时间戳（函数指针）
 * @param [in] hap_success_handle_cb 解析成功后的处理（函数指针）
 * @return int 成功 0 | 失败 -1
 * @date 2023-03-13
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-03-13      <td>lisonglin     <td>新建
 * </table>
 */
int hl_util_hap_init(hl_util_hap_t* hap_ptr, uint8_t* frame_data_addr, uint32_t (*get_time_stamp)(void),
                     void (*hap_success_handle_cb)(hap_protocol_type_t));

/**
 * hl_util_hap_deinit
 * @brief hap协议帧结构体去初始化，释放内存空间
 * @param [in] hap_ptr hap结构体指针
 * @return int 成功 0 | 失败 -1
 * @date 2023-03-13
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-03-13      <td>lisonglin     <td>新建
 * </table>
 */
int hl_util_hap_deinit(hl_util_hap_t* hap_ptr);

/**
 * hl_util_hap_encode
 * @brief hap协议封包函数
 * @param [in] role hap当前角色
 * @param [in] cmd 命令位
 * @param [in] ctrl 控制位
 * @param [in] frame_buf 存放hap协议包的缓冲区
 * @param [in] buf_len 缓冲区大小
 * @param [in] data_addr 数据内容地址
 * @param [in] data_len 数据内容长度
 * @return int 成功 数据包长度 | 失败 -1
 * @date 2023-03-13
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-03-13      <td>lisonglin     <td>新建
 * </table>
 */
int hl_util_hap_encode(hap_role_em role, uint8_t cmd, uint8_t ctrl, uint8_t* frame_buf, uint16_t buf_len,
                       uint8_t* data_addr, uint16_t data_len);

/**
 * hl_util_hap_decode
 * @brief hap协议解包函数
 * @param [in] hap_ptr hap结构体指针
 * @param [in] data_byte 解析单个字节
 * @return int 成功 0 | 失败 -1
 * @date 2023-03-13
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-03-13      <td>lisonglin     <td>新建
 * </table>
 */
int hl_util_hap_decode(hl_util_hap_t* hap_ptr, uint8_t data_byte);

#endif

/* Exported functions --------------------------------------------------------*/
/*
 * EOF
 */
