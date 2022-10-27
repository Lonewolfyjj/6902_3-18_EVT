/**
 * @file hl_test_pre.h
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 用来预研时把一些公用东西放在这里面，后面要删掉！！！
 * @version V1.0
 * @date 2022-10-12
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
 * <tr><td>2022-10-12     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
typedef enum
{
    CMD_TEST,
} hl_mod_disp_cmd_e;

#ifndef RSENUM8
#define HL_ENUM8(EnumName) \
    Enum_##EnumName;       \
    typedef uint8_t EnumName
#endif

#ifndef RSENUM16
#define HL_ENUM16(EnumName) \
    Enum_##EnumName;        \
    typedef uint16_t EnumName
#endif

typedef enum _mode_id_e
{
    APP_MODE = 0x00,
    INPUT_MODE,
    DISPLAY_MODE,
    EXT_COM_MODE,
    UPGRADE_MODE,
    APPLE_AUTH_MODE,
    TELINK_MODE,
    AUDIO_MODE,
    PM_MODE,
} HL_ENUM8(hl_mode_id_e);


typedef struct _mode_to_app_msg_t
{
    hl_mode_id_e sender;
    uint8_t      cmd;
    uint16_t     len;
    union  {
        uint32_t u32_param;
        void*    ptr;
    } param;
} mode_to_app_msg_t;
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/*
 * EOF
 */