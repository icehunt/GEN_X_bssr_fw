/*
 * disp_renderer.c
 *
 *  Created on: Oct 5, 2019
 *      Author: jamesliu
 */

#include "disp_renderer.h"

//########     ###    ########     ###    ##     ##
//##     ##   ## ##   ##     ##   ## ##   ###   ###
//##     ##  ##   ##  ##     ##  ##   ##  #### ####
//########  ##     ## ########  ##     ## ## ### ##
//##        ######### ##   ##   ######### ##     ##
//##        ##     ## ##    ##  ##     ## ##     ##
//##        ##     ## ##     ## ##     ## ##     ##

static const float WHEEL_CIRC_M = 1.5;
static const char* BMS_ALERT_MSG_RESET = "No BMS Errors";
static const char* BMS_ALERT_MSG_BUS_OV = "BUS OVERVOLTAGE!!! @ %d.%dV";
static const char* BMS_ALERT_MSG_BUS_UV = "BUS UNDERVOLTAGE!!! @ %d.%dV";
static const char* BMS_ALERT_MSG_BUS_OC = "BUS OVERCURRENT!!! @ %d.%dA";
static const char* BMS_ALERT_MSG_CELL_OV = "CELL OVERVOLTAGE!!! #%d @ %d.%dV";
static const char* BMS_ALERT_MSG_CELL_UV = "CELL UNDERVOLTAGE!!! #%d @ %d.%dV";
static const char* BMS_ALERT_MSG_CELL_OC = "CELL OVERCURRENT!!! #%d @ %d.%dA";
static const char* BMS_ALERT_MSG_CELL_OT = "CELL OVERTEMPERATURE!!! #%d @ %d.%d°C";
static const char* BMS_ALERT_MSG_CELL_UT = "CELL UNDERTEMPERATURE!!! #%d @ %d.%d°C";
static const char* DCMB_ACC_POS_MSG = "ACC = %d/255";
LV_IMG_DECLARE(DISP_main_background);

//########  ######## ########
//##     ## ##       ##     ##
//##     ## ##       ##     ##
//########  ######   ########
//##        ##       ##
//##        ##       ##
//##        ##       ##

static void lvglTick(void* id);
static void initStyles();
static void createObjects();

// ######  ##     ##
//##    ## ##     ##
//##       ##     ##
// ######  ##     ##
//      ##  ##   ##
//##    ##   ## ##
// ######     ###

static uint32_t battV = 0;
static uint32_t battA = 0;
static uint32_t arrayV = 0;
static uint32_t arrayA = 0;
static uint32_t motV = 0;
static uint32_t motA = 0;

// ######  ######## ##    ## ##       ########  ######
//##    ##    ##     ##  ##  ##       ##       ##    ##
//##          ##      ####   ##       ##       ##
// ######     ##       ##    ##       ######    ######
//      ##    ##       ##    ##       ##             ##
//##    ##    ##       ##    ##       ##       ##    ##
// ######     ##       ##    ######## ########  ######

static lv_style_t screenStl; // Screen Background
static lv_style_t bigNumStl; // Big Number
static lv_style_t targetSpeedStl; // Target Speed
static lv_style_t bigUnitStl; // Big Number's Units
static lv_style_t scrDivStl; // Screen Divider
static lv_style_t smlTxtStl; // Small Number

static void initStyles(){
	// Screen Background
	lv_style_copy(&screenStl, &lv_style_transp_tight);
	screenStl.body.main_color = lv_color_hex3(0x000);
	screenStl.text.color = lv_color_hex3(0x0F0);
	screenStl.text.font = &Hack_8_2FA1F;
	// Big Number
	lv_style_copy(&bigNumStl, &lv_style_transp_tight);
	bigNumStl.body.main_color = lv_color_hex3(0x000);
	bigNumStl.text.color = lv_color_hex3(0x0F0);
	bigNumStl.text.font = &Hack_32_2FA1F;
	// Target Speed
	lv_style_copy(&targetSpeedStl, &bigNumStl);
	targetSpeedStl.text.font = &Hack_16_2FA1F;
	// Big Number's Units
	lv_style_copy(&bigUnitStl, &lv_style_transp_tight);
	bigUnitStl.body.main_color = lv_color_hex3(0x000);
	bigUnitStl.text.color = lv_color_hex3(0x090);
	bigUnitStl.text.font = &Hack_20_2FA1F;
	// Screen Divider
	lv_style_copy(&scrDivStl, &lv_style_transp_tight);
	scrDivStl.body.main_color = lv_color_hex3(0x0C0);
	scrDivStl.body.border.width = 1;
	scrDivStl.body.border.color = lv_color_hex3(0x0C0);
	// Small Text
	lv_style_copy(&smlTxtStl, &bigNumStl);
	smlTxtStl.text.font = &Hack_8_2FA1F;
}

// #######  ########        ## ########  ######  ########  ######
//##     ## ##     ##       ## ##       ##    ##    ##    ##    ##
//##     ## ##     ##       ## ##       ##          ##    ##
//##     ## ########        ## ######   ##          ##     ######
//##     ## ##     ## ##    ## ##       ##          ##          ##
//##     ## ##     ## ##    ## ##       ##    ##    ##    ##    ##
// #######  ########   ######  ########  ######     ##     ######

static lv_obj_t* backgroundImg;
static lv_obj_t* bigSpeedLabel;
static lv_obj_t* bigUnitLabel;
static lv_obj_t* targetSpeedLabel;
static lv_obj_t* battPwrLabel;
static lv_obj_t* arrayPwrLabel;
static lv_obj_t* bmsAlertMessageLabel;
static lv_obj_t* accPositionObj;

static void createObjects(){
	lv_obj_set_style(lv_scr_act(), &screenStl);

	backgroundImg = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(backgroundImg, &DISP_main_background);
	lv_obj_set_pos(backgroundImg, 0, 0);

	bigSpeedLabel = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(bigSpeedLabel, "120");
	lv_label_set_style(bigSpeedLabel, LV_LABEL_STYLE_MAIN, &bigNumStl);
	lv_label_set_align(bigSpeedLabel, LV_LABEL_ALIGN_LEFT);
//	lv_obj_set_pos(bigSpeedLabel, -4, 8);
	lv_obj_set_pos(bigSpeedLabel, 0, 8);

	bigUnitLabel = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(bigUnitLabel, "π•m•s⁻¹");
	lv_label_set_style(bigUnitLabel, LV_LABEL_STYLE_MAIN, &bigUnitStl);
	lv_label_set_align(bigUnitLabel, LV_LABEL_ALIGN_LEFT);
	lv_obj_set_pos(bigSpeedLabel, 54, 23);

	targetSpeedLabel = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(targetSpeedLabel, "069");
	lv_label_set_style(targetSpeedLabel, LV_LABEL_STYLE_MAIN, &targetSpeedStl);
	lv_label_set_align(targetSpeedLabel, LV_LABEL_ALIGN_LEFT);
//	lv_obj_set_pos(targetSpeedLabel, 73, -2);
	lv_obj_set_pos(targetSpeedLabel, 73, 0);

	battPwrLabel = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(battPwrLabel, "● 120.64V -03.45A -00416W IN ");
	lv_label_set_recolor(battPwrLabel, true);
	lv_label_set_style(battPwrLabel, LV_LABEL_STYLE_MAIN, &bigUnitStl);
	lv_label_set_align(battPwrLabel, LV_LABEL_ALIGN_LEFT);
	lv_obj_set_pos(battPwrLabel, 37, 41);

	arrayPwrLabel = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(arrayPwrLabel, "○ 120.32V +02.34A +00281W OUT");
	lv_label_set_recolor(arrayPwrLabel, true);
	lv_label_set_style(arrayPwrLabel, LV_LABEL_STYLE_MAIN, &bigUnitStl);
	lv_label_set_align(arrayPwrLabel, LV_LABEL_ALIGN_LEFT);
	lv_obj_set_pos(arrayPwrLabel, 37, 52);

	bmsAlertMessageLabel = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(bmsAlertMessageLabel, "CELL UNDERTEMPERATURE!!! #12 @ 69.69°C");
	lv_label_set_style(bmsAlertMessageLabel, LV_LABEL_STYLE_MAIN, &smlTxtStl);
	lv_label_set_align(bmsAlertMessageLabel, LV_LABEL_ALIGN_LEFT);
	lv_label_set_long_mode(bmsAlertMessageLabel, LV_LABEL_LONG_SROLL_CIRC);
	lv_obj_set_pos(bmsAlertMessageLabel, 133, 31);
	lv_obj_set_width(bmsAlertMessageLabel, 122);

	accPositionObj = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(accPositionObj, "ACC=NaN/255");
	lv_label_set_style(accPositionObj, LV_LABEL_STYLE_MAIN, &targetSpeedStl);
	lv_label_set_align(accPositionObj, LV_LABEL_ALIGN_LEFT);
	lv_obj_set_pos(accPositionObj, 128, 0);
}

//######## ##     ## ##    ##  ######
//##       ##     ## ###   ## ##    ##
//##       ##     ## ####  ## ##
//######   ##     ## ## ## ## ##
//##       ##     ## ##  #### ##
//##       ##     ## ##   ### ##    ##
//##        #######  ##    ##  ######

void displayTask(void* pv){
	osDelay(10);
	SSD_init();
	lv_init();
	initStyles();
	xTimerStart(xTimerCreate("", 1, pdTRUE, NULL, lvglTick),0);
	lv_disp_buf_t disp_buf;
	lv_color_t* buf = pvPortMalloc((LV_HOR_RES_MAX * LV_VER_RES_MAX) * sizeof(lv_color_t));
	lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * LV_VER_RES_MAX);
	lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);
	disp_drv.flush_cb = my_disp_flush;
	disp_drv.buffer = &disp_buf;
	disp_drv.rounder_cb = my_rounder_cb;
	lv_disp_drv_register(&disp_drv);
	createObjects();
	for(;;){
		lv_task_handler();
		osDelay(1);
	}
}

static void lvglTick(void* id){
	lv_tick_inc(1);
}

//##     ##    ###    ##    ## ########  ##       ######## ########   ######
//##     ##   ## ##   ###   ## ##     ## ##       ##       ##     ## ##    ##
//##     ##  ##   ##  ####  ## ##     ## ##       ##       ##     ## ##
//######### ##     ## ## ## ## ##     ## ##       ######   ########   ######
//##     ## ######### ##  #### ##     ## ##       ##       ##   ##         ##
//##     ## ##     ## ##   ### ##     ## ##       ##       ##    ##  ##    ##
//##     ## ##     ## ##    ## ########  ######## ######## ##     ##  ######

void disp_setMCMBPulseFreq(uint32_t hz){ // critical
	float speed = (float)hz / 16.0 * WHEEL_CIRC_M * 3.6;
	uint8_t buf[10];
	sprintf(buf, "%.0f", speed);
	lv_label_set_text(bigSpeedLabel, buf);
}

void disp_setMCMBSpeedUnit(uint8_t pi);
void disp_setMCMBDispState(uint32_t x);

void disp_setBBMBBusVoltage(uint32_t mv){ // critical
	uint8_t buf[16];
	sprintf(buf, "%d.%dV", mv/1000, mv%1000);
	lv_label_set_text(battPwrLabel, buf);
}

void disp_setBBMBBusCurrent(uint32_t ma){ // critical
	uint8_t buf[16];
	sprintf(buf, "%d.%dA", ma/1000, ma%1000);
	lv_label_set_text(battPwrLabel, buf);
}

void disp_setBBMBBmsAlertType(uint8_t type, uint32_t val){ // critical
	uint8_t buf[64];
	switch (type>>4){
	case 0b0000: // bus metric fault
		switch (type&0xf){
		case 0b0000: // reset
			sprintf(buf, BMS_ALERT_MSG_RESET);
			break;
		case 0b0001: // OV
			sprintf(buf, BMS_ALERT_MSG_BUS_OV, val/1000, val%1000);
			break;
		case 0b0010: // UV
			sprintf(buf, BMS_ALERT_MSG_BUS_UV, val/1000, val%1000);
			break;
		case 0b0011: // OC
			sprintf(buf, BMS_ALERT_MSG_BUS_OC, val/1000, val%1000);
			break;
		}
		break;
	case 0b0001: // OV
		sprintf(buf, BMS_ALERT_MSG_CELL_OV, type&0x1f, val/1000, val%1000);
		break;
	case 0b0010: // UV
		sprintf(buf, BMS_ALERT_MSG_CELL_UV, type&0x1f, val/1000, val%1000);
		break;
	case 0b0011: // OC
		sprintf(buf, BMS_ALERT_MSG_CELL_OC, type&0x1f, val/1000, val%1000);
		break;
	case 0b0100: // OT
		sprintf(buf, BMS_ALERT_MSG_CELL_OT, type&0x1f, val/1000, val%1000);
		break;
	case 0b0101: // UT
		sprintf(buf, BMS_ALERT_MSG_CELL_UT, type&0x1f, val/1000, val%1000);
		break;
	}
	lv_label_set_text(bmsAlertMessageLabel, buf);
}

void disp_setPPTMBBusCurrent(uint32_t ma);
void disp_setDCMBLeftLightState(uint32_t on); // critical
void disp_setDCMBRightLightState(uint32_t on); // critical
void disp_setDCMBStopLightState(uint32_t on); // critical
void disp_setDCMBIgnitionState(uint32_t on);
void disp_setDCMBArrayIgnitionState(uint32_t on);
void disp_setDCMBMotIgnitionState(uint32_t on);

void disp_setDCMBAccPotPosition(uint8_t x){
	uint8_t buf[16];
	sprintf(buf, DCMB_ACC_POS_MSG, x);
	lv_label_set_text(accPositionObj, buf);
}

void disp_setCHASETargetSpeed(uint32_t kph);
void disp_setCHASEAlertType(uint32_t type);
void disp_setCHASETextMessage(uint8_t* pc);
void disp_setCHASERealTime(uint64_t time);
void disp_attachMtaCallback(void(*cb)(uint8_t mta));
void disp_attachDriverAckCallback(void(*cb)(uint8_t x));
void disp_attachVfmUpCallback(void(*cb)(void));
void disp_attachVfmDownCallback(void(*cb)(void));
void disp_attachVfmResetCallback(void(*cb)(void));