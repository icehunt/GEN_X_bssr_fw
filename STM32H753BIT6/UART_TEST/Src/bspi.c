/*
 * bspi.c
 *
 *  Created on: Sep. 26, 2019
 *      Author: Tom B
 */
#include "bspi.h"

#define SPI_TASK_PRIORITY 3
#define SPI_TASK_STACK_SIZE 256
#define SPI_Q_SIZE 64
#define NUM_ADCS 3

// ########  ##     ##
// ##     ## ##     ##
// ##     ## ##     ##
// ########  ##     ##
// ##         ##   ##
// ##          ## ##
// ##           ###

static B_spiHandle_t* bspis[NUM_ADCS];
static SPI_HandleTypeDef* hspis[NUM_ADCS];

// ########  ######## ########
// ##     ## ##       ##     ##
// ##     ## ##       ##     ##
// ########  ######   ########
// ##        ##       ##
// ##        ##       ##
// ##        ##       ##


B_spiQMsg_t* readSpi(B_adcHandle_t *badc);
void doneReadAdc(B_spiQMsg_t *buf);
B_spiHandle_t* B_spiStart(SPI_Handle_typeDef *hspi, B_gpioPin_t* csPins, uint8_t numCS);
static void spiTask(void* pv);

// ######## ##     ## ##    ##  ######
// ##       ##     ## ###   ## ##    ##
// ##       ##     ## ####  ## ##
// ######   ##     ## ## ## ## ##
// ##       ##     ## ##  #### ##
// ##       ##     ## ##   ### ##    ##
// ##        #######  ##    ##  ######

B_spiHandle_t* B_spiStart(SPI_Handle_typeDef *hspi, B_gpioPin_t* csPins, uint8_t numCS){
	B_spiHandle_t *bspi;
	for(int i = 0; i < NUM_SPIS; i++){
		if(hspis[i] == NULL){
			hspis[i] = hspi;
			bspis[i] = pvPortMalloc(sizeof(B_spiHandle_t));
			bspi = bspis[i];
			break;
		}
	}
	bspi->hspi = hspi;
	bspi->spiQ = xQueueCreate(ADC_Q_SIZE, sizeof(b_spiQMsg_t *));
	bspi->numCS = numCS;
	bspi->csPins = csPins;
	bspi->spiSem = xSemaphoreCreateBinary();
	xTaskCreate(spiTask, "spiTask", SPITASK_STACK_SIZE, bspi, SPI_TASK_PRIORITY, &bspi->spiTask);
	return badc;
}


static void spiTask(void* pv){
	B_spiHandle_t *bspi = pv;
	uint16_t *buf;
	for(;;){
		for(int i = 0; i < bspi->numCS; i++){
			B_gpioSet(bspi->csPins[i]);
			HAL_SPI_TransmitReceive_DMA(&hspi2, lmao, lmao+8, 8);
		}
	}
}

//  ######     ###    ##       ##       ########     ###     ######  ##    ##
// ##    ##   ## ##   ##       ##       ##     ##   ## ##   ##    ## ##   ##
// ##        ##   ##  ##       ##       ##     ##  ##   ##  ##       ##  ##
// ##       ##     ## ##       ##       ########  ##     ## ##       #####
// ##       ######### ##       ##       ##     ## ######### ##       ##  ##
// ##    ## ##     ## ##       ##       ##     ## ##     ## ##    ## ##   ##
//  ######  ##     ## ######## ######## ########  ##     ##  ######  ##    ##
HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef* hspi){
	for(int i = 0; i < NUM_SPIS; i++){
		if(hspis[i] == hspi){
			xSemaphoreFiveFromISR(bspis[i]->spiSem, NULL);
		}
	}
}
