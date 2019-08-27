#ifndef _USER_TASK_H__
#define _USER_TASK_H__

/* Standard includes. */
#include <stdio.h>
// #include <conio.h>
#include <string.h>
#include <stdlib.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "cmsis_os.h"
#include "stm32h7xx_hal.h"

/* The values sent to the queue receive task from the queue send task and the
queue send software timer respectively. */
#define mainVALUE_SENT_FROM_TASK			( 100UL )
#define mainVALUE_SENT_FROM_TIMER			( 200UL )

/* The number of items the queue can hold at once. */
#define mainQUEUE_LENGTH					( 5 )

void tSampleSend(void * params);
void tSampleReceive(void * params);

void user_task_init(UART_HandleTypeDef *);


#endif // !_USER_TASK_H__
