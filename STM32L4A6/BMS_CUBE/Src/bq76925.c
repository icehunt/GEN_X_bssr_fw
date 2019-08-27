/*
 * bq76925.cpp
 *
 *  Created on: 2019
 *      Author: wuyuc
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bq76925.h"
#include "FreeRTOS.h"
#include "semphr.h"

static SemaphoreHandle_t xADCSemaphore = NULL;
static BaseType_t xTaskWokenByReceive = pdFALSE;

const uint8_t BqCellSel[9] = {
    BQ_VCSEL_VSS,
    BQ_VCSEL_VC1,
    BQ_VCSEL_VC2,
    BQ_VCSEL_VC3,
    BQ_VCSEL_VC4,
    BQ_VCSEL_VC5,
    BQ_VCSEL_VC6,
    BQ_VCSEL_VREF_0_5,
    BQ_VCSEL_VREF_0_85};

static void log(UART_HandleTypeDef *uart, char *msg)
{
    HAL_UART_Transmit(uart, msg, strlen(msg), 100);
}

uint8_t BQ_getADCRunning(BQ_t *bq)
{
    uint8_t value = 0;
    if (xSemaphoreTakeFromISR(xADCSemaphore, &xTaskWokenByReceive))
    {
        /* We were able to obtain the semaphore and can now access the
        shared resource. */

        value = bq->adcRunning;

        /* We have finished accessing the shared resource.  Release the
        semaphore. */
        xSemaphoreGiveFromISR(xADCSemaphore, &xTaskWokenByReceive);
    }
}

void BQ_setADCRunning(BQ_t *bq, uint8_t value)
{
    if (xSemaphoreTakeFromISR(xADCSemaphore, &xTaskWokenByReceive))
    {
        /* We were able to obtain the semaphore and can now access the
        shared resource. */

        bq->adcRunning = value;

        /* We have finished accessing the shared resource.  Release the
        semaphore. */
        xSemaphoreGiveFromISR(xADCSemaphore, &xTaskWokenByReceive);
    }
}

void BQ_Error(BQ_t *bq)
{
    for (;;)
    {
        // stop here
    }
}

uint8_t BQ_i2cTest(BQ_t *bq)
{
    uint8_t data;
    // check for device id, this should be 0x10
    if (HAL_OK == HAL_I2C_Master_Receive(bq->i2c, 0x4e, &data, 1, 100))
    {
        if (data == 0x10)
            return BQ_OK;
    }
    return BQ_ERROR;
}

void BQ_logAllReg(BQ_t *bq, UART_HandleTypeDef *uart)
{
    log(uart, "BQ REG = \r\n");
    char buffer[100];
    for (int i = 0; i < 0x20; i++)
    {
        sprintf(buffer, "\treg[ 0x%02X ] = 0x%02X\r\n", i, bq->regMap[i]);
        log(uart, buffer);
    }
}

void BQ_writeI2C_IT(BQ_t *bq, uint8_t address, BqI2CTask task)
{
    bq->interrupt.mode = BQ_WRITE;
    bq->interrupt.address = address;
    bq->interrupt.task = task;
#ifdef DEBUG
    if (address >= 0x20)
    {
        BQ_Error(bq);
    }
    else if (bq->i2c == NULL)
    {
        BQ_Error(bq);
    }
#endif
    if (HAL_OK != HAL_I2C_Master_Transmit_IT(bq->i2c,
                                             ((BQ_GROUP_ADDRESS << 3) | (address & 0x1F)) << 1, bq->regMap + (address & 0x1F), 1))
        BQ_Error(bq);
}

void BQ_readI2C_IT(BQ_t *bq, uint8_t address, BqI2CTask task)
{
    bq->interrupt.mode = BQ_READ;
    bq->interrupt.address = address;
    bq->interrupt.task = task;
#ifdef DEBUG
    if (address >= 0x20)
    {
        BQ_Error(bq);
    }
    else if (bq->i2c == NULL)
    {
        BQ_Error(bq);
    }
#endif
    if (HAL_OK != HAL_I2C_Master_Receive_IT(bq->i2c,
                                            ((BQ_GROUP_ADDRESS << 3) | (address & 0x1F)) << 1, bq->regMap + (address & 0x1F), 1))
        BQ_Error(bq);
}

uint8_t BQ_getCurrentAlert(BQ_t *bq)
{
    return bq->regMap[STATUS_ADDRESS] & BQ_CURRENT_ALERT;
}

uint8_t BQ_getCrcErr(BQ_t *bq)
{
    return bq->regMap[STATUS_ADDRESS] & BQ_CRC_ERR;
}

uint8_t BQ_getRefSel(BQ_t *bq)
{
    return bq->regMap[CONFIG_2_ADDRESS] & BQ_REF_SEL;
}

uint8_t BQ_getChipId(BQ_t *bq)
{
    return bq->regMap[CHIP_ID_ADDRESS];
}

void BQ_setRefSel(BQ_t *bq, uint8_t sel)
{
#ifdef DEBUG
    if (sel > 1)
        BQ_Error(bq);
#endif
    bq->regMap[CONFIG_2_ADDRESS] = (bq->regMap[CONFIG_2_ADDRESS] & (~BQ_REF_SEL)) | sel;
    bq->regMap[POWER_CTL_ADDRESS] |= BQ_REF_EN;
}
void BQ_setSleep(BQ_t *bq, uint8_t s)
{
#ifdef DEBUG
    if (s > 1)
        BQ_Error(bq);
#endif
    bq->regMap[POWER_CTL_ADDRESS] = (bq->regMap[POWER_CTL_ADDRESS] & (~BQ_SLEEP)) | (s << 7);
    bq->regMap[POWER_CTL_ADDRESS] = (bq->regMap[POWER_CTL_ADDRESS] & (~BQ_SLEEP_DIS)) | ((1 - s) << 6);
    bq->regMap[POWER_CTL_ADDRESS] |= 0b110U;
}

void BQ_readBattery(BQ_t *bq, uint8_t index, uint8_t caliMode)
{
    if (index == 0)
        index = bq->batIndex;
#ifdef DEBUG
    if (bq->batSetCallback == NULL || bq->batDoneCallback == NULL)
        BQ_Error(bq);
#endif
    if (!index)
        return;
    if (bq->interrupt.task == IDLE)
    {
        BQ_setADCRunning(bq, 0);
        // start of the task
        // step 1 cell sel
        bq->batIndex = index;
        bq->caliMode = caliMode;
        bq->regMap[CELL_CTL_ADDRESS] = BqCellSel[index];
        bq->readDone = 0;
        BQ_writeI2C_IT(bq, CELL_CTL_ADDRESS, READ_BAT);
    }
    else if (bq->interrupt.mode == BQ_WRITE &&
             bq->interrupt.address == CELL_CTL_ADDRESS)
    {
        // step 2 let adc start reading
        BQ_setADCRunning(bq, 1);
        if (bq->batSetCallback != NULL)
            bq->batSetCallback(bq);
        // start read all register
        bq->interrupt.address = 0x1F;
    }
    else if (bq->interrupt.mode == BQ_WRITE && bq->adcRunning == 0 && !bq->caliMode)
    {
        BQ_readI2C_IT(bq, VREF_CAL_ADDRESS, READ_BAT);
    }
    else if (bq->interrupt.mode == BQ_READ && bq->interrupt.address == VREF_CAL_ADDRESS && !bq->caliMode)
    {
        BQ_readI2C_IT(bq, VC_CAL_EXT_1_ADDRESS, READ_BAT);
    }
    else if (bq->interrupt.mode == BQ_READ && bq->interrupt.address == VC_CAL_EXT_1_ADDRESS && !bq->caliMode)
    {
        BQ_readI2C_IT(bq, VC_CAL_EXT_2_ADDRESS, READ_BAT);
    }
    else if (bq->interrupt.mode == BQ_READ && bq->interrupt.address == VC_CAL_EXT_2_ADDRESS && !bq->caliMode)
    {
        BQ_readI2C_IT(bq, VREF_CAL_EXT_ADDRESS, READ_BAT);
    }
    else if (bq->interrupt.mode == BQ_READ && bq->interrupt.address == VREF_CAL_EXT_ADDRESS && !bq->caliMode)
    {
        // read the corresponding battery
        BQ_readI2C_IT(bq, VREF_CAL_ADDRESS + index, READ_BAT);
    }
    else // if (bq->interrupt.mode == BQ_READ) // the last step
    {
        // bq->batIndex = 0;
        // reading part done
        bq->interrupt.task = IDLE;
        bq->readDone = 1;
        if (bq->batDoneCallback != NULL)
            bq->batDoneCallback(bq);
    }
}

void BQ_readTemp(BQ_t *bq, uint8_t tempIndex, uint8_t start)
{
    if (start)
    {
        bq->tempIndex = tempIndex;
        bq->readDone = 0;
        bq->adcRunning = 1;
    }
    else
    {
        bq->readDone = 1;
    }
}

double BQ_getTempVoltage(BQ_t *bq, double ADC_read, UART_HandleTypeDef *uart)
{
    // log(buffer, uart);   
    return BQ_getVoltageFromAdc(bq, ADC_read);
}

double BQ_getTempResists(BQ_t *bq, double volt) {
    return volt * PULLUP_mOhm / 1000. / (VCC3V3_mV / 1000. - volt);
}

double BQ_getTempResult(BQ_t *bq, double volt) {
    return (mohms_to_milliCelcius((int)(BQ_getTempResists(bq, volt) * 1000))) / 1000.;
}

uint8_t BQ_ADCCalibration(BQ_t *bq)
{
    double x1 = 0.5 * (BQ_getRefSel(bq) ? 3 : 1.5);
    double x2 = 0.85 * (BQ_getRefSel(bq) ? 3 : 1.5);
    double y1 = bq->cali.vref_0_5;
    double y2 = bq->cali.vref_0_8_5;
    double slope = (y2 - y1) / (x2 - x1);
    double offset = y1 - slope * x1;
    bq->cali.slope = slope;
    bq->cali.offset = offset;
    return (int) bq->cali.slope;
}

double BQ_getVoltageFromAdc(BQ_t *bq, double adcValue)
{
    return 1. * (adcValue - bq->cali.offset) / bq->cali.slope;
}

int num2toSigned(int num, int bits)
{
    if (num < (2 << (bits - 2)))
    {
        return num;
    }
    else
    {
        return -1 * ((2 << (bits - 1)) - num);
    }
}

double BQ_getVoltage(BQ_t *bq, double ADC_read, UART_HandleTypeDef *uart)
{
    double VREF_NOMINAL = BQ_getRefSel(bq) ? 3 : 1.5;
    double G_VCOUT = BQ_getRefSel(bq) ? 0.6 : 0.3;

    uint8_t VCn_GC_4, VCn_GAIN_CORR, VCn_OC_4, VCn_OFFSET_CORR;
    switch (bq->batIndex)
    {
    case 1:
        VCn_OFFSET_CORR = (bq->regMap[VC1_CAL_ADDRESS] >> 4) & 0xF;
        VCn_GAIN_CORR = bq->regMap[VC1_CAL_ADDRESS] & 0xF;
        VCn_OC_4 = (bq->regMap[VC_CAL_EXT_1_ADDRESS] >> 7) & 0x1;
        VCn_GC_4 = (bq->regMap[VC_CAL_EXT_1_ADDRESS] >> 6) & 0x1;
        break;
    case 2:
        VCn_OFFSET_CORR = (bq->regMap[VC2_CAL_ADDRESS] >> 4) & 0xF;
        VCn_GAIN_CORR = bq->regMap[VC2_CAL_ADDRESS] & 0xF;
        VCn_OC_4 = (bq->regMap[VC_CAL_EXT_1_ADDRESS] >> 5) & 0x1;
        VCn_GC_4 = (bq->regMap[VC_CAL_EXT_1_ADDRESS] >> 4) & 0x1;
        break;
    case 3:
        VCn_OFFSET_CORR = (bq->regMap[VC3_CAL_ADDRESS] >> 4) & 0xF;
        VCn_GAIN_CORR = bq->regMap[VC3_CAL_ADDRESS] & 0xF;
        VCn_OC_4 = (bq->regMap[VC_CAL_EXT_2_ADDRESS] >> 7) & 0x1;
        VCn_GC_4 = (bq->regMap[VC_CAL_EXT_2_ADDRESS] >> 6) & 0x1;
        break;
    case 4:
        VCn_OFFSET_CORR = (bq->regMap[VC4_CAL_ADDRESS] >> 4) & 0xF;
        VCn_GAIN_CORR = bq->regMap[VC4_CAL_ADDRESS] & 0xF;
        VCn_OC_4 = (bq->regMap[VC_CAL_EXT_2_ADDRESS] >> 5) & 0x1;
        VCn_GC_4 = (bq->regMap[VC_CAL_EXT_2_ADDRESS] >> 4) & 0x1;
        break;
    case 5:
        VCn_OFFSET_CORR = (bq->regMap[VC5_CAL_ADDRESS] >> 4) & 0xF;
        VCn_GAIN_CORR = bq->regMap[VC5_CAL_ADDRESS] & 0xF;
        VCn_OC_4 = (bq->regMap[VC_CAL_EXT_2_ADDRESS] >> 3) & 0x1;
        VCn_GC_4 = (bq->regMap[VC_CAL_EXT_2_ADDRESS] >> 2) & 0x1;
        break;
    case 6:
        VCn_OFFSET_CORR = (bq->regMap[VC6_CAL_ADDRESS] >> 4) & 0xF;
        VCn_GAIN_CORR = bq->regMap[VC6_CAL_ADDRESS] & 0xF;
        VCn_OC_4 = (bq->regMap[VC_CAL_EXT_2_ADDRESS] >> 1) & 0x1;
        VCn_GC_4 = (bq->regMap[VC_CAL_EXT_2_ADDRESS] >> 0) & 0x1;
        break;
    default:
        BQ_Error(bq);
    }

    int VREF_GC_4 = (bq->regMap[VREF_CAL_EXT_ADDRESS] & 0x1);
    int VREF_GAIN_CORR = (bq->regMap[VREF_CAL_ADDRESS] & 0xF);
    int VREF_OC_5 = ((bq->regMap[VREF_CAL_EXT_ADDRESS] >> 2) & 0x1);
    int VREF_OC_4 = ((bq->regMap[VREF_CAL_EXT_ADDRESS] >> 1) & 0x1);
    int VREF_OFFSET_CORR = ((bq->regMap[VREF_CAL_ADDRESS] >> 4) & 0xF);

    // ADC_Read = ADC_Count / Full_Scale_Count
    //  VREF_NOMINAL sels 1.5V or 3V by REF_SEL: CONFIG_2 = 0x04
    double VCOUT = (ADC_read - bq->cali.offset) / bq->cali.slope;
    // VCn_GC_4 = 0x17 ~ 0x18
    // VCn_GAIN_CORR = 0x11 ~ 0x16
    double GC_VCOUT = num2toSigned((VCn_GC_4 << 4) + VCn_GAIN_CORR, 5) * 0.001;
    // VCn_OC_4  = px17 ~ 0x18
    // VCn_OFFSET_CORR = 0x11 ~ 0x16
    double OC_VCOUT = num2toSigned((VCn_OC_4 << 4) + VCn_OFFSET_CORR, 5) * 0.001;
    // VREF_GC_4  = 0x1B
    // VREF_GAIN_CORR = 0x10
    // VREF_OC_5  = 0x1B
    // VREF_OC_4  = 0x1B
    // VREF_OFFSET_CORR = 0x10
    // VREF_NOMINAL = 0x04
    double GC_VREF = (1 + num2toSigned((VREF_GC_4 << 4) + VREF_GAIN_CORR, 5) * 0.001) +
                     num2toSigned((VREF_OC_5 << 5) + (VREF_OC_4 << 4) + VREF_OFFSET_CORR, 6) * 0.001 /
                         VREF_NOMINAL;
    // char buffer[400];
    // sprintf(buffer, "")
    // if (uart) {
    //     char buffer[200];
    //     sprintf(buffer, "\tDetail\tGC_VCOUT:%d\tVREF:%d\tVCn_GC_4:%d\t", (int)(GC_VCOUT));
    //     HAL_UART_Transmit(&huart2, buffer, strlen(buffer), 1000);
    // }
    double voltage = (VCOUT * GC_VREF + OC_VCOUT) / G_VCOUT * (1 + GC_VCOUT);
    bq->cellVoltages[bq->batIndex] = voltage;

    return voltage;
}

uint8_t BQ_checkVoltage(BQ_t *bq, uint8_t index) {
    static double lowVolts[] = {2.4175, 2.495, 2.5725, 2.619, 2.6345, 2.65};
    static double highVolts[] = {4.4325, 4.355, 4.2775, 4.231, 4.2155, 4.2};
    
    double volt = bq->cellVoltages[index];
    bq->warningCode = BQ_GOOD;  // Reset BQ_warningCode

    /* check for the low volt warnings */
    static int lowVoltsLen = sizeof(lowVolts) / sizeof(lowVolts[0]);
    for (int i = 0; i < lowVoltsLen; i++) {
        if (volt < lowVolts[i]) {
            bq->warningCode = BQ_VOLTAGE_TOO_LOW_15 + i;
            return bq->warningCode;
        }
    }
    
    /* check for the high volt warnings */
    static int highVoltsLen = sizeof(highVolts) / sizeof(highVolts[0]);
    for (int i = 0; i < highVoltsLen; i++) {
        if (volt > highVolts[i]) {
            bq->warningCode = BQ_VOLTAGE_TOO_HIGH_15 + i;
            return bq->warningCode;
        }
    }

    return bq->warningCode;
}

uint8_t BQ_checkTemp(BQ_t *bq, double temp) {
    static double lowTemps[] = {-6.75, -4.5, -2.25, -0.9, -0.45};
    static double highTemps[] = {51.75, 49.5, 47.25, 45.9, 45.45, 45};
    
    bq->warningCode = BQ_GOOD;  // Reset BQ_warningCode

    /* check for the low temp warnings */
    static int lowTempsLen = sizeof(lowTemps) / sizeof(lowTemps[0]);
    for (int i = 0; i < lowTempsLen; i++) {
        if (temp < lowTemps[i]) {
            bq->warningCode = BQ_TEMP_TOO_LOW_15 + i;
            return bq->warningCode;
        }
    }
    
    /* check for the high temp warnings */
    static int highTempsLen = sizeof(highTemps) / sizeof(highTemps[0]);
    for (int i = 0; i < highTempsLen; i++) {
        if (temp > highTemps[i]) {
            bq->warningCode = BQ_TEMP_TOO_HIGH_15 + i;
            return bq->warningCode;
        }
    }

    return bq->warningCode;
}

const char* BQ_getWarningMsg(BQ_t *bq) {
    // static char noWarning[] = "BQ is OK";
    static char warningMsg[][32] = {
        [0] = "BQ is OK",
        [0x10] = "BQ_VOLTAGE_TOO_LOW < 15%%",
            "BQ_VOLTAGE_TOO_LOW < 10%%",
            "BQ_VOLTAGE_TOO_LOW <  5%%",
            "BQ_VOLTAGE_TOO_LOW <  2%%",
            "BQ_VOLTAGE_TOO_LOW <  1%%",
            "BQ_VOLTAGE_TOO_LOW <  0%%",
        [0x20] = "BQ_VOLTAGE_TOO_HIGH > 15%%",
            "BQ_VOLTAGE_TOO_HIGH > 10%%",
            "BQ_VOLTAGE_TOO_HIGH >  5%%",
            "BQ_VOLTAGE_TOO_HIGH >  2%%",
            "BQ_VOLTAGE_TOO_HIGH >  1%%",
            "BQ_VOLTAGE_TOO_HIGH >  0%%",
        [0x30] = "BQ_TEMP_TOO_LOW < 15%%",
            "BQ_TEMP_TOO_LOW < 10%%",
            "BQ_TEMP_TOO_LOW <  5%%",
            "BQ_TEMP_TOO_LOW <  2%%",
            "BQ_TEMP_TOO_LOW <  1%%",
            "BQ_TEMP_TOO_LOW <  0%%",
        [0x40] = "BQ_TEMP_TOO_HIGH > 15%%",
            "BQ_TEMP_TOO_HIGH > 10%%",
            "BQ_TEMP_TOO_HIGH >  5%%",
            "BQ_TEMP_TOO_HIGH >  2%%",
            "BQ_TEMP_TOO_HIGH >  1%%",
            "BQ_TEMP_TOO_HIGH >  0%%"
    };
    // static char lowVoltageWarningMsgs[][32] = {
    //     "BQ_VOLTAGE_TOO_LOW < 15%%",
    //     "BQ_VOLTAGE_TOO_LOW < 10%%",
    //     "BQ_VOLTAGE_TOO_LOW <  5%%",
    //     "BQ_VOLTAGE_TOO_LOW <  2%%",
    //     "BQ_VOLTAGE_TOO_LOW <  1%%",
    //     "BQ_VOLTAGE_TOO_LOW <  0%%"
    // };

    // static char highVoltageWarningMsgs[][32] = {
    //     "BQ_VOLTAGE_TOO_HIGH > 15%%",
    //     "BQ_VOLTAGE_TOO_HIGH > 10%%",
    //     "BQ_VOLTAGE_TOO_HIGH >  5%%",
    //     "BQ_VOLTAGE_TOO_HIGH >  2%%",
    //     "BQ_VOLTAGE_TOO_HIGH >  1%%",
    //     "BQ_VOLTAGE_TOO_HIGH >  0%%"
    // };

    // static char lowTempWarningMsgs[][32] = {
    //     "BQ_TEMP_TOO_LOW < 15%%",
    //     "BQ_TEMP_TOO_LOW < 10%%",
    //     "BQ_TEMP_TOO_LOW <  5%%",
    //     "BQ_TEMP_TOO_LOW <  2%%",
    //     "BQ_TEMP_TOO_LOW <  1%%",
    //     "BQ_TEMP_TOO_LOW <  0%%"
    // };

    // static char highTempWarningMsgs[][32] = {
    //     "BQ_TEMP_TOO_HIGH > 15%%",
    //     "BQ_TEMP_TOO_HIGH > 10%%",
    //     "BQ_TEMP_TOO_HIGH >  5%%",
    //     "BQ_TEMP_TOO_HIGH >  2%%",
    //     "BQ_TEMP_TOO_HIGH >  1%%",
    //     "BQ_TEMP_TOO_HIGH >  0%%"
    // };


    // if (bq->warningCode >= 0x10 && bq->warningCode < 0x20) {
    //     return lowVoltageWarningMsgs[bq->warningCode - 0x10];
    // } else 
    // if (bq->warningCode >= 0x20 && bq->warningCode < 0x30) {
    //     return &highVoltageWarningMsgs[bq->warningCode - 0x20];
    // }
    // if (bq->warningCode >= 0x30 && bq->warningCode < 0x30) {
    //     return lowVoltageWarningMsgs[bq->warningCode - 0x30];
    // } else 
    // if (bq->warningCode >= 0x40 && bq->warningCode < 0x40) {
    //     return &highVoltageWarningMsgs[bq->warningCode - 0x40];
    // }
    return warningMsg[bq->warningCode];
}



void BQ_updateStatus_IT(BQ_t *bq)
{
    if (xADCSemaphore == NULL)
        xADCSemaphore = xSemaphoreCreateMutex();
    static int readProgress = 0;
    if (bq->interrupt.task == IDLE)
    {
        // start of the task
        // step 1 reset POR
        bq->regMap[STATUS_ADDRESS] = bq->regMap[STATUS_ADDRESS] & (!BQ_POWER);
        BQ_writeI2C_IT(bq, STATUS_ADDRESS, UPDATE_SATUS);
    }
    else if (bq->interrupt.mode == BQ_WRITE && bq->interrupt.address == STATUS_ADDRESS)
    {
        // step 2 set REF_EN
        BQ_writeI2C_IT(bq, POWER_CTL_ADDRESS, UPDATE_SATUS);
    }
    else if (bq->interrupt.mode == BQ_WRITE && bq->interrupt.address == POWER_CTL_ADDRESS)
    {
        // step 2 set CONFIG_2
        BQ_writeI2C_IT(bq, CONFIG_2_ADDRESS, UPDATE_SATUS);
    }
    else if (bq->interrupt.mode == BQ_WRITE && bq->interrupt.address == CONFIG_2_ADDRESS)
    {
        // step 3 read All reg
        readProgress = 0;
        BQ_readI2C_IT(bq, readProgress, UPDATE_SATUS);
    }
    else if (bq->interrupt.mode == BQ_READ && bq->interrupt.address == readProgress &&
             readProgress < 0x1F)
    {
        // step 4 read CHIP_ID
        readProgress++;
        BQ_readI2C_IT(bq, readProgress, UPDATE_SATUS);
    }
    else
    {
        // end task
        bq->interrupt.task = IDLE;
        if (bq->statusUpdateDone != NULL)
            bq->statusUpdateDone(bq);
    }
}

void BQ_I2cCallback(BQ_t *bq, I2C_HandleTypeDef *i2c)
{
    if (i2c != bq->i2c)
        return;
    if (bq->interrupt.mode == BQ_WRITE && bq->interrupt.task == USER_DEFINED &&
        bq->i2cTxCallback != NULL)
        bq->i2cTxCallback(bq);
    else if (bq->interrupt.mode == BQ_READ && bq->interrupt.task == USER_DEFINED &&
             bq->i2cRxCallback != NULL)
        bq->i2cRxCallback(bq);
    else if (bq->interrupt.task == UPDATE_SATUS)
        BQ_updateStatus_IT(bq);
    else if (bq->interrupt.task == READ_BAT)
        BQ_readBattery(bq, 0, 0);
}
