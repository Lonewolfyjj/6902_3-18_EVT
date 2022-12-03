/**
 * @file hl_mod_display.h
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 显示模块
 * @version V1.0
 * @date 2022-11-11
 * 
 * ██╗  ██╗ ██████╗ ██╗     ██╗  ██╗   ██╗██╗      █████╗ ███╗   ██╗██████╗ 
 * ██║  ██║██╔═══██╗██║     ██║  ╚██╗ ██╔╝██║     ██╔══██╗████╗  ██║██╔══██╗
 * ███████║██║   ██║██║     ██║   ╚████╔╝ ██║     ███████║██╔██╗ ██║██║  ██║
 * ██╔══██║██║   ██║██║     ██║    ╚██╔╝  ██║     ██╔══██║██║╚██╗██║██║  ██║
 * ██║  ██║╚██████╔╝███████╗███████╗██║   ███████╗██║  ██║██║ ╚████║██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-11-11     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#ifndef __HL_MOD_DISPLAY_H__
#define __HL_MOD_DISPLAY_H__

#include "rtdef.h"
#include "hl_config.h"
#include "hl_util_general_type.h"

#define HL_DISPLAY_SUCCESS RT_EOK
#define HL_DISPLAY_FAILED RT_ERROR

#if HL_IS_TX_DEVICE()
// TX
typedef enum _led_mode
{
    /// 当前TX正在配对
    LED_MODE_PAIR = 0,
    /// 已连接
    LED_MODE_CONNECTED,
    /// 重连
    LED_MODE_RECONNECTION,
    /// 降噪并且已经连接
    LED_MODE_DENOISE_CONNECTED,
    /// 降噪并且回连
    LED_MODE_DENOISE_RECONNECTION,

    LED_MODE_ID_CNT

} HL_ENUM8(hl_led_mode);

typedef enum _hl_sound_channel
{
    /// 配置当前闪灯逻辑为左声道逻辑
    LEFT_SOUND_CHANNEL,
    /// 配置当前闪灯逻辑为右声道逻辑
    RIGHT_SOUND_CHANNEL,

} HL_ENUM8(hl_sound_channel);

typedef enum _hl_record_led_mode
{
    /// 录制灯开启
    RECORD_LED_MODE_OPEN,
    /// 录制灯关闭
    RECORD_LED_MODE_CLOSE,

    RECORD_LED_MODE_ID_CNT

} HL_ENUM8(hl_record_led_mode);
typedef enum _hl_cmd_e
{
    /// 状态指示LED变更命令，hl_mod_display_io_ctrl的参数参考<hl_led_mode>,参数为四字节
    MSG_STATE_LED_MODE_CMD,
    /// 录制LED灯变更命令，hl_mod_display_io_ctrl的参数参考<hl_record_led_mode>,参数为四字节
    MSG_RECORD_LED_MODE_CMD,
    /// 左右声道设置命令，hl_mod_display_io_ctrl的参数参考<hl_sound_channel>,参数为四字节
    MSG_SOUND_CHANNEL_CMD,

    /// @brief  命令个数统计
    MSG_ID_CNT

} hl_cmd_e;

#else
// RX
typedef enum _led_mode
{
    /// 当前RX正在配对
    LED_MODE_PAIR = 0,
    /// 当前RX已连接
    LED_MODE_CONNECTED,
    /// 当前RX重连
    LED_MODE_RECONNECTION,

    LED_MODE_ID_CNT

} hl_led_mode;

typedef enum _hl_screen_color_e
{
    /// 全屏黑色显示
    RGB888_BLACK,
    /// 全屏红色显示
    RGB888_RED,
    /// 全屏绿色显示
    RGB888_GREEN,
    /// 全屏蓝色显示
    RGB888_BLUE,
    /// 全屏白色显示
    RGB888_WHITE,

    RGB888_COLOR_CNT,
} HL_ENUM8(hl_screen_color_e);

typedef enum _hl_screen_page_e
{
    PAGE_NONE,
    PAGE_LOGO,
    PAGE_HOME,
    PAGE_MAIN_MENU,
    PAGE_TX_CONF_MENU,
    PAGE_TX_GAIN,
    PAGE_FAST_MENU,
    PAGE_MAX,  //  页面总数
} HL_ENUM8(hl_screen_page_e);

typedef enum _hl_cmd_e
{
    /// RX状态指示灯状态变更命令，io_ctrl的参数参考<hl_led_mode>,参数为四字节
    MSG_STATE_LED_MODE_CMD,
    /// MIPI OLED 颜色变更命令，io_ctrl的参数参考<hl_screen_color_e*>
    MSG_OLED_COLOR_CHANGE_CMD,

    MSG_ID_CNT

} hl_cmd_e;

#endif
/**
 * @brief 显示模块初始化
 * @param [in] display_msq 指向APP传入的消息队列
 * @return uint8_t 结果 HL_DISPLAY_SUCCESS
 *                  失败：HL_DISPLAY_FAILED
 * @date 2022-11-15
 * @author liujie (jie.liu@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-15      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_mod_display_init(void* display_msq);

/**
 * @brief 显示模块反初始化
 * @return uint8_t 结果 成功：HL_DISPLAY_SUCCESS 失败：失败：HL_DISPLAY_FAILED
 * @date 2022-11-15
 * @author liujie (jie.liu@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-15      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_mod_display_deinit(void);

/**
 * @brief 显示模块控制命令，包括，LED控制，OLED全屏显示颜色等
 * @param [in] cmd 命令：见<hl_cmd_e>
 * @param [in] ptr 参数的指针，使用具体见命令的注释
 * @param [in] len 长度，具体见命令的注释
 * @return uint8_t 
 * @date 2022-11-15
 * @author liujie (jie.liu@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-15      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_mod_display_io_ctrl(uint8_t cmd, void* ptr, uint16_t len);

#endif /*__HL_MOD_DISPLAY_H__*/

/*
 * EOF
 */