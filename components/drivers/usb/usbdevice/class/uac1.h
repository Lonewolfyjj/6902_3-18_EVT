/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    uac1.h
  * @version V0.1
  * @brief   usb audio class 1.0 driver
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-09-17     Liangfeng Wu    first implementation
  *
  ******************************************************************************
  */
#ifndef __UAC1_H__
#define __UAC1_H__

#include <rtdevice.h>
#include "drivers/usb_audio.h"

DECLARE_UAC_AC_HEADER_DESCRIPTOR(2);
DECLARE_UAC_AC_HEADER_DESCRIPTOR(1);            // 添加单mic或单spk 音频类描述
DECLARE_UAC_FEATURE_UNIT_DESCRIPTOR(0);
DECLARE_UAC_FORMAT_TYPE_I_DISCRETE_DESC(1);

struct uac1_control
{
#ifdef RT_USB_DEVICE_COMPOSITE
    struct uiad_descriptor iad_desc;
#endif
    struct uinterface_descriptor interface_desc;
    struct uac1_ac_header_descriptor_2 header_desc;
    struct uac_input_terminal_descriptor usb_out_it_desc;
    struct uac1_output_terminal_descriptor io_out_ot_desc;
    struct uac_feature_unit_descriptor_0 usb_out_it_feature_desc;
    struct uac_input_terminal_descriptor io_in_it_desc;
    struct uac1_output_terminal_descriptor usb_in_ot_desc;
    struct uac_feature_unit_descriptor_0 io_in_it_feature_desc;
};
typedef struct uac1_control *uac1_control_t;

struct uac1_mic_control
{
#ifdef RT_USB_DEVICE_COMPOSITE
    struct uiad_descriptor iad_desc;
#endif
    struct uinterface_descriptor interface_desc;
    struct uac1_ac_header_descriptor_1 header_desc;
    // struct uac_input_terminal_descriptor usb_out_it_desc;
    // struct uac1_output_terminal_descriptor io_out_ot_desc;
    // struct uac_feature_unit_descriptor_0 usb_out_it_feature_desc;
    struct uac_input_terminal_descriptor io_in_it_desc;
    struct uac1_output_terminal_descriptor usb_in_ot_desc;
    struct uac_feature_unit_descriptor_0 io_in_it_feature_desc;
};
typedef struct uac1_mic_control *uac1_mic_control_t;

struct uac1_spk_control
{
#ifdef RT_USB_DEVICE_COMPOSITE
    struct uiad_descriptor iad_desc;
#endif
    struct uinterface_descriptor interface_desc;
    struct uac1_ac_header_descriptor_1 header_desc;
    struct uac_input_terminal_descriptor usb_out_it_desc;
    struct uac1_output_terminal_descriptor io_out_ot_desc;
    struct uac_feature_unit_descriptor_0 usb_out_it_feature_desc;
    // struct uac_input_terminal_descriptor io_in_it_desc;
    // struct uac1_output_terminal_descriptor usb_in_ot_desc;
    // struct uac_feature_unit_descriptor_0 io_in_it_feature_desc;
};
typedef struct uac1_spk_control *uac1_spk_control_t;

struct uac1_interface_alt
{
    struct uinterface_descriptor interface_desc;
    struct uac1_as_header_descriptor as_header_desc;
    struct uac_format_type_i_discrete_descriptor_1 type_i_desc;
    struct uaudio_endpoint_descriptor endpoint_desc;
    struct uac_iso_endpoint_descriptor iso_endpoint_desc;
};
typedef struct uac1_interface_alt *uac1_interface_alt_t;

/// uac设备通知枚举
enum _hl_uac_ind_e
{
    /// uac连接通知
    HL_UAC_LINK_IND = 1,
    /// uac设置播放音量通知（host设置dev音量）
    HL_UAC_SET_P_VOL_IND,
    /// uac设置录制音量通知（host设置dev音量）
    HL_UAC_SET_C_VOL_IND,
} hl_uac_ind_e;

/// uac设备命令枚举
enum _hl_uac_cmd_e
{
    /// 获取uac的邮箱指针，参数<struct rt_mailbox *>
    HL_GET_UAC_MB_CMD = 1,
    /// 设置uac capture音量，参数<rt_uint16_t *>
    HL_SET_UAC_C_VOL_CMD,
    /// 获取uac capture音量，参数<rt_uint16_t *>
    HL_GET_UAC_C_VOL_CMD,
    /// 设置uac play音量，参数<rt_uint16_t *>
    HL_SET_UAC_P_VOL_CMD,
    /// 获取uac play音量，参数<rt_uint16_t *>
    HL_GET_UAC_P_VOL_CMD,
    /// 获取uac play是否就绪，参数<rt_uint8_t*：0--未就绪  1--就绪>
    HL_GET_UAC_P_STATE_CMD,
    /// 启动uac capture，参数<RT_NULL>，备注：启动后会缓存一半静音数据
    HL_SET_UAC_C_START_CMD,
} hl_uac_cmd_e;

#endif /* __UAC1_H__ */
