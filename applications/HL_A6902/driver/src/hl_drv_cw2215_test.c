/**
 * @file hl_drv_cw2215_test.c
 * @author lilin (lin.li@hollyland-tech.com)
 * @brief 
 * @version 1.0
 * @date 2022-09-17
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
 * <tr><td>2022-09-17     <td>v1.0     <td>lilin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/

#include "hl_drv_cw2215.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
int hl_drv_cw2215_test_init()
{
    int ret;
    char chip_id;

    ret = hl_drv_cw2215_init();
    if (ret == CW2215_FUNC_RET_ERR) {
        return CW2215_FUNC_RET_ERR;
    }

    ret = hl_drv_cw2215_ctrl(HL_DRV_GUAGE_GET_CHIP_ID, &chip_id, sizeof(chip_id));
    if (ret == CW2215_FUNC_RET_ERR) {
        return CW2215_FUNC_RET_ERR;
    }

    rt_kprintf("chip id : %02x\n", chip_id);

    return 0;
}

MSH_CMD_EXPORT(hl_drv_cw2215_test_init, init cw2215 test);
/*
 * EOF
 */