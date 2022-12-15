/**
 * @file hl_util_nvram_test.c
 * @author yangxianyun (rd52@hollyland-tech.com)
 * @brief 使用hl_util_nvram工具的示例文件，待完善
 * @version 0.1
 * @date 2022-08-15
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-08-15     <td>v1.0     <td>yangxianyun     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "hl_util_nvram.h"
#include "rtthread.h"
#include <drivers/mtd_nor.h>
#include "hal_base.h"
#include "drv_snor.h"
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/

#define HL_UTIL_SNOR_NVRAM_SIZE 4096
#define HL_UTIL_NVRAM_JSON_ADDR 0x120000

#if 0
/* variables -----------------------------------------------------------------*/
static struct rt_mtd_nor_device* snor_device = RT_NULL;
static uint8_t*                  snor_buf    = NULL;
static rt_mutex_t                nvram_mutex = RT_NULL;
/* Private function(only *.c)  -----------------------------------------------*/

static uint8_t _hl_util_nvram_test_init_snor()
{
    snor_device = (struct rt_mtd_nor_device*)rt_device_find("snor");
    if (snor_device == RT_NULL) {
        rt_kprintf("Did not find device: snor....\n");
        return 1;
    }

    snor_buf = (uint8_t*)rt_malloc_align(HL_UTIL_SNOR_NVRAM_SIZE, 64);

    return 0;
}

static uint8_t _hl_util_nvram_test_nvram_read(char* data, uint16_t* len)
{
    uint8_t read_have_data = 1;
    if (NULL == data || NULL == len) {
        return 1;
    }

    uint32_t read_len = 4096;

    *len = rt_mtd_nor_read(snor_device, HL_UTIL_NVRAM_JSON_ADDR, data, read_len);

    return 0;
}

static uint8_t _hl_util_nvram_test_nvram_write(char* data, uint16_t len)
{
    uint8_t  read_have_data = 1;
    uint32_t write_size     = 0;
    if (NULL == data || 0 == len) {
        return 1;
    }

    rt_mtd_nor_erase_block(snor_device, HL_UTIL_NVRAM_JSON_ADDR, len);
    write_size = rt_mtd_nor_write(snor_device, HL_UTIL_NVRAM_JSON_ADDR, data, len);

    if (write_size <= 0) {
        return 2;
    }

    return 0;
}

static uint8_t _hl_util_nvram_test_nvram_mutex_take()
{
    if (nvram_mutex) {
        rt_mutex_take(nvram_mutex, RT_WAITING_FOREVER);
    }

    return 0;
}

static uint8_t _hl_util_nvram_test_nvram_mutex_release()
{
    if (nvram_mutex) {
        rt_mutex_release(nvram_mutex);
    }

    return 0;
}
/* Exported functions --------------------------------------------------------*/

uint8_t hl_nvram_test(int argc, char** argv)
{
#if 1
    int     ret             = 0;
    uint8_t i               = 0;
    uint8_t get_buffer[100] = { 0 };
    if (argc > 1) {
        if (!strcmp("init", argv[1])) {

            _hl_util_nvram_test_init_snor();
            ret =
                hl_util_nvram_param_init(rt_kprintf, _hl_util_nvram_test_nvram_write, _hl_util_nvram_test_nvram_read,
                                         _hl_util_nvram_test_nvram_mutex_take, _hl_util_nvram_test_nvram_mutex_release);
            nvram_mutex = rt_mutex_create("nvram_mutex", RT_IPC_FLAG_FIFO);
            rt_kprintf("init finish\r\n");

        } else if (!strcmp("get", argv[1])) {

            if (argc < 3) {
                rt_kprintf("wrong parameter, please type: hl_nvram_test get keyname\r\n");
                return 0;
            }
            ret = hl_util_nvram_param_get(argv[2], get_buffer, "0", sizeof(get_buffer));
            if (ret) {
                rt_kprintf("paramater %s not exist ret = %d\r\n", argv[2], ret);
            } else {
                rt_kprintf("paramater %s value = %s\r\n", argv[2], get_buffer);
                for (i = 0; i < strlen(get_buffer); i++) {
                    rt_kprintf("%d ", get_buffer[i]);
                }
            }
            rt_kprintf("get finish\r\n");
            return 0;

        } else if (!strcmp("set", argv[1])) {

            if (argc < 4) {
                rt_kprintf("wrong parameter, please type: hl_nvram_test set keyname valu_str\r\n");
                return 0;
            }
            ret = hl_util_nvram_param_set(argv[2], argv[3]);
            rt_kprintf("set ret = %d\r\n", ret);
            return 0;

        } else if (!strcmp("save", argv[1])) {

            ret = hl_util_nvram_param_save();
            rt_kprintf("save ret = %d\r\n", ret);
            return 0;

        } else {
            rt_kprintf("wrong parameter, please type: hl_nvram_test [init/get keyname/set keyname valu_str/save]\r\n");
            return 0;
        }
    } else {
        rt_kprintf("wrong parameter, please type: hl_nvram_test [init/get keyname/set keyname valu_str/save]\r\n");
    }
#else
    uint8_t  datatest[4096] = { 0 };
    uint8_t  data_temp[129] = { 0 };
    uint8_t  i              = 0;
    uint16_t len            = 0;
    int      ret            = _hl_util_nvram_test_nvram_read(datatest, &len);
    rt_kprintf(" ret = %d\r\n", ret);

    for (i = 0; i < (4096 / 128); i++) {
        memcpy(data_temp, &datatest[128 * i], 128);
        rt_kprintf(" %s\r\n", data_temp);
    }
#endif
    return 0;
}

MSH_CMD_EXPORT(hl_nvram_test, hl_nvram_test);

#endif
/*
 * EOF
 */