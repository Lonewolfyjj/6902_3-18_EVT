/**
 * @file hl_mod_display_mng.c
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
#include "hl_mod_display_mng.h"
#if (!HL_IS_TX_DEVICE()) 

/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#define DBG_SECTION_NAME "disp/page"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

#define IS_PAGE(page)   ((page)<(MaxPage))

#ifndef PAGE_NULL
#   define PAGE_NULL NULL
#endif

#ifndef PAGE_MALLOC
#define PAGE_MALLOC     rt_malloc
#endif

#ifndef PAGE_FREE       
#define PAGE_FREE       rt_free
#endif

/* 页面栈 */
typedef struct{
    uint8_t *Ptr;         // 指向栈地址
    uint8_t Size;               // 栈的大小
    uint8_t Top;                //Top指针
} PageStack_TypeDef; 

static PageStack_TypeDef PageStack;     // 页面栈


static PageList_TypeDef* PageList;      // 页面指针地址

static uint8_t MaxPage;             // 最大支持的页面数

static bool IsPageBusy;

static uint8_t NowPage, LastPage, NextPage;
static uint8_t NewPage, OldPage;

/**
  * @brief  初始化页面调度器
  * @param  pageMax: 页面最大数量
  * @param  eventMax: 事件最大数量
  * @retval 无
  */
void PageManager_Init(uint8_t pageMax, uint8_t pageStackSize)
{
    MaxPage = pageMax;
    NewPage = PAGE_NULL;
    OldPage = PAGE_NULL;
    IsPageBusy = false;

    /* 申请内存，清空列表 */
    PageList = PAGE_MALLOC(sizeof(PageList_TypeDef) * MaxPage);
    for(uint8_t page = 0; page < MaxPage; page++)
    {
        PageManager_PageClear(page);
    }
    
    /*页面栈配置*/
    PageStack.Size = pageStackSize;
    PageStack.Ptr = PAGE_MALLOC(sizeof(PageStack.Ptr)*PageStack.Size);
    PageManager_PageStackClear();
}

/**
  * @brief  页面调度器析构
  * @param  无
  * @retval 无
  */
void PageManager_Deinit()
{
    PAGE_FREE(PageList);
    PAGE_FREE(&PageStack);
}

/**
  * @brief  清除一个页面
  * @param  pageID: 页面编号
  * @retval true:成功 false:失败
  */
bool PageManager_PageClear(uint8_t pageID)
{
    if(!IS_PAGE(pageID))
        return false;

    PageList[pageID].SetupCallback = PAGE_NULL;
    PageList[pageID].LoopCallback = PAGE_NULL;
    PageList[pageID].ExitCallback = PAGE_NULL;
    PageList[pageID].EventCallback = PAGE_NULL;

    return true;
}

/**
  * @brief  注册一个基本页面，包含一个初始化函数，循环函数，退出函数，事件函数
  * @param  pageID: 页面编号
  * @param  setupCallback: 初始化函数回调
  * @param  loopCallback: 循环函数回调
  * @param  exitCallback: 退出函数回调
  * @param  eventCallback: 事件函数回调
  * @retval true:成功 false:失败
  */
bool PageManager_PageRegister(\
    uint8_t pageID,\
    CallbackFunction_t setupCallback,\
    CallbackFunction_t loopCallback,\
    CallbackFunction_t exitCallback,\
    EventFunction_t eventCallback\
)\
{
    if(!IS_PAGE(pageID))
        return false;

    PageList[pageID].SetupCallback = setupCallback;
    PageList[pageID].LoopCallback = loopCallback;
    PageList[pageID].ExitCallback = exitCallback;
    PageList[pageID].EventCallback = eventCallback;
    return true;
}

/**
  * @brief  页面事件传递
  * @param  obj: 发生事件的对象
  * @param  event: 事件编号
  * @retval 无
  */
void PageManager_PageEventTransmit(void* obj, int event)
{
    /*将事件传递到当前页面*/
    if(PageList[NowPage].EventCallback != PAGE_NULL)
        PageList[NowPage].EventCallback(obj, event);
}

/**
  * @brief  切换到指定页面
  * @param  pageID: 页面编号
  * @retval 无
  */
void PageManager_PageChangeTo(uint8_t pageID)
{
    if(!IS_PAGE(pageID))
        return;
    
    /*检查页面是否忙碌*/
    if(!IsPageBusy)
    {
        /*新页面ID*/
        NextPage = NewPage = pageID;

        /*标记为忙碌状态*/
        IsPageBusy = true;
    }
}

/**
  * @brief  页面压栈，跳转至该页面
  * @param  pageID: 页面编号
  * @retval true:成功 false:失败
  */
bool PageManager_PagePush(uint8_t pageID)
{
    if(!IS_PAGE(pageID))
        return false;
    
    /*检查页面是否忙碌*/
    if(IsPageBusy)
    {
        LOG_D("page busy(%d)",PageStack.Ptr[PageStack.Top]);
        return false; 
    }
       
    
    /*防止栈溢出*/
    if(PageStack.Top >= PageStack.Size - 1)
        return false;
    
    /*防止重复页面压栈*/
    if(pageID == PageStack.Ptr[PageStack.Top])
        return false;

    /*栈顶指针上移*/
    PageStack.Top++;
    
    /*页面压栈*/
    PageStack.Ptr[PageStack.Top] = pageID;
    
    /*页面跳转*/
    PageManager_PageChangeTo(PageStack.Ptr[PageStack.Top]);
    
    return true;
}

/**
  * @brief  页面弹栈，跳转至上一个页面
  * @param  无
  * @retval true:成功 false:失败
  */
bool PageManager_PagePop()
{
    /*检查页面是否忙碌*/
    if(IsPageBusy)
       return false; 
    
    /*防止栈溢出*/
    if(PageStack.Top == 0)
       return false;
    
    /*清空当前页面*/
    PageStack.Ptr[PageStack.Top] = 0;
    
    /*弹栈，栈顶指针下移*/
    PageStack.Top--;
    
    /*页面跳转*/
    PageManager_PageChangeTo(PageStack.Ptr[PageStack.Top]);
    
    return true;
}

/**
  * @brief  清空页面栈
  * @param  无
  * @retval 无
  */
void PageManager_PageStackClear()
{
    /*检查页面是否忙碌*/
    if(IsPageBusy)
        return;

    if(!PageStack.Size)
        return;

    /*清除栈中左右数据*/
    for(uint8_t i = 0; i < PageStack.Size; i++)
    {
        PageStack.Ptr[i] = 0;
    }
    /*栈顶指针复位*/
    PageStack.Top = 0;
}

/**
  * @brief  页面调度器状态机
  * @param  无
  * @retval 无
  */
void PageManager_Running()
{
    /*页面切换事件*/
    if(NewPage != OldPage)
    {
        /*标记为忙碌状态*/
        IsPageBusy = true;

        /*触发旧页面退出事件*/
        if(PageList[OldPage].ExitCallback != PAGE_NULL && IS_PAGE(OldPage))
            PageList[OldPage].ExitCallback();
        
        /*标记旧页面*/
        LastPage = OldPage;
        
        /*标记新页面为当前页面*/
        NowPage = NewPage;

        /*触发新页面初始化事件*/
        if(PageList[NewPage].SetupCallback != PAGE_NULL && IS_PAGE(NewPage))
            PageList[NewPage].SetupCallback();

        /*新页面初始化完成，标记为旧页面*/
        OldPage = NewPage;
    }
    else
    {
        /*标记页面不忙碌，处于循环状态*/
        IsPageBusy = false;
        
        /*页面循环事件*/
        if(PageList[NowPage].LoopCallback != PAGE_NULL && IS_PAGE(NowPage))
            PageList[NowPage].LoopCallback();
    }
}

// 获取当前的页面
uint8_t PageManager_GetNowPage()
{
    
    /*检查页面是否忙碌*/
    if(!IsPageBusy)
    {
        /*新页面ID*/
        return PageStack.Ptr[PageStack.Top];
    } else {
        return OldPage;
    }
}

#endif
/*
 * EOF
 */