#include "BSSR_GPIO.h"

static int BSSR_GPIO_Num = 0;
static uint16_t BSSR_GPIO_Pins[MAX_GPIO_PIN_NUM];
static void (*BSSR_GPIO_Actions[MAX_GPIO_PIN_NUM]) (uint16_t);

void BSSR_GPIO_IT_Set(uint16_t GPIO_Pin, void (*action) (uint16_t)) {
    BSSR_GPIO_Pins[BSSR_GPIO_Num] = GPIO_Pin;
    BSSR_GPIO_Actions[BSSR_GPIO_Num ++] = action;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    for (int i = 0; i < BSSR_GPIO_Num; i++) {
        if (GPIO_Pin & BSSR_GPIO_Pins[i]) {
            BSSR_GPIO_Actions[i] (GPIO_Pin);
        }
    }
}