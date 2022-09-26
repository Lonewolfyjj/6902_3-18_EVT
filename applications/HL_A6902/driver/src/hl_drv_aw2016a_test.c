/**
 * @file hl_drv_aw2016a_test.c
 * @author lilin (lin.li@hollyland-tech.com)
 * @brief LED测试源文件
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

#include "hl_drv_aw2016a.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
int hl_drv_aw2016a_test_init(int argc, char *argv[])
{
    int     ret;
    uint8_t chip_id;
    uint8_t work_mode;
    uint8_t led_chan;
    hl_drv_aw2016a_pattern_param_st pattern_param;
    hl_drv_aw2016a_output_current_st output_param;
    hl_drv_aw2016a_pwm_level_st pwm_param;

    rt_kprintf("\naw2016a test\n");

    hl_drv_aw2016a_init();

    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_GET_CHIP_ID, &chip_id, sizeof(chip_id));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    rt_kprintf("chip_id:%02x\n", chip_id);

    work_mode = HL_DRV_AW2016A_ACTIVE_MODE;

    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_WORK_MODE, &work_mode, sizeof(work_mode));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    rt_kprintf("set work mode\n");

    work_mode = HL_DRV_AW2016A_IMAX_5MA;

    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_GLOBAL_MAX_OUTPUT_CURRENT, &work_mode,
                              sizeof(work_mode));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    rt_kprintf("set g current max\n");

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;

    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_OPEN_LED_CHANNEL, &led_chan, sizeof(led_chan));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    rt_kprintf("open led \n");

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;

    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_PATTERN_MODE, &led_chan, sizeof(led_chan));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    rt_kprintf("set pattern mode \n");

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    pattern_param.repeat = 0;
    pattern_param.t0 = 0;
    pattern_param.t1 = 9;
    pattern_param.t2 = 3;
    pattern_param.t3 = 9;
    pattern_param.t4 = 0;
    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_PATTERN_MODE_PARAM, &pattern_param, sizeof(pattern_param));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    rt_kprintf("set pattern mode param \n");

    output_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL1;
    output_param.current = atoi(argv[1]);
    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_LED_CHANNEL_OUTPUT_CURRENT, &output_param, sizeof(output_param));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    rt_kprintf("set output current \n");

    output_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL2;
    output_param.current = atoi(argv[2]);
    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_LED_CHANNEL_OUTPUT_CURRENT, &output_param, sizeof(output_param));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    rt_kprintf("set output current \n");

    output_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL3;
    output_param.current = atoi(argv[3]);
    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_LED_CHANNEL_OUTPUT_CURRENT, &output_param, sizeof(output_param));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    rt_kprintf("set output current \n");

    pwm_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL1 | HL_DRV_AW2016A_LED_CHANNEL2 | HL_DRV_AW2016A_LED_CHANNEL3;
    pwm_param.pwm_level = atoi(argv[4]);
    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_LED_CHANNEL_PWM_LEVEL, &pwm_param, sizeof(pwm_param));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    rt_kprintf("set brightness \n");

    return 0;
}

MSH_CMD_EXPORT(hl_drv_aw2016a_test_init, init aw2016a test : r g b l);
/*
 * EOF
 */