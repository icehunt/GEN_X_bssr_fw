#include "main.h"
#include "cmsis_os.h"

typedef struct{
	GPIO_TypeDef *gpio;
	uint16_t pin;
} B_gpioPin_t;

static void B_gpioSet(B_gpioPin_t *pin);
static void B_gpioReset(B_gpioPin_t *pin);
static GPIO_PinState B_gpioRead(B_gpioPin_t *pin);


// PIN Defines
B_gpioPin_t GPIO_OUT_0 = {GPIOE, GPIO_PIN_3};
B_gpioPin_t GPIO_OUT_1 = {GPIOI, GPIO_PIN_9};
B_gpioPin_t GPIO_OUT_2 = {GPIOF, GPIO_PIN_2};
B_gpioPin_t GPIO_OUT_3 = {GPIOI, GPIO_PIN_12};
B_gpioPin_t GPIO_OUT_4 = {GPIOI, GPIO_PIN_13};
B_gpioPin_t GPIO_OUT_5 = {GPIOI, GPIO_PIN_14};
B_gpioPin_t GPIO_OUT_6 = {GPIOI, GPIO_PIN_15};
B_gpioPin_t GPIO_OUT_7 = {GPIOG, GPIO_PIN_0};
B_gpioPin_t GPIO_OUT_8 = {GPIOG, GPIO_PIN_1};
B_gpioPin_t GPIO_OUT_
B_gpioPin_t GPIO_EXTI_0 = {GPIOJ, GPIO_PIN_0};
B_gpioPin_t GPIO_EXTI_1 = {GPIOJ, GPIO_PIN_1};
B_gpioPin_t GPIO_EXTI_2 = {GPIOJ, GPIO_PIN_2};
B_gpioPin_t GPIO_EXTI_3 = {GPIOJ, GPIO_PIN_3};
B_gpioPin_t GPIO_EXTI_4 = {GPIOJ, GPIO_PIN_4};
B_gpioPin_t GPIO_EXTI_11 = {GPIOE, GPIO_PIN_11};
B_gpioPin_t GPIO_EXTI_12 = {GPIOE, GPIO_PIN_12};
B_gpioPin_t GPIO_EXTI_13 = {GPIOE, GPIO_PIN_13};
B_gpioPin_t GPIO_EXTI_15 = {GPIOE, GPIO_PIN_15};

