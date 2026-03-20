#ifndef _headfile_h
#define _headfile_h

#include "stm32f1xx_hal.h"
#include "stdint.h"
#include "stdio.h"

/***************** App ****************/
#include "main.h"
#include "gpio.h"

/***************** Bsp ****************/
#include "bsp_gpio.h"
#include "bsp_iic.h"
#include "bsp_dwt.h"
/***************** Modules ****************/
#include "led.h"
#include "beep.h"
#include "key.h"
#include "oled.h"
#include "nrf24l01.h"
#include "joystick.h"

/***************** App ****************/
#include "utils.h"
#include "remote.h"
#include "ui.h"

#include "freertos.h"
#include "task.h"
#include "cmsis_os.h"
/************************ 标志位 *********************/

/************************ 全局变量 *********************/

#endif