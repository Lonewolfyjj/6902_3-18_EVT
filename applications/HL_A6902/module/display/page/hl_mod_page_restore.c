
/**
 * @file hl_mod_page_restore.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-12-09
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
 * <tr><td>2022-12-09     <td>v1.0     <td>liujie     <td>内容
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
#include "page_test.h"
#include "page_menu.h"
#include "hl_mod_input.h"

//恢复出厂设置界面
static void hl_resetfactory_test_cb(hl_two_in_one_check_t event_num)
{
    printf("event_num = %d\n", event_num);
}
static void resetfactory_test(void)
{
    hl_lvgl_s_two_in_one_init_t s_two_in_one_test = {
        .func_cb             = hl_resetfactory_test_cb,
        .ptr_lift            = "取消",
        .ptr_right           = "确定",
        .ptr_top             = "是否进入配对",
        .s_two_in_one_choose = HL_S_TWO_ONE_CHOOSE_LEFT,
    };
    hl_mod_s_two_in_one_init(&s_two_in_one_test);

    hl_lvgl_s_two_in_one_ioctl_t s_two_in_one_test_ctl = {
        .s_two_in_one_choose = HL_S_TWO_ONE_CHOOSE_LEFT,
    };

    hl_mod_s_two_in_one_ioctl(&s_two_in_one_test_ctl);
}

static void hl_mod_page_setup(void)
{
    resetfactory_test();
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_s_two_in_one_ioctl_t s_two_in_one_test_ctl = {
        .s_two_in_one_choose = HL_S_TWO_ONE_CHOOSE_EXIT,
    };

    hl_mod_s_two_in_one_ioctl(&s_two_in_one_test_ctl);
    s_two_in_one_test_ctl.s_two_in_one_choose = HL_S_TWO_ONE_CHOOSE_DEL_STYLE;
    hl_mod_s_two_in_one_ioctl(&s_two_in_one_test_ctl);
}

static void hl_mod_page_loop(void)
{
    // 返回按键
    hl_mod_menu_backbtn_scan();
}

PAGE_DEC(PAGE_RESTORE)
{
    bool result;

    result = PageManager_PageRegister(PAGE_RESTORE, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_RESTORE init fail\n");
    }
}

#endif
/*
 * EOF
 */



