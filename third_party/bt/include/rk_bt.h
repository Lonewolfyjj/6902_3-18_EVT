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

#ifndef __RK_BT_H__
#define __RK_BT_H__

#include "osi/bt_config.h"
#include <stdint.h>
#include <stdbool.h>
#include "rk_err.h"
#include "osi/thread.h"
#include "osi/types.h"
#include "btHwControl.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RK_BT_CONTROLLER_CONFIG_MAGIC_VAL  0x5A5AA5A5

//extern bt_hw_control_t;

/**
 * @brief Bluetooth mode for controller enable/disable
 */
typedef enum
{
    RK_BT_MODE_IDLE       = 0x00,   /*!< Bluetooth is not running */
    RK_BT_MODE_BLE        = 0x01,   /*!< Run BLE mode */
    RK_BT_MODE_CLASSIC_BT = 0x02,   /*!< Run Classic BT mode */
    RK_BT_MODE_BTDM       = 0x03,   /*!< Run dual mode */
} rk_bt_mode_t;

#ifdef CONFIG_BT_ENABLED
/* While scanning, if the free memory value in controller is less than SCAN_SEND_ADV_RESERVED_SIZE,
the adv packet will be discarded until the memory is restored. */
#define SCAN_SEND_ADV_RESERVED_SIZE        1000
/* enable controller log debug when adv lost */
#define CONTROLLER_ADV_LOST_DEBUG_BIT      (0<<0)

#ifdef CONFIG_BT_HCI_UART_NO
#define BT_HCI_UART_NO_DEFAULT                      CONFIG_BT_HCI_UART_NO
#else
#define BT_HCI_UART_NO_DEFAULT                      1
#endif /* BT_HCI_UART_NO_DEFAULT */

#ifdef CONFIG_BT_HCI_UART_BAUDRATE
#define BT_HCI_UART_BAUDRATE_DEFAULT                CONFIG_BT_HCI_UART_BAUDRATE
#else
#define BT_HCI_UART_BAUDRATE_DEFAULT                921600
#endif /* BT_HCI_UART_BAUDRATE_DEFAULT */

#ifdef CONFIG_SCAN_DUPLICATE_TYPE
#define SCAN_DUPLICATE_TYPE_VALUE  CONFIG_SCAN_DUPLICATE_TYPE
#else
#define SCAN_DUPLICATE_TYPE_VALUE  0
#endif

/* normal adv cache size */
#ifdef CONFIG_DUPLICATE_SCAN_CACHE_SIZE
#define NORMAL_SCAN_DUPLICATE_CACHE_SIZE            CONFIG_DUPLICATE_SCAN_CACHE_SIZE
#else
#define NORMAL_SCAN_DUPLICATE_CACHE_SIZE            20
#endif

#ifndef CONFIG_BLE_MESH_SCAN_DUPLICATE_EN
#define CONFIG_BLE_MESH_SCAN_DUPLICATE_EN FALSE
#endif

#define SCAN_DUPLICATE_MODE_NORMAL_ADV_ONLY         0
#define SCAN_DUPLICATE_MODE_NORMAL_ADV_MESH_ADV     1

#ifdef CONFIG_BLE_MESH_SCAN_DUPLICATE_EN
#define SCAN_DUPLICATE_MODE                     SCAN_DUPLICATE_MODE_NORMAL_ADV_MESH_ADV
#ifdef CONFIG_MESH_DUPLICATE_SCAN_CACHE_SIZE
#define MESH_DUPLICATE_SCAN_CACHE_SIZE          CONFIG_MESH_DUPLICATE_SCAN_CACHE_SIZE
#else
#define MESH_DUPLICATE_SCAN_CACHE_SIZE          50
#endif
#else
#define SCAN_DUPLICATE_MODE SCAN_DUPLICATE_MODE_NORMAL_ADV_ONLY
#define MESH_DUPLICATE_SCAN_CACHE_SIZE          0
#endif

#if defined(CONFIG_BTDM_CONTROLLER_MODE_BLE_ONLY)
#define BTDM_CONTROLLER_MODE_EFF                    RK_BT_MODE_BLE
#elif defined(CONFIG_BTDM_CONTROLLER_MODE_BR_EDR_ONLY)
#define BTDM_CONTROLLER_MODE_EFF                    RK_BT_MODE_CLASSIC_BT
#else
#define BTDM_CONTROLLER_MODE_EFF                    RK_BT_MODE_BTDM
#endif

#define BTDM_CONTROLLER_BLE_MAX_CONN_LIMIT          9   //Maximum BLE connection limitation
#define BTDM_CONTROLLER_BR_EDR_MAX_ACL_CONN_LIMIT   7   //Maximum ACL connection limitation
#define BTDM_CONTROLLER_BR_EDR_MAX_SYNC_CONN_LIMIT  3   //Maximum SCO/eSCO connection limitation

#define BT_CONTROLLER_INIT_CONFIG_DEFAULT() {                              \
    .controller_task_stack_size = RK_TASK_BT_CONTROLLER_STACK,            \
    .controller_task_prio = RK_TASK_BT_CONTROLLER_PRIO,                   \
    .hci_uart_no = BT_HCI_UART_NO_DEFAULT,                                 \
    .hci_uart_baudrate = BT_HCI_UART_BAUDRATE_DEFAULT,                     \
    .scan_duplicate_mode = SCAN_DUPLICATE_MODE,                            \
    .scan_duplicate_type = SCAN_DUPLICATE_TYPE_VALUE,                     \
    .normal_adv_size = NORMAL_SCAN_DUPLICATE_CACHE_SIZE,                   \
    .mesh_adv_size = MESH_DUPLICATE_SCAN_CACHE_SIZE,                       \
    .send_adv_reserved_size = SCAN_SEND_ADV_RESERVED_SIZE,                 \
    .controller_debug_flag = CONTROLLER_ADV_LOST_DEBUG_BIT,                \
    .mode = BTDM_CONTROLLER_MODE_EFF,                                      \
    .ble_max_conn = CONFIG_BTDM_CONTROLLER_BLE_MAX_CONN_EFF,               \
    .bt_max_acl_conn = CONFIG_BTDM_CONTROLLER_BR_EDR_MAX_ACL_CONN_EFF,     \
    .bt_max_sync_conn = CONFIG_BTDM_CONTROLLER_BR_EDR_MAX_SYNC_CONN_EFF,   \
    .magic = RK_BT_CONTROLLER_CONFIG_MAGIC_VAL,                           \
};

#else
#define BT_CONTROLLER_INIT_CONFIG_DEFAULT() {0}; _Static_assert(0, "please enable bluetooth in menuconfig to use bt.h");
#endif

/**
 * @brief Controller config options, depend on config mask.
 *        Config mask indicate which functions enabled, this means
 *        some options or parameters of some functions enabled by config mask.
 */
typedef struct
{
    /*
     * Following parameters can be configured runtime, when call rk_bt_controller_init()
     */
    uint16_t controller_task_stack_size;    /*!< Bluetooth controller task stack size */
    uint8_t controller_task_prio;           /*!< Bluetooth controller task priority */
    uint8_t hci_uart_no;                    /*!< If use UART1/2 as HCI IO interface, indicate UART number */
    uint32_t hci_uart_baudrate;             /*!< If use UART1/2 as HCI IO interface, indicate UART baudrate */
    uint8_t scan_duplicate_mode;            /*!< scan duplicate mode */
    uint8_t scan_duplicate_type;            /*!< scan duplicate type */
    uint16_t normal_adv_size;               /*!< Normal adv size for scan duplicate */
    uint16_t mesh_adv_size;                 /*!< Mesh adv size for scan duplicate */
    uint16_t send_adv_reserved_size;        /*!< Controller minimum memory value */
    uint32_t  controller_debug_flag;        /*!< Controller debug log flag */
    uint8_t mode;                           /*!< Controller mode: BR/EDR, BLE or Dual Mode */
    uint8_t ble_max_conn;                   /*!< BLE maximum connection numbers */
    uint8_t bt_max_acl_conn;                /*!< BR/EDR maximum ACL connection numbers */
    /*
     * Following parameters can not be configured runtime when call rk_bt_controller_init()
     * It will be overwrite with a constant value which in menuconfig or from a macro.
     * So, do not modify the value when rk_bt_controller_init()
     */
    uint8_t bt_max_sync_conn;               /*!< BR/EDR maximum ACL connection numbers. Effective in menuconfig */
    uint32_t magic;                         /*!< Magic number */
} rk_bt_controller_config_t;

/**
 * @brief Bluetooth controller enable/disable/initialised/de-initialised status
 */
typedef enum
{
    RK_BT_CONTROLLER_STATUS_IDLE = 0,
    RK_BT_CONTROLLER_STATUS_INITED,
    RK_BT_CONTROLLER_STATUS_ENABLED,
    RK_BT_CONTROLLER_STATUS_NUM,
} rk_bt_controller_status_t;

/**
 * @brief BLE tx power type
 *        RK_BLE_PWR_TYPE_CONN_HDL0-8: for each connection, and only be set after connection completed.
 *                                      when disconnect, the correspond TX power is not effected.
 *        RK_BLE_PWR_TYPE_ADV : for advertising/scan response.
 *        RK_BLE_PWR_TYPE_SCAN : for scan.
 *        RK_BLE_PWR_TYPE_DEFAULT : if each connection's TX power is not set, it will use this default value.
 *                                   if neither in scan mode nor in adv mode, it will use this default value.
 *        If none of power type is set, system will use RK_PWR_LVL_P3 as default for ADV/SCAN/CONN0-9.
 */
typedef enum
{
    RK_BLE_PWR_TYPE_CONN_HDL0  = 0,            /*!< For connection handle 0 */
    RK_BLE_PWR_TYPE_CONN_HDL1  = 1,            /*!< For connection handle 1 */
    RK_BLE_PWR_TYPE_CONN_HDL2  = 2,            /*!< For connection handle 2 */
    RK_BLE_PWR_TYPE_CONN_HDL3  = 3,            /*!< For connection handle 3 */
    RK_BLE_PWR_TYPE_CONN_HDL4  = 4,            /*!< For connection handle 4 */
    RK_BLE_PWR_TYPE_CONN_HDL5  = 5,            /*!< For connection handle 5 */
    RK_BLE_PWR_TYPE_CONN_HDL6  = 6,            /*!< For connection handle 6 */
    RK_BLE_PWR_TYPE_CONN_HDL7  = 7,            /*!< For connection handle 7 */
    RK_BLE_PWR_TYPE_CONN_HDL8  = 8,            /*!< For connection handle 8 */
    RK_BLE_PWR_TYPE_ADV        = 9,            /*!< For advertising */
    RK_BLE_PWR_TYPE_SCAN       = 10,           /*!< For scan */
    RK_BLE_PWR_TYPE_DEFAULT    = 11,           /*!< For default, if not set other, it will use default value */
    RK_BLE_PWR_TYPE_NUM        = 12,           /*!< TYPE numbers */
} rk_ble_power_type_t;

/**
 * @brief Bluetooth TX power level(index), it's just a index corresponding to power(dbm).
 */
typedef enum
{
    RK_PWR_LVL_N12 = 0,                /*!< Corresponding to -12dbm */
    RK_PWR_LVL_N9  = 1,                /*!< Corresponding to  -9dbm */
    RK_PWR_LVL_N6  = 2,                /*!< Corresponding to  -6dbm */
    RK_PWR_LVL_N3  = 3,                /*!< Corresponding to  -3dbm */
    RK_PWR_LVL_N0  = 4,                /*!< Corresponding to   0dbm */
    RK_PWR_LVL_P3  = 5,                /*!< Corresponding to  +3dbm */
    RK_PWR_LVL_P6  = 6,                /*!< Corresponding to  +6dbm */
    RK_PWR_LVL_P9  = 7,                /*!< Corresponding to  +9dbm */
    RK_PWR_LVL_N14 = RK_PWR_LVL_N12,  /*!< Backward compatibility! Setting to -14dbm will actually result to -12dbm */
    RK_PWR_LVL_N11 = RK_PWR_LVL_N9,   /*!< Backward compatibility! Setting to -11dbm will actually result to  -9dbm */
    RK_PWR_LVL_N8  = RK_PWR_LVL_N6,   /*!< Backward compatibility! Setting to  -8dbm will actually result to  -6dbm */
    RK_PWR_LVL_N5  = RK_PWR_LVL_N3,   /*!< Backward compatibility! Setting to  -5dbm will actually result to  -3dbm */
    RK_PWR_LVL_N2  = RK_PWR_LVL_N0,   /*!< Backward compatibility! Setting to  -2dbm will actually result to   0dbm */
    RK_PWR_LVL_P1  = RK_PWR_LVL_P3,   /*!< Backward compatibility! Setting to  +1dbm will actually result to  +3dbm */
    RK_PWR_LVL_P4  = RK_PWR_LVL_P6,   /*!< Backward compatibility! Setting to  +4dbm will actually result to  +6dbm */
    RK_PWR_LVL_P7  = RK_PWR_LVL_P9,   /*!< Backward compatibility! Setting to  +7dbm will actually result to  +9dbm */
} rk_power_level_t;

/**
 * @brief Bluetooth audio data transport path
 */
typedef enum
{
    RK_SCO_DATA_PATH_HCI = 0,            /*!< data over HCI transport */
    RK_SCO_DATA_PATH_PCM = 1,            /*!< data over PCM interface */
} rk_sco_data_path_t;

/**
 * @brief  Set BLE TX power
 *         Connection Tx power should only be set after connection created.
 * @param  power_type : The type of which tx power, could set Advertising/Connection/Default and etc
 * @param  power_level: Power level(index) corresponding to absolute value(dbm)
 * @return              RK_OK - success, other - failed
 */
osi_err_t rk_ble_tx_power_set(rk_ble_power_type_t power_type, rk_power_level_t power_level);

/**
 * @brief  Get BLE TX power
 *         Connection Tx power should only be get after connection created.
 * @param  power_type : The type of which tx power, could set Advertising/Connection/Default and etc
 * @return             >= 0 - Power level, < 0 - Invalid
 */
rk_power_level_t rk_ble_tx_power_get(rk_ble_power_type_t power_type);

/**
 * @brief  Set BR/EDR TX power
 *         BR/EDR power control will use the power in range of minimum value and maximum value.
 *         The power level will effect the global BR/EDR TX power, such inquire, page, connection and so on.
 *         Please call the function after rk_bt_controller_enable and before any function which cause RF do TX.
 *         So you can call the function before doing discovery, profile init and so on.
 *         For example, if you want BR/EDR use the new TX power to do inquire, you should call
 *         this function before inquire. Another word, If call this function when BR/EDR is in inquire(ING),
 *         please do inquire again after call this function.
 *         Default minimum power level is RK_PWR_LVL_N0, and maximum power level is RK_PWR_LVL_P3.
 * @param  min_power_level: The minimum power level
 * @param  max_power_level: The maximum power level
 * @return              RK_OK - success, other - failed
 */
osi_err_t rk_bredr_tx_power_set(rk_power_level_t min_power_level, rk_power_level_t max_power_level);

/**
 * @brief  Get BR/EDR TX power
 *         If the argument is not NULL, then store the corresponding value.
 * @param  min_power_level: The minimum power level
 * @param  max_power_level: The maximum power level
 * @return              RK_OK - success, other - failed
 */
osi_err_t rk_bredr_tx_power_get(rk_power_level_t *min_power_level, rk_power_level_t *max_power_level);

/**
 * @brief  set default SCO data path
 *         Should be called after controller is enabled, and before (e)SCO link is established
 * @param  data_path: SCO data path
 * @return              RK_OK - success, other - failed
 */
osi_err_t rk_bredr_sco_datapath_set(rk_sco_data_path_t data_path);

/**
 * @brief       Initialize BT controller to allocate task and other resource.
 *              This function should be called only once, before any other BT functions are called.
 * @param  cfg: Initial configuration of BT controller. Different from previous version, there's a mode and some
 *              connection configuration in "cfg" to configure controller work mode and allocate the resource which is needed.
 * @return      RK_OK - success, other - failed
 */
osi_err_t rk_bt_controller_init(rk_bt_controller_config_t *cfg, bt_hw_control_t *hw_control);

/**
 * @brief  De-initialize BT controller to free resource and delete task.
 *
 * This function should be called only once, after any other BT functions are called.
 * This function is not whole completed, rk_bt_controller_init cannot called after this function.
 * @return  RK_OK - success, other - failed
 */
osi_err_t rk_bt_controller_deinit(void);

/**
 * @brief Enable BT controller.
 *               Due to a known issue, you cannot call rk_bt_controller_enable() a second time
 *               to change the controller mode dynamically. To change controller mode, call
 *               rk_bt_controller_disable() and then call rk_bt_controller_enable() with the new mode.
 * @param mode : the mode(BLE/BT/BTDM) to enable. For compatible of API, retain this argument. This mode must be
 *               equal as the mode in "cfg" of rk_bt_controller_init().
 * @return       RK_OK - success, other - failed
 */
osi_err_t rk_bt_controller_enable(rk_bt_mode_t mode);

/**
 * @brief  Disable BT controller
 * @return       RK_OK - success, other - failed
 */
osi_err_t rk_bt_controller_disable(void);

/**
 * @brief  Get BT controller is initialised/de-initialised/enabled/disabled
 * @return status value
 */
rk_bt_controller_status_t rk_bt_controller_get_status(void);

/** @brief rk_vhci_host_callback
 *  used for vhci call host function to notify what host need to do
 */
typedef struct rk_vhci_host_callback
{
    void (*notify_host_send_available)(void);               /*!< callback used to notify that the host can send packet to controller */
    int (*notify_host_recv)(uint8_t *data, uint16_t len);   /*!< callback used to notify that the controller has a packet to send to the host*/
} rk_vhci_host_callback_t;

/** @brief rk_vhci_host_check_send_available
 *  used for check actively if the host can send packet to controller or not.
 *  @return true for ready to send, false means cannot send packet
 */
bool rk_vhci_host_check_send_available(void);

/** @brief rk_vhci_host_send_packet
 * host send packet to controller
 *
 * Should not call this function from within a critical section
 * or when the scheduler is suspended.
 *
 * @param data the packet point
 * @param len the packet length
 */
void rk_vhci_host_send_packet(uint8_t *data, uint16_t len);

/** @brief rk_vhci_host_recv_packet
 * host get packet from controller
 *
 * Should not call this function from within a critical section
 * or when the scheduler is suspended.
 *
 * @param data the packet point
 * @return pkt length
 */
int rk_vhci_host_recv_packet(uint8_t *data);

/** @brief rk_vhci_host_register_callback
 * register the vhci reference callback
 * struct defined by vhci_host_callback structure.
 * @param callback rk_vhci_host_callback type variable
 * @return RK_OK - success, RK_FAIL - failed
 */
//osi_err_t rk_vhci_host_register_callback(const rk_vhci_host_callback_t *callback);

/** @brief rk_bt_controller_mem_release
 * release the controller memory as per the mode
 *
 * This function releases the BSS, data and other sections of the controller to heap. The total size is about 70k bytes.
 *
 * rk_bt_controller_mem_release(mode) should be called only before rk_bt_controller_init()
 * or after rk_bt_controller_deinit().
 *
 * Note that once BT controller memory is released, the process cannot be reversed. It means you cannot use the bluetooth
 * mode which you have released by this function.
 *
 * If your firmware will later upgrade the Bluetooth controller mode (BLE -> BT Classic or disabled -> enabled)
 * then do not call this function.
 *
 * If the app calls rk_bt_controller_enable(RK_BT_MODE_BLE) to use BLE only then it is safe to call
 * rk_bt_controller_mem_release(RK_BT_MODE_CLASSIC_BT) at initialization time to free unused BT Classic memory.
 *
 * If the mode is RK_BT_MODE_BTDM, then it may be useful to call API rk_bt_mem_release(RK_BT_MODE_BTDM) instead,
 * which internally calls rk_bt_controller_mem_release(RK_BT_MODE_BTDM) and additionally releases the BSS and data
 * consumed by the BT/BLE host stack to heap. For more details about usage please refer to the documentation of
 * rk_bt_mem_release() function
 *
 * @param mode : the mode want to release memory
 * @return RK_OK - success, other - failed
 */
osi_err_t rk_bt_controller_mem_release(rk_bt_mode_t mode);

/** @brief rk_bt_mem_release
 * release controller memory and BSS and data section of the BT/BLE host stack as per the mode
 *
 * This function first releases controller memory by internally calling rk_bt_controller_mem_release().
 * Additionally, if the mode is set to RK_BT_MODE_BTDM, it also releases the BSS and data consumed by the BT/BLE host stack to heap
 *
 * Note that once BT memory is released, the process cannot be reversed. It means you cannot use the bluetooth
 * mode which you have released by this function.
 *
 * If your firmware will later upgrade the Bluetooth controller mode (BLE -> BT Classic or disabled -> enabled)
 * then do not call this function.
 *
 * If you never intend to use bluetooth in a current boot-up cycle, you can call rk_bt_mem_release(RK_BT_MODE_BTDM)
 * before rk_bt_controller_init or after rk_bt_controller_deinit.
 *
 * For example, if a user only uses bluetooth for setting the WiFi configuration, and does not use bluetooth in the rest of the product operation".
 * In such cases, after receiving the WiFi configuration, you can disable/deinit bluetooth and release its memory.
 * Below is the sequence of APIs to be called for such scenarios:
 *
 *      rk_bluedroid_disable();
 *      rk_bluedroid_deinit();
 *      rk_bt_controller_disable();
 *      rk_bt_controller_deinit();
 *      rk_bt_mem_release(RK_BT_MODE_BTDM);
 *
 * @param mode : the mode whose memory is to be released
 * @return RK_OK - success, other - failed
 */
osi_err_t rk_bt_mem_release(rk_bt_mode_t mode);

/**
 * @brief enable bluetooth to enter modem sleep
 *
 * Note that this function shall not be invoked before rk_bt_controller_enable()
 *
 * There are currently two options for bluetooth modem sleep, one is ORIG mode, and another is EVED Mode. EVED Mode is intended for BLE only.
 *
 * For ORIG mode:
 * Bluetooth modem sleep is enabled in controller start up by default if CONFIG_BTDM_CONTROLLER_MODEM_SLEEP is set and "ORIG mode" is selected. In ORIG modem sleep mode, bluetooth controller will switch off some components and pause to work every now and then, if there is no event to process; and wakeup according to the scheduled interval and resume the work. It can also wakeup earlier upon external request using function "rk_bt_controller_wakeup_request".
 *
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 */
osi_err_t rk_bt_sleep_enable(void);


/**
 * @brief disable bluetooth modem sleep
 *
 * Note that this function shall not be invoked before rk_bt_controller_enable()
 *
 * If rk_bt_sleep_disable() is called, bluetooth controller will not be allowed to enter modem sleep;
 *
 * If ORIG modem sleep mode is in use, if this function is called, bluetooth controller may not immediately wake up if it is dormant then.
 * In this case, rk_bt_controller_wakeup_request() can be used to shorten the time for wakeup.
 *
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 */
osi_err_t rk_bt_sleep_disable(void);

/**
 * @brief to check whether bluetooth controller is sleeping at the instant, if modem sleep is enabled
 *
 * Note that this function shall not be invoked before rk_bt_controller_enable()
 * This function is supposed to be used ORIG mode of modem sleep
 *
 * @return  true if in modem sleep state, false otherwise
 */
bool rk_bt_controller_is_sleeping(void);

/**
 * @brief request controller to wakeup from sleeping state during sleep mode
 *
 * Note that this function shall not be invoked before rk_bt_controller_enable()
 * Note that this function is supposed to be used ORIG mode of modem sleep
 * Note that after this request, bluetooth controller may again enter sleep as long as the modem sleep is enabled
 *
 * Profiling shows that it takes several milliseconds to wakeup from modem sleep after this request.
 * Generally it takes longer if 32kHz XTAL is used than the main XTAL, due to the lower frequency of the former as the bluetooth low power clock source.
 */
void rk_bt_controller_wakeup_request(void);

/**
 * @brief Manually clear scan duplicate list
 *
 * Note that scan duplicate list will be automatically cleared when the maximum amount of device in the filter is reached
 * the amount of device in the filter can be configured in menuconfig.
 *
 *
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 */
osi_err_t rk_ble_scan_dupilcate_list_flush(void);

#ifdef __cplusplus
}
#endif

#endif /* __RK_BT_H__ */
