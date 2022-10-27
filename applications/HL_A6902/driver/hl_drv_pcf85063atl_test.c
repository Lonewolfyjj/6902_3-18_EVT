/**
 * @file hl_drv_pcf85063atl_test.c
 * @author libo (rd46@hollyland-tech.com)
 * @brief 
 * @version 1.0
 * @date 2022-09-19
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-19     <td>v1.0     <td>libo           <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/*
 * EOF
 */
#include <string.h>
#include <rtdevice.h>
#include"hl_drv_pcf85063atl.h"



/**
 * 
 * @brief demo
 * @param [in] argc 
 * @param [in] argv 
 * @date 2022-09-22
 * @author libo (rd46@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-22       <td>libo           <td>新建
 * </table>
 */
void hl_drv_pcf85063atl_export(int argc, char** argv)
{
    rtc_time time;
    memset(&time, 0, sizeof(rtc_time));

    hl_drv_rtc_pcf85063_init();
    rt_kprintf(" argc :%d argv[0]: %s\n", argc, argv[1]);
    if (argc == 8) {
        if (!strcmp("set", argv[1])) {
            time.year   = atoi(argv[2]);
            time.month  = atoi(argv[3]);
            time.day    = atoi(argv[4]);
            time.hour   = atoi(argv[5]);
            time.minute = atoi(argv[6]);
            time.second = atoi(argv[7]);
            hl_drv_rtc_pcf85063_io_ctrl(RTC_SET_TIME, (void*)&time, sizeof(rtc_time));
        } else {
            rt_kprintf(" parm1 no set!\n");
        }
    }

    if (argc == 2) {
        if (!strcmp("get", argv[1])) {
            hl_drv_rtc_pcf85063_io_ctrl(RTC_GET_TIME, (void*)&time, sizeof(rtc_time));
            /*时间存储格式是按 BCD 码的格式来存储的。如果需要使用，需要转换成十六进制之后才能使用，时间年的范围：0~99，需要注意*/
            rt_kprintf(" year:   %d\n", time.year);
            rt_kprintf(" month:  %d\n", time.month&0x1f);
            rt_kprintf(" day:    %d\n", time.day&0x3f);
            rt_kprintf(" hour:   %d\n", time.hour&0x3f);
            rt_kprintf(" minute: %d\n", time.minute&0x7f);
            rt_kprintf(" second: %d\n", time.second&0x7f);
        } else {
            rt_kprintf(" parm1 no  get!\n");
        }
    }
}

MSH_CMD_EXPORT(hl_drv_pcf85063atl_export, parm1 : set/get parm2-6: year month day hour minute);