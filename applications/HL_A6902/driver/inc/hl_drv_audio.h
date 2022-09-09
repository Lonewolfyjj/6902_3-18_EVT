/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  * Change Logs:
  * Date           Author       Notes
  * 2019-07-10     sugar      first implementation.
  */

#ifndef __HL_DRV_AUDIO_H__
#define __HL_DRV_AUDIO_H__

#include "hal_audio.h"

#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN
#include "pcm/pcm_types.h"
#endif

typedef void(*drv_audio_stream_rcv_cb_t)(void* dst, const void* src, rt_ubase_t count);

rt_err_t hl_drv_audio_register_stream(drv_audio_stream_rcv_cb_t audio_cb_func);


#endif /* __RK_AUDIO_H__ */
