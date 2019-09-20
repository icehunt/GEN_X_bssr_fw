#ifndef __rhTemp_H
#define __rhTemp_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "FreeRTOS.h"

#define I2C_ADDR 0x40

// I2C commands
#define CMD_R_USR1              0xE7
#define CMD_W_USR1              0xE6
#define CMD_RESET               0xFE
#define CMD_READ_T_AFTER_RH     0xE0
#define CMD_READ_RH_NO_HOLD     0xF5
#define CMD_READ_ID1            0xFA0F
#define CMD_READ_ID2            0xFCC9
#define CMD_READ_FIRMWARE       0x84B8

#define RHT_TIMEOUT             2   // ms

#define REG_USR1_PRESET         0x39

uint16_t readRH();
uint16_t readTemp();
void initRHTempSensor();

#ifdef __cplusplus
}
#endif
#endif 