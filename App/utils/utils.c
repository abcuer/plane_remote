#include "beep.h"
#include "headfile.h"
#include "led.h"

static int16_t feedback_tick = 0; 
volatile Device_Action_t g_device_action = ACT_NONE;
/**
 * @brief 纯净版 LED/BEEP 扫描逻辑 (50ms 周期)
 */
// void Led_Scan(void) 
// {
//     // --- 1. 处理按键动作请求 ---
//     if (g_device_action == ACT_KEY_PRESS) 
//     {
//         g_device_action = ACT_NONE;
//         feedback_tick = 2; // 2*50ms = 100ms 的反馈时间
        
//         // 瞬间动作：鸣叫并点亮下灯
//         SetBeepMode(BEEP, BEEP_ON);
//         SetLedMode(rLED_DOWN, LED_ON);
//     }

//     // --- 2. 计时器处理 ---
//     if (feedback_tick > 0) 
//     {
//         feedback_tick--;
//         if (feedback_tick == 0) 
//         {
//             SetBeepMode(BEEP, BEEP_OFF); // 时间到，关蜂鸣器
//             // 此处不需要手动关下灯，下方的常驻逻辑会自动修正它
//         }
//     }

//     // --- 3. 常驻状态显示 (仅在反馈结束时生效) ---
//     if (feedback_tick <= 0) 
//     {
//         if (tx_data.CONNECT) 
//         {
//             // 连接成功：上亮下灭
//             SetLedMode(rLED_UP, LED_ON);
//             SetLedMode(rLED_DOWN, LED_OFF);
//         } 
//         else 
//         {
//             // 连接断开：下亮上灭
//             SetLedMode(rLED_UP, LED_OFF);
//             SetLedMode(rLED_DOWN, LED_ON);
//         }
//     }
// }

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
    JoyStick_Init();
    NRF24L01_Init();
    OLED_Init();

    SetLedMode(rLED_UP, LED_ON);
    SetLedMode(rLED_DOWN, LED_ON);
    SetBeepMode(BEEP, BEEP_ON);
    delay_ms(100);
    SetLedMode(rLED_UP, LED_OFF);
    SetLedMode(rLED_DOWN, LED_OFF);
    SetBeepMode(BEEP, BEEP_OFF);
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

void DataShow(void)
{
     // 定义参数数组
        char labels[] = {'T', 'Y', 'P', 'R'};
        int32_t values[] = {tx_data.THR, tx_data.YAW, tx_data.PIT, tx_data.ROL};
        
        for(uint8_t i = 0; i < 4; i++) 
        {
            OLED_ShowChar(i + 1, 1, labels[i]);
            OLED_DrawCompactBar(i * 2, 12, 25, values[i]);
            OLED_ShowNum(i + 1, 6, values[i], 4);
        }

        // --- 右侧剩余空间显示系统信息 ---
        
        // 电压：显示在第二行末尾 (Line 2, Col 12)
        OLED_ShowFloatNum(2, 12, (float)stick.BAT / 100.0f, 1, 2);
        OLED_ShowChar(2, 16, 'V');

        // 锁定状态：显示在第四行末尾 (Line 4, Col 12)
        if(tx_data.LOCK_KEY > 0) {
            OLED_ShowString(4, 13, "LCK"); 
        } else {
            OLED_ShowString(4, 13, "ACT");
        }

}