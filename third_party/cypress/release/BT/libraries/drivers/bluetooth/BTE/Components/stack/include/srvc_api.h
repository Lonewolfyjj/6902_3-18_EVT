/****************************************************************************
**
**  Name:       srvc_api.h
**  Function    this file contains the definitions for the DIS API
**
**
**  Copyright (c) 1999-2011, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
**
******************************************************************************/
#ifndef SRVC_DIS_API_H
#define SRVC_DIS_API_H

#include "bt_target.h"
#include "gatt_api.h"
#include "gattdefs.h"

#define DIS_SUCCESS             GATT_SUCCESS
#define DIS_ILLEGAL_PARAM       GATT_ILLEGAL_PARAMETER
#define DIS_NO_RESOURCES        GATT_NO_RESOURCES
typedef UINT8 tDIS_STATUS;


/*****************************************************************************
**  Data structure for DIS
*****************************************************************************/

#define DIS_ATTR_SYS_ID_BIT         0x0001
#define DIS_ATTR_MODEL_NUM_BIT      0x0002
#define DIS_ATTR_SERIAL_NUM_BIT     0x0004
#define DIS_ATTR_FW_NUM_BIT         0x0008
#define DIS_ATTR_HW_NUM_BIT         0x0010
#define DIS_ATTR_SW_NUM_BIT         0x0020
#define DIS_ATTR_MANU_NAME_BIT      0x0040
#define DIS_ATTR_IEEE_DATA_BIT      0x0080
#define DIS_ATTR_PNP_ID_BIT         0x0100
typedef UINT16  tDIS_ATTR_MASK;

#define DIS_ATTR_ALL_MASK           0xffff

typedef tDIS_ATTR_MASK tDIS_ATTR_BIT ;

typedef struct
{
    UINT16      len;
    UINT8       *p_data;
} tDIS_STRING;

typedef struct
{
    UINT16       vendor_id;
    UINT16       product_id;
    UINT16       product_version;
    UINT8        vendor_id_src;

} tDIS_PNP_ID;

typedef union
{
    UINT64              system_id;
    tDIS_PNP_ID         pnp_id;
    tDIS_STRING         data_str;
} tDIS_ATTR;

#define DIS_MAX_STRING_DATA     7

typedef struct
{
    UINT16                  attr_mask;
    UINT64                  system_id;
    tDIS_PNP_ID             pnp_id;
    UINT8                   *data_string[DIS_MAX_STRING_DATA];
} tDIS_VALUE;


typedef void (tDIS_READ_CBACK)(BD_ADDR addr, tDIS_VALUE *p_dis_value);

/*****************************************************************************
**  Data structure used by Battery Service
*****************************************************************************/
typedef struct
{
    BD_ADDR remote_bda;
    BOOLEAN need_rsp;
    UINT16  clt_cfg;
} tBA_WRITE_DATA;

#define BA_READ_CLT_CFG_REQ     1
#define BA_READ_PRE_FMT_REQ     2
#define BA_READ_RPT_REF_REQ     3
#define BA_READ_LEVEL_REQ       4
#define BA_WRITE_CLT_CFG_REQ    5

typedef void (tBA_CBACK)(UINT8 app_id, UINT8 event, tBA_WRITE_DATA *p_data);

#define BA_LEVEL_NOTIFY         0x01
#define BA_LEVEL_PRE_FMT        0x02
#define BA_LEVEL_RPT_REF        0x04
typedef UINT8   tBA_LEVEL_DESCR;

typedef struct
{
    BOOLEAN             is_pri;
    tBA_LEVEL_DESCR     ba_level_descr;
    tGATT_TRANSPORT     transport;
    tBA_CBACK       *p_cback;
} tBA_REG_INFO;

typedef union
{
    UINT8       ba_level;
    UINT16      clt_cfg;
    tGATT_CHAR_RPT_REF  rpt_ref;
    tGATT_CHAR_PRES     pres_fmt;
} tBA_RSP_DATA;

/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif
/*****************************************************************************
**  Service Engine API
*****************************************************************************/
/*******************************************************************************
**
** Function         srvc_eng_init
**
** Description      Initializa the GATT Service engine, register a GATT application
**                  as for a central service management.
**
*******************************************************************************/
GATT_API extern tGATT_STATUS srvc_eng_init(void);

/*******************************************************************************
**
** Function         srvc_eng_deinit
**
** Description      DeInitialize the GATT Service engine.
**
*******************************************************************************/
GATT_API extern void srvc_eng_deinit(void);

/*****************************************************************************
**  DIS Server Function
*****************************************************************************/

/*******************************************************************************
**
** Function         DIS_SrInit
**
** Description      Initializa the Device Information Service Server.
**
*******************************************************************************/
GATT_API extern tDIS_STATUS DIS_SrInit(tDIS_ATTR_MASK dis_attr_mask);
/*******************************************************************************
**
** Function         DIS_SrUpdate
**
** Description      Update the DIS server attribute values
**
*******************************************************************************/
GATT_API extern tDIS_STATUS DIS_SrUpdate(tDIS_ATTR_BIT dis_attr_bit, tDIS_ATTR *p_info);
/*****************************************************************************
**  DIS Client Function
*****************************************************************************/
/*******************************************************************************
**
** Function         DIS_ReadDISInfo
**
** Description      Read remote device DIS information.
**
** Returns          void
**
*******************************************************************************/
GATT_API extern BOOLEAN DIS_ReadDISInfo(BD_ADDR peer_bda, tDIS_READ_CBACK *p_cback);

/*******************************************************************************
**      BATTERY SERVICE API
*******************************************************************************/
/*******************************************************************************
**
** Function         Battery_Instantiate
**
** Description      Instantiate a Battery service
**
*******************************************************************************/
GATT_API extern UINT16 Battery_Instantiate(UINT8 app_id, tBA_REG_INFO *p_reg_info);

/*******************************************************************************
**
** Function         Battery_Rsp
**
** Description      Respond to a battery service request
**
*******************************************************************************/
GATT_API extern void Battery_Rsp(UINT8 app_id, tGATT_STATUS st, UINT8 event, tBA_RSP_DATA *p_rsp);
/*******************************************************************************
**
** Function         Battery_Notify
**
** Description      Send battery level notification
**
*******************************************************************************/
GATT_API extern void Battery_Notify(UINT8 app_id, BD_ADDR remote_bda, UINT8 battery_level);


#ifdef __cplusplus

}
#endif

#endif


