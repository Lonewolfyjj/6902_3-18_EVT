// /**
//  * @file lvgl2rtt.h
//  * @author liujie (jie.liu@hollyland-tech.com)
//  * @brief 
//  * @version V1.0
//  * @date 2022-10-23
//  * 
//  * ██╗  ██╗ ██████╗ ██╗     ██╗  ██╗   ██╗██╗      █████╗ ███╗   ██╗██████╗ 
//  * ██║  ██║██╔═══██╗██║     ██║  ╚██╗ ██╔╝██║     ██╔══██╗████╗  ██║██╔══██╗
//  * ███████║██║   ██║██║     ██║   ╚████╔╝ ██║     ███████║██╔██╗ ██║██║  ██║
//  * ██╔══██║██║   ██║██║     ██║    ╚██╔╝  ██║     ██╔══██║██║╚██╗██║██║  ██║
//  * ██║  ██║╚██████╔╝███████╗███████╗██║   ███████╗██║  ██║██║ ╚████║██████╔╝
//  * ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝
//  * @copyright Copyright (c) 2022 hollyland
//  * 
//  * @par 修改日志:
//  * <table>
//  * <tr><th>Date           <th>Version  <th>Author         <th>Description
//  * <tr><td>2022-10-23     <td>v1.0     <td>liujie     <td>内容
//  * </table>
//  * 
//  */ 
// /* Define to prevent recursive inclusion -------------------------------------*/

// #ifndef __LVGL2RTT_H__
// #define __LVGL2RTT_H__


// /* Includes ------------------------------------------------------------------*/
// /* typedef -------------------------------------------------------------------*/
// typedef void (*lv_rtt_disp_init)(void);
// typedef void (*lv_rtt_indev_init)(void);
// typedef void (*lv_rtt_user_gui_init)(void);
// /* define --------------------------------------------------------------------*/




// /*=========================
//    MEMORY SETTINGS
//  *=========================*/



// /*====================
//    HAL SETTINGS
//  *====================*/




// /* variables -----------------------------------------------------------------*/
// /* Private function(only *.c)  -----------------------------------------------*/
// /* Exported functions --------------------------------------------------------*/

// /**
//  * @brief 注册显示初始化回调函数
//  * @param [in] func 
//  * @date 2022-10-23
//  * @author liujie (jie.liu@hollyland-tech.com)
//  * @details 
//  * @note 
//  * @par 修改日志:
//  * <table>
//  * <tr><th>Date             <th>Author         <th>Description
//  * <tr><td>2022-10-23      <td>liujie     <td>新建
//  * </table>
//  */
// void lvgl2rtt_port_disp_reg(lv_rtt_disp_init func);

// /**
//  * @brief 注册输入初始化回调函数
//  * @param [in] func 
//  * @date 2022-10-23
//  * @author liujie (jie.liu@hollyland-tech.com)
//  * @details 
//  * @note 
//  * @par 修改日志:
//  * <table>
//  * <tr><th>Date             <th>Author         <th>Description
//  * <tr><td>2022-10-23      <td>liujie     <td>新建
//  * </table>
//  */

// void lvgl2rtt_port_indev_reg(lv_rtt_indev_init func);
// /**
//  * @brief lvgl线程初始化
//  * @return uint8_t 
//  * @date 2022-10-23
//  * @author liujie (jie.liu@hollyland-tech.com)
//  * @details 
//  * @note 
//  * @par 修改日志:
//  * <table>
//  * <tr><th>Date             <th>Author         <th>Description
//  * <tr><td>2022-10-23      <td>liujie     <td>新建
//  * </table>
//  */

// /**
//  * @brief 注册用户界面回调函数
//  * @param [in] func 
//  * @date 2022-10-23
//  * @author liujie (jie.liu@hollyland-tech.com)
//  * @details 
//  * @note 
//  * @par 修改日志:
//  * <table>
//  * <tr><th>Date             <th>Author         <th>Description
//  * <tr><td>2022-10-23      <td>liujie     <td>新建
//  * </table>
//  */
// void lvgl2rtt_port_usergui_reg(lv_rtt_user_gui_init func);

// /**
//  * @brief 注册初始化用户注册界面回调函数
//  * @date 2022-10-23
//  * @author liujie (jie.liu@hollyland-tech.com)
//  * @details 
//  * @note 
//  * @par 修改日志:
//  * <table>
//  * <tr><th>Date             <th>Author         <th>Description
//  * <tr><td>2022-10-23      <td>liujie     <td>新建
//  * </table>
//  */
// void lvgl2rtt_port_usergui_reg();
// int hl_lvgl2thread_init(void);

// /**
//  * @brief lvgl线程反初始化
//  * @return uint8_t 
//  * @date 2022-10-23
//  * @author liujie (jie.liu@hollyland-tech.com)
//  * @details 
//  * @note 
//  * @par 修改日志:
//  * <table>
//  * <tr><th>Date             <th>Author         <th>Description
//  * <tr><td>2022-10-23      <td>liujie     <td>新建
//  * </table>
//  */
// int hl_lvgl2thread_deinit(void);


// #endif /*LV_CONF_H*/
// /*
//  * EOF
//  */