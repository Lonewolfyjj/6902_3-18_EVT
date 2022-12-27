
/**
 * @file hl_mod_page_low_cut.c
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

//低切界面
static void hl_diqie_test_cb(hl_three_in_one_check_t event_num)
{
    uint8_t value = 0;
    switch(event_num){
        case HL_THREE_ONE_CHECK_LEFT:
            value = LOW_CUT_OFF;
            break;
        case HL_THREE_ONE_CHECK_MID:
            value = LOW_CUT_75HZ;
            break;
        case HL_THREE_ONE_CHECK_RIGHT:
            value = LOW_CUT_150HZ;
            break;
        default:
            return ;
            break;
    }    
    hl_mod_display_send_msg(LOW_CUT_VAL_IND,&value,0);
}

static void hl_mod_page_setup(void)
{
    hl_lvgl_three_in_one_init_t three_in_one_test = 
    {
        .func_cb = hl_diqie_test_cb,
        .ptr_lift = "无低切",
        .ptr_mid = "75HZ",
        .ptr_right = "150HZ",
        .ptr_top = "低切",
        .three_in_one_choose = HL_THREE_ONE_CHOOSE_LEFT,
    };
    hl_mod_three_in_one_init(&three_in_one_test);

    hl_lvgl_three_in_one_ioctl_t three_in_one_test_ctl = 
    {
        .three_in_one_choose = HL_THREE_ONE_CHOOSE_LEFT,
    };

    hl_mod_three_in_one_ioctl(&three_in_one_test_ctl);
}

static void hl_mod_page_exit(void)
{
    hl_lvgl_three_in_one_ioctl_t three_in_one_test_ctl;

    three_in_one_test_ctl.three_in_one_choose = HL_THREE_ONE_CHOOSE_EXIT;
    hl_mod_three_in_one_ioctl(&three_in_one_test_ctl);

    // three_in_one_test_ctl.three_in_one_choose = HL_THREE_ONE_CHOOSE_DEL_STYLE;
    // hl_mod_three_in_one_ioctl(&three_in_one_test_ctl);
}

static void hl_mod_page_loop(void)
{
    // 返回按键
    hl_mod_menu_backbtn_scan();
  
}

PAGE_DEC(PAGE_TX_LOW_CUT)
{
    bool result;

    result     = PageManager_PageRegister(PAGE_TX_LOW_CUT, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit,
                                      NULL);

    if (result == false) {
        LV_LOG_USER("page_tx_low_cut init fail\n");
    }
}

#endif
/*
 * EOF
 */



