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
#include "GFX.h"
#include "Song.h"

#define PF1   (*((volatile uint32_t *)0x40025008))
#define PF4   (*((volatile uint32_t *)0x40025040))

const uint32_t TempoStore[3] = {60, 120, 240};
void PortF_Init(void);
void DelayWait(uint32_t X);
void Pause(void);

int main (void){
	PLL_Init(Bus80MHz);
	PortF_Init();
	LCD_Init();
	GFX_Init();
	GFX_Paddle(0, 160, 0);
	GFX_Paddle(1, 200, 0);
	GFX_Ball(160, 160, 0);
	DelayWait(1000);
	GFX_Paddle(0, 170, 0);
	GFX_Paddle(1, 190, 0);
	GFX_Ball(180, 180, 0);
	DelayWait(1000);
	GFX_Paddle(0, 20, 0);
	GFX_Paddle(1, 20, 0);
	GFX_Ball(30, 30, 0);
	while(1);
}

int mainLab5 (void){
	uint32_t button;
	uint32_t state = 0; //0 is paused, 1 is playing
	uint32_t tempo = 1;
	PLL_Init(Bus80MHz);
	PortF_Init();
	Switch_Init();
  	ST7735_InitR(INITR_REDTAB); 
	
	ST7735_OutString("Hello\n");
	while(1){
		PF1 ^= 0x02;
		button = Switch_GetButton();
		PF1 ^= 0x02;
		if(button == 0){
			PF1 ^= 0x02;
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
		PF1 ^= 0x02;
		Switch_ResetButton();
	}
}

void Pause(void){
  while(PF4==0x00){ 
    DelayWait(10); 
  }
  while(PF4==0x10){
    DelayWait(10);
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
// PF4 is input
// Make PF2 an output, enable digital I/O, ensure alt. functions off
void PortF_Init(void){ 
  SYSCTL_RCGCGPIO_R |= 0x20;        // 1) activate clock for Port F
  while((SYSCTL_PRGPIO_R&0x20)==0){}; // allow time for clock to start
                                    // 2) no need to unlock PF2, PF4
  GPIO_PORTF_PCTL_R &= ~0x000F0F00; // 3) regular GPIO
  GPIO_PORTF_AMSEL_R &= ~0x16;      // 4) disable analog function on PF2, PF4
  GPIO_PORTF_PUR_R |= 0x10;         // 5) pullup for PF4
  GPIO_PORTF_DIR_R |= 0x06;         // 5) set direction to output
  GPIO_PORTF_AFSEL_R &= ~0x16;      // 6) regular port function
  GPIO_PORTF_DEN_R |= 0x16;         // 7) enable digital port
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF00F)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;               // disable analog functionality on PF
}
