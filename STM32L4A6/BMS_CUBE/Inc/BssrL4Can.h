#ifndef BSSR_L4_CAN_H__
#define BSSR_L4_CAN_H__
#include "stm32l4xx_hal.h"
#include <stdint.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "queue.h"

#define CAN_ENABLED 1

#define CAN_Q_LENGTH 10
#define CAN_ITEM_SIZE 8

/**
 * Blue Sky 77 BMS CAN Frame ID format:
 * bit[0-4]: 0x0
 * bit[5-7]: DEVICE ID
 * bit[8-10]: BATTERY ID (controlled by logBqResultsCan() in ../Src/main.c)
 */
#define BSSR_CAN_TX_DEVICE_ID 0x5   // change device id here
#define BSSR_CAN_FILTER_ID 0x0
#define BSSR_CAN_FILTER_MASK 0x7FE

/** Task Priorities */
#define CAN_QUEUE_RECEIVE_TASK_PRIORITY (tskIDLE_PRIORITY+2)
#define CAN_QUEUE_SEND_TASK_PRIORITY (tskIDLE_PRIORITY+1)

/** TASK intervals */
#define CAN_TRANSMIT_TASK_INTERVAL 100 /* ms */

typedef struct L4CAN {
    CAN_HandleTypeDef * handle;
    // int id;
    CAN_FilterTypeDef filterConfig;
    CAN_TxHeaderTypeDef txHeader;
    CAN_RxHeaderTypeDef rxHeader;
    uint32_t TxMailbox0, TxMailbox1, TxMailbox2;
    uint8_t TxData[8];

} BssrL4CAN;

void BSSR_CAN_Test(BssrL4CAN * can);

void BSSR_CAN_Set_Uart( UART_HandleTypeDef * uart);

void BSSR_CAN_TASK_INIT(CAN_HandleTypeDef * can, UART_HandleTypeDef * huart2);

/** start the can bus 
 * This function will only run after can initialization and only once
 **/
void BSSR_CAN_Start();

/**
 * transferrint data with id
 * @param id: 1 11-bit id value
 * @param data: 8 8-bit uint value
 */
void BSSR_CAN_Tx(uint32_t id, uint8_t * data);

// /** A task to handle the transmition */
// void BSSR_CAN_TxTask(void * pvParameters);
// void BSSR_CAN_TASK_INIT2(FDCAN_HandleTypeDef * hfdcan);
void BSSR_CAN_testTask(void * pvParameters);

// TODO init functions  SS. 8.2.2

// TODO IO Operation functions  SS. 8.2.3

// TODO state and error check functions  SS. 8.2.4

#endif
