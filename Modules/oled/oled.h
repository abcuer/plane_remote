#ifndef __OLED_H
#define __OLED_H

#include "stdint.h"
#include "bsp_iic.h"

// OLED I2C地址（7位地址，需要左移1位）
#define OLED_ADDR    0x78    // 0x3C左移1位
#define OLED_CMD_MODE    0x00    // 命令模式
#define OLED_DATA_MODE   0x40    // 数据模式

// 函数声明
void OLED_Init(void);
void OLED_Clear(void);
uint32_t OLED_Pow(uint32_t X, uint32_t Y);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowFloatNum(uint8_t Line, uint8_t Column, float Number, uint8_t IntLength, uint8_t FraLength);
void OLED_DrawCompactBar(uint8_t Page, uint8_t Column, uint8_t Width, int32_t Value);
void OLED_ShowStringReverse(uint8_t Line, uint8_t Column, char *String);

#endif