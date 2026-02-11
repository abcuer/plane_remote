#include "oled.h"
#include "oled_font.h"
#include "bsp_delay.h"
#include "stm32f1xx_hal.h"
#include "bsp_iic.h"

// 定义 OLED 的 I2C 总线实例
static iic_bus_t oled_bus = {
    .IIC_SDA_PORT = OLED_SDA_GPIO_Port,
    .IIC_SDA_PIN = OLED_SDA_Pin,
    .IIC_SCL_PORT = OLED_SCL_GPIO_Port,
    .IIC_SCL_PIN = OLED_SCL_Pin
};
/**
  * @brief  OLED写命令
  */
static void OLED_WriteCommand(uint8_t Command)
{
    IICStart(&oled_bus);
    IICSendByte(&oled_bus, OLED_ADDR);
    IICWaitAck(&oled_bus);
    IICSendByte(&oled_bus, 0x00); // 控制字节：写命令
    IICWaitAck(&oled_bus);
    IICSendByte(&oled_bus, Command);
    IICWaitAck(&oled_bus);
    IICStop(&oled_bus);
}

/**
  * @brief  OLED写数据
  */
static void OLED_WriteData(uint8_t Data)
{
    IICStart(&oled_bus);
    IICSendByte(&oled_bus, OLED_ADDR);
    IICWaitAck(&oled_bus);
    IICSendByte(&oled_bus, 0x40); // 控制字节：写数据
    IICWaitAck(&oled_bus);
    IICSendByte(&oled_bus, Data);
    IICWaitAck(&oled_bus);
    IICStop(&oled_bus);
}

/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
static void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//设置Y位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}

/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}

/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}

/**
  * @brief  OLED显示浮点数（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的浮点数
  * @param  IntLength 整数部分显示的位数
  * @param  FraLength 小数部分显示的位数（精度）
  * @retval 无
  */
void OLED_ShowFloatNum(uint8_t Line, uint8_t Column, float Number, uint8_t IntLength, uint8_t FraLength)
{
	uint32_t PowNum, IntNum, FraNum;
	// 1. 显示整数部分
	IntNum = (uint32_t)Number;
	OLED_ShowNum(Line, Column, IntNum, IntLength);
	OLED_ShowChar(Line, Column + IntLength, '.');
	PowNum = OLED_Pow(10, FraLength); 
	// 提取小数部分并四舍五入（+0.5用于简单的四舍五入补偿）
	FraNum = (uint32_t)((Number - IntNum) * PowNum + 0.5f);
	OLED_ShowNum(Line, Column + IntLength + 1, FraNum, FraLength);
}

/**
  * @brief  绘制紧凑型进度条 (高度 8 像素，占用 1 个 Page)
  * @param  Page   页坐标 (0~7)
  * @param  Column 像素列坐标 (0~127)
  * @param  Width  条形图总像素宽度
  * @param  Value  当前数值 (1000~2000)
  */
void OLED_DrawCompactBar(uint8_t Page, uint8_t Column, uint8_t Width, int32_t Value)
{
    if (Value < 1000) Value = 1000;
    if (Value > 2000) Value = 2000;
    
    // 计算填充长度
    uint8_t fillWidth = (uint8_t)((Value - 1000) * Width / 1000);
    
    OLED_SetCursor(Page, Column);
    for (uint8_t i = 0; i < Width; i++)
    {
        if (i == 0 || i == Width - 1) 
            OLED_WriteData(0xFF); // 左右边界
        else if (i == Width / 2) 
            OLED_WriteData(0xDB); // 中轴线 (1500)
        else if (i < fillWidth) 
            OLED_WriteData(0xBD); // 已填充部分 (上下实线)
        else 
            OLED_WriteData(0x81); // 未填充部分 (仅上下边框)
    }
}
/**
  * @brief  OLED初始化
  */
void OLED_Init(void)
{
    delay_ms(5);
    IICInit(&oled_bus); // 使用通用驱动初始化引脚
    
    OLED_WriteCommand(0xAE); // 关闭显示
    OLED_WriteCommand(0xD5); // 设置显示时钟分频
    OLED_WriteCommand(0x80);
    OLED_WriteCommand(0xA8); // 设置多路复用率
    OLED_WriteCommand(0x3F);
    OLED_WriteCommand(0xD3); // 设置显示偏移
    OLED_WriteCommand(0x00);
    OLED_WriteCommand(0x40); // 设置显示开始行
    OLED_WriteCommand(0xA1); // 左右方向
    OLED_WriteCommand(0xC8); // 上下方向
    OLED_WriteCommand(0xDA); // COM引脚配置
    OLED_WriteCommand(0x12);
    OLED_WriteCommand(0x81); // 对比度
    OLED_WriteCommand(0xCF);
    OLED_WriteCommand(0xD9); // 预充电周期
    OLED_WriteCommand(0xF1);
    OLED_WriteCommand(0xDB); // VCOMH级别
    OLED_WriteCommand(0x30);
    OLED_WriteCommand(0xA4); // 全屏点亮/正常
    OLED_WriteCommand(0xA6); // 正常显示
    OLED_WriteCommand(0x8D); // 充电泵
    OLED_WriteCommand(0x14);
    OLED_WriteCommand(0xAF); // 开启显示
        
    OLED_Clear();            // 清屏
}