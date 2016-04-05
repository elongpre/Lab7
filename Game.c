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
const int32_t Angle[13] = {30,40,50,60,70,80,90,100,110,120,130,140,150}
const int32_t X_Diff[13] = {866,766,643,500,342,174,0,-174,-342,-500,-643,-766,866}
const int32_t Y_Diff[13] = {500,698,876,1047,1222,1296,1571,1745,1920,2094,2269,2443,2518}

void ballTrajectory(int32_t angle, int32_t curr_x, int32_t curr_y);
void ballBounce(int32_t angle, int32_t curr_x, int32_t curr_y);
uint32_t Game_Pause(uint32_t option);
// GFX_Paddle(uint16_t x, uint16_t y, uint16_t option);

uint16_t Game_Play(uint16_t option){
	ballTrajectory(90,159,119);		//play game; start the ball in the middle 
}

uint32_t Game_Pause(uint32_t option){
	//change score
	if(){			//check if someone has won the game --> first to reach 7?

	} else{			//if no one has won yet, keep playing
		Game_Play();
	}

	
}

void ballTrajectory(int32_t angle, int32_t curr_x, int32_t curr_y){
	//--> increment over slope
	uint32_t i, j;
	for(i=0;i<9;i++){
		if Angle[i] == angle {
			j = i;
		}
	}
	int32_t new_y = (curr_y*1000 + Y_Diff[j])/1000;
	int32_t new_x = (curr_x*1000 + X_Diff[j])/1000;
	GFX_Ball(new_x, new_y, null);

}

void ballBounce(int32_t angle, uint32_t padddle_center, int32_t curr_x, int32_t curr_y){

	if(curr_x == (9 + BALLR)){ 										//if ball edge is flush with left paddle

		if(curr_y > padddle_center){

			uint32_t index = ((((curr_y*1000 - padddle_center*1000)/3000)+500)/1000) + 6;				//find the index of the hit spot
			if(index > 12){
				//ball keeps moving; left teams loses, so right team gets a point!
			} else {
				ballTrajectory(Angle[index],curr_x,curr_y);			//bounce!
			}
			

		} else {
			int32_t index = (((padddle_center*1000 - curr_y*1000)/3000)+500)/1000;						//find the index of the hit spot
			if(index < 0){
				//ball keeps moving; left team loses, so right team gets a point!
			} else {
				ballTrajectory(Angle[index],curr_x,curr_y);			//bounce!
			}
			
		}


	} else if(curr_x == (309 - BALLR)){ 							//if ball edge is flush with right paddle

		if(curr_y > padddle_center){

			uint32_t index = ((((curr_y*1000 - padddle_center*1000)/3000)+500)/1000) + 6;				//find the index of the hit spot; rounding
			if(index > 12){
				//ball keeps moving; right team loses, so left team gets a point!
			} else {
				ballTrajectory(Angle[index],curr_x,curr_y);			//bounce!
			}
			

		} else {
			int32_t index = (((padddle_center*1000 - curr_y*1000)/3000)+500)/1000;						//find the index of the hit spot; rounding
			if(index < 0){
				//ball keeps moving; right team loses, so left team gets a point!
			} else {
				ballTrajectory(Angle[index],curr_x,curr_y);			//bounce!
			}
			
		}

	} else if((curr_y == (0 + BALLR))||(curr_y == (239 - BALLR))){ 	//if the ball touches either horizontal edge

		angle = 180 - angle; //flip the angle
		ballTrajectory(angle,curr_x,curr_y);

	} else if((curr_x == (0 + BALLR))||(curr_x == (319 - BALLR))){	//if the ball touches either vertical side, the ball should go off the screen
		//TODO: make the ball go off the screen
		Game_Pause(); 

	}
}
