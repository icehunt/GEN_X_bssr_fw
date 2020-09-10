#include <bgpio_writepin.h>
#include "main.h"
// ########  ##     ##
// ##     ## ##     ##
// ##     ## ##     ##
// ########  ##     ##
// ##         ##   ##
// ##          ## ##
// ##           ###

// ########  ######## ########
// ##     ## ##       ##     ##
// ##     ## ##       ##     ##
// ########  ######   ########
// ##        ##       ##
// ##        ##       ##
// ##        ##       ##

void BHAL_GPIO_WritePin(uint16_t pinNumber,
						GPIO_PinState pinState);

// ######## ##     ## ##    ##  ######
// ##       ##     ## ###   ## ##    ##
// ##       ##     ## ####  ## ##
// ######   ##     ## ## ## ## ##
// ##       ##     ## ##  #### ##
// ##       ##     ## ##   ### ##    ##
// ##        #######  ##    ##  ######

void BHAL_GPIO_WritePin(uint16_t pinNumber, GPIO_PinState pinState){
	HAL_GPIO_WritePin(peripherals[pinNumber-1], pins[pinNumber-1], pinState);
}
