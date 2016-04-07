// Music.c
// Evan Longpre and Alex Wishlinski
// 23 Feb. 2016
// manages the music to output including changing notes
// and outputing the sine wave to the dac
// Lab 5
// Mahesh Srinivasan
// 03 Mar. 2016 
#include "DAC.h"
#include "../inc/tm4c123gh6pm.h"
#include "Music.h"
#include "Song.h"

uint16_t SongLoc;

void EnableInterrupts(void);
void DisableInterrupts(void);
void Timer0A_Init(uint32_t val);

void Music_Play(uint32_t tempo){
  DAC_Init(song[0]);
  SongLoc = 0;
  Timer0A_Init(20000);  //4000hz music  
}

void Music_Stop(void){
  Timer0A_Init(0);
}

void Timer0A_Init(uint32_t val){
  volatile uint32_t delay;
  if(val == 0){
    SYSCTL_RCGCTIMER_R &= ~0x01;
    return;
  }
  DisableInterrupts();
  // **** general initialization ****
  SYSCTL_RCGCTIMER_R |= 0x01;		// activate timer0
  delay = SYSCTL_RCGCTIMER_R;       // allow time to finish activating
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; 	// disable timer0A during setup
  TIMER0_CFG_R = 0x00000000;               	// configure for 32-bit timer mode
  // **** timer0A initialization ****
                                   	// configure for periodic mode
  TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
  TIMER0_TAILR_R = val;         	// start value 
  TIMER0_IMR_R |= TIMER_IMR_TATOIM;	// enable timeout (rollover) interrupt
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// clear timer0A timeout flag
  TIMER0_CTL_R |= TIMER_CTL_TAEN;  	// enable timer0A 16-b, periodic, interrupts
  // **** interrupt initialization ****
                                   	// Timer0A=priority 4
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; // top 3 bits
  NVIC_EN0_R = 1<<19;              	// enable interrupt 19 in NVIC	
  EnableInterrupts();
}

void Timer0A_Handler(void){
	TIMER0_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer0A timeout
  DAC_Out((song[SongLoc]+1000)*2);

  if(SongLoc == SONGLEN){
    SongLoc = 0;
  } else {
    SongLoc++;
  }
}
