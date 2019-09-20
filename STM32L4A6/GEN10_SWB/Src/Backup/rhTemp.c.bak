//#include "rhTemp.h"
//#include "FreeRTOS.h"
////#include "serial.h"
//#include "string.h"
//#include "main.h"
//
//void initRHTempSensor() {
//    uint8_t CMD[2];
//    uint8_t buf[8];
//    uint8_t retCode = 0x31;
//
//    HAL_Delay(80);  // 80ms powerup delay
//
////    // Check chip code
////    *((uint16_t *) (&CMD)) = CMD_READ_ID2;
////    HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDR, &CMD, 2, RHT_TIMEOUT);
////    HAL_I2C_Master_Receive(&hi2c1, I2C_ADDR, &buf, 6, RHT_TIMEOUT);
////    if(buf[0] != 0x06) {
////        char msg[] = "You fucked up, the chip is not Si7006!!!\n\n";
////        send_data(msg, strlen(msg));
////        while(1);   // HALT
////    }
//
//    // Check firmware version
//    *((uint16_t *) (&CMD)) = CMD_READ_FIRMWARE;
//    buf[0] = 0x85;
//    retCode = HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDR, &CMD, 2, RHT_TIMEOUT);
//    retCode = HAL_I2C_Master_Receive(&hi2c1, I2C_ADDR, &buf, 1, RHT_TIMEOUT);
//    if(buf[0] == 0xFF) {
//        char msg[] = "Si7006 firmware v1 detected...\n";
////        send_data(msg, strlen(msg));
//    } else if (buf[0] == 0x20) {
//        char msg[] = "Si7006 firmware v2 detected...\n";
////        send_data(msg, strlen(msg));
//    } else {
//        char msg[] = "No firmware response detected...\n";
////        send_data(msg, strlen(msg));
//        while(1);
//    }
//
//    // Send RESET
//    CMD[0] = CMD_RESET;
//    HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDR, &CMD, 1, RHT_TIMEOUT);
//
//    HAL_Delay(15);  // 15 ms reset delay
//
//    // Check supply voltage and settings
//    CMD[0] = CMD_R_USR1;
//    HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDR, &CMD, 1, RHT_TIMEOUT);
//    HAL_I2C_Master_Receive(&hi2c1, I2C_ADDR, &buf, 1, RHT_TIMEOUT);
//    if (!(((buf[0]) >> 6) & 0x1)) {
//        char msg[] = "Vmin undervoltage detected...\n";
////        send_data(msg, strlen(msg));
//        while(1);
//    }
//
//    if(buf[0] != REG_USR1_PRESET) {
//        CMD[0] = CMD_W_USR1;
//        CMD[1] = REG_USR1_PRESET;
//        HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDR, &CMD, 2, RHT_TIMEOUT);
//    }
//
//    char msg[] = "Si7006 init complete!\n";
////    send_data(msg, strlen(msg));
//}
//
//uint16_t readRH() {
//    uint8_t buf[2];
//    uint8_t CMD;
//
//    CMD = CMD_READ_RH_NO_HOLD;
//    HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDR, &CMD, 1, RHT_TIMEOUT);
//    HAL_I2C_Master_Receive(&hi2c1, I2C_ADDR, &buf, 1, 1);   //  Minimal timeout for NACK
//    osDelay(23);
//    HAL_I2C_Master_Receive(&hi2c1, I2C_ADDR, &buf, 2, RHT_TIMEOUT);
//
//    return (uint16_t)buf;
//}
//
//uint16_t readTemp() {
//    uint8_t buf[2];
//    uint8_t CMD;
//
//    CMD = CMD_READ_T_AFTER_RH;
//    HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDR, &CMD, 1, RHT_TIMEOUT);
//    HAL_I2C_Master_Receive(&hi2c1, I2C_ADDR, &buf, 2, RHT_TIMEOUT);
//
//    return (uint16_t)buf;
//}
