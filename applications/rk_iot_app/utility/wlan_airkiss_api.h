/*
 *  Copyright (C) 2019, Fuzhou Rockchip Electronics Co., Ltd.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Fuzhou Rockchip Electronics Co., Ltd. nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _WLAN_AIRKISS_API_H_
#define _WLAN_AIRKISS_API_H_

#include <stdint.h>
#include "net/wlan/wlan_defs.h"
#include "lwip/netif.h"

#define LWIP_POSIX_SOCKETS_IO_NAMES 1

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    WLAN_AIRKISS_SUCCESS = 0,   /* success */
    WLAN_AIRKISS_FAIL    = -1,  /* general error */
    WLAN_AIRKISS_TIMEOUT = -2,  /* wait timeout */
    WLAN_AIRKISS_INVALID = -3,  /* invalid argument */
} wlan_airkiss_status_t;

/*
 * airkiss_recv() return value define
 */
typedef enum
{
    AIRKISS_STATUS_CONTINUE = 0,            /* decode normal, continue airkiss_recv to decode success */
    AIRKISS_STATUS_CHANNEL_LOCKED = 1,      /* channel has locked, stop switch channel immediately */
    AIRKISS_STATUS_COMPLETE = 2             /* decode success, can get reuslt by airkiss_get_result() */
} airkiss_status_t;

typedef struct wlan_airkiss_result
{
    uint8_t ssid[WLAN_SSID_MAX_LEN];
    uint8_t ssid_len;
    uint8_t passphrase[WLAN_PASSPHRASE_MAX_LEN + 1]; /* ASCII string ending with '\0' */
    uint8_t random_num;
} wlan_airkiss_result_t;

wlan_airkiss_status_t wlan_airkiss_start(struct netif *nif, char *key);
airkiss_status_t wlan_airkiss_get_status(void);
wlan_airkiss_status_t wlan_airkiss_get_result(wlan_airkiss_result_t *result);
wlan_airkiss_status_t wlan_airkiss_wait(uint32_t timeout_ms);
wlan_airkiss_status_t
wlan_airkiss_connect_ack(struct netif *nif, uint32_t timeout_ms,
                         wlan_airkiss_result_t *result);

int wlan_airkiss_stop(void);


#ifdef __cplusplus
}
#endif
#endif