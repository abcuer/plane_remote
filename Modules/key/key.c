#include "key.h"
#include "remote.h"
#include "led.h"
#include "beep.h"
#include "bsp_gpio.h"

static KEYInstance key[KEY_NUM];

/**
 * @brief 获取原始物理电平对应的按键状态
 */
static KEY_State_e GetKeyState(KEY_Type_e KeyType)
{
    KEYInstance *ins = &key[KeyType];
    GPIO_PinState pinState = HAL_GPIO_ReadPin(ins->StaticParam.GPIO_Port, ins->StaticParam.GPIO_Pin);
    
    // 如果物理电平等于有效电平，则判定为按下
    return (pinState == (GPIO_PinState)ins->StaticParam.PressLevel) ? KEY_PRESSED : KEY_RELEASED;
}

/**
 * @brief 内部初始化函数
 */
static void Key_Init(KeyStaticParam_s *config, KEY_Type_e KeyType)
{
    if (KeyType >= KEY_NUM) return;
    
    // 1. 复制静态配置
    key[KeyType].StaticParam = *config;
    GPIO_Input(config->GPIO_Port, config->GPIO_Pin, GPIO_MODE_INPUT);
    
    // 3. 初始化运行参数
    key[KeyType].RunningParam.LastState = KEY_RELEASED;
    key[KeyType].RunningParam.LastTick = 0;
}

/**
 * @brief 获取按键触发事件
 * @return 1: 有效按下（已消抖）, 0: 无事件
 */
uint8_t Key_GetNum(KEY_Type_e KeyType)
{
    if (KeyType >= KEY_NUM) return 0;
    
    KEYInstance *instance = &key[KeyType];
    KEY_State_e currentState = GetKeyState(KeyType);
    uint8_t key_event = 0;
    if (currentState == KEY_PRESSED && instance->RunningParam.LastState == KEY_RELEASED)
    {
        if (GetKeyState(KeyType) == KEY_PRESSED)
        {
            key_event = 1;
        }
    }
    instance->RunningParam.LastState = currentState;
    return key_event;
}

/**
 * @brief 按键设备总初始化
 */
void KeyDeviceInit(void)
{
    KeyStaticParam_s config;

    // USER KEY 配置
    config.GPIO_Port = GPIOB;
    config.GPIO_Pin = K1_Pin;
    config.PressLevel = KEY_LOW_LEVEL_PRESS;
    config.Mode = KEY_MODE_NORMAL;
    Key_Init(&config, K1);

    config.GPIO_Port = GPIOB;
    config.GPIO_Pin = K2_Pin;
    config.PressLevel = KEY_LOW_LEVEL_PRESS;
    config.Mode = KEY_MODE_NORMAL;
    Key_Init(&config, K2);

    config.GPIO_Port = GPIOB;
    config.GPIO_Pin = K3_FRONT_Pin;
    config.PressLevel = KEY_LOW_LEVEL_PRESS;
    config.Mode = KEY_MODE_NORMAL;
    Key_Init(&config, K3_FRONT);

    config.GPIO_Port = GPIOB;
    config.GPIO_Pin = K4_BACK_Pin;
    config.PressLevel = KEY_LOW_LEVEL_PRESS;
    config.Mode = KEY_MODE_NORMAL;
    Key_Init(&config, K4_BACK);

    config.GPIO_Port = GPIOB;
    config.GPIO_Pin = K5_RIGHT_Pin;
    config.PressLevel = KEY_LOW_LEVEL_PRESS;
    config.Mode = KEY_MODE_NORMAL;
    Key_Init(&config, K5_RIGHT);

    config.GPIO_Port = GPIOB;
    config.GPIO_Pin = K6_LEFT_Pin;
    config.PressLevel = KEY_LOW_LEVEL_PRESS;
    config.Mode = KEY_MODE_NORMAL;
    Key_Init(&config, K6_LEFT);    

    config.GPIO_Port = GPIOC;
    config.GPIO_Pin = K7_UP_Pin;
    config.PressLevel = KEY_LOW_LEVEL_PRESS;
    config.Mode = KEY_MODE_NORMAL;
    Key_Init(&config, K7_UP);   

    config.GPIO_Port = GPIOB;
    config.GPIO_Pin = K8_DOWN_Pin;
    config.PressLevel = KEY_LOW_LEVEL_PRESS;
    config.Mode = KEY_MODE_NORMAL;
    Key_Init(&config, K8_DOWN);  

    config.GPIO_Port = GPIOC;
    config.GPIO_Pin = K9_P_Pin;
    config.PressLevel = KEY_LOW_LEVEL_PRESS;
    config.Mode = KEY_MODE_NORMAL;
    Key_Init(&config, K9_P);      
    
    config.GPIO_Port = GPIOC;
    config.GPIO_Pin = K10_M_Pin;
    config.PressLevel = KEY_LOW_LEVEL_PRESS;
    config.Mode = KEY_MODE_NORMAL;
    Key_Init(&config, K10_M);   

    config.GPIO_Port = GPIOB;
    config.GPIO_Pin = K11_LOCK_Pin;
    config.PressLevel = KEY_LOW_LEVEL_PRESS;
    config.Mode = KEY_MODE_NORMAL;
    Key_Init(&config, K11_LOCK); 

    config.GPIO_Port = GPIOB;
    config.GPIO_Pin = K12_SET_Pin;
    config.PressLevel = KEY_LOW_LEVEL_PRESS;
    config.Mode = KEY_MODE_NORMAL;
    Key_Init(&config, K12_SET); 
}

void Key_Scan(void)
{
    static uint8_t is_locked = 0; 
    
    // 这里建议 Key_GetNum 采用非阻塞的触发模式
    if (Key_GetNum(K11_LOCK) == 1) // 检测到触发动作
    {
        is_locked = !is_locked; // 状态取反：0变1, 1变0
    }

    if (is_locked) 
    {
        tx_data.LOCK_KEY = 1;
        SetLedMode(rLED_DOWN, LED_ON);
    }
    else
    {
        tx_data.LOCK_KEY = 0;
        SetLedMode(rLED_DOWN, LED_OFF);
    }
}