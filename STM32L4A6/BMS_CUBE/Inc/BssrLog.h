#include "stm32l4xx_hal.h"
#include <stdint.h>

typedef enum {
    LOG_VERBOSE = 0x0,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} BSSR_LOG_MODE;

/** precompile switch */
// { EN_LOG_VERBOSE , EN_LOG_INFO , EN_LOG_WARNING , EN_LOG_ERROR }
#define EN_MODES 0b1111

/** user controlled switch */
typedef struct {
    uint8_t en_modes;
    // char [][4] shortModeName = {"V", "I", "W", "E"};
    // char [][4] modeName = {"VERBOSE", "INFO", "WARNING", "ERROR"};
} BSSR_LOG_HANDLE;

/**
 * General function to send through the uart
 * 
 * @param {uart} uart handle
 * @param {mode} mode of the log message
 * @param {tag} label before the log message
 * @param {msg} the real log message
 * @param {timeoutOrInterrupt} if this is larger than 0, it is a timeout in block mode, otherwise interrupt mode
 */
void BSSR_log(UART_HandleTypeDef *uart, BSSR_LOG_MODE mode, char * tag, char *msg, uint32_t timeoutOrInterrupt);