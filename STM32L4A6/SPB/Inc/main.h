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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GPIO_CAMERA_Pin GPIO_PIN_4
#define GPIO_CAMERA_GPIO_Port GPIOB
#define GPIO_CAMERA_EXTI_IRQn EXTI4_IRQn
#define GPIO_FWDREV_Pin GPIO_PIN_5
#define GPIO_FWDREV_GPIO_Port GPIOB
#define GPIO_FWDREV_EXTI_IRQn EXTI9_5_IRQn
#define GPIO_FAN_Pin GPIO_PIN_6
#define GPIO_FAN_GPIO_Port GPIOB
#define GPIO_FAN_EXTI_IRQn EXTI9_5_IRQn
#define GPIO_LP_Pin GPIO_PIN_7
#define GPIO_LP_GPIO_Port GPIOB
#define GPIO_LP_EXTI_IRQn EXTI9_5_IRQn
#define GPIO_AUX_Pin GPIO_PIN_3
#define GPIO_AUX_GPIO_Port GPIOH
#define GPIO_AUX_EXTI_IRQn EXTI3_IRQn
#define GPIO_IGNITION_Pin GPIO_PIN_8
#define GPIO_IGNITION_GPIO_Port GPIOB
#define GPIO_IGNITION_EXTI_IRQn EXTI9_5_IRQn
#define GPIO_ARRAY_Pin GPIO_PIN_9
#define GPIO_ARRAY_GPIO_Port GPIOB
#define GPIO_ARRAY_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
