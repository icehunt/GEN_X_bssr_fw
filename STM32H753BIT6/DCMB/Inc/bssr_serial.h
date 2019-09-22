
#include "FreeRTOS.h"
#include "queue.h"
#include "stm32h7xx_hal.h"

#define SERIAL_Q_LENGTH 10
#define SERIAL_ITEM_SIZE 260 // 256 bytes of payload + 4 for header
#define BSSR_SERIAL_TX_ID 0x04
#define BSSR_SERIAL_MAX_SIZE 128
#define BSSR_SERIAL_START 0xa5
#define BSSR_SERIAL_ESCAPE 0x5a
#include "FreeRTOS.h"
#include "queue.h"
#include "stm32h7xx_hal.h"


void bssr_serial_start(UART_HandleTypeDef *uart, UART_HandleTypeDef *debug_uart, CRC_HandleTypeDef *crc);

void BSSR_Serial_Tx(char *data, char length);
void BSSR_Serial_TxTask(void *pvParameters);
void BSSR_Serial_TASK_INIT(UART_HandleTypeDef *huart, UART_HandleTypeDef *hdebug_uart);


typedef struct {
    uint8_t buf[SERIAL_ITEM_SIZE]; // 256 for data + header
    uint8_t acks;
    long timeout;
} serial_packet;
