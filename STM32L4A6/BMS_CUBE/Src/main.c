/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file : main.c
* @brief: Main program body
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
* All rights reserved.</center></h2>
*
* This software component is licensed by ST under Ultimate Liberty license
* SLA0044, the "License"; You may not use this file except in compliance with
* the License. You may obtain a copy of the License at:
* www.st.com/SLA0044
*
******************************************************************************
*/
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "bq76925.h"
#include <stdlib.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc2;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

osThreadId defaultTaskHandle;
/* USER CODE BEGIN PV */
osThreadId adcReadTaskHandle;
int adc1count = 0;
int adc2count = 0;
double adc1sum = 0.0;
uint32_t adc2sum = 0;

osThreadId bqTaskHandle;
BQ_t bq;
ADC_ChannelConfTypeDef adc2_channel;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_ADC2_Init(void);
void StartDefaultTask(void const * argument);

/* USER CODE BEGIN PFP */
void StartAdcReadTask(void const *argument);
void StartBqTask(void const *argument);

void BqStatusUpdateDoneCallback(BQ_t *);
void BqI2cTxCallback(BQ_t *);
void BqI2cRxCallback(BQ_t *);
void BqBatSetCallback(BQ_t *);
void BqBatDoneCallback(BQ_t *);

void setADC2Channel(uint8_t channel);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

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
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */

    osThreadDef(adcReadTask, StartAdcReadTask, osPriorityNormal, 0, 128);
    adcReadTaskHandle = osThreadCreate(osThread(adcReadTask), NULL);

    osThreadDef(bqTask, StartBqTask, osPriorityAboveNormal, 0, 128);
    bqTaskHandle = osThreadCreate(osThread(bqTask), NULL);
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */
  /** Common config 
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc2.Init.LowPowerAutoWait = DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc2.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x10909CEC;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED_B_Pin|LED_G_Pin|LED_R_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : IF_ALERT_Pin */
  GPIO_InitStruct.Pin = IF_ALERT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(IF_ALERT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_B_Pin LED_G_Pin LED_R_Pin */
  GPIO_InitStruct.Pin = LED_B_Pin|LED_G_Pin|LED_R_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void setADC2Channel(uint8_t channel)
{
    static unsigned channelList[] = {
        ADC_CHANNEL_0, ADC_CHANNEL_1, ADC_CHANNEL_2, ADC_CHANNEL_3, ADC_CHANNEL_4,
        ADC_CHANNEL_5, ADC_CHANNEL_6, ADC_CHANNEL_7, ADC_CHANNEL_8, ADC_CHANNEL_9,
        ADC_CHANNEL_10, ADC_CHANNEL_11};
    adc2_channel.Channel = channelList[channel];
    adc2_channel.Rank = ADC_REGULAR_RANK_1;
    adc2_channel.SamplingTime = 12.5;
    adc2_channel.SingleDiff = ADC_SINGLE_ENDED;
    adc2_channel.OffsetNumber = ADC_OFFSET_NONE;
    adc2_channel.Offset = 0x000;
    HAL_ADC_ConfigChannel(&hadc2, &adc2_channel);
}

void StartAdcReadTask(void const *argument)
{
    // HAL_ADC_Start_IT(&hadc1);
    HAL_UART_Transmit(&huart2, "Start ADC read\r\n", strlen("Start ADC read\r\n"), 100);
    setADC2Channel(2);
    HAL_ADC_Start_IT(&hadc2);

    // char s[10] = "Alive\r\n";
    // HAL_ADC_Start(&hadc2);
    for (;;)
    {
        // HAL_UART_Transmit_IT(&huart2, s, strlen(s));
        // HAL_GPIO_TogglePin(GPIOC, LED_G_Pin);
        // HAL_ADC_PollForConversion(&hadc2, 1000);
        // int a = HAL_ADC_GetValue(&hadc2);
        // char msg[50];
        // sprintf(msg, "ADC Value %d\r\n", a);
        // HAL_UART_Transmit(&huart2, buffer, strlen(buffer), 100);
        // if (HAL_ADC_STATE_READY & HAL_ADC_GetState(&hadc2)) HAL_ADC_Start_IT(&hadc2);
        osDelay(HAL_MAX_DELAY);
    }
}

void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef *hadc)
{
    for (;;)
        ;
}
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
    for (;;)
        ;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    // if(hadc == &hadc1){
    // adc1sum += HAL_ADC_GetValue(&hadc1);
    // adc1count++;
    // }else
    int adc2Val;
    if (hadc == &hadc2)
    {
        //	adc2sum =hadc->Instance->DR;
        adc2Val = HAL_ADC_GetValue(hadc);
        adc2sum += adc2Val;
        adc2count++;
    }
    //adc2sum += HAL_ADC_GetValue(hadc);
    //char buffer[50];
    // int val = (adc2Val*3333)/4096;
    //sprintf(buffer, "ADC Value %d\r\n", adc2Val);
    //HAL_UART_Transmit(&huart2, buffer, strlen(buffer), 100);
    // HAL_ADC_Start_IT(&hadc2);
}

void voltageCheck(uint8_t voltageIndex) {
    // check for upper limit

}

void StartBqTask(void const *argument)
{

    bq.i2c = &hi2c1;
    bq.statusUpdateDone = BqStatusUpdateDoneCallback;
    bq.i2cTxCallback = BqI2cTxCallback;
    bq.i2cRxCallback = BqI2cRxCallback;
    bq.batSetCallback = BqBatSetCallback;
    bq.batDoneCallback = BqBatDoneCallback;
    BQ_setRefSel(&bq, 1);
    BQ_setSleep(&bq, 0);
    char msg[120];
    int cnt = 0;
    while (BQ_ERROR == BQ_i2cTest(&bq))
    {
//    	osDelay(10);
        // ERROR here
        sprintf(msg, "Try connecting the I2C %d times......\r\n", cnt++);
        HAL_UART_Transmit(&huart2, msg, strlen(msg), 1000);
    }
    int data;
    // bq.regMap[CELL_CTL_ADDRESS] = 0b00000;
    BQ_updateStatus_IT(&bq);

    int bq_cnt = 0;
    double voltages[7];
    double ADC_result;
    int firstSetOfData = 1; // the first set of data is not accurate

    for (;;)
    {
        // HAL_UART_Transmit_IT(&huart2, s, strlen(s));
        // HAL_GPIO_TogglePin(GPIOC, LED_G_Pin);
        if (bq.adcRunning && adc2count >= 100)
        {
            /* TODO stop ADC reading here */
            ADC_result = (1.0 * adc2sum / adc2count); // this should equals (ADC_Count / ADC_FULL_SCALE_COUNT)
            // ADC_result = 2929. / 4096.;// NOTE test only

            bq.adcRunning = 0;
            if (bq.mode == BQ_MODE_VOLTAGE)
            {
                BQ_readBattery(&bq, 0, 0);
            }
            else
            {
                BQ_readTemp(&bq, 0, 0);
            }
        }
        else if (bq.readDone && adc2count >= 100) // read I2C values
        {

            // if (bq.batIndex <= 6)
            /* calibrate first !!!! */
            /**
             * Read final data and move to the next channel
             * Order:
             *      ADC Cali    readBat 7, 1
             *      ADC Cali    readBat 8, 1
             *      Voltage 1-6 readBat [1...6], 0
             *      Temp 1-5    readTemp [1...5], 1     
             *  */

            int voltageIndex = bq.batIndex;
            osDelay(10); // wait for all adc done
            if (bq.mode == BQ_MODE_TEMPERATURE)
            {
                double volt = BQ_getTempVoltage(&bq, ADC_result, &huart2);
                double resis = BQ_getTempResists(&bq, volt);
                double temp = BQ_getTempResult(&bq, volt);
                sprintf(msg, "Temp:%4d\tTemp:%d'C\tRes:%6dohm\tADC:%6d\tVolt:%6dmv\r\n", 
                        bq.tempIndex, (int) (temp), (int) (resis) , (int)ADC_result, (int) (volt * 1000));
                HAL_UART_Transmit(&huart2, msg, strlen(msg), 1000);

                if (!firstSetOfData && BQ_GOOD != BQ_checkTemp(&bq, temp)) {
                    char * warningMsg = BQ_getWarningMsg(&bq);
                    sprintf(msg, "=======================================\r\n");
                    HAL_UART_Transmit(&huart2, msg, strlen(msg), 1000);
                    sprintf(msg, "| ⚠ TEMP WARNING: %s |\r\n", warningMsg);
                    HAL_UART_Transmit(&huart2, msg, strlen(msg), 1000);
                    sprintf(msg, "=======================================\r\n");
                    HAL_UART_Transmit(&huart2, msg, strlen(msg), 1000);
                }

                // start new query
                if (bq.tempIndex == 1)
                {
                    setADC2Channel(5);
                    BQ_readTemp(&bq, 2, 1);
                }
                else if (bq.tempIndex == 2)
                {
                    setADC2Channel(6);
                    BQ_readTemp(&bq, 3, 1);
                }
                else if (bq.tempIndex == 3)
                {
                    setADC2Channel(9);
                    BQ_readTemp(&bq, 4, 1);
                }
                else if (bq.tempIndex == 4)
                {
                    setADC2Channel(10);
                    BQ_readTemp(&bq, 5, 1);
                }
                else if (bq.tempIndex == 5)
                {
                    setADC2Channel(2);
                    bq.mode = BQ_MODE_VOLTAGE;
                    BQ_readBattery(&bq, 7, 1);

                    firstSetOfData = 0;
                }
            }
            else if (voltageIndex <= 6)
            {
                double VCOUT = BQ_getVoltageFromAdc(&bq, ADC_result); 
                double voltage = BQ_getVoltage(&bq, ADC_result, NULL);
                // voltage += (voltageIndex > 1 ? voltages[voltageIndex - 1] : 0);
                voltages[voltageIndex] = voltage;
                sprintf(msg, "Bat:%d\tVoltage:%6dmv\tADC:%5d\tVcout:%6dmv\r\n", (int)bq.batIndex, (int)(voltage * 1000.), (int)(ADC_result), (int)(VCOUT * 1000.));
                HAL_UART_Transmit(&huart2, msg, strlen(msg), 1000);

                if (!firstSetOfData && BQ_GOOD != BQ_checkVoltage(&bq, voltageIndex)) {
                    char * warningMsg = BQ_getWarningMsg(&bq);
                    sprintf(msg, "=======================================\r\n");
                    HAL_UART_Transmit(&huart2, msg, strlen(msg), 1000);
                    sprintf(msg, "| ⚠ VOLTAGE WARNING: %s |\r\n", warningMsg);
                    HAL_UART_Transmit(&huart2, msg, strlen(msg), 1000);
                    sprintf(msg, "=======================================\r\n");
                    HAL_UART_Transmit(&huart2, msg, strlen(msg), 1000);
                }

                if (voltageIndex < 5)
                {
                    BQ_readBattery(&bq, voltageIndex + 1, 0);
                }
                else
                {
                    setADC2Channel(4);
                    bq.mode = BQ_MODE_TEMPERATURE;
                    BQ_readTemp(&bq, 1, 1);
                }
            }
            else if (voltageIndex == 7) // check for 0.5x Vref
            {
                bq.cali.vref_0_5 = ADC_result;
                sprintf(msg, "0.5xVref:\tADC:%d\r\n", (int)ADC_result);
                HAL_UART_Transmit(&huart2, msg, strlen(msg), 1000);
                BQ_readBattery(&bq, 8, 1);
            }
            else if (voltageIndex == 8)
            {
                bq.cali.vref_0_8_5 = ADC_result;
                sprintf(msg, "0.85xVref:\tADC:%d\r\n", (int)ADC_result);
                HAL_UART_Transmit(&huart2, msg, strlen(msg), 1000);
                BQ_ADCCalibration(&bq);
                sprintf(msg, "ADC Slope:%6d\tOffset:%6d\r\n", (int)bq.cali.slope, (int)bq.cali.offset);
                HAL_UART_Transmit(&huart2, msg, strlen(msg), 1000);
                BQ_readBattery(&bq, 1, 0);
            }

            // stop adcRunning
            // BQ_setADCRunning(&bq, 0);
            adc2count = 0;
            adc2sum = 0;
            // switch to the next battery
            // BQ_readBattery(&bq, bq.batIndex % 6);// + 1);
            // BQ_readBattery(&bq, bq.batIndex % 8 + 1, (bq.batIndex % 8 + 1) > 6 ? 1 : 0);
        }
        else if (bq.adcRunning)
        {
            HAL_ADC_Start_IT(&hadc2);
            //			sprintf(msg, 'Status report: adc_running: %d, adc_count: %d\r\n', xxx, adc2count);
            //			HAL_UART_Transmit(&huart2, msg, strlen(msg), 1000);
        }
        osDelay(10);
    }
}

void BqStatusUpdateDoneCallback(BQ_t *bq_ptr)
{
    // start to from battery 1
    // BQ_logAllReg(bq_ptr, &huart2);
    bq_ptr->mode = BQ_MODE_VOLTAGE;
    BQ_readBattery(bq_ptr, 7, 1);
}
void BqI2cTxCallback(BQ_t *bq_ptr)
{
    // USER defined I2C task callback
}
void BqI2cRxCallback(BQ_t *bq_ptr)
{
    // USER defined I2C task callback
}
void BqBatSetCallback(BQ_t *bq_ptr)
{
    /* TODO start ADC reading here */
    adc2count = 0;
    adc2sum = 0;
    // HAL_ADC_Start_IT(&hadc2);
}
void BqBatDoneCallback(BQ_t *bq_ptr)
{
    //BQ_logAllReg(bq_ptr, &huart2);
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    BQ_I2cCallback(&bq, hi2c);
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    BQ_I2cCallback(&bq, hi2c);
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
* @briefFunction implementing the defaultTask thread.
* @paramargument: Not used 
* @retval None
*/
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN 5 */
    /* Infinite loop */
    //	HAL_UART_Transmit(&huart2, "Start default task\r\n", strlen("Start default task\r\n"), 100);
    int seed = 0;
    for (;;)
    {
        HAL_GPIO_TogglePin(GPIOC, LED_B_Pin);
        osDelay(500);
    }
  /* USER CODE END 5 */ 
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    char msg[30] = "UNKNOWN ERROR!\r\n";
    HAL_UART_Transmit(&huart2, msg, strlen(msg), 100);
    for (;;)
        ;
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
 tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
