/**
 * @file hl_board_commom.c
 * @author yangxianyun (rd52@hollyland-tech.com)
 * @brief Hollyland 的板子通用初始化
 * @version 0.1
 * @date 2022-12-19
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
 * <tr><td>2022-12-19     <td>v1.0     <td>yangxianyun     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_BOARD_COMMOM_H__
#define __HL_BOARD_COMMOM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hl_util_nvram.h"
#include <drivers/mtd_nor.h>
#include "hal_base.h"
#include "drv_snor.h"
#include "finsh.h"
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
/// NVRAM的地址，具体值为setting.ini中nvran的 （区块地址*0x200）
#define HL_BOARD_NVRAM_JSON_ADDR 0x2E0000
/* variables -----------------------------------------------------------------*/
static struct rt_mtd_nor_device* snor_device = RT_NULL;
static rt_mutex_t                nvram_mutex = RT_NULL;
static sg_nvram_inint_flag = 0;

/* Private function(only *.c)  -----------------------------------------------*/

/**
 * 
 * @brief nvram的flash初始化，本板子的nvram是通过flash来达成非易失特性的
 * @return uint8_t 0：正常初始化 | 非0：异常初始化
 * @date 2022-12-19
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-12-19      <td>yangxianyun     <td>新建
 * </table>
 */
static uint8_t _hl_board_nvram_init_snor()
{
    snor_device = (struct rt_mtd_nor_device*)rt_device_find("snor");
    if (snor_device == RT_NULL) {
        rt_kprintf("Did not find device: snor....\n");
        return 1;
    }

    return 0;
}

/**
 * 
 * @brief nvram的非易失读取函数
 * @param [in] data 读取的缓存地址
 * @param [in] len 读取的数据长度
 * @return uint8_t 0：读取成功 | 非0：读取异常
 * @date 2022-12-19
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-12-19      <td>yangxianyun     <td>新建
 * </table>
 */
static uint8_t _hl_board_nvram_read(char* data, uint16_t* len)
{
    uint8_t read_have_data = 1;
    if (NULL == data || NULL == len) {
        return 1;
    }

    uint32_t read_len = 4096;

    *len = rt_mtd_nor_read(snor_device, HL_BOARD_NVRAM_JSON_ADDR, data, read_len);

    return 0;
}

/**
 * 
 * @brief nvram的非易失写入函数
 * @param [in] data 写入的缓存地址
 * @param [in] len 写入的数据长度
 * @return uint8_t  0：写入成功 | 非0：写入异常
 * @date 2022-12-19
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-12-19      <td>yangxianyun     <td>新建
 * </table>
 */
static uint8_t _hl_board_nvram_write(char* data, uint16_t len)
{
    uint8_t  read_have_data = 1;
    uint32_t write_size     = 0;
    if (NULL == data || 0 == len) {
        return 1;
    }

    rt_mtd_nor_erase_block(snor_device, HL_BOARD_NVRAM_JSON_ADDR, len);
    write_size = rt_mtd_nor_write(snor_device, HL_BOARD_NVRAM_JSON_ADDR, data, len);

    if (write_size <= 0) {
        return 2;
    }

    return 0;
}

/**
 * 
 * @brief nvram的操作互斥锁获取
 * @return uint8_t 0：获取成功 | 非0：获取失败
 * @date 2022-12-19
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-12-19      <td>yangxianyun     <td>新建
 * </table>
 */
static uint8_t _hl_board_nvram_mutex_take()
{
    if (nvram_mutex) {
        rt_mutex_take(nvram_mutex, RT_WAITING_FOREVER);
    }

    return 0;
}

/**
 * 
 * @brief nvram的操作互斥锁释放
 * @return uint8_t 0：释放成功 | 非0：释放失败
 * @date 2022-12-19
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-12-19      <td>yangxianyun     <td>新建
 * </table>
 */
static uint8_t _hl_board_nvram_mutex_release()
{
    if (nvram_mutex) {
        rt_mutex_release(nvram_mutex);
    }

    return 0;
}

/**
 * 
 * @brief nvram获取参数的值
 * @param [in] argc 
 * @param [in] argv 
 * @return int 
 * @date 2022-12-19
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-12-19      <td>yangxianyun     <td>新建
 * </table>
 */
static int _hl_board_nvram_get_value(int argc, char **argv)
{
    if (argc < 1) {
        rt_kprintf("too few args\r\n");
        return 1;
    }

    char data[128] = {0};
    hl_util_nvram_param_get(argv[1], data, data, sizeof(data));
    rt_kprintf("%s:%s\r\n", argv[1], data);
    return 0;
}
MSH_CMD_EXPORT(_hl_board_nvram_get_value, _hl_board_nvram_get_value param);

/**
 * 
 * @brief nvram写入参数的值
 * @param [in] argc 
 * @param [in] argv 
 * @return int 
 * @date 2022-12-19
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-12-19      <td>yangxianyun     <td>新建
 * </table>
 */
static int _hl_board_nvram_set_value(int argc, char **argv)
{
    if (argc < 3) {
        rt_kprintf("too few args\r\n");
    }

    hl_util_nvram_param_set(argv[1], argv[2]);
    rt_kprintf("%s:%s\r\n", argv[1], argv[2]);
    hl_util_nvram_param_save();
    return 0;
}
MSH_CMD_EXPORT(_hl_board_nvram_set_value, _hl_board_nvram_set_value param value);
/* Exported functions --------------------------------------------------------*/

/**
 * 
 * @brief nvram的板子初始化函数，在本板子（RK2108D）中，需要在USB枚举前进行初始化来获取USB的SN号，所以该函数通过extern来在hid.c的create方法中调用，请注意
 * @return uint8_t 0：正常初始化 | 非0：异常初始化
 * @date 2022-12-19
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-12-19      <td>yangxianyun     <td>新建
 * </table>
 */
uint8_t hl_board_nvram_init()
{
    if (!sg_nvram_inint_flag) {
        _hl_board_nvram_init_snor();
        nvram_mutex = rt_mutex_create("nvram_mutex", RT_IPC_FLAG_FIFO);
        hl_util_nvram_param_init(rt_kprintf, _hl_board_nvram_write, _hl_board_nvram_read, _hl_board_nvram_mutex_take,
                                _hl_board_nvram_mutex_release);
        sg_nvram_inint_flag = 1;
    }
    return 0;
}

#ifdef __cplusplus
}
#endif
#endif  //__HL_BOARD_COMMOM_H__
/*
 * EOF
 */
