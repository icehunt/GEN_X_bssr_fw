/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "usart.h"
#include "dma.h"
#include "crc.h"
#include "bssr_serial.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define IGNITION 8
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
char rx_buf[2048];
uint16_t cur_pos = 0;
uint16_t uart3_head = 0;
uint16_t uart3_tail = 0;
uint8_t ignition_presses = 0;
uint8_t ignition_state = 0;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 256);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
    
    

  /* USER CODE BEGIN StartDefaultTask */
  //HAL_UART_Transmit_IT(&huart4, "Test\r\n", strlen("Test\r\n"));
  //HAL_UART_Receive_DMA(&huart3, rx_buf, 12);
  /* Infinite loop */
  HAL_UART_Receive_DMA(&huart3, rx_buf, 2048);
  osDelay(1000);
  uint16_t available = 0;
  int start_byte = -1;
  uint32_t crc_result;
  uint32_t crc_input;
  uint8_t data;
  uint16_t upper_index;
  for(;;){
    	BSSR_Serial_Tx("test", 4);
      osDelay(100);
  }
  for(;;)
  {
	//HAL_UART_Receive_IT(&huart4, rx_buf, 12);
	//HAL_UART_Receive(&huart4, rx_buf+1, 1, 2000);
	  //HAL_UART_Receive_IT(&huart3, rx_buf, 1);
	  while(((upper_index = get_upper_index(huart3.hdmarx))-uart3_tail) >= 7){

		  for(uart3_tail; uart3_tail < upper_index; uart3_tail++){
			  if(rx_buf[uart3_tail] == 0xa5){
				  crc_result = ~HAL_CRC_Calculate(&hcrc, rx_buf+uart3_tail, 3);
				  crc_input = rx_buf[uart3_tail + 6] << 24 ;
				  crc_input = crc_input | (rx_buf[uart3_tail + 5] << 16);
				  crc_input = crc_input | (rx_buf[uart3_tail + 4] << 8);
				  crc_input = crc_input | (rx_buf[uart3_tail + 3]);
				  if(crc_input == crc_result){
					  data = rx_buf[uart3_tail + 2];
					  if(data != 255){
						  if(!(data&IGNITION)){
							  if(ignition_presses == 1){
								  ignition_presses = 0;
								  ignition_state ^= 1;
								  uint8_t buf[2] = {0x01, ignition_state};
								  BSSR_Serial_Tx(buf, 2);
							  } else {
								  ignition_presses = 1;
							  }
						  }
						  data += 1;
					  }
				  }
			  }
		  }
	  }
	  volatile long av =__HAL_DMA_GET_COUNTER(huart3.hdmarx);
	//;void* available = huart3.hdmarx->Instance->NDT;
    //osDelay(10);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void HAL_UART_RxCpltCallback (UART_HandleTypeDef * huart){
  if(huart == &huart4){
    HAL_UART_Transmit_IT(&huart2, rx_buf, 12);
    HAL_UART_Receive_IT(&huart4, rx_buf, 12);
  } else if(huart == &huart3){
	  uart3_head = 0;
	HAL_UART_Transmit_IT(&huart3, rx_buf, 12);
  }
}
void HAL_UART_RxHalfCpltCallback (UART_HandleTypeDef * huart){
	if(huart == &huart3){
		rx_buf[0]++;
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
