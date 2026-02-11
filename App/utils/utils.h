#ifndef __UTILS_H
#define __UTILS_H

// 定义灯光和蜂鸣器的动作触发器
typedef enum {
    ACT_NONE = 0,
    ACT_KEY_PRESS      // 按键声光提醒
} Device_Action_t;

void System_Init(void);
void Key_Test_Function(void);
void Test_NRF24L01_Init(void);
void DataShow(void);

extern volatile Device_Action_t g_device_action;

#endif
