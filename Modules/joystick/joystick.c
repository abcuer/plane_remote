#include "joystick.h"
#include "key.h"

uint16_t ADC_ConvertedValue[6] = {0};
Stick_Struct stick = {0};
Filter_Struct Filter_THR, Filter_YAW, Filter_ROL, Filter_PIT, Filter_BAT;

void JoyStick_Init(void)
{
    /* 1. 校准 ADC */
    HAL_ADCEx_Calibration_Start(&hadc1);
    /* 2. 开启 DMA 循环采集 (长度为6) */
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_ConvertedValue, 6);
}

//消除中值漂移误差（油门位最低位零漂）
static void Stick_Mid_Limit(Stick_Struct *stick)
{
	if(stick->THR>998  && stick->THR<1004) stick->THR = 1000;
	if(stick->YAW>1510 && stick->YAW<1520) stick->YAW = 1500;
    if(stick->PIT>1500 && stick->PIT<1510) stick->PIT = 1500;
	if(stick->ROL>1470 && stick->ROL<1480) stick->ROL = 1500;
}

// static void TX_Limit(Stick_Struct *tx_data)
static void TX_Limit(Remote_Data_Struct *tx_data)
{
    tx_data->THR = Limit(tx_data->THR, 1000, 2000);
    tx_data->PIT = Limit(tx_data->PIT, 1000, 2000);
    tx_data->ROL = Limit(tx_data->ROL, 1000, 2000);
    tx_data->YAW = Limit(tx_data->YAW, 1000, 2000);
}

//摇杆原始数据滤波
static void Stick_Filter(Stick_Struct *stick)
{
	static uint8_t Filter_Count = 0;
	
	//丢数据
	Filter_THR.sum -= Filter_THR.old[Filter_Count];
	Filter_YAW.sum -= Filter_YAW.old[Filter_Count];
	Filter_ROL.sum -= Filter_ROL.old[Filter_Count];
	Filter_PIT.sum -= Filter_PIT.old[Filter_Count];
	Filter_BAT.sum -= Filter_BAT.old[Filter_Count];
	
	Filter_THR.old[Filter_Count] = stick->THR;
	Filter_YAW.old[Filter_Count] = stick->YAW;
	Filter_ROL.old[Filter_Count] = stick->ROL;
	Filter_PIT.old[Filter_Count] = stick->PIT;
	Filter_BAT.old[Filter_Count] = stick->BAT;
	
	//更新数据
	Filter_THR.sum += Filter_THR.old[Filter_Count];
	Filter_YAW.sum += Filter_YAW.old[Filter_Count];
	Filter_ROL.sum += Filter_ROL.old[Filter_Count];
	Filter_PIT.sum += Filter_PIT.old[Filter_Count];
	Filter_BAT.sum += Filter_BAT.old[Filter_Count];
	
	//输出数据
	stick->THR = Filter_THR.sum / FilterNum;
	stick->YAW = Filter_YAW.sum / FilterNum;
	stick->ROL = Filter_ROL.sum / FilterNum;
	stick->PIT = Filter_PIT.sum / FilterNum;
	stick->BAT = Filter_BAT.sum / FilterNum;
	
	Filter_Count++;
	if(Filter_Count == FilterNum) Filter_Count=0;
}

//2ms调用一次，获取摇杆原始数据（含自身误差修正以及滤波）
void Stick_Scan(void)
{
    stick.THR = 1000 + (uint16_t)(0.25f*ADC_ConvertedValue[3]);  //油门, A3
	stick.PIT = 1000 + (uint16_t)(0.25f*ADC_ConvertedValue[1]);  //方向：前后, A1
    stick.YAW = 1000 + (uint16_t)(0.25f*ADC_ConvertedValue[2]);  //偏航：旋转, A2
	stick.ROL = 2000 - (uint16_t)(0.25f*ADC_ConvertedValue[0]);  //方向：左右, A0
	//测量电池电压 +44是防反接二极管的压价通过万用表测量得出    
	stick.BAT =(uint16_t)(2.0f*ADC_ConvertedValue[4]/ADC_ConvertedValue[5]*1.2f*100)+44;  
    
    Stick_Filter(&stick);
}

// 10ms
void Update_TX_Data(void)
{
    Stick_Mid_Limit(&stick);
    tx_data.THR = stick.THR;
    tx_data.YAW = stick.YAW;
    tx_data.PIT = stick.PIT;
    tx_data.ROL = stick.ROL;
    // Key_Scan();
    // 遥控器断联 / tx_data.LOCK_KEY为1，tx_data.THR = 1000;
    if(tx_data.LOCK_KEY)
	{
		tx_data.THR = 1000;
		tx_data.PIT = 1500;
		tx_data.ROL = 1500;
		tx_data.YAW = 1500;
	} 
    TX_Limit(&tx_data);
    // OLED显示数据
}