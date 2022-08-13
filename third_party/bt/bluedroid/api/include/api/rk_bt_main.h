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

#ifndef __RK_BT_MAIN_H__
#define __RK_BT_MAIN_H__

#include "rk_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Bluetooth stack status type, to indicate whether the bluetooth stack is ready
 */
typedef enum
{
    RK_BLUEDROID_STATUS_UNINITIALIZED   = 0,        /*!< Bluetooth not initialized */
    RK_BLUEDROID_STATUS_INITIALIZED,                /*!< Bluetooth initialized but not enabled */
    RK_BLUEDROID_STATUS_ENABLED                     /*!< Bluetooth initialized and enabled */
} rk_bluedroid_status_t;

/**
 * @brief     Get bluetooth stack status
 *
 * @return    Bluetooth stack status
 *
 */
rk_bluedroid_status_t rk_bluedroid_get_status(void);

/**
 * @brief     Enable bluetooth, must after rk_bluedroid_init()
 *
 * @return
 *            - RK_OK : Succeed
 *            - Other  : Failed
 */
osi_err_t rk_bluedroid_enable(void);

/**
 * @brief     Disable bluetooth, must prior to rk_bluedroid_deinit()
 *
 * @return
 *            - RK_OK : Succeed
 *            - Other  : Failed
 */
osi_err_t rk_bluedroid_disable(void);

/**
 * @brief     Init and alloc the resource for bluetooth, must be prior to every bluetooth stuff
 *
 * @return
 *            - RK_OK : Succeed
 *            - Other  : Failed
 */
osi_err_t rk_bluedroid_init(void);

/**
 * @brief     Deinit and free the resource for bluetooth, must be after every bluetooth stuff
 *
 * @return
 *            - RK_OK : Succeed
 *            - Other  : Failed
 */
osi_err_t rk_bluedroid_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* __RK_BT_MAIN_H__ */
