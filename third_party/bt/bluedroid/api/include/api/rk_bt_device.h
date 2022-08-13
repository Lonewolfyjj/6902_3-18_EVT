// Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __RK_BT_DEVICE_H__
#define __RK_BT_DEVICE_H__

#include <stdint.h>
#include <stdbool.h>
#include "rk_err.h"
#include "rk_bt_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * @brief      Get bluetooth device address.  Must use after "rk_bluedroid_enable".
 *
 * @return     bluetooth device address (six bytes), or NULL if bluetooth stack is not enabled
 */
const uint8_t *rk_bt_dev_get_address(void);


/**
 * @brief           Set bluetooth device name. This function should be called after rk_bluedroid_enable()
 *                  completes successfully.
 *                  A BR/EDR/LE device type shall have a single Bluetooth device name which shall be
 *                  identical irrespective of the physical channel used to perform the name discovery procedure.
 *
 * @param[in]       name : device name to be set
 *
 * @return
 *                  - RK_OK : Succeed
 *                  - RK_ERR_INVALID_ARG : if name is NULL pointer or empty, or string length out of limit
 *                  - RK_ERR_INVALID_STATE : if bluetooth stack is not yet enabled
 *                  - RK_FAIL : others
 */
osi_err_t rk_bt_dev_set_device_name(const char *name);

#ifdef __cplusplus
}
#endif


#endif /* __RK_BT_DEVICE_H__ */
