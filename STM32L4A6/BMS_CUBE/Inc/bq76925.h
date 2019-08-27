#ifndef BQ76925_H__
#include "stm32l4xx_hal.h"
#include <stdint.h>
#include <stdlib.h>
#include "thermistor.h"

#define VCC3V3_mV 3300
#define PULLUP_mOhm 10000000

// debug mode switch
#ifndef DEBUG
	#define DEBUG
#endif 

// device address
#define BQ_GROUP_ADDRESS (0b0100U)

// constants
#define BQ_OK (1U)
#define BQ_ERROR (0U)

// adc full counts 
#define ADC_FULL_SCALE_COUNT (4096)

// r/o mode
#define BQ_READ (0U)
#define BQ_WRITE (1U)

// over current alert; ADDRESS=0x0
#define BQ_CURRENT_ALERT (1U << 2)
#define BQ_CRC_ERR (1U << 1)
#define BQ_POWER (1U)

// vcout sel ADDRESS=0x01
#define BQ_VCSEL_VSS (0U)
#define BQ_VCSEL_VREF_0_5 (2U << 4)
#define BQ_VCSEL_VREF_0_85 (3U << 4)
#define BQ_VCSEL_VC1 ((1U << 4) + 0U)
#define BQ_VCSEL_VC2 ((1U << 4) + 1U)
#define BQ_VCSEL_VC3 ((1U << 4) + 2U)
#define BQ_VCSEL_VC4 ((1U << 4) + 3U)
#define BQ_VCSEL_VC5 ((1U << 4) + 4U)
#define BQ_VCSEL_VC6 ((1U << 4) + 5U)
#define BQ_VCSEL_VTEMP ((1U << 4) + 6U)

// balance control (turns on) ADDRESS=0x02
#define BQ_BAL_1 (1U)
#define BQ_BAL_2 (1U << 1)
#define BQ_BAL_3 (1U << 2)
#define BQ_BAL_4 (1U << 3)
#define BQ_BAL_5 (1U << 4)
#define BQ_BAL_6 (1U << 5)

// typedef uint8_t BqConfig1, all about current, not used
// config 2     // ADDRESS=0x04
#define BQ_CRC_EN (1U << 7)
#define BQ_REF_SEL (1U)

// power control ADDRESS=0x04 + 0
#define BQ_SLEEP (1U << 7)
#define BQ_SLEEP_DIS (1U << 6)
#define BQ_I_COMP_EN (1U << 4) // curren comparator
#define BQ_AMP_EN (1U << 3)    // current amplifier
#define BQ_VC_AMP_EN (1U << 2) // cell amplifier
#define BQ_VTB_EN (1U << 1)    // thermistor bias
#define BQ_REF_EN (1U << 0)    // voltage ref

#define BQ_MODE_VOLTAGE 0U
#define BQ_MODE_TEMPERATURE 1U

#define BQ_GOOD (1U)

typedef enum {
    BQ_VOLTAGE_TOO_LOW_15 = 0x10,
    BQ_VOLTAGE_TOO_LOW_10,
    BQ_VOLTAGE_TOO_LOW_5 ,
    BQ_VOLTAGE_TOO_LOW_2 ,
    BQ_VOLTAGE_TOO_LOW_1,
    BQ_VOLTAGE_TOO_LOW_0
} LowVoltageWarning;

typedef enum {
    BQ_VOLTAGE_TOO_HIGH_15 = 0x20,
    BQ_VOLTAGE_TOO_HIGH_10,
    BQ_VOLTAGE_TOO_HIGH_5 ,
    BQ_VOLTAGE_TOO_HIGH_2 ,
    BQ_VOLTAGE_TOO_HIGH_1,
    BQ_VOLTAGE_TOO_HIGH_0
} HighVoltageWarning;

typedef enum {
    BQ_TEMP_TOO_LOW_15 = 0x30,
    BQ_TEMP_TOO_LOW_10,
    BQ_TEMP_TOO_LOW_5 ,
    BQ_TEMP_TOO_LOW_2 ,
    BQ_TEMP_TOO_LOW_1,
    BQ_TEMP_TOO_LOW_0
} LowTempWarning;

typedef enum {
    BQ_TEMP_TOO_HIGH_15 = 0x40,
    BQ_TEMP_TOO_HIGH_10,
    BQ_TEMP_TOO_HIGH_5 ,
    BQ_TEMP_TOO_HIGH_2 ,
    BQ_TEMP_TOO_HIGH_1,
    BQ_TEMP_TOO_HIGH_0
} HighTempWarning;

// reg address
typedef enum {
	STATUS_ADDRESS = (0x0U),
	CELL_CTL_ADDRESS = (0x1U),
	BAL_CTL_ADDRESS = (0x2U),
	CONFIG_1_ADDRESS = (0x3U),
	CONFIG_2_ADDRESS = (0x4U),
	POWER_CTL_ADDRESS = (0x5U),
	// 0x6 reserved
	CHIP_ID_ADDRESS = (0x7U),
	// 0x8 ~ 0xF reserved
	VREF_CAL_ADDRESS = (0x10U),
	VC1_CAL_ADDRESS = (0x11U),
	VC2_CAL_ADDRESS = (0x12U),
	VC3_CAL_ADDRESS = (0x13U),
	VC4_CAL_ADDRESS = (0x14U),
	VC5_CAL_ADDRESS = (0x15U),
	VC6_CAL_ADDRESS = (0x16U),
	VC_CAL_EXT_1_ADDRESS = (0x17U),
	VC_CAL_EXT_2_ADDRESS = (0x18U),
	VREF_CAL_EXT_ADDRESS = (0x1BU)
} BqRegAddress;

// tasks of BQ board
typedef enum  {
	IDLE = 0,
	USER_DEFINED = 1,
	UPDATE_SATUS = 2,
	READ_BAT = 3,
} BqI2CTask;

typedef struct  {
	uint8_t mode;
	uint8_t address;
	BqI2CTask task;
} BqInterrupt_t;

// cell select list
extern const uint8_t BqCellSel[9];

/**
 * the calibrated formula for adc is voltage = (readout - offset) / slope
 */
typedef struct BQ_ADC_Cali{
	double vref_0_5;
	double vref_0_8_5;
	double slope;
	double offset;
} BQ_ADC_Cali_t;

typedef struct BQ{
    I2C_HandleTypeDef *i2c; // I2C Handle object, need to be set at first
	BqInterrupt_t interrupt;
	BQ_ADC_Cali_t cali;
	uint8_t regMap[0x20];
	uint8_t adcRunning; 
	uint8_t batIndex;
    uint8_t tempIndex;
    uint8_t caliMode;
	uint8_t readDone;
    uint8_t mode;
    double cellVoltages[6];
    uint8_t warningCode;

    /** 
     * status update finished callback
     */
    void (*statusUpdateDone)(struct BQ *);

    /**
     * tx done callback
     */
    void (*i2cTxCallback)(struct BQ *);

    /**
     * tx done callback
     */
    void (*i2cRxCallback)(struct BQ *);

    /**
     * battery set 
     * (adc should start recording)
     * ADC should start to read 
     */
    void (*batSetCallback)(struct BQ *);

    /**
     * BQ read done
     * (adc should stop recording)
     * and read BQ_getVoltage here
     * ADC should push the data 
     */
    void (*batDoneCallback)(struct BQ *);
} BQ_t;

/**
 * Helper function
 * from 2's complement number to signed number
 */
int num2toSigned(int bin, int bits);

/**
 * adc calibration
 */
uint8_t BQ_ADCCalibration(BQ_t * bq);

/**
 * get real world voltage from adc
 */
double BQ_getVoltageFromAdc(BQ_t * bq, double adcValue);

/**
 * ATTENTION: This funtion is protected by mutex
 */
uint8_t BQ_getADCRunning(BQ_t * bq);

/**
 * ATTENTION: This funtion is protected by mutex
 */
void BQ_setADCRunning(BQ_t * bq, uint8_t value);

/**
 * BQ I2C Test
 * ANCHOR This is a blokcing operation
 */
uint8_t BQ_i2cTest(BQ_t* bq);

/**
 * print BQ reg data
 * ANCHOR This is a blocking operation
 */
void BQ_logAllReg(BQ_t* bq, UART_HandleTypeDef* uart);

/**
 * internal eror handle
 */
void BQ_Error(BQ_t* bq);

void BQ_I2cCallback(BQ_t* bq, I2C_HandleTypeDef *i2c);

/**
 * write config and read back status of the BQ baord
 */
void BQ_updateStatus_IT(BQ_t* bq);

/**
 * set up write interrupt
 */
void BQ_writeI2C_IT(BQ_t* bq, uint8_t address, BqI2CTask task);

/**
 * set up read interrupt
 */
void BQ_readI2C_IT(BQ_t* bq, uint8_t address, BqI2CTask task);

/**
 * Mutators
 */
uint8_t BQ_getCurrentAlert(BQ_t *bq);
uint8_t BQ_getCrcErr(BQ_t *bq);
uint8_t BQ_getRefSel(BQ_t *bq);
uint8_t BQ_getChipId(BQ_t *bq);

void BQ_setRefSel(BQ_t *bq, uint8_t sel);
void BQ_setSleep(BQ_t *bq, uint8_t sleep);

/**
 * start read battery task 
 * NOTE index cannot be 0
 * index in [1...6] is for VCout_n
 * index = 7, caliMode = 1 is for 0.5x Vref
 * index = 8, caliMode = 1 is for 0.85x Vref
 * 
 * call BQ_readBattery twice and then get voltage from BQ_getVoltage
 */
void BQ_readBattery(BQ_t *bq, uint8_t index, uint8_t caliMode);
double BQ_getVoltage(BQ_t *bq, double ADC_read, UART_HandleTypeDef *uart);
uint8_t BQ_checkVoltage(BQ_t *bq, uint8_t index);

/**
 * start read battery task 
 * start is set to 1 when first run this function 
 * then call the readTemp start = 0
 * at last read resistance data from BQ_getTemp
 */
void BQ_readTemp(BQ_t *bq, uint8_t tempIndex, uint8_t start);
double BQ_getTempVoltage(BQ_t *bq, double ADC_read, UART_HandleTypeDef *uart);
double BQ_getTempResists(BQ_t *bq, double voltage);
double BQ_getTempResult(BQ_t *bq, double voltage);
uint8_t BQ_checkTemp(BQ_t *bq, double temp);

/**
 * Handling the warning message
 */
const char* BQ_getWarningMsg(BQ_t *bq);

#endif // !BQ76925_H__
