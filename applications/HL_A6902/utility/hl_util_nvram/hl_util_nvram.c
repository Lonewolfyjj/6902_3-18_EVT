/**
 * @file hl_util_nvram.c
 * @author yangxianyun (rd52@hollyland-tech.com)
 * @brief NVRam的具体实现文件，包含了cJSON的功能
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
#include "cJSON/cJSON.h"
#include "hl_util_nvram.h"
#include <string.h>
#include <stdlib.h>
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/

#define HL_UTIL_NVRAM_JSON_BUFFER_SIZE 4096

/* variables -----------------------------------------------------------------*/

/// nvram的控制句柄
static hl_util_nvram_t_p sg_nvram_handle = NULL;

static uint8_t* sg_json_str_buffer;
static cJSON*   sg_json_paramaters;

/* Private function(only *.c)  -----------------------------------------------*/

static void _hl_util_nvram_test_cJSON()
{
#if 1
    cJSON*     usr;
    cJSON*     item;
    char*      data;
    uint16_t   len       = 0;
    int        ret       = 0;
    cJSON_bool check_ret = cJSON_False;

    ret = sg_nvram_handle->nvram_read((char*)sg_json_str_buffer, &len);
    if (len) {
        usr = cJSON_Parse(sg_json_str_buffer);
        sg_nvram_handle->std_printf("get nvram read len = %d, str len = %d\r\n", len, strlen(sg_json_str_buffer));
    } else {
        sg_nvram_handle->std_printf("error of get nvram\r\n");
        return;
    }

    check_ret = cJSON_HasObjectItem(usr, "HL_SN2");
    sg_nvram_handle->std_printf("has object? = %d\r\n", check_ret);

    item = cJSON_GetObjectItem(usr, "HL_SN2");
    sg_nvram_handle->std_printf("string = %s, value = %s\r\n", item->string, item->valuestring);

    // data = cJSON_Print(usr);  //将json形式打印成正常字符串形式(带有\r\n)
    //	    data = cJSON_PrintUnformatted(usr);   //将json形式打印成正常字符串形式(没有\r\n)
    // sg_nvram_handle->std_printf("%s\r\n", data);  //通过串口打印出来

    /********************/
    /*		释放内存				*/
    /*********************/
    cJSON_Delete(usr);
    // free(data);
#else
    // do nothing
#endif
}

static void _hl_util_nvram_print_json_data()
{
    if (!sg_nvram_handle) {
        sg_nvram_handle->std_printf("error: nvram not init\r\n");
        return;
    }
    uint8_t data_temp[129] = { 0 };
    uint8_t i              = 0;
    int     ret            = 0;
    char*   data;

    data = cJSON_Print(sg_json_paramaters);  //将json形式打印成正常字符串形式(没有\r\n)
    sg_nvram_handle->std_printf("\r\n");
    for (i = 0; i < (strlen(data) / 128); i++) {
        memcpy(data_temp, &data[128 * i], 128);
        sg_nvram_handle->std_printf("%s", data_temp);
    }
    memcpy(data_temp, &data[128 * i], 128);
    sg_nvram_handle->std_printf("%s", data_temp);
    free(data);
}

static void _hl_util_nvram_print_json_data_from_a_json(char* cjosn_data, uint16_t len)
{
    if (!sg_nvram_handle) {
        sg_nvram_handle->std_printf("error: nvram not init\r\n");
        return;
    }
    uint8_t data_temp[129] = { 0 };
    uint8_t i              = 0;
    int     ret            = 0;

    sg_nvram_handle->std_printf("\r\n");
    for (i = 0; i < (len / 128); i++) {
        memcpy(data_temp, &cjosn_data[128 * i], 128);
        sg_nvram_handle->std_printf("%s", data_temp);
    }
    memcpy(data_temp, &cjosn_data[128 * i], 128);
    sg_nvram_handle->std_printf("%s", data_temp);
}

/* Exported functions --------------------------------------------------------*/

uint8_t hl_util_nvram_param_init(void (*std_printf)(const char* fmt, ...),
                                 uint8_t (*nvram_write)(char* data, uint16_t len),
                                 uint8_t (*nvram_read)(char* data, uint16_t* len), uint8_t (*nvram_mutex_take)(),
                                 uint8_t (*nvram_mutex_release)())
{
    int      ret = 0;
    uint16_t len;
    if (NULL == std_printf || NULL == nvram_read || NULL == nvram_write || sg_nvram_handle || NULL == nvram_mutex_take
        || NULL == nvram_mutex_release) {
        sg_nvram_handle->std_printf("error: para is null\r\n");
        return 1;
    }
    sg_nvram_handle                      = malloc(sizeof(hl_util_nvram_t));
    sg_nvram_handle->enable              = 1;
    sg_nvram_handle->status              = HL_EM_UTIL_NVRAM_INITIALIZED;
    sg_nvram_handle->std_printf          = std_printf;
    sg_nvram_handle->nvram_read          = nvram_read;
    sg_nvram_handle->nvram_write         = nvram_write;
    sg_nvram_handle->nvram_mutex_take    = nvram_mutex_take;
    sg_nvram_handle->nvram_mutex_release = nvram_mutex_release;
    sg_nvram_handle->have_changed        = 0;

    sg_json_str_buffer = malloc(HL_UTIL_NVRAM_JSON_BUFFER_SIZE);
    if (NULL == sg_json_str_buffer) {
        sg_nvram_handle->std_printf("error malloc data\r\n");
        return 2;
    }

    ret = sg_nvram_handle->nvram_read((char*)sg_json_str_buffer, &len);

    if (len) {
        sg_json_paramaters = cJSON_Parse(sg_json_str_buffer);
        sg_nvram_handle->std_printf("get nvram read len = %d, str len = %d\r\n", len, strlen(sg_json_str_buffer));
    } else {
        sg_nvram_handle->std_printf("error of get nvram json\r\n");
        return 3;
    }

    _hl_util_nvram_print_json_data();

    return 0;
}

uint8_t hl_util_nvram_param_deinit()
{
    if (!sg_nvram_handle) {
        sg_nvram_handle->std_printf("error: nvram not init\r\n");
        return 1;
    }

    hl_util_nvram_param_save();
    free(sg_json_str_buffer);
    cJSON_Delete(sg_json_paramaters);
    free(sg_nvram_handle);
    return 0;
}

uint8_t hl_util_nvram_param_get(char* param_key, char* param_value, char* default_value, uint16_t value_len)
{
    if (!sg_nvram_handle || NULL == param_key || NULL == param_value || NULL == default_value || value_len == 0) {
        // sg_nvram_handle->std_printf("error: nvram not init\r\n");
        return 1;
    }

    if (sg_nvram_handle->nvram_mutex_take) {
        sg_nvram_handle->nvram_mutex_take();
    }

    cJSON*     item;
    cJSON_bool check_ret = cJSON_False;
    uint8_t     ret       = 0;

    check_ret = cJSON_HasObjectItem(sg_json_paramaters, param_key);

    if (check_ret) {
        item = cJSON_GetObjectItem(sg_json_paramaters, param_key);
        if (value_len < strlen(item->valuestring) + 1) {
            sg_nvram_handle->std_printf(
                "error: nvram get value buffer len no enough bufferlen = %d, value_len = %d\r\n", value_len,
                strlen(item->valuestring) + 1);
            ret = 3;
        }
        strcpy(param_value, item->valuestring);
    } else {
        sg_nvram_handle->std_printf("error: nvram have no item %d\r\n", param_key);
        ret = 2;
    }

    if (sg_nvram_handle->nvram_mutex_release) {
        sg_nvram_handle->nvram_mutex_release();
    }

    return ret;
}

uint8_t hl_util_nvram_param_get_integer(char* param_key, int* param_value, int default_value)
{
    if (!sg_nvram_handle || NULL == param_key || NULL == param_value) {
        // sg_nvram_handle->std_printf("error: nvram not init\r\n");
        return 1;
    }

    if (sg_nvram_handle->nvram_mutex_take) {
        sg_nvram_handle->nvram_mutex_take();
    }

    //抽象一下，代码冗余了

    cJSON*     item;
    cJSON_bool check_ret = cJSON_False;
    uint8_t    ret       = 0;

    check_ret = cJSON_HasObjectItem(sg_json_paramaters, param_key);
    if (check_ret) {
        item = cJSON_GetObjectItem(sg_json_paramaters, param_key);
        // strcpy(param_value, item->valueint);
        *param_value = atoi(item->valuestring);
    } else {
        sg_nvram_handle->std_printf("error: nvram have no item %d\r\n", param_key);
        ret = 2;
    }

    if (sg_nvram_handle->nvram_mutex_release) {
        sg_nvram_handle->nvram_mutex_release();
    }

    return ret;
}

uint8_t hl_util_nvram_param_set(char* param_key, char* param_value)
{
    if (!sg_nvram_handle || NULL == param_key || NULL == param_value) {
        // sg_nvram_handle->std_printf("error: nvram not init\r\n");
        return 1;
    }

    if (strlen(param_value) > HL_UTIL_NVRAM_ITEM_LEN_LIMIT || strlen(param_key) > HL_UTIL_NVRAM_ITEM_LEN_LIMIT) {
        sg_nvram_handle->std_printf("error: item len out of range key_len = %d, ovalue_len = %d\r\n", strlen(param_key),
                                    strlen(param_value));
        return 4;
    }

    if (sg_nvram_handle->nvram_mutex_take) {
        sg_nvram_handle->nvram_mutex_take();
    }

    cJSON_bool check_ret = cJSON_False;
    uint8_t    ret       = 0;

    check_ret = cJSON_HasObjectItem(sg_json_paramaters, param_key);
    if (check_ret) {
        check_ret = cJSON_ReplaceItemInObject(sg_json_paramaters, param_key, cJSON_CreateString(param_value));
        if (!check_ret) {
            sg_nvram_handle->std_printf("replace wrong!!\r\n");
            ret = 3;
        }
        sg_nvram_handle->have_changed = 1;
    } else {
        sg_nvram_handle->std_printf("error: nvram have no item %d\r\n", param_key);
        ret = 2;
    }

    if (sg_nvram_handle->nvram_mutex_release) {
        sg_nvram_handle->nvram_mutex_release();
    }
    return ret;
}

uint8_t hl_util_nvram_param_save()
{
    char* data;
    int   ret = 0;

    if (!sg_nvram_handle) {
        sg_nvram_handle->std_printf("error: nvram not init\r\n");
        return 1;
    }

    //有更改才写入
    sg_nvram_handle->have_changed = 1;
    if (sg_nvram_handle->have_changed == 1) {
        data = cJSON_PrintUnformatted(sg_json_paramaters);
        _hl_util_nvram_print_json_data_from_a_json(data, strlen(data));
        memset(sg_json_str_buffer, 0x00, HL_UTIL_NVRAM_JSON_BUFFER_SIZE);
        strcpy(sg_json_str_buffer, data);
        ret = sg_nvram_handle->nvram_write(sg_json_str_buffer, HL_UTIL_NVRAM_JSON_BUFFER_SIZE);
        free(data);
    } else {
        sg_nvram_handle->std_printf("error: nvram no change to save\r\n");
        ret = 2;
    }

    return ret;
}

/*
 * EOF
 */