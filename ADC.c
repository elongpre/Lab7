// ADCSWTrigger.c
// Runs on LM4F120/TM4C123
// Provide functions that sample ADC inputs PE4, PE5 using SS2
// to be triggered by software and trigger two conversions,
// wait for them to finish, and return the two results.
// Daniel Valvano
// May 2, 2015

#include "ADC.h"
#include "../inc/tm4c123gh6pm.h"


//use PE0, 5 on channel 3 and 8
void ADC_Init(void){ 
  volatile uint32_t delay;                         
  SYSCTL_RCGCADC_R |= 0x00000001; // 1) activate ADC0
  SYSCTL_RCGCGPIO_R |= 0x00000010; // 1) activate clock for Port E
  delay = SYSCTL_RCGCGPIO_R;      // 2) allow time for clock to stabilize

  GPIO_PORTE_DIR_R &= ~0x21;      // 3) make PE4 PE5 input
  GPIO_PORTE_AFSEL_R |= 0x21;     // 4) enable alternate function on PE4 PE5
  GPIO_PORTE_DEN_R &= ~0x21;      // 5) disable digital I/O on PE4 PE5
                                  // 5a) configure PE4 as ?? (skip this line because PCTL is for digital only)
  GPIO_PORTE_PCTL_R &= ~0x00F0000F;
  GPIO_PORTE_AMSEL_R |= 0x21;     // 6) enable analog functionality on PE4 PE5

  ADC0_PC_R &= ~0xF;              // 8) clear max sample rate field
  ADC0_PC_R |= 0x1;               //    configure for 125K samples/sec
  ADC0_SSPRI_R = 0x3210;          // 9) Sequencer 3 is lowest priority
  ADC0_ACTSS_R &= ~0x0004;        // 10) disable sample sequencer 2
  ADC0_EMUX_R &= ~0x0F00;         // 11) seq2 is software trigger
  ADC0_SSMUX2_R = 0x0038;         // 12) set channels for SS2
  ADC0_SSCTL2_R = 0x0060;         // 13) no TS0 D0 IE0 END0 TS1 D1, yes IE1 END1
  ADC0_IM_R &= ~0x0004;           // 14) disable SS2 interrupts
  ADC0_ACTSS_R |= 0x0004;         // 15) enable sample sequencer 2
}


void ADC_In(uint32_t data[2]){ 
  ADC0_PSSI_R = 0x0004;            // 1) initiate SS2
  while((ADC0_RIS_R&0x04)==0){};   // 2) wait for conversion done
  data[1] = ADC0_SSFIFO2_R&0xFFF;  // 3A) read first result
  data[0] = ADC0_SSFIFO2_R&0xFFF;  // 3B) read second result
  data[0] = 4095-data[0];
  data[1] = data[1]/(4096/240);    // Convert ADC values to scale 
  data[0] = data[0]/(4096/240);    // to the LCD Screen

  ADC0_ISC_R = 0x0004;             // 4) acknowledge completion
}
