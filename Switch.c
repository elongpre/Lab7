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
	
void DisableInterrupts(void);
void EnableInterrupts(void);
void DelayWaitXms(void);

uint32_t LastButton;

void Switch_Init(void){
  volatile uint32_t delay; 
	DisableInterrupts();
  	SYSCTL_RCGCGPIO_R |= 0x00000008; // activate port D
    delay = SYSCTL_RCGCGPIO_R;
    GPIO_PORTD_LOCK_R = 0x4C4F434B;     // unlock
    GPIO_PORTD_CR_R |= 0x30;            // allow changes
  	GPIO_PORTD_AMSEL_R &= ~0x30;// disable analog function on PE5-4
  	GPIO_PORTD_PCTL_R &= ~0x00FF0000; // configure PE5-4 as GPIO 
  	GPIO_PORTD_DIR_R &= ~0x30;  // make PE5-4 in 
  	GPIO_PORTD_AFSEL_R &= ~0x30;// disable alt funct on PE5-4 
  	GPIO_PORTD_DEN_R |= 0x30;   // enable digital I/O on PE5-4
  	GPIO_PORTD_IS_R &= ~0x30;   // PE5-4 is edge-sensitive 
  	GPIO_PORTD_IBE_R &= ~0x30;  // PE5-4 is not both edges 
  	GPIO_PORTD_IEV_R |= 0x30;   // PE5-4 rising edge event
  	GPIO_PORTD_ICR_R = 0x30;    // clear flag5-4
  	GPIO_PORTD_IM_R |= 0x30;    // enable interrupt on PE5-4
  	                            // GPIO PortE=priority 3
  	NVIC_PRI0_R = (NVIC_PRI0_R&0x00FFFFFF)|0x60000000; // bits 5-7
  	NVIC_EN0_R = 1<<3; // enable interrupt 4 in NVIC
  	LastButton = 0;
  	EnableInterrupts();
}

void GPIOPortD_Handler(void){
  if(GPIO_PORTD_RIS_R&0x10){  // poll PE1
    GPIO_PORTD_ICR_R = 0x10;  // acknowledge flag
    LastButton = 2;
  } else if(GPIO_PORTD_RIS_R&0x20){  // poll PE2
    GPIO_PORTD_ICR_R = 0x20;  // acknowledge flag
 	  LastButton = 1;
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
	uint32_t n = 100;
	uint32_t volatile time;
  	while(n){
    	time = 727240*2/91;  // 10msec
    	while(time){
      	time--;
    	}
    	n--;
  	}	
}
