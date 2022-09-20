/**
 * @file hl_drv_cw2215.c
 * @author lilin (lin.li@hollyland-tech.com)
 * @brief 电量计驱动源文件，使用IIC驱动来控制CW2215电量计。
 * @version 1.0
 * @date 2022-09-01
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-01     <td>v1.0     <td>lilin     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/

#include "hl_drv_cw2215.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/

#define DBG_LOG rt_kprintf

/* IIC Macro */

#define CW2215_IIC_READ_FLAG 1
#define CW2215_IIC_WRITE_FLAG 0

///IIC设备地址
#define CW2215_IIC_DEV_ADDR 0x64

#if HL_GET_DEVICE_TYPE() == 1
///使用的IIC接口名字
#define CW2215_IIC_BUS_NAME "i2c1"
#else
#define CW2215_IIC_BUS_NAME "i2c0"
#endif
/* CW2215 reg base addr Macro */

#define CW2215_REG_CHIP_ID 0x00
#define CW2215_REG_VCELL_H 0x02
#define CW2215_REG_VCELL_L 0x03
#define CW2215_REG_SOC_INT 0x04
#define CW2215_REG_SOC_DECIMAL 0x05
#define CW2215_REG_TEMP 0x06
#define CW2215_REG_MODE_CONFIG 0x08
///中断配置寄存器，设置中断源
#define CW2215_REG_GPIO_CONFIG 0x0A
#define CW2215_REG_SOC_ALERT 0x0B
#define CW2215_REG_TEMP_MAX 0x0C
#define CW2215_REG_TEMP_MIN 0x0D
#define CW2215_REG_CURRENT_H 0x0E
#define CW2215_REG_CURRENT_L 0x0F
#define CW2215_REG_T_HOST_H 0xA0
#define CW2215_REG_T_HOST_L 0xA1
#define CW2215_REG_USER_CONF 0xA2
#define CW2215_REG_CYCLE_H 0xA4
#define CW2215_REG_CYCLE_L 0xA5
#define CW2215_REG_SOH 0xA6
#define CW2215_REG_IC_STATE 0xA7
#define CW2215_REG_STB_CUR_H 0xA8
#define CW2215_REG_STB_CUR_L 0xA9
#define CW2215_REG_FW_VERSION 0xAB
#define CW2215_REG_BAT_PROFILE 0x10

/* chip info Macro */

///chip id
#define CW2215_BATINFO_SIZE 80
#define CW2215_RSENSE 10

#define CW2215_ERROR_IIC -1
#define CW2215_ERROR_CHIP_ID -2
#define CW2215_ERROR_TIME_OUT -3
#define CW2215_NOT_ACTIVE 1
#define CW2215_PROFILE_NOT_READY 2
#define CW2215_PROFILE_NEED_UPDATE 3

#define CW2215_CONFIG_MODE_ACTIVE 0x00
#define CW2215_CONFIG_UPDATE_FLG 0x80
#define CW2215_CONFIG_MODE_RESTART 0x30
#define CW2215_CONFIG_MODE_SLEEP 0xF0

///中断配置寄存器默认值，[6:4]为中断源使能，[2:0]为中断事件标志位
#define CW2215_REG_GPIO_CONFIG_DEFAULT_VALUE 0
///0 ~ 0x7F。 0 ~ 0x64:SOC达到指定值中断，0x65~0x7F：SOC每变化1产生中断
#define CW2215_GPIO_SOC_IRQ_DEFAULT_VALUE 0
#define CW2215_REG_TEMP_MAX_DEFAULT_VALUE 0
#define CW2215_REG_TEMP_MIN_DEFAULT_VALUE 0
#define CW2215_IC_READY_MARK 0x0C

#define CW2215_SLEEP_COUNTS 80

/* variables -----------------------------------------------------------------*/

/* private  variable */

static struct rt_i2c_bus_device* _p_i2c_bus = NULL;
static uint8_t                   _init_flag = 0;

#if HL_GET_DEVICE_TYPE() == 1
///Tx电池profile信息
static uint8_t battery_profile_info[CW2215_BATINFO_SIZE] = {
    0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAD, 0xCB, 0xCC, 0xCE, 0xC5, 0xC2, 0x95, 0x57,
    0x2E, 0xD7, 0xC2, 0x9B, 0x83, 0x6E, 0x5C, 0x4D, 0x42, 0x36, 0x27, 0x48, 0x30, 0xD2, 0x67, 0xE5,
    0xD0, 0xB0, 0xC1, 0xD6, 0x83, 0x99, 0x9A, 0x98, 0x94, 0x9A, 0xA8, 0x8E, 0x72, 0x5E, 0x4E, 0x44,
    0x34, 0x4A, 0x6C, 0x87, 0x9F, 0x98, 0x57, 0x6C, 0x20, 0x00, 0x57, 0x10, 0x00, 0x60, 0x5B, 0x00,
    0x00, 0x00, 0x64, 0x11, 0x83, 0x4F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAD
};
#elif HL_GET_DEVICE_TYPE() == 0
///Rx电池profile信息
static uint8_t battery_profile_info[CW2215_BATINFO_SIZE] = {
    0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9E, 0xAC, 0xB4, 0xCE, 0xC3, 0xC9, 0x88, 0x48,
    0x22, 0xFF, 0xFF, 0xDD, 0x99, 0x82, 0x61, 0x50, 0x45, 0x39, 0x28, 0x88, 0x43, 0xD9, 0x86, 0xCF,
    0xCC, 0xCC, 0xCA, 0xC7, 0xC6, 0xB7, 0xD3, 0xCD, 0xB1, 0xC8, 0xA7, 0x98, 0x8D, 0x86, 0x7D, 0x75,
    0x73, 0x80, 0x82, 0x91, 0xA3, 0x7C, 0x65, 0x5F, 0x20, 0x00, 0x57, 0x10, 0x00, 0x72, 0x55, 0x00,
    0x00, 0x00, 0x64, 0x14, 0x81, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBA
};

#else
#error "HL_GET_DEVICE_TYPE() not 1 or 0!"
#endif

/* Private function(only *.c)  -----------------------------------------------*/

static inline int cw_read(unsigned char PointReg, unsigned char* pData)
{
    struct rt_i2c_msg msgs[2] = { 0 };

    msgs[0].addr  = CW2215_IIC_DEV_ADDR;
    msgs[0].flags = RT_I2C_WR;  // | RT_I2C_NO_STOP;
    msgs[0].buf   = &PointReg;
    msgs[0].len   = 1;

    msgs[1].addr  = CW2215_IIC_DEV_ADDR;
    msgs[1].flags = RT_I2C_RD | RT_I2C_NO_READ_ACK;
    msgs[1].buf   = pData;
    msgs[1].len   = 1;

    if (rt_i2c_transfer(_p_i2c_bus, msgs, 2) == 2) {
        return CW2215_FUNC_RET_OK;
    } else {
        DBG_LOG("i2c read err!\n");
        return CW2215_FUNC_RET_ERR;
    }
}

static inline int cw_write(unsigned char PointReg, unsigned char* pData)
{
    struct rt_i2c_msg msgs[1] = { 0 };
    uint8_t           buf[2];

    buf[0] = PointReg;
    buf[1] = *pData;

    msgs[0].addr  = CW2215_IIC_DEV_ADDR;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = buf;
    msgs[0].len   = 2;

    if (rt_i2c_transfer(_p_i2c_bus, msgs, 1) == 1) {
        return CW2215_FUNC_RET_OK;
    } else {
        DBG_LOG("i2c write err!\n");
        return CW2215_FUNC_RET_ERR;
    }
}

static inline int cw_read_nbyte(unsigned char point_reg, unsigned char* r_pdata, unsigned char len)
{
    struct rt_i2c_msg msgs[2] = { 0 };

    msgs[0].addr  = CW2215_IIC_DEV_ADDR;
    msgs[0].flags = RT_I2C_WR;  //| RT_I2C_NO_STOP;
    msgs[0].buf   = &point_reg;
    msgs[0].len   = 1;

    msgs[1].addr  = CW2215_IIC_DEV_ADDR;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = r_pdata;
    msgs[1].len   = len;

    if (rt_i2c_transfer(_p_i2c_bus, msgs, 2) == 2) {
        return CW2215_FUNC_RET_OK;
    } else {
        return CW2215_FUNC_RET_ERR;
    }
}

static int get_chip_id(uint8_t* p_chip_id)
{
    int           ret;
    unsigned char reg_val;

    ret = cw_read(CW2215_REG_CHIP_ID, &reg_val);
    if (ret == CW2215_FUNC_RET_ERR) {
        return CW2215_ERROR_IIC;
    }

    *p_chip_id = reg_val;

    return 0;
}

/**
 * 
 * @brief 内部初始化电量计时使用，用来判断ic当前状态，判断是否需要更新profile，是否需要重启电量计
 * @return int 
 * @date 2022-09-01
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lilin     <td>新建
 * </table>
 */
static int get_state(void)
{
    int           ret;
    unsigned char reg_val;
    int           i;

    ret = cw_read(CW2215_REG_MODE_CONFIG, &reg_val);
    if (ret == CW2215_FUNC_RET_ERR) {
        return CW2215_ERROR_IIC;
    }

    if (reg_val != CW2215_CONFIG_MODE_ACTIVE) {
        DBG_LOG("Guage state: not active!\n");
        return CW2215_NOT_ACTIVE;
    }

    ret = cw_read(CW2215_REG_SOC_ALERT, &reg_val);
    if (ret == CW2215_FUNC_RET_ERR) {
        return CW2215_ERROR_IIC;
    }

    if (0x00 == (reg_val & CW2215_CONFIG_UPDATE_FLG)) {
        DBG_LOG("Guage state: not ready!\n");
        return CW2215_PROFILE_NOT_READY;
    }

    for (i = 0; i < CW2215_BATINFO_SIZE; i++) {
        ret = cw_read((CW2215_REG_BAT_PROFILE + i), &reg_val);
        if (ret == CW2215_FUNC_RET_ERR) {
            return CW2215_ERROR_IIC;
        }

        /*printf("R reg[%02X] = %02X\n", REG_BAT_PROFILE + i, reg_val);*/
        if (battery_profile_info[i] != reg_val)
            break;
    }

    if (i != CW2215_BATINFO_SIZE) {
        DBG_LOG("Guage state: need update!\n");
        return CW2215_PROFILE_NEED_UPDATE;
    }

    DBG_LOG("Guage state: normal!\n");

    return 0;
}

/**
 * 
 * @brief 用来关闭ic，进入超低功耗<0.5uA模式，但需要注意的是ic没有真正的sleep，进入sleep相当于shutdown，再次使用ic的时候需要调用init函数启动ic。
 * @return int 
 * @date 2022-09-01
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lilin     <td>新建
 * </table>
 */
static int sleep(void)
{
    int           ret;
    unsigned char reg_val = CW2215_CONFIG_MODE_RESTART;

    ret = cw_write(CW2215_REG_MODE_CONFIG, &reg_val);
    if (ret == CW2215_FUNC_RET_ERR) {
        return CW2215_ERROR_IIC;
    }

    rt_thread_mdelay(25); /* Here delay must >= 20 ms */

    reg_val = CW2215_CONFIG_MODE_SLEEP;
    ret     = cw_write(CW2215_REG_MODE_CONFIG, &reg_val);
    if (ret == CW2215_FUNC_RET_ERR) {
        return CW2215_ERROR_IIC;
    }

    rt_thread_mdelay(10);

    return 0;
}

/**
 * 
 * @brief 内部初始化电量计时使用，用来重写profile
 * @param [in] buf 
 * @return int 
 * @date 2022-09-01
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lilin     <td>新建
 * </table>
 */
static int write_profile(unsigned char buf[])
{
    int ret;
    int i;

    for (i = 0; i < CW2215_BATINFO_SIZE; i++) {
        ret = cw_write(CW2215_REG_BAT_PROFILE + i, &buf[i]);
        if (ret == CW2215_FUNC_RET_ERR) {
            return CW2215_ERROR_IIC;
        }
    }

    return 0;
}

/**
 * 
 * @brief 内部初始化电量计时使用，使ic进入工作模式
 * @return int 
 * @date 2022-09-01
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lilin     <td>新建
 * </table>
 */
static int active()
{
    int           ret;
    unsigned char reg_val = CW2215_CONFIG_MODE_RESTART;

    ret = cw_write(CW2215_REG_MODE_CONFIG, &reg_val);
    if (ret == CW2215_FUNC_RET_ERR) {
        return CW2215_ERROR_IIC;
    }

    rt_thread_mdelay(25); /* Here delay must >= 20 ms */

    reg_val = CW2215_CONFIG_MODE_ACTIVE;
    ret     = cw_write(CW2215_REG_MODE_CONFIG, &reg_val);
    if (ret == CW2215_FUNC_RET_ERR) {
        return CW2215_ERROR_IIC;
    }

    rt_thread_mdelay(10);

    return 0;
}

/**
 * 
 * @brief 内部初始化电量计时使用，根据代码配置电量计profile并调用active，之后判断ic是否能够成功的启动
 * @return int 
 * @date 2022-09-01
 * @author lilin (lin.li@hollyland-tech.com)
 * 
 * @details 
 * @note 
 * @par 修改日志:
 * <table>
 * <tr><th>Date             <th>Author         <th>Description
 * <tr><td>2022-09-01      <td>lilin     <td>新建
 * </table>
 */
static int config_start_ic()
{
    int           ret     = 0;
    unsigned char reg_val = 0;
    // unsigned int  count   = 0;

    ret = sleep();
    if (ret < 0) {
        return ret;
    }

    /* update new battery info */
    ret = write_profile(battery_profile_info);
    if (ret < 0)
        return ret;

    /* set UPDATE_FLAG AND SOC INTTERRUP VALUE*/
    reg_val = CW2215_CONFIG_UPDATE_FLG | CW2215_GPIO_SOC_IRQ_DEFAULT_VALUE;
    ret     = cw_write(CW2215_REG_SOC_ALERT, &reg_val);
    if (ret == CW2215_FUNC_RET_ERR) {
        return CW2215_ERROR_IIC;
    }

    /*close all interruptes*/
    reg_val = 0;
    ret     = cw_write(CW2215_REG_GPIO_CONFIG, &reg_val);
    if (ret == CW2215_FUNC_RET_ERR) {
        return CW2215_ERROR_IIC;
    }

    /*set inerruptes*/
    reg_val = CW2215_REG_GPIO_CONFIG_DEFAULT_VALUE;
    ret     = cw_write(CW2215_REG_GPIO_CONFIG, &reg_val);
    if (ret == CW2215_FUNC_RET_ERR) {
        return CW2215_ERROR_IIC;
    }

    reg_val = CW2215_REG_TEMP_MAX_DEFAULT_VALUE;
    ret     = cw_write(CW2215_REG_TEMP_MAX, &reg_val);
    if (ret == CW2215_FUNC_RET_ERR) {
        return CW2215_ERROR_IIC;
    }

    reg_val = CW2215_REG_TEMP_MIN_DEFAULT_VALUE;
    ret     = cw_write(CW2215_REG_TEMP_MIN, &reg_val);
    if (ret == CW2215_FUNC_RET_ERR) {
        return CW2215_ERROR_IIC;
    }

    ret = active();
    if (ret < 0)
        return ret;

    // while (1) {
    // 	cw_read(CW2215_REG_IC_STATE, &reg_val);
    // 	if (CW2215_IC_READY_MARK == (reg_val & CW2215_IC_READY_MARK))
    // 	{
    // 		break;
    // 	}

    // 	count++;
    // 	if (count >= CW2215_SLEEP_COUNTS) {
    // 		sleep();
    // 		return CW2215_ERROR_TIME_OUT;
    // 	}

    // 	rt_thread_mdelay(100); /*sleep 100 ms must*/
    // }

    return 0;
}

static int init(void)
{
    int     ret;
    uint8_t chip_id;

    ret = get_chip_id(&chip_id);
    if (ret) {
        return ret;
    }

    if (chip_id != CW2215_CHIP_ID) {
        DBG_LOG("chip id err:%02x, STD id:%02x", chip_id, CW2215_CHIP_ID);
        return CW2215_ERROR_CHIP_ID;
    }

    ret = get_state();
    if (ret < 0) {
        return ret;
    }

    if (ret != 0) {
        DBG_LOG("config Guage!\n");
        ret = config_start_ic();
        if (ret < 0)
            return ret;
    }

    return 0;
}

static int get_vcell(uint16_t* p_vol)
{
    int      ret;
    uint8_t  vcell_l, vcell_h;
    uint16_t vcell;

    ret = cw_read(CW2215_REG_VCELL_L, &vcell_l);
    if (ret == CW2215_FUNC_RET_ERR) {
        return CW2215_ERROR_IIC;
    }

    ret = cw_read(CW2215_REG_VCELL_H, &vcell_h);
    if (ret == CW2215_FUNC_RET_ERR) {
        return CW2215_ERROR_IIC;
    }

    vcell = ((uint16_t)(vcell_h) << 8) | vcell_l;
    vcell = vcell * 5 / 16;

    *p_vol = vcell;

    return 0;
}

static int get_soc(hl_st_drv_guage_soc_t* p_soc)
{
    int     ret;
    uint8_t soc, soc_d;

    ret = cw_read(CW2215_REG_SOC_INT, &soc);
    if (ret == CW2215_FUNC_RET_ERR) {
        return CW2215_ERROR_IIC;
    }

    ret = cw_read(CW2215_REG_SOC_DECIMAL, &soc_d);
    if (ret == CW2215_FUNC_RET_ERR) {
        return CW2215_ERROR_IIC;
    }

    p_soc->soc   = soc;
    p_soc->soc_d = soc_d;

    return 0;
}

static int get_temp(hl_st_drv_guage_temp_t* p_temp)
{
    int           ret     = 0;
    unsigned char reg_val = 0;
    int           temp    = 0;

    ret = cw_read(CW2215_REG_TEMP, &reg_val);
    if (ret)
        return CW2215_ERROR_IIC;

    temp = (int)reg_val * 10 / 2 - 400;

    p_temp->temp   = temp / 10;
    p_temp->temp_d = temp - (p_temp->temp * 10);

    return 0;
}

static int32_t get_complement_code(uint16_t raw_code)
{
    int32_t complement_code = 0;
    int     dir             = 0;

    if (0 != (raw_code & 0x8000)) {
        dir      = -1;
        raw_code = (~raw_code) + 1;
    } else {
        dir = 1;
    }

    complement_code = (int32_t)raw_code * dir;

    return complement_code;
}

static int get_current(int32_t* p_current)
{
    int     ret          = 0;
    int32_t current_temp = 0;
    uint8_t current_h;
    uint8_t current_l;

    ret = cw_read(CW2215_REG_CURRENT_H, &current_h);
    if (ret)
        return CW2215_ERROR_IIC;

    ret = cw_read(CW2215_REG_CURRENT_L, &current_l);
    if (ret)
        return CW2215_ERROR_IIC;

    current_temp = get_complement_code((current_h << 8) + current_l);
    current_temp = ((current_temp)*160 / CW2215_RSENSE / 100);

    *p_current = current_temp;

    return 0;
}

static int get_soh(uint8_t* p_soh)
{
    int           ret     = 0;
    unsigned char reg_val = 0;
    unsigned char SOH     = 0;

    ret = cw_read(CW2215_REG_SOH, &reg_val);
    if (ret)
        return CW2215_ERROR_IIC;

    SOH    = reg_val;
    *p_soh = SOH;

    return 0;
}

static int get_cycle_count(uint32_t* p_cycle_count)
{
    int          ret             = 0;
    uint8_t      temp_val_buff_h = 0;
    uint8_t      temp_val_buff_l = 0;
    unsigned int temp_val_buff   = 0;
    unsigned int cycle_buff      = 0;

    ret = cw_read(CW2215_REG_CYCLE_H, &temp_val_buff_h);
    if (ret)
        return CW2215_ERROR_IIC;

    ret = cw_read(CW2215_REG_CYCLE_L, &temp_val_buff_l);
    if (ret)
        return CW2215_ERROR_IIC;

    temp_val_buff  = (temp_val_buff_h << 8) + temp_val_buff_l;
    cycle_buff     = temp_val_buff / 16;
    *p_cycle_count = cycle_buff;

    return 0;
}

static int dump_all_register_value(void)
{
    unsigned char reg_val = 0;
    int           i       = 0;

    for (i = 0; i <= 0x5F; i++) {
        cw_read(i, &reg_val);
        /*Please add print if use*/
        /*printf("reg[%02X] = %02X\n", i, (int)reg_val);*/
    }
    for (i = 0xA0; i <= 0xAB; i++) {
        cw_read(i, &reg_val);
        /*Please add print if use*/
        /*printf("reg[%02X] = %02X\n", i, (int)reg_val);*/
    }
    return 0;
}

/* Exported functions --------------------------------------------------------*/

int8_t hl_drv_cw2215_init(void)
{
    int ret;

    if (_init_flag != 0) {
        DBG_LOG("Guage is already inited!\n");
        return CW2215_FUNC_RET_ERR;
    }

    _p_i2c_bus = (struct rt_i2c_bus_device*)rt_device_find(CW2215_IIC_BUS_NAME);
    if (_p_i2c_bus == NULL) {
        DBG_LOG("i2c dev not found!:%s", CW2215_IIC_BUS_NAME);
        return CW2215_FUNC_RET_ERR;
    }

    ret = init();
    if (ret < 0) {
        DBG_LOG("Guage init err!\n");
        return CW2215_FUNC_RET_ERR;
    }

    DBG_LOG("\nGuage init success!\n");

    _init_flag = 1;

    return CW2215_FUNC_RET_OK;
}

int8_t hl_drv_cw2215_deinit(void)
{
    int ret;
    if (_init_flag != 1) {
        DBG_LOG("Guage is not inited!\n");
        return CW2215_FUNC_RET_ERR;
    }

    ret = sleep();
    if (ret < 0) {
        return CW2215_FUNC_RET_ERR;
    }

    _p_i2c_bus = NULL;

    DBG_LOG("Guage deinit success!\n");
    _init_flag = 0;
    return CW2215_FUNC_RET_OK;
}

int8_t hl_drv_cw2215_ctrl(uint8_t op, void* arg, int32_t arg_size)
{
    int ret;
    if (_init_flag != 1) {
        DBG_LOG("Guage is not inited!\n");
        return CW2215_FUNC_RET_ERR;
    }

    switch (op) {
        case HL_DRV_GUAGE_GET_CHIP_ID: {
            if (arg_size != sizeof(char)) {
                DBG_LOG("size err, ctrl arg need <char> type pointer!\n");
                return CW2215_FUNC_RET_ERR;
            }

            ret = get_chip_id((uint8_t*)arg);
            if (ret < 0) {
                return CW2215_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_GUAGE_GET_VOLTAGE: {
            if (arg_size != sizeof(uint16_t)) {
                DBG_LOG("size err, ctrl arg need <uint16_t> type pointer!\n");
                return CW2215_FUNC_RET_ERR;
            }

            ret = get_vcell((uint16_t*)arg);
            if (ret < 0) {
                return CW2215_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_GUAGE_GET_SOC: {
            if (arg_size != sizeof(hl_st_drv_guage_soc_t)) {
                DBG_LOG("size err, ctrl arg need <hl_st_drv_guage_soc_t> type pointer!\n");
                return CW2215_FUNC_RET_ERR;
            }

            ret = get_soc((hl_st_drv_guage_soc_t*)arg);
            if (ret < 0) {
                return CW2215_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_GUAGE_GET_TEMP: {
            if (arg_size != sizeof(hl_st_drv_guage_temp_t)) {
                DBG_LOG("size err, ctrl arg need <hl_st_drv_guage_temp_t> type pointer!\n");
                return CW2215_FUNC_RET_ERR;
            }

            ret = get_temp((hl_st_drv_guage_temp_t*)arg);
            if (ret < 0) {
                return CW2215_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_GUAGE_GET_CURRENT: {
            if (arg_size != sizeof(int32_t)) {
                DBG_LOG("size err, ctrl arg need <int32_t> type pointer!\n");
                return CW2215_FUNC_RET_ERR;
            }

            ret = get_current((int32_t*)arg);
            if (ret < 0) {
                return CW2215_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_GUAGE_GET_SOH: {
            if (arg_size != sizeof(uint8_t)) {
                DBG_LOG("size err, ctrl arg need <uint8_t> type pointer!\n");
                return CW2215_FUNC_RET_ERR;
            }

            ret = get_soh((uint8_t*)arg);
            if (ret < 0) {
                return CW2215_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_GUAGE_GET_CYCLE_COUNT: {
            if (arg_size != sizeof(uint32_t)) {
                DBG_LOG("size err, ctrl arg need <uint32_t> type pointer!\n");
                return CW2215_FUNC_RET_ERR;
            }

            ret = get_cycle_count((uint32_t*)arg);
            if (ret < 0) {
                return CW2215_FUNC_RET_ERR;
            }
        } break;
        case HL_DRV_GUAGE_DUMP_ALL_REGISTER_VALUE: {
            ret = dump_all_register_value();
            if (ret < 0) {
                return CW2215_FUNC_RET_ERR;
            }
        } break;
        default:
            break;
    }

    return CW2215_FUNC_RET_OK;
}

/*
 * EOF
 */