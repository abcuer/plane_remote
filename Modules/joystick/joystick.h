#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#include "adc.h"
#include "remote.h"

#define FilterNum 10
#define Limit(x, min, max)  ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

typedef struct  
{
	int16_t THR;	// 油门
	int16_t YAW;	// 偏航
	int16_t ROL;    // 滚转
	int16_t PIT;	// 俯仰
	int16_t BAT;	// 遥控电压
}Stick_Struct;

typedef struct  
{
	uint32_t sum;
	uint16_t old[FilterNum];
}Filter_Struct;

void JoyStick_Init(void);
void Stick_Scan(void);
void Update_TX_Data(void);

extern Stick_Struct stick;

#endif