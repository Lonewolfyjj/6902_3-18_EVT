/**
 * @file hl_mod_page.h
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-12-05
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
 * <tr><td>2022-12-05     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_MOD_PAGE_H__
#define __HL_MOD_PAGE_H__
/* Includes ------------------------------------------------------------------*/
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

// void hl_mod_page_home_init(void);
// void hl_mod_page_menu_init(void);
// void hl_mod_page_txconf_init(void);

PAGE_DEC(PAGE_HOME);
PAGE_DEC(PAGE_MAIN_MENU);

PAGE_DEC(PAGE_AUTO_POWEROFF);
PAGE_DEC(PAGE_AUTO_RECORD);
PAGE_DEC(PAGE_FAST_TX_CONFIG);
PAGE_DEC(PAGE_TX_LED_BRIGHT);
PAGE_DEC(PAGE_TX_LOW_CUT);
PAGE_DEC(PAGE_MONITOR_SET);
PAGE_DEC(PAGE_NOISE_REDUCTION_INTENSITY);
PAGE_DEC(PAGE_OTHER_SET);
PAGE_DEC(PAGE_PAIR);
PAGE_DEC(PAGE_RECORD_FORMAT);
PAGE_DEC(PAGE_RECORD_PROTECT);
PAGE_DEC(PAGE_RESTORE);
PAGE_DEC(PAGE_SOUND_EFFECT_MODE);
PAGE_DEC(PAGE_SOUND_MODULE);
PAGE_DEC(PAGE_SYS_TIME_SET);
PAGE_DEC(PAGE_TX_CONF_MENU);
PAGE_DEC(PAGE_TX_GAIN_CONF);
PAGE_DEC(PAGE_VERSION);
PAGE_DEC(PAGE_VOLUME_MENU);
PAGE_DEC(PAGE_MONITOR_VOLUME_SET);
// PAGE_DEC(PAGE_UACIN_VOLUME_SET);
PAGE_DEC(PAGE_UACOUT_VOLUME_SET);
PAGE_DEC(PAGE_QUICK_SETTINGS);
PAGE_DEC(PAGE_LINE_OUT_STEREO_LEFT);
PAGE_DEC(PAGE_LINE_OUT_STEREO_RIGHT);
PAGE_DEC(PAGE_TX_GAIN_TX1);
PAGE_DEC(PAGE_TX_GAIN_TX2);
PAGE_DEC(PAGE_BOX_IN);
PAGE_DEC(PAGE_LOGO);
PAGE_DEC(PAGE_POWEROFF_CHARGE);
PAGE_DEC(PAGE_PARING);
PAGE_DEC(PAGE_UPGRADE);
PAGE_DEC(PAGE_FAST_TX2_CONFIG);
PAGE_DEC(PAGE_FORMAT);
PAGE_DEC(PAGE_SOUNDOUT_SETTING);
PAGE_DEC(PAGE_OPEN_MSC);

#endif /*__HL_MOD_PAGE_H__*/
/*
 * EOF
 */