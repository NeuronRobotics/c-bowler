/*
 * LED.c
 *
 *  Created on: Nov 22, 2009
 *      Author: hephaestus
 */
#include "arch/pic32/BowlerConfig.h"
#include "Bowler/Bowler.h"

void InitLEDS(void){
	startLED();
}

void SetGreen(uint16_t Duty){
	SET_GREEN(Duty);
}

void SetRed(uint16_t Duty){
	SET_RED(Duty);
}

void SetBlue(uint16_t Duty){
	SET_BLUE(Duty);
}

void SetColor(uint16_t r,uint16_t g,uint16_t b){
	SetGreen(g);
	SetRed(r);
	SetBlue(b);
}
