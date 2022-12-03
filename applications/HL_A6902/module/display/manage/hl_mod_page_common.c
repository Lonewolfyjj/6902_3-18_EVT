/**
 * @file hl_mod_page_common.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-12-01
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
 * <tr><td>2022-12-01     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "hl_mod_page_common.h"
#include "hl_mod_display_mng.h"
#include "lvgl.h"

#if !HL_IS_TX_DEVICE()
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
#define DBG_SECTION_NAME "display"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

static hl_scr_in_data_t in_data = {0};

void hl_mod_display_scr_set_page(uint32_t now_page)
{
    in_data.in_cmd.page_id = now_page;
}

hl_screen_page_e hl_mod_display_scr_get_page(void)
{
    return in_data.in_cmd.page_id;
}

static uint8_t hl_mod_display_scr_scan_incmd(uint32_t* now_screen_mode)
{
    static hl_scr_in_cmd_t last_in_cmd;
    hl_scr_in_cmd_t        now;

    now.page_id = hl_mod_display_scr_get_page();

    if (last_in_cmd.page_id != now.page_id) {
        last_in_cmd.page_id = now.page_id;
        *now_screen_mode    = now.page_id;
        LOG_E("scr new pageid(%d)\n",now.page_id);

        return HL_DISPLAY_SUCCESS;
    } else {
        return HL_DISPLAY_FAILED;
    }
}

//模外设置的代码
uint8_t hl_mod_display_scr_page_incmd(void)
{
    uint8_t now_page_id;

    if (HL_DISPLAY_SUCCESS == hl_mod_display_scr_scan_incmd(&now_page_id)) {
        if ( false == PageManager_PagePush(now_page_id) ) {
            return HL_DISPLAY_FAILED;
        } else {
            return HL_DISPLAY_SUCCESS;
        }
    }

    return HL_DISPLAY_SUCCESS;
}

static void delete (lv_obj_t* obj, bool obj_typ)
{
    uint32_t child_cnt = 0, i;
    child_cnt          = lv_obj_get_child_cnt(obj);
    if (child_cnt == 0) {
        lv_obj_del(obj);
    } else {
        for (i = 0; i < child_cnt; i++) {
            delete (lv_obj_get_child(obj, i), true);
        }
        if (obj_typ) {
            lv_obj_del(obj);
        }
    }
}
//使用方法

// // 递归删除子控件

void hl_mod_page_delete(lv_obj_t* obj)
{
    delete (obj, false);
}

#endif
/*
 * EOF
 */