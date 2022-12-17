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
#if HL_IS_TX_DEVICE()  //tx
int hl_drv_aw2016a_test_breath(int argc, char* argv[])
{
    int                            ret;
    bool                           flag;
    uint8_t                        chip_id;
    uint8_t                        led_chan;
    hl_drv_aw2016a_breath_param_st pattern_param;
    hl_drv_aw2016a_light_st        light;

    rt_kprintf("\naw2016a test\n");

    hl_drv_aw2016a_init();

    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_GET_CHIP_ID, &chip_id, sizeof(chip_id));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    rt_kprintf("chip_id:%02x\n", chip_id);

    flag = true;

    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_SYNC_CONTROL_MODE, &flag, sizeof(flag));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    rt_kprintf("set sync control mode \n");

    led_chan = HL_DRV_AW2016A_LED_CHANNEL1;

    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_PATTERN_MODE, &led_chan, sizeof(led_chan));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    rt_kprintf("set pattern mode \n");

    pattern_param.led_chan = HL_DRV_AW2016A_LED_CHANNEL1;
    pattern_param.repeat   = 0;
    pattern_param.t0       = 0;
    pattern_param.t1       = 9;
    pattern_param.t2       = 3;
    pattern_param.t3       = 9;
    pattern_param.t4       = 0;

    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_BREATH_PARAM, &pattern_param,
                              sizeof(pattern_param));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    rt_kprintf("set pattern mode param \n");

    light.r          = atoi(argv[1]);
    light.g          = atoi(argv[2]);
    light.b          = atoi(argv[3]);
    light.brightness = atoi(argv[4]);

    ret = hl_drv_aw2016a_ctrl(HL_DRV_AW2016A_LED0, HL_DRV_AW2016A_SET_LED_LIGHT_EFFECT, &light, sizeof(light));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    rt_kprintf("set led light \n");

    return 0;
}

int hl_drv_aw2016a_test_led_ctrl(int argc, char* argv[])
{
    int                        ret;
    bool                       flag;
    uint8_t                    chip_id;
    hl_drv_aw2016a_led_ctrl_st led_ctrl;

    rt_kprintf("\naw2016a test\n");

    hl_drv_aw2016a_init();

    ret = hl_drv_aw2016a_ctrl(atoi(argv[1]), HL_DRV_AW2016A_GET_CHIP_ID, &chip_id, sizeof(chip_id));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    rt_kprintf("chip_id:%02x\n", chip_id);

    led_ctrl.breath_mode = atoi(argv[2]);
    led_ctrl.color = atoi(argv[3]);

    ret = hl_drv_aw2016a_ctrl(atoi(argv[1]), HL_DRV_AW2016A_LED_CTRL, &led_ctrl, sizeof(led_ctrl));
    if (ret == AW2016A_FUNC_RET_ERR) {
        return AW2016A_FUNC_RET_ERR;
    }

    return 0;
}

MSH_CMD_EXPORT(hl_drv_aw2016a_test_breath, init aw2016a test : r g b l);
MSH_CMD_EXPORT(hl_drv_aw2016a_test_led_ctrl, test led ctrl cmd : led breath_mode color);
#endif
/*
 * EOF
 */