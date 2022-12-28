/**
 * @file hl_mod_page_page_other_set.c
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


#define MENU_ICON_NUM 4
// 下级菜单表
static const hl_screen_page_e next_level_menu_tab[MENU_ICON_NUM] = {
    PAGE_PAIR,
    PAGE_SYS_TIME_SET,
    PAGE_VERSION,
    PAGE_RESTORE,
};

static int8_t menu_center_icon = 0;

//通用设置菜单界面
LV_IMG_DECLARE(Menu_pair);           //配对
LV_IMG_DECLARE(Menu_time_config);    //时间设置
LV_IMG_DECLARE(Menu_verson);         //版本信息
LV_IMG_DECLARE(Menu_reset_factory);  //恢复出厂设置

static void page_9_test_cb(uint32_t current)
{
    if (!(current & 0x80)) {
        hl_mod_next_menu_enter(next_level_menu_tab, current & 0x7F, MENU_ICON_NUM);
    } else {
        hl_mod_knob_select_val_set(&menu_center_icon, current & 0x7F);
    }
}

static void page_9_test(void)
{
    hl_display_screen_change_s* flag = hl_mod_page_get_screen_change_flag();
    
    menu_data_t pic_list[4] = {
        ADD_IMG_DATA(NULL, NULL, &Menu_pair, "配对"),
        ADD_IMG_DATA(NULL, NULL, &Menu_time_config, "时间设置"),
        ADD_IMG_DATA(NULL, NULL, &Menu_verson, "版本信息"),
        ADD_IMG_DATA(NULL, NULL, &Menu_reset_factory, "恢复出厂设置"),
    };
    // 如果是下一级菜单，就默认显示最左边图标
    if (flag->menu_defaut) {
        flag->menu_defaut = 0;
        hl_mod_knob_select_val_set(&menu_center_icon, 0);
    }
    page_menu_init(pic_list, 4, page_9_test_cb,menu_center_icon);
}

static void hl_mod_page_setup(void)
{
    page_9_test();
}

static void hl_mod_page_exit(void)
{
    lv_menu_exit();
}

static void hl_mod_page_loop(void)
{
    hl_mod_menu_enterbtn_scan(hl_mod_knob_select_val_get(&menu_center_icon));
    hl_mod_menu_backbtn_scan();
    if (hl_mod_knob_select_val_change(&menu_center_icon,0,MENU_ICON_NUM - 1, false) ) {
        lv_set_icon_postion(menu_center_icon, false);
    }
}

PAGE_DEC(PAGE_OTHER_SET)
{
    bool result;

    result = PageManager_PageRegister(PAGE_OTHER_SET, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit, NULL);

    if (result == false) {
        LV_LOG_USER("PAGE_OTHER_SET init fail\n");
    }
}

#endif
/*
 * EOF
 */



