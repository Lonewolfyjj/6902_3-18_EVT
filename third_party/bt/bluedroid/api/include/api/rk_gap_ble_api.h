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

#ifndef __RK_GAP_BLE_API_H__
#define __RK_GAP_BLE_API_H__

#include <stdint.h>
#include <stdbool.h>

#include "rk_err.h"
#include "rk_bt_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@{
 * BLE_ADV_DATA_FLAG data flag bit definition used for advertising data flag
 */
#define RK_BLE_ADV_FLAG_LIMIT_DISC         (0x01 << 0)
#define RK_BLE_ADV_FLAG_GEN_DISC           (0x01 << 1)
#define RK_BLE_ADV_FLAG_BREDR_NOT_SPT      (0x01 << 2)
#define RK_BLE_ADV_FLAG_DMT_CONTROLLER_SPT (0x01 << 3)
#define RK_BLE_ADV_FLAG_DMT_HOST_SPT       (0x01 << 4)
#define RK_BLE_ADV_FLAG_NON_LIMIT_DISC     (0x00 )
/**
 * @}
 */

/* relate to BTM_LE_KEY_xxx in stack/btm_api.h */
#define RK_LE_KEY_NONE                    0                                                                         /* relate to BTM_LE_KEY_NONE in stack/btm_api.h */
#define RK_LE_KEY_PENC                    (1 << 0)   /*!< encryption key, encryption information of peer device */  /* relate to BTM_LE_KEY_PENC in stack/btm_api.h */
#define RK_LE_KEY_PID                     (1 << 1)   /*!< identity key of the peer device */                        /* relate to BTM_LE_KEY_PID in stack/btm_api.h */
#define RK_LE_KEY_PCSRK                   (1 << 2)   /*!< peer SRK */                                               /* relate to BTM_LE_KEY_PCSRK in stack/btm_api.h */
#define RK_LE_KEY_PLK                     (1 << 3)   /*!< Link key*/                                                /* relate to BTM_LE_KEY_PLK in stack/btm_api.h */
#define RK_LE_KEY_LLK                     (RK_LE_KEY_PLK << 4)                                                     /* relate to BTM_LE_KEY_LLK in stack/btm_api.h */
#define RK_LE_KEY_LENC                    (RK_LE_KEY_PENC << 4)   /*!< master role security information:div */     /* relate to BTM_LE_KEY_LENC in stack/btm_api.h */
#define RK_LE_KEY_LID                     (RK_LE_KEY_PID << 4)    /*!< master device ID key */                     /* relate to BTM_LE_KEY_LID in stack/btm_api.h */
#define RK_LE_KEY_LCSRK                   (RK_LE_KEY_PCSRK << 4)  /*!< local CSRK has been deliver to peer */      /* relate to BTM_LE_KEY_LCSRK in stack/btm_api.h */
typedef uint8_t rk_ble_key_type_t;

/* relate to BTM_LE_AUTH_xxx in stack/btm_api.h */
#define RK_LE_AUTH_NO_BOND                 0x00                                     /*!< 0*/                     /* relate to BTM_LE_AUTH_NO_BOND in stack/btm_api.h */
#define RK_LE_AUTH_BOND                    0x01                                     /*!< 1 << 0 */               /* relate to BTM_LE_AUTH_BOND in stack/btm_api.h */
#define RK_LE_AUTH_REQ_MITM                (1 << 2)                                 /*!< 1 << 2 */               /* relate to BTM_LE_AUTH_REQ_MITM in stack/btm_api.h */
#define RK_LE_AUTH_REQ_BOND_MITM           (RK_LE_AUTH_BOND | RK_LE_AUTH_REQ_MITM)/*!< 0101*/
#define RK_LE_AUTH_REQ_SC_ONLY             (1 << 3)                                 /*!< 1 << 3 */               /* relate to BTM_LE_AUTH_REQ_SC_ONLY in stack/btm_api.h */
#define RK_LE_AUTH_REQ_SC_BOND             (RK_LE_AUTH_BOND | RK_LE_AUTH_REQ_SC_ONLY)            /*!< 1001 */  /* relate to BTM_LE_AUTH_REQ_SC_BOND in stack/btm_api.h */
#define RK_LE_AUTH_REQ_SC_MITM             (RK_LE_AUTH_REQ_MITM | RK_LE_AUTH_REQ_SC_ONLY)        /*!< 1100 */  /* relate to BTM_LE_AUTH_REQ_SC_MITM in stack/btm_api.h */
#define RK_LE_AUTH_REQ_SC_MITM_BOND        (RK_LE_AUTH_REQ_MITM | RK_LE_AUTH_REQ_SC_ONLY | RK_LE_AUTH_BOND)   /*!< 1101 */  /* relate to BTM_LE_AUTH_REQ_SC_MITM_BOND in stack/btm_api.h */
typedef uint8_t   rk_ble_auth_req_t;         /*!< combination of the above bit pattern */

#define RK_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_DISABLE 0
#define RK_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_ENABLE  1

#define RK_BLE_OOB_DISABLE 0
#define RK_BLE_OOB_ENABLE  1

/* relate to BTM_IO_CAP_xxx in stack/btm_api.h */
#define RK_IO_CAP_OUT                      0   /*!< DisplayOnly */         /* relate to BTM_IO_CAP_OUT in stack/btm_api.h */
#define RK_IO_CAP_IO                       1   /*!< DisplayYesNo */        /* relate to BTM_IO_CAP_IO in stack/btm_api.h */
#define RK_IO_CAP_IN                       2   /*!< KeyboardOnly */        /* relate to BTM_IO_CAP_IN in stack/btm_api.h */
#define RK_IO_CAP_NONE                     3   /*!< NoInputNoOutput */     /* relate to BTM_IO_CAP_NONE in stack/btm_api.h */
#define RK_IO_CAP_KBDISP                   4   /*!< Keyboard display */    /* relate to BTM_IO_CAP_KBDISP in stack/btm_api.h */

#define RK_BLE_APPEARANCE_UNKNOWN                 0x0000 /* relate to BTM_BLE_APPEARANCE_UNKNOWN in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_PHONE           0x0040 /* relate to BTM_BLE_APPEARANCE_GENERIC_PHONE in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_COMPUTER        0x0080 /* relate to BTM_BLE_APPEARANCE_GENERIC_COMPUTER in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_WATCH           0x00C0 /* relate to BTM_BLE_APPEARANCE_GENERIC_WATCH in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_SPORTS_WATCH            0x00C1 /* relate to BTM_BLE_APPEARANCE_SPORTS_WATCH in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_CLOCK           0x0100 /* relate to BTM_BLE_APPEARANCE_GENERIC_CLOCK in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_DISPLAY         0x0140 /* relate to BTM_BLE_APPEARANCE_GENERIC_DISPLAY in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_REMOTE          0x0180 /* relate to BTM_BLE_APPEARANCE_GENERIC_REMOTE in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_EYEGLASSES      0x01C0 /* relate to BTM_BLE_APPEARANCE_GENERIC_EYEGLASSES in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_TAG             0x0200 /* relate to BTM_BLE_APPEARANCE_GENERIC_TAG in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_KEYRING         0x0240 /* relate to BTM_BLE_APPEARANCE_GENERIC_KEYRING in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_MEDIA_PLAYER    0x0280 /* relate to BTM_BLE_APPEARANCE_GENERIC_MEDIA_PLAYER in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_BARCODE_SCANNER 0x02C0 /* relate to BTM_BLE_APPEARANCE_GENERIC_BARCODE_SCANNER in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_THERMOMETER     0x0300 /* relate to BTM_BLE_APPEARANCE_GENERIC_THERMOMETER in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_THERMOMETER_EAR         0x0301 /* relate to BTM_BLE_APPEARANCE_THERMOMETER_EAR in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_HEART_RATE      0x0340 /* relate to BTM_BLE_APPEARANCE_GENERIC_HEART_RATE in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_HEART_RATE_BELT         0x0341 /* relate to BTM_BLE_APPEARANCE_HEART_RATE_BELT in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_BLOOD_PRESSURE  0x0380 /* relate to BTM_BLE_APPEARANCE_GENERIC_BLOOD_PRESSURE in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_BLOOD_PRESSURE_ARM      0x0381 /* relate to BTM_BLE_APPEARANCE_BLOOD_PRESSURE_ARM in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_BLOOD_PRESSURE_WRIST    0x0382 /* relate to BTM_BLE_APPEARANCE_BLOOD_PRESSURE_WRIST in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_HID             0x03C0 /* relate to BTM_BLE_APPEARANCE_GENERIC_HID in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_HID_KEYBOARD            0x03C1 /* relate to BTM_BLE_APPEARANCE_HID_KEYBOARD in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_HID_MOUSE               0x03C2 /* relate to BTM_BLE_APPEARANCE_HID_MOUSE in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_HID_JOYSTICK            0x03C3 /* relate to BTM_BLE_APPEARANCE_HID_JOYSTICK in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_HID_GAMEPAD             0x03C4 /* relate to BTM_BLE_APPEARANCE_HID_GAMEPAD in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_HID_DIGITIZER_TABLET    0x03C5 /* relate to BTM_BLE_APPEARANCE_HID_DIGITIZER_TABLET in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_HID_CARD_READER         0x03C6 /* relate to BTM_BLE_APPEARANCE_HID_CARD_READER in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_HID_DIGITAL_PEN         0x03C7 /* relate to BTM_BLE_APPEARANCE_HID_DIGITAL_PEN in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_HID_BARCODE_SCANNER     0x03C8 /* relate to BTM_BLE_APPEARANCE_HID_BARCODE_SCANNER in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_GLUCOSE         0x0400 /* relate to BTM_BLE_APPEARANCE_GENERIC_GLUCOSE in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_WALKING         0x0440 /* relate to BTM_BLE_APPEARANCE_GENERIC_WALKING in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_WALKING_IN_SHOE         0x0441 /* relate to BTM_BLE_APPEARANCE_WALKING_IN_SHOE in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_WALKING_ON_SHOE         0x0442 /* relate to BTM_BLE_APPEARANCE_WALKING_ON_SHOE in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_WALKING_ON_HIP          0x0443 /* relate to BTM_BLE_APPEARANCE_WALKING_ON_HIP in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_CYCLING         0x0480 /* relate to BTM_BLE_APPEARANCE_GENERIC_CYCLING in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_CYCLING_COMPUTER        0x0481 /* relate to BTM_BLE_APPEARANCE_CYCLING_COMPUTER in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_CYCLING_SPEED           0x0482 /* relate to BTM_BLE_APPEARANCE_CYCLING_SPEED in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_CYCLING_CADENCE         0x0483 /* relate to BTM_BLE_APPEARANCE_CYCLING_CADENCE in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_CYCLING_POWER           0x0484 /* relate to BTM_BLE_APPEARANCE_CYCLING_POWER in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_CYCLING_SPEED_CADENCE   0x0485 /* relate to BTM_BLE_APPEARANCE_CYCLING_SPEED_CADENCE in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_PULSE_OXIMETER  0x0C40 /* relate to BTM_BLE_APPEARANCE_GENERIC_PULSE_OXIMETER in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_PULSE_OXIMETER_FINGERTIP 0x0C41 /* relate to BTM_BLE_APPEARANCE_PULSE_OXIMETER_FINGERTIP in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_PULSE_OXIMETER_WRIST    0x0C42 /* relate to BTM_BLE_APPEARANCE_PULSE_OXIMETER_WRIST in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_WEIGHT          0x0C80 /* relate to BTM_BLE_APPEARANCE_GENERIC_WEIGHT in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_PERSONAL_MOBILITY_DEVICE    0x0CC0 /* relate to BTM_BLE_APPEARANCE_GENERIC_PERSONAL_MOBILITY_DEVICE in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_POWERED_WHEELCHAIR                  0x0CC1 /* relate to BTM_BLE_APPEARANCE_POWERED_WHEELCHAIR in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_MOBILITY_SCOOTER                    0x0CC2 /* relate to BTM_BLE_APPEARANCE_MOBILITY_SCOOTER in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_CONTINUOUS_GLUCOSE_MONITOR  0x0D00 /* relate to BTM_BLE_APPEARANCE_GENERIC_CONTINUOUS_GLUCOSE_MONITOR in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_INSULIN_PUMP                0x0D40 /* relate to BTM_BLE_APPEARANCE_GENERIC_INSULIN_PUMP in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_INSULIN_PUMP_DURABLE_PUMP           0x0D41 /* relate to BTM_BLE_APPEARANCE_INSULIN_PUMP_DURABLE_PUMP in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_INSULIN_PUMP_PATCH_PUMP             0x0D44 /* relate to BTM_BLE_APPEARANCE_INSULIN_PUMP_PATCH_PUMP in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_INSULIN_PEN                         0x0D48 /* relate to BTM_BLE_APPEARANCE_INSULIN_PEN in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_MEDICATION_DELIVERY         0x0D80 /* relate to BTM_BLE_APPEARANCE_GENERIC_MEDICATION_DELIVERY in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_GENERIC_OUTDOOR_SPORTS  0x1440             /* relate to BTM_BLE_APPEARANCE_GENERIC_OUTDOOR_SPORTS in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION 0x1441             /* relate to BTM_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_AND_NAV     0x1442 /* relate to BTM_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_AND_NAV in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_POD         0x1443 /* relate to BTM_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_POD in stack/btm_ble_api.h */
#define RK_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_POD_AND_NAV 0x1444 /* relate to BTM_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_POD_AND_NAV in stack/btm_ble_api.h */

typedef uint8_t rk_ble_io_cap_t;               /*!< combination of the io capability */

/// GAP BLE callback event type
typedef enum
{
    RK_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT        = 0,       /*!< When advertising data set complete, the event comes */
    RK_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT,             /*!< When scan response data set complete, the event comes */
    RK_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT,                /*!< When scan parameters set complete, the event comes */
    RK_GAP_BLE_SCAN_RESULT_EVT,                            /*!< When one scan result ready, the event comes each time */
    RK_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT,              /*!< When raw advertising data set complete, the event comes */
    RK_GAP_BLE_SCAN_RSP_DATA_RAW_SET_COMPLETE_EVT,         /*!< When raw advertising data set complete, the event comes */
    RK_GAP_BLE_ADV_START_COMPLETE_EVT,                     /*!< When start advertising complete, the event comes */
    RK_GAP_BLE_SCAN_START_COMPLETE_EVT,                    /*!< When start scan complete, the event comes */
    RK_GAP_BLE_AUTH_CMPL_EVT,                              /* Authentication complete indication. */
    RK_GAP_BLE_KEY_EVT,                                    /* BLE  key event for peer device keys */
    RK_GAP_BLE_SEC_REQ_EVT,                                /* BLE  security request */
    RK_GAP_BLE_PASSKEY_NOTIF_EVT,                          /* passkey notification event */
    RK_GAP_BLE_PASSKEY_REQ_EVT,                            /* passkey request event */
    RK_GAP_BLE_OOB_REQ_EVT,                                /* OOB request event */
    RK_GAP_BLE_LOCAL_IR_EVT,                               /* BLE local IR event */
    RK_GAP_BLE_LOCAL_ER_EVT,                               /* BLE local ER event */
    RK_GAP_BLE_NC_REQ_EVT,                                 /* Numeric Comparison request event */
    RK_GAP_BLE_ADV_STOP_COMPLETE_EVT,                      /*!< When stop adv complete, the event comes */
    RK_GAP_BLE_SCAN_STOP_COMPLETE_EVT,                     /*!< When stop scan complete, the event comes */
    RK_GAP_BLE_SET_STATIC_RAND_ADDR_EVT,                   /*!< When set the static rand address complete, the event comes */
    RK_GAP_BLE_UPDATE_CONN_PARAMS_EVT,                     /*!< When update connection parameters complete, the event comes */
    RK_GAP_BLE_SET_PKT_LENGTH_COMPLETE_EVT,                /*!< When set pkt length complete, the event comes */
    RK_GAP_BLE_SET_LOCAL_PRIVACY_COMPLETE_EVT,             /*!< When  Enable/disable privacy on the local device complete, the event comes */
    RK_GAP_BLE_REMOVE_BOND_DEV_COMPLETE_EVT,               /*!< When remove the bond device complete, the event comes */
    RK_GAP_BLE_CLEAR_BOND_DEV_COMPLETE_EVT,                /*!< When clear the bond device clear complete, the event comes */
    RK_GAP_BLE_GET_BOND_DEV_COMPLETE_EVT,                  /*!< When get the bond device list complete, the event comes */
    RK_GAP_BLE_READ_RSSI_COMPLETE_EVT,                     /*!< When read the rssi complete, the event comes */
    RK_GAP_BLE_UPDATE_WHITELIST_COMPLETE_EVT,              /*!< When add or remove whitelist complete, the event comes */
    RK_GAP_BLE_UPDATE_DUPLICATE_EXCEPTIONAL_LIST_COMPLETE_EVT,  /*!< When update duplicate exceptional list complete, the event comes */
    RK_GAP_BLE_EVT_MAX,
} rk_gap_ble_cb_event_t;
/// This is the old name, just for backwards compatibility
#define RK_GAP_BLE_ADD_WHITELIST_COMPLETE_EVT RK_GAP_BLE_UPDATE_WHITELIST_COMPLETE_EVT

/// Advertising data maximum length
#define RK_BLE_ADV_DATA_LEN_MAX               31
/// Scan response data maximum length
#define RK_BLE_SCAN_RSP_DATA_LEN_MAX          31

/* relate to BTM_BLE_AD_TYPE_xxx in stack/btm_ble_api.h */
/// The type of advertising data(not adv_type)
typedef enum
{
    RK_BLE_AD_TYPE_FLAG                     = 0x01,    /* relate to BTM_BLE_AD_TYPE_FLAG in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_16SRV_PART               = 0x02,    /* relate to BTM_BLE_AD_TYPE_16SRV_PART in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_16SRV_CMPL               = 0x03,    /* relate to BTM_BLE_AD_TYPE_16SRV_CMPL in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_32SRV_PART               = 0x04,    /* relate to BTM_BLE_AD_TYPE_32SRV_PART in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_32SRV_CMPL               = 0x05,    /* relate to BTM_BLE_AD_TYPE_32SRV_CMPL in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_128SRV_PART              = 0x06,    /* relate to BTM_BLE_AD_TYPE_128SRV_PART in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_128SRV_CMPL              = 0x07,    /* relate to BTM_BLE_AD_TYPE_128SRV_CMPL in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_NAME_SHORT               = 0x08,    /* relate to BTM_BLE_AD_TYPE_NAME_SHORT in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_NAME_CMPL                = 0x09,    /* relate to BTM_BLE_AD_TYPE_NAME_CMPL in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_TX_PWR                   = 0x0A,    /* relate to BTM_BLE_AD_TYPE_TX_PWR in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_DEV_CLASS                = 0x0D,    /* relate to BTM_BLE_AD_TYPE_DEV_CLASS in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_SM_TK                    = 0x10,    /* relate to BTM_BLE_AD_TYPE_SM_TK in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_SM_OOB_FLAG              = 0x11,    /* relate to BTM_BLE_AD_TYPE_SM_OOB_FLAG in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_INT_RANGE                = 0x12,    /* relate to BTM_BLE_AD_TYPE_INT_RANGE in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_SOL_SRV_UUID             = 0x14,    /* relate to BTM_BLE_AD_TYPE_SOL_SRV_UUID in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_128SOL_SRV_UUID          = 0x15,    /* relate to BTM_BLE_AD_TYPE_128SOL_SRV_UUID in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_SERVICE_DATA             = 0x16,    /* relate to BTM_BLE_AD_TYPE_SERVICE_DATA in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_PUBLIC_TARGET            = 0x17,    /* relate to BTM_BLE_AD_TYPE_PUBLIC_TARGET in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_RANDOM_TARGET            = 0x18,    /* relate to BTM_BLE_AD_TYPE_RANDOM_TARGET in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_APPEARANCE               = 0x19,    /* relate to BTM_BLE_AD_TYPE_APPEARANCE in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_ADV_INT                  = 0x1A,    /* relate to BTM_BLE_AD_TYPE_ADV_INT in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_LE_DEV_ADDR              = 0x1b,    /* relate to BTM_BLE_AD_TYPE_LE_DEV_ADDR in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_LE_ROLE                  = 0x1c,    /* relate to BTM_BLE_AD_TYPE_LE_ROLE in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_SPAIR_C256               = 0x1d,    /* relate to BTM_BLE_AD_TYPE_SPAIR_C256 in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_SPAIR_R256               = 0x1e,    /* relate to BTM_BLE_AD_TYPE_SPAIR_R256 in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_32SOL_SRV_UUID           = 0x1f,    /* relate to BTM_BLE_AD_TYPE_32SOL_SRV_UUID in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_32SERVICE_DATA           = 0x20,    /* relate to BTM_BLE_AD_TYPE_32SERVICE_DATA in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_128SERVICE_DATA          = 0x21,    /* relate to BTM_BLE_AD_TYPE_128SERVICE_DATA in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_LE_SECURE_CONFIRM        = 0x22,    /* relate to BTM_BLE_AD_TYPE_LE_SECURE_CONFIRM in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_LE_SECURE_RANDOM         = 0x23,    /* relate to BTM_BLE_AD_TYPE_LE_SECURE_RANDOM in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_URI                      = 0x24,    /* relate to BTM_BLE_AD_TYPE_URI in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_INDOOR_POSITION          = 0x25,    /* relate to BTM_BLE_AD_TYPE_INDOOR_POSITION in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_TRANS_DISC_DATA          = 0x26,    /* relate to BTM_BLE_AD_TYPE_TRANS_DISC_DATA in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_LE_SUPPORT_FEATURE       = 0x27,    /* relate to BTM_BLE_AD_TYPE_LE_SUPPORT_FEATURE in stack/btm_ble_api.h */
    RK_BLE_AD_TYPE_CHAN_MAP_UPDATE          = 0x28,    /* relate to BTM_BLE_AD_TYPE_CHAN_MAP_UPDATE in stack/btm_ble_api.h */
    RK_BLE_AD_MANUFACTURER_SPECIFIC_TYPE    = 0xFF,    /* relate to BTM_BLE_AD_MANUFACTURER_SPECIFIC_TYPE in stack/btm_ble_api.h */
} rk_ble_adv_data_type;

/// Advertising mode
typedef enum
{
    ADV_TYPE_IND                = 0x00,
    ADV_TYPE_DIRECT_IND_HIGH    = 0x01,
    ADV_TYPE_SCAN_IND           = 0x02,
    ADV_TYPE_NONCONN_IND        = 0x03,
    ADV_TYPE_DIRECT_IND_LOW     = 0x04,
} rk_ble_adv_type_t;

/// Advertising channel mask
typedef enum
{
    ADV_CHNL_37     = 0x01,
    ADV_CHNL_38     = 0x02,
    ADV_CHNL_39     = 0x04,
    ADV_CHNL_ALL    = 0x07,
} rk_ble_adv_channel_t;

typedef enum
{
    ///Allow both scan and connection requests from anyone
    ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY  = 0x00,
    ///Allow both scan req from White List devices only and connection req from anyone
    ADV_FILTER_ALLOW_SCAN_WLST_CON_ANY,
    ///Allow both scan req from anyone and connection req from White List devices only
    ADV_FILTER_ALLOW_SCAN_ANY_CON_WLST,
    ///Allow scan and connection requests from White List devices only
    ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST,
    ///Enumeration end value for advertising filter policy value check
} rk_ble_adv_filter_t;


/* relate to BTA_DM_BLE_SEC_xxx in bta/bta_api.h */
typedef enum
{
    RK_BLE_SEC_ENCRYPT = 1,            /* relate to BTA_DM_BLE_SEC_ENCRYPT in bta/bta_api.h. If the device has already
                                           bonded, the stack will used LTK to encrypt with the remote device directly.
                                           Else if the device hasn't bonded, the stack will used the default authentication request
                                           used the rk_ble_gap_set_security_param function set by the user. */
    RK_BLE_SEC_ENCRYPT_NO_MITM,        /* relate to BTA_DM_BLE_SEC_ENCRYPT_NO_MITM in bta/bta_api.h. If the device has already
                                           bonded, the stack will check the LTK Whether the authentication request has been met, if met, used the LTK
                                           to encrypt with the remote device directly, else Re-pair with the remote device.
                                           Else if the device hasn't bonded, the stack will used NO MITM authentication request in the current link instead of
                                           used the authreq in the rk_ble_gap_set_security_param function set by the user. */
    RK_BLE_SEC_ENCRYPT_MITM,           /* relate to BTA_DM_BLE_SEC_ENCRYPT_MITM in bta/bta_api.h. If the device has already
                                           bonded, the stack will check the LTK Whether the authentication request has been met, if met, used the LTK
                                           to encrypt with the remote device directly, else Re-pair with the remote device.
                                           Else if the device hasn't bonded, the stack will used MITM authentication request in the current link instead of
                                           used the authreq in the rk_ble_gap_set_security_param function set by the user. */
} rk_ble_sec_act_t;

typedef enum
{
    RK_BLE_SM_PASSKEY = 0,
    RK_BLE_SM_AUTHEN_REQ_MODE,
    RK_BLE_SM_IOCAP_MODE,
    RK_BLE_SM_SET_INIT_KEY,
    RK_BLE_SM_SET_RSP_KEY,
    RK_BLE_SM_MAX_KEY_SIZE,
    RK_BLE_SM_SET_STATIC_PASSKEY,
    RK_BLE_SM_CLEAR_STATIC_PASSKEY,
    RK_BLE_SM_ONLY_ACCEPT_SPECIFIED_SEC_AUTH,
    RK_BLE_SM_OOB_SUPPORT,
    RK_BLE_SM_MAX_PARAM,
} rk_ble_sm_param_t;

/// Advertising parameters
typedef struct
{
    uint16_t                adv_int_min;        /*!< Minimum advertising interval for
                                                  undirected and low duty cycle directed advertising.
                                                  Range: 0x0020 to 0x4000 Default: N = 0x0800 (1.28 second)
                                                  Time = N * 0.625 msec Time Range: 20 ms to 10.24 sec */
    uint16_t                adv_int_max;        /*!< Maximum advertising interval for
                                                  undirected and low duty cycle directed advertising.
                                                  Range: 0x0020 to 0x4000 Default: N = 0x0800 (1.28 second)
                                                  Time = N * 0.625 msec Time Range: 20 ms to 10.24 sec Advertising max interval */
    rk_ble_adv_type_t      adv_type;           /*!< Advertising type */
    rk_ble_addr_type_t     own_addr_type;      /*!< Owner bluetooth device address type */
    rk_bd_addr_t           peer_addr;          /*!< Peer device bluetooth device address */
    rk_ble_addr_type_t     peer_addr_type;     /*!< Peer device bluetooth device address type, only support public address type and random address type */
    rk_ble_adv_channel_t   channel_map;        /*!< Advertising channel map */
    rk_ble_adv_filter_t    adv_filter_policy;  /*!< Advertising filter policy */
} rk_ble_adv_params_t;

/// Advertising data content, according to "Supplement to the Bluetooth Core Specification"
typedef struct
{
    bool                    set_scan_rsp;           /*!< Set this advertising data as scan response or not*/
    bool                    include_name;           /*!< Advertising data include device name or not */
    bool                    include_txpower;        /*!< Advertising data include TX power */
    int                     min_interval;           /*!< Advertising data show slave preferred connection min interval.
                                                    The connection interval in the following manner:
                                                    connIntervalmin = Conn_Interval_Min * 1.25 ms
                                                    Conn_Interval_Min range: 0x0006 to 0x0C80
                                                    Value of 0xFFFF indicates no specific minimum.
                                                    Values not defined above are reserved for future use.*/

    int                     max_interval;           /*!< Advertising data show slave preferred connection max interval.
                                                    The connection interval in the following manner:
                                                    connIntervalmax = Conn_Interval_Max * 1.25 ms
                                                    Conn_Interval_Max range: 0x0006 to 0x0C80
                                                    Conn_Interval_Max shall be equal to or greater than the Conn_Interval_Min.
                                                    Value of 0xFFFF indicates no specific maximum.
                                                    Values not defined above are reserved for future use.*/

    int                     appearance;             /*!< External appearance of device */
    uint16_t                manufacturer_len;       /*!< Manufacturer data length */
    uint8_t                 *p_manufacturer_data;   /*!< Manufacturer data point */
    uint16_t                service_data_len;       /*!< Service data length */
    uint8_t                 *p_service_data;        /*!< Service data point */
    uint16_t                service_uuid_len;       /*!< Service uuid length */
    uint8_t                 *p_service_uuid;        /*!< Service uuid array point */
    uint8_t                 flag;                   /*!< Advertising flag of discovery mode, see BLE_ADV_DATA_FLAG detail */
} rk_ble_adv_data_t;

/// Ble scan type
typedef enum
{
    BLE_SCAN_TYPE_PASSIVE   =   0x0,            /*!< Passive scan */
    BLE_SCAN_TYPE_ACTIVE    =   0x1,            /*!< Active scan */
} rk_ble_scan_type_t;

/// Ble scan filter type
typedef enum
{
    BLE_SCAN_FILTER_ALLOW_ALL           = 0x0,  /*!< Accept all :
                                                  1. advertisement packets except directed advertising packets not addressed to this device (default). */
    BLE_SCAN_FILTER_ALLOW_ONLY_WLST     = 0x1,  /*!< Accept only :
                                                  1. advertisement packets from devices where the advertiser’s address is in the White list.
                                                  2. Directed advertising packets which are not addressed for this device shall be ignored. */
    BLE_SCAN_FILTER_ALLOW_UND_RPA_DIR   = 0x2,  /*!< Accept all :
                                                  1. undirected advertisement packets, and
                                                  2. directed advertising packets where the initiator address is a resolvable private address, and
                                                  3. directed advertising packets addressed to this device. */
    BLE_SCAN_FILTER_ALLOW_WLIST_PRA_DIR = 0x3,  /*!< Accept all :
                                                  1. advertisement packets from devices where the advertiser’s address is in the White list, and
                                                  2. directed advertising packets where the initiator address is a resolvable private address, and
                                                  3. directed advertising packets addressed to this device.*/
} rk_ble_scan_filter_t;

/// Ble scan duplicate type
typedef enum
{
    BLE_SCAN_DUPLICATE_DISABLE           = 0x0,  /*!< the Link Layer should generate advertising reports to the host for each packet received */
    BLE_SCAN_DUPLICATE_ENABLE            = 0x1,  /*!< the Link Layer should filter out duplicate advertising reports to the Host */
    BLE_SCAN_DUPLICATE_MAX               = 0x2,  /*!< 0x02 – 0xFF, Reserved for future use */
} rk_ble_scan_duplicate_t;

/// Ble scan parameters
typedef struct
{
    rk_ble_scan_type_t     scan_type;              /*!< Scan type */
    rk_ble_addr_type_t     own_addr_type;          /*!< Owner address type */
    rk_ble_scan_filter_t   scan_filter_policy;     /*!< Scan filter policy */
    uint16_t                scan_interval;          /*!< Scan interval. This is defined as the time interval from
                                                      when the Controller started its last LE scan until it begins the subsequent LE scan.
                                                      Range: 0x0004 to 0x4000 Default: 0x0010 (10 ms)
                                                      Time = N * 0.625 msec
                                                      Time Range: 2.5 msec to 10.24 seconds*/
    uint16_t                scan_window;            /*!< Scan window. The duration of the LE scan. LE_Scan_Window
                                                      shall be less than or equal to LE_Scan_Interval
                                                      Range: 0x0004 to 0x4000 Default: 0x0010 (10 ms)
                                                      Time = N * 0.625 msec
                                                      Time Range: 2.5 msec to 10240 msec */
    rk_ble_scan_duplicate_t  scan_duplicate;       /*!< The Scan_Duplicates parameter controls whether the Link Layer should filter out
                                                        duplicate advertising reports (BLE_SCAN_DUPLICATE_ENABLE) to the Host, or if the Link Layer should generate
                                                        advertising reports for each packet received */
} rk_ble_scan_params_t;

/// Connection update parameters
typedef struct
{
    rk_bd_addr_t bda;                              /*!< Bluetooth device address */
    uint16_t min_int;                               /*!< Min connection interval */
    uint16_t max_int;                               /*!< Max connection interval */
    uint16_t latency;                               /*!< Slave latency for the connection in number of connection events. Range: 0x0000 to 0x01F3 */
    uint16_t timeout;                               /*!< Supervision timeout for the LE Link. Range: 0x000A to 0x0C80.
                                                      Mandatory Range: 0x000A to 0x0C80 Time = N * 10 msec
                                                      Time Range: 100 msec to 32 seconds */
} rk_ble_conn_update_params_t;

/**
* @brief BLE pkt date length keys
*/
typedef struct
{
    uint16_t rx_len;                   /*!< pkt rx data length value */
    uint16_t tx_len;                   /*!< pkt tx data length value */
} rk_ble_pkt_data_length_params_t;

/**
* @brief BLE encryption keys
*/
typedef struct
{
    rk_bt_octet16_t     ltk;          /*!< The long term key*/
    rk_bt_octet8_t      rand;         /*!< The random number*/
    uint16_t             ediv;         /*!< The ediv value*/
    uint8_t              sec_level;    /*!< The security level of the security link*/
    uint8_t              key_size;     /*!< The key size(7~16) of the security link*/
} rk_ble_penc_keys_t;                 /*!< The key type*/

/**
* @brief  BLE CSRK keys
*/
typedef struct
{
    uint32_t            counter;      /*!< The counter */
    rk_bt_octet16_t    csrk;         /*!< The csrk key */
    uint8_t             sec_level;    /*!< The security level */
} rk_ble_pcsrk_keys_t;               /*!< The pcsrk key type */

/**
* @brief  BLE pid keys
*/
typedef struct
{
    rk_bt_octet16_t          irk;           /*!< The irk value */
    rk_ble_addr_type_t       addr_type;     /*!< The address type */
    rk_bd_addr_t             static_addr;   /*!< The static address */
} rk_ble_pid_keys_t;                        /*!< The pid key type */

/**
* @brief  BLE Encryption reproduction keys
*/
typedef struct
{
    rk_bt_octet16_t  ltk;                  /*!< The long term key */
    uint16_t          div;                  /*!< The div value */
    uint8_t           key_size;             /*!< The key size of the security link */
    uint8_t           sec_level;            /*!< The security level of the security link */
} rk_ble_lenc_keys_t;                      /*!< The  key type */

/**
* @brief  BLE SRK keys
*/
typedef struct
{
    uint32_t          counter;              /*!< The counter value */
    uint16_t          div;                  /*!< The div value */
    uint8_t           sec_level;            /*!< The security level of the security link */
    rk_bt_octet16_t  csrk;                 /*!< The csrk key value */
} rk_ble_lcsrk_keys;                       /*!< The csrk key type */

/**
* @brief  Structure associated with RK_KEY_NOTIF_EVT
*/
typedef struct
{
    rk_bd_addr_t  bd_addr;        /*!< peer address */
    uint32_t       passkey;        /*!< the numeric value for comparison. If just_works, do not show this number to UI */
} rk_ble_sec_key_notif_t;         /*!< BLE key notify type*/

/**
* @brief  Structure of the security request
*/
typedef struct
{
    rk_bd_addr_t  bd_addr;        /*!< peer address */
} rk_ble_sec_req_t;               /*!< BLE security request type*/

/**
* @brief  union type of the security key value
*/
typedef union
{
    rk_ble_penc_keys_t   penc_key;       /*!< received peer encryption key */
    rk_ble_pcsrk_keys_t  pcsrk_key;      /*!< received peer device SRK */
    rk_ble_pid_keys_t    pid_key;        /*!< peer device ID key */
    rk_ble_lenc_keys_t   lenc_key;       /*!< local encryption reproduction keys LTK = = d1(ER,DIV,0)*/
    rk_ble_lcsrk_keys    lcsrk_key;      /*!< local device CSRK = d1(ER,DIV,1)*/
} rk_ble_key_value_t;                    /*!< ble key value type*/

/**
* @brief  struct type of the bond key information value
*/
typedef struct
{
    rk_ble_key_mask_t    key_mask;       /*!< the key mask to indicate witch key is present */
    rk_ble_penc_keys_t   penc_key;       /*!< received peer encryption key */
    rk_ble_pcsrk_keys_t  pcsrk_key;      /*!< received peer device SRK */
    rk_ble_pid_keys_t    pid_key;        /*!< peer device ID key */
} rk_ble_bond_key_info_t;                /*!< ble bond key information value type */

/**
* @brief  struct type of the bond device value
*/
typedef struct
{
    rk_bd_addr_t  bd_addr;               /*!< peer address */
    rk_ble_bond_key_info_t bond_key;     /*!< the bond key information */
} rk_ble_bond_dev_t;                     /*!< the ble bond device type */


/**
* @brief  union type of the security key value
*/
typedef struct
{
    rk_bd_addr_t               bd_addr;        /*!< peer address */
    rk_ble_key_type_t          key_type;       /*!< key type of the security link */
    rk_ble_key_value_t         p_key_value;    /*!< the pointer to the key value */
} rk_ble_key_t;                                /*!< the union to the ble key value type*/

/**
* @brief  structure type of the ble local id keys value
*/
typedef struct
{
    rk_bt_octet16_t       ir;                  /*!< the 16 bits of the ir value */
    rk_bt_octet16_t       irk;                 /*!< the 16 bits of the ir key value */
    rk_bt_octet16_t       dhk;                 /*!< the 16 bits of the dh key value */
} rk_ble_local_id_keys_t;                      /*!< the structure of the ble local id keys value type*/


/**
  * @brief Structure associated with RK_AUTH_CMPL_EVT
  */
typedef struct
{
    rk_bd_addr_t         bd_addr;               /*!< BD address peer device. */
    bool                  key_present;           /*!< Valid link key value in key element */
    rk_link_key          key;                   /*!< Link key associated with peer device. */
    uint8_t               key_type;              /*!< The type of Link Key */
    bool                  success;               /*!< TRUE of authentication succeeded, FALSE if failed. */
    uint8_t               fail_reason;           /*!< The HCI reason/error code for when success=FALSE */
    rk_ble_addr_type_t   addr_type;             /*!< Peer device address type */
    rk_bt_dev_type_t     dev_type;              /*!< Device type */
    rk_ble_auth_req_t    auth_mode;             /*!< authentication mode */
} rk_ble_auth_cmpl_t;                           /*!< The ble authentication complete cb type */

/**
  * @brief union associated with ble security
  */
typedef union
{
    rk_ble_sec_key_notif_t    key_notif;      /*!< passkey notification */
    rk_ble_sec_req_t          ble_req;        /*!< BLE SMP related request */
    rk_ble_key_t              ble_key;        /*!< BLE SMP keys used when pairing */
    rk_ble_local_id_keys_t    ble_id_keys;    /*!< BLE IR event */
    rk_ble_auth_cmpl_t        auth_cmpl;      /*!< Authentication complete indication. */
} rk_ble_sec_t;                               /*!< BLE security type */

/// Sub Event of RK_GAP_BLE_SCAN_RESULT_EVT
typedef enum
{
    RK_GAP_SEARCH_INQ_RES_EVT             = 0,      /*!< Inquiry result for a peer device. */
    RK_GAP_SEARCH_INQ_CMPL_EVT            = 1,      /*!< Inquiry complete. */
    RK_GAP_SEARCH_DISC_RES_EVT            = 2,      /*!< Discovery result for a peer device. */
    RK_GAP_SEARCH_DISC_BLE_RES_EVT        = 3,      /*!< Discovery result for BLE GATT based service on a peer device. */
    RK_GAP_SEARCH_DISC_CMPL_EVT           = 4,      /*!< Discovery complete. */
    RK_GAP_SEARCH_DI_DISC_CMPL_EVT        = 5,      /*!< Discovery complete. */
    RK_GAP_SEARCH_SEARCH_CANCEL_CMPL_EVT  = 6,      /*!< Search cancelled */
    RK_GAP_SEARCH_INQ_DISCARD_NUM_EVT     = 7,      /*!< The number of pkt discarded by flow control */
} rk_gap_search_evt_t;

/**
 * @brief Ble scan result event type, to indicate the
 *        result is scan response or advertising data or other
 */
typedef enum
{
    RK_BLE_EVT_CONN_ADV         = 0x00,        /*!< Connectable undirected advertising (ADV_IND) */
    RK_BLE_EVT_CONN_DIR_ADV     = 0x01,        /*!< Connectable directed advertising (ADV_DIRECT_IND) */
    RK_BLE_EVT_DISC_ADV         = 0x02,        /*!< Scannable undirected advertising (ADV_SCAN_IND) */
    RK_BLE_EVT_NON_CONN_ADV     = 0x03,        /*!< Non connectable undirected advertising (ADV_NONCONN_IND) */
    RK_BLE_EVT_SCAN_RSP         = 0x04,        /*!< Scan Response (SCAN_RSP) */
} rk_ble_evt_type_t;

typedef enum
{
    RK_BLE_WHITELIST_REMOVE     = 0X00,    /*!< remove mac from whitelist */
    RK_BLE_WHITELIST_ADD        = 0X01,    /*!< add address to whitelist */
} rk_ble_wl_opration_t;

typedef enum
{
    RK_BLE_DUPLICATE_EXCEPTIONAL_LIST_ADD      = 0,  /*!< Add device info into duplicate scan exceptional list */
    RK_BLE_DUPLICATE_EXCEPTIONAL_LIST_REMOVE,        /*!< Remove device info from duplicate scan exceptional list */
    RK_BLE_DUPLICATE_EXCEPTIONAL_LIST_CLEAN,         /*!< Clean duplicate scan exceptional list */
} rk_bt_duplicate_exceptional_subcode_type_t;

#define BLE_BIT(n) (1UL<<(n))

typedef enum
{
    RK_BLE_DUPLICATE_SCAN_EXCEPTIONAL_INFO_ADV_ADDR       = 0,  /*!< BLE advertising address , device info will be added into RK_BLE_DUPLICATE_SCAN_EXCEPTIONAL_ADDR_LIST */
    RK_BLE_DUPLICATE_SCAN_EXCEPTIONAL_INFO_MESH_LINK_ID,        /*!< BLE mesh link ID, it is for BLE mesh, device info will be added into RK_BLE_DUPLICATE_SCAN_EXCEPTIONAL_MESH_LINK_ID_LIST */
    RK_BLE_DUPLICATE_SCAN_EXCEPTIONAL_INFO_MESH_BEACON_TYPE,    /*!< BLE mesh beacon AD type, the format is | Len | 0x2B | Beacon Type | Beacon Data | */
    RK_BLE_DUPLICATE_SCAN_EXCEPTIONAL_INFO_MESH_PROV_SRV_ADV,   /*!< BLE mesh provisioning service uuid, the format is | 0x02 | 0x01 | flags | 0x03 | 0x03 | 0x1827 | .... |` */
    RK_BLE_DUPLICATE_SCAN_EXCEPTIONAL_INFO_MESH_PROXY_SRV_ADV,  /*!< BLE mesh adv with proxy service uuid, the format is | 0x02 | 0x01 | flags | 0x03 | 0x03 | 0x1828 | .... |` */
} rk_ble_duplicate_exceptional_info_type_t;

typedef enum
{
    RK_BLE_DUPLICATE_SCAN_EXCEPTIONAL_ADDR_LIST                  = BLE_BIT(0),             /*!< duplicate scan exceptional addr list */
    RK_BLE_DUPLICATE_SCAN_EXCEPTIONAL_MESH_LINK_ID_LIST          = BLE_BIT(1),             /*!< duplicate scan exceptional mesh link ID list */
    RK_BLE_DUPLICATE_SCAN_EXCEPTIONAL_MESH_BEACON_TYPE_LIST      = BLE_BIT(2),             /*!< duplicate scan exceptional mesh beacon type list */
    RK_BLE_DUPLICATE_SCAN_EXCEPTIONAL_MESH_PROV_SRV_ADV_LIST     = BLE_BIT(3),             /*!< duplicate scan exceptional mesh adv with provisioning service uuid */
    RK_BLE_DUPLICATE_SCAN_EXCEPTIONAL_MESH_PROXY_SRV_ADV_LIST    = BLE_BIT(4),             /*!< duplicate scan exceptional mesh adv with provisioning service uuid */
    RK_BLE_DUPLICATE_SCAN_EXCEPTIONAL_ALL_LIST                   = 0xFFFF,                 /*!< duplicate scan exceptional all list */
} rk_duplicate_scan_exceptional_list_type_t;

typedef uint8_t rk_duplicate_info_t[RK_BD_ADDR_LEN];

/**
 * @brief Gap callback parameters union
 */
typedef union
{
    /**
     * @brief RK_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT
     */
    struct ble_adv_data_cmpl_evt_param
    {
        rk_bt_status_t status;                     /*!< Indicate the set advertising data operation success status */
    } adv_data_cmpl;                                /*!< Event parameter of RK_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT */
    /**
     * @brief RK_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT
     */
    struct ble_scan_rsp_data_cmpl_evt_param
    {
        rk_bt_status_t status;                     /*!< Indicate the set scan response data operation success status */
    } scan_rsp_data_cmpl;                           /*!< Event parameter of RK_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT */
    /**
     * @brief RK_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT
     */
    struct ble_scan_param_cmpl_evt_param
    {
        rk_bt_status_t status;                     /*!< Indicate the set scan param operation success status */
    } scan_param_cmpl;                              /*!< Event parameter of RK_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT */
    /**
     * @brief RK_GAP_BLE_SCAN_RESULT_EVT
     */
    struct ble_scan_result_evt_param
    {
        rk_gap_search_evt_t search_evt;            /*!< Search event type */
        rk_bd_addr_t bda;                          /*!< Bluetooth device address which has been searched */
        rk_bt_dev_type_t dev_type;                 /*!< Device type */
        rk_ble_addr_type_t ble_addr_type;          /*!< Ble device address type */
        rk_ble_evt_type_t ble_evt_type;            /*!< Ble scan result event type */
        int rssi;                                   /*!< Searched device's RSSI */
        uint8_t  ble_adv[RK_BLE_ADV_DATA_LEN_MAX + RK_BLE_SCAN_RSP_DATA_LEN_MAX];     /*!< Received EIR */
        int flag;                                   /*!< Advertising data flag bit */
        int num_resps;                              /*!< Scan result number */
        uint8_t adv_data_len;                       /*!< Adv data length */
        uint8_t scan_rsp_len;                       /*!< Scan response length */
        uint32_t num_dis;                          /*!< The number of discard packets */
    } scan_rst;                                     /*!< Event parameter of RK_GAP_BLE_SCAN_RESULT_EVT */
    /**
     * @brief RK_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT
     */
    struct ble_adv_data_raw_cmpl_evt_param
    {
        rk_bt_status_t status;                     /*!< Indicate the set raw advertising data operation success status */
    } adv_data_raw_cmpl;                            /*!< Event parameter of RK_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT */
    /**
     * @brief RK_GAP_BLE_SCAN_RSP_DATA_RAW_SET_COMPLETE_EVT
     */
    struct ble_scan_rsp_data_raw_cmpl_evt_param
    {
        rk_bt_status_t status;                     /*!< Indicate the set raw advertising data operation success status */
    } scan_rsp_data_raw_cmpl;                       /*!< Event parameter of RK_GAP_BLE_SCAN_RSP_DATA_RAW_SET_COMPLETE_EVT */
    /**
     * @brief RK_GAP_BLE_ADV_START_COMPLETE_EVT
     */
    struct ble_adv_start_cmpl_evt_param
    {
        rk_bt_status_t status;                     /*!< Indicate advertising start operation success status */
    } adv_start_cmpl;                               /*!< Event parameter of RK_GAP_BLE_ADV_START_COMPLETE_EVT */
    /**
     * @brief RK_GAP_BLE_SCAN_START_COMPLETE_EVT
     */
    struct ble_scan_start_cmpl_evt_param
    {
        rk_bt_status_t status;                     /*!< Indicate scan start operation success status */
    } scan_start_cmpl;                              /*!< Event parameter of RK_GAP_BLE_SCAN_START_COMPLETE_EVT */

    rk_ble_sec_t ble_security;                     /*!< ble gap security union type */
    /**
     * @brief RK_GAP_BLE_SCAN_STOP_COMPLETE_EVT
     */
    struct ble_scan_stop_cmpl_evt_param
    {
        rk_bt_status_t status;                     /*!< Indicate scan stop operation success status */
    } scan_stop_cmpl;                               /*!< Event parameter of RK_GAP_BLE_SCAN_STOP_COMPLETE_EVT */
    /**
     * @brief RK_GAP_BLE_ADV_STOP_COMPLETE_EVT
     */
    struct ble_adv_stop_cmpl_evt_param
    {
        rk_bt_status_t status;                     /*!< Indicate adv stop operation success status */
    } adv_stop_cmpl;                                /*!< Event parameter of RK_GAP_BLE_ADV_STOP_COMPLETE_EVT */
    /**
     * @brief RK_GAP_BLE_SET_STATIC_RAND_ADDR_EVT
     */
    struct ble_set_rand_cmpl_evt_param
    {
        rk_bt_status_t status;                     /*!< Indicate set static rand address operation success status */
    } set_rand_addr_cmpl;                           /*!< Event parameter of RK_GAP_BLE_SET_STATIC_RAND_ADDR_EVT */
    /**
     * @brief RK_GAP_BLE_UPDATE_CONN_PARAMS_EVT
     */
    struct ble_update_conn_params_evt_param
    {
        rk_bt_status_t status;                    /*!< Indicate update connection parameters success status */
        rk_bd_addr_t bda;                         /*!< Bluetooth device address */
        uint16_t min_int;                          /*!< Min connection interval */
        uint16_t max_int;                          /*!< Max connection interval */
        uint16_t latency;                          /*!< Slave latency for the connection in number of connection events. Range: 0x0000 to 0x01F3 */
        uint16_t conn_int;                         /*!< Current connection interval */
        uint16_t timeout;                          /*!< Supervision timeout for the LE Link. Range: 0x000A to 0x0C80.
                                                     Mandatory Range: 0x000A to 0x0C80 Time = N * 10 msec */
    } update_conn_params;                          /*!< Event parameter of RK_GAP_BLE_UPDATE_CONN_PARAMS_EVT */
    /**
     * @brief RK_GAP_BLE_SET_PKT_LENGTH_COMPLETE_EVT
     */
    struct ble_pkt_data_length_cmpl_evt_param
    {
        rk_bt_status_t status;                     /*!< Indicate the set pkt data length operation success status */
        rk_ble_pkt_data_length_params_t params;    /*!<  pkt data length value */
    } pkt_data_lenth_cmpl;                          /*!< Event parameter of RK_GAP_BLE_SET_PKT_LENGTH_COMPLETE_EVT */
    /**
     * @brief RK_GAP_BLE_SET_LOCAL_PRIVACY_COMPLETE_EVT
     */
    struct ble_local_privacy_cmpl_evt_param
    {
        rk_bt_status_t status;                     /*!< Indicate the set local privacy operation success status */
    } local_privacy_cmpl;                           /*!< Event parameter of RK_GAP_BLE_SET_LOCAL_PRIVACY_COMPLETE_EVT */
    /**
     * @brief RK_GAP_BLE_REMOVE_BOND_DEV_COMPLETE_EVT
     */
    struct ble_remove_bond_dev_cmpl_evt_param
    {
        rk_bt_status_t status;                     /*!< Indicate the remove bond device operation success status */
        rk_bd_addr_t bd_addr;                      /*!< The device address which has been remove from the bond list */
    } remove_bond_dev_cmpl;                         /*!< Event parameter of RK_GAP_BLE_REMOVE_BOND_DEV_COMPLETE_EVT */
    /**
     * @brief RK_GAP_BLE_CLEAR_BOND_DEV_COMPLETE_EVT
     */
    struct ble_clear_bond_dev_cmpl_evt_param
    {
        rk_bt_status_t status;                     /*!< Indicate the clear bond device operation success status */
    } clear_bond_dev_cmpl;                          /*!< Event parameter of RK_GAP_BLE_CLEAR_BOND_DEV_COMPLETE_EVT */
    /**
     * @brief RK_GAP_BLE_GET_BOND_DEV_COMPLETE_EVT
     */
    struct ble_get_bond_dev_cmpl_evt_param
    {
        rk_bt_status_t status;                     /*!< Indicate the get bond device operation success status */
        uint8_t dev_num;                            /*!< Indicate the get number device in the bond list */
        rk_ble_bond_dev_t *bond_dev;               /*!< the pointer to the bond device Structure */
    } get_bond_dev_cmpl;                            /*!< Event parameter of RK_GAP_BLE_GET_BOND_DEV_COMPLETE_EVT */
    /**
     * @brief RK_GAP_BLE_READ_RSSI_COMPLETE_EVT
     */
    struct ble_read_rssi_cmpl_evt_param
    {
        rk_bt_status_t status;                     /*!< Indicate the read adv tx power operation success status */
        int8_t rssi;                                /*!< The ble remote device rssi value, the range is from -127 to 20, the unit is dbm,
                                                         if the RSSI cannot be read, the RSSI metric shall be set to 127. */
        rk_bd_addr_t remote_addr;                  /*!< The remote device address */
    } read_rssi_cmpl;                               /*!< Event parameter of RK_GAP_BLE_READ_RSSI_COMPLETE_EVT */
    /**
     * @brief RK_GAP_BLE_UPDATE_WHITELIST_COMPLETE_EVT
     */
    struct ble_update_whitelist_cmpl_evt_param
    {
        rk_bt_status_t status;                     /*!< Indicate the add or remove whitelist operation success status */
        rk_ble_wl_opration_t wl_opration;          /*!< The value is RK_BLE_WHITELIST_ADD if add address to whitelist operation success, RK_BLE_WHITELIST_REMOVE if remove address from the whitelist operation success */
    } update_whitelist_cmpl;                        /*!< Event parameter of RK_GAP_BLE_UPDATE_WHITELIST_COMPLETE_EVT */
    /**
     * @brief RK_GAP_BLE_UPDATE_DUPLICATE_EXCEPTIONAL_LIST_COMPLETE_EVT
     */
    struct ble_update_duplicate_exceptional_list_cmpl_evt_param
    {
        rk_bt_status_t status;                     /*!< Indicate update duplicate scan exceptional list operation success status */
        uint8_t         subcode;                    /*!< Define in rk_bt_duplicate_exceptional_subcode_type_t */
        uint16_t         length;                     /*!< The length of device_info */
        rk_duplicate_info_t device_info;           /*!< device information, when subcode is RK_BLE_DUPLICATE_EXCEPTIONAL_LIST_CLEAN, the value is invalid */
    } update_duplicate_exceptional_list_cmpl;       /*!< Event parameter of RK_GAP_BLE_UPDATE_DUPLICATE_EXCEPTIONAL_LIST_COMPLETE_EVT */
} rk_ble_gap_cb_param_t;

/**
 * @brief GAP callback function type
 * @param event : Event type
 * @param param : Point to callback parameter, currently is union type
 */
typedef void (* rk_gap_ble_cb_t)(rk_gap_ble_cb_event_t event, rk_ble_gap_cb_param_t *param);

/**
 * @brief           This function is called to occur gap event, such as scan result
 *
 * @param[in]       callback: callback function
 *
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 *
 */
osi_err_t rk_ble_gap_register_callback(rk_gap_ble_cb_t callback);


/**
 * @brief           This function is called to override the BTA default ADV parameters.
 *
 * @param[in]       adv_data: Pointer to User defined ADV data structure. This
 *                  memory space can not be freed until callback of config_adv_data
 *                  is received.
 *
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 *
 */
osi_err_t rk_ble_gap_config_adv_data(rk_ble_adv_data_t *adv_data);



/**
 * @brief           This function is called to set scan parameters
 *
 * @param[in]       scan_params: Pointer to User defined scan_params data structure. This
 *                  memory space can not be freed until callback of set_scan_params
 *
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 *
 */
osi_err_t rk_ble_gap_set_scan_params(rk_ble_scan_params_t *scan_params);


/**
 * @brief           This procedure keep the device scanning the peer device which advertising on the air
 *
 * @param[in]       duration: Keeping the scanning time, the unit is second.
 *
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 *
 */
osi_err_t rk_ble_gap_start_scanning(uint32_t duration);


/**
 * @brief          This function call to stop the device scanning the peer device which advertising on the air
 * @return
 *                 - RK_OK : success
 *                  - other  : failed
 *
 */
osi_err_t rk_ble_gap_stop_scanning(void);

/**
 * @brief           This function is called to start advertising.
 *
 * @param[in]       adv_params: pointer to User defined adv_params data structure.

 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 *
 */
osi_err_t rk_ble_gap_start_advertising(rk_ble_adv_params_t *adv_params);



/**
 * @brief           This function is called to stop advertising.
 *
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 *
 */
osi_err_t rk_ble_gap_stop_advertising(void);



/**
 * @brief           Update connection parameters, can only be used when connection is up.
 *
 * @param[in]       params   -  connection update parameters
 *
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 *
 */
osi_err_t rk_ble_gap_update_conn_params(rk_ble_conn_update_params_t *params);


/**
 * @brief           This function is to set maximum LE data packet size
 *
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 *
 */
osi_err_t rk_ble_gap_set_pkt_data_len(rk_bd_addr_t remote_device, uint16_t tx_data_length);

/**
 * @brief           This function sets the random address for the application
 *
 * @param[in]       rand_addr: the random address which should be setting
 *
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 *
 */
osi_err_t rk_ble_gap_set_rand_addr(rk_bd_addr_t rand_addr);

/**
 * @brief           This function clears the random address for the application
 *
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 *
 */
osi_err_t rk_ble_gap_clear_rand_addr(void);



/**
 * @brief           Enable/disable privacy on the local device
 *
 * @param[in]       privacy_enable   - enable/disable privacy on remote device.
 *
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 *
 */
osi_err_t rk_ble_gap_config_local_privacy(bool privacy_enable);

/**
 * @brief           set local gap appearance icon
 *
 *
 * @param[in]       icon   - External appearance value, these values are defined by the Bluetooth SIG, please refer to
 *                  https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.gap.appearance.xml
 *
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 *
 */
osi_err_t rk_ble_gap_config_local_icon(uint16_t icon);

/**
* @brief            Add or remove device from white list
*
* @param[in]        add_remove: the value is true if added the ble device to the white list, and false remove to the white list.
* @param[in]        remote_bda: the remote device address add/remove from the white list.
* @param[in]        wl_addr_type: whitelist address type
* @return
*                     - RK_OK : success
*                     - other  : failed
*
*/
osi_err_t rk_ble_gap_update_whitelist(bool add_remove, rk_bd_addr_t remote_bda, rk_ble_wl_addr_type_t wl_addr_type);

/**
* @brief            Get the whitelist size in the controller
*
* @param[out]       length: the white list length.
* @return
*                     - RK_OK : success
*                     - other  : failed
*
*/
osi_err_t rk_ble_gap_get_whitelist_size(uint16_t *length);

/**
* @brief            This function is called to set the preferred connection
*                   parameters when default connection parameter is not desired before connecting.
*                   This API can only be used in the master role.
*
* @param[in]        bd_addr: BD address of the peripheral
* @param[in]        min_conn_int: minimum preferred connection interval
* @param[in]        max_conn_int: maximum preferred connection interval
* @param[in]        slave_latency: preferred slave latency
* @param[in]        supervision_tout: preferred supervision timeout
*
* @return
*                   - RK_OK : success
*                   - other  : failed
*
*/
osi_err_t rk_ble_gap_set_prefer_conn_params(rk_bd_addr_t bd_addr,
        uint16_t min_conn_int, uint16_t max_conn_int,
        uint16_t slave_latency, uint16_t supervision_tout);

/**
 * @brief           Set device name to the local device
 *
 * @param[in]       name   -  device name.
 *
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 *
 */
osi_err_t rk_ble_gap_set_device_name(const char *name);

/**
 * @brief          This function is called to get local used address and adress type.
 *                 uint8_t *rk_bt_dev_get_address(void) get the public address
 *
 * @param[in]       local_used_addr - current local used ble address (six bytes)
 * @param[in]       addr_type   - ble address type
 *
 * @return          - RK_OK : success
 *                  - other  : failed
 *
 */
osi_err_t rk_ble_gap_get_local_used_addr(rk_bd_addr_t local_used_addr, uint8_t *addr_type);
/**
 * @brief          This function is called to get ADV data for a specific type.
 *
 * @param[in]       adv_data - pointer of ADV data which to be resolved
 * @param[in]       type   - finding ADV data type
 * @param[out]      length - return the length of ADV data not including type
 *
 * @return          pointer of ADV data
 *
 */
uint8_t *rk_ble_resolve_adv_data(uint8_t *adv_data, uint8_t type, uint8_t *length);

/**
 * @brief           This function is called to set raw advertising data. User need to fill
 *                  ADV data by self.
 *
 * @param[in]       raw_data : raw advertising data
 * @param[in]       raw_data_len : raw advertising data length , less than 31 bytes
 *
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 *
 */
osi_err_t rk_ble_gap_config_adv_data_raw(uint8_t *raw_data, uint32_t raw_data_len);

/**
 * @brief           This function is called to set raw scan response data. User need to fill
 *                  scan response data by self.
 *
 * @param[in]       raw_data : raw scan response data
 * @param[in]       raw_data_len : raw scan response data length , less than 31 bytes
 *
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 */
osi_err_t rk_ble_gap_config_scan_rsp_data_raw(uint8_t *raw_data, uint32_t raw_data_len);

/**
 * @brief           This function is called to read the RSSI of remote device.
 *                  The address of link policy results are returned in the gap callback function with
 *                  RK_GAP_BLE_READ_RSSI_COMPLETE_EVT event.
 *
 * @param[in]       remote_addr : The remote connection device address.
 *
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 */
osi_err_t rk_ble_gap_read_rssi(rk_bd_addr_t remote_addr);

/**
 * @brief           This function is called to add a device info into the duplicate scan exceptional list.
 *
 *
 * @param[in]       type: device info type, it is defined in rk_ble_duplicate_exceptional_info_type_t
 *                  when type is MESH_BEACON_TYPE, MESH_PROV_SRV_ADV or MESH_PROXY_SRV_ADV , device_info is invalid.
 * @param[in]       device_info: the device information.
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 */
osi_err_t rk_ble_gap_add_duplicate_scan_exceptional_device(rk_ble_duplicate_exceptional_info_type_t type, rk_duplicate_info_t device_info);

/**
 * @brief           This function is called to remove a device info from the duplicate scan exceptional list.
 *
 *
 * @param[in]       type: device info type, it is defined in rk_ble_duplicate_exceptional_info_type_t
 *                  when type is MESH_BEACON_TYPE, MESH_PROV_SRV_ADV or MESH_PROXY_SRV_ADV , device_info is invalid.
 * @param[in]       device_info: the device information.
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 */
osi_err_t rk_ble_gap_remove_duplicate_scan_exceptional_device(rk_ble_duplicate_exceptional_info_type_t type, rk_duplicate_info_t device_info);

/**
 * @brief           This function is called to clean the duplicate scan exceptional list.
 *                  This API will delete all device information in the duplicate scan exceptional list.
 *
 *
 * @param[in]       list_type: duplicate scan exceptional list type, the value can be one or more of rk_duplicate_scan_exceptional_list_type_t.
 *
 * @return
 *                  - RK_OK : success
 *                  - other  : failed
 */
osi_err_t rk_ble_gap_clean_duplicate_scan_exceptional_list(rk_duplicate_scan_exceptional_list_type_t list_type);

#if (SMP_INCLUDED == TRUE)
/**
* @brief             Set a GAP security parameter value. Overrides the default value.
*
* @param[in]       param_type : the type of the param which to be set
* @param[in]       value  : the param value
* @param[in]       len : the length of the param value
*
* @return            - RK_OK : success
*                       - other  : failed
*
*/
osi_err_t rk_ble_gap_set_security_param(rk_ble_sm_param_t param_type,
                                        void *value, uint8_t len);

/**
* @brief             Grant security request access.
*
* @param[in]       bd_addr : BD address of the peer
* @param[in]       accept  :  accept the security request or not
*
* @return            - RK_OK : success
*                    - other  : failed
*
*/
osi_err_t rk_ble_gap_security_rsp(rk_bd_addr_t bd_addr,  bool accept);


/**
* @brief             Set a gap parameter value. Use this function to change
*                    the default GAP parameter values.
*
* @param[in]       bd_addr : the address of the peer device need to encryption
* @param[in]       sec_act  : This is the security action to indicate
*                                   what kind of BLE security level is required for
*                                   the BLE link if the BLE is supported
*
* @return            - RK_OK : success
*                       - other  : failed
*
*/
osi_err_t rk_ble_set_encryption(rk_bd_addr_t bd_addr, rk_ble_sec_act_t sec_act);

/**
* @brief          Reply the key value to the peer device in the legacy connection stage.
*
* @param[in]      bd_addr : BD address of the peer
* @param[in]      accept : passkey entry successful or declined.
* @param[in]      passkey : passkey value, must be a 6 digit number,
*                                     can be lead by 0.
*
* @return            - RK_OK : success
*                  - other  : failed
*
*/
osi_err_t rk_ble_passkey_reply(rk_bd_addr_t bd_addr, bool accept, uint32_t passkey);


/**
* @brief           Reply the confirm value to the peer device in the secure connection stage.
*
* @param[in]       bd_addr : BD address of the peer device
* @param[in]       accept : numbers to compare are the same or different.
*
* @return            - RK_OK : success
*                       - other  : failed
*
*/
osi_err_t rk_ble_confirm_reply(rk_bd_addr_t bd_addr, bool accept);

/**
* @brief           Removes a device from the security database list of
*                  peer device. It manages unpairing event while connected.
*
* @param[in]       bd_addr : BD address of the peer device
*
* @return            - RK_OK : success
*                       - other  : failed
*
*/
osi_err_t rk_ble_remove_bond_device(rk_bd_addr_t bd_addr);

/**
* @brief           Get the device number from the security database list of peer device.
*                  It will return the device bonded number immediately.
*
* @return          - >= 0 : bonded devices number.
*                  - RK_FAIL  : failed
*
*/
int rk_ble_get_bond_device_num(void);


/**
* @brief           Get the device from the security database list of peer device.
*                  It will return the device bonded information immediately.
* @param[inout]    dev_num: Indicate the dev_list array(buffer) size as input.
*                           If dev_num is large enough, it means the actual number as output.
*                           Suggest that dev_num value equal to rk_ble_get_bond_device_num().
*
* @param[out]      dev_list: an array(buffer) of `rk_ble_bond_dev_t` type. Use for storing the bonded devices address.
*                            The dev_list should be allocated by who call this API.
* @return          - RK_OK : success
*                  - other  : failed
*
*/
osi_err_t rk_ble_get_bond_device_list(int *dev_num, rk_ble_bond_dev_t *dev_list);

/**
* @brief           This function is called to provide the OOB data for
*                  SMP in response to RK_GAP_BLE_OOB_REQ_EVT
*
* @param[in]       bd_addr: BD address of the peer device.
* @param[in]       TK: TK value, the TK value shall be a 128-bit random number
* @param[in]       len: length of tk, should always be 128-bit
*
* @return          - RK_OK : success
*                  - other  : failed
*
*/
osi_err_t rk_ble_oob_req_reply(rk_bd_addr_t bd_addr, uint8_t *TK, uint8_t len);

#endif /* #if (SMP_INCLUDED == TRUE) */

/**
* @brief           This function is to disconnect the physical connection of the peer device
*                  gattc may have multiple virtual GATT server connections when multiple app_id registered.
*                  rk_ble_gattc_close (rk_gatt_if_t gattc_if, uint16_t conn_id) only close one virtual GATT server connection.
*                  if there exist other virtual GATT server connections, it does not disconnect the physical connection.
*                  rk_ble_gap_disconnect(rk_bd_addr_t remote_device) disconnect the physical connection directly.
*
*
*
* @param[in]       remote_device : BD address of the peer device
*
* @return            - RK_OK : success
*                    - other  : failed
*
*/
osi_err_t rk_ble_gap_disconnect(rk_bd_addr_t remote_device);

#ifdef __cplusplus
}
#endif

#endif /* __RK_GAP_BLE_API_H__ */
