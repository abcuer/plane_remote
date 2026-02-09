#ifndef _LED_H
#define _LED_H

#include "stm32f1xx_hal.h"

#define LED_NUM 2

#define LED_Port GPIOB
#define rLED_UP_Pin GPIO_PIN_1         // 上方红灯
#define rLED_DOWN_Pin GPIO_PIN_13        // 下方红灯

typedef enum
{
    rLED_UP = 0,
    rLED_DOWN,
} LED_Type_e;

typedef enum
{
    LED_OFF = 0,
    LED_ON,
    LED_TOGGLE
} LED_Mode_e;

typedef enum
{
    LED_LOW_LEVEL_ON = 0,
    LED_HIGH_LEVEL_ON,
} LED_ActiveLevel_e;

typedef struct
{
    GPIO_TypeDef* GPIO_Port;
    uint16_t GPIO_Pin;
    LED_ActiveLevel_e ActiveLevel; // 工作时的电平
} LedStaticParam_s;

typedef struct
{
    LED_Mode_e CurrentMode;
} LedRunningParam_s;

typedef struct
{
    LedStaticParam_s StaticParam;
    LedRunningParam_s RunningParam;
} LEDInstance;

void LedDevice_Init(void);
void SetLedMode(LED_Type_e LEDType, LED_Mode_e Mode);
void Led_SyncBlink_Process(void);
void Led_AltBlink_Process(void);

#endif