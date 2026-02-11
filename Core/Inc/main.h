/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define K7_UP_Pin GPIO_PIN_13
#define K7_UP_GPIO_Port GPIOC
#define K9_P_Pin GPIO_PIN_14
#define K9_P_GPIO_Port GPIOC
#define K10_M_Pin GPIO_PIN_15
#define K10_M_GPIO_Port GPIOC
#define ROL_Pin GPIO_PIN_0
#define ROL_GPIO_Port GPIOA
#define PIT_Pin GPIO_PIN_1
#define PIT_GPIO_Port GPIOA
#define YAW_Pin GPIO_PIN_2
#define YAW_GPIO_Port GPIOA
#define THR_Pin GPIO_PIN_3
#define THR_GPIO_Port GPIOA
#define NRF_CSN_Pin GPIO_PIN_4
#define NRF_CSN_GPIO_Port GPIOA
#define NRF_SCK_Pin GPIO_PIN_5
#define NRF_SCK_GPIO_Port GPIOA
#define NRF_MISO_Pin GPIO_PIN_6
#define NRF_MISO_GPIO_Port GPIOA
#define NRF_MOSI_Pin GPIO_PIN_7
#define NRF_MOSI_GPIO_Port GPIOA
#define BAT_Pin GPIO_PIN_0
#define BAT_GPIO_Port GPIOB
#define rLED_UP_Pin GPIO_PIN_1
#define rLED_UP_GPIO_Port GPIOB
#define BEEP_Pin GPIO_PIN_10
#define BEEP_GPIO_Port GPIOB
#define K8_DOWN_Pin GPIO_PIN_11
#define K8_DOWN_GPIO_Port GPIOB
#define K11_LOCK_Pin GPIO_PIN_12
#define K11_LOCK_GPIO_Port GPIOB
#define rLED_DOWN_Pin GPIO_PIN_13
#define rLED_DOWN_GPIO_Port GPIOB
#define K1_Pin GPIO_PIN_14
#define K1_GPIO_Port GPIOB
#define K2_Pin GPIO_PIN_15
#define K2_GPIO_Port GPIOB
#define NRF_CE_Pin GPIO_PIN_15
#define NRF_CE_GPIO_Port GPIOA
#define K3_FRONT_Pin GPIO_PIN_3
#define K3_FRONT_GPIO_Port GPIOB
#define K6_LEFT_Pin GPIO_PIN_4
#define K6_LEFT_GPIO_Port GPIOB
#define K4_BACK_Pin GPIO_PIN_5
#define K4_BACK_GPIO_Port GPIOB
#define K5_RIGHT_Pin GPIO_PIN_6
#define K5_RIGHT_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_7
#define OLED_SDA_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_8
#define OLED_SCL_GPIO_Port GPIOB
#define K12_SET_Pin GPIO_PIN_9
#define K12_SET_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
