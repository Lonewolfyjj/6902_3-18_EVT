/**
 * @file hl_mod_page_tx_gain_conf.c
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




static void hl_mod_page_setup(void)
{

}

static void hl_mod_page_exit(void)
{
   
}

static void hl_mod_page_loop(void)
{
    uint8_t key_event;

    key_event  = hl_mod_get_knob_okkey_val();
    
    if (key_event == HL_KEY_EVENT_SHORT) {
    }
  
}

PAGE_DEC(PAGE_TX_GAIN_CONF)
{
    bool result;

    result     = PageManager_PageRegister(PAGE_TX_GAIN_CONF, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit,
                                      NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_TX_GAIN_CONF init fail\n");
    }
}

#endif