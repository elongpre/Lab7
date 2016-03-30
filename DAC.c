// DAC.c
// Evan Longpre and Alex Wishlinski
// 23 Feb. 2016
// Interface with DAX
// Lab 5
// Mahesh Srinivasan
// 03 Mar. 2016 
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"

void DAC_Init(uint16_t data){
  SYSCTL_RCGCSSI_R |= 0x02;       // activate SSI1
  SYSCTL_RCGCGPIO_R |= 0x08;      // activate port D
  while((SYSCTL_PRGPIO_R&0x08) == 0){};// ready?
  GPIO_PORTD_AFSEL_R |= 0x0B;     // enable alt funct on PD0,1,3
  GPIO_PORTD_DEN_R |= 0x0B;       // configure PD0,1,3 as SSI
  GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R&0xFFFF0F00)+0x00002022;
  GPIO_PORTD_AMSEL_R = 0;         // disable analog functionality on PA
  SSI1_CR1_R = 0x00000000;        // disable SSI, master mode
  SSI1_CPSR_R = 0x02;             // 8 MHz SSIClk 
  SSI1_CR0_R &= ~(0x0000FF70);    // SCR = 0, SPH = 1, SPO = 0 Freescale
  SSI1_CR0_R |= 0x0F;             // DSS = 16-bit data
  SSI1_DR_R = data;               // load 'data' into transmit FIFO
  SSI1_CR1_R |= 0x00000002;       // enable SSI
}

void DAC_Out(uint16_t data){   
  while((SSI1_SR_R&0x00000002)==0){};// SSI Transmit FIFO Not Full
  SSI1_DR_R = data;                 // data out, no reply
}
