/**
 * @file hl_drv_ins5830b.c
 * @author dujunjie (junjie.du@hollyland-tech.com)
 * @brief 
 * @version 1.0
 * @date 2023-01-05
 * 
 * ██╗  ██╗ ██████╗ ██╗     ██╗  ██╗   ██╗██╗      █████╗ ███╗   ██╗██████╗ 
 * ██║  ██║██╔═══██╗██║     ██║  ╚██╗ ██╔╝██║     ██╔══██╗████╗  ██║██╔══██╗
 * ███████║██║   ██║██║     ██║   ╚████╔╝ ██║     ███████║██╔██╗ ██║██║  ██║
 * ██╔══██║██║   ██║██║     ██║    ╚██╔╝  ██║     ██╔══██║██║╚██╗██║██║  ██║
 * ██║  ██║╚██████╔╝███████╗███████╗██║   ███████╗██║  ██║██║ ╚████║██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝
 * @copyright Copyright (c) 2023 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2023-01-05     <td>v1.0     <td>dujunjie       <td>初次发布
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "hl_drv_ins5830b.h"
#include "string.h"
/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/
#define INS5830B_DEBUG
#ifdef INS5830B_DEBUG
#define ins_printf(...) rt_kprintf(__VA_ARGS__)
#else
#define ins_printf(...)
#endif
/* variables -----------------------------------------------------------------*/

static struct rt_i2c_bus_device* i2c_bus        = RT_NULL; /* I2C总线设备句柄 */
static const uint8_t             table_week[12] = { 0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5 };

static uint8_t rtc_week_list[] = {
    HL_WEEK_SUNDAY,   HL_WEEK_MONDAY, HL_WEEK_TUESDAY,  HL_WEEK_WEDNESDAY,
    HL_WEEK_THURSDAY, HL_WEEK_FIRDAY, HL_WEEK_SATURDAY,
};

/* Private function(only *.c)  -----------------------------------------------*/

static rt_err_t hl_i2c_write_reg(struct rt_i2c_bus_device* bus, rt_uint8_t reg, rt_uint8_t* wbuf, rt_uint16_t buf_len)
{
    struct rt_i2c_msg msgs[2];
    rt_uint8_t*       buf = (rt_uint8_t*)rt_malloc(buf_len + 4);

    rt_memset(buf, 0, buf_len + 4);

    buf[0] = reg;
    rt_memcpy(&buf[1], wbuf, buf_len);

    msgs[0].addr  = INS5830B_DEVICE_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = buf;
    msgs[0].len   = buf_len + 1;

    // 调用I2C设备接口传输数据
    if (rt_i2c_transfer(bus, msgs, 1) == 1) {
        rt_free(buf);
        return HL_SUCCESS;
    } else {
        rt_free(buf);
        return HL_FAILED;
    }
}

static rt_err_t hl_i2c_read_reg(struct rt_i2c_bus_device* bus, rt_uint8_t reg, rt_uint8_t* rbuf, rt_uint16_t buf_len)
{
    struct rt_i2c_msg msgs[2];

    msgs[0].addr  = INS5830B_DEVICE_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &reg;
    msgs[0].len   = 1;

    msgs[1].addr  = INS5830B_DEVICE_ADDRESS;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = rbuf;
    msgs[1].len   = buf_len;

    // 调用I2C设备接口传输数据
    if (rt_i2c_transfer(bus, msgs, 2) == 2) {
        return HL_SUCCESS;
    } else {
        return HL_FAILED;
    }
}

static uint8_t hl_drv_rtc_get_week(uint16_t year, uint8_t month, uint8_t day)
{
    uint16_t tmp;
    uint8_t  yearH, yearL;

    yearH = year / 100;
    yearL = year % 100;
    // 如果为21世纪,年份数加100
    if (yearH > 19) {
        yearL += 100;
    }
    // 所过闰年数只算1900年之后的
    tmp = yearL + yearL / 4;
    tmp = tmp % 7;
    tmp = tmp + day + table_week[month - 1];
    if (yearL % 4 == 0 && month < 3) {
        tmp--;
    }
    return (tmp % 7);
}

static int hl_drv_rtc_settime(HL_INS5830B_RTC_IOCTL_T* rtc_ptr)
{
    HL_INS5830B_RTC_T rtc_config;
    rtc_config.rtc_year.year_high   = (rtc_ptr->year - 2000) / 10;
    rtc_config.rtc_year.year_low    = (rtc_ptr->year - 2000) % 10;
    rtc_config.rtc_month.month_high = rtc_ptr->month / 10;
    rtc_config.rtc_month.month_low  = rtc_ptr->month % 10;
    rtc_config.rtc_day.day_high     = rtc_ptr->day / 10;
    rtc_config.rtc_day.day_low      = rtc_ptr->day % 10;
    rtc_config.rtc_week             = rtc_week_list[hl_drv_rtc_get_week(rtc_ptr->year, rtc_ptr->month, rtc_ptr->day)];
    rtc_config.rtc_hour.hour_high   = rtc_ptr->hour / 10;
    rtc_config.rtc_hour.hour_low    = rtc_ptr->hour % 10;
    rtc_config.rtc_min.min_high     = rtc_ptr->min / 10;
    rtc_config.rtc_min.min_low      = rtc_ptr->min % 10;
    rtc_config.rtc_sec.sec_high     = rtc_ptr->sec / 10;
    rtc_config.rtc_sec.sec_low      = rtc_ptr->sec % 10;

    if (hl_i2c_write_reg(i2c_bus, INS5830B_RTC_FIRST_REG, (rt_uint8_t*)&rtc_config, sizeof(HL_INS5830B_RTC_T))) {
        ins_printf("%s write err !\n", __FUNCTION__);
        goto INIT_ERR;
    }
    return HL_SUCCESS;
INIT_ERR:
    return HL_FAILED;
}

static int hl_drv_rtc_gettime(HL_INS5830B_RTC_IOCTL_T* rtc_ptr)
{
    HL_INS5830B_RTC_T rtc_config;
    if (hl_i2c_read_reg(i2c_bus, INS5830B_RTC_FIRST_REG, (rt_uint8_t*)&rtc_config, sizeof(HL_INS5830B_RTC_T))) {
        ins_printf("%s read err !\n", __FUNCTION__);
        goto INIT_ERR;
    }
    rtc_ptr->year  = (rtc_config.rtc_year.year_high * 10) + rtc_config.rtc_year.year_low + 2000;
    rtc_ptr->month = (rtc_config.rtc_month.month_high * 10) + rtc_config.rtc_month.month_low;
    rtc_ptr->day   = (rtc_config.rtc_day.day_high * 10) + rtc_config.rtc_day.day_low;
    rtc_ptr->hour  = (rtc_config.rtc_hour.hour_high * 10) + rtc_config.rtc_hour.hour_low;
    rtc_ptr->min   = (rtc_config.rtc_min.min_high * 10) + rtc_config.rtc_min.min_low;
    rtc_ptr->sec   = (rtc_config.rtc_sec.sec_high * 10) + rtc_config.rtc_sec.sec_low;
    return HL_SUCCESS;
INIT_ERR:
    return HL_FAILED;
}

int hl_drv_ins5830b_init(void)
{
    uint8_t inien_init;
    i2c_bus = (struct rt_i2c_bus_device*)rt_device_find(INS5830B_IIC_NAME);
    if (hl_i2c_read_reg(i2c_bus, INS5830B_INIEN_SET_REG, &inien_init, 1)) {
        ins_printf("%s init read err !\n", __FUNCTION__);
        goto INIT_ERR;
    }
    inien_init |= (1 << 4);
    if (hl_i2c_write_reg(i2c_bus, INS5830B_INIEN_SET_REG, &inien_init, 1)) {
        ins_printf("%s init write err !\n", __FUNCTION__);
        goto INIT_ERR;
    }
    ins_printf("Ins5830b init successed !\n");
    return HL_SUCCESS;
INIT_ERR:
    ins_printf("Ins5830b init failed !\n");
    return HL_FAILED;
}

int hl_drv_ins5830b_io_ctrl(uint8_t cmd, void* ptr, uint8_t len)
{
    HL_INS5830B_RTC_IOCTL_T* rtc_ptr = (HL_INS5830B_RTC_IOCTL_T*)ptr;
    if (ptr == NULL || len != 1) {
        ins_printf("ins5830b param err ! len : [ %X ]\n", len);
        goto INIT_ERR;
    }
    switch (cmd) {
        case INS5830B_RTC_SET_TIME_CMD:
            if (hl_drv_rtc_settime(rtc_ptr)) {
                goto INIT_ERR;
            }
            break;
        case INS5830B_RTC_GET_TIME_CMD:
            if (hl_drv_rtc_gettime(rtc_ptr)) {
                goto INIT_ERR;
            }
            break;
        default:
            break;
    }
    return HL_SUCCESS;
INIT_ERR:
    return HL_FAILED;
}


void rtc_ins5830b_test(int argc, char** argv)
{
    HL_INS5830B_RTC_IOCTL_T   rtc_test;
    if (!strcmp("init", argv[1])) {
       hl_drv_ins5830b_init();
    }else if(!strcmp("set", argv[1])){
        rtc_test.year = atoi(argv[2]);
        rtc_test.month = atoi(argv[3]);
        rtc_test.day = atoi(argv[4]);
        rtc_test.hour = atoi(argv[5]);
        rtc_test.min = atoi(argv[6]);
        rtc_test.sec = atoi(argv[7]);
        hl_drv_ins5830b_io_ctrl(INS5830B_RTC_SET_TIME_CMD,&rtc_test,1);
    }else if(!strcmp("get", argv[1])){
        hl_drv_ins5830b_io_ctrl(INS5830B_RTC_GET_TIME_CMD,&rtc_test,1);
        ins_printf("year  : %d\n",rtc_test.year);
        ins_printf("month : %d\n",rtc_test.month);
        ins_printf("day   : %d\n",rtc_test.day);
        ins_printf("hour  : %d\n",rtc_test.hour);
        ins_printf("min   : %d\n",rtc_test.min);
        ins_printf("sec   : %d\n",rtc_test.sec);
    }
}

MSH_CMD_EXPORT(rtc_ins5830b_test, run rtc_ins5830b_test);
/* Exported functions --------------------------------------------------------*/
/*
 * EOF
 */