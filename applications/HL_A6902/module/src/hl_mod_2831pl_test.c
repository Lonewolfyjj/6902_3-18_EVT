/**
 * @file hl_mod_2831pl_test.c
 * @author lilin (lin.li@hollyland-tech.com)
 * @brief 
 * @version 1.0
 * @date 2022-09-15
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-15     <td>v1.0     <td>lilin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/

#include "hl_mod_2831pl.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
int hl_mod_2831pl_test_init(void)
{
    hl_mod_2831pl_init(NULL);
    return 0;
}

static void uart_cmd_pair_s(void)
{
    hl_mod_2831pl_wireless_pair_e arg = PL_WIRELESS_PAIR_START;

    hl_mod_2831pl_ctrl(HL_MOD_PL_WIRELESS_PAIR, &arg, sizeof(arg));
}

static void uart_cmd_pair_p(void)
{
    hl_mod_2831pl_wireless_pair_e arg = PL_WIRELESS_PAIR_STOP;

    hl_mod_2831pl_ctrl(HL_MOD_PL_WIRELESS_PAIR, &arg, sizeof(arg));
}

static void uart_cmd_slave(void)
{
    hl_mod_2831pl_work_mode_e arg = PL_WORK_MODE_SLAVE;

    hl_mod_2831pl_ctrl(HL_MOD_PL_SWITCH_SLAVE_MASTER, &arg, sizeof(arg));
}

static void uart_cmd_master(void)
{
    hl_mod_2831pl_work_mode_e arg = PL_WORK_MODE_MASTER;

    hl_mod_2831pl_ctrl(HL_MOD_PL_SWITCH_SLAVE_MASTER, &arg, sizeof(arg));
}

MSH_CMD_EXPORT(hl_mod_2831pl_test_init, init 2831pl test mode);

MSH_CMD_EXPORT(uart_cmd_pair_s, start wireless pair);

MSH_CMD_EXPORT(uart_cmd_pair_p, stop wireless pair);

MSH_CMD_EXPORT(uart_cmd_slave, switch slave mode);

MSH_CMD_EXPORT(uart_cmd_master, switch master mode);

/*
 * EOF
 */