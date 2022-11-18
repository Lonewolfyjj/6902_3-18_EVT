/**
 * @file hl_drv_coreclock_cfg.h
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
#ifndef __HL_DRV_CORECLOCK_CFG_H
#define __HL_DRV_CORECLOCK_CFG_H
/* Includes ------------------------------------------------------------------*/
/* typedef -------------------------------------------------------------------*/
enum eclock_t
{
    e_PLL_GPLL = 0,
    e_PLL_CPLL,
    e_ACLK_DSP,
    e_PCLK_DSP,
    e_SCLK_SHRM,
    e_PCLK_SHRM,
    e_CLK_UART0_SRC,
    e_CLK_UART0_FRAC,
    e_CLK_UART0,
    e_CLK_UART1_SRC,
    e_CLK_UART1_FRAC,
    e_CLK_UART1,
    e_CLK_UART2_SRC,
    e_CLK_UART2_FRAC,
    e_CLK_UART2,
    e_CLK_I2C0,
    e_CLK_I2C1,
    e_CLK_I2C2,
    e_MCLK_PDM0,
    e_HCLK_AUDIO,
    e_CLK_I2S8CH_SRC,
    e_CLK_I2S8CH_FRAC,
    e_MCLK_I2S8CH,
    e_I2S_MCLKOUT,
    e_ACLK_DMAC,
    e_PCLK_AUDIO,
    e_ACLK_VOP,
    e_DCLK_VOP_S,
    e_CLK_GPIO_DBG0,
    e_CLK_GPIO_DBG1,
    e_PCLK_ALIVE,
    e_HCLK_ALIVE,
    e_CLK_TESTOUT,
    e_CLK_PDM_SAMP,
    e_HCLK_M4,
    e_CLK_SPI1,
    e_SCLK_SFC_SRC,
    e_CLK_SPI2,
    e_CLK_SDIO_SRC,
    e_CLK_AUDPWM_SRC,
    e_CLK_AUDPWM_FRAC,
    e_CLK_AUDPWM,
    e_CLK_I2S1_8CH_SRC,
    e_CLK_I2S1_8CH_FRAC,
    e_MCLK_I2S1_8CH,
    e_I2S1_MCLKOUT,
    e_CLK_PWM,
    e_HCLK_LOGIC,
    e_PCLK_LOGIC,
    e_ACLK_LOGIC,
    e_CLK_CIFOUT,
    e_CLK_USB2PHY_REF_FRAC,
    e_CLK_USB2PHY_REF,
    e_CLK_DPHY_REF_FRAC,
    e_CLK_DPHY_REF,
    e_SCLK_SFC1_SRC,
    e_CLK_32K,
    e_OCC_SCAN_CLK_DPHYLANBYTE,
    e_MAX_NUM
};


typedef struct _HL_CLOCK_INPUT_PARAM_T
{
    enum eclock_t cfg_opt;
    uint32_t fre; //频率，单位 ： MHz
}HL_CLOCK_INPUT_PARAM_T;
/* define --------------------------------------------------------------------*/

/* 函数返回状态 */
#define HL_SUCCESS 0
#define HL_FAILED 1

/*  操作命令 */
#define INTO_LOWPOWER 0
#define EXIT_LOWPOWER 1
#define CTRL_FREQUENCY 2

/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
uint8_t hl_drv_lowpower_io_ctrl(uint8_t cmd, void* ptr, uint8_t len);
/* Exported functions --------------------------------------------------------*/
/*
 * EOF
 */
#endif