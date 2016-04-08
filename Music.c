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

#define C2 38223   // 65.406 Hz
#define Cs2 36077   // 69.296 Hz
#define D2 34052   // 73.416 Hz
#define Ds2 32141   // 77.782 Hz
#define E2 30337   // 82.407 Hz
#define F2 28635   // 87.307 Hz
#define Fs2 27027   // 92.499 Hz
#define G2 25511   // 97.999 Hz
#define Gs2 24079   // 103.826 Hz
#define A2 22727   // 110.000 Hz
#define As2 21452   // 116.541 Hz
#define B2 20248   // 123.471 Hz
#define C3 19111   // 130.813 Hz
#define Cs3 18039   // 138.591 Hz
#define D3 17026   // 146.832 Hz
#define Ds3 16071   // 155.563 Hz
#define E3 15169   // 164.814 Hz
#define F3 14317   // 174.614 Hz
#define Fs3 13514   // 184.997 Hz
#define G3 12755   // 195.998 Hz
#define Gs3 12039   // 207.652 Hz
#define A3 11364   // 220.000 Hz
#define As3 10726   // 233.082 Hz
#define B3 10124   // 246.942 Hz
#define C4 9556   // 261.626 Hz
#define Cs4 9019   // 277.183 Hz
#define D4 8513   // 293.665 Hz
#define Ds4 8035   // 311.127 Hz
#define E4 7584   // 329.628 Hz
#define F4 7159   // 349.228 Hz
#define Fs4 6757   // 369.994 Hz
#define G4 6378   // 391.995 Hz
#define Gs4 6020   // 415.305 Hz
#define A4 5682   // 440.000 Hz
#define As4 5363   // 466.164 Hz
#define B4 5062   // 493.883 Hz
#define C5 4778   // 523.251 Hz
#define Cs5 4510   // 554.365 Hz
#define D5 4257   // 587.330 Hz
#define Ds5 4018   // 622.254 Hz
#define E5 3792   // 659.255 Hz
#define F5 3579   // 698.456 Hz
#define Fs5 3378   // 739.989 Hz
#define G5 3189   // 783.991 Hz
#define Gs5 3010   // 830.609 Hz
#define A5 2841   // 880.000 Hz
#define As5 2681   // 932.328 Hz
#define B5 2531   // 987.767 Hz
#define C6 2389   // 1046.502 Hz
#define Cs6 2255   // 1108.731 Hz
#define D6 2128   // 1174.659 Hz
#define Ds6 2009   // 1244.508 Hz
#define E6 1896   // 1318.510 Hz
#define F6 1790   // 1396.913 Hz
#define Fs6 1689   // 1479.978 Hz
#define G6 1594   // 1567.982 Hz
#define Gs6 1505   // 1661.219 Hz
#define A6 1420   // 1760.000 Hz
#define As6 1341   // 1864.655 Hz
#define B6 1265   // 1975.533 Hz
#define REST 1

#define PF2   (*((volatile uint32_t *)0x40025010))


typedef struct StringStruct {
  uint32_t note;
  uint32_t duration;
}StringStruct;


StringStruct TrebleNotes[40] = { {E4, 2}, 
          {G4, 4}, {A4, 2}, {B4, 3}, {Cs5, 1}, {B4, 2}, 
          {A4, 4}, {Fs4, 2}, {D4, 3}, {E4, 1}, {Fs4, 2}, 
          {G4, 4}, {E4, 2}, {E4, 3}, {Ds4, 1}, {E4, 2},
          {G4, 4}, {Ds4, 2}, {B3, 4}, {E4, 2},
          {G4, 4}, {A4, 2}, {B4, 3}, {Cs5, 1}, {B4, 2},
          {A4, 4}, {Fs4, 2}, {D4, 3}, {E4, 1}, {Fs4, 2},
          {G4, 3}, {Fs4, 1}, {E4, 2}, {Ds4, 3}, {Cs4, 1}, {Ds4, 2},
          {E4, 4}, {E4, 2}, {E4, 4}, {REST, 2} };

StringStruct BassNotes[47] = { {REST, 2},
          {E3, 2}, {G3, 2}, {B3, 2}, {G3, 2}, {B3, 2}, {D4, 2},
          {D3, 2}, {Fs3, 2}, {A3, 2}, {D3, 2}, {Fs3, 2}, {A3, 2}, 
          {E3, 2}, {G3, 2}, {B3, 2}, {E3, 2}, {G3, 2}, {B3, 2},
          {B2, 2}, {Ds3, 2}, {Fs3, 2}, {B2, 4}, {REST, 2},
          {E3, 2}, {G3, 2}, {B3, 2}, {G3, 2}, {B3, 2}, {D4, 2}, 
          {D3, 2}, {Fs3, 2}, {A3, 2}, {D3, 2}, {Fs3, 2}, {A3, 2}, 
          {E3, 2}, {G3, 2}, {C4, 2}, {B2, 2}, {Ds3, 2}, {Fs3, 2}, 
          {E3, 2}, {G3, 2}, {B3, 2}, {E3, 4}, {REST, 2} };

#define WAVESIZE 256
#define WAVEDIV 8

const uint32_t SoundWave[256] = {  
  1024, 1036, 1049, 1061, 1073, 1085, 1097, 1109, 1122, 1134, 1145, 
  1157, 1169, 1181, 1192, 1204, 1215, 1227, 1238, 1249, 1260, 1270, 
  1281, 1291, 1302, 1312, 1322, 1332, 1341, 1351, 1360, 1369, 1378, 
  1386, 1394, 1403, 1411, 1418, 1426, 1433, 1440, 1446, 1453, 1459, 
  1465, 1471, 1476, 1481, 1486, 1490, 1495, 1499, 1502, 1506, 1509, 
  1512, 1514, 1517, 1519, 1520, 1522, 1523, 1523, 1524, 1524, 1524, 
  1523, 1523, 1522, 1520, 1519, 1517, 1514, 1512, 1509, 1506, 1502, 
  1499, 1495, 1490, 1486, 1481, 1476, 1471, 1465, 1459, 1453, 1446, 
  1440, 1433, 1426, 1418, 1411, 1403, 1394, 1386, 1378, 1369, 1360, 
  1351, 1341, 1332, 1322, 1312, 1302, 1291, 1281, 1270, 1260, 1249, 
  1238, 1227, 1215, 1204, 1192, 1181, 1169, 1157, 1145, 1134, 1122, 
  1109, 1097, 1085, 1073, 1061, 1049, 1036, 1024, 1012, 999, 987, 
  975, 963, 951, 939, 926, 914, 903, 891, 879, 867, 856, 844, 833, 
  821, 810, 799, 788, 778, 767, 757, 746, 736, 726, 716, 707, 697, 
  688, 679, 670, 662, 654, 645, 637, 630, 622, 615, 608, 602, 595, 
  589, 583, 577, 572, 567, 562, 558, 553, 549, 546, 542, 539, 536, 
  534, 531, 529, 528, 526, 525, 525, 524, 524, 524, 525, 525, 526, 
  528, 529, 531, 534, 536, 539, 542, 546, 549, 553, 558, 562, 567, 
  572, 577, 583, 589, 595, 602, 608, 615, 622, 630, 637, 645, 654, 
  662, 670, 679, 688, 697, 707, 716, 726, 736, 746, 757, 767, 778, 
  788, 799, 810, 821, 833, 844, 856, 867, 879, 891, 903, 914, 926, 
  939, 951, 963, 975, 987, 999, 1012
};

const uint32_t SoundWave1[128] = {  
  1024, 1049, 1073, 1097, 1122, 1145, 1169, 1192, 1215, 1238, 1260, 
  1281, 1302, 1322, 1341, 1360, 1378, 1394, 1411, 1426, 1440, 1453, 
  1465, 1476, 1486, 1495, 1502, 1509, 1514, 1519, 1522, 1523, 1524, 
  1523, 1522, 1519, 1514, 1509, 1502, 1495, 1486, 1476, 1465, 1453, 
  1440, 1426, 1411, 1394, 1378, 1360, 1341, 1322, 1302, 1281, 1260, 
  1238, 1215, 1192, 1169, 1145, 1122, 1097, 1073, 1049, 1024, 999, 
  975, 951, 926, 903, 879, 856, 833, 810, 788, 767, 746, 726, 707, 
  688, 670, 654, 637, 622, 608, 595, 583, 572, 562, 553, 546, 539, 
  534, 529, 526, 525, 524, 525, 526, 529, 534, 539, 546, 553, 562, 
  572, 583, 595, 608, 622, 637, 654, 670, 688, 707, 726, 746, 767, 
  788, 810, 833, 856, 879, 903, 926, 951, 975, 999
}; 

uint32_t TrebleLoc;
uint32_t BassLoc;
uint32_t TrebleDur;
uint32_t BassDur;
uint32_t TrebleWav;
uint32_t BassWav;
uint32_t TrebleWavLoc;
uint32_t BassWavLoc;

uint16_t SongLoc;

void EnableInterrupts(void);
void DisableInterrupts(void);
void Timer0A_Init(uint32_t val);
void Timer1A_Init(uint32_t val);
void Timer2A_Init(uint32_t val);
void Timer3A_Init(void);
void Timer3A_Halt(void);

void Music_Init(void){
    TrebleLoc = 0;
    BassLoc = 0;
}

void Music_Play(uint32_t tempo){
  DAC_Init(song[0]);
  SongLoc = 0;
  Timer0A_Init(20000);  //4000hz music  
}

void Music_Stop(void){
  Timer0A_Init(0);
}

void Music_PlayNote(uint32_t note){
  DAC_Init(SoundWave[0]);
  BassWav = SoundWave[0];
  Timer1A_Init(TrebleNotes[2].note);
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

void Timer1A_Init(uint32_t val){
  volatile uint32_t delay;
  TrebleWavLoc = 0;
  TrebleWav = SoundWave[TrebleWavLoc];
  if(val == 0){
    SYSCTL_RCGCTIMER_R &= ~0x02;
    return;
  }
  DisableInterrupts();
  // **** general initialization ****
  SYSCTL_RCGCTIMER_R |= 0x02;   // activate timer1
  delay = SYSCTL_RCGCTIMER_R;       // allow time to finish activating
  TIMER1_CTL_R &= ~TIMER_CTL_TAEN;  // disable timer1A during setup
  TIMER1_CFG_R = 0x00000000;                  // configure for 32-bit timer mode
  // **** timer1A initialization ****
                                    // configure for periodic mode
  TIMER1_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
  TIMER1_TAILR_R = val/WAVEDIV;          // start value 
  TIMER1_IMR_R |= TIMER_IMR_TATOIM; // enable timeout (rollover) interrupt
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// clear timer1A timeout flag
  TIMER1_CTL_R |= TIMER_CTL_TAEN;   // enable timer1A 16-b, periodic, interrupts
  // **** interrupt initialization ****
                                    // Timer1A=priority 4
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00008000; // top 3 bits
  NVIC_EN0_R = 1<<21;               // enable interrupt 19 in NVIC  
  EnableInterrupts();
}

void Timer1A_Handler(void){
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer1A timeout
  if(TrebleWavLoc == (WAVESIZE-1)){
    TrebleWavLoc = 0;
  } else {
    TrebleWavLoc++;
  }
  TrebleWav = SoundWave[TrebleWavLoc];
	DAC_Out(TrebleWav);
}

void Timer2A_Init(uint32_t val){
  volatile uint32_t delay;
  BassWavLoc = 0;
  BassWav = SoundWave[BassWavLoc];
  if(val == 0){
    SYSCTL_RCGCTIMER_R &= ~0x04;
    return;
  }
  DisableInterrupts();
  // **** general initialization ****
  SYSCTL_RCGCTIMER_R |= 0x04;   // activate timer2
  delay = SYSCTL_RCGCTIMER_R;       // allow time to finish activating
  TIMER2_CTL_R &= ~TIMER_CTL_TAEN;  // disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;                  // configure for 32-bit timer mode
  // **** timer2A initialization ****
                                    // configure for periodic mode
  TIMER2_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
  TIMER2_TAILR_R = val/WAVEDIV;          // start value 
  TIMER2_IMR_R |= TIMER_IMR_TATOIM; // enable timeout (rollover) interrupt
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// clear timer2A timeout flag
  TIMER2_CTL_R |= TIMER_CTL_TAEN;   // enable timer2A 16-b, periodic, interrupts
  // **** interrupt initialization ****
                                    // Timer2A=priority 4
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00008000; // top 3 bits
  NVIC_EN0_R = 1<<23;               // enable interrupt 23 in NVIC  
  EnableInterrupts();
}

void Timer2A_Handler(void){
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer2A timeout
  if(BassWavLoc == (WAVESIZE-1)){
    BassWavLoc = 0;
  } else {
    BassWavLoc++;
  }
  BassWav = SoundWave[BassWavLoc];
}

void Timer3A_Init(void){ 
  volatile uint32_t delay;
  DisableInterrupts();
  SYSCTL_RCGCTIMER_R |= 0x08;// activate timer3
  delay = SYSCTL_RCGCTIMER_R;          // allow time to finish activating
  TIMER3_CTL_R &= ~TIMER_CTL_TAEN; // disable timer3A during setup
  TIMER3_CFG_R = 0x00000000;                // configure for 32-bit timer mode
  TIMER3_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
  TIMER3_TAILR_R = 250;         // start value 
  TIMER3_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
  TIMER3_ICR_R = TIMER_ICR_TATOCINT;// clear timer3A timeout flag
  TIMER3_CTL_R |= TIMER_CTL_TAEN;  // enable timer3A 16-b, periodic, interrupts
  // **** interrupt initialization ****
                                    // Timer3A=priority 2
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|0x40000000; // top 3 bits
  NVIC_EN1_R = 1<<(35-32);           // enable IRQ 35 in NVIC  
	EnableInterrupts();
}

void Timer3A_Halt(void){
  SYSCTL_RCGCTIMER_R &= ~0x08;
}

void Timer3A_Handler(void){
  TIMER3_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer3A timeout
  DAC_Out(TrebleWav+BassWav);
}
