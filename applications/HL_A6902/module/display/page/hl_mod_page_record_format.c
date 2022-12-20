
/**
 * @file hl_mod_page_record_format.c
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
#include "hl_util_general_type.h"


//格式化界面
static void hl_storage_test_cb(hl_storage_check_t event_num)
{
    printf("event_num = %d\n", event_num);
}

static void hl_mod_page_setup(void)
{
    hl_lvgl_storage_init_t data = 
    {
        .func_cb = hl_storage_test_cb,
        .ptr_time_tx1 = "9h可用",
        .ptr_time_tx2 = "3h可用",
        .storage_choose = HL_STORAGE_CHOOSE_RIGHT,
        .used_tx1 = 15,
        .used_tx2 = 68,
    };
    hl_mod_storage_init(&data);

    hl_lvgl_storage_ioctl_t storage_ctl = 
    {
        .storage_choose = HL_STORAGE_CHOOSE_RIGHT,
    };
    hl_mod_storage_ioctl(&storage_ctl);
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_storage_ioctl_t storage_ctl = 
    {
        .storage_choose = HL_STORAGE_CHOOSE_EXIT,
    };
    hl_mod_storage_ioctl(&storage_ctl);
    storage_ctl.storage_choose = HL_STORAGE_CHOOSE_DEL_STYLE;
    hl_mod_storage_ioctl(&storage_ctl);
}

static void hl_mod_page_loop(void)
{
    // 返回按键
    hl_mod_menu_backbtn_scan();  
}

PAGE_DEC(PAGE_RECORD_FORMAT)
{
    bool result;

    result     = PageManager_PageRegister(PAGE_RECORD_FORMAT, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit,
                                      NULL);

    if (result == false) {
        LV_LOG_USER("page_record_format init fail\n");
    }
}

#endif
/*
 * EOF
 */



