/**
 * @file hl_mod_page_pairing.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 配对中页面，配对成功转到home界面
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
#include "page_arc.h"
#include "hl_util_general_type.h"
#include "page_language.h"

static void hl_mod_page_setup(void)
{
    hl_lvgl_arc_init_t init;
    a6902_language_typ_t* page_ptr = (a6902_language_typ_t *)hl_a6902_language_ptr_get();

    init.ptr = page_ptr->arc_page_ptr->page_arc->ptr_paring;//"配对中";

    hl_mod_arc_init(&init);
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_arc_ioctl_t ioctrl;

    ioctrl.ioctl_cmd = HL_ARC_DELETE_PAGE;

    hl_mod_arc_ioctl(&ioctrl);
}

static void hl_mod_page_loop(void)
{
    hl_rf_state_e               rf_net_connect;
    hl_display_screen_s*        data_ptr = hl_mod_page_get_screen_data_ptr();
    hl_display_screen_change_s* flag     = hl_mod_page_get_screen_change_flag();

    if (flag->rf_net_connect) {

        rf_net_connect = data_ptr->rf_net_connect;

        if (HL_RF_L_CONNECT <= rf_net_connect && rf_net_connect <= HL_RF_LR_CONNECT) {
            hl_mod_menu_goto_home_page();
        } else if (rf_net_connect == HL_RF_UNCONNECT) {
            // 失败返回主页面, 主页面不显示TX1/2
            hl_mod_menu_goto_home_page();
        }
    }
}

PAGE_DEC(PAGE_PARING)
{
    bool result;

    result = PageManager_PageRegister(PAGE_PARING, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_POWEROFF_CHARGE init fail\n");
    }
}

#endif
/*
 * EOF
 */
