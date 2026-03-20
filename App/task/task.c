#include "headfile.h"

// 任务执行周期(ms)
#define COMM_PERIOD 8
#define KEY_PERIOD 20
#define SHOW_PERIOD 30

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
