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
int hl_drv_aw21009_test_open(int argc, char* argv[])  //自动呼吸模式
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

    auto_breath.rise_time    = HL_DRV_AW21009_PAT_TIME_2S60;
    auto_breath.on_time      = HL_DRV_AW21009_PAT_TIME_0S38;
    auto_breath.fall_time    = HL_DRV_AW21009_PAT_TIME_2S60;
    auto_breath.off_time     = HL_DRV_AW21009_PAT_TIME_0S00;
    auto_breath.repeat_times = 0;
    auto_breath.start_point  = HL_DRV_AW21009_START_POINT_T0;
    auto_breath.stop_point   = HL_DRV_AW21009_STOP_POINT_T1;

    hl_drv_aw21009_ctrl(atoi(argv[6]), HL_DRV_AW21009_SET_AUTO_BREATH_PARAM, &auto_breath, sizeof(auto_breath));

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

int hl_drv_aw21009_test_group(int argc, char* argv[])  //组控制模式
{
    bool                            flag;
    hl_drv_aw21009_led_group_e      led_group;
    hl_drv_aw21009_led_light_st     light;
    hl_drv_aw21009_pattern_mode_e   pat_mode;
    hl_drv_aw21009_pwm_resolution_e br_pwm;

    hl_drv_aw21009_init();

    br_pwm = BR_RESOLUTION_12BIT;

    hl_drv_aw21009_ctrl(atoi(argv[5]), HL_DRV_AW21009_SET_PWM_RESOLUTION, &br_pwm, sizeof(br_pwm));

    flag = false;

    hl_drv_aw21009_ctrl(atoi(argv[5]), HL_DRV_AW21009_SET_SINGLE_BYTE_MODE, &flag, sizeof(flag));

    flag = false;

    hl_drv_aw21009_ctrl(atoi(argv[5]), HL_DRV_AW21009_SET_GROUP_CTRL_DISABLE, &flag, sizeof(flag));

    led_group = HL_DRV_AW21009_LED_GROUP_1 | HL_DRV_AW21009_LED_GROUP_2 | HL_DRV_AW21009_LED_GROUP_3;

    hl_drv_aw21009_ctrl(atoi(argv[5]), HL_DRV_AW21009_SET_LED_GROUP_MODE, &led_group, sizeof(led_group));

    light.r          = atoi(argv[1]);
    light.g          = atoi(argv[2]);
    light.b          = atoi(argv[3]);
    light.brightness = atoi(argv[4]);

    hl_drv_aw21009_ctrl(atoi(argv[5]), HL_DRV_AW21009_SET_LED_GROUP_LIGHT_EFFECT, &light, sizeof(light));

    pat_mode = HL_DRV_AW21009_PAT_MODE_DISABLE;

    hl_drv_aw21009_ctrl(atoi(argv[5]), HL_DRV_AW21009_SET_PATTERN_MODE, &pat_mode, sizeof(pat_mode));

    return 0;
}

int hl_drv_aw21009_test_single(int argc, char* argv[])  //独立控制模式
{
    bool                                  flag;
    hl_drv_aw21009_led_group_e            led_group;
    hl_drv_aw21009_led_chan_brightness_st led_chan_brightness;
    hl_drv_aw21009_led_chan_color_st      led_chan_color;
    hl_drv_aw21009_pattern_mode_e         pat_mode;
    hl_drv_aw21009_pwm_resolution_e       br_pwm;

    hl_drv_aw21009_init();

    br_pwm = BR_RESOLUTION_8BIT;

    hl_drv_aw21009_ctrl(atoi(argv[5]), HL_DRV_AW21009_SET_PWM_RESOLUTION, &br_pwm, sizeof(br_pwm));

    flag = true;

    hl_drv_aw21009_ctrl(atoi(argv[5]), HL_DRV_AW21009_SET_SINGLE_BYTE_MODE, &flag, sizeof(flag));

    flag = false;

    hl_drv_aw21009_ctrl(atoi(argv[5]), HL_DRV_AW21009_SET_RGB_MODE, &flag, sizeof(flag));

    flag = true;

    hl_drv_aw21009_ctrl(atoi(argv[5]), HL_DRV_AW21009_SET_GROUP_CTRL_DISABLE, &flag, sizeof(flag));

    led_group = HL_DRV_AW21009_LED_GROUP_NONE;

    hl_drv_aw21009_ctrl(atoi(argv[5]), HL_DRV_AW21009_SET_LED_GROUP_MODE, &led_group, sizeof(led_group));

    led_chan_brightness.led_chan   = atoi(argv[1]);
    led_chan_brightness.brightness = atoi(argv[2]);

    hl_drv_aw21009_ctrl(atoi(argv[5]), HL_DRV_AW21009_SET_LED_CHAN_BRIGHTNESS, &led_chan_brightness,
                        sizeof(led_chan_brightness));

    led_chan_color.led_chan = atoi(argv[3]);
    led_chan_color.color    = atoi(argv[4]);

    hl_drv_aw21009_ctrl(atoi(argv[5]), HL_DRV_AW21009_SET_LED_CHAN_COLOR, &led_chan_color, sizeof(led_chan_color));

    pat_mode = HL_DRV_AW21009_PAT_MODE_DISABLE;

    hl_drv_aw21009_ctrl(atoi(argv[5]), HL_DRV_AW21009_SET_PATTERN_MODE, &pat_mode, sizeof(pat_mode));

    return 0;
}

int hl_drv_aw21009_test_manual(int argc, char* argv[])  //manual模式
{
    bool                                flag;
    hl_drv_aw21009_pattern_mode_e       pat_mode;
    hl_drv_aw21009_auto_breath_param_st auto_breath;
    hl_drv_aw21009_led_light_st         light;
    hl_drv_aw21009_led_group_e          led_group;

    hl_drv_aw21009_init();

    flag = false;

    hl_drv_aw21009_ctrl(atoi(argv[3]), HL_DRV_AW21009_SET_GROUP_CTRL_DISABLE, &flag, sizeof(flag));

    led_group = HL_DRV_AW21009_LED_GROUP_1 | HL_DRV_AW21009_LED_GROUP_2 | HL_DRV_AW21009_LED_GROUP_3;

    hl_drv_aw21009_ctrl(atoi(argv[3]), HL_DRV_AW21009_SET_LED_GROUP_MODE, &led_group, sizeof(led_group));

    auto_breath.rise_time    = HL_DRV_AW21009_PAT_TIME_0S38;
    auto_breath.on_time      = HL_DRV_AW21009_PAT_TIME_0S38;
    auto_breath.fall_time    = HL_DRV_AW21009_PAT_TIME_0S38;
    auto_breath.off_time     = HL_DRV_AW21009_PAT_TIME_0S00;
    auto_breath.repeat_times = 0;
    auto_breath.start_point  = HL_DRV_AW21009_START_POINT_T0;
    auto_breath.stop_point   = HL_DRV_AW21009_STOP_POINT_T1;

    hl_drv_aw21009_ctrl(atoi(argv[3]), HL_DRV_AW21009_SET_AUTO_BREATH_PARAM, &auto_breath, sizeof(auto_breath));

    light.r          = 100;
    light.g          = 100;
    light.b          = 100;
    light.brightness = 0xFF00;

    hl_drv_aw21009_ctrl(atoi(argv[3]), HL_DRV_AW21009_SET_LED_GROUP_LIGHT_EFFECT, &light, sizeof(light));

    pat_mode = HL_DRV_AW21009_PAT_MANUAL_MODE;

    hl_drv_aw21009_ctrl(atoi(argv[3]), HL_DRV_AW21009_SET_PATTERN_MODE, &pat_mode, sizeof(pat_mode));

    flag = atoi(argv[2]);

    hl_drv_aw21009_ctrl(atoi(argv[3]), HL_DRV_AW21009_MANUAL_SET_RAMP, &flag, sizeof(flag));

    flag = atoi(argv[1]);

    hl_drv_aw21009_ctrl(atoi(argv[3]), HL_DRV_AW21009_MANUAL_SET_SWITCH, &flag, sizeof(flag));

    return 0;
}

MSH_CMD_EXPORT(hl_drv_aw21009_test_open, open aw21009 test : r g b l group num);

MSH_CMD_EXPORT(hl_drv_aw21009_test_group, group ctrol test : r g b l num);

MSH_CMD_EXPORT(hl_drv_aw21009_test_single, single ctrol test : ch l ch color num);

MSH_CMD_EXPORT(hl_drv_aw21009_test_manual, manual ctrol test : switch ramp num);
/*
 * EOF
 */