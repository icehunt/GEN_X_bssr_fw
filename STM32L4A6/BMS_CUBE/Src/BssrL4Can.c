#include "BssrL4Can.h"
#include "task.h"
#include <string.h>
#include <stdio.h>
#define DEBUG_ON

static BssrL4CAN bCan;
static UART_HandleTypeDef * uart;

static QueueHandle_t xCanTxQueue = NULL;
static QueueHandle_t xCanRxQueue = NULL;

void BSSR_CAN_Set_Uart( UART_HandleTypeDef * huart) {
    uart = huart;
}

static void BSSR_CAN_Error(char *msg, CAN_HandleTypeDef *hcan) {
    char buffer[128];
    sprintf(buffer, "CAN TASK ERROR: %s\r\nRebooting...\r\n\r\n", msg);
    HAL_UART_Transmit(uart, buffer, strlen(buffer), 200);
    if (hcan != NULL) {
        sprintf(buffer, "\tERROR CODE: 0x%lx\r\n", HAL_CAN_GetError(hcan));
        if (uart != NULL)
            HAL_UART_Transmit(uart, buffer, strlen(buffer), 200);
    }
#ifdef DEBUG_ON
    HAL_NVIC_SystemReset();
    for (;;);
#endif
}

static inline void BSSR_CAN_Log(char *msg) {
#ifdef DEBUG_ON
    char buffer[128];
    sprintf(buffer, "CAN TASK Message: %s\r\n", msg);
    if (uart != NULL)
        HAL_UART_Transmit(uart, buffer, strlen(buffer), 1000);
#endif
}

static inline void BSSR_CAN_Status_Log(CAN_HandleTypeDef * can) {
#ifdef DEBUG_ON
    switch (HAL_CAN_GetState(can)) {
        case HAL_CAN_STATE_RESET:
            BSSR_CAN_Log("HAL_CAN_STATE_RESET");
            break;
        case HAL_CAN_STATE_READY:
            BSSR_CAN_Log("HAL_CAN_STATE_READY");
            break;  
        case HAL_CAN_STATE_LISTENING:
            BSSR_CAN_Log("HAL_CAN_STATE_LISTENING");
            break;
        case HAL_CAN_STATE_SLEEP_PENDING:
            BSSR_CAN_Log("HAL_CAN_STATE_SLEEP_PENDING");
            break;
        case HAL_CAN_STATE_SLEEP_ACTIVE:
            BSSR_CAN_Log("HAL_CAN_STATE_SLEEP_ACTIVE");
            break;
        case HAL_CAN_STATE_ERROR:
            BSSR_CAN_Log("HAL_CAN_STATE_ERROR");
            break;
        default:
            return;
    }
#endif // DEBUG
}

void BSSR_CAN_Test(BssrL4CAN * can) {
    /***
     * Example code from: https://st--c.eu10.content.force.com/sfc/dist/version/download/?oid=00Db0000000YtG6&ids=0680X000006HxTW&d=%2Fa%2F0X0000000ayX%2F88jLLXCT3K5cAKBDLIwfRvrqV8wrr5Rvq0_amyQl1dk&asPdf=false
     * 
     * 1. enable the can controller interface clock
     * __HAL_RCC_CAN1_CLK_ENABLE()
     * DONE BY CubeMX
     * 
     * 2. CAN pins configuration
     *      - Enable the CAN GPIO clock
     *          - __HAL_RCC_GPIOx_CLK_ENABLE()
     *      - Connect and configure the involved CAN pins using the following function
     *          - HAL_GPIO_Init()
     * DONE BY CubeMX
     * 
     * 3. Init and configure the CAN using the HAL_CAN_Init()
     * DONE BY CubeMX
     * 
     * 3a. Set up filter
     * 
     * 3b. HAL_CAN_Start
     * 
     * 3c. activate notification for IT operation
     * 
     * 4. Transmit the desired CAN frame using HAL_CAN_Transmit() or with _IT()
     * 
     * 5. Receive a CAN frame using HAL_CAN_Receive() or with _IT()
     * 
     **/

    // 3a) Set up filter to accept all message 
    bCan.filterConfig.FilterIdHigh = 0x0000;
    bCan.filterConfig.FilterIdLow = 0x0000;
    bCan.filterConfig.FilterMaskIdHigh = 0x0000;
    bCan.filterConfig.FilterMaskIdLow = 0x0000;
    bCan.filterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    bCan.filterConfig.FilterBank = 0;
    bCan.filterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    bCan.filterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    bCan.filterConfig.FilterActivation = ENABLE;
    bCan.filterConfig.SlaveStartFilterBank = 0;
    if (HAL_CAN_ConfigFilter(bCan.handle, &bCan.filterConfig) != HAL_OK) {
        BSSR_CAN_Error("HAL_CAN_ConfigFilter", bCan.handle);
    }

    // 3b) HAL_CAN_Start
    if (HAL_CAN_Start(bCan.handle) != HAL_OK) {
        BSSR_CAN_Error("HAL_CAN_Start", bCan.handle);        
    }

    // 3c) notification
    if (HAL_OK != (HAL_CAN_ActivateNotification(bCan.handle, 
            CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO0_FULL |
            CAN_IT_RX_FIFO0_OVERRUN | CAN_IT_RX_FIFO1_MSG_PENDING | CAN_IT_RX_FIFO1_FULL |
            CAN_IT_RX_FIFO1_OVERRUN | CAN_IT_WAKEUP | CAN_IT_SLEEP_ACK |
            CAN_IT_ERROR_WARNING | CAN_IT_ERROR_PASSIVE | CAN_IT_BUSOFF |
            CAN_IT_LAST_ERROR_CODE | CAN_IT_ERROR))) {
        BSSR_CAN_Error("HAL_CAN_ActivateNotification", bCan.handle);
    }

    // // 4a) Add Transmit message
    // bCan.txHeader.StdId = 0x321;
    // bCan.txHeader.ExtId = 0x1;
    // bCan.txHeader.IDE = CAN_ID_STD;
    // bCan.txHeader.RTR = CAN_RTR_DATA;
    // bCan.txHeader.DLC = 8;
    // bCan.txHeader.TransmitGlobalTime = DISABLE;
    // bCan.TxData[0] = 1;
    // bCan.TxData[1] = 2;
    // bCan.TxData[2] = 3;
    // bCan.TxData[3] = 4;
    // bCan.TxData[4] = 5;
    // bCan.TxData[5] = 6;
    // bCan.TxData[6] = 7;
    // bCan.TxData[7] = 8;

    xTaskCreate(BSSR_CAN_testTask, "CanTestTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    // HAL_CAN_AddTxMessage(bCan.handle, &bCan.txHeader, bCan.TxData, &bCan.TxMailbox0);
}

void BSSR_CAN_testTask(void * pvParameters) {

    char msg[CAN_ITEM_SIZE] = {0};
    int a = -1;
    vTaskDelay(1000);
    for(;;) {
        a = (a + 1) % 10;
        msg [0] = 'T';
        msg [1] = a + 48;
        msg [2] = ':';
        for (int i = 3; i < CAN_ITEM_SIZE; i++) {
            msg[i] = 'a' + (a + i) % 26;
        }
        msg [CAN_ITEM_SIZE - 1] = 0;
        BSSR_CAN_Tx(a, msg);
        vTaskDelay(997);
    }
}

void BSSR_CAN_TASK_INIT(CAN_HandleTypeDef * can, UART_HandleTypeDef * huart2) {
    bCan.handle = can;
    BSSR_CAN_Set_Uart(huart2);
    BSSR_CAN_Start();
    // BSSR_CAN_Log("CAN TxTask Started!");
    // xTaskCreate(BSSR_CAN_testTask, "CanTestTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);

    // xTaskCreate(BSSR_CAN_TxTask, "CanTxTask", configMINIMAL_STACK_SIZE, NULL, CAN_QUEUE_SEND_TASK_PRIORITY, NULL);
}

void BSSR_CAN_Start() {

    //0. Initialize the queue for can tx
    if ((xCanTxQueue = xQueueCreate(CAN_Q_LENGTH, CAN_ITEM_SIZE * sizeof(char))) == NULL) {
        //Error
        BSSR_CAN_Error("create xCanTxQueue", NULL);
    }
    if ((xCanRxQueue = xQueueCreate(CAN_Q_LENGTH, CAN_ITEM_SIZE * sizeof(char))) == NULL) {
        //Error
        BSSR_CAN_Error("create xCanRxQueue", NULL);
    }
    
    // 3a) Set up filter to accept all message 
    bCan.filterConfig.FilterIdHigh = BSSR_CAN_FILTER_ID >> 4;
    bCan.filterConfig.FilterIdLow = BSSR_CAN_FILTER_ID & 0xFFFF;
    bCan.filterConfig.FilterMaskIdHigh = BSSR_CAN_FILTER_MASK >> 4;
    bCan.filterConfig.FilterMaskIdLow = BSSR_CAN_FILTER_MASK & 0xFFFF;
    bCan.filterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    bCan.filterConfig.FilterBank = 0;
    bCan.filterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    bCan.filterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    bCan.filterConfig.FilterActivation = ENABLE;
    bCan.filterConfig.SlaveStartFilterBank = 0;

    bCan.TxMailbox[0] = CAN_TX_MAILBOX0;
    bCan.TxMailbox[1] = CAN_TX_MAILBOX1;
    bCan.TxMailbox[2] = CAN_TX_MAILBOX2;

    if (HAL_CAN_ConfigFilter(bCan.handle, &bCan.filterConfig) != HAL_OK) {
        BSSR_CAN_Error("set HAL_CAN_ConfigFilter", bCan.handle);
    }

    // 3b) HAL_CAN_Start
    if (HAL_CAN_Start(bCan.handle) != HAL_OK) {
        BSSR_CAN_Error("HAL_CAN_Start", bCan.handle);        
    }

    // 3c) notification
    if (HAL_OK != (HAL_CAN_ActivateNotification(bCan.handle, 
            CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO0_FULL |
            CAN_IT_RX_FIFO0_OVERRUN | CAN_IT_RX_FIFO1_MSG_PENDING | CAN_IT_RX_FIFO1_FULL |
            CAN_IT_RX_FIFO1_OVERRUN | CAN_IT_WAKEUP | CAN_IT_SLEEP_ACK |
            CAN_IT_ERROR_WARNING | CAN_IT_ERROR_PASSIVE | CAN_IT_BUSOFF |
            CAN_IT_LAST_ERROR_CODE | CAN_IT_ERROR))) {
        BSSR_CAN_Error("set HAL_CAN_ActivateNotification", bCan.handle);
    }
}

void BSSR_CAN_Tx(uint32_t id, uint8_t * data) {
    // 4a) Add Transmit message
    id = id & 0x7FF;
    bCan.txHeader.StdId = id;
    bCan.txHeader.IDE = CAN_ID_STD;
    bCan.txHeader.RTR = CAN_RTR_DATA;
    bCan.txHeader.DLC = 8;
    bCan.txHeader.TransmitGlobalTime = DISABLE;

    HAL_CAN_AddTxMessage(bCan.handle, &bCan.txHeader, data, bCan.TxMailbox[id % 3]);

    // 3c) notification
    if (HAL_OK != (HAL_CAN_ActivateNotification(bCan.handle, 
            CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO0_FULL |
            CAN_IT_RX_FIFO0_OVERRUN | CAN_IT_RX_FIFO1_MSG_PENDING | CAN_IT_RX_FIFO1_FULL |
            CAN_IT_RX_FIFO1_OVERRUN | CAN_IT_WAKEUP | CAN_IT_SLEEP_ACK |
            CAN_IT_ERROR_WARNING | CAN_IT_ERROR_PASSIVE | CAN_IT_BUSOFF |
            CAN_IT_LAST_ERROR_CODE | CAN_IT_ERROR))) {
        BSSR_CAN_Error("CAN_ActivateNotifaction", bCan.handle);
    }

}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) {
    // Callback
    BSSR_CAN_Error("unknown", hcan);  
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    // Callback
    BSSR_CAN_Log("RxFifo0 Message Received");  
    BSSR_CAN_Status_Log(hcan);

    uint8_t msg[8] = {0};
    if (HAL_OK == HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &bCan.rxHeader,msg)) {
        char buffer[120];
        sprintf(buffer, "MSG RECEIVED ID=%ld, CONTENT=", bCan.rxHeader.StdId);
        for (int i = 0; i < bCan.rxHeader.DLC; i ++) {
            char msgInHex[20];
            sprintf(msgInHex, "%x ", msg[i]);
            strcat(buffer, msgInHex);
        }
        BSSR_CAN_Log(buffer);
    }

    // 3c) notification
    if (HAL_OK != (HAL_CAN_ActivateNotification(bCan.handle, 
            CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO0_FULL |
            CAN_IT_RX_FIFO0_OVERRUN | CAN_IT_RX_FIFO1_MSG_PENDING | CAN_IT_RX_FIFO1_FULL |
            CAN_IT_RX_FIFO1_OVERRUN | CAN_IT_WAKEUP | CAN_IT_SLEEP_ACK |
            CAN_IT_ERROR_WARNING | CAN_IT_ERROR_PASSIVE | CAN_IT_BUSOFF |
            CAN_IT_LAST_ERROR_CODE | CAN_IT_ERROR))) {
        BSSR_CAN_Error("CAN_ActivateNotifaction", bCan.handle);
    }
}

//Unused callbacks
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan) {
    // Callback
    BSSR_CAN_Log("TxMailbox0 Complete");
}
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan) {
    // Callback
    BSSR_CAN_Log("TxMailbox1 Complete");    
}
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan) {
    // Callback
    BSSR_CAN_Log("TxMailbox2 Complete");  
}
void HAL_CAN_TxMailbox0AbortCallback(CAN_HandleTypeDef *hcan) {
    // Callback
    BSSR_CAN_Log("TxMailbox0 Abort");  
}
void HAL_CAN_TxMailbox1AbortCallback(CAN_HandleTypeDef *hcan) {
    // Callback
    BSSR_CAN_Log("TxMailbox1 Abort");  
}
void HAL_CAN_TxMailbox2AbortCallback(CAN_HandleTypeDef *hcan) {
    // Callback
    BSSR_CAN_Log("TxMailbox2 Abort");  
}

void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan) {
    // Callback
    BSSR_CAN_Log("RxFifo0 Message Full");  
}
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    // Callback
    BSSR_CAN_Log("RxFifo1 Message Received");  
}
void HAL_CAN_RxFifo1FullCallback(CAN_HandleTypeDef *hcan) {
    // Callback
    BSSR_CAN_Log("RxFifo1 Message Full");  
}
void HAL_CAN_SleepCallback(CAN_HandleTypeDef *hcan) {
    // Callback
    BSSR_CAN_Log("Sleep");  
}
void HAL_CAN_WakeUpFromRxMsgCallback(CAN_HandleTypeDef *hcan) {
    // Callback
    BSSR_CAN_Log("Wake up");  
}
