#include "bssr_serial.h"
#include "stm32h7xx_hal.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "usart.h"
#include "crc.h"
static UART_HandleTypeDef *uart;
static UART_HandleTypeDef *debug_uart;
static QueueHandle_t xSerialTxQueue = NULL;
static QueueHandle_t xSerialRxQueue = NULL;
static CRC_HandleTypeDef *crc;
static char msg[BSSR_SERIAL_MAX_SIZE + 1] = {0};
static char buffer[BSSR_SERIAL_MAX_SIZE +1];
uint8_t tx_seq_num;
int txAvailable = 1;
SemaphoreHandle_t uart4TxSem;

static volatile double fake_rand() {
    static int seed = 1;
    const static int a = 1627, b = 2549, m = 10000;
    seed = (seed * a + b) % m;
    return seed/m;
}

static void BSSR_Serial_Error(char *msg, UART_HandleTypeDef *huart){
    char buffer[128];
    sprintf(buffer, "Serial TASK: %s\r\n", msg);
    HAL_UART_Transmit_IT(debug_uart, buffer, strlen(buffer));
    if(huart != NULL){
        sprintf(buffer, "\t ERROR CODE: 0x%x\r\n", HAL_UART_GetError(huart));
        HAL_UART_Transmit_IT(debug_uart, buffer, strlen(buffer));
    }
}

static void BSSR_Serial_Log(char *msg){
    char buffer[128];
    sprintf(buffer, "CAN TASK Message: %s\r\n", msg);
    HAL_UART_Transmit_IT(debug_uart, buffer, strlen(buffer));
}

static inline void BSSR_Serial_Status_Log(UART_HandleTypeDef *huart){
    switch (HAL_UART_GetState(huart))
    {
    case HAL_UART_STATE_RESET:
        BSSR_Serial_Log("HAL_UART_STATE_RESET");
        break;
    case HAL_UART_STATE_READY:
        BSSR_Serial_Log("HAL_UART_STATE_READY");
        break;
    case HAL_UART_STATE_ERROR:
        BSSR_Serial_Log("HAL_UART_STATE_ERROR");
        break;
    case HAL_UART_STATE_BUSY:
        BSSR_Serial_Log("HAL_UART_STATE_Busy");
    default:
        break;
    }
}

void BSSR_Serial_TASK_INIT(UART_HandleTypeDef *huart, UART_HandleTypeDef *hdebug_uart){
    uart = huart;
    debug_uart = hdebug_uart;
    BSSR_Serial_Start();
    BSSR_Serial_Log("Serial task started");
    xTaskCreate(BSSR_Serial_TxTask, "SerialTxTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
}

void BSSR_Serial_Start(){
    uart4TxSem = xSemaphoreCreateBinary();
    xSemaphoreGive(uart4TxSem);
    xSerialTxQueue = xQueueCreate(SERIAL_Q_LENGTH, SERIAL_ITEM_SIZE * sizeof(char));
    if(xSerialTxQueue == NULL){
        BSSR_Serial_Error("create xSerialTxQueue", NULL);
    }
    xSerialRxQueue = xQueueCreate(SERIAL_Q_LENGTH, SERIAL_ITEM_SIZE * sizeof(char));
    if(xSerialRxQueue == NULL){
        BSSR_Serial_Error("create xSerialRxQueue", NULL);
    }
    txAvailable = 1;
}

void BSSR_Serial_Tx(char *data, char length){
    char d[SERIAL_ITEM_SIZE];
    d[0] = BSSR_SERIAL_START;
    d[2] = BSSR_SERIAL_TX_ID;
    d[3] = 0; // Set this to 0 to indicate to the tx task that this is not an ack. 
    int payload_length = 4;

    for(int i = 0; i < length; i++){
        if(data[i] == BSSR_SERIAL_START || data[i] == BSSR_SERIAL_ESCAPE){
            d[payload_length] = BSSR_SERIAL_ESCAPE;
            payload_length++;
        }
        d[payload_length] = data[i];
        payload_length++;
    }
    for(int i = 0; i <= (payload_length) %4; i++){
        d[payload_length+i] = 0;
    }
    payload_length = payload_length + payload_length%4;
    d[1] = payload_length-4;
    xQueueSendToBack(xSerialTxQueue, d, 0);
}

void BSSR_Serial_TxTask(void *pvParameters){
    char msg[SERIAL_ITEM_SIZE+4];
    uint32_t crc_result;
    for(;;){
        if(xQueueReceive(xSerialTxQueue, msg, portMAX_DELAY) == pdTRUE){
            xSemaphoreTake(uart4TxSem, portMAX_DELAY);
            msg[3] = tx_seq_num;
            tx_seq_num++;
            crc_result = ~HAL_CRC_Calculate(&hcrc, msg, msg[1]+4);
            //memcpy(msg[msg[1]+4], crc_result, sizeof(uint32_t));
            //msg[msg[1]+4] = crc_result;
            msg[msg[1]+7] = crc_result &255;
            msg[msg[1]+6] = (crc_result>>8) &255;
            msg[msg[1]+5] = (crc_result>>16) &255;
            msg[msg[1]+4] = (crc_result>>24) &255;

            HAL_UART_Transmit_IT(uart, msg, (int) msg[1] + 8);
            //HAL_UART_Transmit_IT(uart, crc_result, 4);
            txAvailable = 0;
            xSemaphoreTake(uart4TxSem, portMAX_DELAY);
            xSemaphoreGive(uart4TxSem);
        }
    }
}

void HAL_UART_TxCpltCallback (UART_HandleTypeDef * huart){
    if(huart == &huart4){
        xSemaphoreGive(uart4TxSem);
    }
}
