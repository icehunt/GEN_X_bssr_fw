/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : stm32l4xx_hal_msp.c
  * Description        : This file provides code for the MSP Initialization 
  *                      and de-Initialization codes.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */
 
/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/
  /* PendSV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);

  /* Peripheral interrupt init */
  /* RCC_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(RCC_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(RCC_IRQn);

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/**
* @brief ADC MSP Initialization
* This function configures the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hadc->Instance==ADC2)
  {
  /* USER CODE BEGIN ADC2_MspInit 0 */

  /* USER CODE END ADC2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_ADC_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC2 GPIO Configuration    
    PC0     ------> ADC2_IN1
    PC1     ------> ADC2_IN2
    PC2     ------> ADC2_IN3
    PC3     ------> ADC2_IN4
    PA0     ------> ADC2_IN5
    PA1     ------> ADC2_IN6
    PA4     ------> ADC2_IN9
    PA5     ------> ADC2_IN10 
    */
    GPIO_InitStruct.Pin = ADC_VREF_Pin|ADC_VC_Pin|ADC_VI_Pin|ADC_T1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ADC_T2_Pin|ADC_T3_Pin|ADC_T4_Pin|ADC_T5_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ADC2 interrupt Init */
    HAL_NVIC_SetPriority(ADC1_2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
  /* USER CODE BEGIN ADC2_MspInit 1 */

  /* USER CODE END ADC2_MspInit 1 */
  }

}

/**
* @brief ADC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance==ADC2)
  {
  /* USER CODE BEGIN ADC2_MspDeInit 0 */

  /* USER CODE END ADC2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC_CLK_DISABLE();
  
    /**ADC2 GPIO Configuration    
    PC0     ------> ADC2_IN1
    PC1     ------> ADC2_IN2
    PC2     ------> ADC2_IN3
    PC3     ------> ADC2_IN4
    PA0     ------> ADC2_IN5
    PA1     ------> ADC2_IN6
    PA4     ------> ADC2_IN9
    PA5     ------> ADC2_IN10 
    */
    HAL_GPIO_DeInit(GPIOC, ADC_VREF_Pin|ADC_VC_Pin|ADC_VI_Pin|ADC_T1_Pin);

    HAL_GPIO_DeInit(GPIOA, ADC_T2_Pin|ADC_T3_Pin|ADC_T4_Pin|ADC_T5_Pin);

    /* ADC2 interrupt DeInit */
    HAL_NVIC_DisableIRQ(ADC1_2_IRQn);
  /* USER CODE BEGIN ADC2_MspDeInit 1 */

  /* USER CODE END ADC2_MspDeInit 1 */
  }

}

/**
* @brief CAN MSP Initialization
* This function configures the hardware resources used in this example
* @param hcan: CAN handle pointer
* @retval None
*/
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hcan->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN1 GPIO Configuration    
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }

}

/**
* @brief CAN MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hcan: CAN handle pointer
* @retval None
*/
void HAL_CAN_MspDeInit(CAN_HandleTypeDef* hcan)
{
  if(hcan->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();
  
    /**CAN1 GPIO Configuration    
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }

}

/**
* @brief I2C MSP Initialization
* This function configures the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hi2c->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspInit 0 */

  /* USER CODE END I2C2_MspInit 0 */
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C2 GPIO Configuration    
    PB13     ------> I2C2_SCL
    PB14     ------> I2C2_SDA 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();
    /* I2C2 interrupt Init */
    HAL_NVIC_SetPriority(I2C2_EV_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
    HAL_NVIC_SetPriority(I2C2_ER_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
  /* USER CODE BEGIN I2C2_MspInit 1 */

  /* USER CODE END I2C2_MspInit 1 */
  }

}

/**
* @brief I2C MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspDeInit 0 */

  /* USER CODE END I2C2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C2_CLK_DISABLE();
  
    /**I2C2 GPIO Configuration    
    PB13     ------> I2C2_SCL
    PB14     ------> I2C2_SDA 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13|GPIO_PIN_14);

    /* I2C2 interrupt DeInit */
    HAL_NVIC_DisableIRQ(I2C2_EV_IRQn);
    HAL_NVIC_DisableIRQ(I2C2_ER_IRQn);
  /* USER CODE BEGIN I2C2_MspDeInit 1 */

  /* USER CODE END I2C2_MspDeInit 1 */
  }

}

/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }

}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();
  
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 interrupt DeInit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
