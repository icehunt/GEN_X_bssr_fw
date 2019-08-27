#include "user_task.h"
#include "main.h"
#include <string.h>

static UART_HandleTypeDef * huart = NULL;

static char *testString[10] = {"Hello\r\n", "World\r\n", "Good\r\n",
                        "Morning\r\n", "Best\r\n", "Wish\r\n",
                        "Happy\r\n", "Holiday\r\n"};

/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;

/* A software timer that is started from the tick hook. */
static TimerHandle_t xTimer = NULL;

/* mutex */
static SemaphoreHandle_t xSemaphore = NULL;

/* callback of all timers */
void vTimerCallback(TimerHandle_t timer) {
    const uint32_t maxExpiryCount = 100;
    uint32_t expiryCounter = 0;
    configASSERT(timer);  // check timer is NULL
    expiryCounter = (uint32_t) pvTimerGetTimerID(timer);   // counter saved as id
    expiryCounter++;
    if (expiryCounter >= maxExpiryCount) {
        xTimerStop(timer, 0);
    } else {
        vTimerSetTimerID(timer, (void *)expiryCounter);
        
        char msg[20];
        sprintf(msg, "Times up: %d !!!\r\n", expiryCounter);

        
        if( xSemaphoreTake( xSemaphore, ( TickType_t ) 100 ) == pdTRUE )
        {
            /* We were able to obtain the semaphore and can now access the
            shared resource. */

	        HAL_UART_Transmit(huart, msg, strlen(msg), 100);
            
            GPIO_PinState pin = HAL_GPIO_ReadPin(GPIO_OUT30_GPIO_Port, GPIO_OUT30_Pin);
            if (pin == GPIO_PIN_SET)
                HAL_UART_Transmit(huart, "\r\n\r\nAlive30\r\n\r\n", strlen("\r\n\r\nAlive30\r\n\r\n"), 100);

            pin = HAL_GPIO_ReadPin(GPIO_OUT29_GPIO_Port, GPIO_OUT29_Pin);
            if (pin == GPIO_PIN_SET)
                HAL_UART_Transmit(huart, "\r\n\r\nAlive29\r\n\r\n", strlen("\r\n\r\nAlive29\r\n\r\n"), 100);

            pin = HAL_GPIO_ReadPin(GPIO_OUT28_GPIO_Port, GPIO_OUT28_Pin);
            if (pin == GPIO_PIN_SET)
                HAL_UART_Transmit(huart, "\r\n\r\nAlive28\r\n\r\n", strlen("\r\n\r\nAlive28\r\n\r\n"), 100);

            /* We have finished accessing the shared resource.  Release the
            semaphore. */
            xSemaphoreGive( xSemaphore );
        }
    }
}

void user_task_init(UART_HandleTypeDef * handler)
{
    huart = handler;
    xQueue = xQueueCreate(mainQUEUE_LENGTH, 10 * sizeof(char));

    xSemaphore = xSemaphoreCreateMutex();

    xTaskCreate(tSampleSend, "Task Send", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(tSampleReceive, "Task Receive", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    xTimer = xTimerCreate( /* Just a text name, not used by the RTOS
                     kernel. */
                     "Timer",
                     /* The timer period in ticks, must be
                     greater than 0. */
                     350,
                     /* The timers will auto-reload themselves
                     when they expire. */
                     pdTRUE,
                     /* The ID is used to store a count of the
                     number of times the timer has expired, which
                     is initialised to 0. */
                     ( void * ) 0,
                     /* Each timer calls the same callback when
                     it expires. */
                     vTimerCallback
                   );
    xTimerStart( xTimer, 0 );
}

void tSampleReceive(void *pvParameters)
{
    char *pcTaskName;
    pcTaskName = (char *)pvParameters;
    // portTickType xLastWakeTime = xTaskGetTickCount();
    char msg[10];
    for (;;)
    {
        xQueueReceive(xQueue, msg, portMAX_DELAY);
        if( xSemaphoreTake( xSemaphore, ( TickType_t ) 100 ) == pdTRUE )
        {
            /* We were able to obtain the semaphore and can now access the
            shared resource. */

	        HAL_UART_Transmit(huart, msg, strlen(msg), 100);

            /* We have finished accessing the shared resource.  Release the
            semaphore. */
            xSemaphoreGive( xSemaphore );
        }
    }
}
//static char * a = "hello";

void tSampleSend(void *pvParameters)
{
    char *pcTaskName;
    pcTaskName = (char *)pvParameters;
    int msgi = 0;
    // portTickType xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
        xQueueSendToBack(xQueue, testString[msgi], 0);
        msgi = (msgi + 1) % 8;
        vTaskDelay(rand() % 100 + 299);
    }
}