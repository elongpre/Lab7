// Switch.c
// Evan Longpre and Alex Wishlinski
// 23 Feb. 2016
// Collects inputs from the switches
// Lab 5
// Mahesh Srinivasan
// 03 Mar. 2016 
#include <stdint.h>
#include "Switch.h"
#include "inc/tm4c123gh6pm.h"

#define SW_ONE      (*((volatile uint32_t *)0x40024004))  //PE1
#define SW_TWO      (*((volatile uint32_t *)0x40024008))  //PE2
#define SW_THREE    (*((volatile uint32_t *)0x40024010))  //PE3
#define SW_FOUR     (*((volatile uint32_t *)0x40024020))  //PE4
//#define SW_FOUR			(*((volatile uint32_t *)0x40025040))	//PF4
	
void DisableInterrupts(void);
void EnableInterrupts(void);
void DelayWaitXms(void);

uint32_t LastButton;

void Switch_Init(void){
	DisableInterrupts();
  	SYSCTL_RCGCGPIO_R |= 0x00000010; // activate port E
  	GPIO_PORTE_AMSEL_R &= ~0x1E;// disable analog function on PE5-4
  	GPIO_PORTE_PCTL_R &= ~0x00FF0000; // configure PE5-4 as GPIO 
  	GPIO_PORTE_DIR_R &= ~0x1E;  // make PE5-4 in 
  	GPIO_PORTE_AFSEL_R &= ~0x1E;// disable alt funct on PE5-4 
  	GPIO_PORTE_DEN_R |= 0x1E;   // enable digital I/O on PE5-4
  	GPIO_PORTE_IS_R &= ~0x1E;   // PE5-4 is edge-sensitive 
  	GPIO_PORTE_IBE_R &= ~0x1E;  // PE5-4 is not both edges 
  	GPIO_PORTE_IEV_R |= 0x1E;   // PE5-4 rising edge event
  	GPIO_PORTE_ICR_R = 0x1E;    // clear flag5-4
  	GPIO_PORTE_IM_R |= 0x1E;    // enable interrupt on PE5-4
  	                            // GPIO PortE=priority 3
  	NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00)|0x00000060; // bits 5-7
  	NVIC_EN0_R = 1<<4; // enable interrupt 4 in NVIC
  	LastButton = 0;
  	EnableInterrupts();
}

void GPIOPortE_Handler(void){
  if(GPIO_PORTE_RIS_R&0x02){  // poll PE1
    GPIO_PORTE_ICR_R = 0x02;  // acknowledge flag
    LastButton = 1;
  } else if(GPIO_PORTE_RIS_R&0x04){  // poll PE2
    GPIO_PORTE_ICR_R = 0x04;  // acknowledge flag
 	LastButton = 2;
  } else if(GPIO_PORTE_RIS_R&0x08){  // poll PE3
    GPIO_PORTE_ICR_R = 0x08;  // acknowledge flag
  	LastButton = 3;
  } else if(GPIO_PORTE_RIS_R&0x10){  // poll PE4
    GPIO_PORTE_ICR_R = 0x10;  // acknowledge flag
  	LastButton = 4;
  }
}

uint32_t Switch_GetButton(void){
	return LastButton;
}

void Switch_ResetButton(void){
	DelayWaitXms();
	LastButton = 0;
}

// Subroutine to wait X msec
// Inputs: None
// Outputs: None
// Notes: ...
void DelayWaitXms(void){
	uint32_t n = 50;
	uint32_t volatile time;
  	while(n){
    	time = 727240*2/91;  // 10msec
    	while(time){
      	time--;
    	}
    	n--;
  	}	
}
