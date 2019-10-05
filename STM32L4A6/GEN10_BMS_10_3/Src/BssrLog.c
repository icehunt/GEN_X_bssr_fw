#include "BssrLog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static BSSR_LOG_HANDLE logHandle = {0b1111};

void BSSR_log(UART_HandleTypeDef *uart, BSSR_LOG_MODE mode, char * tag, char *msg, uint32_t timeout) {
    if (!((0x1 << mode) | EN_MODES | logHandle.en_modes) ) return;   // this mode is not enabled
    
}