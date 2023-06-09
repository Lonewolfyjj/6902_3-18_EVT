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

#ifndef __THREAD_H__
#define __THREAD_H__

#include "osi/types.h"
#include "common/bt_defs.h"

#define portBASE_TYPE int

struct bt_task_evt
{
    uint32_t    sig;    //task sig
    void       *par;    //point to task param
    void       *cb;     //point to function cb
    void       *arg;    //point to function arg
};
typedef struct bt_task_evt BtTaskEvt_t;

typedef bt_status_t (* BtTaskCb_t)(void *arg);

typedef enum
{
    SIG_HCI_HAL_RECV_PACKET = 0,
    SIG_HCI_HAL_NUM,
} SIG_HCI_HAL_t;

typedef enum
{
    SIG_HCI_HOST_SEND_AVAILABLE = 0,
    SIG_HCI_HOST_NUM,
} SIG_HCI_HOST_t;

typedef enum
{
    SIG_BTU_START_UP = 0,
    SIG_BTU_HCI_MSG,
    SIG_BTU_BTA_MSG,
    SIG_BTU_BTA_ALARM,
    SIG_BTU_GENERAL_ALARM,
    SIG_BTU_ONESHOT_ALARM,
    SIG_BTU_L2CAP_ALARM,
    SIG_BTU_NUM,
} SIG_BTU_t;


/* Bt contoller Task */
/* controller */
#define RK_TASK_BT_CONTROLLER_PRIO   (RK_TASK_PRIO_MAX - 2)

// #ifdef CONFIG_NEWLIB_NANO_FORMAT
// #define TASK_EXTRA_STACK_SIZE      (0)
// #else
#define TASK_EXTRA_STACK_SIZE      (1024)
//#endif

#ifndef CONFIG_BTU_TASK_STACK_SIZE
    #define CONFIG_BTU_TASK_STACK_SIZE   4096
#endif
#ifndef CONFIG_BTC_TASK_STACK_SIZE
    #define CONFIG_BTC_TASK_STACK_SIZE   3072
#endif
#ifndef CONFIG_A2DP_SINK_TASK_STACK_SIZE
    #define CONFIG_A2DP_SINK_TASK_STACK_SIZE 2048
#endif

#define OSI_TASK_TICK_VALUE      10
#define OSI_TASK_PRIO_MAX           10
//RT_THREAD_PRIORITY_MAX
#define BT_TASK_EXTRA_STACK_SIZE      TASK_EXTRA_STACK_SIZE
#define RK_TASK_BT_CONTROLLER_STACK  (3584 + TASK_EXTRA_STACK_SIZE)

#define HCI_HOST_TASK_VALUE                 (OSI_TASK_TICK_VALUE)
#ifndef HCI_HOST_TASK_STACK_SIZE
    #define HCI_HOST_TASK_STACK_SIZE        (2048 + BT_TASK_EXTRA_STACK_SIZE)
#endif
#define HCI_HOST_TASK_PRIO              (OSI_TASK_PRIO_MAX + 3)
#define HCI_HOST_TASK_NAME              "hciHostT"
#define HCI_HOST_QUEUE_LEN              40

#define HCI_H4_TASK_VALUE               (OSI_TASK_TICK_VALUE)
#ifndef HCI_H4_TASK_STACK_SIZE
    #define HCI_H4_TASK_STACK_SIZE          (2048 + BT_TASK_EXTRA_STACK_SIZE)
#endif
#define HCI_H4_TASK_PRIO                (OSI_TASK_PRIO_MAX + 5)
#define HCI_H4_TASK_NAME                "hci_rx_T"
#define HCI_H4_QUEUE_LEN                1

#define BTU_TASK_VALUE                      (OSI_TASK_TICK_VALUE)
#ifndef BTU_TASK_STACK_SIZE
    #define BTU_TASK_STACK_SIZE             (CONFIG_BTU_TASK_STACK_SIZE + BT_TASK_EXTRA_STACK_SIZE)
#endif
#define BTU_TASK_PRIO                   (OSI_TASK_PRIO_MAX + 7)
#define BTU_TASK_NAME                   "btuT"
#define BTU_QUEUE_LEN                   50


#define BTC_TASK_VALUE                       (OSI_TASK_TICK_VALUE)
#ifndef BTC_TASK_STACK_SIZE
    #define BTC_TASK_STACK_SIZE             (CONFIG_BTC_TASK_STACK_SIZE + BT_TASK_EXTRA_STACK_SIZE) //by menuconfig
#endif
#define BTC_TASK_NAME                   "btcT"
#define BTC_TASK_PRIO                   (OSI_TASK_PRIO_MAX + 9)
#define BTC_TASK_QUEUE_LEN              60


#define BTC_A2DP_SINK_TASK_VALUE                     (OSI_TASK_TICK_VALUE)
#ifndef BTC_A2DP_SINK_TASK_STACK_SIZE
    #define BTC_A2DP_SINK_TASK_STACK_SIZE         (CONFIG_A2DP_SINK_TASK_STACK_SIZE + BT_TASK_EXTRA_STACK_SIZE) // by menuconfig
#endif
#define BTC_A2DP_SINK_TASK_NAME               "BtA2dSinkT"
#define BTC_A2DP_SINK_TASK_PRIO               (OSI_TASK_PRIO_MAX + 3)
#define BTC_A2DP_SINK_DATA_QUEUE_LEN          (3)
#define BTC_A2DP_SINK_CTRL_QUEUE_LEN          (5)
#define BTC_A2DP_SINK_TASK_QUEUE_SET_LEN      (BTC_A2DP_SINK_DATA_QUEUE_LEN + BTC_A2DP_SINK_CTRL_QUEUE_LEN)

#define BTC_A2DP_SOURCE_TASK_VALUE                    (OSI_TASK_TICK_VALUE)
#ifndef BTC_A2DP_SOURCE_TASK_STACK_SIZE
    #define BTC_A2DP_SOURCE_TASK_STACK_SIZE       (CONFIG_A2DP_SOURCE_TASK_STACK_SIZE + BT_TASK_EXTRA_STACK_SIZE) // by menuconfig
#endif
#define BTC_A2DP_SOURCE_TASK_NAME             "BtA2dSourceT"
#define BTC_A2DP_SOURCE_TASK_PRIO             (OSI_TASK_PRIO_MAX + 3)
#define BTC_A2DP_SOURCE_DATA_QUEUE_LEN        (3)
#define BTC_A2DP_SOURCE_CTRL_QUEUE_LEN        (5)
#define BTC_A2DP_SOURCE_TASK_QUEUE_SET_LEN    (BTC_A2DP_SOURCE_DATA_QUEUE_LEN + BTC_A2DP_SOURCE_CTRL_QUEUE_LEN)


#define TASK_POST_NON_BLOCKING          (0)
#define TASK_POST_BLOCKING              (RT_WAITING_FOREVER)
typedef uint32_t task_post_t;           /* Timeout of task post return, unit TICK */

typedef enum
{
    TASK_POST_SUCCESS = 0,
    TASK_POST_FAIL,
} task_post_status_t;

task_post_status_t btu_task_post(uint32_t sig, void *param, task_post_t timeout);
task_post_status_t hci_host_task_post(task_post_t timeout);
task_post_status_t hci_hal_h4_task_post(task_post_t timeout);

#endif /* __THREAD_H__ */
