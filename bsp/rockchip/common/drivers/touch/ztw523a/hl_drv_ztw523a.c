#include "hl_drv_ztw523a.h"
#if ! HL_IS_TX_DEVICE()
#include "touch.h"
#include "hal_pinctrl.h"

#define DBG_SECTION_NAME "drv_ztw523a"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>


#define CURRENT_TOUCH_DEVICE_NAME "ZTW523A"

#define CURRENT_TOUCH_IRQ_PIN GPIO1_C1
#define CURRENT_TOUCH_IRQ_MODE PIN_MODE_INPUT_PULLUP

#define CURRENT_TOUCH_RESET_PIN GPIO1_C2
#define CURRENT_TOUCH_RESET_MODE PIN_MODE_OUTPUT

#define CURRENT_TOUCH_RESET_PIN_H PIN_HIGH
#define CURRENT_TOUCH_RESET_PIN_L PIN_LOW

#define CURRENT_TOUCH_POINT_NUM 1
#define CURRENT_TOUCH_X_RANGE TPD_RES_MAX_X
#define CURRENT_TOUCH_Y_RANGE TPD_RES_MAX_Y

// #define BUTTON_DOWN 1
// #define BUTTON_UP   0

static rt_size_t hl_drv_ztw523a_touchinfo(struct rt_touch_device *touch, void *buf, rt_size_t touch_num);

static uint16_t                  ic_int_mask = 0;
static uint16_t                  chip_code   = 0;
static struct rt_i2c_bus_device* i2c_bus     = RT_NULL; /* I2C总线设备句柄 */
static rt_touch_t                touch_hand = RT_NULL;
static rt_device_t               touch_dev  = RT_NULL;

const static struct rt_touch_ops rt_touch_ops_top = {
    .touch_control   = RT_NULL,
    .touch_readpoint = hl_drv_ztw523a_touchinfo,
};

const static struct rt_touch_info rt_touch_info = {
    .point_num = CURRENT_TOUCH_POINT_NUM,
    .range_x   = CURRENT_TOUCH_X_RANGE,
    .range_y   = CURRENT_TOUCH_Y_RANGE,
    .type      = RT_TOUCH_TYPE_CAPACITANCE,
    .vendor    = RT_TOUCH_VENDOR_FT,
};

const struct rt_touch_config rt_touch_config = {
    .dev_name     = CURRENT_TOUCH_DEVICE_NAME,
    .irq_pin.mode = CURRENT_TOUCH_IRQ_MODE,
    .irq_pin.pin  = CURRENT_TOUCH_IRQ_PIN,
    .user_data    = RT_NULL,
};


static rt_err_t hl_i2c_write_reg(struct rt_i2c_bus_device* bus, rt_uint16_t reg, rt_uint16_t* data, rt_uint16_t datalen)
{
	rt_err_t	ret;
    rt_uint8_t        buf[4];
    struct rt_i2c_msg msgs[2];

    buf[1] = (reg >> 8);        // reg
    buf[0] = (reg & 0xFF);      //

	if(data != RT_NULL){
		buf[3] = (data[0] >> 8);    // data
		buf[2] = (data[0] & 0xFF);  //
	}

    msgs[0].addr  = ZTW523A_DEVICE_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = buf;
    msgs[0].len   = datalen;
    /*
    msgs[1].addr  = ZTW523A_DEVICE_ADDRESS;
    msgs[1].flags = RT_I2C_WR | RT_I2C_NO_START;
    msgs[1].buf   = &buf[2];
    msgs[1].len   = 2;
    */
    // 调用I2C设备接口传输数据
    if (rt_i2c_transfer(bus, msgs, 1) == 1){
        return HL_SUCCESS;
	}
    else{
        return HL_FAILED;
	}
}

static rt_err_t hl_i2c_read_reg(struct rt_i2c_bus_device* bus, rt_uint16_t reg, rt_uint8_t* rbuf, rt_uint16_t buflen)
{
    rt_uint8_t        buf[4] = { 0, 0, 0, 0 };
    struct rt_i2c_msg msgs[2];

    buf[1] = (reg >> 8);      // reg
    buf[0] = (reg & 0x00FF);  //

    msgs[0].addr  = ZTW523A_DEVICE_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = buf;
    msgs[0].len   = 2;

    msgs[1].addr  = ZTW523A_DEVICE_ADDRESS;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = rbuf;
    msgs[1].len   = buflen;

    // 调用I2C设备接口传输数据
    if (rt_i2c_transfer(bus, msgs, 2) == 2) {
        return HL_SUCCESS;
    } else
        return HL_FAILED;
}

static uint8_t hl_drv_ztw523a_write_reg(uint16_t reg, uint16_t data)
{
    uint8_t ret = HL_SUCCESS;
    ret         = hl_i2c_write_reg(i2c_bus, reg, &data, 4);
    return ret;
}

static uint8_t hl_drv_ztw523a_write_cmd(uint16_t reg)
{
    uint8_t ret = HL_SUCCESS;
    ret         = hl_i2c_write_reg(i2c_bus, reg, RT_NULL, 2);
    return ret;
}

static uint8_t hl_drv_ztw523a_read_data(uint16_t reg, uint8_t* rbuf, uint8_t buflen)
{
    uint8_t ret = HL_SUCCESS;
    ret         = hl_i2c_read_reg(i2c_bus, reg,rbuf, buflen);
    return ret;
}

static void hl_drv_msleep(unsigned long msec)
{
    rt_thread_mdelay(msec);
}

void hl_drv_ztw523a_power_control(bool enable)
{
    /*reset 设置为输出*/
    rt_pin_mode(CURRENT_TOUCH_RESET_PIN, CURRENT_TOUCH_RESET_MODE);
    rt_pin_write(CURRENT_TOUCH_RESET_PIN, CURRENT_TOUCH_RESET_PIN_H);
    if (enable) {
        /*reset 高低高*/
        hl_drv_msleep(1);
        rt_pin_write(CURRENT_TOUCH_RESET_PIN, CURRENT_TOUCH_RESET_PIN_H);
        hl_drv_msleep(10);
        rt_pin_write(CURRENT_TOUCH_RESET_PIN, CURRENT_TOUCH_RESET_PIN_L);
        hl_drv_msleep(100);
        rt_pin_write(CURRENT_TOUCH_RESET_PIN, CURRENT_TOUCH_RESET_PIN_H);
        hl_drv_msleep(CHIP_ON_DELAY);
    } else {
        rt_pin_write(CURRENT_TOUCH_RESET_PIN, CURRENT_TOUCH_RESET_PIN_L); /*reset 拉低防止漏电*/
        hl_drv_msleep(CHIP_OFF_DELAY);
    }
}

static bool hl_drv_ztw523a_power_sequence(void) /*软件的上电序列*/
{
    int ret = 0;

    ret = hl_drv_ztw523a_write_reg(0xc000, 0x0001); /*往0xc000寄存器地址中写入0x0001值*/
    if (ret != HL_SUCCESS) {
        LOG_E("power sequence error (vendor cmd enable)\n");
        return HL_FAILED;
    }
    hl_drv_msleep(1);

    ret = hl_drv_ztw523a_read_data(0xcc00, (uint8_t*)&chip_code, 2); /*读0xcc00地址的值,芯片的ID */
    if (ret != HL_SUCCESS) {
        LOG_E("fail to read chip code\n");
        return HL_FAILED;
    }
    hl_drv_msleep(1);
    LOG_D("chip code = 0x%X\n", chip_code); /*log打印出来  uint16_t chip_code*/

    ret = hl_drv_ztw523a_write_cmd(0xc004); /*直接写入0xc004 command,不需要再写寄存器的值*/
    if (ret != HL_SUCCESS) {
        LOG_E("power sequence error (intn clear)\n");
        return HL_FAILED;
    }
    hl_drv_msleep(1);

    ret = hl_drv_ztw523a_write_reg(0xc002, 0x0001);
    if (ret != HL_SUCCESS) {
        LOG_E("power sequence error (nvm init)\n");
        return HL_FAILED;
    }
    hl_drv_msleep(2);
    ret = hl_drv_ztw523a_write_reg(0xc001, 0x0001);
    if (ret != HL_SUCCESS) {
        LOG_E("power sequence error (program start)\n");
        return HL_FAILED;
    }
    hl_drv_msleep(FIRMWARE_ON_DELAY); /*delay*/
    LOG_D("ztw523a_power_sequence PASS.\n");

    return HL_FAILED;
}

static bool hl_drv_ztw523a_init(void) /*初始化芯片的一些寄存器,寄存器地址的宏定义在后面*/
{
    int      i;
    uint16_t firmware_version;
    uint16_t reg_data_version;

    uint32_t fw_size = 0;
    int      ret     = 0;

    for (i = 0; i < 10; i++) {
        if (hl_drv_ztw523a_write_cmd(ZINITIX_SWRESET_CMD) == HL_SUCCESS)
            break;
        hl_drv_msleep(10);
    }
    ret = hl_drv_ztw523a_read_data(ZINITIX_FIRMWARE_VERSION, (uint8_t*)&firmware_version, 2); /*固件主版本号u16 firmware_version;*/
    if (ret != HL_SUCCESS) {
        LOG_E("fail to read FIRMWARE_VERSION\n");
        goto fail_init;
    }
    LOG_D(" touch FW version = %d\r\n", firmware_version);

    ret = hl_drv_ztw523a_read_data(ZINITIX_DATA_VERSION_REG, (uint8_t*)&reg_data_version,
                       2); /*固件寄存器版本号u16 reg_data_version;*/
    if (ret != HL_SUCCESS) {
        LOG_E("fail to read reg version\n");
        goto fail_init;
    }
    LOG_D(" touch register version = %X\r\n", reg_data_version);

    ret = hl_drv_ztw523a_write_reg(ZINITIX_INITIAL_TOUCH_MODE, TOUCH_POINT_MODE);
    if (ret != HL_SUCCESS) {
        LOG_E("fail to write inital touch mode\n");
        goto fail_init;
    }

    ret = hl_drv_ztw523a_write_reg(ZINITIX_TOUCH_MODE, TOUCH_POINT_MODE);
    if (ret != HL_SUCCESS) {
        LOG_E("fail to write touh mode\n");
        goto fail_init;
    }

    ret = hl_drv_ztw523a_write_reg(ZINITIX_SUPPORTED_FINGER_NUM, (uint16_t)MAX_SUPPORTED_FINGER_NUM);
    if (ret != HL_SUCCESS) {
        LOG_E("fail to write finger num\n");
        goto fail_init;
    }

    ret = hl_drv_ztw523a_write_reg(ZINITIX_X_RESOLUTION, (uint16_t)(TPD_RES_MAX_X));
    if (ret != HL_SUCCESS) {
        LOG_E("fail to write x resolution\n");
        goto fail_init;
    }

    ret = hl_drv_ztw523a_write_reg(ZINITIX_Y_RESOLUTION, (uint16_t)(TPD_RES_MAX_Y));
    if (ret != HL_SUCCESS) {
        LOG_E("fail to write Y resolution\n");
        goto fail_init;
    }

    ret = hl_drv_ztw523a_write_cmd(ZINITIX_CALIBRATE_CMD);
    if (ret != HL_SUCCESS) {
        LOG_E("fail to write cali cmd\n");
        goto fail_init;
    }
    for (i = 0; i < 10; i++) {
        hl_drv_ztw523a_write_cmd(ZINITIX_CLEAR_INT_STATUS_CMD);
        hl_drv_msleep(1);
    }
    LOG_D("ztw523a successfully initialized\r\n");
    return HL_FAILED;

fail_init:
    LOG_D("ztw523a failed initialized\r\n");

    return HL_FAILED;
}

static rt_size_t hl_drv_ztw523a_touchinfo(struct rt_touch_device *touch, void *buf, rt_size_t touch_num)/*中断报点函数*/
{
    int ret = 0, i = 0, x = 0, y = 0;
    struct ztw523a_ts_event* ptr = (struct ztw523a_ts_event*)buf;
    /*多点触控第一个点的信息*/
    ret = hl_drv_ztw523a_read_data(ZINITIX_POINT_STATUS_REG, (uint8_t*)(&touch_info), 8);
    if (ret != HL_SUCCESS) {
        LOG_E("fail to read point reg\n");
        goto exit;
    }

    ret = hl_drv_ztw523a_read_data(ZINITIX_POINT_STATUS_REG + 4, ((uint8_t*)&touch_info + 8), 2);
    if (ret != HL_SUCCESS) {
        LOG_E("fail to read point reg\n");
        goto exit;
    }
    /*后续点的信息*/
    // for (i = 1; i < MAX_SUPPORTED_FINGER_NUM; i++) {
    //     if (zinitix_bit_test(touch_info.event_flag, i)) {

    //         ret = hl_drv_ztw523a_read_data(ZINITIX_POINT_STATUS_REG + 2 + (i * 4), (uint8_t*)(&touch_info.coord[i]),
    //                            sizeof(struct _ts_zinitix_coord));
    //         if (ret != HL_SUCCESS) {
    //             LOG_E("fail to read point reg\n");
    //             goto exit;
    //         }
    //     }
    // }
    // ztw_printf("touch_info.status=0x%x\n", touch_info.status);
    if (touch_info.status == 0x0) {
        // LOG_E("periodical_interrupt\r\n");
        goto exit;
    }
    /*down up 事件的判断*/
    for (i = 0; i < MAX_SUPPORTED_FINGER_NUM; i++) {
        if (zinitix_bit_test(touch_info.coord[i].sub_status, SUB_BIT_DOWN)
            || zinitix_bit_test(touch_info.coord[i].sub_status, SUB_BIT_MOVE)
            || zinitix_bit_test(touch_info.coord[i].sub_status, SUB_BIT_EXIST)) {

            // x = touch_info.coord[i].x;
            // y = touch_info.coord[i].y;
            // ztw_printf("finger dwon [%02d] x = %d, y = %d \r\n", i, x, y);

            // Send_down_event(i, x, y); /*平台上报down事件函数*/
            ptr->x = touch_info.coord[i].x;
            ptr->y = touch_info.coord[i].y;
            ptr->type = 1;
        } else{// if (zinitix_bit_test(touch_info.coord[i].sub_status, SUB_BIT_UP)) {
            // ztw_printf("finger [%02d] up \r\n", i);
            ptr->type = 0;
            // Send_up_event(i); /*平台上报up事件函数*/
        } 
        // else {
        //     memset(&touch_info.coord[i], 0x0, sizeof(struct _ts_zinitix_coord));
        // }
    }    
    return HL_SUCCESS;
exit:
    /*clear中断，clear后中断会被拉高*/
    hl_drv_ztw523a_write_cmd(ZINITIX_CLEAR_INT_STATUS_CMD);
    return HL_FAILED;
}

static int hl_drv_ztw523a_dev_init(void)
{
    touch_dev = rt_device_find("ZTW523A");
    memset(&touch_info, 0, sizeof(struct _ts_zinitix_point_info));
    if (touch_dev == RT_NULL) {
        LOG_E("Can't find ztw523a device ZTW523A\n");
        return HL_FAILED;
    }
    if (rt_device_init(touch_dev) != RT_EOK) {
        LOG_E("open ztw523a device init failed!");
        return HL_FAILED;
    }
    if (rt_device_open(touch_dev, RT_DEVICE_FLAG_RDONLY) != RT_EOK) {
        LOG_E("open ztw523a device failed!");
        return HL_FAILED;
    }
    return HL_SUCCESS;
}

rt_err_t hl_drv_ztw523a_dev_read_info(struct ztw523a_ts_event* touch_pos)
{
    rt_device_read(touch_dev, 0, touch_pos, 1);
    // rt_kprintf("touch_pos->type = %d\ttouch_pos->x = %d\ttouch_pos->y = %d\n", touch_pos->type, touch_pos->x, touch_pos->y);
    return 0;
}

static int hl_drv_ztw523a_i2c_init(void)
{
    /*Initialize I2C bus, you should implement it based on your platform*/
    i2c_bus = (struct rt_i2c_bus_device*)rt_device_find(ZTW523A_IIC_NAME);

    if (i2c_bus == RT_NULL) {
        LOG_E("can't find %s device!\n", ZTW523A_IIC_NAME);
        return HL_FAILED;
    }
    return HL_SUCCESS;
}

static void hl_drv_tp_Init(void)
{
	hl_drv_ztw523a_i2c_init();
    hl_drv_ztw523a_power_control(1);
    hl_drv_ztw523a_power_sequence();
    hl_drv_ztw523a_init();
}


static int hl_drv_ztw523a_dev_register(void)
{
    rt_err_t ret;
    hl_drv_tp_Init();
    touch_hand = (struct rt_touch_device*)rt_malloc(sizeof(struct rt_touch_device));
    if (touch_hand == RT_NULL) {
        LOG_E("Touch device ztw523a create fail !\n");
        return HL_FAILED;
    }
    touch_hand->config = rt_touch_config;
    touch_hand->info   = rt_touch_info;
    touch_hand->ops    = &rt_touch_ops_top;
    // touch_hand->irq_handle = fts_touch_irq_callback;

    ret = rt_hw_touch_register(touch_hand, CURRENT_TOUCH_DEVICE_NAME, RT_DEVICE_FLAG_RDONLY, RT_NULL);
    if (ret != RT_EOK) {
        LOG_E("Touch device ztw523a register fail !\n");
        LOG_E("Touch device ztw523a init fail !\n");
        return HL_FAILED;
    }
    if (hl_drv_ztw523a_dev_init() == HL_FAILED) {
        LOG_E("Touch device ztw523a init fail !\n");
        return HL_FAILED;
    }
    LOG_D("Touch device ztw523a init success !\n");
    return HL_SUCCESS;
}

INIT_DEVICE_EXPORT(hl_drv_ztw523a_dev_register);


int hl_drv_ztw523a_botton_status(void)
{
    uint16_t button;
    hl_drv_ztw523a_read_data(0x00AA,(uint8_t *)&button,2);
    switch(button)
    {
        case 1:
            button = BUTTON_DOWN;
            // LOG_D("Botton down!\n");
        break;
        case 0x100:
            button = BUTTON_UP;
            // LOG_D("Botton up!\n");
        break;
        default:
            button = BUTTON_UP;
        break;
    }
    return button;
}

#endif
