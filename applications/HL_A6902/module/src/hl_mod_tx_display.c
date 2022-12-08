#include "hl_mod_display.h"
#if HL_IS_TX_DEVICE()
// static int hl_mod_lvgl_init(void)
// {
//     uint8_t i = 0;
//     rt_kprintf("lvgl_init \n");
//     lv_init();
//     // lv_port_disp_init();
//     lv_port_indev_init();
//     // lv_user_gui_init();

//     /* handle the tasks of LVGL */
//     while(i<20)
//     {
//         i++;
//         rt_kprintf("lvgl_init : LV_COLOR_MIX_ROUND_OFS = %d\n",LV_COLOR_MIX_ROUND_OFS);
//         lv_task_handler();
//         rt_thread_mdelay(LV_DISP_DEF_REFR_PERIOD);
//     }
//     return 0;
// }

// MSH_CMD_EXPORT(hl_mod_lvgl_init, run lvgl_mod_init);


uint8_t hl_mod_display_init(void* display_msq)
{
    return 0;
}

/**
 * @brief 显示模块反初始化
 * @return uint8_t 结果 成功：HL_DISPLAY_SUCCESS 失败：失败：HL_DISPLAY_FAILED
 * @date 2022-11-15
 * @author liujie (jie.liu@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-15      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_mod_display_deinit(void)
{
    return 0;
}

/**
 * @brief 显示模块控制命令，包括，LED控制，OLED全屏显示颜色等
 * @param [in] cmd 命令：见<hl_cmd_e>
 * @param [in] ptr 参数的指针，使用具体见命令的注释
 * @param [in] len 长度，具体见命令的注释
 * @return uint8_t 
 * @date 2022-11-15
 * @author liujie (jie.liu@hollyland-tech.com)
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-11-15      <td>liujie     <td>新建
 * </table>
 */
uint8_t hl_mod_display_io_ctrl(uint8_t cmd, void* ptr, uint16_t len)
{
    return 0;
}


#endif