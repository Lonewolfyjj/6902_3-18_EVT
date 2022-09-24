/**
 * @file hl_drv_aw21009_test.c
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

#include "hl_drv_aw21009.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
int hl_drv_aw21009_test_open(void)
{
    hl_drv_aw21009_init();
    hl_drv_aw21009_ctrl(HL_DRV_AW21009_LED_DEV_1, HL_DRV_AW21009_TEMP_LED_OPEN, NULL, 0);

    return 0;
}

int hl_drv_aw21009_test_close(void)
{
    hl_drv_aw21009_init();
    hl_drv_aw21009_ctrl(HL_DRV_AW21009_LED_DEV_1, HL_DRV_AW21009_TEMP_LED_CLOSE, NULL, 0);

    return 0;
}

MSH_CMD_EXPORT(hl_drv_aw21009_test_open, open aw21009 test);

MSH_CMD_EXPORT(hl_drv_aw21009_test_close, close aw21009 test);
/*
 * EOF
 */