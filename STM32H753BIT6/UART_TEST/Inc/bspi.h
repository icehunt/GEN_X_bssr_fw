#include "main.h"
#include "cmsis_os.h"
#include "bgpio.h"

typedef struct{
	B_gpioPin_t csPin;
	uint16_t data;
}B_spiQMsg_t;

typedef struct{
	SPI_HandleTypeDef* hspi;
	QueueHandle_t 			spiQ;
	TaskHandle_t			spiTask;
	uint8_t					numCS;
	SemaphoreHandle_t 		spiSem;
	B_gpioPin_t				*csPins;
}B_spiHandle_t;

B_spiQMsg_t* readSpi(B_spiHandle_t *badc);
void doneReadSpi(B_spiQMsg_t *buf);
B_spiHandle_t* B_spiStart(SPI_Handle_typeDef *hspi, B_gpioPin_t* csPins, uint8_t numCS);

