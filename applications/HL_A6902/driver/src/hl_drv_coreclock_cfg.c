/**
 * @file hl_drv_coreclock_cfg.c
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief 
 * @version 1.0
 * @date 2022-11-08
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
 * <tr><td>2022-11-08     <td>v1.0     <td>dujunjie     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
#include <rtdevice.h>
#include "hal_base.h"
#include "drv_clock.h"
#include "hl_config.h"
#include "hl_drv_coreclock_cfg.h"
/* typedef -------------------------------------------------------------------*/
typedef struct _clock_name_t
{
    char name[25];
    uint32_t postion;
    uint32_t hz;
}clock_name_t;
/* define --------------------------------------------------------------------*/
#define ADD_LABEL(param) \
    { .name = #param, .postion = param, .hz = 0 ,}

/* variables -----------------------------------------------------------------*/

static clock_name_t clock_name[] = 
{
    ADD_LABEL(PLL_GPLL),
    ADD_LABEL(PLL_CPLL),
    ADD_LABEL(ACLK_DSP),
    ADD_LABEL(PCLK_DSP),
    ADD_LABEL(SCLK_SHRM),
    ADD_LABEL(PCLK_SHRM),
    ADD_LABEL(CLK_UART0_SRC),
    ADD_LABEL(CLK_UART0_FRAC),
    ADD_LABEL(CLK_UART0),
    ADD_LABEL(CLK_UART1_SRC),
    ADD_LABEL(CLK_UART1_FRAC),
    ADD_LABEL(CLK_UART1),
    ADD_LABEL(CLK_UART2_SRC),
    ADD_LABEL(CLK_UART2_FRAC),
    ADD_LABEL(CLK_UART2),
    ADD_LABEL(CLK_I2C0),
    ADD_LABEL(CLK_I2C1),
    ADD_LABEL(CLK_I2C2),
    ADD_LABEL(MCLK_PDM0),
    ADD_LABEL(HCLK_AUDIO),
    ADD_LABEL(CLK_I2S8CH_SRC),
    ADD_LABEL(CLK_I2S8CH_FRAC),
    ADD_LABEL(MCLK_I2S8CH),
    ADD_LABEL(I2S_MCLKOUT),
    ADD_LABEL(ACLK_DMAC),
    ADD_LABEL(PCLK_AUDIO),
    ADD_LABEL(ACLK_VOP),
    ADD_LABEL(DCLK_VOP_S),
    ADD_LABEL(CLK_GPIO_DBG0),
    ADD_LABEL(CLK_GPIO_DBG1),
    ADD_LABEL(PCLK_ALIVE),
    ADD_LABEL(HCLK_ALIVE),
    ADD_LABEL(CLK_TESTOUT),
    ADD_LABEL(CLK_PDM_SAMP),
    ADD_LABEL(HCLK_M4),
    ADD_LABEL(CLK_SPI1),
    ADD_LABEL(SCLK_SFC_SRC),
    ADD_LABEL(CLK_SPI2),
    ADD_LABEL(CLK_SDIO_SRC),
    ADD_LABEL(CLK_AUDPWM_SRC),
    ADD_LABEL(CLK_AUDPWM_FRAC),
    ADD_LABEL(CLK_AUDPWM),
    ADD_LABEL(CLK_I2S1_8CH_SRC),
    ADD_LABEL(CLK_I2S1_8CH_FRAC),
    ADD_LABEL(MCLK_I2S1_8CH),
    ADD_LABEL(I2S1_MCLKOUT),
    ADD_LABEL(CLK_PWM),
    ADD_LABEL(HCLK_LOGIC),
    ADD_LABEL(PCLK_LOGIC),
    ADD_LABEL(ACLK_LOGIC),
    ADD_LABEL(CLK_CIFOUT),
    ADD_LABEL(CLK_USB2PHY_REF_FRAC),
    ADD_LABEL(CLK_USB2PHY_REF),
    ADD_LABEL(CLK_DPHY_REF_FRAC),
    ADD_LABEL(CLK_DPHY_REF),
    ADD_LABEL(SCLK_SFC1_SRC),
    ADD_LABEL(CLK_32K),
    ADD_LABEL(OCC_SCAN_CLK_DPHYLANBYTE),
};
/* Private function(only *.c)  -----------------------------------------------*/
#if HL_IS_TX_DEVICE()

static void hl_mod_pm_gpio_deal()
{
    rt_pin_mode(GPIO1_A2, PIN_MODE_INPUT_PULLDOWN); // GPIO_DC3V3_EN en 

    rt_pin_mode(GPIO1_C4, PIN_MODE_INPUT_PULLDOWN); // GPIO_RF_PWR_EN en 

    rt_pin_mode(GPIO1_A5, PIN_MODE_INPUT_PULLDOWN); //  en GPIO_EMMC_PWR_EN

    rt_pin_mode(GPIO1_A7, PIN_MODE_INPUT_PULLDOWN); //  en GPIO_2831P_EN

    rt_pin_mode(GPIO0_C6, PIN_MODE_INPUT_PULLDOWN); // GPIO_PWR_EN en 

    rt_pin_mode(GPIO1_A3, PIN_MODE_INPUT_PULLUP); // pair key

    rt_pin_mode(GPIO1_B2, PIN_MODE_INPUT_PULLUP); // pwr key

    rt_pin_mode(GPIO1_C1, PIN_MODE_INPUT_PULLUP); // sw rec

    rt_pin_mode(GPIO1_A4, PIN_MODE_INPUT_PULLDOWN); // sw rec

    rt_pin_mode(GPIO1_A4, PIN_MODE_INPUT_PULLDOWN); // VBUS_DET

    rt_pin_mode(GPIO1_B1, PIN_MODE_INPUT); // GAUGE

    rt_pin_mode(GPIO1_C7, PIN_MODE_INPUT_PULLDOWN); // GPIO_ATS_RESET
    
    rt_pin_mode(GPIO1_A0, PIN_MODE_INPUT_PULLDOWN); // 2831p uart2 
    rt_pin_mode(GPIO1_A1, PIN_MODE_INPUT_PULLDOWN);

    rt_pin_mode(GPIO0_D1, PIN_MODE_INPUT_PULLDOWN); // pogo uart
    rt_pin_mode(GPIO0_D2, PIN_MODE_INPUT_PULLDOWN);

    rt_pin_mode(GPIO0_D5, PIN_MODE_INPUT_PULLDOWN); //i2s

    rt_pin_mode(GPIO0_C0, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(GPIO0_C1, PIN_MODE_INPUT_PULLUP);   // emmc cmd
    rt_pin_mode(GPIO0_C2, PIN_MODE_INPUT_PULLDOWN);  
    rt_pin_mode(GPIO0_C3, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(GPIO0_C4, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(GPIO0_C5, PIN_MODE_INPUT_PULLDOWN);
    
    rt_pin_mode(GPIO0_A3, PIN_MODE_INPUT_PULLDOWN); // GPIO_EMMC_RST 
    
    rt_pin_mode(GPIO0_B2, PIN_MODE_INPUT_PULLUP); // IIC M1
    rt_pin_mode(GPIO0_B3, PIN_MODE_INPUT_PULLUP);
    
    rt_pin_mode(GPIO1_C2, PIN_MODE_INPUT_PULLUP); // IIC M2
    rt_pin_mode(GPIO1_C3, PIN_MODE_INPUT_PULLUP);

    rt_pin_mode(GPIO1_C5, PIN_MODE_INPUT_PULLDOWN); // mic det
    rt_pin_mode(GPIO1_C6, PIN_MODE_INPUT_PULLDOWN); // mic en

    rt_pin_mode(GPIO1_A6, PIN_MODE_INPUT_PULLDOWN); // jtag
    
    rt_pin_mode(GPIO0_D3, PIN_MODE_INPUT_PULLDOWN); // i2s
    rt_pin_mode(GPIO0_D4, PIN_MODE_INPUT_PULLDOWN);

    rt_pin_mode(GPIO0_B0, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(GPIO0_A7, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(GPIO0_A6, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(GPIO0_A4, PIN_MODE_INPUT_PULLDOWN);

    rt_pin_mode(GPIO0_A0, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(GPIO0_A2, PIN_MODE_INPUT_PULLDOWN);
    
    rt_pin_mode(GPIO0_A1, PIN_MODE_INPUT_PULLDOWN); // other pin (not use)
    rt_pin_mode(GPIO0_A5, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(GPIO0_B1, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(GPIO0_D7, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(GPIO1_B0, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(GPIO1_C0, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(GPIO0_D6, PIN_MODE_INPUT_PULLDOWN);
}

#else

static void hl_mod_pm_gpio_deal()
{
    // hl_hal_gpio_init(GPIO_USB_SW);// GPIO_USB_SW
    // hl_hal_gpio_low(GPIO_USB_SW); // 
    rt_pin_mode(GPIO1_A5, PIN_MODE_INPUT_PULLDOWN); // GPIO_USB_SW


    // hl_hal_gpio_init(GPIO_RF_PWR_EN);// GPIO_RF_PWR_EN
    // hl_hal_gpio_low(GPIO_RF_PWR_EN); // 
    rt_pin_mode(GPIO1_A7, PIN_MODE_INPUT_PULLDOWN); // GPIO_RF_PWR_EN


    // hl_hal_gpio_init(GPIO_AMP_EN);// 0b0 3.3V 
    // hl_hal_gpio_high(GPIO_AMP_EN); // 
    rt_pin_mode(GPIO1_C3, PIN_MODE_INPUT_PULLUP); // GPIO_AMP_EN

    // hl_hal_gpio_init(GPIO_PWR_EN);//  AMP EN 
    // hl_hal_gpio_low(GPIO_PWR_EN); // 
    rt_pin_mode(GPIO0_B0, PIN_MODE_INPUT_PULLDOWN); // GPIO_PWR_EN

    rt_pin_mode(GPIO0_A5, PIN_MODE_INPUT_PULLDOWN); // l_vol
    rt_pin_mode(GPIO0_A6, PIN_MODE_INPUT_PULLDOWN); //
    //_tgpio_output(0, GPIO0_A7, 3); // 0_A7 l_ok_key
    rt_pin_mode(GPIO0_A7, PIN_MODE_INPUT_PULLUP); //

    //rt_pin_mode(GPIO0_C6, PIN_MODE_OUTPUT); // r_ok_ker
    //_tgpio_output(0, GPIO0_C6, 3); // r_ok_ker
    rt_pin_mode(GPIO0_C6, PIN_MODE_INPUT_PULLUP); //

    rt_pin_mode(GPIO0_C4, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(GPIO0_C5, PIN_MODE_INPUT_PULLDOWN);

    //_tgpio_output(0, GPIO0_C1, 3);          //0c1 jackdet 输出高
    rt_pin_mode(GPIO0_C1, PIN_MODE_INPUT_PULLUP); //

    rt_pin_mode(GPIO0_D3, PIN_MODE_INPUT_PULLDOWN); // i2s

    rt_pin_mode(GPIO0_A4, PIN_MODE_INPUT_PULLDOWN); // i2s mclk

    rt_pin_mode(GPIO0_D4, PIN_MODE_INPUT_PULLDOWN); // i2s lfsync

    rt_pin_mode(GPIO0_D6, PIN_MODE_INPUT_PULLDOWN); // i2s sd0

    //_tgpio_output(0, GPIO0_C2, 3); // i2c 
    //_tgpio_output(0, GPIO0_C3, 3);// i2c 
    rt_pin_mode(GPIO0_C2, PIN_MODE_INPUT_PULLUP); //
    rt_pin_mode(GPIO0_C3, PIN_MODE_INPUT_PULLUP); //

    rt_pin_mode(GPIO1_B2, PIN_MODE_INPUT_PULLDOWN); // gsensor  int1

    // _tgpio_output(1, GPIO0_A2, 3); // i2c 
    // _tgpio_output(1, GPIO0_A3, 3);// i2c
    rt_pin_mode(GPIO1_A2, PIN_MODE_INPUT_PULLUP); //
    rt_pin_mode(GPIO1_A3, PIN_MODE_INPUT_PULLUP); // 

    rt_pin_mode(GPIO0_C0, PIN_MODE_INPUT_PULLDOWN); // oled

    rt_pin_mode(GPIO1_C0, PIN_MODE_INPUT_PULLDOWN); // oled

    rt_pin_mode(GPIO1_C1, PIN_MODE_INPUT_PULLDOWN); // oled

    rt_pin_mode(GPIO1_C2, PIN_MODE_INPUT_PULLDOWN); // oled

    

    rt_pin_mode(GPIO1_C4, PIN_MODE_INPUT_PULLDOWN); // oled spi

    rt_pin_mode(GPIO1_C5, PIN_MODE_INPUT_PULLDOWN); // oled spi
    
    rt_pin_mode(GPIO1_C6, PIN_MODE_INPUT_PULLDOWN); // oled spi

    rt_pin_mode(GPIO1_C7, PIN_MODE_INPUT_PULLDOWN); // oled spi

    // _tgpio_output(1, GPIO0_B1, 3); // power key 高电平
    rt_pin_mode(GPIO1_B1, PIN_MODE_INPUT_PULLUP); //

    rt_pin_mode(GPIO0_D1, PIN_MODE_INPUT_PULLDOWN); // uart1
    rt_pin_mode(GPIO0_D2, PIN_MODE_INPUT_PULLDOWN); // uart1

    rt_pin_mode(GPIO1_A4, PIN_MODE_INPUT_PULLDOWN); // vbusdet


    rt_pin_mode(GPIO1_A0, PIN_MODE_INPUT_PULLDOWN); // uart2 tl
    rt_pin_mode(GPIO1_A1, PIN_MODE_INPUT_PULLDOWN); // uart2 tl

    rt_pin_mode(GPIO0_A0, PIN_MODE_INPUT_PULLDOWN); // iss1 tl
    rt_pin_mode(GPIO0_A1, PIN_MODE_INPUT_PULLDOWN); // iss1 tl
    rt_pin_mode(GPIO0_A2, PIN_MODE_INPUT_PULLDOWN); // iss1 tl

    rt_pin_mode(GPIO1_B0, PIN_MODE_INPUT_PULLDOWN); // rst tl

    //_tgpio_output(1, GPIO0_A5, 0); // usb switch  低
    rt_pin_mode(GPIO1_A5, PIN_MODE_INPUT_PULLDOWN); //

    rt_pin_mode(GPIO0_A3, PIN_MODE_INPUT_PULLUP); // gauge

    rt_pin_mode(GPIO0_B1, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(GPIO0_D7, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(GPIO1_C2, PIN_MODE_INPUT_PULLDOWN);

}
#endif


uint8_t hl_drv_lowpower_io_ctrl(uint8_t cmd, void* ptr, uint8_t len)
{
    uint8_t i = 0;
    HL_CLOCK_INPUT_PARAM_T* param = (HL_CLOCK_INPUT_PARAM_T*)ptr;
    if(len >= e_MAX_NUM){
        rt_kprintf("Input led err : %d !\n",len);
        goto ERR;
    }
    if(cmd == INTO_LOWPOWER){
        hl_mod_pm_gpio_deal();
        if(clk_set_rate(clock_name[0].postion,25000000) != RT_EOK){
            rt_kprintf("Set PLL_GPLL clock frequency 25 MHz fail !\n");
            goto ERR;
        }
        if(clk_set_rate(clock_name[1].postion,25000000) != RT_EOK){
            rt_kprintf("Set PLL_CPLL clock frequency 25 MHz fail !\n");
            goto ERR;
        }
    }
    if(cmd == EXIT_LOWPOWER){
        if(clk_set_rate(clock_name[0].postion,1188000000) != RT_EOK){
            rt_kprintf("Set PLL_GPLL clock frequency 1188 MHz fail !\n");
            goto ERR;
        }
        if(clk_set_rate(clock_name[1].postion,1188000000) != RT_EOK){
            rt_kprintf("Set PLL_CPLL clock frequency 1188 MHz fail !\n");
            goto ERR;
        }
    }
    if(cmd == CTRL_FREQUENCY){
        for(i = 0;i<len;i++){
            param[i].fre = param[i].fre * 1000000;
            if(clk_set_rate(clock_name[param[i].cfg_opt].postion,param[i].fre) != RT_EOK){
                rt_kprintf("Set Freq fail : %d Hz!\n",param[i].fre);
                goto ERR;
            }
        }
    }
    return HL_SUCCESS;
ERR:
    return HL_FAILED;
}

#if 0
static int hl_drv_get_frequency_test(void)
{
    uint8_t i = 0 , j = 0;
    for(i=0;i<58;i++){
        clock_name[i].hz = clk_get_rate(clock_name[i].postion);
    }
    for(i = 0;i < 58;i++){
        rt_kprintf("%-25s",clock_name[i].name);
        if((i+1) % 5 == 0 && i != 0){
            rt_kprintf("\n");
            for(j = i - 4;j <= i;j++){
                rt_kprintf("%-25d",clock_name[j].hz);
            }
            rt_kprintf("\n\n");
        }
    }
    rt_kprintf("\n");
    rt_kprintf("%-25d",clock_name[55].hz);
    rt_kprintf("%-25d",clock_name[56].hz);
    rt_kprintf("%-25d",clock_name[57].hz);
    rt_kprintf("\n\n\n");
    return RT_EOK;
}

static int hl_drv_set_frequency_test(int argc, char** argv)
{
    uint8_t i = 0;
    uint32_t option = 0, param = 0;
    option = atoi(argv[1]);
    param = atoi(argv[2]) * 1000000;

    if(clk_set_rate(clock_name[option].postion,param) != RT_EOK){
        rt_kprintf("Set Freq fail !\n");
    }

    clock_name[option].hz = clk_get_rate(clock_name[option].postion);

    rt_kprintf("Set %s = %d\n",clock_name[option].name,clock_name[option].hz);

    return RT_EOK;
}

static int hl_drv_set_lopwer_mod_test(int argc, char** argv)
{
    hl_mod_pm_gpio_deal();
    return RT_EOK;
}

MSH_CMD_EXPORT(hl_drv_set_lopwer_mod_test, run hl_drv_set_lopwer_mod_test);
MSH_CMD_EXPORT(hl_drv_set_frequency_test, run hl_drv_set_frequency_test);
MSH_CMD_EXPORT(hl_drv_get_frequency_test, run hl_drv_get_frequency_test);

#endif
/* Exported functions --------------------------------------------------------*/
/*
 * EOF
 */