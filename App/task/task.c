#include "headfile.h"


// 2ms
void StartStickTask(void const * argument)
{
    for(;;)
    {
        Stick_Scan();
        osDelay(2);
    }
}

// 10ms
void StartCommonTask(void const * argument)
{
    for(;;)
    {
        Remote_Send_Task();
        osDelay(10);
    }
}

// 20ms
void StartKeyTask(void const * argument)
{
    for(;;)
    {
        Key_Scan();
        osDelay(20);
    }
}

// 50ms
// void StartLedTask(void const * argument)
// {
//     for(;;)
//     {
//         Led_Scan();
//         osDelay(50);
//     }
// }

// 50ms
void StartShowTask(void const * argument)
{
    for(;;)
    {
        DataShow();
        osDelay(100); 
    }
}
// void StartShowTask(void const * argument)
// {
//     for(;;)
//     {
//         OLED_ShowSignedNum(1,3, tx_data.THR, 4);
//         OLED_ShowSignedNum(2,3, tx_data.YAW, 4);
//         OLED_ShowSignedNum(3,3, tx_data.PIT, 4);
//         OLED_ShowSignedNum(4,3, tx_data.ROL, 4);
//         OLED_ShowSignedNum(4,9, tx_data.LOCK_KEY, 2);
//         // 遥控器电压 
//         OLED_ShowFloatNum(1, 10, (float)stick.BAT / 100.0f, 1, 2);
//         osDelay(100);
//     }
// }