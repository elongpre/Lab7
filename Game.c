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


#define PF1   (*((volatile uint32_t *)0x40025008))
#define PF4   (*((volatile uint32_t *)0x40025040))

const uint32_t PlayerMode[3] = {0, 1, 2}; //Mode 0 is demo mode, Mode 1 is Computer vs. Human, and Mode 2 is Human vs. Human
// const int32_t Angle[9] = {20,40,60,80,90,100,120,140,160}
// const int32_t X_Diff[9] = {940,766,500,174,0,-174,-500,-766,-940}
// const int32_t Y_Diff[9] = {342,643,866,985,1000,985,866,643,342}
const int32_t Angle1[13] = {60,50,40,30,20,10,0,350,340,330,320,310,300};
const int32_t X_Diff1[13] = {500,643,766,866,940,985,1000,985,940,866,766,643,500};
const int32_t Y_Diff1[13] = {866,766,643,500,342,174,0,-174,-342,-500,-643,-766,-866};
const int32_t Angle2[13] = {120,130,140,150,160,170,180,190,200,210,220,230,240};
const int32_t X_Diff2[13] = {-500,-643,-766,-866,-940,-985-1000,-985,-940,-866,-766,-643,-500};
const int32_t Y_Diff2[13] = {866,766,643,500,342,174,0,-174,-342,-500,-643,-766,-866};
uint32_t Player1;		//left
uint32_t Player2;		//right
uint32_t Paddle;		//which paddle was previously hit; left is 0, right is 1

void ballTrajectory(int32_t angle, int32_t curr_x, int32_t curr_y);
void ballBounce(int32_t angle, int32_t curr_x, int32_t curr_y);
uint32_t Game_Pause(uint32_t option);
void AI_paddle_control(int32_t angle, uint32_t ball_x, uint32_t ball_y);
// GFX_Paddle(uint16_t x, uint16_t y, uint16_t option);

uint16_t Game_Play(Boolean NewGame, uint16_t option){
	if(NewGame){
		Player1 = 0;
		Player2 = 0;
	}
	ballTrajectory(90,159,119);		//play game; start the ball in the middle 
}

uint32_t Game_Pause(uint32_t option){
	//change score
	if(Player1==5){			//check if someone has won the game --> first to reach 5
		char winner[] = "Player1 has won!";
		LCD_Text(winner,uint16_t x,uint16_t y,uint16_t DimFont,LCD_White,LCD_Black);

	} else if(Player2==5) {
		char winner[] = "Player2 has won!";
		LCD_Text(winner,uint16_t x,uint16_t y,uint16_t DimFont,LCD_White,LCD_Black);
	}
	else{			//if no one has won yet, keep playing
		Game_Play(0, 0);
	}
}

// left paddle is 0, right paddle is 1
void ballTrajectory(int32_t angle, int32_t curr_x, int32_t curr_y){
	//--> increment over slope
	uint32_t i, j;
	int32_t new_y;
	int32_t new_x;

	if(Paddle==0){
		for(i=0;i<9;i++){
			if Angle1[i] == angle {
			j = i;
			}
		}
		new_y = (curr_y*1000 + Y_Diff1[j])/1000;
		new_x = (curr_x*1000 + X_Diff1[j])/1000;
	} else {
		for(i=0;i<9;i++){
			if Angle2[i] == angle {
				j = i;
			}
		}
		new_y = (curr_y*1000 + Y_Diff2[j])/1000;
		new_x = (curr_x*1000 + X_Diff2[j])/1000;
	}

	GFX_Ball(new_x, new_y, 0);
}

void ballBounce(int32_t angle, uint32_t paddle_center, int32_t curr_x, int32_t curr_y){

	if(curr_x == (9 + BALLR)){ 										//if ball edge is flush with left paddle

		if(curr_y > paddle_center){

			uint32_t index = ((((curr_y*1000 - paddle_center*1000)/3000)+500)/1000) + 6;				//find the index of the hit spot
			if(index > 12){
				//ball keeps moving; left teams loses, so right team gets a point!
			} else {
				ballTrajectory(Angle1[index],0,curr_x,curr_y);			//bounce!
			}
			

		} else {
			int32_t index = (((paddle_center*1000 - curr_y*1000)/3000)+500)/1000;						//find the index of the hit spot
			if(index < 0){
				//ball keeps moving; left team loses, so right team gets a point!
			} else {
				ballTrajectory(Angle1[index],0,curr_x,curr_y);			//bounce!
			}
			
		}


	} else if(curr_x == (309 - BALLR)){ 							//if ball edge is flush with right paddle

		if(curr_y > paddle_center){

			uint32_t index = ((((curr_y*1000 - paddle_center*1000)/3000)+500)/1000) + 6;				//find the index of the hit spot; rounding
			if(index > 12){
				//ball keeps moving; right team loses, so left team gets a point!
			} else {
				ballTrajectory(Angle2[index],1,curr_x,curr_y);			//bounce!
			}
		} else {
			int32_t index = (((paddle_center*1000 - curr_y*1000)/3000)+500)/1000;						//find the index of the hit spot; rounding
			if(index < 0){
				//ball keeps moving; right team loses, so left team gets a point!
			} else {
				ballTrajectory(Angle2[index],1,curr_x,curr_y);			//bounce!
			}
		}

	} else if((curr_y == (0 + BALLR))||(curr_y == (239 - BALLR))){ 	//if the ball touches either horizontal edge
		uint32_t i, j;
		int32_t index;
		if(Paddle==0){
			for(i=0;i<9;i++){
				if Angle1[i] == angle {
					j = i;
				}
			}
			index = j - 12;
			if(index<0){
				index = index*(-1);
			}
			angle = Angle1[index];
		} else {
			for(i=0;i<9;i++){
				if Angle2[i] == angle {
					j = i;
				}
			}
			index = j - 12;
			if(index<0){
				index = index*(-1);
			}
			angle = Angle2[index];
		}
		// angle = 180 - angle; //flip the angle
		ballTrajectory(angle,curr_x,curr_y);

	} else if(curr_x == (0 + BALLR)){	//if the ball touches either vertical side, the ball should go off the screen
		//TODO: make the ball go off the screen
		Player2++;			//Player2 scores!
		Game_Pause(); 

	} else if(curr_x == (319 - BALLR)){	//if the ball touches either vertical side, the ball should go off the screen
		//TODO: make the ball go off the screen
		Player1++;			//Player1 scores!
		Game_Pause(); 
	}
}

void AI_paddle_control(int32_t angle, uint32_t ball_x, uint32_t ball_y, uint32_t option){	//assuming the ball has just hit the other paddle
	//--> increment over slope
	uint32_t i, j, paddle_y;
	int32_t y, x, b;

	if(Paddle==0){
		for(i=0;i<9;i++){
			if Angle1[i] == angle {
				j = i;
			}
		}
		y = (curr_y*1000 + Y_Diff1[j]);
		x = (curr_x*1000 + X_Diff1[j]);
	} else{
		for(i=0;i<9;i++){
			if Angle2[i] == angle {
				j = i;
			}
		}
		y = (curr_y*1000 + Y_Diff2[j]);
		x = (curr_x*1000 + X_Diff2[j]);
	}

	if(option == 0){									//if the left paddle is controlled by AI
		b = -(y/x)*(314000); // b = -mx
		paddle_y = ((y/x)*(314000) + b)/1000;
	} else{												//if the right paddle is controlled by AI
		b = -(y/x)*(9000); // b = -mx
		paddle_y = ((y/x)*(9000) + b)/1000;
	}

	//x is 0 for left, 1 for right paddle
	//y is the y value on the axis
	GFX_Paddle(option, paddle_y, null);

}

