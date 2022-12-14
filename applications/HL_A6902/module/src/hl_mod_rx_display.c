/**
 * @file hl_mod_rx_display.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-12-07
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
 * <tr><td>2022-12-07     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/*
 * EOF
 */
#include "hl_mod_display.h"

#if !HL_IS_TX_DEVICE()

#include "hl_drv_aw2016a.h"
#include "hl_drv_rm690a0.h"
#include "hl_util_msg_type.h"
#include "hl_hal_gpio.h"
#include "hl_mod_page_common.h"
#include <rtthread.h>
#include "hl_util_msg_type.h"



#define DBG_SECTION_NAME "display"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

#define DISPLAY_THREAD_PRIORITY 18
#define DISPLAY_THREAD_STACK_SIZE 20480
#define DISPLAY_THREAD_TIMESLICE 5

// 单位 毫秒
#define RTHEAD_DELAY_TIME LV_DISP_DEF_REFR_PERIOD

static rt_thread_t display_tid = RT_NULL;

typedef struct _hl_display_msg_t
{
    /// 消息队列句柄
    rt_mq_t msg_hander;
    ///  消息
    mode_to_app_msg_t msg;

} hl_display_msg_t;

static hl_display_msg_t hl_mod_display;

static uint8_t hl_mod_display_led_mode_get(uint32_t* now_led_mode);

static void hl_mod_display_led_set(void);

static int  hl_mod_display_led_init(uint8_t led_num);
static void hl_mod_display_led_color_set(uint8_t lednum, uint8_t red, uint8_t green, uint8_t blue);

uint8_t hl_mod_display_out_task()
{
    return HL_DISPLAY_SUCCESS;
}

uint8_t hl_mod_display_io_ctrl(uint8_t cmd, void* ptr, uint16_t len)
{
    uint8_t res = HL_DISPLAY_SUCCESS;

    switch (cmd) {
        case MSG_INPUT_UPDATE_CMD: {
            mode_to_app_msg_t msg = { 0 };
            msg = *(mode_to_app_msg_t*)ptr;
            hl_mod_indev_val_get(&msg);

            break;
        }

        default:
            break;
    }
    LOG_D("cmd=%d,*ptr=%d\r\n", cmd, *(uint32_t*)ptr);
    return res;
}

// RX
static void hl_mod_display_task(void* param)
{

    while (1) {
        
        PageManager_Running();
        // rt_thread_mdelay(RTHEAD_DELAY_TIME);
        lv_task_handler();
        rt_thread_mdelay(LV_DISP_DEF_REFR_PERIOD);
    }
}

uint8_t hl_mod_display_deinit(void)
{
    if (display_tid != RT_NULL) {
        rt_thread_delete(display_tid);
    }
    hl_drv_aw2016a_deinit();
    hl_drv_rm690a0_deinit();

    return HL_DISPLAY_SUCCESS;
}

uint8_t hl_mod_display_init(void* display_msq)
{
    uint32_t frambufferaddr;

    if (display_msq == NULL) {
        LOG_D("msghander err!");
        return HL_DISPLAY_FAILED;
    }

    hl_mod_display.msg_hander = (rt_mq_t)display_msq;

    hl_drv_aw2016a_deinit();
    hl_drv_aw2016a_init();


    // RX
    hl_drv_rm690a0_init();
    lvgl2rtt_init();

    hl_mod_page_all_init();

    display_tid = rt_thread_create("display_thread", hl_mod_display_task, RT_NULL, DISPLAY_THREAD_STACK_SIZE,
                                   DISPLAY_THREAD_PRIORITY, DISPLAY_THREAD_TIMESLICE);

    if (display_tid != RT_NULL) {

        LOG_D("display thread init ok!\r\n");
        rt_thread_startup(display_tid);

        return HL_DISPLAY_SUCCESS;
    }

    return HL_DISPLAY_FAILED;
}
#endif
