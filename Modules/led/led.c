#include "led.h"
#include "bsp_gpio.h"
#include "bsp_delay.h"

static LEDInstance led[LED_NUM];
/**
 * @brief 获取任意一个LED的数据
 */
static const LEDInstance *GetLedInstance(LED_Type_e LedType)
{
	return &led[LedType];
}

/**
 * @brief 根据当前状态更新GPIO电平
 */
 static void UpdatePinLevel(LED_Type_e LedType)
{
    LEDInstance *instance = &led[LedType];
    GPIO_PinState pinState;

    if (instance->RunningParam.CurrentMode == LED_ON) {
        // 如果逻辑要点亮，输出配置的有效电平
        pinState = (instance->StaticParam.ActiveLevel == LED_HIGH_LEVEL_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    } else {
        // 如果逻辑要熄灭，输出有效电平的反向电平
        pinState = (instance->StaticParam.ActiveLevel == LED_HIGH_LEVEL_ON) ? GPIO_PIN_RESET : GPIO_PIN_SET;
    }
    
    HAL_GPIO_WritePin(instance->StaticParam.GPIO_Port, instance->StaticParam.GPIO_Pin, pinState);
}
// static void UpdatePinLevel(LED_Type_e LedType)
// {
//     LEDInstance *instance = &led[LedType];
//     // 逻辑：状态与有效电平相同则置位，不同则复位
//     GPIO_PinState pinState = (instance->RunningParam.CurrentMode == instance->StaticParam.ActiveLevel) ? 
//                               GPIO_PIN_SET : GPIO_PIN_RESET;
    
//     HAL_GPIO_WritePin(instance->StaticParam.GPIO_Port, instance->StaticParam.GPIO_Pin, pinState);
// }

/**
 * @brief 设置LED状态
 */
void SetLedMode(LED_Type_e LedType, LED_Mode_e Mode)
{
    if (LedType >= LED_NUM) return;

    // 如果是翻转模式，直接操作硬件并更新内部状态记录
    if (Mode == LED_TOGGLE)
    {
        HAL_GPIO_TogglePin(led[LedType].StaticParam.GPIO_Port, led[LedType].StaticParam.GPIO_Pin);
        
        // 同步更新 RunningParam 里的状态，保持逻辑一致性
        led[LedType].RunningParam.CurrentMode = 
            (led[LedType].RunningParam.CurrentMode == LED_ON) ? LED_OFF : LED_ON;
    }
    else
    {
        // 普通的 ON/OFF 逻辑
        led[LedType].RunningParam.CurrentMode = Mode;
        UpdatePinLevel(LedType);
    }
}

static void Led_Init(LedStaticParam_s *config, LED_Type_e LedType)
{
    if (LedType >= LED_NUM) return; 
    // 复制配置
    led[LedType].StaticParam = *config;
	GPIO_Output(config->GPIO_Port, config->GPIO_Pin, BSP_GPIO_SPEED_LOW);
    // 初始化时关闭LED
    SetLedMode(LedType, LED_OFF);
}

void LedDevice_Init(void)
{ 
	LedStaticParam_s config;

	config.GPIO_Port = LED_Port;
	config.GPIO_Pin = rLED_UP_Pin;
	config.ActiveLevel = LED_HIGH_LEVEL_ON; // 点亮的电平
	Led_Init(&config, rLED_UP);

	config.GPIO_Port = LED_Port;
	config.GPIO_Pin = rLED_DOWN_Pin;
	config.ActiveLevel = LED_HIGH_LEVEL_ON; // 点亮的电平
	Led_Init(&config, rLED_DOWN);
}

/**
 * @brief 所有LED同步闪烁一次
 * @note 需在定时处理函数或主循环中按周期调用
 */
void Led_SyncBlink_Process(void)
{
    static LED_Mode_e state = LED_ON;
    state = (state == LED_ON) ? LED_OFF : LED_ON;
    
    for (int i = 0; i < LED_NUM; i++) {
        SetLedMode((LED_Type_e)i, state);
		delay_ms(100);
    }
}

/**
 * @brief LED交替闪烁
 */
void Led_AltBlink_Process(void) 
{
    static LED_Mode_e baseState = LED_ON;
    baseState = (baseState == LED_ON) ? LED_OFF : LED_ON;
    SetLedMode(rLED_UP, baseState);
    SetLedMode(rLED_DOWN, baseState);
	delay_ms(100);
}