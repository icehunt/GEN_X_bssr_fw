#ifndef __BSSR_GPIO_H__
#define __BSSR_GPIO_H__

#include <stdint.h>

#define MAX_GPIO_PIN_NUM 20

void BSSR_GPIO_IT_Set(uint16_t GPIO_Pin, void (*action) (uint16_t));

#endif // !__BSSR_GPIO_H__
