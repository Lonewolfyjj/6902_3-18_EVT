/**
 * @file hl_util_nvram.h
 * @author yangxianyun (rd52@hollyland-tech.com)
 * @brief NVRam功能的头文件
 *      该模块主要功能有：
 *          1. 从NVRAM（非易失性内存：在单片机中可以是Flash/EMMC，在Linux中可以使用文件系统中的文件）中读取和写入参数值
 *          2. 可以根据键值对获取一些参数的内容
 *          3. 可以动态地更改参数的值
 *          4. 可以保存键值对参数内容
 * 
 *      该工具依赖了[cJSON](https://github.com/DaveGamble/cJSON)来完成功能，该工具完全开源、免费闭源商用
 * @version 0.1
 * @date 2022-08-11
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-08-11     <td>v1.0     <td>yangxianyun     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HL_UTIL_NVRAM_H__
#define __HL_UTIL_NVRAM_H__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* typedef -------------------------------------------------------------------*/
typedef enum _hl_util_nvram_e_
{
    /// 默认值，未初始化
    HL_EM_UTIL_NVRAM_NOT_INIT = 0x00,
    /// 已经初始化
    HL_EM_UTIL_NVRAM_INITIALIZED,
    /// 未检测到cJSON文件
    HL_EM_UTIL_NVRAM_NOT_HAVE_cJSON,
    /// 正常运行中
    HL_EM_UTIL_NVRAM_NORMAL,
} hl_util_nvram_e;

/// @brief
typedef struct _hl_util_nvram_t_
{
    /// 是否初始化
    uint8_t enable;
    /// 当前状态
    uint8_t status;
    /// 是否使用set 设置了新的参数
    uint8_t have_changed;
    /// 标准输出函数，用于打印数据
    void (*std_printf)(const char* fmt, ...);
    /**
     * @brief 外部实现的非易失性内存（flash/emmc）的写入方法，需要实现根据参数的具体内容写入到具体位置的功能
     *      该函数主要实现
     *          1. 健壮性检查
     *          2. 将给定的字符串写入到flash/emmc中
     * @date 2022-08-16
     * @author yangxianyun (rd52@hollyland-tech.com)
     * 
     * @details 
     * @note 
     */
    uint8_t (*nvram_write)(char* data, uint16_t len);

    /**
     * 
     * @brief 外部实现的非易失性内存（flash/emmc）的读取方法，需要实现根据参数的具体内容读取flash/emmc内容到具体位置的功能
     * @date 2022-08-16
     * @author yangxianyun (rd52@hollyland-tech.com)
     * 
     * @details 
     * @note 
     */
    uint8_t (*nvram_read)(char* data, uint16_t* len);

    /**
     * 
     * @brief 外部实现NVRam需要的互斥锁获取
     * @date 2022-12-08
     * @author yangxianyun (rd52@hollyland-tech.com)
     * 
     * @details 
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2022-12-08      <td>yangxianyun     <td>新建
     * </table>
     */
    uint8_t (*nvram_mutex_take)();

    /**
     * 
     * @brief 外部实现的NVRam的互斥锁释放
     * @date 2022-12-08
     * @author yangxianyun (rd52@hollyland-tech.com)
     * 
     * @details 
     * @note 
     * @par 修改日志:
     * <table>
     * <tr><th>Date             <th>Author         <th>Description
     * <tr><td>2022-12-08      <td>yangxianyun     <td>新建
     * </table>
     */
    uint8_t (*nvram_mutex_release)();
} hl_util_nvram_t, *hl_util_nvram_t_p;

/* define --------------------------------------------------------------------*/

#define HL_UTIL_NVRAM_ITEM_LEN_LIMIT (128)
/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * hl_util_nvram_param_init
 * @brief 初始化函数，用于在系统启动时进行初始化相关内容
 *      该函数主要功能：
 *          1. 申请资源
 *          2. 读取cJSON字符串生成句柄
 *          3. 将状态配置
 * @param [in] std_printf 注册的标准打印输出函数
 * @return uint8_t 初始化初始化状态 0：初始化成功 | 非0：初始化失败
 * @date 2022-08-15
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-15      <td>yangxianyun     <td>新建
 * </table>
 */
uint8_t hl_util_nvram_param_init(void (*std_printf)(const char* fmt, ...),
                                 uint8_t (*nvram_write)(char* data, uint16_t len),
                                 uint8_t (*nvram_read)(char* data, uint16_t* len), uint8_t (*nvram_mutex_take)(),
                                 uint8_t (*nvram_mutex_release)());

/**
 * hl_util_nvram_param_deinit
 * @brief 去初始化函数，用清除初始化时申请的资源
 * @return uint8_t 去初始化结果 0：成功 | 非0：失败
 * @date 2022-08-15
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-15      <td>yangxianyun     <td>新建
 * </table>
 */
uint8_t hl_util_nvram_param_deinit();

/**
 * hl_util_nvram_param_get
 * @brief 获取参数，返回的是字符串结果
 * @param [in] param_key 欲获取的参数名
 * @param [out] param_value 获取的参数的值的保存地址
 * @param [in] default_value 获取失败时的默认参数
 * @return uint8_t 获取状态： 0：获取成功 | 非0：获取失败
 * @date 2022-08-15
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-15      <td>yangxianyun     <td>新建
 * </table>
 */
uint8_t hl_util_nvram_param_get(char* param_key, char* param_value, char* default_value, uint16_t value_len);

/**
 * hl_util_nvram_param_get_integer
 * @brief 获取参数，返回的是数值结果
 * @param [in] param_key 欲获取的参数名
 * @param [out] param_value 获取的参数的值的保存地址
 * @param [in] default_value 获取失败时的默认参数
 * @param [in] value_len 字符串长度
 * @return uint8_t 获取状态： 0：获取成功 | 非0：获取失败
 * @date 2022-08-15
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-15      <td>yangxianyun     <td>新建
 * </table>
 */
uint8_t hl_util_nvram_param_get_integer(char* param_key, int* param_value, int default_value);

/**
 * hl_util_nvram_param_set
 * @brief 写入参数值的函数
 * @param [in] param_key 写入的参数名
 * @param [in] param_value 写入的参数的内容
 * @param [in] value_len 字符串长度
 * @return uint8_t 写入状态 0：成功 | 非0：失败
 * @date 2022-08-15
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-15      <td>yangxianyun     <td>新建
 * </table>
 */
uint8_t hl_util_nvram_param_set(char* param_key, char* param_value);

/**
 * hl_util_nvram_param_set_integer
 * @brief 获取数值参数，返回的是数值结果
 * @param [in] param_key 写入的参数名
 * @param [in] integer_value 写入的参数的值的保存地址
 * @return uint8_t 写入状态 0：成功 | 非0：失败 
 * @date 2023-02-07
 * @author lixiang (rd37@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2023-02-07      <td>lixiang     <td>新建
 * </table>
 */
uint8_t hl_util_nvram_param_set_integer(char* param_key, uint8_t integer_value);

/**
 * hl_util_nvram_param_save
 * @brief 保存JSON的内容到NVRAM中
 * @return uint8_t 保存状态 0：保存成功 | 非0：保存失败
 * @date 2022-08-17
 * @author yangxianyun (rd52@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-08-17      <td>yangxianyun     <td>新建
 * </table>
 */
uint8_t hl_util_nvram_param_save();

#endif  //__HL_UTIL_NVRAM_H__
        /*
 * EOF
 */