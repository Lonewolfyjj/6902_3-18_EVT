/**
 * @file hl_mod_page_upgrade.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2023-01-05
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
 * <tr><td>2023-01-05     <td>v1.0     <td>liujie     <td>内容
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
#include "page_upgrade.h"
#include "hl_util_general_type.h"
#include "page_language.h"
static void hl_mod_page_setup(void)
{
    hl_display_screen_s* data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_lvgl_upgrade_init_t init;
    data_ptr->upgrade_progress = 99;
    init.upgrade_progress = data_ptr->upgrade_progress;
    hl_mod_lvgl_upgrade_init(&init);
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_upgrade_ioctl_t ioctrl;
    ioctrl.upgrade_ioctl = HL_UPGRADE_CLEAR_CMD;

    hl_mod_lvgl_upgrade_ioctl(&ioctrl);
}

static void hl_mod_page_loop(void)
{
    uint8_t                     upgrade_process = 0;
    hl_display_screen_s*        data_ptr        = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag            = hl_mod_page_get_screen_change_flag();
    hl_lvgl_upgrade_ioctl_t     ioctrl;
    a6902_language_typ_t* page_ptr = (a6902_language_typ_t *)hl_a6902_language_ptr_get();
    static uint8_t status = 0;
    if (status == 1) {
        return;
    }

    switch (data_ptr->upgrade_status) {
        case HL_UPGRADE_STATUS_NORMAL:
            break;
        case HL_UPGRADE_STATUS_UPGRADE:
            if (flag->upgrade_progress) {
                hl_mod_display_mux_take();
                flag->upgrade_progress = 0;
                upgrade_process        = data_ptr->upgrade_progress;
                hl_mod_display_mux_release();

                ioctrl.upgrade_progress = upgrade_process;
                ioctrl.upgrade_ioctl    = HL_UPGRADE_PARAM_CMD;
                hl_mod_lvgl_upgrade_ioctl(&ioctrl);
            }
            break;
        case HL_UPGRADE_STATUS_SUCCESS:

            ioctrl.upgrade_ioctl = HL_UPGRADE_CLEAR_CMD;
            hl_mod_lvgl_upgrade_ioctl(&ioctrl);
            ioctrl.ptr           = page_ptr->upgrade_page_ptr->page_upgrade->ptr_success;//"升级成功";
            ioctrl.upgrade_ioctl = HL_UPGRADE_SUCCESS_CMD;
            hl_mod_lvgl_upgrade_ioctl(&ioctrl);
            if (!status) {
                status = 1;
            }
            break;
        case HL_UPGRADE_STATUS_FAIL:
            ioctrl.upgrade_ioctl = HL_UPGRADE_CLEAR_CMD;

            hl_mod_lvgl_upgrade_ioctl(&ioctrl);
            if (!status) {
                status = 1;
            }
            ioctrl.ptr           = "升级失败";
            ioctrl.upgrade_ioctl = HL_UPGRADE_FAIL_CMD;
            hl_mod_lvgl_upgrade_ioctl(&ioctrl);
            break;
        default:
            break;
    }
}

PAGE_DEC(PAGE_UPGRADE)
{
    bool result;

    result = PageManager_PageRegister(PAGE_UPGRADE, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_UPGRADE init fail\n");
    }
}

#endif
/*
 * EOF
 */
