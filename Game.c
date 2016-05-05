// Game.c
// Evan Longpre and Alex Wishlinski
// 30 Mar. 2016
// Game driver for Lab 8
// Lab 8
// Mahesh Srinivasan
// 30 Mar. 2016 

#include "inc/tm4c123gh6pm.h"
#include "Music.h"
#include "PLL.h"
#include "Switch.h"
#include "ST7735.h"
#include "DAC.h"
#include "LCD.h"
#include "Background.h"
#include "GFX.h"
#include "ADC.h"
#include "Switch.h"

// const uint32_t PlayerMode[3] = {0, 1, 2}; //Mode 0 is demo mode, Mode 1 is Computer vs. Human, and Mode 2 is Human vs. Human
// const int32_t Angle[9] = {20,40,60,80,90,100,120,140,160}
// const int32_t X_Diff[9] = {940,766,500,174,0,-174,-500,-766,-940}
// const int32_t Y_Diff[9] = {342,643,866,985,1000,985,866,643,342}
const int32_t Angle1[13] = {60,50,40,30,20,10,0,350,340,330,320,310,300};
const int32_t X_Diff1[13] = {500,643,766,866,940,985,1000,985,940,866,766,643,500};					//differentials based on the angle
const int32_t Y_Diff1[13] = {-866,-766,-643,-500,-342,-174,0,174,342,500,643,766,866};
const int32_t Angle2[13] = {120,130,140,150,160,170,180,190,200,210,220,230,240};
const int32_t X_Diff2[13] = {-500,-643,-766,-866,-940,-985,-1000,-985,-940,-866,-766,-643,-500};
const int32_t Y_Diff2[13] = {-866,-766,-643,-500,-342,-174,0,174,342,500,643,766,866};
uint32_t Player1;		//left; current score
uint32_t Player2;		//right; current score
uint32_t Paddle;		//which paddle was previously hit; left is 0, right is 1
uint32_t Paddle1_Center;	//current center of paddle
uint32_t Paddle2_Center;	//current center of paddle
uint32_t Ball_X;		
uint32_t Ball_Y;
uint32_t Ball_Angle;
uint32_t Ball_Direction; //the direction in which the is currently moving. 0 is moving to the right; 1 is moving to the left
uint32_t Debounce;
uint32_t Ai0;
uint32_t Ai1;


void ballTrajectory(int32_t angle, int32_t curr_x, int32_t curr_y);
void ballBounce(int32_t angle, int32_t curr_x, int32_t curr_y);
uint32_t Game_Score(uint32_t option);
void AI_paddle_control(int32_t angle, uint32_t ball_x, uint32_t ball_y, uint32_t option);
void Timer1A_Init(uint32_t val);
void Timer1A_Handler(void);
void EnableInterrupts(void);
void DisableInterrupts(void);
void DelayWait(uint32_t X);

// GFX_Paddle(uint16_t x, uint16_t y, uint16_t option);

uint16_t Game_Play(uint16_t NewGame, uint16_t option){
	if(NewGame){
		Switch_Init();
		Player1 = 0;
		Player2 = 0;
		Ball_Angle = 6;
		Ball_Direction = 0;
		Ai0 = 1;
		Ai1 = 1;
	} else { //TODO: randomize the direction/angle of ball for each round
		Ball_Angle = 6;
		Ball_Direction = 0;
	}
	Debounce = 0;
	Paddle1_Center = 160;
	Paddle2_Center = 160;
	Ball_X = 160;
	Ball_Y = 120;
	Timer1A_Init(1000000);
	ballTrajectory(Ball_Angle,Ball_X,Ball_Y);		//play game; start the ball in the middle 
	return 1;
}

uint32_t Game_Score(uint32_t option){
	//change score
	GFX_BallDel();
	if(Player1==5){			//check if someone has won the game --> first to reach 5
		LCD_Text("Player1 has won!", 160, 120, 16,LCD_WHITE,LCD_BLACK);
		Ball_Direction = 2;
	} else if(Player2==5) {
		LCD_Text("Player2 has won!", 160, 120, 16,LCD_WHITE,LCD_BLACK);
		Ball_Direction = 2;
	}
	else{			//if no one has won yet, keep playing
		char integer_string1[32];
		char integer_string2[32];
		sprintf(integer_string1,"%d",Player1);
		sprintf(integer_string2,"%d",Player2);
		char update[65] = "";
		strcat(update,integer_string1);
		strcat(update,"\t");
		strcat(update,integer_string2);

		LCD_Text("Player1\tPlayer2", 160, 100, 16,LCD_WHITE,LCD_BLACK);
		LCD_Text(update, 160, 140, 16,LCD_WHITE,LCD_BLACK);
		DelayWait(3000);
		// LCD_Fill();
		LCD_Text("Player1\tPlayer2", 160, 100, 16,LCD_BLACK,LCD_BLACK);	//LOL my ghetto erase
		LCD_Text(update, 160, 140, 16,LCD_BLACK,LCD_BLACK);
		Game_Play(0, 0);	//start a new round
	}
	return 1;
}

// left paddle is 0, right paddle is 1
void ballTrajectory(int32_t angle, int32_t curr_x, int32_t curr_y){
	//--> increment over slope
	int32_t new_y;
	int32_t new_x;

	curr_x = Ball_X;
	curr_y = Ball_Y;
	if(Ball_Direction==0){ //moving left to right
		new_y = (curr_y*500 + Y_Diff1[Ball_Angle])/500;	//calculate the new coordinates based on the differentials
		new_x = (curr_x*500 + X_Diff1[Ball_Angle])/500;
	} else { //moving right to left
		new_y = (curr_y*500 + Y_Diff2[Ball_Angle])/500;
		new_x = (curr_x*500 + X_Diff2[Ball_Angle])/500;
	}
	Ball_X = new_x;
	Ball_Y = new_y;
	GFX_Ball(new_x, new_y, Ball_Direction);
}

void ballBounce(int32_t angle, int32_t curr_x, int32_t curr_y){
	//Update paddle centers
	curr_x = Ball_X;
	curr_y = Ball_Y;

	if(Debounce){
		Debounce--;
		return;
	}
	if(curr_x <= (15 + BALLR)){ 										//if ball edge is flush with left paddle

		if((curr_y > Paddle1_Center)&&(curr_y<= (Paddle1_Center+19))){								//if the ball is above the center of the paddle

			int32_t index = ((((curr_y*1000 - Paddle1_Center*1000)/3)+500)/1000) + 6;				//find the index of the hit spot
			Ball_Angle = index;											//set the new ball angle
			Ball_Direction = 0;											//switch the direction of the ball to moving to the right		
			if(index > 12){
				//ball keeps moving; left teams loses, so right team gets a point!
				Player2++;
				Game_Score(0);
			} else {
				Paddle = 0;
				ballTrajectory(Angle1[Ball_Angle],curr_x,curr_y);			//bounce!
			}
			

		} else if ((curr_y <= Paddle1_Center)&&(curr_y >= (Paddle1_Center-19))) {													//if the ball is at or below the center of the paddle
			int32_t index = (((Paddle1_Center*1000 - curr_y*1000)/3)+500)/1000;						//find the index of the hit spot
			Ball_Angle = index;										//set the new ball angle
			Ball_Direction = 0;											//switch the direction of the ball to moving to the right		
			if(index < 0){
				//ball keeps moving; left team loses, so right team gets a point!
				Player2++;
				Game_Score(0);
			} else {
				Paddle = 0; //hits left paddle
				ballTrajectory(Angle1[Ball_Angle],curr_x,curr_y);			//bounce!
			}
			
		} else {	//if the ball is not near the paddle, keep going

			if(curr_x <= (7 + BALLR)){	//if the ball touches the left side, the ball should go off the screen
				//make the ball go off the screen
				GFX_BallDel();
				Player2++;			//Player2 scores!
				Game_Score(0); 
			} else if((curr_y <= (2 + BALLR))||(curr_y >= (237 - BALLR))){ 	//if the ball touches either horizontal edge
				int32_t index;
				index = Ball_Angle - 12;
				if(index<0){
					index = index*(-1);
				}
				Ball_Angle = index;
				ballTrajectory(Ball_Angle,curr_x,curr_y);
			} 
		}
		Debounce = 2;

	} else if(curr_x >= (304 - BALLR)){ 							//if ball edge is flush with right paddle

		if((curr_y > Paddle2_Center)&&(curr_y<= (Paddle2_Center+19))){								//if the ball is above the center of the paddle

			int32_t index = ((((curr_y*1000 - Paddle2_Center*1000)/3)+500)/1000) + 6;				//find the index of the hit spot; rounding
			Ball_Angle = index;										//set the new ball angle
			Ball_Direction = 1;											//switch the direction of the ball to moving to the left		
			if(index > 12){
				//ball keeps moving; right team loses, so left team gets a point!
				Player1++;
				Game_Score(0);
			} else {
				Paddle = 1;	//hits right paddle
				ballTrajectory(Angle2[index],curr_x,curr_y);			//bounce!
			}
		} else if ((curr_y <= Paddle2_Center)&&(curr_y >= (Paddle2_Center-19))) {													//if the ball is at or below the center of the paddle
			int32_t index = (((Paddle2_Center*1000 - curr_y*1000)/3)+500)/1000;						//find the index of the hit spot; rounding
			Ball_Angle = index;										//set the new ball angle
			Ball_Direction = 1;											//switch the direction of the ball to moving to the left	
			if(index < 0){
				//ball keeps moving; right team loses, so left team gets a point!
				Player1++;
				Game_Score(0);
			} else {
				Paddle = 1; //hits right paddle
				ballTrajectory(Angle2[Ball_Angle],curr_x,curr_y);			//bounce!
			}
		} else {	//if the ball is not near the paddle, keep going

			if(curr_x >= (312 - BALLR)){	//if the ball touches the right side, the ball should go off the screen
				//make the ball go off the screen
				GFX_BallDel();
				Player1++;			//Player1 scores!
				Game_Score(0); 
			} else if((curr_y <= (2 + BALLR))||(curr_y >= (237 - BALLR))){ 	//if the ball touches either horizontal edge
				int32_t index;
				index = Ball_Angle - 12;
				if(index<0){
					index = index*(-1);
				}
				Ball_Angle = index;
				ballTrajectory(Ball_Angle,curr_x,curr_y);
			} 
		}
		Debounce = 2;	

	} else if((curr_y <= (2 + BALLR))||(curr_y >= (237 - BALLR))){ 	//if the ball touches either horizontal edge
		int32_t index;
		index = Ball_Angle - 12;
		if(index<0){
			index = index*(-1);
		}
		Ball_Angle = index;
		ballTrajectory(Ball_Angle,curr_x,curr_y);
		Debounce = 2;

	} else if(curr_x <= (7 + BALLR)){	//if the ball touches the left side, the ball should go off the screen
		//make the ball go off the screen
		GFX_BallDel();
		Player2++;			//Player2 scores!
		Game_Score(0); 

	} else if(curr_x >= (312 - BALLR)){	//if the ball touches the right side, the ball should go off the screen
		//ake the ball go off the screen
		GFX_BallDel();
		Player1++;			//Player1 scores!
		Game_Score(0); 
	}
}

void AI_paddle_control(int32_t angle, uint32_t ball_x, uint32_t ball_y, uint32_t option){	//assuming the ball has just hit the other paddle
	uint32_t paddle_y;
	int32_t y, x, b;

	ball_y = Ball_Y;
	ball_x = Ball_X;

	//x is 0 for left, 1 for right paddle
	//y is the y value on the axis

	//GFX_Paddle(option, paddle_y, 0);
	if (option == 0){
		if(ball_y>(Paddle1_Center+6)){
			paddle_y = Paddle1_Center + 5;
			GFX_Paddle(0, paddle_y, 0);
		} else if(ball_y<(Paddle1_Center-6)){
			paddle_y = Paddle1_Center - 5;
			GFX_Paddle(0, paddle_y, 0);
		} else {
			paddle_y = Paddle1_Center;
		}
		Paddle1_Center = paddle_y;
	}else if (option == 1){
		if(ball_y>(Paddle2_Center+3)){
			paddle_y = Paddle2_Center + 5;
			GFX_Paddle(1, paddle_y, 0);
		} else if(ball_y<(Paddle2_Center-3)){
			paddle_y = Paddle2_Center - 5;
			GFX_Paddle(1, paddle_y, 0);
		} else {
			paddle_y = Paddle2_Center;
		}
		Paddle2_Center = paddle_y;
	}

}

void Timer1A_Init(uint32_t val){
  volatile uint32_t delay;

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
  TIMER1_TAILR_R = val;          // start value 
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
  uint32_t data[2];
  uint32_t button;
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer1A timeout

  ADC_In(data);
  button = Switch_GetButton();
  if(button == 1){	
  	Ai0^=0x01;
		Switch_ResetButton();
  } else if (button == 2){
  	Ai1^=0x01;
		Switch_ResetButton();
  }

  if(Ai0==1){
  	AI_paddle_control(Ball_Angle, Ball_X, Ball_Y, 0);
  } else {
  	GFX_Paddle(0,data[1],0);
  	Paddle1_Center = data[1];
  }

  if(Ai1==1){
  	AI_paddle_control(Ball_Angle, Ball_X, Ball_Y, 1);
  } else {
  	GFX_Paddle(1,data[0],0);
  	Paddle2_Center = data[0];
  }
  
  if(Ball_Direction == 2){
  	return;
  }
  ballTrajectory(Ball_Angle, Ball_X, Ball_Y);
  ballBounce(Ball_Angle, Ball_X, Ball_Y);
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
