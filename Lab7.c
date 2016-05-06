// Lab5.c
// Evan Longpre and Alex Wishlinski
// 23 Feb. 2016
// Main driver for Lab5
// Lab 5
// Mahesh Srinivasan
// 03 Mar. 2016 
#include "inc/tm4c123gh6pm.h"
#include "Music.h"
#include "PLL.h"
#include "Switch.h"
#include "ST7735.h"
#include "DAC.h"
#include "LCD.h"
#include "ADC.h"
#include "GFX.h"
#include "Song.h"
#include "Game.h"

const uint32_t TempoStore[3] = {60, 120, 240};
void PortF_Init(void);
void Pause(void);

int main (void){
	PLL_Init(Bus80MHz);
	GFX_Init();
	GFX_Paddle(0, 220, 0);
	GFX_Paddle(1, 200, 0);
	GFX_Ball(160, 160, 1);
	Game_Play(1, 0);
	ADC_Init();
	Music_Play();
	while(1){
	}
}

