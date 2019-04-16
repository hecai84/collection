/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define DHT_Pin GPIO_PIN_2
#define DHT_GPIO_Port GPIOA
#define Sck1_Pin GPIO_PIN_3
#define Sck1_GPIO_Port GPIOA
#define Sck2_Pin GPIO_PIN_4
#define Sck2_GPIO_Port GPIOA
#define Sck3_Pin GPIO_PIN_5
#define Sck3_GPIO_Port GPIOA
#define Sck4_Pin GPIO_PIN_6
#define Sck4_GPIO_Port GPIOA
#define Sck5_Pin GPIO_PIN_7
#define Sck5_GPIO_Port GPIOA
#define Dt4_Pin GPIO_PIN_0
#define Dt4_GPIO_Port GPIOB
#define Dt3_Pin GPIO_PIN_1
#define Dt3_GPIO_Port GPIOB
#define Dt2_Pin GPIO_PIN_10
#define Dt2_GPIO_Port GPIOB
#define Dt1_Pin GPIO_PIN_11
#define Dt1_GPIO_Port GPIOB
#define Dt5_Pin GPIO_PIN_12
#define Dt5_GPIO_Port GPIOB
#define Dt6_Pin GPIO_PIN_13
#define Dt6_GPIO_Port GPIOB
#define Dt7_Pin GPIO_PIN_14
#define Dt7_GPIO_Port GPIOB
#define Dt8_Pin GPIO_PIN_15
#define Dt8_GPIO_Port GPIOB
#define Sck6_Pin GPIO_PIN_8
#define Sck6_GPIO_Port GPIOA
#define Sck7_Pin GPIO_PIN_11
#define Sck7_GPIO_Port GPIOA
#define Sck8_Pin GPIO_PIN_12
#define Sck8_GPIO_Port GPIOA
#define ButtonLed_Pin GPIO_PIN_10
#define ButtonLed_GPIO_Port GPIOC
#define Door1_Pin GPIO_PIN_2
#define Door1_GPIO_Port GPIOD
#define Lock2_Pin GPIO_PIN_4
#define Lock2_GPIO_Port GPIOB
#define Lock1_Pin GPIO_PIN_5
#define Lock1_GPIO_Port GPIOB
#define Door2_Pin GPIO_PIN_8
#define Door2_GPIO_Port GPIOB
#define Relay_Pin GPIO_PIN_9
#define Relay_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
