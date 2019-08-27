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
#define ADC_VREF_Pin GPIO_PIN_0
#define ADC_VREF_GPIO_Port GPIOC
#define ADC_VC_Pin GPIO_PIN_1
#define ADC_VC_GPIO_Port GPIOC
#define ADC_VI_Pin GPIO_PIN_2
#define ADC_VI_GPIO_Port GPIOC
#define ADC_T1_Pin GPIO_PIN_3
#define ADC_T1_GPIO_Port GPIOC
#define ADC_T2_Pin GPIO_PIN_0
#define ADC_T2_GPIO_Port GPIOA
#define ADC_T3_Pin GPIO_PIN_1
#define ADC_T3_GPIO_Port GPIOA
#define ADC_T4_Pin GPIO_PIN_4
#define ADC_T4_GPIO_Port GPIOA
#define ADC_T5_Pin GPIO_PIN_5
#define ADC_T5_GPIO_Port GPIOA
#define IF_ALERT_Pin GPIO_PIN_1
#define IF_ALERT_GPIO_Port GPIOB
#define LED_B_Pin GPIO_PIN_6
#define LED_B_GPIO_Port GPIOC
#define LED_G_Pin GPIO_PIN_7
#define LED_G_GPIO_Port GPIOC
#define LED_R_Pin GPIO_PIN_8
#define LED_R_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
