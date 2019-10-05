/*
 * launchCodes.c
 *
 *  Created on: Sep 20, 2019
 *      Author: jamesliu
 */

#include "../../GEN10_SPB/Inc/launchCodes.h"

#include "main.h"

void LAUNCH_armNuke(void){
	// Unlock Flash CR
	*(uint32_t*)0x40022008 = 0x45670123;
	*(uint32_t*)0x40022008 = 0xCDEF89AB;
	// Unlock Flash OPTR
	*(uint32_t*)0x4002200C = 0x08192A3B;
	*(uint32_t*)0x4002200C = 0x4C5D6E7F;
	// Set BOOT bits
	*(uint32_t*)0x40022020 &= 0xFBFFFFFF;
	*(uint32_t*)0x40022020 |= 0x08000000;
	// Lock Flash back up
	*(uint32_t*)0x40022014 |= 0x80000000;
}

void LAUNCH_disarmNuke(void){
	// Unlock Flash CR
	*(uint32_t*)0x40022008 = 0x45670123;
	*(uint32_t*)0x40022008 = 0xCDEF89AB;
	// Unlock Flash OPTR
	*(uint32_t*)0x4002200C = 0x08192A3B;
	*(uint32_t*)0x4002200C = 0x4C5D6E7F;
	// Set BOOT bits
	*(uint32_t*)0x40022020 |= 0x04000000;
	// Lock Flash back up
	*(uint32_t*)0x40022014 |= 0x80000000;
}
