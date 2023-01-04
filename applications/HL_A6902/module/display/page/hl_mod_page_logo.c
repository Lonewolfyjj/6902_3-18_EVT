/**
 * @file hl_mod_page_logo.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2023-01-04
 * 
 * ██╗  ██╗ ██████╗ ██╗     ██╗  ██╗   ██╗██╗      █████╗ ███╗   ██╗██████╗ 
 * ██║  ██║██╔═══██╗██║     ██║  ╚██╗ ██╔╝██║     ██╔══██╗████╗  ██║██╔══██╗
 * ███████║██║   ██║██║     ██║   ╚████╔╝ ██║     ███████║██╔██╗ ██║██║  ██║
 * ██╔══██║██║   ██║██║     ██║    ╚██╔╝  ██║     ██╔══██║██║╚██╗██║██║  ██║
 * ██║  ██║╚██████╔╝███████╗███████╗██║   ███████╗██║  ██║██║ ╚████║██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝
 * @copyright Copyright (c) 2023 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2023-01-04     <td>v1.0     <td>liujie     <td>内容
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

#include "hl_mod_page_common.h"

#if (!HL_IS_TX_DEVICE())
#include "hl_mod_display.h"
#include "lvgl.h"
#include "hl_mod_page.h"
#include "lv_port_indev.h"
#include "hl_util_general_type.h"

// logo页面定时器
#define LOGO_PAGE_RUNTIME 3000

hl_screenofftime_t logo_page_timer;

static void logo_page_timer_trige(bool updateflag)
{
    if (updateflag == true) {
        // 时间到更新页面
        rt_kprintf("home\n");
        PageManager_PagePush(PAGE_HOME);
    } else {
    }
}

static void hl_mod_page_setup(void)
{
    rt_kprintf("PAGE_LOGO1\n");
    logo_page_timer.outtime  = LOGO_PAGE_RUNTIME;
    logo_page_timer.trigfunc = logo_page_timer_trige;
    hl_mod_page_screenofftimer_init(&logo_page_timer);
    rt_kprintf("PAGE_LOGO\n");
}

static void hl_mod_page_exit(void)
{
    rt_kprintf("exitPAGE_LOGO\n");
    hl_mod_page_screenofftimer_close(&logo_page_timer);
}

static void hl_mod_page_loop(void)
{
    uint8_t                     data;
    hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();

    hl_mod_page_screenofftimer_scan(&logo_page_timer);

    if (flag->sys_status.out_box_poweroff_charge) {
        hl_mod_display_mux_take();
        flag->sys_status.out_box_poweroff_charge = 0;
        data                                     = data_ptr->sys_status.out_box_poweroff_charge;
        hl_mod_display_mux_release();

        if (data) {
            rt_kprintf("chage\n");
            PageManager_PagePush(PAGE_POWEROFF_CHARGE);
        }
    }
}

PAGE_DEC(PAGE_LOGO)
{
    bool result;

    result = PageManager_PageRegister(PAGE_LOGO, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_LOGO init fail\n");
    }
}

#endif
/*
 * EOF
 */
