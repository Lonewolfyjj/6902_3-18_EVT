/**
 * @file hl_mod_input.c
 * @author liujie (jie.liu@hollyland-tech.com)
 * @brief 输入模块，输出（TX/RX）按键的长按、短按、双击、按下、释放，输出耳机、USB等插入事件
 * @version 1.0
 * @date 2022-09-01
 * 
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-09-01     <td>v1.0     <td>liujie     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/

#include "rtthread.h"
#include "hl_mod_input.h"
#include "hl_hal_gpio.h"
#include "rtdevice.h"

/* define --------------------------------------------------------------------*/

/// 消抖时间单位：系统时间节拍数
#define JITTER_TIME 50
/// 双击间隔时间
#define DOUBLE_PRESS_TIME 400
/// 插入的消抖时间 ms
#define INSERT_DEBANCE_TIME 1000

#define TASK_SCAN_PERIOD 600

#define INPUT_THREAD_PRIORITY 25
#define INPUT_THREAD_STACK_SIZE 512
#define INPUT_THREAD_TIMESLICE 5

/// 按键定时器的响应事件 系统时钟节拍*20 = 20ms
#define INPUT_KEY_TIMER_TICKS 20

#define HL_SUCCESS 0
#define HL_FAILED 1
#define HL_PRINT rt_kprintf

enum
{
    KEYS,
    SWITCHS,
};

/* typedef -------------------------------------------------------------------*/
#if HL_GET_DEVICE_TYPE()

typedef enum _hl_input_key_e
{
    TX_PWR_KEY = 0,
    TX_PAIR_KEY,
    TX_REC_KEY,
    HL_INPUT_KEYS,
} hl_input_key_e;

typedef enum _hl_input_insert_e
{
    VBUS_DET = 0,
    MIC_DET,
    HL_INPUT_INSERT,
} hl_input_insert_e;

#else
//RX 参数
typedef enum _hl_input_key_e
{
    RX_PWR_KEY = 0,
    RX_L_VOL_KEY,
    RX_R_VOL_KEY,
    HL_INPUT_KEYS,
} hl_input_key_e;

typedef enum _hl_input_insert_e
{
    VBUS_DET = 0,
    HP_DET,
    HL_INPUT_INSERT,
} hl_input_insert_e;

#endif /* HL_GET_DEVICE_TYPE() */

/// 按键检测的参数
typedef struct _key_param_s
{
    /// 长按时间
    uint32_t long_press_time;
    /// TODO
} key_param_s;

typedef enum _hl_key_state_e
{
    KEY_STATE_IDLE = 0,
    KEY_STATE_JITTER,
    KEY_STATE_PRESS,
    KEY_STATE_DOUBLE,
    KEY_STATE_LONG,
} hl_key_state_e;

typedef enum _hl_insert_state_e
{
    INSERT_STATE_RELEASE = 0,
    INSERT_STATE_WAIT,
    INSERT_STATE_INSERTED,
} hl_insert_state_e;

typedef enum _hl_input_insert_state_e
{
    // 数据线、耳机未插入
    LINE_OUT = 0,
    // 数据线、耳机插入
    LINE_INSERT,
} hl_input_insert_state_e;
typedef enum _hl_key_press_state_e
{
    KEY_PRESS_NO = 0,
    KEY_PRESS_YES,
} hl_key_press_state_e;

/* variables -----------------------------------------------------------------*/

typedef struct _hl_time_t
{
    /// 超时器是否有效
    uint8_t activity;
    /// 已超时标志
    uint8_t timeout_flag;
    /// 超时时间(开始时间+设置的时间)
    uint32_t timeout_time;
} hl_time_t;

typedef struct _key_state_s
{
    // 最近的按键状态
    hl_key_state_e last_state;
    // 按键超时定时器
    hl_time_t timer;
    // 按下计数
    uint8_t click_cout;
} key_state_s;



typedef struct _hl_input_key_s
{
    // 按键的状态机信息
    key_state_s keystate;
    // 按键的参数
    key_param_s param;
    // 按键的触发的事件
    hl_input_key_e event;
    //新的事件触发的标志： true 事件触发
    bool trigger;
} hl_input_key_s;

typedef struct _insert_state_s
{
    // 最近的按键状态
    hl_key_state_e key_last_state;
    // 按键超时定时器
    hl_time_t key_timer;
} insert_state_s;
static rt_thread_t input_tid = RT_NULL;

typedef struct _hl_input_mod_t
{
    /// 消息队列句柄
    rt_mq_t msg_hander;
    ///  消息
    mode_to_app_msg_t msg;
} hl_input_msg_t;

#if HL_GET_DEVICE_TYPE()

static hl_gpio_pin_e hl_keys_map[HL_INPUT_KEYS]       = { GPIO_PWR_KEY, GPIO_PAIR_KEY, GPIO_REC_KEY };
static key_param_s   hl_keys_param_map[HL_INPUT_KEYS] = { {3000}, {3000}, {3000} };

static hl_gpio_pin_e hl_insert_map[HL_INPUT_INSERT] = { GPIO_VBUS_DET, GPIO_MIC_DET };
#else
static hl_gpio_pin_e hl_keys_map[HL_INPUT_KEYS]       = { GPIO_PWR_KEY, GPIO_L_VOL_KEY, GPIO_R_VOL_KEY };
static key_param_s   hl_keys_param_map[HL_INPUT_KEYS] = { {3000}, {2000}, {2000} };

static hl_gpio_pin_e hl_insert_map[HL_INPUT_INSERT] = { GPIO_VBUS_DET, GPIO_HP_DET };
#endif
static hl_input_msg_t hl_input_msg = { 0 };
/* 按键事件 */
static hl_input_key_s hl_input_keys[HL_INPUT_KEYS] = { 0 };

/* 插入事件 */
static hl_switch_event_e hl_insert_event[HL_INPUT_INSERT] = { 0 };
/* 按键的状态机信息 */
static insert_state_s insert_state[HL_INPUT_INSERT] = { 0 };

/* Private function(only *.c)  -----------------------------------------------*/
/* 输入模块相关函数 */
//static void hl_mod_input_set_param(void);
static void hl_mod_input_task(void* param);
static void hl_mod_input_send_msg(uint8_t msg_cmd, uint32_t param);

/* 按键相关函数 */
static uint8_t hl_mod_input_key_init(void);
static uint8_t hl_mod_input_key_deinit();
static void    hl_mod_input_key_scan(hl_input_key_s* key, uint8_t key_num);

static void    hl_mod_input_send_comb_key(void);

/* 插入相关函数 */
static uint8_t hl_mod_input_insert_init(void);
static uint8_t hl_mod_input_insert_deinit();
static bool    hl_mod_input_insert_scan(hl_input_insert_e insert_nums);
static void hl_mod_input_insert_send(void);
static hl_input_insert_state_e hl_mod_input_insert_read(hl_input_insert_e insert_map);

bool hl_util_timeout_set(hl_time_t* p_timer, uint32_t time)
{
    if (p_timer == NULL) {
        return false;
    }
    p_timer->timeout_time = (uint32_t)rt_tick_get() + time;
    p_timer->timeout_flag = false;
    p_timer->activity     = true;

    return true;
}

bool hl_util_timeout_close(hl_time_t* p_timer)
{
    if (p_timer == NULL) {
        return false;
    }

    p_timer->timeout_flag = false;
    p_timer->activity     = false;
    return true;
}

bool hl_util_timeout_judge(hl_time_t* p_timer)
{
    if (p_timer->activity != true) {
        return false;
    }

    if (rt_tick_get() > p_timer->timeout_time) {
        p_timer->timeout_flag = true;
        p_timer->activity     = false;
        return true;
    }

    return false;
}

static void hl_mod_input_send_msg(uint8_t msg_cmd, uint32_t param)
{
    rt_err_t res;

    hl_input_msg.msg.sender          = INPUT_MODE;
    hl_input_msg.msg.cmd             = msg_cmd;
    hl_input_msg.msg.len             = 0;
    hl_input_msg.msg.param.u32_param = param;

    res = rt_mq_send(hl_input_msg.msg_hander, (void*)(&hl_input_msg.msg), sizeof(mode_to_app_msg_t));
    if (res == -RT_EFULL) {
        HL_PRINT("INPUT_MODE msgq full");
    } else if (res == -RT_ERROR) {
        HL_PRINT("INPUT_MODE msgq err");
    }
}

#if HL_GET_DEVICE_TYPE()
// TX
static input_mod_msg_cmd_e pin_index_to_msg_cmd(uint8_t nums, uint8_t class)
{
    input_mod_msg_cmd_e res;
    switch (class) {
        case KEYS:
            switch (nums) {
                case TX_PWR_KEY:
                    res = MSG_TX_PWR_KEY;
                    break;
                case TX_PAIR_KEY:
                    res = MSG_TX_PAIR_KEY;
                    break;
                case TX_REC_KEY:
                    res = MSG_TX_REC_KEY;
                    break;
                default:
                    break;
            }
            break;
        case SWITCHS:
            switch (nums) {
                case VBUS_DET:
                    res = MSG_TX_VBUS_DET;
                    break;
                case MIC_DET:
                    break;
                    res = MSG_TX_MIC_DET;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return res;
}
static uint8_t comb_key_check(hl_input_key_e key1, hl_input_key_e key2)
{
    input_mod_msg_cmd_e res = 0xFF;
    if (key1 == TX_PAIR_KEY && key2 == TX_REC_KEY) {
        res = MSG_COMB_PAIR_REC;
    } else if (key1 == TX_REC_KEY && key2 == TX_PWR_KEY) {
        res = MSG_COMB_REC_PWR;
    } else if (key1 == TX_PAIR_KEY && key2 == TX_PWR_KEY) {
        res = MSG_COMB_PAIR_PWR;
    }
    return res;
}
#else
//RX
static input_mod_msg_cmd_e pin_index_to_msg_cmd(uint8_t nums, uint8_t class)
{
    input_mod_msg_cmd_e res;
    switch (class) {
        case KEYS:
            switch (nums) {
                case RX_PWR_KEY:
                    res = MSG_RX_PWR_KEY;
                    break;
                case RX_L_VOL_KEY:
                    res = MSG_RX_L_VOL_KEY;
                    break;
                case RX_R_VOL_KEY:
                    res = MSG_RX_R_VOL_KEY;
                    break;
                default:
                    break;
            }
            break;
        case SWITCHS:
            switch (nums) {
                case MSG_RX_VBUS_DET:
                    res = MSG_RX_VBUS_DET;
                    break;
                case MSG_RX_HP_DET:
                    break;
                    res = MSG_RX_HP_DET;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return res;
}

static uint8_t comb_key_check(hl_input_key_e key1, hl_input_key_e key2)
{
    input_mod_msg_cmd_e res = 0xFF;
    if (key1 == RX_L_VOL_KEY && key2 == RX_R_VOL_KEY) {
        res = MSG_COMB_L_R_VOL;
    }
    return res;
}
#endif

static uint8_t hl_mod_input_key_init(void)
{
    uint8_t i;

    // RX初始化dd
    for (i = 0; i < HL_INPUT_KEYS; i++) {
        if (RT_EOK != hl_hal_gpio_init(hl_keys_map[i])) {
            return HL_FAILED;
        }

        hl_input_keys[i].event                 = HL_KEY_EVENT_IDLE;
        hl_input_keys[i].trigger = false;
        hl_input_keys[i].keystate.click_cout              = 0;
        hl_input_keys[i].keystate.last_state              = KEY_STATE_IDLE;
        hl_input_keys[i].param.long_press_time = hl_keys_param_map[i].long_press_time;
        hl_util_timeout_close(&hl_input_keys[i].keystate.timer);
    }

    return HL_SUCCESS;
}

static uint8_t hl_mod_input_key_deinit()
{
    uint8_t i;

    // RX初始化dd
    for (i = 0; i < HL_INPUT_KEYS; i++) {
        if (RT_EOK != hl_hal_gpio_deinit(hl_keys_map[i])) {
            return HL_FAILED;
        }

        hl_input_keys[i].event                 = HL_KEY_EVENT_IDLE;
        hl_input_keys[i].trigger = false;
        hl_input_keys[i].keystate.click_cout              = 0;
        hl_input_keys[i].keystate.last_state              = KEY_STATE_IDLE;
        hl_input_keys[i].param.long_press_time = hl_keys_param_map[i].long_press_time;
        hl_util_timeout_close(&hl_input_keys[i].keystate.timer);
    }

    return HL_SUCCESS;
}

static bool hl_mod_input_insert_scan(hl_input_insert_e insert_nums)
{
    hl_input_insert_state_e insert_pin_state;
    bool                    update_flag = false;
    insert_state_s*         state       = &insert_state[insert_nums];

    insert_pin_state = hl_mod_input_insert_read(insert_nums);

    switch (state->key_last_state) {
        case INSERT_STATE_RELEASE:
            if (insert_pin_state == LINE_INSERT) {
                hl_util_timeout_set(&(state->key_timer), INSERT_DEBANCE_TIME);
                state->key_last_state = INSERT_STATE_WAIT;
            }

            break;
        // 按键按下
        case INSERT_STATE_WAIT:
            if (hl_util_timeout_judge(&(state->key_timer))) {
                if (insert_pin_state == LINE_INSERT) {
                    state->key_last_state = INSERT_STATE_INSERTED;
                    if (hl_insert_event[insert_nums] == HL_SWITCH_EVENT_OFF) {
                        update_flag                  = true;
                        hl_insert_event[insert_nums] = HL_SWITCH_EVENT_ON;
                    }

                } else if (insert_pin_state == LINE_OUT) {
                        state->key_last_state = INSERT_STATE_RELEASE;
                        if (hl_insert_event[insert_nums] == HL_SWITCH_EVENT_ON) {
                            update_flag                  = true;
                            hl_insert_event[insert_nums] = HL_SWITCH_EVENT_OFF;
                        }
                    }
            }
            break;

        case INSERT_STATE_INSERTED:
            if (insert_pin_state == LINE_OUT) {
                hl_util_timeout_set(&(state->key_timer), INSERT_DEBANCE_TIME);
                state->key_last_state = INSERT_STATE_WAIT;
            }
            break;
        default:
            break;
    }
    return update_flag;
}

static void hl_mod_input_send_single(void)
{
    uint8_t i;
    for (i = 0; i < HL_INPUT_KEYS; i++) {
        if (hl_input_keys[i].trigger == true) {
            hl_mod_input_send_msg(pin_index_to_msg_cmd(i, KEYS), hl_input_keys[i].trigger);
            hl_input_keys[i].trigger = true;
        }
    }
}

static void hl_mod_input_task(void* param)
{
    uint8_t i =0 ;
    hl_time_t send_period = { 0,0,0 };

    while (1) {
        for (i = 0; i < HL_INPUT_KEYS; i++) {
            hl_mod_input_key_scan(&hl_input_keys[i], i);
        }
        for (i = 0; i < HL_INPUT_INSERT; i++) {
            hl_mod_input_insert_scan(i);
        }

        // 发送消息到其他模块
        if (hl_util_timeout_judge(&send_period)) {

            hl_mod_input_send_comb_key();
            hl_mod_input_send_single();
            hl_mod_input_insert_send();
            hl_util_timeout_set(&send_period, TASK_SCAN_PERIOD);
        }
        rt_thread_mdelay(10);
    }
}

#if HL_GET_DEVICE_TYPE()
// TX
static uint8_t hl_mod_input_key_read(hl_input_key_e key_map)
{
    switch (key_map) {
        case TX_PWR_KEY:
            if (hl_hal_gpio_read(hl_keys_map[TX_PWR_KEY]) == PIN_LOW) {
                return KEY_PRESS_YES;
            }
            break;
        case TX_PAIR_KEY:
            if (hl_hal_gpio_read(hl_keys_map[TX_PAIR_KEY]) == PIN_LOW) {
                return KEY_PRESS_YES;
            }
            break;
        case TX_REC_KEY:
            if (hl_hal_gpio_read(hl_keys_map[TX_REC_KEY]) == PIN_LOW) {
                return KEY_PRESS_YES;
            }
            break;
        default:
            break;
    }
    return KEY_PRESS_NO;
}
#else
// RX
static uint8_t hl_mod_input_key_read(hl_input_key_e key_map)
{
    switch (key_map) {
        case RX_PWR_KEY:
            if (hl_hal_gpio_read(hl_keys_map[RX_PWR_KEY]) == PIN_LOW) {
                return KEY_PRESS_YES;
            }
            break;
        case RX_L_VOL_KEY:
            if (hl_hal_gpio_read(hl_keys_map[RX_L_VOL_KEY]) == PIN_LOW) {
                return KEY_PRESS_YES;
            }
            break;
        case RX_R_VOL_KEY:
            if (hl_hal_gpio_read(hl_keys_map[RX_R_VOL_KEY]) == PIN_LOW) {
                return KEY_PRESS_YES;
            }
            break;
        default:
            break;
    }
    return KEY_PRESS_NO;
}
#endif
static void hl_mod_input_send_comb_key(void)
{
    uint8_t i            = 0;
    uint8_t trigger_keys = 0;
    uint8_t key[2]       = { 0xFF, 0xFF };

    for (i = 0; i < HL_INPUT_KEYS; i++) {
        if (hl_input_keys[i].trigger == true) {
            key[trigger_keys++] = i;
            if (trigger_keys > 2)
                trigger_keys = 2;
        }
    }

    if (hl_input_keys[key[0]].event == hl_input_keys[key[1]].event && trigger_keys == 2) {
        trigger_keys = comb_key_check(key[0], key[1]);
        if (trigger_keys != 0xFF) {
            hl_input_keys[key[0]].trigger = false;
            hl_input_keys[key[1]].trigger = false;
            hl_mod_input_send_msg(trigger_keys, (uint32_t)hl_input_keys[key[1]].event);
        }
    }
}

static void hl_mod_input_key_scan(hl_input_key_s* key, uint8_t key_num)
{
    hl_key_press_state_e key_press_state;

    key_press_state = hl_mod_input_key_read(key_num);

    switch (key->keystate.last_state) {
        case KEY_STATE_IDLE:
            key->keystate.click_cout = 0;
            if (key_press_state == KEY_PRESS_YES) {
                HL_PRINT("state = %d\r\n", key->keystate.last_state);
            }

            break;

        // 消抖
        case KEY_STATE_JITTER:
            if (hl_util_timeout_judge(&(key->keystate.timer))) {
                HL_PRINT("state = %d\r\n", key->keystate.last_state);

                if (key_press_state == KEY_PRESS_YES) {

                    key->keystate.last_state = KEY_STATE_PRESS;
                    HL_PRINT("state = %d\r\n", key->keystate.last_state);
                    hl_util_timeout_set(&(key->keystate.timer), key->param.long_press_time);
                    key->event   = HL_KEY_EVENT_PRESS;
                    key->trigger = true;
                }
            }
            break;

        // 按键按下
        case KEY_STATE_PRESS:
            if (hl_util_timeout_judge(&(key->keystate.timer))) {
                key->keystate.last_state = KEY_STATE_LONG;

            } else if (KEY_PRESS_YES != key_press_state) {

                key->keystate.last_state = KEY_STATE_DOUBLE;
                hl_util_timeout_set(&(key->keystate.timer), DOUBLE_PRESS_TIME);
                HL_PRINT("state = %d\r\n", key->keystate.last_state);
            }
            break;

        // 双击判断
        case KEY_STATE_DOUBLE:
            if (!hl_util_timeout_judge(&(key->keystate.timer))) {

                if (key->keystate.click_cout == 1) {
                    key->event               = HL_KEY_EVENT_DOUBLE;  // <- 双击
                    key->trigger             = true;
                    key->keystate.click_cout = 0;
                    key->keystate.last_state = KEY_STATE_IDLE;
                }

                if (key_press_state == KEY_PRESS_YES) {

                    key->keystate.last_state = KEY_STATE_JITTER;
                    hl_util_timeout_set(&(key->keystate.timer), JITTER_TIME);
                    key->keystate.click_cout++;
                }
            } else if (key_press_state == KEY_PRESS_NO) {
                key->keystate.last_state = KEY_STATE_IDLE;
                key->event               = HL_KEY_EVENT_SHORT;  // <- 单击
                key->trigger             = true;
                key->keystate.click_cout = 0;
            }
            break;

        // 长按判断
        case KEY_STATE_LONG:
            if (key_press_state != KEY_PRESS_YES) {
                key->event               = HL_KEY_EVENT_LONG;  // <- 长按
                key->trigger             = true;
                key->keystate.last_state = KEY_STATE_IDLE;
            }
            break;
        default:
            break;
    }
}
#if HL_GET_DEVICE_TYPE()
static hl_input_insert_state_e hl_mod_input_insert_read(hl_input_insert_e insert_map)
{
    switch (insert_map) {
        case VBUS_DET:
            if (hl_hal_gpio_read(hl_insert_map[VBUS_DET]) == PIN_HIGH) {
                return LINE_INSERT;
            }
            break;
        case MIC_DET:
            if (hl_hal_gpio_read(hl_insert_map[MIC_DET]) == PIN_LOW) {
                return LINE_INSERT;
            }
            break;
        default:
            break;
    }
    return LINE_OUT;
}
#else
static hl_input_insert_state_e hl_mod_input_insert_read(hl_input_insert_e insert_map)
{
    switch (insert_map) {
        case VBUS_DET:
            if (hl_hal_gpio_read(hl_insert_map[VBUS_DET]) == PIN_HIGH) {
                return LINE_INSERT;
            }
            break;
        case HP_DET:
            if (hl_hal_gpio_read(hl_insert_map[HP_DET]) == PIN_LOW) {
                return LINE_INSERT;
            }
            break;
        default:
            break;
    }
    return LINE_OUT;
}
#endif


static void hl_mod_input_insert_send(void)
{
    uint8_t i = 0;

    for (i = 0; i < HL_INPUT_INSERT; i++) {
        if (true == hl_mod_input_insert_scan(i)) {
            hl_mod_input_send_msg(pin_index_to_msg_cmd(i, SWITCHS), hl_insert_event[i]);
        }
    }
}

static uint8_t hl_mod_input_insert_init(void)
{
    uint8_t i;

    for (i = 0; i < HL_INPUT_INSERT; i++) {
        if (RT_EOK != hl_hal_gpio_init(hl_insert_map[i])) {
            return HL_FAILED;
        }
    }
    rt_memset((uint8_t*)insert_state, 0, HL_INPUT_INSERT);

    return HL_SUCCESS;
}

static uint8_t hl_mod_input_insert_deinit()
{
    uint8_t i;

    for (i = 0; i < HL_INPUT_INSERT; i++) {
        if (RT_EOK != hl_hal_gpio_deinit(hl_insert_map[i])) {
            return HL_FAILED;
        }
    }
    rt_memset((uint8_t*)insert_state, 0, HL_INPUT_INSERT);

    return HL_SUCCESS;
}
/* Exported functions --------------------------------------------------------*/

uint8_t hl_mod_input_init(void* msg_hander)
{
    rt_memset((uint8_t*)hl_input_keys, 0, HL_INPUT_KEYS * sizeof(hl_input_key_s));
    hl_input_msg.msg_hander = (rt_mq_t)msg_hander;

    if (hl_input_msg.msg_hander == NULL) {
        HL_PRINT("msghander err!");
        return HL_FAILED;
    }

    rt_memset(&hl_input_msg.msg, 0, sizeof(hl_input_msg.msg));

    if (HL_SUCCESS != hl_mod_input_key_init()) {
        HL_PRINT("key init err!");
        return HL_FAILED;
    }

    if (HL_SUCCESS != hl_mod_input_insert_init()) {
        HL_PRINT("insert init err!");
        return HL_FAILED;
    }

    input_tid = rt_thread_create("input_thread", \
                                hl_mod_input_task, \
                                RT_NULL, \
                                INPUT_THREAD_STACK_SIZE, \
                                INPUT_THREAD_PRIORITY, \
                                INPUT_THREAD_TIMESLICE);

    if (input_tid != RT_NULL) {
        if (input_tid->error != RT_EOK) {
            HL_PRINT("input thread init err!");
            rt_thread_delete(input_tid);
        }
    } else {
        HL_PRINT("input thread init err!");
        return HL_FAILED;
    }

    return HL_SUCCESS;
}

uint8_t hl_mod_input_deinit(void)
{
    if (HL_SUCCESS != hl_mod_input_key_deinit()) {
        HL_PRINT("key deinit err!");
        return HL_FAILED;
    }

    if (HL_SUCCESS != hl_mod_input_insert_deinit()) {
        HL_PRINT("insert deinit err!");
        return HL_FAILED;
    }

    if (input_tid != RT_NULL) {
        rt_thread_delete(input_tid);
    }

    rt_memset((uint8_t*)hl_input_keys, 0, HL_INPUT_KEYS * sizeof(hl_input_key_s));

    return HL_SUCCESS;
}
/*
 * EOF
 */
