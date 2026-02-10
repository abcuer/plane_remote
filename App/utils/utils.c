#include "headfile.h"
#include "bsp_delay.h"
#include "led.h"

/**
 * @brief 系统初始化函数，初始化所有模块和外设
 * @param 无
 * @retval 无
 */
void System_Init(void)
{
    LedDevice_Init();
    BeepDeviceInit();
    KeyDeviceInit();
    NRF24L01_Init();
    HAL_Delay(100);
    // NRF24L01_init();

    // SPL06_Init();
    // Flow_Init();    
}

/**
 * @brief 按键测试函数 - 按一次按键就声光提示
 */
void Key_Test_Function(void)
{
    // 测试所有按键
    for (KEY_Type_e key = K1; key < KEY_NUM; key++)
    {
        if (Key_GetNum(key) == 1)  // 检测到按键按下
        {
            // 声光提示（LED和蜂鸣器切换状态）
            SetLedMode(rLED_DOWN, LED_TOGGLE);
            SetBeepMode(BEEP, BEEP_TOGGLE);
            HAL_Delay(100);  // 保持100ms
            
            // 恢复声光状态（再次切换）
            SetLedMode(rLED_DOWN, LED_TOGGLE);
            SetBeepMode(BEEP, BEEP_TOGGLE);
            
            // 可选：打印按键编号（如果有串口）
            // printf("Key %d pressed\r\n", key + 1);
            
            HAL_Delay(50);  // 防止连续检测
        }
    }
}

/**
 * @brief 测试NRF24L01初始化
 */
void Test_NRF24L01_Init(void)
{   
    // 检查初始化结果
    uint8_t check_result = NRF24L01_Check();
    
    if (check_result == 0)
    {
        SetLedMode(rLED_UP, LED_ON);    // 上方红灯亮表示成功
        SetBeepMode(BEEP, BEEP_ON);
        HAL_Delay(100);
        SetBeepMode(BEEP, BEEP_OFF);
        SetLedMode(rLED_UP, LED_OFF);
    }
    else
    {
        SetLedMode(rLED_DOWN, LED_ON);  // 下方红灯亮表示失败
        HAL_Delay(300);
        SetLedMode(rLED_DOWN, LED_OFF);
    }
    
    HAL_Delay(1000);
}