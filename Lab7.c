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

const uint32_t TempoStore[3] = {60, 120, 240};
void PortF_Init(void);
void DelayWait(uint32_t X);
void Pause(void);

int main (void){
	uint32_t data[2];
	uint32_t button, state1, state2;
	PLL_Init(Bus80MHz);
	GFX_Init();
	GFX_Paddle(0, 160, 0);
	GFX_Paddle(1, 200, 0);
	GFX_Ball(160, 160, 0);
	ADC_Init();
	Switch_Init();
	state1 = 0;
	state2 = 0;
	while(1){
		ADC_In(data);
		button = Switch_GetButton();
		if(button == 1){
			if(state1 == 1){
				LCD_Circle(160, 160, 5, 1, LCD_GREEN);
				state1 = 0;
			} else {
				LCD_Circle(160, 160, 5, 1, LCD_BLUE);
				state1 = 1;
			}
			Switch_ResetButton();
		} else if(button == 2){
			if(state2 == 1){
				LCD_Circle(160, 160, 5, 1, LCD_RED);
				state2 = 0;
			} else {
				LCD_Circle(160, 160, 5, 1, LCD_YELLOW);
				state2 = 1;
			}
			Switch_ResetButton();
		}
	}
}

int mainLab5 (void){
	uint32_t button;
	uint32_t state = 0; //0 is paused, 1 is playing
	uint32_t tempo = 1;
	PLL_Init(Bus80MHz);
	Switch_Init();
  	ST7735_InitR(INITR_REDTAB); 
	
	ST7735_OutString("Hello\n");
	while(1){
		button = Switch_GetButton();
		if(button == 0){
			continue;
		}else if(button == 1){
			if(state == 0){
				Music_Play(TempoStore[tempo]);
				state = 1;
				ST7735_OutString("Play Music\n");
			} else {
				Music_Stop();
				state = 0;
				ST7735_OutString("Pause Music\n");
			}
		} else if(button == 2){
			Music_Stop();
			Music_Init();
			Music_Play(TempoStore[tempo]);
			state = 1;
			ST7735_OutString("Rewind and Play Music\n");
		} else if(button == 3){
			if(tempo == 2){
				tempo = 0;
			} else {
				tempo++;
			}
			Music_Stop();
			Music_Play(TempoStore[tempo]);
			ST7735_OutString("Change Tempo\n");
		} else if(button == 4){
			Music_Stop();
			state = 0;
			Music_PlayNote(5682);
			ST7735_OutString("Play Note: A\n");
		}
		Switch_ResetButton();
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
