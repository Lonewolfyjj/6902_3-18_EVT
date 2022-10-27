#include "hl_mod_apple_auth.h"

/// IAP2线程句柄
static struct rt_thread hl_mod_apple_auth_iap2_thread;
/// EAP线程句柄
static struct rt_thread hl_mod_apple_auth_eap_thread;

/// IAP2线程栈数组
static char iap2_thread_stack[IAP2_THREAD_STACK_SIZE];
/// EAP线程栈数组
static char eap_thread_stack[EAP_THREAD_STACK_SIZE];

/**
 * hl_mod_apple_auth_iap2_thread_entry
 * @brief 苹果认证iap2协议流程
 * @param [in] parameter 
 * @date 2022-09-02
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-02      <td>lisonglin     <td>新建
 * </table>
 */
static void hl_mod_apple_auth_iap2_thread_entry(void* parameter)
{
    // -----------------
}

/**
 * hl_mod_apple_auth_eap_thread_entry
 * @brief 苹果外部接口通信协议处理流程
 * @param [in] parameter 
 * @date 2022-09-02
 * @author lisonglin (songlin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-02      <td>lisonglin     <td>新建
 * </table>
 */
static void hl_mod_apple_auth_eap_thread_entry(void* parameter)
{
    // -----------------
}

int hl_mod_apple_auth_init()
{
    rt_err_t result;

    result =
        rt_thread_init(&hl_mod_apple_auth_iap2_thread, "apple_auth_iap2", hl_mod_apple_auth_iap2_thread_entry, RT_NULL,
                       &iap2_thread_stack[0], sizeof(iap2_thread_stack), IAP2_THREAD_PRIORITY, IAP2_THREAD_TIMESLICE);

    result =
        rt_thread_init(&hl_mod_apple_auth_eap_thread, "apple_auth_eap", hl_mod_apple_auth_eap_thread_entry, RT_NULL,
                       &eap_thread_stack[0], sizeof(eap_thread_stack), EAP_THREAD_PRIORITY, EAP_THREAD_TIMESLICE);

    if (result == RT_EOK) {
        rt_kprintf("hl_mod_apple_auth_thread init succeed!\n");
    } else {
        rt_kprintf("hl_mod_apple_auth_thread init faild!\n");
    }

    return  0;
}

int hl_mod_apple_auth_deinit()
{
    return  0;
}

int hl_mod_apple_auth_start()
{
    rt_thread_startup(&hl_mod_apple_auth_iap2_thread);
    rt_thread_startup(&hl_mod_apple_auth_eap_thread);

    return 0;
}

int hl_mod_apple_auth_stop()
{
    return  0;
}
