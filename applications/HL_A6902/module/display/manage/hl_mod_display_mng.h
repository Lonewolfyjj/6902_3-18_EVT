/**
 * @file hl_mod_display_mng.h
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-11-30
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
 * <tr><td>2022-11-30     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PAGEMANAGER_H
#define __PAGEMANAGER_H
/* Includes ------------------------------------------------------------------*/
#include "rtdef.h"
#include "hl_config.h"
#if !HL_IS_TX_DEVICE() 
#include "stdbool.h"


#ifdef __cplusplus
extern "C" {
#endif


// 注册页面的通用操作的宏定义
// #define PAGE_REG(name)\
// do{\
//     hl_mod_##name();\
// }while(0)

// #define PAGE_DEC(name)   void hl_mod_##name##_init(void)


typedef void(*CallbackFunction_t)(void);

typedef void(*EventFunction_t)(void*,int);


// 页面操作回调函数初始化
typedef struct {
    CallbackFunction_t SetupCallback;
    CallbackFunction_t LoopCallback;
    CallbackFunction_t ExitCallback;
    EventFunction_t EventCallback;
} PageList_TypeDef;

//页面管理函数初始化
void PageManager_Init(uint8_t pageMax, uint8_t pageStackSize);


void PageManager_Deinit();

bool PageManager_PageRegister(\
    uint8_t pageID,\
    CallbackFunction_t setupCallback,\
    CallbackFunction_t loopCallback,\
    CallbackFunction_t exitCallback,\
    EventFunction_t eventCallback\
);

bool PageManager_PageClear(uint8_t pageID);

bool PageManager_PagePush(uint8_t pageID);

bool PageManager_PagePop();

void PageManager_PageChangeTo(uint8_t pageID);

void PageManager_PageEventTransmit(void* obj, int event);

void PageManager_PageStackClear();

void PageManager_Running();

#ifdef __cplusplus
} /* extern "C" */
#endif




/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#endif 
#endif
/*
 * EOF
 */