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
int hl_drv_aw21009_test_open(int argc, char* argv[])
{
    bool                                flag;
    hl_drv_aw21009_led_group_e          led_group;
    hl_drv_aw21009_auto_breath_param_st auto_breath;
    hl_drv_aw21009_pattern_mode_e       pat_mode;
    hl_drv_aw21009_led_light_st         light;

    hl_drv_aw21009_init();

    flag = true;

    hl_drv_aw21009_ctrl(atoi(argv[6]), HL_DRV_AW21009_SET_GROUP_CTRL_DISABLE, &flag, sizeof(flag));

    led_group = HL_DRV_AW21009_LED_GROUP_1 | HL_DRV_AW21009_LED_GROUP_2 | HL_DRV_AW21009_LED_GROUP_3;

    hl_drv_aw21009_ctrl(atoi(argv[6]), HL_DRV_AW21009_SET_LED_GROUP_MODE, &led_group, sizeof(led_group));

    auto_breath.rise_time    = 9;
    auto_breath.on_time      = 3;
    auto_breath.fall_time    = 9;
    auto_breath.off_time     = 0;
    auto_breath.repeat_times = 0;
    auto_breath.start_point  = HL_DRV_AW21009_START_POINT_T0;
    auto_breath.stop_point   = HL_DRV_AW21009_STOP_POINT_T1;

    hl_drv_aw21009_ctrl(atoi(argv[6]), HL_DRV_AW21009_SET_AUTO_BREATH_PARAM, &auto_breath,
                        sizeof(auto_breath));

    light.r          = atoi(argv[1]);
    light.g          = atoi(argv[2]);
    light.b          = atoi(argv[3]);
    light.brightness = atoi(argv[4]);
    light.led_group  = atoi(argv[5]);

    hl_drv_aw21009_ctrl(atoi(argv[6]), HL_DRV_AW21009_SET_LED_LIGHT_EFFECT, &light, sizeof(light));

    pat_mode = HL_DRV_AW21009_PAT_AUTO_BREATH_MODE;

    hl_drv_aw21009_ctrl(atoi(argv[6]), HL_DRV_AW21009_SET_PATTERN_MODE, &pat_mode, sizeof(pat_mode));

    return 0;
}

int hl_drv_aw21009_test_close(void)
{
    hl_drv_aw21009_init();

    //TBD

    return 0;
}

MSH_CMD_EXPORT(hl_drv_aw21009_test_open, open aw21009 test : r g b l ch);

MSH_CMD_EXPORT(hl_drv_aw21009_test_close, close aw21009 test);
/*
 * EOF
 */