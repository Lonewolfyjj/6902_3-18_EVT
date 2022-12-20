/**
 * @file hl_mod_page_fast_tx_config.c
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

//录制开关、静音、降噪开关界面
static void hl_moreone_test_cb(hl_storage_check_t event_num)
{
    printf("event_num = %d\n", event_num);
}

static  void fast_conf_init(void)
{    
    hl_lvgl_moreone_init_t data = 
    {
        .func_cb = hl_moreone_test_cb,
        .moreone_choose_opt.option_one = HL_MOREONE_CHOOSE_LEFT,
        .moreone_choose_opt.option_two = HL_MOREONE_CHOOSE_RIGHT,
        .moreone_choose_opt.option_three = HL_MOREONE_CHOOSE_LEFT,
        .moreone_mid_opt = HL_MOREONE_OPTION_THREE,
    };
    hl_mod_moreone_init(&data);

    hl_lvgl_moreone_ioctl_t moreone_ctl = 
    {
        .cmd_type = HL_MOREONE_OPTION_CMD,
        .moreone_choose_opt = HL_MOREONE_OPTION_TWO,
        .moreone_choose = HL_MOREONE_CHOOSE_LEFT
    };
    hl_mod_moreone_ioctl(&moreone_ctl);
}


static void hl_mod_page_setup(void)
{
    fast_conf_init();
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_moreone_ioctl_t data;
    data.cmd_type           = HL_MOREONE_POSTION_CMD;
    data.moreone_choose     = 0;
    data.moreone_choose_opt = HL_MOREONE_OPTION_EXTI;
    hl_mod_moreone_ioctl(&data);

    data.cmd_type           = HL_MOREONE_POSTION_CMD;
    data.moreone_choose     = 0;
    data.moreone_choose_opt = HL_MOREONE_OPTION_DELETE_STYLE;
    hl_mod_moreone_ioctl(&data);
}

static void hl_mod_page_loop(void)
{
    hl_mod_menu_backbtn_scan();
}

PAGE_DEC(PAGE_FAST_TX_CONFIG)
{
    bool result;

    result     = PageManager_PageRegister(PAGE_FAST_TX_CONFIG, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit,
                                      NULL);

    if (result == false) {
        LV_LOG_USER("page_fast_tx_config init fail\n");
    }
}

#endif
/*
 * EOF
 */



