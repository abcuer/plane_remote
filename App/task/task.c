#include "headfile.h"

// 任务执行周期(ms)
#define STICK_PERIOD 2
#define COMM_PERIOD 6
#define KEY_PERIOD 10
#define SHOW_PERIOD 20

void StartStickTask(void const * argument)
{
    for(;;)
    {
        Stick_Scan();
        osDelay(STICK_PERIOD);
    }
}

void StartCommonTask(void const * argument)
{
    for(;;)
    {
        Remote_SendData();
        osDelay(COMM_PERIOD);
    }
}

void StartKeyTask(void const * argument)
{
    for(;;)
    {
        KeyScan();
        osDelay(KEY_PERIOD);
    }
}

void StartShowTask(void const * argument)
{
    for(;;)
    {
        ShowData();
        LedScan();
        osDelay(SHOW_PERIOD); 
    }
}
