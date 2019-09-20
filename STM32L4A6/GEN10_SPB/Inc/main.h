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
#include "stm32l4xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CAMERA_LED_Pin GPIO_PIN_0
#define CAMERA_LED_GPIO_Port GPIOC
#define FWD_REV_LED_Pin GPIO_PIN_1
#define FWD_REV_LED_GPIO_Port GPIOC
#define FAN_LED_Pin GPIO_PIN_2
#define FAN_LED_GPIO_Port GPIOC
#define IGNITION_LED_Pin GPIO_PIN_3
#define IGNITION_LED_GPIO_Port GPIOC
#define LEDR_Pin GPIO_PIN_7
#define LEDR_GPIO_Port GPIOA
#define ARRAY_LED_Pin GPIO_PIN_4
#define ARRAY_LED_GPIO_Port GPIOC
#define AUX0_LED_Pin GPIO_PIN_5
#define AUX0_LED_GPIO_Port GPIOC
#define LEDG_Pin GPIO_PIN_0
#define LEDG_GPIO_Port GPIOB
#define LEDB_Pin GPIO_PIN_1
#define LEDB_GPIO_Port GPIOB
#define CAMERA_Pin GPIO_PIN_2
#define CAMERA_GPIO_Port GPIOB
#define AUX1_LED_Pin GPIO_PIN_6
#define AUX1_LED_GPIO_Port GPIOC
#define AUX2_LED_Pin GPIO_PIN_7
#define AUX2_LED_GPIO_Port GPIOC
#define LP_LED_Pin GPIO_PIN_8
#define LP_LED_GPIO_Port GPIOC
#define FWD_REV_Pin GPIO_PIN_3
#define FWD_REV_GPIO_Port GPIOB
#define FAN_Pin GPIO_PIN_4
#define FAN_GPIO_Port GPIOB
#define IGNITION_Pin GPIO_PIN_5
#define IGNITION_GPIO_Port GPIOB
#define ARRAY_Pin GPIO_PIN_6
#define ARRAY_GPIO_Port GPIOB
#define AUX0_Pin GPIO_PIN_7
#define AUX0_GPIO_Port GPIOB
#define AUX1_Pin GPIO_PIN_8
#define AUX1_GPIO_Port GPIOB
#define AUX2_Pin GPIO_PIN_9
#define AUX2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
