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
void DelayWait(uint32_t X);
void Pause(void);

int main (void){
	uint32_t button, state1, state2;
	PLL_Init(Bus80MHz);
	GFX_Init();
	GFX_Paddle(0, 220, 0);
	GFX_Paddle(1, 200, 0);
	GFX_Ball(160, 160, 1);
	//Game_Play(1, 0);
	Music_Play(1);
	ADC_Init();
	Switch_Init();
	state1 = 0;
	state2 = 0;
	LCD_Text("start", 160,160,16,LCD_GREEN,LCD_RED);
	while(1){
		button = Switch_GetButton();
		if(button == 1){
			if(state1 == 1){
				LCD_Text("Ball 0", 160,160,8,LCD_GREEN,LCD_RED);
				GFX_Ball(160, 160, 0);
				state1 = 0;
			} else {
				LCD_Text("Ball 1", 160,160,8,LCD_GREEN,LCD_RED);
				GFX_Ball(160, 160, 1);
				state1 = 1;
			}
			Switch_ResetButton();
		} else if(button == 2){
			if(state2 == 1){
				LCD_Text("Music Stop", 160,160,8,LCD_GREEN,LCD_RED);
				Music_Stop();
				state2 = 0;
			} else {
				LCD_Text("Music Play", 160,160,8,LCD_GREEN,LCD_RED);
				Music_Play(1);
				state2 = 1;
			}
			Switch_ResetButton();
		}
	}
}

// Subroutine to wait X msec
// Inputs: None
// Outputs: None
// Notes: ...
void DelayWait(uint32_t X){
	uint32_t n = X;
	uint32_t volatile time;
  	while(n){
    	time = 727240*2/91;  // 10msec
    	while(time){
      	time--;
    	}
    	n--;
  	}	
}
