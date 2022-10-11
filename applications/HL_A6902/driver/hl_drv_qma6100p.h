#ifndef __HL_DRV_QMA6100P_H__
#define __HL_DRV_QMA6100P_H__
#include <stdbool.h>
#include <string.h>
#include "drivers/i2c.h"

#ifndef int8_t
#define		int8_t					char
#endif
#ifndef uint8_t
#define		uint8_t					unsigned char
#endif
#ifndef int16_t
#define		int16_t					short
#endif
#ifndef uint16_t
#define		uint16_t				unsigned short
#endif
#ifndef int32_t
#define		int32_t					int
#endif
#ifndef uint32_t
#define		uint32_t				unsigned int
#endif
#ifndef int64_t
#define		int64_t					long long
#endif
#ifndef uint64_t
#define		uint64_t				unsigned long long
#endif
#ifndef float32_t
#define		float32_t				float
#endif
#ifndef float64_t
#define		float64_t				double
#endif
	
	


#define HL_QMA6100_LOG		rt_kprintf
#define HL_QMA6100_ERR		rt_kprintf

//#define HL_QMA6100_QST_USE_SPI
//#define HL_QMA6100_DATA_READY
//#define HL_QMA6100_FIFO_FUNC
#define HL_QMA6100_ANY_MOTION
//#define HL_QMA6100_NO_MOTION
//#define HL_QMA6100_SIGNIFICANT_MOTION
//#define HL_QMA6100_STEPCOUNTER
//#define HL_QMA6100_STEP_INT
//#define HL_QMA6100_SIGNIFICANT_STEP_INT
//#define HL_QMA6100_TAP_FUNC
//#define HL_QMA6100_HAND_RAISE_DOWN
//#define HL_QMA6100_INT_LATCH

//#define HL_QMA6100_ODR_13
//#define HL_QMA6100_ODR_27
#define HL_QMA6100_ODR_55
//#define HL_QMA6100_ODR_100



#define HL_QMA6100_DEVICE_ID		    0xfa
#define HL_QMA6100_I2C_SLAVE_ADDR		0x12	// AD0 GND 0x12, AD0 VDD 0x13
#define HL_QMA6100_I2C_SLAVE_ADDR2		0x13	// AD0 GND 0x12, AD0 VDD 0x13
#define HL_QMA6100_FAIL					1
#define HL_QMA6100_SUCCESS				0

#define GRAVITY_EARTH_1000          	9807	// about (9.80665f)*1000   mm/s2
#define HL_QMA6100_ABS(X) 				((X) < 0 ? (-1 * (X)) : (X))

#define HL_QMA6100_DELAY				0xff
/*Register Map*/
#define HL_QMA6100_CHIP_ID		    	0x00
#define HL_QMA6100_XOUTL				0x01
#define HL_QMA6100_XOUTH				0x02
#define HL_QMA6100_YOUTL				0x03
#define HL_QMA6100_YOUTH				0x04
#define HL_QMA6100_ZOUTL				0x05
#define HL_QMA6100_ZOUTH				0x06
#define HL_QMA6100_STEP_CNT_L			0x07
#define HL_QMA6100_INT_STAT0			0x09
#define HL_QMA6100_INT_STAT1			0x0a
#define HL_QMA6100_INT_STAT2			0x0b
#define HL_QMA6100_INT_STAT3			0x0c
#define HL_QMA6100_STEP_CNT_M			0x0d
#define HL_QMA6100_FIFO_STATE			0x0e
#define HL_QMA6100_REG_RANGE			0x0f
#define HL_QMA6100_REG_BW_ODR			0x10
#define HL_QMA6100_REG_POWER_CTL		0x11
#define HL_QMA6100_STEP_SAMPLE_CNT		0x12
#define HL_QMA6100_STEP_PRECISION		0x13
#define HL_QMA6100_STEP_TIME_LOW		0x14
#define HL_QMA6100_STEP_TIME_UP			0x15
#define HL_QMA6100_INTPIN_CFG			0x20
#define HL_QMA6100_INT_CFG				0x21
#define HL_QMA6100_OS_CUST_X		    0x27
#define HL_QMA6100_OS_CUST_Y			0x28
#define HL_QMA6100_OS_CUST_Z			0x29
/*ODR SET @lower ODR*/
#define QMA6981_ODR_7808HZ				0x04
#define QMA6981_ODR_3904HZ				0x03
#define QMA6981_ODR_1952HZ				0x02
#define QMA6981_ODR_976HZ				0x01
#define QMA6981_ODR_488HZ				0x00
#define QMA6981_ODR_244HZ				0x05
#define QMA6981_ODR_122HZ				0x06
#define QMA6981_ODR_61HZ				0x07

/* Accelerometer Sensor Full Scale */
#define HL_QMA6100_RANGE_2G				0x01
#define HL_QMA6100_RANGE_4G				0x02
#define HL_QMA6100_RANGE_8G				0x04
#define HL_QMA6100_RANGE_16G			0x08
#define HL_QMA6100_RANGE_32G			0x0f


#define TAP_QUIET_TH					3//5  //8*31.25MG = 250MG
#define TAP_QUIET_TIME_30MS				(1<<7)
#define TAP_QUIET_TIME_20MS				(0<<7)
#define TAP_SHOCK_TIME_50MS				(1<<6)
#define TAP_SHOCK_TIME_75MS				(0<<6)
#define TAP_TRIPLE_WAIT_QUAD_DELAY_Y				(1<<5)
#define TAP_TRIPLE_NOT_WAIT_QUAD_DELAY_Y			(0<<5)
#define TAP_EARIN_Y						(1<<4)
#define TAP_EARIN_N						(0<<4)
#define TAP_DUR_100MS					(0)
#define TAP_DUR_150MS					(0x01)
#define TAP_DUR_200MS					(0x02)
#define TAP_DUR_250MS					(0x03)
#define TAP_DUR_300MS					(0x04)
#define TAP_DUR_400MS					(0x05)
#define TAP_DUR_500MS					(0x06)
#define TAP_DUR_700MS					(0x07)


#define TAP_AXIS_X						(0<<7)|(0<<6)
#define TAP_AXIS_Y						(0<<7)|(1<<6)
#define TAP_AXIS_Z						(1<<7)|(0<<6)
#define TAP_AXIS_XYZ					(1<<7)|(1<<6)
#define TAP_SHOCK_TH					0x05//0x08////0x0e  //0x14*31.25mg = 625mg

enum QMA6100_CTRL_CMD
{
    QMA6100_GET_EULWER_ANGLE,
};

typedef enum
{
	HL_QMA6100_DISABLE = 0,
	HL_QMA6100_ENABLE = 1
}HL_QMA6100_enable;

typedef enum
{
	HL_QMA6100_MODE_SLEEP,
	HL_QMA6100_MODE_ACTIVE,
	HL_QMA6100_MODE_MAX
}HL_QMA6100_power_mode;

typedef enum
{
	HL_QMA6100_FIFO_MODE_NONE,
	HL_QMA6100_FIFO_MODE_FIFO,
	HL_QMA6100_FIFO_MODE_STREAM,
	HL_QMA6100_FIFO_MODE_BYPASS,
	HL_QMA6100_FIFO_MODE_MAX
}HL_QMA6100_fifo_mode;

typedef enum
{
	HL_QMA6100_TAP_SINGLE = 0x80,
	HL_QMA6100_TAP_DOUBLE = 0x20,
	HL_QMA6100_TAP_TRIPLE = 0x10,
	HL_QMA6100_TAP_QUARTER = 0x01,
	HL_QMA6100_TAP_MAX = 0xff
}HL_QMA6100_tap;

typedef enum
{
	HL_QMA6100_MAP_INT1,
	HL_QMA6100_MAP_INT2,
	HL_QMA6100_MAP_INT_NONE
}HL_QMA6100_int_map;

typedef enum
{
	HL_QMA6100_MCLK_500K = 0x00,
	HL_QMA6100_MCLK_333K = 0x01,
	HL_QMA6100_MCLK_200K = 0x02,
	HL_QMA6100_MCLK_100K = 0x03,
	HL_QMA6100_MCLK_50K = 0x04,
	HL_QMA6100_MCLK_20K = 0x05,
	HL_QMA6100_MCLK_10K = 0x06,
	HL_QMA6100_MCLK_5K = 0x07,
	HL_QMA6100_MCLK_RESERVED = 0xff
}HL_QMA6100_mclk;

typedef enum
{
	HL_QMA6100_DIV_512 = 0x00,
	HL_QMA6100_DIV_256 = 0x01,
	HL_QMA6100_DIV_128 = 0x02,
	HL_QMA6100_DIV_64 = 0x03,
	HL_QMA6100_DIV_32 = 0x04,
	HL_QMA6100_DIV_1024 = 0x05,
	HL_QMA6100_DIV_2048 = 0x06,
	HL_QMA6100_DIV_4096 = 0x07,
	HL_QMA6100_DIV_RESERVED = 0xff
}HL_QMA6100_div;

typedef enum
{
	HL_QMA6100_LPF_0 = (0x00<<5),
	HL_QMA6100_LPF_2 = (0x01<<5),
	HL_QMA6100_LPF_4 = (0x02<<5),
	HL_QMA6100_LPF_16 = (0x03<<5),
	HL_QMA6100_LPF_RESERVED = 0xff
}HL_QMA6100_lpf;

typedef enum
{
  HL_QMA6100_STEP_LPF_0 = (0x00<<6),
  HL_QMA6100_STEP_LPF_2 = (0x01<<6),
  HL_QMA6100_STEP_LPF_4 = (0x02<<6),
  HL_QMA6100_STEP_LPF_8 = (0x03<<6),
  HL_QMA6100_STEP_LPF_RESERVED = 0xff
}HL_QMA6100_step_lpf;

typedef enum
{
  HL_QMA6100_STEP_AXIS_ALL = 0x00,
  HL_QMA6100_STEP_AXIS_YZ = 0x01,
  HL_QMA6100_STEP_AXIS_XZ = 0x02,
  HL_QMA6100_STEP_AXIS_XY = 0x03,
  HL_QMA6100_STEP_AXIS_RESERVED = 0xff
}HL_QMA6100_step_axis;

typedef enum
{
  HL_QMA6100_STEP_START_0 = 0x00,
  HL_QMA6100_STEP_START_4 = 0x20,
  HL_QMA6100_STEP_START_8 = 0x40,
  HL_QMA6100_STEP_START_12 = 0x60,
  HL_QMA6100_STEP_START_16 = 0x80,
  HL_QMA6100_STEP_START_24 = 0xa0,
  HL_QMA6100_STEP_START_32 = 0xc0,
  HL_QMA6100_STEP_START_40 = 0xe0,
  HL_QMA6100_STEP_START_RESERVED = 0xff
}HL_QMA6100_step_start_cnt;


typedef struct
{
    int16_t sign[3];
    uint16_t map[3];
}qst_convert_t;

typedef struct {
    int32_t pitch;
    int32_t roll;
    int z
}euler_angle_t;

typedef struct
{
	uint8_t					slave;
	uint8_t					chip_id;
	int32_t					lsb_1g;
	uint8_t					layout;
	qst_convert_t			cvt;
	HL_QMA6100_fifo_mode	fifo_mode;
	int32_t					fifo_len;	
	int32_t					raw[3];

}HL_QMA6100_data_t;

// extern int32_t HL_QMA6100_writereg(uint8_t reg_add,uint8_t reg_dat);
// extern int32_t HL_QMA6100_readreg(uint8_t reg_add,uint8_t *buf,uint16_t num);
// extern uint8_t HL_QMA6100_chip_id(void);
// extern int32_t HL_QMA6100_init(void);
// extern int32_t HL_QMA6100_initialize(void);
// extern int32_t HL_QMA6100_set_range(unsigned char range);
// extern int32_t HL_QMA6100_set_mode_odr(int32_t mode, int32_t mclk, int32_t div, int32_t lpf);
// extern int32_t HL_QMA6100_read_raw_xyz(int32_t *data);
// extern int32_t HL_QMA6100_read_acc_xyz(int32_t *accData);
// #if defined(HL_QMA6100_DATA_READY)
// extern void HL_QMA6100_drdy_config(int32_t int_map, int32_t enable);
// #endif
// #if defined(HL_QMA6100_FIFO_FUNC)
// extern void HL_QMA6100_fifo_config(int32_t int_map, int32_t enable);
// extern int32_t HL_QMA6100_read_fifo(uint8_t *fifo_buf);
// extern void HL_QMA6100_exe_fifo(uint8_t *fifo_buf);
// extern uint8_t* HL_QMA6100_get_fifo(void);
// #endif

// #if defined(HL_QMA6100_STEPCOUNTER)
// extern uint32_t HL_QMA6100_read_stepcounter(void);
// extern void HL_QMA6100_stepcounter_config(int32_t enable);
// #if defined(HL_QMA6100_STEP_INT)
// extern void HL_QMA6100_step_int_config(int32_t int_map, int32_t enable);
// #endif
// #if defined(HL_QMA6100_SIGNIFICANT_STEP_INT)
// extern void HL_QMA6100_sigstep_int_config(int32_t int_map, int32_t enable);
// #endif
// #endif

// #if defined(HL_QMA6100_ANY_MOTION)
// extern void HL_QMA6100_anymotion_config(int32_t int_map, int32_t enable);
// #endif
// #if defined(HL_QMA6100_NO_MOTION)
// extern void HL_QMA6100_nomotion_config(int32_t int_map, int32_t enable);
// #endif

// extern void HL_QMA6100_irq_hdlr(void);

uint8_t hl_drv_qma6100p_init(void);

uint8_t hl_drv_qma6100p_deinit(void);

uint8_t hl_drv_qma6100p_io_ctrl(uint8_t cmd, void * ptr, uint16_t len);

uint8_t HL_QMA6100P_get_Euler_Angle(int32_t * ptr);

uint8_t HL_QMA6100P_get();

#endif