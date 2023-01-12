#include "hl_drv_ztw523a.h"
#include "hl_drv_touch_firmware.h"
#if ! HL_IS_TX_DEVICE()
#include "touch.h"
#include "hal_pinctrl.h"
#include <rtthread.h>
#include <rtdevice.h>
#include "string.h"

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
static uint8_t 					 tp_flag 	 = 0; 
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

static rt_err_t hl_i2c_write_data(struct rt_i2c_bus_device* bus, rt_uint16_t reg, rt_uint8_t* data, rt_uint16_t datalen)
{
	rt_err_t	ret;
    // rt_uint8_t        buf[4];
    struct rt_i2c_msg msgs[2];

    rt_uint8_t  * buf = (rt_uint8_t  *)rt_malloc(datalen + 4);

    rt_memset(buf,0,datalen + 4);

    buf[1] = (reg >> 8);        // reg
    buf[0] = (reg & 0xFF);      //

    rt_memcpy(&buf[2],data,datalen);

	// if(data != RT_NULL){
	// 	buf[3] = (data[0] >> 8);    // data
	// 	buf[2] = (data[0] & 0xFF);  //
	// }

    msgs[0].addr  = ZTW523A_DEVICE_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = buf;
    msgs[0].len   = datalen+2;
    /*
    msgs[1].addr  = ZTW523A_DEVICE_ADDRESS;
    msgs[1].flags = RT_I2C_WR | RT_I2C_NO_START;
    msgs[1].buf   = &buf[2];
    msgs[1].len   = 2;
    */
    // 调用I2C设备接口传输数据
    if (rt_i2c_transfer(bus, msgs, 1) == 1){
        rt_free(buf);
        return HL_SUCCESS;
	}
    else{
        rt_free(buf);
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

static uint8_t hl_drv_ztw523a_write_data(uint16_t reg, uint8_t * data,uint16_t data_len)
{
    uint8_t ret = HL_SUCCESS;
    ret         = hl_i2c_write_data(i2c_bus, reg, data, data_len);
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








#if TOUCH_ONESHOT_UPGRADE

static bool ts_check_need_upgrade(uint16_t curRegVersion)
{
	uint16_t	newRegVersion;
	
	newRegVersion = (uint16_t) (m_firmware_data[60] | (m_firmware_data[61]<<8));
	
	rt_kprintf("cur reg data version = 0x%x, new reg data version = 0x%x\n",curRegVersion, newRegVersion);
	
	if (curRegVersion < newRegVersion)
		return true;
	
	return false;
}

static bool ts_hw_calibration(void)
{
	uint16_t	chip_eeprom_info;
	int time_out = 0;
	int ret=0;
	
	ret=hl_drv_ztw523a_write_reg(ZINITIX_TOUCH_MODE, 0x07);
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("fail to set cali touch mode\n");
		return false;
	}
	hl_drv_msleep(10);
	hl_drv_ztw523a_write_cmd(ZINITIX_CLEAR_INT_STATUS_CMD);
	hl_drv_msleep(10);
	hl_drv_ztw523a_write_cmd(ZINITIX_CLEAR_INT_STATUS_CMD);	
	hl_drv_msleep(50);
	hl_drv_ztw523a_write_cmd(ZINITIX_CLEAR_INT_STATUS_CMD);	
	hl_drv_msleep(10);
	ret=hl_drv_ztw523a_write_cmd(ZINITIX_CALIBRATE_CMD);
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("fail to write cali cmd\n");
		return false;
	}
	hl_drv_ztw523a_write_cmd(ZINITIX_CLEAR_INT_STATUS_CMD);
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("fail to clear int status \n");
		return false;
	}
	hl_drv_msleep(10);
	hl_drv_ztw523a_write_cmd(ZINITIX_CLEAR_INT_STATUS_CMD);
	
	/* wait for h/w calibration*/
	do {
		hl_drv_msleep(500);
		hl_drv_ztw523a_write_cmd(ZINITIX_CLEAR_INT_STATUS_CMD);			
		ret=hl_drv_ztw523a_read_data(ZINITIX_EEPROM_INFO_REG,(uint8_t *)&chip_eeprom_info, 2) ;
		if(ret!=HL_SUCCESS)
		{
			rt_kprintf("fail to read eeprom info\n");
			return false;
		}
		rt_kprintf("touch eeprom info = 0x%04X\r\n",chip_eeprom_info);

		if (!zinitix_bit_test(chip_eeprom_info, 0))
			break;
		if(time_out++ == 4)
		{
			hl_drv_ztw523a_write_cmd(ZINITIX_CALIBRATE_CMD);
			hl_drv_msleep(10);
			hl_drv_ztw523a_write_cmd(ZINITIX_CLEAR_INT_STATUS_CMD);						
			rt_kprintf("h/w calibration retry timeout.\n");
		}
		if(time_out++ > 10)
		{
			rt_kprintf("[error] h/w calibration timeout.\n");
			break;						
		}
	}while (1);
	
	hl_drv_ztw523a_write_reg(ZINITIX_INITIAL_TOUCH_MODE, TOUCH_POINT_MODE);
	hl_drv_ztw523a_write_reg(ZINITIX_TOUCH_MODE, TOUCH_POINT_MODE);
	if(ic_int_mask!=0)
	{
		ret=hl_drv_ztw523a_write_reg( ZINITIX_INT_ENABLE_FLAG, ic_int_mask);
		if(ret!=HL_SUCCESS)
		{
			rt_kprintf("fail to write int flag\n");
			return false;
		}
	}
	hl_drv_ztw523a_write_reg(0xc003, 0x0001);
	hl_drv_ztw523a_write_reg(0xc104, 0x0001);
	hl_drv_msleep(1);
	
	ret=hl_drv_ztw523a_write_cmd(ZINITIX_SAVE_CALIBRATION_CMD) ;
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("fail to write save cali cmd\n");
		return false;
	}
	hl_drv_msleep(1000);	
	hl_drv_ztw523a_write_reg( 0xc003, 0x0000);
	hl_drv_ztw523a_write_reg( 0xc104, 0x0000);
	
	return true;				
	
}


static uint8_t ts_upgrade_firmware(const uint8_t *firmware_data, uint32_t size)
{
	uint16_t flash_addr;
	uint8_t *verify_data;
	int retry_cnt = 0;
	int i;
	int page_sz = 64;
	uint16_t chip_code;
	int ret=0;
	uint8_t TC_SECTOR_SZ=8;
	
	verify_data = (uint8_t*)rt_malloc(size);
	if (verify_data == NULL) {
	rt_kprintf("cannot alloc verify buffer\n");
	return false;
	}
	
retry_upgrade:
	
	hl_drv_ztw523a_power_control(0);
	hl_drv_ztw523a_power_control(1);
	
	ret=hl_drv_ztw523a_write_reg(0xc000, 0x0001);
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("power sequence error (vendor cmd enable)\n");
		goto fail_upgrade;
	}
	hl_drv_msleep(1);
	ret=hl_drv_ztw523a_read_data(0xcc00, (uint8_t *)&chip_code, 2);
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("fail to read chip code\n");
		goto fail_upgrade;
	}
	rt_kprintf("chip code = 0x%x\n", chip_code);
	
	if((chip_code == 0xf400)) 
	{
		page_sz = 128;
		
		//size = 32*1024;
	} 
	else if(chip_code == 0xe240)
	{
		page_sz = 64;
		
		//size = 32*1024;
	}
	else if(chip_code == 0xE700 )//ZT7554
	{
	    page_sz = 128;
		
		//size = 64*1024;
	}
	else if(chip_code == 0xE548)//ZT7548
	{
	    page_sz = 64;
		
		//size = 48*1024;
	}
	else if((chip_code == 0xE538)||(chip_code == 0xE532))//ZT7532
	{
	    page_sz = 64;
		
		//size = 44*1024;
	}
	else 
	{
		page_sz = 64;
		
		//size = 24*1024;
	}
	
	hl_drv_msleep(1);
	ret=hl_drv_ztw523a_write_cmd(0xc004);
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("power sequence error (intn clear)\n");
		goto fail_upgrade;
	}
	hl_drv_msleep(1);
	
	ret=hl_drv_ztw523a_write_reg(0xc002, 0x0001) ;
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("power sequence error (nvm init)\n");
		goto fail_upgrade;
	}
	hl_drv_msleep(10);
	
	rt_kprintf("init flash\n");
	ret=hl_drv_ztw523a_write_reg(0xc003, 0x0001) ;
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("fail to write nvm vpp on\n");
		goto fail_upgrade;
	}
	hl_drv_msleep(1);
	
	
	ret=hl_drv_ztw523a_write_reg(0xc104, 0x0001) ;
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("fail to write nvm wp disable\n");
		goto fail_upgrade;
	}
	
	ret=hl_drv_ztw523a_write_cmd(ZINITIX_INIT_FLASH) ;
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("fail to init flash\n");
		goto fail_upgrade;
	}
	hl_drv_msleep(1);

	rt_kprintf("writing firmware data\n");
	for (flash_addr = 0; flash_addr < size; ) 
	{
		//rt_kprintf(KERN_ERR  "Addr:0x%04x\n", flash_addr);
		for (i = 0; i < page_sz/TC_SECTOR_SZ; i++) 
		{
			ret=hl_drv_ztw523a_write_data(ZINITIX_WRITE_FLASH,(uint8_t *)&firmware_data[flash_addr],TC_SECTOR_SZ);
			if(ret!=HL_SUCCESS)
			{
				rt_kprintf("error : write zinitix tc firmare\n");
				goto fail_upgrade;
			}
			flash_addr += TC_SECTOR_SZ;
			hl_drv_msleep(1);
		}
		hl_drv_msleep(30);	
	
	}
	hl_drv_msleep(100);
	
	ret=hl_drv_ztw523a_write_reg( 0xc003, 0x0001) ;
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("fail to write nvm vpp on\n");
		goto fail_upgrade;
	}	
	
	ret=hl_drv_ztw523a_write_reg( 0xc104, 0x0001) ;
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("fail to write nvm wp disable\n");
		goto fail_upgrade;
	}
	
	ret=hl_drv_ztw523a_write_cmd( ZINITIX_INIT_FLASH) ;
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("fail to init flash\n");
		goto fail_upgrade;
	}
	
	rt_kprintf("read firmware data\n");
	for (flash_addr = 0; flash_addr < size; ) 
	{
		for (i = 0; i < page_sz/TC_SECTOR_SZ; i++) 
		{
			//rt_kprintf("read :addr=%04x, len=%d\n", flash_addr, TC_SECTOR_SZ);
			ret=hl_drv_ztw523a_read_data(ZINITIX_READ_FLASH,(uint8_t*)&verify_data[flash_addr], TC_SECTOR_SZ);
			if(ret!=HL_SUCCESS)
			{
				rt_kprintf("error : read zinitix tc firmare\n");
				goto fail_upgrade;
			}
			flash_addr += TC_SECTOR_SZ;
		}
	}
	/* verify */
	rt_kprintf("verify firmware data\n");
	if (memcmp((uint8_t *)&firmware_data[0], (uint8_t *)&verify_data[0], size) == 0) 
	{
		rt_kprintf("upgrade finished\n");
		rt_free(verify_data);

		hl_drv_ztw523a_power_control(0);
		hl_drv_ztw523a_power_control(1);
		hl_drv_ztw523a_power_sequence();
		return true;
	}
	
	
fail_upgrade:
	
	hl_drv_ztw523a_power_control(0);
	if (retry_cnt++ < ZINITIX_INIT_RETRY_CNT) 
	{
		rt_kprintf("upgrade fail : so retry... (%d)\n", retry_cnt);
		goto retry_upgrade;		
	}
	
	if (verify_data != NULL)
	rt_free(verify_data);
	
	rt_kprintf("upgrade fail..\n");
	return false;
	
	
}


static void hl_drv_upgread_tp(void)
{
    int i; 
	uint16_t firmware_version;
	uint16_t reg_data_version;
	
	uint32_t fw_size=0;
	int ret=0;

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

    fw_size=sizeof(m_firmware_data);
	rt_kprintf("fw_size = %d KB\r\n", fw_size/1024);

	ret=hl_drv_ztw523a_read_data( ZINITIX_INT_ENABLE_FLAG,(uint8_t *)&ic_int_mask, 2) ;
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("fail to read int reg\n");
		goto fail_init;
	}
	rt_kprintf(" touch int reg = 0x%x\r\n",ic_int_mask);

	if (ts_check_need_upgrade(reg_data_version) == true)
	{
		rt_kprintf("start upgrade firmware\n");

		if(ts_upgrade_firmware(m_firmware_data,fw_size) == 0)
		goto fail_init;

		if(ts_hw_calibration() == 0)
		goto fail_init;
		/* disable chip interrupt */
		ret=hl_drv_ztw523a_write_reg( ZINITIX_INT_ENABLE_FLAG, 0);
		if(ret!=HL_SUCCESS)
		{
			rt_kprintf("fail to write int flag\n");
			goto fail_init;
		}
		ret=hl_drv_ztw523a_read_data( ZINITIX_FIRMWARE_VERSION,(uint8_t *)&firmware_version, 2);/*固件主版本号u16 firmware_version;*/
		if(ret!=HL_SUCCESS)
		{
			rt_kprintf("fail to read FIRMWARE_VERSION\n");
			goto fail_init;
		}
		rt_kprintf(" touch FW version = %d\r\n",firmware_version);

		ret=hl_drv_ztw523a_read_data( ZINITIX_DATA_VERSION_REG,(uint8_t *)&reg_data_version, 2) ;/*固件寄存器版本号u16 reg_data_version;*/
		if(ret!=HL_SUCCESS)
		{
			rt_kprintf("fail to read reg version\n");
			goto fail_init;
		}
		rt_kprintf(" touch register version = %d\r\n",reg_data_version);

	}

    

    ret=hl_drv_ztw523a_write_reg( ZINITIX_INITIAL_TOUCH_MODE, TOUCH_POINT_MODE);
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("fail to write inital touch mode\n");
		goto fail_init;
	}
	
	ret=hl_drv_ztw523a_write_reg( ZINITIX_TOUCH_MODE, TOUCH_POINT_MODE);
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("fail to write touh mode\n");
		goto fail_init;
	}
	
	ret=hl_drv_ztw523a_write_reg(ZINITIX_SUPPORTED_FINGER_NUM,(uint16_t)MAX_SUPPORTED_FINGER_NUM);
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("fail to write finger num\n");
		goto fail_init;
	}
	
	ret=hl_drv_ztw523a_write_reg(ZINITIX_X_RESOLUTION,(uint16_t)(TPD_RES_MAX_X));
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("fail to write x resolution\n");
		goto fail_init;
	}
	
	ret=hl_drv_ztw523a_write_reg(ZINITIX_Y_RESOLUTION,(uint16_t)(TPD_RES_MAX_Y));
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("fail to write Y resolution\n");
		goto fail_init;
	}
		
	ret=hl_drv_ztw523a_write_cmd( ZINITIX_CALIBRATE_CMD);
	if(ret!=HL_SUCCESS)
	{
		rt_kprintf("fail to write cali cmd\n");
		goto fail_init;
	}
#if TOUCH_ONESHOT_UPGRADE
	ret=hl_drv_ztw523a_write_reg( ZINITIX_INT_ENABLE_FLAG, ic_int_mask);
		if(ret!=HL_SUCCESS)
		{
			rt_kprintf("fail to write int flag\n");
			goto fail_init;
		}
#endif
	for (i = 0; i < 10; i++) 
	{
		hl_drv_ztw523a_write_cmd(ZINITIX_CLEAR_INT_STATUS_CMD);
		hl_drv_msleep(1);
	}
	rt_kprintf("successfully initialized\r\n");
	return true;

fail_init:
	rt_kprintf("failed initialized\r\n");

	return false;
}

// MSH_CMD_EXPORT(hl_drv_upgread_tp, run hl_drv_upgread_tp);
#endif





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
        LOG_E("fail to read point reg 1\n");
        goto exit;
    }

    ret = hl_drv_ztw523a_read_data(ZINITIX_POINT_STATUS_REG + 4, ((uint8_t*)&touch_info + 8), 2);
    if (ret != HL_SUCCESS) {
        LOG_E("fail to read point reg 2\n");
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

    hl_drv_ztw523a_write_cmd(ZINITIX_CLEAR_INT_STATUS_CMD);
    return HL_SUCCESS;
exit:
    /*clear中断，clear后中断会被拉高*/

	hl_drv_ztw523a_power_control(1);
    hl_drv_ztw523a_power_sequence();
    hl_drv_ztw523a_init();
    // hl_drv_ztw523a_write_cmd(ZINITIX_CLEAR_INT_STATUS_CMD);
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
#if TOUCH_ONESHOT_UPGRADE == 0
    rt_device_read(touch_dev, 0, touch_pos, 1);
#else
	if(tp_flag != 2){
		rt_device_read(touch_dev, 0, touch_pos, 1);
	}
	if(tp_flag == 1){
		rt_kprintf("touch_pos->type = %d\ttouch_pos->x = %d\ttouch_pos->y = %d\n", touch_pos->type, touch_pos->x, touch_pos->y);
	}    
#endif
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
    uint16_t button = BUTTON_UP;
	if(tp_flag != 2){
		hl_drv_ztw523a_read_data(0x00AA,(uint8_t *)&button,2);
		switch(button)
		{
			case 1:
				button = BUTTON_DOWN;
			break;
			case 0x100:
				button = BUTTON_UP;
			break;
			default:
				button = BUTTON_UP;
			break;
		}
		if(tp_flag == 1){
			rt_kprintf("button = %d\n", button);
		}
	}    
    return button;
}


void touch_log_ctl(int argc, char** argv)
{
	uint16_t read_reg,reg_addr;
	if (!strcmp("open", argv[1])) {
       tp_flag = 1;
    }else if (!strcmp("init", argv[1])){
		tp_flag = 0;
		hl_drv_tp_Init();
	}else if (!strcmp("upgrade", argv[1])){
		tp_flag = 2;
		hl_drv_upgread_tp();
	}else if(!strcmp("read", argv[1])){
		reg_addr = atoi(argv[2]);
		hl_drv_ztw523a_read_data(reg_addr,(uint8_t *)&read_reg,2);
		rt_kprintf("Addr[0x%X] = %X\n",reg_addr,read_reg);
	}else {
       tp_flag = 0;
    }
}

MSH_CMD_EXPORT(touch_log_ctl, run touch_log_ctl);

#endif
