/**
 * @file hl_hal_gpio_test.c
 * @author luzhanghao (rd14@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-09-16
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
 * <tr><td>2022-09-16     <td>v1.0     <td>luzhanghao     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#if 1
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <rtdevice.h>
#include "hl_hal_gpio.h"
#include "hal_pinctrl.h"
#include "iomux.h"
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
hl_gpio_pin_e irq_arg_table[USER_GPIO_COUNT];  // 中断参数列表，存放中断枚举编号

/* Private function(only *.c)  -----------------------------------------------*/
void hl_hal_gpio_test_irq_process(void *args)
{
    hl_gpio_pin_e gpio_pin_e = *(hl_gpio_pin_e *)args;

    rt_kprintf("irq enum = %d\r\n", gpio_pin_e);
}

/* Exported functions --------------------------------------------------------*/
// hl_hal_gpio_test [enum] [reset | set | read | irq] 
int hl_hal_gpio_test(int argc, char** argv)
{
    hl_gpio_pin_e gpio_pin_e = GPIO_PWR_KEY;
    int           gpio_level;

    if (argc == 3) {
        gpio_pin_e = (hl_gpio_pin_e) atoi(argv[1]);
        if (gpio_pin_e >= USER_GPIO_COUNT) {
            rt_kprintf("wrong parameter emum, hl_hal_gpio_test [enum] [reset | set | read | irq]  enum must less than USER_GPIO_COUNT(%d) \r\n", USER_GPIO_COUNT);
            return -1;
        }

        if (!strcmp("reset", argv[2])) {                     // 初始化gpio，并输出低电平
            hl_hal_gpio_init(gpio_pin_e);
            hl_hal_gpio_low(gpio_pin_e);
            rt_kprintf("gpio reset completed\r\n");
        } else if (!strcmp("set", argv[2])) {                // 初始化gpio，并输出高电平
            hl_hal_gpio_init(gpio_pin_e);
            hl_hal_gpio_high(gpio_pin_e);
            rt_kprintf("gpio set completed\r\n");
        } else if (!strcmp("read", argv[2])) {               // 初始化gpio，并读取其电平
            hl_hal_gpio_init(gpio_pin_e);
            gpio_level = hl_hal_gpio_read(gpio_pin_e);
            rt_kprintf("gpio read completed, gpio level = %d \r\n", gpio_level);
        } else if (!strcmp("irq", argv[2])) {                // 初始化gpio，并打开中断，绑定中断函数
            irq_arg_table[gpio_pin_e] = gpio_pin_e;
            hl_hal_gpio_init(gpio_pin_e);
            hl_hal_gpio_attach_irq(gpio_pin_e, PIN_IRQ_MODE_FALLING, hl_hal_gpio_test_irq_process, &irq_arg_table[gpio_pin_e]);
            hl_hal_gpio_irq_enable(gpio_pin_e, PIN_IRQ_ENABLE);
            rt_kprintf("gpio set irq completed\r\n");
        } else {
            rt_kprintf("wrong parameter subcommand, hl_hal_gpio_test [enum] [reset | set | read | irq],   subcommand must equal to [reset | set | read | irq] \r\n");
        }
    } else {
        rt_kprintf("wrong parameter, please type: hl_hal_gpio_test [enum] [reset | set | read | irq] \r\n");
        return -1;
    }

    return 0;
}
MSH_CMD_EXPORT(hl_hal_gpio_test, gpio test cmd);


void hl_hal_gpio_pwr_on(void)
{
    // 打开总电源
    hl_hal_gpio_init(GPIO_ALL_POWER);
    hl_hal_gpio_high(GPIO_ALL_POWER);

#if HL_IS_TX_DEVICE()
    // 设置I2S为高阻抗
    hl_hal_gpio_init(I2S1_MCLK);
    hl_hal_gpio_init(I2S1_SCLK);
    hl_hal_gpio_init(I2S1_LCLK);
    hl_hal_gpio_init(I2S1_DTATE);
    hl_hal_gpio_init(I2S0_SCLK);
    hl_hal_gpio_init(I2S0_LCLK);
    hl_hal_gpio_init(I2S0_DTATE);

    rt_thread_mdelay(80);

    hl_hal_gpio_init(GPIO_DC3V3_EN);
    hl_hal_gpio_high(GPIO_DC3V3_EN);

    rt_thread_mdelay(10);

    // 配置I2S(codec上电之后)
#ifdef RT_USING_I2STDM1
    i2s1_output_iomux_config();
#endif
#ifdef RT_USING_I2STDM
    i2s0_input_iomux_config();
#endif

    hl_hal_gpio_init(GPIO_ALL_POWER);
    hl_hal_gpio_high(GPIO_ALL_POWER);
    rt_kprintf("A6902 Tx Device Ver:%s enable power!\r\n", A6902_VERSION);
#else   
    // 设置I2S为高阻抗
    hl_hal_gpio_init(I2S1_MCLK);
    hl_hal_gpio_init(I2S1_SCLK);
    hl_hal_gpio_init(I2S1_LCLK);
    hl_hal_gpio_init(I2S1_DTATE);
    hl_hal_gpio_init(I2S0_SCLK);
    hl_hal_gpio_init(I2S0_LCLK);
    hl_hal_gpio_init(I2S0_DTATE);

    rt_thread_mdelay(100);

    hl_hal_gpio_init(GPIO_CODEC_EN);
    hl_hal_gpio_high(GPIO_CODEC_EN);

    rt_thread_mdelay(10);

    // 配置I2S(codec上电之后)
#ifdef RT_USING_I2STDM1
    i2s1_input_iomux_config();
#endif
#ifdef RT_USING_I2STDM
    i2s0_output_iomux_config();
#endif   

#if (A6902_RX_HL_EN || A6902_RX_HL_CH)
    rt_kprintf("A6902 Rx Device Ver:%s enable power!\r\n", A6902_VERSION);
#endif
#endif
    rt_thread_mdelay(10);
}
INIT_PREV_EXPORT(hl_hal_gpio_pwr_on);

// int hl_hal_gpio_low_test(int argc, char** argv)
// {
//     uint8_t i = 0;
//     for(i = GPIO_PWR_KEY;i<USER_GPIO_COUNT;i++){
//         hl_hal_gpio_init(i);
//         hl_hal_gpio_low(i);
//     }
//     return RT_EOK;
// }
// INIT_PREV_EXPORT(hl_hal_gpio_low_test);
#endif
/*
 * EOF
 */